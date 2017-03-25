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

#include "cal3d/global.h"
#include "cal3d/vector.h"

namespace cal3d{
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
	public:
		CalSpringSystem(CalModel *pModel);
		~CalSpringSystem() { }

		// member functions	
	public:

		/**get the gravity vector of the spring system instance.**/
		const CalVector & getGravityVector() const        { return m_vGravity; }
		/**set the gravity vector of the spring system instance.**/
		void setGravityVector(const CalVector & vGravity) { m_vGravity = vGravity; }
		/** get the force vector of the spring system instance**/
		const CalVector & getForceVector() const          { return m_vForce; }
		/** set the force vector of the spring system instance**/
		void setForceVector(const CalVector & vForce)     { m_vForce = vForce; }
		/**get Enable or disable the collision system**/
		bool isCollisionDetection()const                  { return m_collision; }
		/**set Enable or disable the collision system**/
		void setCollisionDetection(bool collision)        { m_collision = collision; }


		void calculateForces(CalSubmesh *pSubmesh, float deltaTime);
		void calculateVertices(CalSubmesh *pSubmesh, float deltaTime);
		void update(float deltaTime);
		void resetPositions();

	private:
		CalModel *m_pModel;
		CalVector m_vGravity;
		CalVector m_vForce;
		bool      m_collision;
	};
}
#endif

//****************************************************************************//
