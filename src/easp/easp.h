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
#include "alerttextedit.h"
#include "config.h"
#include "modebutton.h"

#define EASP_ALERT_QUAN 5

class MainWidget : public QWidget
{
  Q_OBJECT;
 public:
  MainWidget(QWidget *parent=0);
  QSize sizeHint() const;

 private slots:
  void autoData();
  void liveSendData();
  void cannedSendData();
  void autoSendData(int id);
  void alertScanData();
  void alertSelectedData(int id);
  void alertClosedData(int id);
  void rlmReadyReadData();
  void quit();

 protected:
  void resizeEvent(QResizeEvent *e);
  void keyPressEvent(QKeyEvent *e);
  void closeEvent(QCloseEvent *e);

 private:
  void SendNextAlert();
  void ProcessNowPlaying(unsigned cartnum);
  bool ProcessNewAlert(Alert *alert);
  void DisplayAlertButton(AlertButton *button);
  void CompactButtons();
  void SendRml(const QString &rml);
  void BringToTop();
  bool AlertLoaded() const;
  void RetireAlertFile(const QString &filename) const;
  QLabel *main_title_label;
  QLabel *main_datetime_label;
  AlertTextEdit *main_text_text;
  AlertButton *main_alert_buttons[EASP_ALERT_QUAN];
  ModeButton *main_auto_button;
  bool main_auto;
  QPushButton *main_livesend_button;
  QPushButton *main_cannedsend_button;
  QTimer *main_alert_scan_timer;
  Config *main_config;
  QMap<QString,Alert *> main_alerts;
  int main_selected_alert_id;
  QUdpSocket *main_rml_socket;
  bool main_next_is_voicetrack;
};


#endif  // EASP_H
