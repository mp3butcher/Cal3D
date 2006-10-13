/*******************************************************
* buffersource_test.cpp
*
* Unittest for testing buffersource.cpp
*
********************************************************/

#include "UnitTest++.h"
#include "Unittest.h"

#include "cal3d/buffersource.h"

TEST( CalBufferSource )
{
  TEST_INIT( CalBufferSource::CalBufferSource );
   
  CalBufferSource *m_pBufferSrc  = new CalBufferSource(NULL);
  CHECK( m_pBufferSrc != NULL );

  TEST_RES( object init. failed ); 

}
