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

#include <unistd.h>

#include <rivendell/rd_getuseragent.h>
#include <rivendell/rd_getversion.h>
#include <rivendell/rd_import.h>
#include <rivendell/rd_removecart.h>

#include "config.h"
#include "profile.h"

Config::Config()
{
  conf_user_agent=QString("Mozilla/5.0 (X11; Linux ")+ARCH+") "+
    "easpanel/"+VERSION+" "+RD_GetUserAgent()+RD_GetVersion();
  clear();
}


QHostAddress Config::rivendellHostAddress() const
{
  return conf_rivendell_host_address;
}


int Config::rivendellLogMachine() const
{
  return conf_rivendell_log_machine;
}


QString Config::rivendellAlertAudioGroup() const
{
  return conf_rivendell_alert_audio_group;
}


QString Config::rivendellUser() const
{
  return conf_rivendell_user;
}


QString Config::rivendellPassword() const
{
  return conf_rivendell_password;
}


unsigned Config::rivendellAlertToneCart() const
{
  return conf_rivendell_alert_tone_cart;
}


unsigned Config::rivendellFriendlyIntroCart() const
{
  return conf_rivendell_friendly_intro_cart;
}


unsigned Config::rivendellFriendlyOutroCart() const
{
  return conf_rivendell_friendly_outro_cart;
}


QString Config::pathsEasMessages() const
{
  return conf_paths_eas_messages;
}


QString Config::pathsEasAudio() const
{
  return conf_paths_eas_audio;
}


unsigned Config::pathsRlmReceivePort() const
{
  return conf_paths_rlm_receive_port;
}


QString Config::dump() const
{
  QString ret="";

  ret+="[Rivendell]\n";
  ret+="HostAddress="+rivendellHostAddress().toString()+"\n";
  ret+="AlertAudioGroup="+rivendellAlertAudioGroup()+"\n";
  ret+="User="+rivendellUser()+"\n";
  ret+="Password="+rivendellPassword()+"\n";
  ret+="AlertToneCart="+QString().sprintf("%u",rivendellAlertToneCart())+"\n";
  ret+="FriendlyIntroCart="+
    QString().sprintf("%u",rivendellFriendlyIntroCart())+"\n";
  ret+="FriendlyOutroCart="+
    QString().sprintf("%u",rivendellFriendlyOutroCart())+"\n";
  ret+="\n";
  ret+="[Paths]\n";
  ret+="EasMessages="+pathsEasMessages()+"\n";
  ret+="EasAudio="+pathsEasAudio()+"\n";
  ret+="RlmReceivePort="+QString().sprintf("%u",pathsRlmReceivePort())+"\n";
  ret+="\n";

  return ret;
}


bool Config::load()
{
  bool ret=false;
  Profile *p=new Profile();

  ret=p->setSource(CONFIG_FILE_NAME);

  conf_rivendell_host_address=
    QHostAddress(p->stringValue("Rivendell","HostAddress","127.0.0.1"));
  conf_rivendell_log_machine=p->intValue("Rivendell","LogMachine",1);
  conf_rivendell_alert_audio_group=
    p->stringValue("Rivendell","AlertAudioGroup","EAS");
  conf_rivendell_user=p->stringValue("Rivendell","User","user");
  conf_rivendell_password=p->stringValue("Rivendell","Password");
  conf_rivendell_alert_tone_cart=p->intValue("Rivendell","AlertToneCart");
  conf_rivendell_friendly_intro_cart=
    p->intValue("Rivendell","FriendlyIntroCart");
  conf_rivendell_friendly_outro_cart=
    p->intValue("Rivendell","FriendlyOutroCart");

  conf_paths_eas_messages=
    p->stringValue("Paths","EasMessages","/var/eas/messages");
  conf_paths_eas_audio=p->stringValue("Paths","EasAudio","/var/eas/audio");
  conf_paths_rlm_receive_port=p->intValue("Paths","RlmReceivePort",2634);

  delete p;

  return ret;
}


void Config::clear()
{
  conf_rivendell_host_address=QHostAddress();
  conf_rivendell_log_machine=0;
  conf_rivendell_alert_audio_group="";
  conf_rivendell_user="";
  conf_rivendell_password="";
  conf_rivendell_alert_tone_cart=0;
  conf_rivendell_friendly_intro_cart=0;
  conf_rivendell_friendly_outro_cart=0;
  conf_paths_eas_messages="";
  conf_paths_eas_audio="";
  conf_paths_rlm_receive_port=0;
}


unsigned Config::importCart(const QString &title,const QString &filename,
			    QString *err_msg)
{
  struct rd_cartimport *carts=NULL;
  unsigned numrecs=0;
  int ret=0;

  if(RD_ImportCart(&carts,conf_rivendell_host_address.toString().toUtf8(),
		   conf_rivendell_user.toUtf8(),
		   conf_rivendell_password.toUtf8(),"",0,0,1,0,0,0,1,
		   conf_rivendell_alert_audio_group.toUtf8(),
		   title.toUtf8(),
		   (conf_paths_eas_audio+"/"+filename).toUtf8(),
		   conf_user_agent.toUtf8(),&numrecs)!=0) {
    *err_msg="unspecified error";
    if(numrecs>0) {
      *err_msg=carts[0].error_string;
      delete carts;
    }
    return 0;
  }
  if(numrecs>=1) {
    ret=carts[0].cart_number;
  }
  unlink((conf_paths_eas_audio+"/"+filename).toUtf8());
  free(carts);
  return ret;
}


bool Config::removeCart(unsigned cartnum,QString *err_msg)
{
  return RD_RemoveCart(conf_rivendell_host_address.toString().toUtf8(),
		       conf_rivendell_user.toUtf8(),
		       conf_rivendell_password.toUtf8(),
		       "",cartnum,conf_user_agent.toUtf8())==0;
}
