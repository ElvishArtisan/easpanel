// test-alert-manifest.cpp
//
// Test the 'Alert' class parser routines
//
//   (C) Copyright 2020 Fred Gleason <fredg@paravelsystems.com>
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

#include <QCoreApplication>

#include "cmdswitch.h"
#include "test_alert_manifest.h"

MainObject::MainObject(QWidget *parent)
{
  QString filename;

  CmdSwitch *cmd=new CmdSwitch("test_alert_manifest",TEST_ALERT_MANIFEST_USAGE);
  for(unsigned i=0;i<cmd->keys();i++) {
    if(cmd->key(i)=="--filename") {
      filename=cmd->value(i);
      cmd->setProcessed(i,true);
    }
    if(!cmd->processed(i)) {
      fprintf(stderr,"test_alert_manifest: unknown option \"%s\"\n",
	      cmd->key(i).toUtf8().constData());
      exit(1);
    }
  }
  delete cmd;

  //
  // Sanity Checks
  //
  if(filename.isEmpty()) {
    fprintf(stderr,"test_alert_manifest: you must supply a --filename\n");
    exit(1);
  }

  Alert *alert=new Alert();
  if(!alert->load(filename)) {
    printf("load failed\n");
    exit(0);
  }
  printf("%s\n",alert->dump().toUtf8().constData());
  delete alert;

  exit(0);
}


int main(int argc,char *argv[])
{
  QCoreApplication a(argc,argv);

  new MainObject();

  return a.exec();
}
