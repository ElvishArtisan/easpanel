// easbelt.cpp
//
// File conveyor utility for easpanel
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

#include <unistd.h>

#include <QCoreApplication>

#include "easbelt.h"

MainObject::MainObject(QObject *parent)
  : QObject(parent)
{
  bool dump_config=false;
  d_one_shot=false;
  d_task_timer=NULL;

  CmdSwitch *cmd=new CmdSwitch("easp",VERSION,EASBELT_USAGE);
  for(int i=0;i<cmd->keys();i++) {
    if(cmd->key(i)=="-d") {
      openlog("easp",LOG_PERROR,LOG_USER);
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--dump-config") {
      dump_config=true;
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--one-shot") {
      d_one_shot=true;
      cmd->setProcessed(i,true);
    }
    if(!cmd->processed(i)) {
      fprintf(stderr,"easbelt: unknown command-line option \"%s\"",
	      cmd->key(i).toUtf8().constData());
      exit(1);
    }
  }
  delete cmd;

  //
  // Main Configuration
  //
  d_config=new Config();
  if(!d_config->load()) {
    fprintf(stderr,"easbelt: unable to load configuration\n");
    exit(1);
  }
  if(dump_config) {
    printf("%s\n",d_config->dump().toUtf8().constData());
    exit(0);
  }

  //
  // Process Handler
  //
  d_task_mapper=new QSignalMapper(this);
  connect(d_task_mapper,SIGNAL(mapped(QObject *)),
	  this,SLOT(taskFinished(QObject *)));
  for(int i=0;i<d_config->easbeltQuantity();i++) {
    if(d_config->easbeltDestinationQuantity(i)>0) {
      d_task_timer=new QTimer(this);
      d_task_timer->setSingleShot(true);
      connect(d_task_timer,SIGNAL(timeout()),this,SLOT(taskData()));
      d_task_timer->start(1000);
      break;
    }
  }
  if(d_task_timer==NULL) {
    syslog(LOG_DEBUG,"nothing to do, idling...");
  }
}


void MainObject::taskData()
{
  for(int i=0;i<d_config->easbeltQuantity();i++) {
    QStringList f0=
      d_config->easbeltSourceFiles(i).split("/",QString::KeepEmptyParts);
    QString filter=f0.last();
    f0.removeLast();
    QDir dir(f0.join("/"));
    dir.setFilter(QDir::Files|QDir::Readable);
    dir.setSorting(QDir::Time);
    dir.setNameFilters(QStringList(filter));
    QStringList files=dir.entryList();
    if(files.size()>0) {
      for(int j=0;j<files.size();j++) {
	QString src_pathname=dir.path()+"/"+files.at(j);
	for(int k=0;k<d_config->easbeltDestinationQuantity(i);k++) {
	  QStringList args;
	  args.push_back("-o");
	  args.push_back("IdentitiesOnly=yes");
	  args.push_back("-o");
	  args.push_back("BatchMode=yes");
	  args.push_back("-i");
	  args.push_back(d_config->easbeltDestinationSftpIdentity(i,k));
	  args.push_back(src_pathname.trimmed());
	  args.push_back(d_config->easbeltDestinationSftpUsername(i,k)+
			 "@"+
			 d_config->easbeltDestinationHostname(i,k)+
			 ":"+
			 d_config->easbeltDestinationPath(i,k));
	  QProcess *proc=new QProcess(this);
	  d_task_mapper->setMapping(proc,proc);
	  connect(proc,SIGNAL(finished(int,QProcess::ExitStatus)),
		  d_task_mapper,SLOT(map()));
	  d_task_map[proc]=
	    new BeltTask(proc,i,k,src_pathname,EASBELT_SCP_BINARY);
	  proc->start(EASBELT_SCP_BINARY,args);
	  syslog(LOG_DEBUG,"transfer %s started: \"scp %s\"",
		 d_task_map.value(proc)->logId().toUtf8().constData(),
		 args.join(" ").toUtf8().constData());
	}
      }
    }
  }
  if(!d_one_shot) {
    d_task_timer->start(EASBELT_TASK_INTERVAL);
  }
}


void MainObject::taskFinished(QObject *obj)
{
  QProcess *proc=(QProcess *)obj;
  BeltTask *task=d_task_map.value(proc);
  int src_id=-1;
  QString src_filename;
  if(task==NULL) {
    syslog(LOG_WARNING,"no task record found for transfer process \"%s %s\"",
	   proc->program().toUtf8().constData(),
	   proc->arguments().join(" ").toUtf8().constData());
  }
  else {
    src_id=task->sourceId();
    src_filename=task->sourceFilename();
    if(proc->exitStatus()!=QProcess::NormalExit) {
      syslog(LOG_WARNING,"transfer %s crashed: \"%s\"",
	     task->logId().toUtf8().constData(),
	     d_task_map.value(proc)->command().toUtf8().constData());
    }
    else {
      if(proc->exitCode()!=0) {
	syslog(LOG_WARNING,"transfer %s returned non-zero exit code: \"%s\"",
	       task->logId().toUtf8().constData(),
	       proc->readAllStandardError().constData());
      }
      else {
	syslog(LOG_DEBUG,"transfer %s completed",
	       task->logId().toUtf8().constData());
      }
    }
  }
  if(task!=NULL) {
    delete task;
    d_task_map.remove(proc);
    proc->deleteLater();
  }
  CleanSource(src_id,src_filename);
}


bool MainObject::CleanSource(int src_id,const QString &src_filename) const
{
  for(QMap<QProcess *,BeltTask *>::const_iterator it=d_task_map.begin();
      it!=d_task_map.end();it++) {
    if(it.value()->sourceFilename()==src_filename) {
      return false;
    }
  }
  unlink(src_filename.toUtf8());
  syslog(LOG_DEBUG,"deleted source file %d:%s",
	 1+src_id,
	 src_filename.toUtf8().constData());
  return true;
}


int main(int argc,char *argv[])
{
  QCoreApplication a(argc,argv);

  new MainObject();

  return a.exec();
}
