//****************************************************************************//
// matrix.h                                                                   //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_MATRIX_H
#define CAL_MATRIX_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "cal3d/global.h"

//****************************************************************************//
// Forward declarations                                                       //
//****************************************************************************//

class CalQuaternion;

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

/*****************************************************************************/
/** The matrix class.
*****************************************************************************/

class CAL3D_API CalMatrix
{
	// member variables
public:
	float dxdx,dydx,dzdx;
	float dxdy,dydy,dzdy;
	float dxdz,dydz,dzdz;
	
	// constructors/destructor
public:
	inline CalMatrix()	: dxdx(0.0), dxdy(0.0), dxdz(0.0),
		dydx(0.0), dydy(0.0), dydz(0.0),
		dzdx(0.0), dzdy(0.0), dzdz(0.0)
	{
	}
	
	CalMatrix(const CalQuaternion &q);
	
	inline CalMatrix(float weight, const CalMatrix &m)
	{
		dxdx = m.dxdx*weight;
		dxdy = m.dxdy*weight;
		dxdz = m.dxdz*weight;
		dydx = m.dydx*weight;
		dydy = m.dydy*weight;
		dydz = m.dydz*weight;
		dzdx = m.dzdx*weight;
		dzdy = m.dzdy*weight;
		dzdz = m.dzdz*weight;
	}
	
	
	inline ~CalMatrix() {};
	
	// member functions
public:
	inline void operator=(const CalQuaternion& q);
	
	inline void operator=(const CalMatrix& m)
	{
		dxdx=m.dxdx; dxdy=m.dxdy; dxdz=m.dxdz;
		dydx=m.dydx; dydy=m.dydy; dydz=m.dydz;
		dzdx=m.dzdx; dzdy=m.dzdy; dzdz=m.dzdz;
	}
	inline void operator *= (const CalMatrix &m)
	{
		float ndxdx=m.dxdx*dxdx+m.dxdy*dydx+m.dxdz*dzdx;
		float ndydx=m.dydx*dxdx+m.dydy*dydx+m.dydz*dzdx;
		float ndzdx=m.dzdx*dxdx+m.dzdy*dydx+m.dzdz*dzdx;
		
		float ndxdy=m.dxdx*dxdy+m.dxdy*dydy+m.dxdz*dzdy;
		float ndydy=m.dydx*dxdy+m.dydy*dydy+m.dydz*dzdy;
		float ndzdy=m.dzdx*dxdy+m.dzdy*dydy+m.dzdz*dzdy;
		
		float ndxdz=m.dxdx*dxdz+m.dxdy*dydz+m.dxdz*dzdz;
		float ndydz=m.dydx*dxdz+m.dydy*dydz+m.dydz*dzdz;
		float ndzdz=m.dzdx*dxdz+m.dzdy*dydz+m.dzdz*dzdz;
		
		dxdx=ndxdx;
		dxdy=ndxdy;
		dxdz=ndxdz;
		dydx=ndydx;
		dydy=ndydy;
		dydz=ndydz;
		dzdx=ndzdx;
		dzdy=ndzdy;
		dzdz=ndzdz;
	}
	
	inline void operator *= (float factor)
	{
		dxdx *= factor;
		dxdy *= factor;
		dxdz *= factor;
		dydx *= factor;
		dydy *= factor;
		dydz *= factor;
		dzdx *= factor;
		dzdy *= factor;
		dzdz *= factor;
	}
	inline void blend(float factor, const CalMatrix& m)
	{
		dxdx += m.dxdx*factor;
		dxdy += m.dxdy*factor;
		dxdz += m.dxdz*factor;
		dydx += m.dydx*factor;
		dydy += m.dydy*factor;
		dydz += m.dydz*factor;
		dzdx += m.dzdx*factor;
		dzdy += m.dzdy*factor;
		dzdz += m.dzdz*factor;
	}
	
	inline float det()
	{
		return dxdx * (dydy*dzdz-dydz*dzdy)
            -dxdy* ( dydx*dzdz-dzdx*dydz)
			+dxdz* (dydx*dzdy-dzdx*dydy);
	}
	
};


#endif

//****************************************************************************//
