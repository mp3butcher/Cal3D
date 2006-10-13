/*******************************************************
* coremodel_test.cpp
*
* Unittest for testing coremodel.cpp
*
********************************************************/

#include "UnitTest++.h"
#include "Unittest.h"

#include "cal3d/coremodel.h"
#include "cal3d/coreanimation.h"
#include "cal3d/corebone.h"
#include "cal3d/corekeyframe.h"
#include "cal3d/coreskeleton.h"
#include "cal3d/coretrack.h"

#include "cal3d/saver.h"
#include "cal3d/loader.h"


TEST( CalCoreModel_construction )
{
  TEST_INIT( CalCoreModel::CalCoreModel );

  CalCoreModel *m_pCoreModel = new CalCoreModel("test-model");

  TEST_RES( object init. failed );  
}

TEST( CalCoreModel_getCoreAnimationId )
{
  TEST_INIT( CalCoreModel::getCoreAnimationId );

  CalCoreModel *m_pCoreModel = new CalCoreModel("test-model");

  CalCoreAnimation *m_pCoreAnim = new CalCoreAnimation();
  m_pCoreModel->addCoreAnimation(m_pCoreAnim);

  m_pCoreModel->addAnimationName("test-anim", 0);
  CHECK ( 0 == m_pCoreModel->getCoreAnimationId("test-anim"));
  
  TEST_RES( getCoreAnimationId failed )
}

TEST( CalCoreModel_loadCoreAnimation )
{
  TEST_INIT( CalCoreModel::loadCoreAnimation );

  // we save a CoreAnimation to a file
  CalCoreSkeleton *m_pCoreSkel = new CalCoreSkeleton();
  CalCoreAnimation *m_pCoreAnim = new CalCoreAnimation();
  CalCoreTrack *m_pCoreTrack = new CalCoreTrack();
  CalCoreKeyframe *m_pCoreKeyframe = new CalCoreKeyframe();
  CalCoreBone *m_pCoreBone = new CalCoreBone("bone1");

  m_pCoreSkel->addCoreBone(m_pCoreBone);
  m_pCoreTrack->addCoreKeyframe(m_pCoreKeyframe);  
  m_pCoreTrack->setCoreBoneId(0);
  m_pCoreAnim->addCoreTrack(m_pCoreTrack);
  m_pCoreAnim->setDuration(1.0);
  CalSaver::saveCoreAnimation( "tmp.cov", m_pCoreAnim, NULL );

  // we try to load it
  CalCoreModel *m_pCoreModel = new CalCoreModel("test-model");
  m_pCoreModel->setCoreSkeleton(m_pCoreSkel);

  CHECK ( -1 != m_pCoreModel->loadCoreAnimation("tmp.cov", "test-anim"));
  m_pCoreModel->unloadCoreAnimation(0);
  CHECK ( -1 != m_pCoreModel->loadCoreAnimation("tmp.cov", "test-anim"));
  
  TEST_RES( loadCoreAnimation failed )
}

TEST( CalCoreModel_loadCoreMaterial )
{
  TEST_INIT( CalCoreModel::loadCoreMaterial );

  // we save a CoreMaterial to a file
  CalCoreMaterial *m_pCoreMat = new CalCoreMaterial();
  CalSaver::saveCoreMaterial( "tmp.cov", m_pCoreMat );

  // we try to load it
  CalCoreModel *m_pCoreModel = new CalCoreModel("test-model");
  CalCoreSkeleton *m_pCoreSkel = new CalCoreSkeleton();
  m_pCoreModel->setCoreSkeleton(m_pCoreSkel);

  CHECK ( -1 != m_pCoreModel->loadCoreMaterial("tmp.cov", "test-mat"));
  m_pCoreModel->unloadCoreMaterial(0);
  CHECK ( -1 != m_pCoreModel->loadCoreMaterial("tmp.cov", "test-mat"));
  
  TEST_RES( loadCoreMaterial failed )
}

TEST( CalCoreModel_loadCoreMesh )
{
  TEST_INIT( CalCoreModel::loadCoreMesh );

  // we save a CoreMaterial to a file
  CalCoreMesh *m_pCoreMesh = new CalCoreMesh();
  CalSaver::saveCoreMesh( "tmp.cov", m_pCoreMesh );

  // we try to load it
  CalCoreModel *m_pCoreModel = new CalCoreModel("test-model");
  CalCoreSkeleton *m_pCoreSkel = new CalCoreSkeleton();
  m_pCoreModel->setCoreSkeleton(m_pCoreSkel);

  CHECK ( -1 != m_pCoreModel->loadCoreMesh("tmp.cov", "test-mesh"));
  m_pCoreModel->unloadCoreMesh(0);
  CHECK ( -1 != m_pCoreModel->loadCoreMesh("tmp.cov", "test-mesh"));
  
  TEST_RES( loadCoreMesh failed )
}

TEST( CalCoreModel_scale )
{
  TEST_INIT( CalCoreModel::getCoreAnimationId );

  CalCoreModel *m_pCoreModel = new CalCoreModel("test-model");
  CalCoreSkeleton *m_pCoreSkel = new CalCoreSkeleton();
  m_pCoreModel->setCoreSkeleton(m_pCoreSkel);

  CalCoreAnimation *m_pCoreAnim = new CalCoreAnimation();
  m_pCoreModel->addCoreAnimation(m_pCoreAnim);

  CalCoreMesh *m_pCoreMesh = new CalCoreMesh();
  m_pCoreModel->addCoreMesh(m_pCoreMesh);

  m_pCoreModel->scale(0.5);
  
  TEST_RES( scale failed )
}

TEST( CalCoreModel_getBoneId )
{
  TEST_INIT( CalCoreModel::getBoneId );

  CalCoreModel *m_pCoreModel = new CalCoreModel("test-model");
  CalCoreSkeleton *m_pCoreSkel = new CalCoreSkeleton();
  m_pCoreModel->setCoreSkeleton(m_pCoreSkel);

  CHECK ( -1 == m_pCoreModel->getBoneId("none"));
  
  TEST_RES( getBoneId failed )
}

TEST( CalCoreModel_addBoneName )
{
  TEST_INIT( CalCoreModel::getBoneId );

  CalCoreModel *m_pCoreModel = new CalCoreModel("test-model");
  CalCoreSkeleton *m_pCoreSkel = new CalCoreSkeleton();
  m_pCoreModel->setCoreSkeleton(m_pCoreSkel);

  m_pCoreModel->addBoneName("myBone",1);
  
  TEST_RES( addBoneName failed )
}
