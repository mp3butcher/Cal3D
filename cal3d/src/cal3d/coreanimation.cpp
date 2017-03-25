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

#include "cal3d/coreanimation.h" 
#include "cal3d/coretrack.h"
#include "cal3d/coreskeleton.h"
#include "cal3d/corebone.h"

using namespace cal3d;

CalCoreAnimation::CalCoreAnimation()
{
}


CalCoreAnimation::~CalCoreAnimation()
{
	std::list<CalCoreTrack *>::iterator iteratorCoreTrack;
	for (iteratorCoreTrack = m_listCoreTrack.begin(); iteratorCoreTrack != m_listCoreTrack.end(); ++iteratorCoreTrack)
	{
		CalCoreTrack* pTrack = *iteratorCoreTrack;
                //pTrack->destroy();
		delete pTrack;
	}
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
  *         \li \b false if an error happened
  *****************************************************************************/
bool CalCoreAnimation::removeCoreTrack(CalCoreTrack *pCoreTrack){
	std::list<CalCoreTrack *>::iterator iteratorCoreTrack;
	for (iteratorCoreTrack = m_listCoreTrack.begin(); iteratorCoreTrack != m_listCoreTrack.end(); ++iteratorCoreTrack)
	{
		// get the core bone
		CalCoreTrack *pCoreTrack;
		pCoreTrack = *iteratorCoreTrack;

		// check if we found the matching core bone
		if (pCoreTrack == pCoreTrack) {
			m_listCoreTrack.erase(iteratorCoreTrack);
			return true;
		}
	}
	return false;
}
bool CalCoreAnimation::addCoreTrack(CalCoreTrack *pCoreTrack)
{
	m_listCoreTrack.push_back(pCoreTrack);

	return true;
}

/*****************************************************************************/
size_t CalCoreAnimation::size()
{
	size_t r = sizeof(*this);
	std::list<CalCoreTrack *>::iterator iter1;
	for (iter1 = m_listCoreTrack.begin(); iter1 != m_listCoreTrack.end(); ++iter1)
	{
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
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreTrack *CalCoreAnimation::getCoreTrack(int coreBoneId)
{
	// loop through all core track
	std::list<CalCoreTrack *>::iterator iteratorCoreTrack;
	for (iteratorCoreTrack = m_listCoreTrack.begin(); iteratorCoreTrack != m_listCoreTrack.end(); ++iteratorCoreTrack)
	{
		// get the core bone
		CalCoreTrack *pCoreTrack;
		pCoreTrack = *iteratorCoreTrack;

		// check if we found the matching core bone
		if (pCoreTrack->getCoreBoneId() == coreBoneId) return pCoreTrack;
	}

	// no match found
	return 0;
}


/*WTF?
void CalCoreAnimation::fillInvalidTranslations( CalCoreSkeleton * skel )
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


*/

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
	for (iteratorCoreTrack = m_listCoreTrack.begin(); iteratorCoreTrack != m_listCoreTrack.end(); ++iteratorCoreTrack)
	{
		(*iteratorCoreTrack)->scale(factor);
	}
}



/*****************************************************************************/
/**
  * Add a callback to the current list of callbacks for this CoreAnim.
  *
  * @param  callback     Ptr to a subclass of this abstract class implementing the callback function.
  * @param  min_interval Minimum interval (in seconds) between callbacks.  Specifying 0 means call every update().
  *
  *****************************************************************************/

void CalCoreAnimation::registerCallback(CalAnimationCallback *callback, float min_interval)
{
	CallbackRecord record;
	record.callback = callback;
	record.min_interval = min_interval;

	m_listCallbacks.push_back(record);
}

/*****************************************************************************/
/**
  * Remove a callback from the current list of callbacks for this Anim.
  * Callback objects not removed this way will be deleted in the dtor of the Anim.
  *
  * @param  callback     Ptr to a subclass of this abstract class implementing the callback function to remove.
  *
  *****************************************************************************/

void CalCoreAnimation::removeCallback(CalAnimationCallback *callback)
{
	for (std::vector<CallbackRecord>::iterator i = m_listCallbacks.begin(); i != m_listCallbacks.end(); i++)
	{
		if ((*i).callback == callback)
		{
			m_listCallbacks.erase(i);
			return;
		}
	}
}


/*****************************************************************************/
/** Returns the total number of core keyframes used for this animation.
  *
  * This function returns the total number of core keyframes used for this
  * animation instance (i.e.: the sum of all core keyframes of all core tracks).
  *
  * @return A reference to the core track list.
  *****************************************************************************/

unsigned int CalCoreAnimation::getTotalKeyframesCount() const
{
	unsigned int nbKeys = 0;
	for (std::list<CalCoreTrack*>::const_iterator it = m_listCoreTrack.begin(); it != m_listCoreTrack.end(); ++it)
	{
		CalCoreTrack *track = *it;
		nbKeys += track->getCoreKeyframeCount();
	}
	return nbKeys;
}
