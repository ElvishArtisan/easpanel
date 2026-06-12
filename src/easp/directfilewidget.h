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

class DirectFileWidget : public QFrame
{
  Q_OBJECT;
 public:
  DirectFileWidget(QUdpSocket *rml_sock,Config *c,QWidget *parent=0);
  QSize sizeHint() const;
  void playedCart(unsigned cartnum);

 private slots:
  void onButtonData(int id);
  void offButtonData(int id);
  void eventStartedData(int id);
  void eventStoppedData(int id);
  void scanningStartedData(int id);
  void scanningStoppedData(int id);

 protected:
  void resizeEvent(QResizeEvent *e);

 private:
  QSignalMapper *d_on_button_mapper;
  QList<QPushButton *> d_on_buttons;
  QSignalMapper *d_off_button_mapper;
  QList<QPushButton *> d_off_buttons;
  QList<QLabel *> d_description_labels;
  QList<FileDetector *> d_detectors;
  Config *d_config;
};


#endif  // DIRECTFILEWIDGET_H
