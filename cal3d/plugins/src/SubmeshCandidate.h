//----------------------------------------------------------------------------//
// SubmeshCandidate.h                                                         //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef SUBMESH_CANDIDATE_H
#define SUBMESH_CANDIDATE_H


class CVertexCandidate;


class CSubmeshCandidate
{
public:
	struct Face
	{
		int lodId;
		int vertexId[3];
		int vertexLodId[3];
	};

	struct Spring
	{
		int vertexId[2];
		float springCoefficient;
		float idleLength;
		int priority;
	};

	CSubmeshCandidate();
	~CSubmeshCandidate();

	bool AddFace(int vertexId1, int vertexId2, int vertexId3);
	int AddVertexCandidate(CVertexCandidate *pVertexCandidate);
	void AdjustBoneAssignment(int maxBoneCount, float weightThreshold);
	bool CalculateLOD();
	bool CalculateSpringSystem();
	void Clear();
	bool Create(int materialThreadId, int mapCount);
	bool DisableLOD();
	int GetFaceCount();
	int GetLodCount();
	int GetMapCount();
	int GetMaterialThreadId();
	std::vector<Face>& GetVectorFace();
	std::vector<Spring>& GetVectorSpring();
	std::vector<CVertexCandidate *>& GetVectorVertexCandidate();

private:
	bool AddSpring(int vertexId1, int vertexId2);
	bool AdjustSpringVertex(int vertexId1, int vertexId2);
	static bool SpringCompare(Spring& springOne, Spring& springTwo);

	int m_materialThreadId;
	int m_mapCount;
	std::vector<CVertexCandidate *> m_vectorVertexCandidate;
	std::vector<Face> m_vectorFace;
	std::vector<Spring> m_vectorSpring;
	int m_lodCount;

};

#endif
