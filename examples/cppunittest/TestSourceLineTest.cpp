#include "CoreSuite.h"
#include "TestSourceLineTest.h"
#include <cppunit/TestFailure.h>
#include <cppunit/TestListener.h>
#include <cppunit/TestResult.h>
#include <cppunit/extensions/TestDecorator.h>

namespace
{
  class SourceLineTestListener : public CPPUNIT_NS::TestListener
  {
  public:
    void addFailure( const CPPUNIT_NS::TestFailure &failure ) override
    {
      m_failure = failure.sourceLine();
    }

    const CPPUNIT_NS::SourceLine& failure() const
    {
      return m_failure;
    }

  private:
    CPPUNIT_NS::SourceLine m_failure;
  };


  class SourceLineMockTest : public CPPUNIT_NS::TestCase
  {
  public:
    using MockFunction = std::function<void()>;

  public:
    SourceLineMockTest( MockFunction testFunction )
        : CPPUNIT_NS::TestCase("")
        , m_testFunction(testFunction)
    {
    }

    void runTest() override
    {
        CPPUNIT_ASSERT(m_testFunction);
        m_testFunction();
    }

  private:
      MockFunction m_testFunction;
  };

} // namespace


CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( TestSourceLineTest,
                                       coreSuiteName() );


// TODO: compat API tests?
// TODO: direct API tests?


void
TestSourceLineTest::testSourceLine()
{
  std::string file;
  int line = 0;

#define ASSIGN(file, line) (file) = __FILE__; (line) = __LINE__

  // ASSIGN has to be on the same line with ASSERT for test to work
  // all check should fail assertion
  static const SourceLineMockTest::MockFunction checks[] = {
    [&file, &line] { ASSIGN(file, line); CPPUNIT_ASSERT( false ); },
    [&file, &line] { ASSIGN(file, line); CPPUNIT_ASSERT_MESSAGE( "msg", false ); },
    [&file, &line] { ASSIGN(file, line); CPPUNIT_FAIL( "msg" ); },
    [&file, &line] { ASSIGN(file, line); CPPUNIT_ASSERT_EQUAL( 1, 2 ); },
    [&file, &line] { ASSIGN(file, line); CPPUNIT_ASSERT_EQUAL_MESSAGE( "msg", 1, 2 ); },
    [&file, &line] { ASSIGN(file, line); CPPUNIT_ASSERT_LESS( 1, 2 ); },
    [&file, &line] { ASSIGN(file, line); CPPUNIT_ASSERT_GREATER( 2, 1 ); },
    [&file, &line] { ASSIGN(file, line); CPPUNIT_ASSERT_LESSEQUAL( 1, 2 ); },
    [&file, &line] { ASSIGN(file, line); CPPUNIT_ASSERT_GREATEREQUAL( 2, 1 ); },
    [&file, &line] { ASSIGN(file, line); CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, 1.0, 0.001 ); },
    [&file, &line] { ASSIGN(file, line); CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE( "msg", 0.0, 1.0, 0.001 ); },
    [&file, &line] { ASSIGN(file, line); CPPUNIT_ASSERT_THROW( while ( 0 ), std::runtime_error ); },
    [&file, &line] { ASSIGN(file, line); CPPUNIT_ASSERT_THROW_MESSAGE( "msg", while ( 0 ), std::runtime_error ); },
    [&file, &line] { ASSIGN(file, line); CPPUNIT_ASSERT_NO_THROW( throw CPPUNIT_NS::Exception() ); },
    [&file, &line] { ASSIGN(file, line); CPPUNIT_ASSERT_NO_THROW_MESSAGE( "msg", throw CPPUNIT_NS::Exception() ); },
    [&file, &line] { ASSIGN(file, line); CPPUNIT_ASSERT_ASSERTION_FAIL( int x = 2; (void)(x); ); },
    [&file, &line] { ASSIGN(file, line); CPPUNIT_ASSERT_ASSERTION_FAIL_MESSAGE( "msg", int x = 2; (void)(x); ); },
    [&file, &line] { ASSIGN(file, line); CPPUNIT_ASSERT_ASSERTION_PASS( throw CPPUNIT_NS::Exception() ); },
    [&file, &line] { ASSIGN(file, line); CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE( "msg", throw CPPUNIT_NS::Exception() ); },
  };

#undef ASSIGN

  for ( const auto &check : checks )
  {
    auto test = new SourceLineMockTest( check );
    auto decorator = new CPPUNIT_NS::TestDecorator( test );

    SourceLineTestListener listener;
    CPPUNIT_NS::TestResult result;
    result.addListener( &listener );
    decorator->run( &result );

    const auto &sourceLine = listener.failure();

    CPPUNIT_ASSERT( sourceLine.isValid() );

    // std::cout << sourceLine.fileName() << ":" << sourceLine.lineNumber() << std::endl;
    // std::cout << __FILE__ << ":" << __LINE__ << std::endl;
    // std::cout << file << ":" << line << std::endl;

    CPPUNIT_ASSERT_EQUAL( file, sourceLine.fileName() );
    CPPUNIT_ASSERT_EQUAL( line, sourceLine.lineNumber() );

    delete decorator;
  }
}
