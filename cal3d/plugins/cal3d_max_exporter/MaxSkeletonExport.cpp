//----------------------------------------------------------------------------//
// MaxSkeletonExport.cpp                                                      //
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
#include "MaxSkeletonExport.h"
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

CMaxSkeletonExport::CMaxSkeletonExport()
{
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CMaxSkeletonExport::~CMaxSkeletonExport()
{
}

//----------------------------------------------------------------------------//
// Following methods have to be implemented to make it a valid plugin         //
//----------------------------------------------------------------------------//

const TCHAR *CMaxSkeletonExport::AuthorName()
{
	return _T("Bruno 'Beosil' Heidelberger");
}

const TCHAR *CMaxSkeletonExport::CopyrightMessage()
{
	return _T("Copyright (C) 2001 Bruno 'Beosil' Heidelberger");
}

int CMaxSkeletonExport::DoExport(const TCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts, DWORD options)
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

	// export the skeleton
	if(!theExporter.ExportSkeleton(name))
	{
		AfxMessageBox(theExporter.GetLastError().c_str(), MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	return 1;
}

int CMaxSkeletonExport::ExportSkeletonFromMaxscriptCall(const TCHAR *name, INodeTab& _tabnode, bool bShowUI)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// create an export interface for 3d studio max and set automatically the bones to export from Maxscript
	CMaxInterface maxInterface;
	if(!maxInterface.Create(NULL, GetCOREInterface(),_tabnode))
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

        // export the skeleton
	if(!maxInterface.ExportSkeletonFromMaxscriptCall(name, bShowUI))
	{
		AfxMessageBox(theExporter.GetLastError().c_str(), MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	return 1;
}

const TCHAR *CMaxSkeletonExport::Ext(int i)
{
	switch(i)
	{
	case 0:
		return _T("csf");
	case 1:
		return _T("xsf");
	default:
		return _T("");
	}
}

int CMaxSkeletonExport::ExtCount()
{
	return 2;
}

const TCHAR *CMaxSkeletonExport::LongDesc()
{
	return _T("Cal3D Skeleton File");
}

const TCHAR *CMaxSkeletonExport::OtherMessage1()
{
	return _T("");
}

const TCHAR *CMaxSkeletonExport::OtherMessage2()
{
	return _T("");
}

const TCHAR *CMaxSkeletonExport::ShortDesc()
{
	return _T("Cal3D Skeleton File");
}

void CMaxSkeletonExport::ShowAbout(HWND hWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CDialog dlg(IDD_ABOUT);
	dlg.DoModal();
}

unsigned int CMaxSkeletonExport::Version()
{
	return 50;
}

//----------------------------------------------------------------------------//
