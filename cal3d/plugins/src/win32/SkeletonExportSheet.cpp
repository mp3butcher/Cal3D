//----------------------------------------------------------------------------//
// SkeletonExportSheet.cpp                                                    //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#include "StdAfx.h"
#include "SkeletonExportSheet.h"

//----------------------------------------------------------------------------//
// Debug                                                                      //
//----------------------------------------------------------------------------//

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//----------------------------------------------------------------------------//
// Message mapping                                                            //
//----------------------------------------------------------------------------//

BEGIN_MESSAGE_MAP(CSkeletonExportSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CSkeletonExportSheet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//----------------------------------------------------------------------------//
// Constructors                                                               //
//----------------------------------------------------------------------------//

CSkeletonExportSheet::CSkeletonExportSheet(UINT nIDCaption, CWnd *pParentWnd, UINT iSelectPage) : CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	FillPages();
}

CSkeletonExportSheet::CSkeletonExportSheet(LPCTSTR pszCaption, CWnd *pParentWnd, UINT iSelectPage) : CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	FillPages();
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CSkeletonExportSheet::~CSkeletonExportSheet()
{
}

//----------------------------------------------------------------------------//
// Fill in all propoerty pages                                                //
//----------------------------------------------------------------------------//

void CSkeletonExportSheet::FillPages()
{
	// add all property pages to this sheet
	m_skeletonHierarchyPage.SetStep(1, 1);
	m_skeletonHierarchyPage.SetDescription(IDS_SKELETON_EXPORT_DESCRIPTION);
	AddPage(&m_skeletonHierarchyPage);
}

//----------------------------------------------------------------------------//
// Set the node hierarchy                                                     //
//----------------------------------------------------------------------------//

void CSkeletonExportSheet::SetSkeletonCandidate(CSkeletonCandidate *pSkeletonCandidate)
{
	m_skeletonHierarchyPage.SetSkeletonCandidate(pSkeletonCandidate);
}

//----------------------------------------------------------------------------//
