//****************************************************************************//
// coresubmesh.h                                                              //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//           (C) 2002 Laurent 'Maxun' Desmecht                                //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_CORESUBMESH_H
#define CAL_CORESUBMESH_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "global.h"
#include "vector.h"

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

 /*****************************************************************************/
/** The core submesh class.
  *****************************************************************************/

class CAL3D_API CalCoreSubmesh
{
// misc
public:
  /// The core submesh TextureCoordinate.
  typedef struct
  {
    float u, v;
  } TextureCoordinate;

  /// The core submesh Influence.
  typedef struct
  {
    int boneId;
    float weight;
  } Influence;

  /// The core submesh PhysicalProperty.
  typedef struct
  {
    float weight;
  } PhysicalProperty;

  /// The core submesh Vertex.
  typedef struct
  {
    CalVector position;
    CalVector normal;
    std::vector<Influence> vectorInfluence;
    int collapseId;
    int faceCollapseCount;
  } Vertex;

  /// The core submesh Face.
  typedef struct
  {
#ifdef CAL_16BIT_INDICES
    unsigned short vertexId[3];
#else
    int vertexId[3];
#endif
  } Face;
  
  /// The core submesh Spring.
  typedef struct
  {
    int vertexId[2];
    float springCoefficient;
    float idleLength;
  } Spring;

// member variables
protected:
  std::vector<Vertex> m_vectorVertex;
  std::vector<std::vector<TextureCoordinate> > m_vectorvectorTextureCoordinate;
  std::vector<PhysicalProperty> m_vectorPhysicalProperty;
  std::vector<Face> m_vectorFace;
  std::vector<Spring> m_vectorSpring;
  int m_coreMaterialThreadId;
  int m_lodCount;

// constructors/destructor
public:
  CalCoreSubmesh();
  virtual ~CalCoreSubmesh();

// member functions	
public:
  bool create();
  void destroy();
  int getCoreMaterialThreadId();
  int getFaceCount();
  int getLodCount();
  int getSpringCount();
  std::vector<Face>& getVectorFace();
  std::vector<PhysicalProperty>& getVectorPhysicalProperty();
  std::vector<Spring>& getVectorSpring();
  std::vector<std::vector<TextureCoordinate> >& getVectorVectorTextureCoordinate();
  std::vector<Vertex>& getVectorVertex();
  int getVertexCount();
  bool reserve(int vertexCount, int textureCoordinateCount, int faceCount, int springCount);
  void setCoreMaterialThreadId(int coreMaterialThreadId);
  bool setFace(int faceId, const Face& face);
  void setLodCount(int lodCount);
  bool setPhysicalProperty(int vertexId, const PhysicalProperty& physicalProperty);
  bool setSpring(int springId, const Spring& spring);
  bool setTextureCoordinate(int vertexId, int textureCoordinateId, const TextureCoordinate& textureCoordinate);
  bool setVertex(int vertexId, const Vertex& vertex);
};

#endif

//****************************************************************************//
