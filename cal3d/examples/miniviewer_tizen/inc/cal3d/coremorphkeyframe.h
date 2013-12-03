//****************************************************************************//
// coreMorphKeyframe.h                                                             //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_COREMORPHKEYFRAME_H
#define CAL_COREMORPHKEYFRAME_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "cal3d/global.h"
#include "cal3d/matrix.h"
#include "cal3d/vector.h"
#include "cal3d/quaternion.h"

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

 /*****************************************************************************/
/** The core keyframe class.
  *****************************************************************************/

class CAL3D_API CalCoreMorphKeyframe
{
// member variables
protected:
  float m_time;
  float m_weight;

// constructors/destructor
public:
  CalCoreMorphKeyframe();
  virtual ~CalCoreMorphKeyframe();

// member functions
public:
  bool create();
  void destroy();
  float getTime() const;
  void setTime(float time);

  float getWeight() const;
  void setWeight(float);
};

#endif

//****************************************************************************//
