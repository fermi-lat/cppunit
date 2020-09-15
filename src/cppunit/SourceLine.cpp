#include <cppunit/SourceLine.h>
#include <tuple>


CPPUNIT_NS_BEGIN


SourceLine &
SourceLine::operator =( const SourceLine &other )
{
   if ( this != &other )
   {
      std::tie( m_fileName, m_lineNumber)
        = std::tie( other.m_fileName, other.m_lineNumber );
   }
   return *this;
}


bool 
SourceLine::isValid() const
{
  return ( m_fileName != nullptr && !std::string( m_fileName ).empty() );
}


int 
SourceLine::lineNumber() const
{
  return m_lineNumber;
}


std::string 
SourceLine::fileName() const
{
  return ( m_fileName != nullptr ? std::string( m_fileName ) : "" );
}


bool 
SourceLine::operator ==( const SourceLine &other ) const
{
  return std::tie( m_fileName, m_lineNumber )
    == std::tie( other.m_fileName, other.m_lineNumber);
}


CPPUNIT_NS_END
