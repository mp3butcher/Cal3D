//****************************************************************************//
// animation_cycle.cpp                                                        //
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

#include "cal3d/animation_cycle.h"
#include "cal3d/error.h"
#include "cal3d/coreanimation.h"

 /*****************************************************************************/
/** Constructs the animation cycle instance.
  *
  * This function is the default constructor of the animation cycle instance.
  *****************************************************************************/

CalAnimationCycle::CalAnimationCycle() : CalAnimation()
{
  m_type = TYPE_CYCLE;
  m_state = STATE_SYNC;
}

 /*****************************************************************************/
/** Destructs the animation cycle instance.
  *
  * This function is the destructor of the animation cycle instance.
  *****************************************************************************/

CalAnimationCycle::~CalAnimationCycle()
{
}

 /*****************************************************************************/
/** Interpolates the weight of the animation cycle instance.
  *
  * This function interpolates the weight of the animation cycle instance to a
  * new value in a given amount of time.
  *
  * @param weight The weight to interpolate the animation cycle instance to.
  * @param delay The time in seconds until the new weight should be reached.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalAnimationCycle::blend(float weight, float delay)
{
  m_targetWeight = weight;
  m_targetDelay = delay;

  return true;
}

 /*****************************************************************************/
/** Creates the animation cycle instance.
  *
  * This function creates the animation cycle instance based on a core
  * animation.
  *
  * @param pCoreAnimation A pointer to the core animation on which this
  *                       animation cycle instance should be based on.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalAnimationCycle::create(CalCoreAnimation *pCoreAnimation)
{
  if(pCoreAnimation == 0)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  m_pCoreAnimation = pCoreAnimation;

  // set default weights and delay
  m_weight = 0.0f;
  m_targetDelay = 0.0f;
  m_targetWeight = 0.0f;

  return true;
}

 /*****************************************************************************/
/** Destroys the animation cycle instance.
  *
  * This function destroys all data stored in the animation cycle instance and
  * frees all allocated memory.
  *****************************************************************************/

void CalAnimationCycle::destroy()
{
  m_pCoreAnimation = 0;
}

 /*****************************************************************************/
/** Puts the animation cycle instance into async state.
  *
  * This function puts the animation cycle instance into async state, which
  * means that it will end after the current running cycle.
  *
  * @param time The time in seconds at which the animation cycle instance was
  *             unlinked from the global mixer animation cycle.
  * @param duration The current duration of the global mixer animation cycle in
  *                 seconds at the time of the unlinking.
  *****************************************************************************/

void CalAnimationCycle::setAsync(float time, float duration)
{
  // check if thie animation cycle is already async
  if(m_state != STATE_ASYNC)
  {
    if(duration == 0.0f)
    {
      m_timeFactor = 1.0f;
      m_time = 0.0f;
    }
    else
    {
      m_timeFactor = m_pCoreAnimation->getDuration() / duration;
      m_time = time * m_timeFactor;
    }

    m_state = STATE_ASYNC;
  }
}

 /*****************************************************************************/
/** Updates the animation cycle instance.
  *
  * This function updates the animation cycle instance for a given amount of
  * time.
  *
  * @param deltaTime The elapsed time in seconds since the last update.
  *
  * @return One of the following values:
  *         \li \b true if the animation cycle instance is still active
  *         \li \b false if the execution of the animation cycle instance has
  *             ended
  *****************************************************************************/

bool CalAnimationCycle::update(float deltaTime)
{
  if(m_targetDelay <= fabs(deltaTime))
  {
    // we reached target delay, set to full weight
    m_weight = m_targetWeight;
    m_targetDelay = 0.0f;

    // check if we reached the cycles end
    if(m_weight == 0.0f)
    {
      return false;
    }
  }
  else
  {
    // not reached target delay yet, interpolate between current and target weight
    float factor;
    factor = deltaTime / m_targetDelay;
    m_weight = (1.0f - factor) * m_weight + factor * m_targetWeight;
    m_targetDelay -= deltaTime;
  }

  // update animation cycle time if it is in async state
  if(m_state == STATE_ASYNC)
  {
    m_time += deltaTime * m_timeFactor;
    if(m_time >= m_pCoreAnimation->getDuration())
    {
      m_time = (float) fmod(m_time, m_pCoreAnimation->getDuration());
    }
  }

  return true;
}

//****************************************************************************//
