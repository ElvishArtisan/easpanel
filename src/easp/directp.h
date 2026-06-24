// directp.h
//
// Direct importer control applet for EasPanel
//
//   (C) Copyright 2018-2020 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef DIRECTP_H
#define DIRECTP_H

#include <QLabel>
#include <QList>
#include <QTextEdit>
#include <QTimer>
#include <QMap>
#include <QPushButton>
#include <QUdpSocket>
#include <QWidget>

#include "config.h"
#include "directfilewidget.h"
#include "sigwatcher.h"

#define DIRECTP_USAGE "[--config-file=<conf-path>] [--dump-config]|[--no-publish-point-cleanup] [--no-raise] [--automatic|--paused]\n\n"

class MainWidget : public QWidget
{
  Q_OBJECT;
 public:
  MainWidget(QWidget *parent=0);
  QSize sizeHint() const;

 private slots:
  void rlmReadyReadData();
  void bringToTop();
  void receivedSignalData(int signum);
  void quit();

 protected:
  void resizeEvent(QResizeEvent *e);
  void keyPressEvent(QKeyEvent *e);
  void closeEvent(QCloseEvent *e);

 private:
  QList<pid_t> GetPids();
  DirectFileWidget *main_direct_file_widget;
  QUdpSocket *main_rml_socket;
  SigWatcher *d_sig_watcher;
  Config *main_config;
  bool d_raise_on_alert;
};


#endif  // DIRECTP_H
