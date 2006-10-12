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

#include "cal3d/global.h"

class CalCoreTrack;
class CalCoreSkeleton;

class CAL3D_API CalCoreAnimation
{
public:
  CalCoreAnimation();
  ~CalCoreAnimation();

  size_t size();
  void fillInvalidTranslations( CalCoreSkeleton * skel );
  bool addCoreTrack(CalCoreTrack *pCoreTrack);
  bool create();
  void destroy();
  CalCoreTrack *getCoreTrack(int coreBoneId);
  unsigned int numCoreTracks();
  CalCoreTrack * nthCoreTrack( unsigned int );
  float getDuration();
  std::list<CalCoreTrack *>& getListCoreTrack();
  void setDuration(float duration);
  void scale(float factor);
  static int getNumCoreAnimations();

private:
  float m_duration;
  std::list<CalCoreTrack *> m_listCoreTrack;
};

#endif

//****************************************************************************//
