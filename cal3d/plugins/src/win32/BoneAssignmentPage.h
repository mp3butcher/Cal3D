//----------------------------------------------------------------------------//
// BoneAssignmentPage.h                                                       //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef BONE_ASSIGNMENT_PAGE_H
#define BONE_ASSIGNMENT_PAGE_H

//----------------------------------------------------------------------------//
// Forward declarations                                                       //
//----------------------------------------------------------------------------//

class CSkeletonCandidate;
class CMeshCandidate;

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CBoneAssignmentPage : public CPropertyPage
{
// member variables
protected:
	CString m_strStep;
	UINT m_nDescriptionID;
	int m_stepIndex;
	int m_stepTotal;
	CMeshCandidate *m_pMeshCandidate;
	CSkeletonCandidate *m_pSkeletonCandidate;
	int m_maxBoneCount;
	float m_weightThreshold;

// constructors/destructor
public:
	CBoneAssignmentPage();
	virtual ~CBoneAssignmentPage();

// member functions
public:
	int GetMaxBoneCount();
	float GetWeightThreshold();
	void SetMeshCandidate(CMeshCandidate *pMeshCandidate);
	void SetSkeletonCandidate(CSkeletonCandidate *pSkeletonCandidate);
	void SetDescription(UINT nID);
	void SetStep(int index, int total);

protected:
	BOOL BeginPage();
	LRESULT EndPage();

// MSVC++ stuff
	//{{AFX_DATA(CBoneAssignmentPage)
	enum { IDD = IDD_BONE_ASSIGNMENT_PAGE };
	CEdit	m_maxBoneCountEdit;
	CEdit	m_weightThresholdEdit;
	CStatic	m_stepStatic;
	CStatic	m_descriptionStatic;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CBoneAssignmentPage)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	virtual BOOL OnWizardFinish();
	protected:
	virtual void DoDataExchange(CDataExchange *pDX);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CBoneAssignmentPage)
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
