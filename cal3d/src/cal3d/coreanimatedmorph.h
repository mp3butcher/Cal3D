//****************************************************************************//
// coreanimatedMorph.h                                                            //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_COREANIMATEDMORPH_H
#define CAL_COREANIMATEDMORPH_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "cal3d/global.h"
#include "cal3d/coremorphtrack.h"

//****************************************************************************//
// Forward declarations                                                       //
//****************************************************************************//

class CalCoreMorphTrack;

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

 /*****************************************************************************/
/** The core animatedMorph class.
  *****************************************************************************/


class CAL3D_API CalCoreAnimatedMorph
{
// member variables
protected:
  float m_duration;
  std::list<CalCoreMorphTrack> m_listCoreTrack;
  std::list<CalCoreMorphTrack*> m_tracksToDelete;

// constructors/destructor
public:
  CalCoreAnimatedMorph();
  virtual ~CalCoreAnimatedMorph();

// member functions 
public:
  bool addCoreTrack(CalCoreMorphTrack *pCoreTrack);
  bool create();
  void destroy();
  CalCoreMorphTrack *getCoreTrack(std::string const & trackId);
  float getDuration() const;
  std::list<CalCoreMorphTrack>& getListCoreTrack();
  const std::list<CalCoreMorphTrack>& getListCoreTrack() const;
  void setDuration(float duration);
  void scale(float factor);
  void removeZeroScaleTracks();
  static int getNumCoreAnimatedMorphs();
};

#endif

//****************************************************************************//
