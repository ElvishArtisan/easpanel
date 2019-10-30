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
#include <QKeyEvent>
#include <QMessageBox>
#include <QStringList>

#include "cmdswitch.h"
#include "easp.h"

//
// Icons
//
#include "../../icons/easpanel-22x22.xpm"

MainWidget::MainWidget(QWidget *parent)
  : QWidget(parent,Qt::CustomizeWindowHint|Qt::WindowMinimizeButtonHint|Qt::WindowMaximizeButtonHint)
{
  main_auto=false;
  main_selected_alert_id=-1;
  main_next_is_voicetrack=false;
  setFocusPolicy(Qt::StrongFocus);

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
  QFont mode_font(font().family(),2*font().pointSize(),QFont::Normal);

  //
  // Set Window Titlebar
  //
  setWindowTitle(QString("EASPanel - v")+VERSION);
  setWindowIcon(QPixmap(easpanel_22x22_xpm));

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
  main_text_text=new AlertTextEdit(this);
  connect(main_text_text,SIGNAL(quitRequested()),this,SLOT(quit()));

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
  main_auto_button=new ModeButton(tr("LiveAssist"),this);
  main_auto_button->setFont(mode_font);
  main_auto_button->setStyleSheet("background-color: #FFFF00");
  connect(main_auto_button,SIGNAL(clicked()),this,SLOT(autoData()));
  connect(main_auto_button,SIGNAL(quitRequested()),this,SLOT(quit()));

  main_livesend_button=new QPushButton(tr("Send to Log"),this);
  main_livesend_button->setFont(bold_font);
  connect(main_livesend_button,SIGNAL(clicked()),this,SLOT(liveSendData()));
  main_livesend_button->setDisabled(true);

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


void MainWidget::autoData()
{
  int ready_id=-1;

  if(main_auto) {
    main_auto_button->setText(tr("LiveAssist"));
    main_auto_button->setStyleSheet("background-color: #FFFF00");
    main_auto=false;
  }
  else {
    main_auto_button->setText(tr("Automatic"));
    main_auto_button->setStyleSheet("background-color: #00FF00");
    main_auto=true;
    for(int i=0;i<EASP_ALERT_QUAN;i++) {
      AlertButton *button=main_alert_buttons[i];
      if((button->status()==AlertButton::Ready)&&(ready_id<0)) {
	ready_id=i;
      }
      if(button->status()==AlertButton::Sent) {
	ready_id=-1;
	break;
      }
    }
    if(ready_id>=0) {
      autoSendData(ready_id);
    }
  }
}


void MainWidget::liveSendData()
{
  int offset=0;
  AlertButton *button=main_alert_buttons[main_selected_alert_id];
  Alert *alert=button->alert();
  QString header_transition="PLAY";

  if(alert!=NULL) {
    if(main_next_is_voicetrack) {
      offset=1;
    }
    if(main_config->rivendellLiveassistFriendlyIntroCart()!=0) {
      header_transition="STOP";
    }

    //
    // Load from the bottom up
    //
    if(main_config->liveassistOutroCart(alert->easType())!=0) {
      SendRml(QString().sprintf("PX %d %d %d PLAY!",    // Outro Cart
				1,
				main_config->
				liveassistOutroCart(alert->easType()),
				offset));
      button->setLastCart(main_config->liveassistOutroCart(alert->easType()));
    }
    else {
      button->setLastCart(alert->eomCart());
    }

    SendRml(QString().sprintf("PX %d %d %d STOP!",    // EOM
			      1,
			      alert->eomCart(),offset));
    button->setLastCart(alert->eomCart());

    if(alert->messageCart()!=0) {
      SendRml(QString().sprintf("PX %d %d %d PLAY!",    // Attention Signal
				1,
				alert->attentionCart(),offset));
    }

    SendRml(QString().sprintf("PX %d %d %d ",    // Header
			      1,
			      alert->headerCart(),offset)+
	    header_transition+"!");

    if(main_config->rivendellLiveassistFriendlyIntroCart()!=0) {
      SendRml(QString().sprintf("PX %d %d %d STOP!",    // Intro Cart
				1,
				main_config->
				liveassistIntroCart(alert->easType()),
				offset));
    }

    main_alert_buttons[main_selected_alert_id]->setStatus(AlertButton::Sent);
  }
}


void MainWidget::autoSendData(int id)
{
  int offset=0;
  AlertButton *button=main_alert_buttons[id];
  Alert *alert=button->alert();
  if(alert!=NULL) {
    if(main_next_is_voicetrack) {
      offset=1;
    }

    //
    // Load from the bottom up
    //
    if(main_config->outroCart(alert->easType())!=0) {
      SendRml(QString().sprintf("PX %d %d %d PLAY!",    // Outro Cart
				1,
				main_config->outroCart(alert->easType()),
				offset));
      button->setLastCart(main_config->outroCart(alert->easType()));
    }
    else {
      button->setLastCart(alert->eomCart());
    }

    SendRml(QString().sprintf("PX %d %d %d PLAY!",    // EOM
			      1,
			      alert->eomCart(),offset));

    if(alert->messageCart()!=0) {
      SendRml(QString().sprintf("PX %d %d %d PLAY!",    // Message
				1,
				alert->messageCart(),offset));
      SendRml(QString().sprintf("PX %d %d %d PLAY!",    // Message
				1,
				alert->attentionCart(),offset));
    }

    SendRml(QString().sprintf("PX %d %d %d PLAY!",    // Header
			      1,
			      alert->headerCart(),offset));

    if(main_config->rivendellFriendlyIntroCart()!=0) {
      SendRml(QString().sprintf("PX %d %d %d PLAY!",    // Intro Cart
				1,
				main_config->introCart(alert->easType()),
				offset));
    }
    if(offset==0) {    // Start Immediately
      SendRml("PN 1!");
    }
    else {
      SendRml("PB 3!");
    }
    main_alert_buttons[id]->setStatus(AlertButton::Sent);
  }
}


void MainWidget::alertScanData()
{
  QDir dir(main_config->pathsEasDataDirectory());
  if(!dir.exists()) {
    QMessageBox::critical(this,"EasPanel",
			  tr("Unable to read EAS messages directory!"));
    exit(1);
  }
  dir.setNameFilters(main_config->pathsEasMessageExtensionFilter());
  QStringList files=dir.entryList(QDir::Files|QDir::Readable);
  for(int i=0;i<files.size();i++) {
    if(!main_alerts.contains(files.at(i))) {
      Alert *alert=new Alert();
      main_alerts[files.at(i)]=alert;
    }
  }
  for(QMap<QString,Alert *>::const_iterator it=main_alerts.begin();
      it!=main_alerts.end();it++) {
    it.value()->touch();
    if(it.value()->touches()==4) {
      if(it.value()->load(main_config->pathsEasDataDirectory()+"/"+it.key())) {
	if(ProcessNewAlert(it.value())) {
	  if(main_auto) {
	    SendNextAlert();
	  }
	}
      }
    }
  }

  main_alert_scan_timer->start(1000);
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
    if(alert->attentionCart()!=0) {
      main_config->removeCart(alert->attentionCart(),&err_msg);
    }
    QString filename=alert->filename();
    RetireAlertFile(filename);
    if(!alert->headerAudio().isEmpty()) {
      RetireAlertFile(alert->headerAudio());
    }
    if(!alert->attentionAudio().isEmpty()) {
      RetireAlertFile(alert->attentionAudio());
    }
    if(!alert->messageAudio().isEmpty()) {
      RetireAlertFile(alert->messageAudio());
    }
    if(!alert->eomAudio().isEmpty()) {
      RetireAlertFile(alert->eomAudio());
    }
    delete alert;
    main_alerts.remove(filename);
    main_alert_buttons[id]->setAlert(NULL);
    if(id==main_selected_alert_id) {
      main_title_label->setText(tr("Alert")+QString().sprintf(" %d",id+1));
      main_datetime_label->clear();
      main_text_text->clear();
    }
    main_livesend_button->setDisabled(true);

    CompactButtons();
  }
}


void MainWidget::rlmReadyReadData()
{
  char data[1501];
  int n;
  bool ok=false;

  while((n=main_rml_socket->readDatagram(data,1500))>0) {
    data[n]=0;
    QStringList f0=QString(data).split("\t");
    if(f0.size()==4) {
      unsigned cartnum=f0.at(1).toUInt(&ok);
      if(ok) {
	ProcessNowPlaying(cartnum);
      }
      main_next_is_voicetrack=
	main_config->rivendellVoicetrackGroups().contains(f0.at(2));
    }
  }
}


void MainWidget::quit()
{
  QString err_msg;
  Alert *alert=NULL;

  //
  // Clean up Rivendell carts
  //
  for(int i=0;i<EASP_ALERT_QUAN;i++) {
    if((alert=main_alert_buttons[i]->alert())!=NULL) {
      if(alert->headerCart()!=0) {
	main_config->removeCart(alert->headerCart(),&err_msg);
      }
      if(alert->eomCart()!=0) {
	main_config->removeCart(alert->eomCart(),&err_msg);
      }
      if(alert->messageCart()!=0) {
	main_config->removeCart(alert->messageCart(),&err_msg);
      }
      if(alert->attentionCart()!=0) {
	main_config->removeCart(alert->attentionCart(),&err_msg);
      }
    }
  }

  exit(0);
}


void MainWidget::resizeEvent(QResizeEvent *e)
{
  int w=size().width();
  int h=size().height();

  main_title_label->setGeometry(10,5,w-20,20);
  main_datetime_label->setGeometry(10,32,w-20,20);

  main_auto_button->setGeometry(w-210,5,200,50);

  main_text_text->setGeometry(10,59,2*w/3,h-129);

  main_livesend_button->setGeometry(40,h-60,200,50);
  //  main_cannedsend_button->setGeometry(180,h-60,120,50);

  for(int i=0;i<EASP_ALERT_QUAN;i++) {
    main_alert_buttons[i]->setGeometry(2*w/3+20,
				       59+i*((h-129)/EASP_ALERT_QUAN+13),
				       w/3-30,
				       5+(h-129)/EASP_ALERT_QUAN);
  }
}


void MainWidget::keyPressEvent(QKeyEvent *e)
{
  if((e->key()==Qt::Key_X)&&((e->modifiers()&Qt::AltModifier)!=0)) {
    quit();
  }
  QWidget::keyPressEvent(e);
}


void MainWidget::closeEvent(QCloseEvent *e)
{
  quit();
}


void MainWidget::SendNextAlert()
{
  Alert *alert=NULL;

  if(!AlertLoaded()) {
    for(int i=0;i<EASP_ALERT_QUAN;i++) {
      if(main_alert_buttons[i]->status()==AlertButton::Ready) {
	if((alert=main_alert_buttons[i]->alert())!=NULL) {
	  autoSendData(i);
	  return;
	}
      }
    }
  }
}


void MainWidget::ProcessNowPlaying(unsigned cartnum)
{
  for(int i=0;i<EASP_ALERT_QUAN;i++) {
    AlertButton *button=main_alert_buttons[i];
    if(button->alert()!=NULL) {
      if(button->lastCartPlayed()) {
	alertClosedData(i);  // Dismiss Completed Alert
	if(main_auto) {
	  SendNextAlert();  // Send Next Alert
	}
      }
      else {
	if((cartnum!=0)&&(button->lastCart()==cartnum)) {
	  button->setLastCart(0);
	  button->setLastCartPlayed(true);
	}
      }
    }
  }
}


bool MainWidget::ProcessNewAlert(Alert *alert)
{
  unsigned cartnum=0;
  QString err_msg="";

  for(int i=0;i<EASP_ALERT_QUAN;i++) {
    if(main_alert_buttons[i]->alert()==NULL) {
      AlertButton *button=main_alert_buttons[i];
      button->setAlert(alert);
      button->setStatus(AlertButton::Ready);
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
	  if((cartnum=main_config->
	      importCart("*** EAS ATTENTION SIGNAL *** ["+alert->title()+"]",
			 "/usr/share/easpanel/attention.wav",&err_msg))==0) {
	    button->setStatus(AlertButton::Error);
	    button->addStatusText("Missing attention signal audio ["+err_msg+"]. ");
	  }
	  else {
	    alert->setAttentionCart(cartnum);
	  }
	}
      }
      if(i==main_selected_alert_id) {
	DisplayAlertButton(button);
      }
      BringToTop();
      return true;
    }
  }
  return false;
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
  main_livesend_button->setDisabled(main_auto||(alert==NULL)||
				    (button->status()==AlertButton::Error));
  /*
  main_livesend_button->setDisabled(main_auto||(alert==NULL)||
				    (alert->messageCart()==0)||
				    (button->status()==AlertButton::Error));
  main_cannedsend_button->setDisabled(main_auto||(alert==NULL)||
				      (button->status()==AlertButton::Error));
  */
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


void MainWidget::BringToTop()
{
  setWindowState(Qt::WindowActive|Qt::WindowMaximized);
  raise();
  activateWindow();
}


bool MainWidget::AlertLoaded() const
{
  bool ret=false;

  for(int i=0;i<EASP_ALERT_QUAN;i++) {
    //    ret=ret||(main_alert_buttons[i]->lastCart()!=0);
    ret=(ret||(main_alert_buttons[i]->lastCart()!=0))&&
      (!main_alert_buttons[i]->lastCartPlayed());
  }

  return ret;
}


void MainWidget::RetireAlertFile(const QString &filename) const
{
  if(main_config->pathsEasBackupDirectory().isEmpty()) {
    unlink((main_config->pathsEasDataDirectory()+"/"+filename).toUtf8());
  }
  else {
    rename((main_config->pathsEasDataDirectory()+"/"+filename).toUtf8(),
	   (main_config->pathsEasBackupDirectory()+"/"+filename).toUtf8());
  }
}


int main(int argc,char *argv[])
{
  QApplication::setStyle("Plastique");
  QApplication a(argc,argv);

  MainWidget *w=new MainWidget();
  w->show();

  return a.exec();
}
