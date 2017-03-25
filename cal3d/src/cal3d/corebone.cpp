//****************************************************************************//
// corebone.cpp                                                               //
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
#include "cal3d/corebone.h"
#include "cal3d/coreskeleton.h"
#include "cal3d/coremodel.h"
#include "cal3d/coremesh.h"
#include "cal3d/coresubmesh.h"

using namespace cal3d;
 /*****************************************************************************/
/** Constructs the core bone instance.
  *
  * This function is the default constructor of the core bone instance.
  *****************************************************************************/

CalCoreBone::CalCoreBone(const std::string& name)
  : m_strName(name)
  , m_pCoreSkeleton(0)
  , m_parentId(-1)
  , m_userData(0)
  , m_boundingBoxPrecomputed(false)
{
}

 /*****************************************************************************/
/** remove a child ID.
  *
  * This function remove a core bone ID to the child ID list of the core bone
  * instance.
  *
  * @param childId The ID of the core bone ID that shoud be added to the child
  *                ID list.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happened
  *****************************************************************************/
bool CalCoreBone::removeChildId(int childId)
{
	for (std::list<int>::iterator it = m_listChildId.begin(); it != m_listChildId.end(); it++){
		if (*it == childId){
			m_listChildId.erase(it);
			return true;
		}
    }
	return false;
}

 /*****************************************************************************/
/** Calculates the current state.
  *
  * This function calculates the current state (absolute translation and
  * rotation) of the core bone instance and all its children.
  *****************************************************************************/

void CalCoreBone::calculateState()
{
  if(m_parentId == -1)
  {
    // no parent, this means absolute state == relative state
    m_translationAbsolute = m_translation;
    m_rotationAbsolute = m_rotation;
  }
  else
  {
    // get the parent bone
    CalCoreBone *pParent;
    pParent = m_pCoreSkeleton->getCoreBone(m_parentId);

    // transform relative state with the absolute state of the parent
    m_translationAbsolute = m_translation;
    m_translationAbsolute *= pParent->getRotationAbsolute();
    m_translationAbsolute += pParent->getTranslationAbsolute();

	m_rotationAbsolute = m_rotation;
	m_rotationAbsolute *= pParent->getRotationAbsolute();
  }

  // calculate all child bones
  std::list<int>::iterator iteratorChildId;
  for(iteratorChildId = m_listChildId.begin(); iteratorChildId != m_listChildId.end(); ++iteratorChildId)
  {
    m_pCoreSkeleton->getCoreBone(*iteratorChildId)->calculateState();
  }
}

 /*****************************************************************************/
/** Calculates the bounding box.
  *
  * This function Calculates the bounding box of the core bone instance.
  *
  * @param pCoreModel The coreModel (needed for vertices data.
  *****************************************************************************/

void CalCoreBone::calculateBoundingBox(CalCoreModel * pCoreModel)
{
   int boneId = m_pCoreSkeleton->getCoreBoneId(m_strName);
   bool bBoundsComputed = false;
   
   initBoundingBox();
   
   int meshId;
   for(meshId=0; meshId < pCoreModel->getCoreMeshCount(); ++meshId)
   {
       CalCoreMesh * pCoreMesh = pCoreModel->getCoreMesh(meshId);

       int submeshId;
       for(submeshId=0;submeshId<pCoreMesh->getCoreSubmeshCount();submeshId++)
       {
         CalCoreSubmesh *pCoreSubmesh = pCoreMesh->getCoreSubmesh(submeshId);

         if(pCoreSubmesh->getSpringCount()==0)
         {
            std::vector<CalCoreSubmesh::Vertex>& vectorVertex =  pCoreSubmesh->getVectorVertex();
            for(size_t vertexId=0;vertexId <vectorVertex.size(); ++vertexId)
            {
               for(size_t influenceId=0;influenceId<vectorVertex[vertexId].vectorInfluence.size();++influenceId)
               {
                  CalCoreSubmesh::Influence &influence = vectorVertex[vertexId].vectorInfluence[influenceId];
                  if(influence.boneId == boneId && influence.weight > 0.5f)
                  {
                     const bool updated = updateBoundingBox(vectorVertex[vertexId].position);
                     bBoundsComputed = bBoundsComputed || updated;
                  }
               }
            }   
         }
      }
   }

   // To handle bones with no vertices assigned (do not "optimize" this code away!)
   if(!bBoundsComputed)
   {
      for(int planeId = 0; planeId < 6; ++planeId)
      {
         m_boundingBox.plane[planeId].setPosition(m_translation);
         m_boundingPosition[planeId] = m_translation;
      }
   }

   m_boundingBoxPrecomputed = true;
}

void CalCoreBone::initBoundingBox()
{
   CalQuaternion rot;
   rot=m_rotationBoneSpace;   
  
   rot.invert();
   
   CalVector dir = CalVector(1.0f,0.0f,0.0f);
   dir*=rot;
   m_boundingBox.plane[0].setNormal(dir);

   dir = CalVector(-1.0f,0.0f,0.0f);
   dir*=rot;
   m_boundingBox.plane[1].setNormal(dir);

   dir = CalVector(0.0f,1.0f,0.0f);
   dir*=rot;
   m_boundingBox.plane[2].setNormal(dir);

   dir = CalVector(0.0f,-1.0f,0.0f);
   dir*=rot;
   m_boundingBox.plane[3].setNormal(dir);

   dir = CalVector(0.0f,0.0f,1.0f);
   dir*=rot;
   m_boundingBox.plane[4].setNormal(dir);

   dir = CalVector(0.0f,0.0f,-1.0f);
   dir*=rot;
   m_boundingBox.plane[5].setNormal(dir);
}

 /*****************************************************************************/
/** Updates the bounding box to include the given position.
  *
  * This function Updates the bounding box of the core bone instance to include
  * a given position.
  *
  * @param position The position to be included in the bounding box
  * @return True if the bounding box was changed by this call, false otherwise
  *****************************************************************************/
bool CalCoreBone::updateBoundingBox(const CalVector &position)
{
   bool bBoundsComputed = false;

   for(int planeId = 0; planeId < 6; ++planeId)
   {
      if(m_boundingBox.plane[planeId].eval(position) < 0.0f)
      {
         m_boundingBox.plane[planeId].setPosition(position);
         m_boundingPosition[planeId] = position;
         bBoundsComputed = true;
      }
   }

   return bBoundsComputed;
}

 /*****************************************************************************/
/** Returns the current bounding box.
  *
  * This function returns the current bounding box of the core bone instance.
  *
  * @return bounding box.
  *****************************************************************************/


CalBoundingBox & CalCoreBone::getBoundingBox()
{
   return m_boundingBox;
}

 /*****************************************************************************/
/** Returns the current bounding box.
  *
  * This function returns the current bounding box of the core bone instance.
  *
  * @return bounding box.
  *****************************************************************************/

const CalBoundingBox & CalCoreBone::getBoundingBox() const
{
  return m_boundingBox;
}

void CalCoreBone::getBoundingData(int planeId,CalVector & position) const
{
   position = m_boundingPosition[planeId];
}
 

 /*****************************************************************************/
/** Scale the core bone.
  *
  * This function rescale all the data that are in the core bone instance and
  * in his childs.
  *
  * @param factor A float with the scale factor
  *
  *****************************************************************************/


void CalCoreBone::scale(float factor)
{
	m_translation*=factor;
	m_translationAbsolute*=factor;
	m_translationBoneSpace*=factor;
	
	// calculate all child bones
	std::list<int>::iterator iteratorChildId;
	for(iteratorChildId = m_listChildId.begin(); iteratorChildId != m_listChildId.end(); ++iteratorChildId)
	{
		m_pCoreSkeleton->getCoreBone(*iteratorChildId)->scale(factor);
	}
}

