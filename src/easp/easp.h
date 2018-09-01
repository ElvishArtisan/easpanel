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
#include <QUdpSocket>
#include <QWidget>

#include "alertbutton.h"
#include "config.h"

#define EASP_ALERT_QUAN 5

class MainWidget : public QWidget
{
  Q_OBJECT;
 public:
  MainWidget(QWidget *parnt=0);
  QSize sizeHint() const;

 private slots:
  void liveSendData();
  void cannedSendData();
  void alertScanData();
  void alertSelectedData(int id);
  void alertClosedData(int id);
  void startData();
  void endData();
  void rlmReadyReadData();

 protected:
  void resizeEvent(QResizeEvent *e);

 private:
  void ProcessNowNext(unsigned cartnum);
  void ProcessNewAlert(Alert *alert);
  void DisplayAlertButton(AlertButton *button);
  void CompactButtons();
  void SendRml(const QString &rml);
  QLabel *main_title_label;
  QLabel *main_datetime_label;
  QTextEdit *main_text_text;
  AlertButton *main_alert_buttons[EASP_ALERT_QUAN];
  QPushButton *main_livesend_button;
  QPushButton *main_cannedsend_button;
  //  QPushButton *main_start_button;
  //  QPushButton *main_end_button;
  QTimer *main_alert_scan_timer;
  Config *main_config;
  QMap<QString,Alert *> main_alerts;
  int main_selected_alert_id;
  QUdpSocket *main_rml_socket;
};


#endif  // EASP_H
