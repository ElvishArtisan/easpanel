## automake.am
##
## Makefile for tests
##
## (C) Copyright 2020 Fred Gleason <fredg@paravelsystems.com>
##
##   This program is free software; you can redistribute it and/or modify
##   it under the terms of the GNU General Public License version 2 as
##   published by the Free Software Foundation.
##
##   This program is distributed in the hope that it will be useful,
##   but WITHOUT ANY WARRANTY; without even the implied warranty of
##   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##   GNU General Public License for more details.
##
##   You should have received a copy of the GNU General Public
##   License along with this program; if not, write to the Free Software
##   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
##
##
## Use automake to process this into a Makefile.in

AM_CPPFLAGS = -Wall -DPREFIX=\"$(prefix)\" -DARCH=\"@ARCH@\" -Wno-strict-aliasing @QT4_CFLAGS@ @RIVWEBCAPI_CFLAGS@
MOC = @QT4_MOC@

# The dependency for qt's Meta Object Compiler (moc)
moc_%.cpp:	%.h
	@MOC@ $< -o $@


noinst_PROGRAMS = test_alert_manifest

dist_test_alert_manifest_SOURCES = test_alert_manifest.cpp test_alert_manifest.h

nodist_test_alert_manifest_SOURCES = alert.cpp alert.h\
                                     cmdswitch.cpp cmdswitch.h\
                                     config.cpp config.h\
                                     moc_test_alert_manifest.cpp\
                                     profile.cpp profile.h

test_alert_manifest_LDADD = @GUILIBS@ @RIVWEBCAPI_LIBS@
test_alert_manifest_LDFLAGS = @GUIFLAGS@

CLEANFILES = *~\
             moc_*\
             *.obj\
             *.idb\
             *.pdb\
             *ilk

DISTCLEANFILES = alert.cpp alert.h\
                 cmdswitch.cpp cmdswitch.h\
                 config.cpp config.h\
                 profile.cpp profile.h

MAINTAINERCLEANFILES = *~\
                       Makefile.in
