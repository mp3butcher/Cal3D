//****************************************************************************//
// springsystem.h                                                             //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_SPRINGSYSTEM_H
#define CAL_SPRINGSYSTEM_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "global.h"

//****************************************************************************//
// Forward declarations                                                       //
//****************************************************************************//

class CalModel;
class CalSubmesh;

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

 /*****************************************************************************/
/** The spring system class.
  *****************************************************************************/

class CAL3D_API CalSpringSystem
{
// member variables
public:
  CalModel *m_pModel;

// constructors/destructor
public:
  CalSpringSystem();
	virtual ~CalSpringSystem();

// member functions	
public:
  void calculateForces(CalSubmesh *pSubmesh, float deltaTime);
  void calculateVertices(CalSubmesh *pSubmesh, float deltaTime);
  bool create(CalModel *pModel);
  void destroy();
  void update(float deltaTime);
};

#endif

//****************************************************************************//
