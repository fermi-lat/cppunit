#include "CoreSuite.h"
#include "TestTypeInfoHelperTest.h"
#include <cppunit/extensions/TypeInfoHelper.h>
#include <exception>


CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( TestTypeInfoHelperTest,
                                       coreSuiteName() );


class TestException
{
};


void
TestTypeInfoHelperTest::testGetClassName()
{
  CPPUNIT_ASSERT_EQUAL( std::string( "int" ),
                        CPPUNIT_NS::TypeInfoHelper::getClassName( typeid(int) ) );

  const int i = 0;
  CPPUNIT_ASSERT_EQUAL( std::string( "int" ),
                        CPPUNIT_NS::TypeInfoHelper::getClassName( typeid(i) ) );

  CPPUNIT_ASSERT_EQUAL( std::string( "std::out_of_range" ),
                        CPPUNIT_NS::TypeInfoHelper::getClassName( typeid(std::out_of_range) ) );

  const auto stdException = std::out_of_range( "" );
  CPPUNIT_ASSERT_EQUAL( std::string( "std::out_of_range" ),
                        CPPUNIT_NS::TypeInfoHelper::getClassName( typeid(stdException) ) );

  CPPUNIT_ASSERT_EQUAL( std::string( "TestException" ),
                        CPPUNIT_NS::TypeInfoHelper::getClassName( typeid(TestException) ) );

  const TestException testException;
  CPPUNIT_ASSERT_EQUAL( std::string( "TestException" ),
                        CPPUNIT_NS::TypeInfoHelper::getClassName( typeid(testException) ) );
}
