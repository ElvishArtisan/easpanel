// belttask.h
//
// Process state tracker for easbelt(1)
//
//   (C) Copyright 2026 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef BELTTASK_H
#define BELTTASK_H

#include <QProcess>

class BeltTask
{
 public:
  BeltTask(QProcess *,int src_n,int dst_n,const QString &src_filename,
	   const QString &command);
  QProcess *process() const;
  int sourceId() const;
  int destinationId() const;
  QString sourceFilename() const;
  QString command() const;
  QString logId() const;

 private:
  QProcess *d_process;
  int d_source_id;
  int d_destination_id;
  QString d_source_filename;
  QString d_command;
  QString d_log_id;
};


#endif  // BELTTASK_H
