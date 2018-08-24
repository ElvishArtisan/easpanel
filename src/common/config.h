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
  QString rdairplayHostname() const;
  QString pathsEasMessages() const;
  QString pathsEasAudio() const;
  QString pathsRivendellAudio() const;
  unsigned cartsAlertTone() const;
  unsigned cartsEom() const;
  unsigned cartsFriendlyIntro() const;
  unsigned cartsFriendlyOutro() const;
  unsigned cartsAlertHeaderFirst() const;
  unsigned cartsAlertHeaderLast() const;
  unsigned cartsAlertMessageFirst() const;
  unsigned cartsAlertMessageLast() const;
  bool load();
  void clear();

 private:
  QString conf_rdairplay_hostname;
  QString conf_paths_eas_messages;
  QString conf_paths_eas_audio;
  QString conf_paths_rivendell_audio;
  unsigned conf_carts_alert_tone;
  unsigned conf_carts_eom;
  unsigned conf_carts_friendly_intro;
  unsigned conf_carts_friendly_outro;
  unsigned conf_carts_alert_header_first;
  unsigned conf_carts_alert_header_last;
  unsigned conf_carts_alert_message_first;
  unsigned conf_carts_alert_message_last;
};


#endif  // CONFIG_H
