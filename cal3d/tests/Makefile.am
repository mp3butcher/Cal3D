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
MAINTAINERCLEANFILES = Makefile.in

EXTRA_DIST = \
	$(wildcard cal3d_converter/base.??f)

TESTS_ENVIRONMENT = sh ./run
TESTS = converter/skeleton converter/mesh converter/material converter/animation

.PHONY: ${TESTS}
