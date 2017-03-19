PROJECT(cal3d_max_importer)

 
#ADD_DEFINITIONS(-Dx64BIT)
FIND_PACKAGE(GLUT)
#find_package (Threads) 

SET(PROJECT_INCLUDES
bones.h               MaxAnimationImport.h      MaxSkeletonImport.h
cal3d_max_importer.h  MaxAnimationImportDesc.h  MaxSkeletonImportDesc.h


)
 

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}  )
INCLUDE_DIRECTORIES(   ${GLUT_INCLUDE_PATH})
INCLUDE_DIRECTORIES( ${CMAKE_HOME_DIRECTORY}/src)

SET(PROJECT_SOURCES
 
${PROJECT_INCLUDES}

bones.cpp               MaxAnimationImport.cpp      MaxSkeletonImport.cpp
cal3d_max_importer.cpp  MaxAnimationImportDesc.cpp  MaxSkeletonImportDesc.cpp



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
  
ADD_LIBRARY(cal3d_max_importer MODULE ${PROJECT_SOURCES}   )
TARGET_LINK_LIBRARIES(cal3d_max_importer cal3d  ${ADDITIONALS_LIBS}   ${GLUT_LIBRARY}   
)

SET_TARGET_PROPERTIES(cal3d_max_importer PROPERTIES PROJECT_LABEL "Plugin cal3d_max_importer" )


