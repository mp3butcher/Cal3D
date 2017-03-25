//****************************************************************************//
// coremesh.h                                                                 //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_COREMESH_H
#define CAL_COREMESH_H


#include "cal3d/global.h"
#include "cal3d/refcounted.h"
#include "cal3d/refptr.h"

namespace cal3d{
	class CalCoreSubmesh;


	class CAL3D_API CalCoreMesh : public cal3d::RefCounted
	{
	protected:
		virtual ~CalCoreMesh();

	public:
		typedef std::vector<CalCoreSubmesh *> CalCoreSubmeshVector;
		CalCoreMesh();

		//return total datasize of the mesh in bytes
		unsigned int size();

		//submeshes
		/**add a submesh.**/
		inline int addCoreSubmesh(CalCoreSubmesh *pCoreSubmesh) { int submeshId = m_vectorCoreSubmesh.size();  m_vectorCoreSubmesh.push_back(pCoreSubmesh);  return submeshId; }
		/**get the submesh at given index.**/
		inline CalCoreSubmesh *getCoreSubmesh(int id){ return m_vectorCoreSubmesh[id]; }
		/**get the submesh at given index.**/
		inline const CalCoreSubmesh *getCoreSubmesh(int id) const{ return m_vectorCoreSubmesh[id]; }
		/**remove the submesh at given index.**/
		void removeCoreSubmesh(int submeshID);

		/**Returns the number of core submeshes.**/
		inline unsigned int getCoreSubmeshCount() const{ return m_vectorCoreSubmesh.size(); }
		/**Returns the core submesh vector.**/
		inline std::vector<CalCoreSubmesh *>& getVectorCoreSubmesh(){ return m_vectorCoreSubmesh; }
		/**Returns the core submesh vector.**/
		inline const std::vector<CalCoreSubmesh *>& getVectorCoreSubmesh() const{ return m_vectorCoreSubmesh; }


		//not in format spec but convenient
		inline void setName(const std::string& name){ m_name = name; }
		inline const std::string& getName(void) const{ return m_name; }

		/**add a CalCoreMesh as MorphId (submeshes must have same num vertices)**/
		int addAsMorphTarget(CalCoreMesh *pCoreMesh);

		/**scale all mesh data by factor**/
		void scale(float factor);
	private:
		std::vector<CalCoreSubmesh *> m_vectorCoreSubmesh;
		std::string                   m_name;
	};
	typedef cal3d::RefPtr<CalCoreMesh> CalCoreMeshPtr;
}
#endif
