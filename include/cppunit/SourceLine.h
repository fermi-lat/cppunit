#ifndef CPPUNIT_SOURCELINE_H
#define CPPUNIT_SOURCELINE_H

#include <cppunit/Portability.h>
#include <string>

#if CPPUNIT_HAS_SOURCE_LOCATION
# include <source_location>
#elif CPPUNIT_HAS_EXPERIMETAL_SOURCE_LOCATION
# include <experimental/source_location>
  namespace std
  {
    using source_location = experimental::source_location;
  }
#define CPPUNIT_HAS_SOURCE_LOCATION 1
#endif // CPPUNIT_HAS_SOURCE_LOCATION

/*! \brief Constructs a SourceLine object initialized with the location where the macro is expanded.
 * \ingroup CreatingNewAssertions
 * \relates CppUnit::SourceLine
 * Used to write your own assertion macros.
 * \see Asserter for example of usage.
 */
#if !CPPUNIT_HAS_SOURCE_LOCATION
# define CPPUNIT_SOURCELINE() CPPUNIT_NS::SourceLine( __FILE__, __LINE__ )
#else
  // std::source_location implementation below
#endif // !CPPUNIT_HAS_SOURCE_LOCATION


CPPUNIT_NS_BEGIN


/*! \brief (Implementation) Represents a source line location.
 * \ingroup CreatingNewAssertions
 * \ingroup BrowsingCollectedTestResult
 *
 * Used to capture the failure location in assertion.
 * 
 * Use the CPPUNIT_SOURCELINE() macro to construct that object. Typically used when
 * writing an assertion macro in association with Asserter.
 *
 * \see Asserter.
 */
class CPPUNIT_API SourceLine final
{
public:
  // TODO: Backward compatibility
  constexpr SourceLine() = default;
  constexpr SourceLine( const SourceLine &other ) = default;
  constexpr SourceLine( const char *fileName,
                        int lineNumber )
  : m_fileName( fileName )
  , m_lineNumber( lineNumber )
  {
  }

  SourceLine &operator =( const SourceLine &other );

  bool isValid() const;

  int lineNumber() const;

  std::string fileName() const;

  bool operator ==( const SourceLine &other ) const;

private:
  const char * m_fileName; // FIXME: std::string, requires constexpr std::string though
  int m_lineNumber = -1;
};


CPPUNIT_NS_END


#if CPPUNIT_HAS_SOURCE_LOCATION

consteval CPPUNIT_NS::SourceLine
CPPUNIT_SOURCELINE( const std::source_location &location = std::source_location::current() )
{
  return CPPUNIT_NS::SourceLine( location.file_name(),
                                 location.line() );
}

#endif // CPPUNIT_HAS_SOURCE_LOCATION


#endif  // CPPUNIT_SOURCELINE_H
