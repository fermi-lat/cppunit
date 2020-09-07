#include "CoreSuite.h"
#include "FixturelessTest.h"


CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( FixturelessTest,
                                       coreSuiteName() );


void
FixturelessTest::setUp()
{
  m_msg = "test";
}


void
FixturelessTest::tearDown()
{
}


void
FixturelessTest::testFixtureAccess()
{
  CPPUNIT_ASSERT_EQUAL( std::string("test"), m_msg );
}
