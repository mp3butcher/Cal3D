//----------------------------------------------------------------------------//
// SkeletonFilePage.h                                                         //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef SKELETON_FILE_PAGE_H
#define SKELETON_FILE_PAGE_H

//----------------------------------------------------------------------------//
// Forward declarations                                                       //
//----------------------------------------------------------------------------//

class CSkeletonCandidate;

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CSkeletonFilePage : public CPropertyPage
{
// member variables
protected:
	CString m_strStep;
	CSkeletonCandidate *m_pSkeletonCandidate;
	UINT m_nDescriptionID;
	int m_stepIndex;
	int m_stepTotal;

// constructors/destructor
public:
	CSkeletonFilePage();
	virtual ~CSkeletonFilePage();

// member functions
public:
	void SetDescription(UINT nID);
	void SetSkeletonCandidate(CSkeletonCandidate *pSkeletonCandidate);
	void SetStep(int index, int total);

protected:
	BOOL BeginPage();
	LRESULT EndPage();

// MSVC++ stuff
	//{{AFX_DATA(CSkeletonFilePage)
	enum { IDD = IDD_SKELETON_FILE_PAGE };
	CComboBox	m_lruCombo;
	CStatic	m_stepStatic;
	CStatic	m_descriptionStatic;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSkeletonFilePage)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	virtual BOOL OnWizardFinish();
	protected:
	virtual void DoDataExchange(CDataExchange *pDX);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSkeletonFilePage)
	afx_msg void OnAbout();
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowse();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

};

//----------------------------------------------------------------------------//
// MSVC++ stuff                                                               //
//----------------------------------------------------------------------------//

//{{AFX_INSERT_LOCATION}}

#endif

//----------------------------------------------------------------------------//
