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
#include "cal3d/mesh.h"
#include "cal3d/submesh.h"
#include "cal3d/coremesh.h"
#include "cal3d/coresubmesh.h"

 /*****************************************************************************/
/** Constructs the morph target mixer instance.
  *
  * This function is the default constructor of the morph target mixer instance.
  *****************************************************************************/

CalMorphTargetMixer::CalMorphTargetMixer()
{
  m_pMesh = 0;
}

 /*****************************************************************************/
/** Destructs the morph target mixer instance.
  *
  * This function is the destructor of the morph target mixer instance.
  *****************************************************************************/

CalMorphTargetMixer::~CalMorphTargetMixer()
{
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
  *         \li \b false if an error happend
  *****************************************************************************/
bool CalMorphTargetMixer::blend(int id, float weight, float delay)
{
  if((id < 0) || (id >= (int)m_vectorCurrentWeight.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }
  float currentWeight = 1.0f - m_vectorCurrentWeight[id];
  float endWeight = 1.0f - weight;
  if(currentWeight != 0.0f)
  {
    float factor = endWeight/currentWeight;
    std::vector<float>::iterator iteratorCurrentWeight = m_vectorCurrentWeight.begin();
    std::vector<float>::iterator iteratorEndWeight = m_vectorEndWeight.begin();
    while(iteratorCurrentWeight!=m_vectorCurrentWeight.end())
    {
      (*iteratorEndWeight) = (*iteratorCurrentWeight)*factor;
      ++iteratorCurrentWeight;
      ++iteratorEndWeight;
    }
  }
  else
  {
    float factor = endWeight/((float)m_vectorEndWeight.size());
    std::vector<float>::iterator iteratorEndWeight = m_vectorEndWeight.begin();
    while(iteratorEndWeight!=m_vectorEndWeight.end())
    {
      (*iteratorEndWeight) = factor;
      ++iteratorEndWeight;
    }
  }
  m_vectorEndWeight[id] = weight;
  m_duration = delay;
  return true;
}

 /*****************************************************************************/
/** Interpolates the weight of the base vertices.
  *
  * This function interpolates the weight of the base vertices a new value
  * in a given amount of time.
  *
  * @param weight The weight to interpolate the base vertices to.
  * @param delay The time in seconds until the new weight should be reached.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalMorphTargetMixer::blendBase(float weight, float delay)
{
  float currentWeight = 0.0f;
  std::vector<float>::iterator iteratorCurrentWeight = m_vectorCurrentWeight.begin();
  while(iteratorCurrentWeight!=m_vectorCurrentWeight.end())
  {
    currentWeight +=(*iteratorCurrentWeight);
    ++iteratorCurrentWeight;
  }
  float endWeight = 1.0f - weight;
  if(currentWeight != 0.0f)
  {
    float factor = endWeight/currentWeight;
    std::vector<float>::iterator iteratorCurrentWeight = m_vectorCurrentWeight.begin();
    std::vector<float>::iterator iteratorEndWeight = m_vectorEndWeight.begin();
    while(iteratorCurrentWeight!=m_vectorCurrentWeight.end())
    {
      (*iteratorEndWeight) = (*iteratorCurrentWeight)*factor;
      ++iteratorCurrentWeight;
      ++iteratorEndWeight;
    }
  }
  else
  {
    float factor = endWeight/((float)m_vectorEndWeight.size());
    std::vector<float>::iterator iteratorEndWeight = m_vectorEndWeight.begin();
    while(iteratorEndWeight!=m_vectorEndWeight.end())
    {
      (*iteratorEndWeight) = factor;
      ++iteratorEndWeight;
    }
  }
  m_duration = delay;
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
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalMorphTargetMixer::clear(int id, float delay)
{
  if((id < 0) || (id >= (int)m_vectorCurrentWeight.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }
  float currentWeight = 1.0f - m_vectorCurrentWeight[id];
  if(currentWeight != 0.0f)
  {
    float factor = 1.0f/currentWeight;
    std::vector<float>::iterator iteratorCurrentWeight = m_vectorCurrentWeight.begin();
    std::vector<float>::iterator iteratorEndWeight = m_vectorEndWeight.begin();
    while(iteratorCurrentWeight!=m_vectorCurrentWeight.end())
    {
      (*iteratorEndWeight) = (*iteratorCurrentWeight)*factor;
      ++iteratorCurrentWeight;
      ++iteratorEndWeight;
    }
  }
  else
  {
    float factor = 1.0f/((float)m_vectorEndWeight.size());
    std::vector<float>::iterator iteratorEndWeight = m_vectorEndWeight.begin();
    while(iteratorEndWeight!=m_vectorEndWeight.end())
    {
      (*iteratorEndWeight) = factor;
      ++iteratorEndWeight;
    }
  }
  m_vectorEndWeight[id] = 0.0f;
  m_duration = delay;
  return true;
}

 /*****************************************************************************/
/** Fades the base vertices out.
  *
  * This function fades the base vertices out in a given amount of time.
  *
  * @param delay The time in seconds until the the base vertices is
  *              completely removed.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalMorphTargetMixer::clearBase(float delay)
{
  float currentWeight = 0.0f;
  std::vector<float>::iterator iteratorCurrentWeight = m_vectorCurrentWeight.begin();
  while(iteratorCurrentWeight!=m_vectorCurrentWeight.end())
  {
    currentWeight +=(*iteratorCurrentWeight);
    ++iteratorCurrentWeight;
  }
  if(currentWeight != 0.0f)
  {
    float factor = 1.0f/currentWeight;
    std::vector<float>::iterator iteratorCurrentWeight = m_vectorCurrentWeight.begin();
    std::vector<float>::iterator iteratorEndWeight = m_vectorEndWeight.begin();
    while(iteratorCurrentWeight!=m_vectorCurrentWeight.end())
    {
      (*iteratorEndWeight) = (*iteratorCurrentWeight)*factor;
      ++iteratorCurrentWeight;
      ++iteratorEndWeight;
    }
  }
  else
  {
    float factor = 1.0f/((float)m_vectorEndWeight.size());
    std::vector<float>::iterator iteratorEndWeight = m_vectorEndWeight.begin();
    while(iteratorEndWeight!=m_vectorEndWeight.end())
    {
      (*iteratorEndWeight) = factor;
      ++iteratorEndWeight;
    }
  }
  m_duration = delay;
  return true;
}

 /*****************************************************************************/
/** Get the weight of a morph target.
  *
  * @param id The id of the morph target which weight you want.
  *
  * @return The weight of the morph target with the given id.
  *****************************************************************************/
float CalMorphTargetMixer::getCurrentWeight(int id)
{
  if((id < 0) || (id >= (int)m_vectorCurrentWeight.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }
  return m_vectorCurrentWeight[id];
}

 /*****************************************************************************/
/** Get the weight of the base vertices.
  *
  * @return The weight of the base vertices.
  *****************************************************************************/
float CalMorphTargetMixer::getCurrentWeightBase()
{
  float currentWeight = 1.0f;
  std::vector<float>::iterator iteratorCurrentWeight = m_vectorCurrentWeight.begin();
  while(iteratorCurrentWeight!=m_vectorCurrentWeight.end())
  {
    currentWeight -=(*iteratorCurrentWeight);
    ++iteratorCurrentWeight;
  }
  return currentWeight;
}
 /*****************************************************************************/
/** Creates the morph target mixer instance.
  *
  * This function creates the mixer instance.
  *
  * @param pMesh A pointer to the mesh that should be managed with this 
  *               morph target mixer instance.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalMorphTargetMixer::create(CalMesh *pMesh)
{
  if(pMesh == 0)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }
  
  m_pMesh = pMesh;

  CalCoreSubmesh *coreSubmesh = pMesh->getCoreMesh()->getCoreSubmesh(0);
  if (coreSubmesh == 0) 
  {
    CalError::setLastError(CalError::INTERNAL, __FILE__, __LINE__);//HMMM
    return false;
  }
  int subMorphTargetCount = coreSubmesh->getCoreSubMorphTargetCount();
  // reserve the space needed in all the vectors
  m_vectorCurrentWeight.reserve(subMorphTargetCount);
  m_vectorCurrentWeight.resize(subMorphTargetCount);
  m_vectorEndWeight.reserve(subMorphTargetCount);
  m_vectorEndWeight.resize(subMorphTargetCount);
  std::vector<float>::iterator iteratorCurrentWeight = m_vectorCurrentWeight.begin();
  std::vector<float>::iterator iteratorEndWeight = m_vectorEndWeight.begin();
  while(iteratorCurrentWeight!=m_vectorCurrentWeight.end())
  {
    (*iteratorCurrentWeight) = 0.0f;
    (*iteratorEndWeight) = 0.0f;
    ++iteratorCurrentWeight;
    ++iteratorEndWeight;
  }
  m_duration = 0.0f;

  return true;
}

 /*****************************************************************************/
/** Destroys the morph target mixer instance.
  *
  * This function destroys all data stored in the mixer instance and frees all
  * allocated memory.
  *****************************************************************************/

void CalMorphTargetMixer::destroy()
{
  m_pMesh = 0;
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
  if(deltaTime >= m_duration)
  {
    std::vector<float>::iterator iteratorCurrentWeight = m_vectorCurrentWeight.begin();
    std::vector<float>::iterator iteratorEndWeight = m_vectorEndWeight.begin();
    while(iteratorCurrentWeight!=m_vectorCurrentWeight.end())
    {
      (*iteratorCurrentWeight) = (*iteratorEndWeight);
      ++iteratorCurrentWeight;
      ++iteratorEndWeight;
    }
    m_duration = 0.0f;
  }
  else
  {
    std::vector<float>::iterator iteratorCurrentWeight = m_vectorCurrentWeight.begin();
    std::vector<float>::iterator iteratorEndWeight = m_vectorEndWeight.begin();
    while(iteratorCurrentWeight!=m_vectorCurrentWeight.end())
    {
      (*iteratorCurrentWeight) += ((*iteratorEndWeight)-(*iteratorCurrentWeight))*deltaTime/m_duration;
      ++iteratorCurrentWeight;
      ++iteratorEndWeight;
    }
    m_duration -= deltaTime;
  }
  std::vector<CalSubmesh *> &vectorSubmesh = m_pMesh->getVectorSubmesh();
  int morphTargetCount = m_vectorCurrentWeight.size();
  int submeshCount = vectorSubmesh.size();
  int submeshId;
  for(submeshId=0;submeshId<submeshCount;++submeshId)
  {
    int morphTargetId;
    for(morphTargetId=0;morphTargetId<morphTargetCount;++morphTargetId)
    {
      vectorSubmesh[submeshId]->setMorphTargetWeight(morphTargetId,m_vectorCurrentWeight[morphTargetId]);
    }
  }
}

 /*****************************************************************************/
/** Returns the number of morph targets this morph target mixer mixes.
  *
  * @return The number of morph targets this morph target mixer mixes.
  *****************************************************************************/

int CalMorphTargetMixer::getMorphTargetCount()
{
  return m_vectorCurrentWeight.size();
}

//****************************************************************************//
