#*************************************************************************
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

MAINTAINERCLEANFILES = Makefile.in

INCLUDES = -I${top_srcdir}/cal3d/src

EXTRA_DIST = except

TESTS = coverage

noinst_PROGRAMS = coverage

noinst_HEADERS = Unittest.h
coverage_SOURCES = Main.cpp \
                   animation_action_test.cpp \
                   animation_cycle_test.cpp \
                   buffersource_test.cpp \
                   coremesh_test.cpp \
                   coremodel_test.cpp \
                   hardwaremodel_test.cpp \
                   loader_saver_test.cpp \
                   mixer_test.cpp \
                   model_test.cpp \
                   physique_test.cpp \
                   skeleton_test.cpp \
                   springsystem_test.cpp
                    

coverage_CPPFLAGS = @UNITTESTCPP_CFLAGS@
coverage_LDFLAGS = ../../src/cal3d/libcal3d.la @UNITTESTCPP_LIBS@

clean-local:
	-rm -fr *.gcov *.gcda *.gcno tmp.cov tmp.x*

#.PHONY: ${TESTS}
