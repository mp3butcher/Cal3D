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


class CSubmeshCandidate;
class CBaseMesh;
class CBaseNode;
class CSkeletonCandidate;


class CMeshCandidate
{
public:
	CMeshCandidate();
	virtual ~CMeshCandidate();

	bool CalculateLOD();
	bool CalculateSpringSystem();
	void Clear();
	bool Create(CSkeletonCandidate *pSkeletonCandidate, int maxBoneCount, float weightThreshold);
	bool DisableLOD();
	std::vector<CSubmeshCandidate *>& GetVectorSubmeshCandidate();

	bool Create(CBaseNode* _basenode, CSkeletonCandidate *pSkeletonCandidate, int maxBoneCount, float weightThreshold);

private:
    std::vector<CBaseMesh*> m_meshes;
    std::vector<CSubmeshCandidate *> m_vectorSubmeshCandidate;
};

#endif

//----------------------------------------------------------------------------//
