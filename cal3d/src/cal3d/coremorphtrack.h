//****************************************************************************//
// coreMorphTrack.h                                                           //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
// Copyright (C) 2012, 2017 Julien 'mp3butcher' Valentin                      //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_COREMORPHTRACK_H
#define CAL_COREMORPHTRACK_H


#include "cal3d/global.h"
#include "cal3d/matrix.h"
#include "cal3d/vector.h"
#include "cal3d/quaternion.h"
#include "cal3d/coremorphkeyframe.h"
namespace cal3d{
	class CalCoreMorphKeyframe;

	class CAL3D_API CalCoreMorphTrack
	{
		// member variables
	protected:

		unsigned int m_morphID;
		unsigned int m_targetMeshID;
		std::vector<unsigned int> m_targetSubMeshIDs;

		/// List of keyframes, always sorted by time.
		std::vector<CalCoreMorphKeyframe> m_keyframes;

		// constructors/destructor
	public:
		CalCoreMorphTrack();
		virtual ~CalCoreMorphTrack();

		bool getState(float time, float & weightOut);
		/** get the morph index (in targetmesh submeshes morphsvec) **/
		const unsigned int& getMorphID() const{ return m_morphID; }
		/** set the morph index (in targetmesh submeshes morphsvec) **/
		void setMorphID(const unsigned int &index){ m_morphID = index; }

		/** get the index of the targetted mesh in the core model**/
		inline const unsigned int& getTargetMesh() const { return m_targetMeshID; }
		/** set the index of the targetted mesh in the core model**/
		inline void setTargetMesh(const unsigned int &index){ m_targetMeshID = index; }


		/** get the number of submesh targetted by this morph track**/
		const unsigned int getTargetSubMeshCount()const{ return m_targetSubMeshIDs.size(); }
		/** get a submesh index targetted by this morph track by its index**/
		inline const unsigned int &getTargetSubMesh(const unsigned int &id)const{ return m_targetSubMeshIDs[id]; }
		/** add a submesh index targetted by this morph track**/
		inline void addTargetSubMesh(const unsigned int &i){ m_targetSubMeshIDs.push_back(i); }
		/** remove a submesh index targetted by this morph track **/
		inline bool removeTargetSubMesh(const unsigned int &name){ for (std::vector<unsigned int>::iterator i = m_targetSubMeshIDs.begin(); i != m_targetSubMeshIDs.begin(); i++){ if (*i == name){ m_targetSubMeshIDs.erase(i); return true; } }return false; }
		/** get the number of keyframe for this morph track **/
		inline int getCoreMorphKeyframeCount() const{ return m_keyframes.empty(); }

		/** get keyframe for this morph track by its index **/
		inline CalCoreMorphKeyframe* getCoreMorphKeyframe(int idx){ return &m_keyframes[idx]; }
		/** get keyframe for this morph track by its index **/
		inline const CalCoreMorphKeyframe* getCoreMorphKeyframe(int idx) const{ return &m_keyframes[idx]; }

		/** add keyframe to this morph track **/
		bool addCoreMorphKeyframe(CalCoreMorphKeyframe *pCoreKeyframe);

		/** get all keyframes for this morph track **/
		inline const std::vector<CalCoreMorphKeyframe> & getVectorCoreMorphKeyframes() const{ return m_keyframes; }
		/** get all keyframes for this morph track **/
		inline std::vector<CalCoreMorphKeyframe> & getVectorCoreMorphKeyframes(){ return m_keyframes; }
		/** reserve array for size keyframes **/
		void reserve(int size)  { m_keyframes.reserve(size); }

		/** scale the track data **/
		void scale(float factor);

	private:
		std::vector<CalCoreMorphKeyframe>::iterator getUpperBound(float time);
	};
}
#endif

//****************************************************************************//
