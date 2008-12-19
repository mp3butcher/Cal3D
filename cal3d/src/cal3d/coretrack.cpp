//****************************************************************************//
// coretrack.cpp                                                              //
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

#include "cal3d/coretrack.h"
#include "cal3d/corebone.h"
#include "cal3d/coreskeleton.h"
#include "cal3d/error.h"
#include "cal3d/corekeyframe.h"
#include "cal3d/loader.h"

int CalCoreTrack::m_translationRequiredCount = 0;
int CalCoreTrack::m_translationNotRequiredCount = 0;

 /*****************************************************************************/
/** Constructs the core track instance.
  *
  * This function is the default constructor of the core track instance.
  *****************************************************************************/

CalCoreTrack::CalCoreTrack()
  : m_coreBoneId(-1)
{
}


unsigned int
CalCoreTrack::size()
{
  unsigned int r = sizeof( CalCoreTrack );
  std::vector<CalCoreKeyframe *>::iterator iter1;
  for( iter1 = m_keyframes.begin(); iter1 != m_keyframes.end(); ++iter1 ) {
    r += (*iter1)->size();
  }
  return r;
}


 /*****************************************************************************/
/** Destructs the core track instance.
  *
  * This function is the destructor of the core track instance.
  *****************************************************************************/

CalCoreTrack::~CalCoreTrack()
{
  assert(m_keyframes.empty());
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
  *         \li \b false if an error happened
  *****************************************************************************/

bool CalCoreTrack::addCoreKeyframe(CalCoreKeyframe *pCoreKeyframe)
{
  m_keyframes.push_back(pCoreKeyframe);
  int idx = m_keyframes.size() - 1;
  while (idx > 0 && m_keyframes[idx]->getTime() < m_keyframes[idx - 1]->getTime()) {
    std::swap(m_keyframes[idx], m_keyframes[idx - 1]);
    --idx;
  }
  return true;
}


inline float
DistanceSquared( CalVector const & v1, CalVector const & v2 )
{
  float dx = ( v1.x - v2.x );
  float dy = ( v1.y - v2.y );
  float dz = ( v1.z - v2.z );
  float diff2 = dx * dx + dy * dy + dz * dz;
  return diff2;
}

inline float
Distance( CalVector const & p1, CalVector const & p2 )
{
  float dist = sqrtf( DistanceSquared( p1, p2 ) );
  return dist;
}

float
DistanceDegrees( CalQuaternion const & p1, CalQuaternion const & p2 )
{

  // To determine the angular distance between the oris, multiply one by the inverse
  // of the other, which should leave us with an identity ori if they are equal.  If
  // they are not equal, then the angular magnitude of the rotation in degrees is the
  // difference between the oris.
  CalQuaternion odist = p1;
  odist.invert();
  odist *= p2;
  float w = odist.w;
  if( w > 1 ) w = 1;
  if( w < -1 ) w = -1;
  float distrads = 2 * acos( w ); // Non-negative.
  float distdegrees = distrads * 180.0f / 3.141592654f; // Non-negative.
  if( distdegrees > 180.0 ) distdegrees -= 360.0;
  return fabsf( distdegrees );
}

bool
Near( CalVector const & p1, CalQuaternion const & q1, CalVector const & p2, CalQuaternion const & q2,
     double transTolerance,
     double rotTolerance )
{
  float distdegrees = DistanceDegrees( q1, q2 );
  if( distdegrees > rotTolerance ) return false;
  float dist = Distance( p1, p2 );
  if( dist > transTolerance ) return false;
  return true;
}


// Returns true if rounding took place and they were not exactly equal.
bool
CalCoreTrack::roundTranslation( CalCoreKeyframe const * prevp, CalCoreKeyframe * p, double transTolerance  )
{
  CalCoreKeyframe * prev = const_cast< CalCoreKeyframe * >( prevp );
  CalVector translation;
  assert( prev && p );

  // blend between the two keyframes
  translation = prev->getTranslation();
  CalVector const ppos = p->getTranslation();
  float dist = Distance( translation, ppos );

  // Identical returns false.
  if( dist == 0 ) return false;

  // Compare with tolerance.
  if( dist < transTolerance ) { // equal case handled above.
    p->setTranslation( translation );
    return true;
  } else {
    return false;
  }
}

bool
CalCoreTrack::keyframeEliminatable( CalCoreKeyframe * prev, 
                                   CalCoreKeyframe * p, 
                                   CalCoreKeyframe * next,
                                   double transTolerance,
                                   double rotTolerance )
{
  CalVector translation;
  CalQuaternion rotation;
  assert( prev && p && next );
  float time = p->getTime();
  float blendFactor;
  blendFactor = ( time - prev->getTime() ) / ( next->getTime() - prev->getTime() );

  // blend between the two keyframes
  translation = prev->getTranslation();
  translation.blend( blendFactor, next->getTranslation() );
  rotation = prev->getRotation();
  rotation.blend( blendFactor, next->getRotation() );
  CalVector const ppos = p->getTranslation();
  CalQuaternion const pori = p->getRotation();
  return Near( translation, rotation, ppos, pori, transTolerance, rotTolerance );
}



struct KeyLink {
  bool eliminated_;
  CalCoreKeyframe * keyframe_;
  KeyLink * next_;
};


unsigned int
KeyFrameSequenceLength( KeyLink * p, double transTolerance, double rotTolerance )
{
  CalVector translation = p->keyframe_->getTranslation();
  CalQuaternion rotation = p->keyframe_->getRotation();
  p = p->next_;
  unsigned int len = 1;
  while( p ) {
    CalVector const ppos = p->keyframe_->getTranslation();
    CalQuaternion const pori = p->keyframe_->getRotation();
    if( Near( translation, rotation, ppos, pori, transTolerance, rotTolerance ) ) {
      len++;
      p = p->next_;
    } else {
      break;
    }
  }
  return len;
}


void
CalCoreTrack::compress( double translationTolerance, double rotationToleranceDegrees, CalCoreSkeleton * skelOrNull )
{
  unsigned int numFrames = m_keyframes.size();
  if( !numFrames ) return;
  unsigned int numFramesEliminated = 0;

  // I want to iterate through the vector as a list, and remove elements easily.
  static unsigned int arrayLen = 0;
  static KeyLink * keyLinkArray = NULL;
  if( arrayLen < numFrames ) {
    if( keyLinkArray ) {
      delete [] keyLinkArray;
    }
    keyLinkArray = new( KeyLink [ numFrames ] );
    arrayLen = numFrames;
  }
  unsigned int i;
  for( i = 0; i < numFrames; i++ ) {
    KeyLink * kl = & keyLinkArray[ i ];
    kl->keyframe_ = m_keyframes[ i ];
    kl->next_ = ( i == numFrames - 1 ) ? NULL : & keyLinkArray[ i + 1 ];
    kl->eliminated_ = false;
  }

  // Iterate until quiescence.
  bool removedFrame = true;
  while( removedFrame ) {
    removedFrame = false;

    // Loop through the frames, starting with the second, ending with the second to last.
    // If the frame is approximately the same as the interpolated frame between its prev
    // and next frame, then eliminate it.
    KeyLink * prev = & keyLinkArray[ 0 ];
    while( true ) {
      KeyLink * p = prev->next_;
      if( !p || !p->next_ ) break;
      KeyLink * next = p->next_;
      if( keyframeEliminatable( prev->keyframe_, 
		  p->keyframe_, 
		  next->keyframe_,
		  translationTolerance, rotationToleranceDegrees) ) 
	  {
        p->eliminated_ = true;

        // Splice this eliminated keyframe out of the list.
        prev->next_ = next;

        // Pass over next frame, making it prev instead of making it p, since I don't
        // want to consider eliminating two key frames in a row because that can 
        // reduce a slow moving curve to a line no matter the arc
        // of the curve.
        prev = next;
        removedFrame = true;
        numFramesEliminated++;
      } else {
        prev = p;
      }
    }
  }

  // Now go through and round off translation values to the prev value if they are within
  // tolerance.  The reason we do this is so lossless compression algorithms will eliminate
  // redundancy.  There seems to be numerical jitter in the translation when there are rotations,
  // which will make those translation values not compress well.
  int numRounded = 0;
  KeyLink * prev = & keyLinkArray[ 0 ];
  KeyLink * p = prev->next_;
  while( p ) {
    bool didRound = roundTranslation( prev->keyframe_, p->keyframe_, translationTolerance );
    if( didRound ) {
      numRounded++;
    }
    prev = p;
    p = p->next_;
  }
  CalLoader::addAnimationCompressionStatistic( numFrames, numFramesEliminated, numRounded );

  // Rebuild the vector, freeing any of the eliminated keyframes.
  unsigned int numKept = 0;
  for( i = 0; i < numFrames; i++ ) {
    KeyLink * kl = & keyLinkArray[ i ];
    if( !kl->eliminated_ ) {
      m_keyframes[ numKept ] = kl->keyframe_;
      numKept++;
    }
  }
  assert( numKept == numFrames - numFramesEliminated );

  // Delete the eliminated keyframes.
  for( i = 0; i < numFrames; i++ ) {
    KeyLink * kl = & keyLinkArray[ i ];
    if( kl->eliminated_ ) {
      kl->keyframe_->destroy();
    }
  }
  m_keyframes.resize( numKept );

  // Update the flag saying whether the translation, which I have loaded, is actually required.
  // If translation is not required, I can't do any better than that so I leave it alone.
  if( skelOrNull && m_translationRequired ) {
    translationCompressibility( 
      & m_translationRequired, 
      & m_translationIsDynamic, 
      & m_highRangeRequired,
      translationTolerance, CalLoader::keyframePosRangeSmall, skelOrNull );
    if( m_translationRequired ) {
      m_translationRequiredCount++;
    } else {
      m_translationNotRequiredCount++;
    }
  }
}




void
CalCoreTrack::collapseSequences( double translationTolerance, double rotationToleranceDegrees )
{
  unsigned int numFrames = m_keyframes.size();
  if( !numFrames ) return;
  unsigned int numFramesEliminated = 0;

  // I want to iterate through the vector as a list, and remove elements easily.
  static unsigned int arrayLen = 0;
  static KeyLink * keyLinkArray = NULL;
  if( arrayLen < numFrames ) {
    if( keyLinkArray ) {
      delete [] keyLinkArray;
    }
    keyLinkArray = new( KeyLink [ numFrames ] );
    arrayLen = numFrames;
  }
  unsigned int i;
  for( i = 0; i < numFrames; i++ ) {
    KeyLink * kl = & keyLinkArray[ i ];
    kl->keyframe_ = m_keyframes[ i ];
    kl->next_ = ( i == numFrames - 1 ) ? NULL : & keyLinkArray[ i + 1 ];
    kl->eliminated_ = false;
  }

  // This loop does not preserve the next_ pointers.
  // I must not eliminate the first frame in the track or the last, since their time markers
  // bracket the duration of the animation.
  KeyLink * p = & keyLinkArray[ 0 ];
  KeyLink * pstart = p;
  while( p ) {
    unsigned int lengthOfSequence = KeyFrameSequenceLength( p, translationTolerance, rotationToleranceDegrees );
    assert( lengthOfSequence >= 1 );
    if( lengthOfSequence == 1 ) {
      p = p->next_;
    } else {

      // Do not eliminate the first frame of the sequence.  Skip over it instead.
      i = 0;
      if( p == pstart ) {
        i++;
        p = p->next_;
      }

      // Eliminate 0 or more frames before the middle (0 only in the case we skipped the first frame).
      for( ; i < lengthOfSequence / 2; i++ ) {
        p->eliminated_ = true;
        numFramesEliminated++;
        p = p->next_;
      }
      
      // Keep the middle.
      p = p->next_;
      i++;
      
      // Eliminate the rest of the frames in the sequence, but do not eliminate
      // the last frame of the sequence.
      for( ; i < lengthOfSequence && p->next_ ; i++ ) {
        p->eliminated_ = true;
        numFramesEliminated++;
        p = p->next_;
      }
    }
  }

  // Rebuild the vector, freeing any of the eliminated keyframes.
  unsigned int numKept = 0;
  for( i = 0; i < numFrames; i++ ) {
    KeyLink * kl = & keyLinkArray[ i ];
    if( !kl->eliminated_ ) {
      m_keyframes[ numKept ] = kl->keyframe_;
      numKept++;
    }
  }
  assert( numKept == numFrames - numFramesEliminated );

  // Delete the eliminated keyframes.
  for( i = 0; i < numFrames; i++ ) {
    KeyLink * kl = & keyLinkArray[ i ];
    if( kl->eliminated_ ) {
      kl->keyframe_->destroy();
    }
  }
  m_keyframes.resize( numKept );
}


void
CalCoreTrack::fillInvalidTranslations( CalVector const & trans )
{
  unsigned int numFrames = m_keyframes.size();
  for( unsigned int i = 0; i < numFrames; i++ ) {
    CalCoreKeyframe * keyframe = m_keyframes[ i ];
    const CalVector & kftrans = keyframe->getTranslation();
    if( TranslationInvalid( kftrans ) ) {
      keyframe->setTranslation( trans );
    }
  }
}



void
CalCoreTrack::translationCompressibility( bool * transRequiredResult, bool * transDynamicResult, bool * highRangeRequiredResult,
                                  float threshold, float highRangeThreshold, CalCoreSkeleton * skel )
{
  * transRequiredResult = false;
  * transDynamicResult = false;
  * highRangeRequiredResult = false;
  unsigned int numFrames = m_keyframes.size();
  CalCoreBone * cb = skel->getCoreBone( m_coreBoneId );
  const CalVector & cbtrans = cb->getTranslation();
  CalVector trans0;
  float t2 = threshold * threshold;
  unsigned int i;
  for( i = 0; i < numFrames; i++ ) {
    CalCoreKeyframe * keyframe = m_keyframes[ i ];
    const CalVector & kftrans = keyframe->getTranslation();
    if( fabsf( kftrans.x ) >= highRangeThreshold
      ||  fabsf( kftrans.y ) >= highRangeThreshold
      ||  fabsf( kftrans.z ) >= highRangeThreshold ) {
      * highRangeRequiredResult = true;
    }
    if( i == 0 ) {
      trans0 = keyframe->getTranslation();
    } else {
      float d2 = DistanceSquared( trans0, kftrans );
      if( d2 > t2 ) {
        * transDynamicResult = true;
      }
    }
    float d2 = DistanceSquared( cbtrans, kftrans );
    if( d2 > t2 ) {
      * transRequiredResult = true;
    }
  }
}



 /*****************************************************************************/
/** Creates the core track instance.
  *
  * This function creates the core track instance.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happened
  *****************************************************************************/

void CalCoreTrack::create()
{
  m_translationRequired = true;
  m_highRangeRequired = true;
  m_translationIsDynamic = true;
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
	for (unsigned int i = 0; i < m_keyframes.size(); ++i)
	{
		m_keyframes[i]->destroy();
		delete m_keyframes[i];
	}
  m_keyframes.clear();

  m_coreBoneId = -1;
}

 /*****************************************************************************/
/** Returns a specified state.
  *
  * This function returns the state (translation and rotation of the core bone)
  * for the specified time and duration.
  *
  * @param time The time in seconds at which the state should be returned.
  * @param translation A reference to the translation reference that will be
  *                    filled with the specified state.
  * @param rotation A reference to the rotation reference that will be filled
  *                 with the specified state.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happened
  *****************************************************************************/

bool CalCoreTrack::getState(float time, CalVector& translation, CalQuaternion& rotation) const
{
  std::vector<CalCoreKeyframe*>::const_iterator iteratorCoreKeyframeBefore;
  std::vector<CalCoreKeyframe*>::const_iterator iteratorCoreKeyframeAfter;

  // get the keyframe after the requested time
  iteratorCoreKeyframeAfter = getUpperBound(time);

  // check if the time is after the last keyframe
  if(iteratorCoreKeyframeAfter == m_keyframes.end())
  {
    // return the last keyframe state
    --iteratorCoreKeyframeAfter;
    rotation = (*iteratorCoreKeyframeAfter)->getRotation();
    translation = (*iteratorCoreKeyframeAfter)->getTranslation();

    return true;
  }

  // check if the time is before the first keyframe
  if(iteratorCoreKeyframeAfter == m_keyframes.begin())
  {
    // return the first keyframe state
    rotation = (*iteratorCoreKeyframeAfter)->getRotation();
    translation = (*iteratorCoreKeyframeAfter)->getTranslation();

    return true;
  }

  // get the keyframe before the requested one
  iteratorCoreKeyframeBefore = iteratorCoreKeyframeAfter;
  --iteratorCoreKeyframeBefore;

  // get the two keyframe pointers
  CalCoreKeyframe *pCoreKeyframeBefore;
  pCoreKeyframeBefore = *iteratorCoreKeyframeBefore;
  CalCoreKeyframe *pCoreKeyframeAfter;
  pCoreKeyframeAfter = *iteratorCoreKeyframeAfter;

  // calculate the blending factor between the two keyframe states
  float blendFactor;
  blendFactor = (time - pCoreKeyframeBefore->getTime()) / (pCoreKeyframeAfter->getTime() - pCoreKeyframeBefore->getTime());

  // blend between the two keyframes
  translation = pCoreKeyframeBefore->getTranslation();
  translation.blend(blendFactor, pCoreKeyframeAfter->getTranslation());

  rotation = pCoreKeyframeBefore->getRotation();
  rotation.blend(blendFactor, pCoreKeyframeAfter->getRotation());

  return true;
}

std::vector<CalCoreKeyframe *>::const_iterator CalCoreTrack::getUpperBound(float time) const
{

  int lowerBound = 0;
  int upperBound = m_keyframes.size()-1;
	//static int aa = 0;

	//upperBound += aa;
	//upperBound %= m_keyframes.size();
	//aa++;
	//time = m_keyframes[upperBound]->getTime();

  while(lowerBound<upperBound-1)
  {
	  int middle = (lowerBound+upperBound)/2;

	  if(time >= m_keyframes[middle]->getTime())
	  {
		  lowerBound=middle;
	  }
	  else
	  {
		  upperBound=middle;
	  }
		//break;
  }

  return m_keyframes.begin() + upperBound;

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
  *         \li \b false if an error happened
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


int CalCoreTrack::getCoreKeyframeCount() const
{
  return m_keyframes.size();
}


CalCoreKeyframe *CalCoreTrack::getCoreKeyframe(int idx)
{
  return m_keyframes[idx];
}


const CalCoreKeyframe *CalCoreTrack::getCoreKeyframe(int idx) const
{
  return m_keyframes[idx];
}

 /*****************************************************************************/
/** Scale the core track.
  *
  * This function rescale all the data that are in the core track instance.
  *
  * @param factor A float with the scale factor
  *
  *****************************************************************************/


void CalCoreTrack::scale(float factor)
{
  for(size_t keyframeId = 0; keyframeId < m_keyframes.size(); keyframeId++)
  {
    CalVector translation = m_keyframes[keyframeId]->getTranslation();
    translation*=factor;
    m_keyframes[keyframeId]->setTranslation(translation);
  }

}

//****************************************************************************//
