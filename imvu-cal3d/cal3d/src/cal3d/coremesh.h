//****************************************************************************//
// coremesh.h                                                                 //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_COREMESH_H
#define CAL_COREMESH_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "cal3d/global.h"

//****************************************************************************//
// Forward declarations                                                       //
//****************************************************************************//

class CalCoreSubmesh;

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

 /*****************************************************************************/
/** The core mesh class.
  *****************************************************************************/

class CAL3D_API CalCoreMesh
{
// member variables
protected:
  std::vector<CalCoreSubmesh *> m_vectorCoreSubmesh;

// constructors/destructor
public:
  CalCoreMesh();
  virtual ~CalCoreMesh();

// member functions	
public:
  typedef std::vector<CalCoreSubmesh *> CalCoreSubmeshVector;
  unsigned int size();
  int addCoreSubmesh(CalCoreSubmesh *pCoreSubmesh);
  bool create();
  void destroy();
  CalCoreSubmesh *getCoreSubmesh(int id);
  int getCoreSubmeshCount();
  void reserve(int submeshes) { m_vectorCoreSubmesh.reserve(submeshes); }
  std::vector<CalCoreSubmesh *>& getVectorCoreSubmesh();
  int addAsMorphTarget(CalCoreMesh *pCoreMesh, std::string const & morphTargetName);
  void scale(float factor);
};

#endif

//****************************************************************************//
