//----------------------------------------------------------------------------//
// MayaMateial.h                                                              //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger, Thomas Cowell        //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//
#ifndef MAYA_MATERIAL_H
#define MAYA_MATERIAL_H

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//
#include "BaseMaterial.h"

#include <maya/MObject.h>

#include <vector>
#include <string>

class CMayaInterface;


//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//
class CMayaMaterial : public CBaseMaterial
{
public:
	CMayaMaterial ();
	virtual ~CMayaMaterial ();

	bool Create (const MObject &shaderObj);

	void GetAmbientColor(float *pColor);
	void GetDiffuseColor(float *pColor);
	int GetMapCount();
	std::string GetMapFilename(int mapId);
	std::string GetName();
	void GetSpecularColor(float *pColor);
	float GetShininess();

	MObject &GetMayaObject () { return m_mayaObj; }
	
private:
	MObject			m_mayaObj;
	

	float		m_AmbientColor[4];
	float		m_DiffuseColor[4];
	float		m_SpecularColor[4];
	float		m_Shinyness;

	std::string				 m_strName;
	std::vector<std::string> m_Maps;
};

#endif