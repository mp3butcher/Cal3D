//----------------------------------------------------------------------------//
// cal3d_milk_exporter.cpp                                                    //
// Copyright (C) 2002 Bruno 'Beosil' Heidelberger                             //
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
#include "cal3d_milk_exporter.h"
#include "MilkExport.h"

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

CMilkExporterApp theDll;

//----------------------------------------------------------------------------//
// Exported functions                                                         //
//----------------------------------------------------------------------------//

cMsPlugIn *CreatePlugIn()
{
  return theDll.GetPluginInstance();
}

//----------------------------------------------------------------------------//
// Message mapping                                                            //
//----------------------------------------------------------------------------//

BEGIN_MESSAGE_MAP(CMilkExporterApp, CWinApp)
	//{{AFX_MSG_MAP(CMilkExporterApp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//----------------------------------------------------------------------------//
// Constructors                                                               //
//----------------------------------------------------------------------------//

CMilkExporterApp::CMilkExporterApp()
{
}

//----------------------------------------------------------------------------//
// Get a new plugin instance                                                  //
//----------------------------------------------------------------------------//

cMsPlugIn *CMilkExporterApp::GetPluginInstance()
{
  return new CMilkExport();
}

//----------------------------------------------------------------------------//
// Initialize the DLL instance                                                //
//----------------------------------------------------------------------------//

BOOL CMilkExporterApp::InitInstance() 
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
