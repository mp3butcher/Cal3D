//----------------------------------------------------------------------------//
// SelectionDialog.cpp                                                        //
// Copyright (C) 2002 Bruno 'Beosil' Heidelberger                             //
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
#include "SelectionDialog.h"
#include "Exporter.h"

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

BEGIN_MESSAGE_MAP(CSelectionDialog, CDialog)
	//{{AFX_MSG_MAP(CSelectionDialog)
	ON_BN_CLICKED(ID_APP_ABOUT, OnAbout)
	ON_BN_CLICKED(IDC_SELECTION_ANIMATION, OnSelectionAnimation)
	ON_BN_CLICKED(IDC_SELECTION_MATERIAL, OnSelectionMaterial)
	ON_BN_CLICKED(IDC_SELECTION_MESH, OnSelectionMesh)
	ON_BN_CLICKED(IDC_SELECTION_SKELETON, OnSelectionSkeleton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//----------------------------------------------------------------------------//
// Constructors                                                               //
//----------------------------------------------------------------------------//

CSelectionDialog::CSelectionDialog(CWnd* pParent) : CDialog(CSelectionDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectionDialog)
	//}}AFX_DATA_INIT
}

//----------------------------------------------------------------------------//
// Data exchange callback                                                     //
//----------------------------------------------------------------------------//

void CSelectionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CSelectionDialog)
	//}}AFX_DATA_MAP
}

//----------------------------------------------------------------------------//
// Get the selected export type                                               //
//----------------------------------------------------------------------------//

CSelectionDialog::TYPE CSelectionDialog::GetType()
{
  return m_type;
}

//----------------------------------------------------------------------------//
// About button callback                                                      //
//----------------------------------------------------------------------------//

void CSelectionDialog::OnAbout() 
{
  CDialog dlg(IDD_ABOUT);
	dlg.DoModal();
}

//----------------------------------------------------------------------------//
// Dialog initialization callback                                             //
//----------------------------------------------------------------------------//

BOOL CSelectionDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
  // check the skeleton selection
  ((CButton *)GetDlgItem(IDC_SELECTION_SKELETON))->SetCheck(1);

  m_type = TYPE_SKELETON;

 	return TRUE;
}

//----------------------------------------------------------------------------//
// Selection callbacks                                                        //
//----------------------------------------------------------------------------//

void CSelectionDialog::OnSelectionAnimation() 
{
  m_type = TYPE_ANIMATION;
}

void CSelectionDialog::OnSelectionMaterial() 
{
  m_type = TYPE_MATERIAL;
}

void CSelectionDialog::OnSelectionMesh() 
{
  m_type = TYPE_MESH;
}

void CSelectionDialog::OnSelectionSkeleton() 
{
  m_type = TYPE_SKELETON;
}

//----------------------------------------------------------------------------//
