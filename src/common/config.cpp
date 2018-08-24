// config.cpp
//
// Configuration values for easpanel
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

#include "config.h"
#include "profile.h"

Config::Config()
{
  clear();
}


QString Config::rdairplayHostname() const
{
  return conf_rdairplay_hostname;
}


QString Config::pathsEasMessages() const
{
  return conf_paths_eas_messages;
}


QString Config::pathsEasAudio() const
{
  return conf_paths_eas_audio;
}


QString Config::pathsRivendellAudio() const
{
  return conf_paths_rivendell_audio;
}


unsigned Config::cartsAlertTone() const
{
  return conf_carts_alert_tone;
}


unsigned Config::cartsEom() const
{
  return conf_carts_eom;
}


unsigned Config::cartsFriendlyIntro() const
{
  return conf_carts_friendly_intro;
}


unsigned Config::cartsFriendlyOutro() const
{
  return conf_carts_friendly_outro;
}


unsigned Config::cartsAlertHeaderFirst() const
{
  return conf_carts_alert_header_first;
}


unsigned Config::cartsAlertHeaderLast() const
{
  return conf_carts_alert_header_last;
}


unsigned Config::cartsAlertMessageFirst() const
{
  return conf_carts_alert_message_first;
}


unsigned Config::cartsAlertMessageLast() const
{
  return conf_carts_alert_message_last;
}


QString Config::dump() const
{
  QString ret="";

  ret+="[RDAirPlay]\n";
  ret+="Hostname="+rdairplayHostname()+"\n";
  ret+="\n";
  ret+="[Paths]\n";
  ret+="EasMessages="+pathsEasMessages()+"\n";
  ret+="EasAudio="+pathsEasAudio()+"\n";
  ret+="RivendellAudio="+pathsRivendellAudio()+"\n";
  ret+="\n";
  ret+="[Carts]\n";
  ret+=QString().sprintf("AlertTone=%u\n",cartsAlertTone());
  ret+=QString().sprintf("Eom=%u\n",cartsEom());
  ret+=QString().sprintf("FriendlyIntro=%u\n",cartsFriendlyIntro());
  ret+=QString().sprintf("FriendlyOutro=%u\n",cartsFriendlyOutro());
  ret+=QString().sprintf("AlertHeaderFirst=%u\n",cartsAlertHeaderFirst());
  ret+=QString().sprintf("AlertHeaderLast=%u\n",cartsAlertHeaderLast());
  ret+=QString().sprintf("AlertMessageFirst=%u\n",cartsAlertMessageFirst());
  ret+=QString().sprintf("AlertMessageLast=%u\n",cartsAlertMessageLast());

  return ret;
}


bool Config::load()
{
  bool ret=false;
  Profile *p=new Profile();

  ret=p->setSource(CONFIG_FILE_NAME);

  conf_rdairplay_hostname=p->stringValue("RDAirPlay","Hostname","localhost");
  conf_paths_eas_messages=
    p->stringValue("Paths","EasMessages","/var/eas/messages");
  conf_paths_eas_audio=p->stringValue("Paths","EasAudio","/var/eas/audio");
  conf_paths_rivendell_audio=
    p->stringValue("Paths","RivendellAudio","/var/snd");
  conf_carts_alert_tone=p->intValue("Carts","AlertTone");
  conf_carts_eom=p->intValue("Carts","Eom");
  conf_carts_friendly_intro=p->intValue("Carts","FriendlyIntro");
  conf_carts_friendly_outro=p->intValue("Carts","FriendlyOutro");
  conf_carts_alert_header_first=p->intValue("Carts","AlertHeaderFirst");
  conf_carts_alert_header_last=p->intValue("Carts","AlertHeaderLast");
  conf_carts_alert_message_first=p->intValue("Carts","AlertMessageFirst");
  conf_carts_alert_message_last=p->intValue("Carts","AlertMessageLast");

  delete p;

  return ret;
}


void Config::clear()
{
  conf_rdairplay_hostname="";
  conf_paths_eas_messages="";
  conf_paths_eas_audio="";
  conf_paths_rivendell_audio="";
  conf_carts_alert_tone=0;
  conf_carts_eom=0;
  conf_carts_friendly_intro=0;
  conf_carts_friendly_outro=0;
  conf_carts_alert_header_first=0;
  conf_carts_alert_header_last=0;
  conf_carts_alert_message_first=0;
  conf_carts_alert_message_last=0;
}
