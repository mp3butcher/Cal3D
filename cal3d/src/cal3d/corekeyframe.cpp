//****************************************************************************//
// corekeyframe.cpp                                                           //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "cal3d/corekeyframe.h"

using namespace cal3d;

 /*****************************************************************************/
/** Constructs the core keyframe instance.
  *
  * This function is the default constructor of the core keyframe instance.
  *****************************************************************************/

CalCoreKeyframe::CalCoreKeyframe()
  : m_time(0.0f)
{
}

unsigned int
CalCoreKeyframe::size()
{
  return sizeof( CalCoreKeyframe );
}


 /*****************************************************************************/
/** Destructs the core keyframe instance.
  *
  * This function is the destructor of the core keyframe instance.
  *****************************************************************************/

CalCoreKeyframe::~CalCoreKeyframe()
{
}
