//****************************************************************************//
// coremorphtrack.cpp                                                              //
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

#include "cal3d/coremorphtrack.h"
#include "cal3d/error.h"
#include "cal3d/coremorphkeyframe.h"

 /*****************************************************************************/
/** Constructs the core track instance.
  *
  * This function is the default constructor of the core track instance.
  *****************************************************************************/

CalCoreMorphTrack::CalCoreMorphTrack()
  : m_morphName("")
{
}

 /*****************************************************************************/
/** Destructs the core track instance.
  *
  * This function is the destructor of the core track instance.
  *****************************************************************************/

CalCoreMorphTrack::~CalCoreMorphTrack()
{
  //when CalCoreMorphTrack value objects die (from copying around etc), they might have keyframes still?
  //assert(m_keyframes.empty());
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

bool CalCoreMorphTrack::addCoreMorphKeyframe(CalCoreMorphKeyframe *pCoreMorphKeyframe)
{
  m_keyframes.push_back(*pCoreMorphKeyframe);
  m_keyframesToDelete.push_back(pCoreMorphKeyframe);
  int idx = m_keyframes.size() - 1;
  while (idx > 0 && m_keyframes[idx].getTime() < m_keyframes[idx - 1].getTime()) {
    std::swap(m_keyframes[idx], m_keyframes[idx - 1]);
    --idx;
  }

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

bool CalCoreMorphTrack::create()
{
  return true;
}

 /*****************************************************************************/
/** Destroys the core track instance.
  *
  * This function destroys all data stored in the core track instance and frees
  * all allocated memory.
  *****************************************************************************/

void CalCoreMorphTrack::destroy()
{
  // destroy all core keyframes
  for (unsigned int i = 0; i < m_keyframes.size(); ++i)
  {
    m_keyframes[i].destroy();
    //delete m_keyframes[i];
  }
  m_keyframes.clear();

  for (unsigned int i = 0; i < m_keyframesToDelete.size(); ++i)
  {
    delete m_keyframesToDelete[i];
  }
  m_keyframesToDelete.clear();

  m_morphName = "";
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

std::string CalCoreMorphTrack::getMorphName() const
{
  return m_morphName;
}

 /*****************************************************************************/
/** Returns a specified state.
  *
  * This function returns the state 
  * for the specified time and duration.
  *
  * @param time The time in seconds at which the state should be returned.
  * @param weight outparam
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalCoreMorphTrack::getState(float time, float & weight)
{
  std::vector<CalCoreMorphKeyframe>::iterator iteratorCoreMorphKeyframeBefore;
  std::vector<CalCoreMorphKeyframe>::iterator iteratorCoreMorphKeyframeAfter;

  // get the keyframe after the requested time
  iteratorCoreMorphKeyframeAfter = getUpperBound(time);

  // check if the time is after the last keyframe
  if(iteratorCoreMorphKeyframeAfter == m_keyframes.end())
  {
    // return the last keyframe state
    --iteratorCoreMorphKeyframeAfter;
    weight = (*iteratorCoreMorphKeyframeAfter).getWeight();

    return true;
  }

  // check if the time is before the first keyframe
  if(iteratorCoreMorphKeyframeAfter == m_keyframes.begin())
  {
    // return the first keyframe state
    weight = (*iteratorCoreMorphKeyframeAfter).getWeight();

    return true;
  }

  // get the keyframe before the requested one
  iteratorCoreMorphKeyframeBefore = iteratorCoreMorphKeyframeAfter;
  --iteratorCoreMorphKeyframeBefore;

  // get the two keyframe pointers
  CalCoreMorphKeyframe *pCoreMorphKeyframeBefore;
  pCoreMorphKeyframeBefore = &(*iteratorCoreMorphKeyframeBefore);
  CalCoreMorphKeyframe *pCoreMorphKeyframeAfter;
  pCoreMorphKeyframeAfter = &(*iteratorCoreMorphKeyframeAfter);

  // calculate the blending factor between the two keyframe states
  float blendFactor;
  blendFactor = (time - pCoreMorphKeyframeBefore->getTime()) / (pCoreMorphKeyframeAfter->getTime() - pCoreMorphKeyframeBefore->getTime());

  // blend between the two keyframes
  weight = pCoreMorphKeyframeBefore->getWeight();
  float otherWeight = pCoreMorphKeyframeAfter->getWeight();
  weight += blendFactor * (otherWeight-weight);

  return true;
}

std::vector<CalCoreMorphKeyframe>::iterator CalCoreMorphTrack::getUpperBound(float time)
{

  int lowerBound = 0;
  int upperBound = m_keyframes.size()-1;

  while(lowerBound<upperBound-1)
  {
      int middle = (lowerBound+upperBound)/2;

      if(time >= m_keyframes[middle].getTime())
      {
          lowerBound=middle;
      }
      else
      {
          upperBound=middle;
      }
  }

  return m_keyframes.begin() + upperBound;

}

 /*****************************************************************************/
/** Sets the ID of the core bone.
  *
  * This function sets the ID of the core bone to which the core track instance
  * is attached to.
  *
  * @param morphname The ID of the bone to which the core track instance should
  *                   be attached to.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

void CalCoreMorphTrack::setMorphName(std::string morphname)
{
  m_morphName = morphname;
}


int CalCoreMorphTrack::getCoreMorphKeyframeCount() const
{
  return m_keyframes.size();
}

void
CalCoreMorphTrack::reserve(int size)
{
  m_keyframes.reserve(size);
}




CalCoreMorphKeyframe* CalCoreMorphTrack::getCoreMorphKeyframe(int idx) 
{
  return &(m_keyframes[idx]);
}

const CalCoreMorphKeyframe* CalCoreMorphTrack::getCoreMorphKeyframe(int idx) const
{
   return &(m_keyframes[idx]);
}


 /*****************************************************************************/
/** Scale the core track.
  *
  * This function rescale all the data that are in the core track instance.
  *
  * @param factor A float with the scale factor
  *
  *****************************************************************************/


void CalCoreMorphTrack::scale(float factor)
{
    unsigned int keyframeId;
    for(keyframeId = 0; keyframeId < m_keyframes.size(); keyframeId++)
    {
        float weight = m_keyframes[keyframeId].getWeight();
        weight*=factor;
        m_keyframes[keyframeId].setWeight(weight);
    }

}

const std::vector<CalCoreMorphKeyframe> &
CalCoreMorphTrack::getVectorCoreMorphKeyframes() const
{
   return m_keyframes;
}

std::vector<CalCoreMorphKeyframe> &
CalCoreMorphTrack::getVectorCoreMorphKeyframes()
{
  return m_keyframes;
}



//****************************************************************************//
