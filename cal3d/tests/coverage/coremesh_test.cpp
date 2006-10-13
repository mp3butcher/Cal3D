/*******************************************************
* coremesh_test.cpp
*
* Unittest for testing coremesh.cpp
*
********************************************************/

#include "UnitTest++.h"
#include "Unittest.h"

#include "cal3d/coremesh.h"
#include "cal3d/coresubmesh.h"

TEST( CalCoreMesh_scale )
{
  TEST_INIT( CalCoreMesh::scale );

  CalCoreMesh *m_pCoreMesh = new CalCoreMesh();
  CalCoreSubmesh *m_pCoreSubmesh = new CalCoreSubmesh();
  m_pCoreMesh->addCoreSubmesh(m_pCoreSubmesh);  

  m_pCoreMesh->scale(0.5);

  TEST_RES( scale failed )
}
