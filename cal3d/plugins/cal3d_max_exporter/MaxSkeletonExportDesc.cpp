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

// initialize variables to hold names used as keyword parameters

#include "Maxscrpt\Maxscrpt.h"
#include "maxscrpt\Strings.h"
#include "maxscrpt\arrays.h"
#include "maxscrpt\numbers.h"
#include "maxscrpt\maxobj.h"

#include "exporter.h"

#include "maxscrpt\definsfn.h"

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
  def_name(axisGL);
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

char * CMaxSkeletonExportDesc::GetRsrcString(long n)
{
	return NULL;
}


//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//Create a C++ function to be called by Maxscript to start the skeleton exporter automatically

def_visible_primitive( ExportCalSkel,	"ExportCalSkel" );
Value* ExportCalSkel_cf(Value** arg_list, int count)
{	
	int			i;
	INodeTab	tabnode;
	char*		fullpathfilename;
	int			ArraySize		;
	bool		bShowUI			;
  bool bUseAxisGL=false; 

  // Cedric Pinson, now we can export in gl coordinates
	check_arg_count_with_keys(ExportCalSkel, 3, count);
	Value* transform= key_arg_or_default(transform, &false_value);
	type_check(transform, Boolean, "[The axisGL argument of ExportCalSkel should be a boolean that is true if you want to export in openGL axis]");

	type_check(arg_list[0], String, "[The first argument of ExportCalSkel should be a string that is a full path name of the file to export]");
	type_check(arg_list[1], Array , "[The 2nd argument of ExportCalSkel should be an array of nodes]");
	type_check(arg_list[2], Boolean,"[The 3rd argument of ExportCalSkel should be a boolean that tells if you want to use the UI or not to select nodes of skeleton]");
	
	try
	{
		fullpathfilename	= arg_list[0]->to_string();
    bUseAxisGL       = (transform->to_bool() != 0);

		//Get Array
		Array* BonesArray	= static_cast<Array*>(arg_list[1]);
		ArraySize			= BonesArray->size;	

		bShowUI				= !!(arg_list[2]->to_bool());

		if (! strcmp(fullpathfilename,"")) return new Integer (1);
		if (! ArraySize)		return new Integer (2);
 
		for (i=0;i<ArraySize;i++)
		{
			if (BonesArray->data[i]->is_kind_of(class_tag(MAXNode)) )
			{
				INode* _node	= 	BonesArray->data[i]->to_node();
				if (! _node)return new Integer (3);

				tabnode.Append(1,&_node);
			}
		}

    theExporter.SetAxisGL(bUseAxisGL);


		//Call the exporter from Maxscript
    if (CMaxSkeletonExport::ExportSkeletonFromMaxscriptCall(fullpathfilename,tabnode, bShowUI) ) {
      // reset axis gl
      theExporter.SetAxisGL(false);
			return new Integer (0);
    }
    theExporter.SetAxisGL(false);
		return new Integer (-1);
	}
	catch(...)
	{	
    theExporter.SetAxisGL(false);

		//MessageBox(NULL,"Exception catched in ExportCalSkel C++ function","Error",MB_OK);
		return new Integer (-2);
	}
}
