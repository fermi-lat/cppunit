#include <memory>
#include <string>
#include <cppunit/TestCaller.h>
#include <cppunit/TestResult.h>
#include "CoreSuite.h"
#include "FixturelessTest.h"


CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( FixturelessTest,
                                       coreSuiteName() );


namespace
{

template <typename Fixture>
void testFixtureless( const std::string& name )
{
  Fixture fixture;

  CPPUNIT_NS::TestCaller<Fixture>* testCaller =
  CPPUNIT_NS::detail::makeNewTestCaller( name,
                                         [] ( Fixture *fixture ) {
                                           fixture->test();
                                         },
                                         &fixture,
                                         false );
  CPPUNIT_NS::TestResult result;
  testCaller->run( &result );

  fixture.verify();
}

} // namespace


void FixturelessTest::testFixturelessClassic()
{
  testFixtureless<FixturelessTestComplete>( "testFixturelessClassic" );
}


#if CPPUNIT_HAS_CONCEPTS

void FixturelessTest::testFixturelessSetUpOnly()
{
  testFixtureless<FixturelessTestSetUpOnly>( "testFixturelessSetUpOnly" );
}


void FixturelessTest::testFixturelessTearDownOnly()
{
  testFixtureless<FixturelessTestTearDownOnly>( "testFixturelessTearDownOnly" );
}


void FixturelessTest::testFixturelessMinimal()
{
  testFixtureless<FixturelessTestMinimal>( "testFixturelessMinimal" );
}

#endif // CPPUNIT_HAS_CONCEPTS
