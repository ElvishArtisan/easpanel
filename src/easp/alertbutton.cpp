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
  alert_eom_played=false;
  alert_selected=false;

  //
  // Fonts
  //
  QFont title_font(font().family(),font().pointSize()+2,QFont::Bold);
  QFont bold_font(font().family(),font().pointSize(),QFont::Bold);
  QFont status_font(font().family(),font().pointSize(),QFont::Normal,true);

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
  // Status Label
  //
  alert_status_label=new QLabel(this);
  alert_status_label->setFont(status_font);
  alert_status_label->setAlignment(Qt::AlignLeft|Qt::AlignTop);
  alert_status_label->setWordWrap(true);
  alert_status_label->hide();

  //
  // Datetime Label
  //
  alert_datetime_label=new QLabel(this);
  alert_datetime_label->setAlignment(Qt::AlignLeft|Qt::AlignTop);
}


int AlertButton::id() const
{
  return alert_id;
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
  alert_eom_played=false;
  alert_alert=alert;
  setStatus(AlertButton::New);
}


bool AlertButton::selected() const
{
  return alert_selected;
}


void AlertButton::setSelected(bool state)
{
  if(alert_selected!=state) {
    alert_selected=state;
    UpdateColor();
  }
}


AlertButton::Status AlertButton::status() const
{
  return alert_status;
}


void AlertButton::setStatus(AlertButton::Status status)
{
  if(status!=alert_status) {
    alert_status=status;
    UpdateColor();
  }
}


QString AlertButton::statusText() const
{
  return alert_status_label->text();
}


void AlertButton::addStatusText(const QString &str)
{
  if(alert_status_label->text().isEmpty()) {
    alert_status_label->setText(str);
  }
  else {
    alert_status_label->setText(alert_status_label->text()+" "+str);
  }
}


bool AlertButton::eomPlayed() const
{
  return alert_eom_played;
}


void AlertButton::setEomPlayed(bool state)
{
  alert_eom_played=state;
}


void AlertButton::copyFrom(AlertButton *button)
{
  alert_alert=button->alert();
  alert_eom_played=button->eomPlayed();
  alert_selected=false;
  alert_title_label->setText(button->alert()->title());
  alert_datetime_label->
    setText(tr("Issued")+": "+
	    button->alert()->issuedDateTime().toString("MMMM d @ h:mm ap")+"\n"+
	    tr("Expires")+": "+
	    button->alert()->expiresDateTime().toString("MMMM d @ h:mm ap"));
  alert_status=button->status();
  alert_status_label->setText(button->statusText());
  alert_mouse_pressed=false;

  UpdateColor();
}


void AlertButton::clear()
{
  alert_alert=NULL;
  alert_eom_played=false;
  alert_selected=false;
  alert_title_label->clear();
  alert_datetime_label->clear();
  alert_status=AlertButton::New;
  alert_status_label->clear();
  alert_mouse_pressed=false;

  UpdateColor();
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
  alert_title_label->setGeometry(10,h/4,w-20,16);
  alert_status_label->setGeometry(10,h/4+20,w-20,h-(h/4+20+32));
  alert_datetime_label->setGeometry(10,h-40,w-20,32);
}


void AlertButton::UpdateColor()
{
  if(alert_selected) {
    if(alert_status==AlertButton::Error) {
      setStyleSheet("background-color: "+ALERTBUTTON_ERROR_COLOR);
    }
    else {
      setStyleSheet("background-color: "+ALERTBUTTON_SELECTED_COLOR);
    }
  }
  else {
    if(alert_status==AlertButton::Error) {
      setStyleSheet("background-color: "+ALERTBUTTON_ERROR_COLOR);
    }
    else {
      setStyleSheet("");
    }
  }
  alert_status_label->setVisible(alert_status==AlertButton::Error);
}
