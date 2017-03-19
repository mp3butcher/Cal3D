PROJECT(cal3d_max_exporter)

 
#ADD_DEFINITIONS(-Dx64BIT)
FIND_PACKAGE(GLUT)
#find_package (Threads) 

SET(PROJECT_INCLUDES
 cal3d_max_exporter.h      MaxMeshExportDesc.h
max2ogl.h                 MaxMorphAnimationExport.h
MaxAnimationExport.h      MaxMorphAnimationExportDesc.h
MaxAnimationExportDesc.h  MaxNode.h
MaxInterface.h            MaxNullView.h
MaxMaterial.h             MaxSkeletonExport.h
MaxMaterialExport.h       MaxSkeletonExportDesc.h
MaxMaterialExportDesc.h   resource.h
MaxMesh.h                 StdAfx.h
MaxMeshExport.h
 

)
 

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}  )
INCLUDE_DIRECTORIES(   ${GLUT_INCLUDE_PATH})
INCLUDE_DIRECTORIES( ${CMAKE_HOME_DIRECTORY}/src)

SET(PROJECT_SOURCES
 
${PROJECT_INCLUDES}

cal3d_max_exporter.cpp               MaxMeshExportDesc.cpp
max2ogl.cpp                          MaxMeshExportMaxscriptCall.cpp
MaxAnimationExport.cpp               MaxMorphAnimationExport.cpp
MaxAnimationExportDesc.cpp           MaxMorphAnimationExportDesc.cpp
MaxAnimationExportMaxscriptCall.cpp  MaxMorphAnimationExportMaxscriptCall.cpp
MaxInterface.cpp                     MaxNode.cpp
MaxMaterial.cpp                      MaxNullView.cpp
MaxMaterialExport.cpp                MaxSkeletonExport.cpp
MaxMaterialExportDesc.cpp            MaxSkeletonExportDesc.cpp
MaxMaterialExportMaxscriptCall.cpp   MaxSkeletonExportMaxscriptCall.cpp
MaxMesh.cpp                          StdAfx.cpp
MaxMeshExport.cpp


)
 

 
#FIND_PACKAGE(GLEW REQUIRED)
 

 
IF(WIN32)

INCLUDE_DIRECTORIES(   ${CMAKE_HOME_DIRECTORY}/Glut)
ELSE(WIN32)
 
 #linux only : add GL LIBS
 SET(ADDITIONALS_LIBS  
 GL 
 GLU 
pthread
 )

ENDIF(WIN32)
  
ADD_LIBRARY(cal3d_max_exporter MODULE ${PROJECT_SOURCES}   )
TARGET_LINK_LIBRARIES(cal3d_max_exporter cal3d  ${ADDITIONALS_LIBS}   ${GLUT_LIBRARY}   
)

SET_TARGET_PROPERTIES(cal3d_max_exporter PROPERTIES PROJECT_LABEL "Plugin cal3d_max_exporter" )


