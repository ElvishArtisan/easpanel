// directp.cpp
//
// Direct importer control applet for EasPanel
//
//   (C) Copyright 2018-2020 Fred Gleason <fredg@paravelsystems.com>
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
#include <stdlib.h>
#include <syslog.h>

#include <QApplication>
#include <QKeyEvent>
#include <QMessageBox>

#include "directp.h"
#include "cmdswitch.h"

//
// Icons
//
#include "../../icons/easpanel-22x22.xpm"

MainWidget::MainWidget(QWidget *parent)
  : QWidget(parent,Qt::CustomizeWindowHint|Qt::WindowMinimizeButtonHint|Qt::WindowMaximizeButtonHint)
{
  d_raise_on_alert=true;

  CmdSwitch *cmd=new CmdSwitch("directp",VERSION,DIRECTP_USAGE);
  for(int i=0;i<cmd->keys();i++) {
    if(cmd->key(i)=="-d") {
      openlog("directp",LOG_PERROR,LOG_USER);
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--no-raise") {
      d_raise_on_alert=false;
      cmd->setProcessed(i,true);
    }
    if(!cmd->processed(i)) {
      QMessageBox::critical(this,"DirectPanel - "+tr("Unknown Option"),
			    tr("Unknown command-line option:")+
			    " \""+cmd->key(i)+"\".");
      exit(1);
    }
  }
  delete cmd;

  //
  // Main Configuration
  //
  main_config=new Config();
  main_config->load();

  //
  // RML Socket
  //
  main_rml_socket=new QUdpSocket(this);
  if(!main_rml_socket->bind(QHostAddress::AnyIPv4,main_config->pathsRlmReceivePort())) {
    QMessageBox::critical(this,"EAS Panel",
			  tr("Unable to bind UDP port")+
			  QString::
			  asprintf(" %u!",main_config->pathsRlmReceivePort()));
    exit(1);
  }
  connect(main_rml_socket,SIGNAL(readyRead()),this,SLOT(rlmReadyReadData()));

  //
  // Set Window Titlebar
  //
  setWindowTitle(QString("DirectPanel - v")+VERSION);
  setWindowIcon(QPixmap(easpanel_22x22_xpm));

  main_direct_file_widget=
    new DirectFileWidget(main_rml_socket,main_config,this);
  connect(main_direct_file_widget,SIGNAL(quitRequested()),this,SLOT(quit()));
  connect(main_direct_file_widget,SIGNAL(raiseRequested()),
	  this,SLOT(bringToTop()));
  setMinimumSize(sizeHint());
  setMaximumHeight(sizeHint().height());
}


QSize MainWidget::sizeHint() const
{
  return main_direct_file_widget->sizeHint();
}


void MainWidget::rlmReadyReadData()
{
  char data[1501];
  int n;
  bool ok=false;
  QHostAddress orig_addr;

  while((n=main_rml_socket->readDatagram(data,1500,&orig_addr))>0) {
    data[n]=0;
    QStringList f0=QString(data).split("\t");
    if(f0.size()==4) {
      unsigned cartnum=f0.at(1).toUInt(&ok);
      if(ok) {
	main_direct_file_widget->playedCart(cartnum);
      }
    }
  }
}


void MainWidget::bringToTop()
{
  if(d_raise_on_alert) {
    setWindowState(Qt::WindowActive|Qt::WindowMaximized);
    raise();
    activateWindow();
  }
}


void MainWidget::quit()
{
  exit(0);
}


void MainWidget::resizeEvent(QResizeEvent *e)
{
  main_direct_file_widget->setGeometry(0,0,size().width(),size().height());
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


int main(int argc,char *argv[])
{
  QApplication a(argc,argv);

  MainWidget *w=new MainWidget();
  w->show();

  return a.exec();
}
