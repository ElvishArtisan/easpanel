// directfilewidget.h
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

#ifndef DIRECTFILEWIDGET_H
#define DIRECTFILEWIDGET_H

#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QSignalMapper>

#include "config.h"
#include "filedetector.h"
#include "modebutton.h"

class DirectFileWidget : public QFrame
{
  Q_OBJECT;
 public:
  DirectFileWidget(QUdpSocket *rml_sock,Config *c,QWidget *parent=0);
  QSize sizeHint() const;
  void playedCart(unsigned cartnum);
  void cleanPaths() const;
  void setAutomaticMode(int id=-1);
  void setPausedMode(int id=-1);

 signals:
  void raiseRequested();
  void quitRequested();

 private slots:
  void autoData(int id);
  void eventStartedData(int id,FileInfo *info);
  void eventStoppedData(int id);
  void scanningStartedData(int id);
  void scanningStoppedData(int id);
  void quitRequestedData();

 protected:
  void resizeEvent(QResizeEvent *e);

 private:
  void SetPausedMode(int id);
  void SetAutomaticMode(int id);
  QSignalMapper *d_mode_button_mapper;
  QList<ModeButton *> d_mode_buttons;
  QList<QLabel *> d_description_labels;
  QList<QLabel *> d_datetime_labels;
  QList<FileDetector *> d_detectors;
  QList<bool> d_autos;
  Config *d_config;
};


#endif  // DIRECTFILEWIDGET_H
