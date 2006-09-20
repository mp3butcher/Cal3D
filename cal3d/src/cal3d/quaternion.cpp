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

#include "cal3d/quaternion.h"
#include "cal3d/matrix.h"
#include "cal3d/vector.h"

#include <float.h>

 /*****************************************************************************/
/** Constructs the quaternion instance.
  *
  * This function is the default constructor of the quaternion instance.
  *****************************************************************************/
/*
CalQuaternion::CalQuaternion()
  : x(0.0f), y(0.0f), z(0.0f), w(1.0f)
{
}
*/
 /*****************************************************************************/
/** Constructs the quaternion instance.
  *
  * This function is a constructor of the quaternion instance.
  *
  * @param q The quaternion to construct this quaternion instance from.
  *****************************************************************************/
/*
CalQuaternion::CalQuaternion(const CalQuaternion& q)
  : x(q.x), y(q.y), z(q.z), w(q.w)
{
}
*/
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
/*
CalQuaternion::CalQuaternion(float qx, float qy, float qz, float qw)
  : x(qx), y(qy), z(qz), w(qw)
{
}
*/
 /*****************************************************************************/
/** Destructs the quaternion instance.
  *
  * This function is the destructor of the quaternion instance.
  *****************************************************************************/
/*
CalQuaternion::~CalQuaternion()
{
}
*/
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
/*
float& CalQuaternion::operator[](unsigned int index)
{
  return (&x)[index];
}
*/
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
/*
const float& CalQuaternion::operator[](unsigned int index) const
{
  return (&x)[index];
}
*/
 /*****************************************************************************/
/** Equates the quaternion instance with another quaternion.
  *
  * This operator equates the quaternion instance with another quaternion.
  *
  * @param q The quaternion to equate the quaternion instance with.
  *****************************************************************************/
/*
void CalQuaternion::operator=(const CalQuaternion& q)
{
  x = q.x;
  y = q.y;
  z = q.z;
  w = q.w;
}
*/
 /*****************************************************************************/
/** Multiplies another quaternion to the quaternion instance.
  *
  * This operator multiplies another quaternion to the quaternion instance.
  *
  * @param q The quaternion to be multiplied.
  *****************************************************************************/
/*
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
*/
 /*****************************************************************************/
/** Multiplies a vector to the quaternion instance.
  *
  * This operator multiplies a vector to the quaternion instance.
  *
  * @param v The vector to be multiplied.
  *****************************************************************************/
/*
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
*/
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
/*
CalQuaternion operator*(const CalQuaternion& q, const CalQuaternion& r)
{
  return CalQuaternion(
    r.w * q.x + r.x * q.w + r.y * q.z - r.z * q.y,
    r.w * q.y - r.x * q.z + r.y * q.w + r.z * q.x,
    r.w * q.z + r.x * q.y - r.y * q.x + r.z * q.w,
    r.w * q.w - r.x * q.x - r.y * q.y - r.z * q.z
  );
}
*/
 /*****************************************************************************/
/** Interpolates the quaternion instance to another quaternion.
  *
  * This function interpolates the quaternion instance to another quaternion by
  * a given factor.
  *
  * @param d The blending factor in the range [0.0, 1.0].
  * @param v The quaternion to be interpolated to.
  *****************************************************************************/
/*
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
    theta = (float) acos(norm);

    float s;
    s = (float) (1.0f / sin(theta));

    inv_d = (float) sin((1.0f - d) * theta) * s;
    d = (float) sin(d * theta) * s;
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
*/
 /*****************************************************************************/
/** Clears the quaternion instance.
  *
  * This function clears the quaternion instance.
  *****************************************************************************/
/*
void CalQuaternion::clear()
{
  x = 0.0f;
  y = 0.0f;
  z = 0.0f;
  w = 1.0f;
}
*/
 /*****************************************************************************/
/** Conjugates the quaternion instance.
  *
  * This function conjugates the quaternion instance.
  *****************************************************************************/
/*
void CalQuaternion::conjugate()
{
  x = -x;
  y = -y;
  z = -z;
}
*/
 /*****************************************************************************/
/** Inverts the quaternion instance.
  *
  * This function inverts the quaternion instance.
  *****************************************************************************/
/*
void CalQuaternion::invert()
{
  conjugate();
  const float norm = (x*x) + (y*y) + (z*z) + (w*w);

  if (norm == 0.0f) return;
  
  const float inv_norm = 1 / norm;
  x *= inv_norm;
  y *= inv_norm;
  z *= inv_norm;
  w *= inv_norm;
}
*/

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
/*
void CalQuaternion::set(float qx, float qy, float qz, float qw)
{
  x = qx;
  y = qy;
  z = qz;
  w = qw;
}
*/
 /*****************************************************************************/
/** Computes the shortest arc quaternion that will rotate one vector to another.
  *
  * This function finds the shortest arc quaternion. 
  * Based on equations from "Game Programming Gems" - chapter 2.10
  *
  * @param from The original vector
  * @param to The target vector
  *****************************************************************************/
/*
CalQuaternion shortestArc( const CalVector& from, const CalVector& to )
{
   CalVector cross = from % to; //Compute vector cross product
   float dot = from * to ;      //Compute dot product

   dot = (float) sqrt( 2*(dot+1) ) ; //We will use this equation twice

   cross /= dot ; //Get the x, y, z components

   //Return with the w component (Note that w is inverted because Cal3D has
   // left-handed rotations )
   return CalQuaternion( cross[0], cross[1], cross[2], -dot/2 ) ; 

}
*/
//****************************************************************************//

 /*****************************************************************************/
/** Returns a compressed representation of the quaternion.
  * Compresses the quaternion from traditional 4*4 = 16 bytes to 6 bytes.
	* This is a lossy compression based on article "Quaternion, Compression"
	* by Zarb-Adami in GPG3.
  *
  * @param s0 First word of the compressed quaternion.
  * @param s1 Second word of the compressed quaternion.
  * @param s2 Third word of the compressed quaternion.
  *****************************************************************************/

void CalQuaternion::compress(short &s0, short &s1, short &s2)
{
	static const float scale = 1.41421f;

	if (w > x && w > y && w > z) {

		float rx = x * scale;
		float ry = y * scale;
		float rz = z * scale;

		// w is the biggest element, make sure it's also positive, if not, negate the quaternion
		if (w < 0) {
			rx = -rx;
			ry = -ry;
			rz = -rz;
		}

		if (rx < -1) rx = -1;
		else if (rx > 1) rx = 1;
		if (ry < -1) ry = -1;
		else if (ry > 1) ry = 1;
		if (rz < -1) rz = -1;
		else if (rz > 1) rz = 1;

		s0 = short(rx * 32767);
		s1 = short((int(ry * 32767) & 0xfffe) | 1);
		s2 = short((int(rz * 32767) & 0xfffe) | 1);
	}
	else if (z > x && z > y) {

		float rx = x * scale;
		float ry = y * scale;
		float rw = w * scale;

		// z is the biggest element, make sure it's also positive, if not, negate the quaternion
		if (w < 0) {
			rx = -rx;
			ry = -ry;
			rw = -rw;
		}

		if (rx < -1) rx = -1;
		else if (rx > 1) rx = 1;
		if (ry < -1) ry = -1;
		else if (ry > 1) ry = 1;
		if (rw < -1) rw = -1;
		else if (rw > 1) rw = 1;

		s0 = short(rx * 32767);
		s1 = short((int(ry * 32767) & 0xfffe) | 1);
		s2 = short((int(rw * 32767) & 0xfffe) | 0);
	}
	else if (y > x) {

		float rx = x * scale;
		float rz = z * scale;
		float rw = w * scale;

		// y is the biggest element, make sure it's also positive, if not, negate the quaternion
		if (y < 0) {
			rx = -rx;
			rz = -rz;
			rw = -rw;
		}

		if (rx < -1) rx = -1;
		else if (rx > 1) rx = 1;
		if (rz < -1) rz = -1;
		else if (rz > 1) rz = 1;
		if (rw < -1) rw = -1;
		else if (rw > 1) rw = 1;

		s0 = short(rx * 32767);
		s1 = short((int(rz * 32767) & 0xfffe) | 0);
		s2 = short((int(rw * 32767) & 0xfffe) | 1);
	}
	else {

		float ry = y * scale;
		float rz = z * scale;
		float rw = w * scale;

		// x is the biggest element, make sure it's also positive, if not, negate the quaternion
		if (x < 0) {
			ry = -ry;
			rz = -rz;
			rw = -rw;
		}

		if (ry < -1) ry = -1;
		else if (ry > 1) ry = 1;
		if (rz < -1) rz = -1;
		else if (rz > 1) rz = 1;
		if (rw < -1) rw = -1;
		else if (rw > 1) rw = 1;

		s0 = short(ry * 32767);
		s1 = short((int(rz * 32767) & 0xfffe) | 0);
		s2 = short((int(rw * 32767) & 0xfffe) | 0);
	}
}

 /*****************************************************************************/
/** Sets new values from a compressed quaternion.
  * Decompresses the quaternion from 6 bytes to traditional 4*4 = 16 bytes.
	* The compression is based on article "Quaternion, Compression"
	* by Zarb-Adami in GPG3.
  *
  * @param s0 First word of the compressed quaternion.
  * @param s1 Second word of the compressed quaternion.
  * @param s2 Third word of the compressed quaternion.
  *****************************************************************************/

void CalQuaternion::decompress(short &s0, short &s1, short &s2)
{
	int which = ((s1 & 1) << 1) | (s2 & 1);
	s1 &= 0xfffe;
	s2 &= 0xfffe;

	static const float scale = 1.0f / 32767.0f / 1.41421f;

	if (which == 3) {
		x = s0 * scale;
		y = s1 * scale;
		z = s2 * scale;

		w = 1 - (x*x) - (y*y) - (z*z);
		if (w > FLT_EPSILON)
			w = sqrt(w);
	}
	else if (which == 2) {
		x = s0 * scale;
		y = s1 * scale;
		w = s2 * scale;

		z = 1 - (x*x) - (y*y) - (w*w);
		if (z > FLT_EPSILON)
			z = sqrt(z);
	}
	else if (which == 1) {
		x = s0 * scale;
		z = s1 * scale;
		w = s2 * scale;

		y = 1 - (x*x) - (z*z) - (w*w);
		if (y > FLT_EPSILON)
			y = sqrt(y);
	}
	else {
		y = s0 * scale;
		z = s1 * scale;
		w = s2 * scale;

		x = 1 - (y*y) - (z*z) - (w*w);
		if (x > FLT_EPSILON)
			x = sqrt(x);
	}
}
