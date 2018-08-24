#!/bin/sh

# link_common.sh
#
#  Link common sources
#
#   (C) Copyright 2018 Fred Gleason <fredg@paravelsystems.com>
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

DESTDIR=$1

rm -f src/$DESTDIR/alert.cpp
ln -s ../../src/common/alert.cpp src/$DESTDIR/alert.cpp
rm -f src/$DESTDIR/alert.h
ln -s ../../src/common/alert.h src/$DESTDIR/alert.h

rm -f src/$DESTDIR/cmdswitch.cpp
ln -s ../../src/common/cmdswitch.cpp src/$DESTDIR/cmdswitch.cpp
rm -f src/$DESTDIR/cmdswitch.h
ln -s ../../src/common/cmdswitch.h src/$DESTDIR/cmdswitch.h

rm -f src/$DESTDIR/config.cpp
ln -s ../../src/common/config.cpp src/$DESTDIR/config.cpp
rm -f src/$DESTDIR/config.h
ln -s ../../src/common/config.h src/$DESTDIR/config.h

rm -f src/$DESTDIR/profile.cpp
ln -s ../../src/common/profile.cpp src/$DESTDIR/profile.cpp
rm -f src/$DESTDIR/profile.h
ln -s ../../src/common/profile.h src/$DESTDIR/profile.h
