//----------------------------------------------------------------------------//
// MaxMeshExportDesc.cpp                                                      //
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
#include "MaxMeshExportDesc.h"
#include "MaxMeshExport.h"

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

CMaxMeshExportDesc::CMaxMeshExportDesc()
{
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CMaxMeshExportDesc::~CMaxMeshExportDesc()
{
}

//----------------------------------------------------------------------------//
// Set the DLL instance handle of this plugin                                 //
//----------------------------------------------------------------------------//

void CMaxMeshExportDesc::SetInstance(HINSTANCE hInstance)
{
	m_hInstance = hInstance;
}

//----------------------------------------------------------------------------//
// Following methods have to be implemented to make it a valid plugin         //
//----------------------------------------------------------------------------//

const TCHAR *CMaxMeshExportDesc::Category()
{
	return _T("Game Utilities");
}

Class_ID CMaxMeshExportDesc::ClassID()
{
	return MAX_MESH_EXPORT_ID;
}

const TCHAR *CMaxMeshExportDesc::ClassName()
{
	return _T("Cal3D Mesh Export");
}

void *CMaxMeshExportDesc::Create(BOOL loading)
{
	return new CMaxMeshExport();
}

HINSTANCE CMaxMeshExportDesc::HInstance()
{
	return m_hInstance;
}

const TCHAR *CMaxMeshExportDesc::InternalName()
{
	return _T("Cal3D_Mesh_Export");
} 

int CMaxMeshExportDesc::IsPublic()
{
	return 1;
}

SClass_ID CMaxMeshExportDesc::SuperClassID()
{
	return SCENE_EXPORT_CLASS_ID;
}

//----------------------------------------------------------------------------//
