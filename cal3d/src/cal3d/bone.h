//****************************************************************************//
// bone.h                                                                     //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_BONE_H
#define CAL_BONE_H


#include "cal3d/global.h"
#include "cal3d/vector.h"
#include "cal3d/quaternion.h"
#include "cal3d/matrix.h"
#include "cal3d/corebone.h"

namespace cal3d{
	//class CalCoreBone;
	class CalSkeleton;
	class CalModel;
	class CalCoreModel;


	class CAL3D_API CalBone
	{
	public:
		CalBone(CalCoreBone *coreBone);
		~CalBone() { }

		/**get the core bone model of this bone instance**/
		CalCoreBone *getCoreBone()                              { return m_pCoreBone; }
		/**get the core bone model of this bone instance**/
		const CalCoreBone *getCoreBone() const                  { return m_pCoreBone; }

		/**set the skeleton owning this bone**/
		inline void setSkeleton(CalSkeleton *pSkeleton)         { m_pSkeleton = pSkeleton; }
		/**get the skeleton owning this bone**/
		inline const CalSkeleton * getSkeleton()const           { return m_pSkeleton; }

		/** Calculates the bounding box.**/
		virtual void calculateBoundingBox();
		/** Returns the current bounding box.**/
		inline const CalBoundingBox & getBoundingBox()          { return m_boundingBox; }

		/**get updated absolute transform matrix of the bone**/
		inline const CalMatrix& getTransformMatrix() const      { return m_transformMatrix; }
		/**get updated absolute translation of the bone**/
		inline const CalVector& getTranslationAbsolute() const{ return m_translationAbsolute; }
		/**returns the updated absolute rotation of the bone instance.**/
		inline const CalQuaternion& getRotationAbsolute() const { return m_rotationAbsolute; }

		/**get updated translation to bring a point into the bone instance space.**/
		inline const CalVector& getTranslationBoneSpace() const { return m_translationBoneSpace; }
		/**returns the updated rotation to bring a point into the bone instance space.**/
		inline const CalQuaternion& getRotationBoneSpace() const{ return m_rotationBoneSpace; }


		/** Caveat: For theses changes to appear, calculateState() must be called  afterwards.**/
		/**sets the current relative translation of the bone instance.**/
		inline void setTranslation(const CalVector& translation){ m_translation = translation;  m_accumulatedWeightAbsolute = 1.0f; m_accumulatedWeight = 1.0f; }
		/**get  translation of the bone relative**/
		inline const CalVector& getTranslation() const          { return m_translation; }
		/**sets the current relative rotation of the bone instance.**/
		inline void setRotation(const CalQuaternion& rotation)  { m_rotation = rotation;        m_accumulatedWeightAbsolute = 1.0f;  m_accumulatedWeight = 1.0f; }
		/**get  rotation of the bone relative**/
		inline const CalQuaternion& getRotation() const         { return m_rotation; }


		/** updates AbsoluteTransformMatrix and BoneSpaceTransform of the bone instance and all its children.**/
		void calculateState();
		/**clear AbsoluteTransformMatrix and BoneSpaceTransform of the bone instance and all its children.**/
		void clearState();
		/**Resets the bone transform state variables for rotation and translation.**/
		void setCoreTransformStateVariables();
		/**Resets the bone to its core state**/
		void setCoreState();
		/**Resets the bone to its core state for all child bones**/
		void setCoreStateRecursive();
		/** locks the current state (absolute translation and rotation) of the bone instance and all its children**/
		void lockState();
	protected:
		friend class CalMixer;
		// w.r.t. absolute coord system in 3dsMax (Z up), not local coord of bone.
		inline void setMeshScaleAbsolute(CalVector const & sv) { m_meshScaleAbsolute = sv; }
		/** interpolates the current state (relative translation and
		* rotation) of the bone instance to another state of a given weight.
		*
		* @param unrampedWeight The blending weight, not incorporating ramp value
		* @param translation The relative translation to be interpolated to.
		* @param rotation The relative rotation to be interpolated to.
		* @param scale Optional scale from 0-1 applies to transformation directly without affecting weights.
		* @param replace If true, subsequent animations will have their weight attenuated by 1 - rampValue.
		* @param rampValue Amount to attenuate weight when ramping in/out the animation.
		* @param absoluteTranslation If true, use the translation as absolute, otherwise add it to the current bone translation as relative.**/
		void blendState(float unrampedWeight, const CalVector& translation,
			const CalQuaternion & rotation, float scale = 1.0f,
			bool replace = false, float rampValue = 1.0f,
			bool absoluteTranslation = true);

	private:
		CalCoreBone   *m_pCoreBone;
		CalSkeleton   *m_pSkeleton;
		float          m_accumulatedWeight;
		float          m_accumulatedWeightAbsolute;
		float          m_accumulatedReplacementAttenuation;
		float          m_firstBlendScale;
		CalVector      m_meshScaleAbsolute; // w.r.t. absolute coord system in 3dsMax (Z up), not local coord of bone.
		CalVector      m_translation;
		CalQuaternion  m_rotation;
		CalVector      m_translationAbsolute;
		CalQuaternion  m_rotationAbsolute;
		CalVector      m_translationBoneSpace;
		CalQuaternion  m_rotationBoneSpace;
		CalMatrix      m_transformMatrix;
		CalBoundingBox m_boundingBox;
	};
}
#endif

//****************************************************************************//
