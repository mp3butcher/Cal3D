//----------------------------------------------------------------------------//
// BoneCandidate.h                                                            //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef BONE_CANDIDATE_H
#define BONE_CANDIDATE_H

//----------------------------------------------------------------------------//
// Forward declarations                                                       //
//----------------------------------------------------------------------------//

class CBaseNode;

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CBoneCandidate
{
// member variables
protected:
	int m_id;
	int m_parentId;
	std::list<int> m_listChildId;
	bool m_bSelected;
	CBaseNode *m_pNode;
	int m_selectedId;

// constructors/destructor
public:
	CBoneCandidate();
	virtual ~CBoneCandidate();

// member functions
public:
	bool AddChildId(int childId);
	bool Create(int id, int parentId, CBaseNode *pNode);
	int GetId();
	std::list<int> GetListChildId();
	std::string GetName();
	CBaseNode *GetNode();
	int GetParentId();
	int GetSelectedId();
	bool IsSelected();
	void SetSelected(bool bSelected);
	void SetSelectedId(int selectedId);
};

#endif

//----------------------------------------------------------------------------//
