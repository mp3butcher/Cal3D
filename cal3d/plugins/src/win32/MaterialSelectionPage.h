//----------------------------------------------------------------------------//
// MaterialSelectionPage.h                                                    //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef MATERIAL_SELECTION_PAGE_H
#define MATERIAL_SELECTION_PAGE_H

//----------------------------------------------------------------------------//
// Forward declarations                                                       //
//----------------------------------------------------------------------------//

class CMaterialLibraryCandidate;

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CMaterialSelectionPage : public CPropertyPage
{
// member variables
protected:
	CString m_strStep;
	UINT m_nDescriptionID;
	int m_stepIndex;
	int m_stepTotal;
	CMaterialLibraryCandidate *m_pMaterialLibraryCandidate;

// constructors/destructor
public:
	CMaterialSelectionPage();
	virtual ~CMaterialSelectionPage();

// member functions
public:
	void SetDescription(UINT nID);
	void SetMaterialLibraryCandidate(CMaterialLibraryCandidate *pMaterialLibraryCandidate);
	void SetStep(int index, int total);

protected:
	BOOL BeginPage();
	LRESULT EndPage();

// MSVC++ stuff
	//{{AFX_DATA(CMaterialSelectionPage)
	enum { IDD = IDD_MATERIAL_SELECTION_PAGE };
	CListBox	m_materialCtrl;
	CStatic	m_stepStatic;
	CStatic	m_descriptionStatic;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CMaterialSelectionPage)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	virtual BOOL OnWizardFinish();
	protected:
	virtual void DoDataExchange(CDataExchange *pDX);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMaterialSelectionPage)
	afx_msg void OnAbout();
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
