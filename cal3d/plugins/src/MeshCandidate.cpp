//----------------------------------------------------------------------------//
// MeshCandidate.cpp                                                          //
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
#include "MeshCandidate.h"
#include "SubmeshCandidate.h"
#include "BaseInterface.h"
#include "BaseMesh.h"
#include "BaseNode.h"
#include "VertexCandidate.h"

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

CMeshCandidate::CMeshCandidate()
{
	m_pNode = 0;
	m_pMesh = 0;
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CMeshCandidate::~CMeshCandidate()
{
	Clear();
}

//----------------------------------------------------------------------------//
// Calculate the LOD for the whole mesh candidate                             //
//----------------------------------------------------------------------------//

bool CMeshCandidate::CalculateLOD()
{
	// calculate the LOD for all the submesh candidates
	for(size_t submeshCandidateId = 0; submeshCandidateId < m_vectorSubmeshCandidate.size(); submeshCandidateId++)
	{
		// only handle non-empty submesh candidates
		if(m_vectorSubmeshCandidate[submeshCandidateId]->GetFaceCount() > 0)
		{
			if(!m_vectorSubmeshCandidate[submeshCandidateId]->CalculateLOD()) return false;
		}
	}
	
	return true;
}

//----------------------------------------------------------------------------//
// Calculate possible spring systems for the whole mesh candidate             //
//----------------------------------------------------------------------------//

bool CMeshCandidate::CalculateSpringSystem()
{
	// calculate the LOD for all the submesh candidates
	for(size_t submeshCandidateId = 0; submeshCandidateId < m_vectorSubmeshCandidate.size(); submeshCandidateId++)
	{
		// only handle non-empty submesh candidates
		if(m_vectorSubmeshCandidate[submeshCandidateId]->GetFaceCount() > 0)
		{
			if(!m_vectorSubmeshCandidate[submeshCandidateId]->CalculateSpringSystem()) return false;
		}
	}
	
	return true;
}

//----------------------------------------------------------------------------//
// Clear this mesh candidate                                                  //
//----------------------------------------------------------------------------//

void CMeshCandidate::Clear()
{
	// destroy all submesh candidates stored in this mesh candidate
	for(size_t submeshCandidateId = 0; submeshCandidateId < m_vectorSubmeshCandidate.size(); submeshCandidateId++)
	{
		delete m_vectorSubmeshCandidate[submeshCandidateId];
	}
	m_vectorSubmeshCandidate.clear();

	delete m_pNode;
  m_pNode = 0;

	delete m_pMesh;
  m_pMesh = 0;
}

//----------------------------------------------------------------------------//
// Create a mesh candidate                                                    //
//----------------------------------------------------------------------------//

bool CMeshCandidate::Create(CSkeletonCandidate *pSkeletonCandidate, int maxBoneCount, float weightThreshold)
{
	// clear current content
	Clear();

  int meshCount;
  meshCount = 0;

  // loop through all the selected nodes
  int nodeId;
  for(nodeId = 0; nodeId < theExporter.GetInterface()->GetSelectedNodeCount(); nodeId++)
  {
    // get the selected node
    CBaseNode *pNode;
    pNode = theExporter.GetInterface()->GetSelectedNode(nodeId);

    // check if it is a mesh node
    if(theExporter.GetInterface()->IsMesh(pNode))
    {
      if(meshCount == 0) m_pNode = pNode;
      meshCount++;
    }

    // delete all nodes except the one that should be exported
    if(pNode != m_pNode) delete pNode;
  }

	// check if one (and only one!) node/mesh is selected
	if(meshCount != 1)
	{
		theExporter.SetLastError("Select one (and only one) mesh to export!", __FILE__, __LINE__);
		return false;
	}

	// check if the node is a mesh candidate
	if((m_pNode == 0) || !theExporter.GetInterface()->IsMesh(m_pNode))
	{
		theExporter.SetLastError("No mesh selected!", __FILE__, __LINE__);
		return false;
	}

	// get the mesh
	m_pMesh = theExporter.GetInterface()->GetMesh(m_pNode);
	if(m_pMesh == 0) return false;

	// get the number of materials of the mesh
	int materialCount;
	materialCount = m_pMesh->GetMaterialCount();

	// create all the submesh candidates
	int submeshId;
	for(submeshId = 0; submeshId < materialCount; submeshId++)
	{
		// allocate a new submesh candidate
		CSubmeshCandidate *pSubmeshCandidate;
		pSubmeshCandidate = new CSubmeshCandidate();
		if(pSubmeshCandidate == 0)
		{
			theExporter.SetLastError("Memory allocation failed!", __FILE__, __LINE__);
			return false;
		}

		// create the new submesh candidate
		if(!pSubmeshCandidate->Create(m_pMesh->GetSubmeshMaterialThreadId(submeshId), m_pMesh->GetSubmeshMapCount(submeshId))) return false;

		// add submesh candidate to the submesh candidate vector
		m_vectorSubmeshCandidate.push_back(pSubmeshCandidate);
	}

	// get the number of faces of the mesh
	int faceCount;
	faceCount = m_pMesh->GetFaceCount();

	// start the progress info
	theExporter.GetInterface()->StartProgressInfo("Analyzing the mesh...");

	// loop through all faces of the mesh and put it into the corresponding submesh
	int faceId;
	for(faceId = 0; faceId < faceCount; faceId++)
	{
		// update the progress info
		theExporter.GetInterface()->SetProgressInfo(int(100.0f * (float)faceId / (float)faceCount));

		// get the material id of the face
		int materialId;
		materialId = m_pMesh->GetFaceMaterialId(faceId);
		if(materialId == -1)
		{
			theExporter.GetInterface()->StopProgressInfo();
			return false;
		}

		// check if the material id is valid
		if((materialId < 0) || (materialId >= materialCount))
		{
			theExporter.GetInterface()->StopProgressInfo();
			theExporter.SetLastError("Invalid material id found!", __FILE__, __LINE__);
			return false;
		}

		// get the first vertex of the face
		CVertexCandidate *pVertexCandidate;
		pVertexCandidate = m_pMesh->GetVertexCandidate(pSkeletonCandidate, faceId, 0);
    if(pVertexCandidate == 0) continue;

		// add it to the corresponding submesh
		int vertexId1;
		vertexId1 = m_vectorSubmeshCandidate[materialId]->AddVertexCandidate(pVertexCandidate);

		// get the second vertex of the face
		pVertexCandidate = m_pMesh->GetVertexCandidate(pSkeletonCandidate, faceId, 1);

		// add it to the corresponding submesh
		int vertexId2;
		vertexId2 = m_vectorSubmeshCandidate[materialId]->AddVertexCandidate(pVertexCandidate);

		// get the third vertex of the face
		pVertexCandidate = m_pMesh->GetVertexCandidate(pSkeletonCandidate, faceId, 2);

		// add it to the corresponding submesh
		int vertexId3;
		vertexId3 = m_vectorSubmeshCandidate[materialId]->AddVertexCandidate(pVertexCandidate);

		if((vertexId1 == -1) ||(vertexId2 == -1) ||(vertexId3 == -1))
		{
			theExporter.GetInterface()->StopProgressInfo();
			return false;
		}

		// add the face to the corresponding submesh
		if(!m_vectorSubmeshCandidate[materialId]->AddFace(vertexId1, vertexId2, vertexId3))
		{
			theExporter.GetInterface()->StopProgressInfo();
			return false;
		}
	}

	// stop the progress info
	theExporter.GetInterface()->StopProgressInfo();

	// adjust all bone assignments in the submesh candidates
	for(submeshId = 0; submeshId < int(m_vectorSubmeshCandidate.size()); submeshId++)
	{
		m_vectorSubmeshCandidate[submeshId]->AdjustBoneAssignment(maxBoneCount, weightThreshold);
	}


	return true;
}

//----------------------------------------------------------------------------//
// Disable the LOD for the whole mesh candidate                               //
//----------------------------------------------------------------------------//

bool CMeshCandidate::DisableLOD()
{
	// disable the LOD for all the submesh candidates
	for(size_t submeshCandidateId = 0; submeshCandidateId < m_vectorSubmeshCandidate.size(); submeshCandidateId++)
	{
		// only handle non-empty submesh candidates
		if(m_vectorSubmeshCandidate[submeshCandidateId]->GetFaceCount() > 0)
		{
			if(!m_vectorSubmeshCandidate[submeshCandidateId]->DisableLOD()) return false;
		}
	}
	
	return true;
}

//----------------------------------------------------------------------------//
// Get the submesh candidate vector                                           //
//----------------------------------------------------------------------------//

std::vector<CSubmeshCandidate *>& CMeshCandidate::GetVectorSubmeshCandidate()
{
	return m_vectorSubmeshCandidate;
}

//----------------------------------------------------------------------------//
