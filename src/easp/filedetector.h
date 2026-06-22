// filedetector.h
//
// Scan a directory for files.
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

#ifndef FILEDETECTOR_H
#define FILEDETECTOR_H

#include <QDir>
#include <QFileInfo>
#include <QMap>
#include <QObject>
#include <QTimer>
#include <QUdpSocket>

#include "config.h"

class FileInfo
{
 public:
  FileInfo(QFileInfo info);
  QFileInfo fileInfo() const;
  unsigned cartNumber() const;
  void setCartNumber(unsigned cartnum);
  int stage() const;
  bool touched() const;
  bool touch(const QFileInfo &info);
  void reset();
  bool isDeletable() const;
  void makeDeletable();

 private:
  QFileInfo d_file_info;
  unsigned d_cart_number;
  int d_stage;
  bool d_touched;
  bool d_deletable;
};


class FileDetector : public QObject
{
  Q_OBJECT;
 public:
  FileDetector(int id,QUdpSocket *rml_sock,Config *c,QObject *parent=0);
  ~FileDetector();
  int id() const;
  bool eventActive() const;
  bool isScanning() const;
  QString path() const;
  bool setPath(const QString &path);
  void playedCart(unsigned cartnum);

 signals:
  void fileAdded(int id,const QString &pathname);
  void fileRemoved(int id,const QString &pathname);
  void eventStarted(int id,FileInfo *info);
  void eventStopped(int id);
  void scanningStarted(int id);
  void scanningStopped(int id);

 public slots:
  void startScanning();
  void stopScanning();

 private slots:
  void scanData();

 private:
  void ProcessFile(FileInfo *info);
  bool CartIsLoaded(unsigned cartnum) const;
  int d_id;
  QUdpSocket *d_rml_socket;
  QDir *d_path_dir;
  QMap<QString,FileInfo *> d_file_infos;
  QTimer *d_scan_timer;
  Config *d_config;
  QList<unsigned> d_event_carts;
  FileInfo *d_event_info;
  bool d_event_loaded;
  //  bool d_event_active;
  bool d_scanning;
};


#endif  // FILEDETECTOR_H
