//****************************************************************************//
// skeleton.h                                                                 //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
// Copyright (C) 2012 Julien 'mp3butcher' Valentin										  //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_SKELETON_H
#define CAL_SKELETON_H

#include "cal3d/global.h"
namespace cal3d{
	class CalCoreSkeleton;
	class CalCoreModel;
	class CalBone;

	class CAL3D_API CalSkeleton
	{
	public:
		CalSkeleton(CalCoreSkeleton *pCoreSkeleton);
		~CalSkeleton();

		//bones collection
		/** Provides access to a bone.returns the bone with the given ID.  **/
		inline const CalBone *getBone(int boneId) const			{ return m_vectorBone[boneId]; }
		/** Provides access to a bone.returns the bone with the given ID.  **/
		inline CalBone *getBone(int boneId)					{ return m_vectorBone[boneId]; }

		/** Provides access to the bone vector.returns the bone vector of the skeleton instance.  **/
		inline const std::vector<CalBone *>& getVectorBone() const            { return m_vectorBone; }
		/** Provides access to the bone vector.returns the bone vector of the skeleton instance.  **/
		inline std::vector<CalBone *>& getVectorBone()			{ return m_vectorBone; }

		/** Provides access to thecore skeleton.returns core skeleton (model) of the skeleton instance.  **/
		inline const CalCoreSkeleton *getCoreSkeleton() const		{ return m_pCoreSkeleton; }
		/** Provides access to thecore skeleton.returns core skeleton (model) of the skeleton instance.  **/
		inline CalCoreSkeleton *getCoreSkeleton()				{ return m_pCoreSkeleton; }


		/** Get the bounding box of the skeleton
		 * @param min The vector where the min values of bb are stored.
		 * @param man The vector where the max values of bb are stored.**/
		void getBoneBoundingBox(float *min, float *max);
		/** compute the bounding box of the skeleton  **/
		void calculateBoundingBoxes();

		/** Locks the state of the skeleton instance by recursively **/
		void lockState();
		/** Calculates the state of the skeleton instance by recursively calculating the states of its bones	**/
		void calculateState();
		/** Clears the state of the skeleton instance by recursively clears the states of its bones	**/
		void clearState();

	private:
		CalCoreSkeleton       *m_pCoreSkeleton;
		std::vector<CalBone *> m_vectorBone;
		bool                   m_isBoundingBoxesComputed;
	};
}
#endif

//****************************************************************************//
