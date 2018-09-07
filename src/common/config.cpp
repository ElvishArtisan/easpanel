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

#include <rivwebcapi/rd_getuseragent.h>
#include <rivwebcapi/rd_getversion.h>
#include <rivwebcapi/rd_import.h>
#include <rivwebcapi/rd_removecart.h>

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


QStringList Config::rivendellVoicetrackGroups() const
{
  return conf_rivendell_voicetrack_groups;
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


int Config::rivendellNormalizationLevel() const
{
  return conf_rivendell_normalization_level;
}


int Config::rivendellAutotrimLevel() const
{
  return conf_rivendell_autotrim_level;
}


QString Config::pathsEasDataDirectory() const
{
  return conf_paths_eas_data_directory;
}


QString Config::pathsEasMessageExtension() const
{
  return conf_paths_eas_message_extension;
}


QStringList Config::pathsEasMessageExtensionFilter()
{
  return conf_paths_eas_message_extension_filter;
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
  ret+="VoicetrackGroups=";
  QStringList groups=rivendellVoicetrackGroups();
  for(int i=0;i<groups.size();i++) {
    ret+=groups.at(i)+",";
  }
  ret=ret.left(ret.size()-1);
  ret+="\n";
  ret+="User="+rivendellUser()+"\n";
  ret+="Password="+rivendellPassword()+"\n";
  ret+="AlertToneCart="+QString().sprintf("%u",rivendellAlertToneCart())+"\n";
  ret+="FriendlyIntroCart="+
    QString().sprintf("%u",rivendellFriendlyIntroCart())+"\n";
  ret+="FriendlyOutroCart="+
    QString().sprintf("%u",rivendellFriendlyOutroCart())+"\n";
  ret+="NormalizationLevel="+
    QString().sprintf("%d",rivendellNormalizationLevel())+"\n";
  ret+="AutotrimLevel="+
    QString().sprintf("%d",rivendellAutotrimLevel())+"\n";
  ret+="\n";
  ret+="[Paths]\n";
  ret+="EasDataDirectory="+pathsEasDataDirectory()+"\n";
  ret+="EasMessageExtension="+pathsEasMessageExtension()+"\n";
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
  conf_rivendell_voicetrack_groups=
    p->stringValue("Rivendell","VoicetrackGroups").
    split(",",QString::SkipEmptyParts);
  for(int i=0;i<conf_rivendell_voicetrack_groups.size();i++) {
    conf_rivendell_voicetrack_groups[i]=
      conf_rivendell_voicetrack_groups.at(i).trimmed();
  }
  conf_rivendell_user=p->stringValue("Rivendell","User","user");
  conf_rivendell_password=p->stringValue("Rivendell","Password");
  conf_rivendell_alert_tone_cart=p->intValue("Rivendell","AlertToneCart");
  conf_rivendell_friendly_intro_cart=
    p->intValue("Rivendell","FriendlyIntroCart");
  conf_rivendell_friendly_outro_cart=
    p->intValue("Rivendell","FriendlyOutroCart");
  conf_rivendell_normalization_level=
    p->intValue("Rivendell","NormalizationLevel",-11);
  conf_rivendell_autotrim_level=p->intValue("Rivendell","AutotrimLevel");

  conf_paths_eas_data_directory=
    p->stringValue("Paths","EasDataDirectory","/var/eas");
  conf_paths_eas_message_extension=
    p->stringValue("Paths","EasMessageExtension","txt");
  conf_paths_eas_message_extension_filter.clear();
  conf_paths_eas_message_extension_filter.
    push_back("*."+conf_paths_eas_message_extension);
  conf_paths_rlm_receive_port=p->intValue("Paths","RlmReceivePort",2634);

  delete p;

  return ret;
}


void Config::clear()
{
  conf_rivendell_host_address=QHostAddress();
  conf_rivendell_log_machine=0;
  conf_rivendell_alert_audio_group="";
  conf_rivendell_voicetrack_groups.clear();
  conf_rivendell_user="";
  conf_rivendell_password="";
  conf_rivendell_alert_tone_cart=0;
  conf_rivendell_friendly_intro_cart=0;
  conf_rivendell_friendly_outro_cart=0;
  conf_rivendell_normalization_level=0;
  conf_rivendell_autotrim_level=0;
  conf_paths_eas_data_directory="";
  conf_paths_eas_message_extension="";
  conf_paths_rlm_receive_port=0;
}


unsigned Config::importCart(const QString &title,const QString &filename,
			    QString *err_msg)
{
  struct rd_cartimport *carts=NULL;
  unsigned numrecs=0;
  int ret=0;

  if(RD_ImportCart(&carts,
		   conf_rivendell_host_address.toString().toUtf8(),
		   conf_rivendell_user.toUtf8(),        // Rivendell User
		   conf_rivendell_password.toUtf8(),    // Rivendell Passowrd
		   "",                                  // Rivendell Ticket
		   0,                                   // Cart Number
		   0,                                   // Cut Number
		   1,                                   // Channels
		   conf_rivendell_normalization_level,  
		   conf_rivendell_autotrim_level,
		   0,                                   // Don't Use Metadata
		   1,                                   // Create new cart/cut
		   conf_rivendell_alert_audio_group.toUtf8(), // Rivendell Group
		   title.toUtf8(),                      // Cart Title
		   (conf_paths_eas_data_directory+"/"+filename).toUtf8(), // Source File
		   conf_user_agent.toUtf8(),            // User Agent String
		   &numrecs)!=0) {
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
