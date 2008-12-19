//----------------------------------------------------------------------------//
// VertexCandidate.cpp                                                        //
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
// Static member variable initialization                                      //
//----------------------------------------------------------------------------//

float CVertexCandidate::EPSILON = 0.000001f;

//----------------------------------------------------------------------------//
// Constructors                                                               //
//----------------------------------------------------------------------------//

CVertexCandidate::CVertexCandidate()
{
	m_physicalProperty.weight = 0.0f;
	m_physicalProperty.springCount = 0;
	m_physicalProperty.constraintDistance = -1;
   m_hasUniqueId = false;
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CVertexCandidate::~CVertexCandidate()
{
}

//----------------------------------------------------------------------------//
// Compares the vertex candidate to a given one                               //
//----------------------------------------------------------------------------//

bool CVertexCandidate::operator==(CVertexCandidate& vertexCandidate)
{
	// compare the position
	if((fabs(m_position[0] - vertexCandidate.m_position[0]) >= EPSILON)
		|| (fabs(m_position[1] - vertexCandidate.m_position[1]) >= EPSILON)
		|| (fabs(m_position[2] - vertexCandidate.m_position[2]) >= EPSILON)) return false;

	// compare the normal
	if((fabs(m_normal[0] - vertexCandidate.m_normal[0]) >= EPSILON)
		|| (fabs(m_normal[1] - vertexCandidate.m_normal[1]) >= EPSILON)
		|| (fabs(m_normal[2] - vertexCandidate.m_normal[2]) >= EPSILON)) return false;

	// compare the texture coordinates
	if(m_vectorTextureCoordinate.size() != vertexCandidate.m_vectorTextureCoordinate.size()) return false;

	for(size_t textureCoordinateId = 0; textureCoordinateId < m_vectorTextureCoordinate.size(); textureCoordinateId++)
	{
		if((fabs(m_vectorTextureCoordinate[textureCoordinateId].u - vertexCandidate.m_vectorTextureCoordinate[textureCoordinateId].u) >= EPSILON)
			|| (fabs(m_vectorTextureCoordinate[textureCoordinateId].v - vertexCandidate.m_vectorTextureCoordinate[textureCoordinateId].v) >= EPSILON)) return false;
	}

	// TODO: compare influences?
/*
	// compare the influences
	if(m_vectorInfluence.size() != vertexCandidate.m_vectorInfluence.size()) return false;

	int influenceId;
	for(influenceId = 0; influenceId < m_vectorInfluence.size(); influenceId++)
	{
	}
*/
   if( m_hasUniqueId && vertexCandidate.m_hasUniqueId ) 
   {
      if( m_uniqueId == vertexCandidate.m_uniqueId ) 
      {
         return true;
      }
      else 
      {
         return false;
      }
   }
	// no differences found
	return true;
}

//----------------------------------------------------------------------------//
// Add an influence to the vertex candidate                                   //
//----------------------------------------------------------------------------//

void CVertexCandidate::AddInfluence(int boneId, float weight)
{
	// check if there is already an influence for this bone ( weird 3ds max behaviour =P )
	size_t influenceId;
	for(influenceId = 0; influenceId < m_vectorInfluence.size(); influenceId++)
	{
		// compare bone id
		if(m_vectorInfluence[influenceId].boneId == boneId)
		{
			// just add the weights
			m_vectorInfluence[influenceId].weight += weight;
			break;
		}
	}

	// create an influence object if there is none for the given bone
	if(influenceId == m_vectorInfluence.size())
	{
		Influence influence;
		influence.boneId = boneId;
		influence.weight = weight;

		// add it to the influence vector
		m_vectorInfluence.push_back(influence);
	}

	// sort all the influences by weight
	std::sort(m_vectorInfluence.begin(), m_vectorInfluence.end(), CompareInfluenceWeight);
}

//----------------------------------------------------------------------------//
// Add a neighbour to the vertex candidate                                    //
//----------------------------------------------------------------------------//

void CVertexCandidate::AddNeighbour(int vertexId)
{
  m_setNeighbour.insert(vertexId);
}

//----------------------------------------------------------------------------//
// Add a texture coordinate pair to the vertex candidate                      //
//----------------------------------------------------------------------------//

void CVertexCandidate::AddTextureCoordinate(float u, float v)
{
	// create a texture coordinate object
	TextureCoordinate textureCoordinate;
	textureCoordinate.u = u;
	textureCoordinate.v = v;

	// add it to the texture coordinate vector
	m_vectorTextureCoordinate.push_back(textureCoordinate);
}

//----------------------------------------------------------------------------//
// Adjust the bone assignment for a given max bone count and weight threshold //
//----------------------------------------------------------------------------//

void CVertexCandidate::AdjustBoneAssignment(int maxBoneCount, float weightThreshold)
{
	// erase all influences below the weight threshold
	std::vector<Influence>::iterator iteratorInfluence;
	for(iteratorInfluence = m_vectorInfluence.begin(); iteratorInfluence != m_vectorInfluence.end(); )
	{
		// check against the weight threshold
		if((*iteratorInfluence).weight < weightThreshold)
		{
			iteratorInfluence = m_vectorInfluence.erase(iteratorInfluence);
		}
		else
		{
			++iteratorInfluence;
		}
	}

	// erase all but the first few influences specified by max bone count
	if(m_vectorInfluence.size() > size_t(maxBoneCount))
	{
		m_vectorInfluence.resize(maxBoneCount);
	}

	// get the total weight of the influence
	float weight;
	weight = 0.0f;
	
        size_t influenceId;
	for(influenceId = 0; influenceId < m_vectorInfluence.size(); influenceId++)
	{
		weight += m_vectorInfluence[influenceId].weight;
	}

	// if there is no total weight, distribute it evenly over all influences
	if((weight < EPSILON) && (m_vectorInfluence.size() > 0))
	{
		weight = 1.0f / (float)m_vectorInfluence.size();
	}

	// normalize all influence weights
	for(influenceId = 0; influenceId < m_vectorInfluence.size(); influenceId++)
	{
		m_vectorInfluence[influenceId].weight /= weight;
	}
}

//----------------------------------------------------------------------------//
// Compare the weight of two given influences                                 //
//----------------------------------------------------------------------------//

bool CVertexCandidate::CompareInfluenceWeight(const Influence& influence1, const Influence& influence2)
{
	return influence1.weight > influence2.weight;
}

//----------------------------------------------------------------------------//
// Create a vertex candidate                                                  //
//----------------------------------------------------------------------------//

bool CVertexCandidate::Create()
{
	return true;
}

//----------------------------------------------------------------------------//
// Get the collapse id of the vertex candidate                                //
//----------------------------------------------------------------------------//

int CVertexCandidate::GetCollapseId()
{
	return m_collapseId;
}

//----------------------------------------------------------------------------//
// Get the number of face collapses of the vertex candidate                   //
//----------------------------------------------------------------------------//

int CVertexCandidate::GetFaceCollapseCount()
{
	return m_faceCollapseCount;
}

//----------------------------------------------------------------------------//
// Get the number of influences of the vertex candidate                       //
//----------------------------------------------------------------------------//

int CVertexCandidate::GetInfluenceCount()
{
	return m_vectorInfluence.size();
}

//----------------------------------------------------------------------------//
// Get the LOD id of the vertex candidate                                     //
//----------------------------------------------------------------------------//

int CVertexCandidate::GetLodId()
{
	return m_lodId;
}

//----------------------------------------------------------------------------//
// Get the normal of the vertex candidate                                     //
//----------------------------------------------------------------------------//

void CVertexCandidate::GetNormal(CalVector& normal)
{
	normal = m_normal;
}

//----------------------------------------------------------------------------//
// Get the physical property of the vertex candidate                          //
//----------------------------------------------------------------------------//

void CVertexCandidate::GetPhysicalProperty(PhysicalProperty& physicalProperty)
{
	physicalProperty = m_physicalProperty;
}

//----------------------------------------------------------------------------//
// Get the position of the vertex candidate                                   //
//----------------------------------------------------------------------------//

void CVertexCandidate::GetPosition(CalVector& position)
{
	position = m_position;
}

//----------------------------------------------------------------------------//
// Get the neighbours of the vertex candidate                                 //
//----------------------------------------------------------------------------//

std::set<int>& CVertexCandidate::GetSetNeighbour()
{
	return m_setNeighbour;
}

//----------------------------------------------------------------------------//
// Get the influence vector                                                   //
//----------------------------------------------------------------------------//

std::vector<CVertexCandidate::Influence>& CVertexCandidate::GetVectorInfluence()
{
	return m_vectorInfluence;
}

//----------------------------------------------------------------------------//
// Get the texture coordinate vector                                          //
//----------------------------------------------------------------------------//

std::vector<CVertexCandidate::TextureCoordinate>& CVertexCandidate::GetVectorTextureCoordinate()
{
	return m_vectorTextureCoordinate;
}

//----------------------------------------------------------------------------//
// Set the number of face collapses of the vertex candidate                   //
//----------------------------------------------------------------------------//

void CVertexCandidate::SetFaceCollapseCount(int faceCollapseCount)
{
	m_faceCollapseCount = faceCollapseCount;
}

//----------------------------------------------------------------------------//
// Set the collapse id of the vertex candidate                                //
//----------------------------------------------------------------------------//

void CVertexCandidate::SetCollapseId(int collapseId)
{
	m_collapseId = collapseId;
}

//----------------------------------------------------------------------------//
// Set the LOD id of the vertex candidate                                     //
//----------------------------------------------------------------------------//

void CVertexCandidate::SetLodId(int lodId)
{
	m_lodId = lodId;
}

//----------------------------------------------------------------------------//
// Set the normal of the vertex candidate                                     //
//----------------------------------------------------------------------------//

void CVertexCandidate::SetNormal(float nx, float ny, float nz)
{
	m_normal[0] = nx;
	m_normal[1] = ny;
	m_normal[2] = nz;
}

//----------------------------------------------------------------------------//
// Set the physical property of the vertex candidate                          //
//----------------------------------------------------------------------------//

void CVertexCandidate::SetPhysicalProperty(float weight, int springCount, int constraintDistance)
{
	m_physicalProperty.weight = weight;
	m_physicalProperty.springCount = springCount;
	m_physicalProperty.constraintDistance = constraintDistance;
}

//----------------------------------------------------------------------------//
// Set the position of the vertex candidate                                   //
//----------------------------------------------------------------------------//

void CVertexCandidate::SetPosition(float x, float y, float z)
{
	m_position[0] = x;
	m_position[1] = y;
	m_position[2] = z;
}

void 
CVertexCandidate::SetVertColor( CalVector c )
{
   m_color = c;
}

void 
CVertexCandidate::GetVertColor( CalVector &c )
{
   c.set(m_color.x, m_color.y, m_color.z);
}


int 
CVertexCandidate::GetUniqueId()
{
   return m_uniqueId;
}

void 
CVertexCandidate::SetUniqueId(int id)
{
   m_hasUniqueId = true;
   m_uniqueId = id;
}
//----------------------------------------------------------------------------//
