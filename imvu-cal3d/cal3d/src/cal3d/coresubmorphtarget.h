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

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "cal3d/global.h"
#include "cal3d/vector.h"
#include "cal3d/coresubmesh.h"

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

 /*****************************************************************************/
/** The core sub morph target class.
  *****************************************************************************/

class CAL3D_API CalCoreSubMorphTarget
{
// misc
public:
  /// The core sub morph target BlendVertex.
  typedef struct
  {
    CalVector position;
    CalVector normal;
    std::vector<CalCoreSubmesh::TextureCoordinate> textureCoords;
    bool create() { return true; }
    bool destroy() { return true; }
  } BlendVertex;
  
// member variables
protected:
  std::vector<BlendVertex *> m_vectorBlendVertex;
  std::string m_morphTargetName;
  CalMorphTargetType m_morphTargetType;

// constructors/destructor
public:
  CalCoreSubMorphTarget();
  virtual ~CalCoreSubMorphTarget();

// member functions
public:
  typedef std::vector<BlendVertex *> VectorBlendVertex;
  bool create();
  unsigned int size();
  void destroy();
  int getBlendVertexCount();
  std::vector<BlendVertex *>& getVectorBlendVertex();
  inline bool hasBlendVertex( int blendVertexId ) {
    return m_vectorBlendVertex[blendVertexId] != NULL;
  }
  inline BlendVertex const * getBlendVertex( int blendVertexId ) {
    return m_vectorBlendVertex[blendVertexId];
  }
  bool reserve(int blendVertexCount);
  bool setBlendVertex(int vertexId, const BlendVertex& vertex);
  CalMorphTargetType morphTargetType();

  void setName( std::string );
  std::string name() const;
};
#endif
//****************************************************************************//
