//****************************************************************************//
// mixer.h                                                                    //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_MIXER_H
#define CAL_MIXER_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "cal3d/global.h"

//****************************************************************************//
// Forward declarations                                                       //
//****************************************************************************//

class CalModel;
class CalAnimation;
class CalAnimationAction;
class CalAnimationCycle;

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

 /*****************************************************************************/
/** The mixer class.
  *****************************************************************************/

class CAL3D_API CalMixer
{
// member variables
public:
  CalModel *m_pModel;
  std::vector<CalAnimation *> m_vectorAnimation;
  std::list<CalAnimationAction *> m_listAnimationAction;
  std::list<CalAnimationCycle *> m_listAnimationCycle;
  float m_animationTime;
  float m_animationDuration;

// constructors/destructor
public:
  CalMixer();
  virtual ~CalMixer();

// member functions	
public:
  bool blendCycle(int id, float weight, float delay);
  bool clearCycle(int id, float delay);
  bool create(CalModel *pModel);
  void destroy();
  bool executeAction(int id, float delayIn, float delayOut);
  void updateAnimation(float deltaTime);
  void updateSkeleton();
};

#endif

//****************************************************************************//
