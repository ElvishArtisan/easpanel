// alert.cpp
//
// Abstract an EAS alert
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

#include <QFile>
#include <QFileInfo>
#include <QTextStream>

#include "alert.h"
#include "profile.h"

Alert::Alert()
{
  clear();
}


QString Alert::filename() const
{
  return alert_filename;
}


QString Alert::title() const
{
  return alert_title;
}


QDateTime Alert::datestamp() const
{
  return alert_datestamp;
}


QDateTime Alert::issuedDateTime() const
{
  return alert_issued_datetime;
}


QDateTime Alert::expiresDateTime() const
{
  return alert_expires_datetime;
}


QString Alert::headerAudio() const
{
  return alert_header_audio;
}


QString Alert::messageAudio() const
{
  return alert_message_audio;
}


QString Alert::text() const
{
  return alert_text;
}


void Alert::setText(const QString &str)
{
  alert_text=str;
}


QString Alert::dump() const
{
  QString ret="";

  ret+="[Alert]\n";
  ret+="Title="+title()+"\n";
  ret+="Issued="+issuedDateTime().toString("yyyy-MM-dd mm:mm:ss")+"\n";
  ret+="Expires="+expiresDateTime().toString("yyyy-MM-dd mm:mm:ss")+"\n";
  ret+="HeaderAudio="+headerAudio()+"\n";
  ret+="MessageAudio="+messageAudio()+"\n";
  ret+="\n";
  ret+="[Text]\n";
  ret+=text()+"\n";

  return ret;
}


bool Alert::load(const QString &pathname)
{
  clear();

  QFile file(pathname);
  if(!file.open(QIODevice::ReadOnly)) {
    return false;
  }
  QFileInfo fileinfo(file);
  alert_filename=fileinfo.fileName();
  alert_datestamp=fileinfo.lastModified();
  QTextStream stream(&file);
  do {
    if(stream.readLine()=="[Text]") {
      alert_text=stream.readAll();
      break;
    }
  } while (!stream.atEnd());

  file.close();
  Profile *p=new Profile();

  p->setSource(pathname);
  alert_title=p->stringValue("Alert","Title");
  alert_issued_datetime=p->dateTimeValue("Alert","Issued");
  alert_expires_datetime=p->dateTimeValue("Alert","Expires");
  alert_header_audio=p->stringValue("Alert","HeaderAudio");
  alert_message_audio=p->stringValue("Alert","MessageAudio");
  delete p;

  return true;
}


void Alert::clear()
{
  alert_title="";
  alert_issued_datetime=QDateTime();
  alert_expires_datetime=QDateTime();
  alert_header_audio="";
  alert_message_audio="";
  alert_text="";
}
