//****************************************************************************//
// corekeyframe.h                                                             //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_COREKEYFRAME_H
#define CAL_COREKEYFRAME_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "global.h"
#include "vector.h"
#include "quaternion.h"

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

 /*****************************************************************************/
/** The core keyframe class.
  *****************************************************************************/

class CAL3D_API CalCoreKeyframe
{
// member variables
protected:
  float m_time;
  CalVector m_translation;
  CalQuaternion m_rotation;

// constructors/destructor
public:
	CalCoreKeyframe();
	virtual ~CalCoreKeyframe();
	
// member functions
public:
  bool create();
  void destroy();
  const CalQuaternion& getRotation();
  float getTime();
  const CalVector& getTranslation();
  void setRotation(const CalQuaternion& rotation);
  void setTime(float time);
  void setTranslation(const CalVector& translation);
};

#endif

//****************************************************************************//
