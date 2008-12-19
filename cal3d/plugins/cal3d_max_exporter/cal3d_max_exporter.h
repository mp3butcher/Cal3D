//----------------------------------------------------------------------------//
// cal3d_max_exporter.h                                                       //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef CAL3D_MAX_EXPORTER_H
#define CAL3D_MAX_EXPORTER_H

//----------------------------------------------------------------------------//
// Forward declarations                                                       //
//----------------------------------------------------------------------------//

class CMaxSkeletonExportDesc;
class CMaxAnimationExportDesc;
class CMaxMorphAnimationExportDesc;
class CMaxMeshExportDesc;
class CMaxMaterialExportDesc;

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CMaxExporterApp : public CWinApp
{
// member variables
protected:
	static CMaxSkeletonExportDesc m_maxSkeletonExportDesc;
	static CMaxAnimationExportDesc m_maxAnimationExportDesc;
	static CMaxMorphAnimationExportDesc m_maxMorphAnimationExportDesc;
	static CMaxMeshExportDesc m_maxMeshExportDesc;
	static CMaxMaterialExportDesc m_maxMaterialExportDesc;

// constructors/destructor
public:
	CMaxExporterApp();

// member functions
public:
	ClassDesc *GetClassDesc(int id);
	int GetClassDescCount();

// MSVC++ stuff
	//{{AFX_VIRTUAL(CMaxExporterApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMaxExporterApp)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//----------------------------------------------------------------------------//
// The one and only DLL instance                                              //
//----------------------------------------------------------------------------//

extern CMaxExporterApp theDll;

//----------------------------------------------------------------------------//
// MSVC++ stuff                                                               //
//----------------------------------------------------------------------------//

//{{AFX_INSERT_LOCATION}}

#endif

//----------------------------------------------------------------------------//
