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

all_libraries = @all_libraries@
LDFLAGS = @LDFLAGS@

EXTRA_DIST = cal3d.rc

SOMAJOR = 0
SOMINOR = 9
SOSUBMINOR = 0

lib_LTLIBRARIES = libcal3d.la

libcal3d_la_SOURCES = animation.cpp animation_action.cpp animation_cycle.cpp bone.cpp cal3d_wrapper.cpp coreanimation.cpp corebone.cpp corekeyframe.cpp corematerial.cpp coremesh.cpp coremodel.cpp coreskeleton.cpp coresubmesh.cpp coretrack.cpp error.cpp global.cpp loader.cpp matrix.cpp mesh.cpp mixer.cpp model.cpp physique.cpp platform.cpp quaternion.cpp renderer.cpp saver.cpp skeleton.cpp springsystem.cpp submesh.cpp vector.cpp tinyxml.cpp tinyxmlerror.cpp tinyxmlparser.cpp tinystr.cpp
libcal3d_la_LDFLAGS = $(all_libraries) -no-undefined -version-info $(SOMAJOR):$(SOMINOR):$(SOSUBMINOR) $(LDFLAGS)

pkginclude_HEADERS = animation.h animation_action.h animation_cycle.h bone.h cal3d.h cal3d_wrapper.h coreanimation.h corebone.h corekeyframe.h corematerial.h coremesh.h coremodel.h coreskeleton.h coresubmesh.h coretrack.h error.h global.h loader.h matrix.h mesh.h mixer.h model.h physique.h platform.h quaternion.h renderer.h resource.h saver.h skeleton.h springsystem.h submesh.h vector.h tinyxml.h tinystr.h

# *************************************************************************

