# ************************************************************************
#
# This file is part of the Cal3D library/package
# Copyright (C) 2006 Loic Dachary <loic@gnu.org>
#
# This library is free software; you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 2.1 of the License, or (at
# your option) any later version.
#
# ************************************************************************

if COVERAGE
  COVERAGE_SUBDIR = coverage
else
  COVERAGE_SUBDIR =
endif

SUBDIRS = ${COVERAGE_SUBDIR}

MAINTAINERCLEANFILES = Makefile.in

INCLUDES = -I${top_srcdir}/cal3d/src

EXTRA_DIST = \
	$(wildcard cal3d_converter/base.??f)

TESTS_ENVIRONMENT = sh ./run
TESTS = converter/skeleton converter/mesh converter/material converter/animation quaternion

noinst_PROGRAMS = quaternion

quaternion_SOURCES = quaternion.cpp
quaternion_LDFLAGS = ../src/cal3d/libcal3d.la

.PHONY: ${TESTS}
