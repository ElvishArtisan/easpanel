// sigwatcher.h
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

#ifndef SIGWATCHER_H
#define SIGWATCHER_H

#include <signal.h>

#include <QLocalSocket>
#include <QObject>

class SigWatcher : public QObject
{
  Q_OBJECT;
 public:
  SigWatcher(QObject *parent=NULL);
  ~SigWatcher();
  QList<int> watchedSignals() const;
  void addWatchedSignal(int signum);
  void removeWatchedSignal(int signum);

 signals:
  void receivedSignal(int signum);

 private slots:
  void readyReadData();
  
 private:
  QLocalSocket *d_socket;
  QList<int> d_watched_signals;
};


#endif  // SIGWATCHER_H
