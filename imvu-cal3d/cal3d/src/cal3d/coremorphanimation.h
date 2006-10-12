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
#define CAL_COREMOPRHANIMATION_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "cal3d/global.h"

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

 /*****************************************************************************/
/** The core morph animation class.
  *****************************************************************************/

class CAL3D_API CalCoreMorphAnimation
{
// member variables
protected:
  std::vector<int> m_vectorCoreMeshID;
  std::vector<int> m_vectorMorphTargetID;

// constructors/destructor
public:
  CalCoreMorphAnimation();
  virtual ~CalCoreMorphAnimation();

// member functions	
public:
  bool addMorphTarget(int coreMeshID,int morphTargetID);
  bool create();
  void destroy();
  std::vector<int>& getVectorCoreMeshID();
  std::vector<int>& getVectorMorphTargetID();
};

#endif

//****************************************************************************//
