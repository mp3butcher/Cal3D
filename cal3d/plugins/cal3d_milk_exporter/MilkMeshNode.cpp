//----------------------------------------------------------------------------//
// MilkMeshNode.cpp                                                           //
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
#include "MilkMeshNode.h"
#include "BaseInterface.h"

//----------------------------------------------------------------------------//
// Constructors                                                               //
//----------------------------------------------------------------------------//

CMilkMeshNode::CMilkMeshNode()
{
  m_pIMesh = 0;
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CMilkMeshNode::~CMilkMeshNode()
{
}

//----------------------------------------------------------------------------//
// Create a milk node instance                                                //
//----------------------------------------------------------------------------//

bool CMilkMeshNode::Create(msMesh *pIMesh)
{
	// check if the internal mesh is valid
	if(pIMesh == 0)
	{
		theExporter.SetLastError("Invalid handle.", __FILE__, __LINE__);
		return false;
	}

	m_pIMesh = pIMesh;

	return true;
}

//----------------------------------------------------------------------------//
// Get the child node for a given child node id                               //
//----------------------------------------------------------------------------//

CBaseNode *CMilkMeshNode::GetChild(int childId)
{
  return 0;
}

//----------------------------------------------------------------------------//
// Get the number of children of the node                                     //
//----------------------------------------------------------------------------//

int CMilkMeshNode::GetChildCount()
{
  return 0;
}

//----------------------------------------------------------------------------//
// Get the internal mesh of the node                                          //
//----------------------------------------------------------------------------//

msMesh *CMilkMeshNode::GetIMesh()
{
	return m_pIMesh;
}

//----------------------------------------------------------------------------//
// Get the name of the node                                                   //
//----------------------------------------------------------------------------//

std::string CMilkMeshNode::GetName()
{
	// check if the internal node is valid
	if(m_pIMesh == 0) return "<void>";

	return m_pIMesh->szName;
}

//----------------------------------------------------------------------------//
// Get the type of the node                                                   //
//----------------------------------------------------------------------------//

CBaseNode::Type CMilkMeshNode::GetType()
{
  return TYPE_MESH;
}

//----------------------------------------------------------------------------//

bool CMilkMeshNode::operator==(const CBaseNode& rhs) const
{
    const CMilkMeshNode* milkmesh = dynamic_cast<const CMilkMeshNode*>(&rhs);
    if (!milkmesh)
        return false;

    return m_pIMesh == milkmesh->m_pIMesh;
}
