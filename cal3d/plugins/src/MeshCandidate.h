//----------------------------------------------------------------------------//
// MeshCandidate.h                                                            //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef MESH_CANDIDATE_H
#define MESH_CANDIDATE_H

//----------------------------------------------------------------------------//
// Forward declarations                                                       //
//----------------------------------------------------------------------------//

class CSubmeshCandidate;
class CBaseMesh;
class CBaseNode;
class CSkeletonCandidate;

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CMeshCandidate
{
// member variables
protected:
	CBaseNode *m_pNode;
	CBaseMesh *m_pMesh;
	std::vector<CSubmeshCandidate *> m_vectorSubmeshCandidate;

// constructors/destructor
public:
	CMeshCandidate();
	virtual ~CMeshCandidate();

// member functions
public:
	bool CalculateLOD();
	bool CalculateSpringSystem();
	void Clear();
	bool Create(CSkeletonCandidate *pSkeletonCandidate, int maxBoneCount, float weightThreshold);
	bool DisableLOD();
	std::vector<CSubmeshCandidate *>& GetVectorSubmeshCandidate();

	bool Create(CBaseNode* _basenode, CSkeletonCandidate *pSkeletonCandidate, int maxBoneCount, float weightThreshold);
};

#endif

//----------------------------------------------------------------------------//
