//****************************************************************************//
// mesh.cpp                                                                   //
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

#include "mesh.h"
#include "error.h"
#include "coremesh.h"
#include "coresubmesh.h"
#include "submesh.h"
#include "coremodel.h"
#include "model.h"

 /*****************************************************************************/
/** Constructs the mesh instance.
  *
  * This function is the default constructor of the mesh instance.
  *****************************************************************************/

CalMesh::CalMesh()
{
  m_pModel = 0;
  m_pCoreMesh = 0;
}

 /*****************************************************************************/
/** Destructs the mesh instance.
  *
  * This function is the destructor of the mesh instance.
  *****************************************************************************/

CalMesh::~CalMesh()
{
  assert(m_vectorSubmesh.empty());
}

 /*****************************************************************************/
/** Creates the mesh instance.
  *
  * This function creates the mesh instance based on a core mesh.
  *
  * @param pCoreMesh A pointer to the core mesh on which this mesh instance
  *                  should be based on.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalMesh::create(CalCoreMesh *pCoreMesh)
{
  if(pCoreMesh == 0)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  m_pCoreMesh = pCoreMesh;

  // clone the mesh structure of the core mesh
  std::vector<CalCoreSubmesh *>& vectorCoreSubmesh = pCoreMesh->getVectorCoreSubmesh();

  // get the number of submeshes
  int submeshCount;
  submeshCount = vectorCoreSubmesh.size();

  // reserve space in the bone vector
  m_vectorSubmesh.reserve(submeshCount);

  // clone every core submesh
  int submeshId;
  for(submeshId = 0; submeshId < submeshCount; submeshId++)
  {
    CalSubmesh *pSubmesh;
    pSubmesh = new CalSubmesh();
    if(pSubmesh == 0)
    {
      CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
      return false;
    }

    // create a submesh for every core submesh
    if(!pSubmesh->create(vectorCoreSubmesh[submeshId]))
    {
      delete pSubmesh;
      return false;
    }

    // insert submesh into submesh vector
    m_vectorSubmesh.push_back(pSubmesh);
  }

  return true;
}

 /*****************************************************************************/
/** Destroys the mesh instance.
  *
  * This function destroys all data stored in the mesh instance and frees all
  * allocated memory.
  *****************************************************************************/

void CalMesh::destroy()
{
  // destroy all submeshes
  std::vector<CalSubmesh *>::iterator iteratorSubmesh;
  for(iteratorSubmesh = m_vectorSubmesh.begin(); iteratorSubmesh != m_vectorSubmesh.end(); ++iteratorSubmesh)
  {
    (*iteratorSubmesh)->destroy();
    delete (*iteratorSubmesh);
  }
  m_vectorSubmesh.clear();

  m_pCoreMesh = 0;
}

 /*****************************************************************************/
/** Provides access to the core mesh.
  *
  * This function returns the core mesh on which this mesh instance is based on.
  *
  * @return One of the following values:
  *         \li a pointer to the core mesh
  *         \li \b 0 if an error happend
  *****************************************************************************/

CalCoreMesh *CalMesh::getCoreMesh()
{
  return m_pCoreMesh;
}

 /*****************************************************************************/
/** Provides access to a submesh.
  *
  * This function returns the submesh with the given ID.
  *
  * @param id The ID of the submesh that should be returned.
  *
  * @return One of the following values:
  *         \li a pointer to the submesh
  *         \li \b 0 if an error happend
  *****************************************************************************/

CalSubmesh *CalMesh::getSubmesh(int id)
{
  if((id < 0) || (id >= (int)m_vectorSubmesh.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return 0;
  }

  return m_vectorSubmesh[id];
}

 /*****************************************************************************/
/** Returns the number of submeshes.
  *
  * This function returns the number of submeshes in the mesh instance.
  *
  * @return The number of submeshes.
  *****************************************************************************/

int CalMesh::getSubmeshCount()
{
  return m_vectorSubmesh.size();
}

 /*****************************************************************************/
/** Returns the submesh vector.
  *
  * This function returns the vector that contains all submeshes of the mesh
  * instance.
  *
  * @return A reference to the submesh vector.
  *****************************************************************************/

std::vector<CalSubmesh *>& CalMesh::getVectorSubmesh()
{
  return m_vectorSubmesh;
}

 /*****************************************************************************/
/** Sets the LOD level.
  *
  * This function sets the LOD level of the mesh instance.
  *
  * @param lodLevel The LOD level in the range [0.0, 1.0].
  *****************************************************************************/

void CalMesh::setLodLevel(float lodLevel)
{
  // change lod level of every submesh
  int submeshId;
  for(submeshId = 0; submeshId < (int)m_vectorSubmesh.size(); submeshId++)
  {
    // set the lod level in the submesh
    m_vectorSubmesh[submeshId]->setLodLevel(lodLevel);
  }
}

 /*****************************************************************************/
/** Sets the material set.
  *
  * This function sets the material set of the mesh instance.
  *
  * @param setId The ID of the material set.
  *****************************************************************************/

void CalMesh::setMaterialSet(int setId)
{
  // change material of every submesh
  int submeshId;
  for(submeshId = 0; submeshId < (int)m_vectorSubmesh.size(); submeshId++)
  {
    // get the core material thread id of the submesh
    int coreMaterialThreadId;
    coreMaterialThreadId = m_vectorSubmesh[submeshId]->getCoreSubmesh()->getCoreMaterialThreadId();

    // get the core material id for the given set id in the material thread
    int coreMaterialId;
    coreMaterialId = m_pModel->getCoreModel()->getCoreMaterialId(coreMaterialThreadId, setId);

    // set the new core material id in the submesh
    m_vectorSubmesh[submeshId]->setCoreMaterialId(coreMaterialId);
  }
}

 /*****************************************************************************/
/** Sets the model.
  *
  * This function sets the model to which the mesh instance is attached to.
  *
  * @param pModel The model to which the mesh instance should be attached to.
  *****************************************************************************/

void CalMesh::setModel(CalModel *pModel)
{
  m_pModel = pModel;
}

//****************************************************************************//
