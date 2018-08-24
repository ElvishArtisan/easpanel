// cmdswitch.cpp
//
// Process Command-Line Switches
//
//   (C) Copyright 2012-2018 Fred Gleason <fredg@paravelsystems.com>
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

#include <stdlib.h>
#include <stdio.h>

#include <QCoreApplication>

#include "cmdswitch.h"

CmdSwitch::CmdSwitch(const char *modname,const char *usage)
{
  unsigned l=0;
  bool handled=false;

  for(int i=1;i<qApp->argc();i++) {
#ifndef WIN32
    if(!strcmp(qApp->argv()[i],"--version")) {
      printf("%s v%s\n",modname,VERSION);
      exit(0);
    }
#endif  // WIN32
    if(!strcmp(qApp->argv()[i],"--help")) {
      printf("\n%s %s\n",modname,usage);
      exit(0);
    }
    l=strlen(qApp->argv()[i]);
    handled=false;
    for(unsigned j=0;j<l;j++) {
      if(qApp->argv()[i][j]=='=') {
	switch_keys.push_back(QString(qApp->argv()[i]).left(j));
	switch_values.push_back(QString(qApp->argv()[i]).right(l-(j+1)));
	switch_processed.push_back(false);
	j=l;
	handled=true;
      }
    }
    if(!handled) {
      switch_keys.push_back(QString(qApp->argv()[i]));
      switch_values.push_back(QString(""));
      switch_processed.push_back(false);
    }
  }
}


unsigned CmdSwitch::keys() const
{
  return switch_keys.size();
}


QString CmdSwitch::key(unsigned n) const
{
  return switch_keys[n];
}


QString CmdSwitch::value(unsigned n) const
{
  return switch_values[n];
}


bool CmdSwitch::processed(unsigned n) const
{
  return switch_processed[n];
}


void CmdSwitch::setProcessed(unsigned n,bool state)
{
  switch_processed[n]=state;
}


bool CmdSwitch::allProcessed() const
{
  for(unsigned i=0;i<switch_processed.size();i++) {
    if(!switch_processed[i]) {
      return false;
    }
  }
  return true;
}
