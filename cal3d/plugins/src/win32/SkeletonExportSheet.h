//----------------------------------------------------------------------------//
// SkeletonExportSheet.h                                                      //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef SKELETON_EXPORT_SHEET_H
#define SKELETON_EXPORT_SHEET_H

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#include "SkeletonHierarchyPage.h"

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CSkeletonExportSheet : public CPropertySheet
{
// member variables
protected:
	CSkeletonHierarchyPage m_skeletonHierarchyPage;

// constructors/destructor
public:
	CSkeletonExportSheet(UINT nIDCaption, CWnd *pParentWnd = NULL, UINT iSelectPage = 0);
	CSkeletonExportSheet(LPCTSTR pszCaption, CWnd *pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CSkeletonExportSheet();

// member functions
public:
	void SetSkeletonCandidate(CSkeletonCandidate *pSkeletonCandidate);

protected:
	void FillPages();

// MSVC++ stuff
	//{{AFX_VIRTUAL(CSkeletonExportSheet)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSkeletonExportSheet)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//----------------------------------------------------------------------------//
// MSVC++ stuff                                                               //
//----------------------------------------------------------------------------//

//{{AFX_INSERT_LOCATION}}

#endif

//----------------------------------------------------------------------------//
