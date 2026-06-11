// directfile.cpp
//
// Direct File Importer Subsystem
//
//   (C) Copyright 2026 Fred Gleason <fredg@paravelsystems.com>
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

#include <syslog.h>

#include "easp.h"

DirectFile::DirectFile(QUdpSocket *rml_sock,Config *c,QObject *parent)
  : QObject(parent)
{
  d_config=c;

  //
  // File Detectors
  //
  for(int i=0;i<d_config->directFileQuantity();i++) {
    d_detectors.push_back(new FileDetector(i,rml_sock,d_config,this));
    connect(d_detectors.back(),SIGNAL(fileAdded(int,const QString &)),
	    this,SLOT(detectorFileAddedData(int,const QString &)));
    connect(d_detectors.back(),SIGNAL(fileRemoved(int,const QString &)),
	    this,SLOT(detectorFileRemovedData(int,const QString &)));

    if(d_detectors.back()->setPath(d_config->directFilePath(i))) {
      syslog(LOG_DEBUG,"DirectFile%d set path for %s",
	     1+i,d_config->directFilePath(i).toUtf8().constData());
    }
    else {
      syslog(LOG_ERR,"DirectFile%d failed to set path %s",
	     1+i,d_config->directFilePath(i).toUtf8().constData());
      exit(1);
    }
  }
}


void DirectFile::playedCart(unsigned cartnum)
{
  for(int i=0;i<d_detectors.size();i++) {
    d_detectors.at(i)->playedCart(cartnum);
  }
}


void DirectFile::detectorFileAddedData(int id,const QString &pathname)
{
  syslog(LOG_DEBUG,"DirectFile%d added file: %s",
	 1+id,pathname.toUtf8().constData());
}


void DirectFile::detectorFileRemovedData(int id,const QString &pathname)
{
  syslog(LOG_DEBUG,"DirectFile%d deleted file: %s",
	 1+id,pathname.toUtf8().constData());
}
