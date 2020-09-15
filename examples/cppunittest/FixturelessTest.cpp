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


#if CPPUNIT_HAS_CONCEPTS


CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( FixturelessTestSetupOnly,
                                       coreSuiteName() );


void
FixturelessTestSetupOnly::setUp()
{
  m_msg = "setup_only_test";
}


void
FixturelessTestSetupOnly::testFixtureAccess()
{
  CPPUNIT_ASSERT_EQUAL( std::string("setup_only_test"), m_msg );
}


CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( FixturelessTestMinimal,
                                       coreSuiteName() );


void
FixturelessTestMinimal::testFixtureAccess()
{
  CPPUNIT_ASSERT_EQUAL( std::string("minimal_test"), m_msg );
}

#endif // CPPUNIT_HAS_CONCEPTS
