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

#include "Maxscrpt\Maxscrpt.h"
#include "maxscrpt\Strings.h"
#include "maxscrpt\arrays.h"
#include "maxscrpt\numbers.h"
#include "maxscrpt\MaxMats.h"
#include "maxscrpt\definsfn.h"

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

char * CMaxMaterialExportDesc::GetRsrcString(long n)
{
	return NULL;
}


//----------------------------------------------------------------------------//
//Create a C++ function to be called by Maxscript to start the material exporter automatically
def_visible_primitive( ExportCalMat,	"ExportCalMat" );
Value* ExportCalMat_cf(Value** arg_list, int count)
{	
	int i = 0;
	char* fullpathfilename;

	check_arg_count(ExportCalMat, 2, count);
	type_check(arg_list[0], String, "[The first argument of ExportCalMat should be a string]");
	type_check(arg_list[1], MAXMaterial , "[The 2nd argument of ExportCalMat should be a standard material]");
	
	try
	{
		fullpathfilename		= arg_list[0]->to_string();
		StdMat* stdmat;

		if (! strcmp(fullpathfilename,"")) return new Integer(1);

		//Get material
		Value* val;
		val = arg_list[1];
		if (! val)return new Integer(2);

		if (val->is_kind_of(class_tag(MAXMaterial)) )
		{
			MtlBase* _mtl;
			_mtl = 	val->to_mtlbase();
			if (! _mtl)return new Integer(3);

			stdmat = static_cast<StdMat*>(_mtl);
		}
		else
		{
			return new Integer(3);
		}

		if ( CMaxMaterialExport::ExportMaterialFromMaxscriptCall(fullpathfilename, stdmat) )
			return new Integer(0);
		
		return new Integer(-1);
	}
	catch(...)
	{	
		//MessageBox(NULL,"Exception catched in ExportCalMat C++ function","Error",MB_OK);
		return new Integer(-2);
	}
}
