//****************************************************************************//
// physique.cpp                                                               //
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
#include "physique.h"
#include "coremodel.h"
#include "model.h"
#include "mesh.h"
#include "submesh.h"
#include "skeleton.h"
#include "bone.h"
#include "coresubmesh.h"

 /*****************************************************************************/
/** Constructs the physique instance.
  *
  * This function is the default constructor of the physique instance.
  *****************************************************************************/

CalPhysique::CalPhysique()
{
  m_pModel = 0;
}

 /*****************************************************************************/
/** Destructs the physique instance.
  *
  * This function is the destructor of the physique instance.
  *****************************************************************************/

CalPhysique::~CalPhysique()
{
}

 /*****************************************************************************/
/** Calculates the transformed vertex data.
  *
  * This function calculates and returns the transformed vertex data of a
  * specific submesh.
  *
  * @param pSubmesh A pointer to the submesh from which the vertex data should
  *                 be calculated and returned.
  * @param pVertexBuffer A pointer to the user-provided buffer where the vertex
  *                      data is written to.
  *
  * @return The number of vertices written to the buffer.
  *****************************************************************************/

int CalPhysique::calculateVertices(CalSubmesh *pSubmesh, float *pVertexBuffer)
{
  // get bone vector of the skeleton
  std::vector<CalBone *>& vectorBone = m_pModel->getSkeleton()->getVectorBone();

  // get vertex vector of the core submesh
  std::vector<CalCoreSubmesh::Vertex>& vectorVertex = pSubmesh->getCoreSubmesh()->getVectorVertex();

  // get physical property vector of the core submesh
  std::vector<CalCoreSubmesh::PhysicalProperty>& vectorPhysicalProperty = pSubmesh->getCoreSubmesh()->getVectorPhysicalProperty();

  // get the number of vertices
  int vertexCount;
  vertexCount = pSubmesh->getVertexCount();

  // calculate all submesh vertices
  int vertexId;
  for(vertexId = 0; vertexId < vertexCount; vertexId++)
  {
    // get the vertex
    CalCoreSubmesh::Vertex& vertex = vectorVertex[vertexId];

    // initialize vertex
    float x, y, z;
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;

    // blend together all vertex influences
    int influenceId;
	int influenceCount=(int)vertex.vectorInfluence.size();
    for(influenceId = 0; influenceId < influenceCount; influenceId++)
    {
      // get the influence
      CalCoreSubmesh::Influence& influence = vertex.vectorInfluence[influenceId];

      // get the bone of the influence vertex
      CalBone *pBone;
      pBone = vectorBone[influence.boneId];

      // transform vertex with current state of the bone
      CalVector v(vertex.position);
      v *= pBone->getTransformMatrix();
      v += pBone->getTranslationBoneSpace();

      x += influence.weight * v.x;
      y += influence.weight * v.y;
      z += influence.weight * v.z;
    }

    // save vertex position
    if(pSubmesh->getCoreSubmesh()->getSpringCount() > 0)
    {
      // get the pgysical property of the vertex
      CalCoreSubmesh::PhysicalProperty& physicalProperty = vectorPhysicalProperty[vertexId];

      // assign new vertex position if there is no vertex weight
      if(physicalProperty.weight == 0.0f)
      {
        pVertexBuffer[0] = x;
        pVertexBuffer[1] = y;
        pVertexBuffer[2] = z;
      }
    }
    else
    {
      pVertexBuffer[0] = x;
      pVertexBuffer[1] = y;
      pVertexBuffer[2] = z;
    }

    // next vertex position in buffer
    pVertexBuffer += 3;
  }

  return vertexCount;
}

 /*****************************************************************************/
/** Calculates the transformed normal data.
  *
  * This function calculates and returns the transformed normal data of a
  * specific submesh.
  *
  * @param pSubmesh A pointer to the submesh from which the normal data should
  *                 be calculated and returned.
  * @param pNormalBuffer A pointer to the user-provided buffer where the normal
  *                      data is written to.
  *
  * @return The number of normals written to the buffer.
  *****************************************************************************/

int CalPhysique::calculateNormals(CalSubmesh *pSubmesh, float *pNormalBuffer)
{
  // get bone vector of the skeleton
  std::vector<CalBone *>& vectorBone = m_pModel->getSkeleton()->getVectorBone();

  // get vertex vector of the submesh
  std::vector<CalCoreSubmesh::Vertex>& vectorVertex = pSubmesh->getCoreSubmesh()->getVectorVertex();

  // get the number of vertices
  int vertexCount;
  vertexCount = pSubmesh->getVertexCount();

  // calculate normal for all submesh vertices
  int vertexId;
  for(vertexId = 0; vertexId < vertexCount; vertexId++)
  {
    // get the vertex
    CalCoreSubmesh::Vertex& vertex = vectorVertex[vertexId];

    // initialize normal
    float nx, ny, nz;
    nx = 0.0f;
    ny = 0.0f;
    nz = 0.0f;

    // blend together all vertex influences
    int influenceId;
	int influenceCount=(int)vertex.vectorInfluence.size();
    for(influenceId = 0; influenceId < influenceCount; influenceId++)
    {
      // get the influence
      CalCoreSubmesh::Influence& influence = vertex.vectorInfluence[influenceId];

      // get the bone of the influence vertex
      CalBone *pBone;
      pBone = vectorBone[influence.boneId];

      // transform normal with current state of the bone
      CalVector v(vertex.normal);
      v *= pBone->getTransformMatrix(); 

      nx += influence.weight * v.x;
      ny += influence.weight * v.y;
      nz += influence.weight * v.z;
    }

    // re-normalize normal
    float scale;
    scale = 1.0f / sqrt(nx * nx + ny * ny + nz * nz);

    pNormalBuffer[0] = nx * scale;
    pNormalBuffer[1] = ny * scale;
    pNormalBuffer[2] = nz * scale;

    // next vertex position in buffer
    pNormalBuffer += 3;
  }

  return vertexCount;
}

 /*****************************************************************************/
/** Calculates the transformed vertex data.
  *
  * This function calculates and returns the transformed vertex and the transformed 
  * normal datadata of a specific submesh.
  *
  * @param pSubmesh A pointer to the submesh from which the vertex data should
  *                 be calculated and returned.
  * @param pVertexBuffer A pointer to the user-provided buffer where the vertex
  *                      data is written to.
  *
  * @return The number of vertices written to the buffer.
  *****************************************************************************/

int CalPhysique::calculateVerticesAndNormals(CalSubmesh *pSubmesh, float *pVertexBuffer)
{
  // get bone vector of the skeleton
  std::vector<CalBone *>& vectorBone = m_pModel->getSkeleton()->getVectorBone();

  // get vertex vector of the core submesh
  std::vector<CalCoreSubmesh::Vertex>& vectorVertex = pSubmesh->getCoreSubmesh()->getVectorVertex();

  // get physical property vector of the core submesh
  std::vector<CalCoreSubmesh::PhysicalProperty>& vectorPhysicalProperty = pSubmesh->getCoreSubmesh()->getVectorPhysicalProperty();

  // get the number of vertices
  int vertexCount;
  vertexCount = pSubmesh->getVertexCount();

  // calculate all submesh vertices
  int vertexId;
  for(vertexId = 0; vertexId < vertexCount; vertexId++)
  {
    // get the vertex
    CalCoreSubmesh::Vertex& vertex = vectorVertex[vertexId];

    // initialize vertex
    float x, y, z;
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;

	// initialize normal
    float nx, ny, nz;
    nx = 0.0f;
    ny = 0.0f;
    nz = 0.0f;

    // blend together all vertex influences
    int influenceId;
	int influenceCount=(int)vertex.vectorInfluence.size();
    for(influenceId = 0; influenceId < influenceCount; influenceId++)
    {
      // get the influence
      CalCoreSubmesh::Influence& influence = vertex.vectorInfluence[influenceId];

      // get the bone of the influence vertex
      CalBone *pBone;
      pBone = vectorBone[influence.boneId];

      // transform vertex with current state of the bone
      CalVector v(vertex.position);
      v *= pBone->getTransformMatrix();
      v += pBone->getTranslationBoneSpace();

      x += influence.weight * v.x;
      y += influence.weight * v.y;
      z += influence.weight * v.z;

	  // transform normal with current state of the bone
      CalVector n(vertex.normal);
      n *= pBone->getTransformMatrix();

      nx += influence.weight * n.x;
      ny += influence.weight * n.y;
      nz += influence.weight * n.z;
    }

    // save vertex position
    if(pSubmesh->getCoreSubmesh()->getSpringCount() > 0)
    {
      // get the pgysical property of the vertex
      CalCoreSubmesh::PhysicalProperty& physicalProperty = vectorPhysicalProperty[vertexId];

      // assign new vertex position if there is no vertex weight
      if(physicalProperty.weight == 0.0f)
      {
        pVertexBuffer[0] = x;
        pVertexBuffer[1] = y;
        pVertexBuffer[2] = z;
      }
    }
    else
    {
      pVertexBuffer[0] = x;
      pVertexBuffer[1] = y;
      pVertexBuffer[2] = z;
    }
    
	// re-normalize normal
    float scale;
    scale = 1.0f / sqrt(nx * nx + ny * ny + nz * nz);

    pVertexBuffer[3] = nx * scale;
    pVertexBuffer[4] = ny * scale;
    pVertexBuffer[5] = nz * scale;

	// next vertex position in buffer	
	pVertexBuffer += 6;

  }

  return vertexCount;
}

 /*****************************************************************************/
/** Calculates the transformed vertex data.
  *
  * This function calculates and returns the transformed vertex, the transformed 
  * normal datadata and the texture coords of a specific submesh.
  *
  * @param pSubmesh A pointer to the submesh from which the vertex data should
  *                 be calculated and returned.
  * 
  * @param pVertexBuffer A pointer to the user-provided buffer where the vertex
  *                      data is written to.
  *
  * @param NumTexCoords A integer with the number of texture coords
  *
  * @return The number of vertices written to the buffer.
  *****************************************************************************/


int CalPhysique::calculateVerticesNormalsAndTexCoords(CalSubmesh *pSubmesh, float *pVertexBuffer,int NumTexCoords)
{
  // get bone vector of the skeleton
  std::vector<CalBone *>& vectorBone = m_pModel->getSkeleton()->getVectorBone();

  // get vertex vector of the core submesh
  std::vector<CalCoreSubmesh::Vertex>& vectorVertex = pSubmesh->getCoreSubmesh()->getVectorVertex();

  // get the texture coordinate vector vector
  std::vector<std::vector<CalCoreSubmesh::TextureCoordinate> >& vectorvectorTextureCoordinate = pSubmesh->getCoreSubmesh()->getVectorVectorTextureCoordinate();

  int TextureCoordinateCount=(int)vectorvectorTextureCoordinate.size();

  // check if the map id is valid
  if(((NumTexCoords < 0) || (NumTexCoords > TextureCoordinateCount)))
  {
	 if(TextureCoordinateCount!=0)
	 {    
		 CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
		 return -1;
	 }
  }  

  // get physical property vector of the core submesh
  std::vector<CalCoreSubmesh::PhysicalProperty>& vectorPhysicalProperty = pSubmesh->getCoreSubmesh()->getVectorPhysicalProperty();

  // get the number of vertices
  int vertexCount;
  vertexCount = pSubmesh->getVertexCount();

  // calculate all submesh vertices
  int vertexId;
  for(vertexId = 0; vertexId < vertexCount; vertexId++)
  {
    // get the vertex
    CalCoreSubmesh::Vertex& vertex = vectorVertex[vertexId];

    // initialize vertex
    float x, y, z;
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;

	// initialize normal
    float nx, ny, nz;
    nx = 0.0f;
    ny = 0.0f;
    nz = 0.0f;

    // blend together all vertex influences
    int influenceId;
	int influenceCount=(int)vertex.vectorInfluence.size();
    for(influenceId = 0; influenceId < influenceCount; influenceId++)
    {
      // get the influence
      CalCoreSubmesh::Influence& influence = vertex.vectorInfluence[influenceId];

      // get the bone of the influence vertex
      CalBone *pBone;
      pBone = vectorBone[influence.boneId];

      // transform vertex with current state of the bone
      CalVector v(vertex.position);
      v *= pBone->getTransformMatrix();
      v += pBone->getTranslationBoneSpace();

      x += influence.weight * v.x;
      y += influence.weight * v.y;
      z += influence.weight * v.z;

	  // transform normal with current state of the bone
      CalVector n(vertex.normal);	  
      n *= pBone->getTransformMatrix();

      nx += influence.weight * n.x;
      ny += influence.weight * n.y;
      nz += influence.weight * n.z;
    }

    // save vertex position
    if(pSubmesh->getCoreSubmesh()->getSpringCount() > 0)
    {
      // get the pgysical property of the vertex
      CalCoreSubmesh::PhysicalProperty& physicalProperty = vectorPhysicalProperty[vertexId];

      // assign new vertex position if there is no vertex weight
      if(physicalProperty.weight == 0.0f)
      {
        pVertexBuffer[0] = x;
        pVertexBuffer[1] = y;
        pVertexBuffer[2] = z;
      }
    }
    else
    {
      pVertexBuffer[0] = x;
      pVertexBuffer[1] = y;
      pVertexBuffer[2] = z;
    }
    
	// re-normalize normal
    float scale;
    scale = 1.0f / sqrt(nx * nx + ny * ny + nz * nz);

    pVertexBuffer[3] = nx * scale;
    pVertexBuffer[4] = ny * scale;
    pVertexBuffer[5] = nz * scale;

	pVertexBuffer += 6;
	
	if(TextureCoordinateCount==0)
	{
		pVertexBuffer+=NumTexCoords*2;
	}
	else
	{
		for(int mapId=0;mapId<NumTexCoords;mapId++)
		{
			pVertexBuffer[0] = vectorvectorTextureCoordinate[mapId][vertexId].u;
			pVertexBuffer[1] = vectorvectorTextureCoordinate[mapId][vertexId].v;			
			pVertexBuffer += 2;
		}
	}
  }

  return vertexCount;
}



 /*****************************************************************************/
/** Creates the physique instance.
  *
  * This function creates the physique instance.
  *
  * @param pModel A pointer to the model that should be managed with this
  *               physique instance.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalPhysique::create(CalModel *pModel)
{
  if(pModel == 0)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  m_pModel = pModel;

  CalCoreModel *pCoreModel;
  pCoreModel = m_pModel->getCoreModel();
  if(pCoreModel == 0)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  return true;
}

 /*****************************************************************************/
/** Destroys the physique instance.
  *
  * This function destroys all data stored in the physique instance and frees
  * all allocated memory.
  *****************************************************************************/

void CalPhysique::destroy()
{
  m_pModel = 0;
}

 /*****************************************************************************/
/** Updates all the internally handled attached meshes.
  *
  * This function updates all the attached meshes of the model that are handled
  * internally.
  *****************************************************************************/

void CalPhysique::update()
{
  // get the attached meshes vector
  std::vector<CalMesh *>& vectorMesh = m_pModel->getVectorMesh();

  // loop through all the attached meshes
  std::vector<CalMesh *>::iterator iteratorMesh;
  for(iteratorMesh = vectorMesh.begin(); iteratorMesh != vectorMesh.end(); ++iteratorMesh)
  {
    // get the submesh vector of the mesh
    std::vector<CalSubmesh *>& vectorSubmesh = (*iteratorMesh)->getVectorSubmesh();

    // loop through all the submeshes of the mesh
    std::vector<CalSubmesh *>::iterator iteratorSubmesh;
    for(iteratorSubmesh = vectorSubmesh.begin(); iteratorSubmesh != vectorSubmesh.end(); ++iteratorSubmesh)
    {
      // check if the submesh handles vertex data internally
      if((*iteratorSubmesh)->hasInternalData())
      {
        // calculate the transformed vertices and store them in the submesh
        std::vector<CalVector>& vectorVertex = (*iteratorSubmesh)->getVectorVertex();
        calculateVertices(*iteratorSubmesh, (float *)&vectorVertex[0]);

        // calculate the transformed normals and store them in the submesh
        std::vector<CalVector>& vectorNormal = (*iteratorSubmesh)->getVectorNormal();
        calculateNormals(*iteratorSubmesh, (float *)&vectorNormal[0]);
      }
    }
  }
}

//****************************************************************************//
