#if defined(_MSC_VER) && _MSC_VER == 1200
#pragma warning(disable: 4786)
#endif

//****************************************************************************//
// mixer.cpp                                                                  //
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

#include "cal3d/error.h"
#include "cal3d/mixer.h"
#include "cal3d/coremodel.h"
#include "cal3d/corebone.h"
#include "cal3d/coreanimation.h"
#include "cal3d/coretrack.h"
#include "cal3d/corekeyframe.h"
#include "cal3d/model.h"
#include "cal3d/skeleton.h"
#include "cal3d/bone.h"
#include "cal3d/animation.h"
#include "cal3d/animation_action.h"
#include "cal3d/animation_cycle.h"

 /*****************************************************************************/
/** Constructs the mixer instance.
  *
  * This function is the default constructor of the mixer instance.
  *****************************************************************************/

CalMixer::CalMixer()
{
}

 /*****************************************************************************/
/** Destructs the mixer instance.
  *
  * This function is the destructor of the mixer instance.
  *****************************************************************************/

CalMixer::~CalMixer()
{
  assert(m_listAnimationAction.empty());
}

 /*****************************************************************************/
/** This function returns CalAnimationAction for given coreAnimationId.
  *
  * This function returns CalAnimationAction for given coreAnimationId.
  *
  * @param id The ID of the core animation.
  *
  * @return One of the following values:
  *         \li \b NULL if no action exists for given coreAnimationId.
  *         \li \b pointer to CalAnimationAction for the given coreAnimationId.
  *****************************************************************************/
CalAnimationAction *
CalMixer::animationActionFromCoreAnimationId( int coreAnimationId )
{
  std::list<CalAnimationAction *>::iterator iteratorAnimationAction;
  iteratorAnimationAction = m_listAnimationAction.begin();
  while(iteratorAnimationAction != m_listAnimationAction.end())
  {
    // update and check if animation action is still active
    CalAnimationAction * aa = *iteratorAnimationAction;
    CalCoreAnimation * ca = aa->getCoreAnimation();
    if( ca ) {
      CalCoreAnimation * ca2 = m_pModel->getCoreModel()->getCoreAnimation( coreAnimationId );
      if( ca == ca2 ) return aa;
    }
    ++iteratorAnimationAction;
  }
  return NULL;
}


 /*****************************************************************************/
/** Is action playing?
  *
  * Actions turn off automatically so you might need to know if one is playing.
  *
  * @param id The ID of the core animation.
  *
  * @return One of the following values:
  *         \li \b true if playing
  *         \li \b false if not
  *****************************************************************************/
bool
CalMixer::actionOn( int coreAnimationId )
{
  return animationActionFromCoreAnimationId( coreAnimationId ) ? true : false;
}


 /*****************************************************************************/
/** Add a manual animation instance.
  *
  * Add a manual animation instance for this core animation if one
  * does not already exist.  Only one instance can exist per core animation.
  * A manual animation instance can be on or off while still existing.
  * If it is off, it retains its state (time, amplitude), but
  * doesn't have any effect on the skeleton.
  *
  * @param id The ID of the core animation.
  *
  * @return One of the following values:
  *         \li \b true if didn't already exist
  *         \li \b false if already existed or allocation failed
  *****************************************************************************/
bool 
CalMixer::addManualAnimation( int coreAnimationId )
{ 
  if( animationActionFromCoreAnimationId( coreAnimationId ) ) {
    return false; // Already existed.
  }

  // Create a new action.  Test for error conditions.
  CalAnimationAction * aa = newAnimationAction( coreAnimationId );
  if( !aa ) return false;

  // If we got the action, then configure it as manual.
  return aa->setManual();
}

 /*****************************************************************************/
/** Remove a manual animation instance.
  *
  * Remove a manual animation instance for this core animation if one
  * already exists.
  *
  * @param id The ID of the core animation.
  *
  * @return One of the following values:
  *         \li \b true if already exist
  *         \li \b false if didn't exist
  *****************************************************************************/
bool 
CalMixer::removeManualAnimation( int coreAnimationId )
{
  CalAnimationAction * aa = animationActionFromCoreAnimationId( coreAnimationId );
  if( !aa ) return false;
  m_listAnimationAction.remove( aa );  
  aa->destroy();
  delete aa;
  return true;
}


 /*****************************************************************************/
/** Sets the manual animation on or off.  If off, has no effect but retains
  *
  * Sets the manual animation on or off.  If off, has no effect but retains
  * state.
  *
  * @return One of the following values:
  *         \li \b true if exists and manual
  *         \li \b false otherwise
  *****************************************************************************/
bool 
CalMixer::setManualAnimationOn( int coreAnimationId, bool p )
{
  CalAnimationAction * aa = animationActionFromCoreAnimationId( coreAnimationId );
  if( !aa ) return false;
  return setManualAnimationOn( aa, p );
}


bool 
CalMixer::setManualAnimationOn( CalAnimationAction * aa, bool p )
{
  if( !aa->manual() ) return false;
  return aa->setManualAnimationActionOn( p );
}


 /*****************************************************************************/
/** Sets all the manual animation attributes.
  *
  * Sets all the manual animation attributes.  Action must already be manual.
  *
  * @return One of the following values:
  *         \li \b true if exists and manual
  *         \li \b false otherwise
  *****************************************************************************/
bool
CalMixer::setManualAnimationAttributes( int coreAnimationId, CalMixerManualAnimationAttributes const & p )
{
  CalAnimationAction * aa = animationActionFromCoreAnimationId( coreAnimationId );
  if( !aa ) return false;
  if( !aa->manual() ) return false;
  setManualAnimationOn( aa, p.on_ );
  setManualAnimationTime( aa, p.time_ );
  setManualAnimationWeight( aa, p.weight_ );
  setManualAnimationScale( aa, p.scale_ );
  setManualAnimationRampValue( aa, p.rampValue_ );
  setManualAnimationCompositionFunction( aa, p.compositionFunction_ );
  return true;
}



 /*****************************************************************************/
/** Return duration of the core animation in seconds.
  *
  * Return duration of the core animation in seconds.  The duration goes from the
  * time of the first to the last frame.  Thus if frames were 1/30 of a second
  * long and there were 31 frames, the duration would be one second.  An instance
  * does not have to exist.
  *
  * Result = duration in seconds.
  *
  * @return One of the following values:
  *         \li \b true if exists
  *         \li \b false otherwise
  *****************************************************************************/
bool
CalMixer::animationDuration( int coreAnimationId, float * result )
{
  CalCoreAnimation * ca2 = m_pModel->getCoreModel()->getCoreAnimation( coreAnimationId );
  if( !ca2 ) return false;
  * result = ca2->getDuration();
  return true;
}




 /*****************************************************************************/
/** Sets the time of the manual animation.
  *
  * Sets the time of the manual animation.  The effect of setting the time beyond
  * either end of the animation's duration is to clamp the animation at its
  * first or last frame.  Manual animations do not turn off automatically, and
  * you can set the time arbitrarily.
  *
  * @return One of the following values:
  *         \li \b true if exists and manual
  *         \li \b false otherwise
  *****************************************************************************/
bool 
CalMixer::setManualAnimationTime( int coreAnimationId, float p )
{
  CalAnimationAction * aa = animationActionFromCoreAnimationId( coreAnimationId );
  if( !aa ) return false;
  return setManualAnimationTime( aa, p );
}

bool 
CalMixer::setManualAnimationTime( CalAnimationAction * aa, float p )
{
  aa->setTime( p );
  return true;
}


 /*****************************************************************************/
/** Sets the weight of the manual animation.
  *
  * Sets the weight of the manual animation.  Manual animations do not
  * blend toward a weight target, so you set the weight directly, not a
  * weight target.
  * It is an error to call this function for an animation that is not manual.
  *
  * @return One of the following values:
  *         \li \b true if manual
  *         \li \b false if not manual
  *****************************************************************************/
bool 
CalMixer::setManualAnimationWeight( int coreAnimationId, float p )
{
  CalAnimationAction * aa = animationActionFromCoreAnimationId( coreAnimationId );
  if( !aa ) return false;
  return setManualAnimationWeight( aa, p );
}

bool 
CalMixer::setManualAnimationWeight( CalAnimationAction * aa, float p )
{
  aa->setManualAnimationActionWeight( p );
  return true;
}


 /*****************************************************************************/
/** Sets the scale of the manual animation to 0-1.
  *
  * Sets the scale of the manual animation.  The scale is different from the weight.
  * The weights control the relative influence.  The scale controls amplitude
  * of the animation.  An animation with zero scale but high relative influence,
  * if applied, will drown out other animations that are composed with it, whereas
  * an animation with one scale but zero weight will have no effect.
  * It is an error to call this function for an animation that is not manual.
  *
  * @return One of the following values:
  *         \li \b true if manual
  *         \li \b false if not manual
  *****************************************************************************/
bool 
CalMixer::setManualAnimationScale( int coreAnimationId, float p )
{
  CalAnimationAction * aa = animationActionFromCoreAnimationId( coreAnimationId );
  if( !aa ) return false;
  return setManualAnimationScale( aa, p );
}

bool 
CalMixer::setManualAnimationScale( CalAnimationAction * aa, float p )
{
  aa->setScale( p );
  return true;
}


 /*****************************************************************************/
/** Sets the RampValue of the manual animation to 0-1.
  *
  * Sets the RampValue of the manual animation.
  * It is an error to call this function for an animation that is not manual.
  *
  * @return One of the following values:
  *         \li \b true if manual
  *         \li \b false if not manual
  *****************************************************************************/
bool 
CalMixer::setManualAnimationRampValue( int coreAnimationId, float p )
{
  CalAnimationAction * aa = animationActionFromCoreAnimationId( coreAnimationId );
  if( !aa ) return false;
  return setManualAnimationRampValue( aa, p );
}


bool 
CalMixer::setManualAnimationRampValue( CalAnimationAction * aa, float p )
{
  aa->setRampValue( p );
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
  *         \li \b false if setting to CompositionFunctionNull, or if action with id doesn't exist.
  *****************************************************************************/
bool 
CalMixer::setManualAnimationCompositionFunction( int coreAnimationId, 
                                                CalAnimation::CompositionFunction p )
{
  CalAnimationAction * aa = animationActionFromCoreAnimationId( coreAnimationId );
  if( !aa ) return false;
  return setManualAnimationCompositionFunction( aa, p );
}
  

bool 
CalMixer::setManualAnimationCompositionFunction( CalAnimationAction * aa, 
                                                CalAnimation::CompositionFunction p )
{
  if( p == CalAnimation::CompositionFunctionNull ) return false;
  CalAnimation::CompositionFunction oldValue = aa->getCompositionFunction();

  // If the value isn't changing, then exit here.  Otherwise I would remove it and reinsert
  // it at the front, which wouldn't preserve the property that the most recently inserted
  // animation is highest priority.
  if( oldValue == p ) return true;
  aa->setCompositionFunction( p );

  // Iterate through the list and remove this element.
  m_listAnimationAction.remove( aa );

  // Now insert it back in in the appropriate position.  Replace animations go in at the front.
  // Average animations go in after the replace animations.
  switch( p ) {
  case CalAnimation::CompositionFunctionReplace:
    {

      // Replace animations go on the front of the list.
      m_listAnimationAction.push_front( aa );
      break;
    }
  case CalAnimation::CompositionFunctionCrossFade:
    {

      // Average animations go after replace, but before Average.
      std::list<CalAnimationAction *>::iterator aait2;
      for( aait2 = m_listAnimationAction.begin(); aait2 != m_listAnimationAction.end(); aait2++ ) {
        CalAnimationAction * aa3 = * aait2;
        CalAnimation::CompositionFunction cf = aa3->getCompositionFunction();
        if( cf != CalAnimation::CompositionFunctionReplace ) {
          break;
        }
      }
      m_listAnimationAction.insert( aait2, aa );
      break;
    }
  case CalAnimation::CompositionFunctionAverage:
    {

      // Average animations go before the first Average animation.
      std::list<CalAnimationAction *>::iterator aait2;
      for( aait2 = m_listAnimationAction.begin(); aait2 != m_listAnimationAction.end(); aait2++ ) {
        CalAnimationAction * aa3 = * aait2;
        CalAnimation::CompositionFunction cf = aa3->getCompositionFunction();
        if( cf == CalAnimation::CompositionFunctionAverage ) { // Skip over replace and crossFade animations
          break;
        }
      }
      m_listAnimationAction.insert( aait2, aa );
      break;
    }
  default:
    {
      assert( !"Unexpected" );
      break;
    }
  }
  return true;
}



 /*****************************************************************************/
/** Stop the action.
  *
  * Turn off an action.
  *
  * @param id The ID of the core animation.
  *
  * @return One of the following values:
  *         \li \b true was playing (now stopped)
  *         \li \b false if already not playing
  *****************************************************************************/
bool
CalMixer::stopAction( int coreAnimationId )
{
  CalAnimationAction * aa = animationActionFromCoreAnimationId( coreAnimationId );
  if( !aa ) return false;
  m_listAnimationAction.remove( aa );  
  aa->destroy();
  delete aa;
  return true;
}



bool CalMixer::create(CalModel *pModel)
{

  // By default, all created animations do automatic sequencing as actions
  // or cycles.
  if(pModel == 0)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  m_pModel = pModel;

  CalCoreModel *pCoreModel;
  pCoreModel = m_pModel->getCoreModel();
  if(pCoreModel == 0)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  // set the animation time/duration values to default
  m_animationTime = 0.0f;
  m_animationDuration = 0.0f;
  m_timeFactor = 1.0f;
  m_numBoneAdjustments = 0;

  return true;
}

void CalMixer::destroy()
{
  // destroy all active animation actions
  while(!m_listAnimationAction.empty())
  {
    CalAnimationAction *pAnimationAction;
    pAnimationAction = m_listAnimationAction.front();
    m_listAnimationAction.pop_front();

    pAnimationAction->destroy();
    delete pAnimationAction;
  }
  m_pModel = 0;
}

 /*****************************************************************************/
/** Executes an animation action.
  *
  * This function executes an animation action.
  *
  * @param id The ID of the animation cycle that should be blended.
  * @param delayIn The time in seconds until the animation action reaches the
  *                full weight from the beginning of its execution.
  * @param delayOut The time in seconds in which the animation action reaches
  *                 zero weight at the end of its execution.
  * @param weightTarget No doxygen comment for this. FIXME.
  * @param autoLock     This prevents the Action from being reset and removed
  *                     on the last keyframe if true.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/
bool CalMixer::executeAction(int id, float delayIn, float delayOut, float weightTarget, bool autoLock)
{

  // Create a new action.  Test for error conditions.
  CalAnimationAction * aa = newAnimationAction(id);
  if( !aa ) return false;

  // If we got the action, then configure it for being update().
  return aa->execute(delayIn, delayOut, weightTarget, autoLock);
}



CalAnimationAction * CalMixer::newAnimationAction( int coreAnimationId )
{

  // get the core animation
  CalCoreAnimation *pCoreAnimation;
  pCoreAnimation = m_pModel->getCoreModel()->getCoreAnimation( coreAnimationId );
  if(pCoreAnimation == 0)
  {
    return NULL;
  }

  // allocate a new animation action instance
  CalAnimationAction *pAnimationAction;
  pAnimationAction = new CalAnimationAction();
  if(pAnimationAction == 0)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    return NULL;
  }

  // create the new animation instance
  if(!pAnimationAction->create(pCoreAnimation))
  {
    delete pAnimationAction;
    return NULL;
  }

  // insert new animation into the table
  m_listAnimationAction.push_front(pAnimationAction);
  return pAnimationAction;
}



 /*****************************************************************************/
/** Updates all active animations.
  *
  * This function updates all active non-manual animations of the mixer instance for a
  * given amount of time.  If you only use manual animations, you don't need
  * to call this function.
  *
  * @param deltaTime The elapsed time in seconds since the last update.
  *****************************************************************************/

void CalMixer::updateAnimation(float deltaTime)
{
  // update the current animation time
  if(m_animationDuration == 0.0f)
  {
    m_animationTime = 0.0f;
  }
  else
  {
    m_animationTime += deltaTime * m_timeFactor;
    if(m_animationTime >= m_animationDuration)
    {
      m_animationTime = (float) fmod(m_animationTime, m_animationDuration);
    }
	if (m_animationTime < 0)
      m_animationTime += m_animationDuration;

  }

  // update all active animation actions of this model
  std::list<CalAnimationAction *>::iterator iteratorAnimationAction;
  iteratorAnimationAction = m_listAnimationAction.begin();

  while(iteratorAnimationAction != m_listAnimationAction.end())
  {

    // Update and check if animation action is still active.
    // Manual actions will ignore the call to update and return true because
    // they don't end.
    if((*iteratorAnimationAction)->update(deltaTime))
    {
      ++iteratorAnimationAction;
    }
    else
    {
      // animation action has ended, destroy and remove it from the animation list
      (*iteratorAnimationAction)->destroy();
      delete (*iteratorAnimationAction);
      iteratorAnimationAction = m_listAnimationAction.erase(iteratorAnimationAction);
    }
  }
}


void
CalMixer::applyBoneAdjustments()
{
  CalSkeleton * pSkeleton = m_pModel->getSkeleton();
  std::vector<CalBone *>& vectorBone = pSkeleton->getVectorBone();
  unsigned int i;
  for( i = 0; i < m_numBoneAdjustments; i++ ) {
    CalMixerBoneAdjustmentAndBoneId * ba = & m_boneAdjustmentAndBoneIdArray[ i ];
    CalBone * bo = vectorBone[ ba->boneId_ ];
    CalCoreBone * cbo = bo->getCoreBone();
    if( ba->boneAdjustment_.flags_ & CalMixerBoneAdjustmentFlagMeshScale ) {
      bo->setMeshScaleAbsolute( ba->boneAdjustment_.meshScaleAbsolute_ );
    }
    if( ba->boneAdjustment_.flags_ & CalMixerBoneAdjustmentFlagPosRot ) {
      const CalVector & localPos = cbo->getTranslation();
      CalVector adjustedLocalPos = localPos;
      CalQuaternion adjustedLocalOri = ba->boneAdjustment_.localOri_;
      static float const scale = 1.0f;
      float rampValue = ba->boneAdjustment_.rampValue_;
      static bool const replace = true;
      static float const unrampedWeight = 1.0f;
      bo->blendState( unrampedWeight, 
        adjustedLocalPos,
        adjustedLocalOri, 
        scale, replace, rampValue );
    }
  }
}

bool
CalMixer::addBoneAdjustment( int boneId, CalMixerBoneAdjustment const & ba )
{
  if( m_numBoneAdjustments == CalMixerBoneAdjustmentsMax ) return false;
  m_boneAdjustmentAndBoneIdArray[ m_numBoneAdjustments ].boneAdjustment_ = ba;
  m_boneAdjustmentAndBoneIdArray[ m_numBoneAdjustments ].boneId_ = boneId;
  m_numBoneAdjustments++;
  return true;
}

void
CalMixer::removeAllBoneAdjustments()
{
  m_numBoneAdjustments = 0;
}

bool 
CalMixer::removeBoneAdjustment( int boneId )
{
  unsigned int i;
  for( i = 0; i < m_numBoneAdjustments; i++ ) {
    CalMixerBoneAdjustmentAndBoneId * ba = & m_boneAdjustmentAndBoneIdArray[ i ];
    if( ba->boneId_ == boneId ) break;
  }
  if( i == m_numBoneAdjustments ) return false; // Couldn't find it.
  i++;
  while( i < m_numBoneAdjustments ) {
    m_boneAdjustmentAndBoneIdArray[ i - 1 ] = m_boneAdjustmentAndBoneIdArray[ i ];
    i++;
  }
  m_numBoneAdjustments--;
  return true;
}


unsigned int 
CalMixer::numActiveOneShotAnimations()
{
  
  // get the skeleton we need to update
  CalSkeleton *pSkeleton;
  pSkeleton = m_pModel->getSkeleton();
  if(pSkeleton == 0) return 0;
  unsigned int count = 0;
  
  // loop through all animation actions
  std::list<CalAnimationAction *>::iterator itaa;
  for( itaa = m_listAnimationAction.begin(); itaa != m_listAnimationAction.end(); itaa++ ) {
    
    // get the core animation instance
    CalAnimationAction * aa = * itaa;
    
    // Manual animations can be on or off.  If they are off, they do not apply
    // to the bone.
    if( aa->on() ) {
      count++;
    }
  }
  return count;
}



void CalMixer::updateSkeleton()
{

  // get the skeleton we need to update
  CalSkeleton *pSkeleton;
  pSkeleton = m_pModel->getSkeleton();
  if(pSkeleton == 0) return;

  // clear the skeleton state
  pSkeleton->clearState();

  // get the bone vector of the skeleton
  std::vector<CalBone *>& vectorBone = pSkeleton->getVectorBone();

  // The bone adjustments are "replace" so they have to go first, giving them
  // highest priority and full influence.  Subsequent animations affecting the same bones, 
  // including subsequent replace animations, will have their incluence attenuated appropriately.
  applyBoneAdjustments();

  // loop through all animation actions
  std::list<CalAnimationAction *>::iterator itaa;
  for( itaa = m_listAnimationAction.begin(); itaa != m_listAnimationAction.end(); itaa++ ) {

    // get the core animation instance
    CalAnimationAction * aa = * itaa;
    
    // Manual animations can be on or off.  If they are off, they do not apply
    // to the bone.
    if( aa->on() ) {

      CalCoreAnimation * pCoreAnimation = aa->getCoreAnimation();
      
      // get the list of core tracks of above core animation
      std::list<CalCoreTrack *>& listCoreTrack = pCoreAnimation->getListCoreTrack();
      
      // loop through all core tracks of the core animation
      std::list<CalCoreTrack *>::iterator itct;
      for( itct = listCoreTrack.begin(); itct != listCoreTrack.end(); itct++ ) {
        
        // get the appropriate bone of the track
        CalCoreTrack * ct = * itct;
        if( ct->getCoreBoneId() >= int(vectorBone.size()) ) {
          continue;
        }
        CalBone * pBone = vectorBone[ ct->getCoreBoneId() ];
        
        // get the current translation and rotation
        CalVector translation;
        CalQuaternion rotation;
        ct->getState( aa->getTime(), translation, rotation);
        
        // Replace and CrossFade both blend with the replace function.
        bool replace = aa->getCompositionFunction() != CalAnimation::CompositionFunctionAverage;
        float scale = aa->getScale();
        pBone->blendState( aa->getWeight(), translation, rotation, scale, replace, aa->getRampValue() );
      }
    }
  }

  // === What does lockState() mean?  Why do we need it at all?  It seems only to allow us
  // to blend all the animation actions together into a temporary sum, and then
  // blend all the animation cycles together into a different sum, and then blend
  // the two sums together according to their relative weight sums.  I believe this is mathematically
  // equivalent of blending all the animation actions and cycles together into a single sum,
  // according to their relative weights.
  pSkeleton->lockState();

  // let the skeleton calculate its final state
  pSkeleton->calculateState();
}

/*****************************************************************************/
/** Returns the animation time.
  *
  * This function returns the animation time of the mixer instance.
  *
  * @return The animation time in seconds.
  *****************************************************************************/


float CalMixer::getAnimationTime()
{
	return m_animationTime;
}

/*****************************************************************************/
/** Returns the animation duration.
  *
  * This function returns the animation duration of the mixer instance.
  *
  * @return The animation duration in seconds.
  *****************************************************************************/


float CalMixer::getAnimationDuration()
{
	return m_animationDuration;
}


/*****************************************************************************/
/** Sets the animation time.
  *
  * This function sets the animation time of the mixer instance.
  *
  *****************************************************************************/


void CalMixer::setAnimationTime(float animationTime)
{
	m_animationTime=animationTime;
}

/*****************************************************************************/
/** Set the time factor.
  * 
  * This function sets the time factor of the mixer instance.
  * this time factor affect only sync animation
  *
  *****************************************************************************/


void CalMixer::setTimeFactor(float timeFactor)
{
    m_timeFactor = timeFactor;
}

/*****************************************************************************/
/** Get the time factor.
  * 
  * This function return the time factor of the mixer instance.
  *
  *****************************************************************************/

float CalMixer::getTimeFactor()
{
    return m_timeFactor;
}

//****************************************************************************//
