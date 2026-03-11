#!/bin/sh

# link_common.sh
#
#  Link/Unlink common sources
#
#   (C) Copyright 2018-2026 Fred Gleason <fredg@paravelsystems.com>
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License version 2 as
#   published by the Free Software Foundation.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public
#   License along with this program; if not, write to the Free Software
#   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#

if test $# -ne 2 ; then
    echo "link_common.sh <src-dir> link|unlink"
    exit 1
fi

DESTDIR=$1
OPERATION=$2

rm -f src/$DESTDIR/alert.cpp
rm -f src/$DESTDIR/alert.h
if test $OPERATION = "link" ; then
  ln -s ../../src/common/alert.cpp src/$DESTDIR/alert.cpp
  ln -s ../../src/common/alert.h src/$DESTDIR/alert.h
fi

rm -f src/$DESTDIR/cmdswitch.cpp
rm -f src/$DESTDIR/cmdswitch.h
if test $OPERATION = "link" ; then
  ln -s ../../src/common/cmdswitch.cpp src/$DESTDIR/cmdswitch.cpp
  ln -s ../../src/common/cmdswitch.h src/$DESTDIR/cmdswitch.h
fi

rm -f src/$DESTDIR/config.cpp
rm -f src/$DESTDIR/config.h
if test $OPERATION = "link" ; then
  ln -s ../../src/common/config.cpp src/$DESTDIR/config.cpp
  ln -s ../../src/common/config.h src/$DESTDIR/config.h
fi

rm -f src/$DESTDIR/profile.cpp
rm -f src/$DESTDIR/profile.h
if test $OPERATION = "link" ; then
  ln -s ../../src/common/profile.cpp src/$DESTDIR/profile.cpp
  ln -s ../../src/common/profile.h src/$DESTDIR/profile.h
fi
