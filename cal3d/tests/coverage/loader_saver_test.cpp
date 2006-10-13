/*******************************************************
* saver_test.cpp
*
* Unittest for testing saver.cpp
*
********************************************************/

#include "UnitTest++.h"
#include "Unittest.h"

#include "cal3d/saver.h"
#include "cal3d/loader.h"

#include "cal3d/corekeyframe.h"
#include "cal3d/coreanimation.h"
#include "cal3d/coretrack.h"
#include "cal3d/coreskeleton.h"
#include "cal3d/corebone.h"

TEST( CalSaver_CalLoader_CoreAnimation )
{

  TEST_INIT( CalSaver_CalLoader::CoreAnimation );

  // TESTING Saving/Loading CoreAnimation without compression 

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

  CalLoader::setLoadingMode(LOADER_ROTATE_X_AXIS);
 
  CHECK( CalSaver::saveCoreAnimation( "tmp.cov", m_pCoreAnim, NULL ));
  CHECK( NULL != CalLoader::loadCoreAnimation( "tmp.cov" , m_pCoreSkel));

  // TESTING Saving/Loading CoreAnimation with compression

  CalSaverAnimationOptions *m_pSaverAnimOpt = new CalSaverAnimationOptions();
  m_pSaverAnimOpt->bCompressKeyframes = true;

  CHECK( CalSaver::saveCoreAnimation( "tmp.cov", m_pCoreAnim, m_pSaverAnimOpt));
  CHECK( NULL != CalLoader::loadCoreAnimation( "tmp.cov" , m_pCoreSkel));

  // TESTING Saving/Loading in XML file
  CHECK( CalSaver::saveCoreAnimation( "tmp.xaf", m_pCoreAnim, NULL ));
  CHECK( NULL != CalLoader::loadCoreAnimation( "tmp.xaf" , m_pCoreSkel));

  TEST_RES( CoreAnimation save/load failed );  
}

TEST( CalSaverCalLoader_CoreSkeleton )
{

  TEST_INIT( CalSaver_CalLoader::CoreSkeleton );

  // TESTING Saving/Loading CoreSketelon

  CalCoreSkeleton *m_pCoreSkel = new CalCoreSkeleton();
  CalCoreBone *m_pCoreBoneRoot = new CalCoreBone("bone_root");
  CalCoreBone *m_pCoreBoneChild1 = new CalCoreBone("bone_child1");
 
  m_pCoreSkel->addCoreBone(m_pCoreBoneRoot);
  m_pCoreSkel->addCoreBone(m_pCoreBoneChild1);

  m_pCoreBoneRoot->addChildId(m_pCoreSkel->getCoreBoneId("bone_child1"));
  
  CalLoader::setLoadingMode(LOADER_ROTATE_X_AXIS);

  CHECK( CalSaver::saveCoreSkeleton( "tmp.cov", m_pCoreSkel ));
  CHECK( NULL != CalLoader::loadCoreSkeleton( "tmp.cov"));

  // TESTING Saving/Loading in XML file
  CHECK( CalSaver::saveCoreSkeleton( "tmp.xsf", m_pCoreSkel ));
  CHECK( NULL != CalLoader::loadCoreSkeleton( "tmp.xsf" ));

  TEST_RES( CoreSkeleton save/load failed );  
}

TEST( CalSaverCalLoader_CoreMaterial )
{

  TEST_INIT( CalSaver_CalLoader::CoreMaterial );

  // TESTING Saving/Loading CoreSketelon

  CalCoreMaterial *m_pCoreMat = new CalCoreMaterial();

  CalLoader::setLoadingMode(LOADER_ROTATE_X_AXIS);

  CHECK( CalSaver::saveCoreMaterial( "tmp.cov", m_pCoreMat ));
  CHECK( NULL != CalLoader::loadCoreMaterial( "tmp.cov"));

  // TESTING Saving/Loading in XML file
  CHECK( CalSaver::saveCoreMaterial( "tmp.xrf", m_pCoreMat ));
  CHECK( NULL != CalLoader::loadCoreMaterial( "tmp.xrf" ));

  TEST_RES( CoreMaterial save/load failed );  
}

TEST( CalSaverCalLoader_CoreMesh )
{

  TEST_INIT( CalSaver_CalLoader::CoreMesh );

  // TESTING Saving/Loading CoreSketelon

  CalCoreMesh *m_pCoreMesh = new CalCoreMesh();

  CalLoader::setLoadingMode(LOADER_ROTATE_X_AXIS);

  CHECK( CalSaver::saveCoreMesh( "tmp.cov", m_pCoreMesh ));
  CHECK( NULL != CalLoader::loadCoreMesh( "tmp.cov"));

  // TESTING Saving/Loading in XML file
  CHECK( CalSaver::saveCoreMesh( "tmp.xmf", m_pCoreMesh ));
  CHECK( NULL != CalLoader::loadCoreMesh( "tmp.xmf" ));

  TEST_RES( CoreMesh save/load failed );  
}
