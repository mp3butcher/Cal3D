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

#include "error.h"
#include "model.h"
#include "skeleton.h"
#include "bone.h"
#include "mixer.h"
#include "renderer.h"
#include "coremodel.h"
#include "coreskeleton.h"
#include "coremesh.h"
#include "coresubmesh.h"
#include "mesh.h"
#include "physique.h"
#include "springsystem.h"

 /*****************************************************************************/
/** Constructs the model instance.
  *
  * This function is the default constructor of the model instance.
  *****************************************************************************/

CalModel::CalModel()
{
  m_pCoreModel = 0;
  m_pSkeleton = 0;
  m_pMixer = 0;
  m_pPhysique = 0;
  m_pSpringSystem = 0;
  m_pRenderer = 0;
  m_userData = 0;
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

  // allocate a new mixer instance
  CalMixer *pMixer;
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
/** Provides access to the mixer.
  *
  * This function returns the mixer.
  *
  * @return One of the following values:
  *         \li a pointer to the mixer
  *         \li \b 0 if an error happend
  *****************************************************************************/

CalMixer *CalModel::getMixer()
{
  return m_pMixer;
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

CalRenderer *CalModel::getRenderer()
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
  // m_pMorpher->update(...);
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
