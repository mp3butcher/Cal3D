/*******************************************************
* animation_action_test.cpp
*
* Unittest for testing animation_action.cpp
*
********************************************************/

#include "UnitTest++.h"
#include "Unittest.h"

#include "cal3d/animation_action.h"
#include "cal3d/coreanimation.h"

TEST( CalAnimationAction_creation )
{
  TEST_INIT( CalAnimationAction::CalAnimationAction );

  CalCoreAnimation   *m_pCoreAnim  = new CalCoreAnimation();
  CalAnimationAction *m_pAnimAction = new CalAnimationAction(m_pCoreAnim);
  CHECK( m_pAnimAction != NULL );
  
  TEST_RES( object init. failed );  
}

TEST( CalAnimationAction_execute )
{
  TEST_INIT( CalAnimationAction::execute );

  CalCoreAnimation   *m_pCoreAnim = new CalCoreAnimation();
  CalAnimationAction *m_pAnimAction = new CalAnimationAction(m_pCoreAnim);

  m_pAnimAction->execute(1.0, 1.0, 1.0, false);
  
  TEST_RES( execute error );
}

TEST( CalAnimationAction_update )
{
  TEST_INIT( CalAnimationAction::update );

  CalCoreAnimation   *m_pCoreAnim  = new CalCoreAnimation();
  CalAnimationAction *m_pAnimAction = new CalAnimationAction(m_pCoreAnim);

  m_pAnimAction->execute(0.0, 1.0, 1.0, false);
  m_pAnimAction->update(2.0);
  m_pAnimAction->update(-3.0);
  
  m_pAnimAction->execute(1.0, 1.0, 1.0, false);
  m_pAnimAction->update(0.0);  

  m_pAnimAction->execute(-1.0, 1.0, 1.0, true);
  m_pAnimAction->update(1.0);
  
  TEST_RES( update error );  
}
