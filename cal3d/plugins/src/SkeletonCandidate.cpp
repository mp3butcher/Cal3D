//----------------------------------------------------------------------------//
// SkeletonCandidate.cpp                                                      //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#include "StdAfx.h"
#include "Exporter.h"
#include "SkeletonCandidate.h"
#include "BoneCandidate.h"
#include "BaseInterface.h"
#include "BaseNode.h"

//----------------------------------------------------------------------------//
// Constructors                                                               //
//----------------------------------------------------------------------------//

CSkeletonCandidate::CSkeletonCandidate()
{
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CSkeletonCandidate::~CSkeletonCandidate()
{
	Clear();
}

//----------------------------------------------------------------------------//
// Add a node to a given parent node in the hierarchy                         //
//----------------------------------------------------------------------------//

bool CSkeletonCandidate::AddNode(CBaseNode *pNode, int parentId)
{
	// check if the node is valid
	if(pNode == 0)
	{
		theExporter.SetLastError("Invalid handle!", __FILE__, __LINE__);
		return false;
	}

        // Don't add any nodes that have already been added.
        for (size_t i = 0; i < m_vectorBoneCandidate.size(); ++i)
        {
            if (*m_vectorBoneCandidate[i]->GetNode() == *pNode)
            {
                delete pNode;
                return true;
            }
        }

	// if the node does not get used, we have to delete it!
	bool bDeleteNode = true;

	// Check if the node is a candidate
	//We want to be able to export all type of nodes as bones...
	//if(theExporter.GetInterface()->IsBone(pNode) || theExporter.GetInterface()->IsDummy(pNode))
	{
		// allocate a new bone candidate
		CBoneCandidate *pBoneCandidate = new CBoneCandidate;
		if(pBoneCandidate == 0)
		{
			delete pNode;
			theExporter.SetLastError("Memory allocation failed!", __FILE__, __LINE__);
			return false;
		}

		// create the bone candidate
		if(!pBoneCandidate->Create(m_vectorBoneCandidate.size(), parentId, pNode))
		{
			delete pBoneCandidate;
			return false;
		}

		//when the node is a dummy, it's not selected, so select it anyway...
		pBoneCandidate->SetSelected(true);

		// insert node element into hierarchy
		m_vectorBoneCandidate.push_back(pBoneCandidate);

		// insert node element id into parent node element
		if(parentId == -1)
		{
			// no parent -> this is a root node
			m_listRootBoneCandidateId.push_back(pBoneCandidate->GetId());
		}
		else
		{
			// valid parent -> this is a child node
			m_vectorBoneCandidate[parentId]->AddChildId(pBoneCandidate->GetId());
		}

		// set parent id for the children
		parentId = pBoneCandidate->GetId();

		bDeleteNode = false;
	}

	// handle all children of the node
	for(int childId = 0; childId < pNode->GetChildCount(); childId++)
	{
		if(!AddNode(pNode->GetChild(childId), parentId))
		{
			// free the node of needed
			if(bDeleteNode)
			{
				delete pNode;
			}

			return false;
		}
	}

	// free the node of needed
	if(bDeleteNode)
	{
		delete pNode;
	}

	return true;
}

//----------------------------------------------------------------------------//
// Add a core bone to a given parent node in the hierarchy                    //
//----------------------------------------------------------------------------//

bool CSkeletonCandidate::AddNode(CalCoreSkeleton *pCoreSkeleton, CalCoreBone *pCoreBone, int parentId)
{
	// check if the core skeleton and core bone are valid
	if((pCoreSkeleton == 0) || (pCoreBone == 0))
	{
		theExporter.SetLastError("Invalid handle!", __FILE__, __LINE__);
		return false;
	}

	// find the node with the name of the core bone
	CBaseNode *pNode = theExporter.GetInterface()->GetNode(pCoreBone->getName());
	if(pNode == 0)
	{
		theExporter.SetLastError("Skeleton assignment failed!", __FILE__, __LINE__);
		return false;
	}
 
	// check if  the node is a candidate
	//we want to export all types of nodes as bones
	/*if(!theExporter.GetInterface()->IsBone(pNode) && !theExporter.GetInterface()->IsDummy(pNode))
	{
		delete pNode;
		theExporter.SetLastError("Invalid node types in skeleton!", __FILE__, __LINE__);
		return false;
	}
	*/

	// allocate a new bone candidate
	CBoneCandidate *pBoneCandidate = new CBoneCandidate;
	if(pBoneCandidate == 0)
	{
		delete pNode;
		theExporter.SetLastError("Memory allocation failed!", __FILE__, __LINE__);
		return false;
	}

	// create the bone candidate
	if(!pBoneCandidate->Create(m_vectorBoneCandidate.size(), parentId, pNode))
	{
		delete pBoneCandidate;
		return false;
	}

	// insert node element into hierarchy
	m_vectorBoneCandidate.push_back(pBoneCandidate);

	// insert node element id into parent node element
	if(parentId == -1)
	{
		// no parent -> this is a root node
		m_listRootBoneCandidateId.push_back(pBoneCandidate->GetId());
	}
	else
	{
		// valid parent -> this is a child node
		m_vectorBoneCandidate[parentId]->AddChildId(pBoneCandidate->GetId());
	}

	// get core bone vector
	std::vector<CalCoreBone *>& vectorCoreBone = pCoreSkeleton->getVectorCoreBone();

	// handle all children of the core bone
	std::list<int>::iterator iteratorChildId;
	for(iteratorChildId = pCoreBone->getListChildId().begin(); iteratorChildId != pCoreBone->getListChildId().end(); ++iteratorChildId)
	{
		if(!AddNode(pCoreSkeleton, vectorCoreBone[*iteratorChildId], pBoneCandidate->GetId())) return false;
	}

	return true;
}

//----------------------------------------------------------------------------//
// Build all bone candidate ids within the selected ones                      //
//----------------------------------------------------------------------------//

int CSkeletonCandidate::BuildSelectedId()
{
	// destroy all bone candidates stored in this skeleton candidate
	int selectedId = 0;
	for(size_t boneCandidateId = 0; boneCandidateId < m_vectorBoneCandidate.size(); boneCandidateId++)
	{
		if(m_vectorBoneCandidate[boneCandidateId]->IsSelected())
		{
			m_vectorBoneCandidate[boneCandidateId]->SetSelectedId(selectedId);
			selectedId++;
		}
	}

	return selectedId;
}

//----------------------------------------------------------------------------//
// Clear this skeleton candidate                                              //
//----------------------------------------------------------------------------//

void CSkeletonCandidate::Clear()
{
	// destroy all bone candidates stored in this skeleton candidate
	for(size_t boneCandidateId = 0; boneCandidateId < m_vectorBoneCandidate.size(); boneCandidateId++)
	{
		delete m_vectorBoneCandidate[boneCandidateId];
	}
	m_vectorBoneCandidate.clear();

	// clear root bone candidate id list
	m_listRootBoneCandidateId.clear();

	m_strFilename.erase();
}

//----------------------------------------------------------------------------//
// Create a skeleton candidate instance from the exporter interface           //
//----------------------------------------------------------------------------//

bool CSkeletonCandidate::CreateFromInterface()
{
	// clear current content
	Clear();

	// loop through all selected nodes
	for(int nodeId = 0; nodeId < theExporter.GetInterface()->GetSelectedNodeCount(); nodeId++)
	{
		// recursively add the node to the skeleton candidate
		if(!AddNode(theExporter.GetInterface()->GetSelectedNode(nodeId), -1)) return false;
	}

	return true;
}

//----------------------------------------------------------------------------//
// Create a skeleton candidate instance from a skeleton file                  //
//----------------------------------------------------------------------------//

bool CSkeletonCandidate::CreateFromSkeletonFile(const std::string& strFilename)
{
	// clear current content
	Clear();

	m_strFilename = strFilename;

	// create a core model instance
	CalCoreModel coreModel("dummy");

	// load the core skeleton instance
	if(!coreModel.loadCoreSkeleton(m_strFilename))
	{
		theExporter.SetLastErrorFromCal(__FILE__, __LINE__);
		return false;
	}

	// get core skeleton
	CalCoreSkeleton *pCoreSkeleton = coreModel.getCoreSkeleton();
	
	// get core bone vector
	std::vector<CalCoreBone *>& vectorCoreBone = pCoreSkeleton->getVectorCoreBone();

	// loop through all root core bones
	std::list<int>::iterator iteratorRootCoreBoneId;
	for(iteratorRootCoreBoneId = pCoreSkeleton->getListRootCoreBoneId().begin(); iteratorRootCoreBoneId != pCoreSkeleton->getListRootCoreBoneId().end(); ++iteratorRootCoreBoneId)
	{
		// recursively add the core bone to the skeleton candidate
		if(!AddNode(pCoreSkeleton, vectorCoreBone[*iteratorRootCoreBoneId], -1))
		{
			return false;
		}
	}

	return true;
}


//----------------------------------------------------------------------------//
// Get the bone id for a given bone name                                      //
//----------------------------------------------------------------------------//

int CSkeletonCandidate::GetBoneId(const std::string& strName)
{
	// loop through all bone candidates
	for(size_t boneCandidateId = 0; boneCandidateId < m_vectorBoneCandidate.size(); boneCandidateId++)
	{
		// check if the bone candidate is the one we are looking for
		if(m_vectorBoneCandidate[boneCandidateId]->GetName() == strName) return boneCandidateId;
	}

	return -1;
}

//----------------------------------------------------------------------------//
// Get the name of the file from which the skeleton candidate was built from  //
//----------------------------------------------------------------------------//

const std::string& CSkeletonCandidate::GetFilename()
{
	return m_strFilename;
}

//----------------------------------------------------------------------------//
// Get the root bone candidate id list                                        //
//----------------------------------------------------------------------------//

std::list<int> CSkeletonCandidate::GetListRootBoneCandidateId()
{
	return m_listRootBoneCandidateId;
}

//----------------------------------------------------------------------------//
// Get the select id of the parent for a given bone candidate id              //
//----------------------------------------------------------------------------//

int CSkeletonCandidate::GetParentSelectedId(int boneCandidateId)
{
	// check if the bone candidate id is valid
	if((boneCandidateId < 0) || (boneCandidateId >= (int)m_vectorBoneCandidate.size())) return -1;

	// get the bone candidate
	CBoneCandidate *pBoneCandidate = m_vectorBoneCandidate[boneCandidateId];

	// get the parent id
	int parentId = pBoneCandidate->GetParentId();

	// loop until we found the selected parent
	while(parentId != -1)
	{
		// get the parent bone candidate
		pBoneCandidate = m_vectorBoneCandidate[parentId];

		// check id we found a selected parent
		if(pBoneCandidate->IsSelected()) return pBoneCandidate->GetSelectedId();

		// get the parent id
		parentId = pBoneCandidate->GetParentId();
	}

	// bone candidate is at root within selected ones
	return -1;
}

//----------------------------------------------------------------------------//
// Get the number of selected bone candidates                                 //
//----------------------------------------------------------------------------//

int CSkeletonCandidate::GetSelectedCount()
{
	// loop through all bone candidates
	int selectedCount = 0;
	for(size_t boneCandidateId = 0; boneCandidateId < m_vectorBoneCandidate.size(); boneCandidateId++)
	{
		// check if the bone candidate is selected
		if(m_vectorBoneCandidate[boneCandidateId]->IsSelected()) selectedCount++;
	}

	return selectedCount;
}

//----------------------------------------------------------------------------//
// Get the translation and rotation of a given bone candidate at a given time //
//----------------------------------------------------------------------------//

void CSkeletonCandidate::GetTranslationAndRotation(int boneCandidateId, float time, CalVector& translation, CalQuaternion& rotation)
{
	// clear the translation and the rotation
	translation.clear();
	rotation.clear();

	// check if the bone candidate id is valid
	if((boneCandidateId < 0) || (boneCandidateId >= (int)m_vectorBoneCandidate.size())) return;

	// get the bone candidate
	CBoneCandidate *pBoneCandidate = m_vectorBoneCandidate[boneCandidateId];

	// get the node of the bone candidate
	CBaseNode *pNode = pBoneCandidate->GetNode();

	// get the parent id
	int parentId = pBoneCandidate->GetParentId();

	// get the node of the parent bone candidate
	CBaseNode *pParentNode = 0;
	if(parentId != -1)
        {
		pParentNode = m_vectorBoneCandidate[parentId]->GetNode();
	}

  // get the translation and rotation of the node
	theExporter.GetInterface()->GetTranslationAndRotation(pNode, pParentNode, time, translation, rotation);
}

//----------------------------------------------------------------------------//
// Get the bone space transformation of a given bone candidate at a given time //
//----------------------------------------------------------------------------//

void CSkeletonCandidate::GetTranslationAndRotationBoneSpace(int boneCandidateId, float time, CalVector& translation, CalQuaternion& rotation)
{
	// clear the translation and the rotation
	translation.clear();
	rotation.clear();

	// check if the bone candidate id is valid
	if((boneCandidateId < 0) || (boneCandidateId >= (int)m_vectorBoneCandidate.size())) return;

	// get the bone candidate
	CBoneCandidate *pBoneCandidate = m_vectorBoneCandidate[boneCandidateId];

	// get the node of the bone candidate
	CBaseNode *pNode = pBoneCandidate->GetNode();

  // get the bone space translation and rotation of the node
	theExporter.GetInterface()->GetTranslationAndRotationBoneSpace(pNode, time, translation, rotation);
}

//----------------------------------------------------------------------------//
// Get the bone candidate vector                                              //
//----------------------------------------------------------------------------//

std::vector<CBoneCandidate *> CSkeletonCandidate::GetVectorBoneCandidate()
{
	return m_vectorBoneCandidate;
}
