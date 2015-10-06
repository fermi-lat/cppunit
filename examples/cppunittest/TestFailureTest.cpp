#include "CoreSuite.h"
#include "TestFailureTest.h"
#include <cppunit/TestFailure.h>
#include <cppunit/Exception.h>


CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( TestFailureTest,
                                       coreSuiteName() );


TestFailureTest::TestFailureTest()
{
}


TestFailureTest::~TestFailureTest()
{
}


void 
TestFailureTest::setUp()
{
  m_exceptionDestroyed = false;
}


void 
TestFailureTest::tearDown()
{
}


void 
TestFailureTest::testConstructorAndGetters()
{
  CPPUNIT_NS::TestCase test;
  CPPUNIT_NS::Exception *error = new ObservedException( this );
  checkTestFailure( &test, error,
                    CPPUNIT_NS::TestFailure::ASSERT );
  CPPUNIT_ASSERT( m_exceptionDestroyed );
}


void 
TestFailureTest::testConstructorAndGettersForError()
{
  CPPUNIT_NS::TestCase test;
  CPPUNIT_NS::Exception *error = new ObservedException( this );
  checkTestFailure( &test, error,
                    CPPUNIT_NS::TestFailure::ERROR );
  CPPUNIT_ASSERT( m_exceptionDestroyed );
}


void 
TestFailureTest::exceptionDestroyed()
{
  m_exceptionDestroyed = true;
}


void
TestFailureTest::checkTestFailure( CPPUNIT_NS::Test *test, 
                                   CPPUNIT_NS::Exception *error,
                                   CPPUNIT_NS::TestFailure::FailureType type )
{
  CPPUNIT_NS::TestFailure failure( test, error, type );
  CPPUNIT_ASSERT_EQUAL( test, failure.failedTest() );
  CPPUNIT_ASSERT_EQUAL( error, failure.thrownException() );
  CPPUNIT_ASSERT_EQUAL( type, failure.getFailureType() );
}
