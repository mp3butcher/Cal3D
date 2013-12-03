//****************************************************************************//
// physiquedualquaternion.h                                                   //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_PHYSIQUE_DUAL_QUATERNION_H
#define CAL_PHYSIQUE_DUAL_QUATERNION_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "cal3d/physique.h"

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

class CalPhysiqueDualQuat : public CalPhysique
{
public:
  CalPhysiqueDualQuat(CalModel *pModel)
    : CalPhysique( pModel ) {}

  virtual	int calculateNormals(CalSubmesh *pSubmesh, float *pNormalBuffer, int stride=0) const;
  virtual	int calculateVertices(CalSubmesh *pSubmesh, float *pVertexBuffer, int stride=0) const;
  virtual int calculateVerticesAndNormals(CalSubmesh *pSubmesh, float *pVertexBuffer, int stride=0) const;
  virtual int calculateVerticesNormalsAndTexCoords(CalSubmesh *pSubmesh, float *pVertexBuffer, int NumTexCoords=1) const;
};


#endif	// CAL_PHYSIQUE_DUAL_QUATERNION_H

//****************************************************************************//
