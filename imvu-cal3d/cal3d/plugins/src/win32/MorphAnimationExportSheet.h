//----------------------------------------------------------------------------//
// MorphAnimationExportSheet.h                                                     //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef MORPHANIMATION_EXPORT_SHEET_H
#define MORPHANIMATION_EXPORT_SHEET_H

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#include "MorphAnimationTimePage.h"

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CMorphAnimationExportSheet : public CPropertySheet
{
// member variables
protected:
	CMorphAnimationTimePage m_morphAnimationTimePage;

// constructors/destructor
public:
	CMorphAnimationExportSheet(UINT nIDCaption, CWnd *pParentWnd = NULL, UINT iSelectPage = 0);
	CMorphAnimationExportSheet(LPCTSTR pszCaption, CWnd *pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CMorphAnimationExportSheet();

// member functions
public:
	int GetDisplacement();
	int GetEndFrame();
	int GetFps();
	int GetStartFrame();
	void SetMorphAnimationTime(int startFrame, int endFrame, int displacementFrame, int fps);

protected:
	void FillPages();

// MSVC++ stuff
	//{{AFX_VIRTUAL(CMorphAnimationExportSheet)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMorphAnimationExportSheet)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//----------------------------------------------------------------------------//
// MSVC++ stuff                                                               //
//----------------------------------------------------------------------------//

//{{AFX_INSERT_LOCATION}}

#endif

//----------------------------------------------------------------------------//
