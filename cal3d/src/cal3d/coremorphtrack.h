//****************************************************************************//
// coreMorphTrack.h                                                                //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_COREMORPHTRACK_H
#define CAL_COREMORPHTRACK_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "cal3d/global.h"
#include "cal3d/matrix.h"
#include "cal3d/vector.h"
#include "cal3d/quaternion.h"
#include "cal3d/coremorphkeyframe.h"

//****************************************************************************//
// Forward declarations                                                       //
//****************************************************************************//

class CalCoreMorphKeyframe;

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

//*****************************************************************************/
//** The core track class.
//*****************************************************************************/

class CAL3D_API CalCoreMorphTrack
{
// member variables
protected:

  std::string m_morphName;

  /// List of keyframes, always sorted by time.
  std::vector<CalCoreMorphKeyframe> m_keyframes;
  std::vector<CalCoreMorphKeyframe*> m_keyframesToDelete;

// constructors/destructor
public:
  CalCoreMorphTrack();
  virtual ~CalCoreMorphTrack();

  bool create();
  void destroy();

  bool getState(float time, float & weightOut);

  std::string getMorphName() const;
  void setMorphName(std::string name);
  
  int getCoreMorphKeyframeCount() const;
  void reserve(int);
  
  CalCoreMorphKeyframe* getCoreMorphKeyframe(int idx);
  const CalCoreMorphKeyframe* getCoreMorphKeyframe(int idx) const;

  bool addCoreMorphKeyframe(CalCoreMorphKeyframe *pCoreKeyframe);
  
  const std::vector<CalCoreMorphKeyframe> & getVectorCoreMorphKeyframes() const;
  std::vector<CalCoreMorphKeyframe> & getVectorCoreMorphKeyframes();

  void scale(float factor);

private:
  std::vector<CalCoreMorphKeyframe>::iterator getUpperBound(float time);
};

#endif

//****************************************************************************//
