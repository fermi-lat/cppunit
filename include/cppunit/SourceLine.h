#ifndef CPPUNIT_SOURCELINE_H
#define CPPUNIT_SOURCELINE_H

#if __cplusplus >= 201709L
#  define HAVE_CXX2A 1
#endif // __cplusplus >= 201709L

#include <cppunit/Portability.h>
#include <string>
#if HAVE_CXX2A
#include <experimental/source_location>
#endif // HAVE_CXX2A

/*! \brief Constructs a SourceLine object initialized with the location where the macro is expanded.
 * \ingroup CreatingNewAssertions
 * \relates CppUnit::SourceLine
 * Used to write your own assertion macros.
 * \see Asserter for example of usage.
 */
#define CPPUNIT_SOURCELINE() CPPUNIT_NS::SourceLine( __FILE__, __LINE__ )


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
#if HAVE_CXX2A
  // For compatibility reasons captureLine is false (opt-out) by default
  // and then enabled where appropriate (opt-in)
  explicit SourceLine( bool captureLine = false, 
                       const std::experimental::source_location &location
                         = std::experimental::source_location::current() );
#else
  // Pre-c++20 compatibily is same as calling old default constructor
  // captureLine is just ignored and file/line is initialized into invalid state
  SourceLine( bool captureLine = false );
#endif // HAVE_CXX2A

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

  bool operator ==( const SourceLine &other ) const;
  bool operator !=( const SourceLine &other ) const;

private:
  std::string m_fileName;
  int m_lineNumber;
};


CPPUNIT_NS_END

#endif  // CPPUNIT_SOURCELINE_H
