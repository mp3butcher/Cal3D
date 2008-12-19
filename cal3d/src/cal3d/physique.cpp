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

#include "cal3d/error.h"
#include "cal3d/physique.h"
#include "cal3d/coremodel.h"
#include "cal3d/model.h"
#include "cal3d/mesh.h"
#include "cal3d/submesh.h"
#include "cal3d/skeleton.h"
#include "cal3d/bone.h"
#include "cal3d/coresubmesh.h"
#include "cal3d/coresubmorphtarget.h"

#include <cfloat>

 /*****************************************************************************/
/** Constructs the physique instance.
  *
  * This function is the default constructor of the physique instance.
  *****************************************************************************/

CalPhysique::CalPhysique(CalModel* pModel)
  : m_pModel(0)
  , m_Normalize(true)
{
  assert(pModel);
  m_pModel = pModel;
  m_axisFactorX = 1.0f;
  m_axisFactorY = 1.0f;
  m_axisFactorZ = 1.0f;
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

int CalPhysique::calculateVertices(CalSubmesh *pSubmesh, float *pVertexBuffer, int stride) const
{
  if(stride <= 0)
  {
	  stride = 3*sizeof(float);
  }

  // get bone vector of the skeleton
  std::vector<CalBone *>& vectorBone = m_pModel->getSkeleton()->getVectorBone();

  // get vertex vector of the core submesh
  std::vector<CalCoreSubmesh::Vertex>& vectorVertex = pSubmesh->getCoreSubmesh()->getVectorVertex();

  // get physical property vector of the core submesh
  std::vector<CalCoreSubmesh::PhysicalProperty>& vectorPhysicalProperty = pSubmesh->getCoreSubmesh()->getVectorPhysicalProperty();

  // get the number of vertices
  int vertexCount;
  vertexCount = pSubmesh->getVertexCount();
  
  // get the sub morph target vector from the core sub mesh
  std::vector<CalCoreSubMorphTarget*>& vectorSubMorphTarget =
  pSubmesh->getCoreSubmesh()->getVectorCoreSubMorphTarget();

  // calculate the base weight
  float baseWeight = pSubmesh->getBaseWeight();

  // Get the number of morph targets and cache the weights in an array
  // that can be indexed quickly inside the vertex inner loop.
  int morphTargetCount = pSubmesh->getMorphTargetWeightCount();
  static int const morphTargetCountMax = 100; // Arbitrary.
  if( morphTargetCount > morphTargetCountMax ) {
     morphTargetCount = morphTargetCountMax;
  }
  static float morphTargetScaleArray[ morphTargetCountMax ];
  for(int i = 0; i < morphTargetCount; i++ ) 
  {
     morphTargetScaleArray[ i ] = pSubmesh->getMorphTargetWeight( i ); 
  }

  // Check for spring case
  bool	hasSpringsAndInternalData =
	(pSubmesh->getCoreSubmesh()->getSpringCount() > 0) &&
	pSubmesh->hasInternalData();



  // calculate all submesh vertices
  int vertexId;
  for(vertexId = 0; vertexId < vertexCount; ++vertexId)
  {
    // get the vertex
    CalCoreSubmesh::Vertex& vertex = vectorVertex[vertexId];
    
    // blend the morph targets
    CalVector position(0,0,0);
    if(baseWeight == 1.0f)
    {
       position.x = vertex.position.x;
       position.y = vertex.position.y;
       position.z = vertex.position.z;
    }
    else
    {
      position.x = baseWeight*vertex.position.x;
      position.y = baseWeight*vertex.position.y;
      position.z = baseWeight*vertex.position.z;
      int morphTargetId;
      for(morphTargetId=0; morphTargetId < morphTargetCount;++morphTargetId)
      {
         CalCoreSubMorphTarget::BlendVertex const * blendVertex =
            vectorSubMorphTarget[morphTargetId]->getBlendVertex(vertexId);
         float morphScale = morphTargetScaleArray[ morphTargetId ];
         if( blendVertex ) 
         {
            position.x += morphScale * blendVertex->position.x;
            position.y += morphScale * blendVertex->position.y;
            position.z += morphScale * blendVertex->position.z;
         } 
         else 
         {
            position.x += morphScale * vertex.position.x;
            position.y += morphScale * vertex.position.y;
            position.z += morphScale * vertex.position.z;
         }
      }
    }

    // initialize vertex
    float x, y, z;
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;

    // blend together all vertex influences
    size_t influenceCount=vertex.vectorInfluence.size();
    if(influenceCount == 0) 
	{
      x = position.x;
      y = position.y;
      z = position.z;
    } 
	else 
	{
		for(size_t influenceId = 0; influenceId < influenceCount; ++influenceId)
		{
			// get the influence
			CalCoreSubmesh::Influence& influence = vertex.vectorInfluence[influenceId];
			
			// get the bone of the influence vertex
			CalBone *pBone;
			pBone = vectorBone[influence.boneId];
			
			// transform vertex with current state of the bone
			CalVector v(position);
			v *= pBone->getTransformMatrix();
			v += pBone->getTranslationBoneSpace();
			
			x += influence.weight * v.x;
			y += influence.weight * v.y;
			z += influence.weight * v.z;
		}
	}

    // save vertex position
    if(hasSpringsAndInternalData)
    {
      // get the physical property of the vertex
      CalCoreSubmesh::PhysicalProperty& physicalProperty = vectorPhysicalProperty[vertexId];

      // assign new vertex position if there is no vertex weight
      if(physicalProperty.weight == 0.0f)
      {
        pVertexBuffer[0] = x * m_axisFactorX;
        pVertexBuffer[1] = y * m_axisFactorY;
        pVertexBuffer[2] = z * m_axisFactorZ;
      }
    }
    else
    {
      pVertexBuffer[0] = x * m_axisFactorX;
      pVertexBuffer[1] = y * m_axisFactorY;
      pVertexBuffer[2] = z * m_axisFactorZ;
    }

    // next vertex position in buffer
    pVertexBuffer = (float *)(((char *)pVertexBuffer) + stride) ;
  }

  return vertexCount;
}

 /*****************************************************************************/
/** Calculates one transformed vertex.
  *
  * This function calculates and returns a transformed vertex of a
  * specific submesh.
  *
  * @param pSubmesh A pointer to the submesh from which the vertex should
  *                 be calculated and returned.
  * @param vertexId The id of the vertex that should be transformed.
  *
  * @return The number of vertices written to the buffer.
  *****************************************************************************/

CalVector CalPhysique::calculateVertex(CalSubmesh *pSubmesh, int vertexId)
{
  // get bone vector of the skeleton
  std::vector<CalBone *>& vectorBone = m_pModel->getSkeleton()->getVectorBone();

  // get vertex of the core submesh
  std::vector<CalCoreSubmesh::Vertex>& vectorVertex = pSubmesh->getCoreSubmesh()->getVectorVertex();

  // get physical property vector of the core submesh
  //std::vector<CalCoreSubmesh::PhysicalProperty>& vectorPhysicalProperty = pSubmesh->getCoreSubmesh()->getVectorPhysicalProperty();

  // get the sub morph target vector from the core sub mesh
  std::vector<CalCoreSubMorphTarget*>& vectorSubMorphTarget =
  pSubmesh->getCoreSubmesh()->getVectorCoreSubMorphTarget();

  // calculate the base weight
  float baseWeight = pSubmesh->getBaseWeight();

  // get the number of morph targets
  int morphTargetCount = pSubmesh->getMorphTargetWeightCount();

  // get the vertex
  CalCoreSubmesh::Vertex& vertex = vectorVertex[vertexId];

  // blend the morph targets
  CalVector position(0,0,0);
  if(baseWeight == 1.0f)
  {
    position.x = vertex.position.x;
    position.y = vertex.position.y;
    position.z = vertex.position.z;
  }
  else
  {
    position.x = baseWeight*vertex.position.x;
    position.y = baseWeight*vertex.position.y;
    position.z = baseWeight*vertex.position.z;
    int morphTargetId;
    for(morphTargetId=0; morphTargetId < morphTargetCount;++morphTargetId)
    {
      CalCoreSubMorphTarget::BlendVertex	blendVertex;
	  vectorSubMorphTarget[morphTargetId]->getBlendVertex(vertexId, blendVertex);
      float currentWeight = pSubmesh->getMorphTargetWeight(morphTargetId);
      position.x += currentWeight*blendVertex.position.x;
      position.y += currentWeight*blendVertex.position.y;
      position.z += currentWeight*blendVertex.position.z;
    }
  }

  // initialize vertex
  float x, y, z;
  x = 0.0f;
  y = 0.0f;
  z = 0.0f;

  // blend together all vertex influences
  int influenceId;
  int influenceCount=(int)vertex.vectorInfluence.size();
  if(influenceCount == 0) 
  {
    x = position.x;
    y = position.y;
    z = position.z;
  } 
  else 
  {
	  for(influenceId = 0; influenceId < influenceCount; ++influenceId)
	  {
		  // get the influence
		  CalCoreSubmesh::Influence& influence = vertex.vectorInfluence[influenceId];
		  
		  // get the bone of the influence vertex
		  CalBone *pBone;
		  pBone = vectorBone[influence.boneId];
		  
		  // transform vertex with current state of the bone
		  CalVector v(position);
		  v *= pBone->getTransformMatrix();
		  v += pBone->getTranslationBoneSpace();
		  
		  x += influence.weight * v.x;
		  y += influence.weight * v.y;
		  z += influence.weight * v.z;
	  }
  }
  /*
  // save vertex position
  if(pSubmesh->getCoreSubmesh()->getSpringCount() > 0 && pSubmesh->hasInternalData())
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
  */
  // return the vertex
  //return CalVector(x, y, z);
  return CalVector(x*m_axisFactorX,y*m_axisFactorY,z*m_axisFactorZ);
}
 /*****************************************************************************/
/** Calculates the transformed tangent space data.
  *
  * This function calculates and returns the transformed tangent space data of a
  * specific submesh.
  *
  * @param pSubmesh A pointer to the submesh from which the tangent space data 
  *                 should be calculated and returned.
  * @param mapId
  * @param pTangentSpaceBuffer A pointer to the user-provided buffer where the tangent 
  *                 space data is written to.
  *
  * @return The number of tangent spaces written to the buffer.
  *****************************************************************************/

int CalPhysique::calculateTangentSpaces(CalSubmesh *pSubmesh, int mapId, float *pTangentSpaceBuffer, int stride) const
{
  if((mapId < 0) || (mapId >= (int)pSubmesh->getCoreSubmesh()->getVectorVectorTangentSpace().size())) return false;
  if(stride <= 0)
  {
	  stride = 4*sizeof(float);
  }

  // get bone vector of the skeleton
  std::vector<CalBone *>& vectorBone = m_pModel->getSkeleton()->getVectorBone();

  // get vertex vector of the submesh
  std::vector<CalCoreSubmesh::Vertex>& vectorVertex = pSubmesh->getCoreSubmesh()->getVectorVertex();

  // get tangent space vector of the submesh
  std::vector<CalCoreSubmesh::TangentSpace>& vectorTangentSpace = pSubmesh->getCoreSubmesh()->getVectorVectorTangentSpace()[mapId];
  
  // get the number of vertices
  int vertexCount;
  vertexCount = pSubmesh->getVertexCount();

  // calculate normal for all submesh vertices
  int vertexId;
  for(vertexId = 0; vertexId < vertexCount; vertexId++)
  {
    CalCoreSubmesh::TangentSpace& tangentSpace = vectorTangentSpace[vertexId];

    // get the vertex
    CalCoreSubmesh::Vertex& vertex = vectorVertex[vertexId];

    // initialize tangent
    float tx, ty, tz;
    tx = 0.0f;
    ty = 0.0f;
    tz = 0.0f;

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
      CalVector v(tangentSpace.tangent);
      v *= pBone->getTransformMatrix(); 

      tx += influence.weight * v.x;
      ty += influence.weight * v.y;
      tz += influence.weight * v.z;
    }

    // re-normalize tangent if necessary
    if (m_Normalize)
    {
      float scale;
	  tx/= m_axisFactorX;
	  ty/= m_axisFactorY;
	  tz/= m_axisFactorZ;

      scale = (float)( 1.0f / sqrt(tx * tx + ty * ty + tz * tz));

      pTangentSpaceBuffer[0] = tx * scale;
      pTangentSpaceBuffer[1] = ty * scale;
      pTangentSpaceBuffer[2] = tz * scale;	  
    }
    else
    {
      pTangentSpaceBuffer[0] = tx;
      pTangentSpaceBuffer[1] = ty;
      pTangentSpaceBuffer[2] = tz;
    }

    pTangentSpaceBuffer[3] = tangentSpace.crossFactor;
    // next vertex position in buffer
    pTangentSpaceBuffer = (float *)(((char *)pTangentSpaceBuffer) + stride) ;
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

int CalPhysique::calculateNormals(CalSubmesh *pSubmesh, float *pNormalBuffer, int stride) const
{
  if(stride <= 0)
  {
	  stride = 3*sizeof(float);
  }

  // get bone vector of the skeleton
  std::vector<CalBone *>& vectorBone = m_pModel->getSkeleton()->getVectorBone();

  // get vertex vector of the submesh
  std::vector<CalCoreSubmesh::Vertex>& vectorVertex = pSubmesh->getCoreSubmesh()->getVectorVertex();

  // get the number of vertices
  int vertexCount;
  vertexCount = pSubmesh->getVertexCount();

  // get the sub morph target vector from the core sub mesh
  std::vector<CalCoreSubMorphTarget*>& vectorSubMorphTarget =
  pSubmesh->getCoreSubmesh()->getVectorCoreSubMorphTarget();

  // calculate the base weight
  float baseWeight = pSubmesh->getBaseWeight();

  // get the number of morph targets
  int morphTargetCount = pSubmesh->getMorphTargetWeightCount();

  // calculate normal for all submesh vertices
  int vertexId;
  for(vertexId = 0; vertexId < vertexCount; ++vertexId)
  {
    // get the vertex
    CalCoreSubmesh::Vertex& vertex = vectorVertex[vertexId];

    // blend the morph targets
    CalVector normal(0,0,0);
    if(baseWeight == 1.0f)
    {
      normal.x = vertex.normal.x;
      normal.y = vertex.normal.y;
      normal.z = vertex.normal.z;
    }
    else
    {
      normal.x = baseWeight*vertex.normal.x;
      normal.y = baseWeight*vertex.normal.y;
      normal.z = baseWeight*vertex.normal.z;
      int morphTargetId;
      for(morphTargetId=0; morphTargetId < morphTargetCount;++morphTargetId)
      {
         CalCoreSubMorphTarget::BlendVertex const * blendVertex = 
            vectorSubMorphTarget[morphTargetId]->getBlendVertex(vertexId);
         float currentWeight = pSubmesh->getMorphTargetWeight(morphTargetId);
         if( blendVertex ) {
            normal.x += currentWeight*blendVertex->normal.x;
            normal.y += currentWeight*blendVertex->normal.y;
            normal.z += currentWeight*blendVertex->normal.z;
         } else {
            normal.x += currentWeight*vertex.normal.x;
            normal.y += currentWeight*vertex.normal.y;
            normal.z += currentWeight*vertex.normal.z;
         }
      }
    }

    // initialize normal
    float nx, ny, nz;
    nx = 0.0f;
    ny = 0.0f;
    nz = 0.0f;

    // blend together all vertex influences
    int influenceId;
	int influenceCount=(int)vertex.vectorInfluence.size();
    if(influenceCount == 0) 
	{
      nx = normal.x;
      ny = normal.y;
      nz = normal.z;
    } 
	else 
	{
		for(influenceId = 0; influenceId < influenceCount; ++influenceId)
		{
			// get the influence
			CalCoreSubmesh::Influence& influence = vertex.vectorInfluence[influenceId];
			
			// get the bone of the influence vertex
			CalBone *pBone;
			pBone = vectorBone[influence.boneId];
			
			// transform normal with current state of the bone
			CalVector v(normal);
			v *= pBone->getTransformMatrix(); 
			
			nx += influence.weight * v.x;
			ny += influence.weight * v.y;
			nz += influence.weight * v.z;
		}
	}

    // re-normalize normal if necessary
    if (m_Normalize)
    {
	  nx/= m_axisFactorX;
	  ny/= m_axisFactorY;
	  nz/= m_axisFactorZ;

      float scale;
      scale = (float)( 1.0f / sqrt(nx * nx + ny * ny + nz * nz));

      pNormalBuffer[0] = nx * scale;
      pNormalBuffer[1] = ny * scale;
      pNormalBuffer[2] = nz * scale;
    }
    else
    {
      pNormalBuffer[0] = nx;
      pNormalBuffer[1] = ny;
      pNormalBuffer[2] = nz;
    } 

    // next vertex position in buffer
    pNormalBuffer = (float *)(((char *)pNormalBuffer) + stride) ;
  }

  return vertexCount;
}


static MorphIdAndWeight * MiawCache = NULL;
static unsigned int MiawCacheNumElements = 0;
void
EnlargeMiawCacheAsNecessary( unsigned int numElements )
{
   if( MiawCacheNumElements < numElements ) {
      if( MiawCache ) {
         delete [] MiawCache;
      }

      // Step up exponentially to reduce number of steps.
      MiawCacheNumElements = numElements * 2;
      MiawCache = new( MorphIdAndWeight [ MiawCacheNumElements ] );
   }
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

int CalPhysique::calculateVerticesAndNormals(CalSubmesh *pSubmesh, float *pVertexBuffer, int stride) const
{
  if(stride <= 0)
  {
	  stride = 6*sizeof(float);
  }

  // get bone vector of the skeleton
  std::vector<CalBone *>& vectorBone = m_pModel->getSkeleton()->getVectorBone();
  CalBone ** vectorBonePtr = &vectorBone[0];

  // get vertex vector of the core submesh
  CalCoreSubmesh::Vertex* vectorVertex = &pSubmesh->getCoreSubmesh()->getVectorVertex()[0];

  // get physical property vector of the core submesh
  std::vector<CalCoreSubmesh::PhysicalProperty>& vectorPhysicalProperty = pSubmesh->getCoreSubmesh()->getVectorPhysicalProperty();

  // get the number of vertices
  int vertexCount = pSubmesh->getVertexCount();

  // get the sub morph target vector from the core sub mesh
  std::vector<CalCoreSubMorphTarget*>& vectorSubMorphTarget =
	pSubmesh->getCoreSubmesh()->getVectorCoreSubMorphTarget();

 

  // get the number of morph targets
  int morphTargetCount = pSubmesh->getMorphTargetWeightCount();
  EnlargeMiawCacheAsNecessary( morphTargetCount );
  unsigned int numMiaws;
  pSubmesh->getMorphIdAndWeightArray( MiawCache, & numMiaws, ( unsigned int ) morphTargetCount );

  // calculate the base weight
  float baseWeight2 = pSubmesh->getBaseWeight();

  // Check for spring case
  bool	hasSpringsAndInternalData =
	(pSubmesh->getCoreSubmesh()->getSpringCount() > 0) &&
	pSubmesh->hasInternalData();

  // calculate all submesh vertices
  int vertexId;
  //CalCoreSubMorphTarget::BlendVertex	blendVertex;
  for(vertexId = 0; vertexId < vertexCount; ++vertexId)
  {
    // get the vertex
    CalCoreSubmesh::Vertex& vertex = vectorVertex[vertexId];

    // Off unless normalizing set to on and either there are morph targets or multiple influences.
    bool mustNormalize = false; 

    // blend the morph targets
    CalVector position(0,0,0);
    CalVector normal(0,0,0);
    if(baseWeight2 == 1.0f)
    {
      position = vertex.position;
      normal = vertex.normal;
    }
    else
    {
       float baseWeight = baseWeight2;
       position.x = 0;
       position.y = 0;
       position.z = 0;
       normal.x = 0;
       normal.y = 0;
       normal.z = 0;
       mustNormalize = true; // Morph targets can skew normals.
       unsigned int i;
       for( i = 0; i < numMiaws; i++ ) {
          MorphIdAndWeight * miaw = & MiawCache[ i ];
          int morphTargetId = miaw->morphId_;
          CalCoreSubMorphTarget::BlendVertex const * blendVertex =
             vectorSubMorphTarget[morphTargetId]->getBlendVertex(vertexId);
          float currentWeight = miaw->weight_;
          if( blendVertex ) {
             position.x += currentWeight*blendVertex->position.x;
             position.y += currentWeight*blendVertex->position.y;
             position.z += currentWeight*blendVertex->position.z;
             normal.x += currentWeight*blendVertex->normal.x;
             normal.y += currentWeight*blendVertex->normal.y;
             normal.z += currentWeight*blendVertex->normal.z;
          } else {
             baseWeight += currentWeight;
          }
       }
       position.x += baseWeight*vertex.position.x;
       position.y += baseWeight*vertex.position.y;
       position.z += baseWeight*vertex.position.z;
       normal.x += baseWeight*vertex.normal.x;
       normal.y += baseWeight*vertex.normal.y;
       normal.z += baseWeight*vertex.normal.z;
    }

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
    if(influenceCount > 1) 
	{ 
      mustNormalize = true; // If multiple influences, normalize the normals!
    } 

    for(influenceId = 0; influenceId < influenceCount; ++influenceId)
    {
       // get the influence
       CalCoreSubmesh::Influence& influence = vertex.vectorInfluence[influenceId];

       // get the bone of the influence vertex
       CalBone *pBone;
       pBone = vectorBone[influence.boneId];

       // transform vertex with current state of the bone
       CalVector v(position);
       v *= pBone->getTransformMatrix();
       v += pBone->getTranslationBoneSpace();

       x += influence.weight * v.x;
       y += influence.weight * v.y;
       z += influence.weight * v.z;

       // transform normal with current state of the bone
       CalVector n(normal);
       n *= pBone->getTransformMatrix();

       nx += influence.weight * n.x;
       ny += influence.weight * n.y;
       nz += influence.weight * n.z;
    }
    // save vertex position
    if(hasSpringsAndInternalData)
    {
       mustNormalize = true;

      // get the physical property of the vertex
      CalCoreSubmesh::PhysicalProperty& physicalProperty = vectorPhysicalProperty[vertexId];

      // assign new vertex position if there is no vertex weight
      if(physicalProperty.weight == 0.0f)
      {
        pVertexBuffer[0] = x * m_axisFactorX;
        pVertexBuffer[1] = y * m_axisFactorY;
        pVertexBuffer[2] = z * m_axisFactorZ;
      }
    }
    else
    {
      pVertexBuffer[0] = x * m_axisFactorX;
      pVertexBuffer[1] = y * m_axisFactorY;
      pVertexBuffer[2] = z * m_axisFactorZ;
    }
    
    // re-normalize normal if necessary
    if (m_Normalize && mustNormalize)
    {
	  nx/= m_axisFactorX;
	  ny/= m_axisFactorY;
	  nz/= m_axisFactorZ;

      float scale;
      scale = (float)( 1.0f / sqrt(nx * nx + ny * ny + nz * nz));

      pVertexBuffer[3] = nx * scale;
      pVertexBuffer[4] = ny * scale;
      pVertexBuffer[5] = nz * scale;
    }
    else
    {
      pVertexBuffer[3] = nx;
      pVertexBuffer[4] = ny;
      pVertexBuffer[5] = nz;
    } 


	// next vertex position in buffer	
    pVertexBuffer = (float *)(((char *)pVertexBuffer) + stride) ;
  }

  return vertexCount;
}

 /*****************************************************************************/
/** Calculates the transformed vertex data.
  *
  * This function calculates and returns the transformed vertex, the transformed 
  * normal data and the texture coords of a specific submesh.
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


int CalPhysique::calculateVerticesNormalsAndTexCoords(CalSubmesh *pSubmesh, float *pVertexBuffer, int NumTexCoords) const
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

  // get the sub morph target vector from the core sub mesh
  std::vector<CalCoreSubMorphTarget*>& vectorSubMorphTarget =
  pSubmesh->getCoreSubmesh()->getVectorCoreSubMorphTarget();

  // calculate the base weight
  float baseWeight = pSubmesh->getBaseWeight();

  // get the number of morph targets
  int morphTargetCount = pSubmesh->getMorphTargetWeightCount();

  // Check for spring case
  bool	hasSpringsAndInternalData =
	(pSubmesh->getCoreSubmesh()->getSpringCount() > 0) &&
	pSubmesh->hasInternalData();

  // calculate all submesh vertices
  int vertexId;
  for(vertexId = 0; vertexId < vertexCount; ++vertexId)
  {
    // get the vertex
    CalCoreSubmesh::Vertex& vertex = vectorVertex[vertexId];

    // blend the morph targets
    CalVector position(0,0,0);
    CalVector normal(0,0,0);
    if(baseWeight == 1.0f)
    {
      position.x = vertex.position.x;
      position.y = vertex.position.y;
      position.z = vertex.position.z;
      normal.x = vertex.normal.x;
      normal.y = vertex.normal.y;
      normal.z = vertex.normal.z;
    }
    else
    {
      position.x = baseWeight*vertex.position.x;
      position.y = baseWeight*vertex.position.y;
      position.z = baseWeight*vertex.position.z;
      normal.x = baseWeight*vertex.normal.x;
      normal.y = baseWeight*vertex.normal.y;
      normal.z = baseWeight*vertex.normal.z;
      int morphTargetId;
      for(morphTargetId=0; morphTargetId < morphTargetCount;++morphTargetId)
      {
         CalCoreSubMorphTarget::BlendVertex const * blendVertex =
            vectorSubMorphTarget[morphTargetId]->getBlendVertex(vertexId);
         float currentWeight = pSubmesh->getMorphTargetWeight(morphTargetId);
         if( currentWeight != 0 ) {
            if( blendVertex ) {
               position.x += currentWeight*blendVertex->position.x;
               position.y += currentWeight*blendVertex->position.y;
               position.z += currentWeight*blendVertex->position.z;
               normal.x += currentWeight*blendVertex->normal.x;
               normal.y += currentWeight*blendVertex->normal.y;
               normal.z += currentWeight*blendVertex->normal.z;
            } else {
               position.x += currentWeight*vertex.position.x;
               position.y += currentWeight*vertex.position.y;
               position.z += currentWeight*vertex.position.z;
               normal.x += currentWeight*vertex.normal.x;
               normal.y += currentWeight*vertex.normal.y;
               normal.z += currentWeight*vertex.normal.z;
            }
         }
      }
    }

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
	if(influenceCount == 0) 
	{
      x = position.x;
      y = position.y;
      z = position.z;
      nx = normal.x;
      ny = normal.y;
      nz = normal.z;
    } 
	else 
	{
		for(influenceId = 0; influenceId < influenceCount; ++influenceId)
		{
			// get the influence
			CalCoreSubmesh::Influence& influence = vertex.vectorInfluence[influenceId];
			
			// get the bone of the influence vertex
			CalBone *pBone;
			pBone = vectorBone[influence.boneId];
			
			// transform vertex with current state of the bone
			CalVector v(position);
			v *= pBone->getTransformMatrix();
			v += pBone->getTranslationBoneSpace();
			
			x += influence.weight * v.x;
			y += influence.weight * v.y;
			z += influence.weight * v.z;
			
			// transform normal with current state of the bone
			CalVector n(normal);	  
			n *= pBone->getTransformMatrix();
			
			nx += influence.weight * n.x;
			ny += influence.weight * n.y;
			nz += influence.weight * n.z;
		}
	}

    // save vertex position
    if(hasSpringsAndInternalData)
    {
      // get the pgysical property of the vertex
      CalCoreSubmesh::PhysicalProperty& physicalProperty = vectorPhysicalProperty[vertexId];

      // assign new vertex position if there is no vertex weight
      if(physicalProperty.weight == 0.0f)
      {
        pVertexBuffer[0] = x * m_axisFactorX;
        pVertexBuffer[1] = y * m_axisFactorY;
        pVertexBuffer[2] = z * m_axisFactorZ;
      }
    }
    else
    {
      pVertexBuffer[0] = x * m_axisFactorX;
      pVertexBuffer[1] = y * m_axisFactorY;
      pVertexBuffer[2] = z * m_axisFactorZ;
    }
    
	 // re-normalize normal if necessary
    if (m_Normalize)
    {
	  nx/= m_axisFactorX;
	  ny/= m_axisFactorY;
	  nz/= m_axisFactorZ;

      float scale;
      scale = (float) (1.0f / sqrt(nx * nx + ny * ny + nz * nz));

      pVertexBuffer[3] = nx * scale;
      pVertexBuffer[4] = ny * scale;
      pVertexBuffer[5] = nz * scale;
    }
    else
    {
      pVertexBuffer[3] = nx;
      pVertexBuffer[4] = ny;
      pVertexBuffer[5] = nz;
    }

	pVertexBuffer += 6;
	
	if(TextureCoordinateCount==0)
	{
		pVertexBuffer+=NumTexCoords*2;
	}
	else
	{
		for(int mapId=0; mapId < NumTexCoords; ++mapId)
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

        unsigned mapId;
        for(mapId=0;mapId< (*iteratorSubmesh)->getVectorVectorTangentSpace().size();mapId++)
        {
          if((*iteratorSubmesh)->isTangentsEnabled(mapId))
          {
            std::vector<CalSubmesh::TangentSpace>& vectorTangentSpace = (*iteratorSubmesh)->getVectorVectorTangentSpace()[mapId];
            calculateTangentSpaces(*iteratorSubmesh, mapId,(float *)&vectorTangentSpace[0]);
          }
        }

      }
    }
  }
}

 /*****************************************************************************/
/** Sets the normalization flag to true or false.
  *
  * This function sets the normalization flag on or off. If off, the normals
  * calculated by Cal3D will not be normalized. Instead, this transform is left
  * up to the user.
  *****************************************************************************/

void CalPhysique::setNormalization(bool normalize)
{
  m_Normalize = normalize;
}


void CalPhysique::setAxisFactorX(float factor)
{
	m_axisFactorX = factor;
	m_Normalize = true;	
}

void CalPhysique::setAxisFactorY(float factor)
{
	m_axisFactorY = factor;
	m_Normalize = true;	
}

void CalPhysique::setAxisFactorZ(float factor)
{
	m_axisFactorZ = factor;
	m_Normalize = true;	
}
//****************************************************************************//
