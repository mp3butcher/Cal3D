//----------------------------------------------------------------------------//
// MaxSkeletonExportDesc.cpp                                                  //
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
#include "MaxSkeletonExportDesc.h"
#include "MaxSkeletonExport.h"

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

CMaxSkeletonExportDesc::CMaxSkeletonExportDesc()
{
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CMaxSkeletonExportDesc::~CMaxSkeletonExportDesc()
{
}

//----------------------------------------------------------------------------//
// Set the DLL instance handle of this plugin                                 //
//----------------------------------------------------------------------------//

void CMaxSkeletonExportDesc::SetInstance(HINSTANCE hInstance)
{
	m_hInstance = hInstance;
}

//----------------------------------------------------------------------------//
// Following methods have to be implemented to make it a valid plugin         //
//----------------------------------------------------------------------------//

const TCHAR *CMaxSkeletonExportDesc::Category()
{
	return _T("Game Utilities");
}

Class_ID CMaxSkeletonExportDesc::ClassID()
{
	return MAX_SKELETON_EXPORT_ID;
}

const TCHAR *CMaxSkeletonExportDesc::ClassName()
{
	return _T("Cal3D Skeleton Export");
}

void *CMaxSkeletonExportDesc::Create(BOOL loading)
{
	return new CMaxSkeletonExport();
}

HINSTANCE CMaxSkeletonExportDesc::HInstance()
{
	return m_hInstance;
}

const TCHAR *CMaxSkeletonExportDesc::InternalName()
{
	return _T("Cal3D_Skeleton_Export");
} 

int CMaxSkeletonExportDesc::IsPublic()
{
	return 1;
}

SClass_ID CMaxSkeletonExportDesc::SuperClassID()
{
	return SCENE_EXPORT_CLASS_ID;
}

//----------------------------------------------------------------------------//
