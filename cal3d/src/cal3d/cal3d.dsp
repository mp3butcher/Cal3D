# Microsoft Developer Studio Project File - Name="Cal3D" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Cal3D - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cal3d.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cal3d.mak" CFG="Cal3D - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Cal3D - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Cal3D - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Cal3D - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\lib"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\bin\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /Ob1 /Gy /I "src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "CAL3D_EXPORTS" /D "_MBCS" /FR /GF /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".." /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "CAL3D_EXPORTS" /D "_MBCS" /GF /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x807 /d "NDEBUG"
# ADD RSC /l 0x807 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /pdb:".\Release\cal3d.pdb" /machine:I386 /out:".\bin\cal3d.dll" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /pdb:".\Release\cal3d.pdb" /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Cal3D - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\lib"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\bin\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /GX /ZI /Od /I "src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "CAL3D_EXPORTS" /D "_MBCS" /GZ /c
# ADD CPP /nologo /MDd /W3 /GX /ZI /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "CAL3D_EXPORTS" /D "_MBCS" /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x807 /d "_DEBUG"
# ADD RSC /l 0x807 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /pdb:".\Debug\cal3d.pdb" /debug /machine:I386 /out:".\bin\cal3dd.dll" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /pdb:".\Debug\cal3d.pdb" /debug /machine:I386 /out:"..\..\bin\Debug\cal3d_d.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Cal3D - Win32 Release"
# Name "Cal3D - Win32 Debug"
# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\animation.cpp
DEP_CPP_ANIMA=\
	".\animation.h"\
	".\animcallback.h"\
	".\coreanimation.h"\
	".\error.h"\
	".\global.h"\
	".\platform.h"\
	
NODEP_CPP_ANIMA=\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\animation_action.cpp
DEP_CPP_ANIMAT=\
	".\animation.h"\
	".\animation_action.h"\
	".\coreanimation.h"\
	".\error.h"\
	".\global.h"\
	".\platform.h"\
	
NODEP_CPP_ANIMAT=\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\animation_cycle.cpp
DEP_CPP_ANIMATI=\
	".\animation.h"\
	".\animation_cycle.h"\
	".\coreanimation.h"\
	".\error.h"\
	".\global.h"\
	".\platform.h"\
	
NODEP_CPP_ANIMATI=\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\bone.cpp
DEP_CPP_BONE_=\
	".\bone.h"\
	".\corebone.h"\
	".\coremesh.h"\
	".\coremodel.h"\
	".\coreskeleton.h"\
	".\coresubmesh.h"\
	".\error.h"\
	".\global.h"\
	".\matrix.h"\
	".\platform.h"\
	".\quaternion.h"\
	".\skeleton.h"\
	".\vector.h"\
	
NODEP_CPP_BONE_=\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\buffersource.cpp
DEP_CPP_BUFFE=\
	".\buffersource.h"\
	".\datasource.h"\
	".\error.h"\
	".\global.h"\
	".\platform.h"\
	
NODEP_CPP_BUFFE=\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\cal3d.rc
# End Source File
# Begin Source File

SOURCE=.\cal3d_wrapper.cpp
DEP_CPP_CAL3D=\
	".\animation.h"\
	".\animation_action.h"\
	".\animation_cycle.h"\
	".\bone.h"\
	".\cal3d.h"\
	".\cal3d_wrapper.h"\
	".\coreanimation.h"\
	".\corebone.h"\
	".\corekeyframe.h"\
	".\corematerial.h"\
	".\coremesh.h"\
	".\coremodel.h"\
	".\coremorphanimation.h"\
	".\coreskeleton.h"\
	".\coresubmesh.h"\
	".\coresubmorphtarget.h"\
	".\coretrack.h"\
	".\datasource.h"\
	".\error.h"\
	".\global.h"\
	".\hardwaremodel.h"\
	".\loader.h"\
	".\matrix.h"\
	".\mesh.h"\
	".\mixer.h"\
	".\model.h"\
	".\morphtargetmixer.h"\
	".\physique.h"\
	".\platform.h"\
	".\quaternion.h"\
	".\renderer.h"\
	".\saver.h"\
	".\skeleton.h"\
	".\springsystem.h"\
	".\streamsource.h"\
	".\submesh.h"\
	".\vector.h"\
	
NODEP_CPP_CAL3D=\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\coreanimation.cpp
DEP_CPP_COREA=\
	".\coreanimation.h"\
	".\coretrack.h"\
	".\global.h"\
	".\matrix.h"\
	".\platform.h"\
	".\quaternion.h"\
	".\vector.h"\
	
NODEP_CPP_COREA=\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\corebone.cpp
DEP_CPP_COREB=\
	".\corebone.h"\
	".\coremesh.h"\
	".\coremodel.h"\
	".\coreskeleton.h"\
	".\coresubmesh.h"\
	".\error.h"\
	".\global.h"\
	".\matrix.h"\
	".\platform.h"\
	".\quaternion.h"\
	".\vector.h"\
	
NODEP_CPP_COREB=\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\corekeyframe.cpp
DEP_CPP_COREK=\
	".\corekeyframe.h"\
	".\global.h"\
	".\matrix.h"\
	".\platform.h"\
	".\quaternion.h"\
	".\vector.h"\
	
NODEP_CPP_COREK=\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\corematerial.cpp
DEP_CPP_COREM=\
	".\corematerial.h"\
	".\error.h"\
	".\global.h"\
	".\platform.h"\
	
NODEP_CPP_COREM=\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\coremesh.cpp
DEP_CPP_COREME=\
	".\coremesh.h"\
	".\coresubmesh.h"\
	".\coresubmorphtarget.h"\
	".\error.h"\
	".\global.h"\
	".\matrix.h"\
	".\platform.h"\
	".\vector.h"\
	
NODEP_CPP_COREME=\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\coremodel.cpp
DEP_CPP_COREMO=\
	".\coreanimation.h"\
	".\corematerial.h"\
	".\coremesh.h"\
	".\coremodel.h"\
	".\coremorphanimation.h"\
	".\coreskeleton.h"\
	".\datasource.h"\
	".\error.h"\
	".\global.h"\
	".\loader.h"\
	".\platform.h"\
	".\saver.h"\
	
NODEP_CPP_COREMO=\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\coremorphanimation.cpp
DEP_CPP_COREMOR=\
	".\coremorphanimation.h"\
	".\global.h"\
	".\platform.h"\
	
NODEP_CPP_COREMOR=\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\coreskeleton.cpp
DEP_CPP_CORES=\
	".\corebone.h"\
	".\coreskeleton.h"\
	".\error.h"\
	".\global.h"\
	".\matrix.h"\
	".\platform.h"\
	".\quaternion.h"\
	".\vector.h"\
	
NODEP_CPP_CORES=\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\coresubmesh.cpp
DEP_CPP_CORESU=\
	".\coresubmesh.h"\
	".\coresubmorphtarget.h"\
	".\global.h"\
	".\matrix.h"\
	".\platform.h"\
	".\vector.h"\
	
NODEP_CPP_CORESU=\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\coresubmorphtarget.cpp
DEP_CPP_CORESUB=\
	".\coresubmorphtarget.h"\
	".\global.h"\
	".\matrix.h"\
	".\platform.h"\
	".\vector.h"\
	
NODEP_CPP_CORESUB=\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\coretrack.cpp
DEP_CPP_CORET=\
	".\corekeyframe.h"\
	".\coretrack.h"\
	".\error.h"\
	".\global.h"\
	".\matrix.h"\
	".\platform.h"\
	".\quaternion.h"\
	".\vector.h"\
	
NODEP_CPP_CORET=\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\error.cpp
DEP_CPP_ERROR=\
	".\error.h"\
	".\global.h"\
	".\platform.h"\
	
NODEP_CPP_ERROR=\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\global.cpp
DEP_CPP_GLOBA=\
	".\global.h"\
	".\platform.h"\
	
NODEP_CPP_GLOBA=\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\hardwaremodel.cpp
DEP_CPP_HARDW=\
	".\bone.h"\
	".\corebone.h"\
	".\corematerial.h"\
	".\coremesh.h"\
	".\coremodel.h"\
	".\coreskeleton.h"\
	".\coresubmesh.h"\
	".\error.h"\
	".\global.h"\
	".\hardwaremodel.h"\
	".\matrix.h"\
	".\model.h"\
	".\platform.h"\
	".\quaternion.h"\
	".\skeleton.h"\
	".\vector.h"\
	
NODEP_CPP_HARDW=\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\loader.cpp
DEP_CPP_LOADE=\
	".\buffersource.h"\
	".\coreanimation.h"\
	".\corebone.h"\
	".\corekeyframe.h"\
	".\corematerial.h"\
	".\coremesh.h"\
	".\coremodel.h"\
	".\coreskeleton.h"\
	".\coresubmesh.h"\
	".\coretrack.h"\
	".\datasource.h"\
	".\error.h"\
	".\global.h"\
	".\loader.h"\
	".\matrix.h"\
	".\platform.h"\
	".\quaternion.h"\
	".\streamsource.h"\
	".\tinystr.h"\
	".\tinyxml.h"\
	".\vector.h"\
	
NODEP_CPP_LOADE=\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\matrix.cpp
DEP_CPP_MATRI=\
	".\global.h"\
	".\matrix.h"\
	".\platform.h"\
	".\quaternion.h"\
	".\vector.h"\
	
NODEP_CPP_MATRI=\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mesh.cpp
DEP_CPP_MESH_=\
	".\coremesh.h"\
	".\coremodel.h"\
	".\coresubmesh.h"\
	".\error.h"\
	".\global.h"\
	".\matrix.h"\
	".\mesh.h"\
	".\model.h"\
	".\platform.h"\
	".\submesh.h"\
	".\vector.h"\
	
NODEP_CPP_MESH_=\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mixer.cpp
DEP_CPP_MIXER=\
	".\animation.h"\
	".\animation_action.h"\
	".\animation_cycle.h"\
	".\bone.h"\
	".\coreanimation.h"\
	".\corebone.h"\
	".\corekeyframe.h"\
	".\coremodel.h"\
	".\coretrack.h"\
	".\error.h"\
	".\global.h"\
	".\matrix.h"\
	".\mixer.h"\
	".\model.h"\
	".\platform.h"\
	".\quaternion.h"\
	".\skeleton.h"\
	".\vector.h"\
	
NODEP_CPP_MIXER=\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\model.cpp
DEP_CPP_MODEL=\
	".\bone.h"\
	".\corebone.h"\
	".\coremesh.h"\
	".\coremodel.h"\
	".\coreskeleton.h"\
	".\coresubmesh.h"\
	".\error.h"\
	".\global.h"\
	".\matrix.h"\
	".\mesh.h"\
	".\mixer.h"\
	".\model.h"\
	".\morphtargetmixer.h"\
	".\physique.h"\
	".\platform.h"\
	".\quaternion.h"\
	".\renderer.h"\
	".\skeleton.h"\
	".\springsystem.h"\
	".\vector.h"\
	
NODEP_CPP_MODEL=\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\morphtargetmixer.cpp
DEP_CPP_MORPH=\
	".\coremesh.h"\
	".\coremodel.h"\
	".\coremorphanimation.h"\
	".\coresubmesh.h"\
	".\error.h"\
	".\global.h"\
	".\matrix.h"\
	".\mesh.h"\
	".\model.h"\
	".\morphtargetmixer.h"\
	".\platform.h"\
	".\submesh.h"\
	".\vector.h"\
	
NODEP_CPP_MORPH=\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\physique.cpp
DEP_CPP_PHYSI=\
	".\bone.h"\
	".\corebone.h"\
	".\coremodel.h"\
	".\coresubmesh.h"\
	".\coresubmorphtarget.h"\
	".\error.h"\
	".\global.h"\
	".\matrix.h"\
	".\mesh.h"\
	".\model.h"\
	".\physique.h"\
	".\platform.h"\
	".\quaternion.h"\
	".\skeleton.h"\
	".\submesh.h"\
	".\vector.h"\
	
NODEP_CPP_PHYSI=\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\platform.cpp
DEP_CPP_PLATF=\
	".\platform.h"\
	
NODEP_CPP_PLATF=\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\quaternion.cpp
DEP_CPP_QUATE=\
	".\global.h"\
	".\matrix.h"\
	".\platform.h"\
	".\quaternion.h"\
	".\vector.h"\
	
NODEP_CPP_QUATE=\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\renderer.cpp
DEP_CPP_RENDE=\
	".\bone.h"\
	".\corebone.h"\
	".\corematerial.h"\
	".\coremesh.h"\
	".\coremodel.h"\
	".\coresubmesh.h"\
	".\error.h"\
	".\global.h"\
	".\matrix.h"\
	".\mesh.h"\
	".\model.h"\
	".\physique.h"\
	".\platform.h"\
	".\quaternion.h"\
	".\renderer.h"\
	".\skeleton.h"\
	".\submesh.h"\
	".\vector.h"\
	
NODEP_CPP_RENDE=\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\saver.cpp
DEP_CPP_SAVER=\
	".\coreanimation.h"\
	".\corebone.h"\
	".\corekeyframe.h"\
	".\corematerial.h"\
	".\coremesh.h"\
	".\coremodel.h"\
	".\coreskeleton.h"\
	".\coresubmesh.h"\
	".\coretrack.h"\
	".\error.h"\
	".\global.h"\
	".\matrix.h"\
	".\platform.h"\
	".\quaternion.h"\
	".\saver.h"\
	".\tinystr.h"\
	".\tinyxml.h"\
	".\vector.h"\
	
NODEP_CPP_SAVER=\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\skeleton.cpp
DEP_CPP_SKELE=\
	".\bone.h"\
	".\corebone.h"\
	".\coremodel.h"\
	".\coreskeleton.h"\
	".\error.h"\
	".\global.h"\
	".\matrix.h"\
	".\platform.h"\
	".\quaternion.h"\
	".\skeleton.h"\
	".\vector.h"\
	
NODEP_CPP_SKELE=\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\springsystem.cpp
DEP_CPP_SPRIN=\
	".\bone.h"\
	".\corebone.h"\
	".\coremodel.h"\
	".\coresubmesh.h"\
	".\error.h"\
	".\global.h"\
	".\matrix.h"\
	".\mesh.h"\
	".\model.h"\
	".\platform.h"\
	".\quaternion.h"\
	".\skeleton.h"\
	".\springsystem.h"\
	".\submesh.h"\
	".\vector.h"\
	
NODEP_CPP_SPRIN=\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\streamsource.cpp
DEP_CPP_STREA=\
	".\datasource.h"\
	".\error.h"\
	".\global.h"\
	".\platform.h"\
	".\streamsource.h"\
	
NODEP_CPP_STREA=\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\submesh.cpp
DEP_CPP_SUBME=\
	".\coresubmesh.h"\
	".\error.h"\
	".\global.h"\
	".\matrix.h"\
	".\platform.h"\
	".\submesh.h"\
	".\vector.h"\
	
NODEP_CPP_SUBME=\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\tinystr.cpp
DEP_CPP_TINYS=\
	".\platform.h"\
	".\tinystr.h"\
	".\tinyxml.h"\
	
# End Source File
# Begin Source File

SOURCE=.\tinyxml.cpp
DEP_CPP_TINYX=\
	".\platform.h"\
	".\tinystr.h"\
	".\tinyxml.h"\
	
# End Source File
# Begin Source File

SOURCE=.\tinyxmlerror.cpp
DEP_CPP_TINYXM=\
	".\platform.h"\
	".\tinystr.h"\
	".\tinyxml.h"\
	
# End Source File
# Begin Source File

SOURCE=.\tinyxmlparser.cpp
DEP_CPP_TINYXML=\
	".\platform.h"\
	".\tinystr.h"\
	".\tinyxml.h"\
	
# End Source File
# Begin Source File

SOURCE=.\vector.cpp
DEP_CPP_VECTO=\
	".\global.h"\
	".\matrix.h"\
	".\platform.h"\
	".\quaternion.h"\
	".\vector.h"\
	
NODEP_CPP_VECTO=\
	".\config.h"\
	
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\animation.h
# End Source File
# Begin Source File

SOURCE=.\animation_action.h
# End Source File
# Begin Source File

SOURCE=.\animation_cycle.h
# End Source File
# Begin Source File

SOURCE=.\animcallback.h
# End Source File
# Begin Source File

SOURCE=.\bone.h
# End Source File
# Begin Source File

SOURCE=.\buffersource.h
# End Source File
# Begin Source File

SOURCE=.\cal3d.h
# End Source File
# Begin Source File

SOURCE=.\cal3d_wrapper.h
# End Source File
# Begin Source File

SOURCE=.\coreanimation.h
# End Source File
# Begin Source File

SOURCE=.\corebone.h
# End Source File
# Begin Source File

SOURCE=.\corekeyframe.h
# End Source File
# Begin Source File

SOURCE=.\corematerial.h
# End Source File
# Begin Source File

SOURCE=.\coremesh.h
# End Source File
# Begin Source File

SOURCE=.\coremodel.h
# End Source File
# Begin Source File

SOURCE=.\coremorphanimation.h
# End Source File
# Begin Source File

SOURCE=.\coreskeleton.h
# End Source File
# Begin Source File

SOURCE=.\coresubmesh.h
# End Source File
# Begin Source File

SOURCE=.\coresubmorphtarget.h
# End Source File
# Begin Source File

SOURCE=.\coretrack.h
# End Source File
# Begin Source File

SOURCE=.\datasource.h
# End Source File
# Begin Source File

SOURCE=.\error.h
# End Source File
# Begin Source File

SOURCE=.\global.h
# End Source File
# Begin Source File

SOURCE=.\hardwaremodel.h
# End Source File
# Begin Source File

SOURCE=.\loader.h
# End Source File
# Begin Source File

SOURCE=.\matrix.h
# End Source File
# Begin Source File

SOURCE=.\mesh.h
# End Source File
# Begin Source File

SOURCE=.\mixer.h
# End Source File
# Begin Source File

SOURCE=.\model.h
# End Source File
# Begin Source File

SOURCE=.\morphtargetmixer.h
# End Source File
# Begin Source File

SOURCE=.\physique.h
# End Source File
# Begin Source File

SOURCE=.\platform.h
# End Source File
# Begin Source File

SOURCE=.\quaternion.h
# End Source File
# Begin Source File

SOURCE=.\renderer.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\saver.h
# End Source File
# Begin Source File

SOURCE=.\skeleton.h
# End Source File
# Begin Source File

SOURCE=.\springsystem.h
# End Source File
# Begin Source File

SOURCE=.\streamsource.h
# End Source File
# Begin Source File

SOURCE=.\submesh.h
# End Source File
# Begin Source File

SOURCE=.\tinystr.h
# End Source File
# Begin Source File

SOURCE=.\tinyxml.h
# End Source File
# Begin Source File

SOURCE=.\vector.h
# End Source File
# End Group
# Begin Group "Ressourcendateien"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=..\..\AUTHORS
# End Source File
# Begin Source File

SOURCE=..\..\ChangeLog
# End Source File
# Begin Source File

SOURCE=..\..\COPYING
# End Source File
# Begin Source File

SOURCE=..\..\INSTALL
# End Source File
# Begin Source File

SOURCE=..\..\README
# End Source File
# Begin Source File

SOURCE=..\..\TODO
# End Source File
# End Target
# End Project
