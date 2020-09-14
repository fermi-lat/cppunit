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
# define CPPUNIT_SOURCELINE() CPPUNIT_NS::SourceLine::current( __FILE__, __LINE__ )
#else
# define CPPUNIT_SOURCELINE() CPPUNIT_NS::SourceLine::current()
#endif // !CPPUNIT_HAS_SOURCE_LOCATION


CPPUNIT_NS_BEGIN


/*! \brief Represents a source line location.
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
class CPPUNIT_API SourceLine
{
public:
  SourceLine();

  // Ensure thread-safe copy by detaching the string buffer.
  SourceLine( const SourceLine &other );

  SourceLine( const std::string &fileName,
              int lineNumber );

  SourceLine &operator =( const SourceLine &other );

  /// Destructor.
  virtual ~SourceLine();

  bool isValid() const;

  int lineNumber() const;

  std::string fileName() const;

#if !CPPUNIT_HAS_SOURCE_LOCATION
  static SourceLine current( const std::string &fileName = "",
                             int lineNumber = -1 );
#else
  static SourceLine current( const std::string &fileName 
                               = std::source_location::current().file_name(),
                             int lineNumber
                               = std::source_location::current().line() );
#endif // !CPPUNIT_HAS_SOURCE_LOCATION

  bool operator ==( const SourceLine &other ) const;
  bool operator !=( const SourceLine &other ) const;

private:
  std::string m_fileName;
  int m_lineNumber;
};


CPPUNIT_NS_END

#endif  // CPPUNIT_SOURCELINE_H
