#include <cppunit/TestLeaf.h>


CPPUNIT_NS_BEGIN


TestLeaf::TestLeaf()
  : Test()
  , m_expectedFailure( false )
{
}


int 
TestLeaf::countTestCases() const
{
  return 1;
}


int 
TestLeaf::getChildTestCount() const
{
  return 0;
}


Test *
TestLeaf::doGetChildTestAt( int index ) const
{
  checkIsValidIndex( index );
  return NULL;    // never called, checkIsValidIndex() always throw.
}


void TestLeaf::setExpectedFailure()
{
  m_expectedFailure = true;
}


bool TestLeaf::expectedFailure() const
{
  return m_expectedFailure;
}


CPPUNIT_NS_END
