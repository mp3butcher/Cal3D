//----------------------------------------------------------------------------//
// MilkExport.h                                                               //
// Copyright (C) 2002 Bruno 'Beosil' Heidelberger                             //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef MILK_EXPORT_H
#define MILK_EXPORT_H

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CMilkExport : public cMsPlugIn
{
// constructors/destructor
public:
	CMilkExport();
	virtual ~CMilkExport();

// interface functions	
public:
  int Execute(msModel *pModel);
  const char *GetTitle();
  int GetType();
};

#endif

//----------------------------------------------------------------------------//
