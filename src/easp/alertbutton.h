// alertbutton.h
//
// Display summary details for an EAS alert
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

#ifndef ALERTBUTTON_H
#define ALERTBUTTON_H

#include <QFrame>
#include <QLabel>
#include <QPushButton>

#include "alert.h"

class AlertButton : public QFrame
{
  Q_OBJECT;
 public:
  AlertButton(int id,QWidget *parent=0);
  Alert *alert() const;
  void setAlert(Alert *alert);
  bool selected() const;
  void setSelected(bool state);

 signals:
  void clicked(int id);
  void closeClicked(int id);

 private slots:
  void closeClickedData();

 protected:
  void mouseMoveEvent(QMouseEvent *e);
  void mousePressEvent(QMouseEvent *e);
  void mouseReleaseEvent(QMouseEvent *e);
  void resizeEvent(QResizeEvent *e);

 private:
  QLabel *alert_id_label;
  QPushButton *alert_close_button;
  QLabel *alert_title_label;
  QLabel *alert_datetime_label;
  Alert *alert_alert;
  bool alert_mouse_pressed;
  bool alert_selected;
  int alert_id;
};


#endif  // ALERTBUTTON_H
