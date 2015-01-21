
 
cmake_minimum_required(VERSION 2.4)
if(cross_compile)
SET(CMAKE_SYSTEM_NAME Darwin)
SET(CMAKE_C_COMPILER  /home/xeul/crosscompil/i686-apple-darwin11/bin/i686-apple-darwin11-gcc-4.7)
SET(CMAKE_CXX_COMPILER /home/xeul/crosscompil/i686-apple-darwin11/bin/i686-apple-darwin11-g++-4.7)
# where is the target environment
SET(CMAKE_FIND_ROOT_PATH   /home/xeul/crosscompil/i686-apple-darwin11 )
#/opt/eldk-2007-01-19/ppc_74xx /home/alex/eldk-ppc74xx-inst)
# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
endif()



SET(CMAKE_DEBUG_POSTFIX "d")



FILE(TO_CMAKE_PATH ${CMAKE_HOME_DIRECTORY}/CMake Cal3D_MODULE_PATH)
SET (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${Cal3D_MODULE_PATH})

FILE(TO_CMAKE_PATH ${CMAKE_HOME_DIRECTORY} Cal3D_INSTALL_PREFIX)
SET (CMAKE_INSTALL_PREFIX ${Cal3D_INSTALL_PREFIX} CACHE PATH ${Cal3D_INSTALL_PREFIX})


# output binaries directly into bin
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/bin CACHE PATH ${CMAKE_HOME_DIRECTORY}/bin)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/bin CACHE PATH ${CMAKE_HOME_DIRECTORY}/bin)



# automatically add CMAKE_CURRENT_SOURCE_DIR and CMAKE_CURRENT_BINARY_DIR to the include directories in every directory
SET(CMAKE_INCLUDE_CURRENT_DIR yes)


PROJECT(Cal3D)
#INCLUDE(CPack)


# The CMake version number.
SET(Cal3D_VERSION_MAJOR 0)
SET(Cal3D_VERSION_MINOR 110)
SET(Cal3D_VERSION_PATCH 0)
SET(Cal3D_VERSION "${Cal3D_VERSION_MAJOR}.${Cal3D_VERSION_MINOR}")
SET(Cal3D_VERSION_FULL "${Cal3D_VERSION}.${Cal3D_VERSION_PATCH}")


SUBDIRS(src examples)
 
SET(CMAKE_DEBUG_POSTFIX "d")

if(COMMAND cmake_policy)
      cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

IF(WIN32)

 
ELSE(WIN32)
SET(TIME_GETTIME_LIBRARY )
ENDIF(WIN32)
# comment following lines to disabled testing
#SUBDIRS(tests)
#FIND_PACKAGE(Cppunit REQUIRED)


SET(WALL False)
if(MSVC)
   if(WALL)
      set(WALL 1 CACHE TYPE STRING FORCE)
      set(DISABLEWALL 0 CACHE TYPE STRING FORCE)
   else(WALL)
      set(DISABLEWALL 1 CACHE TYPE STRING FORCE)
   endif(WALL)

   if(NOT DISABLEWALL)

      # Use the highest warning level
      set(CMAKE_CXX_WARNING_LEVEL 4 CACHE TYPE STRING FORCE)

      if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
         STRING(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
      else(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
         SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
      endif(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")

      if(CMAKE_C_FLAGS MATCHES "/W[0-4]")
         STRING(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
      else(CMAKE_C_FLAGS MATCHES "/W[0-4]")
         SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
      endif(CMAKE_C_FLAGS MATCHES "/W[0-4]")

      # add here warnings which should produce an error /weXXXX
      SET(MSVC_W_ERROR   "/we4101 /we4189")
      # add here warnings which should be disabled /wdXXXX
      SET(MSVC_W_DISABLE "/wd4800 /wd4996 /wd4311 /wd4312 /wd4505 /wd4267 /wd4512 /wd4245 /wd4127 /wd4251")

      set(CMAKE_CXX_FLAGS_DEBUG    "${CMAKE_CXX_FLAGS_DEBUG}         /Wp64 ${MSVC_W_ERROR} ${MSVC_W_DISABLE}")
      set(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE}       /Wp64 ${MSVC_W_ERROR} ${MSVC_W_DISABLE}")
      set(CMAKE_CXX_FLAGS_MINSIZEREL  "${CMAKE_CXX_FLAGS_MINSIZEREL} /Wp64 ${MSVC_W_ERROR} ${MSVC_W_DISABLE}")

   else(NOT DISABLEWALL)

      set(DISABLEWALL 1 CACHE TYPE STRING FORCE)
      set(CMAKE_CXX_WARNING_LEVEL 3 CACHE TYPE STRING FORCE)
      set(WARNING_LEVEL_MESSAGE "(switch to warning level 4 with -DWALL=1)")

      # add here warnings which should produce an error /weXXXX
      SET(MSVC_W_ERROR   "/we4101 /we4189")
      # add here warnings which should be disabled /wdXXXX
      SET(MSVC_W_DISABLE "/wd4800 /wd4996 /wd4267")

      set(CMAKE_CXX_FLAGS_DEBUG    "${CMAKE_CXX_FLAGS_DEBUG}         ${MSVC_W_ERROR} ${MSVC_W_DISABLE}")
      set(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE}       ${MSVC_W_ERROR} ${MSVC_W_DISABLE}")
      set(CMAKE_CXX_FLAGS_MINSIZEREL  "${CMAKE_CXX_FLAGS_MINSIZEREL} ${MSVC_W_ERROR} ${MSVC_W_DISABLE}")

   endif(NOT DISABLEWALL)

   #message("----- Warning level      : ${CMAKE_CXX_WARNING_LEVEL}  ${WARNING_LEVEL_MESSAGE}")
   #message("----- Warnings as errors : ${MSVC_W_ERROR}")
   #message("----- Warnings disabled  : ${MSVC_W_DISABLE}")
   #message("")

endif(MSVC)


IF(UNIX)
ADD_DEFINITIONS(-DSIGSLOT_PURE_ISO -fPIC -lGL)
IF(CMAKE_BUILD_TYPE STREQUAL Debug)
ADD_DEFINITIONS(-DDEBUG -D_DEBUG)
ENDIF(CMAKE_BUILD_TYPE STREQUAL Debug)
ELSE(UNIX)
#ADD_DEFINITIONS(-D_NOAUTOLIBMSG)
ENDIF(UNIX)
 
#ENABLE_TESTING()


#INCLUDE(InstallRequiredSystemLibraries)
# If the cmake version includes cpack, use it
#IF(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
SET(CPACK_PACKAGE_NAME "Cal3D")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Cal3D Library")
SET(CPACK_PACKAGE_DESCRIPTION "Cal3D: a skeleton based character animation library")

  SET(CPACK_PACKAGE_VENDOR "MpEbUtCh3r")
  SET (CPACK_PACKAGE_CONTACT "mp3butcher@hotmail.com")
  SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
  SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
   SET(CPACK_PACKAGE_VERSION "${Cal3D_VERSION_MAJOR}")
  SET(CPACK_PACKAGE_VERSION_MAJOR "${Cal3D_VERSION_MAJOR}")
  SET(CPACK_PACKAGE_VERSION_MINOR "${Cal3D_VERSION_MINOR}")
  SET(CPACK_PACKAGE_VERSION_PATCH "${Cal3D_VERSION_PATCH}")
  SET(CPACK_PACKAGE_INSTALL_DIRECTORY "Cal3D ${Cal3D_VERSION_MAJOR}.${Cal3D_VERSION_MINOR}")
  SET(CPACK_SOURCE_PACKAGE_FILE_NAME
      "Cal3D-${Cal3D_VERSION_MAJOR}.${Cal3D_VERSION_MINOR}.${Cal3D_VERSION_PATCH}")
  SET(CPACK_SOURCE_PACKAGE_FILE_NAME ${CPACK_SOURCE_PACKAGE_FILE_NAME}-MpEbUtCh3r)
  IF(NOT DEFINED CPACK_SYSTEM_NAME)
    SET(CPACK_SYSTEM_NAME ${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR})
  ENDIF(NOT DEFINED CPACK_SYSTEM_NAME)






  IF(${CPACK_SYSTEM_NAME} MATCHES Windows)
    IF(CMAKE_CL_64)
      SET(CPACK_SYSTEM_NAME win64-${CMAKE_SYSTEM_PROCESSOR})
    ELSE(CMAKE_CL_64)
      SET(CPACK_SYSTEM_NAME win32-${CMAKE_SYSTEM_PROCESSOR})
    ENDIF(CMAKE_CL_64)
  ENDIF(${CPACK_SYSTEM_NAME} MATCHES Windows)
  IF(NOT DEFINED CPACK_PACKAGE_FILE_NAME)
    SET(CPACK_PACKAGE_FILE_NAME "${CPACK_SOURCE_PACKAGE_FILE_NAME}-${CPACK_SYSTEM_NAME}")
  ENDIF(NOT DEFINED CPACK_PACKAGE_FILE_NAME)





    # install data files
  
  # FILE(GLOB fontfiles "${CMAKE_CURRENT_BINARY_DIR}/bin/times.ttf")
 INSTALL(FILES ${fontfiles}  DESTINATION .)
   
#plateform specifix RESOURCES
  
  IF(WIN32 AND NOT UNIX)


  # installer icon   SET(CPACK_NSIS_MUI_UNIICON "D:\\\\SRC\\\\GL3VolumeOctree\\\\ShortCutIcon.ico")
   #uninstaller icon    SET(CPACK_NSIS_MUI_ICON "D:\\\\SRC\\\\GL3VolumeOctree\\\\ShortCutIcon.ico")
#   NSIPRINT MessageBox MB_OK 'addnigshortcut'

   SET(CPACK_PACKAGE_ICON "${CMAKE_HOME_DIRECTORY}\\\\InstallIcon.bmp")

   #custom rules patch bugs in cpack/nsis (custom start icon+ add/remove desktop icon)
STRING(REGEX REPLACE "/" "\\\\\\\\" HOME_PROJ ${CMAKE_HOME_DIRECTORY})
 SET(CPACK_NSIS_INSTALLED_ICON_NAME "${HOME_PROJ}\\\\ShortCutIcon.ico")

set(CPACK_NSIS_CREATE_ICONS "CreateShortCut '\$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\${PROJECT_NAME}.lnk' '\$INSTDIR\\\\bin\\\\${PROJECT_NAME}.exe' parameters  '${HOME_PROJ}\\\\ShortCutIcon.ico'
    StrCmp '\$INSTALL_DESKTOP' '0' Conditionaldesktopicon
    CreateShortCut '\$desktop\\\\${PROJECT_NAME}.lnk' '\$INSTDIR\\\\bin\\\\${PROJECT_NAME}.exe' parameters  '${HOME_PROJ}\\\\ShortCutIcon.ico'
      Conditionaldesktopicon:
")
  set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "Delete '\$SMPROGRAMS\\\\$START_MENU\\\\${PROJECT_NAME}.lnk'
  StrCmp '\$INSTALL_DESKTOP' '0' Conditionaldesktopicon
  Delete '\$desktop\\\\${PROJECT_NAME}.lnk'
      Conditionaldesktopicon:
  ")

    SET(CPACK_PACKAGE_NAME "Cal3d Library")
	#
      #SET(CPACK_PACKAGE_EXECUTABLES "bin\\\\Cal3D" "Cal3D")
    #   SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\Cal3D.exe")
    # SET(CPACK_NSIS_INSTALLED_ICON_NAME "Cal3D")
      SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} Cal3d Library")
      SET(CPACK_NSIS_HELP_LINK "https:\\\\\\\\gna.org\\\\projects\\\\cal3d")
      SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\home.gna.org\\\\cal3d")
      SET(CPACK_NSIS_CONTACT "mp3butcher@hotmail.com")
      SET(CPACK_NSIS_MODIFY_PATH ON)
      #SET(CPACK_NSIS_PACKAGE_NAME "Cal3Dnsis")
        #   SET(CPACK_NSIS_PROJECT "Cal3Dnsisproj")
	#SET(CPACK_SOURCE_STRIP_FILES "src")
    FILE(GLOB dllfiles "${CMAKE_CURRENT_BINARY_DIR}/bin/Release/*.dll")
  FILE(GLOB exefiles "${CMAKE_CURRENT_BINARY_DIR}/bin/Release/*.exe")
  #FILE(GLOB datafiles "${CMAKE_CURRENT_BINARY_DIR}/data/*.obj")
 INSTALL(FILES  ${dllfiles} ${exefiles} DESTINATION bin)
  INSTALL(FILES ${datafiles} DESTINATION data)



   INSTALL(  FILES
	
	    ${CAL3D_RUNTIME_LIBRARIES_RELEASE}
	   
	     DESTINATION bin)

     
   ELSE(WIN32 AND NOT UNIX)


#INSTALL(FILES  ${exefiles} DESTINATION bin)
INSTALL(PROGRAMS bin/Cal3D DESTINATION bin)

 

FOREACH(item ${LIBSTOINSTALL})
#copy all libs in bin
#get real names

execute_process(
COMMAND readlink ${item} RESULT_VARIABLE res${item} OUTPUT_VARIABLE TotalLinkName${item} ERROR_VARIABLE fok${item}  OUTPUT_STRIP_TRAILING_WHITESPACE)
#copy link target to its real name in bin
if(TotalLinkName${item})
#its a link
set ( LinkName${item}   ${CMAKE_HOME_DIRECTORY}/bin/${TotalLinkName${item}} )
else()
#its not a link
set ( LinkName${item}  ${CMAKE_HOME_DIRECTORY}/bin/ )
endif()
#display copy trace
message("cp -L ${item} ${LinkName${item}} ")
#copy
execute_process(
COMMAND cp -L ${item} ${LinkName${item}}
                OUTPUT_VARIABLE VAROK)
ENDFOREACH(item ${LIBSTOINSTALL})

#use these copies as install stuff
FILE(GLOB dllfiles "${CMAKE_CURRENT_BINARY_DIR}/bin/*.so*")
#Waiting for    INSTALL(FILES   NAMELINK_SKIP) to work
INSTALL(FILES  ${dllfiles} DESTINATION bin NAMELINK_SKIP)

FILE(GLOB linuxlauncher "${CMAKE_CURRENT_BINARY_DIR}/Cal3D")
#Waiting for    INSTALL(FILES   NAMELINK_SKIP) to work
INSTALL(PROGRAMS  ${linuxlauncher} DESTINATION .)

 
      # There is a bug in NSI that does not handle full unix paths properly. Make
      # sure there is at least one set of four (4) backlasshes.
      #SET(CPACK_PACKAGE_ICON "${Cal3D_SOURCE_DIR}/data/Cal3D.png")
      SET(CPACK_PACKAGE_EXECUTABLES "Cal3DTest" "Test Cal3DSDK")
      SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\Release\\\\cal3d_miniviewer.exe")
      SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} a skeleton based character animation library")
      SET(CPACK_NSIS_CONTACT "mp3butcher@hotmail.com")
      SET(CPACK_NSIS_MODIFY_PATH ON)
	SET(CPACK_SOURCE_STRIP_FILES "src")
      #SET(CPACK_STRIP_FILES "bin/ccmake;bin/cmake;bin/cpack;bin/ctest")
      #SET(CPACK_SOURCE_STRIP_FILES "")
      #SET(CPACK_PACKAGE_EXECUTABLES "ccmake" "CMake")
   ENDIF(WIN32 AND NOT UNIX)

SET(CPACK_PACKAGE_EXECUTABLES "Cal3D;miniviewer_gl")
  INCLUDE(CPack)
#ENDIF(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")

MARK_AS_ADVANCED(
   EXECUTABLE_OUTPUT_PATH
   LIBRARY_OUTPUT_PATH
   WALL
)
