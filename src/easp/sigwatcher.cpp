// sigwatcher.cpp
//
// Generate a Qt signal upon reception of a POSIX signal
//
// (C) Copyright 2026 Fred Gleason <fredg@paravelsystems.com>
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of version 2.1 of the GNU Lesser General Public
//    License as published by the Free Software Foundation;
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, 
//    Boston, MA  02111-1307  USA
//

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "sigwatcher.h"

int __sigwatcher_socket;
void __SigWatcher_Callback(int signum)
{
  char data[1]={0xff&signum};

  send(__sigwatcher_socket,data,1,0);
}


SigWatcher::SigWatcher(QObject *parent)
{
  int socks[2];

  if(socketpair(AF_UNIX,SOCK_STREAM,0,socks)!=0) {
    fprintf(stderr,"SigWatcher::failed to create socket pair [%s]\n",
	    strerror(errno));
    ::exit(1);
  }
  __sigwatcher_socket=socks[0];
  d_socket=new QLocalSocket(this);
  connect(d_socket,SIGNAL(readyRead()),this,SLOT(readyReadData()));
  d_socket->setSocketDescriptor(socks[1]);

  //  ::signal(SIGINT,__SigWatcher_Callback);
  //  ::signal(SIGTERM,__SigWatcher_Callback);
}


SigWatcher::~SigWatcher()
{
  ::signal(SIGINT,SIG_DFL);
  ::signal(SIGTERM,SIG_DFL);
  delete d_socket;
}


QList<int> SigWatcher::watchedSignals() const
{
  return d_watched_signals;
}


void SigWatcher::addWatchedSignal(int signum)
{
  if(!d_watched_signals.contains(signum)) {
    ::signal(signum,__SigWatcher_Callback);
    d_watched_signals.push_back(signum);
  }
}


void SigWatcher::removeWatchedSignal(int signum)
{
  if(d_watched_signals.contains(signum)) {
    ::signal(signum,SIG_DFL);
    d_watched_signals.removeAll(signum);
  }
}


void SigWatcher::readyReadData()
{
  QByteArray data;

  data=d_socket->readAll();
  for(int i=0;i<data.size();i++) {
    emit receivedSignal(data.at(i));
  }
}
