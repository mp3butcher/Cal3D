//****************************************************************************//
// physique.h                                                                 //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_PHYSIQUE_H
#define CAL_PHYSIQUE_H

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
/** The physique class.
  *****************************************************************************/

class CAL3D_API CalPhysique
{
// member variables
public:
  CalModel *m_pModel;

// constructors/destructor
public:
  CalPhysique();
  virtual ~CalPhysique();

// member functions	
public:
  int calculateNormals(CalSubmesh *pSubmesh, float *pNormalBuffer);
  int calculateVertices(CalSubmesh *pSubmesh, float *pVertexBuffer);
  bool create(CalModel *pModel);
  void destroy();
  void update();
};

#endif

//****************************************************************************//
