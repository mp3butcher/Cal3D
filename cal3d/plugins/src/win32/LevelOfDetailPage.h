//----------------------------------------------------------------------------//
// LevelOfDetailPage.h                                                        //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef LEVEL_OF_DETAIL_PAGE_H
#define LEVEL_OF_DETAIL_PAGE_H

//----------------------------------------------------------------------------//
// Forward declarations                                                       //
//----------------------------------------------------------------------------//

class CMeshCandidate;

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CLevelOfDetailPage : public CPropertyPage
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
	CLevelOfDetailPage();
	virtual ~CLevelOfDetailPage();

// member functions
public:
	void SetMeshCandidate(CMeshCandidate *pMeshCandidate);
	void SetDescription(UINT nID);
	void SetStep(int index, int total);

protected:
	BOOL BeginPage();
	LRESULT EndPage();

// MSVC++ stuff
	//{{AFX_DATA(CLevelOfDetailPage)
	enum { IDD = IDD_LOD_PAGE };
	CButton	m_enableLODButton;
	CStatic	m_stepStatic;
	CStatic	m_descriptionStatic;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CLevelOfDetailPage)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	virtual BOOL OnWizardFinish();
	protected:
	virtual void DoDataExchange(CDataExchange *pDX);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CLevelOfDetailPage)
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
