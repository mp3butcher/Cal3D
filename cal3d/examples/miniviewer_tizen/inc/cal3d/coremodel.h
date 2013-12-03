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


class CalCoreAnimatedMorph;


class CAL3D_API CalCoreModel
{
public:
  CalCoreModel(const std::string& name);
  CalCoreModel(const CalCoreModel& inOther);
  ~CalCoreModel();

  Cal::UserData getUserData();
  const Cal::UserData getUserData() const;
  void setUserData(Cal::UserData userData);
  const std::string& getName() const;
  void setName( const char* inName );

  void scale(float factor);

  // animations

  int getNumCoreAnimations() const;

  int addCoreAnimation(CalCoreAnimation *pCoreAnimation);
  bool removeCoreAnimation( int id );
  CalCoreAnimation *getCoreAnimation(int coreAnimationId);
  const CalCoreAnimation *getCoreAnimation(int coreAnimationId) const;
  bool removeCoreAnimatedMorph( int id );
  int getCoreAnimationCount() const;
  int loadCoreAnimation(const std::string& strFilename);
  int loadCoreAnimation(void* buffer, const std::string& strAnimationName);
  int loadCoreAnimation(const std::string& strFilename, const std::string& strAnimationName);
  int loadCoreAnimation(void* buffer);
  int unloadCoreAnimation(const std::string& name);
  int unloadCoreAnimation(int coreAnimationId);
  bool saveCoreAnimation(const std::string& strFilename, int coreAnimationId) const;
  bool addAnimationName(const std::string& strAnimationName, int coreAnimationId);
  int getCoreAnimationId(const std::string& strAnimationName) const;

  // morph animations
  int addCoreAnimatedMorph(CalCoreAnimatedMorph *pCoreAnimatedMorph);
  CalCoreAnimatedMorph *getCoreAnimatedMorph(int coreAnimatedMorphId);
  const CalCoreAnimatedMorph *getCoreAnimatedMorph(int coreAnimatedMorphId) const;
  int getCoreAnimatedMorphId(const std::string& strAnimatedMorphName) const;
  int getNumCoreAnimatedMorphs() const;
  int getCoreMorphAnimationCount() const;
  int loadCoreAnimatedMorph(const std::string& strFilename);
  int loadCoreAnimatedMorph(const std::string& strFilename, const std::string& strAnimatedMorphName);
  int loadCoreAnimatedMorph(void* buffer, unsigned int bufferLength);
  int loadCoreAnimatedMorph(void* buffer, unsigned int bufferLength, const std::string& strAnimatedMorphName);
  int unloadCoreAnimatedMorph(const std::string& name);
  int unloadCoreAnimatedMorph(int coreAnimatedMorphId);
  bool saveCoreAnimatedMorph(const std::string& strFilename, int coreAnimatedMorphId) const;
  bool addAnimatedMorphName(const std::string& strAnimatedMorphName, int coreAnimatedMorphId);

  // materials
  int addCoreMaterial(CalCoreMaterial *pCoreMaterial);
  void cloneCoreMaterials();
  bool createInternal(const std::string& strName);
  bool createWithName( char const * strName);
  bool createCoreMaterialThread(int coreMaterialThreadId);
  int getCoreAnimationMaxId();
  CalCoreMaterial *getCoreMaterial(int coreMaterialId);
  const CalCoreMaterial *getCoreMaterial(int coreMaterialId) const;
  int getCoreMaterialCount() const;
  int getCoreMaterialId(int coreMaterialThreadId, int coreMaterialSetId) const;
  int loadCoreMaterial(const std::string& strFilename);
  int loadCoreMaterial(void* buffer, const std::string& strMaterialName);
  int loadCoreMaterial(const std::string& strFilename, const std::string& strMaterialName);
  int loadCoreMaterial(void* buffer);
  int unloadCoreMaterial(const std::string& name);
  int unloadCoreMaterial(int coreMaterialId);
  bool saveCoreMaterial(const std::string& strFilename, int coreMaterialId) const;
  bool setCoreMaterialId(int coreMaterialThreadId, int coreMaterialSetId, int coreMaterialId);
  bool addMaterialName(const std::string& strMaterialName, int coreMaterialId);
  int getCoreMaterialId(const std::string& strMaterialName) const;

  // meshes
  int addCoreMesh(CalCoreMesh *pCoreMesh);
  void replaceCoreMesh( int coreMeshId, CalCoreMesh *pCoreMesh );
  CalCoreMesh *getCoreMesh(int coreMeshId);
  const CalCoreMesh *getCoreMesh(int coreMeshId) const;
  int getCoreMeshCount() const;
  int loadCoreMesh(const std::string& strFilename);
  int loadCoreMesh(void* buffer, const std::string& strMeshName);
  int loadCoreMesh(const std::string& strFilename, const std::string& strMeshName);
  int loadCoreMesh(void* buffer);
  int unloadCoreMesh(const std::string& name);
  int unloadCoreMesh(int coreMeshId);
  bool saveCoreMesh(const std::string& strFilename, int coreMeshId) const;
  bool addMeshName(const std::string& strMeshName, int coreMeshId);
  int getCoreMeshId(const std::string& strMeshName) const;

  // skeleton
  CalCoreSkeleton *getCoreSkeleton();
  const CalCoreSkeleton *getCoreSkeleton() const;
  bool loadCoreSkeleton(const std::string& strFilename);
  bool loadCoreSkeleton(void* buffer);
  bool saveCoreSkeleton(const std::string& strFilename) const;
  void setCoreSkeleton(CalCoreSkeleton *pCoreSkeleton);
  void addBoneName(const std::string& strBoneName, int boneId);
  int getBoneId(const std::string& strBoneName) const;
  void setCoreMeshManagementOn( bool p ) { m_coreMeshManagement = p; }
  bool getCoreMeshManagementOn() { return m_coreMeshManagement; }
  void setCoreAnimationManagementOn( bool p ) { m_coreAnimationManagement = p; }
  bool getCoreAnimationManagementOn() { return m_coreAnimationManagement; }

// member variables
private:
  std::string                           m_strName;
  CalCoreSkeletonPtr                    m_pCoreSkeleton;
  std::vector<CalCoreAnimationPtr>      m_vectorCoreAnimation;
  std::vector<CalCoreAnimatedMorph *>   m_vectorCoreAnimatedMorph;

  std::vector<CalCoreMeshPtr>           m_vectorCoreMesh;
  std::vector<CalCoreMaterialPtr>       m_vectorCoreMaterial;
  std::map<int, std::map<int, int> >    m_mapmapCoreMaterialThread;
  Cal::UserData                         m_userData;
  std::map<std::string, int>            m_animationName;
  std::map<std::string, int>            m_animatedMorphName;
  std::map<std::string, int>            m_materialName;
  std::map<std::string, int>            m_meshName;
  bool                                  m_coreMeshManagement;
  bool                                  m_coreAnimationManagement;
  unsigned int                          m_magic;

};

#endif

//****************************************************************************//
