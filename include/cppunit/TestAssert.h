#ifndef CPPUNIT_TESTASSERT_H
#define CPPUNIT_TESTASSERT_H

#include <cppunit/Portability.h>
#include <cppunit/Exception.h>
#include <cppunit/Asserter.h>
#include <cppunit/extensions/TypeInfoHelper.h>
#include <cppunit/portability/FloatingPoint.h>
#include <cppunit/portability/Stream.h>
#include <cppunit/tools/StringHelper.h>
#if HAVE_CXX2A
# include <concepts>
#endif // HAVE_CXX2A
#include <stdio.h>
#include <float.h> // For struct assertion_traits<double>

// Work around "passing 'T' chooses 'int' over 'unsigned int'" warnings when T
// is an enum type:
#if defined __GNUC__ && (__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 6))
#pragma GCC system_header
#endif


CPPUNIT_NS_BEGIN


#if HAVE_CXX2A

namespace asserts
{


/*! \brief Used in asserts were less comparison is required
 */
template <typename T, typename U>
concept less_testable_with =
  requires (T t, U u)
  {
    { t < u };
  };


/*! \brief Used in asserts to deny their use with floating point numbers
 */
template <typename ExpectedT, typename ActualT>
concept deny_fp_args =
  !std::floating_point<ExpectedT>
  && !std::floating_point<ActualT>;


/*! \brief Used in asserts to check that both arguments
 * can be converted to floating point numbers (namely doubles)
 */
template <typename ExpectedT, typename ActualT, typename To>
concept args_convertible_to =
  std::convertible_to<ExpectedT, To>
  && std::convertible_to<ActualT, To>;


/*! \brief Used to overload asserts for floating point numbers comparison
 */
template <typename ExpectedT, typename ActualT>
concept fp_overload =
  // Allow mixing floating point numbers with integers for this overload.
  // This overload will fit if at least one of the arguments is floating point.
  ( std::floating_point<ExpectedT> || std::floating_point<ActualT> )
  // Both arguments should be convertible to double
  // since assert_traits<double> is going to be used on them
  && args_convertible_to<ExpectedT, ActualT, double>;


/*! \brief This is used to check that lambdas (and other executables)
 * passed to asserts fit expected interface
 */
template <typename T>
concept executable =
  // Executable should be a function that takes no arguments
  std::invocable<T>;


} // namespace asserts

#endif // HAVE_CXX2A


/*! \brief Compares two floating point numbers
 */
bool doublesEqual( double expected,
                   double actual,
                   double epsilon );

/*! \brief Traits used by CPPUNIT_ASSERT* macros.
 *
 * Here is an example of specialising these traits: 
 *
 * \code
 * template<>
 * struct assertion_traits<std::string>   // specialization for the std::string type
 * {
 *   static std::string toString( const std::string& x )
 *   {
 *     std::string text = '"' + x + '"';    // adds quote around the string to see whitespace
 *     OStringStream ost;
 *     ost << text;
 *     return ost.str();
 *   }
 * };
 * \endcode
 */
template <class T>
struct assertion_traits 
{  
    static std::string toString( const T& x )
    {
        return CPPUNIT_NS::StringHelper::toString(x);
    }
};

/*! \brief Traits used by CPPUNIT_ASSERT_DOUBLES_EQUAL(). 
 * 
 * This specialisation from @c struct @c assertion_traits<> ensures that 
 * doubles are converted in full, instead of being rounded to the default 
 * 6 digits of precision. Use the system defined ISO C99 macro DBL_DIG 
 * within float.h is available to define the maximum precision, otherwise
 * use the hard-coded maximum precision of 15.
 */
template <>
struct assertion_traits<double>
{  
    static std::string toString( double x )
    {
#ifdef DBL_DIG
       const int precision = DBL_DIG;
#else
       const int precision = 15;
#endif  // #ifdef DBL_DIG
       char buffer[128];
#ifdef __STDC_SECURE_LIB__ // Use secure version with visual studio 2005 to avoid warning.
       sprintf_s(buffer, sizeof(buffer), "%.*g", precision, x); 
#else	
       sprintf(buffer, "%.*g", precision, x); 
#endif
       return buffer;
    }
};


/*! \brief Message traits used by CPPUNIT_ASSERT* macros.
 *
 * Here is an example of specialising these traits:
 *
 * \code
 * CPPUNIT_NS_BEGIN
 * static std::string message_to_string(const MyType& m)
 * {
 *     return m.getStr();
 * };
 * CPPUNIT_NS_END
 * \endcode
 */
inline std::string message_to_string( const std::string& s )
{
    return s;
}
inline std::string message_to_string( const OStream& out )
{
    OStringStream ost;
    ost << out.rdbuf();
    return ost.str();
}
/// for calls to addDetail
inline AdditionalMessage message_to_string( const AdditionalMessage& msg )
{
    return msg;
}
/// otherwise calls with string literals are ambiguous
inline std::string message_to_string( const char * s )
{
    return s;
}

#if !HAVE_CXX2A
// The following is needed for transition from templates with one type
// argument to templates with two type arguments. I.e.
//
// Before:
//   template <T>
//   assert(T expected, T actual)
//
// After:
//   template <ExpectedT, ActualT>
//   assert(ExpectedT expected, ActualT actual)
//
// So asserts now can explicitly take expected and actual of different types,
// while before compiler would implicitly promote/convert types. For example,
// in a call to CPPUNIT_ASSERT_DOUBLES_EQUAL( float, float ), floats are going
// to be promoted to doubles.
//
// Since this relies on compiler doing types, perhaps the only way to *really*
// make sure old behavior is preserved is to keep relying on compiler.
//
// This define makes it so both before and after can share implementation
// (refer to ExpectedT and ActualT as they were there in both variants),
// but in "before" variant ActualT == ExpectedT.
# define ActualT ExpectedT
#endif // !HAVE_CXX2A

/*! \brief (Implementation) Asserts that two objects of the same type are equals.
 * Use CPPUNIT_ASSERT_EQUAL instead of this function.
 * \sa assertion_traits, Asserter::failNotEqual().
 */
#if !HAVE_CXX2A
template <typename ExpectedT>
#else
template <typename ExpectedT, typename ActualT>
  requires ( std::equality_comparable_with<ExpectedT, ActualT>
  && asserts::deny_fp_args<ExpectedT, ActualT> )
#endif // !HAVE_CXX2A
void assertEquals( const ExpectedT& expected,
                   const ActualT& actual,
                   const std::string& message,
                   const SourceLine& sourceLine )
{
  // This is the same thing that assertion_trats::equal() was doing
  // with the exception that assertion_trats::equal() was calling
  // operator==() on arguments of the same type, but template with two
  // type arguments can call operator== on arguments of different types.
  // In C++20 arguments of different types are intended, in C++11 it's
  // going to call operator==() on arguments of the same type because
  // ExpectedT == ActualT since template is still with one type argument.
  if ( !(expected == actual ) )
  {
    Asserter::failNotEqual( assertion_traits<ExpectedT>::toString( expected ),
                            assertion_traits<ActualT>::toString( actual ),
                            sourceLine,
                            message );
  }
}

/*! \brief (Implementation) Asserts that two double are equals given a tolerance.
 * Use CPPUNIT_ASSERT_DOUBLES_EQUAL instead of this function.
 * \sa Asserter::failNotEqual().
 * \sa CPPUNIT_ASSERT_DOUBLES_EQUAL for detailed semantic of the assertion.
 */
#if !HAVE_CXX2A
template <typename ExpectedT,
  typename std::enable_if<(
    // It was assertDoubleEquals(double, double, double) before. For backward
    // compatibility, restrict template type arguments to floating point numbers
    std::is_convertible<ExpectedT, double>::value
  ), int>::type = 0>
#else
template <typename ExpectedT, typename ActualT>
  requires ( asserts::args_convertible_to<ExpectedT, ActualT, double> )
#endif // !HAVE_CXX2A
void assertDoubleEquals( const ExpectedT& expected,
                         const ActualT& actual,
                         double epsilon,
                         const std::string& message,
                         const SourceLine& sourceLine )
{
  const bool equal = doublesEqual( expected, actual, epsilon );

  AdditionalMessage msg( "Delta   : " +
                         assertion_traits<double>::toString( epsilon ) );
  msg.addDetail( AdditionalMessage(message) );

  Asserter::failNotEqualIf( !equal,
                            assertion_traits<double>::toString( expected ),
                            assertion_traits<double>::toString( actual ),
                            sourceLine,
                            msg,
                            "double equality assertion failed" );
}

/*! \brief (Implementation) Asserts that an object is less than another one of the same type
 * Use CPPUNIT_ASSERT_LESS, CPPUNIT_ASSERT_GREATER instead of this function.
 * \sa assertion_traits, Asserter::failNotLess().
 */
#if !HAVE_CXX2A
template <typename ExpectedT>
#else
template <typename ExpectedT, typename ActualT>
    requires ( asserts::less_testable_with<ActualT, ExpectedT> )
#endif // !HAVE_CXX2A
void assertLess( const ExpectedT& expected,
                 const ActualT& actual,
                 const std::string& message,
                 const SourceLine& sourceLine )
{
    if ( !( actual < expected ) )
    {
        Asserter::failNotLess( assertion_traits<ExpectedT>::toString( expected ),
                               assertion_traits<ActualT>::toString( actual ),
                               sourceLine,
                               message );
    }
}


/*! \brief (Implementation) Asserts that an object is less than another one of the same type
 * Use CPPUNIT_ASSERT_LESS, CPPUNIT_ASSERT_GREATER instead of this function.
 * \sa assertion_traits, Asserter::failNotLess().
 */
#if !HAVE_CXX2A
template <typename ExpectedT>
#else
template <typename ExpectedT, typename ActualT>
    requires ( asserts::less_testable_with<ExpectedT, ActualT> )
#endif // !HAVE_CXX2A
void assertGreater( const ExpectedT& expected,
                    const ActualT& actual,
                    const std::string& message,
                    const SourceLine& sourceLine )
{
    if ( !( expected < actual ) )
    {
        Asserter::failNotGreater( assertion_traits<ExpectedT>::toString( expected ),
                                  assertion_traits<ActualT>::toString( actual ),
                               sourceLine,
                               message );
    }
}

/*! \brief (Implementation) Asserts that two objects of the same type are equals.
 * Use CPPUNIT_ASSERT_LESSEQUAL, CPPUNIT_ASSERT_GREATEREQUAL instead of this function.
 * \sa assertion_traits, Asserter::failNotLessEqual().
 */
#if !HAVE_CXX2A
template <typename ExpectedT>
#else
template <typename ExpectedT, typename ActualT>
    requires ( asserts::less_testable_with<ActualT, ExpectedT>
    && std::equality_comparable_with<ExpectedT, ActualT>
    // XXX: C++11 version accepts floats here, but this isn't entirely correct.
    // At very least for floating point numbers it should be overload
    // that accepts epsilon like assertEqual() and CPPUNIT_ASSERT_DOUBLES_EQUAL() do.
    // However that would require additional macros that are't there:
    //   CPPUNIT_ASSERT_DOUBLES_LESS_EQUAL( float, float, epsilon )
    //   CPPUNIT_ASSERT_DOUBLES_GREATER_EQUAL( float, float, epsilon )
    && asserts::deny_fp_args<ExpectedT, ActualT> )
#endif // !HAVE_CXX2A
void assertLessEqual( const ExpectedT& expected,
                      const ActualT& actual,
                      const std::string& message,
                      const SourceLine& sourceLine )
{
    if ( !( actual <= expected ) )
    {
        Asserter::failNotLessEqual( assertion_traits<ExpectedT>::toString( expected ),
                                    assertion_traits<ActualT>::toString( actual ),
                               sourceLine,
                               message );
    }
}

#if HAVE_CXX2A
/*! \brief (Implementation) Less or equal implementation for floating point numbers
 * \sa Asserter::failNotLessEqual().
 */
template <typename ExpectedT, typename ActualT>
    requires ( asserts::less_testable_with<ActualT, ExpectedT>
    && asserts::args_convertible_to<ExpectedT, ActualT, double> )
void assertDoubleLessEqual( const ExpectedT& expected,
                            const ActualT& actual,
                            double epsilon,
                            const std::string& message,
                            const SourceLine& sourceLine )
{
    AdditionalMessage msg( "Delta   : " +
                           assertion_traits<double>::toString( epsilon ) );
    msg.addDetail( AdditionalMessage(message) );

    if ( !( actual < expected )
    && !( doublesEqual( expected, actual, epsilon ) ) )
    {
        Asserter::failNotLessEqual( assertion_traits<double>::toString( expected ),
                                    assertion_traits<double>::toString( actual ),
                                    sourceLine,
                                    msg );
    }
}

// XXX: Since regular assertLessEqual() forbids operations on floating point numbers
// in C++20, one way to work this around without introducing new macros is to provide
// overload for doubles that perform correct comparison with some default epsilon.
inline double CPPUNIT_DEFAULT_EPSILON = std::numeric_limits<float>::min();

// XXX: So CPPUNIT_ASSERT_LESSEQUAL( double, double ) works and works correctly
// (in regard to epsilon), but there is no way to pass epsilon. If such way would exist,
// for instance, if CPPUNIT_ASSERT_DOUBLES_LESSEQUAL( expected, actual, epsilon ) existed,
// it should have been routed to assertDoubleLessEqual() directly and this overload
// is no longer needed.
/*! \brief (Implementation) Less or equal implementation for floating point numbers
 * \sa assertDoubleLessEqual, Asserter::failNotLessEqual().
 */
template <typename ExpectedT, typename ActualT>
    requires ( asserts::less_testable_with<ActualT, ExpectedT>
    && asserts::fp_overload<ExpectedT, ActualT> )
void assertLessEqual( const ExpectedT& expected,
                      const ActualT& actual,
                      const std::string& message,
                      const SourceLine& sourceLine )
{
    assertDoubleLessEqual( expected,
                           actual,
                           CPPUNIT_DEFAULT_EPSILON,
                           message,
                           sourceLine );
}
#endif // HAVE_CXX2A

/*! \brief (Implementation) Asserts that two objects of the same type are equals.
 * Use CPPUNIT_ASSERT_LESSEQUAL, CPPUNIT_ASSERT_GREATEREQUAL instead of this function.
 * \sa assertion_traits, Asserter::failNotLessEqual().
 */
#if !HAVE_CXX2A
template <typename ExpectedT>
#else
template <typename ExpectedT, typename ActualT>
    requires ( asserts::less_testable_with<ExpectedT, ActualT>
    && std::equality_comparable_with<ExpectedT, ActualT>
    // XXX: same constraints for floats as in assertLessEqual()
    && asserts::deny_fp_args<ExpectedT, ActualT> )
#endif // !HAVE_CXX2A
void assertGreaterEqual( const ExpectedT& expected,
                         const ActualT& actual,
                         const std::string& message,
                         const SourceLine& sourceLine )
{
    if ( !( expected <= actual ) )
    {
        Asserter::failNotGreaterEqual( assertion_traits<ExpectedT>::toString( expected ),
                                       assertion_traits<ActualT>::toString( actual ),
                               sourceLine,
                               message );
    }
}

#if HAVE_CXX2A
/*! \brief (Implementation) Greater or equal implementation for floating point numbers
 * \sa Asserter::failNotGreaterEqual().
 */
template <typename ExpectedT, typename ActualT>
    requires ( asserts::less_testable_with<ExpectedT, ActualT>
    && asserts::args_convertible_to<ExpectedT, ActualT, double> )
void assertDoubleGreaterEqual( const ExpectedT& expected,
                               const ActualT& actual,
                               double epsilon,
                               const std::string& message,
                               const SourceLine& sourceLine )
{
    AdditionalMessage msg( "Delta   : " +
                           assertion_traits<double>::toString( epsilon ) );
    msg.addDetail( AdditionalMessage(message) );

    if ( !( expected < actual )
    && !( doublesEqual( expected, actual, epsilon ) ) )
    {
        Asserter::failNotGreaterEqual( assertion_traits<double>::toString( expected ),
                                       assertion_traits<double>::toString( actual ),
                                       sourceLine,
                                       msg );
    }
}

// XXX: Same rationale as for assertLessEqual() overload for floating point numbers
/*! \brief (Implementation) Greater or equal implementation for floating point numbers
 * \sa assertDoubleGreaterEqual, Asserter::failNotGreaterEqual().
 */
template <typename ExpectedT, typename ActualT>
    requires ( asserts::less_testable_with<ActualT, ExpectedT>
    // XXX: same constraints for floats as in assertLessEqual()
    && asserts::fp_overload<ExpectedT, ActualT> )
void assertGreaterEqual( const ExpectedT& expected,
                         const ActualT& actual,
                         const std::string& message,
                         const SourceLine& sourceLine )
{
    assertDoubleGreaterEqual( expected,
                              actual,
                              CPPUNIT_DEFAULT_EPSILON,
                              message,
                              sourceLine );
}
#endif // HAVE_CXX2A

// Discard stuff used for transition from templates with one type argument
// two templates with two type arguments
#if !HAVE_CXX2A
# undef ActualT
#endif // !HAVE_CXX2A

/* A set of macros which allow us to get the line number
 * and file name at the point of an error.
 * Just goes to show that preprocessors do have some
 * redeeming qualities.
 */
#if CPPUNIT_HAVE_CPP_SOURCE_ANNOTATION
/** Assertions that a condition is \c true.
 * \ingroup Assertions
 */
#define CPPUNIT_ASSERT(condition)                                                 \
  ( CPPUNIT_NS::Asserter::failIf( !(condition),                                   \
                                 CPPUNIT_NS::Message( "assertion failed",         \
                                                      "Expression: " #condition), \
                                 CPPUNIT_SOURCELINE() ) )
#else
#define CPPUNIT_ASSERT(condition)                                            \
  ( CPPUNIT_NS::Asserter::failIf( !(condition),                              \
                                  CPPUNIT_NS::Message( "assertion failed" ), \
                                  CPPUNIT_SOURCELINE() ) )
#endif

/** Assertion with a user specified message.
 * \ingroup Assertions
 * \param message Message reported in diagnostic if \a condition evaluates
 *                to \c false.
 * \param condition If this condition evaluates to \c false then the
 *                  test failed.
 */
#define CPPUNIT_ASSERT_MESSAGE(message,condition)                          \
  ( CPPUNIT_NS::Asserter::failIf( !(condition),                            \
                                  CPPUNIT_NS::Message( "assertion failed", \
                                                       "Expression: "      \
                                                       #condition,         \
                                                       CPPUNIT_NS::message_to_string(message) ),          \
                                  CPPUNIT_SOURCELINE() ) )

/** Fails with the specified message.
 * \ingroup Assertions
 * \param message Message reported in diagnostic.
 */
#define CPPUNIT_FAIL( message )                                         \
  ( CPPUNIT_NS::Asserter::fail( CPPUNIT_NS::Message( "forced failure",  \
                                                     CPPUNIT_NS::message_to_string(message) ),         \
                                CPPUNIT_SOURCELINE() ) )

// FIXME: Well, i'd add message param to the right place,
// but this looks like it was broken for some time.
// ~~assertEquals() expected 4 arguments, not 3.~~
// assertEquals() expects arguments of different type, i guess.
// Remove maybe? It is unlikely that someone is using something
// that shouldn't work question mark?
#ifdef CPPUNIT_ENABLE_SOURCELINE_DEPRECATED
/// Generalized macro for primitive value comparisons
#define CPPUNIT_ASSERT_EQUAL(expected,actual)                     \
  ( CPPUNIT_NS::assertEquals( (expected),             \
                              (actual),               \
                              __LINE__, __FILE__ ) )
#else
/** Asserts that two values are equals.
 * \ingroup Assertions
 *
 * Equality and string representation can be defined with
 * an appropriate CppUnit::assertion_traits class.
 *
 * A diagnostic is printed if actual and expected values disagree.
 *
 * Requirement for \a expected and \a actual parameters:
 * - They are exactly of the same type
 * - They are serializable into a std::strstream using operator <<.
 * - They can be compared using operator ==. 
 *
 * The last two requirements (serialization and comparison) can be
 * removed by specializing the CppUnit::assertion_traits.
 */
#define CPPUNIT_ASSERT_EQUAL(expected,actual)          \
  ( CPPUNIT_NS::assertEquals( (expected),              \
                              (actual),                \
                              "",                      \
                              CPPUNIT_SOURCELINE() ) )

/** Asserts that two values are equals, provides additional message on failure.
 * \ingroup Assertions
 *
 * Equality and string representation can be defined with
 * an appropriate assertion_traits class.
 *
 * A diagnostic is printed if actual and expected values disagree.
 * The message is printed in addition to the expected and actual value
 * to provide additional information.
 *
 * Requirement for \a expected and \a actual parameters:
 * - They are exactly of the same type
 * - They are serializable into a std::strstream using operator <<.
 * - They can be compared using operator ==. 
 *
 * The last two requirements (serialization and comparison) can be
 * removed by specializing the CppUnit::assertion_traits.
 */
#define CPPUNIT_ASSERT_EQUAL_MESSAGE(message,expected,actual)      \
  ( CPPUNIT_NS::assertEquals( (expected),              \
                              (actual),                \
                              CPPUNIT_NS::message_to_string(message), \
                              CPPUNIT_SOURCELINE() ) )
#endif

/** Asserts that actual is less than expected, provides additional message on failure.
 * \ingroup Assertions
 *
 * Less and string representation can be defined with
 * an appropriate assertion_traits class.
 *
 * A diagnostic is printed if actual is less than expected.
 * The message is printed in addition to the expected and actual value
 * to provide additional information.
 *
 * Requirement for \a expected and \a actual parameters:
 * - They are exactly of the same type
 * - They are serializable into a std::strstream using operator <<.
 * - They can be compared using operator <. 
 *
 * The last two requirements (serialization and comparison) can be
 * removed by specializing the CppUnit::assertion_traits.
 *
 * \sa CPPUNIT_ASSERT_GREATER
 */
#define CPPUNIT_ASSERT_LESS(expected, actual)          \
    ( CPPUNIT_NS::assertLess( (expected),              \
                              (actual),                \
                              "",                      \
                              CPPUNIT_SOURCELINE() ) )

/** Asserts that actual is greater than expected, provides additional message on failure.
 * \ingroup Assertions
 *
 * String representation can be defined with
 * an appropriate assertion_traits class. For comparison assertLess is used.
 *
 * A diagnostic is printed if actual is less than expected.
 * The message is printed in addition to the expected and actual value
 * to provide additional information.
 *
 * Requirement for \a expected and \a actual parameters:
 * - They are exactly of the same type
 * - They are serializable into a std::strstream using operator <<.
 * - They can be compared using operator<. 
 *
 * The last two requirements (serialization and comparison) can be
 * removed by specializing the CppUnit::assertion_traits.
 *
 * \sa CPPUNIT_ASSERT_LESS
 */
#define CPPUNIT_ASSERT_GREATER(expected, actual)       \
    ( CPPUNIT_NS::assertGreater( (expected),           \
                                 (actual),             \
                                 "",                   \
                                 CPPUNIT_SOURCELINE() ) )

/** Asserts that actual is less or equal than expected, provides additional message on failure.
 * \ingroup Assertions
 *
 * LessEqual and string representation can be defined with
 * an appropriate assertion_traits class.
 *
 * A diagnostic is printed if actual is greater than expected.
 * The message is printed in addition to the expected and actual value
 * to provide additional information.
 *
 * Requirement for \a expected and \a actual parameters:
 * - They are exactly of the same type
 * - They are serializable into a std::strstream using operator <<.
 * - They can be compared using operator <=. 
 *
 * The last two requirements (serialization and comparison) can be
 * removed by specializing the CppUnit::assertion_traits.
 *
 * \sa CPPUNIT_ASSERT_GREATEREQUAL
 */
#define CPPUNIT_ASSERT_LESSEQUAL(expected, actual)               \
    ( CPPUNIT_NS::assertLessEqual( (expected),              \
                                   (actual),                \
                                   "",                      \
                                   CPPUNIT_SOURCELINE() ) )

/** Asserts that actual is greater than expected, provides additional message on failure.
 * \ingroup Assertions
 *
 * String representation can be defined with
 * an appropriate assertion_traits class. For comparison assertLess is used.
 *
 * A diagnostic is printed if actual is less than expected.
 * The message is printed in addition to the expected and actual value
 * to provide additional information.
 *
 * Requirement for \a expected and \a actual parameters:
 * - They are exactly of the same type
 * - They are serializable into a std::strstream using operator <<.
 * - They can be compared using operator<=. 
 *
 * The last two requirements (serialization and comparison) can be
 * removed by specializing the CppUnit::assertion_traits.
 *
 * \sa CPPUNIT_ASSERT_LESSEQUAL
 */
#define CPPUNIT_ASSERT_GREATEREQUAL(expected, actual)            \
    ( CPPUNIT_NS::assertGreaterEqual( (expected),                \
                                      (actual),              \
                                      "",                        \
                                      CPPUNIT_SOURCELINE() ) )
/*! \brief Macro for primitive double value comparisons. 
 * \ingroup Assertions
 *
 * The assertion pass if both expected and actual are finite and
 * \c fabs( \c expected - \c actual ) <= \c delta.
 * If either \c expected or actual are infinite (+/- inf), the 
 * assertion pass if \c expected == \c actual.
 * If either \c expected or \c actual is a NaN (not a number), then
 * the assertion fails.
 */
#define CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,actual,delta)        \
  ( CPPUNIT_NS::assertDoubleEquals( (expected),            \
                                    (actual),              \
                                    (delta),               \
                                    "",                    \
                                    CPPUNIT_SOURCELINE() ) )


/*! \brief Macro for primitive double value comparisons, setting a 
 * user-supplied message in case of failure. 
 * \ingroup Assertions
 * \sa CPPUNIT_ASSERT_DOUBLES_EQUAL for detailed semantic of the assertion.
 */
#define CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(message,expected,actual,delta)  \
  ( CPPUNIT_NS::assertDoubleEquals( (expected),            \
                                    (actual),              \
                                    (delta),               \
                                    CPPUNIT_NS::message_to_string(message),  \
                                    CPPUNIT_SOURCELINE() ) )


/** Asserts that the given expression throws an exception of the specified type. 
 * \ingroup Assertions
 * Example of usage:
 * \code
 *   std::vector<int> v;
 *  CPPUNIT_ASSERT_THROW( v.at( 50 ), std::out_of_range );
 * \endcode
 */
# define CPPUNIT_ASSERT_THROW( expression, ExceptionType )              \
   CPPUNIT_ASSERT_THROW_MESSAGE( "",                                    \
                                 expression,                            \
                                 ExceptionType )


// implementation detail
#if defined(CPPUNIT_USE_TYPEINFO_NAME)
#define CPPUNIT_EXTRACT_EXCEPTION_TYPE_( exception, no_rtti_message ) \
   CPPUNIT_NS::TypeInfoHelper::getClassName( typeid(exception) )
#else
#define CPPUNIT_EXTRACT_EXCEPTION_TYPE_( exception, no_rtti_message ) \
   std::string( no_rtti_message )
#endif // CPPUNIT_USE_TYPEINFO_NAME

// implementation detail
#define CPPUNIT_GET_PARAMETER_STRING( parameter ) #parameter

// XXX: Macro was expecting AdditionalMessage instead of std::string
// this might backfire
/** \brief (Implementation) Asserts that executable is throwing exception
 * of expected type. Use CPPUNIT_ASSERT_THROW instead of this function.
 */
template <typename ExceptionT,
          typename Executable>
#if HAVE_CXX2A
   requires ( asserts::executable<Executable> )
#endif // HAVE_CXX2A
void assertThrows( Executable executable,
                   const std::string& message,
                   const SourceLine& sourceLine )
{
   bool cpputCorrectExceptionThrown_ = false;
   CPPUNIT_NS::Message cpputMsg_( "expected exception thrown" );
   cpputMsg_.addDetail( AdditionalMessage(message) );
   cpputMsg_.addDetail( "Expected: " +
                        CPPUNIT_NS::TypeInfoHelper::getClassName( typeid(ExceptionT) ) );

   try
   {
      executable();
   }
   catch ( const ExceptionT & )
   {
      cpputCorrectExceptionThrown_ = true;
   }
   catch ( const std::exception &e )
   {
      cpputMsg_.addDetail( "Actual  : " +
                           CPPUNIT_NS::TypeInfoHelper::getClassName( typeid(e) ) );
      cpputMsg_.addDetail( std::string("What()  : ") + e.what() );
   }
   catch ( ... )
   {
      cpputMsg_.addDetail( "Actual  : unknown." );
   }

   if ( !cpputCorrectExceptionThrown_ )
   {
      CPPUNIT_NS::Asserter::fail( cpputMsg_,
                                  sourceLine );
   }
}


// XXX: Note that this exploits the fact that [&] captures this pointer,
// so lambda that wraps up `expression` can be called from fixture's method.
/** Asserts that the given expression throws an exception of the specified type, 
 * setting a user supplied message in case of failure. 
 * \ingroup Assertions
 * Example of usage:
 * \code
 *   std::vector<int> v;
 *  CPPUNIT_ASSERT_THROW_MESSAGE( "- std::vector<int> v;", v.at( 50 ), std::out_of_range );
 * \endcode
 */
# define CPPUNIT_ASSERT_THROW_MESSAGE( message, expression, ExceptionType )   \
   CPPUNIT_NS::assertThrows<ExceptionType>( [&] { expression; },              \
                                            message,                          \
                                            CPPUNIT_SOURCELINE() )


/** Asserts that the given expression does not throw any exceptions.
 * \ingroup Assertions
 * Example of usage:
 * \code
 *   std::vector<int> v;
 *   v.push_back( 10 );
 *  CPPUNIT_ASSERT_NO_THROW( v.at( 0 ) );
 * \endcode
 */
# define CPPUNIT_ASSERT_NO_THROW( expression )                             \
   CPPUNIT_ASSERT_NO_THROW_MESSAGE( "",                                    \
                                    expression )


// XXX: Macro was expecting AdditionalMessage instead of std::string
// this might backfire
/** \brief (Implementation) Asserts that executable does not throw exception.
 * Use CPPUNIT_ASSERT_NO_THROW instead of this function.
 */
template <typename Executable>
#if HAVE_CXX2A
   requires ( asserts::executable<Executable> )
#endif // HAVE_CXX2A
void assertDoesNotThrow( Executable executable,
                         const std::string& message,
                         const SourceLine& sourceLine )
{
   CPPUNIT_NS::Message cpputMsg_( "unexpected exception caught" );
   cpputMsg_.addDetail( AdditionalMessage(message) );

   try
   {
      executable();
   }
   catch ( const std::exception &e )
   {
      cpputMsg_.addDetail( "Caught: " +
                           CPPUNIT_NS::TypeInfoHelper::getClassName( typeid(e) ) );
      cpputMsg_.addDetail( std::string("What(): ") + e.what() );
      CPPUNIT_NS::Asserter::fail( cpputMsg_,
                                  sourceLine );
   }
   catch ( ... )
   {
      cpputMsg_.addDetail( "Caught: unknown." );
      CPPUNIT_NS::Asserter::fail( cpputMsg_,
                                  sourceLine );
   }
}


/** Asserts that the given expression does not throw any exceptions, 
 * setting a user supplied message in case of failure. 
 * \ingroup Assertions
 * Example of usage:
 * \code
 *   std::vector<int> v;
 *   v.push_back( 10 );
 *  CPPUNIT_ASSERT_NO_THROW( "std::vector<int> v;", v.at( 0 ) );
 * \endcode
 */
# define CPPUNIT_ASSERT_NO_THROW_MESSAGE( message, expression )               \
   CPPUNIT_NS::assertDoesNotThrow( [&] { expression; },                       \
                                   message,                                   \
                                   CPPUNIT_SOURCELINE() )


/** Asserts that an assertion fail.
 * \ingroup Assertions
 * Use to test assertions.
 * Example of usage:
 * \code
 *   CPPUNIT_ASSERT_ASSERTION_FAIL( CPPUNIT_ASSERT( 1 == 2 ) );
 * \endcode
 */
# define CPPUNIT_ASSERT_ASSERTION_FAIL( assertion )                 \
   CPPUNIT_ASSERT_THROW( assertion, CPPUNIT_NS::Exception )


/** Asserts that an assertion fail, with a user-supplied message in 
 * case of error.
 * \ingroup Assertions
 * Use to test assertions.
 * Example of usage:
 * \code
 *   CPPUNIT_ASSERT_ASSERTION_FAIL_MESSAGE( "1 == 2", CPPUNIT_ASSERT( 1 == 2 ) );
 * \endcode
 */
# define CPPUNIT_ASSERT_ASSERTION_FAIL_MESSAGE( message, assertion )    \
   CPPUNIT_ASSERT_THROW_MESSAGE( message, assertion, CPPUNIT_NS::Exception )


/** Asserts that an assertion pass.
 * \ingroup Assertions
 * Use to test assertions.
 * Example of usage:
 * \code
 *   CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT( 1 == 1 ) );
 * \endcode
 */
# define CPPUNIT_ASSERT_ASSERTION_PASS( assertion )                 \
   CPPUNIT_ASSERT_NO_THROW( assertion )


/** Asserts that an assertion pass, with a user-supplied message in 
 * case of failure. 
 * \ingroup Assertions
 * Use to test assertions.
 * Example of usage:
 * \code
 *   CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE( "1 != 1", CPPUNIT_ASSERT( 1 == 1 ) );
 * \endcode
 */
# define CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE( message, assertion )    \
   CPPUNIT_ASSERT_NO_THROW_MESSAGE( message, assertion )




// Backwards compatibility

#if CPPUNIT_ENABLE_NAKED_ASSERT

#undef assert
#define assert(c)                 CPPUNIT_ASSERT(c)
#define assertEqual(e,a)          CPPUNIT_ASSERT_EQUAL(e,a)
#define assertDoublesEqual(e,a,d) CPPUNIT_ASSERT_DOUBLES_EQUAL(e,a,d)
#define assertLongsEqual(e,a)     CPPUNIT_ASSERT_EQUAL(e,a)

#endif


CPPUNIT_NS_END

#endif  // CPPUNIT_TESTASSERT_H
