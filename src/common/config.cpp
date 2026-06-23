// config.cpp
//
// Configuration values for easpanel
//
//   (C) Copyright 2018-2020 Fred Gleason <fredg@paravelsystems.com>
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

#include <syslog.h>
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


bool Config::startupInAuto() const
{
  return conf_startup_in_auto;
}


bool Config::raiseOnNewAlert() const
{
  return conf_raise_on_new_alert;
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


QStringList Config::rivendellAlertOnRml() const
{
  return conf_rivendell_alert_on_rml;
}


QStringList Config::rivendellAlertOffRml() const
{
  return conf_rivendell_alert_off_rml;
}


QStringList Config::rivendellAutomaticRml() const
{
  return conf_rivendell_automatic_rml;
}


QStringList Config::rivendellLiveAssistRml() const
{
  return conf_rivendell_live_assist_rml;
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


int Config::directFileQuantity() const
{
  return conf_direct_file_paths.size();
}


QString Config::directFilePath(int n) const
{
  return conf_direct_file_paths.at(n);
}


QString Config::directFileBackupDirectory(int n) const
{
  return Config::conf_direct_file_backup_directories.at(n);
}


QString Config::directFileDescription(int n) const
{
  return conf_direct_file_descriptions.at(n);
}


Config::SchedulePolicy Config::directFileSchedulePolicy(int n) const
{
  return conf_direct_file_schedule_policies.at(n);
}


unsigned Config::directFileIntroCart(int n) const
{
  return conf_direct_file_intro_carts.at(n);
}


unsigned Config::directFileOutroCart(int n) const
{
  return conf_direct_file_outro_carts.at(n);
}


int Config::easbeltQuantity() const
{
  return conf_easbelt_source_files.size();
}


QString Config::easbeltSourceFiles(int n) const
{
  return conf_easbelt_source_files.at(n);
}


QDir *Config::easbeltSourceFilesDir(int n) const
{
  return conf_easbelt_source_files_dirs.at(n);
}


QString Config::easbeltSftpIdentity(int n) const
{
  return QString(CONFIG_SFTP_IDENTITY_PATH)+"/"+
    conf_easbelt_sftp_identities.at(n);
}


QString Config::easbeltSftpUsername(int n) const
{
  return conf_easbelt_sftp_usernames.at(n);
}


QString Config::easbeltDestinationHostname(int src_n,int n) const
{
  return conf_easbelt_destination_hostnames.at(src_n).at(n);
}


QString Config::easbeltDestinationPath(int src_n,int n) const
{
  return conf_easbelt_destination_paths.at(src_n).at(n);
}


int Config::easbeltDestinationQuantity(int src_n) const
{
  return conf_easbelt_destination_hostnames.at(src_n).size();
}


QString Config::easbeltDestinationSftpIdentity(int src_n,int n) const
{
  return QString(CONFIG_SFTP_IDENTITY_PATH)+"/"+
    conf_easbelt_destination_sftp_identities.at(src_n).at(n);
}


QString Config::easbeltDestinationSftpUsername(int src_n,int n) const
{
  return conf_easbelt_destination_sftp_usernames.at(src_n).at(n);
}


QString Config::dump() const
{
  QString ret="";

  ret+="[Global]\n";
  if(startupInAuto()) {
    ret+="StartupInAuto=Yes\n";
  }
  else {
    ret+="StartupInAuto=No\n";
  }
  if(raiseOnNewAlert()) {
    ret+="RaiseOnNewAlert=Yes\n";
  }
  else {
    ret+="RaiseOnNewAlert=No\n";
  }

  ret+="\n";

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
    QString::asprintf("%u",rivendellLiveassistFriendlyIntroCart())+"\n";
  ret+="FriendlyLiveassistOutroCart="+
    QString::asprintf("%u",rivendellLiveassistFriendlyOutroCart())+"\n";
  ret+="FriendlyIntroCart="+
    QString::asprintf("%u",rivendellFriendlyIntroCart())+"\n";
  ret+="FriendlyOutroCart="+
    QString::asprintf("%u",rivendellFriendlyOutroCart())+"\n";
  ret+="NormalizationLevel="+
    QString::asprintf("%d",rivendellNormalizationLevel())+"\n";
  ret+="AutotrimLevel="+
    QString::asprintf("%d",rivendellAutotrimLevel())+"\n";
  ret+="AlertOnRml="+rivendellAlertOnRml().join("!")+"!\n";
  ret+="AlertOffRml="+rivendellAlertOffRml().join("!")+"!\n";
  ret+="AutomaticRml="+rivendellAutomaticRml().join("!")+"!\n";
  ret+="LiveAssistRml="+rivendellLiveAssistRml().join("!")+"!\n";
  ret+="\n";
  ret+="[Paths]\n";
  ret+="EasDataDirectory="+pathsEasDataDirectory()+"\n";
  ret+="EasBackupDirectory="+pathsEasBackupDirectory()+"\n";
  ret+="EasMessageExtension="+pathsEasMessageExtension()+"\n";
  ret+="RlmReceivePort="+QString::asprintf("%u",pathsRlmReceivePort())+"\n";
  ret+="\n";

  for(int i=0;i<conf_direct_file_paths.size();i++) {
    ret+=QString::asprintf("[DirectFile%d]\n",1+i);
    ret+="Path="+conf_direct_file_paths.at(i)+"\n";
    ret+="BackupDirectory="+conf_direct_file_backup_directories.at(i)+"\n";
    ret+="Description="+conf_direct_file_descriptions.at(i)+"\n";
    ret+="SchedulePolicy="+
      Config::schedulePolicyText(conf_direct_file_schedule_policies.at(i))+"\n";
    ret+=QString::asprintf("IntroCart=%u\n",conf_direct_file_intro_carts.at(i));
    ret+=QString::asprintf("OutroCart=%u\n",conf_direct_file_outro_carts.at(i));
  }

  for(int i=0;i<conf_easbelt_source_files.size();i++) {
    ret+=QString::asprintf("[Easbelt%d]\n",1+i);
    ret+="SourceFiles="+conf_easbelt_source_files.at(i)+"\n";
    ret+="SftpIdentity="+conf_easbelt_sftp_identities.at(i)+"\n";
    ret+="SftpUsername="+conf_easbelt_sftp_usernames.at(i)+"\n";
    for(int j=0;j<conf_easbelt_destination_hostnames.at(i).size();j++) {
      ret+=QString::asprintf("Destination%dHostname=",1+j)+
	conf_easbelt_destination_hostnames.at(i).at(j)+"\n";
      ret+=QString::asprintf("Destination%dPath=",1+j)+
	conf_easbelt_destination_paths.at(i).at(j)+"\n";
      ret+=QString::asprintf("Destination%dSftpIdentity=",1+j)+
	conf_easbelt_destination_sftp_identities.at(i).at(j)+"\n";
      ret+=QString::asprintf("Destination%dSftpUsername=",1+j)+
	conf_easbelt_destination_sftp_usernames.at(i).at(j)+"\n";
    }
    ret+="\n";
  }

  return ret;
}


bool Config::load()
{
  bool ret=false;
  Profile *p=new Profile();

  ret=p->setSource(CONFIG_FILE_NAME);
  if(!ret) {
    ret=p->setSource(CONFIG_FLATPAK_FILE_NAME);
  }
  conf_startup_in_auto=p->boolValue("Global","StartupInAuto",false);
  conf_raise_on_new_alert=p->boolValue("Global","RaiseOnNewAlert",true);

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
  conf_rivendell_alert_on_rml=
    RmlList(p->stringValue("Rivendell","AlertOnRml"));
  conf_rivendell_alert_off_rml=
    RmlList(p->stringValue("Rivendell","AlertOffRml"));
  conf_rivendell_automatic_rml=
    RmlList(p->stringValue("Rivendell","AutomaticRml"));
  conf_rivendell_live_assist_rml=
    RmlList(p->stringValue("Rivendell","LiveAssistRml"));

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
  // (See Table 2 in 47 CFR Part 11 Subpart A 11.31(e) for the official list)
  //
  QStringList eas_codes;
  eas_codes.push_back("EAN");  // Emergency Action Notification
  eas_codes.push_back("NIC");  // National Information Center
  eas_codes.push_back("NPT");  // National Periodic Test
  eas_codes.push_back("RMT");  // Required Monthly Test
  eas_codes.push_back("RWT");  // Required Weekly Test
  eas_codes.push_back("ADR");  // Administrative Message
  eas_codes.push_back("AVW");  // Avalanche Warning
  eas_codes.push_back("AVA");  // Avalance Watch
  eas_codes.push_back("BZW");  // Blizzard Warning
  eas_codes.push_back("BLU");  // Blue Alert
  eas_codes.push_back("CAE");  // Child Abduction Emergency
  eas_codes.push_back("CDW");  // Civil Danger Warning
  eas_codes.push_back("CEM");  // Civil Emergency Message
  eas_codes.push_back("CFW");  // Coastal Flood Warning
  eas_codes.push_back("CFA");  // Coast Flood Watch
  eas_codes.push_back("DSW");  // Dust Storm Warning
  eas_codes.push_back("EQW");  // Earthquake Warning
  eas_codes.push_back("EVI");  // Evacuation Immediate
  eas_codes.push_back("EWW");  // Extreme Wind Warning
  eas_codes.push_back("FRW");  // Fire Warning
  eas_codes.push_back("FFW");  // Flash Flood Warning
  eas_codes.push_back("FFA");  // Flash Flood Watch
  eas_codes.push_back("FFS");  // Flash Flood Statement
  eas_codes.push_back("FLW");  // Flood Warning
  eas_codes.push_back("FLA");  // Flood Watch
  eas_codes.push_back("FLS");  // Flood Statement
  eas_codes.push_back("HMW");  // Hazardous Materials Warning
  eas_codes.push_back("HWW");  // High Wind Warning
  eas_codes.push_back("HWA");  // High Wind Watch
  eas_codes.push_back("HUW");  // Hurricane Warning
  eas_codes.push_back("HUA");  // Hurricane Watch
  eas_codes.push_back("HLS");  // Hurricane Statement
  eas_codes.push_back("LEW");  // Law Enforcement Warning
  eas_codes.push_back("LAE");  // Local Area Emergency
  eas_codes.push_back("MEP");  // Missing and Endangered Persons
  eas_codes.push_back("NMN");  // Network Message Notification
  eas_codes.push_back("TOE");  // 911 Telephone Outage Emergency
  eas_codes.push_back("NUW");  // Nuclear Power Plant Warning
  eas_codes.push_back("DMO");  // Practice/Demo Warning
  eas_codes.push_back("RHW");  // Radiological Hazard Warning
  eas_codes.push_back("SVR");  // Severe Thunderstorm Warning
  eas_codes.push_back("SVA");  // Severe Thunderstorm Watch
  eas_codes.push_back("SVS");  // Severe Weather Statement
  eas_codes.push_back("SPW");  // Shelter in Place Warning
  eas_codes.push_back("SMW");  // Special Marine Warning
  eas_codes.push_back("SPS");  // Special Weather Statement
  eas_codes.push_back("SSA");  // Storm Surge Watch
  eas_codes.push_back("SSW");  // Storm Surge Warning
  eas_codes.push_back("TOR");  // Tornado Warning
  eas_codes.push_back("TOA");  // Tornado Watch
  eas_codes.push_back("TRW");  // Tropical Storm Warning
  eas_codes.push_back("TRA");  // Tropical Storm Watch
  eas_codes.push_back("TSW");  // Tsunami Warning
  eas_codes.push_back("TSA");  // Tsunami Watch
  eas_codes.push_back("VOW");  // Volcano Warning
  eas_codes.push_back("WSW");  // Winter Storm Warning
  eas_codes.push_back("WSA");  // Winter Storm Watch

  for(int i=0;i<eas_codes.size();i++) {
    conf_intro_carts[eas_codes[i]]=p->intValue("IntroCarts",eas_codes[i],-1);
    conf_outro_carts[eas_codes[i]]=p->intValue("OutroCarts",eas_codes[i],-1);

    conf_liveassist_intro_carts[eas_codes[i]]=
      p->intValue("LiveassistIntroCarts",eas_codes[i],-1);
    conf_liveassist_outro_carts[eas_codes[i]]=
      p->intValue("LiveassistOutroCarts",eas_codes[i],-1);
  }

  //
  // Direct File Processors
  //
  int count=0;
  bool ok=false;
  QString section=QString::asprintf("DirectFile%d",1+count);
  QString path=p->stringValue(section,"Path","",&ok);
  while(ok) {
    conf_direct_file_paths.push_back(path);
    conf_direct_file_backup_directories.
      push_back(p->stringValue(section,"BackupDirectory"));
    conf_direct_file_descriptions.
      push_back(p->stringValue(section,"Description",section));
    conf_direct_file_schedule_policies.
      push_back(Config::schedulePolicy(p->stringValue(section,"SchedulePolicy",
						      "next")));
    conf_direct_file_intro_carts.push_back(p->intValue(section,"IntroCart"));
    conf_direct_file_outro_carts.push_back(p->intValue(section,"OutroCart"));
    count++;
    section=QString::asprintf("DirectFile%d",1+count);
    path=p->stringValue(section,"Path","",&ok);
  }

  //
  // Eas Conveyor Belts
  //
  int src_count=0;
  int dest_count=0;
  QString source=QString::asprintf("Easbelt%d",1+src_count);
  QString source_file=p->stringValue(source,"SourceFiles","",&ok);
  while(ok) {
    conf_easbelt_source_files.push_back(source_file);
    QStringList f0=
      conf_easbelt_source_files.back().split("/",QString::KeepEmptyParts);
    QString filter=f0.last();
    f0.removeLast();
    conf_easbelt_source_files_dirs.push_back(new QDir(f0.join("/")));
    conf_easbelt_source_files_dirs.back()->
      setFilter(QDir::Files|QDir::Readable);
    conf_easbelt_source_files_dirs.back()->setSorting(QDir::Time);
    conf_easbelt_source_files_dirs.back()->setNameFilters(QStringList(filter));
    conf_easbelt_sftp_identities.
      push_back(p->stringValue(source,"SftpIdentity"));
    conf_easbelt_sftp_usernames.
      push_back(p->stringValue(source,"SftpUsername"));
    conf_easbelt_destination_hostnames.push_back(QStringList());
    conf_easbelt_destination_paths.push_back(QStringList());
    conf_easbelt_destination_sftp_identities.push_back(QStringList());
    conf_easbelt_destination_sftp_usernames.push_back(QStringList());
    QString dest_hostname=
      p->stringValue(source,
		     QString::asprintf("Destination%dHostname",1+dest_count),
		     "",&ok);
    while(ok) {
      conf_easbelt_destination_hostnames[src_count].push_back(dest_hostname);
      conf_easbelt_destination_paths[src_count].
	push_back(p->stringValue(source,
		     QString::asprintf("Destination%dPath",1+dest_count)));
      conf_easbelt_destination_sftp_identities[src_count].
	push_back(p->stringValue(source,
		     QString::asprintf("Destination%dSftpIdentity",
				       1+dest_count),
				 conf_easbelt_sftp_identities.back()));
      conf_easbelt_destination_sftp_usernames[src_count].
	push_back(p->stringValue(source,
				 QString::asprintf("Destination%dUsername",
						   1+dest_count),
				 conf_easbelt_sftp_usernames.back()));
      dest_count++;
      dest_hostname=
	p->stringValue(source,
		       QString::asprintf("Destination%dHostname",1+dest_count),
		       "",&ok);
    }
    src_count++;
    source=QString::asprintf("Easbelt%d",1+src_count);
    source_file=p->stringValue(source,"SourceFiles","",&ok);
    dest_count=0;
  }

  delete p;

  return ret;
}


void Config::clear()
{
  conf_startup_in_auto=false;
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
  conf_rivendell_alert_on_rml.clear();
  conf_rivendell_alert_off_rml.clear();
  conf_rivendell_automatic_rml.clear();
  conf_rivendell_live_assist_rml.clear();
  conf_paths_eas_data_directory="";
  conf_paths_eas_backup_directory="";
  conf_paths_eas_message_extension="";
  conf_paths_rlm_receive_port=0;
  conf_intro_carts.clear();
  conf_outro_carts.clear();
  conf_liveassist_intro_carts.clear();
  conf_liveassist_outro_carts.clear();
  conf_easbelt_source_files.clear();
  conf_easbelt_sftp_identities.clear();
  conf_easbelt_sftp_usernames.clear();
  conf_easbelt_destination_hostnames.clear();
  conf_easbelt_destination_paths.clear();
  conf_easbelt_destination_sftp_identities.clear();
  conf_easbelt_destination_sftp_usernames.clear();
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
    syslog(LOG_WARNING,"import of file %s [\"%s\"] to group %s failed: %s",
	   filename.toUtf8().constData(),
	   title.toUtf8().constData(),
	   conf_rivendell_alert_audio_group.toUtf8().constData(),
	   err_msg->toUtf8().constData());
    return 0;
  }
  if(numrecs>=1) {
    ret=carts[0].cart_number;
  }
  syslog(LOG_DEBUG,"imported file %s [\"%s\"] to cart %06u in group %s",
	 filename.toUtf8().constData(),
	 title.toUtf8().constData(),
	 ret,
	 conf_rivendell_alert_audio_group.toUtf8().constData());
  free(carts);
  return ret;
}


bool Config::removeCart(unsigned cartnum,QString *err_msg)
{
  int ret=RD_RemoveCart(conf_rivendell_host_address.toString().toUtf8(),
			conf_rivendell_user.toUtf8(),
			conf_rivendell_password.toUtf8(),
			"",cartnum,conf_user_agent.toUtf8());
  if(ret==0) {
    syslog(LOG_DEBUG,"removed cart %06u",cartnum);
  }
  else {
    syslog(LOG_WARNING,"failed to remove cart %06u [response code %d]",
	   cartnum,ret);
  }
  return ret;
}


QStringList Config::RmlList(const QString &rmlstr) const
{
  QStringList ret=rmlstr.split("!",QString::SkipEmptyParts);

  for(int i=0;i<ret.size();i++) {
    ret[i]=ret.at(i)+"!";
  }

  return ret;
}


QString Config::schedulePolicyText(Config::SchedulePolicy policy)
{
  QString ret="unknown";

  switch(policy) {
  case Config::ScheduleDisabled:
    ret="disabled";
    break;

  case Config::ScheduleImmediate:
    ret="immediate";
    break;

  case Config::ScheduleNext:
    ret="next";
    break;

  case Config::ScheduleLast:
    break;
  }

  return ret;
}


Config::SchedulePolicy Config::schedulePolicy(const QString &str)
{
  Config::SchedulePolicy ret=Config::ScheduleDisabled;

  for(int i=0;i<Config::ScheduleLast;i++) {
    Config::SchedulePolicy policy=(Config::SchedulePolicy)i;
    if(Config::schedulePolicyText(policy)==str.toLower()) {
      ret=policy;
    }
  }

  return ret;
}
