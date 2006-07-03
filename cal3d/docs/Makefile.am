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

EXTRA_DIST = api_footer.html api_header.html cal3d.doxygen guide.dsl guide.sgml guide_apiref.sgml guide_architecture.sgml guide_classes_1.gif guide_classes_2.gif guide_classes_3.gif guide_decisions.sgml guide_gfdl.sgml guide_introduction.sgml guide_lgpl.sgml guide_phases.gif guide_pipeline.gif guide_setup.sgml guide_usage.sgml faq/cal3dfaq.doxygen \
	$(wildcard modeling/image*.jpg) \
	$(wildcard modeling/tutorial.html)

SUBDIRS = shared api

doc-api:
	doxygen cal3d.doxygen

doc-guide:
	db2html guide.sgml

clean-local:
	rm -rf guide guide.junk

# ************************************************************************

