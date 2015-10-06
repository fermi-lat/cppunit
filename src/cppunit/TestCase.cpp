#include <cppunit/Portability.h>
#include <cppunit/Exception.h>
#include <cppunit/Protector.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestResult.h>
#include <stdexcept>

#if CPPUNIT_USE_TYPEINFO_NAME
#  include <typeinfo>
#endif

#include "TestCaseMethodFunctor.h"


CPPUNIT_NS_BEGIN


/** Constructs a test case.
 *  \param name the name of the TestCase.
 **/
TestCase::TestCase( const std::string &name, const SourceLine &line )
    : m_name( name )
    , m_declaration( line )
{
}


/// Run the test and catch any exceptions that are triggered by it 
void 
TestCase::run( TestResult *result )
{
  result->startTest(this);
/*
  try {
    setUp();

    try {
      runTest();
    }
    catch ( Exception &e ) {
      Exception *copy = e.clone();
      result->addFailure( this, copy );
    }
    catch ( std::exception &e ) {
      result->addError( this, new Exception( Message( "uncaught std::exception", 
                                                      e.what() ) ) );
    }
    catch (...) {
      Exception *e = new Exception( Message( "uncaught unknown exception" ) );
      result->addError( this, e );
    }

    try {
      tearDown();
    }
    catch (...) {
      result->addError( this, new Exception( Message( "tearDown() failed" ) ) );
    }
  }
  catch (...) {
    result->addError( this, new Exception( Message( "setUp() failed" ) ) );
  }
*/
  if ( result->protect( TestCaseMethodFunctor( this, &TestCase::setUp ),
                        this,
                       "setUp() failed" ) )
  {
    result->protect( TestCaseMethodFunctor( this, &TestCase::runTest ),
                     this );
  }

  result->protect( TestCaseMethodFunctor( this, &TestCase::tearDown ),
                   this,
                   "tearDown() failed" );

  result->endTest( this );
}


/// All the work for runTest is deferred to subclasses 
void 
TestCase::runTest()
{
}


/** Constructs a test case for a suite.
 * \deprecated This constructor was used by fixture when TestFixture did not exist.
 *             Have your fixture inherits TestFixture instead of TestCase.
 * \internal
 *  This TestCase was intended for use by the TestCaller and should not
 *  be used by a test case for which run() is called.
 **/
TestCase::TestCase()
    : m_name( "" )
{
}


/// Destructs a test case
TestCase::~TestCase()
{
}


/// Returns the name of the test case
std::string 
TestCase::getName() const
{ 
  return m_name; 
}


/// Return the source line of the test function declaration
const SourceLine&
TestCase::getDeclaration() const
{
  return m_declaration;
}


CPPUNIT_NS_END
