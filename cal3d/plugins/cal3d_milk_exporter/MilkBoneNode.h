//----------------------------------------------------------------------------//
// MilkBoneNode.h                                                             //
// Copyright (C) 2002 Bruno 'Beosil' Heidelberger                             //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef MILK_BONE_NODE_H
#define MILK_BONE_NODE_H

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#include "BaseNode.h"

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CMilkBoneNode : public CBaseNode
{
// member variables
protected:
  msBone *m_pIBone;
  std::vector<int> m_vectorChildId;

// constructors/destructor
public:
	CMilkBoneNode();
	virtual ~CMilkBoneNode();

// member functions
public:
	bool Create(msBone *pBone);
	CBaseNode *GetChild(int childId);
	int GetChildCount();
  msBone *GetIBone();
	std::string GetName();
  CalVector GetRelativeTranslation(float time);
  CalQuaternion GetRelativeRotation(float time);
	Type GetType();
        bool operator==(const CBaseNode& rhs) const;

protected:
  CalQuaternion CMilkBoneNode::ConvertToQuaternion(msVec3 euler);
};

#endif

//----------------------------------------------------------------------------//
