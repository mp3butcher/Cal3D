//----------------------------------------------------------------------------//
// MaterialMapDialog.cpp                                                      //
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
#include "MaterialMapDialog.h"

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

BEGIN_MESSAGE_MAP(CMaterialMapDialog, CDialog)
	//{{AFX_MSG_MAP(CMaterialMapDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//----------------------------------------------------------------------------//
// Constructors                                                               //
//----------------------------------------------------------------------------//

CMaterialMapDialog::CMaterialMapDialog(CWnd *pParent) : CDialog(CMaterialMapDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMaterialMapDialog)
	m_strFilename = _T("");
	m_strName = _T("");
	//}}AFX_DATA_INIT
}

//----------------------------------------------------------------------------//
// Data exchange callback                                                     //
//----------------------------------------------------------------------------//

void CMaterialMapDialog::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CMaterialMapDialog)
	DDX_Text(pDX, IDC_MAP_FILENAME, m_strFilename);
	DDX_Text(pDX, IDC_MAP_NAME, m_strName);
	//}}AFX_DATA_MAP
}

//----------------------------------------------------------------------------//
// Get the filename of the material map                                       //
//----------------------------------------------------------------------------//

CString CMaterialMapDialog::GetFilename()
{
  return m_strFilename;
}

//----------------------------------------------------------------------------//
// Dialog initialization callback                                             //
//----------------------------------------------------------------------------//

BOOL CMaterialMapDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	return TRUE;
}

//----------------------------------------------------------------------------//
// Set the filename of the material map                                       //
//----------------------------------------------------------------------------//

void CMaterialMapDialog::SetFilename(CString& strFilename)
{
  m_strFilename = strFilename;
}

//----------------------------------------------------------------------------//
// Set the name of the material map                                           //
//----------------------------------------------------------------------------//

void CMaterialMapDialog::SetName(CString& strName)
{
  m_strName = strName;
}

//----------------------------------------------------------------------------//
