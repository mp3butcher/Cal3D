//----------------------------------------------------------------------------//
// max2ogl.cpp                                                                //
// Copyright (C) Mekensleep 2004 - Cedric Pinson                              //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef MAX2OGL_H
#include <matrix3.h>

Matrix3 ConvertMax2Ogl(const Matrix3& _mat);
Point3 ConvertMax2Ogl(const Point3& _p);
 
#endif