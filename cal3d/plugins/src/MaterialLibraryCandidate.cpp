//----------------------------------------------------------------------------//
// MaterialLibraryCandidate.cpp                                               //
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
#include "Exporter.h"
#include "MaterialLibraryCandidate.h"
#include "MaterialCandidate.h"
#include "BaseInterface.h"
#include "BaseMesh.h"

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

CMaterialLibraryCandidate::CMaterialLibraryCandidate()
{
	m_pSelectedMaterialCandidate = 0;
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CMaterialLibraryCandidate::~CMaterialLibraryCandidate()
{
	Clear();
}

//----------------------------------------------------------------------------//
// Clear this mesh candidate                                                  //
//----------------------------------------------------------------------------//

void CMaterialLibraryCandidate::Clear()
{
	// destroy all material candidates stored in this mesh candidate
	for(size_t materialCandidateId = 0; materialCandidateId < m_vectorMaterialCandidate.size(); materialCandidateId++)
	{
		delete m_vectorMaterialCandidate[materialCandidateId];
	}
	m_vectorMaterialCandidate.clear();
}

//----------------------------------------------------------------------------//
// Create a material library candidate instance from the exporter interface   //
//----------------------------------------------------------------------------//

bool CMaterialLibraryCandidate::CreateFromInterface()
{
	// clear current content
	Clear();

	// get the number of materials of the mesh
	int materialCount;
	materialCount = theExporter.GetInterface()->GetMaterialCount();

	// create all the material candidates
	int materialId;
	for(materialId = 0; materialId < materialCount; materialId++)
	{
		// allocate a new material candidate
		CMaterialCandidate *pMaterialCandidate;
		pMaterialCandidate = new CMaterialCandidate();
		if(pMaterialCandidate == 0)
		{
			theExporter.SetLastError("Memory allocation failed!", __FILE__, __LINE__);
			return false;
		}

		// create the new material candidate
		if(!pMaterialCandidate->Create(theExporter.GetInterface()->GetMaterial(materialId))) return false;

		// add material candidate to the material candidate vector
		m_vectorMaterialCandidate.push_back(pMaterialCandidate);
	}

	return true;
}

//----------------------------------------------------------------------------//
// Get the selected material candidate                                        //
//----------------------------------------------------------------------------//

CMaterialCandidate *CMaterialLibraryCandidate::GetSelectedMaterialCandidate()
{
	return m_pSelectedMaterialCandidate;
}

//----------------------------------------------------------------------------//
// Get the material candidate vector                                          //
//----------------------------------------------------------------------------//

std::vector<CMaterialCandidate *>& CMaterialLibraryCandidate::GetVectorMaterialCandidate()
{
	return m_vectorMaterialCandidate;
}

//----------------------------------------------------------------------------//
// Set the selected material candidate                                        //
//----------------------------------------------------------------------------//

void CMaterialLibraryCandidate::SetSelectedMaterialCandidate(CMaterialCandidate *pSelectedMaterialCandidate)
{
	m_pSelectedMaterialCandidate = pSelectedMaterialCandidate;
}

