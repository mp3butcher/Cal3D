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
// Defines Win32                                                              //
//****************************************************************************//

#ifdef _WIN32

#pragma warning(disable : 4251)
#pragma warning(disable : 4786)
  
#ifdef CAL3D_EXPORTS
#define CAL3D_API __declspec(dllexport)
#else
#define CAL3D_API __declspec(dllimport)
#endif

#endif

//****************************************************************************//
// Defines Linux and Cygwin                                                   //
//****************************************************************************//

#if defined(__linux__) || (__CYGWIN__)

#define CAL3D_API

#endif

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
};

#endif

//****************************************************************************//
