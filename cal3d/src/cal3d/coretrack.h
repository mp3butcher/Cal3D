//****************************************************************************//
// coretrack.h                                                                //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_CORETRACK_H
#define CAL_CORETRACK_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "global.h"
#include "vector.h"
#include "quaternion.h"

//****************************************************************************//
// Forward declarations                                                       //
//****************************************************************************//

class CalCoreBone;
class CalCoreKeyframe;

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

 /*****************************************************************************/
/** The core track class.
  *****************************************************************************/

class CAL3D_API CalCoreTrack
{
// member variables
protected:
  int m_coreBoneId;
  std::map<float, CalCoreKeyframe *> m_mapCoreKeyframe;

// constructors/destructor
public:
  CalCoreTrack();
	virtual ~CalCoreTrack();

// member functions	
public:
  bool addCoreKeyframe(CalCoreKeyframe *pCoreKeyframe);
  bool create();
  void destroy();
  int getCoreBoneId();
  std::map<float, CalCoreKeyframe *>& getMapCoreKeyframe();
  bool getState(float time, float duration, CalVector& translation, CalQuaternion& rotation);
  bool setCoreBoneId(int coreBoneId);
};

#endif

//****************************************************************************//
