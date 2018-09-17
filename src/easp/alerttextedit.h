// alerttextedit.h
//
// TextEdit widget for displaying alert details.
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

#ifndef ALERTTEXTEDIT_H
#define ALERTTEXTEDIT_H

#include <QKeyEvent>
#include <QTextEdit>

class AlertTextEdit : public QTextEdit
{
  Q_OBJECT;
 public:
  AlertTextEdit(QWidget *parent=0);

 signals:
  void quitRequested();

 protected:
  void keyPressEvent(QKeyEvent *e);
};


#endif  // ALERTTEXTEDIT_H
