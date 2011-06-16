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

INCLUDES = -I$(top_srcdir)/src

EXTRA_DIST = cal3d.rc cal3d.dsp SConscript

lib_LTLIBRARIES = libcal3d.la

#
# Updating this number must *not* match the cal3d version. It must
# comply with the following rules (see the libtool documentation for
# more information):
#
#   The -version-info flag accepts an argument of the form
#`CURRENT[:REVISION[:AGE]]'.  So, passing `-version-info 10:0:0' sets
#CURRENT to 10, REVISION to 0, and AGE to 0.
#
# Note that AGE must be less than or equal to the CURRENT interface number.
#
#  1. Start with version information of `10:0:0' for each libtool library.
#
#  2. Update the version information only immediately before a public
#     release of your software.  More frequent updates are unnecessary,
#     and only guarantee that the current interface number gets larger
#     faster.
#
#  3. If the library source code has changed at all since the last
#     update, then increment REVISION (`C:R:A' becomes `C:r+1:A').
#
#  4. If any interfaces have been added, removed, or changed since the
#     last update, increment CURRENT, and set REVISION to 0.
#
#  5. If any interfaces have been added since the last public release,
#     then increment AGE.
#
#  6. If any interfaces have been removed since the last public release,
#     then set AGE to 0.
# 
#
VERSION_INFO = 12:0:0

libcal3d_la_SOURCES = \
	animation.cpp \
	animation_action.cpp \
	animation_cycle.cpp \
	bone.cpp \
	buffersource.cpp \
	cal3d_wrapper.cpp \
	coreanimatedmorph.cpp \
	coreanimation.cpp \
	corebone.cpp \
	corekeyframe.cpp \
	corematerial.cpp \
	coremesh.cpp \
	coremodel.cpp \
	coremorphkeyframe.cpp \
	coremorphtrack.cpp \
	coreskeleton.cpp \
	coresubmesh.cpp \
	coresubmorphtarget.cpp \
	coretrack.cpp \
	error.cpp \
	global.cpp \
	hardwaremodel.cpp \
	loader.cpp \
	matrix.cpp \
	mesh.cpp \
	mixer.cpp \
	model.cpp \
	morphtargetmixer.cpp \
	physique.cpp \
	physiquedualquaternion.cpp \
	dualquaternion.cpp \
	platform.cpp \
	quaternion.cpp \
	renderer.cpp \
	saver.cpp \
	skeleton.cpp \
	springsystem.cpp \
	streamsource.cpp \
	submesh.cpp \
	vector.cpp \
	tinyxml.cpp \
	tinyxmlerror.cpp \
	tinyxmlparser.cpp \
	xmlformat.cpp

libcal3d_la_LDFLAGS = -no-undefined -version-info $(VERSION_INFO) 

pkginclude_HEADERS = \
	animation.h \
	animation_action.h \
	animation_cycle.h \
	animcallback.h \
	bone.h \
	buffersource.h \
	cal3d.h \
	cal3d_wrapper.h \
	coreanimatedmorph.h \
	coreanimation.h \
	corebone.h \
	corekeyframe.h \
	corematerial.h \
	coremesh.h \
	coremodel.h \
	coremorphkeyframe.h \
	coremorphtrack.h \
	coreskeleton.h \
	coresubmesh.h \
	coresubmorphtarget.h \
	coretrack.h \
	datasource.h \
	error.h \
	global.h \
	hardwaremodel.h \
	loader.h \
	matrix.h \
	mesh.h \
	mixer.h \
	model.h \
	morphtargetmixer.h \
	physique.h \
	physiquedualquaternion.h \
	dualquaternion.h \
	platform.h \
	quaternion.h \
	refcounted.h \
	refptr.h \
	renderer.h \
	resource.h \
	saver.h \
	skeleton.h \
	springsystem.h \
	streamsource.h \
	submesh.h \
	vector.h \
	tinyxml.h \
	transform.h \
	xmlformat.h


# *************************************************************************

