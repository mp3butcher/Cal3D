//****************************************************************************//
// physiquedualquaternion.cpp                                                 //
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

// This class implements a method described in the paper
// "Dual Quaternions for Rigid Transformation Blending"
// by Kavan, Collins, O'Sullivan, and Zara, 2006.

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "cal3d/error.h"
#include "cal3d/coremodel.h"
#include "cal3d/model.h"
#include "cal3d/mesh.h"
#include "cal3d/submesh.h"
#include "cal3d/skeleton.h"
#include "cal3d/bone.h"
#include "cal3d/coresubmesh.h"
#include "cal3d/coresubmorphtarget.h"
#include "cal3d/physiquedualquaternion.h"
#include "cal3d/dualquaternion.h"

#include <cfloat>
#include <numeric>


static void CalcMorphBlendedPosition( CalSubmesh *pSubmesh, int vertexID,
                                      float baseWeight, std::vector<int>& morphIDs,
                                      CalVector& outPosition )
{
  CalCoreSubmesh::Vertex& vertex =
    pSubmesh->getCoreSubmesh()->getVectorVertex()[vertexID];

  if (morphIDs.empty())
  {
    outPosition = vertex.position;
  }
  else
  {
    // get the sub morph target vector from the core sub mesh
    std::vector<CalCoreSubMorphTarget*>& vectorSubMorphTarget =
      pSubmesh->getCoreSubmesh()->getVectorCoreSubMorphTarget();

    outPosition = baseWeight * vertex.position;

    int morphTargetId;
    for (int i = 0; i < morphIDs.size(); ++i)
    {
      morphTargetId = morphIDs[i];
      float currentWeight = pSubmesh->getMorphTargetWeight(morphTargetId);
      CalCoreSubMorphTarget::BlendVertex	blendVertex;
      vectorSubMorphTarget[morphTargetId]->getBlendVertex(vertexID, blendVertex);
      outPosition += currentWeight * blendVertex.position;
    }
  }
}


static void CalcMorphBlendedNormal( CalSubmesh *pSubmesh, int vertexID,
                                    float baseWeight, std::vector<int>& morphIDs,
                                    CalVector& outNormal )
{
  CalCoreSubmesh::Vertex& vertex =
    pSubmesh->getCoreSubmesh()->getVectorVertex()[vertexID];

  if (morphIDs.empty())
  {
    outNormal = vertex.normal;
  }
  else
  {
    // get the sub morph target vector from the core sub mesh
    std::vector<CalCoreSubMorphTarget*>& vectorSubMorphTarget =
      pSubmesh->getCoreSubmesh()->getVectorCoreSubMorphTarget();

    outNormal = baseWeight * vertex.normal;

    int morphTargetId;
    for (int i = 0; i < morphIDs.size(); ++i)
    {
      morphTargetId = morphIDs[i];
      float currentWeight = pSubmesh->getMorphTargetWeight(morphTargetId);
      CalCoreSubMorphTarget::BlendVertex	blendVertex;
      vectorSubMorphTarget[morphTargetId]->getBlendVertex(vertexID, blendVertex);
      outNormal += currentWeight * blendVertex.normal;
    }
  }
}

static void CalcMorphBlendedPositionAndNormal( CalSubmesh *pSubmesh, int vertexID,
                                               float baseWeight, std::vector<int>& morphIDs,
                                               CalVector& outPosition,
                                               CalVector& outNormal )
{
  CalCoreSubmesh::Vertex& vertex =
    pSubmesh->getCoreSubmesh()->getVectorVertex()[vertexID];

  if (morphIDs.empty())
  {
    outPosition = vertex.position;
    outNormal = vertex.normal;
  }
  else
  {
    // get the sub morph target vector from the core sub mesh
    std::vector<CalCoreSubMorphTarget*>& vectorSubMorphTarget =
      pSubmesh->getCoreSubmesh()->getVectorCoreSubMorphTarget();

    outPosition = baseWeight * vertex.position;
    outNormal = baseWeight * vertex.normal;

    int morphTargetId;
    for (int i = 0; i < morphIDs.size(); ++i)
    {
      morphTargetId = morphIDs[i];
      float currentWeight = pSubmesh->getMorphTargetWeight(morphTargetId);
      CalCoreSubMorphTarget::BlendVertex	blendVertex;
      vectorSubMorphTarget[morphTargetId]->getBlendVertex(vertexID, blendVertex);
      outPosition += currentWeight * blendVertex.position;
      outNormal += currentWeight * blendVertex.normal;
    }
  }
}

static void GetUsedMorphTargetIDs( CalSubmesh *pSubmesh, std::vector<int>& outMorphIDs )
{
  int morphTargetCount = pSubmesh->getMorphTargetWeightCount();

  int morphTargetId;
  for (morphTargetId=0; morphTargetId < morphTargetCount; ++morphTargetId)
  {
    if (pSubmesh->getMorphTargetWeight(morphTargetId) > FLT_EPSILON)
    {
      outMorphIDs.push_back( morphTargetId );
    }
  }
}

static float CalcMorphBaseWeight( CalSubmesh *pSubmesh,  const std::vector<int>& morphIDs )
{
  float	baseWeight = 1.0f;

  for (std::vector<int>::const_iterator i = morphIDs.begin(); i != morphIDs.end(); ++i)
  {
    baseWeight -= pSubmesh->getMorphTargetWeight( *i );
  }

  return baseWeight;
}

inline static void StoreVectorInBuffer( const CalVector& inVec, float* outBuffer )
{
  outBuffer[0] = inVec.x;
  outBuffer[1] = inVec.y;
  outBuffer[2] = inVec.z;
}

static void CalcInfluencedPosition( const CalVector& morphedPosition,
                                    const std::vector<CalCoreSubmesh::Influence>& vectorInfluence,
                                    CalBone *const *vectorBone,
                                    CalVector& outPosition )
{
  // blend together all vertex influences
  size_t influenceCount = vectorInfluence.size();
  if (influenceCount == 0)
  {
    outPosition = morphedPosition;
  }
  else if (influenceCount == 1)
  {
    const CalBone*	oneBone = vectorBone[ vectorInfluence[0].boneId ];
    outPosition = morphedPosition;
    outPosition *= oneBone->getTransformMatrix();
    outPosition += oneBone->getTranslationBoneSpace();
  }
  else
  {
    CalDualQuaternion	blended;
    blended.nondual = CalQuaternion( 0.0f, 0.0f, 0.0f, 0.0f );
    blended.dual = CalQuaternion( 0.0f, 0.0f, 0.0f, 0.0f );
    CalQuaternion	pivot;

    for (size_t influenceId = 0; influenceId < influenceCount; ++influenceId)
    {
      // get the influence
      const CalCoreSubmesh::Influence& influence = vectorInfluence[influenceId];

      // get the bone of the influence vertex
      const CalBone *pBone = vectorBone[influence.boneId];

      // Get the dual quaternion for the bonespace transform
      CalDualQuaternion	boneTransform( pBone->getRotationBoneSpace(),
      	pBone->getTranslationBoneSpace() );

      // A dual quaternion and its negative both represent the same rigid
      // motion.  However, it can matter which one we use here.  If we
      // add two dual quaternions that are approximate negatives of each
      // other, we get a sum near zero, and then normalizing it becomes
      // numerically unstable.  So we try to avoid that case.
      if (influenceId == 0)
      {
        pivot = boneTransform.nondual;
      }
      else if (dot( boneTransform.nondual, pivot ) < 0.0f)
      {
        boneTransform *= -1.0f;
      }

      boneTransform *= influence.weight;

      blended += boneTransform;
    }

    blended.normalize();

    blended.transformPoint( morphedPosition, outPosition );
  }
}

static void CalcInfluencedNormal( const CalVector& morphedNormal,
                                  const std::vector<CalCoreSubmesh::Influence>& vectorInfluence,
                                  CalBone *const *vectorBone,
                                  CalVector& outNormal )
{
  // blend together all vertex influences
  size_t influenceCount = vectorInfluence.size();
  if (influenceCount == 0)
  {
    outNormal = morphedNormal;
  }
  else if (influenceCount == 1)
  {
    const CalBone*	oneBone = vectorBone[ vectorInfluence[0].boneId ];
    outNormal = morphedNormal;
    outNormal *= oneBone->getTransformMatrix();
  }
  else
  {
    CalDualQuaternion	blended;
    blended.nondual = CalQuaternion( 0.0f, 0.0f, 0.0f, 0.0f );
    blended.dual = CalQuaternion( 0.0f, 0.0f, 0.0f, 0.0f );
    CalQuaternion	pivot;

    for (size_t influenceId = 0; influenceId < influenceCount; ++influenceId)
    {
      // get the influence
      const CalCoreSubmesh::Influence& influence = vectorInfluence[influenceId];

      // get the bone of the influence vertex
      const CalBone *pBone = vectorBone[influence.boneId];

      // Get the dual quaternion for the rotation part of the bonespace
      // transform
      CalDualQuaternion	boneTransform( pBone->getRotationBoneSpace(),
        CalVector() );
      if (influenceId == 0)
      {
        pivot = boneTransform.nondual;
      }
      else if (dot( boneTransform.nondual, pivot ) < 0.0f)
      {
        boneTransform *= -1.0f;
      }
      boneTransform *= influence.weight;

      blended += boneTransform;
    }

    blended.transformPoint( morphedNormal, outNormal );
  }
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

int CalPhysiqueDualQuat::calculateVertices(CalSubmesh *pSubmesh, float *pVertexBuffer, int stride) const
{
  if (stride <= 0)
  {
    stride = 3*sizeof(float);
  }

  // get bone vector of the skeleton
  CalBone *const *vectorBone = &m_pModel->getSkeleton()->getVectorBone()[0];

  // get vertex vector of the core submesh
  const CalCoreSubmesh::Vertex* vectorVertex = &pSubmesh->getCoreSubmesh()->getVectorVertex()[0];

  // get physical property vector of the core submesh
  std::vector<CalCoreSubmesh::PhysicalProperty>& vectorPhysicalProperty =
    pSubmesh->getCoreSubmesh()->getVectorPhysicalProperty();

  // get the number of vertices
  const int vertexCount = pSubmesh->getVertexCount();

  // Get IDs of morph targets with nonzero weight
  std::vector<int> morphIDs;
  GetUsedMorphTargetIDs( pSubmesh, morphIDs );

  // calculate the base weight
  float baseWeight = CalcMorphBaseWeight( pSubmesh, morphIDs );

  // Check for spring case
  bool hasSpringsAndInternalData =
    (pSubmesh->getCoreSubmesh()->getSpringCount() > 0) &&
    pSubmesh->hasInternalData();

  // calculate all submesh vertices
  int vertexId;
  for (vertexId = 0; vertexId < vertexCount; ++vertexId)
  {
    // get the vertex
    const CalCoreSubmesh::Vertex& vertex = vectorVertex[vertexId];

    // blend the morph targets
    CalVector morphedPosition;
    CalcMorphBlendedPosition( pSubmesh, vertexId, baseWeight, morphIDs,
      morphedPosition );

    // blend influences by bones
    CalVector	influencedPosition;
    CalcInfluencedPosition( morphedPosition, vertex.vectorInfluence,
      vectorBone, influencedPosition );
    influencedPosition.x *= m_axisFactorX;
    influencedPosition.y *= m_axisFactorY;
    influencedPosition.z *= m_axisFactorZ;

    // save vertex position
    if (hasSpringsAndInternalData)
    {
      // get the physical property of the vertex
      CalCoreSubmesh::PhysicalProperty& physicalProperty = vectorPhysicalProperty[vertexId];

      // assign new vertex position if there is no vertex weight
      if (physicalProperty.weight == 0.0f)
      {
      	StoreVectorInBuffer( influencedPosition, pVertexBuffer );
      }
    }
    else
    {
      StoreVectorInBuffer( influencedPosition, pVertexBuffer );
    }

    // next vertex position in buffer
    pVertexBuffer = (float *)(((char *)pVertexBuffer) + stride) ;
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

int CalPhysiqueDualQuat::calculateNormals(CalSubmesh *pSubmesh, float *pNormalBuffer, int stride) const
{
  if(stride <= 0)
  {
    stride = 3*sizeof(float);
  }

  // get bone vector of the skeleton
  CalBone *const *vectorBone = &m_pModel->getSkeleton()->getVectorBone()[0];

  // get vertex vector of the submesh
  const CalCoreSubmesh::Vertex *vectorVertex = &pSubmesh->getCoreSubmesh()->getVectorVertex()[0];

  // get the number of vertices
  const int vertexCount = pSubmesh->getVertexCount();

  // Get IDs of morph targets with nonzero weight
  std::vector<int> morphIDs;
  GetUsedMorphTargetIDs( pSubmesh, morphIDs );

  // calculate the base weight
  float baseWeight = CalcMorphBaseWeight( pSubmesh, morphIDs );

  // calculate normal for all submesh vertices
  int vertexId;
  for (vertexId = 0; vertexId < vertexCount; ++vertexId)
  {
    // get the vertex
    const CalCoreSubmesh::Vertex& vertex = vectorVertex[vertexId];

    // blend the morph targets
    CalVector morphedNormal;
    CalcMorphBlendedNormal( pSubmesh, vertexId, baseWeight, morphIDs,
      morphedNormal );

    // blend together all vertex influences
    CalVector	influencedNormal;
    CalcInfluencedNormal( morphedNormal, vertex.vectorInfluence,
      vectorBone, influencedNormal );

    // re-normalize normal if necessary
    if (m_Normalize)
    {
      influencedNormal.x /= m_axisFactorX;
      influencedNormal.y /= m_axisFactorY;
      influencedNormal.z /= m_axisFactorZ;
      influencedNormal.normalize();
    }

    StoreVectorInBuffer( influencedNormal, pNormalBuffer );

    // next vertex position in buffer
    pNormalBuffer = (float *)(((char *)pNormalBuffer) + stride) ;
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

int CalPhysiqueDualQuat::calculateVerticesAndNormals(CalSubmesh *pSubmesh, float *pVertexBuffer, int stride) const
{
  if(stride <= 0)
  {
    stride = 6*sizeof(float);
  }

  // get bone vector of the skeleton
  CalBone *const *vectorBone = &m_pModel->getSkeleton()->getVectorBone()[0];

  // get vertex vector of the core submesh
  const CalCoreSubmesh::Vertex *vectorVertex = &pSubmesh->getCoreSubmesh()->getVectorVertex()[0];

  // get physical property vector of the core submesh
  std::vector<CalCoreSubmesh::PhysicalProperty>& vectorPhysicalProperty = pSubmesh->getCoreSubmesh()->getVectorPhysicalProperty();

  // get the number of vertices
  const int vertexCount = pSubmesh->getVertexCount();

  // Get IDs of morph targets with nonzero weight
  std::vector<int> morphIDs;
  GetUsedMorphTargetIDs( pSubmesh, morphIDs );

  // calculate the base weight
  float baseWeight = CalcMorphBaseWeight( pSubmesh, morphIDs );

  // Check for spring case
  bool hasSpringsAndInternalData =
    (pSubmesh->getCoreSubmesh()->getSpringCount() > 0) &&
    pSubmesh->hasInternalData();

  // calculate all submesh vertices
  int vertexId;
  CalCoreSubMorphTarget::BlendVertex	blendVertex;
  for(vertexId = 0; vertexId < vertexCount; ++vertexId)
  {
    // get the vertex
    const CalCoreSubmesh::Vertex& vertex = vectorVertex[vertexId];

    // blend the morph targets
    CalVector morphedPosition;
    CalVector morphedNormal;
    CalcMorphBlendedPositionAndNormal( pSubmesh, vertexId, baseWeight, morphIDs,
      morphedPosition, morphedNormal );

    // blend influences by bones
    CalVector	influencedPosition;
    CalcInfluencedPosition( morphedPosition, vertex.vectorInfluence,
      vectorBone, influencedPosition );
    influencedPosition.x *= m_axisFactorX;
    influencedPosition.y *= m_axisFactorY;
    influencedPosition.z *= m_axisFactorZ;

    CalVector	influencedNormal;
    CalcInfluencedNormal( morphedNormal, vertex.vectorInfluence,
      vectorBone, influencedNormal );

    // re-normalize normal if necessary
    if (m_Normalize)
    {
      influencedNormal.x /= m_axisFactorX;
      influencedNormal.y /= m_axisFactorY;
      influencedNormal.z /= m_axisFactorZ;
      influencedNormal.normalize();
    }

    // save vertex position
    if(hasSpringsAndInternalData)
    {
      // get the physical property of the vertex
      CalCoreSubmesh::PhysicalProperty& physicalProperty = vectorPhysicalProperty[vertexId];

      // assign new vertex position if there is no vertex weight
      if(physicalProperty.weight == 0.0f)
      {
        pVertexBuffer[0] = influencedPosition.x * m_axisFactorX;
        pVertexBuffer[1] = influencedPosition.y * m_axisFactorY;
        pVertexBuffer[2] = influencedPosition.z * m_axisFactorZ;
      }
    }
    else
    {
      pVertexBuffer[0] = influencedPosition.x * m_axisFactorX;
      pVertexBuffer[1] = influencedPosition.y * m_axisFactorY;
      pVertexBuffer[2] = influencedPosition.z * m_axisFactorZ;
    }

    pVertexBuffer[3] = influencedNormal.x;
    pVertexBuffer[4] = influencedNormal.y;
    pVertexBuffer[5] = influencedNormal.z;

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


int CalPhysiqueDualQuat::calculateVerticesNormalsAndTexCoords(CalSubmesh *pSubmesh, float *pVertexBuffer, int NumTexCoords) const
{
  // get bone vector of the skeleton
  CalBone *const *vectorBone = &m_pModel->getSkeleton()->getVectorBone()[0];

  // get vertex vector of the core submesh
  const CalCoreSubmesh::Vertex *vectorVertex = &pSubmesh->getCoreSubmesh()->getVectorVertex()[0];

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
  const int vertexCount = pSubmesh->getVertexCount();

  // Get IDs of morph targets with nonzero weight
  std::vector<int>	morphIDs;
  GetUsedMorphTargetIDs( pSubmesh, morphIDs );

  // calculate the base weight
  float baseWeight = CalcMorphBaseWeight( pSubmesh, morphIDs );

  // Check for spring case
  bool hasSpringsAndInternalData =
    (pSubmesh->getCoreSubmesh()->getSpringCount() > 0) &&
    pSubmesh->hasInternalData();

  // calculate all submesh vertices
  int vertexId;
  CalCoreSubMorphTarget::BlendVertex blendVertex;
  for(vertexId = 0; vertexId < vertexCount; ++vertexId)
  {
    // get the vertex
    const CalCoreSubmesh::Vertex& vertex = vectorVertex[vertexId];

    // blend the morph targets
    CalVector morphedPosition;
    CalVector morphedNormal;
    CalcMorphBlendedPositionAndNormal( pSubmesh, vertexId, baseWeight, morphIDs,
      morphedPosition, morphedNormal );

    // blend influences by bones
    CalVector	influencedPosition;
    CalcInfluencedPosition( morphedPosition, vertex.vectorInfluence,
      vectorBone, influencedPosition );
    influencedPosition.x *= m_axisFactorX;
    influencedPosition.y *= m_axisFactorY;
    influencedPosition.z *= m_axisFactorZ;

    CalVector	influencedNormal;
    CalcInfluencedNormal( morphedNormal, vertex.vectorInfluence,
      vectorBone, influencedNormal );

    // re-normalize normal if necessary
    if (m_Normalize)
    {
      influencedNormal.x /= m_axisFactorX;
      influencedNormal.y /= m_axisFactorY;
      influencedNormal.z /= m_axisFactorZ;
      influencedNormal.normalize();
    }

    // save vertex position
    if(hasSpringsAndInternalData)
    {
      // get the physical property of the vertex
      CalCoreSubmesh::PhysicalProperty& physicalProperty = vectorPhysicalProperty[vertexId];

      // assign new vertex position if there is no vertex weight
      if(physicalProperty.weight == 0.0f)
      {
        pVertexBuffer[0] = influencedPosition.x * m_axisFactorX;
        pVertexBuffer[1] = influencedPosition.y * m_axisFactorY;
        pVertexBuffer[2] = influencedPosition.z * m_axisFactorZ;
      }
    }
    else
    {
      pVertexBuffer[0] = influencedPosition.x * m_axisFactorX;
      pVertexBuffer[1] = influencedPosition.y * m_axisFactorY;
      pVertexBuffer[2] = influencedPosition.z * m_axisFactorZ;
    }

    pVertexBuffer[3] = influencedNormal.x;
    pVertexBuffer[4] = influencedNormal.y;
    pVertexBuffer[5] = influencedNormal.z;

    // next vertex position in buffer	
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
