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

#include "cal3d/global.h"

namespace cal3d{
	class CalModel;
	class CalSubmesh;
	class CalVector;


	class CAL3D_API CalPhysique
	{
	public:
		CalPhysique(CalModel* pModel);
		virtual ~CalPhysique() { }

		int calculateTangentSpaces(CalSubmesh *pSubmesh, int mapId, float *pTangentSpaceBuffer, int stride = 0) const;
		virtual int calculateNormals(CalSubmesh *pSubmesh, float *pNormalBuffer, int stride = 0) const;
		virtual int calculateVertices(CalSubmesh *pSubmesh, float *pVertexBuffer, int stride = 0) const;
		CalVector calculateVertex(CalSubmesh *pSubmesh, int vertexId);
		virtual int calculateVerticesAndNormals(CalSubmesh *pSubmesh, float *pVertexBuffer, int stride = 0) const;
		virtual int calculateVerticesNormalsAndTexCoords(CalSubmesh *pSubmesh, float *pVertexBuffer, int NumTexCoords = 1) const;
		void update();

		/*****************************************************************************/
		/** Sets the normalization flag to true or false.
		  *
		  * This function sets the normalization flag on or off. If off, the normals
		  * calculated by Cal3D will not be normalized. Instead, this transform is left
		  * up to the user.
		  *****************************************************************************/

		void setNormalization(bool normalize) { m_Normalize = normalize; }
		void setAxisFactorX(float factor) { m_axisFactorX = factor;         m_Normalize = true; }
		void setAxisFactorY(float factor) { m_axisFactorY = factor;         m_Normalize = true; }
		void setAxisFactorZ(float factor) { m_axisFactorZ = factor;         m_Normalize = true; }

	protected:
		CalModel *m_pModel;
		bool      m_Normalize;
		float     m_axisFactorX;
		float     m_axisFactorY;
		float     m_axisFactorZ;
	};
}
#endif

//****************************************************************************//
