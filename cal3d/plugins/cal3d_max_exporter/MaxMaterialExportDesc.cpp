//----------------------------------------------------------------------------//
// MaxMaterialExportDesc.cpp                                                  //
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
#include "MaxMaterialExportDesc.h"
#include "MaxMaterialExport.h"

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

CMaxMaterialExportDesc::CMaxMaterialExportDesc()
{
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CMaxMaterialExportDesc::~CMaxMaterialExportDesc()
{
}

//----------------------------------------------------------------------------//
// Set the DLL instance handle of this plugin                                 //
//----------------------------------------------------------------------------//

void CMaxMaterialExportDesc::SetInstance(HINSTANCE hInstance)
{
	m_hInstance = hInstance;
}

//----------------------------------------------------------------------------//
// Following methods have to be implemented to make it a valid plugin         //
//----------------------------------------------------------------------------//

const TCHAR *CMaxMaterialExportDesc::Category()
{
	return _T("Game Utilities");
}

Class_ID CMaxMaterialExportDesc::ClassID()
{
	return MAX_MATERIAL_EXPORT_ID;
}

const TCHAR *CMaxMaterialExportDesc::ClassName()
{
	return _T("Cal3D Material Export");
}

void *CMaxMaterialExportDesc::Create(BOOL loading)
{
	return new CMaxMaterialExport();
}

HINSTANCE CMaxMaterialExportDesc::HInstance()
{
	return m_hInstance;
}

const TCHAR *CMaxMaterialExportDesc::InternalName()
{
	return _T("Cal3D_Material_Export");
} 

int CMaxMaterialExportDesc::IsPublic()
{
	return 1;
}

SClass_ID CMaxMaterialExportDesc::SuperClassID()
{
	return SCENE_EXPORT_CLASS_ID;
}

//----------------------------------------------------------------------------//
