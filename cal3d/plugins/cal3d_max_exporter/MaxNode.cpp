//----------------------------------------------------------------------------//
// MaxNode.cpp                                                                //
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
#include "MaxNode.h"
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

CMaxNode::CMaxNode()
{
	m_pINode = 0;
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CMaxNode::~CMaxNode()
{
}

//----------------------------------------------------------------------------//
// Create a max node instance                                                 //
//----------------------------------------------------------------------------//

bool CMaxNode::Create(INode *pINode)
{
	if(pINode == 0)
	{
		theExporter.SetLastError("Invalid handle.", __FILE__, __LINE__);
		return false;
	}

	m_pINode = pINode;

	return true;
}

//----------------------------------------------------------------------------//
// Get the child node for a given child node id                               //
//----------------------------------------------------------------------------//

CBaseNode *CMaxNode::GetChild(int childId)
{
	// check if the internal node is valid
	if(m_pINode == 0) return 0;

	// check if the given node id is valid
	if((childId < 0) || (childId >= m_pINode->NumberOfChildren()))
	{
		theExporter.SetLastError("Invalid handle.", __FILE__, __LINE__);
		return 0;
	}

	// allocate a new max node instance
	CMaxNode *pNode;
	pNode = new CMaxNode();
	if(pNode == 0)
	{
		theExporter.SetLastError("Memory allocation failed.", __FILE__, __LINE__);
		return false;
	}

	// create the max node
	if(!pNode->Create(m_pINode->GetChildNode(childId)))
	{
		delete pNode;
		return 0;
	}

	return pNode;
}

//----------------------------------------------------------------------------//
// Get the number of children of the node                                     //
//----------------------------------------------------------------------------//

int CMaxNode::GetChildCount()
{
	// check if the internal node is valid
	if(m_pINode == 0) return 0;

	return m_pINode->NumberOfChildren();
}

//----------------------------------------------------------------------------//
// Get the internal max node of the node                                      //
//----------------------------------------------------------------------------//

INode *CMaxNode::GetINode()
{
	return m_pINode;
}

//----------------------------------------------------------------------------//
// Get the name of the node                                                   //
//----------------------------------------------------------------------------//

std::string CMaxNode::GetName()
{
	// check if the internal node is valid
	if(m_pINode == 0) return "<void>";

	return m_pINode->GetName();
}

//----------------------------------------------------------------------------//
// Get the type of the node                                                   //
//----------------------------------------------------------------------------//

CBaseNode::Type CMaxNode::GetType()
{
	// find the corresponding type of the node
	if(theExporter.GetInterface()->IsBone(this)) return TYPE_BONE;
	else if(theExporter.GetInterface()->IsDummy(this)) return TYPE_DUMMY;
	else if(theExporter.GetInterface()->IsMesh(this)) return TYPE_MESH;

	return TYPE_OTHER;
}

//----------------------------------------------------------------------------//

bool CMaxNode::operator==(const CBaseNode& rhs) const
{
    const CMaxNode* mnode = dynamic_cast<const CMaxNode*>(&rhs);
    if (!mnode)
        return false;

    return m_pINode == mnode->m_pINode;
}
