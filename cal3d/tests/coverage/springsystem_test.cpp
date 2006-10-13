/*******************************************************
* springsystem_test.cpp
*
* Unittest for testing springsystem.cpp
*
********************************************************/

#include "UnitTest++.h"
#include "Unittest.h"

#include "cal3d/springsystem.h"
#include "cal3d/vector.h"
#include "cal3d/coremodel.h"
#include "cal3d/coreskeleton.h"
#include "cal3d/coreanimation.h"
#include "cal3d/coresubmesh.h"
#include "cal3d/corebone.h"
#include "cal3d/model.h"
#include "cal3d/submesh.h"

TEST( CalSpringSystem_calculateVertices )
{

  TEST_INIT( CalSpringSystem::calculateVertices );

  CalCoreSubmesh *m_pCoreSubmesh = new CalCoreSubmesh();

  CalCoreSubmesh::Vertex vertex;
  vertex.position.x = 0.0;
  vertex.position.y = 0.0;
  vertex.position.z = 0.0;
  vertex.position.x = 1.0;
  vertex.position.y = 1.0;
  vertex.position.z = 1.0;
  vertex.collapseId = 0;
  vertex.faceCollapseCount = 0;

  CalCoreSubmesh::Spring spring;
  spring.vertexId[0] = 1;
  spring.vertexId[1] = 1;
  spring.springCoefficient = 0.5;
  spring.idleLength = 0.0;

  m_pCoreSubmesh->reserve(2,1,1,1);
  m_pCoreSubmesh->setVertex( 0, vertex );
  m_pCoreSubmesh->setVertex( 1, vertex );
  m_pCoreSubmesh->setSpring(0, spring);

  CalCoreSubmesh::PhysicalProperty phyProp;
  phyProp.weight = 1;

  m_pCoreSubmesh->setPhysicalProperty(0, phyProp );

  CalSubmesh *m_pSubmesh = new CalSubmesh( m_pCoreSubmesh );

  CalCoreModel *m_pCoreModel = new CalCoreModel("test-model");
  CalCoreSkeleton *m_pCoreSkel = new CalCoreSkeleton();
  CalCoreAnimation *m_pCoreAnim = new CalCoreAnimation();
  CalCoreBone     *m_pCoreBone     = new CalCoreBone("testBone");
  m_pCoreSkel->addCoreBone(m_pCoreBone);
  m_pCoreModel->setCoreSkeleton(m_pCoreSkel);
  m_pCoreModel->addCoreAnimation(m_pCoreAnim);
  CalModel *m_pModel = new CalModel(m_pCoreModel);

  CalSpringSystem *m_pSpringsys = new CalSpringSystem(m_pModel);
  m_pSpringsys->setCollisionDetection(true);

  m_pSpringsys->calculateVertices(m_pSubmesh, 0.0);
  
  TEST_RES( CalSpringSystem failed );  
}
