//----------------------------------------------------------------------------//
// MaxMaterialExport.cpp                                                      //
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
#include "MaxMaterialExport.h"
#include "Exporter.h"
#include "MaxInterface.h"

//----------------------------------------------------------------------------//
// Debug                                                                      //
//----------------------------------------------------------------------------//

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//----------------------------------------------------------------------------//
// Constructors                                                               //
//----------------------------------------------------------------------------//

CMaxMaterialExport::CMaxMaterialExport()
{
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CMaxMaterialExport::~CMaxMaterialExport()
{
}

//----------------------------------------------------------------------------//
// Following methods have to be implemented to make it a valid plugin         //
//----------------------------------------------------------------------------//

const TCHAR *CMaxMaterialExport::AuthorName()
{
	return _T("Bruno 'Beosil' Heidelberger");
}

const TCHAR *CMaxMaterialExport::CopyrightMessage()
{
	return _T("Copyright (C) 2001 Bruno 'Beosil' Heidelberger");
}

int CMaxMaterialExport::DoExport(const TCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts, DWORD options)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// create an export interface for 3d studio max
	CMaxInterface maxInterface;
	if(!maxInterface.Create(ei, i))
	{
		AfxMessageBox(theExporter.GetLastError().c_str(), MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	// create an exporter instance
	if(!theExporter.Create(&maxInterface))
	{
		AfxMessageBox(theExporter.GetLastError().c_str(), MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	// export the materials
	if(!theExporter.ExportMaterial(name))
	{
		AfxMessageBox(theExporter.GetLastError().c_str(), MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	return 1;
}

const TCHAR *CMaxMaterialExport::Ext(int i)
{
	switch(i)
	{
	case 0:
		return _T("crf");
	case 1:
		return _T("xrf");
	default:
		return _T("");
	}
}

int CMaxMaterialExport::ExtCount()
{
	return 2;
}

const TCHAR *CMaxMaterialExport::LongDesc()
{
	return _T("Cal3D Material File");
}

const TCHAR *CMaxMaterialExport::OtherMessage1()
{
	return _T("");
}

const TCHAR *CMaxMaterialExport::OtherMessage2()
{
	return _T("");
}

const TCHAR *CMaxMaterialExport::ShortDesc()
{
	return _T("Cal3D Material File");
}

void CMaxMaterialExport::ShowAbout(HWND hWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CDialog dlg(IDD_ABOUT);
	dlg.DoModal();
}

unsigned int CMaxMaterialExport::Version()
{
	return 50;
}

//----------------------------------------------------------------------------//
bool CMaxMaterialExport::ExportMaterialFromMaxscriptCall(const char* fullpathfilename, StdMat* _stdmatfrommaxscript)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// create an export interface for 3d studio max
	CMaxInterface maxInterface;
	
	//Set the tab of materials into our Max interface
	if(! maxInterface.Create(NULL, GetCOREInterface(), _stdmatfrommaxscript))
	{
		AfxMessageBox(theExporter.GetLastError().c_str(), MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	// create an exporter instance
	if(!theExporter.Create(&maxInterface))
	{
		AfxMessageBox(theExporter.GetLastError().c_str(), MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

        // export the materials
	if(!maxInterface.ExportMaterialFromMaxscriptCall(fullpathfilename))
	{
		AfxMessageBox(theExporter.GetLastError().c_str(), MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	return 1;	
}