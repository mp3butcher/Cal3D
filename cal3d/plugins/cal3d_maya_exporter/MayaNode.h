//----------------------------------------------------------------------------//
// MayaNode.h                                                                 //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger, Thomas Cowell        //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//
#ifndef MAYA_BASENODE_H
#define MAYA_BASENODE_H

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//
#include "BaseNode.h"

#include <maya/MGlobal.h>
#include <maya/MDagPath.h>

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//
class CMayaNode : public CBaseNode
{
	// constructors/destructor
public:
	CMayaNode();
	virtual ~CMayaNode();

	bool	Create (MDagPath &path);
	MDagPath& GetMayaDagPath () { return m_dagPath; }
        MDagPath& GetDagPath     () { return m_dagPath; }

	CBaseNode *GetChild(int childId);
	int GetChildCount();
	std::string GetName();
	Type GetType();
        bool operator==(const CBaseNode& rhs) const;

private:
	MDagPath		m_dagPath;
};

#endif