PROJECT(cal3d_milk_exporter)

 
#ADD_DEFINITIONS(-Dx64BIT)
FIND_PACKAGE(GLUT)
#find_package (Threads) 

SET(PROJECT_INCLUDES
cal3d_milk_exporter.h  MilkInterface.h  MilkMeshNode.h
MilkBoneNode.h         MilkMaterial.h   resource.h
MilkExport.h           MilkMesh.h       StdAfx.h



)
 

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}  )
INCLUDE_DIRECTORIES(   ${GLUT_INCLUDE_PATH})
INCLUDE_DIRECTORIES( ${CMAKE_HOME_DIRECTORY}/src)

SET(PROJECT_SOURCES
 
${PROJECT_INCLUDES}
cal3d_milk_exporter.cpp  MilkExport.cpp     MilkMaterial.cpp  MilkMeshNode.cpp
MilkBoneNode.cpp         MilkInterface.cpp  MilkMesh.cpp      StdAfx.cpp

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
  
ADD_LIBRARY(cal3d_milk_exporter MODULE ${PROJECT_SOURCES}   )
TARGET_LINK_LIBRARIES(cal3d_milk_exporter cal3d  ${ADDITIONALS_LIBS}   ${GLUT_LIBRARY}   
)

SET_TARGET_PROPERTIES(cal3d_milk_exporter PROPERTIES PROJECT_LABEL "Plugin cal3d_milk_exporter" )


