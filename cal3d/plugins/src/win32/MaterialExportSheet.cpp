//----------------------------------------------------------------------------//
// MaterialExportSheet.cpp                                                    //
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
#include "MaterialExportSheet.h"

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

BEGIN_MESSAGE_MAP(CMaterialExportSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CMaterialExportSheet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//----------------------------------------------------------------------------//
// Constructors                                                               //
//----------------------------------------------------------------------------//

CMaterialExportSheet::CMaterialExportSheet(UINT nIDCaption, CWnd *pParentWnd, UINT iSelectPage) : CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	FillPages();
}

CMaterialExportSheet::CMaterialExportSheet(LPCTSTR pszCaption, CWnd *pParentWnd, UINT iSelectPage) : CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	FillPages();
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CMaterialExportSheet::~CMaterialExportSheet()
{
}

//----------------------------------------------------------------------------//
// Fill in all propoerty pages                                                //
//----------------------------------------------------------------------------//

void CMaterialExportSheet::FillPages()
{
	// add all property pages to this sheet
	m_materialSelectionPage.SetStep(1, 2);
	m_materialSelectionPage.SetDescription(IDS_MATERIAL_EXPORT_DESCRIPTION);
	AddPage(&m_materialSelectionPage);

	m_materialMapsPage.SetStep(2, 2);
	m_materialMapsPage.SetDescription(IDS_MATERIAL_EXPORT_DESCRIPTION2);
	AddPage(&m_materialMapsPage);
}

//----------------------------------------------------------------------------//
// Set the node hierarchy                                                     //
//----------------------------------------------------------------------------//

void CMaterialExportSheet::SetMaterialLibraryCandidate(CMaterialLibraryCandidate *pMaterialLibraryCandidate)
{
	m_materialSelectionPage.SetMaterialLibraryCandidate(pMaterialLibraryCandidate);
	m_materialMapsPage.SetMaterialLibraryCandidate(pMaterialLibraryCandidate);
}

//----------------------------------------------------------------------------//
