//----------------------------------------------------------------------------//
// cal3d_milk_exporter.h                                                      //
// Copyright (C) 2002 Bruno 'Beosil' Heidelberger                             //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef CAL3D_MILK_EXPORTER_H
#define CAL3D_MILK_EXPORTER_H

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CMilkExporterApp : public CWinApp
{
// member variables

// constructors/destructor
public:
	CMilkExporterApp();

// member functions
public:
  cMsPlugIn *GetPluginInstance();

// MSVC++ stuff
	//{{AFX_VIRTUAL(CMilkExporterApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMilkExporterApp)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//----------------------------------------------------------------------------//
// The one and only DLL instance                                              //
//----------------------------------------------------------------------------//

extern CMilkExporterApp theDll;

//----------------------------------------------------------------------------//
// MSVC++ stuff                                                               //
//----------------------------------------------------------------------------//

//{{AFX_INSERT_LOCATION}}

#endif

//----------------------------------------------------------------------------//
