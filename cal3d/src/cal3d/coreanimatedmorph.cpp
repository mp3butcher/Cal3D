//****************************************************************************//
// coreanimatedMorph.cpp                                                          //
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

#include "cal3d/coreanimatedmorph.h"
#include "cal3d/coremorphtrack.h"

 /*****************************************************************************/
/** Constructs the core animatedMorph instance.
  *
  * This function is the default constructor of the core animatedMorph instance.
  *****************************************************************************/

static int MyNumCalCoreAnimatedMorphs = 0;
int CalCoreAnimatedMorph::getNumCoreAnimatedMorphs() { return MyNumCalCoreAnimatedMorphs; }

CalCoreAnimatedMorph::CalCoreAnimatedMorph()
{
  MyNumCalCoreAnimatedMorphs++;
}

 /*****************************************************************************/
/** Destructs the core animatedMorph instance.
  *
  * This function is the destructor of the core animatedMorph instance.
  *****************************************************************************/

CalCoreAnimatedMorph::~CalCoreAnimatedMorph()
{
  MyNumCalCoreAnimatedMorphs--;
  assert(m_listCoreTrack.empty());
}

 /*****************************************************************************/
/** Adds a core track.
  *
  * This function adds a core track to the core animatedMorph instance.
  *
  * @param pCoreTrack A pointer to the core track that should be added.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalCoreAnimatedMorph::addCoreTrack(CalCoreMorphTrack *pCoreTrack)
{
  m_listCoreTrack.push_back(*pCoreTrack);
  m_tracksToDelete.push_back(pCoreTrack);
  return true;
}

 /*****************************************************************************/
/** Creates the core animatedMorph instance.
  *
  * This function creates the core animatedMorph instance.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalCoreAnimatedMorph::create()
{
  return true;
}

 /*****************************************************************************/
/** Destroys the core animatedMorph instance.
  *
  * This function destroys all data stored in the core animatedMorph instance and
  * frees all allocated memory.
  *****************************************************************************/

void CalCoreAnimatedMorph::destroy()
{
  // destroy all core tracks
  while(!m_listCoreTrack.empty())
  {
    CalCoreMorphTrack *pCoreTrack;
    pCoreTrack = &m_listCoreTrack.front();
    pCoreTrack->destroy();
    m_listCoreTrack.pop_front();

    //delete pCoreTrack;
  }

  while(!m_tracksToDelete.empty()) {
    CalCoreMorphTrack *pCoreTrack;
    pCoreTrack = m_tracksToDelete.front();
    m_tracksToDelete.pop_front();
    delete pCoreTrack;
  }

}

void
CalCoreAnimatedMorph::removeZeroScaleTracks()
{
  std::list<CalCoreMorphTrack> & p = m_listCoreTrack;
  bool changed = true;
  while( changed ) {
    changed = false;
    std::list<CalCoreMorphTrack>::iterator iteratorCoreTrack;
    for(iteratorCoreTrack = p.begin(); iteratorCoreTrack != p.end(); ++iteratorCoreTrack) {
      // get the core bone
      CalCoreMorphTrack *pCoreTrack;
      pCoreTrack = &(*iteratorCoreTrack);
      std::vector<CalCoreMorphKeyframe> & morphNameList = pCoreTrack->getVectorCoreMorphKeyframes();

      bool nonZeroScaleTrack = false;
      for(size_t keyframeId = 0; keyframeId < morphNameList.size(); keyframeId++) {
        float weight = morphNameList[keyframeId].getWeight();
        if( weight != 0.0f ) {
          nonZeroScaleTrack = true;
          break;
        }
      }
      if( !nonZeroScaleTrack ) {
        pCoreTrack->destroy();
        p.erase( iteratorCoreTrack );
        changed = true;
        break;
      }
    }
  }
}



 /*****************************************************************************/
/** Provides access to a core track.
  *
  * This function returns the core track for a given bone ID.
  *
  * @param coreBoneId The core bone ID of the core track that should be
  *                   returned.
  *
  * @return One of the following values:
  *         \li a pointer to the core track
  *         \li \b 0 if an error happend
  *****************************************************************************/

CalCoreMorphTrack *CalCoreAnimatedMorph::getCoreTrack(std::string const & name)
{
  // loop through all core track
  std::list<CalCoreMorphTrack>::iterator iteratorCoreTrack;
  for(iteratorCoreTrack = m_listCoreTrack.begin(); iteratorCoreTrack != m_listCoreTrack.end(); ++iteratorCoreTrack)
  {
    // get the core bone
    CalCoreMorphTrack *pCoreTrack;
    pCoreTrack = &(*iteratorCoreTrack);

    // check if we found the matching core bone
    if(pCoreTrack->getMorphName() == name) return pCoreTrack;
  }

  // no match found
  return 0;
}

 /*****************************************************************************/
/** Returns the duration.
  *
  * This function returns the duration of the core animatedMorph instance.
  *
  * @return The duration in seconds.
  *****************************************************************************/

float CalCoreAnimatedMorph::getDuration() const
{
  return m_duration;
}

 /*****************************************************************************/
/** Returns the core track list.
  *
  * This function returns the list that contains all core tracks of the core
  * animatedMorph instance.
  *
  * @return A reference to the core track list.
  *****************************************************************************/

std::list<CalCoreMorphTrack>& CalCoreAnimatedMorph::getListCoreTrack()
{
  return m_listCoreTrack;
}

/*****************************************************************************/
/** Returns the core track list.
*
* This function returns the list that contains all core tracks of the core
* animatedMorph instance.
*
* @return A reference to the core track list.
*****************************************************************************/

const std::list<CalCoreMorphTrack>& CalCoreAnimatedMorph::getListCoreTrack() const
{
   return m_listCoreTrack;
}

 /*****************************************************************************/
/** Sets the duration.
  *
  * This function sets the duration of the core animatedMorph instance.
  *
  * @param duration The duration in seconds that should be set.
  *****************************************************************************/

void CalCoreAnimatedMorph::setDuration(float duration)
{
  m_duration = duration;
}

 /*****************************************************************************/
/** Scale the core animatedMorph.
  *
  * This function rescale all the skeleton data that are in the core animatedMorph instance
  *
  * @param factor A float with the scale factor
  *
  *****************************************************************************/

void CalCoreAnimatedMorph::scale(float factor)
{
  // loop through all core track
  std::list<CalCoreMorphTrack>::iterator iteratorCoreTrack;
  for(iteratorCoreTrack = m_listCoreTrack.begin(); iteratorCoreTrack != m_listCoreTrack.end(); ++iteratorCoreTrack)
  {
      (*iteratorCoreTrack).scale(factor);
  }
}



//****************************************************************************//
