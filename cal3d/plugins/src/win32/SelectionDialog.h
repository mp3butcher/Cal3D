//----------------------------------------------------------------------------//
// SelectionDialog.h                                                          //
// Copyright (C) 2002 Bruno 'Beosil' Heidelberger                             //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef SELECTION_DIALOG_H
#define SELECTION_DIALOG_H

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CSelectionDialog : public CDialog
{
// miscellaneous
public:
  enum TYPE
  {
    TYPE_NONE,
    TYPE_SKELETON,
    TYPE_ANIMATION,
    TYPE_MESH,
    TYPE_MATERIAL
  };

// member variables
protected:
  TYPE m_type;

// constructors/destructor
public:
	CSelectionDialog(CWnd* pParent = NULL);

// member functions
public:
  TYPE GetType();

// MSVC++ stuff
	//{{AFX_DATA(CSelectionDialog)
	enum { IDD = IDD_SELECTION };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSelectionDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSelectionDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnAbout();
	afx_msg void OnSelectionAnimation();
	afx_msg void OnSelectionMaterial();
	afx_msg void OnSelectionMesh();
	afx_msg void OnSelectionSkeleton();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
//----------------------------------------------------------------------------//
// MSVC++ stuff                                                               //
//----------------------------------------------------------------------------//

//{{AFX_INSERT_LOCATION}}

#endif

//----------------------------------------------------------------------------//
