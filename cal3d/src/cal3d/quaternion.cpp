//****************************************************************************//
// quaternion.cpp                                                             //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "quaternion.h"
#include "vector.h"

 /*****************************************************************************/
/** Constructs the quaternion instance.
  *
  * This function is the default constructor of the quaternion instance.
  *****************************************************************************/

CalQuaternion::CalQuaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f)
{
}

 /*****************************************************************************/
/** Constructs the quaternion instance.
  *
  * This function is a constructor of the quaternion instance.
  *
  * @param q The quaternion to construct this quaternion instance from.
  *****************************************************************************/

CalQuaternion::CalQuaternion(const CalQuaternion& q) : x(q.x), y(q.y), z(q.z), w(q.w)
{
}

 /*****************************************************************************/
/** Constructs the quaternion instance.
  *
  * This function is a constructor of the quaternion instance.
  *
  * @param qx The x component.
  * @param qy The y component.
  * @param qz The z component.
  * @param qw The w component.
  *****************************************************************************/

CalQuaternion::CalQuaternion(float qx, float qy, float qz, float qw) : x(qx), y(qy), z(qz), w(qw)
{
}

 /*****************************************************************************/
/** Destructs the quaternion instance.
  *
  * This function is the destructor of the quaternion instance.
  *****************************************************************************/

CalQuaternion::~CalQuaternion()
{
}

 /*****************************************************************************/
/** Provides access to the components of the quaternion instance.
  *
  * This function provides read and write access to the three components of the
  * quaternion instance.
  *
  * @param index The index to the specific component.
  *
  * @return A reference to the specific component.
  *****************************************************************************/

float& CalQuaternion::operator[](unsigned int index)
{
  return (&x)[index];
}

 /*****************************************************************************/
/** Provides access to the components of the quaternion instance.
  *
  * This function provides read access to the three components of the quaternion
  * instance.
  *
  * @param index The index to the specific component.
  *
  * @return A constant reference to the specific component.
  *****************************************************************************/

const float& CalQuaternion::operator[](unsigned int index) const
{
  return (&x)[index];
}

 /*****************************************************************************/
/** Equates the quaternion instance with another quaternion.
  *
  * This operator equates the quaternion instance with another quaternion.
  *
  * @param q The quaternion to equate the quaternion instance with.
  *****************************************************************************/

void CalQuaternion::operator=(const CalQuaternion& q)
{
  x = q.x;
  y = q.y;
  z = q.z;
  w = q.w;
}

 /*****************************************************************************/
/** Multiplies another quaternion to the quaternion instance.
  *
  * This operator multiplies another quaternion to the quaternion instance.
  *
  * @param q The quaternion to be multiplied.
  *****************************************************************************/

void CalQuaternion::operator*=(const CalQuaternion& q)
{
  float qx, qy, qz, qw;
  qx = x;
  qy = y;
  qz = z;
  qw = w;

  x = qw * q.x + qx * q.w + qy * q.z - qz * q.y;
  y = qw * q.y - qx * q.z + qy * q.w + qz * q.x;
  z = qw * q.z + qx * q.y - qy * q.x + qz * q.w;
  w = qw * q.w - qx * q.x - qy * q.y - qz * q.z;
}

 /*****************************************************************************/
/** Multiplies a vector to the quaternion instance.
  *
  * This operator multiplies a vector to the quaternion instance.
  *
  * @param v The vector to be multiplied.
  *****************************************************************************/

void CalQuaternion::operator*=(const CalVector& v)
{
  float qx, qy, qz, qw;
  qx = x;
  qy = y;
  qz = z;
  qw = w;

  x = qw * v.x            + qy * v.z - qz * v.y;
  y = qw * v.y - qx * v.z            + qz * v.x;
  z = qw * v.z + qx * v.y - qy * v.x;
  w =          - qx * v.x - qy * v.y - qz * v.z;
}

 /*****************************************************************************/
/** Calculates the product of two quaternions.
  *
  * This operator calculates the product of two quaternions.
  *
  * @param q The first quaternion.
  * @param r The second quaternion.
  *
  * @return The product of the two quaternions.
  *****************************************************************************/

CalQuaternion operator*(const CalQuaternion& q, const CalQuaternion& r)
{
  return CalQuaternion(
    r.w * q.x + r.x * q.w + r.y * q.z - r.z * q.y,
    r.w * q.y - r.x * q.z + r.y * q.w + r.z * q.x,
    r.w * q.z + r.x * q.y - r.y * q.x + r.z * q.w,
    r.w * q.w - r.x * q.x - r.y * q.y - r.z * q.z
  );
}

 /*****************************************************************************/
/** Interpolates the quaternion instance to another quaternion.
  *
  * This function interpolates the quaternion instance to another quaternion by
  * a given factor.
  *
  * @param d The blending factor in the range [0.0, 1.0].
  * @param v The quaternion to be interpolated to.
  *****************************************************************************/

void CalQuaternion::blend(float d, const CalQuaternion& q)
{
  float norm;
  norm = x * q.x + y * q.y + z * q.z + w * q.w;

  bool bFlip;
  bFlip = false;

  if(norm < 0.0f)
  {
    norm = -norm;
    bFlip = true;
  }

  float inv_d;
  if(1.0f - norm < 0.000001f)
  {
    inv_d = 1.0f - d;
  }
  else
  {
    float theta;
    theta = acos(norm);

    float s;
    s = 1.0f / sin(theta);

    inv_d = sin((1.0f - d) * theta) * s;
    d = sin(d * theta) * s;
  }

  if(bFlip)
  {
    d = -d;
  }

  x = inv_d * x + d * q.x;
  y = inv_d * y + d * q.y;
  z = inv_d * z + d * q.z;
  w = inv_d * w + d * q.w;
}

 /*****************************************************************************/
/** Clears the quaternion instance.
  *
  * This function clears the quaternion instance.
  *****************************************************************************/

void CalQuaternion::clear()
{
  x = 0.0f;
  y = 0.0f;
  z = 0.0f;
  w = 1.0f;
}

 /*****************************************************************************/
/** Conjugates the quaternion instance.
  *
  * This function conjugates the quaternion instance.
  *****************************************************************************/

void CalQuaternion::conjugate()
{
  x = -x;
  y = -y;
  z = -z;
}

 /*****************************************************************************/
/** Sets new values.
  *
  * This function sets new values in the quaternion instance.
  *
  * @param qx The x component.
  * @param qy The y component.
  * @param qz The z component.
  * @param qw The w component.
  *****************************************************************************/

void CalQuaternion::set(float qx, float qy, float qz, float qw)
{
  x = qx;
  y = qy;
  z = qz;
  w = qw;
}

 /*****************************************************************************/
/** Constructs the matrix instance.
  *
  * This function is the default constructor of the matrix instance.
  *****************************************************************************/

CalMatrix::CalMatrix()
{
  dxdx=dxdy=dxdz=dydx=dydy=dydz=dzdx=dzdy=dzdz=0.0;
}

 /*****************************************************************************/
/** Destructs the matrix instance.
  *
  * This function is the destructor of the matrix instance.
  *****************************************************************************/

CalMatrix::~CalMatrix()
{
}

 /*****************************************************************************/
/** Copying a Matrix
  *
  * This function copies one matrix into another.
  *****************************************************************************/

void CalMatrix::operator =(const CalMatrix& m)
{
  dxdx=m.dxdx; dxdy=m.dxdy; dxdz=m.dxdz;
  dydx=m.dydx; dydy=m.dydy; dydz=m.dydz;
  dzdx=m.dzdx; dzdy=m.dzdy; dzdz=m.dzdz;
}

 /*****************************************************************************/
/** Quaternion to Matrix Conversion Constructor
  *
  * This function converts a quaternion into a rotation matrix.
  *****************************************************************************/

CalMatrix::CalMatrix(const CalQuaternion& q)
{
  *this = q;
}

 /*****************************************************************************/
/** Quaternion to Matrix Conversion.
  *
  * This function converts a quaternion into a rotation matrix.
  *****************************************************************************/

void CalMatrix::operator =(const CalQuaternion& q)
{
  float xx2=q.x*q.x*2;
  float yy2=q.y*q.y*2;
  float zz2=q.z*q.z*2;
  float ww2=q.w*q.w*2;
  float xy2=q.x*q.y*2;
  float zw2=q.z*q.w*2;
  float xz2=q.x*q.z*2;
  float yw2=q.y*q.w*2;
  float yz2=q.y*q.z*2;
  float xw2=q.x*q.w*2;
  dxdx=1-yy2-zz2;   dxdy=  xy2+zw2;  dxdz=  xz2-yw2;
  dydx=  xy2-zw2;   dydy=1-xx2-zz2;  dydz=  yz2+xw2;
  dzdx=  xz2+yw2;   dzdy=  yz2-xw2;  dzdz=1-xx2-yy2;
}

 /*****************************************************************************/
/** Matrix Initialization
  *
  * This function sets one matrix to a factor times another.
  *****************************************************************************/

CalMatrix::CalMatrix(float factor, const CalMatrix& m)
{
  dxdx = m.dxdx*factor;
  dxdy = m.dxdy*factor;
  dxdz = m.dxdz*factor;
  dydx = m.dydx*factor;
  dydy = m.dydy*factor;
  dydz = m.dydz*factor;
  dzdx = m.dzdx*factor;
  dzdy = m.dzdy*factor;
  dzdz = m.dzdz*factor;
}

 /*****************************************************************************/
/** Matrix Blending
  *
  * This function adds a weight times another matrix to the current matrix.
  *****************************************************************************/

void CalMatrix::blend(float factor, const CalMatrix& m)
{
  dxdx += m.dxdx*factor;
  dxdy += m.dxdy*factor;
  dxdz += m.dxdz*factor;
  dydx += m.dydx*factor;
  dydy += m.dydy*factor;
  dydz += m.dydz*factor;
  dzdx += m.dzdx*factor;
  dzdy += m.dzdy*factor;
  dzdz += m.dzdz*factor;
}

 /*****************************************************************************/
/** Matrix Scaling
  *
  * This function multiplies every element in the matrix by the factor.
  *****************************************************************************/

void CalMatrix::operator *=(float factor)
{
  dxdx *= factor;
  dxdy *= factor;
  dxdz *= factor;
  dydx *= factor;
  dydy *= factor;
  dydz *= factor;
  dzdx *= factor;
  dzdy *= factor;
  dzdz *= factor;
}

 /*****************************************************************************/
/** Matrix Multiplication
  *
  * This function multiplies two matrices.
  *****************************************************************************/

void CalMatrix::operator *=(const CalMatrix &m)
{
  float ndxdx=m.dxdx*dxdx+m.dxdy*dydx+m.dxdz*dzdx;
  float ndydx=m.dydx*dxdx+m.dydy*dydx+m.dydz*dzdx;
  float ndzdx=m.dzdx*dxdx+m.dzdy*dydx+m.dzdz*dzdx;
  
  float ndxdy=m.dxdx*dxdy+m.dxdy*dydy+m.dxdz*dzdy;
  float ndydy=m.dydx*dxdy+m.dydy*dydy+m.dydz*dzdy;
  float ndzdy=m.dzdx*dxdy+m.dzdy*dydy+m.dzdz*dzdy;
  
  float ndxdz=m.dxdx*dxdz+m.dxdy*dydz+m.dxdz*dzdz;
  float ndydz=m.dydx*dxdz+m.dydy*dydz+m.dydz*dzdz;
  float ndzdz=m.dzdx*dxdz+m.dzdy*dydz+m.dzdz*dzdz;
  
  dxdx=ndxdx;
  dxdy=ndxdy;
  dxdz=ndxdz;
  dydx=ndydx;
  dydy=ndydy;
  dydz=ndydz;
  dzdx=ndzdx;
  dzdy=ndzdy;
  dzdz=ndzdz;
}


//****************************************************************************//
