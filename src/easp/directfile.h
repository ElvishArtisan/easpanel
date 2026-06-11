// directfile.h
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

#ifndef DIRECTFILE_H
#define DIRECTFILE_H

#include <QObject>
#include <QUdpSocket>

#include "config.h"
#include "filedetector.h"

class DirectFile : public QObject
{
  Q_OBJECT;
 public:
  DirectFile(QUdpSocket *rml_sock,Config *c,QObject *parent=0);
  void playedCart(unsigned cartnum);

 private slots:
  void detectorFileAddedData(int id,const QString &pathname);
  void detectorFileRemovedData(int id,const QString &pathname);

 private:
  QList<FileDetector *> d_detectors;
  Config *d_config;
};


#endif  // DIRECTFILE_H
