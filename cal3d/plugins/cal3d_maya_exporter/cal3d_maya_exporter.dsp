# Microsoft Developer Studio Project File - Name="cal3d_maya_exporter" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=cal3d_maya_exporter - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cal3d_maya_exporter.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cal3d_maya_exporter.mak" CFG="cal3d_maya_exporter - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cal3d_maya_exporter - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "cal3d_maya_exporter - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "cal3d_maya_exporter - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "." /I "../src" /I "../src/win32" /I "../../src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_BOOL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc09 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Foundation.lib OpenMaya.lib OpenMayaAnim.lib /nologo /subsystem:windows /dll /machine:I386 /out:"Release\cal3dExport.mll" /libpath:"..\..\Release" /export:initializePlugin /export:uninitializePlugin

!ELSEIF  "$(CFG)" == "cal3d_maya_exporter - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "." /I "../src" /I "../src/win32" /I "../../src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_BOOL" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc09 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Foundation.lib OpenMaya.lib OpenMayaAnim.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"Debug\cal3dExport.mll" /pdbtype:sept /libpath:"..\..\Debug" /export:initializePlugin /export:uninitializePlugin

!ENDIF 

# Begin Target

# Name "cal3d_maya_exporter - Win32 Release"
# Name "cal3d_maya_exporter - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\src\res\cal3d.bmp
# End Source File
# Begin Source File

SOURCE=..\src\res\cal3d_bar.bmp
# End Source File
# Begin Source File

SOURCE=..\src\res\type_bone.ico
# End Source File
# Begin Source File

SOURCE=..\src\res\type_dummy.ico
# End Source File
# Begin Source File

SOURCE=..\src\res\type_other.ico
# End Source File
# End Group
# Begin Group "Win32"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\win32\AnimationExportSheet.cpp
# End Source File
# Begin Source File

SOURCE=..\src\win32\AnimationTimePage.cpp
# End Source File
# Begin Source File

SOURCE=..\src\win32\BoneAssignmentPage.cpp
# End Source File
# Begin Source File

SOURCE=..\src\win32\LevelOfDetailPage.cpp
# End Source File
# Begin Source File

SOURCE=..\src\win32\MaterialExportSheet.cpp
# End Source File
# Begin Source File

SOURCE=..\src\win32\MaterialMapDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\src\win32\MaterialMapDialog.h
# End Source File
# Begin Source File

SOURCE=..\src\win32\MaterialMapsPage.cpp
# End Source File
# Begin Source File

SOURCE=..\src\win32\MaterialSelectionPage.cpp
# End Source File
# Begin Source File

SOURCE=..\src\win32\MeshExportSheet.cpp
# End Source File
# Begin Source File

SOURCE=..\src\win32\SkeletonExportSheet.cpp
# End Source File
# Begin Source File

SOURCE=..\src\win32\SkeletonFilePage.cpp
# End Source File
# Begin Source File

SOURCE=..\src\win32\SkeletonHierarchyPage.cpp
# End Source File
# Begin Source File

SOURCE=..\src\win32\SpringSystemPage.cpp
# End Source File
# End Group
# Begin Group "ExporterFramework"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\BaseInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\src\BaseInterface.h
# End Source File
# Begin Source File

SOURCE=..\src\BaseMaterial.cpp
# End Source File
# Begin Source File

SOURCE=..\src\BaseMaterial.h
# End Source File
# Begin Source File

SOURCE=..\src\BaseMesh.cpp
# End Source File
# Begin Source File

SOURCE=..\src\BaseMesh.h
# End Source File
# Begin Source File

SOURCE=..\src\BaseNode.cpp
# End Source File
# Begin Source File

SOURCE=..\src\BaseNode.h
# End Source File
# Begin Source File

SOURCE=..\src\BoneCandidate.cpp
# End Source File
# Begin Source File

SOURCE=..\src\BoneCandidate.h
# End Source File
# Begin Source File

SOURCE=.\cal3d_maya_exporter.rc
# End Source File
# Begin Source File

SOURCE=..\src\Exporter.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Exporter.h
# End Source File
# Begin Source File

SOURCE=..\src\Lodder.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Lodder.h
# End Source File
# Begin Source File

SOURCE=..\src\MaterialCandidate.cpp
# End Source File
# Begin Source File

SOURCE=..\src\MaterialCandidate.h
# End Source File
# Begin Source File

SOURCE=..\src\MaterialLibraryCandidate.cpp
# End Source File
# Begin Source File

SOURCE=..\src\MaterialLibraryCandidate.h
# End Source File
# Begin Source File

SOURCE=..\src\MeshCandidate.cpp
# End Source File
# Begin Source File

SOURCE=..\src\MeshCandidate.h
# End Source File
# Begin Source File

SOURCE=..\src\Quadric.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Quadric.h
# End Source File
# Begin Source File

SOURCE=..\src\SkeletonCandidate.cpp
# End Source File
# Begin Source File

SOURCE=..\src\SkeletonCandidate.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\src\SubmeshCandidate.cpp
# End Source File
# Begin Source File

SOURCE=..\src\SubmeshCandidate.h
# End Source File
# Begin Source File

SOURCE=..\src\VertexCandidate.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VertexCandidate.h
# End Source File
# End Group
# Begin Group "MayaExporter"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cal3d_maya_exporter.cpp
# End Source File
# Begin Source File

SOURCE=.\cal3d_maya_exporter.h
# End Source File
# Begin Source File

SOURCE=.\MayaInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\MayaInterface.h
# End Source File
# Begin Source File

SOURCE=.\MayaMaterial.cpp
# End Source File
# Begin Source File

SOURCE=.\MayaMaterial.h
# End Source File
# Begin Source File

SOURCE=.\MayaMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\MayaMesh.h
# End Source File
# Begin Source File

SOURCE=.\MayaNode.cpp
# End Source File
# Begin Source File

SOURCE=.\MayaNode.h
# End Source File
# End Group
# End Target
# End Project
