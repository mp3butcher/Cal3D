PROJECT(CallyDemo)

 
#ADD_DEFINITIONS(-Dx64BIT)
FIND_PACKAGE(GLUT)
#find_package (Threads) 

SET(PROJECT_INCLUDES
 
global.h  tga.h  tick.h 
demo.h menu.h model.h


)
 

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}  )
INCLUDE_DIRECTORIES(   ${GLUT_INCLUDE_PATH})
INCLUDE_DIRECTORIES( ${CMAKE_HOME_DIRECTORY}/src)

SET(PROJECT_SOURCES
 
${PROJECT_INCLUDES}
main.cpp  tga.cpp  tick.cpp 
demo.cpp menu.cpp model.cpp

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
  
ADD_EXECUTABLE(CallyDemo ${PROJECT_SOURCES}   )
TARGET_LINK_LIBRARIES(CallyDemo cal3d  ${ADDITIONALS_LIBS}   ${GLUT_LIBRARY}   
)

SET_TARGET_PROPERTIES(CallyDemo PROPERTIES PROJECT_LABEL "Demo Cally" )


