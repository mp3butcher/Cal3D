//----------------------------------------------------------------------------//
// tick.h                                                                     //
// Copyright (C) 2001 Bruno 'Beosil' Heidelberger                             //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef TICK_H
#define TICK_H

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#include "global.h"

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class Tick
{
// member variables
protected:

// constructors/destructor
public:
  Tick();
  virtual ~Tick();

// member functions
public:
  static unsigned int getTick();
	static double getTime();
};

#endif

//----------------------------------------------------------------------------//
