//----------------------------------------------------------------------------//
// AnimationTimePage.h                                                        //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef ANIMATION_TIME_PAGE_H
#define ANIMATION_TIME_PAGE_H

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CAnimationTimePage : public CPropertyPage
{
// member variables
protected:
	CString m_strStep;
	UINT m_nDescriptionID;
	int m_stepIndex;
	int m_stepTotal;
	int m_startFrame;
	int m_endFrame;
	int m_currentFrame;
	int m_displacement;
	int m_fps;

// constructors/destructor
public:
	CAnimationTimePage();
	virtual ~CAnimationTimePage();

// member functions
public:
	int GetDisplacement();
	int GetEndFrame();
	int GetFps();
	int GetStartFrame();
	void SetAnimationTime(int startFrame, int endFrame, int displacement, int fps);
	void SetDescription(UINT nID);
	void SetStep(int index, int total);

protected:
	BOOL BeginPage();
	LRESULT EndPage();

// MSVC++ stuff
	//{{AFX_DATA(CAnimationTimePage)
	enum { IDD = IDD_ANIMATION_TIME_PAGE };
	CEdit	m_startFrameEdit;
	CEdit	m_fpsEdit;
	CEdit	m_endFrameEdit;
	CEdit	m_displacementEdit;
	CStatic	m_stepStatic;
	CStatic	m_descriptionStatic;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAnimationTimePage)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	virtual BOOL OnWizardFinish();
	protected:
	virtual void DoDataExchange(CDataExchange *pDX);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CAnimationTimePage)
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
