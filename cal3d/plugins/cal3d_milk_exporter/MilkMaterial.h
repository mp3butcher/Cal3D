//----------------------------------------------------------------------------//
// MilkMaterial.h                                                             //
// Copyright (C) 2002 Bruno 'Beosil' Heidelberger                             //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef MILK_MATERIAL_H
#define MILK_MATERIAL_H

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#include "BaseMaterial.h"

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CMilkMaterial : public CBaseMaterial
{
// member variables
protected:
msMaterial *m_pIMaterial;

// constructors/destructor
public:
	CMilkMaterial();
	virtual ~CMilkMaterial();

// member functions
public:
	bool Create(msMaterial *pIMaterial);
	void GetAmbientColor(float *pColor);
	void GetDiffuseColor(float *pColor);
	int GetMapCount();
	std::string GetMapFilename(int mapId);
	std::string GetName();
	void GetSpecularColor(float *pColor);
	float GetShininess();
};

#endif

//----------------------------------------------------------------------------//
