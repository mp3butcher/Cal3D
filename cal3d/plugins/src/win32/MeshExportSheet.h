//----------------------------------------------------------------------------//
// MeshExportSheet.h                                                          //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef MESH_EXPORT_SHEET_H
#define MESH_EXPORT_SHEET_H

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#include "SkeletonFilePage.h"
#include "BoneAssignmentPage.h"
#include "LevelOfDetailPage.h"
#include "SpringSystemPage.h"

//----------------------------------------------------------------------------//
// Forward declarations                                                       //
//----------------------------------------------------------------------------//

class CSkeletonCandidate;
class CMeshCandidate;

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CMeshExportSheet : public CPropertySheet
{
// member variables
protected:
	CSkeletonFilePage m_skeletonFilePage;
	CBoneAssignmentPage m_boneAssignmentPage;
	CLevelOfDetailPage m_levelOfDetailPage;
	CSpringSystemPage m_springSystemPage;

// constructors/destructor
public:
	CMeshExportSheet(UINT nIDCaption, CWnd *pParentWnd = NULL, UINT iSelectPage = 0);
	CMeshExportSheet(LPCTSTR pszCaption, CWnd *pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CMeshExportSheet();

// member functions
public:
	void SetSkeletonCandidate(CSkeletonCandidate *pSkeletonCandidate);
	void SetMeshCandidate(CMeshCandidate *pMeshCandidate);

protected:
	void FillPages();

// MSVC++ stuff
	//{{AFX_VIRTUAL(CMeshExportSheet)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMeshExportSheet)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//----------------------------------------------------------------------------//
// MSVC++ stuff                                                               //
//----------------------------------------------------------------------------//

//{{AFX_INSERT_LOCATION}}

#endif

//----------------------------------------------------------------------------//
