//****************************************************************************//
// quaternion.h                                                               //
// Copyright (C) 2001 Bruno 'Beosil' Heidelberger                             //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_QUATERNION_H
#define CAL_QUATERNION_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "global.h"

//****************************************************************************//
// Forward declarations                                                       //
//****************************************************************************//

class CalVector;

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

 /*****************************************************************************/
/** The quaternion class.
  *****************************************************************************/

class CAL3D_API CalQuaternion
{
// member variables
public:
  float x;
  float y;
  float z;
  float w;

// constructors/destructor
public:
  CalQuaternion();
  CalQuaternion(const CalQuaternion& q);
  CalQuaternion(float qx, float qy, float qz, float qw);
  ~CalQuaternion();

// member functions	
public:
  float& operator[](unsigned int index);
  const float& operator[](unsigned int index) const;
  void operator=(const CalQuaternion& q);
  void operator*=(const CalQuaternion& q);
  void operator*=(const CalVector& v);
  friend CalQuaternion operator*(const CalQuaternion& q, const CalQuaternion& r);
  void blend(float d, const CalQuaternion& q);
};

#endif

//****************************************************************************//
