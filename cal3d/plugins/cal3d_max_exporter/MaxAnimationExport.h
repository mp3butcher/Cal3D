//----------------------------------------------------------------------------//
// MaxAnimationExport.h                                                       //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef MAX_ANIMATION_EXPORT_H
#define MAX_ANIMATION_EXPORT_H

class AnimExportParams
{
	AnimExportParams();
public:
	AnimExportParams(const char* _skeletonfilepath, INodeTab& _tabbones, int startframe, int endframe, int frameoffset, int framerate);
	~AnimExportParams();
	
	char*		m_skeletonfilepath;
	int			m_startframe;
	int			m_endframe;
	int			m_frameoffset;
	int			m_framerate;
	INodeTab	m_tabbones;
};

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//
class CMaxAnimationExport : public SceneExport
{
// constructors/destructor
public:
	CMaxAnimationExport();
	virtual ~CMaxAnimationExport();

// interface functions	
public:
	const TCHAR *AuthorName();
	const TCHAR *CopyrightMessage();
	int DoExport(const TCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts, DWORD options);
	const TCHAR *Ext(int n);
	int ExtCount();
	const TCHAR *LongDesc();
	const TCHAR *OtherMessage1();
	const TCHAR *OtherMessage2();
	const TCHAR *ShortDesc();
	void ShowAbout(HWND hWnd);
	unsigned int Version();

	static bool ExportAnimationFromMaxscriptCall(const TCHAR *name, AnimExportParams* _animexportparams);
};

#endif

//----------------------------------------------------------------------------//
