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
# ADD CPP /nologo /MD /W3 /GX /Ob1 /Gy /I ".." /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "CAL3D_EXPORTS" /D "_MBCS" /FR /GF /c
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
NODEP_CPP_ANIMA=\
	".\cal3d\animation.h"\
	".\cal3d\coreanimation.h"\
	".\cal3d\error.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\animation_action.cpp
NODEP_CPP_ANIMAT=\
	".\cal3d\animation_action.h"\
	".\cal3d\coreanimation.h"\
	".\cal3d\error.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\animation_cycle.cpp
NODEP_CPP_ANIMATI=\
	".\cal3d\animation_cycle.h"\
	".\cal3d\coreanimation.h"\
	".\cal3d\error.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\bone.cpp
NODEP_CPP_BONE_=\
	".\cal3d\bone.h"\
	".\cal3d\corebone.h"\
	".\cal3d\coremesh.h"\
	".\cal3d\coremodel.h"\
	".\cal3d\coreskeleton.h"\
	".\cal3d\coresubmesh.h"\
	".\cal3d\error.h"\
	".\cal3d\skeleton.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\buffersource.cpp
NODEP_CPP_BUFFE=\
	".\cal3d\buffersource.h"\
	".\cal3d\error.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\cal3d.rc
# End Source File
# Begin Source File

SOURCE=.\cal3d_wrapper.cpp
NODEP_CPP_CAL3D=\
	".\cal3d\cal3d.h"\
	".\cal3d\cal3d_wrapper.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\coreanimation.cpp
NODEP_CPP_COREA=\
	".\cal3d\coreanimation.h"\
	".\cal3d\coretrack.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\corebone.cpp
NODEP_CPP_COREB=\
	".\cal3d\corebone.h"\
	".\cal3d\coremesh.h"\
	".\cal3d\coremodel.h"\
	".\cal3d\coreskeleton.h"\
	".\cal3d\coresubmesh.h"\
	".\cal3d\error.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\corekeyframe.cpp
NODEP_CPP_COREK=\
	".\cal3d\corekeyframe.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\corematerial.cpp
NODEP_CPP_COREM=\
	".\cal3d\corematerial.h"\
	".\cal3d\error.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\coremesh.cpp
NODEP_CPP_COREME=\
	".\cal3d\coremesh.h"\
	".\cal3d\coresubmesh.h"\
	".\cal3d\coresubmorphtarget.h"\
	".\cal3d\error.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\coremodel.cpp
NODEP_CPP_COREMO=\
	".\cal3d\coreanimation.h"\
	".\cal3d\corematerial.h"\
	".\cal3d\coremesh.h"\
	".\cal3d\coremodel.h"\
	".\cal3d\coreskeleton.h"\
	".\cal3d\error.h"\
	".\cal3d\loader.h"\
	".\cal3d\saver.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\coreskeleton.cpp
NODEP_CPP_CORES=\
	".\cal3d\corebone.h"\
	".\cal3d\coreskeleton.h"\
	".\cal3d\error.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\coresubmesh.cpp
NODEP_CPP_CORESU=\
	".\cal3d\coresubmesh.h"\
	".\cal3d\coresubmorphtarget.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\coresubmorphtarget.cpp
NODEP_CPP_CORESUB=\
	".\cal3d\coresubmorphtarget.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\coretrack.cpp
NODEP_CPP_CORET=\
	".\cal3d\corekeyframe.h"\
	".\cal3d\coretrack.h"\
	".\cal3d\error.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\error.cpp
NODEP_CPP_ERROR=\
	".\cal3d\error.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\global.cpp
NODEP_CPP_GLOBA=\
	".\cal3d\global.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\loader.cpp
NODEP_CPP_LOADE=\
	".\cal3d\buffersource.h"\
	".\cal3d\coreanimation.h"\
	".\cal3d\corebone.h"\
	".\cal3d\corekeyframe.h"\
	".\cal3d\corematerial.h"\
	".\cal3d\coremesh.h"\
	".\cal3d\coremodel.h"\
	".\cal3d\coreskeleton.h"\
	".\cal3d\coresubmesh.h"\
	".\cal3d\coretrack.h"\
	".\cal3d\error.h"\
	".\cal3d\loader.h"\
	".\cal3d\matrix.h"\
	".\cal3d\quaternion.h"\
	".\cal3d\streamsource.h"\
	".\cal3d\tinyxml.h"\
	".\cal3d\vector.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\matrix.cpp
NODEP_CPP_MATRI=\
	".\cal3d\matrix.h"\
	".\cal3d\quaternion.h"\
	".\cal3d\vector.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mesh.cpp
NODEP_CPP_MESH_=\
	".\cal3d\coremesh.h"\
	".\cal3d\coremodel.h"\
	".\cal3d\coresubmesh.h"\
	".\cal3d\error.h"\
	".\cal3d\mesh.h"\
	".\cal3d\model.h"\
	".\cal3d\morphtargetmixer.h"\
	".\cal3d\submesh.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mixer.cpp
NODEP_CPP_MIXER=\
	".\cal3d\animation.h"\
	".\cal3d\animation_action.h"\
	".\cal3d\animation_cycle.h"\
	".\cal3d\bone.h"\
	".\cal3d\coreanimation.h"\
	".\cal3d\corebone.h"\
	".\cal3d\corekeyframe.h"\
	".\cal3d\coremodel.h"\
	".\cal3d\coretrack.h"\
	".\cal3d\error.h"\
	".\cal3d\mixer.h"\
	".\cal3d\model.h"\
	".\cal3d\skeleton.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\model.cpp
NODEP_CPP_MODEL=\
	".\cal3d\bone.h"\
	".\cal3d\coremesh.h"\
	".\cal3d\coremodel.h"\
	".\cal3d\coreskeleton.h"\
	".\cal3d\coresubmesh.h"\
	".\cal3d\error.h"\
	".\cal3d\mesh.h"\
	".\cal3d\mixer.h"\
	".\cal3d\model.h"\
	".\cal3d\morphtargetmixer.h"\
	".\cal3d\physique.h"\
	".\cal3d\renderer.h"\
	".\cal3d\skeleton.h"\
	".\cal3d\springsystem.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\morphtargetmixer.cpp
NODEP_CPP_MORPH=\
	".\cal3d\coremesh.h"\
	".\cal3d\coresubmesh.h"\
	".\cal3d\error.h"\
	".\cal3d\mesh.h"\
	".\cal3d\morphtargetmixer.h"\
	".\cal3d\submesh.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\physique.cpp
NODEP_CPP_PHYSI=\
	".\cal3d\bone.h"\
	".\cal3d\coremodel.h"\
	".\cal3d\coresubmesh.h"\
	".\cal3d\coresubmorphtarget.h"\
	".\cal3d\error.h"\
	".\cal3d\mesh.h"\
	".\cal3d\model.h"\
	".\cal3d\physique.h"\
	".\cal3d\skeleton.h"\
	".\cal3d\submesh.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\platform.cpp
NODEP_CPP_PLATF=\
	".\cal3d\platform.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\quaternion.cpp
NODEP_CPP_QUATE=\
	".\cal3d\matrix.h"\
	".\cal3d\quaternion.h"\
	".\cal3d\vector.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\renderer.cpp
NODEP_CPP_RENDE=\
	".\cal3d\bone.h"\
	".\cal3d\corematerial.h"\
	".\cal3d\coremesh.h"\
	".\cal3d\coremodel.h"\
	".\cal3d\coresubmesh.h"\
	".\cal3d\error.h"\
	".\cal3d\mesh.h"\
	".\cal3d\model.h"\
	".\cal3d\physique.h"\
	".\cal3d\renderer.h"\
	".\cal3d\skeleton.h"\
	".\cal3d\submesh.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\saver.cpp
NODEP_CPP_SAVER=\
	".\cal3d\coreanimation.h"\
	".\cal3d\corebone.h"\
	".\cal3d\corekeyframe.h"\
	".\cal3d\corematerial.h"\
	".\cal3d\coremesh.h"\
	".\cal3d\coremodel.h"\
	".\cal3d\coreskeleton.h"\
	".\cal3d\coresubmesh.h"\
	".\cal3d\coretrack.h"\
	".\cal3d\error.h"\
	".\cal3d\quaternion.h"\
	".\cal3d\saver.h"\
	".\cal3d\tinyxml.h"\
	".\cal3d\vector.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\skeleton.cpp
NODEP_CPP_SKELE=\
	".\cal3d\bone.h"\
	".\cal3d\corebone.h"\
	".\cal3d\coremodel.h"\
	".\cal3d\coreskeleton.h"\
	".\cal3d\error.h"\
	".\cal3d\skeleton.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\springsystem.cpp
NODEP_CPP_SPRIN=\
	".\cal3d\coremodel.h"\
	".\cal3d\coresubmesh.h"\
	".\cal3d\error.h"\
	".\cal3d\mesh.h"\
	".\cal3d\model.h"\
	".\cal3d\springsystem.h"\
	".\cal3d\submesh.h"\
	".\cal3d\vector.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\streamsource.cpp
NODEP_CPP_STREA=\
	".\cal3d\error.h"\
	".\cal3d\platform.h"\
	".\cal3d\streamsource.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\submesh.cpp
NODEP_CPP_SUBME=\
	".\cal3d\coresubmesh.h"\
	".\cal3d\error.h"\
	".\cal3d\submesh.h"\
	".\config.h"\
	
# End Source File
# Begin Source File

SOURCE=.\tinystr.cpp
NODEP_CPP_TINYS=\
	".\cal3d\tinystr.h"\
	".\cal3d\tinyxml.h"\
	
# End Source File
# Begin Source File

SOURCE=.\tinyxml.cpp
NODEP_CPP_TINYX=\
	".\cal3d\tinyxml.h"\
	
# End Source File
# Begin Source File

SOURCE=.\tinyxmlerror.cpp
NODEP_CPP_TINYXM=\
	".\cal3d\tinyxml.h"\
	
# End Source File
# Begin Source File

SOURCE=.\tinyxmlparser.cpp
NODEP_CPP_TINYXML=\
	".\cal3d\tinyxml.h"\
	
# End Source File
# Begin Source File

SOURCE=.\vector.cpp
NODEP_CPP_VECTO=\
	".\cal3d\matrix.h"\
	".\cal3d\quaternion.h"\
	".\cal3d\vector.h"\
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
