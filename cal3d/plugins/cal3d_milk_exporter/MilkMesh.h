//----------------------------------------------------------------------------//
// MilkMesh.h                                                                 //
// Copyright (C) 2002 Bruno 'Beosil' Heidelberger                             //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef MILK_MESH_H
#define MILK_MESH_H

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#include "BaseMesh.h"

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CMilkMesh : public CBaseMesh
{
// member variables
protected:
	msMesh *m_pMesh;
  msMaterial *m_pMaterial;

// constructors/destructor
public:
	CMilkMesh();
	virtual ~CMilkMesh();

// member functions
public:
  bool Create(msMesh *pMesh);
	int GetFaceCount();
	msMesh *GetMesh();
	int GetMaterialCount();
	int GetFaceMaterialId(int faceId);
	int GetSubmeshMapCount(int submeshId);
	int GetSubmeshMaterialThreadId(int submeshId);
	CVertexCandidate *GetVertexCandidate(CSkeletonCandidate *pSkeletonCandidate, int faceId, int faceVertexId);
};

#endif

//----------------------------------------------------------------------------//
