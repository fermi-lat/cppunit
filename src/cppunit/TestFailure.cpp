#include <cppunit/Exception.h>
#include <cppunit/Test.h>
#include <cppunit/TestFailure.h>
#include <cppunit/TestLeaf.h>

CPPUNIT_NS_BEGIN


/// Constructs a TestFailure with the given test and exception.
TestFailure::TestFailure( Test *failedTest, 
                          Exception *thrownException,
                          FailureType type ) :
    m_failedTest( failedTest ), 
    m_thrownException( thrownException ),
    m_failureType( type )
{
}

/// Deletes the owned exception.
TestFailure::~TestFailure()
{ 
  delete m_thrownException; 
}

/// Gets the failed test.
Test *
TestFailure::failedTest() const
{ 
  return m_failedTest; 
}


/// Gets the thrown exception. Never \c NULL.
Exception *
TestFailure::thrownException() const
{ 
  return m_thrownException; 
}


/// Gets the failure location.
SourceLine 
TestFailure::sourceLine() const
{
  return m_thrownException->sourceLine();
}


/// Indicates if the failure is actually an unexpected error.
bool 
TestFailure::isError() const
{
  return m_failureType == ERROR;
}


/// Indicates if the failure is actually an unexpected pass.
bool
TestFailure::isUnexpectedPass() const
{
  return m_failureType == UNEXPECTED_PASS;
}


/// Indicates if the failure is expected and won't count as a failure.
bool
TestFailure::isExpected() const
{
  TestLeaf *test = dynamic_cast< TestLeaf* >( m_failedTest );
  return ( test && test->expectedFailure() );
}


/// Returns the type of the failure
TestFailure::FailureType
TestFailure::getFailureType() const
{
  return m_failureType;
}


/// Gets the name of the failed test.
std::string 
TestFailure::failedTestName() const
{
  return m_failedTest->getName();
}


TestFailure *
TestFailure::clone() const
{
  return new TestFailure( m_failedTest,
                          m_thrownException->clone(), m_failureType );
}


CPPUNIT_NS_END
