//----------------------------------------------------------------------------//
// MaxAnimationExportDesc.h                                                   //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef MAX_ANIMATION_EXPORT_DESC_H
#define MAX_ANIMATION_EXPORT_DESC_H

//----------------------------------------------------------------------------//
// Defines                                                                    //
//----------------------------------------------------------------------------//

#define MAX_ANIMATION_EXPORT_ID Class_ID(0x50944e94, 0x515a40e7)

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CMaxAnimationExportDesc : public ClassDesc2
{
// member variables
protected:
	HINSTANCE m_hInstance;

// constructors/destructor
public:
	CMaxAnimationExportDesc();
	virtual ~CMaxAnimationExportDesc();

// member functions	
public:
	void SetInstance(HINSTANCE hInstance);

// interface functions	
public:
	const TCHAR *Category();
	Class_ID ClassID();
	const TCHAR *ClassName();
	void *Create(BOOL loading = FALSE);
	HINSTANCE HInstance();
	const TCHAR *InternalName(); 
	int IsPublic();
	SClass_ID SuperClassID();
	char * GetRsrcString(long n);
};

#endif

//----------------------------------------------------------------------------//
