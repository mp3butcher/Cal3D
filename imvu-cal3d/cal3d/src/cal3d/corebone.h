//****************************************************************************//
// corebone.h                                                                 //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
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
  CalCoreBone();
  ~CalCoreBone();

  bool addChildId(int childId);
  void calculateState();
  bool createInternal(const std::string& strName);
  bool createWithName( char const * name );
  void destroy();
  std::list<int>& getListChildId();
  const std::string& getNameInternal();
  void setNameInternal( std::string& str ) { m_strName = str; }
  char const * getName(); // Ptr valid until you call another cal3d function.
  void setName( char const * str ) { m_strName = str; }
  int getParentId();
  CalCoreSkeleton *getCoreSkeleton();
  const CalQuaternion& getRotation();
  const CalQuaternion& getRotationAbsolute();
  const CalQuaternion& getRotationBoneSpace();
  const CalVector& getTranslation();
  const CalVector& getTranslationAbsolute();
  const CalVector& getTranslationBoneSpace();
  Cal::UserData getUserData();
  void setCoreSkeleton(CalCoreSkeleton *pCoreSkeleton);
  void setParentId(int parentId);
  void setRotation(const CalQuaternion& rotation);
  void setRotationBoneSpace(const CalQuaternion& rotation);
  void setTranslation(const CalVector& translation);
  void setTranslationBoneSpace(const CalVector& translation);
  void setUserData(Cal::UserData userData);

  void calculateBoundingBox(CalCoreModel * pCoreModel);
  CalBoundingBox & getBoundingBox();
  void getBoundingData(int planeId,CalVector & position); 
  bool isBoundingBoxPrecomputed();
  void scale(float factor);

  bool hasLightingData();
  void getLightColor( CalVector & );
  void setLightColor( CalVector const & );
  CalLightType  getLightType();
  void setLightType( CalLightType );

private:
  std::string m_strName;
  CalCoreSkeleton *m_pCoreSkeleton;
  int m_parentId;
  std::list<int> m_listChildId;
  CalVector m_translation;
  CalQuaternion m_rotation;
  CalVector m_translationAbsolute;
  CalQuaternion m_rotationAbsolute;
  CalVector m_translationBoneSpace;
  CalQuaternion m_rotationBoneSpace;
  Cal::UserData m_userData;

  CalBoundingBox m_boundingBox;
  CalVector m_boundingPosition[6];
  bool m_boundingBoxPrecomputed;

  CalVector m_lightColor;
  CalLightType m_lightType;
};

#endif

//****************************************************************************//
