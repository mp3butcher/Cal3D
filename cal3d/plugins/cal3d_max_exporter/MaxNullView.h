//----------------------------------------------------------------------------//
// MaxNullView.h                                                              //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef MAX_NULL_VIEW_H
#define MAX_NULL_VIEW_H

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CMaxNullView : public View
{
// member variables
protected:

// constructors/destructor
public:
	CMaxNullView();
	virtual ~CMaxNullView();

// member functions
public:
	Point2 ViewToScreen(Point3 p);
};

#endif

//----------------------------------------------------------------------------//
