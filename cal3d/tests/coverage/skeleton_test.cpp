/*******************************************************
* skeleton_test.cpp
*
* Unittest for testing skeleton.cpp
*
********************************************************/

#include "UnitTest++.h"
#include "Unittest.h"

#include "cal3d/coreskeleton.h"
#include "cal3d/corebone.h"
#include "cal3d/skeleton.h"

TEST( CalSkeleton_getBoneBoundingBox )
{
  TEST_INIT( CalSkeleton::getBoneBoundingBox );

  float min=0.0, max=1.0;

  CalCoreSkeleton *m_pCoreSkeleton = new CalCoreSkeleton();
  CalCoreBone     *m_pCoreBoneRoot  = new CalCoreBone("testBone_root");
  CalCoreBone     *m_pCoreBoneChild = new CalCoreBone("testBone_child");
  m_pCoreSkeleton->addCoreBone(m_pCoreBoneRoot);
  m_pCoreSkeleton->addCoreBone(m_pCoreBoneChild);

  CalSkeleton *m_pSkeleton = new CalSkeleton(m_pCoreSkeleton);
  
  m_pSkeleton->getBoneBoundingBox(&min, &max);

  TEST_RES( getBoneBoundingBox failed );  
}

TEST( CalSkeleton_getBonePoints )
{
  TEST_INIT( CalSkeleton::getBonePoints );

  float pPoints = 1.0;

  CalCoreSkeleton *m_pCoreSkeleton = new CalCoreSkeleton();
  CalCoreBone     *m_pCoreBone     = new CalCoreBone("testBone");
  m_pCoreSkeleton->addCoreBone(m_pCoreBone);

  CalSkeleton *m_pSkeleton = new CalSkeleton(m_pCoreSkeleton);
  m_pSkeleton->getBonePoints(&pPoints);

  TEST_RES( getBonePoints failed );  
}

TEST( CalSkeleton_getBonePointsStatic )
{

  TEST_INIT( CalSkeleton::getBonePointsStatic );

  float pPoints = 1.0;

  CalCoreSkeleton *m_pCoreSkeleton = new CalCoreSkeleton();
  CalCoreBone     *m_pCoreBone     = new CalCoreBone("testBone");
  m_pCoreSkeleton->addCoreBone(m_pCoreBone);

  CalSkeleton *m_pSkeleton = new CalSkeleton(m_pCoreSkeleton);
  m_pSkeleton->getBonePointsStatic(&pPoints);

  TEST_RES( getBonePointsStatic failed );  
}

TEST( CalSkeleton_getBoneLines )
{
  TEST_INIT( CalSkeleton::getBoneLines );

  float pLines = 1.0;

  CalCoreSkeleton *m_pCoreSkeleton = new CalCoreSkeleton();
  CalCoreBone     *m_pCoreBoneRoot  = new CalCoreBone("testBone_root");
  CalCoreBone     *m_pCoreBoneChild = new CalCoreBone("testBone_child");

  m_pCoreSkeleton->addCoreBone(m_pCoreBoneRoot);
  m_pCoreBoneChild->setParentId(m_pCoreSkeleton->getCoreBoneId("testBone_root"));
  m_pCoreSkeleton->addCoreBone(m_pCoreBoneChild);

  CalSkeleton *m_pSkeleton = new CalSkeleton(m_pCoreSkeleton);
  m_pSkeleton->getBoneLines(&pLines);

  TEST_RES( getBoneLines failed );  
}

TEST( CalSkeleton_getBoneLinesStatic )
{
  TEST_INIT( CalSkeleton::getBoneLinesStatic );

  float pLines = 1.0;

  CalCoreSkeleton *m_pCoreSkeleton = new CalCoreSkeleton();
  CalCoreBone     *m_pCoreBoneRoot  = new CalCoreBone("testBone_root");
  CalCoreBone     *m_pCoreBoneChild = new CalCoreBone("testBone_child");

  m_pCoreSkeleton->addCoreBone(m_pCoreBoneRoot);
  m_pCoreBoneChild->setParentId(m_pCoreSkeleton->getCoreBoneId("testBone_root"));
  m_pCoreSkeleton->addCoreBone(m_pCoreBoneChild);

  CalSkeleton *m_pSkeleton = new CalSkeleton(m_pCoreSkeleton);
  m_pSkeleton->getBoneLinesStatic(&pLines);

  TEST_RES( getBoneLinesStatic failed );  
}
