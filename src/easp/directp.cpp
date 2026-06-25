// directp.cpp
//
// Direct importer control applet for EasPanel
//
//   (C) Copyright 2018-2026 Fred Gleason <fredg@paravelsystems.com>
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
#include <signal.h>
#include <syslog.h>
#include <sys/types.h>

#include <QApplication>
#include <QKeyEvent>
#include <QMessageBox>
#include <QProcess>

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
  QString config_file=CONFIG_DIRECTP_FILE_NAME;
  bool dump_config=false;
  bool no_publish_point_cleanup=false;
  bool automatic=false;
  bool paused=false;

  CmdSwitch *cmd=new CmdSwitch("directp",VERSION,DIRECTP_USAGE);
  for(int i=0;i<cmd->keys();i++) {
    if(cmd->key(i)=="-d") {
      openlog("directp",LOG_PERROR,LOG_USER);
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--automatic") {
      automatic=true;
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--config-file") {
      config_file=cmd->value(i);
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--dump-config") {
      dump_config=true;
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--no-publish-point-cleanup") {
      no_publish_point_cleanup=true;
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--no-raise") {
      d_raise_on_alert=false;
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--paused") {
      paused=true;
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
  if(automatic) {
    if(paused) {
      // Conflict error!
      QMessageBox::critical(this,"DirectPanel - "+tr("Error"),
			    tr("The")+" \"--automatic\" "+tr("and")+
			    " \"--paused\" "+
			    tr("switches are mutually exclusive."));
      exit(1);
    }
    else {
      // Using auto mode
    }
  }
  else {
    if(paused) {
      // Using paused mode
    }
    else {
      // Implies automatic mode
    }
  }

  //
  // Main Configuration
  //
  main_config=new Config();
  main_config->load(config_file);
  if(dump_config) {
    printf("%s",main_config->dump().toUtf8().constData());
    exit(0);
  }

  QList<pid_t> pids=GetPids();
  if(pids.size()>0) {
    //
    // Another instance is already running, so tell it to use the
    // specified mode.
    //
    if(automatic) {
      kill(pids.first(),SIGUSR1);
    }
    if(paused) {
      kill(pids.first(),SIGUSR2);
    }
    exit(0);
  }

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

  //
  // File Widget
  //
  main_direct_file_widget=
    new DirectFileWidget(main_rml_socket,main_config,this);
  connect(main_direct_file_widget,SIGNAL(quitRequested()),this,SLOT(quit()));
  connect(main_direct_file_widget,SIGNAL(raiseRequested()),
	  this,SLOT(bringToTop()));
  setMinimumSize(sizeHint());
  setMaximumHeight(sizeHint().height());

  //
  // POSIX Signal Monitoring
  //
  d_sig_watcher=new SigWatcher(this);
  connect(d_sig_watcher,SIGNAL(receivedSignal(int)),
	  this,SLOT(receivedSignalData(int)));
  d_sig_watcher->addWatchedSignal(SIGUSR1);
  d_sig_watcher->addWatchedSignal(SIGUSR2);
  d_sig_watcher->addWatchedSignal(SIGTERM);
  d_sig_watcher->addWatchedSignal(SIGINT);
  
  //
  // Publish Point Cleanup
  //
  if(!no_publish_point_cleanup) {
    main_direct_file_widget->cleanPaths();
  }

  //
  // Set Operating Mode
  //
  if(paused) {
    main_direct_file_widget->setPausedMode();
  }
  if(automatic) {
    main_direct_file_widget->setAutomaticMode();
  }
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


void MainWidget::receivedSignalData(int signum)
{
  switch(signum) {
  case SIGUSR1:
    main_direct_file_widget->setAutomaticMode();
    break;

  case SIGUSR2:
    main_direct_file_widget->setPausedMode();
    break;

  case SIGINT:
  case SIGTERM:
    main_direct_file_widget->setPausedMode();
    main_direct_file_widget->quitRequested();
    break;
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


QList<pid_t> MainWidget::GetPids()
{
  QStringList args;
  pid_t pid;
  bool ok=false;
  QList<pid_t> ret;

  args.push_back("-C");
  args.push_back(qApp->applicationName());
  args.push_back("-o");
  args.push_back("pid=");

  QProcess *proc=new QProcess(this);
  proc->start("ps",args);
  proc->waitForFinished();
  if(proc->exitStatus()!=QProcess::NormalExit) {
    QMessageBox::critical(this,"DirectPanel - "+tr("Error"),
			  tr("ps(1) process crashed!"));
    exit(1);
  }
  if(proc->exitCode()!=0) {
    QMessageBox::critical(this,"DirectPanel - "+tr("Error"),
			  tr("ps(1) returned exit code")+
			  QString::asprintf("%d.\n\n",proc->exitCode())+
			  QString::fromUtf8(proc->readAllStandardError()));
    exit(1);
  }
  QStringList f0=QString::fromUtf8(proc->readAllStandardOutput()).
    split("\n",QString::SkipEmptyParts);
  for(int i=0;i<f0.size();i++) {
    pid=f0.at(i).toInt(&ok);
    if(ok&&(pid>0)&&(pid!=qApp->applicationPid())) {
      ret.push_back(pid);
    }
  }

  return ret;
}


int main(int argc,char *argv[])
{
  QApplication a(argc,argv);

  MainWidget *w=new MainWidget();
  w->show();

  return a.exec();
}
