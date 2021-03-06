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
  int touches() const;
  void touch();
  QString easType() const;
  QString title() const;
  QDateTime datestamp() const;
  QDateTime issuedDateTime() const;
  QDateTime expiresDateTime() const;
  QString headerAudio() const;
  QString eomAudio() const;
  QString attentionAudio() const;
  QString messageAudio() const;
  QString text() const;
  void setText(const QString &str);
  unsigned headerCart() const;
  void setHeaderCart(unsigned cartnum);
  unsigned eomCart() const;
  void setEomCart(unsigned cartnum);
  unsigned attentionCart() const;
  void setAttentionCart(unsigned cartnum);
  unsigned messageCart() const;
  void setMessageCart(unsigned cartnum);
  QString dump() const;
  bool load(const QString &pathname);
  void clear();

 private:
  int UtcOffset() const;
  QString alert_filename;
  int alert_touches;
  QString alert_eas_type;
  QString alert_title;
  QDateTime alert_datestamp;
  QDateTime alert_issued_datetime;
  QDateTime alert_expires_datetime;
  QString alert_header_audio;
  QString alert_eom_audio;
  QString alert_attention_audio;
  QString alert_message_audio;
  QString alert_text;
  unsigned alert_header_cart;
  unsigned alert_attention_cart;
  unsigned alert_eom_cart;
  unsigned alert_message_cart;
};


#endif  // ALERT_H
