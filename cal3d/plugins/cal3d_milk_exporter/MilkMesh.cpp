//----------------------------------------------------------------------------//
// MilkMesh.cpp                                                               //
// Copyright (C) 2002 Bruno 'Beosil' Heidelberger                             //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#include "StdAfx.h"
#include "Exporter.h"
#include "MilkMesh.h"
#include "BaseInterface.h"
#include "SkeletonCandidate.h"
#include "VertexCandidate.h"
#include "MilkInterface.h"


CMilkMesh::CMilkMesh()
{
  m_pMesh = 0;
  m_pMaterial = 0;
}


CMilkMesh::~CMilkMesh()
{
}

//----------------------------------------------------------------------------//
// Create a milk mesh instance                                                //
//----------------------------------------------------------------------------//

bool CMilkMesh::Create(msMesh *pMesh)
{
  // check for valid mesh
  if(pMesh == 0)
  {
    theExporter.SetLastError("Invalid handle.", __FILE__, __LINE__);
    return false;
  }

  m_pMesh = pMesh;

  m_pMaterial = msModel_GetMaterialAt(dynamic_cast<CMilkInterface *>(theExporter.GetInterface())->GetIModel(), msMesh_GetMaterialIndex(pMesh));

  return true;
}

//----------------------------------------------------------------------------//
// Get the number of faces of the mesh                                        //
//----------------------------------------------------------------------------//

int CMilkMesh::GetFaceCount()
{
  // check for valid mesh
  if(m_pMesh == 0)
  {
    theExporter.SetLastError("Invalid handle.", __FILE__, __LINE__);
    return false;
  }

  return msMesh_GetTriangleCount(m_pMesh);
}

//----------------------------------------------------------------------------//
// Get the material id for a given face of the mesh                           //
//----------------------------------------------------------------------------//

int CMilkMesh::GetFaceMaterialId(int faceId)
{
  // check for valid mesh
  if(m_pMesh == 0)
  {
    theExporter.SetLastError("Invalid handle.", __FILE__, __LINE__);
    return -1;
  }

  return 0;
}

//----------------------------------------------------------------------------//
// Get the internal milk mesh of the mesh                                     //
//----------------------------------------------------------------------------//

msMesh *CMilkMesh::GetMesh()
{
  return m_pMesh;
}

//----------------------------------------------------------------------------//
// Get the number of materials of the mesh                                    //
//----------------------------------------------------------------------------//

int CMilkMesh::GetMaterialCount()
{
  return 1;
}

//----------------------------------------------------------------------------//
// Get the number of maps in a given submesh of the mesh                      //
//----------------------------------------------------------------------------//

int CMilkMesh::GetSubmeshMapCount(int submeshId)
{
  // check if the submesh id is valid
  if(submeshId != 0)
  {
    theExporter.SetLastError("Invalid handle.", __FILE__, __LINE__);
    return -1;
  }

  int mapCount;
  mapCount = 0;

  // check all maps of the material
  if(strlen(m_pMaterial->szDiffuseTexture) > 0) mapCount++;
  if(strlen(m_pMaterial->szAlphaTexture) > 0) mapCount++;

  return mapCount;
}

//----------------------------------------------------------------------------//
// Get the material thread id for a given submesh of the mesh                 //
//----------------------------------------------------------------------------//

int CMilkMesh::GetSubmeshMaterialThreadId(int submeshId)
{
  // check if the submesh id is valid
  if(submeshId != 0)
  {
    theExporter.SetLastError("Invalid handle.", __FILE__, __LINE__);
    return -1;
  }

  // get name of the material
  std::string strName;
  strName = m_pMaterial->szName;

  // get positions of the material thread id
  std::string::size_type openPos;
  openPos = strName.find_last_of("[");
  std::string::size_type closePos;
  closePos = strName.find_last_of("]");
  if((openPos == std::string::npos) || (closePos == std::string::npos) || (++openPos >= closePos))
  {
    theExporter.SetLastError("Invalid material thread id in material.", __FILE__, __LINE__);
    return -1;
  }

  // extract material thread id from material name
  std::string strMaterialThreadId;
  strMaterialThreadId = strName.substr(openPos, closePos - openPos);

  int materialThreadId;
  materialThreadId = atoi(strMaterialThreadId.c_str());

  return materialThreadId;
}

//----------------------------------------------------------------------------//
// Get the vertex candidate for a given vertex of a given face of the mesh    //
//----------------------------------------------------------------------------//

CVertexCandidate *CMilkMesh::GetVertexCandidate(CSkeletonCandidate *pSkeletonCandidate, int faceId, int faceVertexId)
{
  // check for valid mesh
  if(m_pMesh == 0)
  {
    theExporter.SetLastError("Invalid handle.", __FILE__, __LINE__);
    return 0;
  }

  // check if face id is valid
  if((faceId < 0) || (faceId >= msMesh_GetTriangleCount(m_pMesh)))
  {
    theExporter.SetLastError("Invalid face id found.", __FILE__, __LINE__);
    return 0;
  }

  // check if face vertex id is valid
  if((faceVertexId < 0) || (faceVertexId >= 3))
  {
    theExporter.SetLastError("Invalid face vertex id found.", __FILE__, __LINE__);
    return 0;
  }

  // allocate a new vertex candidate
  CVertexCandidate *pVertexCandidate;
  pVertexCandidate = new CVertexCandidate();
  if(pVertexCandidate == 0)
  {
    theExporter.SetLastError("Memory allocation failed.", __FILE__, __LINE__);
    return 0;
  }

  // create the new vertex candidate
  if(!pVertexCandidate->Create())
  {
    delete pVertexCandidate;
    return 0;
  }

  // get vertex id
  int vertexId;
  vertexId = m_pMesh->pTriangles[faceId].nVertexIndices[faceVertexId];

  // get the absolute vertex position
  msVertex *pVertex;
  pVertex = msMesh_GetVertexAt(m_pMesh, vertexId);

  // set the vertex candidate position
  pVertexCandidate->SetPosition(pVertex->Vertex[0], pVertex->Vertex[1], pVertex->Vertex[2]);

  // get vertex id
  int normalId;
  normalId = m_pMesh->pTriangles[faceId].nNormalIndices[faceVertexId];

  // get the absolute vertex position
  msVec3 normal;
  msMesh_GetVertexNormalAt(m_pMesh, normalId, normal);

  // set the vertex candidate normal
  pVertexCandidate->SetNormal(normal[0], normal[1], normal[2]);

  // TODO: get the vertex weight (if possible)
  float weight;
  weight = 0.0f;

  // set the vertex candidate weight
  pVertexCandidate->SetPhysicalProperty(weight);

  // get the map count
  int mapCount;
  mapCount = 0;

  // check all maps of the material
  if(strlen(m_pMaterial->szDiffuseTexture) > 0) mapCount++;
  if(strlen(m_pMaterial->szAlphaTexture) > 0) mapCount++;

  // loop through all the mapping channels and extract texture coordinates
  int mapId;
  for(mapId = 0; mapId < mapCount; mapId++)
  {
    // add texture coordinate to the vertex candidate
    pVertexCandidate->AddTextureCoordinate(pVertex->u, -pVertex->v);
  }

  // get the bone
  msBone *pBone;
  pBone = msModel_GetBoneAt(dynamic_cast<CMilkInterface *>(theExporter.GetInterface())->GetIModel(), pVertex->nBoneIndex);

  // get the bone id of the bone from the skeleton candidate
  int boneId;
  boneId = pSkeletonCandidate->GetBoneId(pBone->szName);
  if(boneId == -1) return 0;

  // add the influence to the vertex candidate
  pVertexCandidate->AddInfluence(boneId, 1.0f);

  return pVertexCandidate;
}

//----------------------------------------------------------------------------//
