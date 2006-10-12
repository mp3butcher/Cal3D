//----------------------------------------------------------------------------//
// MorphAnimationCandidate.h                                                            //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef MORPHANIMATION_CANDIDATE_H
#define MORPHANIMATION_CANDIDATE_H

//----------------------------------------------------------------------------//
// Forward declarations                                                       //
//----------------------------------------------------------------------------//

class CBaseMesh;
class CBaseNode;
class CSkeletonCandidate;

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CMorphAnimationCandidate
{
// member variables
protected:
	CBaseNode *m_pNode;
	CBaseMesh *m_pMesh;

// constructors/destructor
public:
	CMorphAnimationCandidate();
	virtual ~CMorphAnimationCandidate();

// member functions
public:
	void Clear();
	bool Create();
        CBaseMesh * meshAtTime(float time);
};

#endif

//----------------------------------------------------------------------------//
