/*******************************************************
* model_test.cpp
*
* Unittest for testing model.cpp
*
********************************************************/

#include "UnitTest++.h"
#include "Unittest.h"

#include "cal3d/coremodel.h"
#include "cal3d/corebone.h"
#include "cal3d/model.h"
#include "cal3d/vector.h"
#include "cal3d/skeleton.h"

TEST( CalModel_getBoundingBox )
{

  TEST_INIT( CalModel::getBoundingBox );

  CalCoreModel *m_pCoreModel = new CalCoreModel("coreModel");
  CalCoreSkeleton *m_pCoreSkel = new CalCoreSkeleton();
  CalCoreBone *m_pCoreBone = new CalCoreBone("bone1");

  m_pCoreBone->setCoreSkeleton(m_pCoreSkel);
  m_pCoreSkel->addCoreBone(m_pCoreBone);
  m_pCoreModel->setCoreSkeleton(m_pCoreSkel);
 
  m_pCoreBone->calculateBoundingBox(m_pCoreModel);
  
  CalModel *m_pModel = new CalModel(m_pCoreModel);

  m_pModel->getBoundingBox(true);
  m_pModel->getBoundingBox(false);

  TEST_RES( getBoundingBox failed );  
}
