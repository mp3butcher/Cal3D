//****************************************************************************//
// coreanimation.h                                                            //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_COREANIMATION_H
#define CAL_COREANIMATION_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "cal3d/global.h"

//****************************************************************************//
// Forward declarations                                                       //
//****************************************************************************//

class CalCoreTrack;

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

 /*****************************************************************************/
/** The core animation class.
  *****************************************************************************/

class CAL3D_API CalCoreAnimation
{
// member variables
protected:
  float m_duration;
  std::list<CalCoreTrack *> m_listCoreTrack;

// constructors/destructor
public:
  CalCoreAnimation();
  virtual ~CalCoreAnimation();

// member functions	
public:
  bool addCoreTrack(CalCoreTrack *pCoreTrack);
  bool create();
  void destroy();
  CalCoreTrack *getCoreTrack(int coreBoneId);
  float getDuration();
  std::list<CalCoreTrack *>& getListCoreTrack();
  void setDuration(float duration);
};

#endif

//****************************************************************************//
