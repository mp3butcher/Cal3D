//****************************************************************************//
// global.h                                                                   //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_GLOBAL_H
#define CAL_GLOBAL_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

// autoconf/automake includes
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// platform dependent includes
#include "platform.h"

// standard includes
#include <cstdlib>
#include <cmath>

// debug includes
#include <cassert>

// STL includes
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <map>

// global Cal3D constants
namespace Cal
{
  // global typedefs
  typedef void *UserData;

  // file magic cookies
  const char SKELETON_FILE_MAGIC[4]  = { 'C', 'S', 'F', '\0' };
  const char ANIMATION_FILE_MAGIC[4] = { 'C', 'A', 'F', '\0' };
  const char MESH_FILE_MAGIC[4]      = { 'C', 'M', 'F', '\0' };
  const char MATERIAL_FILE_MAGIC[4]  = { 'C', 'R', 'F', '\0' };

  // library version
  const unsigned int LIBRARY_VERSION = 700;

  // file versions
  const unsigned int CURRENT_FILE_VERSION = LIBRARY_VERSION;
  const unsigned int EARLIEST_COMPATIBLE_FILE_VERSION = 699;
};

#endif

//****************************************************************************//
