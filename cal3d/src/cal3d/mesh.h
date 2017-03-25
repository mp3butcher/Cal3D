//****************************************************************************//
// mesh.h                                                                     //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_MESH_H
#define CAL_MESH_H


#include "cal3d/global.h"

namespace cal3d{
	class CalCoreModel;
	class CalCoreMesh;
	class CalSubmesh;


	class CAL3D_API CalMesh
	{
		// constructors/destructor
	public:
		CalMesh(CalCoreMesh *pCoreMesh);
		~CalMesh();
		/**returns the core mesh on which this mesh instance is based on**/
		CalCoreMesh *getCoreMesh();
		/**returns the core mesh on which this mesh instance is based on**/
		const CalCoreMesh *getCoreMesh() const;

		/**returns the count of composite submeshes**/
		int getSubmeshCount() const;
		/**returns the submesh at the given index**/
		CalSubmesh *getSubmesh(int id);
		/**returns the submesh at the given index**/
		const CalSubmesh *getSubmesh(int id) const;
		/**returns the submeshes**/
		std::vector<CalSubmesh *>& getVectorSubmesh();
		/**returns the submeshes**/
		const std::vector<CalSubmesh *>& getVectorSubmesh() const;


		/**sets the LOD level of all submeshes**/
		void setLodLevel(float lodLevel);
		/** Sets the material index setId to all submeshes according materialset owned by CalCoreModel.**/
		void setMaterialSet(int setId, CalCoreModel  *m_pCoreModel);
		/**Disable internal data (and thus springs system)**/
		void disableInternalData();

	private:
		CalCoreMesh              *m_pCoreMesh;
		std::vector<CalSubmesh *> m_vectorSubmesh;
	};
}
#endif

//****************************************************************************//
