## Makefile.am
##
## Top level Makefile.am
##
##   (C) Copyright 2018-2026 Fred Gleason <fredg@paravelsystems.com>
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
## Use automake to process this into a Makefile.in

#if DOCBOOK_AM
#  DOCBOOK_AM_OPT = docs
#endif

SUBDIRS = helpers\
          debian\
          conf\
          deps\
          icons\
          snd\
          src\
          systemd\
          xdg

maintainer-clean-local:
	rm -rf .flatpak-builder
	rm -rf easpanel-@VERSION@
	rm -rf flatpak
	rm -rf m4
	rm -rf repo

flatpak:	dist
	./make_flatpak.sh

flatpak-post:
	./post_flatpak.sh

deb:	dist
	./build_debs.sh

rpm:	dist
	cp easpanel-$(VERSION).tar.gz $(HOME)/rpmbuild/SOURCES/
	$(RPMBUILD) -ba --target $(ARCH)-$(VENDOR)-linux easpanel.spec
	mv $(HOME)/rpmbuild/RPMS/$(ARCH)/easpanel-*.rpm $(top_srcdir)/
	mv $(HOME)/rpmbuild/SRPMS/easpanel-*.src.rpm $(top_srcdir)/
	rm $(HOME)/rpmbuild/SOURCES/easpanel-$(VERSION).tar.gz
	rm -rf $(HOME)/rpmbuild/BUILD/easpanel-$(VERSION)

EXTRA_DIST = easpanel.spec.in\
             autogen.sh\
             build_debs.sh.in\
             get_distro.pl\
             get_target.sh\
             link_common.sh\
             make_flatpak.sh\
             PACKAGE_VERSION\
             post_flatpak.sh

CLEANFILES = *~\
             *.buildinfo\
             *.changes\
             *.deb\
             *.ddeb\
             *.flatpak\
             *.mo\
             *.rpm\
             *.tar.gz\
             *.tgz\
             *.zip

DISTCLEANFILES = build_debs.sh\
                 easpanel.spec\
                 config.guess\
                 config.sub\
                 ltmain.sh

MAINTAINERCLEANFILES = *~\
                       *.dsc\
                       *.tar.gz\
                       *.tar.xz\
                       aclocal.m4\
                       compile\
                       config.guess\
                       config.status\
                       config.sub\
                       configure\
                       depcomp\
                       install-sh\
                       libtool\
                       ltmain.sh\
                       Makefile.in\
                       missing\
                       mkinstalldirs
