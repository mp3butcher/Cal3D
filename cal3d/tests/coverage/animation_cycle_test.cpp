/*******************************************************
* animation_cycle_test.cpp
*
* Unittest for testing animation_cycle.cpp
*
********************************************************/

#include "UnitTest++.h"
#include "Unittest.h"

#include "cal3d/animation_cycle.h"
#include "cal3d/coreanimation.h"

TEST( CalAnimationCycle )
{
  TEST_INIT( CalAnimationCycle::CalAnimationCycle );

  CalCoreAnimation   *m_pCoreAnim  = new CalCoreAnimation();
  CalAnimationCycle  *m_pAnimCycle = new CalAnimationCycle(m_pCoreAnim);
  CHECK( m_pAnimCycle != NULL );
  
  TEST_RES( object init. failed );  
}

TEST( CalAnimationCycle_blend )
{
  TEST_INIT( CalAnimationCycle::blend );

  CalCoreAnimation   *m_pCoreAnim  = new CalCoreAnimation();
  CalAnimationCycle  *m_pAnimCycle = new CalAnimationCycle(m_pCoreAnim);

  m_pAnimCycle->blend(1.0, 1.0);
  
  TEST_RES( blend error );
}

TEST( CalAnimationCycle_setAsync )
{
  TEST_INIT( CalAnimationAction::setAsync );

  CalCoreAnimation   *m_pCoreAnim  = new CalCoreAnimation();

  CalAnimationCycle  *m_pAnimCycle = new CalAnimationCycle(m_pCoreAnim);
  m_pAnimCycle->setAsync(1.0, 1.0);

  m_pAnimCycle = new CalAnimationCycle(m_pCoreAnim);
  m_pAnimCycle->setAsync(0.0, 0.0);
  
  TEST_RES( setAsync error );
}

TEST( CalAnimationCycle_update )
{
  TEST_INIT( CalAnimationAction::update );

  CalCoreAnimation   *m_pCoreAnim  = new CalCoreAnimation();
  CalAnimationCycle  *m_pAnimCycle = new CalAnimationCycle(m_pCoreAnim);

  m_pAnimCycle->update(1.0);
  m_pAnimCycle->update(-1.0);

  m_pAnimCycle = new CalAnimationCycle(m_pCoreAnim);
  m_pAnimCycle->blend(5.0, 2.0);
  m_pAnimCycle->setAsync(0.0, 0.0);
  m_pAnimCycle->update(-1.0);
  m_pAnimCycle->update(3.0);
  
  TEST_RES( update error );
}

