//----------------------------------------------------------------------------//
// BoneCandidate.cpp                                                          //
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
#include "BoneCandidate.h"
#include "BaseInterface.h"
#include "BaseNode.h"

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

CBoneCandidate::CBoneCandidate()
{
	m_pNode = 0;
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CBoneCandidate::~CBoneCandidate()
{
	delete m_pNode;
}

//----------------------------------------------------------------------------//
// Add a child id to the child id list                                        //
//----------------------------------------------------------------------------//

bool CBoneCandidate::AddChildId(int childId)
{
	m_listChildId.push_back(childId);

	return true;
}

//----------------------------------------------------------------------------//
// Create a bone candidate                                                    //
//----------------------------------------------------------------------------//

bool CBoneCandidate::Create(int id, int parentId, CBaseNode *pNode)
{
	m_id = id;
	m_parentId = parentId;
	m_pNode = pNode;
	m_bSelected = !theExporter.GetInterface()->IsDummy(pNode);

	return true;
}

//----------------------------------------------------------------------------//
// Get the id of the bone candidate                                           //
//----------------------------------------------------------------------------//

int CBoneCandidate::GetId()
{
	return m_id;
}

//----------------------------------------------------------------------------//
// Get the child id list                                                      //
//----------------------------------------------------------------------------//

std::list<int> CBoneCandidate::GetListChildId()
{
	return m_listChildId;
}

//----------------------------------------------------------------------------//
// Get the name of the bone candidate                                         //
//----------------------------------------------------------------------------//

std::string CBoneCandidate::GetName()
{
	// check if the node is valid
	if(m_pNode == 0) return "<null>";

	return m_pNode->GetName();
}

//----------------------------------------------------------------------------//
// Get the node of the bone candidate                                         //
//----------------------------------------------------------------------------//

CBaseNode *CBoneCandidate::GetNode()
{
	return m_pNode;
}

//----------------------------------------------------------------------------//
// Get the id of the parent bone candidate                                    //
//----------------------------------------------------------------------------//

int CBoneCandidate::GetParentId()
{
	return m_parentId;
}

//----------------------------------------------------------------------------//
// Get the select id of the bone candidate                                    //
//----------------------------------------------------------------------------//

int CBoneCandidate::GetSelectedId()
{
	return m_selectedId;
}

//----------------------------------------------------------------------------//
// Get the select state of the bone candidate                                 //
//----------------------------------------------------------------------------//

bool CBoneCandidate::IsSelected()
{
	return m_bSelected;
}

//----------------------------------------------------------------------------//
// Set the select state of the bone candidate                                 //
//----------------------------------------------------------------------------//

void CBoneCandidate::SetSelected(bool bSelected)
{
	m_bSelected = bSelected;
}

//----------------------------------------------------------------------------//
// Set the select id of the bone candidate                                    //
//----------------------------------------------------------------------------//

void CBoneCandidate::SetSelectedId(int selectedId)
{
	m_selectedId = selectedId;
}

//----------------------------------------------------------------------------//
