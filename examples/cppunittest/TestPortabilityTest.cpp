#include "CoreSuite.h"
#include "TestPortabilityTest.h"
#include <cppunit/Portability.h>
#include <exception>


CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( TestPortabilityTest,
                                       coreSuiteName() );


class TestException
{
};


void
TestPortabilityTest::testDemangle()
{
  CPPUNIT_ASSERT_EQUAL( std::string( "int" ),
                        CPPUNIT_NS::demangle( typeid( int ).name() ) );

  const int i = 0;
  CPPUNIT_ASSERT_EQUAL( std::string( "int" ),
                        CPPUNIT_NS::demangle( typeid( i ).name() ) );

  CPPUNIT_ASSERT_EQUAL( std::string( "std::out_of_range" ),
                        CPPUNIT_NS::demangle( typeid( std::out_of_range ).name() ) );

  const auto stdException = std::out_of_range( "" );
  CPPUNIT_ASSERT_EQUAL( std::string( "std::out_of_range" ),
                        CPPUNIT_NS::demangle( typeid( stdException ).name() ) );

  CPPUNIT_ASSERT_EQUAL( std::string( "TestException" ),
                        CPPUNIT_NS::demangle( typeid( TestException ).name() ) );

  const TestException testException;
  CPPUNIT_ASSERT_EQUAL( std::string( "TestException" ),
                        CPPUNIT_NS::demangle( typeid( testException ).name() ) );
}
