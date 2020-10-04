#ifndef CPPUNIT_HELPER_TESTSUITEBUILDERCONTEXT_TCC
#define CPPUNIT_HELPER_TESTSUITEBUILDERCONTEXT_TCC

#include <cppunit/TestSuite.h>
#include <cppunit/extensions/TestNamer.h>
#include <cppunit/extensions/TestSuiteBuilderContext.h>


CPPUNIT_NS_BEGIN

template <typename Fixture>
TestSuiteBuilderContextTemplate<Fixture>::TestSuiteBuilderContextTemplate( 
                                 TestSuite &suite,
                                 const TestNamer &namer,
                                 TestFixtureFactory<Fixture> &factory )
  : m_suite( suite )
  , m_namer( namer )
  , m_factory( factory )
  , m_properties()
{
}


template <typename Fixture>
void 
TestSuiteBuilderContextTemplate<Fixture>::addTest( Test *test )
{
  m_suite.addTest( test );
}


template <typename Fixture>
std::string 
TestSuiteBuilderContextTemplate<Fixture>::getFixtureName() const
{
  return m_namer.getFixtureName();
}


template <typename Fixture>
std::string 
TestSuiteBuilderContextTemplate<Fixture>::getTestNameFor( 
                                 const std::string &testMethodName ) const
{
  return m_namer.getTestNameFor( testMethodName );
}


template <typename Fixture>
typename TestSuiteBuilderContextTemplate<Fixture>::FixtureType *
TestSuiteBuilderContextTemplate<Fixture>::makeTestFixture() const
{
  return m_factory.makeFixture();
}


template <typename Fixture>
void 
TestSuiteBuilderContextTemplate<Fixture>::addProperty( const std::string &key, 
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

template <typename Fixture>
const std::string 
TestSuiteBuilderContextTemplate<Fixture>::getStringProperty( const std::string &key ) const
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