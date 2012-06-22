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

#ifdef MAX_RELEASE_R13 // Max 2011 and up
#include "maxscript/maxscript.h"
#include "maxscript/foundation/strings.h"
#include "maxscript/foundation/numbers.h"
#include "maxscript/macros/define_instantiation_functions.h"
#include "maxscript/maxwrapper/mxsobjects.h"
#include "maxscript/maxwrapper/maxclasses.h"
#include "maxscript/compiler/parser.h"
#else
#include "maxscrpt/maxscrpt.h"
#include "maxscrpt/Strings.h"
#include "maxscrpt/numbers.h"
#include "maxscrpt/Maxobj.h"
#include "maxscrpt/definsfn.h"
#include "maxscrpt/Parser.h"
#endif

#include "exporter.h"

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


const TCHAR* CMaxMeshExportDesc::GetRsrcString(long n)
{
	return NULL;
}


//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//Create a C++ function to be called by Maxscript to start the Mesh exporter automatically
def_visible_primitive( ExportCalMesh,	"ExportCalMesh" );
Value* ExportCalMesh_cf(Value** arg_list, int count)
{	
	TSTR	Filefullpathfilename	;
	TSTR	Skeletonfullpathfilename;
	int		MaxNumOfBones				;
	float	WeightThreshold				;
	int		bUseLODCreation				;
	int		bUseSpringsystem			;
	INode*	MeshNode					;
  bool bUseAxisGL=false;


  // Cedric Pinson, now we can export in gl coordinates
	check_arg_count_with_keys(ExportCalMesh, 7, count);
	Value* transform= key_arg_or_default(transform, &false_value);
	type_check(transform, Boolean, _T("[The axisGL argument of ExportCalMesh should be a boolean that is true if you want to export in openGL axis]"));

  type_check(arg_list[0], String		, _T("[The first argument of ExportCalMesh should be a string that is a full path name of the file to export]"));
	type_check(arg_list[1], String		, _T("[The 2nd argument of ExportCalMesh should be a string that is the fullpath name of the skeleton file]"));
	type_check(arg_list[2], MAXNode		, _T("[The 3rd argument of ExportCalMesh should be an mesh node that is the mesh to be exported]"));
	type_check(arg_list[3], Integer		, _T("[The 3rd argument of ExportCalMesh should be an integer that is the maximum number of bones per vertex]"));
	type_check(arg_list[4], Float		, _T("[The 4th argument of ExportCalMesh should be a float that is the weight threshold]"));
	type_check(arg_list[5], Boolean		, _T("[The 5th argument of ExportCalMesh should be a boolean that is true if you want LOD creation]"));
	type_check(arg_list[6], Boolean		, _T("[The 6th argument of ExportCalMesh should be a boolean that is true if you want to use spring system]"));
	
	try
	{
		Filefullpathfilename		= arg_list[0]->to_string();
		Skeletonfullpathfilename	= arg_list[1]->to_string();
		MeshNode					= arg_list[2]->to_node();
		MaxNumOfBones				= arg_list[3]->to_int();
		WeightThreshold				= arg_list[4]->to_float();
		bUseLODCreation				= arg_list[5]->to_bool();
		bUseSpringsystem			= arg_list[6]->to_bool();
    bUseAxisGL       = (transform->to_bool() != 0);


		if (Filefullpathfilename.Length() == 0) return new Integer(1);
		if (Skeletonfullpathfilename.Length() == 0) return new Integer(2);

		//Does skeleton file exist ? 
		FILE* _stream = NULL;
		_tfopen_s(&_stream, Skeletonfullpathfilename.data(),_T("r"));
		if (! _stream)return new Integer(3); //Error code number 3
		fclose(_stream);

		if ((MaxNumOfBones <= 0))return new Integer (4);

		if (WeightThreshold < 0.f) return new Integer (5);  

		if (! MeshNode) return new Integer (6); //Null pointer

		//Check if it is a mesh ?
		Object *obj = MeshNode->EvalWorldState(GetCOREInterface()->GetTime()).obj;
		if (! obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0))) return new Integer (7); //Not a Mesh
				
		//Create the parameter structure to be sent to the function ExportMeshFromMaxscriptCall
		MeshMaxscriptExportParams	param ( MeshNode, Skeletonfullpathfilename.data(),	MaxNumOfBones, WeightThreshold,bUseLODCreation,bUseSpringsystem);

    theExporter.SetAxisGL(bUseAxisGL); // set axis wanted
	TSTR fullFilePath(Filefullpathfilename);
    if ( CMaxMeshExport::ExportMeshFromMaxscriptCall(fullFilePath, param) ) {
      // reset to default
      theExporter.SetAxisGL(false);
			return new Integer(0);
    }
		
    theExporter.SetAxisGL(false);
		return new Integer(-1);
	}


	catch(...)
	{	
    theExporter.SetAxisGL(false);

		//MessageBox(NULL,"Exception catched in ExportCalMesh C++ function","Error",MB_OK);
		return new Integer(-2);
	}
}
 