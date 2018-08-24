// easp.h
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

#ifndef EASP_H
#define EASP_H

#include <QLabel>
#include <QTextEdit>
#include <QTimer>
#include <QMap>
#include <QPushButton>
#include <QWidget>

#include "alert.h"
#include "config.h"

class MainWidget : public QWidget
{
  Q_OBJECT;
 public:
  MainWidget(QWidget *parnt=0);
  QSize sizeHint() const;

 private slots:
  void alertScanData();
  void startData();
  void endData();

 protected:
  void resizeEvent(QResizeEvent *e);

 private:
  void ProcessNewAlert(Alert *alert);
  QLabel *main_title_label;
  QLabel *main_datetime_label;
  QTextEdit *main_text_text;
  QPushButton *main_start_button;
  QPushButton *main_end_button;
  QTimer *main_alert_scan_timer;
  Config *main_config;
  QMap<QString,Alert *> main_alerts;
};


#endif  // EASP_H
