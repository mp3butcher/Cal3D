//****************************************************************************//
// coretrack.cpp                                                              //
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

#include "coretrack.h"
#include "error.h"
#include "corekeyframe.h"

 /*****************************************************************************/
/** Constructs the core track instance.
  *
  * This function is the default constructor of the core track instance.
  *****************************************************************************/

CalCoreTrack::CalCoreTrack()
{
  m_coreBoneId = -1;
}

 /*****************************************************************************/
/** Destructs the core track instance.
  *
  * This function is the destructor of the core track instance.
  *****************************************************************************/

CalCoreTrack::~CalCoreTrack()
{
  assert(m_mapCoreKeyframe.empty());
}

 /*****************************************************************************/
/** Adds a core keyframe.
  *
  * This function adds a core keyframe to the core track instance.
  *
  * @param pCoreKeyframe A pointer to the core keyframe that should be added.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalCoreTrack::addCoreKeyframe(CalCoreKeyframe *pCoreKeyframe)
{
  m_mapCoreKeyframe.insert(std::make_pair(pCoreKeyframe->getTime(), pCoreKeyframe));

  return true;
}

 /*****************************************************************************/
/** Creates the core track instance.
  *
  * This function creates the core track instance.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalCoreTrack::create()
{
  return true;
}

 /*****************************************************************************/
/** Destroys the core track instance.
  *
  * This function destroys all data stored in the core track instance and frees
  * all allocated memory.
  *****************************************************************************/

void CalCoreTrack::destroy()
{
  // destroy all core keyframes
  std::map<float, CalCoreKeyframe *>::iterator iteratorCoreKeyframe;

  for(iteratorCoreKeyframe = m_mapCoreKeyframe.begin(); iteratorCoreKeyframe != m_mapCoreKeyframe.end(); ++iteratorCoreKeyframe)
  {
    CalCoreKeyframe *pCoreKeyframe;
    pCoreKeyframe = iteratorCoreKeyframe->second;

    pCoreKeyframe->destroy();
    delete pCoreKeyframe;
  }

  m_mapCoreKeyframe.clear();

  m_coreBoneId = -1;
}

 /*****************************************************************************/
/** Returns the ID of the core bone.
  *
  * This function returns the ID of the core bone to which the core track
  * instance is attached to.
  *
  * @return One of the following values:
  *         \li the \b ID of the core bone
  *         \li \b -1 if an error happend
  *****************************************************************************/

int CalCoreTrack::getCoreBoneId()
{
  return m_coreBoneId;
}

 /*****************************************************************************/
/** Returns the core keyframe map.
  *
  * This function returns the map that contains all core keyframes of the core
  * track instance.
  *
  * @return A reference to the core keyframe map.
  *****************************************************************************/

std::map<float, CalCoreKeyframe *>& CalCoreTrack::getMapCoreKeyframe()
{
  return m_mapCoreKeyframe;
}

 /*****************************************************************************/
/** Returns a specified state.
  *
  * This function returns the state (translation and rotation of the core bone)
  * for the specified time and duration.
  *
  * @param time The time in seconds at which the state should be returned.
  * @param duration The duration of the animation containing this core track
  *                 instance in seconds.
  * @param translation A reference to the translation reference that will be
  *                    filled with the specified state.
  * @param rotation A reference to the rotation reference that will be filled
  *                 with the specified state.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalCoreTrack::getState(float time, float duration, CalVector& translation, CalQuaternion& rotation)
{
  std::map<float, CalCoreKeyframe *>::iterator iteratorCoreKeyframeBefore;
  std::map<float, CalCoreKeyframe *>::iterator iteratorCoreKeyframeAfter;

  // get the one core keyframe before and the one after the requested time
  bool bWrap;
  iteratorCoreKeyframeAfter = m_mapCoreKeyframe.upper_bound(time);

  // check if we have a wrap-around
  if(iteratorCoreKeyframeAfter == m_mapCoreKeyframe.end())
  {
    iteratorCoreKeyframeBefore = iteratorCoreKeyframeAfter;
    --iteratorCoreKeyframeBefore;
    iteratorCoreKeyframeAfter = m_mapCoreKeyframe.begin();

    bWrap = true;
  }
  else
  {
    if(iteratorCoreKeyframeAfter == m_mapCoreKeyframe.begin())
    {
      iteratorCoreKeyframeBefore = m_mapCoreKeyframe.end();
    }
    else
    {
      iteratorCoreKeyframeBefore = iteratorCoreKeyframeAfter;
    }

    --iteratorCoreKeyframeBefore;

    bWrap = false;
  }

  // get the two keyframes
  CalCoreKeyframe *pCoreKeyframeBefore;
  pCoreKeyframeBefore = iteratorCoreKeyframeBefore->second;
  CalCoreKeyframe *pCoreKeyframeAfter;
  pCoreKeyframeAfter = iteratorCoreKeyframeAfter->second;

  // calculate the blending factor between the two keyframe states
  float blendFactor;
  if(bWrap)
  {
    blendFactor = (time - pCoreKeyframeBefore->getTime()) / (duration - pCoreKeyframeBefore->getTime());
  }
  else
  {
    blendFactor = (time - pCoreKeyframeBefore->getTime()) / (pCoreKeyframeAfter->getTime() - pCoreKeyframeBefore->getTime());
  }

  // blend between the two keyframes
  translation = pCoreKeyframeBefore->getTranslation();
  translation.blend(blendFactor, pCoreKeyframeAfter->getTranslation());

  rotation = pCoreKeyframeBefore->getRotation();
  rotation.blend(blendFactor, pCoreKeyframeAfter->getRotation());

  return true;
}

 /*****************************************************************************/
/** Sets the ID of the core bone.
  *
  * This function sets the ID of the core bone to which the core track instance
  * is attached to.
  *
  * @param coreBoneId The ID of the bone to which the core track instance should
  *                   be attached to.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalCoreTrack::setCoreBoneId(int coreBoneId)
{
  if(coreBoneId < 0)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  m_coreBoneId = coreBoneId;

  return true;
}

//****************************************************************************//
