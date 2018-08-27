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

#include <QString>

#define CONFIG_FILE_NAME QString("/etc/easpanel.conf")

class Config
{
 public:
  Config();
  QString rivendellHostname() const;
  QString rivendellAlertAudioGroup() const;
  QString rivendellUser() const;
  QString rivendellPassword() const;
  unsigned rivendellAlertToneCart() const;
  unsigned rivendellEomCart() const;
  unsigned rivendellFriendlyIntroCart() const;
  unsigned rivendellFriendlyOutroCart() const;
  QString pathsEasMessages() const;
  QString pathsEasAudio() const;
  QString pathsRivendellAudio() const;
  QString dump() const;
  bool load();
  void clear();
  unsigned importCart(const QString &title,const QString &filename,
		      QString *err_msg);
  bool removeCart(unsigned cartnum,QString *err_msg);

 private:
  QString conf_rivendell_hostname;
  QString conf_rivendell_alert_audio_group;
  QString conf_rivendell_user;
  QString conf_rivendell_password;
  unsigned conf_rivendell_alert_tone_cart;
  unsigned conf_rivendell_eom_cart;
  unsigned conf_rivendell_friendly_intro_cart;
  unsigned conf_rivendell_friendly_outro_cart;
  QString conf_paths_eas_messages;
  QString conf_paths_eas_audio;
  QString conf_paths_rivendell_audio;
  QString conf_user_agent;
};


#endif  // CONFIG_H
