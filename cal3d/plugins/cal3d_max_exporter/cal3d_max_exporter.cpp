//----------------------------------------------------------------------------//
// cal3d_max_exporter.cpp                                                     //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
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
#include "cal3d_max_exporter.h"
#include "MaxSkeletonExportDesc.h"
#include "MaxAnimationExportDesc.h"
#include "MaxMeshExportDesc.h"
#include "MaxMaterialExportDesc.h"

//----------------------------------------------------------------------------//
// Debug                                                                      //
//----------------------------------------------------------------------------//

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//----------------------------------------------------------------------------//
// The one and only DLL instance                                              //
//----------------------------------------------------------------------------//

CMaxExporterApp theDll;

//----------------------------------------------------------------------------//
// Exported functions                                                         //
//----------------------------------------------------------------------------//

__declspec(dllexport) ClassDesc *LibClassDesc(int id)
{
	return theDll.GetClassDesc(id);
}

__declspec(dllexport) const TCHAR *LibDescription()
{
	return "Cal3D Exporter";
}

__declspec(dllexport) int LibNumberClasses()
{
	return theDll.GetClassDescCount();
}

__declspec(dllexport) ULONG LibVersion()
{
	return VERSION_3DSMAX;
}

//----------------------------------------------------------------------------//
// Static member variables initialization                                     //
//----------------------------------------------------------------------------//

CMaxSkeletonExportDesc CMaxExporterApp::m_maxSkeletonExportDesc;
CMaxAnimationExportDesc CMaxExporterApp::m_maxAnimationExportDesc;
CMaxMeshExportDesc CMaxExporterApp::m_maxMeshExportDesc;
CMaxMaterialExportDesc CMaxExporterApp::m_maxMaterialExportDesc;

//----------------------------------------------------------------------------//
// Message mapping                                                            //
//----------------------------------------------------------------------------//

BEGIN_MESSAGE_MAP(CMaxExporterApp, CWinApp)
	//{{AFX_MSG_MAP(CMaxExporterApp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//----------------------------------------------------------------------------//
// Constructors                                                               //
//----------------------------------------------------------------------------//

CMaxExporterApp::CMaxExporterApp()
{
}

//----------------------------------------------------------------------------//
// Get the class description handle for a given id                            //
//----------------------------------------------------------------------------//

ClassDesc *CMaxExporterApp::GetClassDesc(int id)
{
	switch(id)
	{
		case 0:
			m_maxSkeletonExportDesc.SetInstance(m_hInstance);
			return &m_maxSkeletonExportDesc;
		case 1:
			m_maxAnimationExportDesc.SetInstance(m_hInstance);
			return &m_maxAnimationExportDesc;
		case 2:
			m_maxMeshExportDesc.SetInstance(m_hInstance);
			return &m_maxMeshExportDesc;
		case 3:
			m_maxMaterialExportDesc.SetInstance(m_hInstance);
			return &m_maxMaterialExportDesc;
		default:
			return 0;
	}
}

//----------------------------------------------------------------------------//
// Get the number of class description handles                                //
//----------------------------------------------------------------------------//

int CMaxExporterApp::GetClassDescCount()
{
	return 4;
}

//----------------------------------------------------------------------------//
// Initialize the DLL instance                                                //
//----------------------------------------------------------------------------//

BOOL CMaxExporterApp::InitInstance() 
{
	static bool bInitialized = false;

	// initialize controls once
	if(!bInitialized)
	{
		DisableThreadLibraryCalls(m_hInstance);
		InitCustomControls(m_hInstance);
		InitCommonControls();
		bInitialized = true;
	}

	return TRUE;
}

//----------------------------------------------------------------------------//

