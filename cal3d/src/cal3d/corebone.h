//****************************************************************************//
// corebone.h                                                                 //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
// Copyright (C) 2012       Julien 'mp3butcher' Valentin                      //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_COREBONE_H
#define CAL_COREBONE_H


#include "cal3d/global.h"
#include "cal3d/matrix.h"
#include "cal3d/vector.h"
#include "cal3d/quaternion.h"

namespace cal3d{
	class CalCoreSkeleton;
	class CalCoreModel;


	enum CalLightType {
		LIGHT_TYPE_NONE,
		LIGHT_TYPE_OMNI,
		LIGHT_TYPE_DIRECTIONAL,
		LIGHT_TYPE_TARGET,
		LIGHT_TYPE_AMBIENT
	};


	class CAL3D_API CalCoreBone
	{
	public:
		CalCoreBone(const std::string& name);
		~CalCoreBone() { }
		/**get the core skeleton owning this core bone**/
		inline CalCoreSkeleton *getCoreSkeleton()			{ return m_pCoreSkeleton; }
		/**get the core skeleton owning this core bone**/
		inline const CalCoreSkeleton *getCoreSkeleton() const		{ return m_pCoreSkeleton; }
		/**set the core skeleton owning this core bone**/
		inline void setCoreSkeleton(CalCoreSkeleton *pCoreSkeleton)	{ m_pCoreSkeleton = pCoreSkeleton; }

		/**add a core bone ID to the child ID list of the core bone
		* @param childId The ID of the core bone (in the skel) that shoud be added to the child  **/
		inline void addChildId(int childId)				{ m_listChildId.push_back(childId); }
		/**remove a core bone ID to the child ID list of the core bone
		* @param childId The ID of the core bone (in the skel) that shoud be added to the child  **/
		bool removeChildId(int childid);

		/** return the list of children bones indices**/
		inline std::list<int>& getListChildId()				{ return m_listChildId; }
		/** return the list of children bones indices**/
		inline const std::list<int>& getListChildId() const		{ return m_listChildId; }

		/** get the name of the core bone**/
		inline const std::string& getName() const			{ return m_strName; }
		/** get the name of the core bone**/
		inline void setName(const std::string& name)			{ m_strName = name; }

		/** get the index of the parent bone int the skeleton**/
		inline int getParentId() const					{ return m_parentId; }
		/** set the index of the parent bone int the skeleton**/
		inline void setParentId(int parentId)				{ m_parentId = parentId; }

		/**get the user data stored in the core bone instance.**/
		inline Cal::UserData getUserData()				{ return m_userData; }
		/**get the user data stored in the core bone instance.**/
		inline const Cal::UserData getUserData() const			{ return m_userData; }
		/**set the user data stored in the core bone instance.**/
		inline void setUserData(Cal::UserData userData)			{ m_userData = userData; }


		inline const CalQuaternion& getRotation() const				{ return  m_rotation; }
		inline void setRotation(const CalQuaternion& rotation)		{ m_rotation = rotation; }
		inline const CalQuaternion& getRotationBoneSpace() const	{ return m_rotationBoneSpace; }
		inline void setRotationBoneSpace(const CalQuaternion& rotation){ m_rotationBoneSpace = rotation; }
		inline const CalVector& getTranslation() const				{ return m_translation; }
		inline void setTranslation(const CalVector& translation)	{ m_translation = translation; }
		inline const CalVector& getTranslationBoneSpace() const		{ return m_translationBoneSpace; }
		inline void setTranslationBoneSpace(const CalVector& translation){ m_translationBoneSpace = translation; }

		/**init the bb**/
		void initBoundingBox();
		/** calculateBoundingBox the current bounding box.**/
		void calculateBoundingBox(CalCoreModel * pCoreModel);
		/** Returns the current bounding box.**/
		CalBoundingBox & getBoundingBox();
		/** Returns the current bounding box.**/
		const CalBoundingBox & getBoundingBox() const;
		void getBoundingData(int planeId, CalVector & position) const;
		inline bool isBoundingBoxPrecomputed() const				{ return m_boundingBoxPrecomputed; }
		inline void setBoundingBoxPrecomputed(bool inComputed)		{ m_boundingBoxPrecomputed = inComputed; }
		/**Updates the bounding box of the core bone instance to include a given position**/
		bool updateBoundingBox(const CalVector &position);


		/** Calculates the current state.**/
		void calculateState();

		//get updated value after calculatestate
		/**return updated absoltue rotations.**/
		inline const CalQuaternion& getRotationAbsolute() const{ return m_rotationAbsolute; }
		/**return updated absoltue transaltion.**/
		inline const CalVector& getTranslationAbsolute() const{ return m_translationAbsolute; }

		/**rescale all the data that are in the core bone instance and in his childs.**/
		void scale(float factor);


	private:
		std::string      m_strName;
		CalCoreSkeleton *m_pCoreSkeleton;
		int              m_parentId;
		std::list<int>   m_listChildId;
		CalVector        m_translation;
		CalQuaternion    m_rotation;
		CalVector        m_translationAbsolute;
		CalQuaternion    m_rotationAbsolute;
		CalVector        m_translationBoneSpace;
		CalQuaternion    m_rotationBoneSpace;
		Cal::UserData    m_userData;

		CalBoundingBox   m_boundingBox;
		CalVector        m_boundingPosition[6];
		bool             m_boundingBoxPrecomputed;

	};
}
#endif

//****************************************************************************//
