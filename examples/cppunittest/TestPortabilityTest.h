#ifndef TESTPORTABILITYTEST_H
#define TESTPORTABILITYTEST_H

#include <cppunit/extensions/HelperMacros.h>


class TestPortabilityTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( TestPortabilityTest );
  CPPUNIT_TEST( testDemangle );
  CPPUNIT_TEST_SUITE_END();

public:
  TestPortabilityTest() = default;
  ~TestPortabilityTest() = default;

  TestPortabilityTest( const TestPortabilityTest &copy ) = delete;
  void operator =( const TestPortabilityTest &copy ) = delete;

  void testDemangle();
};

#endif // TESTPORTABILITYTEST_H
