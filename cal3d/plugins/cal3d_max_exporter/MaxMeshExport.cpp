//----------------------------------------------------------------------------//
// MaxMeshExport.cpp                                                          //
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
#include "MaxMeshExport.h"
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

CMaxMeshExport::CMaxMeshExport()
{
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CMaxMeshExport::~CMaxMeshExport()
{
}

//----------------------------------------------------------------------------//
// Following methods have to be implemented to make it a valid plugin         //
//----------------------------------------------------------------------------//

const TCHAR *CMaxMeshExport::AuthorName()
{
	return _T("Bruno 'Beosil' Heidelberger");
}

const TCHAR *CMaxMeshExport::CopyrightMessage()
{
	return _T("Copyright (C) 2001 Bruno 'Beosil' Heidelberger");
}

int CMaxMeshExport::DoExport(const TCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts, DWORD options)
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

	// export the mesh
	if(!theExporter.ExportMesh(name))
	{
		AfxMessageBox(theExporter.GetLastError().c_str(), MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	return 1;
}

const TCHAR *CMaxMeshExport::Ext(int i)
{
	switch(i)
	{
	case 0:
		return _T("cmf");
	case 1:
		return _T("xmf");
	default:
		return _T("");
	}
}

int CMaxMeshExport::ExtCount()
{
	return 2;
}

const TCHAR *CMaxMeshExport::LongDesc()
{
	return _T("Cal3D Mesh File");
}

const TCHAR *CMaxMeshExport::OtherMessage1()
{
	return _T("");
}

const TCHAR *CMaxMeshExport::OtherMessage2()
{
	return _T("");
}

const TCHAR *CMaxMeshExport::ShortDesc()
{
	return _T("Cal3D Mesh File");
}

void CMaxMeshExport::ShowAbout(HWND hWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CDialog dlg(IDD_ABOUT);
	dlg.DoModal();
}

unsigned int CMaxMeshExport::Version()
{
	return 50;
}

//----------------------------------------------------------------------------//
MeshMaxscriptExportParams::MeshMaxscriptExportParams(INode* _MeshNode, const char* _SkeletonFilename, int _MaxNumBonesPerVertex, float _WeightThreshold, int _LODCreation, int _springsystem)
{
	m_MeshNode				= _MeshNode;
	m_SkeletonFilename		= strdup(_SkeletonFilename);
	m_MaxNumBonesPerVertex	= _MaxNumBonesPerVertex;
	m_WeightThreshold		= _WeightThreshold;
	m_LODCreation			= _LODCreation;
	m_springsystem			= _springsystem;
}

MeshMaxscriptExportParams::~MeshMaxscriptExportParams()
{
	if (m_SkeletonFilename)
		delete m_SkeletonFilename;
	m_SkeletonFilename = NULL;
}

int CMaxMeshExport::ExportMeshFromMaxscriptCall(const TCHAR *name, const MeshMaxscriptExportParams& _param)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// create an export interface for 3d studio max
	CMaxInterface maxInterface;
	if(!maxInterface.Create(NULL, GetCOREInterface()))
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

        // export the mesh
	if(!maxInterface.ExportMeshFromMaxscriptCall(name, (void*)&_param))
	{
		AfxMessageBox(theExporter.GetLastError().c_str(), MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

        return 1;	
}
