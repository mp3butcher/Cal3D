//****************************************************************************//
// animation_action.cpp                                                       //
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

#include "cal3d/animation_action.h"
#include "cal3d/error.h"
#include "cal3d/coreanimation.h"

 /*****************************************************************************/
/** Constructs the animation action instance.
  *
  * This function is the default constructor of the animation action instance.
  *****************************************************************************/

CalAnimationAction::CalAnimationAction() : CalAnimation()
{
  m_type = TYPE_ACTION;
}

 /*****************************************************************************/
/** Destructs the animation action instance.
  *
  * This function is the destructor of the animation action instance.
  *****************************************************************************/

CalAnimationAction::~CalAnimationAction()
{
}

 /*****************************************************************************/
/** Creates the animation action instance.
  *
  * This function creates the animation action instance based on a core
  * animation.
  *
  * @param pCoreAnimation A pointer to the core animation on which this
  *                       animation action instance should be based on.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalAnimationAction::create(CalCoreAnimation *pCoreAnimation)
{
  if(pCoreAnimation == 0)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  m_pCoreAnimation = pCoreAnimation;

  return true;
}

 /*****************************************************************************/
/** Destroys the animation action instance.
  *
  * This function destroys all data stored in the animation action instance and
  * frees all allocated memory.
  *****************************************************************************/

void CalAnimationAction::destroy()
{
  m_pCoreAnimation = 0;
}

 /*****************************************************************************/
/** Executes the animation action instance.
  *
  * This function executes the animation action instance.
  *
  * @param delayIn The time in seconds until the animation action instance
  *                reaches the full weight from the beginning of its execution.
  * @param delayOut The time in seconds in which the animation action instance
  *                 reaches zero weight at the end of its execution.
  * @param weightTarget No doxygen comment for this. FIXME.
  * @param autoLock     This prevents the Action from being reset and removed
  *                     on the last keyframe if true.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalAnimationAction::execute(float delayIn, float delayOut, float weightTarget,bool autoLock)
{
  m_state = STATE_IN;
  m_weight = 0.0f;
  m_delayIn = delayIn;
  m_delayOut = delayOut;
  m_time = 0.0f;
  m_weightTarget = weightTarget;
  m_autoLock = autoLock;

  return true;
}

 /*****************************************************************************/
/** Updates the animation action instance.
  *
  * This function updates the animation action instance for a given amount of
  * time.
  *
  * @param deltaTime The elapsed time in seconds since the last update.
  *
  * @return One of the following values:
  *         \li \b true if the animation action instance is still active
  *         \li \b false if the execution of the animation action instance has
  *             ended
  *****************************************************************************/

bool CalAnimationAction::update(float deltaTime)
{
  // update animation action time

  if(m_state != STATE_STOPPED)
  {
	  m_time += deltaTime * m_timeFactor;
  }

  // handle IN phase
  if(m_state == STATE_IN)
  {
    // check if we are still in the IN phase
    if(m_time < m_delayIn)
    {
      m_weight = m_time / m_delayIn * m_weightTarget;
      //m_weight = m_time / m_delayIn;
    }
    else
    {
      m_state = STATE_STEADY;
      m_weight = m_weightTarget;
    }
  }

  // handle STEADY
  if(m_state == STATE_STEADY)
  {
    // check if we reached OUT phase
    if(!m_autoLock && m_time >= m_pCoreAnimation->getDuration() - m_delayOut)
    {
      m_state = STATE_OUT;
    }
    // if the anim is supposed to stay locked on last keyframe, reset the time here.
    else if (m_autoLock && m_time > m_pCoreAnimation->getDuration())
	{
	  m_state = STATE_STOPPED;
	  m_time = m_pCoreAnimation->getDuration();
	}      
  }

  // handle OUT phase
  if(m_state == STATE_OUT)
  {
    // check if we are still in the OUT phase
    if(m_time < m_pCoreAnimation->getDuration())
    {
      m_weight = (m_pCoreAnimation->getDuration() - m_time) / m_delayOut * m_weightTarget;
//      m_weight = (m_pCoreAnimation->getDuration() - m_time) / m_delayOut;
    }
    else
    {
      // we reached the end of the action animation
      m_weight = 0.0f;
      return false;
    }
  }

  return true;

}

//****************************************************************************//
