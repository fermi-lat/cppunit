#include "CoreSuite.h"
#include "TestAssertTest.h"
#include <cppunit/extensions/TypeInfoHelper.h>
#include <cppunit/portability/FloatingPoint.h>
#include <cppunit/TestFailure.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <algorithm>
#include <functional>
#include <limits>

/*
 Note:
 - tests need to be added to test asserEquals() template function and
 use of assertion traits. Some check may need to be added to check
 the message content in Exception.
 - code need to be refactored with the use of a test caller that expect
 an exception.
 */

enum class EnumClass
{
    VALUE1,
    VALUE2
};


CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( TestAssertTest,
                                       coreSuiteName() );


TestAssertTest::TestAssertTest()
{
}


TestAssertTest::~TestAssertTest()
{
}


void 
TestAssertTest::setUp()
{
}


void 
TestAssertTest::tearDown()
{
}


void
TestAssertTest::throwsException()
{
  throw CPPUNIT_NS::Exception();
}


void 
TestAssertTest::testAssertThrow()
{
   // Special case with this pointer referencing fixture
   CPPUNIT_ASSERT_THROW( this->throwsException(), CPPUNIT_NS::Exception );

   CPPUNIT_ASSERT_THROW( throw std::string(), std::string );

   try
   {
      int x;
      CPPUNIT_ASSERT_THROW( x = 1234, std::string );
      (void) x;
   }
   catch ( CPPUNIT_NS::Exception & )
   {
      return;
   }

   throw std::exception();
}


void
TestAssertTest::doesNotThrowException()
{
}


void 
TestAssertTest::testAssertNoThrow()
{
   // Special case with this pointer referencing fixture
   CPPUNIT_ASSERT_NO_THROW( this->doesNotThrowException() );

   int x;
   CPPUNIT_ASSERT_NO_THROW( x = 1234 );
   (void)x;

   try
   {
      CPPUNIT_ASSERT_NO_THROW( throw std::exception() );
   }
   catch ( CPPUNIT_NS::Exception & )
   {
      return;
   }
   throw std::exception();
}


void 
TestAssertTest::testAssertAssertionFail()
{
   CPPUNIT_ASSERT_ASSERTION_FAIL( throw CPPUNIT_NS::Exception() );

   try
   {
      int x;
      CPPUNIT_ASSERT_ASSERTION_FAIL( x = 1234 );
      (void)x;
   }
   catch ( CPPUNIT_NS::Exception & )
   {
      return;
   }

   throw std::exception();
}


void 
TestAssertTest::testAssertAssertionPass()
{
   int x;
   CPPUNIT_ASSERT_ASSERTION_PASS( x = 1234 );
   (void)x;

   try
   {
      CPPUNIT_ASSERT_ASSERTION_PASS( throw CPPUNIT_NS::Exception() );
   }
   catch ( CPPUNIT_NS::Exception & )
   {
      return;
   }

   throw std::exception();
}


void 
TestAssertTest::testAssert()
{
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT( true ) );
  
  CPPUNIT_ASSERT_ASSERTION_FAIL( CPPUNIT_ASSERT( false ) );
}


static int foo() { return 1; }


void 
TestAssertTest::testAssertEqual()
{
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_EQUAL( 1, 1 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_EQUAL( 1, foo() ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_EQUAL( 12345678, 12345678 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_EQUAL( EnumClass::VALUE1, EnumClass::VALUE1 ) );

  CPPUNIT_ASSERT_ASSERTION_FAIL( CPPUNIT_ASSERT_EQUAL( 1, 2 ) );
  CPPUNIT_ASSERT_ASSERTION_FAIL( CPPUNIT_ASSERT_EQUAL( EnumClass::VALUE1, EnumClass::VALUE2 ) );
}


void
TestAssertTest::testAssertLess()
{
    CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_LESS( 2, 1 ) );
    CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_LESS( 12345679, 12345678 ) );

    CPPUNIT_ASSERT_ASSERTION_FAIL( CPPUNIT_ASSERT_LESS( 1, 2 ) );
}


void
TestAssertTest::testAssertGreater()
{
    CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_GREATER( 1, 2 ) );
    CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_GREATER( 12345678, 12345679 ));

    CPPUNIT_ASSERT_ASSERTION_FAIL( CPPUNIT_ASSERT_GREATER( 2, 1 ) );
    CPPUNIT_ASSERT_ASSERTION_FAIL( CPPUNIT_ASSERT_GREATER( 2, 2 ) );
}


void
TestAssertTest::testAssertLessEqual()
{
    CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_LESSEQUAL( 2, 1 ) );
    CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_LESSEQUAL( 12345679, 12345678 ));
    CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_LESSEQUAL( 2, 2 ) );

    CPPUNIT_ASSERT_ASSERTION_FAIL( CPPUNIT_ASSERT_LESSEQUAL( 1, 2 ) );
}

void
TestAssertTest::testAssertGreaterEqual()
{
    CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_GREATEREQUAL( 1, 2 ) );
    CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_GREATEREQUAL( 12345678, 12345679 ));
    CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_GREATEREQUAL( 12345678, 12345678 ));
    CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_GREATEREQUAL( 2, 2 ) );

    CPPUNIT_ASSERT_ASSERTION_FAIL( CPPUNIT_ASSERT_GREATEREQUAL( 2, 1 ) );
}


void 
TestAssertTest::testAssertMessageTrue()
{
  CPPUNIT_ASSERT_ASSERTION_PASS( 
     CPPUNIT_ASSERT_MESSAGE( "This test should not failed", true ) );
}


void 
TestAssertTest::testAssertMessageFalse()
{
  bool exceptionCaught = false;
  std::string message( "This test message should not be seen" );
  try
  {
    CPPUNIT_ASSERT_MESSAGE( message, 2==3 );
  }
  catch( CPPUNIT_NS::Exception &e )
  {
    exceptionCaught = true; // ok, we were expecting an exception.
    checkMessageContains( &e, message );
  }

  CPPUNIT_ASSERT( exceptionCaught );
}


void 
TestAssertTest::testAssertDoubleEquals()
{
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.1, 1.2, 0.101 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.2, 1.1, 0.101 ) );

  CPPUNIT_ASSERT_ASSERTION_FAIL( CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.1, 1.2, 0.09 ) );
  CPPUNIT_ASSERT_ASSERTION_FAIL( CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.2, 1.1, 0.09 ) );
}

/*
 * Test that the error message from CPPUNIT_ASSERT_DOUBLES_EQUAL() 
 * has more than the default 6 digits of precision.
 */
void 
TestAssertTest::testAssertDoubleEqualsPrecision()
{
  std::string failure( "2.000000001" );
  try
  {
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, 2.000000001, 1 );
  }
  catch( CPPUNIT_NS::Exception &e )
  {
    checkMessageContains( &e, failure );
    return;
  }
  CPPUNIT_FAIL( "Expected assertion failure" );
}


void 
TestAssertTest::testAssertDoubleNonFinite()
{
  double inf = std::numeric_limits<double>::infinity();
  double nan = std::numeric_limits<double>::quiet_NaN();
  // test our portable floating-point primitives that detect NaN values
  CPPUNIT_ASSERT( CPPUNIT_NS::floatingPointIsUnordered( nan ) );
  CPPUNIT_ASSERT( !CPPUNIT_NS::floatingPointIsUnordered( inf ) );
  CPPUNIT_ASSERT( !CPPUNIT_NS::floatingPointIsUnordered( -inf ) );
  CPPUNIT_ASSERT( !CPPUNIT_NS::floatingPointIsUnordered( 1.0 ) );
  CPPUNIT_ASSERT( !CPPUNIT_NS::floatingPointIsUnordered( 1.5 ) );
  CPPUNIT_ASSERT( !CPPUNIT_NS::floatingPointIsUnordered( 2.0 ) );
  CPPUNIT_ASSERT( !CPPUNIT_NS::floatingPointIsUnordered( 2.5 ) );
  CPPUNIT_ASSERT( !CPPUNIT_NS::floatingPointIsUnordered( 0.0 ) );
  CPPUNIT_ASSERT( !CPPUNIT_NS::floatingPointIsUnordered( -1.0 ) );
  CPPUNIT_ASSERT( !CPPUNIT_NS::floatingPointIsUnordered( -2.0 ) );
  // test our portable floating-point primitives that detect finite values
  CPPUNIT_ASSERT( CPPUNIT_NS::floatingPointIsFinite( 0.0 ) );
  CPPUNIT_ASSERT( CPPUNIT_NS::floatingPointIsFinite( 0.5 ) );
  CPPUNIT_ASSERT( CPPUNIT_NS::floatingPointIsFinite( 1.0 ) );
  CPPUNIT_ASSERT( CPPUNIT_NS::floatingPointIsFinite( 1.5 ) );
  CPPUNIT_ASSERT( CPPUNIT_NS::floatingPointIsFinite( 2.0 ) );
  CPPUNIT_ASSERT( CPPUNIT_NS::floatingPointIsFinite( 2.5 ) );
  CPPUNIT_ASSERT( CPPUNIT_NS::floatingPointIsFinite( -1.5 ) );
  CPPUNIT_ASSERT( !CPPUNIT_NS::floatingPointIsFinite( nan ) );
  CPPUNIT_ASSERT( !CPPUNIT_NS::floatingPointIsFinite( inf ) );
  CPPUNIT_ASSERT( !CPPUNIT_NS::floatingPointIsFinite( -inf ) );
  // Infinity tests
  CPPUNIT_ASSERT( inf == inf );
  CPPUNIT_ASSERT( -inf == -inf );
  CPPUNIT_ASSERT( -inf != inf );
  CPPUNIT_ASSERT( -inf < inf );
  CPPUNIT_ASSERT( inf > -inf );
  CPPUNIT_ASSERT_ASSERTION_FAIL( CPPUNIT_ASSERT_DOUBLES_EQUAL( inf, 0.0, 1.0 ) );
  CPPUNIT_ASSERT_ASSERTION_FAIL( CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, inf, 1.0 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_EQUAL( inf, inf, 1.0 ) );
  // NaN tests 
  CPPUNIT_ASSERT_ASSERTION_FAIL( CPPUNIT_ASSERT_DOUBLES_EQUAL( nan, 0.0, 1.0 ) );
  CPPUNIT_ASSERT_ASSERTION_FAIL( CPPUNIT_ASSERT_DOUBLES_EQUAL( nan, nan, 1.0 ) );
  CPPUNIT_ASSERT_ASSERTION_FAIL( CPPUNIT_ASSERT_DOUBLES_EQUAL( nan, inf, 1.0 ) );
  CPPUNIT_ASSERT_ASSERTION_FAIL( CPPUNIT_ASSERT_DOUBLES_EQUAL( inf, nan, 1.0 ) );
}


void
TestAssertTest::testAssertDoubleEqualsMixedTypes()
{
  // This test covers types mixing in CPPUNIT_ASSERT_DOUBLES_EQUAL()
  // that existed in C++11 implementation due to compiler converting types.
  // In C++20 implementation this kind of types mixing is explicitly
  // supported in other asserts too.

  // Mixing floats, doubles and ints
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_EQUAL( 2, 2.0, 0.1 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.0, 2, 0.1 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_EQUAL( 2, 2.0f, 0.1 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.0f, 2, 0.1 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.0f, 2.0, 0.1 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.0, 2.0f, 0.1 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.0f, 2.0, 0.1 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.0, 2.0f, 0.1 ) );

  // Integer types converted to double
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_EQUAL( 2, 2, 0.1 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_EQUAL( 2, 2, 0.1 ) );
}


void
TestAssertTest::testAssertDoubleLessEqualAsserts()
{
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_LESSEQUAL( 2.0, 1.0, 0.1 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_LESSEQUAL( 2.0, 2.0, 0.1 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_LESSEQUAL( 2.0, 2.09, 0.1 ) );

  CPPUNIT_ASSERT_ASSERTION_FAIL( CPPUNIT_ASSERT_DOUBLES_LESSEQUAL( 2.0, 2.1, 0.1 ) );

  // Test types mixing
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_LESSEQUAL( 2.0, 1.0f, 0.1 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_LESSEQUAL( 2.0f, 1.0, 0.1 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_LESSEQUAL( 2.0f, 1, 0.1 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_LESSEQUAL( 2, 1.0f, 0.1 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_LESSEQUAL( 2, 1.0, 0.1 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_LESSEQUAL( 2.0, 1, 0.1 ) );

  // Integers
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_LESSEQUAL( 2, 2, 0.1 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_LESSEQUAL( 2L, 2LL, 0.1 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_LESSEQUAL( 2U, 2LU, 0.1 ) );
}


void
TestAssertTest::testAssertDoubleGreaterEqualAsserts()
{
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_GREATEREQUAL( 2.0, 3.0, 0.1 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_GREATEREQUAL( 2.0, 2.0, 0.1 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_GREATEREQUAL( 2.0, 1.91, 0.1 ) );

  CPPUNIT_ASSERT_ASSERTION_FAIL( CPPUNIT_ASSERT_DOUBLES_GREATEREQUAL( 2.0, 1.9, 0.1 ) );

  // Test types mixing
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_GREATEREQUAL( 2.0, 3.0f, 0.1 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_GREATEREQUAL( 2.0f, 3.0, 0.1 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_GREATEREQUAL( 2.0f, 3, 0.1 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_GREATEREQUAL( 2, 3.0f, 0.1 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_GREATEREQUAL( 2, 3.0, 0.1 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_GREATEREQUAL( 2.0, 3, 0.1 ) );

  // Integers
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_GREATEREQUAL( 2, 2, 0.1 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_GREATEREQUAL( 2L, 2LL, 0.1 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_DOUBLES_GREATEREQUAL( 2U, 2LU, 0.1 ) );
}


void 
TestAssertTest::testFail()
{
  bool exceptionCaught = false;
  std::string failure( "FailureMessage" );
  try
  {
    CPPUNIT_FAIL( failure );
  }
  catch( CPPUNIT_NS::Exception &e )
  {
    exceptionCaught = true;
    checkMessageContains( &e, failure );
  }
  CPPUNIT_ASSERT( exceptionCaught );
}


void 
TestAssertTest::checkMessageContains( CPPUNIT_NS::Exception *e,
                                      std::string expected )
{
  std::string actual = e->what();
  CPPUNIT_ASSERT_MESSAGE( "Expected message not found: " + expected +
                          ", was: " + actual,
      std::search( actual.begin(), actual.end(), 
                   expected.begin(), expected.end() ) != actual.end() );
}


void
TestAssertTest::testAssertExceptionDetail()
{
  // All checks should fail assertion so exception detail can be inspected
  static const struct
  {
    const std::string name;
    const std::string expectedDetail;
    const std::function<void()> function;
  }
  checks[] =
  {
    {
      "CPPUNIT_ASSERT_THROW",
      "Expected: " + CPPUNIT_NS::TypeInfoHelper::getClassName( typeid(std::runtime_error) ),
      [] { CPPUNIT_ASSERT_THROW( , std::runtime_error ); },
    },
    {
      "CPPUNIT_ASSERT_THROW_MESSAGE",
      "Expected: " + CPPUNIT_NS::TypeInfoHelper::getClassName( typeid(std::runtime_error) ),
      [] { CPPUNIT_ASSERT_THROW_MESSAGE( "msg", , std::runtime_error ); },
    },
    {
      "CPPUNIT_ASSERT_NO_THROW",
      "Caught: " + CPPUNIT_NS::TypeInfoHelper::getClassName( typeid(std::runtime_error) ),
      [] { CPPUNIT_ASSERT_NO_THROW( throw std::runtime_error( "" ) ); },
    },
    {
      "CPPUNIT_ASSERT_NO_THROW_MESSAGE",
      "Caught: " + CPPUNIT_NS::TypeInfoHelper::getClassName( typeid(std::runtime_error) ),
      [] { CPPUNIT_ASSERT_NO_THROW_MESSAGE( "msg", throw std::runtime_error( "" ) ); },
    },
    // When CppUnit assertion fails, it throws CPPUNIT_NS::Exception
    {
      "CPPUNIT_ASSERT_ASSERTION_FAIL",
      "Expected: " + CPPUNIT_NS::TypeInfoHelper::getClassName( typeid(CPPUNIT_NS::Exception) ),
      [] { CPPUNIT_ASSERT_ASSERTION_FAIL(); },
    },
    {
      "CPPUNIT_ASSERT_ASSERTION_FAIL_MESSAGE",
      "Expected: " + CPPUNIT_NS::TypeInfoHelper::getClassName( typeid(CPPUNIT_NS::Exception) ),
      [] { CPPUNIT_ASSERT_ASSERTION_FAIL_MESSAGE( "msg", ); },
    },
    {
      "CPPUNIT_ASSERT_ASSERTION_PASS",
      "Caught: " + CPPUNIT_NS::TypeInfoHelper::getClassName( typeid(CPPUNIT_NS::Exception) ),
      [] { CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT( false ) ); },
    },
    {
      "CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE",
      "Caught: " + CPPUNIT_NS::TypeInfoHelper::getClassName( typeid(CPPUNIT_NS::Exception) ),
      [] { CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE( "msg", CPPUNIT_ASSERT( false ) ); },
    },
  };

  for ( const auto &check : checks )
  {
    CPPUNIT_NS::TestResult controller;
    CPPUNIT_NS::TestResultCollector result;
    controller.addListener( &result );
    CPPUNIT_NS::TestRunner runner;

    runner.addTest( new CPPUNIT_NS::TestCaller<TestAssertTest>( check.name,
                                                                check.function,
                                                                new TestAssertTest() ) );
    runner.run( controller );

    CPPUNIT_ASSERT_EQUAL_MESSAGE( "All checks are supposed to fail (assert), "
                                  "check that didn't assert was " + check.name,
                                  1,
                                  result.testFailures() );

    const CPPUNIT_NS::TestFailure *failure = *result.failures().begin();
    CPPUNIT_ASSERT( failure != nullptr );

    const CPPUNIT_NS::Exception *exception = failure->thrownException();
    CPPUNIT_ASSERT( exception != nullptr );

    const CPPUNIT_NS::Message message = exception->message();

    bool detailFound = false;
    for ( int i = 0; i < message.detailCount(); ++i )
    {
      if ( message.detailAt( i ) == check.expectedDetail )
      {
        detailFound = true;
        break;
      }
    }

    CPPUNIT_ASSERT_MESSAGE( "Was expecting detail: " + check.expectedDetail,
                            detailFound );
  }
}


#if CPPUNIT_HAS_CONCEPTS

void
TestAssertTest::testConceptsAsserts()
{
  // Mixing types in CPPUNIT_ASSERT_EQUAL()
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_EQUAL( 0u, std::vector<int>().size() ) );
  CPPUNIT_ASSERT_ASSERTION_FAIL( CPPUNIT_ASSERT_EQUAL( 1u, std::vector<int>().size() ) );

  // Mixing types in CPPUNIT_ASSERT_LESS()
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_LESS( 2, 1LL ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_LESS( 2LL, 1 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_LESS( 2, 1.0 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_LESS( 2.0, 1 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_LESS( 2, 1.0f ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_LESS( 2.0f, 1 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_LESS( 2.0f, 1.0 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_LESS( 2.0, 1.0f ) );

  // Mixing types in CPPUNIT_ASSERT_GREATER()
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_GREATER( 1, 2LL ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_GREATER( 1LL, 2 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_GREATER( 1, 2.0 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_GREATER( 1.0, 2 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_GREATER( 1, 2.0f ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_GREATER( 1.0f, 2 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_GREATER( 1.0f, 2.0 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_GREATER( 1.0, 2.0 ) );

  // Mixing types in CPPUNIT_ASSERT_LESSEQUAL() is actually not quite
  // possible in C++20 because it won't compile for floating point types
  // and CPPUNIT_ASSERT_DOUBLES_LESSEQUAL() should be used instead.
  // So the floowing asserts are going to test intergers of different
  // width but of the same signess (otherwise it's going to be
  // comparison of values of different signess).
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_LESSEQUAL( 2, 1L ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_LESSEQUAL( 2L, 1 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_LESSEQUAL( 2, 1LL ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_LESSEQUAL( 2LL, 1 ) );

  // Mixing types in CPPUNIT_ASSERT_GREATEREQUAL()
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_GREATEREQUAL( 1, 2LL ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_GREATEREQUAL( 1LL, 2 ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_GREATEREQUAL( 1, 2L ) );
  CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT_GREATEREQUAL( 1L, 2 ) );
}

#endif // CPPUNIT_HAS_CONCEPTS
