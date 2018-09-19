// config.h
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

#ifndef CONFIG_H
#define CONFIG_H

#include <QHostAddress>
#include <QString>
#include <QStringList>

#define CONFIG_FILE_NAME QString("/etc/easpanel.conf")

class Config
{
 public:
  Config();
  QHostAddress rivendellHostAddress() const;
  int rivendellLogMachine() const;
  QString rivendellAlertAudioGroup() const;
  QStringList rivendellVoicetrackGroups() const;
  QString rivendellUser() const;
  QString rivendellPassword() const;
  unsigned rivendellLiveassistFriendlyIntroCart() const;
  unsigned rivendellLiveassistFriendlyOutroCart() const;
  unsigned rivendellFriendlyIntroCart() const;
  unsigned rivendellFriendlyOutroCart() const;
  int rivendellNormalizationLevel() const;
  int rivendellAutotrimLevel() const;
  QString pathsEasDataDirectory() const;
  QString pathsEasMessageExtension() const;
  QStringList pathsEasMessageExtensionFilter();
  unsigned pathsRlmReceivePort() const;
  QString dump() const;
  bool load();
  void clear();
  unsigned importCart(const QString &title,const QString &filename,
		      QString *err_msg);
  bool removeCart(unsigned cartnum,QString *err_msg);

 private:
  QHostAddress conf_rivendell_host_address;
  int conf_rivendell_log_machine;
  QString conf_rivendell_alert_audio_group;
  QStringList conf_rivendell_voicetrack_groups;
  QString conf_rivendell_user;
  QString conf_rivendell_password;
  unsigned conf_rivendell_liveassist_friendly_intro_cart;
  unsigned conf_rivendell_liveassist_friendly_outro_cart;
  unsigned conf_rivendell_friendly_intro_cart;
  unsigned conf_rivendell_friendly_outro_cart;
  int conf_rivendell_normalization_level;
  int conf_rivendell_autotrim_level;
  QString conf_paths_eas_data_directory;
  QString conf_paths_eas_message_extension;
  QStringList conf_paths_eas_message_extension_filter;
  unsigned conf_paths_rlm_receive_port;
  QString conf_user_agent;
};


#endif  // CONFIG_H
