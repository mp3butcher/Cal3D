//----------------------------------------------------------------------------//
// Lodder.cpp                                                                 //
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
#include "Lodder.h"

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

CLodder::CLodder()
{
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CLodder::~CLodder()
{
}

//----------------------------------------------------------------------------//
// Add a face to the lodder                                                   //
//----------------------------------------------------------------------------//

bool CLodder::AddFace(const Face& face)
{
	// get the next face id
	int faceId;
	faceId = m_vectorFace.size();

	// insert the face into the face vector
	m_vectorFace.push_back(face);

	// insert face id into all the face vertices
	m_vectorVertex[face.vertexId[0]].setFaceId.insert(faceId);
	m_vectorVertex[face.vertexId[1]].setFaceId.insert(faceId);
	m_vectorVertex[face.vertexId[2]].setFaceId.insert(faceId);

	// insert all the neighbour ids into all the face vertices
	m_vectorVertex[face.vertexId[0]].setNeighbourId.insert(face.vertexId[1]);
	m_vectorVertex[face.vertexId[0]].setNeighbourId.insert(face.vertexId[2]);
	m_vectorVertex[face.vertexId[1]].setNeighbourId.insert(face.vertexId[0]);
	m_vectorVertex[face.vertexId[1]].setNeighbourId.insert(face.vertexId[2]);
	m_vectorVertex[face.vertexId[2]].setNeighbourId.insert(face.vertexId[0]);
	m_vectorVertex[face.vertexId[2]].setNeighbourId.insert(face.vertexId[1]);

	// initialize the data fields
	m_vectorFace[faceId].lodId = -1;

	return true;
}

//----------------------------------------------------------------------------//
// Add a vertex to the lodder                                                 //
//----------------------------------------------------------------------------//

bool CLodder::AddVertex(const Vertex& vertex)
{
	// get the next vertex id
	int vertexId;
	vertexId = m_vectorVertex.size();

	// insert the vertex into the vertex vector
	m_vectorVertex.push_back(vertex);

	// initialize the data fields
	m_vectorVertex[vertexId].lodId = -1;
	m_vectorVertex[vertexId].collapseId = -1;
	m_vectorVertex[vertexId].faceCollapseCount = 0;
	m_vectorVertex[vertexId].error = FLT_MAX;
	m_vectorVertex[vertexId].bCollapsed = false;
	m_vectorVertex[vertexId].bFreezed = false;

	return true;
}

//----------------------------------------------------------------------------//
// Calculate all levels of detail                                             //
//----------------------------------------------------------------------------//

bool CLodder::CalculateLevels()
{
	// initialize all the vertex quadrics
	CalculateAllQuadrics();

	// freeze the border vertices
	FreezeBorders();

	// calculate the best edge collapse for every vertex
	size_t vertexId;
	for(vertexId = 0; vertexId < m_vectorVertex.size(); vertexId++)
	{
		// calculate the best edge collapse for the vertex
		CalculateVertexCollapse(vertexId);
	}

	// initialize the next vertex/face lod id
	m_nextVertexLodId = m_vectorVertex.size() - 1;
	m_nextFaceLodId = m_vectorFace.size() - 1;

	// loop until there is no more edge collapse to do
	while(true)
	{
		// get the next vertex to collapse
		int collapsingVertexId;
		collapsingVertexId = GetNextCollapsingVertexId();
		
		// stop if there are no more edge collapses left
		if(collapsingVertexId == -1) break;

		// collapse the vertex
		CollapseVertex(collapsingVertexId);
	}

	// assign the lod ids to all unassigned vertices and faces
	m_nextVertexLodId = 0;
	m_nextFaceLodId = 0;

	// set all lod ids of the unassigned vertices
	for(vertexId = 0; vertexId < m_vectorVertex.size(); vertexId++)
	{
		if(m_vectorVertex[vertexId].lodId == -1)
		{
			m_vectorVertex[vertexId].lodId = m_nextVertexLodId++;
			m_vectorVertex[vertexId].collapseId = -1;
		}
	}

	// transform the collapse ids of the vertices to the lod ids
	for(vertexId = 0; vertexId < m_vectorVertex.size(); vertexId++)
	{
		if(m_vectorVertex[vertexId].collapseId != -1)
		{
			m_vectorVertex[vertexId].collapseId = m_vectorVertex[m_vectorVertex[vertexId].collapseId].lodId;
		}
	}

	// set all lod ids of the unassigned faces
	for(size_t faceId = 0; faceId < m_vectorFace.size(); faceId++)
	{
		if(m_vectorFace[faceId].lodId == -1) m_vectorFace[faceId].lodId = m_nextFaceLodId++;
	}

	// store the lod count
	m_lodCount = m_vectorVertex.size() - m_nextVertexLodId;

	return true;
}

//----------------------------------------------------------------------------//
// Calculate all the quadrics for each vertex                                 //
//----------------------------------------------------------------------------//

void CLodder::CalculateAllQuadrics()
{
	// loop through all the faces
	for(size_t faceId = 0; faceId < m_vectorFace.size(); faceId++)
	{
		// get the face
		Face& face = m_vectorFace[faceId];

		// calculate the area of the face
		float x1, y1, z1;
		x1 = m_vectorVertex[face.vertexId[1]].x - m_vectorVertex[face.vertexId[0]].x;
		y1 = m_vectorVertex[face.vertexId[1]].y - m_vectorVertex[face.vertexId[0]].y;
		z1 = m_vectorVertex[face.vertexId[1]].z - m_vectorVertex[face.vertexId[0]].z;

		float x2, y2, z2;
		x2 = m_vectorVertex[face.vertexId[2]].x - m_vectorVertex[face.vertexId[0]].x;
		y2 = m_vectorVertex[face.vertexId[2]].y - m_vectorVertex[face.vertexId[0]].y;
		z2 = m_vectorVertex[face.vertexId[2]].z - m_vectorVertex[face.vertexId[0]].z;

		float nx, ny, nz;
		nx = y1 * z2 - y2 * z1;
		ny = z1 * x2 - z2 * x1;
		nz = x1 * y2 - x2 * y1;

		// get the lnegth of the normal
		float length;
		length = sqrt(nx * nx + ny * ny + nz * nz);

		// calculate the face area
		float area;
		area = 0.5f * length;

		// normalize the face normal
		nx /= length;
		ny /= length;
		nz /= length;

		// check the calculated normal with one of the vertex normal, flip the calculated normal if necessary
		if(nx * m_vectorVertex[face.vertexId[0]].nx + ny * m_vectorVertex[face.vertexId[0]].ny + nz * m_vectorVertex[face.vertexId[0]].nz < 0.0f)
		{
			nx = -nx;
			ny = -ny;
			nz = -nz;
		}

		// get the fundamental quadric for the face
		CQuadric q(nx, ny, nz, -(m_vectorVertex[face.vertexId[0]].x * nx + m_vectorVertex[face.vertexId[0]].y * ny + m_vectorVertex[face.vertexId[0]].z * nz), area);

		// scale the quadric with the area
		q.Scale(area);

		// add the quadric to each face vertex
		m_vectorVertex[face.vertexId[0]].quadric.Add(q);
		m_vectorVertex[face.vertexId[1]].quadric.Add(q);
		m_vectorVertex[face.vertexId[2]].quadric.Add(q);
	}
}

//----------------------------------------------------------------------------//
// Calculate the edge collapse error for two given vertex                     //
//----------------------------------------------------------------------------//

float CLodder::CalculateCollapseError(int collapsingVertexId, int targetVertexId)
{
	// get the collapsing vertex
	Vertex& collapsingVertex = m_vectorVertex[collapsingVertexId];

	// get the target vertex
	Vertex& targetVertex = m_vectorVertex[targetVertexId];

	// check all faces of the collapsing vertex for normal flipping
	std::set<int>::iterator iteratorFaceId;
	for(iteratorFaceId = collapsingVertex.setFaceId.begin(); iteratorFaceId != collapsingVertex.setFaceId.end(); ++iteratorFaceId)
	{
		// get the face
		Face& face = m_vectorFace[*iteratorFaceId];

		// check if the collapsing edge is in this face and the face would not be degenerated afterwards
		if(((face.vertexId[0] == targetVertexId) || (face.vertexId[1] == targetVertexId) || (face.vertexId[2] == targetVertexId))
			&& (face.vertexId[0] != collapsingVertexId) && (face.vertexId[1] != collapsingVertexId) && (face.vertexId[2] != collapsingVertexId))
		{
			int v1, v2, v3;
			v1 = face.vertexId[0];
			v2 = face.vertexId[1];
			v3 = face.vertexId[2];

			// calculate the normal of the face
			float x1, y1, z1;
			x1 = m_vectorVertex[v2].x - m_vectorVertex[v1].x;
			y1 = m_vectorVertex[v2].y - m_vectorVertex[v1].y;
			z1 = m_vectorVertex[v2].z - m_vectorVertex[v1].z;

			float x2, y2, z2;
			x2 = m_vectorVertex[v3].x - m_vectorVertex[v1].x;
			y2 = m_vectorVertex[v3].y - m_vectorVertex[v1].y;
			z2 = m_vectorVertex[v3].z - m_vectorVertex[v1].z;

			float nx, ny, nz;
			nx = y1 * z2 - y2 * z1;
			ny = z1 * x2 - z2 * x1;
			nz = x1 * y2 - x2 * y1;

			float length;
			length = sqrt(nx * nx + ny * ny + nz * nz);
			nx /= length;
			ny /= length;
			nz /= length;

			// collapse the vertex
			if(v1 == collapsingVertexId) v1 = targetVertexId;
			if(v2 == collapsingVertexId) v2 = targetVertexId;
			if(v3 == collapsingVertexId) v3 = targetVertexId;

			// calculate the normal of the face again with new vertices
			x1 = m_vectorVertex[v2].x - m_vectorVertex[v1].x;
			y1 = m_vectorVertex[v2].y - m_vectorVertex[v1].y;
			z1 = m_vectorVertex[v2].z - m_vectorVertex[v1].z;

			x2 = m_vectorVertex[v3].x - m_vectorVertex[v1].x;
			y2 = m_vectorVertex[v3].y - m_vectorVertex[v1].y;
			z2 = m_vectorVertex[v3].z - m_vectorVertex[v1].z;

			float nx2, ny2, nz2;
			nx2 = y1 * z2 - y2 * z1;
			ny2 = z1 * x2 - z2 * x1;
			nz2 = x1 * y2 - x2 * y1;

			length = sqrt(nx2 * nx2 + ny2 * ny2 + nz2 * nz2);
			nx2 /= length;
			ny2 /= length;
			nz2 /= length;

			// check if the face normal has flipped
			if(nx * nx2 + ny * ny2 + nz * nz2 < 0.0f) return FLT_MAX / 2.0f;
		}
	}

	// quadric error computation
	CQuadric q(collapsingVertex.quadric);
	q.Add(targetVertex.quadric);

	return float(q.Evaluate(targetVertex.x, targetVertex.y, targetVertex.z) / q.GetArea());
}

//----------------------------------------------------------------------------//
// Calculate the best edge collapse for a given vertex                        //
//----------------------------------------------------------------------------//

void CLodder::CalculateVertexCollapse(int vertexId)
{
	// get the vertex
	Vertex& vertex = m_vectorVertex[vertexId];

	// initialize the collapse id of the vertex
	vertex.collapseId = -1;
	vertex.error = FLT_MAX;

	// check if the vertex is isolated
	if(vertex.setNeighbourId.size() == 0)
	{
		// vertex can be eliminated without error
		vertex.error = -1.0f;
		return;
	}

	// loop through all the neighbours of the vertex
	std::set<int>::iterator iteratorNeighbourId;
	for(iteratorNeighbourId = vertex.setNeighbourId.begin(); iteratorNeighbourId != vertex.setNeighbourId.end(); ++iteratorNeighbourId)
	{
		// get the error for collapsing the vertex to the current neighbour
		float error;
		error = CalculateCollapseError(vertexId, *iteratorNeighbourId);

		// if a better collapse is found, adjust the collapsing data in the vertex
		if(error < vertex.error)
		{
			vertex.error = error;
			vertex.collapseId = *iteratorNeighbourId;
		}
	}

}

//----------------------------------------------------------------------------//
// Collapse the given vertex                                                  //
//----------------------------------------------------------------------------//

void CLodder::CollapseVertex(int vertexId)
{
	// get the vertex
	Vertex& vertex = m_vectorVertex[vertexId];

	// set the vertex lod id in the collapsing vertex
	vertex.lodId = m_nextVertexLodId--;
	vertex.bCollapsed = true;

	// we will store all modified vertices, so their error can be recalculated
	std::set<int> setModifiedVertexId;

	// adjust the vertex quadric
	vertex.quadric.Add(m_vectorVertex[vertex.collapseId].quadric);

	// delete all faces with the collapsing edge
	std::set<int>::iterator iteratorFaceId;
	for(iteratorFaceId = vertex.setFaceId.begin(); iteratorFaceId != vertex.setFaceId.end(); )
	{
		// get the face
		Face& face = m_vectorFace[*iteratorFaceId];

		// check if the face contains the collapsing edge
		if((face.vertexId[0] == vertex.collapseId) || (face.vertexId[1] == vertex.collapseId) || (face.vertexId[2] == vertex.collapseId))
		{
			// get the face id
			int faceId;
			faceId = *iteratorFaceId;

			// erase the face from all the face vertices
			m_vectorVertex[face.vertexId[0]].setFaceId.erase(faceId);
			setModifiedVertexId.insert(face.vertexId[0]);

			m_vectorVertex[face.vertexId[1]].setFaceId.erase(faceId);
			setModifiedVertexId.insert(face.vertexId[1]);

			m_vectorVertex[face.vertexId[2]].setFaceId.erase(faceId);
			setModifiedVertexId.insert(face.vertexId[2]);

			// set the face lod id in the deleted face
			m_vectorFace[faceId].lodId = m_nextFaceLodId--;

			// the collapsing vertex has killed another face
			vertex.faceCollapseCount++;

			// start checking from the beginning (TODO: optimize)
			iteratorFaceId = vertex.setFaceId.begin();
		}
		else
		{
			 ++iteratorFaceId;
		}
	}

	// update all remaining faces with the new collapsed vertex
	for(iteratorFaceId = vertex.setFaceId.begin(); iteratorFaceId != vertex.setFaceId.end(); ++iteratorFaceId)
	{
		// get the face
		Face& face = m_vectorFace[*iteratorFaceId];

		// replace the collapsing vertex with the target one
		if(face.vertexId[0] == vertexId) face.vertexId[0] = vertex.collapseId;
		if(face.vertexId[1] == vertexId) face.vertexId[1] = vertex.collapseId;
		if(face.vertexId[2] == vertexId) face.vertexId[2] = vertex.collapseId;

		// insert this face into the new collapsed vertex
		m_vectorVertex[vertex.collapseId].setFaceId.insert(*iteratorFaceId);
	}
	vertex.setFaceId.clear();

	// remove the vertex from all its neighbours
	std::set<int>::iterator iteratorNeighbourId;
	for(iteratorNeighbourId = vertex.setNeighbourId.begin(); iteratorNeighbourId != vertex.setNeighbourId.end(); ++iteratorNeighbourId)
	{
		// remove the vertex from the neighbour set
		if(m_vectorVertex[*iteratorNeighbourId].setNeighbourId.erase(vertexId) > 0)
		{
			// insert the new collapsed vertex into the neighbour set
			if(*iteratorNeighbourId != vertex.collapseId)
			{
				m_vectorVertex[*iteratorNeighbourId].setNeighbourId.insert(vertex.collapseId);
				m_vectorVertex[vertex.collapseId].setNeighbourId.insert(*iteratorNeighbourId);
				setModifiedVertexId.insert(*iteratorNeighbourId);
			}
		}
	}
	vertex.setNeighbourId.clear();

	// do not recalculate edge collapse on collapsed vertex 
	setModifiedVertexId.erase(vertexId);

	// do recalculate edge collapse on the new collapsed vertex 
	if(vertex.collapseId != -1) setModifiedVertexId.insert(vertex.collapseId);

	// recalculate the best edge collapse for all the modified vertices
	std::set<int>::iterator iteratorModifiedVertexId;
	for(iteratorModifiedVertexId = setModifiedVertexId.begin(); iteratorModifiedVertexId != setModifiedVertexId.end(); ++iteratorModifiedVertexId)
	{
		// calculate the new vertex collapse
		CalculateVertexCollapse(*iteratorModifiedVertexId);
	}
}

//----------------------------------------------------------------------------//
// Create a lodder instance                                                   //
//----------------------------------------------------------------------------//

bool CLodder::Create(int vertexCount, int faceCount)
{
	// check if vertex and face numbers are valid
	if((vertexCount <= 0) || (faceCount <= 0))
	{
		theExporter.SetLastError("Invalid number of vertices/faces for LOD!", __FILE__, __LINE__);
		return false;
	}

	// reserve space for the vertex and face vector
	m_vectorVertex.reserve(vertexCount);
	m_vectorFace.reserve(faceCount);

	return true;
}

//----------------------------------------------------------------------------//
// Freeze all the vertices on borders                                         //
//----------------------------------------------------------------------------//

void CLodder::FreezeBorders()
{
	// loop through all the vertices
	for(size_t vertexId = 0; vertexId < m_vectorVertex.size(); vertexId++)
	{
		// get the vertex
		Vertex& vertex = m_vectorVertex[vertexId];

		// check the edge to each neighbour
		std::set<int>::iterator iteratorNeighbourId;
		for(iteratorNeighbourId = vertex.setNeighbourId.begin(); iteratorNeighbourId != vertex.setNeighbourId.end(); ++iteratorNeighbourId)
		{
			// get the neighbour vertex
			Vertex& neighbourVertex = m_vectorVertex[*iteratorNeighbourId];

			// count the number of vertex faces containing this edge and store one of them
			int faceCount;
			faceCount = 0;
			int oneFaceId;
			oneFaceId = -1;

			// loop through all the vertex faces
			std::set<int>::iterator iteratorFaceId;
			for(iteratorFaceId = vertex.setFaceId.begin(); iteratorFaceId != vertex.setFaceId.end(); ++iteratorFaceId)
			{
				// get the face
				Face& face = m_vectorFace[*iteratorFaceId];

				// test if the neighbour is in that face
				if((face.vertexId[0] == *iteratorNeighbourId) || (face.vertexId[1] == *iteratorNeighbourId) || (face.vertexId[2] == *iteratorNeighbourId))
				{
					oneFaceId = *iteratorFaceId;
					faceCount++;
				}
			}

			// check if the edge is a border, if so, freeze the vertex
			if(faceCount < 2)
			{
				// get the one face we stored earlier
				Face& face = m_vectorFace[oneFaceId];
/*
				// calculate the face normal from the three face vertex normals
				float nx, ny, nz;
				nx = m_vectorVertex[face.vertexId[0]].nx + m_vectorVertex[face.vertexId[1]].nx + m_vectorVertex[face.vertexId[2]].nx;
				ny = m_vectorVertex[face.vertexId[0]].ny + m_vectorVertex[face.vertexId[1]].ny + m_vectorVertex[face.vertexId[2]].ny;
				nz = m_vectorVertex[face.vertexId[0]].nz + m_vectorVertex[face.vertexId[1]].nz + m_vectorVertex[face.vertexId[2]].nz;

				// normalize the face normal
				float length;
				length = sqrt(nx * nx + ny * ny + nz * nz);
				nx /= length;
				ny /= length;
				nz /= length;

				// create the quadric for the border edge
				CQuadric q(vertex.x, vertex.y, vertex.z, neighbourVertex.x, neighbourVertex.y, neighbourVertex.z, nx, ny, nz);

				// adjust the quadric of the two vertices of the edge
				vertex.quadric.Add(q);
				neighbourVertex.quadric.Add(q);
*/
				// freeze the vertex
				vertex.bFreezed = true;

				// break out of the neighbour loop, vertex can not be *more* freezed than now
				break;
			}
		}
	}
}

//----------------------------------------------------------------------------//
// Get the LOD id of a given face                                             //
//----------------------------------------------------------------------------//

int CLodder::GetFaceLodId(int faceId)
{
	return m_vectorFace[faceId].lodId;
}

//----------------------------------------------------------------------------//
// Get the number of LOD steps                                                //
//----------------------------------------------------------------------------//

int CLodder::GetLodCount()
{
	return m_lodCount;
}

//----------------------------------------------------------------------------//
// Get the next vertex collapse                                               //
//----------------------------------------------------------------------------//

int CLodder::GetNextCollapsingVertexId()
{
	// initialize the error
	float error;
	error = FLT_MAX;

	// initialize the collapsing vertex id
	int collapsingVertexId;
	collapsingVertexId = -1;

	// loop through all the vertices
	for(size_t vertexId = 0; vertexId < m_vectorVertex.size(); vertexId++)
	{
		// check if we found a better edge collapse
		if(!m_vectorVertex[vertexId].bCollapsed && !m_vectorVertex[vertexId].bFreezed && (m_vectorVertex[vertexId].error < error))
		{
			collapsingVertexId = vertexId;
			error = m_vectorVertex[vertexId].error;
		}
	}

	return collapsingVertexId;
}

//----------------------------------------------------------------------------//
// Get the collapse id of a given vertex                                      //
//----------------------------------------------------------------------------//

int CLodder::GetVertexCollapseId(int vertexId)
{
	return m_vectorVertex[vertexId].collapseId;
}

//----------------------------------------------------------------------------//
// Get the number of face collapses of a given vertex                         //
//----------------------------------------------------------------------------//

int CLodder::GetVertexFaceCollapseCount(int vertexId)
{
	return m_vectorVertex[vertexId].faceCollapseCount;
}

//----------------------------------------------------------------------------//
// Get the LOD id of a given vertex                                           //
//----------------------------------------------------------------------------//

int CLodder::GetVertexLodId(int vertexId)
{
	return m_vectorVertex[vertexId].lodId;
}

//----------------------------------------------------------------------------//


//*************** DEBUG-STUFF ******************//
/*
void CLodder::Dump()
{
	int vertexId;
	for(vertexId = 0; vertexId < m_vectorVertex.size(); vertexId++)
	{
		Vertex& vertex = m_vectorVertex[vertexId];

		str.Format("vertex #%d : %c %f -> %d, neighbours: ", vertexId, vertex.bCollapsed ? 'X' : '-', (vertex.error == FLT_MAX) ? -99.99f : vertex.error, vertex.collapseId);
		
		std::set<int>::iterator iteratorNeighbourId;
		for(iteratorNeighbourId = vertex.setNeighbourId.begin(); iteratorNeighbourId != vertex.setNeighbourId.end(); ++iteratorNeighbourId)
		{
			CString strNeighbourId;
			strNeighbourId.Format("%d, ", *iteratorNeighbourId);
			str += strNeighbourId;
		}

		str += "faces: ";

		std::set<int>::iterator iteratorFaceId;
		for(iteratorFaceId = vertex.setFaceId.begin(); iteratorFaceId != vertex.setFaceId.end(); ++iteratorFaceId)
		{
			if(iteratorFaceId != vertex.setFaceId.begin()) str += ", ";
			CString strFaceId;
			strFaceId.Format("%d", *iteratorFaceId);
			str += strFaceId;
		}

		str += "\n";

		OutputDebugString(str);
	}

	Sleep(1000);

	int faceId;
	for(faceId = 0; faceId < m_vectorFace.size(); faceId++)
	{
		Face& face = m_vectorFace[faceId];

		str.Format("face #%d : %d %d %d\n", faceId, face.vertexId[0], face.vertexId[1], face.vertexId[2]);
		OutputDebugString(str);
	}

	Sleep(1000);
}
*/