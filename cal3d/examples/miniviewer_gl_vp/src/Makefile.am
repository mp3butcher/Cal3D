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

INCLUDES = $(all_includes)

bin_PROGRAMS = cal3d_miniviewer_gl_vp

cal3d_miniviewer_gl_vp_SOURCES = main.cpp tick.cpp viewer.cpp tga.cpp glew.c
cal3d_miniviewer_gl_vp_LDFLAGS = $(all_libraries) -DGLEW_STATIC

noinst_HEADERS = global.h viewer.h tick.h tga.h

# ************************************************************************

