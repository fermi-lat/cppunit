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

class FixturelessTestSetupOnly
{
  CPPUNIT_TEST_SUITE( FixturelessTestSetupOnly );
  CPPUNIT_TEST( testFixtureAccess );
  CPPUNIT_TEST_SUITE_END();

public:
  FixturelessTestSetupOnly() = default;
  ~FixturelessTestSetupOnly() = default;

  FixturelessTestSetupOnly( const FixturelessTestSetupOnly &copy ) = delete;
  void operator =( const FixturelessTestSetupOnly &copy ) = delete;

  void setUp();

  void testFixtureAccess();

protected:
  std::string m_msg;
};


// TODO: FixturelessTestTeardownOnly


class FixturelessTestMinimal
{
  CPPUNIT_TEST_SUITE( FixturelessTestMinimal );
  CPPUNIT_TEST( testFixtureAccess );
  CPPUNIT_TEST_SUITE_END();

public:
  FixturelessTestMinimal() = default;
  ~FixturelessTestMinimal() = default;

  FixturelessTestMinimal( const FixturelessTestMinimal &copy ) = delete;
  void operator =( const FixturelessTestMinimal &copy ) = delete;

  void testFixtureAccess();

protected:
  std::string m_msg = "minimal_test";
};

#endif // CPPUNIT_HAS_CONCEPTS


#if CPPUNIT_HAS_CONCEPTS

static_assert( CPPUNIT_NS::core::test_fixture<FixturelessTest> );
static_assert( CPPUNIT_NS::core::test_fixture_setup<FixturelessTestSetupOnly>
  && !CPPUNIT_NS::core::test_fixture_teardown<FixturelessTestSetupOnly> );
static_assert( CPPUNIT_NS::core::test_fixture_less<FixturelessTestMinimal>);

#endif // CPPUNIT_HAS_CONCEPTS

#endif  // FIXTURELESS_H
