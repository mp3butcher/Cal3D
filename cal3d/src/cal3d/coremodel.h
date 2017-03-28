//****************************************************************************//
// coremodel.h                                                                //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_COREMODEL_H
#define CAL_COREMODEL_H


#include "cal3d/coreanimation.h"
#include "cal3d/corematerial.h"
#include "cal3d/coremesh.h"
#include "cal3d/coreskeleton.h"
#include "cal3d/global.h"

namespace cal3d{
	class CalCoreAnimatedMorph;

	class CAL3D_API CalCoreModel
	{
	public:
		CalCoreModel(const std::string& name);
		CalCoreModel(const CalCoreModel& inOther);
		~CalCoreModel();

		/** get the user data stored in the core model instance.**/
		Cal::UserData getUserData(){ return m_userData; }
		/** get the user data stored in the core model instance.**/
		const Cal::UserData getUserData() const{ return m_userData; }
		/** set the user data stored in the core model instance.**/
		void setUserData(Cal::UserData userData){ m_userData = userData; }

		/** get the name of core model .**/
		inline const std::string& getName() const{ return m_strName; }
		/** set the name of core model .**/
		inline void setName(std::string& inName){ m_strName = inName; }

		void scale(float factor);

		// animations
		/*** @return The number of core animations.**/
		inline int getCoreAnimationCount() const{ return m_vectorCoreAnimation.size(); }
		/*** add a core animation.**/
		int addCoreAnimation(CalCoreAnimation *pCoreAnimation);
		/*** remove a core animation.**/
		bool removeCoreAnimation(int id);
		/*** get a core animation by its index in core model vecAnimation**/
		CalCoreAnimation *getCoreAnimation(int coreAnimationId);
		/*** get a core animation by its index in core model vecAnimation**/
		const CalCoreAnimation *getCoreAnimation(int coreAnimationId) const;

		/*** load a core animation from a file**/
		int loadCoreAnimation(const std::string& strFilename);
		/** Loads a core animation and bind it to a name.
		* This function loads a core animation from a file. It is equivalent
		* to calling addAnimName(strAnimationName, loadCoreAnimation(strFilename)).
		* If strAnimationName is already associated to a coreAnimationId because
		* of a previous call to addAnimName, the same coreAnimationId will
		* be used.
		*
		* @param strFilename The file from which the core animation should be loaded
		*                    from.
		* @param strAnimationName A string that is associated with an anim ID number.
		*
		* @return One of the following values:
		*         \li the assigned \b ID of the loaded core animation
		*         \li \b -1 if an error happened**/
		int loadCoreAnimation(const std::string& strFilename, const std::string& strAnimationName);
		int loadCoreAnimationFromXMLstring(const char* buffer);
		int loadCoreAnimationFromXMLstring(const char* buffer, const std::string& strAnimationName);
		int unloadCoreAnimation(const std::string& name);
		int unloadCoreAnimation(int coreAnimationId);
		bool saveCoreAnimation(const std::string& strFilename, int coreAnimationId) const;
		bool addAnimationName(const std::string& strAnimationName, int coreAnimationId);
		int getCoreAnimationId(const std::string& strAnimationName) const;

		// morph animations
		inline int getCoreMorphAnimationCount() const{   return int(m_vectorCoreAnimatedMorph.size());}
		int addCoreAnimatedMorph(CalCoreAnimatedMorph *pCoreAnimatedMorph);
		bool removeCoreAnimatedMorph(int id);
		CalCoreAnimatedMorph *getCoreAnimatedMorph(int coreAnimatedMorphId);
		const CalCoreAnimatedMorph *getCoreAnimatedMorph(int coreAnimatedMorphId) const;
		int getCoreAnimatedMorphId(const std::string& strAnimatedMorphName) const;
		int loadCoreAnimatedMorph(const std::string& strFilename);
		int loadCoreAnimatedMorph(const std::string& strFilename, const std::string& strAnimatedMorphName);
		int loadCoreAnimatedMorphFromXMLstring(const char* buffer, unsigned int bufferLength);
		int loadCoreAnimatedMorphFromXMLstring(const char* buffer, unsigned int bufferLength, const std::string& strAnimatedMorphName);
		int unloadCoreAnimatedMorph(const std::string& name);
		int unloadCoreAnimatedMorph(int coreAnimatedMorphId);
		bool saveCoreAnimatedMorph(const std::string& strFilename, int coreAnimatedMorphId) const;
		bool addAnimatedMorphName(const std::string& strAnimatedMorphName, int coreAnimatedMorphId);

		// materials
		int addCoreMaterial(CalCoreMaterial *pCoreMaterial);
		void cloneCoreMaterials();
		bool createInternal(const std::string& strName);
		bool createWithName(char const * strName);
		bool createCoreMaterialThread(int coreMaterialThreadId);
		int getCoreAnimationMaxId();
		CalCoreMaterial *getCoreMaterial(int coreMaterialId);
		const CalCoreMaterial *getCoreMaterial(int coreMaterialId) const;
		int getCoreMaterialCount() const;
		int getCoreMaterialId(int coreMaterialThreadId, int coreMaterialSetId) const;
		int loadCoreMaterial(const std::string& strFilename);
		int loadCoreMaterialFromXMLstring(const char* buffer, const std::string& strMaterialName);
		int loadCoreMaterial(const std::string& strFilename, const std::string& strMaterialName);
		int loadCoreMaterialFromXMLstring(const char* buffer);
		int unloadCoreMaterial(const std::string& name);
		int unloadCoreMaterial(int coreMaterialId);
		bool saveCoreMaterial(const std::string& strFilename, int coreMaterialId) const;
		bool setCoreMaterialId(int coreMaterialThreadId, int coreMaterialSetId, int coreMaterialId);
		bool addMaterialName(const std::string& strMaterialName, int coreMaterialId);
		int getCoreMaterialId(const std::string& strMaterialName) const;

		// meshes
		int addCoreMesh(CalCoreMesh *pCoreMesh);
		void replaceCoreMesh(int coreMeshId, CalCoreMesh *pCoreMesh);
		CalCoreMesh *getCoreMesh(int coreMeshId);
		const CalCoreMesh *getCoreMesh(int coreMeshId) const;
		int getCoreMeshCount() const;
		int loadCoreMesh(const std::string& strFilename);
		int loadCoreMeshFromXMLstring(const char* buffer, const std::string& strMeshName);
		int loadCoreMesh(const std::string& strFilename, const std::string& strMeshName);
		int loadCoreMeshFromXMLstring(const char* buffer);
		int unloadCoreMesh(const std::string& name);
		int unloadCoreMesh(int coreMeshId);
		bool saveCoreMesh(const std::string& strFilename, int coreMeshId) const;
		bool addMeshName(const std::string& strMeshName, int coreMeshId);
		int getCoreMeshId(const std::string& strMeshName) const;

		// skeleton
		inline CalCoreSkeleton *getCoreSkeleton(){ return m_pCoreSkeleton.get(); }
		inline const CalCoreSkeleton *getCoreSkeleton() const{ return m_pCoreSkeleton.get(); }
		inline void setCoreSkeleton(CalCoreSkeleton *pCoreSkeleton){ m_pCoreSkeleton = pCoreSkeleton; }
		bool loadCoreSkeleton(const std::string& strFilename);
		bool loadCoreSkeletonFromXMLstring(const char* buffer);
		bool saveCoreSkeleton(const std::string& strFilename) const;
		void addBoneName(const std::string& strBoneName, int boneId);
		int getBoneId(const std::string& strBoneName) const;


		// member variables
	private:
		std::string                           m_strName;
		CalCoreSkeletonPtr                    m_pCoreSkeleton;
		std::vector<CalCoreAnimationPtr>      m_vectorCoreAnimation;
		std::vector<CalCoreAnimatedMorph *>   m_vectorCoreAnimatedMorph;

		std::vector<CalCoreMeshPtr>           m_vectorCoreMesh;
		std::vector<CalCoreMeshPtr>           m_vectorMorphMesh;
		std::vector<CalCoreMaterialPtr>       m_vectorCoreMaterial;
		std::map<int, std::map<int, int> >    m_mapmapCoreMaterialThread;
		Cal::UserData                         m_userData;
		std::map<std::string, int>            m_animationName;
		std::map<std::string, int>            m_animatedMorphName;
		std::map<std::string, int>            m_materialName;
		std::map<std::string, int>            m_meshName;
		unsigned int                          m_magic;

	};
}
#endif

//****************************************************************************//
