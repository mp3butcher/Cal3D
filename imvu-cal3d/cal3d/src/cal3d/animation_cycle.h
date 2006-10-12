//****************************************************************************//
// animation_cycle.h                                                          //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_ANIMATION_CYCLE_H
#define CAL_ANIMATION_CYCLE_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "cal3d/global.h"
#include "cal3d/animation.h"

//****************************************************************************//
// Forward declarations                                                       //
//****************************************************************************//

class CalCoreAnimation;

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

 /*****************************************************************************/
/** The animation cycle class.
  *****************************************************************************/

class CAL3D_API CalAnimationCycle : public CalAnimation
{
// member variables
protected:
  float m_targetDelay;
  float m_targetWeight;

// constructors/destructor
public:
  CalAnimationCycle();
  virtual ~CalAnimationCycle();

// member functions	
public:
  bool blend(float weight, float delay);
  bool create(CalCoreAnimation *pCoreAnimation);
  void destroy();
  void setAsync(float time, float duration);
  bool update(float deltaTime);
};

#endif

//****************************************************************************//
