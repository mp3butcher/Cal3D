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

#include "Maxscrpt\Maxscrpt.h"
#include "maxscrpt\Strings.h"
#include "maxscrpt\arrays.h"
#include "maxscrpt\numbers.h"
#include "maxscrpt\maxobj.h"
#include "maxscrpt\definsfn.h"

#include "exporter.h"

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


char * CMaxAnimationExportDesc::GetRsrcString(long n)
{
	return NULL;
}


//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//Create a C++ function to be called by Maxscript to start the Animation exporter automatically
def_visible_primitive( ExportCalAnim,	"ExportCalAnim" );
Value* ExportCalAnim_cf(Value** arg_list, int count)
{	
	int i								;
	char*	Filefullpathfilename		;
	char*	Skeletonfullpathfilename	;
	Array*	BonesArray					;
	int		StartFrame					;
	int		EndFrame					;
	int		FrameOffset					;
	int		FrameRate					;
  bool   bUseAxisGL=false;

  // Cedric Pinson, now we can export in gl coordinates
	check_arg_count_with_keys(ExportCalAnim, 7, count);
	Value* transform= key_arg_or_default(transform, &false_value);
	type_check(transform, Boolean, "[The axisGL argument of ExportCalAnim should be a boolean that is true if you want to export in openGL axis]");

	type_check(arg_list[0], String	, "[The first argument of ExportCalAnim should be a string that is a full path name of the file to export]");
	type_check(arg_list[1], String	, "[The 2nd argument of ExportCalAnim should be a string that is the fullpath name of the skeleton file]");
	type_check(arg_list[2], Array	, "[The 3rd argument of ExportCalAnim should be an array of nodes to get anim from]");
	type_check(arg_list[3], Integer	, "[The 4th argument of ExportCalAnim should be an integer that is the start frame number]");
	type_check(arg_list[4], Integer	, "[The 5th argument of ExportCalAnim should be an integer that is the end frame number]");
	type_check(arg_list[5], Integer , "[The 6th argument of ExportCalAnim should be an integer that is the frame offset]");
	type_check(arg_list[6], Integer , "[The 7th argument of ExportCalAnim should be an integer that is the framerate]");
	
	try
	{
  bool   bUseAxisGL=false;

  // Cedric Pinson, now we can export in gl coordinates
	check_arg_count_with_keys(ExportCalAnim, 7, count);
	Value* transform= key_arg_or_default(transform, &false_value);
	type_check(transform, Boolean, "[The axisGL argument of ExportCalAnim should be a boolean that is true if you want to export in openGL axis]");
		Filefullpathfilename		= arg_list[0]->to_string();
		Skeletonfullpathfilename	= arg_list[1]->to_string();
		BonesArray					= static_cast<Array*>(arg_list[2]);
		StartFrame					= arg_list[3]->to_int();
		EndFrame					= arg_list[4]->to_int();
		FrameOffset					= arg_list[5]->to_int();
		FrameRate					= arg_list[6]->to_int();

		if (! strcmp(Filefullpathfilename,""))return new Integer(1);

		if (! strcmp(Skeletonfullpathfilename,"")) return new Integer(2);

		//Does skeleton file exist ?
		FILE* _stream;
		_stream = fopen(Skeletonfullpathfilename,"r");
		if (! _stream)return new Integer(3); //Error code number 3
		fclose(_stream);

		//Get the elements of the bones array
		int ArraySize;
    bUseAxisGL       = (transform->to_bool() != 0);

		ArraySize = BonesArray->size;

		if (! ArraySize) return new Integer(4);
		
		if (StartFrame < 0)	return new Integer(5);

		if (EndFrame < 0)return new Integer(6);

		if (StartFrame > EndFrame ) return new Integer(7);

		if (FrameOffset < 0) return new Integer(8);

		if (FrameRate < 0) return new Integer(9);

		INodeTab	tabnode;
		for (i=0;i<ArraySize;i++)
		{
			if (BonesArray->data[i]->is_kind_of(class_tag(MAXNode)) )
			{
				INode* _node	= 	BonesArray->data[i]->to_node();
				if (! _node)return new Integer(10);
				tabnode.Append(1,&_node);
			}
		}		
		
		AnimExportParams param(Skeletonfullpathfilename, tabnode, StartFrame, EndFrame, FrameOffset, FrameRate);

    theExporter.SetAxisGL(bUseAxisGL); // set axis wanted
    if (CMaxAnimationExport::ExportAnimationFromMaxscriptCall(Filefullpathfilename, &param)) {
      //reset to default
      theExporter.SetAxisGL(false);
			return new Integer(0);
    }


    theExporter.SetAxisGL(false);
		return new Integer(-1);
	}


	catch(...)
	{	
    theExporter.SetAxisGL(false);
		//MessageBox(NULL,"Exception catched in ExportCalAnim C++ function","Error",MB_OK);
		return new Integer(-2);
	}
}
