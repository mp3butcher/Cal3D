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
  } BlendVertex;
  
// member variables
protected:
  std::vector<BlendVertex> m_vectorBlendVertex;

// constructors/destructor
public:
  CalCoreSubMorphTarget();
  virtual ~CalCoreSubMorphTarget();

// member functions
public:
  bool create();
  void destroy();
  int getBlendVertexCount();
  std::vector<BlendVertex>& getVectorBlendVertex();
  bool reserve(int blendVertexCount);
  bool setBlendVertex(int vertexId, const BlendVertex& vertex);
};
#endif
//****************************************************************************//
