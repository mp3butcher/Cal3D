//****************************************************************************//
// main.cpp                                                                   //
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

#include "cal3d/cal3d.h"

//****************************************************************************//
// This is just a dummy application for compiling the library                 //
//****************************************************************************//

int main(int argc, char *argv[])
{
  std::cout << "Thanks for using Cal3D!" << std::endl;

  // create a core model instance
  CalCoreModel calCoreModel;
  if(!calCoreModel.create("dummy"))
  {
    CalError::printLastError();
    return -1;
  }

  // load a core model description
  if(!calCoreModel.loadCoreSkeleton("dummy.csf"))
  {
    CalError::printLastError();
    return -1;
  }

  // load a core animation description
  int animationId;
  animationId = calCoreModel.loadCoreAnimation("dummy.caf");
  if(animationId == -1)
  {
    CalError::printLastError();
    return -1;
  }

  // load a core mesh description
  int meshId;
  meshId = calCoreModel.loadCoreMesh("dummy.cmf");
  if(meshId == -1)
  {
    CalError::printLastError();
    return -1;
  }

  // create a model instance from the loaded core model
  CalModel calModel;
  if(!calModel.create(&calCoreModel))
  {
    CalError::printLastError();
    return -1;
  }

  // destroy model instance
  calModel.destroy();

  // destroy core model instance
  calCoreModel.destroy();

  return 0;
}

//****************************************************************************//
