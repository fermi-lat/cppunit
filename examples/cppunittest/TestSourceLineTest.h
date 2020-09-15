#ifndef TESTSOURCELINETEST_H
#define TESTSOURCELINETEST_H

#include <cppunit/extensions/HelperMacros.h>


class TestSourceLineTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( TestSourceLineTest );
  CPPUNIT_TEST( testSourceLine );
  CPPUNIT_TEST_SUITE_END();

public:
  TestSourceLineTest() = default;
  ~TestSourceLineTest() = default;

  TestSourceLineTest( const TestSourceLineTest &copy ) = delete;
  void operator =( const TestSourceLineTest &copy ) = delete;

  void testSourceLine();
};

#endif // TESTSOURCELINETEST_H
