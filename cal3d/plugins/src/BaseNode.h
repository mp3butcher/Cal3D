//----------------------------------------------------------------------------//
// BaseNode.h                                                                 //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef BASE_NODE_H
#define BASE_NODE_H

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CBaseNode
{
// misc
public:
	enum Type
	{
		TYPE_OTHER = 0,
		TYPE_BONE,
		TYPE_DUMMY,
    TYPE_MESH
	};

// member variables
protected:

// constructors/destructor
protected:
	CBaseNode();
public:
	virtual ~CBaseNode();

// member functions
public:
	virtual CBaseNode *GetChild(int childId) = 0;
	virtual int GetChildCount() = 0;
	virtual std::string GetName() = 0;
	virtual Type GetType() = 0;
};

#endif

//----------------------------------------------------------------------------//
