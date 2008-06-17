//****************************************************************************//
// dualquaternion.cpp                                                         //
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

#include "cal3d/dualquaternion.h"

#include <float.h>

CalDualQuaternion::CalDualQuaternion(	const CalQuaternion& inRotation,
                                      const CalVector& inTranslation )
  : nondual( inRotation )
{
  /*
    A translation by a vector (x, y, z) is represented by the dual quaternion
    1 + e/2 (x i + y j + z k)
    where e is the dual unit.
    So, if we rotate by a quaternion r and then translate by (x, y, z), we
    get
    r (1 + e/2 (x i + y j + z k)) = r + e/2 r (x i + y j + z k).
    That is, the dual part is (r/2) * (x i + y j + z k).
  */
  dual = inRotation;
  dual *= 0.5f;
  dual *= CalQuaternion( inTranslation.x, inTranslation.y, inTranslation.z, 0.0f );
}

void CalDualQuaternion::normalize()
{
  /*
    If the nondual part is nonzero, it can be shown that
      len = len(nondual) + e dot(nondual, dual) / len(nondual)
    where e is the dual unit.  To normalize a dual quaternion, we divide by
    its length,
      (nondual + e dual) / len .
    One can simplify by multiplying numerator and denominator by the dual of
    the denominator, leading to:
      [len(nondual)]^(-1) nondual +
        e ([len(nondual)]^(-1) dual -
          dot(nondual, dual) [len(nondual)]^(-3) nondual)
    If we let nd stand for the normalized nondual, i.e.,
      nd = [len(nondual)]^(-1) nondual,
    then we can convert our normalized dual quaternion to
      nd + e ([len(nondual)]^(-1) dual - dot(nd, dual)[len(nondual)]^(-1) nd)
  */
  float	lenNondual = sqrt( dot( nondual, nondual ) );
  if (lenNondual != 0.0f)
  {
    float	lenRecip = 1.0f / lenNondual;

    nondual *= lenRecip;

    dual *= lenRecip;

    CalQuaternion	r(nondual);
    r *= dot( nondual, dual );
    r *= -1.0f;
    dual += r;
  }
}

void CalDualQuaternion::transformPoint( const CalVector& inPt, CalVector& outPt ) const
{
  CalQuaternion	ptAsQuat( inPt.x, inPt.y, inPt.z, 0.0f );
  CalQuaternion	nonDualConj( nondual );
  nonDualConj.conjugate();
  CalQuaternion	a( nonDualConj );
  a *= dual;
  a *= 2.0f;

  CalQuaternion	b( nonDualConj );
  b *= ptAsQuat;
  b *= nondual;

  a += b;

  outPt.x = a.x;
  outPt.y = a.y;
  outPt.z = a.z;
}
