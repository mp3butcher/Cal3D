//----------------------------------------------------------------------------//
// MaxMaterial.cpp                                                            //
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
#include "MaxMaterial.h"
#include "BaseInterface.h"

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

CMaxMaterial::CMaxMaterial()
{
	m_pIStdMat = 0;
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CMaxMaterial::~CMaxMaterial()
{
}

//----------------------------------------------------------------------------//
// Create a max material instance                                             //
//----------------------------------------------------------------------------//

bool CMaxMaterial::Create(StdMat *pIStdMat)
{
	// check if the internal node is valid
	if(pIStdMat == 0)
	{
		theExporter.SetLastError("Invalid handle.", __FILE__, __LINE__);
		return false;
	}

	m_pIStdMat = pIStdMat;

	return true;
}

//----------------------------------------------------------------------------//
// Get the ambient color of the material                                      //
//----------------------------------------------------------------------------//

void CMaxMaterial::GetAmbientColor(float *pColor)
{
	// get color from the internal standard material
	Color color;
	color = m_pIStdMat->GetAmbient(0);

	pColor[0] = color.r;
	pColor[1] = color.g;
	pColor[2] = color.b;
	pColor[3] = 0.0f;
}

//----------------------------------------------------------------------------//
// Get the diffuse color of the material                                      //
//----------------------------------------------------------------------------//

void CMaxMaterial::GetDiffuseColor(float *pColor)
{
	// get color from the internal standard material
	Color color;
	color = m_pIStdMat->GetDiffuse(0);

	pColor[0] = color.r;
	pColor[1] = color.g;
	pColor[2] = color.b;
	pColor[3] = m_pIStdMat->GetOpacity(0);
}

//----------------------------------------------------------------------------//
// Get the number of maps of the material                                     //
//----------------------------------------------------------------------------//

int CMaxMaterial::GetMapCount()
{
	int mapCount;
	mapCount = 0;

	// loop through all maps of the material
	int mapId;
	for(mapId = 0; mapId < m_pIStdMat->NumSubTexmaps(); mapId++)
	{
		// get the texture map
		Texmap *pTexMap;
		pTexMap = m_pIStdMat->GetSubTexmap(mapId);

		// check if the map is valid
		if((pTexMap != 0) && (m_pIStdMat->MapEnabled(mapId)))
		{
			mapCount++;
		}
	}

	return mapCount;
}

//----------------------------------------------------------------------------//
// Get the filename of the bitmap of the given map                            //
//----------------------------------------------------------------------------//

std::string CMaxMaterial::GetMapFilename(int mapId)
{
	int mapCount;
	mapCount = 0;

	// loop through all maps of the material
	int materialMapId;
	for(materialMapId = 0; materialMapId < m_pIStdMat->NumSubTexmaps(); materialMapId++)
	{
		// get the texture map
		Texmap *pTexMap;
		pTexMap = m_pIStdMat->GetSubTexmap(materialMapId);

		// check if the map is valid
		if((pTexMap != 0) && (m_pIStdMat->MapEnabled(materialMapId)))
		{
			// check if we reached the wanted map
			if(mapId == mapCount)
			{
				if(pTexMap->ClassID() == Class_ID(BMTEX_CLASS_ID, 0))
				{
					// get the full filepath
					std::string strFilename;
					strFilename = ((BitmapTex *)pTexMap)->GetMapName();

					// extract the filename
					std::string::size_type pos;
					pos = strFilename.find_last_of("\\/:");
					if(pos == std::string::npos) return strFilename;

					return strFilename.substr(pos + 1);
				}
				else return "<none>";
			}

			mapCount++;
		}
	}

	return "";
}


//----------------------------------------------------------------------------//
// Get the slot type of the given map                            //
//----------------------------------------------------------------------------//

std::string CMaxMaterial::GetMapType(int mapId)
{
	int mapCount;
	mapCount = 0;

	// loop through all maps of the material
	int materialMapId;
	for(materialMapId = 0; materialMapId < m_pIStdMat->NumSubTexmaps(); materialMapId++)
	{
		// get the texture map
		Texmap *pTexMap;
		pTexMap = m_pIStdMat->GetSubTexmap(materialMapId);

		// check if the map is valid
		if((pTexMap != 0) && (m_pIStdMat->MapEnabled(materialMapId)))
		{
			// check if we reached the wanted map
			if(mapId == mapCount)
			{
				if(pTexMap->ClassID() == Class_ID(BMTEX_CLASS_ID, 0))
				{
					// get the full filepath
					std::string mt = m_pIStdMat->GetSubTexmapSlotName(materialMapId);
          return mt;
				}
				else return "<none>";
			}
			mapCount++;
		}
	}

	return "";
}

//----------------------------------------------------------------------------//
// Get the name of the material                                               //
//----------------------------------------------------------------------------//

std::string CMaxMaterial::GetName()
{
	// check if the internal material is valid
	if(m_pIStdMat == 0) return "<void>";

	const char* name = m_pIStdMat->GetName();
	return name;
}

//----------------------------------------------------------------------------//
// Get the shininess factor of the material                                   //
//----------------------------------------------------------------------------//

float CMaxMaterial::GetShininess()
{
	return m_pIStdMat->GetShinStr(0);
}

//----------------------------------------------------------------------------//
// Get the specular color of the material                                     //
//----------------------------------------------------------------------------//

void CMaxMaterial::GetSpecularColor(float *pColor)
{
	// get color from the internal standard material
	Color color;
	color = m_pIStdMat->GetSpecular(0);

	pColor[0] = color.r;
	pColor[1] = color.g;
	pColor[2] = color.b;
	pColor[3] = 0.0f;
}

//----------------------------------------------------------------------------//
