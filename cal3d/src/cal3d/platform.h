//****************************************************************************//
// platform.h                                                                 //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_PLATFORM_H
#define CAL_PLATFORM_H

//****************************************************************************//
// Compiler configuration                                                     //
//****************************************************************************//

#if defined(_WIN32) && !defined(__MINGW32__)
#pragma warning(disable : 4251)
#pragma warning(disable : 4786)
#endif

#if !defined(_WIN32) || defined(__MINGW32__)
#define stricmp strcasecmp
#endif


//****************************************************************************//
// Dynamic library export setup                                               //
//****************************************************************************//

#if defined(_WIN32) && !defined(__MINGW32__)

#ifdef CAL3D_EXPORTS
#define CAL3D_API __declspec(dllexport)
#else
#define CAL3D_API __declspec(dllimport)
#endif

#else

#define CAL3D_API

#endif

//****************************************************************************//
// Endianness setup                                                           //
//****************************************************************************//

#if defined(__i386__) || \
    defined(__alpha__) || \
    defined(__FreeBSD__) || \
    (defined(__mips__) && defined(MIPSEL)) || \
    defined(__MIPSEL__) || \
    (defined(__arm__) && defined(ARMEL)) || \
    defined(__ARMEL__) || defined(_WIN32) || defined(__APPLE__)

#define CAL3D_LITTLE_ENDIAN

#else

#define CAL3D_BIG_ENDIAN

#endif

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

// standard includes
#include <stdlib.h>
#include <math.h>

// debug includes
#include <assert.h>

// STL includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

 /*****************************************************************************/
/** The platform class.
  *****************************************************************************/

class CAL3D_API CalPlatform
{
// constructors/destructor
protected:
  CalPlatform();
  virtual ~CalPlatform();

// member functions	
public:
  static bool readBytes(std::istream& input, void *pBuffer, int length);
  static bool readFloat(std::istream& input, float& value);
  static bool readInteger(std::istream& input, int& value);
  static bool readString(std::istream& input, std::string& strValue);

  static bool readBytes(void* input, void *pBuffer, int length);
  static bool readFloat(void* input, float& value);
  static bool readInteger(void* input, int& value);
  static bool readString(void* input, std::string& strValue);

  static bool writeBytes(std::ostream& output, const void *pBuffer, int length);
  static bool writeFloat(std::ostream& output, float value);
  static bool writeInteger(std::ostream& output, int value);
  static bool writeString(std::ostream& output, const std::string& strValue);
};

#endif

//****************************************************************************//
