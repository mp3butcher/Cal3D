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

 /*****************************************************************************/
/** Constructs the core keyframe instance.
  *
  * This function is the default constructor of the core keyframe instance.
  *****************************************************************************/

CalCoreKeyframe::CalCoreKeyframe()
  : m_time(0.0f)
{
}

 /*****************************************************************************/
/** Destructs the core keyframe instance.
  *
  * This function is the destructor of the core keyframe instance.
  *****************************************************************************/

CalCoreKeyframe::~CalCoreKeyframe()
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

bool CalCoreKeyframe::create()
{
  return true;
}

 /*****************************************************************************/
/** Destroys the core keyframe instance.
  *
  * This function destroys all data stored in the core keyframe instance and
  * frees all allocated memory.
  *****************************************************************************/

void CalCoreKeyframe::destroy()
{
}

 /*****************************************************************************/
/** Returns the rotation.
  *
  * This function returns the rotation of the core keyframe instance.
  *
  * @return The rotation as quaternion.
  *****************************************************************************/

const CalQuaternion& CalCoreKeyframe::getRotation()
{
  return m_rotation;
}

 /*****************************************************************************/
/** Returns the translation.
  *
  * This function returns the translation of the core keyframe instance.
  *
  * @return The translation as vector.
  *****************************************************************************/

const CalVector& CalCoreKeyframe::getTranslation()
{
  return m_translation;
}

 /*****************************************************************************/
/** Sets the rotation.
  *
  * This function sets the rotation of the core keyframe instance.
  *
  * @param rotation The rotation as quaternion.
  *****************************************************************************/

void CalCoreKeyframe::setRotation(const CalQuaternion& rotation)
{
  m_rotation = rotation;
}

 /*****************************************************************************/
/** Sets the time.
  *
  * This function sets the time of the core keyframe instance.
  *
  * @param rotation The time in seconds.
  *****************************************************************************/

void CalCoreKeyframe::setTime(float time)
{
  m_time = time;
}

 /*****************************************************************************/
/** Sets the translation.
  *
  * This function sets the translation of the core keyframe instance.
  *
  * @param rotation The translation as vector.
  *****************************************************************************/

void CalCoreKeyframe::setTranslation(const CalVector& translation)
{
  m_translation = translation;
}

//****************************************************************************//
