//----------------------------------------------------------------------------//
// Lodder.h                                                                   //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef LODDER_H
#define LODDER_H

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#include "Quadric.h"

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CLodder
{
// misc
public:
	typedef struct
	{
		int lodId;
		int collapseId;
		CQuadric quadric;
		float error;
		int faceCollapseCount;
		float x, y, z;
		float nx, ny, nz;
		std::set<int> setNeighbourId;
		std::set<int> setFaceId;
		bool bCollapsed;
		bool bFreezed;
	} Vertex;

	typedef struct
	{
		int lodId;
		int vertexId[3];
	} Face;

// member variables
protected:
	std::vector<Vertex> m_vectorVertex;
	std::vector<Face> m_vectorFace;
	int m_nextVertexLodId;
	int m_nextFaceLodId;
	int m_lodCount;

// constructors/destructor
public:
	CLodder();
	virtual ~CLodder();

// member functions
public:
	bool AddVertex(const Vertex& vertex);
	bool AddFace(const Face& face);
	bool CalculateLevels();
	bool Create(int vertexCount, int faceCount);
	int GetFaceLodId(int faceId);
	int GetLodCount();
	int GetVertexCollapseId(int vertexId);
	int GetVertexFaceCollapseCount(int vertexId);
	int GetVertexLodId(int vertexId);

protected:
	void CalculateAllQuadrics();
	float CalculateCollapseError(int collapsingVertexId, int targetVertexId);
	void CalculateVertexCollapse(int vertexId);
	void CollapseVertex(int vertexId);
	void FreezeBorders();
	int GetNextCollapsingVertexId();

	void Dump();
};

#endif

//----------------------------------------------------------------------------//
