# Microsoft Developer Studio Project File - Name="Cal3D" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Cal3D - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Cal3D.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Cal3D.mak" CFG="Cal3D - Win32 Release"
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
# PROP Output_Dir ".\lib"
# PROP Intermediate_Dir ".\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /I "src" /W3 /Ob1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "CAL3D_EXPORTS" /D "_MBCS" /GF /Gy /Fp".\Release/cal3d.pch" /Fo".\Release/" /Fd".\Release/" /FR /c /GX 
# ADD CPP /nologo /MD /I "src" /W3 /Ob1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "CAL3D_EXPORTS" /D "_MBCS" /GF /Gy /Fp".\Release/cal3d.pch" /Fo".\Release/" /Fd".\Release/" /FR /c /GX 
# ADD BASE MTL /nologo /D"NDEBUG" /mktyplib203 /tlb".\Release\cal3d.tlb" /win32 
# ADD MTL /nologo /D"NDEBUG" /mktyplib203 /tlb".\Release\cal3d.tlb" /win32 
# ADD BASE RSC /l 2055 /d "NDEBUG" 
# ADD RSC /l 2055 /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /dll /out:".\bin\cal3d.dll" /incremental:no /pdb:".\Release\cal3d.pdb" /pdbtype:sept /subsystem:windows /implib:".\lib/cal3d.lib" /MACHINE:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /dll /out:".\bin\cal3d.dll" /incremental:no /pdb:".\Release\cal3d.pdb" /pdbtype:sept /subsystem:windows /implib:".\lib/cal3d.lib" /MACHINE:I386

!ELSEIF  "$(CFG)" == "Cal3D - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\lib"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\lib"
# PROP Intermediate_Dir ".\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /I "src" /ZI /W3 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "CAL3D_EXPORTS" /D "_MBCS" /Fp".\Debug/cal3d.pch" /Fo".\Debug/" /Fd".\Debug/" /GZ /c /GX 
# ADD CPP /nologo /MDd /I "src" /ZI /W3 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "CAL3D_EXPORTS" /D "_MBCS" /Fp".\Debug/cal3d.pch" /Fo".\Debug/" /Fd".\Debug/" /GZ /c /GX 
# ADD BASE MTL /nologo /D"_DEBUG" /mktyplib203 /tlb".\Debug\cal3d.tlb" /win32 
# ADD MTL /nologo /D"_DEBUG" /mktyplib203 /tlb".\Debug\cal3d.tlb" /win32 
# ADD BASE RSC /l 2055 /d "_DEBUG" 
# ADD RSC /l 2055 /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /dll /out:".\bin\cal3dd.dll" /incremental:yes /debug /pdb:".\Debug\cal3d.pdb" /pdbtype:sept /subsystem:windows /implib:".\lib/cal3dd.lib" /MACHINE:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /dll /out:".\bin\cal3dd.dll" /incremental:yes /debug /pdb:".\Debug\cal3d.pdb" /pdbtype:sept /subsystem:windows /implib:".\lib/cal3dd.lib" /MACHINE:I386

!ENDIF

# Begin Target

# Name "Cal3D - Win32 Release"
# Name "Cal3D - Win32 Debug"
# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\cal3d\animation.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\animation_action.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\animation_cycle.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\bone.cpp
# End Source File
# Begin Source File

SOURCE=src\cal3d\buffersource.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\cal3d.rc

!IF  "$(CFG)" == "Cal3D - Win32 Release"

# ADD RSC /l 1033 /i "src\cal3d" 
!ELSEIF  "$(CFG)" == "Cal3D - Win32 Debug"

# ADD RSC /l 1033 /i "src\cal3d" 
!ENDIF

# End Source File
# Begin Source File

SOURCE=.\src\cal3d\cal3d_wrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\coreanimation.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\corebone.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\corekeyframe.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\corematerial.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\coremesh.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\coremodel.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\coreskeleton.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\coresubmesh.cpp
# End Source File
# Begin Source File

SOURCE=src\cal3d\coresubmorphtarget.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\coretrack.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\error.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\global.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\loader.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\matrix.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\mesh.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\mixer.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\model.cpp
# End Source File
# Begin Source File

SOURCE=src\cal3d\morphtargetmixer.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\physique.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\platform.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\quaternion.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\renderer.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\saver.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\skeleton.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\springsystem.cpp
# End Source File
# Begin Source File

SOURCE=src\cal3d\streamsource.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\submesh.cpp
# End Source File
# Begin Source File

SOURCE=src\cal3d\tinystr.cpp
# End Source File
# Begin Source File

SOURCE=src\cal3d\tinyxml.cpp
# End Source File
# Begin Source File

SOURCE=src\cal3d\tinyxmlerror.cpp
# End Source File
# Begin Source File

SOURCE=src\cal3d\tinyxmlparser.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\vector.cpp
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\src\cal3d\animation.h
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\animation_action.h
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\animation_cycle.h
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\bone.h
# End Source File
# Begin Source File

SOURCE=src\cal3d\buffersource.h
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\cal3d.h
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\cal3d_wrapper.h
# End Source File
# Begin Source File

SOURCE=.\config.h
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\coreanimation.h
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\corebone.h
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\corekeyframe.h
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\corematerial.h
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\coremesh.h
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\coremodel.h
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\coreskeleton.h
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\coresubmesh.h
# End Source File
# Begin Source File

SOURCE=src\cal3d\coresubmorphtarget.h
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\coretrack.h
# End Source File
# Begin Source File

SOURCE=src\cal3d\datasource.h
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\error.h
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\global.h
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\loader.h
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\matrix.h
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\mesh.h
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\mixer.h
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\model.h
# End Source File
# Begin Source File

SOURCE=src\cal3d\morphtargetmixer.h
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\physique.h
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\platform.h
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\quaternion.h
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\renderer.h
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\resource.h
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\saver.h
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\skeleton.h
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\springsystem.h
# End Source File
# Begin Source File

SOURCE=src\cal3d\streamsource.h
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\submesh.h
# End Source File
# Begin Source File

SOURCE=src\cal3d\tinystr.h
# End Source File
# Begin Source File

SOURCE=src\cal3d\tinyxml.h
# End Source File
# Begin Source File

SOURCE=.\src\cal3d\vector.h
# End Source File
# End Group
# Begin Group "Ressourcendateien"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\AUTHORS
# End Source File
# Begin Source File

SOURCE=.\COPYING
# End Source File
# Begin Source File

SOURCE=.\ChangeLog
# End Source File
# Begin Source File

SOURCE=.\INSTALL
# End Source File
# Begin Source File

SOURCE=.\README
# End Source File
# Begin Source File

SOURCE=.\TODO
# End Source File
# End Target
# End Project

