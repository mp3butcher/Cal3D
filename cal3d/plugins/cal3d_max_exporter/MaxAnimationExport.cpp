//----------------------------------------------------------------------------//
// MaxAnimationExport.cpp                                                     //
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
#include "MaxAnimationExport.h"
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

CMaxAnimationExport::CMaxAnimationExport()
{
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CMaxAnimationExport::~CMaxAnimationExport()
{
}

//----------------------------------------------------------------------------//
// Following methods have to be implemented to make it a valid plugin         //
//----------------------------------------------------------------------------//

const TCHAR *CMaxAnimationExport::AuthorName()
{
	return _T("Bruno 'Beosil' Heidelberger");
}

const TCHAR *CMaxAnimationExport::CopyrightMessage()
{
	return _T("Copyright (C) 2001 Bruno 'Beosil' Heidelberger");
}

int CMaxAnimationExport::DoExport(const TCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts, DWORD options)
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

	// export the animation
	if(!theExporter.ExportAnimation(name))
	{
		AfxMessageBox(theExporter.GetLastError().c_str(), MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	return 1;
}

const TCHAR *CMaxAnimationExport::Ext(int i)
{
	switch(i)
	{
	case 0:
		return _T("caf");
	case 1:
		return _T("xaf");
	default:
		return _T("");
	}
}

int CMaxAnimationExport::ExtCount()
{
	return 2;
}

const TCHAR *CMaxAnimationExport::LongDesc()
{
	return _T("Cal3D Animation File");
}

const TCHAR *CMaxAnimationExport::OtherMessage1()
{
	return _T("");
}

const TCHAR *CMaxAnimationExport::OtherMessage2()
{
	return _T("");
}

const TCHAR *CMaxAnimationExport::ShortDesc()
{
	return _T("Cal3D Animation File");
}

void CMaxAnimationExport::ShowAbout(HWND hWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CDialog dlg(IDD_ABOUT);
	dlg.DoModal();
}

unsigned int CMaxAnimationExport::Version()
{
	return 50;
}

bool CMaxAnimationExport::ExportAnimationFromMaxscriptCall(const TCHAR *name, AnimExportParams* _animexportparams)
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

        // export the animation
	if(! maxInterface.ExportAnimationFromMaxscriptCall(name, (void*)_animexportparams))
	{
		AfxMessageBox(theExporter.GetLastError().c_str(), MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	return 1;
}

//----------------------------------------------------------------------------//

AnimExportParams::AnimExportParams(const char* _skeletonfilepath, INodeTab& _tabbones, int _startframe, int _endframe, int _frameoffset, int _framerate)
{
	int i = 0;

	m_skeletonfilepath	= strdup(_skeletonfilepath);
	m_startframe		= _startframe;
	m_endframe			= _endframe;
	m_frameoffset		= _frameoffset;
	m_framerate			= _framerate;

	//Copy thr array of nodes
	m_tabbones.ZeroCount();
	const int num = _tabbones.Count();
	for (i=0;i<num;i++)
	{
		INode* node = _tabbones[i];
		m_tabbones.Append(1,&node);
	}
}

AnimExportParams::~AnimExportParams()
{
	if (m_skeletonfilepath)
	{
		delete m_skeletonfilepath;
		m_skeletonfilepath = NULL;
	}
}

