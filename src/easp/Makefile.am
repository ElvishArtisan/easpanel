## Makefile.am
##
## Makefile for easp(1)
##
## (C) Copyright 2018-2026 Fred Gleason <fredg@paravelsystems.com>
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

AM_CPPFLAGS = -Wall -DPREFIX=\"$(prefix)\" -std=c++11 -fPIC -DARCH=\"@ARCH@\" -Wno-strict-aliasing @QT5GUI_CFLAGS@ @RIVWEBCAPI_CFLAGS@

# The dependency for qt's Meta Object Compiler (moc)
moc_%.cpp:	%.h
	@QT5_MOC@ $< -o $@


bin_PROGRAMS = directp\
               easp

dist_directp_SOURCES = directfilewidget.cpp directfilewidget.h\
                       directp.cpp directp.h\
                       filedetector.cpp filedetector.h\
                       modebutton.cpp modebutton.h

nodist_directp_SOURCES = cmdswitch.cpp cmdswitch.h\
	                 config.cpp config.h\
                         moc_directfilewidget.cpp\
                         moc_directp.cpp\
                         moc_filedetector.cpp\
                         moc_modebutton.cpp\
	                 profile.cpp profile.h

dist_easp_SOURCES = alertbutton.cpp alertbutton.h\
                    alerttextedit.cpp alerttextedit.h\
                    directfilewidget.cpp directfilewidget.h\
                    easp.cpp easp.h\
                    filedetector.cpp filedetector.h\
                    modebutton.cpp modebutton.h\
                    alert.cpp alert.h\
	            cmdswitch.cpp cmdswitch.h\
	            config.cpp config.h\
	            profile.cpp profile.h

nodist_easp_SOURCES = moc_alertbutton.cpp\
                      moc_alerttextedit.cpp\
                      moc_directfilewidget.cpp\
                      moc_easp.cpp\
                      moc_filedetector.cpp\
                      moc_modebutton.cpp

directp_LDADD = @QT5GUI_LIBS@ @RIVWEBCAPI_LIBS@

easp_LDADD = @QT5GUI_LIBS@ @RIVWEBCAPI_LIBS@

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
