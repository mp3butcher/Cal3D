//----------------------------------------------------------------------------//
// MaterialSelectionPage.cpp                                                  //
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
#include "MaterialSelectionPage.h"
#include "Exporter.h"
#include "MaterialLibraryCandidate.h"
#include "MaterialCandidate.h"

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

BEGIN_MESSAGE_MAP(CMaterialSelectionPage, CPropertyPage)
	//{{AFX_MSG_MAP(CMaterialSelectionPage)
	ON_BN_CLICKED(ID_APP_ABOUT, OnAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//----------------------------------------------------------------------------//
// Constructors                                                               //
//----------------------------------------------------------------------------//

CMaterialSelectionPage::CMaterialSelectionPage() : CPropertyPage(CMaterialSelectionPage::IDD)
{
	m_nDescriptionID = IDS_NULL;

	//{{AFX_DATA_INIT(CMaterialSelectionPage)
	//}}AFX_DATA_INIT
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CMaterialSelectionPage::~CMaterialSelectionPage()
{
}

//----------------------------------------------------------------------------//
// Initialize the page data                                                   //
//----------------------------------------------------------------------------//

BOOL CMaterialSelectionPage::BeginPage()
{
	// initialize material list control
	m_materialCtrl.ResetContent();

	if(m_pMaterialLibraryCandidate != 0)
	{
		// get root bone candidate list
		std::vector<CMaterialCandidate *>& vectorMaterialCandidate = m_pMaterialLibraryCandidate->GetVectorMaterialCandidate();

		// loop through all material candidates
		for(size_t materialCandidateId = 0; materialCandidateId < vectorMaterialCandidate.size(); materialCandidateId++)
		{
			// insert material candidate into the list control
			int candidateIndex;
			candidateIndex = m_materialCtrl.AddString(vectorMaterialCandidate[materialCandidateId]->GetName().c_str());
			m_materialCtrl.SetItemDataPtr(candidateIndex, vectorMaterialCandidate[materialCandidateId]);
		}
	}

	return TRUE;
}

//----------------------------------------------------------------------------//
// Data exchange callback                                                     //
//----------------------------------------------------------------------------//

void CMaterialSelectionPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CMaterialSelectionPage)
	DDX_Control(pDX, IDC_MATERIAL, m_materialCtrl);
	DDX_Control(pDX, IDC_STEP, m_stepStatic);
	DDX_Control(pDX, IDC_DESCRIPTION, m_descriptionStatic);
	//}}AFX_DATA_MAP
}

//----------------------------------------------------------------------------//
// Cleanup the page data                                                   //
//----------------------------------------------------------------------------//

LRESULT CMaterialSelectionPage::EndPage()
{
	// get the currently selected candidate index
	int candidateIndex;
	candidateIndex = m_materialCtrl.GetCurSel();
	if(candidateIndex == LB_ERR)
	{
		theExporter.SetLastError("No material selected.", __FILE__, __LINE__);
		AfxMessageBox(theExporter.GetLastError().c_str(), MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	// get the material candidate
	CMaterialCandidate *pMaterialCandidate;
	pMaterialCandidate = (CMaterialCandidate *)m_materialCtrl.GetItemDataPtr(candidateIndex);
	if(pMaterialCandidate == (CMaterialCandidate *)-1)
	{
		theExporter.SetLastError("Invalid material selected.", __FILE__, __LINE__);
		AfxMessageBox(theExporter.GetLastError().c_str(), MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	// mark material candidate as selected
	m_pMaterialLibraryCandidate->SetSelectedMaterialCandidate(pMaterialCandidate);

	return 0;
}

//----------------------------------------------------------------------------//
// About button callback                                                      //
//----------------------------------------------------------------------------//

void CMaterialSelectionPage::OnAbout() 
{
	CDialog dlg(IDD_ABOUT);
	dlg.DoModal();
}

//----------------------------------------------------------------------------//
// Dialog initialization callback                                             //
//----------------------------------------------------------------------------//

BOOL CMaterialSelectionPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// set step and description text
	m_stepStatic.SetWindowText(m_strStep);
	CString str;
	str.LoadString(m_nDescriptionID);
	m_descriptionStatic.SetWindowText(str);

	return TRUE;
}

//----------------------------------------------------------------------------//
// Activation callback                                                        //
//----------------------------------------------------------------------------//

BOOL CMaterialSelectionPage::OnSetActive() 
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

	// initialize page data
	if(!BeginPage()) return FALSE;

	return CPropertyPage::OnSetActive();
}

//----------------------------------------------------------------------------//
// Finish button callback                                                     //
//----------------------------------------------------------------------------//

BOOL CMaterialSelectionPage::OnWizardFinish() 
{
	// cleanup page data
	if(EndPage() == -1) return FALSE;
	
	return CPropertyPage::OnWizardFinish();
}

//----------------------------------------------------------------------------//
// Next button callback                                                       //
//----------------------------------------------------------------------------//

LRESULT CMaterialSelectionPage::OnWizardNext() 
{
	// cleanup page data
	if(EndPage() == -1) return -1;
	
	return CPropertyPage::OnWizardNext();
}

//----------------------------------------------------------------------------//
// Set the description text id                                                //
//----------------------------------------------------------------------------//

void CMaterialSelectionPage::SetDescription(UINT nID)
{
	m_nDescriptionID = nID;
}

//----------------------------------------------------------------------------//
// Set the material library                                                   //
//----------------------------------------------------------------------------//

void CMaterialSelectionPage::SetMaterialLibraryCandidate(CMaterialLibraryCandidate *pMaterialLibraryCandidate)
{
	m_pMaterialLibraryCandidate = pMaterialLibraryCandidate;
}

//----------------------------------------------------------------------------//
// Set the description text id                                                //
//----------------------------------------------------------------------------//

void CMaterialSelectionPage::SetStep(int index, int total)
{
	m_stepIndex = index;
	m_stepTotal = total;

	m_strStep.Format("Step %d of %d", m_stepIndex, m_stepTotal);
}

//----------------------------------------------------------------------------//
