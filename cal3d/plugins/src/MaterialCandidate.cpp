//----------------------------------------------------------------------------//
// MaterialCandidate.cpp                                                      //
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
#include "MaterialCandidate.h"
#include "BaseInterface.h"
#include "BaseMaterial.h"

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

CMaterialCandidate::CMaterialCandidate()
{
	m_pMaterial = 0;
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CMaterialCandidate::~CMaterialCandidate()
{
	delete m_pMaterial;
}

//----------------------------------------------------------------------------//
// Create a bone candidate                                                    //
//----------------------------------------------------------------------------//

bool CMaterialCandidate::Create(CBaseMaterial *pMaterial)
{
	// check if the material is valid
	if(pMaterial == 0)
	{
		theExporter.SetLastError("Invalid handle.", __FILE__, __LINE__);
		return false;
	}

	m_pMaterial = pMaterial;

	// get the number of maps in the material
	int mapCount;
	mapCount = pMaterial->GetMapCount();

	// loop trough all maps
	int mapId;
	for(mapId = 0; mapId < mapCount; mapId++)
	{
		// create a map instance
		Map map;
		map.strFilename = pMaterial->GetMapFilename(mapId);

		// add map to the map vector of this material
		m_vectorMap.push_back(map);
	}

	return true;
}

//----------------------------------------------------------------------------//
// Get the ambient color of the material candidate                            //
//----------------------------------------------------------------------------//

void CMaterialCandidate::GetAmbientColor(float *pColor)
{
	m_pMaterial->GetAmbientColor(pColor);
}

//----------------------------------------------------------------------------//
// Get the diffuse color of the material candidate                            //
//----------------------------------------------------------------------------//

void CMaterialCandidate::GetDiffuseColor(float *pColor)
{
	m_pMaterial->GetDiffuseColor(pColor);
}

//----------------------------------------------------------------------------//
// Get the name of the material candidate                                     //
//----------------------------------------------------------------------------//

std::string CMaterialCandidate::GetName()
{
	// check if the node is valid
	if(m_pMaterial == 0) return "<null>";

	return m_pMaterial->GetName();
}

//----------------------------------------------------------------------------//
// Get the material of the material candidate                                 //
//----------------------------------------------------------------------------//

CBaseMaterial *CMaterialCandidate::GetMaterial()
{
	return m_pMaterial;
}

//----------------------------------------------------------------------------//
// Get the shininess factor of the material candidate                         //
//----------------------------------------------------------------------------//

float CMaterialCandidate::GetShininess()
{
	return m_pMaterial->GetShininess();
}

//----------------------------------------------------------------------------//
// Get the specular color of the material candidate                           //
//----------------------------------------------------------------------------//

void CMaterialCandidate::GetSpecularColor(float *pColor)
{
	m_pMaterial->GetSpecularColor(pColor);
}

//----------------------------------------------------------------------------//
// Get the map vector                                                         //
//----------------------------------------------------------------------------//

std::vector<CMaterialCandidate::Map>& CMaterialCandidate::GetVectorMap()
{
	return m_vectorMap;
}

//----------------------------------------------------------------------------//
