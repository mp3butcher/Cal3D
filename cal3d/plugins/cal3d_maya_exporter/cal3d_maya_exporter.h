//----------------------------------------------------------------------------//
// cal3d_maya_exporter.h                                                       //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger, Thomas Cowell        //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef CAL3D_MAX_EXPORTER_H
#define CAL3D_MAX_EXPORTER_H

class CMayaExporterApp : public CWinApp
{
public:
    CMayaExporterApp() { }

public:
	
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

extern CMayaExporterApp theDll;

//----------------------------------------------------------------------------//
// MSVC++ stuff                                                               //
//----------------------------------------------------------------------------//

//{{AFX_INSERT_LOCATION}}

#endif



//----------------------------------------------------------------------------//

