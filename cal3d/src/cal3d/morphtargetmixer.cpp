//****************************************************************************//
// morphtargetmixer.cpp                                                       //
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
#include "cal3d/morphtargetmixer.h"
#include "cal3d/model.h"
#include "cal3d/coremodel.h"
#include "cal3d/coreanimatedmorph.h"
#include "cal3d/coremorphtrack.h"
#include "cal3d/mesh.h"
#include "cal3d/submesh.h"

 /*****************************************************************************/
/** Constructs the morph target mixer instance.
  *
  * This function is the default constructor of the morph target mixer instance.
  *****************************************************************************/

CalMorphTargetMixer::CalMorphTargetMixer(CalModel *pModel):
mPlayTime(0.f),
mPlayedAnimatedMorphID(-1)
{
  assert(pModel);
  m_pModel = pModel;
}


/*****************************************************************************/
/** Interpolates the weight of a morph target.
  *
  * This function interpolates the weight of a morph target a new value
  * in a given amount of time.
  *
  * @param id The ID of the morph target that should be blended.
  * @param weight The weight to interpolate the morph target to.
  * @param delay The time in seconds until the new weight should be reached.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happened
  *****************************************************************************/
bool CalMorphTargetMixer::blend(int id, float weight, float delay)
{
   mPlayTime = 0.f;
   mPlayedAnimatedMorphID = id;
   return true;
}

 /*****************************************************************************/
/** Fades a morph target out.
  *
  * This function fades a morph target out in a given amount of time.
  *
  * @param id The ID of the morph target that should be faded out.
  * @param delay The time in seconds until the the morph target is
  *              completely removed.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happened
  *****************************************************************************/

bool CalMorphTargetMixer::clear(int id, float delay)
{
   mPlayTime = 0.f;
   mPlayedAnimatedMorphID = -1;
   return true;
}

 /*****************************************************************************/
/** Get the weight of a morph target.
  *
  * @param id The id of the morph target which weight you want.
  *
  * @return The weight of the morph target with the given id.
  *****************************************************************************/
float CalMorphTargetMixer::getCurrentWeight(int id) const
{
   return 0.f;
}


 /*****************************************************************************/
/** Copy data from one mixer (for the same core model) to another.
  *
  * @param inOther The mixer to copy.
  * @return True on success.
  *****************************************************************************/
bool CalMorphTargetMixer::copy( const CalMorphTargetMixer& inOther )
{
	if (inOther.m_pModel->getCoreModel() != this->m_pModel->getCoreModel())
	{
  		CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
  		return false;
	}

   mPlayTime = inOther.mPlayTime;
   mPlayedAnimatedMorphID = inOther.mPlayedAnimatedMorphID;

   return true;
}

 /*****************************************************************************/
/** Updates all morph targets.
  *
  * This function updates all morph targets of the mixer instance for a
  * given amount of time.
  *
  * @param deltaTime The elapsed time in seconds since the last update.
  *****************************************************************************/

void CalMorphTargetMixer::update(float deltaTime)
{
   if (mPlayedAnimatedMorphID < 0)
   {
      return; //nothing to play
   }

   mPlayTime += deltaTime;

   const CalCoreAnimatedMorph* morph = m_pModel->getCoreModel()->getCoreAnimatedMorph(mPlayedAnimatedMorphID);
   if (morph == NULL)
   {
      return;
   }

   SetTrackWeights(*morph, mPlayTime);


  //int morphAnimationID = 0;
  //while(morphAnimationID<getMorphTargetCount())
  //{
  //  CalCoreMorphAnimation* pCoreMorphAnimation = 
  //                 m_pModel->getCoreModel()->getCoreMorphAnimation(morphAnimationID);
  //  std::vector<int>& vectorCoreMeshID = pCoreMorphAnimation->getVectorCoreMeshID();
  //  std::vector<int>& vectorMorphTargetID = pCoreMorphAnimation->getVectorMorphTargetID();
  //  size_t meshIterator = 0;
  //  while(meshIterator<vectorCoreMeshID.size())
  //  {
  //    int coreMeshID = vectorCoreMeshID[meshIterator];
  //    const CalMesh *theMesh = m_pModel->getMesh( coreMeshID );
  //    if (theMesh)
  //    {
  //      const std::vector<CalSubmesh *> &vectorSubmesh = 
  //        theMesh->getVectorSubmesh();
  //      int submeshCount = vectorSubmesh.size();
  //      int submeshId;
  //      for(submeshId=0;submeshId<submeshCount;++submeshId)
  //      {
  //        vectorSubmesh[submeshId]->setMorphTargetWeight 
  //          (vectorMorphTargetID[meshIterator],
  //          m_vectorCurrentWeight[morphAnimationID]);
  //      }
  //    }
  //    ++meshIterator;
  //  }
  //  ++morphAnimationID;
  //}
}

 /*****************************************************************************/
/** Returns the number of morph targets this morph target mixer mixes.
  *
  * @return The number of morph targets this morph target mixer mixes.
  *****************************************************************************/

int CalMorphTargetMixer::getMorphTargetCount() const
{
   return 0;
}

//////////////////////////////////////////////////////////////////////////
void CalMorphTargetMixer::SetTrackWeights(const CalCoreAnimatedMorph& morph, float elapsedTime)
{
   //For every track in the animation, find the weight of the key frame
   //that's related to the elapsedTime and adjust the weight of the morph target
   //mesh that matches the track's name.

   const std::list<CalCoreMorphTrack> tracks = morph.getListCoreTrack();

   std::list<CalCoreMorphTrack>::const_iterator itr;

   for (itr=tracks.begin(); itr!=tracks.end(); ++itr)
   {
      const CalCoreMorphTrack *track = &(*itr);
      const std::vector<CalCoreMorphKeyframe> &keyFrames = track->getVectorCoreMorphKeyframes();      

      float trackWeight = 0.f;

      if (keyFrames.empty())
      {
         trackWeight = 0.f;
      }
      else if (keyFrames.back().getTime() > elapsedTime)
      {
         //only calc the weight if there are key frames left to be played
         trackWeight = CalcKeyframeWeight(keyFrames, elapsedTime);
      }

      ApplyWeightToMorphMesh(track->getMorphName(), trackWeight);
   }

   return ;
}

/** Apply a linear interpolation between the two supplied numbers using a
* third percentage value.
*
* @param x : specifies the left bound of the range.
* @param y : specifies the right bound of the range.
* @param t: the normalized value with respect to the specified range to be interpolated.
* @return the interpolated value for the coefficient of the range.
*/
template <typename Real>
inline Real Lerp(Real x, Real y, Real t)
{
   return x + t * (y - x);
}

/// Normalizes a value within a specified space range.
/// Usage:  To find the normalized value for a range:
/// float nX = CalculateNormal( valueX , xMin , xMax );
/// @param sX the value with respect to the specified range to be normalized.
/// @param sMin specifies the left bound of the range.
/// @param sMax specifies the right bound of the range.
/// @return the normalized coefficient for the input to the range.
template<typename T>
T CalculateNormal(T sX, T sMin, T sMax)
{
   T delta( sMax - sMin );
   return( (sX-sMin) / delta );
}


/// Calculates the corresponding value for a mirrored space.
/// Given defined ranges for X space and Y space, and a known value in X space,
/// where X space and Y space are linearly related, find the corresponding value in Y space.
/// Usage: float y = MapRangeValue(x,xMin,xMax,yMin,yMax);
/// @param sX the value with respect to the X range to be transformed into the Y range.
/// @param xMin specifies the left bound of the X range.
/// @param xMax specifies the right bound of the X range.
/// @param yMin specifies the left bound of the Y range.
/// @param yMax specifies the right bound of the Y range.
template<typename T>
T MapRangeValue(T sX, T xMin, T xMax, T yMin, T yMax)
{
   return( Lerp( yMin, yMax, CalculateNormal(sX,xMin,xMax)) );
}


//////////////////////////////////////////////////////////////////////////
float CalMorphTargetMixer::CalcKeyframeWeight(const std::vector<CalCoreMorphKeyframe> &keyFrames, float elapsedTime)
{
   std::vector<CalCoreMorphKeyframe>::const_iterator keyframeItr;

   //find the first key frame that has a time greater than the elapsed time
   for (keyframeItr = keyFrames.begin(); keyframeItr != keyFrames.end(); ++keyframeItr)
   {
      if ((*keyframeItr).getTime() > elapsedTime)
      {
         break;
      }
   }

   //if the key frame is the first, or there aren't any key frames left to play
   //then set the weight to zero
   if (keyframeItr == keyFrames.end() ||
       keyframeItr == keyFrames.begin())
   {
      return (0.f);
   }

   const float endTime = (*keyframeItr).getTime();
   const float endWeight = (*keyframeItr).getWeight();
   
   //get the previous key frame
   --keyframeItr;
   const float startTime = (*keyframeItr).getTime();
   const float startWeight = (*keyframeItr).getWeight();
  
   return (MapRangeValue(elapsedTime, startTime, endTime, startWeight, endWeight));
}

//////////////////////////////////////////////////////////////////////////
void CalMorphTargetMixer::ApplyWeightToMorphMesh(const std::string& morphMeshName,
                                                 float trackWeight)
{
   //Apply the given weight to the morph mesh.

   //TODO.  This is pretty ugly.  Need a better way to find the right CalSubMesh
   //to set the morph target weight on.
   std::vector<CalMesh*> meshVec = m_pModel->getVectorMesh();
   std::vector<CalMesh*>::iterator meshItr;
   for (meshItr=meshVec.begin(); meshItr!=meshVec.end(); ++meshItr)
   {
      std::vector<CalSubmesh*> subMeshVec = (*meshItr)->getVectorSubmesh();
      std::vector<CalSubmesh*>::iterator subMeshitr;
      for (subMeshitr=subMeshVec.begin(); subMeshitr!=subMeshVec.end(); ++subMeshitr)
      {
         (*subMeshitr)->setMorphTargetWeight(morphMeshName, trackWeight);
      }
   }
}
//****************************************************************************//
