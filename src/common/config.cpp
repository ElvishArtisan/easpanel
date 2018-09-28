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


unsigned Config::rivendellLiveassistFriendlyIntroCart() const
{
  return conf_rivendell_liveassist_friendly_intro_cart;
}


unsigned Config::rivendellLiveassistFriendlyOutroCart() const
{
  return conf_rivendell_liveassist_friendly_outro_cart;
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


QString Config::pathsEasBackupDirectory() const
{
  return conf_paths_eas_backup_directory;
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


int Config::introCart(const QString &code) const
{
  int ret=conf_intro_carts.value(code,-1);

  if(ret<0) {
    ret=rivendellFriendlyIntroCart();
  }
  return ret;
}


int Config::outroCart(const QString &code) const
{
  int ret=conf_outro_carts.value(code,-1);

  if(ret<0) {
    ret=rivendellFriendlyOutroCart();
  }
  return ret;
}


int Config::liveassistIntroCart(const QString &code) const
{
  int ret=conf_liveassist_intro_carts.value(code,-1);

  if(ret<0) {
    ret=rivendellLiveassistFriendlyIntroCart();
  }
  return ret;
}


int Config::liveassistOutroCart(const QString &code) const
{
  int ret=conf_liveassist_outro_carts.value(code,-1);

  if(ret<0) {
    ret=rivendellLiveassistFriendlyOutroCart();
  }
  return ret;
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
  ret+="FriendlyLiveassistIntroCart="+
    QString().sprintf("%u",rivendellLiveassistFriendlyIntroCart())+"\n";
  ret+="FriendlyLiveassistOutroCart="+
    QString().sprintf("%u",rivendellLiveassistFriendlyOutroCart())+"\n";
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
  ret+="EasBackupDirectory="+pathsEasBackupDirectory()+"\n";
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
  conf_rivendell_liveassist_friendly_intro_cart=
    p->intValue("Rivendell","LiveassistFriendlyIntroCart");
  conf_rivendell_liveassist_friendly_outro_cart=
    p->intValue("Rivendell","LiveassistFriendlyOutroCart");
  conf_rivendell_friendly_intro_cart=
    p->intValue("Rivendell","FriendlyIntroCart");
  conf_rivendell_friendly_outro_cart=
    p->intValue("Rivendell","FriendlyOutroCart");
  conf_rivendell_normalization_level=
    p->intValue("Rivendell","NormalizationLevel",-11);
  conf_rivendell_autotrim_level=p->intValue("Rivendell","AutotrimLevel");

  conf_paths_eas_data_directory=
    p->stringValue("Paths","EasDataDirectory","/var/eas");
  conf_paths_eas_backup_directory=p->stringValue("Paths","EasBackupDirectory");
  conf_paths_eas_message_extension=
    p->stringValue("Paths","EasMessageExtension","txt");
  conf_paths_eas_message_extension_filter.clear();
  conf_paths_eas_message_extension_filter.
    push_back("*."+conf_paths_eas_message_extension);
  conf_paths_rlm_receive_port=p->intValue("Paths","RlmReceivePort",2634);

  //
  // EAS Codes
  //
  QStringList eas_codes;
  eas_codes.push_back("EAN");
  eas_codes.push_back("NPT");
  eas_codes.push_back("NIC");
  eas_codes.push_back("RMT");
  eas_codes.push_back("RWT");
  eas_codes.push_back("ADR");
  eas_codes.push_back("AVW");
  eas_codes.push_back("AVA");
  eas_codes.push_back("BZW");
  eas_codes.push_back("CAE");
  eas_codes.push_back("CDW");
  eas_codes.push_back("CEM");
  eas_codes.push_back("CFW");
  eas_codes.push_back("CFA");
  eas_codes.push_back("DMO");
  eas_codes.push_back("DSW");
  eas_codes.push_back("EQW");
  eas_codes.push_back("EWW");
  eas_codes.push_back("EVI");
  eas_codes.push_back("FRW");
  eas_codes.push_back("FFW");
  eas_codes.push_back("FFA");
  eas_codes.push_back("FFS");
  eas_codes.push_back("FLW");
  eas_codes.push_back("FLA");
  eas_codes.push_back("FLS");
  eas_codes.push_back("HMW");
  eas_codes.push_back("HWW");
  eas_codes.push_back("HWA");
  eas_codes.push_back("HUW");
  eas_codes.push_back("HUA");
  eas_codes.push_back("HLS");
  eas_codes.push_back("LEW");
  eas_codes.push_back("LAE");
  eas_codes.push_back("NMN");
  eas_codes.push_back("TOE");
  eas_codes.push_back("NUW");
  eas_codes.push_back("RHW");
  eas_codes.push_back("SVR");
  eas_codes.push_back("SVA");
  eas_codes.push_back("SVS");
  eas_codes.push_back("SPW");
  eas_codes.push_back("SMW");
  eas_codes.push_back("SPS");
  eas_codes.push_back("SSA");
  eas_codes.push_back("SSW");
  eas_codes.push_back("TOR");
  eas_codes.push_back("TOA");
  eas_codes.push_back("TRW");
  eas_codes.push_back("TRA");
  eas_codes.push_back("TSW");
  eas_codes.push_back("TSA");
  eas_codes.push_back("VOW");
  eas_codes.push_back("WSA");
  eas_codes.push_back("WSW");

  for(int i=0;i<eas_codes.size();i++) {
    conf_intro_carts[eas_codes[i]]=p->intValue("IntroCarts",eas_codes[i],-1);
    conf_outro_carts[eas_codes[i]]=p->intValue("OutroCarts",eas_codes[i],-1);

    conf_liveassist_intro_carts[eas_codes[i]]=
      p->intValue("LiveassistIntroCarts",eas_codes[i],-1);
    conf_liveassist_outro_carts[eas_codes[i]]=
      p->intValue("LiveassistOutroCarts",eas_codes[i],-1);
  }

  delete p;

  return ret;
}


void Config::clear()
{
  conf_rivendell_host_address=QHostAddress();
  conf_rivendell_alert_audio_group="";
  conf_rivendell_voicetrack_groups.clear();
  conf_rivendell_user="";
  conf_rivendell_password="";
  conf_rivendell_liveassist_friendly_intro_cart=0;
  conf_rivendell_liveassist_friendly_outro_cart=0;
  conf_rivendell_friendly_intro_cart=0;
  conf_rivendell_friendly_outro_cart=0;
  conf_rivendell_normalization_level=0;
  conf_rivendell_autotrim_level=0;
  conf_paths_eas_data_directory="";
  conf_paths_eas_backup_directory="";
  conf_paths_eas_message_extension="";
  conf_paths_rlm_receive_port=0;
  conf_intro_carts.clear();
  conf_outro_carts.clear();
  conf_liveassist_intro_carts.clear();
  conf_liveassist_outro_carts.clear();
}


unsigned Config::importCart(const QString &title,const QString &filename,
			    QString *err_msg)
{
  struct rd_cartimport *carts=NULL;
  unsigned numrecs=0;
  int ret=0;
  QString pathname=filename;
  if(pathname.left(1)!="/") {
    pathname=conf_paths_eas_data_directory+"/"+filename;
  }

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
		   pathname.toUtf8(), // Source File
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
