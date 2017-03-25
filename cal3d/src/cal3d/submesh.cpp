//****************************************************************************//
// submesh.cpp                                                                //
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

#include "cal3d/submesh.h"
#include "cal3d/error.h"
#include "cal3d/coresubmesh.h"
#include "cal3d/coresubmorphtarget.h"

#include <string.h>
using namespace cal3d;
// For Exclusive type morph targets, we record a replacement attenuation after
// encountering the first Replace blend.  Until then, we recognize that we do
// not yet have a value by setting this field to this specific invalid value.
static float const ReplacementAttenuationNull = 100.0; // Any number not between zero and one.

CalSubmesh::CalSubmesh(CalCoreSubmesh *coreSubmesh)
{
    assert(coreSubmesh);

    m_pCoreSubmesh = coreSubmesh;

    // reserve memory for the face vector
    m_vectorFace.reserve(m_pCoreSubmesh->getFaceCount());
    m_vectorFace.resize(m_pCoreSubmesh->getFaceCount());

    // set the initial lod level
    setLodLevel(1.0f);

    // set the initial material id
    m_coreMaterialId = -1;

    //Setting the morph target weights
    m_vectorMorphTargetWeight.reserve(m_pCoreSubmesh->getCoreSubMorphTargetCount());
    m_vectorMorphTargetWeight.resize(m_pCoreSubmesh->getCoreSubMorphTargetCount());
    m_vectorAccumulatedWeight.reserve(m_pCoreSubmesh->getCoreSubMorphTargetCount());
    m_vectorAccumulatedWeight.resize(m_pCoreSubmesh->getCoreSubMorphTargetCount());
    m_vectorReplacementAttenuation.reserve(m_pCoreSubmesh->getCoreSubMorphTargetCount());
    m_vectorReplacementAttenuation.resize(m_pCoreSubmesh->getCoreSubMorphTargetCount());

    // Array is indexed by group, and there can't be more groups than there are morph targets.
    m_vectorSubMorphTargetGroupAttenuator.reserve(m_pCoreSubmesh->getCoreSubMorphTargetCount());
    m_vectorSubMorphTargetGroupAttenuator.resize(m_pCoreSubmesh->getCoreSubMorphTargetCount());
    m_vectorSubMorphTargetGroupAttenuation.reserve(m_pCoreSubmesh->getCoreSubMorphTargetCount());
    m_vectorSubMorphTargetGroupAttenuation.resize(m_pCoreSubmesh->getCoreSubMorphTargetCount());

    int morphTargetId;
    for(morphTargetId = 0; morphTargetId<m_pCoreSubmesh->getCoreSubMorphTargetCount(); ++morphTargetId)
    {
        m_vectorSubMorphTargetGroupAttenuator[ morphTargetId ] = -1; // No attenuator by default.
        m_vectorSubMorphTargetGroupAttenuation[ morphTargetId ] = 0.0f; // No attenuation by default.
        m_vectorMorphTargetWeight[morphTargetId] = 0.0f;
        m_vectorAccumulatedWeight[morphTargetId] = 0.0f;
        m_vectorReplacementAttenuation[morphTargetId] = ReplacementAttenuationNull;
    }

    // check if the submesh instance must handle the vertex and normal data internally
    if(m_pCoreSubmesh->getSpringCount() > 0)
    {
        m_vectorVertex.reserve(m_pCoreSubmesh->getVertexCount());
        m_vectorVertex.resize(m_pCoreSubmesh->getVertexCount());
        m_vectorNormal.reserve(m_pCoreSubmesh->getVertexCount());
        m_vectorNormal.resize(m_pCoreSubmesh->getVertexCount());

        m_vectorvectorTangentSpace.reserve(m_pCoreSubmesh->getVectorVectorTangentSpace().size());
        m_vectorvectorTangentSpace.resize(m_pCoreSubmesh->getVectorVectorTangentSpace().size());

        m_vectorPhysicalProperty.reserve(m_pCoreSubmesh->getVertexCount());
        m_vectorPhysicalProperty.resize(m_pCoreSubmesh->getVertexCount());

        // get the vertex vector of the core submesh
        std::vector<CalCoreSubmesh::Vertex>& vectorVertex = m_pCoreSubmesh->getVectorVertex();

        // copy the data from the core submesh as default values
        int vertexId;
        for(vertexId = 0; vertexId < m_pCoreSubmesh->getVertexCount(); ++vertexId)
        {
            // copy the vertex data
            m_vectorVertex[vertexId] = vectorVertex[vertexId].position;
            m_vectorPhysicalProperty[vertexId].position = vectorVertex[vertexId].position;
            m_vectorPhysicalProperty[vertexId].positionOld = vectorVertex[vertexId].position;

            // copy the normal data
            m_vectorNormal[vertexId] = vectorVertex[vertexId].normal;
        }

        m_bInternalData = true;
    }
    else
    {
        m_bInternalData = false;
    }
}



/*****************************************************************************/
/** Provides access to the face data.
  *
  * This function returns the face data (vertex indices) of the submesh
  * instance. The LOD setting of the submesh instance is taken into account.
  *
  * @param pFaceBuffer A pointer to the user-provided buffer where the face
  *                    data is written to.
  *
  * @return The number of faces written to the buffer.
  *****************************************************************************/
int CalSubmesh::getFaces(CalIndex *pFaceBuffer) const
{
    // copy the face vector to the face buffer
    memcpy(pFaceBuffer, &m_vectorFace[0], m_faceCount * sizeof(Face));

    return m_faceCount;
}

/*****************************************************************************/
/** Disable internal data (and thus springs system)
  *
  *****************************************************************************/

void CalSubmesh::disableInternalData()
{
    if(m_bInternalData)
    {
        m_vectorVertex.clear();
        m_vectorNormal.clear();
        m_vectorvectorTangentSpace.clear();
        m_vectorPhysicalProperty.clear();
        m_bInternalData=false;
    }

}

/*****************************************************************************/
/** Returns true if tangent vectors are enabled.
  *
  * This function returns true if the submesh contains tangent vectors.
  *
  * @return True if tangent vectors are enabled.
  *****************************************************************************/

bool CalSubmesh::isTangentsEnabled(int mapId) const
{
    return m_pCoreSubmesh->isTangentsEnabled(mapId);
}

/*****************************************************************************/
/** Enables (and calculates) or disables the storage of tangent spaces.
  *
  * This function enables or disables the storage of tangent space bases.
  *****************************************************************************/

bool CalSubmesh::enableTangents(int mapId, bool enabled)
{
    if(!m_pCoreSubmesh->enableTangents(mapId,enabled))
        return false;

    if(!m_bInternalData)
        return true;

    if(!enabled)
    {
        m_vectorvectorTangentSpace[mapId].clear();
        return true;
    }

    m_vectorvectorTangentSpace[mapId].reserve(m_pCoreSubmesh->getVertexCount());
    m_vectorvectorTangentSpace[mapId].resize(m_pCoreSubmesh->getVertexCount());

    // get the tangent space vector of the core submesh
    std::vector<CalCoreSubmesh::TangentSpace >& vectorTangentSpace = m_pCoreSubmesh->getVectorVectorTangentSpace()[mapId];

    // copy the data from the core submesh as default values
    int vertexId;
    for(vertexId = 0; vertexId < m_pCoreSubmesh->getVertexCount(); vertexId++)
    {
        // copy the tangent space data
        m_vectorvectorTangentSpace[mapId][vertexId].tangent=vectorTangentSpace[vertexId].tangent;
        m_vectorvectorTangentSpace[mapId][vertexId].crossFactor=vectorTangentSpace[vertexId].crossFactor;
    }

    return true;
}

/*****************************************************************************/
/** Sets the LOD level.
  *
  * This function sets the LOD level of the submesh instance.
  *
  * @param lodLevel The LOD level in the range [0.0, 1.0].
  *****************************************************************************/

void CalSubmesh::setLodLevel(float lodLevel)
{
    // clamp the lod level to [0.0, 1.0]
    if(lodLevel < 0.0f) lodLevel = 0.0f;
    if(lodLevel > 1.0f) lodLevel = 1.0f;

    // get the lod count of the core submesh
    int lodCount;
    lodCount = m_pCoreSubmesh->getLodCount();

    // calculate the target lod count
    lodCount = (int)((1.0f - lodLevel) * lodCount);

    // get vertex vector of the core submesh
    std::vector<CalCoreSubmesh::Vertex>& vectorVertex = m_pCoreSubmesh->getVectorVertex();
    int coreVertexCount = vectorVertex.size();
    const CalCoreSubmesh::Vertex*	coreVertexPtr = &vectorVertex[0];

    // calculate the new number of vertices
    m_vertexCount = coreVertexCount - lodCount;

    // get face vector of the core submesh
    std::vector<CalCoreSubmesh::Face>& vectorFace = m_pCoreSubmesh->getVectorFace();
    const CalCoreSubmesh::Face* coreFacePtr = &vectorFace[0];

    // calculate the new number of faces
    m_faceCount = vectorFace.size();

    int vertexId;
    for(vertexId = coreVertexCount - 1; vertexId >= m_vertexCount; --vertexId)
    {
        m_faceCount -= coreVertexPtr[vertexId].faceCollapseCount;
    }

    // fill the face vector with the collapsed vertex ids
    int faceId;
    Face*	myFacePtr = &m_vectorFace[0];
    for(faceId = 0; faceId < m_faceCount; ++faceId)
    {
        int vertexId;
        for(vertexId = 0; vertexId < 3; ++vertexId)
        {
            // get the vertex id
            CalIndex collapsedVertexId;
            collapsedVertexId = coreFacePtr[faceId].vertexId[vertexId];

            // collapse the vertex id until it fits into the current lod level
            while(collapsedVertexId >= m_vertexCount)
                collapsedVertexId = coreVertexPtr[collapsedVertexId].collapseId;

            // store the collapse vertex id in the submesh face vector
            myFacePtr[faceId].vertexId[vertexId] = collapsedVertexId;
        }
    }
}
  

//****************************************************************************//
