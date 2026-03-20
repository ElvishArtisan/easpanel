// belttask.cpp
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

#include "belttask.h"

BeltTask::BeltTask(QProcess *proc,int src_n,int dst_n,
		   const QString &src_filename,const QString &command)
{
  d_process=proc;
  d_source_id=src_n;
  d_destination_id=dst_n;
  d_source_filename=src_filename;
  d_command=command;
  d_log_id=QString::asprintf("%d:%d:",1+src_n,1+dst_n)+
    src_filename.split("/",QString::KeepEmptyParts).last();
}


QProcess *BeltTask::process() const
{
  return d_process;
}


int BeltTask::sourceId() const
{
  return d_source_id;
}


int BeltTask::destinationId() const
{
  return d_destination_id;
}


QString BeltTask::sourceFilename() const
{
  return d_source_filename;
}


QString BeltTask::command() const
{
  return d_command;
}


QString BeltTask::logId() const
{
  return d_log_id;
}
