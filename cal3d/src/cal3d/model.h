//****************************************************************************//
// model.h                                                                    //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_MODEL_H
#define CAL_MODEL_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "cal3d/global.h"
#include "cal3d/vector.h"


//****************************************************************************//
// Forward declarations                                                       //
//****************************************************************************//

class CalCoreModel;
class CalSkeleton;
class CalMixer;
class CalMorphTargetMixer;
class CalPhysique;
class CalSpringSystem;
class CalRenderer;
class CalMesh;

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

 /*****************************************************************************/
/** The model class.
  *****************************************************************************/

class CAL3D_API CalModel
{
// member variables
protected:
  CalCoreModel *m_pCoreModel;
  CalSkeleton *m_pSkeleton;
  CalMixer *m_pMixer;
  CalMorphTargetMixer *m_pMorphTargetMixer;
  CalPhysique *m_pPhysique;
  CalSpringSystem *m_pSpringSystem;
  CalRenderer *m_pRenderer;
  Cal::UserData m_userData;
  std::vector<CalMesh *> m_vectorMesh;
  CalBoundingBox m_boundingBox;

// constructors/destructor
public: 
  CalModel();
  virtual ~CalModel();

// member functions
public:
  bool attachMesh(int coreMeshId);
  bool create(CalCoreModel *pCoreModel);
  void destroy();
  bool detachMesh(int coreMeshId);
  CalCoreModel *getCoreModel();
  CalMesh *getMesh(int coreMeshId);
  CalMixer *getMixer();
  CalMorphTargetMixer *getMorphTargetMixer();
  CalPhysique *getPhysique();
  CalRenderer *getRenderer();
  CalSkeleton *getSkeleton();
  CalSpringSystem *getSpringSystem();
  CalBoundingBox & getBoundingBox(bool precision = false);
  Cal::UserData getUserData();
  std::vector<CalMesh *>& getVectorMesh();
  void setLodLevel(float lodLevel);
  void setMaterialSet(int setId);
  void setUserData(Cal::UserData userData);
  void update(float deltaTime);
  void disableInternalData();
};

#endif

//****************************************************************************//
