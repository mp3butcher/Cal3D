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
#include "matrix.h"
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
/*****************************************************************************/

class CAL3D_API CalCoreTrack
{
// member variables
protected:
  /// The index of the associated CoreBone in the CoreSkeleton.
  int m_coreBoneId;

  /// List of keyframes, always sorted by time.
  std::vector<CalCoreKeyframe*> m_keyframes;

// constructors/destructor
public:
  CalCoreTrack();
  virtual ~CalCoreTrack();

  bool create();
  void destroy();

  bool getState(float time, CalVector& translation, CalQuaternion& rotation);

  int getCoreBoneId();
  bool setCoreBoneId(int coreBoneId);
  
  int getCoreKeyframeCount();
  CalCoreKeyframe* getCoreKeyframe(int idx);

  bool addCoreKeyframe(CalCoreKeyframe *pCoreKeyframe);

private:
  std::vector<CalCoreKeyframe*>::iterator getUpperBound(float time);
};

#endif

//****************************************************************************//
