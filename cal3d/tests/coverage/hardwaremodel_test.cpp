/*******************************************************
* harwaremodel_test.cpp
*
* Unittest for testing hardwaremodel.cpp
*
********************************************************/

#include "UnitTest++.h"
#include "Unittest.h"

#include "cal3d/hardwaremodel.h"
#include "cal3d/corebone.h"
#include "cal3d/coremodel.h"
#include "cal3d/coremesh.h"
#include "cal3d/coresubmesh.h"
#include "cal3d/coreskeleton.h"
#include "cal3d/skeleton.h"
#include "cal3d/quaternion.h"

TEST( CalHardwareModel )
{
  TEST_INIT( CalHardwareModel::CalHardwareModel );

  CalCoreModel *m_pCoreModel = new CalCoreModel("test");
  CalHardwareModel *m_pHardModel = new CalHardwareModel(m_pCoreModel);
  CHECK( m_pHardModel != NULL );
  
  TEST_RES( object init. failed );  
}

TEST( CalHardwareModel_getRotationBoneSpace )
{
  TEST_INIT( CalHardwareModel::getRotationBoneSpace );

  CalCoreModel *m_pCoreModel = new CalCoreModel("model-test");

  CalCoreSkeleton *m_pCoreSkel = new CalCoreSkeleton();
  CalCoreBone *m_pCoreBone = new CalCoreBone("bone-test");
  m_pCoreSkel->addCoreBone(m_pCoreBone);

  CalSkeleton *m_pSkel = new CalSkeleton(m_pCoreSkel);

  CalCoreMesh *m_pCoreMesh = new CalCoreMesh();
  CalCoreSubmesh *m_pCoreSubmesh = new CalCoreSubmesh();

  m_pCoreSubmesh->reserve(1, 0, 1, 0);

  CalCoreSubmesh::Face face;
  face.vertexId[0] = 0;
  face.vertexId[1] = 0;
  face.vertexId[2] = 0;

  CalCoreSubmesh::Vertex vertex;
  vertex.position.x = 0.0;
  vertex.position.y = 0.0;
  vertex.position.z = 0.0;
  vertex.position.x = 1.0;
  vertex.position.y = 1.0;
  vertex.position.z = 1.0;
  vertex.collapseId = 0;
  vertex.faceCollapseCount = 0;

  m_pCoreSubmesh->setFace(0, face);  
  m_pCoreSubmesh->setVertex(0, vertex);

  m_pCoreMesh->addCoreSubmesh(m_pCoreSubmesh);  
  m_pCoreModel->addCoreMesh(m_pCoreMesh);

  CalHardwareModel *m_pHardModel = new CalHardwareModel(m_pCoreModel);

  char vertexBuf[32];
  char normalBuf[32];
  char weightBuf[32];
  char matrixIndBuf[32];
  CalIndex calIndex[32];

  m_pHardModel->setVertexBuffer( vertexBuf, sizeof(char) ); 
  m_pHardModel->setMatrixIndexBuffer( matrixIndBuf, sizeof(char) );  
  m_pHardModel->setNormalBuffer( normalBuf, sizeof(char) ); 
  m_pHardModel->setWeightBuffer( weightBuf, sizeof(char) ); 
  m_pHardModel->setIndexBuffer( calIndex );

  m_pHardModel->load(0,0,1);
  m_pHardModel->selectHardwareMesh(0);

  m_pHardModel->CalHardwareModel::getRotationBoneSpace(m_pCoreSkel->getCoreBoneId("test"), m_pSkel);
  
  TEST_RES( getRotationBoneSpace failed );  
}

TEST( CalHardwareModel_getTranslationBoneSpace )
{
  TEST_INIT( CalHardwareModel::getTranslationBoneSpace );

  CalCoreModel *m_pCoreModel = new CalCoreModel("model-test");

  CalCoreSkeleton *m_pCoreSkel = new CalCoreSkeleton();
  CalCoreBone *m_pCoreBone = new CalCoreBone("bone-test");
  m_pCoreSkel->addCoreBone(m_pCoreBone);

  CalSkeleton *m_pSkel = new CalSkeleton(m_pCoreSkel);

  CalCoreMesh *m_pCoreMesh = new CalCoreMesh();
  CalCoreSubmesh *m_pCoreSubmesh = new CalCoreSubmesh();

  m_pCoreSubmesh->reserve(1, 0, 1, 0);

  CalCoreSubmesh::Face face;
  face.vertexId[0] = 0;
  face.vertexId[1] = 0;
  face.vertexId[2] = 0;

  CalCoreSubmesh::Vertex vertex;
  vertex.position.x = 0.0;
  vertex.position.y = 0.0;
  vertex.position.z = 0.0;
  vertex.position.x = 1.0;
  vertex.position.y = 1.0;
  vertex.position.z = 1.0;
  vertex.collapseId = 0;
  vertex.faceCollapseCount = 0;

  m_pCoreSubmesh->setFace(0, face);  
  m_pCoreSubmesh->setVertex(0, vertex);

  m_pCoreMesh->addCoreSubmesh(m_pCoreSubmesh);  
  m_pCoreModel->addCoreMesh(m_pCoreMesh);

  CalHardwareModel *m_pHardModel = new CalHardwareModel(m_pCoreModel);

  char vertexBuf[32];
  char normalBuf[32];
  char weightBuf[32];
  char matrixIndBuf[32];
  CalIndex calIndex[32];

  m_pHardModel->setVertexBuffer( vertexBuf, sizeof(char) ); 
  m_pHardModel->setMatrixIndexBuffer( matrixIndBuf, sizeof(char) );  
  m_pHardModel->setNormalBuffer( normalBuf, sizeof(char) ); 
  m_pHardModel->setWeightBuffer( weightBuf, sizeof(char) ); 
  m_pHardModel->setIndexBuffer( calIndex );

  m_pHardModel->load(0,0,1);

  m_pHardModel->selectHardwareMesh(0);

  m_pHardModel->CalHardwareModel::getTranslationBoneSpace(m_pCoreSkel->getCoreBoneId("test"), m_pSkel);
  
  TEST_RES( getTranslationBoneSpace failed );  
}
