#ifndef CPPUNIT_HELPER_TESTSUITEBUILDERCONTEXT_TCC
#define CPPUNIT_HELPER_TESTSUITEBUILDERCONTEXT_TCC

#include <cppunit/TestSuite.h>
#include <cppunit/extensions/TestFixtureFactory.h>
#include <cppunit/extensions/TestNamer.h>
#include <cppunit/extensions/TestSuiteBuilderContext.h>


CPPUNIT_NS_BEGIN

template <class Fixture>
TestSuiteBuilderContextBase<Fixture>::TestSuiteBuilderContextBase( 
                                 TestSuite &suite,
                                 const TestNamer &namer,
                                 TestFixtureFactory<Fixture> &factory )
  : m_suite( suite )
  , m_namer( namer )
  , m_factory( factory )
  , m_properties()
{
}


template <class Fixture>
void 
TestSuiteBuilderContextBase<Fixture>::addTest( Test *test )
{
  m_suite.addTest( test );
}


template <class Fixture>
std::string 
TestSuiteBuilderContextBase<Fixture>::getFixtureName() const
{
  return m_namer.getFixtureName();
}


template <class Fixture>
std::string 
TestSuiteBuilderContextBase<Fixture>::getTestNameFor( 
                                 const std::string &testMethodName ) const
{
  return m_namer.getTestNameFor( testMethodName );
}


template <class Fixture>
typename TestSuiteBuilderContextBase<Fixture>::FixtureType *
TestSuiteBuilderContextBase<Fixture>::makeTestFixture() const
{
  return m_factory.makeFixture();
}


template <class Fixture>
void 
TestSuiteBuilderContextBase<Fixture>::addProperty( const std::string &key, 
                                          const std::string &value )
{
  Properties::iterator it = m_properties.begin();
  for ( ; it != m_properties.end(); ++it )
  {
    if ( (*it).first == key )
    {
      (*it).second = value;
      return;
    }
  }

  Property property( key, value );
  m_properties.push_back( property );
}

template <class Fixture>
const std::string 
TestSuiteBuilderContextBase<Fixture>::getStringProperty( const std::string &key ) const
{
  Properties::const_iterator it = m_properties.begin();
  for ( ; it != m_properties.end(); ++it )
  {
    if ( (*it).first == key )
      return (*it).second;
  }
  return "";
}


CPPUNIT_NS_END

#endif // CPPUNIT_HELPER_TESTSUITEBUILDERCONTEXT_TCC