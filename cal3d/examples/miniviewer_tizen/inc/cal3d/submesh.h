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


#include "cal3d/global.h"
#include "cal3d/vector.h"
#include "cal3d/coresubmesh.h"


class CalCoreSubmesh;


// Structure used to return an array of the morphs that have non-zero weights.
struct MorphIdAndWeight {
  int morphId_;
  float weight_;
};
class CAL3D_API CalSubmesh
{
public:
  struct PhysicalProperty
  {
    CalVector position;
    CalVector positionOld;
    CalVector force;
  };

  struct TangentSpace
  {
    CalVector tangent;
    float crossFactor;
  };

  struct Face
  {
    CalIndex vertexId[3];
  };

public:
  CalSubmesh(CalCoreSubmesh *coreSubmesh);
  ~CalSubmesh() { }

  CalCoreSubmesh *getCoreSubmesh();
  const CalCoreSubmesh *getCoreSubmesh() const;
  int getCoreMaterialId() const;
  int getFaceCount() const;
  int getFaces(CalIndex *pFaceBuffer) const;
  std::vector<CalVector>& getVectorNormal();
  const std::vector<CalVector>& getVectorNormal() const;
  std::vector<std::vector<TangentSpace> >& getVectorVectorTangentSpace();
  const std::vector<std::vector<TangentSpace> >& getVectorVectorTangentSpace() const;
  std::vector<PhysicalProperty>& getVectorPhysicalProperty();
  const std::vector<PhysicalProperty>& getVectorPhysicalProperty() const;
  std::vector<CalVector>& getVectorVertex();
  const std::vector<CalVector>& getVectorVertex() const;
  int getVertexCount() const;
  bool hasInternalData() const;
  void disableInternalData();
  void setCoreMaterialId(int coreMaterialId);
  void setLodLevel(float lodLevel);
  bool isTangentsEnabled(int mapId) const;
  bool enableTangents(int mapId, bool enabled);
  void setMorphTargetWeight(int blendId, float weight);
  float getMorphTargetWeight(int blendId) const;
  void setMorphTargetWeight(std::string const & morphName,float weight);
  int getMorphTargetWeightCount() const;
  bool getMorphTargetWeight(std::string const & morphName, float * weightOut);
  void getMorphIdAndWeightArray( MorphIdAndWeight * arrayResult, 
    unsigned int * numMiawsResult, 
    unsigned int maxMiaws );
  float getBaseWeight() const;
  std::vector<float>& getVectorMorphTargetWeight();
  const std::vector<float>& getVectorMorphTargetWeight() const;
  void clearMorphTargetScales();
  void clearMorphTargetState( std::string const & morphName );
  void blendMorphTargetScale( std::string const & morphName, 
    float scale, 
    float unrampedWeight, 
    float rampValue,
    bool replace );
  void setSubMorphTargetGroupAttenuatorArray( unsigned int len, int const * morphTargetIdArray );
  void setSubMorphTargetGroupAttenuationArray( unsigned int len, float const * attenuationArray );
private:
  CalCoreSubmesh                         *m_pCoreSubmesh;
  std::vector<float>                      m_vectorMorphTargetWeight;
  std::vector<float>                      m_vectorAccumulatedWeight;
  std::vector<float>                      m_vectorReplacementAttenuation;
  std::vector<CalVector>                  m_vectorVertex;
  std::vector<CalVector>                  m_vectorNormal;
  std::vector<std::vector<TangentSpace> > m_vectorvectorTangentSpace;
  std::vector<Face>                       m_vectorFace;
  std::vector<PhysicalProperty>           m_vectorPhysicalProperty;
  std::vector<int>                        m_vectorSubMorphTargetGroupAttenuator;
  std::vector<float>                      m_vectorSubMorphTargetGroupAttenuation;  
  int                                     m_vertexCount;
  int                                     m_faceCount;
  int                                     m_coreMaterialId;
  bool                                    m_bInternalData;
};

#endif
