// easbelt.h
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

#ifndef EASBELT_H
#define EASBELT_H

#include <syslog.h>

#include <QMap>
#include <QObject>
#include <QSignalMapper>
#include <QTimer>

#include "cmdswitch.h"
#include "config.h"
#include "belttask.h"

#define EASBELT_SCP_BINARY "/usr/bin/scp"
#define EASBELT_TASK_INTERVAL 5000
#define EASBELT_USAGE "[--dump-config]\n\n"

class MainObject : public QObject
{
  Q_OBJECT;
 public:
  MainObject(QObject *parent=0);

 private slots:
  void taskData();
  void taskFinished(QObject *obj);

 private:
  bool CleanSource(int src_id,const QString &src_filename) const;
  QMap<QProcess *,BeltTask *> d_task_map;
  QSignalMapper *d_task_mapper;
  Config *d_config;
  QTimer *d_task_timer;
  bool d_one_shot;
};


#endif  // EASBELT_H
