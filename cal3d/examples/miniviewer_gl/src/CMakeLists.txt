PROJECT(miniviewer_gl)

 

#find_package (Threads) 

SET(PROJECT_INCLUDES
 
global.h  tga.h  tick.h  viewer.h

)
 

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}  )

INCLUDE_DIRECTORIES( ${CMAKE_HOME_DIRECTORY}/src)

SET(PROJECT_SOURCES
 
${PROJECT_INCLUDES}
main.cpp  tga.cpp  tick.cpp  viewer.cpp


)
 

ADD_DEFINITIONS(-Dx64BIT)
 
FIND_PACKAGE(GLEW REQUIRED)
 

 
IF(WIN32)

INCLUDE_DIRECTORIES(   ${CMAKE_HOME_DIRECTORY}/Glut)
ELSE(WIN32)
 
 #linux only : add GL LIBS
 SET(ADDITIONALS_LIBS  GL GLU GLEW glut )

ENDIF(WIN32)
 
#${CAL3D_LIBRARIES}
ADD_EXECUTABLE(miniviewer_gl ${PROJECT_SOURCES}   )
TARGET_LINK_LIBRARIES(miniviewer_gl cal3d  ${CMAKE_THREAD_LIBS_INIT} ${ADDITIONALS_LIBS}   
# ${THREAD_LIBRARIES}  ${GLEW_STATIC_LIBRARY} 
#glsl_optimizer
)
SET_TARGET_PROPERTIES(miniviewer_gl PROPERTIES PROJECT_LABEL "Application miniviewer_gl" )


