//----------------------------------------------------------------------------//
// BoneAssignmentPage.cpp                                                     //
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
#include "BoneAssignmentPage.h"
#include "Exporter.h"
#include "MeshCandidate.h"

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

BEGIN_MESSAGE_MAP(CBoneAssignmentPage, CPropertyPage)
	//{{AFX_MSG_MAP(CBoneAssignmentPage)
	ON_BN_CLICKED(ID_APP_ABOUT, OnAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//----------------------------------------------------------------------------//
// Constructors                                                               //
//----------------------------------------------------------------------------//

CBoneAssignmentPage::CBoneAssignmentPage() : CPropertyPage(CBoneAssignmentPage::IDD)
{
	m_nDescriptionID = IDS_NULL;

	m_maxBoneCount = 999;
	m_weightThreshold = 0.01f;

	//{{AFX_DATA_INIT(CBoneAssignmentPage)
	//}}AFX_DATA_INIT
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CBoneAssignmentPage::~CBoneAssignmentPage()
{
}

//----------------------------------------------------------------------------//
// Initialize the page data                                                   //
//----------------------------------------------------------------------------//

BOOL CBoneAssignmentPage::BeginPage()
{
	return TRUE;
}

//----------------------------------------------------------------------------//
// Data exchange callback                                                     //
//----------------------------------------------------------------------------//

void CBoneAssignmentPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CBoneAssignmentPage)
	DDX_Control(pDX, IDC_MAX_BONE_COUNT, m_maxBoneCountEdit);
	DDX_Control(pDX, IDC_WEIGHT_THRESHOLD, m_weightThresholdEdit);
	DDX_Control(pDX, IDC_STEP, m_stepStatic);
	DDX_Control(pDX, IDC_DESCRIPTION, m_descriptionStatic);
	//}}AFX_DATA_MAP
}

//----------------------------------------------------------------------------//
// Cleanup the page data                                                   //
//----------------------------------------------------------------------------//

LRESULT CBoneAssignmentPage::EndPage()
{
	// get the time values
	CString strValue;

	m_maxBoneCountEdit.GetWindowText(strValue);
	m_maxBoneCount = atoi(strValue);

	m_weightThresholdEdit.GetWindowText(strValue);
	m_weightThreshold = float(atof(strValue));

	// create the mesh candidate
	if(!m_pMeshCandidate->Create(m_pSkeletonCandidate, m_maxBoneCount, m_weightThreshold))
	{
		AfxMessageBox(theExporter.GetLastError().c_str(), MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	return 0;
}

//----------------------------------------------------------------------------//
// Get the maximum bone count per mesh vertex                                 //
//----------------------------------------------------------------------------//

int CBoneAssignmentPage::GetMaxBoneCount()
{
	return m_maxBoneCount;
}

//----------------------------------------------------------------------------//
// Get the weight threshold for bones                                         //
//----------------------------------------------------------------------------//

float CBoneAssignmentPage::GetWeightThreshold()
{
	return m_weightThreshold;
}

//----------------------------------------------------------------------------//
// About button callback                                                      //
//----------------------------------------------------------------------------//

void CBoneAssignmentPage::OnAbout() 
{
	CDialog dlg(IDD_ABOUT);
	dlg.DoModal();
}

//----------------------------------------------------------------------------//
// Dialog initialization callback                                             //
//----------------------------------------------------------------------------//

BOOL CBoneAssignmentPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// set step and description text
	m_stepStatic.SetWindowText(m_strStep);
	CString str;
	str.LoadString(m_nDescriptionID);
	m_descriptionStatic.SetWindowText(str);

	// set the time values
	CString strValue;

	strValue.Format("%d", m_maxBoneCount);
	m_maxBoneCountEdit.SetWindowText(strValue);

	strValue.Format("%f", m_weightThreshold);
	m_weightThresholdEdit.SetWindowText(strValue);

	return TRUE;
}

//----------------------------------------------------------------------------//
// Activation callback                                                        //
//----------------------------------------------------------------------------//

BOOL CBoneAssignmentPage::OnSetActive() 
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

BOOL CBoneAssignmentPage::OnWizardFinish() 
{
	// cleanup page data
	if(EndPage() == -1) return FALSE;

	return CPropertyPage::OnWizardFinish();
}

//----------------------------------------------------------------------------//
// Next button callback                                                       //
//----------------------------------------------------------------------------//

LRESULT CBoneAssignmentPage::OnWizardNext() 
{
	// cleanup page data
	if(EndPage() == -1) return -1;
	
	return CPropertyPage::OnWizardNext();
}

//----------------------------------------------------------------------------//
// Set the description text id                                                //
//----------------------------------------------------------------------------//

void CBoneAssignmentPage::SetDescription(UINT nID)
{
	m_nDescriptionID = nID;
}

//----------------------------------------------------------------------------//
// Set the mesh candidate                                                     //
//----------------------------------------------------------------------------//

void CBoneAssignmentPage::SetMeshCandidate(CMeshCandidate *pMeshCandidate)
{
	m_pMeshCandidate = pMeshCandidate;
}

//----------------------------------------------------------------------------//
// Set the skeleton candidate                                                 //
//----------------------------------------------------------------------------//

void CBoneAssignmentPage::SetSkeletonCandidate(CSkeletonCandidate *pSkeletonCandidate)
{
	m_pSkeletonCandidate = pSkeletonCandidate;
}

//----------------------------------------------------------------------------//
// Set the description text id                                                //
//----------------------------------------------------------------------------//

void CBoneAssignmentPage::SetStep(int index, int total)
{
	m_stepIndex = index;
	m_stepTotal = total;

	m_strStep.Format("Step %d of %d", m_stepIndex, m_stepTotal);
}

//----------------------------------------------------------------------------//
