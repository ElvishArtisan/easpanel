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
#include <QMap>
#include <QStringList>
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


int Alert::touches() const
{
  return alert_touches;
}


void Alert::touch()
{
  alert_touches++;
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


QString Alert::eomAudio() const
{
  return alert_eom_audio;
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


unsigned Alert::headerCart() const
{
  return alert_header_cart;
}


void Alert::setHeaderCart(unsigned cartnum)
{
  alert_header_cart=cartnum;
}


unsigned Alert::eomCart() const
{
  return alert_eom_cart;
}


void Alert::setEomCart(unsigned cartnum)
{
  alert_eom_cart=cartnum;
}


unsigned Alert::messageCart() const
{
  return alert_message_cart;
}


void Alert::setMessageCart(unsigned cartnum)
{
  alert_message_cart=cartnum;
}


QString Alert::dump() const
{
  QString ret="";

  ret+="[Alert]\n";
  ret+="Title="+title()+"\n";
  ret+="Issued="+issuedDateTime().toString("yyyy-MM-dd mm:mm:ss")+"\n";
  ret+="Expires="+expiresDateTime().toString("yyyy-MM-dd mm:mm:ss")+"\n";
  ret+="HeaderAudio="+headerAudio()+"\n";
  ret+="EomAudio="+eomAudio()+"\n";
  ret+="MessageAudio="+messageAudio()+"\n";
  ret+="\n";
  ret+="[Text]\n";
  ret+=text()+"\n";

  return ret;
}


bool Alert::load(const QString &pathname)
{
  QString line;
  QString key;
  QMap<QString,QString> values;
  bool escaped=false;

  QMap<QString,QString> event_codes;
  event_codes["EAN"]=QObject::tr("Emergency Action Notification");
  event_codes["NPT"]=QObject::tr("National Periodic Test");
  event_codes["NIC"]=QObject::tr("National Information Center");
  event_codes["RMT"]=QObject::tr("Required Monthly Test");
  event_codes["RWT"]=QObject::tr("Required Weekly Test");
  event_codes["ADR"]=QObject::tr("Administrative Message");
  event_codes["AVW"]=QObject::tr("Avalanche Warning");
  event_codes["AVA"]=QObject::tr("Avalanche Watch");
  event_codes["BZW"]=QObject::tr("Blizzard Warning");
  event_codes["CAE"]=QObject::tr("Child Abduction Emergency");
  event_codes["CDW"]=QObject::tr("Civil Danger Warning");
  event_codes["CEM"]=QObject::tr("Civil Emergency Message");
  event_codes["CFW"]=QObject::tr("Coastal Flood Warning");
  event_codes["CFA"]=QObject::tr("Coastal Flood Watch");
  event_codes["DMO"]=QObject::tr("Demo / Practice Warning");
  event_codes["DSW"]=QObject::tr("Dust Storm Warning");
  event_codes["EQW"]=QObject::tr("Earthquake Warning");
  event_codes["EWW"]=QObject::tr("Extreme Wind Warning");
  event_codes["EVI"]=QObject::tr("Evancuation Immediate");
  event_codes["FRW"]=QObject::tr("Fire Warning");
  event_codes["FFW"]=QObject::tr("Flash Flood Warning");
  event_codes["FFA"]=QObject::tr("Flash Flood Watch");
  event_codes["FFS"]=QObject::tr("Flash Flood Statement");
  event_codes["FLW"]=QObject::tr("Flood Warning");
  event_codes["FLA"]=QObject::tr("Flood Watch");
  event_codes["FLS"]=QObject::tr("Flood Statement");
  event_codes["HMW"]=QObject::tr("Hazardous Materials Warning");
  event_codes["HWW"]=QObject::tr("High Wind Warning");
  event_codes["HWA"]=QObject::tr("High Wind Watch");
  event_codes["HUW"]=QObject::tr("Hurricane Warning");
  event_codes["HUA"]=QObject::tr("Hurricane Watch");
  event_codes["HLS"]=QObject::tr("Hurricane Statement");
  event_codes["LEW"]=QObject::tr("Law Enforcement Warning");
  event_codes["LAE"]=QObject::tr("Local Area Emergency");
  event_codes["NMN"]=QObject::tr("Network Message Notification");
  event_codes["TOE"]=QObject::tr("911 Telephone Outage");
  event_codes["NUW"]=QObject::tr("Nuclear Power Plant Warning");
  event_codes["RHW"]=QObject::tr("Radiological Hazard Warning");
  event_codes["SVR"]=QObject::tr("Severe Thunderstorm Warning");
  event_codes["SVA"]=QObject::tr("Severe Thunderstorm Watch");
  event_codes["SVS"]=QObject::tr("Severe Weather Statement");
  event_codes["SPW"]=QObject::tr("Shelter in Place Warning");
  event_codes["SMW"]=QObject::tr("Special Marine Warning");
  event_codes["SPS"]=QObject::tr("Special Weather Statement");
  event_codes["SSA"]=QObject::tr("Storm Surge Watch");
  event_codes["SSW"]=QObject::tr("Storm Surge Warning");
  event_codes["TOR"]=QObject::tr("Tornade Warning");
  event_codes["TOA"]=QObject::tr("Tornade Watch");
  event_codes["TRW"]=QObject::tr("Tropical Storm Warning");
  event_codes["TRA"]=QObject::tr("Tropical Storm Watch");
  event_codes["TSW"]=QObject::tr("Tsunami Warning");
  event_codes["TSA"]=QObject::tr("Tusnami Watch");
  event_codes["VOW"]=QObject::tr("Volcano Warning");
  event_codes["WSA"]=QObject::tr("Winter Storm Watch");
  event_codes["WSW"]=QObject::tr("Winter Storm Warning");

  QFile file(pathname);
  if(!file.open(QIODevice::ReadOnly)) {
    return false;
  }
  QFileInfo fileinfo(file);
  alert_filename=fileinfo.fileName();
  alert_datestamp=fileinfo.lastModified();
  QTextStream stream(&file);

  while(stream.readLine()!="#BEGIN");
  while((line=stream.readLine())!="#END") {
    if(escaped) {
      if(line.right(1)=="'") {
	escaped=false;
      }
      values[key]+=line.replace("'","")+"\n";
    }
    else {
      QStringList f0=line.split("=",QString::KeepEmptyParts);
      if(f0.size()>=2) {
	key=f0.at(0).trimmed();
	for(int i=2;i<f0.size();i++) {
	  f0[1]="="+f0[i];
	}
	if(f0[1].right(1)!="'") {
	  values[key]=f0[1].replace("'","")+"\n";
	  escaped=true;
	}
	else {
	  values[key]=f0[1].replace("'","");
	}
      }
    }
  }

  if(values.find("EAS.TYPE")!=values.constEnd()) {
    if(event_codes.find(values.find("EAS.TYPE").value().trimmed())!=
       event_codes.constEnd()) {
      alert_title=
	event_codes.find(values.find("EAS.TYPE").value().trimmed()).value();
    }
    else {
      alert_title=values.find("EAS.TYPE").value().trimmed();
    }
  }
  if(values.find("EAS.START_TIME")!=values.constEnd()) {
    alert_issued_datetime=
      QDateTime::fromTime_t(values.find("EAS.START_TIME").value().toInt());
  }
  if(values.find("EAS.END_TIME")!=values.constEnd()) {
    alert_expires_datetime=
      QDateTime::fromTime_t(values.find("EAS.END_TIME").value().toInt());
  }
  if(values.find("EAS.AUDIO.FILE.HEADER")!=values.constEnd()) {
    alert_header_audio=values.find("EAS.AUDIO.FILE.HEADER").value().trimmed();
  }
  if(values.find("EAS.AUDIO.FILE.EOM")!=values.constEnd()) {
    alert_eom_audio=values.find("EAS.AUDIO.FILE.EOM").value().trimmed();
  }
  if(values.find("EAS.AUDIO.FILE.ALERT")!=values.constEnd()) {
    alert_message_audio=values.find("EAS.AUDIO.FILE.ALERT").value().trimmed();
  }
  if(values.find("EAS.TRANSLATION")!=values.constEnd()) {
    alert_text=values.find("EAS.TRANSLATION").value();
  }
  /*
  for(QMap<QString,QString>::const_iterator it=values.constBegin();
      it!=values.constEnd();it++) {
    printf("%s = %s\n",(const char *)it.key().toUtf8(),
	   (const char *)it.value().toUtf8());
  }
  */
  return true;
}


void Alert::clear()
{
  alert_title="";
  alert_touches=0;
  alert_issued_datetime=QDateTime();
  alert_expires_datetime=QDateTime();
  alert_header_audio="";
  alert_eom_audio="";
  alert_message_audio="";
  alert_text="";
  alert_header_cart=0;
  alert_eom_cart=0;
  alert_message_cart=0;
}
