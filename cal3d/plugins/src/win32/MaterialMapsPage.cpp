//----------------------------------------------------------------------------//
// MaterialMapsPage.cpp                                                       //
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
#include "MaterialMapsPage.h"
#include "MaterialMapDialog.h"
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

BEGIN_MESSAGE_MAP(CMaterialMapsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CMaterialMapsPage)
	ON_BN_CLICKED(ID_APP_ABOUT, OnAbout)
	ON_NOTIFY(NM_DBLCLK, IDC_MAPS, OnMapDoubleClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//----------------------------------------------------------------------------//
// Constructors                                                               //
//----------------------------------------------------------------------------//

CMaterialMapsPage::CMaterialMapsPage() : CPropertyPage(CMaterialMapsPage::IDD)
{
	m_nDescriptionID = IDS_NULL;

	//{{AFX_DATA_INIT(CMaterialMapsPage)
	//}}AFX_DATA_INIT
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CMaterialMapsPage::~CMaterialMapsPage()
{
}

//----------------------------------------------------------------------------//
// Initialize the page data                                                   //
//----------------------------------------------------------------------------//

BOOL CMaterialMapsPage::BeginPage()
{
	// initialize maps control
	m_mapsCtrl.DeleteAllItems();

	// get the selected material candidate
	CMaterialCandidate *pMaterialCandidate;
	pMaterialCandidate = m_pMaterialLibraryCandidate->GetSelectedMaterialCandidate();
	if(pMaterialCandidate == 0)
	{
		theExporter.SetLastError("No material selected!", __FILE__, __LINE__);
		AfxMessageBox(theExporter.GetLastError().c_str(), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	// get map vector of material candidate
	std::vector<CMaterialCandidate::Map>& vectorMap = pMaterialCandidate->GetVectorMap();

	// loop through all maps of the material candidate
	for(size_t mapId = 0; mapId < vectorMap.size(); mapId++)
	{
		CString strMap;
		strMap.Format("Layer #%d", mapId);
		int hItem = m_mapsCtrl.InsertItem(mapId, strMap);
		m_mapsCtrl.SetItemText(hItem, 1, vectorMap[mapId].strFilename.c_str());
	}

	return TRUE;
}

//----------------------------------------------------------------------------//
// Data exchange callback                                                     //
//----------------------------------------------------------------------------//

void CMaterialMapsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CMaterialMapsPage)
	DDX_Control(pDX, IDC_MAPS, m_mapsCtrl);
	DDX_Control(pDX, IDC_STEP, m_stepStatic);
	DDX_Control(pDX, IDC_DESCRIPTION, m_descriptionStatic);
	//}}AFX_DATA_MAP
}

//----------------------------------------------------------------------------//
// Cleanup the page data                                                   //
//----------------------------------------------------------------------------//

LRESULT CMaterialMapsPage::EndPage()
{
	return 0;
}

//----------------------------------------------------------------------------//
// About button callback                                                      //
//----------------------------------------------------------------------------//

void CMaterialMapsPage::OnAbout() 
{
	CDialog dlg(IDD_ABOUT);
	dlg.DoModal();
}

//----------------------------------------------------------------------------//
// Dialog initialization callback                                             //
//----------------------------------------------------------------------------//

BOOL CMaterialMapsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// set step and description text
	m_stepStatic.SetWindowText(m_strStep);
	CString str;
	str.LoadString(m_nDescriptionID);
	m_descriptionStatic.SetWindowText(str);

	// initialize the columns of the maps control
	m_mapsCtrl.InsertColumn(0, "Map");
	m_mapsCtrl.SetColumnWidth(0, 60);
	m_mapsCtrl.InsertColumn(1, "Filename");
	m_mapsCtrl.SetColumnWidth(1, 400);

  m_mapsCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	return TRUE;
}

//----------------------------------------------------------------------------//
// Doubleclick callback                                                       //
//----------------------------------------------------------------------------//

void CMaterialMapsPage::OnMapDoubleClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int selectedItem;
  selectedItem = m_mapsCtrl.GetNextItem(-1, LVNI_SELECTED);
	
  if(selectedItem >= 0)
  {
    CMaterialMapDialog dlg;

    // set the name in the material map dialog
    CString str;
    str.Format("Layer #%d", selectedItem);
    dlg.SetName(str);

	  // get the selected material candidate
	  CMaterialCandidate *pMaterialCandidate;
	  pMaterialCandidate = m_pMaterialLibraryCandidate->GetSelectedMaterialCandidate();

    // get the map vector of material candidate
	  std::vector<CMaterialCandidate::Map>& vectorMap = pMaterialCandidate->GetVectorMap();

    // set the filename in the material map dialog
    str = vectorMap[selectedItem].strFilename.c_str();
    dlg.SetFilename(str);

    if(dlg.DoModal() == IDOK)
    {
      // store new filename from the material map dialog
      vectorMap[selectedItem].strFilename = dlg.GetFilename();

      // update the material map list
   		m_mapsCtrl.SetItemText(selectedItem, 1, dlg.GetFilename());
    }
  }

	*pResult = 0;
}

//----------------------------------------------------------------------------//
// Activation callback                                                        //
//----------------------------------------------------------------------------//

BOOL CMaterialMapsPage::OnSetActive() 
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

BOOL CMaterialMapsPage::OnWizardFinish() 
{
	// cleanup page data
	EndPage();
	
	return CPropertyPage::OnWizardFinish();
}

//----------------------------------------------------------------------------//
// Next button callback                                                       //
//----------------------------------------------------------------------------//

LRESULT CMaterialMapsPage::OnWizardNext() 
{
	// cleanup page data
	EndPage();
	
	return CPropertyPage::OnWizardNext();
}

//----------------------------------------------------------------------------//
// Set the description text id                                                //
//----------------------------------------------------------------------------//

void CMaterialMapsPage::SetDescription(UINT nID)
{
	m_nDescriptionID = nID;
}

//----------------------------------------------------------------------------//
// Set the material library                                                   //
//----------------------------------------------------------------------------//

void CMaterialMapsPage::SetMaterialLibraryCandidate(CMaterialLibraryCandidate *pMaterialLibraryCandidate)
{
	m_pMaterialLibraryCandidate = pMaterialLibraryCandidate;
}

//----------------------------------------------------------------------------//
// Set the description text id                                                //
//----------------------------------------------------------------------------//

void CMaterialMapsPage::SetStep(int index, int total)
{
	m_stepIndex = index;
	m_stepTotal = total;

	m_strStep.Format("Step %d of %d", m_stepIndex, m_stepTotal);
}

//----------------------------------------------------------------------------//
