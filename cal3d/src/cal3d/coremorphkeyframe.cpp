//****************************************************************************//
// coreMorphKeyframe.cpp                                                           //
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

#include "cal3d/coremorphkeyframe.h"

 /*****************************************************************************/
/** Constructs the core keyframe instance.
  *
  * This function is the default constructor of the core keyframe instance.
  *****************************************************************************/

CalCoreMorphKeyframe::CalCoreMorphKeyframe()
  : m_time(0.0f)
{
}

 /*****************************************************************************/
/** Destructs the core keyframe instance.
  *
  * This function is the destructor of the core keyframe instance.
  *****************************************************************************/

CalCoreMorphKeyframe::~CalCoreMorphKeyframe()
{
}

 /*****************************************************************************/
/** Creates the core keyframe instance.
  *
  * This function creates the core keyframe instance.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalCoreMorphKeyframe::create()
{
  return true;
}

 /*****************************************************************************/
/** Destroys the core keyframe instance.
  *
  * This function destroys all data stored in the core keyframe instance and
  * frees all allocated memory.
  *****************************************************************************/

void CalCoreMorphKeyframe::destroy()
{
}

 /*****************************************************************************/
/** Returns the time.
  *
  * This function returns the time of the core keyframe instance.
  *
  * @return The time in seconds.
  *****************************************************************************/

float CalCoreMorphKeyframe::getTime() const
{
  return m_time;
}

 /*****************************************************************************/
/** Returns the weight.
  *
  * This function returns the weight of the core keyframe instance.
  *
  * @return The weight as vector.
  *****************************************************************************/

float CalCoreMorphKeyframe::getWeight() const
{
  return m_weight;
}

 /*****************************************************************************/
/** Sets the time.
  *
  * This function sets the time of the core keyframe instance.
  *
  * @param rotation The time in seconds.
  *****************************************************************************/

void CalCoreMorphKeyframe::setTime(float time)
{
  m_time = time;
}

 /*****************************************************************************/
/** Sets the weight.
  *
  * This function sets the weight of the core keyframe instance.
  *
  * @param weight float
  *****************************************************************************/

void CalCoreMorphKeyframe::setWeight(float weight)
{
  m_weight = weight;
}

//****************************************************************************//
