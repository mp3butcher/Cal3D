//****************************************************************************//
// vector.h                                                                   //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_VECTOR_H
#define CAL_VECTOR_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "cal3d/global.h"

//****************************************************************************//
// Forward declarations                                                       //
//****************************************************************************//

class CalQuaternion;
class CalMatrix;

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

 /*****************************************************************************/
/** The vector class.
  *****************************************************************************/

class CAL3D_API CalVector
{
// member variables
public:
  float x;
  float y;
  float z;

// constructors/destructor
public:
  CalVector();
  CalVector(const CalVector& v);
  CalVector(float vx, float vy, float vz);
  ~CalVector();

// member functions
public:
  float& operator[](unsigned int i);
  const float& operator[](unsigned int i) const;
  void operator=(const CalVector& v);
  void operator+=(const CalVector& v);
  void operator-=(const CalVector& v);
  void operator*=(const float d);
  void operator*=(const CalQuaternion& q);
  void operator*=(const CalMatrix &m);
  void operator/=(const float d);
  bool operator==(const CalVector& v);
  friend CAL3D_API CalVector operator+(const CalVector& v, const CalVector& u);
  friend CAL3D_API CalVector operator-(const CalVector& v, const CalVector& u);
  friend CAL3D_API CalVector operator*(const CalVector& v, const float d);
  friend CAL3D_API CalVector operator*(const float d, const CalVector& v);
  friend CAL3D_API CalVector operator/(const CalVector& v, const float d);
  friend CAL3D_API float operator*(const CalVector& v, const CalVector& u);
  friend CAL3D_API CalVector operator%(const CalVector& v, const CalVector& u);
  void blend(float d, const CalVector& v);
  void clear();
  float length();
  float normalize();
  void set(float vx, float vy, float vz);
};

 /*****************************************************************************/
/** The plane class.
  *****************************************************************************/


class CAL3D_API CalPlane
{
   public:
      float a,b,c,d;
      
      // These methods are made only to calculate the bounding boxes,
      // don't use them in you program
      
      float eval(CalVector &p);
      void setPosition(CalVector &p);
      void setNormal(CalVector &p);
};

 /*****************************************************************************/
/** The bounding box class.
  *****************************************************************************/


class CAL3D_API CalBoundingBox
{
   public:
     CalPlane plane[6];
     
     void computePoints(CalVector *p);
   
};



#endif

//****************************************************************************//
