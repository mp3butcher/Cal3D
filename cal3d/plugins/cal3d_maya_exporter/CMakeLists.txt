PROJECT(cal3d_maya_exporter)

 
#ADD_DEFINITIONS(-Dx64BIT)
FIND_PACKAGE(GLUT)
#find_package (Threads) 

SET(PROJECT_INCLUDES
cal3d_maya_exporter.h  MayaMaterial.h  MayaNode.h  StdAfx.h
MayaInterface.h        MayaMesh.h      resource.h



)
 

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}  )
INCLUDE_DIRECTORIES(   ${GLUT_INCLUDE_PATH})
INCLUDE_DIRECTORIES( ${CMAKE_HOME_DIRECTORY}/src)

SET(PROJECT_SOURCES
 
${PROJECT_INCLUDES}
cal3d_maya_exporter.cpp  MayaInterface.cpp  MayaMesh.cpp  StdAfx.cpp
cal3dExportCmd.cpp       MayaMaterial.cpp   MayaNode.cpp




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
  
ADD_LIBRARY(cal3d_maya_exporter MODULE ${PROJECT_SOURCES}   )
TARGET_LINK_LIBRARIES(cal3d_maya_exporter cal3d  ${ADDITIONALS_LIBS}   ${GLUT_LIBRARY}   
)

SET_TARGET_PROPERTIES(cal3d_maya_exporter PROPERTIES PROJECT_LABEL "Plugin cal3d_maya_exporter" )


