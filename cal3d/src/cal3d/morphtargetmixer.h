//****************************************************************************//
// morphtargetmixer.h                                                         //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
// 2008 Erik Johnson
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_MORPHTARGETMIXER_H
#define CAL_MORPHTARGETMIXER_H


#include "cal3d/global.h"
#include <cal3d/coremorphkeyframe.h>
#include <cal3d/coreanimatedmorph.h>

class CalModel;


class CAL3D_API CalMorphTargetMixer
{
public:
  CalMorphTargetMixer(CalModel *model);
  ~CalMorphTargetMixer() { }

  bool blend(int id, float weight, float delay);
  bool clear(int id, float delay);
  bool copy( const CalMorphTargetMixer& inOther );
  float getCurrentWeight(int id) const;
  //float getCurrentWeightBase() const;
  int getMorphTargetCount() const;
  void update(float deltaTime);

private:
  //std::vector<float> m_vectorCurrentWeight;
  //std::vector<float> m_vectorEndWeight;
  //std::vector<float> m_vectorDuration;
  CalModel          *m_pModel;

  float mPlayTime;
  int mPlayedAnimatedMorphID;

  void SetTrackWeights(const CalCoreAnimatedMorph& morph, float elapsedTime);

  void ApplyWeightToMorphMesh(const std::string &morphMeshName, float trackWeight);
  float CalcKeyframeWeight(const std::vector<CalCoreMorphKeyframe> &keyframes, float elapsedTime);
};

#endif

//****************************************************************************//
