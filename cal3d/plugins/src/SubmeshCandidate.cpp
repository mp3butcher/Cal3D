//----------------------------------------------------------------------------//
// SubmeshCandidate.cpp                                                       //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//


#include "StdAfx.h"
#include "Exporter.h"
#include "SubmeshCandidate.h"
#include "VertexCandidate.h"
#include "Lodder.h"


CSubmeshCandidate::CSubmeshCandidate()
{
}


CSubmeshCandidate::~CSubmeshCandidate()
{
  Clear();
}


bool CSubmeshCandidate::AddFace(int vertexId1, int vertexId2, int vertexId3)
{
  // create a face object
  Face face;
  face.vertexId[0] = vertexId1;
  face.vertexId[1] = vertexId2;
  face.vertexId[2] = vertexId3;

  // insert it into the face vector
  m_vectorFace.push_back(face);

  // adjust the neighbour sets in each vertex candidate
  m_vectorVertexCandidate[vertexId1]->AddNeighbour(vertexId2);
  m_vectorVertexCandidate[vertexId1]->AddNeighbour(vertexId3);
  m_vectorVertexCandidate[vertexId2]->AddNeighbour(vertexId1);
  m_vectorVertexCandidate[vertexId2]->AddNeighbour(vertexId3);
  m_vectorVertexCandidate[vertexId3]->AddNeighbour(vertexId1);
  m_vectorVertexCandidate[vertexId3]->AddNeighbour(vertexId2);

  return true;
}

//----------------------------------------------------------------------------//
// Add a spring to this submesh candidate                                     //
//----------------------------------------------------------------------------//

bool CSubmeshCandidate::AddSpring(int vertexId1, int vertexId2)
{
  // check for valid vertices
  if(vertexId1 == vertexId2) return false;

  // make the lower vertex id the first one
  if(vertexId1 > vertexId2)
  {
    int tempId;
    tempId = vertexId1;
    vertexId1 = vertexId2;
    vertexId2 = tempId;
  }

  // get the physical property of each vertex
  CVertexCandidate::PhysicalProperty physicalProperty[2];
  m_vectorVertexCandidate[vertexId1]->GetPhysicalProperty(physicalProperty[0]);
  m_vectorVertexCandidate[vertexId2]->GetPhysicalProperty(physicalProperty[1]);

  // at least on of the vertices must have a weight
  if((physicalProperty[0].weight == 0.0f) && (physicalProperty[1].weight == 0.0f)) return false;

  // loop through the existing springs
  for(size_t springId = 0; springId < m_vectorSpring.size(); springId++)
  {
    // check if we have an equal spring
    if((m_vectorSpring[springId].vertexId[0] == vertexId1) && (m_vectorSpring[springId].vertexId[1] == vertexId2))
    {
      return false;
    }
  }
  
  // create a spring object
  Spring spring;
  spring.vertexId[0] = vertexId1;
  spring.vertexId[1] = vertexId2;

  // TODO: Expose this to the exporter GUI
  spring.springCoefficient = 0.3f;

  // get the two vertex positions
  CalVector vertexPosition[2];
  m_vectorVertexCandidate[vertexId1]->GetPosition(vertexPosition[0]);
  m_vectorVertexCandidate[vertexId2]->GetPosition(vertexPosition[1]);

  // calculate the spring length
  vertexPosition[0] -= vertexPosition[1];
  spring.idleLength = vertexPosition[0].length();

  // insert it into the spring vector
  m_vectorSpring.push_back(spring);

  // adjust the number of springs and the minimal distance to a constraint vertex of both vertices
  physicalProperty[0].springCount++;
  if(physicalProperty[0].weight == 0.0f) physicalProperty[0].constraintDistance = 0;
  m_vectorVertexCandidate[vertexId1]->SetPhysicalProperty(physicalProperty[0].weight, physicalProperty[0].springCount, physicalProperty[0].constraintDistance);

  physicalProperty[1].springCount++;
  if(physicalProperty[1].weight == 0.0f) physicalProperty[1].constraintDistance = 0;
  m_vectorVertexCandidate[vertexId2]->SetPhysicalProperty(physicalProperty[1].weight, physicalProperty[1].springCount, physicalProperty[1].constraintDistance);

  return true;
}

//----------------------------------------------------------------------------//
// Add a vertex candidate to this submesh candidate                           //
//----------------------------------------------------------------------------//

int CSubmeshCandidate::AddVertexCandidate(CVertexCandidate *pVertexCandidate)
{
  // check if the vertex candidate has the same number of maps as this submesh candidate
  if(pVertexCandidate->GetVectorTextureCoordinate().size() != m_mapCount)
  {
    theExporter.SetLastError("Map count mismatch (vertex <-> submesh)!", __FILE__, __LINE__);
    return -1;
  }

  // get the number of vertex candidates in this submesh
  int vertexCandidateCount;
  vertexCandidateCount = m_vectorVertexCandidate.size();

  // loop through the existing vertex candidates
  for(int vertexCandidateId = 0; vertexCandidateId < vertexCandidateCount; vertexCandidateId++)
  {
    // check if we have an equal vertex candidate
    if(*pVertexCandidate == *m_vectorVertexCandidate[vertexCandidateId])
    {
      // we found one, delete the submitted one
      delete pVertexCandidate;
      return vertexCandidateId;
    }
  }

  // vertex candidate is unique, add it to the vertex candidate vector
  m_vectorVertexCandidate.push_back(pVertexCandidate);

  return vertexCandidateCount;
}

//----------------------------------------------------------------------------//
// Adjust the bone assignment for a given max bone count and weight threshold //
//----------------------------------------------------------------------------//

void CSubmeshCandidate::AdjustBoneAssignment(int maxBoneCount, float weightThreshold)
{
  // adjust the bone assignments in all vertex candidates
  for(size_t vertexCandidateId = 0; vertexCandidateId < m_vectorVertexCandidate.size(); vertexCandidateId++)
  {
    m_vectorVertexCandidate[vertexCandidateId]->AdjustBoneAssignment(maxBoneCount, weightThreshold);
  }
}

//----------------------------------------------------------------------------//
// Adjust the internal data of a specified vertex with a spring neighbour     //
//----------------------------------------------------------------------------//

bool CSubmeshCandidate::AdjustSpringVertex(int vertexId1, int vertexId2)
{
  // get the physical property of each vertex
  CVertexCandidate::PhysicalProperty physicalProperty[2];
  m_vectorVertexCandidate[vertexId1]->GetPhysicalProperty(physicalProperty[0]);
  m_vectorVertexCandidate[vertexId2]->GetPhysicalProperty(physicalProperty[1]);
  
  // adjust the distance to the nearest constraint vertex if necessary
  if((physicalProperty[0].constraintDistance == -1) || (physicalProperty[0].constraintDistance > physicalProperty[1].constraintDistance + 1))
  {
    if(physicalProperty[1].constraintDistance != -1)
    {
      physicalProperty[0].constraintDistance = physicalProperty[1].constraintDistance + 1;
      m_vectorVertexCandidate[vertexId1]->SetPhysicalProperty(physicalProperty[0].weight, physicalProperty[0].springCount, physicalProperty[0].constraintDistance);
    }
    return true;
  }

  return false;
}
  
//----------------------------------------------------------------------------//
// Calculate the LOD of this submesh candidate                                //
//----------------------------------------------------------------------------//

bool CSubmeshCandidate::CalculateLOD()
{
  // create a lodder instance
  CLodder lodder;
  if(!lodder.Create(m_vectorVertexCandidate.size(), m_vectorFace.size())) return false;

  // add all vertices to the lodder
  size_t vertexCandidateId;
  for(vertexCandidateId = 0; vertexCandidateId < m_vectorVertexCandidate.size(); vertexCandidateId++)
  {
    // get the vertex candidate
    CVertexCandidate *pVertexCandidate;
    pVertexCandidate = m_vectorVertexCandidate[vertexCandidateId];

    // get the positoin of the vertex candidate
    CalVector position;
    pVertexCandidate->GetPosition(position);

    // get the normal of the vertex candidate
    CalVector normal;
    pVertexCandidate->GetNormal(normal);

    // create a lodder vertex
    CLodder::Vertex vertex;
    vertex.x = position[0];
    vertex.y = position[1];
    vertex.z = position[2];
    vertex.nx = normal[0];
    vertex.ny = normal[1];
    vertex.nz = normal[2];

    // add vertex to the lodder
    if(!lodder.AddVertex(vertex)) return false;
  }

  // add all faces to the lodder
  size_t faceId;
  for(faceId = 0; faceId < m_vectorFace.size(); faceId++)
  {
    // get the face
    Face& face = m_vectorFace[faceId];

    // create a lodder face
    CLodder::Face lodderFace;
    lodderFace.vertexId[0] = face.vertexId[0];
    lodderFace.vertexId[1] = face.vertexId[1];
    lodderFace.vertexId[2] = face.vertexId[2];

    // add face to the lodder
    if(!lodder.AddFace(lodderFace)) return false;
  }

  // calculate LOD levels
  if(!lodder.CalculateLevels()) return false;

  // set the LOD id of all vertices
  for(vertexCandidateId = 0; vertexCandidateId < m_vectorVertexCandidate.size(); vertexCandidateId++)
  {
    // get the vertex candidate
    CVertexCandidate *pVertexCandidate;
    pVertexCandidate = m_vectorVertexCandidate[vertexCandidateId];

    // set the LOD id of the vertex candidates
    pVertexCandidate->SetLodId(lodder.GetVertexLodId(vertexCandidateId));

    // set the collapse id of the vertex candidates
    pVertexCandidate->SetCollapseId(lodder.GetVertexCollapseId(vertexCandidateId));

    // set the face collapse count of the vertex candidates
    pVertexCandidate->SetFaceCollapseCount(lodder.GetVertexFaceCollapseCount(vertexCandidateId));
  }

  // set the LOD id of all face vertices
  for(faceId = 0; faceId < m_vectorFace.size(); faceId++)
  {
    // get the face
    Face& face = m_vectorFace[faceId];

    // set the LOD id of the face
    face.lodId = lodder.GetFaceLodId(faceId);

    // set the LOD id of the face vertices
    face.vertexLodId[0] = lodder.GetVertexLodId(face.vertexId[0]);
    face.vertexLodId[1] = lodder.GetVertexLodId(face.vertexId[1]);
    face.vertexLodId[2] = lodder.GetVertexLodId(face.vertexId[2]);
  }

  // set the LOD step count
  m_lodCount = lodder.GetLodCount();

  return true;
}

//----------------------------------------------------------------------------//
// Calculate possible spring system of this submesh candidate                 //
//----------------------------------------------------------------------------//

bool CSubmeshCandidate::CalculateSpringSystem()
{
  // loop through all the vertex candidates of the submesh candidate
  size_t vertexId;
  for(vertexId = 0; vertexId < m_vectorVertexCandidate.size(); vertexId++)
  {
    // start from the current vertex candidate
    std::set<int> setNeighbour;
    setNeighbour.insert(vertexId);

    // get all the neighbours of the current vertex candidate up to the given level
    int levelId;
    for(levelId = 0; levelId < 2; levelId++)
    {
      std::set<int> setNeighbourNeighbour;

      std::set<int>::iterator iteratorNeighbour;
      for(iteratorNeighbour = setNeighbour.begin(); iteratorNeighbour != setNeighbour.end(); ++iteratorNeighbour)
      {
        std::set<int>::iterator iteratorNeighbourNeighbour;
        for(iteratorNeighbourNeighbour = m_vectorVertexCandidate[*iteratorNeighbour]->GetSetNeighbour().begin(); iteratorNeighbourNeighbour != m_vectorVertexCandidate[*iteratorNeighbour]->GetSetNeighbour().end(); ++iteratorNeighbourNeighbour)
        {
          setNeighbourNeighbour.insert(*iteratorNeighbourNeighbour);
        }
      }

      for(iteratorNeighbour = setNeighbourNeighbour.begin(); iteratorNeighbour != setNeighbourNeighbour.end(); ++iteratorNeighbour)
      {
        setNeighbour.insert(*iteratorNeighbour);
      }
    }

    // add springs to all these neighbours
    std::set<int>::iterator iteratorNeighbour;
    for(iteratorNeighbour = setNeighbour.begin(); iteratorNeighbour != setNeighbour.end(); ++iteratorNeighbour)
    {
      AddSpring(vertexId, *iteratorNeighbour);
    }
  }

/* OLD VERSION
  // loop through all the faces of the submesh candidate
  int faceId;
  for(faceId = 0; faceId < m_vectorFace.size(); faceId++)
  {
    // get the face
    Face& face = m_vectorFace[faceId];

    // add all edges to the spring system as springs
    AddSpring(face.vertexId[0], face.vertexId[1]);
    AddSpring(face.vertexId[1], face.vertexId[2]);
    AddSpring(face.vertexId[2], face.vertexId[0]);
  }
*/

  // loop until we have a stable system
  bool bModified;
  do
  {
    // clear the modification flag
     bModified = false;

    // loop through all the springs of the submesh candidate
    for(size_t springId = 0; springId < m_vectorSpring.size(); springId++)
    {
      // get the spring
      Spring& spring = m_vectorSpring[springId];

      // adjust the two vertices connected to the spring
      bModified |= AdjustSpringVertex(spring.vertexId[0], spring.vertexId[1]);
      bModified |= AdjustSpringVertex(spring.vertexId[1], spring.vertexId[0]);
    }
  } while(bModified);

  // loop through all the springs of the submesh candidate
  for(size_t springId = 0; springId < m_vectorSpring.size(); springId++)
  {
    // get the spring
    Spring& spring = m_vectorSpring[springId];

    // get the physical properties of the two spring vertices
    CVertexCandidate::PhysicalProperty physicalProperty[2];
    m_vectorVertexCandidate[spring.vertexId[0]]->GetPhysicalProperty(physicalProperty[0]);
    m_vectorVertexCandidate[spring.vertexId[1]]->GetPhysicalProperty(physicalProperty[1]);

    spring.priority = (physicalProperty[0].constraintDistance < physicalProperty[1].constraintDistance) ? physicalProperty[0].constraintDistance : physicalProperty[1].constraintDistance;
  }

  // sort the springs
  std::sort(m_vectorSpring.begin(), m_vectorSpring.end(), SpringCompare);

/* DEBUG
for(springId = 0; springId < m_vectorSpring.size(); springId++)
{
  // get the spring
  Spring& spring = m_vectorSpring[springId];

  CString str;
  str.Format("spring %d: priority: %d, %d <-> %d\n", springId, spring.priority, spring.vertexId[0], spring.vertexId[1]);
  OutputDebugString(str);
}
*/

  return true;
}

//----------------------------------------------------------------------------//
// Clear this submesh candidate                                               //
//----------------------------------------------------------------------------//

void CSubmeshCandidate::Clear()
{
  // destroy all vertex candidates stored in this submesh candidate
  for(size_t vertexCandidateId = 0; vertexCandidateId < m_vectorVertexCandidate.size(); vertexCandidateId++)
  {
    delete m_vectorVertexCandidate[vertexCandidateId];
  }
  m_vectorVertexCandidate.clear();
}

//----------------------------------------------------------------------------//
// Create a submesh candidate                                                 //
//----------------------------------------------------------------------------//

bool CSubmeshCandidate::Create(int materialThreadId, int mapCount)
{
  // clear current content
  Clear();

  m_materialThreadId = materialThreadId;
  m_mapCount = mapCount;

  return true;
}

//----------------------------------------------------------------------------//
// Disable the LOD of this submesh candidate                                  //
//----------------------------------------------------------------------------//

bool CSubmeshCandidate::DisableLOD()
{
  // set the LOD id of all vertices to a disabled value
  for(size_t vertexCandidateId = 0; vertexCandidateId < m_vectorVertexCandidate.size(); vertexCandidateId++)
  {
    // get the vertex candidate
    CVertexCandidate *pVertexCandidate;
    pVertexCandidate = m_vectorVertexCandidate[vertexCandidateId];

    // set the LOD id of the vertex candidates
    pVertexCandidate->SetLodId(vertexCandidateId);

    // set the collapse id of the vertex candidates
    pVertexCandidate->SetCollapseId(-1);

    // set the face collapse count of the vertex candidates
    pVertexCandidate->SetFaceCollapseCount(0);
  }

  // set the LOD id of all face vertices to a disabled value
  for(size_t faceId = 0; faceId < m_vectorFace.size(); faceId++)
  {
    // get the face
    Face& face = m_vectorFace[faceId];

    // set the LOD id of the face
    face.lodId = faceId;

    // set the LOD id of the face vertices
    face.vertexLodId[0] = face.vertexId[0];
    face.vertexLodId[1] = face.vertexId[1];
    face.vertexLodId[2] = face.vertexId[2];
  }

  // set the LOD step count
  m_lodCount = 0;

  return true;
}

//----------------------------------------------------------------------------//
// Get the number of faces of this submesh candidate                          //
//----------------------------------------------------------------------------//

int CSubmeshCandidate::GetFaceCount()
{
  return m_vectorFace.size();
}

//----------------------------------------------------------------------------//
// Get the number of LOD steps of this submesh candidate                      //
//----------------------------------------------------------------------------//

int CSubmeshCandidate::GetLodCount()
{
  return m_lodCount;
}

//----------------------------------------------------------------------------//
// Get the number of maps in the submesh candidate                            //
//----------------------------------------------------------------------------//

int CSubmeshCandidate::GetMapCount()
{
  return m_mapCount;
}

//----------------------------------------------------------------------------//
// Get the material id                                                        //
//----------------------------------------------------------------------------//

int CSubmeshCandidate::GetMaterialThreadId()
{
  return m_materialThreadId;
}

//----------------------------------------------------------------------------//
// Get the face vector                                                        //
//----------------------------------------------------------------------------//

std::vector<CSubmeshCandidate::Face>& CSubmeshCandidate::GetVectorFace()
{
  return m_vectorFace;
}

//----------------------------------------------------------------------------//
// Get the spring vector                                                        //
//----------------------------------------------------------------------------//

std::vector<CSubmeshCandidate::Spring>& CSubmeshCandidate::GetVectorSpring()
{
  return m_vectorSpring;
}

//----------------------------------------------------------------------------//
// Get the vertex candidate vector                                            //
//----------------------------------------------------------------------------//

std::vector<CVertexCandidate *>& CSubmeshCandidate::GetVectorVertexCandidate()
{
  return m_vectorVertexCandidate;
}

//----------------------------------------------------------------------------//
// Compare two springs                                                        //
//----------------------------------------------------------------------------//

bool CSubmeshCandidate::SpringCompare(Spring& springOne, Spring& springTwo)
{
  return springOne.priority < springTwo.priority;
}

//----------------------------------------------------------------------------//
