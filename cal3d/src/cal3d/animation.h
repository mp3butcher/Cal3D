//****************************************************************************//
// animation.h                                                                //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
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

#include "cal3d/global.h"

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
  enum Type
  {
    TYPE_NONE = 0,
    TYPE_CYCLE,
    TYPE_POSE,
    TYPE_ACTION
  };

  enum State
  {
    STATE_NONE = 0,
    STATE_SYNC,
    STATE_ASYNC,
    STATE_IN,
    STATE_STEADY,
    STATE_OUT
  };

// member variables
protected:
  CalCoreAnimation *m_pCoreAnimation;
  Type m_type;
  State m_state;
  float m_time;
  float m_timeFactor;
  float m_weight;

// constructors/destructor
protected:
  CalAnimation();
public:
  virtual ~CalAnimation();

// member functions	
public:
  virtual bool create(CalCoreAnimation *pCoreAnimation) = 0;
  virtual void destroy();
  CalCoreAnimation *getCoreAnimation();
  State getState();
  float getTime();
  Type getType();
  float getWeight();
};

#endif

//****************************************************************************//
