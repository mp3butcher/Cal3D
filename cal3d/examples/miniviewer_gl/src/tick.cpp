//----------------------------------------------------------------------------//
// tick.cpp                                                                   //
// Copyright (C) 2001 Bruno 'Beosil' Heidelberger                             //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include "tick.h"

//----------------------------------------------------------------------------//
// Constructors                                                               //
//----------------------------------------------------------------------------//

Tick::Tick()
{
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

Tick::~Tick()
{
}

//----------------------------------------------------------------------------//
// Get the current millisecond tick count                                     //
//----------------------------------------------------------------------------//

unsigned int Tick::getTick()
{
#ifdef _WIN32
	return GetTickCount();
#else

  struct timeval now;
  gettimeofday(&now, 0);
  return now.tv_sec * 1000 + now.tv_usec / 1000;
#endif
}

//----------------------------------------------------------------------------//
