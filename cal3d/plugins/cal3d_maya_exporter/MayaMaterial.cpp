//----------------------------------------------------------------------------//
// MayaMateial.cpp                                                            //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger, Thomas Cowell        //
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
#include "MayaMaterial.h"

#include <maya/MItDependencyGraph.h>
#include <maya/MFnLambertShader.h>
#include <maya/MFnReflectShader.h>
#include <maya/MColor.h>
#include <maya/MPlug.h>

#include "exporter.h"

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
CMayaMaterial::CMayaMaterial()
{
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//
CMayaMaterial::~CMayaMaterial()
{

}

//----------------------------------------------------------------------------//
// Implementation                                                             //
//----------------------------------------------------------------------------//
bool CMayaMaterial::Create (const MObject &shaderObj)
{
	MStatus				status = MS::kSuccess;
	
	m_mayaObj = shaderObj;

	// Look for lambert, because all shaders derive from lambert
	MItDependencyGraph shaderIter(m_mayaObj, MFn::kLambert, MItDependencyGraph::kUpstream, MItDependencyGraph::kDepthFirst, MItDependencyGraph::kNodeLevel, &status);
    if (status == MS::kFailure)
	    return false;

	MFnLambertShader	fnShader (shaderIter.thisNode(), &status);
	if (status == MS::kFailure)
	    return false;
	
	// Get lambert out of the shader
	m_strName = fnShader.name ().asChar();

	MColor clrDiffuse = fnShader.color ();
	m_DiffuseColor[0] = clrDiffuse.r; 
	m_DiffuseColor[1] = clrDiffuse.g;
	m_DiffuseColor[2] = clrDiffuse.b;
	m_DiffuseColor[3] = clrDiffuse.a;

	MColor clrAmbient = fnShader.ambientColor();
	m_AmbientColor[0] = clrAmbient.r; 
	m_AmbientColor[1] = clrAmbient.g;
	m_AmbientColor[2] = clrAmbient.b;
	m_AmbientColor[3] = clrAmbient.a;

	// See if this shader is compatible with a reflectivity shader (phong, blinn), 
	// so we can get speculartiy attributes.
	MFnReflectShader	fnReflectShader (shaderIter.thisNode(), &status);
	if (status == MS::kSuccess)
	{
		MColor clrSpec = fnReflectShader.specularColor ();
		m_SpecularColor[0] = clrSpec.r; 
		m_SpecularColor[1] = clrSpec.g;
		m_SpecularColor[2] = clrSpec.b;
		m_SpecularColor[3] = clrSpec.a;

		// TODO: Get shinyness from maya?
		m_Shinyness = 0;
	}
	else
	{
		// If we arn't reflective, then set some default values for the specularity
		m_Shinyness = 0;
		m_SpecularColor[0] = m_SpecularColor[1] = m_SpecularColor[2] = m_SpecularColor[3] = 1.0f;
	}


	// See if there is a file node attached to the colour plug
	MPlug	colorPlug = fnShader.findPlug("color", &status);
    if (status == MS::kSuccess)
    {
        MItDependencyGraph fileTextureIter(colorPlug.node(), MFn::kFileTexture, 
										  MItDependencyGraph::kUpstream, 
										  MItDependencyGraph::kDepthFirst, 
										  MItDependencyGraph::kNodeLevel, 
										  &status); 
		
        if (status == MS::kSuccess) // There is a filetexture node under this
	    {
            MFnDependencyNode nodeFn(fileTextureIter.thisNode());
		    MPlug plug = nodeFn.findPlug("fileTextureName", &status);
		    if (status == MS::kSuccess)
		    {
                // Get filename
			    MString		MStrFilename;
			    plug.getValue (MStrFilename);

				m_Maps.push_back (MStrFilename.asChar());
            }
        }
    }
	
	return true;
}


void CMayaMaterial::GetAmbientColor(float *pColor)
{
	memcpy (pColor, m_AmbientColor, sizeof (float)*4);
}

void CMayaMaterial::GetDiffuseColor(float *pColor)
{
	memcpy (pColor, m_DiffuseColor, sizeof (float)*4);
}

void CMayaMaterial::GetSpecularColor(float *pColor)
{
	memcpy (pColor, m_SpecularColor, sizeof (float)*4);
}

float CMayaMaterial::GetShininess()
{
	return m_Shinyness;
}

int CMayaMaterial::GetMapCount()
{
	return m_Maps.size ();
}

std::string CMayaMaterial::GetMapFilename(int mapId)
{
	return m_Maps.at (mapId);
}

std::string CMayaMaterial::GetName()
{
	return m_strName;
}

