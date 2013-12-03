//****************************************************************************//
// coresubmorphtarget.h                                                       //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_CORESUBMORPHTARGET_H
#define CAL_CORESUBMORPHTARGET_H


#include "cal3d/global.h"
#include "cal3d/vector.h"
#include "cal3d/refcounted.h"
#include "cal3d/refptr.h"
#include "cal3d/coresubmesh.h"

class CAL3D_API CalCoreSubMorphTarget
{
public:
  struct BlendVertex
  {
    CalVector position;
    CalVector normal;
    std::vector<CalCoreSubmesh::TextureCoordinate> textureCoords;
    bool create() { return true; }
    bool destroy() { return true; }
  };
  
public:
  CalCoreSubMorphTarget();
  virtual ~CalCoreSubMorphTarget() { }
  
  virtual void setCoreSubmesh( CalCoreSubmesh *inCoreSubmesh );
  const CalCoreSubmesh *getCoreSubmesh() const;

  int getBlendVertexCount() const;
  unsigned int size();
  typedef std::vector<BlendVertex *> VectorBlendVertex;

  std::vector<BlendVertex *>& getVectorBlendVertex();
  inline bool hasBlendVertex( int blendVertexId ) {
    return m_vectorBlendVertex[blendVertexId] != NULL;
  }
  inline BlendVertex const * getBlendVertex( int blendVertexId ) {
    return m_vectorBlendVertex[blendVertexId];
  }

  inline const BlendVertex* getBlendVertex( int blendVertexId ) const {
     return m_vectorBlendVertex[blendVertexId];
  }

  const std::vector<BlendVertex*>& getVectorBlendVertex() const;
  virtual bool reserve(int blendVertexCount);
  bool setBlendVertex(int vertexId, const BlendVertex& vertex);

  void	getBlendVertex( int vertexId, BlendVertex& outVertex ) const;
  CalMorphTargetType morphTargetType() const;

  void setName( std::string );
  std::string name() const;

private:
  CalCoreSubMorphTarget( const CalCoreSubMorphTarget& inOther );	// unimp

  std::vector<BlendVertex*>  m_vectorBlendVertex;
  CalCoreSubmesh           *m_coreSubmesh;
  std::string               m_morphTargetName;
  CalMorphTargetType        m_morphTargetType;
};

// The difference map is reference counted because we can use the same difference
// map on multiple actor heads.
class CalSharedDifferenceMap : public cal3d::RefCounted
{
public:
			CalSharedDifferenceMap() : m_NextIndex(0),
					m_vectorVertexIndexSize(0),
					m_vectorVertexIndexPtr(0),
					m_vectorBlendVertexPtr(0) { }

  bool reserve(int blendVertexCount);
  bool appendBlendVertex(int vertexId, const CalCoreSubMorphTarget::BlendVertex& vertex);
  
  bool	getBlendVertex( int vertexId, CalCoreSubMorphTarget::BlendVertex& outVertex ) const;

protected:
			~CalSharedDifferenceMap() { }

private:
  std::vector<CalCoreSubMorphTarget::BlendVertex>   m_vectorBlendVertex;
  std::vector<int>                                  m_vectorVertexIndex;
  mutable	int                                       m_NextIndex;

  // performance optimization: direct access to vector size and arrays
  mutable int                                       m_vectorVertexIndexSize;
  mutable const int                                *m_vectorVertexIndexPtr;
  mutable const CalCoreSubMorphTarget::BlendVertex *m_vectorBlendVertexPtr;
};
typedef cal3d::RefPtr<CalSharedDifferenceMap> CalSharedDifferenceMapPtr;


class CalCoreSubMorphTargetDiffMap : public CalCoreSubMorphTarget
{
public:
			CalCoreSubMorphTargetDiffMap();
			CalCoreSubMorphTargetDiffMap( const CalCoreSubMorphTargetDiffMap& inOther );
			~CalCoreSubMorphTargetDiffMap() { }

	virtual bool reserve(int blendVertexCount);
	virtual void	setCoreSubmesh( CalCoreSubmesh* inCoreSubmesh );

	bool appendBlendVertex(int vertexId, const CalCoreSubMorphTarget::BlendVertex& vertex);

private:
	CalSharedDifferenceMapPtr	m_diffMap;
};

#endif
//****************************************************************************//
