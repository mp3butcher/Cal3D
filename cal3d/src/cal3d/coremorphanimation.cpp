//****************************************************************************//
// coremorphanimation.cpp                                                     //
// Copyright (C) 2003 Steven Geens                                            //
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

#include "cal3d/coremorphanimation.h"

 /*****************************************************************************/
/** Constructs the core morph animation instance.
  *
  * This function is the default constructor of the core morph animation 
  * instance.
  *****************************************************************************/

CalCoreMorphAnimation::CalCoreMorphAnimation()
{
}

 /*****************************************************************************/
/** Destructs the core morph animation instance.
  *
  * This function is the destructor of the core morph animation instance.
  *****************************************************************************/

CalCoreMorphAnimation::~CalCoreMorphAnimation()
{
  assert(m_vectorCoreMeshID.empty());
  assert(m_vectorMorphTargetID.empty());
}

 /*****************************************************************************/
/** Adds a core mesh ID and a morph target ID of that core mesh.
  *
  * @param coreMeshID A core mesh ID that should be added.
  * @param morphTargetID A morph target ID that should be added.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalCoreMorphAnimation::addMorphTarget(int coreMeshID,int morphTargetID)
{
  m_vectorCoreMeshID.push_back(coreMeshID);
  m_vectorMorphTargetID.push_back(morphTargetID);

  return true;
}

 /*****************************************************************************/
/** Creates the core morph animation instance.
  *
  * This function creates the core morph animation instance.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalCoreMorphAnimation::create()
{
  return true;
}

 /*****************************************************************************/
/** Destroys the core morph animation instance.
  *
  * This function destroys all data stored in the core morph animation instance 
  * and frees all allocated memory.
  *****************************************************************************/

void CalCoreMorphAnimation::destroy()
{
  m_vectorCoreMeshID.clear();
  m_vectorMorphTargetID.clear();
}

 /*****************************************************************************/
/** Returns the core mesh ID list.
  *
  * This function returns the list that contains all core mesh IDs of the core
  * morph animation instance.
  *
  * @return A reference to the core mesh ID list.
  *****************************************************************************/

std::vector<int>& CalCoreMorphAnimation::getVectorCoreMeshID()
{
  return m_vectorCoreMeshID;
}

 /*****************************************************************************/
/** Returns the morph target ID list.
  *
  * This function returns the list that contains all morph target  IDs of the core
  * morph animation instance.
  *
  * @return A reference to the morph target ID list.
  *****************************************************************************/

std::vector<int>& CalCoreMorphAnimation::getVectorMorphTargetID()
{
  return m_vectorMorphTargetID;
}

//****************************************************************************//
