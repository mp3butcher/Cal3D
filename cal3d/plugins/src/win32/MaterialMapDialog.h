//----------------------------------------------------------------------------//
// MaterialMapDialog.h                                                        //
// Copyright (C) 2002 Bruno 'Beosil' Heidelberger                             //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef MATERIAL_MAP_DIALOG_H
#define MATERIAL_MAP_DIALOG_H

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CMaterialMapDialog : public CDialog
{
// member variables
protected:

// constructors/destructor
public:
	CMaterialMapDialog(CWnd *pParent = NULL);

// member functions
public:
  CString GetFilename();
  void SetFilename(CString& strFilename);
  void SetName(CString& strName);

// MSVC++ stuff
	//{{AFX_DATA(CMaterialMapDialog)
	enum { IDD = IDD_MATERIAL_MAP };
	CString	m_strFilename;
	CString	m_strName;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CMaterialMapDialog)
	protected:
	virtual void DoDataExchange(CDataExchange *pDX);
	//}}AFX_VIRTUAL

  //{{AFX_MSG(CMaterialMapDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG

  DECLARE_MESSAGE_MAP()
};

//----------------------------------------------------------------------------//
// MSVC++ stuff                                                               //
//----------------------------------------------------------------------------//

//{{AFX_INSERT_LOCATION}}

#endif

//----------------------------------------------------------------------------//
