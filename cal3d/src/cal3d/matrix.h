//****************************************************************************//
// matrix.h                                                                   //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_MATRIX_H
#define CAL_MATRIX_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "cal3d/global.h"

//****************************************************************************//
// Forward declarations                                                       //
//****************************************************************************//

class CalQuaternion;

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

 /*****************************************************************************/
/** The matrix class.
  *****************************************************************************/

class CAL3D_API CalMatrix
{
// member variables
public:
  // float dxdx,dydx,dzdx;
  // float dxdy,dydy,dzdy;
  // float dxdz,dydz,dzdz;

  float dxdx,dxdy,dxdz;
  float dydx,dydy,dydz;
  float dzdx,dzdy,dzdz;

// constructors/destructor
public:
  CalMatrix();
  CalMatrix(const CalQuaternion &q);
  CalMatrix(float weight, const CalMatrix &m);
  ~CalMatrix();

// member functions
public:
  void operator=(const CalQuaternion& q);
  void operator=(const CalMatrix& m);
  void operator *= (const CalMatrix &m);
  void operator *= (float factor);
  void blend(float d, const CalMatrix& m);
};


#endif

//****************************************************************************//
