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

CalMorphTargetMixer::CalMorphTargetMixer(CalModel *pModel)
{
  assert(pModel);
  m_pModel = pModel;
}

/*****************************************************************************/
/** Interpolates the morph target weights based on an animation.
  *
  * This function sets a morph target animation at a specific time interval.
  *
  * @param id The ID of the morph target that should be blended.
  * @param weight The weight to interpolate the morph target to.
  * @param time The current time value to set the animation at.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happened
  *****************************************************************************/
bool CalMorphTargetMixer::manualBlend(int id, float weight, float time)
{
   // First check if the animation is already playing.
   int count = (int)mAnimList.size();
   for (int index = 0; index < count; ++index)
   {
      MorphAnimData& data = mAnimList[index];

      if (data.animatedMorphID == id)
      {
         // Reset the currently playing animation.
         data.isManual = true;

         data.weight = weight;
         data.looping = false;

         data.playTime = time;
         data.currentWeight = weight;
         data.fadeIn = 0.0f;
         data.fadeInTime = 0.0f;
         data.fadeOut = -1.0f;
         data.fadeOutTime = 0.0f;
         return true;
      }
   }

   // If we get here, create a new animation entry to play.
   MorphAnimData data;
   data.isManual = true;
   data.animatedMorphID = id;
   data.weight = weight;
   data.looping = false;
   data.playTime = time;
   data.currentWeight = weight;
   data.fadeIn = 0.0f;
   data.fadeInTime = 0.0f;
   data.fadeOut = -1.0f;
   data.fadeOutTime = 0.0f;
   mAnimList.push_back(data);
   return true;
}

/*****************************************************************************/
/** Interpolates the morph target weights based on an animation.
  *
  * This function interpolates the weight of a morph target a new value
  * in a given amount of time.
  *
  * @param id The ID of the morph target that should be blended.
  * @param weight The weight to interpolate the morph target to.
  * @param delayIn The time in seconds until the new weight should be reached.
  * @param delayOut The time in seconds until the weight goes back to zero at the end of the animation.
  * @param looping True if the morph animation should continuously loop.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happened
  *****************************************************************************/
bool CalMorphTargetMixer::blend(int id, float weight, float delayIn, float delayOut, bool looping)
{
   // First check if the animation is already playing.
   int count = (int)mAnimList.size();
   for (int index = 0; index < count; ++index)
   {
      MorphAnimData& data = mAnimList[index];

      if (data.animatedMorphID == id)
      {
         // Reset the currently playing animation.
         data.isManual = false;

         data.weight = weight;
         data.looping = looping;

         data.playTime = 0.0f;
         data.currentWeight = 0.0f;
         data.fadeIn = 0.0f;
         data.fadeInTime = delayIn;
         data.fadeOut = -1.0f;
         data.fadeOutTime = delayOut;
         return true;
      }
   }

   // If we get here, create a new animation entry to play.
   MorphAnimData data;
   data.isManual = false;
   data.animatedMorphID = id;
   data.weight = weight;
   data.looping = looping;
   data.playTime = 0.0f;
   data.currentWeight = 0.0f;
   data.fadeIn = 0.0f;
   data.fadeInTime = delayIn;
   data.fadeOut = -1.0f;
   data.fadeOutTime = delayOut;
   mAnimList.push_back(data);
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
   // First check if the animation is already playing.
   int count = (int)mAnimList.size();
   for (int index = 0; index < count; ++index)
   {
      MorphAnimData& data = mAnimList[index];

      if (data.animatedMorphID == id)
      {
         if (delay <= 0.0f)
         {
            // Turn all weights for this animation off.
            const CalCoreAnimatedMorph* morph = m_pModel->getCoreModel()->getCoreAnimatedMorph(id);
            if (!morph) return false;

            const std::list<CalCoreMorphTrack> tracks = morph->getListCoreTrack();

            std::list<CalCoreMorphTrack>::const_iterator itr;
            for (itr=tracks.begin(); itr!=tracks.end(); ++itr)
            {
               const CalCoreMorphTrack *track = &(*itr);
               ApplyWeightToMorphMesh(track->getMorphName(), 0.0f);
            }

            mAnimList.erase(mAnimList.begin() + index);
            return true;
         }

         data.looping = false;
         data.fadeOut = 0.0f;
         data.fadeOutTime = delay;
         return true;
      }
   }

   return false;
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
   int count = (int)mAnimList.size();
   for (int index = 0; index < count; ++index)
   {
      const MorphAnimData& data = mAnimList[index];

      if (data.animatedMorphID == id)
      {
         return data.currentWeight;
      }
   }

   return 0.0f;
}

 /*****************************************************************************/
/** Get the name of a morph target animation.
  *
  * @param id The id of the morph target.
  *
  * @return The morph target animation name.
  *****************************************************************************/
const std::string& CalMorphTargetMixer::getMorphName(int id) const
{
   const CalCoreAnimatedMorph* morph = m_pModel->getCoreModel()->getCoreAnimatedMorph(id);
   if (morph)
   {
      return morph->getName();
   }

   return "";
}

/*****************************************************************************/
/** Get the number of tracks for a morph target.
  *
  * @param id The id of the morph target.
  *
  * @return The number of tracks for the morph target with the given id.
  *****************************************************************************/
int CalMorphTargetMixer::getTrackCount(int id) const
{
   const CalCoreAnimatedMorph* morph = m_pModel->getCoreModel()->getCoreAnimatedMorph(id);
   if (morph)
   {
      return (int)morph->getListCoreTrack().size();
   }

   return 0;
}

/*****************************************************************************/
/** Get the tracks for a morph target.
  *
  * @param id The id of the morph target.
  *
  * @return The tracks for the morph target with the given id.
  *****************************************************************************/
const std::list<CalCoreMorphTrack>& CalMorphTargetMixer::getMorphTracks(int id) const
{
   const CalCoreAnimatedMorph* morph = m_pModel->getCoreModel()->getCoreAnimatedMorph(id);
   if (morph)
   {
      return morph->getListCoreTrack();
   }

   return std::list<CalCoreMorphTrack>();
}

 /*****************************************************************************/
/** Get the number of keyframes for a morph target.
  *
  * @param id The id of the morph target.
  *
  * @return The number of keyframes for the morph target with the given id.
  *****************************************************************************/
int CalMorphTargetMixer::getKeyframeCount(int id) const
{
   const CalCoreAnimatedMorph* morph = m_pModel->getCoreModel()->getCoreAnimatedMorph(id);
   if (morph)
   {
      const std::list<CalCoreMorphTrack> tracks = morph->getListCoreTrack();

      std::list<CalCoreMorphTrack>::const_iterator itr;

      int keyFrames = 0;
      for (itr=tracks.begin(); itr!=tracks.end(); ++itr)
      {
         const CalCoreMorphTrack *track = &(*itr);
         keyFrames += (int)track->getVectorCoreMorphKeyframes().size();
      }

      return keyFrames;
   }

   return 0;
}

 /*****************************************************************************/
/** Get the duration of a morph target.
  *
  * @param id The id of the morph target.
  *
  * @return The duration of the morph target with the given id.
  *****************************************************************************/
float CalMorphTargetMixer::getDuration(int id) const
{
   const CalCoreAnimatedMorph* morph = m_pModel->getCoreModel()->getCoreAnimatedMorph(id);
   if (morph)
   {
      return morph->getDuration();
   }

   return -1.0f;
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

   mAnimList = inOther.mAnimList;

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
   for (int index = 0; index < (int)mAnimList.size(); ++index)
   {
      MorphAnimData& data = mAnimList[index];

      const CalCoreAnimatedMorph* morph = m_pModel->getCoreModel()->getCoreAnimatedMorph(data.animatedMorphID);
      if (!morph) continue;

      // Only non-manual animations interpolate the play time and fade values.
      if (!data.isManual)
      {
         // Update the play time.
         data.playTime += deltaTime;

         // Update the fade in time.
         if (data.fadeIn < data.fadeInTime)
         {
            data.fadeIn += deltaTime;
            if (data.fadeIn > data.fadeInTime)
            {
               data.fadeIn = -1.0f;
               data.fadeInTime = 0.0f;
            }
         }

         // Update the fade out time.
         if (data.fadeOut > -1.0f)
         {
            data.fadeOut += deltaTime;
         }
      }

      // Update the morph weight.
      SetTrackWeights(*morph, data);

      // If we are finished fading out, clear this animation.
      if (data.fadeOut > -1.0f)
      {
         if (data.fadeOut >= data.fadeOutTime)
         {
            clear(data.animatedMorphID, 0.0f);
            index--;
         }
      }
   }

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
   return m_pModel->getCoreModel()->getNumCoreAnimatedMorphs();
}

//////////////////////////////////////////////////////////////////////////
void CalMorphTargetMixer::SetTrackWeights(const CalCoreAnimatedMorph& morph, MorphAnimData& data)
{
   //For every track in the animation, find the weight of the key frame
   //that's related to the elapsedTime and adjust the weight of the morph target
   //mesh that matches the track's name.

   const std::list<CalCoreMorphTrack>& tracks = morph.getListCoreTrack();

   std::list<CalCoreMorphTrack>::const_iterator itr;

   // If we are at the end of our animation.
   if (morph.getDuration() < data.playTime)
   {
      // Loop the animation.
      if (data.looping)
      {
         data.playTime = 0.0f;
      }
      // Fade the animation out.
      else if (data.fadeOut <= -1.0f)
      {
         float fadeOut = data.fadeOutTime;
         clear(data.animatedMorphID, fadeOut);
         if (fadeOut <= 0.0f)
         {
            return;
         }
      }
   }

   // Apply our fade in weight, if needed.
   float alpha = 1.0f;
   if (data.fadeIn >= 0.0f && data.fadeInTime > 0.0f)
   {
      alpha = data.fadeIn / data.fadeInTime;
   }

   // Apply our fade out weight, if needed.
   if (data.fadeOut >= 0.0f && data.fadeOutTime > 0.0f)
   {
      alpha = 1.0f - data.fadeOut / data.fadeOutTime;
   }

   data.currentWeight = alpha * data.weight;

   for (itr=tracks.begin(); itr!=tracks.end(); ++itr)
   {
      const CalCoreMorphTrack *track = &(*itr);
      const std::vector<CalCoreMorphKeyframe> &keyFrames = track->getVectorCoreMorphKeyframes(); 

      float weight = 0.0f;

      if (!keyFrames.empty() && keyFrames.back().getTime() > data.playTime)
      {
         //only calc the weight if there are key frames left to be played
         weight = CalcKeyframeWeight(keyFrames, data.playTime) * data.weight;
      }

      weight *= alpha;

      ApplyWeightToMorphMesh(track->getMorphName(), weight);
   }
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
