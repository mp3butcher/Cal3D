//****************************************************************************//
// skeleton.h                                                                 //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_SKELETON_H
#define CAL_SKELETON_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "cal3d/global.h"

//****************************************************************************//
// Forward declarations                                                       //
//****************************************************************************//

class CalCoreSkeleton;
class CalCoreModel;
class CalBone;

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

 /*****************************************************************************/
/** The skeleton class.
  *****************************************************************************/

class CAL3D_API CalSkeleton
{
// member variables
protected:
  CalCoreSkeleton *m_pCoreSkeleton;
  std::vector<CalBone *> m_vectorBone;
  bool m_isBoundingBoxesComputed;

// constructors/destructor
public:
  CalSkeleton();
  virtual ~CalSkeleton();

// member functions
public:
  void calculateState();
  void clearState();
  bool create(CalCoreSkeleton *pCoreSkeleton);
  void destroy();
  CalBone *getBone(int boneId);
  CalCoreSkeleton *getCoreSkeleton();
  std::vector<CalBone *>& getVectorBone();
  void lockState();
  void getBoneBoundingBox(float *min, float *max);
  void calculateBoundingBoxes();



// DEBUG-CODE
  int getBonePoints(float *pPoints);
  int getBonePointsStatic(float *pPoints);
  int getBoneLines(float *pLines);
  int getBoneLinesStatic(float *pLines);
};

#endif

//****************************************************************************//
