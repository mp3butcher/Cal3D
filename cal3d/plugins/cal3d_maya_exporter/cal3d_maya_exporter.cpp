//----------------------------------------------------------------------------//
// cal3d_maya_exporter.cpp                                                     //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger, Thomas Cowell        //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//
#include "StdAfx.h"
#include "cal3d_maya_exporter.h"

#include "Exporter.h"
#include "MayaInterface.h"

#include <maya/MFnPlugin.h>
#include <maya/MPxFileTranslator.h>
#include <maya/MGlobal.h>


//----------------------------------------------------------------------------//
// The one and only DLL instance                                              //
//----------------------------------------------------------------------------//
CMayaExporterApp theDll;


//----------------------------------------------------------------------------//
// cal3d animation exporter                                                   //
//----------------------------------------------------------------------------//
class cal3dAnimationExport : public MPxFileTranslator 
{
public:
    static void *creator (void) { return new cal3dAnimationExport; }

    MStatus reader (const MFileObject& file, const MString& optionsString, MPxFileTranslator::FileAccessMode mode)
    {
        return MS::kFailure;
    }

    MStatus writer (const MFileObject& file, const MString& optionsString, MPxFileTranslator::FileAccessMode mode)
    {
        AFX_MANAGE_STATE(AfxGetStaticModuleState());
        
        CMayaInterface mayaInterface;

        if (!mayaInterface.Create (mode == MPxFileTranslator::kExportActiveAccessMode))
        {
            MGlobal::displayError("Failed to initialize cal3d CMayaInterface.");
            return MS::kFailure;
        }

        // create an exporter instance
        if(!theExporter.Create(&mayaInterface))
        {
            MGlobal::displayError(theExporter.GetLastError().c_str());
            return MS::kFailure;
        }

        if (!theExporter.ExportAnimation (file.fullName().asChar()))
        {
            MGlobal::displayError(theExporter.GetLastError().c_str());
            return MS::kFailure;
        }
        
        return MS::kSuccess;
    }

    bool haveReadMethod (void) const { return false; }
    bool haveWriteMethod (void) const { return true; }
    
    virtual MString defaultExtension (void) const { return MString("xaf"); }
    
    bool canBeOpened (void) const { return true; }
    
    MFileKind identifyFile (const MFileObject& fileName, const char *buffer, short size) const
    {
        return  MFileKind::kNotMyFileType;
    }
};


//----------------------------------------------------------------------------//
// cal3d skeleton exporter                                                        //
//----------------------------------------------------------------------------//
class cal3dSkeletonExport : public MPxFileTranslator 
{
public:
    static void *creator (void) { return new cal3dSkeletonExport; }

    MStatus reader (const MFileObject& file, const MString& optionsString, MPxFileTranslator::FileAccessMode mode)
    {
        return MS::kFailure;
    }

    MStatus writer (const MFileObject& file, const MString& optionsString, MPxFileTranslator::FileAccessMode mode)
    {
        AFX_MANAGE_STATE(AfxGetStaticModuleState());
        
        CMayaInterface mayaInterface;

        if (!mayaInterface.Create (mode == MPxFileTranslator::kExportActiveAccessMode))
        {
            MGlobal::displayError("Failed to initialize cal3d CMayaInterface.");
            return MS::kFailure;
        }

        // create an exporter instance
        if(!theExporter.Create(&mayaInterface))
        {
            MGlobal::displayError(theExporter.GetLastError().c_str());
            return MS::kFailure;
        }

        if (!theExporter.ExportSkeleton (file.fullName().asChar()))
        {
            MGlobal::displayError(theExporter.GetLastError().c_str());
            return MS::kFailure;
        }
        
        return MS::kSuccess;
    }

    bool haveReadMethod (void) const { return false; }
    bool haveWriteMethod (void) const { return true; }
    
    virtual MString defaultExtension (void) const { return MString("xsf"); }
    
    bool canBeOpened (void) const { return true; }
    
    MFileKind identifyFile (const MFileObject& fileName, const char *buffer, short size) const
    {
        return  MFileKind::kNotMyFileType;
    }
};


//----------------------------------------------------------------------------//
// cal3d Mesh exporter                                                        //
//----------------------------------------------------------------------------//
class cal3dMeshExport : public MPxFileTranslator 
{
public:
    cal3dMeshExport (void){ };
    virtual ~cal3dMeshExport (void){};

    static void *creator (void) { return new cal3dMeshExport; }

    MStatus reader (const MFileObject& file, const MString& optionsString, MPxFileTranslator::FileAccessMode mode)
    {
        return MS::kFailure;
    }

    MStatus writer (const MFileObject& file, const MString& optionsString, MPxFileTranslator::FileAccessMode mode)
    {
        AFX_MANAGE_STATE(AfxGetStaticModuleState());
        
        CMayaInterface mayaInterface;

        if (!mayaInterface.Create (mode == MPxFileTranslator::kExportActiveAccessMode))
        {
            MGlobal::displayError("Failed to initialize cal3d CMayaInterface.");
            return MS::kFailure;
        }


        // create an exporter instance
        if(!theExporter.Create(&mayaInterface))
        {
            mayaInterface.Destroy ();
            MGlobal::displayError(theExporter.GetLastError().c_str());
            return MS::kFailure;
        }

        if (!theExporter.ExportMesh (file.fullName().asChar()))
        {
            mayaInterface.Destroy ();
            MGlobal::displayError(theExporter.GetLastError().c_str());
            return MS::kFailure;
        }

        mayaInterface.Destroy ();
        
        return MS::kSuccess;
    }

    bool haveReadMethod (void) const { return false; }
    bool haveWriteMethod (void) const { return true; }
    
    virtual MString defaultExtension (void) const { return MString("xmf"); }
    
    bool canBeOpened (void) const { return true; }
    
    MFileKind identifyFile (const MFileObject& fileName, const char *buffer, short size) const
    {
        return  MFileKind::kNotMyFileType;
    }
};

//----------------------------------------------------------------------------//
// cal3d mateial exporter                                                         //
//----------------------------------------------------------------------------//
class cal3dMaterialExport : public MPxFileTranslator 
{
public:
    cal3dMaterialExport (void){ };
    virtual ~cal3dMaterialExport (void){};

    static void *creator (void) { return new cal3dMaterialExport; }

    MStatus reader (const MFileObject& file, const MString& optionsString, MPxFileTranslator::FileAccessMode mode)
    {
        return MS::kFailure;
    }

    MStatus writer (const MFileObject& file, const MString& optionsString, MPxFileTranslator::FileAccessMode mode)
    {
        AFX_MANAGE_STATE(AfxGetStaticModuleState());
        
        CMayaInterface mayaInterface;

        if (!mayaInterface.Create (mode == MPxFileTranslator::kExportActiveAccessMode))
        {
            MGlobal::displayError("Failed to initialize cal3d CMayaInterface.");
            return MS::kFailure;
        }


        // create an exporter instance
        if(!theExporter.Create(&mayaInterface))
        {
            MGlobal::displayError(theExporter.GetLastError().c_str());
            return MS::kFailure;
        }

        if (!theExporter.ExportMaterial (file.fullName().asChar()))
        {
            MGlobal::displayError(theExporter.GetLastError().c_str());
            return MS::kFailure;
        }
        
        return MS::kSuccess;
    }

    bool haveReadMethod (void) const { return false; }
    bool haveWriteMethod (void) const { return true; }
    
    virtual MString defaultExtension (void) const { return MString("xrf"); }
    
    bool canBeOpened (void) const { return true; }
    
    MFileKind identifyFile (const MFileObject& fileName, const char *buffer, short size) const
    {
        return  MFileKind::kNotMyFileType;
    }
};

//----------------------------------------------------------------------------//
// Maya Init Functions                                                        //
//----------------------------------------------------------------------------//
MStatus initializePlugin( MObject obj )
{
    MStatus status = MS::kSuccess;
    MFnPlugin plugin( obj, "cal3d", "1.0", "Any");

    status = plugin.registerFileTranslator ("Cal3D Animation",
                                            "",
                                            cal3dAnimationExport::creator);
    if (!status)
    {
        status.perror ("registerFileTranslator (Cal3D Animation)");
    }

    status = plugin.registerFileTranslator ("Cal3D Skeleton",
                                           "",
                                           cal3dSkeletonExport::creator);
    if (!status)
    {
       status.perror ("registerFileTranslator (Cal3D Skeleton)");
    }

    status = plugin.registerFileTranslator ("Cal3D Mesh",
                                           "",
                                           cal3dMeshExport::creator);
    if (!status)
    {
       status.perror ("registerFileTranslator (Cal3D Mesh)");
    }

    status = plugin.registerFileTranslator ("Cal3D Material",
                                           "",
                                           cal3dMaterialExport::creator);
    if (!status)
    {
       status.perror ("registerFileTranslator (Cal3D Material)");
    }


    return status;  
}

MStatus uninitializePlugin( MObject obj )
{
    MStatus status = MS::kSuccess;
    MFnPlugin plugin( obj );

    status = plugin.deregisterFileTranslator ("Cal3D Animation");
    if (!status)
    {
        status.perror ("deregisterFileTranslator (Cal3D Animation)");
    }

    status = plugin.deregisterFileTranslator ("Cal3D Material");
    if (!status)
    {
        status.perror ("deregisterFileTranslator (Cal3D Material)");
    }

    status = plugin.deregisterFileTranslator ("Cal3D Mesh");
    if (!status)
    {
        status.perror ("deregisterFileTranslator (Cal3D Mesh)");
    }

    status = plugin.deregisterFileTranslator ("Cal3D Skeleton");
    if (!status)
    {
        status.perror ("deregisterFileTranslator (Cal3D Skeleton)");
    }

    return status;
}

//----------------------------------------------------------------------------//
// Message mapping                                                            //
//----------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CMayaExporterApp, CWinApp)
    //{{AFX_MSG_MAP(CMayaExporterApp)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//----------------------------------------------------------------------------//
// Initialize the DLL instance                                                //
//----------------------------------------------------------------------------//
BOOL CMayaExporterApp::InitInstance() 
{
    static bool bInitialized = false;

    // initialize controls once
    if(!bInitialized)
    {
        DisableThreadLibraryCalls(m_hInstance);
        InitCommonControls();
        bInitialized = true;
    }

    return TRUE;
}

//----------------------------------------------------------------------------//
