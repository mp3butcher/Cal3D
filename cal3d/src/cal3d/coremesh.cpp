//****************************************************************************//
// coremesh.cpp                                                               //
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

#include "cal3d/coremesh.h"
#include "cal3d/error.h"
#include "cal3d/coresubmesh.h"
#include "cal3d/coresubmorphtarget.h"

 /*****************************************************************************/
/** Constructs the core mesh instance.
  *
  * This function is the default constructor of the core mesh instance.
  *****************************************************************************/

CalCoreMesh::CalCoreMesh()
{
}

 /*****************************************************************************/
/** Destructs the core mesh instance.
  *
  * This function is the destructor of the core mesh instance.
  *****************************************************************************/

CalCoreMesh::~CalCoreMesh()
{
  assert(m_vectorCoreSubmesh.empty());
}

 /*****************************************************************************/
/** Adds a core submesh.
  *
  * This function adds a core submesh to the core mesh instance.
  *
  * @param pCoreSubmesh A pointer to the core submesh that should be added.
  *
  * @return One of the following values:
  *         \li the assigned submesh \b ID of the added core submesh
  *         \li \b -1 if an error happend
  *****************************************************************************/

int CalCoreMesh::addCoreSubmesh(CalCoreSubmesh *pCoreSubmesh)
{
  // get next bone id
  int submeshId;
  submeshId = m_vectorCoreSubmesh.size();

  m_vectorCoreSubmesh.push_back(pCoreSubmesh);

  return submeshId;
}

 /*****************************************************************************/
/** Creates the core mesh instance.
  *
  * This function creates the core mesh instance.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalCoreMesh::create()
{
  return true;
}

 /*****************************************************************************/
/** Destroys the core mesh instance.
  *
  * This function destroys all data stored in the core mesh instance and frees
  * all allocated memory.
  *****************************************************************************/

void CalCoreMesh::destroy()
{
  // destroy all core submeshes
  std::vector<CalCoreSubmesh *>::iterator iteratorCoreSubmesh;
  for(iteratorCoreSubmesh = m_vectorCoreSubmesh.begin(); iteratorCoreSubmesh != m_vectorCoreSubmesh.end(); ++iteratorCoreSubmesh)
  {
    (*iteratorCoreSubmesh)->destroy();
    delete (*iteratorCoreSubmesh);
  }

  m_vectorCoreSubmesh.clear();
}

 /*****************************************************************************/
/** Provides access to a core submesh.
  *
  * This function returns the core submesh with the given ID.
  *
  * @param id The ID of the core submesh that should be returned.
  *
  * @return One of the following values:
  *         \li a pointer to the core submesh
  *         \li \b 0 if an error happend
  *****************************************************************************/

CalCoreSubmesh *CalCoreMesh::getCoreSubmesh(int id)
{
  if((id < 0) || (id >= (int)m_vectorCoreSubmesh.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return 0;
  }

  return m_vectorCoreSubmesh[id];
}

 /*****************************************************************************/
/** Returns the number of core submeshes.
  *
  * This function returns the number of core submeshes in the core mesh
  * instance.
  *
  * @return The number of core submeshes.
  *****************************************************************************/

int CalCoreMesh::getCoreSubmeshCount()
{
  return m_vectorCoreSubmesh.size();
}

 /*****************************************************************************/
/** Returns the core submesh vector.
  *
  * This function returns the vector that contains all core submeshes of the
  * core mesh instance.
  *
  * @return A reference to the core submesh vector.
  *****************************************************************************/

std::vector<CalCoreSubmesh *>& CalCoreMesh::getVectorCoreSubmesh()
{
  return m_vectorCoreSubmesh;
}

 /*****************************************************************************/
/** Adds a core submesh.
  *
  * This function adds a core mesh as a blend target.
  * It adds appropriate CalCoreSubMorphTargets to each of the core sub meshes.
  *
  * @param pCoreMesh A pointer to the core mesh that shoulb become a blend target.
  *
  * @return One of the following values:
  *         \li the assigned morph target \b ID of the added blend target
  *         \li \b -1 if an error happend
  *****************************************************************************/

int CalCoreMesh::addAsMorphTarget(CalCoreMesh *pCoreMesh)
{
  //Check if the numbers of vertices allow a blending
  std::vector<CalCoreSubmesh *>& otherVectorCoreSubmesh = pCoreMesh->getVectorCoreSubmesh();
  if (m_vectorCoreSubmesh.size() != otherVectorCoreSubmesh.size())
  {
    CalError::setLastError(CalError::INTERNAL, __FILE__, __LINE__);
    return -1;
  }
  if (m_vectorCoreSubmesh.size() == 0)
  {
    CalError::setLastError(CalError::INTERNAL, __FILE__, __LINE__);
    return -1;
  }
  std::vector<CalCoreSubmesh *>::iterator iteratorCoreSubmesh = m_vectorCoreSubmesh.begin();
  std::vector<CalCoreSubmesh *>::iterator otherIteratorCoreSubmesh = otherVectorCoreSubmesh.begin();
  int subMorphTargetID = (*iteratorCoreSubmesh)->getCoreSubMorphTargetCount();
  while(iteratorCoreSubmesh != m_vectorCoreSubmesh.end())
  {
    if((*iteratorCoreSubmesh)->getVertexCount() != (*otherIteratorCoreSubmesh)->getVertexCount())
    {
      CalError::setLastError(CalError::INTERNAL, __FILE__, __LINE__);
      return -1;
    }
    ++iteratorCoreSubmesh;
    ++otherIteratorCoreSubmesh;
  }
  //Adding the blend targets to each of the core sub meshes
  iteratorCoreSubmesh = m_vectorCoreSubmesh.begin();
  otherIteratorCoreSubmesh = otherVectorCoreSubmesh.begin();
  while(iteratorCoreSubmesh != m_vectorCoreSubmesh.end())
  {
    int vertexCount = (*otherIteratorCoreSubmesh)->getVertexCount();
    CalCoreSubMorphTarget *pCalCoreSubMorphTarget = new CalCoreSubMorphTarget();
    if(!pCalCoreSubMorphTarget->create()) return -1;
    if(!pCalCoreSubMorphTarget->reserve(vertexCount)) return -1;
    std::vector<CalCoreSubmesh::Vertex>& vectorVertex = (*otherIteratorCoreSubmesh)->getVectorVertex();
    std::vector<CalCoreSubmesh::Vertex>::iterator iteratorVectorVertex = vectorVertex.begin();
    for(int i = 0;i<vertexCount;++i)
    {
      CalCoreSubMorphTarget::BlendVertex blendVertex;
      blendVertex.position = (*iteratorVectorVertex).position;
      blendVertex.normal = (*iteratorVectorVertex).normal;
      if(!pCalCoreSubMorphTarget->setBlendVertex(i,blendVertex)) return -1;
      ++iteratorVectorVertex;
    }
    (*iteratorCoreSubmesh)->addCoreSubMorphTarget(pCalCoreSubMorphTarget);
    ++iteratorCoreSubmesh;
    ++otherIteratorCoreSubmesh;
  }
  return subMorphTargetID;
}

//****************************************************************************//
