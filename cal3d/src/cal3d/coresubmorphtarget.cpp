//****************************************************************************//
// coresubmorphtarget.cpp                                                     //
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


#include "cal3d/coresubmorphtarget.h"
#include "cal3d/coresubmesh.h"
#include <cstring>

//////////////////////////////////////////////////////////////////////////
CalCoreSubMorphTarget::CalCoreSubMorphTarget() :
 m_coreSubmesh( NULL ),
 m_morphTargetType(CalMorphTargetTypeAdditive)
{

}

/*****************************************************************************/
/** Returns the blend vertex vector.
  *
  * This function returns the vector that contains all blend vertices of the core
  * sub morph target instance.
  *
  * @return A reference to the blend vertex vector.
  *****************************************************************************/

std::vector<CalCoreSubMorphTarget::BlendVertex*>& CalCoreSubMorphTarget::getVectorBlendVertex()
{
  return m_vectorBlendVertex;
}

unsigned int
CalCoreSubMorphTarget::size()
{
  unsigned int r = sizeof( CalCoreSubMorphTarget );
  r += sizeof( CalMorphTargetType );

  // Assume single texture coordinate pair.
  r += ( sizeof( BlendVertex ) + sizeof( CalCoreSubmesh::TextureCoordinate ) ) * m_vectorBlendVertex.size();
  r += m_morphTargetName.size();
  return r;
}


 /*****************************************************************************/
/** Returns the blend vertex vector.
  *
  * This function returns the vector that contains all blend vertices of the core
  * sub morph target instance.
  *
  * @return A reference to the blend vertex vector.
  *****************************************************************************/

const std::vector<CalCoreSubMorphTarget::BlendVertex*>& CalCoreSubMorphTarget::getVectorBlendVertex() const
{
  return m_vectorBlendVertex;
}

 /*****************************************************************************/
/** Returns the number of blend vertices.
  *
  * This function returns the number of blend vertices in the
  * core sub morph target instance.
  *
  * @return The number of blend vertices.
  *****************************************************************************/

int CalCoreSubMorphTarget::getBlendVertexCount() const
{
  return m_vectorBlendVertex.size();
}

 /*****************************************************************************/
/** Record the core submesh that this morph applies to.
  *
  * This function associates the sub morph target with a core submesh.
  *
  * @param The core submesh.
  *****************************************************************************/

void CalCoreSubMorphTarget::setCoreSubmesh( CalCoreSubmesh *inCoreSubmesh )
{
	m_coreSubmesh = inCoreSubmesh;
}


 /*****************************************************************************/
/** Returns the core submesh associated with this morph.
  *
  * This function returns a pointer to the core submesh.
  *
  * @return The number of blend vertices.
  *****************************************************************************/

const CalCoreSubmesh *CalCoreSubMorphTarget::getCoreSubmesh() const
{
	return m_coreSubmesh;
}


 /*****************************************************************************/
/** Reserves memory for the blend vertices.
 *
 * This function reserves memory for the blend vertices
 * of the core sub morph target instance.
 *
 * @param blendVertexCount The number of blend vertices that
 *                    this core sub morph target instance should be able to hold.
 *
 * @return One of the following values:
 *         \li \b true if successful
 *         \li \b false if an error happened
 *****************************************************************************/

bool CalCoreSubMorphTarget::reserve(int blendVertexCount)
{
	bool	success = true;
	try
	{
		// reserve the space needed in all the vectors
		m_vectorBlendVertex.reserve(blendVertexCount);
		m_vectorBlendVertex.resize(blendVertexCount);
	}
	catch (...)
	{
		success = false;
	}
  for( unsigned int i = 0; i < m_vectorBlendVertex.size(); i++ ) {
    m_vectorBlendVertex[i] = NULL;
  }

	return success;
}



 /*****************************************************************************/
/** Sets a specified blend vertex.
  *
  * This function sets a specified blend vertex in the core sub morph target instance.
  *
  * @param vertexId  The ID of the vertex.
  * @param vertex The vertex that should be set.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happened
  *****************************************************************************/

bool CalCoreSubMorphTarget::setBlendVertex(int blendVertexId, const BlendVertex& blendVertex)
{
  if((blendVertexId < 0) || (blendVertexId >= (int)m_vectorBlendVertex.size())) return false;

  if( m_vectorBlendVertex[blendVertexId] == NULL ) {
    m_vectorBlendVertex[blendVertexId] = new BlendVertex();
  }
  m_vectorBlendVertex[blendVertexId]->position = blendVertex.position;
  m_vectorBlendVertex[blendVertexId]->normal = blendVertex.normal;
  m_vectorBlendVertex[blendVertexId]->textureCoords.clear();
  m_vectorBlendVertex[blendVertexId]->textureCoords.reserve(blendVertex.textureCoords.size());
  for( unsigned int tcI = 0; tcI < blendVertex.textureCoords.size(); tcI++ ) {
    m_vectorBlendVertex[blendVertexId]->textureCoords.push_back(blendVertex.textureCoords[tcI]);
  }

  return true;
}


 /*****************************************************************************/
/** Return type of morph target, which is determined from parsing the name.
  *
  * @return One of the enum values.
  *****************************************************************************/
CalMorphTargetType
CalCoreSubMorphTarget::morphTargetType() const
{
  return m_morphTargetType;
}
 /*****************************************************************************/
/** Returns one blend vertex.
  *
  * This function returns a blend vertex from this sub morph target.
  *
  * @param vertexId  The ID of the vertex.
  * @param outVertex Receives the blend vertex.
  *****************************************************************************/

void	CalCoreSubMorphTarget::getBlendVertex( int vertexId, BlendVertex& outVertex ) const
{
	outVertex = *m_vectorBlendVertex[ vertexId ];
}

//#pragma mark -

 /*****************************************************************************/
/** Reserves memory for the blend vertices.
 *
 * This function reserves memory for the blend vertices
 * of the difference map.
 *
 * @param blendVertexCount The number of blend vertices that
 *                    this difference map should be able to hold.
 *
 * @return One of the following values:
 *         \li \b true if successful
 *         \li \b false if an error happened
 *****************************************************************************/

bool CalSharedDifferenceMap::reserve(int blendVertexCount)
{
	bool	success = true;
	try
	{
		// reserve the space needed in all the vectors
		m_vectorBlendVertex.reserve(blendVertexCount);
		m_vectorVertexIndex.reserve(blendVertexCount);
	}
	catch (...)
	{
		success = false;
	}

	return success;
}

 /*****************************************************************************/
/** Record one blend vertex.
  *
  * This function adds a blend vertex to a sparse array. Vertices must be added
  * in increasing order of ID.
  *
  * @param vertexId  The ID of the vertex.
  * @param vertex    A blend vertex.
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happened
  *****************************************************************************/

bool CalSharedDifferenceMap::appendBlendVertex(int vertexID,
	const CalCoreSubMorphTarget::BlendVertex& vertex)
{
	if ( (vertexID < 0) || (m_vectorVertexIndex.size() >= m_vectorVertexIndex.capacity()) )
	{
		return false;
	}
	if ( (! m_vectorVertexIndex.empty()) && (m_vectorVertexIndex.back() >= vertexID) )
	{
		return false;
	}
	m_vectorVertexIndex.push_back( vertexID );
	m_vectorBlendVertex.push_back( vertex );
	return true;
}

 /*****************************************************************************/
/** Retrieve one blend vertex.
  *
  * This function gets a blend vertex from the difference map if it is present.
  *
  * If we were supporting random access, we would look up the vertex ID using
  * binary search.  But actually, the physique calls getBlendVertex
  * sequentially, so we optimize for that access.
  *
  * @param vertexId  The ID of the vertex.
  * @param outVertex A blend vertex.
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if the vertex is not present in the difference map
  *****************************************************************************/

bool	CalSharedDifferenceMap::getBlendVertex( int vertexId,
									CalCoreSubMorphTarget::BlendVertex& outVertex ) const
{
	// I assume that we do not call getBlendVertex until after we are done
	// appending vertices, so at this point we can cache direct access values
	// for the vectors.
	if (m_vectorVertexIndexPtr == NULL)
	{
		m_vectorVertexIndexPtr = &m_vectorVertexIndex[0];
		m_vectorVertexIndexSize = m_vectorVertexIndex.size();
		m_vectorBlendVertexPtr = &m_vectorBlendVertex[0];
	}

	// With sequential access, the normal situation will be that
	// (m_NextIndex == m_vectorVertexIndexSize) or (vertexId <= m_vectorVertexIndex[ m_NextIndex ])
	// and
	// either m_NextIndex == 0 or vertexId > m_vectorVertexIndex[ m_NextIndex - 1 ].
	// Make sure that condition holds.
	while ( (m_NextIndex < m_vectorVertexIndexSize) &&
		(vertexId > m_vectorVertexIndexPtr[ m_NextIndex ]) )
	{
		m_NextIndex += 1;
	}
	while ( (m_NextIndex > 0) && (vertexId <= m_vectorVertexIndexPtr[ m_NextIndex - 1 ]) )
	{
		m_NextIndex -= 1;
	}

	bool	didFind = false;

	if ( (m_NextIndex < m_vectorVertexIndexSize) &&
		(vertexId == m_vectorVertexIndexPtr[ m_NextIndex ] ) )
	{
		outVertex = m_vectorBlendVertexPtr[ m_NextIndex ];
		didFind = true;
		m_NextIndex += 1;
	}
	return didFind;
}

//#pragma mark -

CalCoreSubMorphTargetDiffMap::CalCoreSubMorphTargetDiffMap()
	: m_diffMap( new CalSharedDifferenceMap )
{
}

CalCoreSubMorphTargetDiffMap::CalCoreSubMorphTargetDiffMap( const CalCoreSubMorphTargetDiffMap& inOther )
	: m_diffMap( inOther.m_diffMap )
{
}

 /*****************************************************************************/
/** Reserves memory for the blend vertices.
 *
 * This function reserves memory for the blend vertices
 * of the difference map.
 *
 * @param blendVertexCount The number of blend vertices that
 *                    this sub morph target should be able to hold.
 *
 * @return One of the following values:
 *         \li \b true if successful
 *         \li \b false if an error happened
 *****************************************************************************/

bool CalCoreSubMorphTargetDiffMap::reserve(int blendVertexCount)
{
	return m_diffMap->reserve( blendVertexCount ) &&
		CalCoreSubMorphTarget::reserve( blendVertexCount );
}


 /*****************************************************************************/
/** Record the core submesh that this morph applies to.
  *
  * This function associates the sub morph target with a core submesh and caches
  * blend vertices using the difference map.
  *
  * @param The core submesh.
  *****************************************************************************/

void	CalCoreSubMorphTargetDiffMap::setCoreSubmesh( CalCoreSubmesh* inCoreSubmesh )
{
	CalCoreSubMorphTarget::setCoreSubmesh( inCoreSubmesh );

	// Cache the blend vertices computed from the core vertices and the
	// difference map.
	const std::vector<CalCoreSubmesh::Vertex>&	coreVerts(
		inCoreSubmesh->getVectorVertex() );
	const unsigned int kNumVerts = coreVerts.size();
	CalCoreSubMorphTarget::reserve( kNumVerts );
	unsigned int vertexId;
	BlendVertex	theVert, offset;
	for (vertexId = 0; vertexId < kNumVerts; ++vertexId)
	{
		theVert.position = coreVerts[vertexId].position;
		theVert.normal = coreVerts[vertexId].normal;
		if (m_diffMap->getBlendVertex( vertexId, offset ))
		{
			theVert.position += offset.position;
			theVert.normal += offset.normal;
		}
		setBlendVertex( vertexId, theVert );
	}
}


 /*****************************************************************************/
/** Record one blend vertex.
  *
  * This function adds a blend vertex to the difference map. Vertices must be added
  * in increasing order of ID.
  *
  * @param vertexId  The ID of the vertex.
  * @param vertex    A blend vertex.
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happened
  *****************************************************************************/

bool CalCoreSubMorphTargetDiffMap::appendBlendVertex(int vertexId,
	const CalCoreSubMorphTarget::BlendVertex& vertex)
{
	return m_diffMap->appendBlendVertex( vertexId, vertex );
}

//****************************************************************************//

void
CalCoreSubMorphTarget::setName( std::string s )
{
  m_morphTargetName = s;

  // If the name ends in ".Additive" or ".Exclusive" or ".Clamped"
  // or ".Average" then set the type of the morph target.  By
  // default it is Additive.
  m_morphTargetType = CalMorphTargetTypeAdditive;
  char const * s2 = s.c_str();
  char const * dot = strrchr( s2, '.' );
  if( dot ) {
    dot++;
    if( stricmp( dot, "exclusive" ) == 0 ) {
      m_morphTargetType = CalMorphTargetTypeExclusive;
    } else if( stricmp( dot, "additive" ) == 0 ) {
      m_morphTargetType = CalMorphTargetTypeAdditive;
    } else if( stricmp( dot, "clamped" ) == 0 ) {
      m_morphTargetType = CalMorphTargetTypeClamped;
    } else if( stricmp( dot, "average" ) == 0 ) {
      m_morphTargetType = CalMorphTargetTypeAverage;
    }
  }
}


std::string
CalCoreSubMorphTarget::name() const
{
  return m_morphTargetName;
}


