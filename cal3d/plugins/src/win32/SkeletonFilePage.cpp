//----------------------------------------------------------------------------//
// SkeletonFilePage.cpp                                                       //
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
#include "SkeletonFilePage.h"
#include "Exporter.h"
#include "SkeletonCandidate.h"

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

BEGIN_MESSAGE_MAP(CSkeletonFilePage, CPropertyPage)
	//{{AFX_MSG_MAP(CSkeletonFilePage)
	ON_BN_CLICKED(ID_APP_ABOUT, OnAbout)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//----------------------------------------------------------------------------//
// Constructors                                                               //
//----------------------------------------------------------------------------//

CSkeletonFilePage::CSkeletonFilePage() : CPropertyPage(CSkeletonFilePage::IDD)
{
	m_nDescriptionID = IDS_NULL;

	//{{AFX_DATA_INIT(CSkeletonFilePage)
	//}}AFX_DATA_INIT
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CSkeletonFilePage::~CSkeletonFilePage()
{
}

//----------------------------------------------------------------------------//
// Initialize the page data                                                   //
//----------------------------------------------------------------------------//

BOOL CSkeletonFilePage::BeginPage()
{
	HKEY hk; 
	DWORD dwtype;
	LONG lret=RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\Cal3D\\Exporter",NULL,KEY_READ|KEY_WRITE|KEY_SET_VALUE,&hk);
	if(lret==ERROR_SUCCESS && NULL!=hk)
	{
		unsigned char pbuf[256];
        DWORD dwlen=sizeof(pbuf);

		const char* valname="skeleton";
        lret=RegQueryValueEx(hk,valname,NULL,&dwtype,pbuf,&dwlen);
		if(lret==ERROR_SUCCESS)
		{
			m_lruCombo.SetWindowText((char*)pbuf);			
		}		
		RegCloseKey(hk);
	}

	return TRUE;
}

//----------------------------------------------------------------------------//
// Data exchange callback                                                     //
//----------------------------------------------------------------------------//

void CSkeletonFilePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CSkeletonFilePage)
	DDX_Control(pDX, IDC_LRU, m_lruCombo);
	DDX_Control(pDX, IDC_STEP, m_stepStatic);
	DDX_Control(pDX, IDC_DESCRIPTION, m_descriptionStatic);
	//}}AFX_DATA_MAP
}

//----------------------------------------------------------------------------//
// Cleanup the page data                                                   //
//----------------------------------------------------------------------------//

LRESULT CSkeletonFilePage::EndPage()
{
	// get the filename
	CString strFilename;
	m_lruCombo.GetWindowText(strFilename);
	if(strFilename.IsEmpty())
	{
		theExporter.SetLastError("No file selected.", __FILE__, __LINE__);
		AfxMessageBox(theExporter.GetLastError().c_str(), MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	// create the skeleton candidate from the skeleton file
	if(!m_pSkeletonCandidate->CreateFromSkeletonFile((LPCTSTR)strFilename))
	{
		AfxMessageBox(theExporter.GetLastError().c_str(), MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	return 0;
}

//----------------------------------------------------------------------------//
// About button callback                                                      //
//----------------------------------------------------------------------------//

void CSkeletonFilePage::OnAbout() 
{
	CDialog dlg(IDD_ABOUT);
	dlg.DoModal();
}

//----------------------------------------------------------------------------//
// Browse button callback                                                     //
//----------------------------------------------------------------------------//

void CSkeletonFilePage::OnBrowse() 
{
	// display file dialog
	CString strFilename;
	m_lruCombo.GetWindowText(strFilename);

	CFileDialog dlg(
      TRUE,
      "csf",
      strFilename,
      OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
      "Cal3D Skeleton Files (*.csf;*.xsf)|*.csf;*.xsf|All Files (*.*)|*.*||",
      this);
	if(dlg.DoModal() != IDOK) return;

	// set new filename
	strFilename = dlg.GetPathName();
	m_lruCombo.SetWindowText(strFilename);
	
	HKEY hk;
	LONG lret=RegCreateKey(HKEY_CURRENT_USER, "Software\\Cal3D\\Exporter", &hk);
	if(lret==ERROR_SUCCESS && NULL!=hk)
	{
		lret=RegSetValueEx(hk,"skeleton",NULL,REG_SZ,(unsigned char *)strFilename.GetBuffer(1) ,strFilename.GetLength());
		RegCloseKey(hk);
	}


}

//----------------------------------------------------------------------------//
// Dialog initialization callback                                             //
//----------------------------------------------------------------------------//

BOOL CSkeletonFilePage::OnInitDialog() 
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

BOOL CSkeletonFilePage::OnSetActive() 
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

BOOL CSkeletonFilePage::OnWizardFinish() 
{
	// cleanup page data
	if(EndPage() == -1) return FALSE;
	
	return CPropertyPage::OnWizardFinish();
}

//----------------------------------------------------------------------------//
// Next button callback                                                       //
//----------------------------------------------------------------------------//

LRESULT CSkeletonFilePage::OnWizardNext() 
{
	// cleanup page data
	if(EndPage() == -1) return -1;
	
	return CPropertyPage::OnWizardNext();
}

//----------------------------------------------------------------------------//
// Set the description text id                                                //
//----------------------------------------------------------------------------//

void CSkeletonFilePage::SetDescription(UINT nID)
{
	m_nDescriptionID = nID;
}

//----------------------------------------------------------------------------//
// Set the node hierarchy                                                     //
//----------------------------------------------------------------------------//

void CSkeletonFilePage::SetSkeletonCandidate(CSkeletonCandidate *pSkeletonCandidate)
{
	m_pSkeletonCandidate = pSkeletonCandidate;
}

//----------------------------------------------------------------------------//
// Set the description text id                                                //
//----------------------------------------------------------------------------//

void CSkeletonFilePage::SetStep(int index, int total)
{
	m_stepIndex = index;
	m_stepTotal = total;

	m_strStep.Format("Step %d of %d", m_stepIndex, m_stepTotal);
}

//----------------------------------------------------------------------------//
