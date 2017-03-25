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

using namespace cal3d;
 /*****************************************************************************/
/** Constructs the core animatedMorph instance.
  *
  * This function is the default constructor of the core animatedMorph instance.
  *****************************************************************************/

CalCoreAnimatedMorph::CalCoreAnimatedMorph()
{
}

 /*****************************************************************************/
/** Destructs the core animatedMorph instance.
  *
  * This function is the destructor of the core animatedMorph instance.
  *****************************************************************************/

CalCoreAnimatedMorph::~CalCoreAnimatedMorph()
{
  // destroy all core tracks
  while(!m_listCoreTrack.empty())
  {
    CalCoreMorphTrack *pCoreTrack;
    pCoreTrack = &m_listCoreTrack.front();
    m_listCoreTrack.pop_front();
    delete pCoreTrack;
  }

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
  return true;
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
        //pCoreTrack->destroy();
delete pCoreTrack;
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

CalCoreMorphTrack *CalCoreAnimatedMorph::getCoreTrack(const unsigned int & name)
{
  // loop through all core track
  std::list<CalCoreMorphTrack>::iterator iteratorCoreTrack;
  for(iteratorCoreTrack = m_listCoreTrack.begin(); iteratorCoreTrack != m_listCoreTrack.end(); ++iteratorCoreTrack)
  {
    // get the core bone
    CalCoreMorphTrack *pCoreTrack;
    pCoreTrack = &(*iteratorCoreTrack);

    // check if we found the matching core bone
    if(pCoreTrack->getMorphID() == name) return pCoreTrack;
  }

  // no match found
  return 0;
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
