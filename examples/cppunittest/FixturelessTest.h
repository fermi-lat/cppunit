#ifndef FIXTURELESS_H
#define FIXTURELESS_H

#include <cppunit/extensions/HelperMacros.h>
#include <string>


class FixturelessTest
{
  CPPUNIT_TEST_SUITE( FixturelessTest );
  CPPUNIT_TEST( testFixtureAccess );
  CPPUNIT_TEST_SUITE_END();

public:
  FixturelessTest() = default;
  ~FixturelessTest() = default;

  FixturelessTest( const FixturelessTest &copy ) = delete;
  void operator =( const FixturelessTest &copy ) = delete;

  void setUp();
  void tearDown();

  void testFixtureAccess();

protected:
  std::string m_msg;
};


#if CPPUNIT_HAS_CONCEPTS

static_assert( CPPUNIT_NS::core::test_fixture<FixturelessTest> );

#endif // CPPUNIT_HAS_CONCEPTS

#endif  // FIXTURELESS_H
