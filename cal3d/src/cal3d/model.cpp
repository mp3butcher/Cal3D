//****************************************************************************//
// model.cpp                                                                  //
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
#include "cal3d/model.h"
#include "cal3d/skeleton.h"
#include "cal3d/bone.h"
#include "cal3d/mixer.h"
#include "cal3d/renderer.h"
#include "cal3d/coremodel.h"
#include "cal3d/coreskeleton.h"
#include "cal3d/coremesh.h"
#include "cal3d/coresubmesh.h"
#include "cal3d/mesh.h"
#include "cal3d/morphtargetmixer.h"
#include "cal3d/physique.h"
#include "cal3d/springsystem.h"

 /*****************************************************************************/
/** Constructs the model instance.
  *
  * This function is the default constructor of the model instance.
  *****************************************************************************/

CalModel::CalModel()
  :  m_pCoreModel(0), m_pSkeleton(0), m_pMixer(0), m_pPhysique(0), 
     m_pSpringSystem(0), m_pRenderer(0), m_userData(0), m_pMorphTargetMixer(0)
{
}

 /*****************************************************************************/
/** Destructs the model instance.
  *
  * This function is the destructor of the model instance.
  *****************************************************************************/

CalModel::~CalModel()
{
  assert(m_vectorMesh.empty());
}

 /*****************************************************************************/
/** Attachs a mesh.
  *
  * This function attachs a mesh to the model instance.
  *
  * @param coreMeshId The ID of the mesh that should be attached.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalModel::attachMesh(int coreMeshId)
{
  // check if the id is valid
  if((coreMeshId < 0) ||(coreMeshId >= m_pCoreModel->getCoreMeshCount()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  // get the core mesh
  CalCoreMesh *pCoreMesh;
  pCoreMesh = m_pCoreModel->getCoreMesh(coreMeshId);

  // check if the mesh is already attached
  int meshId;
  for(meshId = 0; meshId < (int)m_vectorMesh.size(); ++meshId)
  {
    // check if we found the matching mesh
    if(m_vectorMesh[meshId]->getCoreMesh() == pCoreMesh)
    {
      // mesh is already active -> do nothing
      return true;
    }
  }

  // allocate a new mesh instance
  CalMesh *pMesh;
  pMesh = new CalMesh();
  if(pMesh == 0)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    return false;
  }

  // create the new mesh instance
  if(!pMesh->create(pCoreMesh))
  {
    delete pMesh;
    return false;
  }

  // set model in the mesh instance
  pMesh->setModel(this);

  // insert the new mesh into the active list
  m_vectorMesh.push_back(pMesh);

  return true;
}

 /*****************************************************************************/
/** Creates the model instance.
  *
  * This function creates the model instance based on a core model.
  *
  * @param pCoreModel A pointer to the core model on which this model instance
  *                   should be based on.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalModel::create(CalCoreModel *pCoreModel)
{
  if(pCoreModel == 0)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  m_pCoreModel = pCoreModel;

  // allocate a new skeleton instance
  CalSkeleton *pSkeleton;
  pSkeleton = new CalSkeleton();
  if(pSkeleton == 0)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    return false;
  }

  // create the skeleton from the core skeleton
  if(!pSkeleton->create(pCoreModel->getCoreSkeleton()))
  {
    delete pSkeleton;
    return false;
  }

  m_pSkeleton = pSkeleton;

  // if a mixer was already set (from a previous call to create or
  // a call to setAbstractMixer), re-use it. Otherwise create a
  // CalMixer instance.
  CalAbstractMixer *pMixer;
  if(m_pMixer) {
    pMixer = m_pMixer;
    pMixer->destroy();
  } else
    pMixer = new CalMixer();
  
  if(pMixer == 0)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    m_pSkeleton->destroy();
    delete m_pSkeleton;
    return false;
  }

  // create the mixer from this model
  if(!pMixer->create(this))
  {
    m_pSkeleton->destroy();
    delete m_pSkeleton;
    delete pMixer;
    return false;
  }

  m_pMixer = pMixer;

  // Create the morph target mixer from this model
  m_pMorphTargetMixer = new CalMorphTargetMixer();
  if(!m_pMorphTargetMixer->create(this)) return false;

  // allocate a new physqiue instance
  CalPhysique *pPhysique;
  pPhysique = new CalPhysique();
  if(pPhysique == 0)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    m_pMixer->destroy();
    delete m_pMixer;
    m_pSkeleton->destroy();
    delete m_pSkeleton;
    return false;
  }

  // create the physique from this model
  if(!pPhysique->create(this))
  {
    m_pMixer->destroy();
    delete m_pMixer;
    m_pSkeleton->destroy();
    delete m_pSkeleton;
    delete pPhysique;
    return false;
  }

  m_pPhysique = pPhysique;

  // allocate a new spring system instance
  CalSpringSystem *pSpringSystem;
  pSpringSystem = new CalSpringSystem();
  if(pSpringSystem == 0)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    m_pPhysique->destroy();
    delete m_pPhysique;
    m_pMixer->destroy();
    delete m_pMixer;
    m_pSkeleton->destroy();
    delete m_pSkeleton;
    return false;
  }

  // create the spring system from this model
  if(!pSpringSystem->create(this))
  {
    m_pPhysique->destroy();
    delete m_pPhysique;
    m_pMixer->destroy();
    delete m_pMixer;
    m_pSkeleton->destroy();
    delete m_pSkeleton;
    delete pSpringSystem;
    return false;
  }

  m_pSpringSystem = pSpringSystem;

  // allocate a new renderer instance
  CalRenderer *pRenderer;
  pRenderer = new CalRenderer();
  if(pRenderer == 0)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    m_pSpringSystem->destroy();
    delete m_pSpringSystem;
    m_pPhysique->destroy();
    delete m_pPhysique;
    m_pMixer->destroy();
    delete m_pMixer;
    m_pSkeleton->destroy();
    delete m_pSkeleton;
    return false;
  }

  // create the renderer from this model
  if(!pRenderer->create(this))
  {
    m_pSpringSystem->destroy();
    delete m_pSpringSystem;
    m_pPhysique->destroy();
    delete m_pPhysique;
    m_pMixer->destroy();
    delete m_pMixer;
    m_pSkeleton->destroy();
    delete m_pSkeleton;
    delete pRenderer;
    return false;
  }

  m_pRenderer = pRenderer;

  // initialize the user data
  m_userData = 0;

  return true;
}

 /*****************************************************************************/
/** Destroys the model instance.
  *
  * This function destroys all data stored in the model instance and frees all
  * allocated memory.
  *****************************************************************************/

void CalModel::destroy()
{
  // destroy all active meshes
  int meshId;
  for(meshId = 0; meshId < (int)m_vectorMesh.size(); ++meshId)
  {
    m_vectorMesh[meshId]->destroy();
    delete m_vectorMesh[meshId];
  }
  m_vectorMesh.clear();

  // destroy the renderer instance
  if(m_pRenderer != 0)
  {
    m_pRenderer->destroy();
    delete m_pRenderer;
    m_pRenderer = 0;
  }

  // destroy the spring system instance
  if(m_pSpringSystem != 0)
  {
    m_pSpringSystem->destroy();
    delete m_pSpringSystem;
    m_pSpringSystem = 0;
  }

  // destroy the physique instance
  if(m_pPhysique != 0)
  {
    m_pPhysique->destroy();
    delete m_pPhysique;
    m_pPhysique = 0;
  }

  // destroy the mixer instance
  if(m_pMixer != 0)
  {
    m_pMixer->destroy();
    delete m_pMixer;
    m_pMixer = 0;
  }
  
  // destroy the morph target mixer instance
  if(m_pMorphTargetMixer != 0)
  {
    m_pMorphTargetMixer->destroy();
    delete m_pMorphTargetMixer;
    m_pMorphTargetMixer = 0;
  }
  
  // destroy the skeleton instance
  if(m_pSkeleton != 0)
  {
    m_pSkeleton->destroy();
    delete m_pSkeleton;
    m_pSkeleton = 0;
  }

  m_pCoreModel = 0;
}

 /*****************************************************************************/
/** Detaches a mesh.
  *
  * This function detaches a mesh from the model instance.
  *
  * @param coreMeshId The ID of the mesh that should be detached.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalModel::detachMesh(int coreMeshId)
{
  // check if the id is valid
  if((coreMeshId < 0) ||(coreMeshId >= m_pCoreModel->getCoreMeshCount()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  // get the core mesh
  CalCoreMesh *pCoreMesh;
  pCoreMesh = m_pCoreModel->getCoreMesh(coreMeshId);

  // find the mesh for the given id
  std::vector<CalMesh *>::iterator iteratorMesh;
  for(iteratorMesh = m_vectorMesh.begin(); iteratorMesh != m_vectorMesh.end(); ++iteratorMesh)
  {
    // get the mesh
    CalMesh *pMesh;
    pMesh = *iteratorMesh;

    // check if we found the matching mesh
    if(pMesh->getCoreMesh() == pCoreMesh)
    {
      // destroy the mesh
      pMesh->destroy();
      delete pMesh;

      // erase the mesh out of the active mesh list
      m_vectorMesh.erase(iteratorMesh);

      return true;
    }
  }

  return false;
}

 /*****************************************************************************/
/** Provides access to the core model.
  *
  * This function returns the core model on which this model instance is based
  * on.
  *
  * @return One of the following values:
  *         \li a pointer to the core model
  *         \li \b 0 if an error happend
  *****************************************************************************/

CalCoreModel *CalModel::getCoreModel()
{
  return m_pCoreModel;
}

 /*****************************************************************************/
/** Provides access to an attached mesh.
  *
  * This function returns the attached mesh with the given core mesh ID.
  *
  * @param coreMeshId The core mesh ID of the mesh that should be returned.
  *
  * @return One of the following values:
  *         \li a pointer to the mesh
  *         \li \b 0 if an error happend
  *****************************************************************************/

CalMesh *CalModel::getMesh(int coreMeshId)
{
  // check if the id is valid
  if((coreMeshId < 0) ||(coreMeshId >= m_pCoreModel->getCoreMeshCount()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return 0;
  }

  // get the core mesh
  CalCoreMesh *pCoreMesh;
  pCoreMesh = m_pCoreModel->getCoreMesh(coreMeshId);

  // search the mesh
  int meshId;
  for(meshId = 0; meshId < (int)m_vectorMesh.size(); ++meshId)
  {
    // check if we found the matching mesh
    if(m_vectorMesh[meshId]->getCoreMesh() == pCoreMesh)
    {
      return m_vectorMesh[meshId];
    }
  }

  return 0;
}

/*****************************************************************************/
/** Returns the mixer.
 *
 * If a mixer that is not an instance of CalMixer was set with the
 * CalModel::setAbstractMixer method, an INVALID_MIXER_TYPE error (see
 * CalError) is set and 0 is returned.
 *
 * @return \li a pointer to the mixer
 *         \li \b 0 if an error happend
 *****************************************************************************/

CalMixer *CalModel::getMixer()
{
  if(m_pMixer == 0)
    return 0;

  if(m_pMixer->isDefaultMixer() == false) {
    CalError::setLastError(CalError::INVALID_MIXER_TYPE, __FILE__, __LINE__);
    return 0;
  } else {
    return (CalMixer*)(m_pMixer);
  }
}

/*****************************************************************************/
/** Returns the mixer. 
 *
 * @return \li a pointer to the mixer
 *         \li \b 0 if no mixer was set
 *****************************************************************************/

CalAbstractMixer *CalModel::getAbstractMixer()
{
  return m_pMixer;
}

/*****************************************************************************/
/** Sets the mixer to a CalAbstractMixer subclass instance.
 *
 * If a mixer was already set (with CalModel::setAbstractMixer or
 * because the CalModel::create method created a CalMixer instance),
 * its \b destroy method is called. The existing mixer is not
 * deallocated, it is the responsibility of the caller to call the
 * getAbstractMixer method and deallocate the returned instance if
 * appropriate.
 *
 * \b pMixer will be deallocated by cal3d if and only if the
 * CalModel::destroy function is called.
 *
 * The \b create method of pMixer is called.
 *
 * pMixer may be null. After setting a null pointer, the caller MUST
 * call CalModel::create or CalModel::setAbstractMixer with a non-null
 * pointer before any other method is called.
 *
 * @param pMixer is a pointer to a CalAbstractMixer subclass instance.
 *
 *****************************************************************************/

void CalModel::setAbstractMixer(CalAbstractMixer* pMixer)
{
  if(m_pMixer != 0)
    m_pMixer->destroy();

  m_pMixer = pMixer;

  if(m_pMixer != 0)
    m_pMixer->create(this);
}

/*****************************************************************************/
/** Provides access to the morph target mixer.
  *
  * This function returns the morph target mixer.
  *
  * @return One of the following values:
  *         \li a pointer to the morph target mixer
  *         \li \b 0 if an error happend
  *****************************************************************************/

CalMorphTargetMixer *CalModel::getMorphTargetMixer()
{
  return m_pMorphTargetMixer;
}

 /*****************************************************************************/
/** Provides access to the physique.
  *
  * This function returns the physique.
  *
  * @return One of the following values:
  *         \li a pointer to the physique
  *         \li \b 0 if an error happend
  *****************************************************************************/

CalPhysique *CalModel::getPhysique()
{
  return m_pPhysique;
}

 /*****************************************************************************/
/** Provides access to the renderer.
  *
  * This function returns the renderer.
  *
  * @return One of the following values:
  *         \li a pointer to the renderer
  *         \li \b 0 if an error happend
  *****************************************************************************/

CalRenderer *CalModel::getRenderer() const
{
  return m_pRenderer;
}

 /*****************************************************************************/
/** Provides access to the skeleton.
  *
  * This function returns the skeleton.
  *
  * @return One of the following values:
  *         \li a pointer to the skeleton
  *         \li \b 0 if an error happend
  *****************************************************************************/

CalSkeleton *CalModel::getSkeleton()
{
  return m_pSkeleton;
}

 /*****************************************************************************/
/** Provides access to the spring system.
  *
  * This function returns the spring system.
  *
  * @return One of the following values:
  *         \li a pointer to the spring system
  *         \li \b 0 if an error happend
  *****************************************************************************/

CalSpringSystem *CalModel::getSpringSystem()
{
  return m_pSpringSystem;
}

 /*****************************************************************************/
/** Returns the global bounding box of the model.
  *
  * This function returns the global bounding box of the model.
  *
  * @param precision : indicate if the function need to compute a 
  *        correct bounding box
  *
  * @return bounding box.
  *****************************************************************************/


CalBoundingBox & CalModel::getBoundingBox(bool precision)
{
	CalVector v;
	v = CalVector(1.0f,0.0f,0.0f);	
	m_boundingBox.plane[0].setNormal(v);
	v = CalVector(-1.0f,0.0f,0.0f);	
	m_boundingBox.plane[1].setNormal(v);
	v = CalVector(0.0f,1.0f,0.0f);	
	m_boundingBox.plane[2].setNormal(v);
	v = CalVector(0.0f,-1.0f,0.0f);	
	m_boundingBox.plane[3].setNormal(v);
	v = CalVector(0.0f,0.0f,1.0f);	
	m_boundingBox.plane[4].setNormal(v);
	v = CalVector(0.0f,0.0f,-1.0f);	
	m_boundingBox.plane[5].setNormal(v);

	if(precision)
		m_pSkeleton->calculateBoundingBoxes();

	
	std::vector<CalBone *> & vectorBone =  m_pSkeleton->getVectorBone();
		
	std::vector<CalBone *>::iterator iteratorBone;
	for(iteratorBone = vectorBone.begin(); iteratorBone != vectorBone.end(); ++iteratorBone)
	{
		// If it's just an approximation that are needed then
		// we just compute the bounding box from the skeleton

		if(!precision || !(*iteratorBone)->getCoreBone()->isBoundingBoxPrecomputed())
		{
			
			CalVector translation = (*iteratorBone)->getTranslationAbsolute();
			
			int planeId;
			for(planeId = 0; planeId < 6; ++planeId)
			{
				if(m_boundingBox.plane[planeId].eval(translation) < 0.0f)
				{
					m_boundingBox.plane[planeId].setPosition(translation);
				}
			}
		}
		else
		{
			CalBoundingBox localBoundingBox = (*iteratorBone)->getBoundingBox();
			CalVector points[8];
			localBoundingBox.computePoints(points);
			
			for(int i=0; i < 8; i++)
			{				
				int planeId;
				for(planeId = 0; planeId < 6; ++planeId)
				{
					if(m_boundingBox.plane[planeId].eval(points[i]) < 0.0f)
					{
						m_boundingBox.plane[planeId].setPosition(points[i]);
					}
				}
			}				
		}
	}
	
	return m_boundingBox;
}

 /*****************************************************************************/
/** Provides access to the user data.
  *
  * This function returns the user data stored in the model instance.
  *
  * @return The user data stored in the model instance.
  *****************************************************************************/

Cal::UserData CalModel::getUserData()
{
  return m_userData;
}

 /*****************************************************************************/
/** Returns the mesh vector.
  *
  * This function returns the vector that contains all attached meshes of the
  * model instance.
  *
  * @return A reference to the mesh vector.
  *****************************************************************************/

std::vector<CalMesh *>& CalModel::getVectorMesh()
{
  return m_vectorMesh;
}

 /*****************************************************************************/
/** Sets the LOD level.
  *
  * This function sets the LOD level of all attached meshes.
  *
  * @param lodLevel The LOD level in the range [0.0, 1.0].
  *****************************************************************************/

void CalModel::setLodLevel(float lodLevel)
{
  // set the lod level in all meshes
  std::vector<CalMesh *>::iterator iteratorMesh;
  for(iteratorMesh = m_vectorMesh.begin(); iteratorMesh != m_vectorMesh.end(); ++iteratorMesh)
  {
    // set the lod level in the mesh
    (*iteratorMesh)->setLodLevel(lodLevel);
  }
}

 /*****************************************************************************/
/** Sets the material set.
  *
  * This function sets the material set of all attached meshes.
  *
  * @param setId The ID of the material set.
  *****************************************************************************/

void CalModel::setMaterialSet(int setId)
{
  // set the lod level in all meshes
  std::vector<CalMesh *>::iterator iteratorMesh;
  for(iteratorMesh = m_vectorMesh.begin(); iteratorMesh != m_vectorMesh.end(); ++iteratorMesh)
  {
    // set the material set in the mesh
    (*iteratorMesh)->setMaterialSet(setId);
  }
}

 /*****************************************************************************/
/** Stores user data.
  *
  * This function stores user data in the model instance.
  *
  * @param userData The user data that should be stored.
  *****************************************************************************/

void CalModel::setUserData(Cal::UserData userData)
{
  m_userData = userData;
}

 /*****************************************************************************/
/** Updates the model instance.
  *
  * This function updates the model instance for a given amount of time.
  *
  * @param deltaTime The elapsed time in seconds since the last update.
  *****************************************************************************/

void CalModel::update(float deltaTime)
{
  m_pMixer->updateAnimation(deltaTime);
  m_pMixer->updateSkeleton();
  std::vector<CalMesh *>::iterator iteratorVectorMesh = m_vectorMesh.begin();
  // m_pMorpher->update(...);
  m_pMorphTargetMixer->update(deltaTime);
  m_pPhysique->update();
  m_pSpringSystem->update(deltaTime);
}

/*****************************************************************************/
/** Disable internal data (and thus springs system)
  *
  *****************************************************************************/

void CalModel::disableInternalData()
{
  // Disable internal data in all meshes
  std::vector<CalMesh *>::iterator iteratorMesh;
  for(iteratorMesh = m_vectorMesh.begin(); iteratorMesh != m_vectorMesh.end(); ++iteratorMesh)
  {
    // Disable internal data in the mesh
    (*iteratorMesh)->disableInternalData();
  }
}

//****************************************************************************//
