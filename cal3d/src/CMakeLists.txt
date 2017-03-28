PROJECT(cal3d)

SET(PROJECT_INCLUDES
cal3d/animation_action.h  cal3d/cal3d.h              cal3d/corekeyframe.h       cal3d/coreskeleton.h        cal3d/error.h          cal3d/mixer.h        
cal3d/quaternion.h  cal3d/skeleton.h      cal3d/transform.h
cal3d/animation_cycle.h   cal3d/cal3d_wrapper.h      cal3d/corematerial.h       cal3d/coresubmesh.h         cal3d/global.h         cal3d/model.h                   
cal3d/refcounted.h  cal3d/springsystem.h  cal3d/vector.h
cal3d/animation.h         cal3d/calxmlbindings.h     cal3d/coremesh.h           cal3d/coresubmorphtarget.h  cal3d/hardwaremodel.h  cal3d/morphtargetmixer.h        
cal3d/refptr.h      cal3d/streamsource.h  cal3d/xmlformat.h
cal3d/animcallback.h      cal3d/coreanimatedmorph.h  cal3d/coremodel.h          cal3d/coretrack.h           cal3d/loader.h         cal3d/physiquedualquaternion.h  
cal3d/renderer.h    cal3d/submesh.h
cal3d/bone.h              cal3d/coreanimation.h      cal3d/coremorphkeyframe.h  cal3d/datasource.h          cal3d/matrix.h         cal3d/physique.h                
cal3d/resource.h    cal3d/tinybind.h
cal3d/buffersource.h      cal3d/corebone.h           cal3d/coremorphtrack.h     cal3d/dualquaternion.h      cal3d/mesh.h           cal3d/platform.h                
cal3d/saver.h      cal3d/tinyxml.h
)
 

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}    "./")

SET(PROJECT_SOURCES
#is it usefull	cal3d/calxmlbindings.cpp    
cal3d/animation_action.cpp 
cal3d/coremesh.cpp
cal3d/coresubmorphtarget.cpp  
cal3d/loader.cpp            cal3d/physique.cpp                cal3d/skeleton.cpp      cal3d/tinyxmlerror.cpp
cal3d/animation.cpp         cal3d/coreanimatedmorph.cpp  cal3d/coremodel.cpp          cal3d/coretrack.cpp          
cal3d/matrix.cpp            cal3d/physiquedualquaternion.cpp  cal3d/springsystem.cpp  cal3d/tinyxmlparser.cpp
cal3d/animation_cycle.cpp   cal3d/coreanimation.cpp      cal3d/coremorphkeyframe.cpp  cal3d/dualquaternion.cpp     
 cal3d/mesh.cpp              cal3d/platform.cpp                cal3d/streamsource.cpp  cal3d/vector.cpp
cal3d/bone.cpp              cal3d/corebone.cpp           cal3d/coremorphtrack.cpp     cal3d/error.cpp              
 cal3d/mixer.cpp             cal3d/quaternion.cpp              cal3d/submesh.cpp       cal3d/xmlformat.cpp
cal3d/buffersource.cpp      cal3d/corekeyframe.cpp       cal3d/coreskeleton.cpp       cal3d/global.cpp             
 cal3d/model.cpp             cal3d/renderer.cpp                cal3d/tinybind.cpp
cal3d/cal3d_wrapper.cpp     cal3d/corematerial.cpp       cal3d/coresubmesh.cpp        cal3d/hardwaremodel.cpp       
cal3d/morphtargetmixer.cpp  cal3d/saver.cpp                   cal3d/tinyxml.cpp
)


ADD_DEFINITIONS(-DCAL3D_EXPORTS)
ADD_LIBRARY(cal3d  SHARED 
${PROJECT_SOURCES} 
${PROJECT_INCLUDES}
)
TARGET_LINK_LIBRARIES(cal3d )
SET_TARGET_PROPERTIES(cal3d PROPERTIES PROJECT_LABEL "Cal3d Library" )

INSTALL(TARGETS cal3d  DESTINATION bin)
