//----------------------------------------------------------------------------//
// MilkMaterial.cpp                                                           //
// Copyright (C) 2002 Bruno 'Beosil' Heidelberger                             //
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
#include "MilkMaterial.h"
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

CMilkMaterial::CMilkMaterial()
{
	m_pIMaterial = 0;
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CMilkMaterial::~CMilkMaterial()
{
}

//----------------------------------------------------------------------------//
// Create a milk material instance                                            //
//----------------------------------------------------------------------------//

bool CMilkMaterial::Create(msMaterial *pIMaterial)
{
	// check if the internal material is valid
	if(pIMaterial == 0)
	{
		theExporter.SetLastError("Invalid handle.", __FILE__, __LINE__);
		return false;
	}

	m_pIMaterial = pIMaterial;

	return true;
}

//----------------------------------------------------------------------------//
// Get the ambient color of the material                                      //
//----------------------------------------------------------------------------//

void CMilkMaterial::GetAmbientColor(float *pColor)
{
	// get color from the internal material
	msVec4 color;
	msMaterial_GetAmbient(m_pIMaterial, color);

	pColor[0] = color[0];
	pColor[1] = color[1];
	pColor[2] = color[2];
	pColor[3] = color[3];
}

//----------------------------------------------------------------------------//
// Get the diffuse color of the material                                      //
//----------------------------------------------------------------------------//

void CMilkMaterial::GetDiffuseColor(float *pColor)
{
	// get color from the internal material
	msVec4 color;
	msMaterial_GetDiffuse(m_pIMaterial, color);

	pColor[0] = color[0];
	pColor[1] = color[1];
	pColor[2] = color[2];
	pColor[3] = color[3];
}

//----------------------------------------------------------------------------//
// Get the number of maps of the material                                     //
//----------------------------------------------------------------------------//

int CMilkMaterial::GetMapCount()
{
	int mapCount;
	mapCount = 0;

	// check all maps of the material
  if(strlen(m_pIMaterial->szDiffuseTexture) > 0) mapCount++;
  if(strlen(m_pIMaterial->szAlphaTexture) > 0) mapCount++;

	return mapCount;
}

//----------------------------------------------------------------------------//
// Get the filename of the bitmap of the given map                            //
//----------------------------------------------------------------------------//

std::string CMilkMaterial::GetMapFilename(int mapId)
{
  switch(mapId)
  {
  case 0:
    return m_pIMaterial->szDiffuseTexture;
    break;
  case 1:
    return m_pIMaterial->szAlphaTexture;
    break;
  }

  return "";
}

//----------------------------------------------------------------------------//
// Get the name of the material                                               //
//----------------------------------------------------------------------------//

std::string CMilkMaterial::GetName()
{
	// check if the internal material is valid
	if(m_pIMaterial == 0) return "<void>";

	return m_pIMaterial->szName;
}

//----------------------------------------------------------------------------//
// Get the shininess factor of the material                                   //
//----------------------------------------------------------------------------//

float CMilkMaterial::GetShininess()
{
	return msMaterial_GetShininess(m_pIMaterial);
}

//----------------------------------------------------------------------------//
// Get the specular color of the material                                     //
//----------------------------------------------------------------------------//

void CMilkMaterial::GetSpecularColor(float *pColor)
{
	// get color from the internal material
	msVec4 color;
	msMaterial_GetSpecular(m_pIMaterial, color);

	pColor[0] = color[0];
	pColor[1] = color[1];
	pColor[2] = color[2];
	pColor[3] = color[3];
}

//----------------------------------------------------------------------------//
