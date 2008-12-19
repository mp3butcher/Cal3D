//----------------------------------------------------------------------------//
// MaxMaterial.h                                                              //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef MAX_MATERIAL_H
#define MAX_MATERIAL_H

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#include "BaseMaterial.h"

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CMaxMaterial : public CBaseMaterial
{
// member variables
protected:
	StdMat *m_pIStdMat;

// constructors/destructor
public:
	CMaxMaterial();
	virtual ~CMaxMaterial();

// member functions
public:
	bool Create(StdMat *pIStdMat);
	void GetAmbientColor(float *pColor);
	void GetDiffuseColor(float *pColor);
	int GetMapCount();
	std::string GetMapFilename(int mapId);
  std::string GetMapType(int mapId);
	std::string GetName();
	void GetSpecularColor(float *pColor);
	float GetShininess();
};

#endif

//----------------------------------------------------------------------------//
