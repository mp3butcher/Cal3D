//****************************************************************************//
// coresubmesh.cpp                                                            //
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

#include "coresubmesh.h"

 /*****************************************************************************/
/** Constructs the core submesh instance.
  *
  * This function is the default constructor of the core submesh instance.
  *****************************************************************************/

CalCoreSubmesh::CalCoreSubmesh()
{
  m_coreMaterialThreadId = 0;
  m_lodCount = 0;
}

 /*****************************************************************************/
/** Destructs the core submesh instance.
  *
  * This function is the destructor of the core submesh instance.
  *****************************************************************************/

CalCoreSubmesh::~CalCoreSubmesh()
{
  assert(m_vectorFace.empty());
  assert(m_vectorVertex.empty());
  assert(m_vectorPhysicalProperty.empty());
  assert(m_vectorvectorTextureCoordinate.empty());
  assert(m_vectorSpring.empty());
}

 /*****************************************************************************/
/** Creates the core submesh instance.
  *
  * This function creates the core submesh instance.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalCoreSubmesh::create()
{
  return true;
}

 /*****************************************************************************/
/** Destroys the core submesh instance.
  *
  * This function destroys all data stored in the core submesh instance and
  * frees all allocated memory.
  *****************************************************************************/

void CalCoreSubmesh::destroy()
{
  // destroy all data
  m_vectorFace.clear();
  m_vectorVertex.clear();
  m_vectorPhysicalProperty.clear();
  m_vectorvectorTextureCoordinate.clear();
  m_vectorSpring.clear();
}

 /*****************************************************************************/
/** Returns the ID of the core material thread.
  *
  * This function returns the ID of the core material thread of this core
  * submesh instance.
  *
  * @return The ID of the core material thread.
  *****************************************************************************/

int CalCoreSubmesh::getCoreMaterialThreadId()
{
  return m_coreMaterialThreadId;
}

 /*****************************************************************************/
/** Returns the number of faces.
  *
  * This function returns the number of faces in the core submesh instance.
  *
  * @return The number of faces.
  *****************************************************************************/

int CalCoreSubmesh::getFaceCount()
{
  return m_vectorFace.size();
}

 /*****************************************************************************/
/** Returns the number of LOD steps.
  *
  * This function returns the number of LOD steps in the core submesh instance.
  *
  * @return The number of LOD steps.
  *****************************************************************************/

int CalCoreSubmesh::getLodCount()
{
  return m_lodCount;
}

 /*****************************************************************************/
/** Returns the number of springs.
  *
  * This function returns the number of springs in the core submesh instance.
  *
  * @return The number of springs.
  *****************************************************************************/

int CalCoreSubmesh::getSpringCount()
{
  return m_vectorSpring.size();
}

 /*****************************************************************************/
/** Returns the face vector.
  *
  * This function returns the vector that contains all faces of the core submesh
  * instance.
  *
  * @return A reference to the face vector.
  *****************************************************************************/

std::vector<CalCoreSubmesh::Face>& CalCoreSubmesh::getVectorFace()
{
  return m_vectorFace;
}

 /*****************************************************************************/
/** Returns the physical property vector.
  *
  * This function returns the vector that contains all physical properties of
  * the core submesh instance.
  *
  * @return A reference to the physical property vector.
  *****************************************************************************/

std::vector<CalCoreSubmesh::PhysicalProperty>& CalCoreSubmesh::getVectorPhysicalProperty()
{
  return m_vectorPhysicalProperty;
}

 /*****************************************************************************/
/** Returns the spring vector.
  *
  * This function returns the vector that contains all springs of the core
  * submesh instance.
  *
  * @return A reference to the spring vector.
  *****************************************************************************/

std::vector<CalCoreSubmesh::Spring>& CalCoreSubmesh::getVectorSpring()
{
  return m_vectorSpring;
}

 /*****************************************************************************/
/** Returns the texture coordinate vector-vector.
  *
  * This function returns the vector that contains all texture coordinate
  * vectors of the core submesh instance. This vector contains another vector
  * because there can be more than one texture map at each vertex.
  *
  * @return A reference to the texture coordinate vector-vector.
  *****************************************************************************/

std::vector<std::vector<CalCoreSubmesh::TextureCoordinate> > & CalCoreSubmesh::getVectorVectorTextureCoordinate()
{
  return m_vectorvectorTextureCoordinate;
}

 /*****************************************************************************/
/** Returns the vertex vector.
  *
  * This function returns the vector that contains all vertices of the core
  * submesh instance.
  *
  * @return A reference to the vertex vector.
  *****************************************************************************/

std::vector<CalCoreSubmesh::Vertex>& CalCoreSubmesh::getVectorVertex()
{
  return m_vectorVertex;
}

 /*****************************************************************************/
/** Returns the number of vertices.
  *
  * This function returns the number of vertices in the core submesh instance.
  *
  * @return The number of vertices.
  *****************************************************************************/

int CalCoreSubmesh::getVertexCount()
{
  return m_vectorVertex.size();
}

 /*****************************************************************************/
/** Reserves memory for the vertices, faces and texture coordinates.
  *
  * This function reserves memory for the vertices, faces, texture coordinates
  * and springs of the core submesh instance.
  *
  * @param vertexCount The number of vertices that this core submesh instance
  *                    should be able to hold.
  * @param textureCoordinateCount The number of texture coordinates that this
  *                               core submesh instance should be able to hold.
  * @param faceCount The number of faces that this core submesh instance should
  *                  be able to hold.
  * @param springCount The number of springs that this core submesh instance
  *                  should be able to hold.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalCoreSubmesh::reserve(int vertexCount, int textureCoordinateCount, int faceCount, int springCount)
{
  // reserve the space needed in all the vectors
  m_vectorVertex.reserve(vertexCount);
  m_vectorVertex.resize(vertexCount);

  m_vectorvectorTextureCoordinate.reserve(textureCoordinateCount);
  m_vectorvectorTextureCoordinate.resize(textureCoordinateCount);

  int textureCoordinateId;
  for(textureCoordinateId = 0; textureCoordinateId < textureCoordinateCount; textureCoordinateId++)
  {
    m_vectorvectorTextureCoordinate[textureCoordinateId].reserve(vertexCount);
    m_vectorvectorTextureCoordinate[textureCoordinateId].resize(vertexCount);
  }

  m_vectorFace.reserve(faceCount);
  m_vectorFace.resize(faceCount);

  m_vectorSpring.reserve(springCount);
  m_vectorSpring.resize(springCount);

  // reserve the space for the physical properties if we have springs in the core submesh instance
  if(springCount > 0)
  {
    m_vectorPhysicalProperty.reserve(vertexCount);
    m_vectorPhysicalProperty.resize(vertexCount);
  }

  return true;
}

 /*****************************************************************************/
/** Sets the ID of the core material thread.
  *
  * This function sets the ID of the core material thread of the core submesh
  * instance.
  *
  * @param coreMaterialThreadId The ID of the core material thread that should
  *                             be set.
  *****************************************************************************/

void CalCoreSubmesh::setCoreMaterialThreadId(int coreMaterialThreadId)
{
  m_coreMaterialThreadId = coreMaterialThreadId;
}

 /*****************************************************************************/
/** Sets a specified face.
  *
  * This function sets a specified face in the core submesh instance.
  *
  * @param faceId  The ID of the face.
  * @param face The face that should be set.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalCoreSubmesh::setFace(int faceId, const Face& face)
{
  if((faceId < 0) || (faceId >= (int)m_vectorFace.size())) return false;

  m_vectorFace[faceId] = face;

  return true;
}

 /*****************************************************************************/
/** Sets the number of LOD steps.
  *
  * This function sets the number of LOD steps of the core submesh instance.
  *
  * @param lodCount The number of LOD steps that should be set.
  *****************************************************************************/

void CalCoreSubmesh::setLodCount(int lodCount)
{
  m_lodCount = lodCount;
}

 /*****************************************************************************/
/** Sets a specified physical property.
  *
  * This function sets a specified physical property in the core submesh
  * instance.
  *
  * @param vertexId  The ID of the vertex.
  * @param physicalProperty The physical property that should be set.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalCoreSubmesh::setPhysicalProperty(int vertexId, const PhysicalProperty& physicalProperty)
{
  if((vertexId < 0) || (vertexId >= (int)m_vectorPhysicalProperty.size())) return false;

  m_vectorPhysicalProperty[vertexId] = physicalProperty;

  return true;
}

 /*****************************************************************************/
/** Sets a specified spring.
  *
  * This function sets a specified spring in the core submesh instance.
  *
  * @param springId  The ID of the spring.
  * @param spring The spring that should be set.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalCoreSubmesh::setSpring(int springId, const Spring& spring)
{
  if((springId < 0) || (springId >= (int)m_vectorSpring.size())) return false;

  m_vectorSpring[springId] = spring;

  return true;
}

 /*****************************************************************************/
/** Sets a specified texture coordinate.
  *
  * This function sets a specified texture coordinate in the core submesh
  * instance.
  *
  * @param vertexId  The ID of the vertex.
  * @param textureCoordinateId  The ID of the texture coordinate.
  * @param textureCoordinate The texture coordinate that should be set.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalCoreSubmesh::setTextureCoordinate(int vertexId, int textureCoordinateId, const TextureCoordinate& textureCoordinate)
{
  if((textureCoordinateId < 0) || (textureCoordinateId >= (int)m_vectorvectorTextureCoordinate.size())) return false;
  if((vertexId < 0) || (vertexId >= (int)m_vectorvectorTextureCoordinate[textureCoordinateId].size())) return false;

  m_vectorvectorTextureCoordinate[textureCoordinateId][vertexId] = textureCoordinate;

  return true;
}

 /*****************************************************************************/
/** Sets a specified vertex.
  *
  * This function sets a specified vertex in the core submesh instance.
  *
  * @param vertexId  The ID of the vertex.
  * @param vertex The vertex that should be set.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalCoreSubmesh::setVertex(int vertexId, const Vertex& vertex)
{
  if((vertexId < 0) || (vertexId >= (int)m_vectorVertex.size())) return false;

  m_vectorVertex[vertexId] = vertex;

  return true;
}

//****************************************************************************//
