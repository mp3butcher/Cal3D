//----------------------------------------------------------------------------//
// MayaNode.cpp                                                               //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger, Thomas Cowell        //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//
#include "StdAfx.h"
#include "MayaNode.h"

#include <maya/MFnDagNode.h>

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
CMayaNode::CMayaNode()
{
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//
CMayaNode::~CMayaNode()
{

}

//----------------------------------------------------------------------------//
// Implementation                                                             //
//----------------------------------------------------------------------------//
bool	CMayaNode::Create (MDagPath &path)
{
	m_dagPath = path;
	
	return true;
}

CBaseNode *CMayaNode::GetChild(int childId)
{
	MStatus status;
	MFnDagNode dagNode (m_dagPath, &status);
	if (status != MS::kSuccess)
		return 0;

	MObject objChild = dagNode.child (childId, &status);
	if (status != MS::kSuccess)
		return 0;

	MFnDagNode	childDagNode (objChild, &status);	
	if (status != MS::kSuccess)
		return 0;

	MDagPath childPath; 
	if (childDagNode.getPath (childPath) != MS::kSuccess)
		return 0;

	CMayaNode *pNode = new CMayaNode;
	if (!pNode->Create (childPath))
	{
		delete pNode;
		return 0;
	}

	return pNode;
}

int CMayaNode::GetChildCount()
{
	MStatus status;
	MFnDagNode dagNode (m_dagPath, &status);
	if (status != MS::kSuccess)
		return 0;

	return dagNode.childCount();
}

std::string CMayaNode::GetName()
{
	MStatus status;
	MFnDependencyNode	depNode (m_dagPath.node(), &status);
	if (status != MS::kSuccess)
		return "";

	return depNode.name ().asChar();
}

CBaseNode::Type CMayaNode::GetType()
{
	if (m_dagPath.hasFn (MFn::kMesh))
		return CBaseNode::TYPE_MESH;
	else
	if (m_dagPath.hasFn (MFn::kJoint))
		return CBaseNode::TYPE_BONE;
	else
	if (m_dagPath.hasFn (MFn::kLocator))
		return CBaseNode::TYPE_DUMMY;
	else
		return CBaseNode::TYPE_OTHER;
}

bool CMayaNode::operator==(const CBaseNode& rhs) const
{
    const CMayaNode* mnode = dynamic_cast<const CMayaNode*>(&rhs);
    if (!mnode)
        return false;

    return m_dagPath == mnode->m_dagPath;
}
