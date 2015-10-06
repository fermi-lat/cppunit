#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/Test.h>
#include <cppunit/TestFailure.h>
#include <cppunit/TestLeaf.h>
#include <cppunit/portability/Stream.h>


CPPUNIT_NS_BEGIN


BriefTestProgressListener::BriefTestProgressListener()
    : m_lastTestFailed( false )
{
}


BriefTestProgressListener::~BriefTestProgressListener()
{
}


void 
BriefTestProgressListener::startTest( Test *test )
{
  stdCOut() << test->getName();
  stdCOut().flush();

  m_lastTestFailed = false;
}


void 
BriefTestProgressListener::addFailure( const TestFailure &failure )
{
  if (failure.isError())
    stdCOut() <<  " : error";
  else
  {
    stdCOut() <<  " : assertion";
    if (failure.isExpected())
      stdCOut() <<  " (expected)";
  }
  m_lastTestFailed  = true;
}


void 
BriefTestProgressListener::endTest( Test * )
{
  if ( !m_lastTestFailed )
    stdCOut()  <<  " : OK";
  stdCOut() << "\n";
}


CPPUNIT_NS_END

