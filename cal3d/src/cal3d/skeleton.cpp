//****************************************************************************//
// calskeleton.cpp                                                            //
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

#include "cal3d/skeleton.h"
#include "cal3d/error.h"
#include "cal3d/bone.h"
#include "cal3d/coreskeleton.h"
#include "cal3d/coremodel.h"
#include "cal3d/corebone.h" // DEBUG

using namespace cal3d;
 /*****************************************************************************/
/** Constructs the skeleton instance.
  *
  * This function is the default constructor of the skeleton instance.
  *****************************************************************************/

CalSkeleton::CalSkeleton(CalCoreSkeleton *pCoreSkeleton)
  : m_pCoreSkeleton(0)
  , m_isBoundingBoxesComputed(false)
{
  assert(pCoreSkeleton);
  m_pCoreSkeleton = pCoreSkeleton;

  // clone the skeleton structure of the core skeleton
  std::vector<CalCoreBone *>& vectorCoreBone = pCoreSkeleton->getVectorCoreBone();

  // get the number of bones
  int boneCount = vectorCoreBone.size();

  // reserve space in the bone vector
  m_vectorBone.reserve(boneCount);

  // clone every core bone
  for(int boneId = 0; boneId < boneCount; ++boneId)
  {
    CalBone *pBone = new CalBone(vectorCoreBone[boneId]);

    // set skeleton in the bone instance
    pBone->setSkeleton(this);

    // insert bone into bone vector
    m_vectorBone.push_back(pBone);
  }
}

 /*****************************************************************************/
/** Destructs the skeleton instance.
  *
  * This function is the destructor of the skeleton instance.
  *****************************************************************************/

CalSkeleton::~CalSkeleton()
{
  // destroy all bones
  std::vector<CalBone *>::iterator iteratorBone;
  for(iteratorBone = m_vectorBone.begin(); iteratorBone != m_vectorBone.end(); ++iteratorBone)
  {
    delete (*iteratorBone);
  }
}

 /*****************************************************************************/
/** Calculates the state of the skeleton instance.
  *
  * This function calculates the state of the skeleton instance by recursively
  * calculating the states of its bones.
  *****************************************************************************/

void CalSkeleton::calculateState()
{
  // calculate all bone states of the skeleton
  const std::vector<int>& listRootCoreBoneId = m_pCoreSkeleton->getVectorRootCoreBoneId();

  std::vector<int>::const_iterator iteratorRootBoneId;
  for(iteratorRootBoneId = listRootCoreBoneId.begin(); iteratorRootBoneId != listRootCoreBoneId.end(); ++iteratorRootBoneId)
  {
    m_vectorBone[*iteratorRootBoneId]->calculateState();
  }
  m_isBoundingBoxesComputed=false;
}

 /*****************************************************************************/
/** Clears the state of the skeleton instance.
  *
  * This function clears the state of the skeleton instance by recursively
  * clearing the states of its bones.
  *****************************************************************************/

void CalSkeleton::clearState()
{
  // clear all bone states of the skeleton
  std::vector<CalBone *>::iterator iteratorBone;
  for(iteratorBone = m_vectorBone.begin(); iteratorBone != m_vectorBone.end(); ++iteratorBone)
  {
    (*iteratorBone)->clearState();
  }
  m_isBoundingBoxesComputed=false;
}
 
 /*****************************************************************************/
/** Locks the state of the skeleton instance.
  *
  * This function locks the state of the skeleton instance by recursively
  * locking the states of its bones.
  *****************************************************************************/

void CalSkeleton::lockState()
{
  // lock all bone states of the skeleton
  std::vector<CalBone *>::iterator iteratorBone;
  for(iteratorBone = m_vectorBone.begin(); iteratorBone != m_vectorBone.end(); ++iteratorBone)
  {
    (*iteratorBone)->lockState();
  }
}

/*****************************************************************************/
/** Calculates axis aligned bounding box of skeleton bones
  *
  * @param min The vector where the min values of bb are stored.
  * @param man The vector where the max values of bb are stored.
  *
  *****************************************************************************/

void CalSkeleton::getBoneBoundingBox(float *min, float *max)
{
  if(!m_isBoundingBoxesComputed)
  {
	  calculateBoundingBoxes();
  }


  std::vector<CalBone *>::iterator iteratorBone;

  if ((iteratorBone = m_vectorBone.begin()) != m_vectorBone.end()) {
    const CalVector& translation = (*iteratorBone)->getTranslationAbsolute();

    min[0] = max[0] = translation[0];
    min[1] = max[1] = translation[1];
    min[2] = max[2] = translation[2];

    ++iteratorBone;
  }

  for(; iteratorBone != m_vectorBone.end(); ++iteratorBone) {
    const CalVector& translation = (*iteratorBone)->getTranslationAbsolute();

    if (translation[0] > max[0])
      max[0] = translation[0];
    else if (translation[0] < min[0])
      min[0] = translation[0];

    if (translation[1] > max[1])
      max[1] = translation[1];
    else if (translation[1] < min[1])
      min[1] = translation[1];

    if (translation[2] > max[2])
      max[2] = translation[2];
    else if (translation[2] < min[2])
      min[2] = translation[2];

  }

}
 /*****************************************************************************/
/** Calculates bounding boxes.
  *
  * This function Calculates the bounding box of every bone in the Skeleton.
  *
  *****************************************************************************/


void CalSkeleton::calculateBoundingBoxes()
{
   if(m_isBoundingBoxesComputed) 
	   return;

   for(size_t boneId=0;boneId<m_vectorBone.size();++boneId)
   {
      m_vectorBone[boneId]->calculateBoundingBox();
   }
   m_isBoundingBoxesComputed=true;

}

