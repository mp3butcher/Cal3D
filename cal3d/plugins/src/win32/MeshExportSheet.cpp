//----------------------------------------------------------------------------//
// MeshExportSheet.cpp                                                        //
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
#include "MeshExportSheet.h"

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

BEGIN_MESSAGE_MAP(CMeshExportSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CMeshExportSheet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//----------------------------------------------------------------------------//
// Constructors                                                               //
//----------------------------------------------------------------------------//

CMeshExportSheet::CMeshExportSheet(UINT nIDCaption, CWnd *pParentWnd, UINT iSelectPage) : CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	FillPages();
}

CMeshExportSheet::CMeshExportSheet(LPCTSTR pszCaption, CWnd *pParentWnd, UINT iSelectPage) : CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	FillPages();
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CMeshExportSheet::~CMeshExportSheet()
{
}

//----------------------------------------------------------------------------//
// Fill in all propoerty pages                                                //
//----------------------------------------------------------------------------//

void CMeshExportSheet::FillPages()
{
	// add all property pages to this sheet
	m_skeletonFilePage.SetStep(1, 4);
	m_skeletonFilePage.SetDescription(IDS_MESH_EXPORT_DESCRIPTION);
	AddPage(&m_skeletonFilePage);

	m_boneAssignmentPage.SetStep(2, 4);
	m_boneAssignmentPage.SetDescription(IDS_MESH_EXPORT_DESCRIPTION2);
	AddPage(&m_boneAssignmentPage);

	m_levelOfDetailPage.SetStep(3, 4);
	m_levelOfDetailPage.SetDescription(IDS_MESH_EXPORT_DESCRIPTION3);
	AddPage(&m_levelOfDetailPage);

	m_springSystemPage.SetStep(4, 4);
	m_springSystemPage.SetDescription(IDS_MESH_EXPORT_DESCRIPTION4);
	AddPage(&m_springSystemPage);
}

//----------------------------------------------------------------------------//
// Set the mesh candidate                                                     //
//----------------------------------------------------------------------------//

void CMeshExportSheet::SetMeshCandidate(CMeshCandidate *pMeshCandidate)
{
	m_boneAssignmentPage.SetMeshCandidate(pMeshCandidate);
	m_levelOfDetailPage.SetMeshCandidate(pMeshCandidate);
	m_springSystemPage.SetMeshCandidate(pMeshCandidate);
}

//----------------------------------------------------------------------------//
// Set the node hierarchy                                                     //
//----------------------------------------------------------------------------//

void CMeshExportSheet::SetSkeletonCandidate(CSkeletonCandidate *pSkeletonCandidate)
{
	m_skeletonFilePage.SetSkeletonCandidate(pSkeletonCandidate);
	m_boneAssignmentPage.SetSkeletonCandidate(pSkeletonCandidate);
}

//----------------------------------------------------------------------------//
