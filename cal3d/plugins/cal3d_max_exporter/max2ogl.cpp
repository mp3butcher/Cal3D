//----------------------------------------------------------------------------//
// max2ogl.cpp                                                                //
// Copyright (C) Mekensleep 2004 - Cedric Pinson                              //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#include "StdAfx.h"
#include "max2ogl.h"

Matrix3 ConvertMax2Ogl(const Matrix3& _mat)
{
  //return _mat;

  //T = R x _mat x R-1
  Matrix3 tm,tmogl,tmoglInv;
  tmogl.IdentityMatrix();
  tmogl.RotateX(PI*0.5f);
  tmoglInv=tmogl;
  tmoglInv.Invert();
  tm=tmogl*_mat*tmoglInv;
  return tm;
}


Point3 ConvertMax2Ogl(const Point3& _p)
{
  //return _p;
  return Point3(_p[0],_p[2],-_p[1]); 
} 