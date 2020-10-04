#ifndef FIXTURELESS_H
#define FIXTURELESS_H

#include <cppunit/extensions/HelperMacros.h>


class FixturelessTestComplete
{
  CPPUNIT_TEST_SUITE( FixturelessTestComplete );
  CPPUNIT_TEST( test );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp()
  {
    m_setUpCalled = true;
  }

  void tearDown()
  {
    m_tearDownCalled = true;
  }

  void test()
  {
    m_testCaseCalled = true;
  }

  void verify() const
  {
    CPPUNIT_ASSERT( m_setUpCalled );
    CPPUNIT_ASSERT( m_testCaseCalled );
    CPPUNIT_ASSERT( m_tearDownCalled );
  }

private:
  bool m_setUpCalled = false;
  bool m_tearDownCalled = false;
  bool m_testCaseCalled = false;
};


#if CPPUNIT_HAS_CONCEPTS

class FixturelessTestSetUpOnly
{
  CPPUNIT_TEST_SUITE( FixturelessTestSetUpOnly );
  CPPUNIT_TEST( test );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp()
  {
    m_setUpCalled = true;
  }

  void test()
  {
    m_testCaseCalled = true;
  }

  void verify() const
  {
    CPPUNIT_ASSERT( m_setUpCalled );
    CPPUNIT_ASSERT( m_testCaseCalled );
  }

protected:
  bool m_setUpCalled = false;
  bool m_testCaseCalled = false;
};


class FixturelessTestTearDownOnly
{
  CPPUNIT_TEST_SUITE( FixturelessTestTearDownOnly );
  CPPUNIT_TEST( test );
  CPPUNIT_TEST_SUITE_END();

public:
  void tearDown()
  {
    m_tearDownCalled = true;
  }

  void test()
  {
    m_testCaseCalled = true;
  }

  void verify() const
  {
    CPPUNIT_ASSERT( m_testCaseCalled );
    CPPUNIT_ASSERT( m_tearDownCalled );
  }

protected:
  bool m_tearDownCalled = false;
  bool m_testCaseCalled = false;
};


class FixturelessTestMinimal
{
  CPPUNIT_TEST_SUITE( FixturelessTestMinimal );
  CPPUNIT_TEST( test );
  CPPUNIT_TEST_SUITE_END();

public:
  void test()
  {
    m_testCaseCalled = true;
  }

  void verify() const
  {
    CPPUNIT_ASSERT( m_testCaseCalled );
  }

protected:
  bool m_testCaseCalled = false;
};

#endif // CPPUNIT_HAS_CONCEPTS


#if CPPUNIT_HAS_CONCEPTS

static_assert( CPPUNIT_NS::detail::test_fixture<FixturelessTestComplete> );
static_assert( CPPUNIT_NS::detail::test_fixture_has_setup<FixturelessTestSetUpOnly>
  && !CPPUNIT_NS::detail::test_fixture_has_teardown<FixturelessTestSetUpOnly> );
static_assert( !CPPUNIT_NS::detail::test_fixture_has_setup<FixturelessTestTearDownOnly>
  && CPPUNIT_NS::detail::test_fixture_has_teardown<FixturelessTestTearDownOnly> );
static_assert( !CPPUNIT_NS::detail::test_fixture<FixturelessTestMinimal>
  && !CPPUNIT_NS::detail::test_fixture_has_setup<FixturelessTestMinimal>
  && !CPPUNIT_NS::detail::test_fixture_has_teardown<FixturelessTestMinimal> );

#endif // CPPUNIT_HAS_CONCEPTS


class FixturelessTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( FixturelessTest );
  CPPUNIT_TEST( testFixturelessClassic );
#if CPPUNIT_HAS_CONCEPTS
  CPPUNIT_TEST( testFixturelessSetUpOnly );
  CPPUNIT_TEST( testFixturelessTearDownOnly );
  CPPUNIT_TEST( testFixturelessMinimal );
#endif // CPPUNIT_HAS_CONCEPTS
  CPPUNIT_TEST_SUITE_END();

public:
  FixturelessTest() = default;
  ~FixturelessTest() = default;

  FixturelessTest( const FixturelessTest &copy ) = delete;
  void operator =( const FixturelessTest &copy ) = delete;

  void testFixturelessClassic();
#if CPPUNIT_HAS_CONCEPTS
  void testFixturelessSetUpOnly();
  void testFixturelessTearDownOnly();
  void testFixturelessMinimal();
#endif // CPPUNIT_HAS_CONCEPTS
};

#endif  // FIXTURELESS_H
