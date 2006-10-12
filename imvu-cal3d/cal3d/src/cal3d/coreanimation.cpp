//****************************************************************************//
// coreanimation.cpp                                                          //
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

#include "cal3d/coreanimation.h" 
#include "cal3d/coretrack.h"
#include "cal3d/coreskeleton.h"
#include "cal3d/corebone.h"

static int MyNumCalCoreAnimations = 0;
int CalCoreAnimation::getNumCoreAnimations() { return MyNumCalCoreAnimations; }

 /*****************************************************************************/
/** Constructs the core animation instance.
  *
  * This function is the default constructor of the core animation instance.
  *****************************************************************************/

CalCoreAnimation::CalCoreAnimation()
{
  MyNumCalCoreAnimations++;
}

 /*****************************************************************************/
/** Destructs the core animation instance.
  *
  * This function is the destructor of the core animation instance.
  *****************************************************************************/

CalCoreAnimation::~CalCoreAnimation()
{
  MyNumCalCoreAnimations--;
  assert(m_listCoreTrack.empty());
}

 /*****************************************************************************/
/** Adds a core track.
  *
  * This function adds a core track to the core animation instance.
  *
  * @param pCoreTrack A pointer to the core track that should be added.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalCoreAnimation::addCoreTrack(CalCoreTrack *pCoreTrack)
{
  m_listCoreTrack.push_back(pCoreTrack);

  return true;
}

 /*****************************************************************************/
/** Creates the core animation instance.
  *
  * This function creates the core animation instance.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalCoreAnimation::create()
{
  return true;
}

 /*****************************************************************************/
/** Destroys the core animation instance.
  *
  * This function destroys all data stored in the core animation instance and
  * frees all allocated memory.
  *****************************************************************************/

void CalCoreAnimation::destroy()
{
  // destroy all core tracks
  while(!m_listCoreTrack.empty())
  {
    CalCoreTrack *pCoreTrack;
    pCoreTrack = m_listCoreTrack.front();
    m_listCoreTrack.pop_front();

    pCoreTrack->destroy();
    delete pCoreTrack;
  }
}


size_t
CalCoreAnimation::size()
{
  size_t r = sizeof( *this );
  std::list<CalCoreTrack *>::iterator iter1;
  for( iter1 = m_listCoreTrack.begin(); iter1 != m_listCoreTrack.end(); ++iter1 ) {
    r += (*iter1)->size() + sizeof(iter1); // Bi-directional list has two pointers.
  }
  return r;
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

CalCoreTrack *CalCoreAnimation::getCoreTrack(int coreBoneId)
{
  // loop through all core track
  std::list<CalCoreTrack *>::iterator iteratorCoreTrack;
  for(iteratorCoreTrack = m_listCoreTrack.begin(); iteratorCoreTrack != m_listCoreTrack.end(); ++iteratorCoreTrack)
  {
    // get the core bone
    CalCoreTrack *pCoreTrack;
    pCoreTrack = *iteratorCoreTrack;

    // check if we found the matching core bone
    if(pCoreTrack->getCoreBoneId() == coreBoneId) return pCoreTrack;
  }

  // no match found
  return 0;
}



void
CalCoreAnimation::fillInvalidTranslations( CalCoreSkeleton * skel )
{
  std::list<CalCoreTrack *>::iterator iteratorCoreTrack;
  for(iteratorCoreTrack = m_listCoreTrack.begin(); iteratorCoreTrack != m_listCoreTrack.end(); ++iteratorCoreTrack)
  {
    CalCoreTrack * tr = * iteratorCoreTrack;
    int boneId = tr->getCoreBoneId();
    assert( boneId != -1 );
    CalCoreBone * bo = skel->getCoreBone( boneId );
    if( bo ) {
      CalVector trans = bo->getTranslation();
      tr->fillInvalidTranslations( trans );
    }
  }
}


unsigned int 
CalCoreAnimation::numCoreTracks()
{
  return m_listCoreTrack.size();
}

CalCoreTrack * 
CalCoreAnimation::nthCoreTrack( unsigned int i )
{
  std::list<CalCoreTrack *>::iterator iteratorCoreTrack;
  for( iteratorCoreTrack = m_listCoreTrack.begin(); iteratorCoreTrack != m_listCoreTrack.end(); ++iteratorCoreTrack ) {
    if( i == 0 ) return * iteratorCoreTrack;
    i--;
  }
  return NULL;
}


 /*****************************************************************************/
/** Returns the duration.
  *
  * This function returns the duration of the core animation instance.
  *
  * @return The duration in seconds.
  *****************************************************************************/

float CalCoreAnimation::getDuration()
{
  return m_duration;
}

 /*****************************************************************************/
/** Returns the core track list.
  *
  * This function returns the list that contains all core tracks of the core
  * animation instance.
  *
  * @return A reference to the core track list.
  *****************************************************************************/

std::list<CalCoreTrack *>& CalCoreAnimation::getListCoreTrack()
{
  return m_listCoreTrack;
}

 /*****************************************************************************/
/** Sets the duration.
  *
  * This function sets the duration of the core animation instance.
  *
  * @param duration The duration in seconds that should be set.
  *****************************************************************************/

void CalCoreAnimation::setDuration(float duration)
{
  m_duration = duration;
}

 /*****************************************************************************/
/** Scale the core animation.
  *
  * This function rescale all the skeleton data that are in the core animation instance
  *
  * @param factor A float with the scale factor
  *
  *****************************************************************************/

void CalCoreAnimation::scale(float factor)
{
  // loop through all core track
  std::list<CalCoreTrack *>::iterator iteratorCoreTrack;
  for(iteratorCoreTrack = m_listCoreTrack.begin(); iteratorCoreTrack != m_listCoreTrack.end(); ++iteratorCoreTrack)
  {
	  (*iteratorCoreTrack)->scale(factor);
  }
}



//****************************************************************************//
