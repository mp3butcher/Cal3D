//----------------------------------------------------------------------------//
// MeshCandidate.cpp                                                          //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//


#include "StdAfx.h"
#include "Exporter.h"
#include "MeshCandidate.h"
#include "SubmeshCandidate.h"
#include "BaseInterface.h"
#include "BaseMesh.h"
#include "BaseNode.h"
#include "VertexCandidate.h"


CMeshCandidate::CMeshCandidate()
{
}


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

  for (size_t i = 0; i < m_meshes.size(); ++i)
  {
    delete m_meshes[i];
  }
  m_meshes.clear();
}

//----------------------------------------------------------------------------//
// Create a mesh candidate                                                    //
//----------------------------------------------------------------------------//

bool CMeshCandidate::Create(CSkeletonCandidate *pSkeletonCandidate, int maxBoneCount, float weightThreshold)
{
  size_t i;
  // clear current content
  Clear();

  // loop through all the selected nodes
  for(int nodeId = 0; nodeId < theExporter.GetInterface()->GetSelectedNodeCount(); nodeId++)
  {
    // get the selected node
    CBaseNode* node = theExporter.GetInterface()->GetSelectedNode(nodeId);

    // check if it is a mesh node
    if(theExporter.GetInterface()->IsMesh(node))
    {
      CBaseMesh* mesh = theExporter.GetInterface()->GetMesh(node);
      if (mesh)
        m_meshes.push_back(mesh);
    }
  }

  // Check if any meshes are selected.
  if(m_meshes.empty())
  {
    theExporter.SetLastError("No meshes were selected for export!", __FILE__, __LINE__);
    return false;
  }

  for (i = 0; i < m_meshes.size(); ++i)
  {
    CBaseMesh* mesh = m_meshes[i];

    // Get the number of materials in this mesh.
    int materialCount = mesh->GetMaterialCount();

    // Create all of the submesh candidates.
    for(int submeshId = 0; submeshId < materialCount; submeshId++)
    {
      // allocate a new submesh candidate
      CSubmeshCandidate *pSubmeshCandidate = new CSubmeshCandidate();

      // create the new submesh candidate
      if(!pSubmeshCandidate->Create(mesh->GetSubmeshMaterialThreadId(submeshId), mesh->GetSubmeshMapCount(submeshId)))
        return false;

      // add submesh candidate to the submesh candidate vector
      m_vectorSubmeshCandidate.push_back(pSubmeshCandidate);
    }
  }

  int baseSubmesh = 0;
  for (i = 0; i < m_meshes.size(); ++i)
  {
    CBaseMesh* mesh = m_meshes[i];
    int materialCount = mesh->GetMaterialCount();

    // start the progress info
    theExporter.GetInterface()->StartProgressInfo("Analyzing the mesh...");

    // loop through all faces of the mesh and put it into the corresponding submesh
    int faceCount = mesh->GetFaceCount();
    for(int faceId = 0; faceId < faceCount; faceId++)
    {
      // update the progress info
      theExporter.GetInterface()->SetProgressInfo(int(100.0f * (float)faceId / (float)faceCount));

      // get the material id of the face
      int materialId = mesh->GetFaceMaterialId(faceId);
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

      CSubmeshCandidate* submeshCandidate = m_vectorSubmeshCandidate[materialId + baseSubmesh];

      // get the first vertex of the face
      CVertexCandidate *pVertexCandidate = mesh->GetVertexCandidate(pSkeletonCandidate, faceId, 0);
      if(pVertexCandidate == 0) continue;

      // add it to the corresponding submesh
      int vertexId1 = submeshCandidate->AddVertexCandidate(pVertexCandidate);

      // get the second vertex of the face
      pVertexCandidate = mesh->GetVertexCandidate(pSkeletonCandidate, faceId, 1);
      assert(pVertexCandidate);

      // add it to the corresponding submesh
      int vertexId2 = submeshCandidate->AddVertexCandidate(pVertexCandidate);

      // get the third vertex of the face
      pVertexCandidate = mesh->GetVertexCandidate(pSkeletonCandidate, faceId, 2);
      assert(pVertexCandidate);

      // add it to the corresponding submesh
      int vertexId3 = submeshCandidate->AddVertexCandidate(pVertexCandidate);

      if((vertexId1 == -1) ||(vertexId2 == -1) ||(vertexId3 == -1))
      {
        theExporter.GetInterface()->StopProgressInfo();
        return false;
      }

      // add the face to the corresponding submesh
      if(!submeshCandidate->AddFace(vertexId1, vertexId2, vertexId3))
      {
        theExporter.GetInterface()->StopProgressInfo();
        return false;
      }
    }

    // stop the progress info
    theExporter.GetInterface()->StopProgressInfo();

    baseSubmesh += materialCount;
  }

  // adjust all bone assignments in the submesh candidates
  for(i = 0; i < m_vectorSubmeshCandidate.size(); ++i)
  {
    m_vectorSubmeshCandidate[i]->AdjustBoneAssignment(maxBoneCount, weightThreshold);
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

//----------------------------------------------------------------------------//
// Create a mesh candidate                                                    //
//----------------------------------------------------------------------------//
bool CMeshCandidate::Create(CBaseNode* _basenode, CSkeletonCandidate *pSkeletonCandidate, int maxBoneCount, float weightThreshold)
{
	// clear current content
	Clear();

	//Directly set the meshnode into the mesh candidate
	// get the mesh
	CBaseMesh* mesh = theExporter.GetInterface()->GetMesh(_basenode);
	if(mesh == 0) return false;

        m_meshes.push_back(mesh);

	// get the number of materials of the mesh
	int materialCount;
	materialCount = mesh->GetMaterialCount();

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
		if(!pSubmeshCandidate->Create(mesh->GetSubmeshMaterialThreadId(submeshId), mesh->GetSubmeshMapCount(submeshId))) return false;

		// add submesh candidate to the submesh candidate vector
		m_vectorSubmeshCandidate.push_back(pSubmeshCandidate);
	}

	// get the number of faces of the mesh
	int faceCount;
	faceCount = mesh->GetFaceCount();

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
		materialId = mesh->GetFaceMaterialId(faceId);
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
		pVertexCandidate = mesh->GetVertexCandidate(pSkeletonCandidate, faceId, 0);
    if(pVertexCandidate == 0) continue;

		// add it to the corresponding submesh
		int vertexId1;
		vertexId1 = m_vectorSubmeshCandidate[materialId]->AddVertexCandidate(pVertexCandidate);

		// get the second vertex of the face
		pVertexCandidate = mesh->GetVertexCandidate(pSkeletonCandidate, faceId, 1);

		// add it to the corresponding submesh
		int vertexId2;
		vertexId2 = m_vectorSubmeshCandidate[materialId]->AddVertexCandidate(pVertexCandidate);

		// get the third vertex of the face
		pVertexCandidate = mesh->GetVertexCandidate(pSkeletonCandidate, faceId, 2);

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
