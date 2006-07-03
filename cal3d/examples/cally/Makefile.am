# ************************************************************************
#
# This file is part of the Cal3D library/package
# Copyright (C) 2002 Bruno 'Beosil' Heidelberger (beosil@swileys.com)
#
# This library is free software; you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 2.1 of the License, or (at
# your option) any later version.
#
# ************************************************************************

AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = cally.dsp \
	$(wildcard data/*.raw) \
	$(wildcard data/*.cfg) \
	$(wildcard data/cally/*.??f) \
	$(wildcard data/paladin/*.??f) \
	$(wildcard data/skeleton/*.??f) \
	ChangeLog README TODO

ACLOCAL_AMFLAGS = -I config

MAINTAINERCLEANFILES = \
	Makefile.in \
	$(wildcard config/*) \
	src/Makefile.in \
	configure \
	config.h.in \
	aclocal.m4

SUBDIRS = src

# ************************************************************************

