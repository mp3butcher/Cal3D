//****************************************************************************//
// coremorphanimation.h                                                       //
// Copyright (C) 2003 Steven Geens                                            //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_COREMORPHANIMATION_H
#define CAL_COREMORPHANIMATION_H

#include "cal3d/global.h"
#include "cal3d/refcounted.h"
#include "cal3d/refptr.h"

class CAL3D_API CalCoreMorphAnimation : public cal3d::RefCounted
{
public:
  CalCoreMorphAnimation()  { }
  ~CalCoreMorphAnimation() { }

  bool addMorphTarget(int coreMeshID, int morphTargetID);
  std::vector<int>& getVectorCoreMeshID();
  const std::vector<int>& getVectorCoreMeshID() const;
  std::vector<int>& getVectorMorphTargetID();
  const std::vector<int>& getVectorMorphTargetID() const;

  const std::string& getName() const;
  void setName( const std::string& name );

private:
  std::vector<int> m_vectorCoreMeshID;
  std::vector<int> m_vectorMorphTargetID;
  std::string      m_name;
};

typedef cal3d::RefPtr<CalCoreMorphAnimation> CalCoreMorphAnimationPtr;

#endif

//****************************************************************************//
