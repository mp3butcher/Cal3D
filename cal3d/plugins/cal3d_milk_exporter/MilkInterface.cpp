//----------------------------------------------------------------------------//
// MilkInterface.cpp                                                          //
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
#include "MilkInterface.h"
#include "MilkBoneNode.h"
#include "MilkMeshNode.h"
#include "MilkMesh.h"
#include "MilkMaterial.h"

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

CMilkInterface::CMilkInterface()
{
  m_pIModel = 0;
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CMilkInterface::~CMilkInterface()
{
}

//----------------------------------------------------------------------------//
// Create a milk interface instance                                           //
//----------------------------------------------------------------------------//

bool CMilkInterface::Create(msModel *pIModel)
{
  m_pIModel = pIModel;

  // search for root bones
  int boneId;
  for(boneId = 0; boneId < msModel_GetBoneCount(pIModel); boneId++)
  {
    // get the internal bone
    msBone *pIBone;
    pIBone = msModel_GetBoneAt(pIModel, boneId);

    // check if it is a root bone (= empty parent name)
    if(strlen(pIBone->szParentName) == 0)
    {
      // add the bone id to the root bone id vector
      m_vectorRootBoneId.push_back(boneId);
    }
  }

  // search for selected meshes
  int meshId;
  for(meshId = 0; meshId < msModel_GetMeshCount(pIModel); meshId++)
  {
    // get the internal mesh
    msMesh *pIMesh;
    pIMesh = msModel_GetMeshAt(pIModel, meshId);

    // check if the mesh is selected
    if(pIMesh->nFlags & (eSelected | eSelected2))
    {
      // add the mesh id to the selected mesh id vector
      m_vectorSelectedMeshId.push_back(meshId);
    }
  }

	return true;
}

//----------------------------------------------------------------------------//
// Get the current frame of the animation                                     //
//----------------------------------------------------------------------------//

int CMilkInterface::GetCurrentFrame()
{
  return msModel_GetFrame(m_pIModel) - 1;
}

//----------------------------------------------------------------------------//
// Get the current time in seconds of the animation                           //
//----------------------------------------------------------------------------//

float CMilkInterface::GetCurrentTime()
{
  return (float)(msModel_GetFrame(m_pIModel) - 1) / (float)GetFps();
}

//----------------------------------------------------------------------------//
// Get the end frame of the animation                                         //
//----------------------------------------------------------------------------//

int CMilkInterface::GetEndFrame()
{
  return msModel_GetTotalFrames(m_pIModel) - 1;
}

//----------------------------------------------------------------------------//
// Get the frames per second (Fps) of the animation                           //
//----------------------------------------------------------------------------//

int CMilkInterface::GetFps()
{
  // TODO: We need access to this value through the MilkShape API!
  return 24;
}

//----------------------------------------------------------------------------//
// Get the internal model                                                     //
//----------------------------------------------------------------------------//

msModel *CMilkInterface::GetIModel()
{
  return m_pIModel;
}

//----------------------------------------------------------------------------//
// Get the main window of the host application                                //
//----------------------------------------------------------------------------//

CWnd *CMilkInterface::GetMainWnd()
{
  return NULL;
}

//----------------------------------------------------------------------------//
// Get the material for a given id                                            //
//----------------------------------------------------------------------------//

CBaseMaterial *CMilkInterface::GetMaterial(int materialId)
{
  // check if the material id is valid
  if((materialId < 0) || (materialId >= msModel_GetMaterialCount(m_pIModel)))
  {
	  theExporter.SetLastError("Invalid handle.", __FILE__, __LINE__);
	  return 0;
  }

	// allocate a new milk material instance
	CMilkMaterial *pMilkMaterial;
	pMilkMaterial = new CMilkMaterial();
	if(pMilkMaterial == 0)
	{
		theExporter.SetLastError("Memory allocation failed.", __FILE__, __LINE__);
		return 0;
	}

	// create the milk material
	if(!pMilkMaterial->Create(msModel_GetMaterialAt(m_pIModel, materialId)))
	{
		delete pMilkMaterial;
		return 0;
	}

	return pMilkMaterial;
}

//----------------------------------------------------------------------------//
// Get the number of materials in the scene                                   //
//----------------------------------------------------------------------------//

int CMilkInterface::GetMaterialCount()
{
  return msModel_GetMaterialCount(m_pIModel);
}

//----------------------------------------------------------------------------//
// Get the mesh of a given node                                               //
//----------------------------------------------------------------------------//

CBaseMesh *CMilkInterface::GetMesh(CBaseNode *pNode)
{
	// check for invalid nodes
  if((pNode == 0) || (pNode->GetType() != CBaseNode::TYPE_MESH))
	{
		theExporter.SetLastError("Invalid handle.", __FILE__, __LINE__);
		return 0;
	}
	
	// downcast the node to a milk mesh node
	CMilkMeshNode *pMilkMeshNode;
	pMilkMeshNode = dynamic_cast<CMilkMeshNode *>(pNode);

	// allocate a new milk mesh instance
	CMilkMesh *pMilkMesh;
	pMilkMesh = new CMilkMesh();
	if(pMilkMesh == 0)
	{
		theExporter.SetLastError("Memory allocation failed.", __FILE__, __LINE__);
		return 0;
	}

	// create the milk mesh
	if(!pMilkMesh->Create(pMilkMeshNode->GetIMesh()))
	{
		delete pMilkMesh;
		return 0;
	}

	return pMilkMesh;
}

//----------------------------------------------------------------------------//
// Get the node with a given name                                             //
//----------------------------------------------------------------------------//

CBaseNode *CMilkInterface::GetNode(const std::string& strName)
{
  // search the bone nodes first
  int nodeId;
  nodeId = msModel_FindBoneByName(m_pIModel, strName.c_str());

  // check if we found a bone node
  if((nodeId >= 0) && (nodeId < msModel_GetBoneCount(m_pIModel)))
  {
	  // allocate a new milk bone node instance
	  CMilkBoneNode *pNode;
	  pNode = new CMilkBoneNode();
	  if(pNode == 0)
	  {
		  theExporter.SetLastError("Memory allocation failed.", __FILE__, __LINE__);
		  return 0;
	  }

	  // create the milk bone node
	  if(!pNode->Create(msModel_GetBoneAt(m_pIModel, nodeId)))
	  {
		  delete pNode;
		  return 0;
	  }

  	return pNode;
  }

  // search the mesh nodes next
  nodeId = msModel_FindMeshByName(m_pIModel, strName.c_str());

  // check if we found a mesh node
  if((nodeId >= 0) && (nodeId < msModel_GetMeshCount(m_pIModel)))
  {
	  // allocate a new milk mesh node instance
	  CMilkMeshNode *pNode;
	  pNode = new CMilkMeshNode();
	  if(pNode == 0)
	  {
		  theExporter.SetLastError("Memory allocation failed.", __FILE__, __LINE__);
		  return 0;
	  }

	  // create the milk mesh node
	  if(!pNode->Create(msModel_GetMeshAt(m_pIModel, nodeId)))
	  {
		  delete pNode;
		  return 0;
	  }

  	return pNode;
  }

  return 0;
}

//----------------------------------------------------------------------------//
// Get the number of selected nodes in the scene                              //
//----------------------------------------------------------------------------//

int CMilkInterface::GetSelectedNodeCount()
{
  // REMEMBER: root bones are always selected for milkshape
  return m_vectorRootBoneId.size() + m_vectorSelectedMeshId.size();
}

//----------------------------------------------------------------------------//
// Get the node for a given node id                                           //
//----------------------------------------------------------------------------//

CBaseNode *CMilkInterface::GetSelectedNode(int nodeId)
{
	// check if the given node id is valid
	if((nodeId < 0) || (nodeId >= GetSelectedNodeCount()))
	{
		theExporter.SetLastError("Invalid handle.", __FILE__, __LINE__);
		return 0;
	}

  // handle bone and mesh nodes separately
  if(nodeId < (int)m_vectorRootBoneId.size())
  {
	  // allocate a new milk bone node instance
	  CMilkBoneNode *pNode;
	  pNode = new CMilkBoneNode();
	  if(pNode == 0)
	  {
		  theExporter.SetLastError("Memory allocation failed.", __FILE__, __LINE__);
		  return 0;
	  }

	  // create the milk bone node
	  if(!pNode->Create(msModel_GetBoneAt(m_pIModel, m_vectorRootBoneId[nodeId])))
	  {
		  delete pNode;
		  return 0;
	  }

  	return pNode;
  }
  else
  {
	  // allocate a new milk mesh node instance
	  CMilkMeshNode *pNode;
	  pNode = new CMilkMeshNode();
	  if(pNode == 0)
	  {
		  theExporter.SetLastError("Memory allocation failed.", __FILE__, __LINE__);
		  return 0;
	  }

	  // create the milk mesh node
	  if(!pNode->Create(msModel_GetMeshAt(m_pIModel, m_vectorSelectedMeshId[nodeId - m_vectorRootBoneId.size()])))
	  {
		  delete pNode;
		  return 0;
	  }

  	return pNode;
  }

  return 0;
}

//----------------------------------------------------------------------------//
// Get the start frame of the animation                                       //
//----------------------------------------------------------------------------//

int CMilkInterface::GetStartFrame()
{
  return 0;
}

//----------------------------------------------------------------------------//
// Get the relative translation and rotation of a given node at a given time  //
//----------------------------------------------------------------------------//

void CMilkInterface::GetTranslationAndRotation(CBaseNode *pNode, CBaseNode *pParentNode, float time, CalVector& translation, CalQuaternion& rotation)
{
	// check for invalid nodes
  if((pNode == 0) || (pNode->GetType() != CBaseNode::TYPE_BONE)) return;

	// downcast the node to a milk bone node
	CMilkBoneNode *pBoneNode;
	pBoneNode = dynamic_cast<CMilkBoneNode *>(pNode);

	// downcast the parent node to a milk bone  node
	CMilkBoneNode *pParentBoneNode;
	if((pParentNode == 0) || (pParentNode->GetType() != CBaseNode::TYPE_BONE))
	{
		pParentBoneNode = 0;
	}
	else
	{
		pParentBoneNode = dynamic_cast<CMilkBoneNode *>(pParentNode);
	}

	// calculate the relative transformation
  translation = pBoneNode->GetRelativeTranslation(time);
  rotation = pBoneNode->GetRelativeRotation(time);
}

//----------------------------------------------------------------------------//
// Get the bone space transformation of a given node at a given time          //
//----------------------------------------------------------------------------//

void CMilkInterface::GetTranslationAndRotationBoneSpace(CBaseNode *pNode, float time, CalVector& translation, CalQuaternion& rotation)
{
	// check for invalid nodes
  if((pNode == 0) || (pNode->GetType() != CBaseNode::TYPE_BONE)) return;

	// downcast the node to a milk bone node
	CMilkBoneNode *pBoneNode;
	pBoneNode = dynamic_cast<CMilkBoneNode *>(pNode);

	// calculate the absolute transformation
	translation = pBoneNode->GetRelativeTranslation(time);
  rotation = pBoneNode->GetRelativeRotation(time);

  msBone *pBone;
  pBone = pBoneNode->GetIBone();

  // build the full transformation from the relative transformation of all parents
  while((pBone != 0) && (strlen(pBone->szParentName) > 0))
  {
    pNode = GetNode(pBone->szParentName);
    if((pNode == 0) || (pNode->GetType() != CBaseNode::TYPE_BONE)) break;

    pBoneNode = dynamic_cast<CMilkBoneNode *>(pNode);

    translation *= pBoneNode->GetRelativeRotation(time);
    translation += pBoneNode->GetRelativeTranslation(time);
    rotation *= pBoneNode->GetRelativeRotation(time);

    pBone = pBoneNode->GetIBone();

    delete pNode;
  }

  // invert the absolute transformation to get the bone space transformation
  rotation.conjugate();
  translation *= -1.0f;
  translation *= rotation;
}

//----------------------------------------------------------------------------//
// Check if the given node is a bone                                          //
//----------------------------------------------------------------------------//

bool CMilkInterface::IsBone(CBaseNode *pNode)
{
	// check for invalid nodes
	if(pNode == 0) return false;

  if(pNode->GetType() == CBaseNode::TYPE_BONE) return true;

	return false;
}

//----------------------------------------------------------------------------//
// Check if the given node is a dummy                                         //
//----------------------------------------------------------------------------//

bool CMilkInterface::IsDummy(CBaseNode *pNode)
{
	// check for invalid nodes
	if(pNode == 0) return false;

  if(pNode->GetType() == CBaseNode::TYPE_DUMMY) return true;

  return false;
}

//----------------------------------------------------------------------------//
// Check if the given node is a mesh                                          //
//----------------------------------------------------------------------------//

bool CMilkInterface::IsMesh(CBaseNode *pNode)
{
	// check for invalid nodes
	if(pNode == 0) return false;

  if(pNode->GetType() == CBaseNode::TYPE_MESH) return true;

  return false;
}

//----------------------------------------------------------------------------//
// Progress callback function                                                 //
//----------------------------------------------------------------------------//

DWORD WINAPI CMilkInterface::ProgressFunction(LPVOID arg)
{
	return 0;
}

//----------------------------------------------------------------------------//
// Set the progress information to a given text and percentage                //
//----------------------------------------------------------------------------//

void CMilkInterface::SetProgressInfo(int percentage)
{
}

//----------------------------------------------------------------------------//
// Start the progress information process                                     //
//----------------------------------------------------------------------------//

void CMilkInterface::StartProgressInfo(const std::string& strText)
{
}

//----------------------------------------------------------------------------//
// Stop the progress information process                                      //
//----------------------------------------------------------------------------//

void CMilkInterface::StopProgressInfo()
{
}

//----------------------------------------------------------------------------//
