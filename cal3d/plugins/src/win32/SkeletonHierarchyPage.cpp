//----------------------------------------------------------------------------//
// SkeletonHierarchyPage.cpp                                                  //
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
#include "SkeletonHierarchyPage.h"
#include "SkeletonCandidate.h"
#include "BoneCandidate.h"
#include "BaseNode.h"

//----------------------------------------------------------------------------//
// Message mapping                                                            //
//----------------------------------------------------------------------------//

BEGIN_MESSAGE_MAP(CSkeletonHierarchyPage, CPropertyPage)
	//{{AFX_MSG_MAP(CSkeletonHierarchyPage)
	ON_BN_CLICKED(ID_APP_ABOUT, OnAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//----------------------------------------------------------------------------//
// Constructors                                                               //
//----------------------------------------------------------------------------//

CSkeletonHierarchyPage::CSkeletonHierarchyPage() : CPropertyPage(CSkeletonHierarchyPage::IDD)
{
	m_nDescriptionID = IDS_NULL;

	//{{AFX_DATA_INIT(CSkeletonHierarchyPage)
	//}}AFX_DATA_INIT
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CSkeletonHierarchyPage::~CSkeletonHierarchyPage()
{
}

//----------------------------------------------------------------------------//
// Initialize the page data                                                   //
//----------------------------------------------------------------------------//

BOOL CSkeletonHierarchyPage::BeginPage()
{
	// initialize hierarchy tree control
	m_hierarchyCtrl.DeleteAllItems();

	if(m_pSkeletonCandidate)
	{
		// get root bone candidate list
		std::list<int>& listRootBoneCandidateId = m_pSkeletonCandidate->GetListRootBoneCandidateId();

		// loop through all root bone candidates
		std::list<int>::iterator iteratorBoneCandidateId;
		for(iteratorBoneCandidateId = listRootBoneCandidateId.begin(); iteratorBoneCandidateId != listRootBoneCandidateId.end(); ++iteratorBoneCandidateId)
		{
			// insert bone candidate into the tree control
			InsertBoneCandidate(*iteratorBoneCandidateId, TVI_ROOT);
		}
	}

	return TRUE;
}

//----------------------------------------------------------------------------//
// Data exchange callback                                                     //
//----------------------------------------------------------------------------//

void CSkeletonHierarchyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CSkeletonHierarchyPage)
	DDX_Control(pDX, IDC_STEP, m_stepStatic);
	DDX_Control(pDX, IDC_DESCRIPTION, m_descriptionStatic);
	DDX_Control(pDX, IDC_HIERARCHY, m_hierarchyCtrl);
	//}}AFX_DATA_MAP
}

//----------------------------------------------------------------------------//
// Cleanup the page data                                                   //
//----------------------------------------------------------------------------//

LRESULT CSkeletonHierarchyPage::EndPage()
{
	// get bone candidate vector
	std::vector<CBoneCandidate *>& vectorBoneCandidate = m_pSkeletonCandidate->GetVectorBoneCandidate();

	// write back the select state of all bone candidates
	HTREEITEM hItem;
	hItem = m_hierarchyCtrl.GetRootItem();
	if(hItem != 0)
	{
		// create a stack
		std::stack<HTREEITEM> stackItem;

		// insert all root items into the stack
		while(hItem != 0)
		{
			stackItem.push(hItem);
			hItem = m_hierarchyCtrl.GetNextSiblingItem(hItem);
		}

		// loop until we handled all items in the stack
		while(!stackItem.empty())
		{
			// get next item
			hItem = stackItem.top();
			stackItem.pop();

			// get bone candidate id
			int boneCandidateId;
			boneCandidateId = (int)m_hierarchyCtrl.GetItemData(hItem);

			// set new select state of the bone candidate
			vectorBoneCandidate[boneCandidateId]->SetSelected((m_hierarchyCtrl.GetItemState(hItem, TVIS_STATEIMAGEMASK) & INDEXTOSTATEIMAGEMASK(2)) != 0);

			// fill all children of the current item into the stack
			if(m_hierarchyCtrl.ItemHasChildren(hItem))
			{
				// get the first child of the current item
				hItem = m_hierarchyCtrl.GetChildItem(hItem);

				// insert all child items into the stack
				while(hItem != 0)
				{
					stackItem.push(hItem);
					hItem = m_hierarchyCtrl.GetNextSiblingItem(hItem);
				}
			}
		}
	}

	return 0;
}

//----------------------------------------------------------------------------//
// Insert a given node element into the hierarchy tree control                //
//----------------------------------------------------------------------------//

void CSkeletonHierarchyPage::InsertBoneCandidate(int boneCandidateId, HTREEITEM hParentItem)
{
	// get bone candidate vector
	std::vector<CBoneCandidate *>& vectorBoneCandidate = m_pSkeletonCandidate->GetVectorBoneCandidate();

	// check if we have a valid bone candidate id
	if((boneCandidateId < 0) || (boneCandidateId >= (int)vectorBoneCandidate.size())) return;

	// get bone candidate
	CBoneCandidate *pBoneCandidate = vectorBoneCandidate[boneCandidateId];

	// insert bone candidate into the tree control
	HTREEITEM hItem = m_hierarchyCtrl.InsertItem(
                TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM | TVIF_STATE,
		pBoneCandidate->GetNode()->GetName().c_str(),
		pBoneCandidate->GetNode()->GetType(), pBoneCandidate->GetNode()->GetType(),
		TVIS_EXPANDED | INDEXTOSTATEIMAGEMASK(pBoneCandidate->IsSelected() ? 2 : 1), TVIS_EXPANDED | TVIS_STATEIMAGEMASK,
		pBoneCandidate->GetId(),
		hParentItem, TVI_LAST);

	// get child id list of bone candidate
	std::list<int>& listChildId = pBoneCandidate->GetListChildId();

	// insert all children bone candidates into the tree control
	std::list<int>::iterator iteratorChildId;
	for(iteratorChildId = listChildId.begin(); iteratorChildId != listChildId.end(); ++iteratorChildId)
	{
		InsertBoneCandidate(*iteratorChildId, hItem);
	}
}

//----------------------------------------------------------------------------//
// About button callback                                                      //
//----------------------------------------------------------------------------//

void CSkeletonHierarchyPage::OnAbout() 
{
	CDialog dlg(IDD_ABOUT);
	dlg.DoModal();
}

//----------------------------------------------------------------------------//
// Dialog initialization callback                                             //
//----------------------------------------------------------------------------//

BOOL CSkeletonHierarchyPage::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// set step and description text
	m_stepStatic.SetWindowText(m_strStep);
	CString str;
	str.LoadString(m_nDescriptionID);
	m_descriptionStatic.SetWindowText(str);

	// initialize image list
	m_imageList.Create(16, 16, ILC_COLOR | ILC_MASK, 3, 1);
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_TYPE_OTHER));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_TYPE_BONE));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_TYPE_DUMMY));
	m_hierarchyCtrl.SetImageList(&m_imageList, TVSIL_NORMAL);

	return TRUE;
}

//----------------------------------------------------------------------------//
// Activation callback                                                        //
//----------------------------------------------------------------------------//

BOOL CSkeletonHierarchyPage::OnSetActive() 
{
	// get property sheet
	CPropertySheet *pPropertySheet = dynamic_cast<CPropertySheet *>(GetParent());

	// set wizard buttons and text
	DWORD buttons = PSWIZB_NEXT;

	if(m_stepIndex == m_stepTotal) buttons |= PSWIZB_FINISH;
	if(m_stepIndex > 1) buttons |= PSWIZB_BACK;

	pPropertySheet->SetWizardButtons(buttons);

	// initialize page data
	if(!BeginPage()) return FALSE;

	return CPropertyPage::OnSetActive();
}

//----------------------------------------------------------------------------//
// Finish button callback                                                //
//----------------------------------------------------------------------------//

BOOL CSkeletonHierarchyPage::OnWizardFinish() 
{
	// cleanup page data
	if(EndPage() == -1) return FALSE;
	
	return CPropertyPage::OnWizardFinish();
}

//----------------------------------------------------------------------------//
// Next button callback                                                       //
//----------------------------------------------------------------------------//

LRESULT CSkeletonHierarchyPage::OnWizardNext() 
{
	// cleanup page data
	if(EndPage() == -1) return -1;
	
	return CPropertyPage::OnWizardNext();
}

//----------------------------------------------------------------------------//
// Set the description text id                                                //
//----------------------------------------------------------------------------//

void CSkeletonHierarchyPage::SetDescription(UINT nID)
{
	m_nDescriptionID = nID;
}

//----------------------------------------------------------------------------//
// Set the node hierarchy                                                     //
//----------------------------------------------------------------------------//

void CSkeletonHierarchyPage::SetSkeletonCandidate(CSkeletonCandidate *pSkeletonCandidate)
{
	m_pSkeletonCandidate = pSkeletonCandidate;
}

//----------------------------------------------------------------------------//
// Set the description text id                                                //
//----------------------------------------------------------------------------//

void CSkeletonHierarchyPage::SetStep(int index, int total)
{
	m_stepIndex = index;
	m_stepTotal = total;

	m_strStep.Format("Step %d of %d", index, total);
}

//----------------------------------------------------------------------------//
