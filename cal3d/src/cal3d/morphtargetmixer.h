//****************************************************************************//
// morphtargetmixer.h                                                         //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_MORPHTARGETMIXER_H
#define CAL_MORPHTARGETMIXER_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "cal3d/global.h"

//****************************************************************************//
// Forward declarations                                                       //
//****************************************************************************//

class CalMesh;

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

 /*****************************************************************************/
/** The morph target mixer class.
  *****************************************************************************/
class CAL3D_API CalMorphTargetMixer
{
// member variables
protected:
  std::vector<float> m_vectorCurrentWeight;
  std::vector<float> m_vectorEndWeight;
  float m_duration;
  CalMesh *m_pMesh;

// constructors/destructor
public:
  CalMorphTargetMixer();
  virtual ~CalMorphTargetMixer();

// member functions
public:
  bool blend(int id, float weight, float delay);
  bool blendBase(float weight, float delay);
  bool clear(int id, float delay);
  bool clearBase(float delay);
  float getCurrentWeight(int id);
  float getCurrentWeightBase();
  int getMorphTargetCount();
  bool create(CalMesh *pMesh);
  void destroy();
  void update(float deltaTime);
};

#endif

//****************************************************************************//
