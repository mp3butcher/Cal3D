//----------------------------------------------------------------------------//
// Quadric.h                                                                  //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef QUADRIC_H
#define QUADRIC_H

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CQuadric
{
// member variables
protected:
	double m_aa, m_ab, m_ac, m_ad;
	double       m_bb, m_bc, m_bd;
	double             m_cc, m_cd;
	double                   m_dd;
	double m_area;

// constructors/destructor
public:
	CQuadric();
	CQuadric(double nx, double ny, double nz, double d, double area);
	CQuadric(double x1, double y1, double z1, double x2, double y2, double z2, double nx, double ny, double nz);
	CQuadric(const CQuadric& q);
	virtual ~CQuadric();

// member functions
public:
	void Add(const CQuadric& q);
	double Evaluate(double x, double y, double z);
	double GetArea();
	void Scale(double factor);
	void Set(double nx, double ny, double nz, double d, double area);
	void Set(double x1, double y1, double z1, double x2, double y2, double z2, double nx, double ny, double nz);
	void Set(const CQuadric& q);

protected:
};

#endif

//----------------------------------------------------------------------------//
