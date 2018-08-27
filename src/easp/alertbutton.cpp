// alertbutton.cpp
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

#include <QMouseEvent>
#include <QStyle>

#include "alertbutton.h"

AlertButton::AlertButton(int id,QWidget *parent)
  : QFrame(parent)
{
  setFrameStyle(QFrame::Panel|QFrame::Sunken);
  alert_alert=NULL;
  alert_mouse_pressed=false;
  alert_id=id;
  alert_selected=false;

  //
  // Fonts
  //
  QFont title_font(font().family(),font().pointSize()+2,QFont::Bold);
  QFont bold_font(font().family(),font().pointSize(),QFont::Bold);

  //
  // ID Label
  //
  alert_id_label=new QLabel(tr("ALERT")+QString().sprintf(" %d",id+1),this);
  alert_id_label->setFont(title_font);
  alert_id_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  //
  // Close Button
  //
  alert_close_button=new QPushButton(this);
  connect(alert_close_button,SIGNAL(clicked()),this,SLOT(closeClickedData()));

  //
  // Title Label
  //
  alert_title_label=new QLabel(this);
  alert_title_label->setFont(bold_font);
  alert_title_label->setAlignment(Qt::AlignLeft|Qt::AlignTop);
  alert_title_label->setWordWrap(true);

  //
  // Datetime Label
  //
  alert_datetime_label=new QLabel(this);
  alert_datetime_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
}


Alert *AlertButton::alert() const
{
  return alert_alert;
}


void AlertButton::setAlert(Alert *alert)
{
  if(alert==NULL) {
    alert_title_label->clear();
    alert_datetime_label->clear();
  }
  else {
    alert_title_label->setText(alert->title());
    alert_datetime_label->
      setText(tr("Issued")+": "+
	      alert->issuedDateTime().toString("MMMM d @ h:mm ap")+"\n"+
	      tr("Expires")+": "+
	      alert->expiresDateTime().toString("MMMM d @ h:mm ap"));
  }
  alert_alert=alert;
}


bool AlertButton::selected() const
{
  return alert_selected;
}


void AlertButton::setSelected(bool state)
{
  if(alert_selected!=state) {
    if(state) {
      setStyleSheet("background-color: #FFFF00");
    }
    else {
      setStyleSheet("");
    }
    alert_selected=state;
  }
}


void AlertButton::closeClickedData()
{
  emit closeClicked(alert_id);
}


void AlertButton::mouseMoveEvent(QMouseEvent *e)
{
  if((e->x()<0)||(e->x()>=width())||(e->y()<0)||(e->y()>=height())) {
    alert_mouse_pressed=false;
  }
}


void AlertButton::mousePressEvent(QMouseEvent *e)
{
  alert_mouse_pressed=e->button()==Qt::LeftButton;
}


void AlertButton::mouseReleaseEvent(QMouseEvent *e)
{
  if((e->button()==Qt::LeftButton)&&alert_mouse_pressed) {
    alert_mouse_pressed=false;
    emit clicked(alert_id);
  }
}


void AlertButton::resizeEvent(QResizeEvent *e)
{
  int w=size().width();
  int h=size().height();

  alert_id_label->setGeometry(10,1,w-35,h/4);
  alert_close_button->setGeometry(w-25,5,20,20);
  alert_close_button->setIconSize(QSize(20,20));
  alert_close_button->
    setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton,0,alert_close_button));
  alert_title_label->setGeometry(10,h/4,w-20,h/2);
  alert_datetime_label->setGeometry(10,5*h/8,w-20,3*h/8);
}
