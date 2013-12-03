//****************************************************************************//
// dualquaternion.h                                                           //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_DUAL_QUATERNION_H
#define CAL_DUAL_QUATERNION_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "cal3d/global.h"
#include "cal3d/vector.h"
#include "cal3d/quaternion.h"

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

class CAL3D_API CalDualQuaternion
{
public:
  // member variables
  CalQuaternion	nondual;
  CalQuaternion	dual;

  // constructors/destructor
  CalDualQuaternion() {}  // default constructor
  CalDualQuaternion( const CalDualQuaternion& inOther ) // copy constructor
    : nondual( inOther.nondual ), dual( inOther.dual ) {}
  CalDualQuaternion( const CalQuaternion& inRotation,
                     const CalVector& inTranslation );
  ~CalDualQuaternion() {}

  // assignment
  inline CalDualQuaternion&	operator=( const CalDualQuaternion& inOther )
  {
    nondual = inOther.nondual;
    dual = inOther.dual;
    return *this;
  }

  inline void operator*=( float s )
  {
    nondual *= s;
    dual *= s;
  }

  inline void operator+=( const CalDualQuaternion& inOther )
  {
    nondual += inOther.nondual;
    dual += inOther.dual;
  }

  void normalize();

  void transformPoint( const CalVector& inPt, CalVector& outPt ) const;
};


#endif	// CAL_DUAL_QUATERNION_H

//****************************************************************************//
