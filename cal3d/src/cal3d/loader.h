//****************************************************************************//
// loader.h                                                                   //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_LOADER_H
#define CAL_LOADER_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "global.h"

//****************************************************************************//
// Forward declarations                                                       //
//****************************************************************************//

class CalCoreModel;
class CalCoreSkeleton;
class CalCoreBone;
class CalCoreAnimation;
class CalCoreTrack;
class CalCoreKeyframe;
class CalCoreMesh;
class CalCoreSubmesh;
class CalCoreMaterial;

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

 /*****************************************************************************/
/** The loader class.
  *****************************************************************************/

class CAL3D_API CalLoader
{
// member functions
public:
  static CalCoreAnimation *loadCoreAnimation(const std::string& strFilename);
  static CalCoreMaterial *loadCoreMaterial(const std::string& strFilename);
  static CalCoreMesh *loadCoreMesh(const std::string& strFilename);
  static CalCoreSkeleton *loadCoreSkeleton(const std::string& strFilename);

private:
  static CalCoreBone *loadCoreBones(std::ifstream& file, const std::string& strFilename);
  static CalCoreKeyframe *loadCoreKeyframe(std::ifstream& file, const std::string& strFilename);
  static CalCoreSubmesh *loadCoreSubmesh(std::ifstream& file, const std::string& strFilename);
  static CalCoreTrack *loadCoreTrack(std::ifstream& file, const std::string& strFilename);

  static CalCoreAnimation *loadXmlCoreAnimation(const std::string& strFilename);
  static CalCoreSkeleton *loadXmlCoreSkeleton(const std::string& strFilename);
  static CalCoreMesh *loadXmlCoreMesh(const std::string& strFilename);
  static CalCoreMaterial *loadXmlCoreMaterial(const std::string& strFilename);
};

#endif

//****************************************************************************//
