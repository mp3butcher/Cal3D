//****************************************************************************//
// bone.cpp                                                                   //
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


#include "cal3d/error.h"
#include "cal3d/bone.h"
#include "cal3d/coremodel.h"
#include "cal3d/coremesh.h"
#include "cal3d/coresubmesh.h"
#include "cal3d/corebone.h"
#include "cal3d/skeleton.h"
#include "cal3d/coreskeleton.h"


CalBone::CalBone(CalCoreBone *coreBone)
  : m_pSkeleton(0)
{
  assert(coreBone);
  m_pCoreBone = coreBone;
  clearState();
}


 /*****************************************************************************/
/** Interpolates the current state to another state.
  *
  * This function interpolates the current state (relative translation and
  * rotation) of the bone instance to another state of a given weight.
  *
  * @param unrampedWeight The blending weight, not incorporating ramp value
  * @param translation The relative translation to be interpolated to.
  * @param rotation The relative rotation to be interpolated to.
  * @param scale Optional scale from 0-1 applies to transformation directly without affecting weights.
  * @param replace If true, subsequent animations will have their weight attenuated by 1 - rampValue.
  * @param rampValue Amount to attenuate weight when ramping in/out the animation.
  *****************************************************************************/

void CalBone::blendState(float unrampedWeight, const CalVector& translation, 
                         const CalQuaternion& rotation, float scale,
                         bool replace, float rampValue )
{

  // Attenuate the weight by the accumulated replacement attenuation.  Each applied
  // "replacement" animation attenuates the weights of the subsequent animations by
  // the inverse of its rampValue, so that when a replacement animation ramps up to
  // full, all lesser priority animations automatically ramp down to zero.
  float rampedWeight = unrampedWeight * rampValue;
  float attenuatedWeight = rampedWeight * m_accumulatedReplacementAttenuation;

  // It appears that quaternion::blend() only works with blend factors of 0-1, so
  // I'll clamp the scale to that range.
  if( scale < 0.0f ) {
    scale = 0.0f;
  }
  if( scale > 1.0f ) {
    scale = 1.0f;
  }

  // Now apply weighted, scaled transformation.  For weights, Cal starts with the
  // first and then blends the later ones in proportion to their weights.  Though this
  // would seem to depend on the order, you can reason by induction that it does not.
  // Each application of an animation gives it the correct proportion to the others in
  // aggregate and leaves in tact the proportions among the others.
  if( m_accumulatedWeightAbsolute == 0.0f ) {

    // It is the first state, so we can just copy it into the bone state.  The first animation
    // must be applied with scale = 1.0 since it is the initial pose rather than something
    // to be blended onto a pose.  If we scale the first state, the skeleton will look like
    // a crumpled spider.
    m_accumulatedWeightAbsolute = attenuatedWeight;
    m_translationAbsolute = translation;
    m_rotationAbsolute = rotation;

    // I would like to scale this blend, but I cannot since it is the initial pose.  Thus I
    // will store away this scale and compensate appropriately on the second blend.  See below.
    // After applying blend2, the blend1 = 1 - blend2.  If I would like to scale blend1 to 30%
    // of its original scale, for example, then I would like,
    //
    //      ( 1 - blend2' ) = 0.3 * ( 1 - blend2 )
    // so,
    //      blend2' = 1 - 0.3 * ( 1 - blend2 )
    //
    // or similarly for any value of m_firstBlendScale instead of 30%.
    m_firstBlendScale = scale;
  } else {

    // Consider an example with two animations, one or both of them "replace" animations.
    // Wave is a "replace" animation, played on top of Walk.  Wave is applied first since it is a 
    // "replace" animation and Walk is not.  Imagine Wave is ramping in, currently at 80%.  Wave sets
    // the initial pose 100% and then Walk is applied over that pose with a blend factor of 0.2.  The result
    // is that Wave is 80% and Walk is 20%, which is what you'd expect for replace semantics.
    //
    // Animation    RampedWeight  AttenuatedWeight    InAccumWeightAbs  OutAccAttenuation   Factor
    // Wave         0.8           0.8                 0.0               0.2 (replace)       n/a (100%)
    // Walk         1.0           0.2                 0.8               0.2 (not replace)   0.2/(0.8+0.2) = 0.2
    //
    // Consider the same example with two animations, but neither of them "replace" animations.
    // Assume Wave is applied first.  Imagine Wave is ramping in, currently at 80%.  Wave sets
    // the initial pose 100% and then Walk is applied over that pose with a blend factor of 0.55.  The result
    // is that Wave is 45% and Walk is 55%, which is about what you'd expect for non-replace semantics.
    //
    // Animation    RampedWeight  AttenuatedWeight    InAccumWeightAbs  OutAccAttenuation   Factor
    // Wave         0.8           0.8                 0.0               1.0 (not replace)   n/a (100%)
    // Walk         1.0           1.0                 0.8               1.0 (not replace)   1.0/(0.8+1.0) = 0.55
    //
    // Consider the same example again but reverse the order of Wave and Walk, so Walk is applied first.
    // As before, imagine Wave is ramping in, currently at 80%.  Walk sets the initial pose 100% 
    // and then Wave is applied over that pose with a blend factor of 0.44.  The result
    // is that Wave is 44% and Walk is 56%, which is also about what you'd expect for non-replace semantics.
    //
    // Animation    RampedWeight  AttenuatedWeight    InAccumWeightAbs  OutAccAttenuation   Factor
    // Walk         1.0           1.0                 0.0               1.0 (not replace)   n/a (100%)
    // Wave         0.8           0.8                 1.0               1.0 (not replace)   0.8/(0.8+1.0) = 0.44
    //
    // Now consider an example in which Point and Wave are both applied over Walk, with Point applied
    // first at highest priority.  Assume that Point is ramped at 90% and Wave is ramped at 80%.  Both
    // Point and Wave are "replace" animations.  Walk is not.  The result is Walk is 2%, Wave is about 8%,
    // and Point is about 90%, which seems like a reasonable result.
    //
    // Animation    RampedWeight  AttenuatedWeight    InAccumWeightAbs  OutAccAttenuation   Factor
    // Point        0.9           0.9                 0                 0.1 (replace)       n/a (100%)
    // Wave         0.8           0.08                0.9               0.02 (replace)      0.08/(0.9+0.08) = 0.082
    // Walk         1.0           0.02                0.98              0.02 (not replace)  0.02/(0.98+0.02) = 0.02
    //
    // Finally, consider an example in which Point and Wave are both applied over Walk, but in which
    // none of the animations is a "replace" animation.  For this example, assume that Point, Wave,
    // and Walk all are fully ramped in at 100%.  The result is Walk is 33%, Wave is about 33%,
    // and Point is about 33%, which seems like the right result.
    //
    // Animation    RampedWeight  AttenuatedWeight    InAccumWeightAbs  OutAccAttenuation   Factor
    // Point        1.0           1.0                 0.0               1.0 (not replace)   n/a (100%)
    // Wave         1.0           1.0                 1.0               1.0 (not replace)   1.0/(1.0+1.0) = 0.5
    // Walk         1.0           1.0                 2.0               1.0 (not replace)   1.0/(1.0+2.0) = 0.33
    float factor = scale * attenuatedWeight / ( m_accumulatedWeightAbsolute + attenuatedWeight );

    // If the scale of the first blend was not 1.0, then I will adjust the factor of the second blend
    // to compensate, 
    //
    //      factor' = 1 - m_firstBlendScale * ( 1 - factor )
    //
    assert( factor <= 1.0f );
    factor = 1.0f - m_firstBlendScale * ( 1.0f - factor );
    m_translationAbsolute.blend(factor, translation);
    m_rotationAbsolute.blend(factor, rotation);
    m_accumulatedWeightAbsolute += attenuatedWeight;
    m_firstBlendScale = 1.0;
  }
  if( replace ) {
    m_accumulatedReplacementAttenuation *= ( 1.0f - rampValue );
  }
}

 /*****************************************************************************/
/** Calculates the current state.
  *
  * This function calculates the current state (absolute translation and
  * rotation, as well as the bone space transformation) of the bone instance
  * and all its children.
  *****************************************************************************/

void CalBone::calculateState()
{
  // check if the bone was not touched by any active animation
  if(m_accumulatedWeight == 0.0f)
  {
    // set the bone to the initial skeleton state
    m_translation = m_pCoreBone->getTranslation();
    m_rotation = m_pCoreBone->getRotation();
  }

  // get parent bone id
  int parentId;
  parentId = m_pCoreBone->getParentId();

  if(parentId == -1)
  {
    // no parent, this means absolute state == relative state
    m_translationAbsolute = m_translation;
    m_rotationAbsolute = m_rotation;
  }
  else
  {
    // get the parent bone
    const CalBone *pParent;
    pParent = m_pSkeleton->getBone(parentId);

    // transform relative state with the absolute state of the parent
    m_translationAbsolute = m_translation;
    m_translationAbsolute *= pParent->getRotationAbsolute();
    m_translationAbsolute += pParent->getTranslationAbsolute();

    m_rotationAbsolute = m_rotation;
    m_rotationAbsolute *= pParent->getRotationAbsolute();
  }

  // calculate the bone space transformation
  m_translationBoneSpace = m_pCoreBone->getTranslationBoneSpace();

  // Must go before the *= m_rotationAbsolute.
  bool meshScalingOn;
  if( m_meshScaleAbsolute.x != 1 || m_meshScaleAbsolute.y != 1 || m_meshScaleAbsolute.z != 1 ) {
    meshScalingOn = true;
    CalVector scalevec;

    // The mesh transformation is intended to apply to the vector from the
    // bone node to the vert, relative to the model's global coordinate system.
    // For example, even though the head node's X axis aims up, the model's 
    // global coordinate system has X to stage right, Z up, and Y stage back.
    //
    // The standard vert transformation is:
    // v1 = vmesh - boneAbsPosInJpose
    // v2 = v1 * boneAbsRotInAnimPose
    // v3 = v2 + boneAbsPosInAnimPose
    //
    // Cal3d does the calculation by:
    // u1 = umesh * transformMatrix
    // u2 = u1 + translationBoneSpace
    //
    // where translationBoneSpace = 
    //   "coreBoneTranslationBoneSpace"
    //   * boneAbsRotInAnimPose
    //   + boneAbsPosInAnimPose
    //
    // and where transformMatrix =
    //   "coreBoneRotBoneSpace"
    //   * boneAbsRotInAnimPose
    //
    // I don't know what "coreBoneRotBoneSpace" and "coreBoneTranslationBoneSpace" actually are,
    // but to add scale to the scandard vert transformation, I simply do:
    // 
    // v3' = vmesh           * scalevec    * boneAbsRotInAnimPose
    //   - boneAbsPosInJpose * scalevec    * boneAbsRotInAnimPose
    //   + boneAbsPosInAnimPose
    //
    // Essentially, the boneAbsPosInJpose is just an extra vector added to
    // each vertex that we want to subtract out.  We must transform the extra
    // vector in exactly the same way we transform the vmesh.  Therefore if we scale the mesh, we
    // must also scale the boneAbsPosInJpose.
    //
    // Expanding out the u2 equation, we have:
    //
    // u2 = umesh * "coreBoneRotBoneSpace"   * boneAbsRotInAnimPose
    //   + "coreBoneTranslationBoneSpace"    * boneAbsRotInAnimPose
    //   + boneAbsPosInAnimPose
    //
    // We assume that "coreBoneTranslationBoneSpace" = vectorThatMustBeSubtractedFromUmesh * "coreBoneRotBoneSpace":
    //
    // u2 = umesh * "coreBoneRotBoneSpace"                                 * boneAbsRotInAnimPose
    //   + vectorThatMustBeSubtractedFromUmesh * "coreBoneRotBoneSpace"    * boneAbsRotInAnimPose
    //   + boneAbsPosInAnimPose
    // 
    // We assume that scale should be applied to umesh, not umesh * "coreBoneRotBoneSpace":
    //
    // u2 = umesh * scaleVec * "coreBoneRotBoneSpace" * boneAbsRotInAnimPose
    //   + "coreBoneTranslationBoneSpace" * "coreBoneRotBoneSpaceInverse" * scaleVec * "coreBoneRotBoneSpace" * boneAbsRotInAnimPose
    //   + boneAbsPosInAnimPose
    //
    // which yields,
    //
    // transformMatrix' =  scaleVec * "coreBoneRotBoneSpace" * boneAbsRotInAnimPose
    //
    // and,
    //
    // translationBoneSpace' = 
    //   coreBoneTranslationBoneSpace * "coreBoneRotBoneSpaceInverse" * scaleVec * "coreBoneRotBoneSpace"
    //   * boneAbsRotInAnimPose
    //   + boneAbsPosInAnimPose

    CalQuaternion coreBoneRotBoneSpaceInverse = m_pCoreBone->getRotationBoneSpace();
    coreBoneRotBoneSpaceInverse.invert();
    m_translationBoneSpace *= coreBoneRotBoneSpaceInverse;
    m_translationBoneSpace.x *= m_meshScaleAbsolute.x;
    m_translationBoneSpace.y *= m_meshScaleAbsolute.y;
    m_translationBoneSpace.z *= m_meshScaleAbsolute.z;
    m_translationBoneSpace *= m_pCoreBone->getRotationBoneSpace();

  } else {
    meshScalingOn = false;
  }
  m_translationBoneSpace *= m_rotationAbsolute;
  m_translationBoneSpace += m_translationAbsolute;

  m_rotationBoneSpace = m_pCoreBone->getRotationBoneSpace();
  m_rotationBoneSpace *= m_rotationAbsolute;

  m_transformMatrix = m_pCoreBone->getRotationBoneSpace();
  if( meshScalingOn ) {

    // By applying each scale component to the row, instead of the column, we
    // are effectively making the scale apply prior to the rotationBoneSpace.
    m_transformMatrix.dxdx *= m_meshScaleAbsolute.x;
    m_transformMatrix.dydx *= m_meshScaleAbsolute.x;
    m_transformMatrix.dzdx *= m_meshScaleAbsolute.x;  

    m_transformMatrix.dxdy *= m_meshScaleAbsolute.y;
    m_transformMatrix.dydy *= m_meshScaleAbsolute.y;
    m_transformMatrix.dzdy *= m_meshScaleAbsolute.y;  

    m_transformMatrix.dxdz *= m_meshScaleAbsolute.z;
    m_transformMatrix.dydz *= m_meshScaleAbsolute.z;
    m_transformMatrix.dzdz *= m_meshScaleAbsolute.z;  
  }
  m_transformMatrix *= m_rotationAbsolute;

  // calculate all child bones
  std::list<int>::iterator iteratorChildId;
  int i = 0;
  for(iteratorChildId = m_pCoreBone->getListChildId().begin(); iteratorChildId != m_pCoreBone->getListChildId().end(); ++iteratorChildId, i++ )
  {
    CalBone * bo = m_pSkeleton->getBone(*iteratorChildId);
    bo->calculateState();
  }
}

 /*****************************************************************************/
/** Clears the current state.
  *
  * This function clears the current state (absolute translation and rotation)
  * of the bone instance and all its children.
  *****************************************************************************/

void CalBone::clearState()
{
  m_accumulatedWeight = 0.0f;
  m_accumulatedWeightAbsolute = 0.0f;
  m_accumulatedReplacementAttenuation = 1.0f;
  m_firstBlendScale = 1.0f;
  m_meshScaleAbsolute.set( 1, 1, 1 );
}


 /*****************************************************************************/
/** Provides access to the core bone.
  *
  * This function returns the core bone on which this bone instance is based on.
  *
  * @return One of the following values:
  *         \li a pointer to the core bone
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreBone *CalBone::getCoreBone()
{
  return m_pCoreBone;
}


 /*****************************************************************************/
/** Provides access to the core bone.
  *
  * This function returns the core bone on which this bone instance is based on.
  *
  * @return One of the following values:
  *         \li a pointer to the core bone
  *         \li \b 0 if an error happened
  *****************************************************************************/

const CalCoreBone *CalBone::getCoreBone() const
{
  return m_pCoreBone;
}

 /*****************************************************************************/
/** Resets the bone to its core state
  *
  * This function changes the state of the bone to its default non-animated
  * position and orientation. Child bones are unaffected and may be animated
  * independently. 
  *****************************************************************************/

void CalBone::setCoreState()
{
   // set the bone to the initial skeleton state
   m_translation = m_pCoreBone->getTranslation();
   m_rotation = m_pCoreBone->getRotation();

   // set the appropriate weights
   m_accumulatedWeightAbsolute = 1.0f;
   m_accumulatedWeight = 1.0f ;

   calculateState() ;
}


 /*****************************************************************************/
/** Resets the bone and children to core states
  *
  * This function changes the state of the bone to its default non-animated
  * position and orientation. All child bones are also set in this manner.
  *****************************************************************************/

void CalBone::setCoreStateRecursive()
{
  // set the bone to the initial skeleton state
  m_translation = m_pCoreBone->getTranslation();
  m_rotation = m_pCoreBone->getRotation();

  // set the appropriate weights
  m_accumulatedWeightAbsolute = 1.0f;
  m_accumulatedWeight = 1.0f ;

  // set core state for all child bones
  std::list<int>::iterator iteratorChildId;
  for(iteratorChildId = m_pCoreBone->getListChildId().begin(); iteratorChildId != m_pCoreBone->getListChildId().end(); ++iteratorChildId)
  {
    m_pSkeleton->getBone(*iteratorChildId)->setCoreStateRecursive();
  }

  calculateState() ;
}

 /*****************************************************************************/
/** Sets the current rotation.
  *
  * This function sets the current relative rotation of the bone instance.
  * Caveat: For this change to appear, calculateState() must be called 
  * afterwards.
  *****************************************************************************/

void CalBone::setRotation(const CalQuaternion& rotation)
{
  m_rotation = rotation;
  m_accumulatedWeightAbsolute = 1.0f;
  m_accumulatedWeight = 1.0f ;
}

 /*****************************************************************************/
/** Returns the current rotation.
  *
  * This function returns the current relative rotation of the bone instance.
  *
  * @return The relative rotation to the parent as quaternion.
  *****************************************************************************/

const CalQuaternion& CalBone::getRotation() const
{
  return m_rotation;
}

 /*****************************************************************************/
/** Returns the current absolute rotation.
  *
  * This function returns the current absolute rotation of the bone instance.
  *
  * @return The absolute rotation to the parent as quaternion.
  *****************************************************************************/

const CalQuaternion& CalBone::getRotationAbsolute() const
{
  return m_rotationAbsolute;
}

 /*****************************************************************************/
/** Returns the current bone space rotation.
  *
  * This function returns the current rotation to bring a point into the bone
  * instance space.
  *
  * @return The rotation to bring a point into bone space.
  *****************************************************************************/

const CalQuaternion& CalBone::getRotationBoneSpace() const
{
  return m_rotationBoneSpace;
}

 /*****************************************************************************/
/** Sets the current translation.
  *
  * This function sets the current relative translation of the bone instance.
  * Caveat: For this change to appear, calculateState() must be called 
  * afterwards.
  *****************************************************************************/

void CalBone::setTranslation(const CalVector& translation)
{
  m_translation = translation;
  m_accumulatedWeightAbsolute = 1.0f;
  m_accumulatedWeight = 1.0f ;
}

 /*****************************************************************************/
/** Returns the current translation.
  *
  * This function returns the current relative translation of the bone instance.
  *
  * @return The relative translation to the parent as quaternion.
  *****************************************************************************/

const CalVector& CalBone::getTranslation() const
{
  return m_translation;
}

 /*****************************************************************************/
/** Returns the current absolute translation.
  *
  * This function returns the current absolute translation of the bone instance.
  *
  * @return The absolute translation to the parent as quaternion.
  *****************************************************************************/

const CalVector& CalBone::getTranslationAbsolute() const
{
  return m_translationAbsolute;
}

 /*****************************************************************************/
/** Returns the current bone space translation.
  *
  * This function returns the current translation to bring a point into the
  *bone instance space.
  *
  * @return The translation to bring a point into bone space.
  *****************************************************************************/

 /*****************************************************************************/
/** Returns the current bone space translation.
  *
  * This function returns the current translation to bring a point into the
  *bone instance space.
  *
  * @return The translation to bring a point into bone space.
  *****************************************************************************/


 /*****************************************************************************/
/** Locks the current state.
  *
  * This function locks the current state (absolute translation and rotation)
  * of the bone instance and all its children.
  *****************************************************************************/

void CalBone::lockState()
{
  // clamp accumulated weight
  if(m_accumulatedWeightAbsolute > 1.0f - m_accumulatedWeight)
  {
    m_accumulatedWeightAbsolute = 1.0f - m_accumulatedWeight;
  }

  if(m_accumulatedWeightAbsolute > 0.0f)
  {
    if(m_accumulatedWeight == 0.0f)
    {
      // it is the first state, so we can just copy it into the bone state
      m_translation = m_translationAbsolute;
      m_rotation = m_rotationAbsolute;

      m_accumulatedWeight = m_accumulatedWeightAbsolute;
    }
    else
    {
      // it is not the first state, so blend all attributes
      float factor;
      factor = m_accumulatedWeightAbsolute / (m_accumulatedWeight + m_accumulatedWeightAbsolute);

      m_translation.blend(factor, m_translationAbsolute);
      m_rotation.blend(factor, m_rotationAbsolute);

      m_accumulatedWeight += m_accumulatedWeightAbsolute;
    }

    m_accumulatedWeightAbsolute = 0.0f;
  }
}

 /*****************************************************************************/
/** Sets the skeleton.
  *
  * This function sets the skeleton to which the bone instance is attached to.
  *
  * @param pSkeleton The skeleton to which the bone instance should be
  *                  attached to.
  *****************************************************************************/

void CalBone::setSkeleton(CalSkeleton *pSkeleton)
{
  m_pSkeleton = pSkeleton;
}

 /*****************************************************************************/
/** Calculates the bounding box.
  *
  * This function Calculates the bounding box of the bone instance.
  *
  *****************************************************************************/


void CalBone::calculateBoundingBox()
{
   if(!getCoreBone()->isBoundingBoxPrecomputed())
	   return;

   CalVector dir = CalVector(1.0f,0.0f,0.0f);
   dir*=getTransformMatrix();
   m_boundingBox.plane[0].setNormal(dir);

   dir = CalVector(-1.0f,0.0f,0.0f);
   dir*=getTransformMatrix();
   m_boundingBox.plane[1].setNormal(dir);

   dir = CalVector(0.0f,1.0f,0.0f);
   dir*=getTransformMatrix();
   m_boundingBox.plane[2].setNormal(dir);

   dir = CalVector(0.0f,-1.0f,0.0f);
   dir*=getTransformMatrix();
   m_boundingBox.plane[3].setNormal(dir);

   dir = CalVector(0.0f,0.0f,1.0f);
   dir*=getTransformMatrix();
   m_boundingBox.plane[4].setNormal(dir);

   dir = CalVector(0.0f,0.0f,-1.0f);
   dir*=getTransformMatrix();
   m_boundingBox.plane[5].setNormal(dir);
   
   int i;
   
   for(i=0;i< 6; i++)
   {
       CalVector position;
       getCoreBone()->getBoundingData(i,position);
      
       position*=getTransformMatrix();
       position+=getTranslationBoneSpace();

       int planeId;
       for(planeId = 0; planeId < 6; ++planeId)
       {
          if(m_boundingBox.plane[planeId].eval(position) < 0.0f)
          {
             m_boundingBox.plane[planeId].setPosition(position);
          }
       }
       
   }
}

 /*****************************************************************************/
/** Returns the current bounding box.
  *
  * This function returns the current bounding box of the bone instance.
  *
  * @return bounding box.
  *****************************************************************************/


CalBoundingBox & CalBone::getBoundingBox()
{
   return m_boundingBox;
}

 /*****************************************************************************/
/** Returns the current bounding box.
  *
  * This function returns the current bounding box of the bone instance.
  *
  * @return bounding box.
  *****************************************************************************/

const CalBoundingBox & CalBone::getBoundingBox() const
{
  return m_boundingBox;
}

//****************************************************************************//
