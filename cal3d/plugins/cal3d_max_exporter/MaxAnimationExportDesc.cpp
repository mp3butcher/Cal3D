//----------------------------------------------------------------------------//
// MaxAnimationExportDesc.cpp                                                 //
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
#include "MaxAnimationExportDesc.h"
#include "MaxAnimationExport.h"

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

CMaxAnimationExportDesc::CMaxAnimationExportDesc()
{
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CMaxAnimationExportDesc::~CMaxAnimationExportDesc()
{
}

//----------------------------------------------------------------------------//
// Set the DLL instance handle of this plugin                                 //
//----------------------------------------------------------------------------//

void CMaxAnimationExportDesc::SetInstance(HINSTANCE hInstance)
{
	m_hInstance = hInstance;
}

//----------------------------------------------------------------------------//
// Following methods have to be implemented to make it a valid plugin         //
//----------------------------------------------------------------------------//

const TCHAR *CMaxAnimationExportDesc::Category()
{
	return _T("Game Utilities");
}

Class_ID CMaxAnimationExportDesc::ClassID()
{
	return MAX_ANIMATION_EXPORT_ID;
}

const TCHAR *CMaxAnimationExportDesc::ClassName()
{
	return _T("Cal3D Animation Export");
}

void *CMaxAnimationExportDesc::Create(BOOL loading)
{
	return new CMaxAnimationExport();
}

HINSTANCE CMaxAnimationExportDesc::HInstance()
{
	return m_hInstance;
}

const TCHAR *CMaxAnimationExportDesc::InternalName()
{
	return _T("Cal3D_Animation_Export");
} 

int CMaxAnimationExportDesc::IsPublic()
{
	return 1;
}

SClass_ID CMaxAnimationExportDesc::SuperClassID()
{
	return SCENE_EXPORT_CLASS_ID;
}

//----------------------------------------------------------------------------//
