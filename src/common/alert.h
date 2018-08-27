// alert.h
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

#ifndef ALERT_H
#define ALERT_H

#include <QDateTime>
#include <QString>

class Alert
{
 public:
  Alert();
  QString filename() const;
  QString title() const;
  QDateTime datestamp() const;
  QDateTime issuedDateTime() const;
  QDateTime expiresDateTime() const;
  QString headerAudio() const;
  QString messageAudio() const;
  QString text() const;
  void setText(const QString &str);
  QString dump() const;
  bool load(const QString &pathname);
  void clear();

 private:
  QString alert_filename;
  QString alert_title;
  QDateTime alert_datestamp;
  QDateTime alert_issued_datetime;
  QDateTime alert_expires_datetime;
  QString alert_header_audio;
  QString alert_message_audio;
  QString alert_text;
};


#endif  // ALERT_H
