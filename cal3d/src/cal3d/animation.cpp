//****************************************************************************//
// animation.cpp                                                              //
// Copyright (C) 2001 Bruno 'Beosil' Heidelberger                             //
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

#include "animation.h"
#include "error.h"
#include "coreanimation.h"

//****************************************************************************//
// Static member variables initialization                                     //
//****************************************************************************//

const unsigned int CalAnimation::TYPE_NONE = 0;
const unsigned int CalAnimation::TYPE_CYCLE = 1;
const unsigned int CalAnimation::TYPE_POSE = 2;
const unsigned int CalAnimation::TYPE_ACTION = 3;

const unsigned int CalAnimation::STATE_NONE = 0;
const unsigned int CalAnimation::STATE_SYNC = 1;
const unsigned int CalAnimation::STATE_ASYNC = 2;
const unsigned int CalAnimation::STATE_IN = 3;
const unsigned int CalAnimation::STATE_STEADY = 4;
const unsigned int CalAnimation::STATE_OUT = 5;

 /*****************************************************************************/
/** Constructs the animation instance.
  *
  * This function is the default constructor of the animation instance.
  *****************************************************************************/

CalAnimation::CalAnimation()
{
  m_pCoreAnimation = 0;
  m_type = TYPE_NONE;
  m_state = STATE_NONE;
  m_time = 0.0f;
  m_timeFactor = 1.0f;
  m_weight = 0.0f;
}

 /*****************************************************************************/
/** Destructs the animation instance.
  *
  * This function is the destructor of the animation instance.
  *****************************************************************************/

CalAnimation::~CalAnimation()
{
}

 /*****************************************************************************/
/** Destroys the animation instance.
  *
  * This function destroys all data stored in the animation instance and frees
  * all allocated memory.
  *****************************************************************************/

void CalAnimation::destroy()
{
  m_pCoreAnimation = 0;
}

 /*****************************************************************************/
/** Provides access to the core animation.
  *
  * This function returns the core animation on which this animation instance
  * is based on.
  *
  * @return One of the following values:
  *         \li a pointer to the core animation
  *         \li \b 0 if an error happend
  *****************************************************************************/

CalCoreAnimation *CalAnimation::getCoreAnimation()
{
  return m_pCoreAnimation;
}

 /*****************************************************************************/
/** Returns the state.
  *
  * This function returns the state of the animation instance.
  *
  * @return One of the following values:
  *         \li \b STATE_NONE
  *         \li \b STATE_SYNC
  *         \li \b STATE_ASYNC
  *         \li \b STATE_IN
  *         \li \b STATE_STEADY
  *         \li \b STATE_OUT
  *****************************************************************************/

unsigned int CalAnimation::getState()
{
  return m_state;
}

 /*****************************************************************************/
/** Returns the time.
  *
  * This function returns the time of the animation instance.
  *
  * @return The time in seconds.
  *****************************************************************************/

float CalAnimation::getTime()
{
  return m_time;
}

 /*****************************************************************************/
/** Returns the type.
  *
  * This function returns the type of the animation instance.
  *
  * @return One of the following values:
  *         \li \b TYPE_NONE
  *         \li \b TYPE_CYCLE
  *         \li \b TYPE_POSE
  *         \li \b TYPE_ACTION
  *****************************************************************************/

unsigned int CalAnimation::getType()
{
  return m_type;
}

 /*****************************************************************************/
/** Returns the weight.
  *
  * This function returns the weight of the animation instance.
  *
  * @return The weight.
  *****************************************************************************/

float CalAnimation::getWeight()
{
  return m_weight;
}

//****************************************************************************//
