//----------------------------------------------------------------------------//
// MaxNode.h                                                                  //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef MAX_NODE_H
#define MAX_NODE_H

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#include "BaseNode.h"

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CMaxNode : public CBaseNode
{
// member variables
protected:
	INode *m_pINode;

// constructors/destructor
public:
	CMaxNode();
	virtual ~CMaxNode();

// member functions
public:
	bool Create(INode *pNode);
	CBaseNode *GetChild(int childId);
	int GetChildCount();
	INode *GetINode();
	std::string GetName();
	Type GetType();
        bool operator==(const CBaseNode& rhs) const;
};

#endif

//----------------------------------------------------------------------------//
