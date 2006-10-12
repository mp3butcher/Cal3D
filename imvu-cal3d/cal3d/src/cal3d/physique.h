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

#include "cal3d/global.h"

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
  bool m_Normalize;

// constructors/destructor
public:
  CalPhysique();
  virtual ~CalPhysique();

// member functions	
public:
  int calculateTangentSpaces(CalSubmesh *pSubmesh, int mapId, float *pTangentSpaceBuffer);
  int calculateNormals(CalSubmesh *pSubmesh, float *pNormalBuffer);
  int calculateVertices(CalSubmesh *pSubmesh, float *pVertexBuffer);
  int calculateVerticesAndNormals(CalSubmesh *pSubmesh, float *pVertexBuffer);
  int calculateVerticesNormalsAndTexCoords(CalSubmesh *pSubmesh, float *pVertexBuffer,int NumTexCoords=1);  
  bool create(CalModel *pModel);
  void destroy();
  void update();
  void setNormalization(bool normalize);
};

#endif

//****************************************************************************//
