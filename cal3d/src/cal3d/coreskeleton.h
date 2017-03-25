//****************************************************************************//
// coreskeleton.h                                                             //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
// Copyright (C) 2012 Julien 'mp3butcher' Valentin                            //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_CORESKELETON_H
#define CAL_CORESKELETON_H

#include "cal3d/global.h"
#include "cal3d/vector.h"
#include "cal3d/refcounted.h"
#include "cal3d/refptr.h"

namespace cal3d{
	class CalCoreBone;
	class CalCoreModel;


	class CAL3D_API CalCoreSkeleton : public cal3d::RefCounted
	{
	protected:
		~CalCoreSkeleton();

	public:
		CalCoreSkeleton();

		/** set the name of the core skeleton **/
		void setName(const std::string& name);
		/** set the name of the cor skeleton **/
		const std::string& getName(void) const;

		//bones vector
		/** add a Bone to the core skeleton **/
		int addCoreBone(CalCoreBone *pCoreBone);
		/** return bones count of the core skeleton **/
		inline unsigned int getNumCoreBones() const { return  m_vectorCoreBone.size(); }

		/** return core bone of the core skeleton at given index **/
		inline CalCoreBone *getCoreBone(int coreBoneId){ return m_vectorCoreBone[coreBoneId]; }
		/** return core bone of the core skeleton at given index **/
		inline const CalCoreBone *getCoreBone(int coreBoneId) const{ return m_vectorCoreBone[coreBoneId]; }
		/** return vector of core bone compositing the core skeleton **/
		inline std::vector<CalCoreBone *>& getVectorCoreBone(){ return m_vectorCoreBone; }
		/** return vector of core bone compositing the core skeleton **/
		inline const std::vector<CalCoreBone *>& getVectorCoreBone() const{ return m_vectorCoreBone; }

		/** returns the vector of ids of root core bones **/
		inline const std::vector<int>& getVectorRootCoreBoneId() const{ return m_vectorRootCoreBoneId; }
		/** calculates the current state of the core skeleton by calculating all the core bone states**/
		void calculateState();
		/** calculates the bounding box of the core skeleton **/
		void calculateBoundingBoxes(CalCoreModel *pCoreModel);

		//helpers

		/** associate a name to a bone **/
		bool mapCoreBoneName(int coreBoneId, const std::string& strName);
		/** return the index of the bone mapped the given name **/
		int getCoreBoneId(const std::string& strName) const;
		/** return the bone mapped the given name **/
		CalCoreBone *getCoreBone(const std::string& strName);
		/** return the bone mapped the given name **/
		const CalCoreBone *getCoreBone(const std::string& strName) const;
		/** scale all the skeleton inner data by factor **/
		void scale(float factor);
	private:
		std::vector<CalCoreBone *>   m_vectorCoreBone;
		std::map< std::string, int > m_mapCoreBoneNames;
		std::vector<int>             m_vectorRootCoreBoneId;
		std::string                  m_name;
	};
	typedef cal3d::RefPtr<CalCoreSkeleton> CalCoreSkeletonPtr;
}
#endif

//****************************************************************************//
