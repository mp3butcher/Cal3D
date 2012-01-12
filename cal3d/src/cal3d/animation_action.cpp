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

CalAnimationAction::CalAnimationAction(CalCoreAnimation *pCoreAnimation)
  : CalAnimation(pCoreAnimation)
{
  setType(TYPE_ACTION);

  // For error checking, I initialize this here so you can't call execute on a manual action.
  m_manualOn = false;
  m_sequencingMode = SequencingModeNull;
  m_scale = 1.0;
  if(pCoreAnimation == 0)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
  }

}


 /*****************************************************************************/
/** Executes the animation action instance.
  *
  * This function executes the animation action instance.  You cannot execute
  * a manual action.
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
  *         \li \b false if was manual
  *****************************************************************************/

bool CalAnimationAction::execute(float delayIn, float delayOut, float weightTarget, bool autoLock)
{

  // You cannot execute a manual action.
  if( m_sequencingMode == SequencingModeManual ) {
    return false;
  }
  
  setState(STATE_IN);
  setWeight(0.0f);
  m_delayIn = delayIn;
  m_delayOut = delayOut;
  setTime(0.0f);
  m_weightTarget = weightTarget;
  m_autoLock = autoLock;
  m_sequencingMode = SequencingModeAutomatic;
  m_manualOn = false; // Irrelevant since not manual.
  m_rampValue = 1.0;
  m_compositionFunction = CompositionFunctionNull; // Initially NULL so we can recognize when it changes.
  return true;
}

 /*****************************************************************************/
/** Configures the action to be a manual action, and on.
  *
  * Configures the action to be a manual action, which the mixer does not
  * call update() on.  Once configuring an animation to be manual, you cannot
  * change it back to non-manual.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/
bool CalAnimationAction::setManual()
{
  setState(STATE_STEADY);
  setWeight(0.0f);
  m_delayIn = 0.0f;
  m_delayOut = 0.0f;
  setTime(0.0f);
  m_weightTarget = 10.0; // For debugging, an outrageous value.  This should be ignored.
  m_autoLock = true;
  m_sequencingMode = SequencingModeManual;
  m_manualOn = true;
  m_rampValue = 1.0;
  m_compositionFunction = CompositionFunctionNull; // Initially NULL so we can recognize when it changes.
  return true;
}


 /*****************************************************************************/
/** Tells you whether the animation action is on, i.e., should it apply to bones.
  *
  * Tells you whether the animation action is on, i.e., should it apply to bones.
  * All actions are on unless they are both manual and explicitly turned off.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/
bool
CalAnimationAction::on()
{
  return m_sequencingMode != SequencingModeManual || m_manualOn;
}



 /*****************************************************************************/
/** Tells you whether the animation action is configured to be manual.
  *
  * Tells you whether the animation action is configured to be manual.
  * Call setManual() to configure it to be manual.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/
bool
CalAnimationAction::manual()
{
  return m_sequencingMode == SequencingModeManual;
}


 /*****************************************************************************/
/** Sets the weight of the manual animation.
  *
  * Sets the weight of the manual animation.  Manual animations do not
  * blend toward a weight target, so you set the weight directly, not a
  * weight target.
  *
  * @return One of the following values:
  *         \li \b true if manual
  *         \li \b false if not manual
  *****************************************************************************/
bool CalAnimationAction::setManualAnimationActionWeight( float p )
{
  if( m_sequencingMode != SequencingModeManual ) return false;
  setWeight(p);
  return true;
}

 /*****************************************************************************/
/** Sets the composition function, which controls how animation blends with other simultaneous animations.
  *
  * If you set it to Replace, then when the animation is fully ramped on, all non-Replace
  * and lower priority Replace animations will have zero influence.  This
  * factor does not apply to cycling animations.  The priority of animations is,
  * firstly whether they are Replace or not, and secondly how recently the animations were
  * added, the most recently added animations having higher priority.
  *
  * @return One of the following values:
  *         \li \b true if not setting to CompositionFunctionNull
  *         \li \b false if setting to CompositionFunctionNull
  *****************************************************************************/
bool 
CalAnimationAction::setCompositionFunction( CompositionFunction p )
{
  if( p == CompositionFunctionNull ) return false;
  if( m_compositionFunction == p ) return true;
  m_compositionFunction = p;
  return true;
}

 /*****************************************************************************/
/** Gets the CompositionFunction of the animation.
  *
  * Gets the CompositionFunction of the animation.  See setRampValue().
  *
  * @return \li \b CompositionFunction value that was set with setCompositionFunction().
  *****************************************************************************/
CalAnimation::CompositionFunction 
CalAnimationAction::getCompositionFunction()
{
  return m_compositionFunction;
}


 /*****************************************************************************/
/** Sets the rampValue of the animation.
  *
  * The rampValue, from 0-1, scales the blend weight.  If the blending function
  * is Replace, the rampValue also scales the blend weight of non-Replace and
  * lower priority animations by 1 - rampValue.  Default should be 1.0.
  *
  * @return \li \b true always.
  *****************************************************************************/
bool
CalAnimationAction::setRampValue( float p )
{
  m_rampValue = p;
  return true;
}


 /*****************************************************************************/
/** Gets the RampValue of the animation.
  *
  * Gets the RampValue of the animation.  See setRampValue().
  *
  * @return \li \b RampValue value that was set with setRampValue().
  *****************************************************************************/
float
CalAnimationAction::getRampValue()
{
  return m_rampValue;
}



 /*****************************************************************************/
/** Gets the scale of the animation.
  *
  * Gets the scale of the animation.  See setScale().
  *
  * @return \li \b scale value that was set with setScale().
  *****************************************************************************/
float CalAnimationAction::getScale()
{
  return m_scale;
}

 /*****************************************************************************/
/** Sets the scale of the animation.
  *
  * Sets the scale of the animation.  Scale is different from weight.  Weight
  * is really relative weight.  All the weights are combined into a sum, and
  * each animation contributes according to the ratio of its weight to the sum.
  * In other words, the total influence of the weights is normalized to 1.0.
  * In contrast, scale factors apply to animation actions independently.  Scaling
  * one animation action up does not decrease the scale of other actions applying
  * to the same bones.  Whereas the weights are used to compose animations onto
  * a skeleton, scales are used to adjust the absolute, non-relative magnitude
  * of an animation.
  *
  * @return One of the following values:
  *         \li \b true if manual
  *         \li \b false if not manual
  *****************************************************************************/
bool CalAnimationAction::setScale( float p )
{
  m_scale = p;
  return true;
}


 /*****************************************************************************/
/** Sets the manual animation on or off.  If off, has no effect but retains
  *
  * Sets the manual animation on or off.  If off, has no effect but retains
  * state.
  *
  * @return One of the following values:
  *         \li \b true if manual
  *         \li \b false if not manual
  *****************************************************************************/
bool CalAnimationAction::setManualAnimationActionOn( bool p )
{
  if( m_sequencingMode != SequencingModeManual ) return false;
  m_manualOn = p;
  return true;
}


 /*****************************************************************************/
/** Updates the animation action instance.
  *
  * This function updates the animation action instance for a given amount of
  * time.  It has no effect on manual actions.
  *
  * @param deltaTime The elapsed time in seconds since the last update.
  *
  * @return One of the following values:
  *         \li \b true if the animation action instance is still active or is manual
  *         \li \b false if the execution of the animation action instance has
  *             ended
  *****************************************************************************/

bool CalAnimationAction::update(float deltaTime)
{

  // Mixer should not call update on manual actions.
  // Return true, not false, if manual, because we ignore manual, and our
  // return parameter indicates whether the action has ended.  A manual action
  // doesn't end.
  if( m_sequencingMode != SequencingModeAutomatic ) return true; 

  // update animation action time

  if(getState() != STATE_STOPPED)
  {
    setTime(getTime() + deltaTime * getTimeFactor());
  }

  // handle IN phase
  if(getState() == STATE_IN)
  {
    // check if we are still in the IN phase
    if(getTime() < m_delayIn)
    {
      setWeight(getTime() / m_delayIn * m_weightTarget);
      //m_weight = m_time / m_delayIn;
    }
    else
    {
      setState(STATE_STEADY);
      setWeight(m_weightTarget);
    }
  }

  // handle STEADY
  if(getState() == STATE_STEADY)
  {
    // check if we reached OUT phase
    if(!m_autoLock && getTime() >= getCoreAnimation()->getDuration() - m_delayOut)
    {
      setState(STATE_OUT);
    }
    // if the anim is supposed to stay locked on last keyframe, reset the time here.
    else if (m_autoLock && getTime() > getCoreAnimation()->getDuration())
    {
      setState(STATE_STOPPED);
      setTime(getCoreAnimation()->getDuration());
    }      
  }

  // handle OUT phase
  if(getState() == STATE_OUT)
  {
    // check if we are still in the OUT phase
    if(getTime() < getCoreAnimation()->getDuration())
    {
      setWeight((getCoreAnimation()->getDuration() - getTime()) / m_delayOut * m_weightTarget);
    }
    else
    {
      // we reached the end of the action animation
      setWeight(0.0f);
      return false;
    }
  }

  return true;

}

//****************************************************************************//
