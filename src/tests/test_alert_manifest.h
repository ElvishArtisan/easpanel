// test-alert-manifest.h
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

#ifndef TEST_ALERT_MANIFEST_H
#define TEST_ALERT_MANIFEST_H

#include <QTimer>
#include <QMap>
#include <QObject>
#include <QUdpSocket>

#include "alert.h"

#define TEST_ALERT_MANIFEST_USAGE "--filename=<alert-file>\n"

class MainObject : public QObject
{
  Q_OBJECT;
 public:
  MainObject(QWidget *parent=0);
};


#endif  // TEST_ALERT_MANIFEST_H
