/*******************************************************
* mixer_test.cpp
*
* Unittest for testing mixer.cpp
*
********************************************************/

#include "UnitTest++.h"
#include "Unittest.h"

#include "cal3d/mixer.h"
#include "cal3d/coremodel.h"
#include "cal3d/model.h"
#include "cal3d/coreskeleton.h"
#include "cal3d/coreanimation.h"
#include "cal3d/coretrack.h"
#include "cal3d/corebone.h"
#include "cal3d/corekeyframe.h"

TEST( CalMixer_construction )
{
  TEST_INIT( CalMixer::CalMixer );

  CalCoreModel *m_pCoreModel = new CalCoreModel("test-model");
  CalCoreSkeleton *m_pCoreSkel = new CalCoreSkeleton();
  m_pCoreModel->setCoreSkeleton(m_pCoreSkel);
  CalModel *m_pModel = new CalModel(m_pCoreModel);
 
  CalMixer *m_pMixer = new CalMixer(m_pModel);

  CHECK( NULL != m_pMixer);

  TEST_RES( object init. failed );  
}

TEST( CalMixer_blendCycle )
{
  TEST_INIT( CalMixer::blendCycle );

  // CalMixer::blendCycle(int id, float weight, float delay)
  CalCoreModel *m_pCoreModel = new CalCoreModel("test-model");
  CalCoreSkeleton *m_pCoreSkel = new CalCoreSkeleton();
  m_pCoreModel->setCoreSkeleton(m_pCoreSkel);
  CalModel *m_pModel = new CalModel(m_pCoreModel);
 
  CalMixer *m_pMixer = new CalMixer(m_pModel);

  // no animation
  m_pMixer->blendCycle(0 , 0.0, 0.0);  

  //--------------------------------
  m_pCoreModel = new CalCoreModel("test-model");
  CalCoreAnimation *m_pCoreAnim = new CalCoreAnimation();
  m_pCoreModel->addCoreAnimation(m_pCoreAnim);
  m_pCoreModel->setCoreSkeleton(m_pCoreSkel);
  m_pModel = new CalModel(m_pCoreModel);
  m_pMixer = new CalMixer(m_pModel);

  CHECK (m_pMixer->blendCycle(0 , 1.0, 1.0));  
  CHECK (m_pMixer->blendCycle(0 , 1.0, 1.0));

  TEST_RES( blendCycle error );
}

TEST( CalMixer_executeAction )
{
  TEST_INIT( CalMixer::executeAction );

  CalCoreModel *m_pCoreModel = new CalCoreModel("test-model");
  CalCoreSkeleton *m_pCoreSkel = new CalCoreSkeleton();
  CalCoreAnimation *m_pCoreAnim = new CalCoreAnimation();
  m_pCoreModel->setCoreSkeleton(m_pCoreSkel);
  m_pCoreModel->addCoreAnimation(m_pCoreAnim);
  CalModel *m_pModel = new CalModel(m_pCoreModel);
 
  CalMixer *m_pMixer = new CalMixer(m_pModel);

  CHECK(m_pMixer->executeAction(0, 1.0, 1.0, 1.0, false));

  TEST_RES( executeAction error );
}

TEST( CalMixer_removeAction )
{
  TEST_INIT( CalMixer::removeAction );

  CalCoreModel *m_pCoreModel = new CalCoreModel("test-model");
  CalCoreSkeleton *m_pCoreSkel = new CalCoreSkeleton();
  CalCoreAnimation *m_pCoreAnim = new CalCoreAnimation();
  m_pCoreModel->setCoreSkeleton(m_pCoreSkel);
  m_pCoreModel->addCoreAnimation(m_pCoreAnim);
  CalModel *m_pModel = new CalModel(m_pCoreModel);
 
  CalMixer *m_pMixer = new CalMixer(m_pModel);
  
  m_pMixer->executeAction(0, 1.0, 1.0, 1.0, false);
  CHECK (m_pMixer->removeAction(0));

  TEST_RES( removeAction error );
}

TEST( CalMixer_updateAnimation )
{
  TEST_INIT( CalMixer::updateAnimation );

  CalCoreModel *m_pCoreModel = new CalCoreModel("test-model");
  CalCoreSkeleton *m_pCoreSkel = new CalCoreSkeleton();
  CalCoreAnimation *m_pCoreAnim = new CalCoreAnimation();

  m_pCoreModel->setCoreSkeleton(m_pCoreSkel);
  m_pCoreModel->addCoreAnimation(m_pCoreAnim);
  CalModel *m_pModel = new CalModel(m_pCoreModel);
 
  CalMixer *m_pMixer = new CalMixer(m_pModel);
  
  m_pMixer->blendCycle(0 , 1.0, 1.0);
  m_pMixer->executeAction(0, 1.0, 1.0, 1.0, false);
  m_pMixer->updateAnimation(0.0);
  m_pMixer->updateAnimation(1.0);
  m_pMixer->clearCycle(0 , 1.0);
  m_pMixer->updateAnimation(1.0);

  TEST_RES( updateAnimation error );
}

TEST( CalMixer_updateSkeleton )
{
  TEST_INIT( CalMixer::updateSkeleton );

  CalCoreModel *m_pCoreModel = new CalCoreModel("test-model");
  CalCoreSkeleton *m_pCoreSkel = new CalCoreSkeleton();
  CalCoreAnimation *m_pCoreAnim = new CalCoreAnimation();
  m_pCoreAnim->setDuration(20.0);
  CalCoreBone *m_pCoreBone = new CalCoreBone("test-bone");

  m_pCoreSkel->addCoreBone(m_pCoreBone);

  CalCoreTrack *m_pCoreTrack = new CalCoreTrack();
  CalCoreKeyframe *m_pCoreKeyframe = new CalCoreKeyframe();

  m_pCoreTrack->addCoreKeyframe(m_pCoreKeyframe);
  m_pCoreTrack->setCoreBoneId(0);

  m_pCoreAnim->addCoreTrack(m_pCoreTrack);
  m_pCoreModel->setCoreSkeleton(m_pCoreSkel);
  m_pCoreModel->addCoreAnimation(m_pCoreAnim);

  CalModel *m_pModel = new CalModel(m_pCoreModel);
  CalMixer *m_pMixer = new CalMixer(m_pModel);

  m_pMixer->blendCycle(0 , 2.0, 5.0);

  m_pMixer->executeAction(0, 1.0, 1.0, 1.0, false);
  m_pMixer->updateAnimation(1.0);
  m_pMixer->updateSkeleton();
  m_pMixer->clearCycle(0, 0.5);
  m_pMixer->updateSkeleton();

  TEST_RES( updateSkeleton error );
}
