#ifndef TESTTYPEINFOHELPERTEST_H
#define TESTTYPEINFOHELPERTEST_H

#include <cppunit/extensions/HelperMacros.h>


class TestTypeInfoHelperTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( TestTypeInfoHelperTest );
  CPPUNIT_TEST( testGetClassName );
  CPPUNIT_TEST_SUITE_END();

public:
  TestTypeInfoHelperTest() = default;
  ~TestTypeInfoHelperTest() = default;

  TestTypeInfoHelperTest( const TestTypeInfoHelperTest &copy ) = delete;
  void operator =( const TestTypeInfoHelperTest &copy ) = delete;

  void testGetClassName();
};

#endif // TESTTYPEINFOHELPERTEST_H
