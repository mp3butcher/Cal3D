//----------------------------------------------------------------------------//
// SpringSystemPage.h                                                         //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef SPRINGSYSTEM_PAGE_H
#define SPRINGSYSTEM_PAGE_H

//----------------------------------------------------------------------------//
// Forward declarations                                                       //
//----------------------------------------------------------------------------//

class CMeshCandidate;

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CSpringSystemPage : public CPropertyPage
{
// member variables
protected:
	CString m_strStep;
	UINT m_nDescriptionID;
	int m_stepIndex;
	int m_stepTotal;
	CMeshCandidate *m_pMeshCandidate;

// constructors/destructor
public:
	CSpringSystemPage();
	~CSpringSystemPage();

// member functions
public:
	void SetMeshCandidate(CMeshCandidate *pMeshCandidate);
	void SetDescription(UINT nID);
	void SetStep(int index, int total);

protected:
	BOOL BeginPage();
	LRESULT EndPage();

// MSVC++ stuff
	//{{AFX_DATA(CSpringSystemPage)
	enum { IDD = IDD_SPRINGSYSTEM_PAGE };
	CButton	m_enableSpringSystemButton;
	CStatic	m_stepStatic;
	CStatic	m_descriptionStatic;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSpringSystemPage)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	virtual BOOL OnWizardFinish();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSpringSystemPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnAbout();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//----------------------------------------------------------------------------//
// MSVC++ stuff                                                               //
//----------------------------------------------------------------------------//

//{{AFX_INSERT_LOCATION}}

#endif

//----------------------------------------------------------------------------//
