# ************************************************************************
#
# This file is part of the Cal3D library/package
# Copyright (C) 2002 Bruno 'Beosil' Heidelberger (beosil@swileys.com)
# Copyright (C) 2006 Loic Dachary <loic@gnu.org>
#
# This library is free software; you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 2.1 of the License, or (at
# your option) any later version.
#
# ************************************************************************

AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = cal3d.dsw SConstruct \
	macosx/cal3d.xcodeproj/dereka.mode1 \
	macosx/cal3d.xcodeproj/dereka.pbxuser \
	macosx/cal3d.xcodeproj/project.pbxproj \
	macosx/English.lproj/InfoPlist.strings \
	macosx/Info.plist

MAINTAINERCLEANFILES = \
	Makefile.in \
	docs/Makefile.in \
	docs/api/Makefile.in \
	docs/shared/Makefile.in \
	src/Makefile.in \
	src/cal3d/Makefile.in \
	aclocal.m4 \
	config.guess \
	config.rpath \
	config.h.in \
	config.sub \
	configure \
	depcomp \
	install-sh \
	ltmain.sh \
	missing \
	mkinstalldirs

SUBDIRS = src docs tests

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = cal3d.pc

# ************************************************************************

