//----------------------------------------------------------------------------//
// MilkBoneNode.cpp                                                           //
// Copyright (C) 2002 Bruno 'Beosil' Heidelberger                             //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#include "StdAfx.h"
#include "Exporter.h"
#include "MilkBoneNode.h"
#include "MilkInterface.h"

//----------------------------------------------------------------------------//
// Constructors                                                               //
//----------------------------------------------------------------------------//

CMilkBoneNode::CMilkBoneNode()
{
  m_pIBone = 0;
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CMilkBoneNode::~CMilkBoneNode()
{
}

//----------------------------------------------------------------------------//
// Convert Milkshape euler angles to a quaternion                             //
//----------------------------------------------------------------------------//

CalQuaternion CMilkBoneNode::ConvertToQuaternion(msVec3 euler)
{
  // calculate the components
  float cosx, cosy, cosz, sinx, siny, sinz, cosc, coss, sinc, sins;
  cosx = cos(euler[0] * 0.5f);
  cosy = cos(euler[1] * 0.5f);
  cosz = cos(euler[2] * 0.5f);
  sinx = sin(euler[0] * 0.5f);
  siny = sin(euler[1] * 0.5f);
  sinz = sin(euler[2] * 0.5f);
  cosc = cosx * cosz;
  coss = cosx * sinz;
  sinc = sinx * cosz;
  sins = sinx * sinz;

  // build the quaternion
  CalQuaternion quaternion;
  quaternion[0] = (cosy * sinc) - (siny * coss);
  quaternion[1] = (cosy * sins) + (siny * cosc);
  quaternion[2] = (cosy * coss) - (siny * sinc);
  // why do we have to negate this???
  // hmmm ... "milkshape style"... well it works this way =)
  quaternion[3] = -((cosy * cosc) + (siny * sins));

  return quaternion;
}

//----------------------------------------------------------------------------//
// Create a milk bone node instance                                           //
//----------------------------------------------------------------------------//

bool CMilkBoneNode::Create(msBone *pIBone)
{
	// check if the internal bone is valid
	if(pIBone == 0)
	{
		theExporter.SetLastError("Invalid handle.", __FILE__, __LINE__);
		return false;
	}

	m_pIBone = pIBone;

  // get the internal model
  msModel *pIModel;
  pIModel = dynamic_cast<CMilkInterface *>(theExporter.GetInterface())->GetIModel();
  if(pIModel == 0)
  {
		theExporter.SetLastError("Invalid handle.", __FILE__, __LINE__);
		return false;
  }

  // get the name of this bone
  std::string strName;
  strName = pIBone->szName;

    // search for children of this bone and create them
  int boneId;
  for(boneId = 0; boneId < msModel_GetBoneCount(pIModel); boneId++)
  {
    // get the name of the bone
    std::string strParentName;
    strParentName = msModel_GetBoneAt(pIModel, boneId)->szParentName;

    // check if it is a child
    if(strName == strParentName)
    {
      // add the child id to the child id vector
      m_vectorChildId.push_back(boneId);
    }
  }

	return true;
}

//----------------------------------------------------------------------------//
// Get the child node for a given child node id                               //
//----------------------------------------------------------------------------//

CBaseNode *CMilkBoneNode::GetChild(int childId)
{
	// check if the internal bone is valid
	if(m_pIBone == 0) return 0;

	// check if the given node id is valid
	if((childId < 0) || (childId >= (int)m_vectorChildId.size()))
	{
		theExporter.SetLastError("Invalid handle.", __FILE__, __LINE__);
		return 0;
	}

  // get the internal model
  msModel *pIModel;
  pIModel = dynamic_cast<CMilkInterface *>(theExporter.GetInterface())->GetIModel();
  if(pIModel == 0)
  {
		theExporter.SetLastError("Invalid handle.", __FILE__, __LINE__);
		return 0;
  }

	// allocate a new milk bone node instance
	CMilkBoneNode *pNode;
	pNode = new CMilkBoneNode();
	if(pNode == 0)
	{
		theExporter.SetLastError("Memory allocation failed.", __FILE__, __LINE__);
		return false;
	}

  // create the milk bone node
	if(!pNode->Create(msModel_GetBoneAt(pIModel, m_vectorChildId[childId])))
	{
		delete pNode;
		return 0;
	}

	return pNode;
}

//----------------------------------------------------------------------------//
// Get the number of children of the node                                     //
//----------------------------------------------------------------------------//

int CMilkBoneNode::GetChildCount()
{
	// check if the internal bone is valid
	if(m_pIBone == 0) return 0;

  return m_vectorChildId.size();
}

//----------------------------------------------------------------------------//
// Get the internal bone of the node                                          //
//----------------------------------------------------------------------------//

msBone *CMilkBoneNode::GetIBone()
{
	return m_pIBone;
}

//----------------------------------------------------------------------------//
// Get the name of the node                                                   //
//----------------------------------------------------------------------------//

std::string CMilkBoneNode::GetName()
{
	// check if the internal bone is valid
	if(m_pIBone == 0) return "<void>";

  return m_pIBone->szName;
}

//----------------------------------------------------------------------------//
// Get the relative translation of the node to its parent                     //
//----------------------------------------------------------------------------//

CalVector CMilkBoneNode::GetRelativeTranslation(float time)
{
  // get the position
  msVec3 position;
  msBone_GetPosition(m_pIBone, position);

  CalVector initial_translation;
  initial_translation[0] = position[0];
  initial_translation[1] = position[1];
  initial_translation[2] = position[2];


  // return if the initial state is requested or if there are no keyframes
  if((time < 0.0f) || (msBone_GetPositionKeyCount(m_pIBone) == 0))
  {
    return initial_translation;
  }


  // calculate the real frame time
  // REMEMBER: milkshape starts at 1.0!
  float frameTime;
  frameTime = 1.0f + time * (float)theExporter.GetInterface()->GetFps();

  // find the keyframe just before the requested time
  msPositionKey *pKeyBefore;

  pKeyBefore = msBone_GetPositionKeyAt(m_pIBone,
                                       msBone_GetPositionKeyCount(m_pIBone)-1);


  int keyId;
  for(keyId = 0; keyId < msBone_GetPositionKeyCount(m_pIBone); keyId++)
  {
    // get the keyframe
    msPositionKey *pKey;
    pKey = msBone_GetPositionKeyAt(m_pIBone, keyId);

    // stop if we are over the requested time
    if(pKey->fTime > frameTime) break;

    pKeyBefore = pKey;
  }



  // get the keyframe just after the requested time
  msPositionKey *pKeyAfter;
  pKeyAfter = msBone_GetPositionKeyAt(m_pIBone, 0);

  if(keyId < msBone_GetPositionKeyCount(m_pIBone))
  {
    pKeyAfter = msBone_GetPositionKeyAt(m_pIBone, keyId);
  }


  // calculate the "just before" translation component
  CalVector translationBefore;
  if(pKeyBefore != 0)
  {
    //translationBefore = ConvertToVector(pKeyBefore->Translation);

    translationBefore[0] = pKeyBefore->Position[0];
    translationBefore[1] = pKeyBefore->Position[1];
    translationBefore[2] = pKeyBefore->Position[2];

    // return if there is no key after this one
    if(pKeyAfter == 0)
    {
      return initial_translation + translationBefore;
    }
  }


  // calculate the "just after" translation component
  CalVector translationAfter;
  if(pKeyAfter != 0)
  {
    //translationAfter = ConvertToVector(pKeyAfter->Translation);
    translationAfter[0] = pKeyAfter->Position[0];
    translationAfter[1] = pKeyAfter->Position[1];
    translationAfter[2] = pKeyAfter->Position[2];

    // return if there is no key before this one
    if(pKeyBefore == 0) return initial_translation + translationAfter;
  }


  // return if both keys are actually the same
  if(pKeyBefore == pKeyAfter)
  {
    return initial_translation + translationAfter;
  }

  // calculate the blending factor
  float factor;
  factor = (frameTime - pKeyBefore->fTime) / (pKeyAfter->fTime - pKeyBefore->fTime);


  // Have to blend the two translations.
  translationBefore = translationBefore + (translationAfter - translationBefore)*factor;

  return initial_translation + translationBefore;
} 

//----------------------------------------------------------------------------//
// Get the relative rotation of the node to its parent                        //
//----------------------------------------------------------------------------//

CalQuaternion CMilkBoneNode::GetRelativeRotation(float time)
{
	// get the initial rotation component
  msVec3 orientation;
  msBone_GetRotation(m_pIBone, orientation);

	// calculate the initial rotation component
  CalQuaternion initialRotation;
  initialRotation = ConvertToQuaternion(orientation);

  // return if the initial state is requested or if there are no keyframes
  if((time < 0.0f) || (msBone_GetRotationKeyCount(m_pIBone) == 0)) return initialRotation;

  // calculate the real frame time
  // REMEMBER: milkshape starts at 1.0!
  float frameTime;
  frameTime = 1.0f + time * (float)theExporter.GetInterface()->GetFps();

  // find the keyframe just before the requested time
  msRotationKey *pKeyBefore;
  pKeyBefore = msBone_GetRotationKeyAt(m_pIBone, msBone_GetRotationKeyCount(m_pIBone) - 1);

  int keyId;
  for(keyId = 0; keyId < msBone_GetRotationKeyCount(m_pIBone); keyId++)
  {
    // get the keyframe
    msRotationKey *pKey;
    pKey = msBone_GetRotationKeyAt(m_pIBone, keyId);

    // stop if we are over the requested time
    if(pKey->fTime > frameTime) break;

    pKeyBefore = pKey;
  }

  // get the keyframe just after the requested time
  msRotationKey *pKeyAfter;
  pKeyAfter = msBone_GetRotationKeyAt(m_pIBone, 0);

  if(keyId < msBone_GetRotationKeyCount(m_pIBone))
  {
    pKeyAfter = msBone_GetRotationKeyAt(m_pIBone, keyId);
  }

  // calculate the "just before" rotation component
  CalQuaternion rotationBefore;
  if(pKeyBefore != 0)
  {
    rotationBefore = ConvertToQuaternion(pKeyBefore->Rotation);

    // return if there is no key after this one
    if(pKeyAfter == 0) return initialRotation * rotationBefore;
  }

	// calculate the "just after" rotation component
  CalQuaternion rotationAfter;
  if(pKeyAfter != 0)
  {
    rotationAfter = ConvertToQuaternion(pKeyAfter->Rotation);

    // return if there is no key before this one
    if(pKeyBefore == 0) return initialRotation * rotationAfter;
  }

  // return if both keys are actually the same
  if(pKeyBefore == pKeyAfter) return initialRotation * rotationAfter;

  // calculate the blending factor
  float factor;
  factor = (frameTime - pKeyBefore->fTime) / (pKeyAfter->fTime - pKeyBefore->fTime);

  // blend the two rotation components
  rotationBefore.blend(factor, rotationAfter);

  return initialRotation * rotationBefore;
}

//----------------------------------------------------------------------------//
// Get the type of the node                                                   //
//----------------------------------------------------------------------------//

CBaseNode::Type CMilkBoneNode::GetType()
{
  return TYPE_BONE;
}

//----------------------------------------------------------------------------//

bool CMilkBoneNode::operator==(const CBaseNode& rhs) const
{
    const CMilkBoneNode* milkbone = dynamic_cast<const CMilkBoneNode*>(&rhs);
    if (!milkbone)
        return false;

    return m_pIBone == milkbone->m_pIBone;
}
