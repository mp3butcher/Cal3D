//----------------------------------------------------------------------------//
// MaterialExportSheet.h                                                      //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef MATERIAL_EXPORT_SHEET_H
#define MATERIAL_EXPORT_SHEET_H

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#include "MaterialSelectionPage.h"
#include "MaterialMapsPage.h"

//----------------------------------------------------------------------------//
// Forward declarations                                                       //
//----------------------------------------------------------------------------//

class CMaterialLibraryCandidate;

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CMaterialExportSheet : public CPropertySheet
{
// member variables
protected:
	CMaterialSelectionPage m_materialSelectionPage;
	CMaterialMapsPage m_materialMapsPage;

// constructors/destructor
public:
	CMaterialExportSheet(UINT nIDCaption, CWnd *pParentWnd = NULL, UINT iSelectPage = 0);
	CMaterialExportSheet(LPCTSTR pszCaption, CWnd *pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CMaterialExportSheet();

// member functions
public:
	void SetMaterialLibraryCandidate(CMaterialLibraryCandidate *pMaterialLibraryCandidate);

protected:
	void FillPages();

// MSVC++ stuff
	//{{AFX_VIRTUAL(CMaterialExportSheet)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMaterialExportSheet)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//----------------------------------------------------------------------------//
// MSVC++ stuff                                                               //
//----------------------------------------------------------------------------//

//{{AFX_INSERT_LOCATION}}

#endif

//----------------------------------------------------------------------------//
