//----------------------------------------------------------------------------//
// Quadric.cpp                                                                //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#include "StdAfx.h"
#include "Quadric.h"

//----------------------------------------------------------------------------//
// Debug                                                                      //
//----------------------------------------------------------------------------//

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//----------------------------------------------------------------------------//
// Constructors                                                               //
//----------------------------------------------------------------------------//

CQuadric::CQuadric()
{
	m_aa = m_ab = m_ac = m_ad = 0.0;
	       m_bb = m_bc = m_bd = 0.0;
	              m_cc = m_cd = 0.0;
	                     m_dd = 0.0;
	m_area = 0.0f;
}

CQuadric::CQuadric(double nx, double ny, double nz, double d, double area)
{
	Set(nx, ny, nz, d, area);
}

CQuadric::CQuadric(double x1, double y1, double z1, double x2, double y2, double z2, double nx, double ny, double nz)
{
	Set(x1, y1, z1, x2, y2, z2, nx, ny, nz);
}

CQuadric::CQuadric(const CQuadric& q)
{
	Set(q);
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CQuadric::~CQuadric()
{
}

//----------------------------------------------------------------------------//
// Add a given quadric to this quadric                                        //
//----------------------------------------------------------------------------//

void CQuadric::Add(const CQuadric& q)
{
	m_aa += q.m_aa; m_ab += q.m_ab; m_ac += q.m_ac; m_ad += q.m_ad;
	                m_bb += q.m_bb; m_bc += q.m_bc; m_bd += q.m_bd;
					                        m_cc += q.m_cc; m_cd += q.m_cd;
					                                        m_dd += q.m_dd;
	m_area += q.m_area;
}

//----------------------------------------------------------------------------//
// Evaluate the quadric error for a given vertex position                     //
//----------------------------------------------------------------------------//

double CQuadric::Evaluate(double x, double y, double z)
{
	return x * x * m_aa + 2.0 * x * y * m_ab + 2.0 * x * z * m_ac + 2.0 * x * m_ad
	                    +       y * y * m_bb + 2.0 * y * z * m_bc + 2.0 * y * m_bd
	                                           +     z * z * m_cc + 2.0 * z * m_cd
                                                                +           m_dd;
}

//----------------------------------------------------------------------------//
// Get the area of this quadric                                               //
//----------------------------------------------------------------------------//

double CQuadric::GetArea()
{
	return m_area;
}

//----------------------------------------------------------------------------//
// Scale the quadric with a given factor                                      //
//----------------------------------------------------------------------------//

void CQuadric::Scale(double factor)
{
	m_aa *= factor; m_ab *= factor; m_ac *= factor; m_ad *= factor;
	                m_bb *= factor; m_bc *= factor; m_bd *= factor;
		        			                m_cc *= factor; m_cd *= factor;
					                                        m_dd *= factor;
}

//----------------------------------------------------------------------------//
// Set the quadric to a new "plane"                                           //
//----------------------------------------------------------------------------//

void CQuadric::Set(double nx, double ny, double nz, double d, double area)
{
	m_aa = nx * nx; m_ab = nx * ny; m_ac = nx * nz; m_ad = nx * d;
	                m_bb = ny * ny; m_bc = ny * nz; m_bd = ny * d;
	                                m_cc = nz * nz; m_cd = nz * d;
	                                                m_dd =  d * d;
	m_area = area;
}

//----------------------------------------------------------------------------//
// Set the quadric to a given edge and face noemal                            //
//----------------------------------------------------------------------------//

void CQuadric::Set(double x1, double y1, double z1, double x2, double y2, double z2, double nx, double ny, double nz)
{
	double dirx, diry, dirz;
	dirx = x2 - x1;
	diry = y2 - y1;
	dirz = z2 - z1;

	double vx, vy, vz;
	vx = ny * dirz - diry * nz;
	vy = nz * dirx - dirz * nx;
	vz = nx * diry - dirx * ny;

	double length;
	length = sqrt(vx * vx + vy * vy + vz * vz);
	vx /= length;
	vy /= length;
	vz /= length;

	double d;
	d = -(vx * x2 + vy * y2 + vz * z2);

	Set(vx, vy, vz, d, 1.0f);

	double weight;
	weight = dirx * dirx + diry * diry + dirz * dirz;

	Scale(weight);
}

//----------------------------------------------------------------------------//
// Set the quadric to a given quadric                                         //
//----------------------------------------------------------------------------//

void CQuadric::Set(const CQuadric& q)
{
	m_aa = q.m_aa; m_ab = q.m_ab; m_ac = q.m_ac; m_ad = q.m_ad;
	               m_bb = q.m_bb; m_bc = q.m_bc; m_bd = q.m_bd;
					                      m_cc = q.m_cc; m_cd = q.m_cd;
					                                     m_dd = q.m_dd;
	m_area = q.m_area;
}

//----------------------------------------------------------------------------//
