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

#include "error.h"
#include "mixer.h"
#include "coremodel.h"
#include "corebone.h"
#include "coreanimation.h"
#include "coretrack.h"
#include "model.h"
#include "skeleton.h"
#include "bone.h"
#include "animation.h"
#include "animation_action.h"
#include "animation_cycle.h"

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
  assert(m_vectorAnimation.empty());
  assert(m_listAnimationCycle.empty());
  assert(m_listAnimationAction.empty());
}

 /*****************************************************************************/
/** Interpolates the weight of an animation cycle.
  *
  * This function interpolates the weight of an animation cycle to a new value
  * in a given amount of time. If the specified animation cycle is not active
  * yet, it is activated.
  *
  * @param id The ID of the animation cycle that should be blended.
  * @param weight The weight to interpolate the animation cycle to.
  * @param delay The time in seconds until the new weight should be reached.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalMixer::blendCycle(int id, float weight, float delay)
{
  if((id < 0) || (id >= (int)m_vectorAnimation.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  // get the animation for the given id
  CalAnimation *pAnimation;
  pAnimation = m_vectorAnimation[id];

  // create a new animation instance if it is not active yet
  if(pAnimation == 0)
  {
    // take the fast way out if we are trying to clear an inactive animation
    if(weight == 0.0f) return true;

    // get the core animation
    CalCoreAnimation *pCoreAnimation;
    pCoreAnimation = m_pModel->getCoreModel()->getCoreAnimation(id);
    if(pCoreAnimation == 0) return false;

    // allocate a new animation cycle instance
    CalAnimationCycle *pAnimationCycle;
    pAnimationCycle = new CalAnimationCycle();
    if(pAnimationCycle == 0)
    {
      CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
      return false;
    }

    // create the new animation instance
    if(!pAnimationCycle->create(pCoreAnimation))
    {
      delete pAnimationCycle;
      return false;
    }

    // insert new animation into the tables
    m_vectorAnimation[id] = pAnimationCycle;
    m_listAnimationCycle.push_front(pAnimationCycle);

    // blend the animation
    return pAnimationCycle->blend(weight, delay);
  }

  // check if this is really a animation cycle instance
  if(pAnimation->getType() != CalAnimation::TYPE_CYCLE)
  {
      CalError::setLastError(CalError::INVALID_ANIMATION_TYPE, __FILE__, __LINE__);
      return false;
  }

  // clear the animation cycle from the active vector if the target weight is zero
  if(weight == 0.0f)
  {
      m_vectorAnimation[id] = 0;
  }

  // cast it to an animation cycle
  CalAnimationCycle *pAnimationCycle;
  pAnimationCycle = (CalAnimationCycle *)pAnimation;

  // blend the animation cycle
  return pAnimationCycle->blend(weight, delay);
}

 /*****************************************************************************/
/** Fades an animation cycle out.
  *
  * This function fades an animation cycle out in a given amount of time.
  *
  * @param id The ID of the animation cycle that should be faded out.
  * @param delay The time in seconds until the the animation cycle is
  *              completely removed.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalMixer::clearCycle(int id, float delay)
{
  // check if the animation id is valid
  if((id < 0) || (id >= (int)m_vectorAnimation.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  // get the animation for the given id
  CalAnimation *pAnimation;
  pAnimation = m_vectorAnimation[id];

  // we can only clear cycles that are active
  if(pAnimation == 0) return true;

  // check if this is really a animation cycle instance
  if(pAnimation->getType() != CalAnimation::TYPE_CYCLE)
  {
      CalError::setLastError(CalError::INVALID_ANIMATION_TYPE, __FILE__, __LINE__);
      return false;
  }

  // clear the animation cycle from the active vector
  m_vectorAnimation[id] = 0;

  // cast it to an animation cycle
  CalAnimationCycle *pAnimationCycle;
  pAnimationCycle = (CalAnimationCycle *)pAnimation;

  // set animation cycle to async state
  pAnimationCycle->setAsync(m_animationTime, m_animationDuration);

  // blend the animation cycle
  return pAnimationCycle->blend(0.0f, delay);
}

 /*****************************************************************************/
/** Creates the mixer instance.
  *
  * This function creates the mixer instance.
  *
  * @param pModel A pointer to the model that should be managed with this mixer
  *               instance.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalMixer::create(CalModel *pModel)
{
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

  // build the animation table
  int coreAnimationCount;
  coreAnimationCount = m_pModel->getCoreModel()->getCoreAnimationCount();

  m_vectorAnimation.reserve(coreAnimationCount);
  m_vectorAnimation.insert(m_vectorAnimation.begin(), (std::vector<CalAnimation *>::size_type)coreAnimationCount, 0);

  // set the animation time/duration values to default
  m_animationTime = 0.0f;
  m_animationDuration = 0.0f;

  return true;
}

 /*****************************************************************************/
/** Destroys the mixer instance.
  *
  * This function destroys all data stored in the mixer instance and frees all
  * allocated memory.
  *****************************************************************************/

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

  // destroy all active animation cycles
  while(!m_listAnimationCycle.empty())
  {
    CalAnimationCycle *pAnimationCycle;
    pAnimationCycle = m_listAnimationCycle.front();
    m_listAnimationCycle.pop_front();

    pAnimationCycle->destroy();
    delete pAnimationCycle;
  }

  // clear the animation table
  m_vectorAnimation.clear();

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
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalMixer::executeAction(int id, float delayIn, float delayOut)
{
  if((id < 0) || (id >= (int)m_vectorAnimation.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  // get the core animation
  CalCoreAnimation *pCoreAnimation;
  pCoreAnimation = m_pModel->getCoreModel()->getCoreAnimation(id);
  if(pCoreAnimation == 0)
  {
    return false;
  }

  // allocate a new animation action instance
  CalAnimationAction *pAnimationAction;
  pAnimationAction = new CalAnimationAction();
  if(pAnimationAction == 0)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    return false;
  }

  // create the new animation instance
  if(!pAnimationAction->create(pCoreAnimation))
  {
    delete pAnimationAction;
    return false;
  }

  // insert new animation into the table
  m_listAnimationAction.push_front(pAnimationAction);

  // execute the animation
  return pAnimationAction->execute(delayIn, delayOut);
}

 /*****************************************************************************/
/** Updates all active animations.
  *
  * This function updates all active animations of the mixer instance for a
  * given amount of time.
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
    m_animationTime += deltaTime;
    if(m_animationTime >= m_animationDuration)
    {
      m_animationTime = (float) fmod(m_animationTime, m_animationDuration);
    }
  }

  // update all active animation actions of this model
  std::list<CalAnimationAction *>::iterator iteratorAnimationAction;
  iteratorAnimationAction = m_listAnimationAction.begin();

  while(iteratorAnimationAction != m_listAnimationAction.end())
  {
    // update and check if animation action is still active
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

  // todo: update all active animation poses of this model

  // update the weight of all active animation cycles of this model
  std::list<CalAnimationCycle *>::iterator iteratorAnimationCycle;
  iteratorAnimationCycle = m_listAnimationCycle.begin();

  float accumulatedWeight, accumulatedDuration;
  accumulatedWeight = 0.0f;
  accumulatedDuration = 0.0f;

  while(iteratorAnimationCycle != m_listAnimationCycle.end())
  {
    // update and check if animation cycle is still active
    if((*iteratorAnimationCycle)->update(deltaTime))
    {
      // check if it is in sync. if yes, update accumulated weight and duration
      if((*iteratorAnimationCycle)->getState() == CalAnimation::STATE_SYNC)
      {
        accumulatedWeight += (*iteratorAnimationCycle)->getWeight();
        accumulatedDuration += (*iteratorAnimationCycle)->getWeight() * (*iteratorAnimationCycle)->getCoreAnimation()->getDuration();
      }

      ++iteratorAnimationCycle;
    }
    else
    {
      // animation cycle has ended, destroy and remove it from the animation list
      (*iteratorAnimationCycle)->destroy();
      delete (*iteratorAnimationCycle);
      iteratorAnimationCycle = m_listAnimationCycle.erase(iteratorAnimationCycle);
    }
  }

  // adjust the global animation cycle duration
  if(accumulatedWeight > 0.0f)
  {
    m_animationDuration = accumulatedDuration / accumulatedWeight;
  }
  else
  {
    m_animationDuration = 0.0f;
  }
}

 /*****************************************************************************/
/** Updates the skeleton.
  *
  * This function updates the skeleton to match the current animation state.
  *****************************************************************************/

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

  // loop through all animation actions
  std::list<CalAnimationAction *>::iterator iteratorAnimationAction;
  for(iteratorAnimationAction = m_listAnimationAction.begin(); iteratorAnimationAction != m_listAnimationAction.end(); ++iteratorAnimationAction)
  {
    // get the core animation instance
    CalCoreAnimation *pCoreAnimation;
    pCoreAnimation = (*iteratorAnimationAction)->getCoreAnimation();

    // get the list of core tracks of above core animation
    std::list<CalCoreTrack *>& listCoreTrack = pCoreAnimation->getListCoreTrack();

    // loop through all core tracks of the core animation
    std::list<CalCoreTrack *>::iterator iteratorCoreTrack;
    for(iteratorCoreTrack = listCoreTrack.begin(); iteratorCoreTrack != listCoreTrack.end(); ++iteratorCoreTrack)
    {
      // get the appropriate bone of the track
      CalBone *pBone;
      pBone = vectorBone[(*iteratorCoreTrack)->getCoreBoneId()];

      // get the current translation and rotation
      CalVector translation;
      CalQuaternion rotation;
      (*iteratorCoreTrack)->getState((*iteratorAnimationAction)->getTime(), translation, rotation);

      // blend the bone state with the new state
      pBone->blendState((*iteratorAnimationAction)->getWeight(), translation, rotation);
    }
  }

  // lock the skeleton state
  pSkeleton->lockState();

  // loop through all animation cycles
  std::list<CalAnimationCycle *>::iterator iteratorAnimationCycle;
  for(iteratorAnimationCycle = m_listAnimationCycle.begin(); iteratorAnimationCycle != m_listAnimationCycle.end(); ++iteratorAnimationCycle)
  {
    // get the core animation instance
    CalCoreAnimation *pCoreAnimation;
    pCoreAnimation = (*iteratorAnimationCycle)->getCoreAnimation();

    // calculate adjusted time
    float animationTime;
    if((*iteratorAnimationCycle)->getState() == CalAnimation::STATE_SYNC)
    {
      if(m_animationDuration == 0.0f)
      {
        animationTime = 0.0f;
      }
      else
      {
        animationTime = m_animationTime * pCoreAnimation->getDuration() / m_animationDuration;
      }
    }
    else
    {
      animationTime = (*iteratorAnimationCycle)->getTime();
    }

    // get the list of core tracks of above core animation
    std::list<CalCoreTrack *>& listCoreTrack = pCoreAnimation->getListCoreTrack();

    // loop through all core tracks of the core animation
    std::list<CalCoreTrack *>::iterator iteratorCoreTrack;
    for(iteratorCoreTrack = listCoreTrack.begin(); iteratorCoreTrack != listCoreTrack.end(); ++iteratorCoreTrack)
    {
      // get the appropriate bone of the track
      CalBone *pBone;
      pBone = vectorBone[(*iteratorCoreTrack)->getCoreBoneId()];

      // get the current translation and rotation
      CalVector translation;
      CalQuaternion rotation;
      (*iteratorCoreTrack)->getState(animationTime, translation, rotation);

      // blend the bone state with the new state
      pBone->blendState((*iteratorAnimationCycle)->getWeight(), translation, rotation);
    }
  }

  // lock the skeleton state
  pSkeleton->lockState();

  // let the skeleton calculate its final state
  pSkeleton->calculateState();
}

//****************************************************************************//
