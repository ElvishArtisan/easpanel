// filedetector.cpp
//
// Scan a directory for files.
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

#include <syslog.h>
#include <unistd.h>

#include "filedetector.h"

FileInfo::FileInfo(QFileInfo info)
{
  d_file_info=info;
  d_cart_number=0;
  d_touched=true;
  d_stage=0;
  d_deletable=false;
}


QFileInfo FileInfo::fileInfo() const
{
  return d_file_info;
}


unsigned FileInfo::cartNumber() const
{
  return d_cart_number;
}


void FileInfo::setCartNumber(unsigned cartnum)
{
  d_cart_number=cartnum;
}


int FileInfo::stage() const
{
  return d_stage;
}


bool FileInfo::touched() const
{
  return d_touched;
}


bool FileInfo::touch(const QFileInfo &info)
{
  if(d_file_info.size()==info.size()) {
    if(d_stage<4) {
      d_stage++;
    }
  }
  else {
    d_file_info=info;
    d_stage=0;
  }
  d_touched=true;
  return d_stage==3;
}


void FileInfo::reset()
{
  d_touched=false;
}


bool FileInfo::isDeletable() const
{
  return d_deletable;
}


void FileInfo::makeDeletable()
{
  d_deletable=true;
}




FileDetector::FileDetector(int id,QUdpSocket *rml_sock,Config *c,QObject *parent)
{
  d_id=id;
  d_event_active=false;
  d_event_loaded=false;
  d_scanning=true;
  d_rml_socket=rml_sock;
  d_config=c;
  d_path_dir=new QDir();

  d_scan_timer=new QTimer(this);
  d_scan_timer->setSingleShot(true);
  connect(d_scan_timer,SIGNAL(timeout()),this,SLOT(scanData()));
}


FileDetector::~FileDetector()
{
  delete d_scan_timer;
  delete d_path_dir;
}


int FileDetector::id() const
{
  return d_id;
}


bool FileDetector::eventActive() const
{
  return d_event_active;
}


bool FileDetector::isScanning() const
{
  return d_scanning;
}


QString FileDetector::path() const
{
  return d_path_dir->path();
}


bool FileDetector::setPath(const QString &path)
{
  d_scan_timer->stop();

  d_file_infos.clear();
  QStringList f0=path.split("/",QString::KeepEmptyParts);
  d_path_dir->setNameFilters(QStringList(f0.last()));
  f0.removeLast();
  d_path_dir->setPath(f0.join("/"));
  d_scan_timer->start(1000);
  return d_path_dir->exists();
}


void FileDetector::playedCart(unsigned cartnum)
{
  QString err_msg;

  //
  // Event State Logic
  //
  if(d_event_loaded) {
    if((cartnum==d_config->directFileIntroCart(id()))||
       (cartnum==d_config->directFileOutroCart(id()))||
       CartIsLoaded(cartnum)) {
      if(!d_event_active) {
	d_event_active=true;
	emit eventStarted(id());
      }
    }
    else {
      if(d_event_active) {
	d_event_active=false;
	d_event_loaded=false;
	emit eventStopped(id());
      }
    }
  }

  //
  // Clean Up Used Rivendell Carts
  //
  for(QMap<QString,FileInfo *>::iterator it=d_file_infos.begin();
      it!=d_file_infos.end();it++) {
    if(it.value()->cartNumber()==cartnum) {
      it.value()->makeDeletable();
    }
    else {
      if(it.value()->isDeletable()) {
	d_config->removeCart(it.value()->cartNumber(),&err_msg);
	it.value()->setCartNumber(0);
	unlink(it.value()->fileInfo().absoluteFilePath().toUtf8());
      }
    }
  }
}


void FileDetector::startScanning()
{
  if(!d_scanning) {
    d_scanning=true;
    emit scanningStarted(id());
  }
}


void FileDetector::stopScanning()
{
  if(d_scanning) {
    d_scanning=false;
    emit scanningStopped(id());
  }
}


void FileDetector::scanData()
{
  for(QMap<QString,FileInfo *>::const_iterator it=d_file_infos.begin();
      it!=d_file_infos.end();it++) {
    it.value()->reset();
  }

  QFileInfoList infos=d_path_dir->entryInfoList(QDir::Files|QDir::Readable);
  for(int i=0;i<infos.size();i++) {
    if(d_file_infos.contains(infos.at(i).absoluteFilePath())) {
      if(d_file_infos.value(infos.at(i).absoluteFilePath())->touch(infos.at(i))) {
	emit fileAdded(d_id,infos.at(i).absoluteFilePath());
	ProcessFile(d_file_infos.value(infos.at(i).absoluteFilePath()));
      }
    }
    else {
      d_file_infos[infos.at(i).absoluteFilePath()]=new FileInfo(infos.at(i));
    }
  }

  for(QMap<QString,FileInfo *>::iterator it=d_file_infos.begin();
      (d_file_infos.size()>0)&&(it!=d_file_infos.end());it++) {
    if(!it.value()->touched()) {
      QString filepath;
      if(it.value()->stage()>=3) {
	filepath=it.value()->fileInfo().absoluteFilePath();
      }
      delete it.value();
      it=d_file_infos.erase(it);
      if(!filepath.isEmpty()) {
	emit fileRemoved(d_id,filepath);
      }
    }
  }
  d_scan_timer->start(1000);
}


void FileDetector::ProcessFile(FileInfo *info)
{
  unsigned cartnum=0;
  QString err_msg;

  if(!d_scanning) {  // Scanning suspended, throw the file away
    syslog(LOG_DEBUG,"DirectFile%d scanning suspended, throwing away file %s",
	   1+id(),info->fileInfo().absoluteFilePath().toUtf8().constData());
    unlink(info->fileInfo().absoluteFilePath().toUtf8());
    return;
  }

  //
  // Import Audio to Rivendell Cart
  //
  if((cartnum=d_config->importCart(d_config->directFileDescription(id()),
			   info->fileInfo().absoluteFilePath(),&err_msg))==0) {
    syslog(LOG_WARNING,"DirectFile%d failed to import file \"%s\" [%s]",
	   1+id(),info->fileInfo().absoluteFilePath().toUtf8().constData(),
	   err_msg.toUtf8().constData());
    info->setCartNumber(0);
    unlink(info->fileInfo().absoluteFilePath().toUtf8());
    return;
  }
  info->setCartNumber(cartnum);

  //
  // Insert Into The Log
  //
  // Outro Cart
  //
  if(d_config->directFileOutroCart(id())!=0) {
    QString rml=QString::asprintf("PX 1 %u 0 PLAY!",
				  d_config->directFileOutroCart(id()));
    d_rml_socket->writeDatagram(rml.toUtf8(),
			      d_config->rivendellHostAddress(),CONFIG_RML_PORT);
    d_event_carts.push_back(d_config->directFileOutroCart(id()));
  }

  //
  // Direct File
  //
  QString rml=QString::asprintf("PX 1 %d 0 PLAY!",cartnum);
  d_rml_socket->writeDatagram(rml.toUtf8(),
			      d_config->rivendellHostAddress(),CONFIG_RML_PORT);
  d_event_carts.push_back(cartnum);

  //
  // Intro Cart
  //
  if(d_config->directFileIntroCart(id())!=0) {
    QString rml=QString::asprintf("PX 1 %u 0 PLAY!",
				  d_config->directFileIntroCart(id()));
    d_rml_socket->writeDatagram(rml.toUtf8(),
			      d_config->rivendellHostAddress(),CONFIG_RML_PORT);
    d_event_carts.push_back(d_config->directFileIntroCart(id()));
  }
  d_event_loaded=true;

  //
  // Start Play-Out
  //
  if(d_config->directFileSchedulePolicy(id())==Config::ScheduleImmediate) {
    rml="PN 1!";
    d_rml_socket->writeDatagram(rml.toUtf8(),
			      d_config->rivendellHostAddress(),CONFIG_RML_PORT);
  }
}


 bool FileDetector::CartIsLoaded(unsigned cartnum) const
 {
   for(QMap<QString,FileInfo *>::const_iterator it=d_file_infos.begin();
       it!=d_file_infos.end();it++) {
     if(it.value()->cartNumber()==cartnum) {
       return true;
     }
   }
   return false;
 }
