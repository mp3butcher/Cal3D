//****************************************************************************//
// animation.h                                                                //
// Copyright (C) 2001 Bruno 'Beosil' Heidelberger                             //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_ANIMATION_H
#define CAL_ANIMATION_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "global.h"

//****************************************************************************//
// Forward declarations                                                       //
//****************************************************************************//

class CalCoreAnimation;

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

 /*****************************************************************************/
/** The animation class.
  *****************************************************************************/

class CAL3D_API CalAnimation
{
// misc
public:
  static const unsigned int TYPE_NONE;
  static const unsigned int TYPE_CYCLE;
  static const unsigned int TYPE_POSE;
  static const unsigned int TYPE_ACTION;

  static const unsigned int STATE_NONE;
  static const unsigned int STATE_SYNC;
  static const unsigned int STATE_ASYNC;
  static const unsigned int STATE_IN;
  static const unsigned int STATE_STEADY;
  static const unsigned int STATE_OUT;

// member variables
protected:
  CalCoreAnimation *m_pCoreAnimation;
  unsigned int m_type;
  unsigned int m_state;
  float m_time;
  float m_timeFactor;
  float m_weight;

// constructors/destructor
protected:
  CalAnimation();
  virtual ~CalAnimation();

// member functions	
public:
  virtual bool create(CalCoreAnimation *pCoreAnimation) = 0;
  virtual void destroy();
  CalCoreAnimation *getCoreAnimation();
  unsigned int getState();
  float getTime();
  unsigned int getType();
  float getWeight();
};

#endif

//****************************************************************************//
