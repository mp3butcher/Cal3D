//----------------------------------------------------------------------------//
// SkeletonHierarchyPage.h                                                    //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef SKELETON_HIERARCHY_PAGE_H
#define SKELETON_HIERARCHY_PAGE_H

//----------------------------------------------------------------------------//
// Forward declarations                                                       //
//----------------------------------------------------------------------------//

class CSkeletonCandidate;

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CSkeletonHierarchyPage : public CPropertyPage
{
// member variables
protected:
	CImageList m_imageList;
	CSkeletonCandidate *m_pSkeletonCandidate;
	CString m_strStep;
	UINT m_nDescriptionID;
	int m_stepIndex;
	int m_stepTotal;

// constructors/destructor
public:
	CSkeletonHierarchyPage();
	virtual ~CSkeletonHierarchyPage();

// member functions
public:
	void SetDescription(UINT nID);
	void SetSkeletonCandidate(CSkeletonCandidate *pSkeletonCandidate);
	void SetStep(int index, int total);

protected:
	BOOL BeginPage();
	LRESULT EndPage();
	void InsertBoneCandidate(int boneCandidateId, HTREEITEM hParentItem);

// MSVC++ stuff
	//{{AFX_DATA(CSkeletonHierarchyPage)
	enum { IDD = IDD_SKELETON_HIERARCHY_PAGE };
	CStatic	m_stepStatic;
	CStatic	m_descriptionStatic;
	CTreeCtrl	m_hierarchyCtrl;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSkeletonHierarchyPage)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	virtual BOOL OnWizardFinish();
	protected:
	virtual void DoDataExchange(CDataExchange *pDX);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSkeletonHierarchyPage)
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
