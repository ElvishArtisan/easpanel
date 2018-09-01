
// easp.cpp
//
// Control panel applet for easpanel
//
//   (C) Copyright 2018 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License version 2 as
//   published by the Free Software Foundation.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public
//   License along with this program; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#include <stdio.h>
#include <unistd.h>

#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <QStringList>

#include "cmdswitch.h"
#include "easp.h"

MainWidget::MainWidget(QWidget *parent)
  : QWidget(parent)
{
  main_selected_alert_id=-1;

  new CmdSwitch("easp","\n");

  //
  // Main Configuration
  //
  main_config=new Config();
  if(!main_config->load()) {
    QMessageBox::critical(this,"EAS Panel",
			  tr("Unable to open configuration file!"));
    exit(1);
  }

  //
  // RML Socket
  //
  main_rml_socket=new QUdpSocket(this);
  if(!main_rml_socket->bind(main_config->pathsRlmReceivePort())) {
    QMessageBox::critical(this,"EAS Panel",
			  tr("Unable to bind UDP port")+
			  QString().
			  sprintf(" %u!",main_config->pathsRlmReceivePort()));
    exit(1);
  }
  connect(main_rml_socket,SIGNAL(readyRead()),this,SLOT(rlmReadyReadData()));

  //
  // Fonts
  //
  QFont bold_font(font().family(),font().pointSize(),QFont::Bold);

  setWindowTitle(QString("EASPanel - v")+VERSION);

  //
  // Title
  //
  main_title_label=new QLabel(this);
  main_title_label->setFont(bold_font);
  main_title_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  //
  // Datetime
  //
  main_datetime_label=new QLabel(this);
  main_datetime_label->setFont(bold_font);
  main_datetime_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  //
  // Text Editor
  //
  main_text_text=new QTextEdit(this);

  //
  // Alert Buttons
  //
  for(int i=0;i<EASP_ALERT_QUAN;i++) {
    main_alert_buttons[i]=new AlertButton(i,this);
    connect(main_alert_buttons[i],SIGNAL(clicked(int)),
	    this,SLOT(alertSelectedData(int)));
    connect(main_alert_buttons[i],SIGNAL(closeClicked(int)),
	    this,SLOT(alertClosedData(int)));
  }

  //
  // Buttons
  //
  main_livesend_button=
    new QPushButton(tr("Send to Log")+"\n("+tr("LIVE")+")",this);
  main_livesend_button->setFont(bold_font);
  connect(main_livesend_button,SIGNAL(clicked()),this,SLOT(liveSendData()));
  main_livesend_button->setDisabled(true);

  main_cannedsend_button=
    new QPushButton(tr("Send to Log")+"\n("+tr("CANNED")+")",this);
  main_cannedsend_button->setFont(bold_font);
  connect(main_cannedsend_button,SIGNAL(clicked()),this,SLOT(cannedSendData()));
  main_cannedsend_button->setDisabled(true);
  /*
  main_start_button=new QPushButton(tr("EAS")+"\n"+tr("Start"),this);
  main_start_button->setStyleSheet("background-color: #008800");
  main_start_button->setFont(bold_font);
  connect(main_start_button,SIGNAL(clicked()),this,SLOT(startData()));
  main_start_button->setDisabled(true);

  main_end_button=new QPushButton(tr("EAS")+"\n"+tr("End"),this);
  main_end_button->setStyleSheet("background-color: #880000");
  main_end_button->setFont(bold_font);
  connect(main_end_button,SIGNAL(clicked()),this,SLOT(endData()));
  main_end_button->setDisabled(true);
  */
  //
  // Alerts
  //
  main_alert_scan_timer=new QTimer(this);
  main_alert_scan_timer->setSingleShot(true);
  connect(main_alert_scan_timer,SIGNAL(timeout()),this,SLOT(alertScanData()));
  alertScanData();
}


QSize MainWidget::sizeHint() const
{
  return QSize(1020,786);
}


void MainWidget::liveSendData()
{
  Alert *alert=main_alert_buttons[main_selected_alert_id]->alert();
  if(alert!=NULL) {
    //
    // Load from the bottom up
    //
    SendRml(QString().sprintf("PX %d %d STOP!",    // EOM
			      main_config->rivendellLogMachine(),
			      alert->eomCart()));

    SendRml(QString().sprintf("PX %d %d PLAY!",    // Attention Signal
			      main_config->rivendellLogMachine(),
			      main_config->rivendellAlertToneCart()));

    SendRml(QString().sprintf("PX %d %d PLAY!",    // Header
			      main_config->rivendellLogMachine(),
			      alert->headerCart()));
  }
}


void MainWidget::cannedSendData()
{
  Alert *alert=main_alert_buttons[main_selected_alert_id]->alert();
  if(alert!=NULL) {
    //
    // Load from the bottom up
    //
    SendRml(QString().sprintf("PX %d %d PLAY!",    // EOM
			      main_config->rivendellLogMachine(),
			      alert->eomCart()));

    if(alert->messageCart()!=0) {
      SendRml(QString().sprintf("PX %d %d PLAY!",    // Message
				main_config->rivendellLogMachine(),
				alert->messageCart()));

      SendRml(QString().sprintf("PX %d %d PLAY!",    // Attention Signal
				main_config->rivendellLogMachine(),
				main_config->rivendellAlertToneCart()));
    }

    SendRml(QString().sprintf("PX %d %d PLAY!",    // Header
			      main_config->rivendellLogMachine(),
			      alert->headerCart()));
  }
}


void MainWidget::alertScanData()
{
  QDir dir(main_config->pathsEasMessages());
  if(!dir.exists()) {
    QMessageBox::critical(this,"EasPanel",
			  tr("Unable to read EAS messages directory!"));
    exit(1);
  }

  QStringList files=dir.entryList(QDir::Files|QDir::Readable);
  for(int i=0;i<files.size();i++) {
    if(!main_alerts.contains(files.at(i))) {
      Alert *alert=new Alert();
      if(alert->load(main_config->pathsEasMessages()+"/"+files.at(i))) {
	main_alerts[files.at(i)]=alert;
	ProcessNewAlert(alert);
      }
    }
  }

  main_alert_scan_timer->start(5000);
}


void MainWidget::alertSelectedData(int id)
{
  if(id!=main_selected_alert_id) {
    //
    // Save previous alert
    //
    if(main_selected_alert_id>=0) {
      if(main_alert_buttons[main_selected_alert_id]->alert()!=NULL) {
	main_alert_buttons[main_selected_alert_id]->alert()->
	  setText(main_text_text->toPlainText());
      }
      main_alert_buttons[main_selected_alert_id]->setSelected(false);
    }
  
    //
    // Display new alert
    //
    DisplayAlertButton(main_alert_buttons[id]);
  }
}


void MainWidget::alertClosedData(int id)
{
  QString err_msg;
  Alert *alert=main_alert_buttons[id]->alert();

  if(alert!=NULL) {
    if(alert->headerCart()!=0) {
      main_config->removeCart(alert->headerCart(),&err_msg);
    }
    if(alert->eomCart()!=0) {
      main_config->removeCart(alert->eomCart(),&err_msg);
    }
    if(alert->messageCart()!=0) {
      main_config->removeCart(alert->messageCart(),&err_msg);
    }
    QString filename=alert->filename();
    unlink((main_config->pathsEasMessages()+"/"+filename).toUtf8());
    delete alert;
    main_alerts.remove(filename);
    main_alert_buttons[id]->setAlert(NULL);
    if(id==main_selected_alert_id) {
      main_title_label->setText(tr("Alert")+QString().sprintf(" %d",id+1));
      main_datetime_label->clear();
      main_text_text->clear();
    }
    main_livesend_button->setDisabled(true);
    main_cannedsend_button->setDisabled(true);

    CompactButtons();
  }
}


void MainWidget::startData()
{
}


void MainWidget::endData()
{
}


void MainWidget::rlmReadyReadData()
{
  char data[1501];
  int n;
  bool ok=false;

  while((n=main_rml_socket->readDatagram(data,1500))>0) {
    data[n]=0;
    QStringList f0=QString(data).split("\t");
    if(f0.size()==2) {
      unsigned cartnum=f0.at(1).toUInt(&ok);
      if(ok||(f0.at(0)==main_config->rivendellAlertAudioGroup())) {
	ProcessNowNext(cartnum);
      }
    }
  }
}


void MainWidget::resizeEvent(QResizeEvent *e)
{
  int w=size().width();
  int h=size().height();

  main_title_label->setGeometry(10,5,w-20,20);
  main_datetime_label->setGeometry(10,32,w-20,20);
  main_text_text->setGeometry(10,59,2*w/3,h-129);

  main_livesend_button->setGeometry(40,h-60,120,50);
  main_cannedsend_button->setGeometry(180,h-60,120,50);
  //  main_start_button->setGeometry(40,h-60,80,50);
  //  main_end_button->setGeometry(2*w/3-100,h-60,80,50);

  for(int i=0;i<EASP_ALERT_QUAN;i++) {
    main_alert_buttons[i]->setGeometry(2*w/3+20,
				       59+i*((h-129)/EASP_ALERT_QUAN+13),
				       w/3-30,
				       5+(h-129)/EASP_ALERT_QUAN);
  }
}


void MainWidget::ProcessNowNext(unsigned cartnum)
{
  for(int i=0;i<EASP_ALERT_QUAN;i++) {
    AlertButton *button=main_alert_buttons[i];
    if(button->alert()!=NULL) {
      if(button->eomPlayed()) {
	alertClosedData(i);
      }
      else {
	if(button->alert()->eomCart()==cartnum) {
	  button->setEomPlayed(true);
	}
      }
    }
  }
}


void MainWidget::ProcessNewAlert(Alert *alert)
{
  unsigned cartnum=0;
  QString err_msg="";

  for(int i=0;i<EASP_ALERT_QUAN;i++) {
    if(main_alert_buttons[i]->alert()==NULL) {
      AlertButton *button=main_alert_buttons[i];
      button->setAlert(alert);
      if((cartnum=main_config->
	  importCart("*** EAS HEADER *** ["+alert->title()+"]",
		     alert->headerAudio(),&err_msg))==0) {
	button->setStatus(AlertButton::Error);
	button->addStatusText("Missing header audio ["+err_msg+"]. ");
      }
      else {
	alert->setHeaderCart(cartnum);
      }
      if((cartnum=main_config->
	  importCart("*** EAS END OF MESSAGE *** ["+alert->title()+"]",
		     alert->eomAudio(),&err_msg))==0) {
	button->setStatus(AlertButton::Error);
	button->addStatusText("Missing eom audio ["+err_msg+"]. ");
      }
      else {
	alert->setEomCart(cartnum);
      }
      if(!alert->messageAudio().isEmpty()) {
	if((cartnum=main_config->
	    importCart("*** EAS MESSAGE *** ["+alert->title()+"]",
		       alert->messageAudio(),&err_msg))==0) {
	  button->setStatus(AlertButton::Error);
	  button->addStatusText("Missing message audio ["+err_msg+"]. ");
	}
	else {
	  alert->setMessageCart(cartnum);
	}
      }
      if(i==main_selected_alert_id) {
	DisplayAlertButton(button);
      }
      return;
    }
  }
}


void MainWidget::DisplayAlertButton(AlertButton *button)
{
  Alert *alert=button->alert();
  if(alert==NULL) {
    main_title_label->
      setText(tr("Alert")+QString().sprintf(" %d",button->id()+1));
    main_datetime_label->clear();
    main_text_text->clear();
  }
  else {
    main_title_label->
      setText(tr("Alert")+QString().sprintf(" %d - ",button->id()+1)+
	      alert->title());
    main_datetime_label->
      setText(tr("Issued:")+" "+
	      alert->issuedDateTime().toString("MMMM d @ h:mm ap")+"  "+
	      tr("Expires:")+" "+
	      alert->expiresDateTime().toString("MMMM d @ h:mm ap"));
    main_text_text->setText(alert->text());
  }
  button->setSelected(true);
  main_selected_alert_id=button->id();
  main_livesend_button->setDisabled((alert==NULL)||(alert->messageCart()==0));
  main_cannedsend_button->setDisabled(alert==NULL);
  //  main_start_button->setDisabled(alert==NULL);
  //  main_end_button->setDisabled(alert==NULL);
}


void MainWidget::CompactButtons()
{
  int empty_slot=-1;

  for(int i=0;i<EASP_ALERT_QUAN;i++) {
    AlertButton *button=main_alert_buttons[i];
    if((button->alert()==NULL)&&(empty_slot<0)) {
      empty_slot=i;
    }
    else {
      if(empty_slot>=0) {
	if(button->alert()!=NULL) {
	  main_alert_buttons[empty_slot]->copyFrom(button);
	  main_selected_alert_id=-1;
	  if(button->selected()) {
	    alertSelectedData(empty_slot);
	  }
	}
	button->clear();
	return;
      }
    }
  }
}


void MainWidget::SendRml(const QString &rml)
{
  main_rml_socket->
    writeDatagram(rml.toUtf8(),main_config->rivendellHostAddress(),5859);
}


int main(int argc,char *argv[])
{
  QApplication::setStyle("Plastique");
  QApplication a(argc,argv);

  MainWidget *w=new MainWidget();
  w->show();

  return a.exec();
}
