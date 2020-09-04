#include "CoreSuite.h"
#include "TestSourceLineTest.h"
#include <cppunit/TestFailure.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <functional>


CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( TestSourceLineTest,
                                       coreSuiteName() );


void
TestSourceLineTest::testSourceLine()
{
  // All checks should fail assertion so assertion location can be inspected
  static const struct
  {
    const CPPUNIT_NS::SourceLine expectedSourceLine;
    const std::function<void()> function;
  }
  checks[] =
  {
    // CPPUNIT_SOURCELINE() has to be on the line before ASSERT for test to work
    // Test will check that recorded assert location is exactly next line after expected
    // (for source code formatting purposes)
    // (this kind of makes sense when dealing with source code location stuff, does it not?)
    {
      CPPUNIT_SOURCELINE(),
      [] { CPPUNIT_ASSERT( false ); }
    },
    {
      CPPUNIT_SOURCELINE(),
      [] { CPPUNIT_ASSERT_MESSAGE( "msg", false ); }
    },
    {
      CPPUNIT_SOURCELINE(),
      [] { CPPUNIT_FAIL( "msg" ); }
    },
    {
      CPPUNIT_SOURCELINE(),
      [] { CPPUNIT_ASSERT_EQUAL( 1, 2 ); }
    },
    {
      CPPUNIT_SOURCELINE(),
      [] { CPPUNIT_ASSERT_EQUAL_MESSAGE( "msg", 1, 2 ); }
    },
    {
      CPPUNIT_SOURCELINE(),
      [] { CPPUNIT_ASSERT_LESS( 1, 2 ); }
    },
    {
      CPPUNIT_SOURCELINE(),
      [] { CPPUNIT_ASSERT_GREATER( 2, 1 ); }
    },
    {
      CPPUNIT_SOURCELINE(),
      [] { CPPUNIT_ASSERT_LESSEQUAL( 1, 2 ); }
    },
    {
      CPPUNIT_SOURCELINE(),
      [] { CPPUNIT_ASSERT_GREATEREQUAL( 2, 1 ); }
    },
    {
      CPPUNIT_SOURCELINE(),
      [] { CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, 1.0, 0.001 ); }
    },
    {
      CPPUNIT_SOURCELINE(),
      [] { CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE( "msg", 0.0, 1.0, 0.001 ); }
    },
    {
      CPPUNIT_SOURCELINE(),
      [] { CPPUNIT_ASSERT_THROW( , std::runtime_error ); }
    },
    {
      CPPUNIT_SOURCELINE(),
      [] { CPPUNIT_ASSERT_THROW_MESSAGE( "msg", , std::runtime_error ); }
    },
    {
      CPPUNIT_SOURCELINE(),
      [] { CPPUNIT_ASSERT_NO_THROW( throw std::runtime_error( "" ) ); }
    },
    {
      CPPUNIT_SOURCELINE(),
      [] { CPPUNIT_ASSERT_NO_THROW_MESSAGE( "msg", throw std::runtime_error( "" ) ); }
    },
    {
      CPPUNIT_SOURCELINE(),
      [] { CPPUNIT_ASSERT_ASSERTION_FAIL( ); }
    },
    {
      CPPUNIT_SOURCELINE(),
      [] { CPPUNIT_ASSERT_ASSERTION_FAIL_MESSAGE( "msg", ); }
    },
    {
      CPPUNIT_SOURCELINE(),
      [] { CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT( false ) ); }
    },
    {
      CPPUNIT_SOURCELINE(),
      [] { CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE( "msg", CPPUNIT_ASSERT( false ) ); }
    },
  };

  for ( const auto &check : checks )
  {
    CPPUNIT_NS::TestResult controller;
    CPPUNIT_NS::TestResultCollector result;
    controller.addListener( &result );
    CPPUNIT_NS::TestRunner runner;

    // TODO: names for checks?
    runner.addTest( new CPPUNIT_NS::TestCaller<TestSourceLineTest>( "",
                                                                    check.function,
                                                                    new TestSourceLineTest() ) );
    runner.run( controller );

    CPPUNIT_ASSERT_EQUAL_MESSAGE( "All checks are supposed to fail (assert)",
                                  1,
                                  result.testFailures() );

    const CPPUNIT_NS::TestFailure *failure = *result.failures().begin();
    CPPUNIT_ASSERT( failure != nullptr );

    const CPPUNIT_NS::SourceLine sourceLine = failure->sourceLine();

    CPPUNIT_ASSERT( sourceLine.isValid() );

    CPPUNIT_ASSERT_EQUAL( check.expectedSourceLine.fileName(),
                          sourceLine.fileName() );
    CPPUNIT_ASSERT_EQUAL( check.expectedSourceLine.lineNumber() + 1,
                          sourceLine.lineNumber() );
  }
}
