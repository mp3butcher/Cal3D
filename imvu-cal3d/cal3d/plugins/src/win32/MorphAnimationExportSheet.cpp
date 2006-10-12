//----------------------------------------------------------------------------//
// MorphAnimationExportSheet.cpp                                                   //
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
#include "MorphAnimationExportSheet.h"

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

BEGIN_MESSAGE_MAP(CMorphAnimationExportSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CMorphAnimationExportSheet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//----------------------------------------------------------------------------//
// Constructors                                                               //
//----------------------------------------------------------------------------//

CMorphAnimationExportSheet::CMorphAnimationExportSheet(UINT nIDCaption, CWnd *pParentWnd, UINT iSelectPage) : CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	FillPages();
}

CMorphAnimationExportSheet::CMorphAnimationExportSheet(LPCTSTR pszCaption, CWnd *pParentWnd, UINT iSelectPage) : CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	FillPages();
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CMorphAnimationExportSheet::~CMorphAnimationExportSheet()
{
}

//----------------------------------------------------------------------------//
// Fill in all propoerty pages                                                //
//----------------------------------------------------------------------------//

void CMorphAnimationExportSheet::FillPages()
{
	// add all property pages to this sheet

	m_morphAnimationTimePage.SetStep(3, 3);
	m_morphAnimationTimePage.SetDescription(IDS_ANIMATION_EXPORT_DESCRIPTION_3);
	AddPage(&m_morphAnimationTimePage);
}

//----------------------------------------------------------------------------//
// Get the displacement in frames                                             //
//----------------------------------------------------------------------------//

int CMorphAnimationExportSheet::GetDisplacement()
{
	return m_morphAnimationTimePage.GetDisplacement();
}

//----------------------------------------------------------------------------//
// Get the end frame                                                          //
//----------------------------------------------------------------------------//

int CMorphAnimationExportSheet::GetEndFrame()
{
	return m_morphAnimationTimePage.GetEndFrame();
}

//----------------------------------------------------------------------------//
// Get the frames per second (Fps)                                            //
//----------------------------------------------------------------------------//

int CMorphAnimationExportSheet::GetFps()
{
	return m_morphAnimationTimePage.GetFps();
}

//----------------------------------------------------------------------------//
// Get the start frame                                                        //
//----------------------------------------------------------------------------//

int CMorphAnimationExportSheet::GetStartFrame()
{
	return m_morphAnimationTimePage.GetStartFrame();
}

//----------------------------------------------------------------------------//
// Set the morphAnimation time values                                              //
//----------------------------------------------------------------------------//

void CMorphAnimationExportSheet::SetMorphAnimationTime(int startFrame, int endFrame, int displacementFrame, int fps)
{
	m_morphAnimationTimePage.SetMorphAnimationTime(startFrame, endFrame, displacementFrame, fps);
}




//----------------------------------------------------------------------------//
