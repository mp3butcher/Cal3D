//----------------------------------------------------------------------------//
// VertexCandidate.h                                                          //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef VERTEX_CANDIDATE_H
#define VERTEX_CANDIDATE_H

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CVertexCandidate
{
// misc
public:
	typedef struct
	{
		float u, v;
	} TextureCoordinate;

	typedef struct
	{
		int boneId;
		float weight;
	} Influence;

	typedef struct
	{
		float weight;
		int springCount;
		int constraintDistance;
	} PhysicalProperty;

// member variables
protected:
	static float EPSILON;
	int m_lodId;
	int m_collapseId;
	int m_faceCollapseCount;
	CalVector m_position;
	CalVector m_normal;
	std::vector<TextureCoordinate> m_vectorTextureCoordinate;
	std::vector<Influence> m_vectorInfluence;
	PhysicalProperty m_physicalProperty;
  std::set<int> m_setNeighbour;
  CalVector m_color; // ===
  int m_uniqueId;
  bool m_hasUniqueId;

// constructors/destructor
public:
	CVertexCandidate();
	virtual ~CVertexCandidate();

// member functions
public:
	bool operator==(CVertexCandidate& vertexCandidate);
	void AddInfluence(int boneId, float weight);
  void AddNeighbour(int vertexId);
	void AddTextureCoordinate(float u, float v);
	void AdjustBoneAssignment(int maxBoneCount, float weightThreshold);
	bool Create();
	int GetCollapseId();
	int GetFaceCollapseCount();
	int GetInfluenceCount();
	int GetLodId();
	void GetNormal(CalVector& normal);
	void GetPhysicalProperty(PhysicalProperty& physicalProperty);
	void GetPosition(CalVector& position);
   void GetVertColor(CalVector &color); // ===
	std::set<int>& GetSetNeighbour();
	std::vector<Influence>& GetVectorInfluence();
	std::vector<TextureCoordinate>& GetVectorTextureCoordinate();
	void SetCollapseId(int collapseId);
	void SetFaceCollapseCount(int faceCollapseCount);
	void SetLodId(int lodId);
	void SetNormal(float nx, float ny, float nz);
	void SetPhysicalProperty(float weight, int springCount = 0, int constraintDistance = -1);
	void SetPosition(float x, float y, float z);
   void SetVertColor( CalVector c );

   int GetUniqueId();
   void SetUniqueId(int id);
protected:
	static bool CompareInfluenceWeight(const Influence& influence1, const Influence& influence2);
};

#endif

//----------------------------------------------------------------------------//
