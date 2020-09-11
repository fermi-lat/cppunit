#ifndef CPPUNIT_TESTCALLER_H    // -*- C++ -*-
#define CPPUNIT_TESTCALLER_H

#include <cppunit/Exception.h>
#include <cppunit/TestCase.h>

#if CPPUNIT_HAS_CONCEPTS
#include <concepts>
#endif // CPPUNIT_HAS_CONCEPTS
#include <functional>


#if defined(CPPUNIT_USE_TYPEINFO_NAME)
#  include <cppunit/extensions/TypeInfoHelper.h>
#endif


CPPUNIT_NS_BEGIN


#if CPPUNIT_HAS_CONCEPTS

namespace core
{

// Callable should take fixture pointer as a first argument.
// If it's a class method, it's going to be this pointer.
// If it's a free-standing function, it's going to be first argument.
template <typename Callable, typename Fixture>
concept test_callable = std::invocable<Callable, Fixture*>;

} // namespace core

#endif // CPPUNIT_HAS_CONCEPTS


/*! \brief Generate a test case from a fixture method.
 * \ingroup WritingTestFixture
 *
 * A test caller provides access to a test case method 
 * on a test fixture class.  Test callers are useful when 
 * you want to run an individual test or add it to a 
 * suite.
 * Test Callers invoke only one Test (i.e. test method) on one 
 * Fixture of a TestFixture.
 * 
 * Here is an example:
 * \code
 * class MathTest : public CppUnit::TestFixture {
 *         ...
 *     public:
 *         void         setUp();
 *         void         tearDown();
 *
 *         void         testAdd();
 *         void         testSubtract();
 * };
 *
 * CppUnit::Test *MathTest::suite() {
 *     CppUnit::TestSuite *suite = new CppUnit::TestSuite;
 *
 *     suite->addTest( new CppUnit::TestCaller<MathTest>( "testAdd", testAdd ) );
 *     return suite;
 * }
 * \endcode
 *
 * You can use a TestCaller to bind any test method on a TestFixture
 * class, as long as it accepts void and returns void.
 * 
 * \see TestCase
 */

template <class Fixture>
#if CPPUNIT_HAS_CONCEPTS
  // Should be possible to create fixture by calling default constructor
  requires ( std::constructible_from<Fixture> )
#endif // CPPUNIT_HAS_CONCEPTS
class TestCaller : public TestCase
{ 
  typedef void (Fixture::*TestMethod)();
    
public:
  /*!
   * Constructor for TestCaller. This constructor builds a new Fixture
   * instance owned by the TestCaller.
   * \param name name of this TestCaller
   * \param test the method this TestCaller calls in runTest()
   */
  TestCaller( std::string name, TestMethod test ) :
      TestCaller( name,
                  test,
                  new Fixture() )
  {
  }

  /*!
   * Constructor for TestCaller. 
   * This constructor does not create a new Fixture instance but accepts
   * an existing one as parameter. The TestCaller will not own the
   * Fixture object.
   * \param name name of this TestCaller
   * \param test the method this TestCaller calls in runTest()
   * \param fixture the Fixture to invoke the test method on.
   */
  TestCaller(std::string name, TestMethod test, Fixture& fixture) :
      TestCaller( name,
                  [this] { m_fixture->setUp(); },
                  std::bind(test, &fixture),
                  [this] { m_fixture->tearDown(); },
                  &fixture,
                  false )
  {
  }
    
  /*!
   * Constructor for TestCaller. 
   * This constructor does not create a new Fixture instance but accepts
   * an existing one as parameter. The TestCaller will own the
   * Fixture object and delete it in its destructor.
   * \param name name of this TestCaller
   * \param test the method this TestCaller calls in runTest()
   * \param fixture the Fixture to invoke the test method on.
   */
  TestCaller(std::string name, TestMethod test, Fixture* fixture) :
	    TestCaller( name,
                  std::bind( test, fixture ),
                  fixture )
  {
  }

  TestCaller(std::string name, std::function<void()> test_function, Fixture* fixture):
      TestCaller( name,
                  [this] { m_fixture->setUp(); },
                  test_function,
                  [this] { m_fixture->tearDown(); },
                  fixture )
  {
  }

  TestCaller( std::string name,
              std::function<void()> setup,
              std::function<void()> test,
              std::function<void()> tearDown,
              Fixture* fixture,
              bool ownFixture = true ) :
    TestCase( name ),
    m_ownFixture( ownFixture ),
    m_fixture( fixture ),
    m_setupFunction( setup ),
    m_testFunction( test ),
    m_tearDownFunction( tearDown )
  {
  }

  ~TestCaller() 
  {
    if (m_ownFixture)
      delete m_fixture;
  }

  void runTest()
  { 
      m_testFunction();
  }  

  void setUp()
  { 
    m_setupFunction();
  }

  void tearDown()
  { 
    m_tearDownFunction();
  }

  std::string toString() const
  { 
  	return "TestCaller " + getName(); 
  }

private: 
  TestCaller( const TestCaller &other ); 
  TestCaller &operator =( const TestCaller &other );

private:
  bool m_ownFixture;
  Fixture *m_fixture;
  std::function<void()> m_setupFunction;
  std::function<void()> m_testFunction;
  std::function<void()> m_tearDownFunction;
};


template <typename Fixture, typename Callable>
#if CPPUNIT_HAS_CONCEPTS
  // Make sure that typename fits expected TestFixture interface.
  // This doesn't do much except for the fact that in C++11 template
  // is going to be instantiated, compiled and in case of mismatched
  // type compiler is going to show error somewhere in instantiated
  // code, but in C++20 compiler will check constraints before compiling
  // template and will show error if constraints aren't satisfied.
  //
  // Actually, C++11 error message is shorter and cleaner than C++20 one.
  // It just says that class does not contain setUp()/tearDown()
  // while C++20 message is more verbose and includes whole stack of
  // calls with macros expansion.
  requires ( core::test_fixture<Fixture>
  // Callable should be invocable with pointer to Fixture as a 1st argument
  && core::test_callable<Callable, Fixture> )
#endif // CPPUNIT_HAS_CONCEPTS
TestCaller<Fixture>* makeNewTestCaller( const std::string& name,
                                        Callable test,
                                        Fixture* fixture )
{
  return new TestCaller<Fixture>( name,
                                  std::bind( test, fixture ),
                                  fixture );
}


#if CPPUNIT_HAS_CONCEPTS

template <typename Fixture, typename Callable>
  // Overload for classes that don't match full fixture requirements
  requires ( !core::test_fixture<Fixture>
  && core::test_callable<Callable, Fixture> )
TestCaller<Fixture>* makeNewTestCaller( const std::string& name,
                                        Callable test,
                                        Fixture* fixture )
{
  std::function<void()> setup = [] {}; // No-op setup
  if constexpr ( core::test_fixture_has_setup<Fixture> )
  {
    setup = [fixture] { fixture->setUp(); };
  }

  std::function<void()> tearDown = [] {}; // No-op teardown
  if constexpr ( core::test_fixture_has_teardown<Fixture> )
  {
    tearDown = [fixture] { fixture->tearDown(); };
  }

  return new TestCaller<Fixture>( name,
                                  setup,
                                  std::bind( test, fixture ),
                                  tearDown,
                                  fixture );
}

#endif // CPPUNIT_HAS_CONCEPTS


CPPUNIT_NS_END

#endif // CPPUNIT_TESTCALLER_H
