//----------------------------------------------------------------------------//
// MilkMeshNode.h                                                             //
// Copyright (C) 2002 Bruno 'Beosil' Heidelberger                             //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef MILK_MESH_NODE_H
#define MILK_MESH_NODE_H

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#include "BaseNode.h"

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CMilkMeshNode : public CBaseNode
{
// member variables
protected:
  msMesh *m_pIMesh;

// constructors/destructor
public:
	CMilkMeshNode();
	virtual ~CMilkMeshNode();

// member functions
public:
	bool Create(msMesh *pIMesh);
	CBaseNode *GetChild(int childId);
	int GetChildCount();
  msMesh *GetIMesh();
	std::string GetName();
	Type GetType();
        bool operator==(const CBaseNode& rhs) const;
};

#endif

//----------------------------------------------------------------------------//
