//----------------------------------------------------------------------------//
// AnimationTimePage.cpp                                                      //
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
#include "AnimationTimePage.h"

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

BEGIN_MESSAGE_MAP(CAnimationTimePage, CPropertyPage)
	//{{AFX_MSG_MAP(CAnimationTimePage)
	ON_BN_CLICKED(ID_APP_ABOUT, OnAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//----------------------------------------------------------------------------//
// Constructors                                                               //
//----------------------------------------------------------------------------//

CAnimationTimePage::CAnimationTimePage() : CPropertyPage(CAnimationTimePage::IDD)
{
	m_nDescriptionID = IDS_NULL;

	//{{AFX_DATA_INIT(CAnimationTimePage)
	//}}AFX_DATA_INIT
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CAnimationTimePage::~CAnimationTimePage()
{
}

//----------------------------------------------------------------------------//
// Initialize the page data                                                   //
//----------------------------------------------------------------------------//

BOOL CAnimationTimePage::BeginPage()
{
	return TRUE;
}

//----------------------------------------------------------------------------//
// Data exchange callback                                                     //
//----------------------------------------------------------------------------//

void CAnimationTimePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CAnimationTimePage)
	DDX_Control(pDX, IDC_START_FRAME, m_startFrameEdit);
	DDX_Control(pDX, IDC_FPS, m_fpsEdit);
	DDX_Control(pDX, IDC_END_FRAME, m_endFrameEdit);
	DDX_Control(pDX, IDC_DISPLACEMENT, m_displacementEdit);
	DDX_Control(pDX, IDC_STEP, m_stepStatic);
	DDX_Control(pDX, IDC_DESCRIPTION, m_descriptionStatic);
	//}}AFX_DATA_MAP
}

//----------------------------------------------------------------------------//
// Cleanup the page data                                                   //
//----------------------------------------------------------------------------//

LRESULT CAnimationTimePage::EndPage()
{
	// get the time values
	CString strValue;

	m_startFrameEdit.GetWindowText(strValue);
	m_startFrame = atoi(strValue);

	m_endFrameEdit.GetWindowText(strValue);
	m_endFrame = atoi(strValue);

	m_displacementEdit.GetWindowText(strValue);
	m_displacement = atoi(strValue);

	m_fpsEdit.GetWindowText(strValue);
	m_fps = atoi(strValue);

	return 0;
}

//----------------------------------------------------------------------------//
// Get the displacement in frames                                             //
//----------------------------------------------------------------------------//

int CAnimationTimePage::GetDisplacement()
{
	return m_displacement;
}

//----------------------------------------------------------------------------//
// Get the end frame                                                          //
//----------------------------------------------------------------------------//

int CAnimationTimePage::GetEndFrame()
{
	return m_endFrame;
}

//----------------------------------------------------------------------------//
// Get the frames per second (Fps)                                            //
//----------------------------------------------------------------------------//

int CAnimationTimePage::GetFps()
{
	return m_fps;
}

//----------------------------------------------------------------------------//
// Get the start frame                                                        //
//----------------------------------------------------------------------------//

int CAnimationTimePage::GetStartFrame()
{
	return m_startFrame;
}

//----------------------------------------------------------------------------//
// About button callback                                                      //
//----------------------------------------------------------------------------//

void CAnimationTimePage::OnAbout() 
{
	CDialog dlg(IDD_ABOUT);
	dlg.DoModal();
}

//----------------------------------------------------------------------------//
// Dialog initialization callback                                             //
//----------------------------------------------------------------------------//

BOOL CAnimationTimePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// set step and description text
	m_stepStatic.SetWindowText(m_strStep);
	CString str;
	str.LoadString(m_nDescriptionID);
	m_descriptionStatic.SetWindowText(str);

	// set the time values
	CString strValue;

	strValue.Format("%d", m_startFrame);
	m_startFrameEdit.SetWindowText(strValue);

	strValue.Format("%d", m_endFrame);
	m_endFrameEdit.SetWindowText(strValue);

	strValue.Format("%d", m_displacement);
	m_displacementEdit.SetWindowText(strValue);

	strValue.Format("%d", m_fps);
	m_fpsEdit.SetWindowText(strValue);

	return TRUE;
}

//----------------------------------------------------------------------------//
// Activation callback                                                        //
//----------------------------------------------------------------------------//

BOOL CAnimationTimePage::OnSetActive() 
{
	// get property sheet
	CPropertySheet *pPropertySheet;
	pPropertySheet = dynamic_cast<CPropertySheet *>(GetParent());

	// set wizard buttons and text
	DWORD buttons;
	buttons = PSWIZB_NEXT;

	if(m_stepIndex == m_stepTotal) buttons |= PSWIZB_FINISH;
	if(m_stepIndex > 1) buttons |= PSWIZB_BACK;

	pPropertySheet->SetWizardButtons(buttons);

	// initialize pafe data
	if(!BeginPage()) return FALSE;

	return CPropertyPage::OnSetActive();
}

//----------------------------------------------------------------------------//
// Finish button callback                                                     //
//----------------------------------------------------------------------------//

BOOL CAnimationTimePage::OnWizardFinish() 
{
	// cleanup page data
	if(EndPage() == -1) return FALSE;

	return CPropertyPage::OnWizardFinish();
}

//----------------------------------------------------------------------------//
// Next button callback                                                       //
//----------------------------------------------------------------------------//

LRESULT CAnimationTimePage::OnWizardNext() 
{
	// cleanup page data
	if(EndPage() == -1) return -1;
	
	return CPropertyPage::OnWizardNext();
}

//----------------------------------------------------------------------------//
// Set the animation time values                                              //
//----------------------------------------------------------------------------//

void CAnimationTimePage::SetAnimationTime(int startFrame, int endFrame, int displacement, int fps)
{
	m_startFrame = startFrame;
	m_endFrame = endFrame;
	m_displacement = displacement;
	m_fps = fps;
}

//----------------------------------------------------------------------------//
// Set the description text id                                                //
//----------------------------------------------------------------------------//

void CAnimationTimePage::SetDescription(UINT nID)
{
	m_nDescriptionID = nID;
}

//----------------------------------------------------------------------------//
// Set the description text id                                                //
//----------------------------------------------------------------------------//

void CAnimationTimePage::SetStep(int index, int total)
{
	m_stepIndex = index;
	m_stepTotal = total;

	m_strStep.Format("Step %d of %d", m_stepIndex, m_stepTotal);
}

//----------------------------------------------------------------------------//
