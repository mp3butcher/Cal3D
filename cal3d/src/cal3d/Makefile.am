####### kdevelop will overwrite this part!!! (begin)##########
lib_LTLIBRARIES = libcal3d.la

libcal3d_la_SOURCES = springsystem.cpp physique.cpp submesh.cpp corematerial.cpp mesh.cpp renderer.cpp coresubmesh.cpp saver.cpp animation_action.cpp coremesh.cpp loader.cpp vector.cpp platform.cpp model.cpp quaternion.cpp skeleton.cpp mixer.cpp global.cpp error.cpp coretrack.cpp coreskeleton.cpp coremodel.cpp corekeyframe.cpp corebone.cpp coreanimation.cpp bone.cpp animation_cycle.cpp animation.cpp 


EXTRA_DIST = animation.cpp animation_cycle.cpp bone.cpp coreanimation.cpp corebone.cpp corekeyframe.cpp coremodel.cpp coreskeleton.cpp coretrack.cpp error.cpp global.cpp mixer.cpp skeleton.cpp quaternion.cpp model.cpp platform.cpp vector.cpp loader.cpp coremesh.cpp animation_action.cpp saver.cpp coresubmesh.cpp renderer.cpp mesh.cpp corematerial.cpp submesh.cpp cal3d.rc resource.h physique.cpp springsystem.cpp 

####### kdevelop will overwrite this part!!! (end)############
SOMAJOR = 0
SOMINOR = 0
SOSUBMINOR = 0

# here you should add all header files, which contains the functionality
# of your shared lib (published for the users)
pkginclude_HEADERS = cal3d.h animation.h animation_action.h animation_cycle.h bone.h coreanimation.h corebone.h corekeyframe.h corematerial.h coremesh.h coremodel.h coreskeleton.h coresubmesh.h coretrack.h loader.h mesh.h mixer.h model.h quaternion.h renderer.h saver.h skeleton.h submesh.h global.h platform.h vector.h error.h springsystem.h physique.h

# here you should add all header files, which shouldn
noinst_HEADERS =

libcal3d_la_LDFLAGS = $(all_libraries) -no-undefined -version-info $(SOMAJOR):$(SOMINOR):$(SOSUBMINOR) $(LDFLAGS)

# add here the libraries which have to be include
# for building the shared lib
#
#  please consider also the usage of the make environment variables
#  like
#  $(LIB_KDEUI), $(LIB_QT), $(LIB_HTML), etc.
#  instead of the hardcoded entries, which have placed
#  in here.
#  The following line is only a suggestion!
#
libcal3d_la_LIBADD = 

