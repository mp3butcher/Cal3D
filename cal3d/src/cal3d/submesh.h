//****************************************************************************//
// submesh.h                                                                  //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_SUBMESH_H
#define CAL_SUBMESH_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "cal3d/global.h"
#include "cal3d/vector.h"

//****************************************************************************//
// Forward declarations                                                       //
//****************************************************************************//

class CalCoreSubmesh;

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

 /*****************************************************************************/
/** The submesh class.
  *****************************************************************************/

class CAL3D_API CalSubmesh
{
// misc
public:
  /// The submesh PhysicalProperty.
  typedef struct
  {
    CalVector position;
    CalVector positionOld;
    CalVector force;
  } PhysicalProperty;

  /// The submesh TangentSpace.
  struct TangentSpace
  {
    CalVector tangent;
    float crossFactor;
  };


  /// The submesh Face.
  typedef struct
  {
    CalIndex vertexId[3];
  } Face;

// member variables
protected:
  CalCoreSubmesh *m_pCoreSubmesh;
  std::vector<float> m_vectorMorphTargetWeight;
  std::vector<CalVector> m_vectorVertex;
  std::vector<CalVector> m_vectorNormal;
  std::vector<std::vector<TangentSpace> > m_vectorvectorTangentSpace;
  std::vector<Face> m_vectorFace;
  std::vector<PhysicalProperty> m_vectorPhysicalProperty;
  int m_vertexCount;
  int m_faceCount;
  int m_coreMaterialId;
  bool m_bInternalData;

// constructors/destructor
public:
  CalSubmesh();
  virtual ~CalSubmesh();

// member functions	
public:
  bool create(CalCoreSubmesh *pCoreSubmesh);
  void destroy();
  CalCoreSubmesh *getCoreSubmesh();
  int getCoreMaterialId();
  int getFaceCount();
  int getFaces(CalIndex *pFaceBuffer);
  std::vector<CalVector>& getVectorNormal();
  std::vector<std::vector<TangentSpace> >& getVectorVectorTangentSpace();
  std::vector<PhysicalProperty>& getVectorPhysicalProperty();
  std::vector<CalVector>& getVectorVertex();
  int getVertexCount();
  bool hasInternalData();
  void disableInternalData();
  void setCoreMaterialId(int coreMaterialId);
  void setLodLevel(float lodLevel);
  bool isTangentsEnabled(int mapId);
  bool enableTangents(int mapId, bool enabled);
  std::vector<float>& getVectorWeight();
  void setMorphTargetWeight(int blendId,float weight);
  float getMorphTargetWeight(int blendId);
  float getBaseWeight();
  int getMorphTargetWeightCount();
  std::vector<float>& getVectorMorphTargetWeight();

};

#endif

//****************************************************************************//
