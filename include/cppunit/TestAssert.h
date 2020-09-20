#ifndef CPPUNIT_TESTASSERT_H
#define CPPUNIT_TESTASSERT_H

#include <cppunit/Portability.h>
#include <cppunit/Exception.h>
#include <cppunit/Asserter.h>
#include <cppunit/extensions/TypeInfoHelper.h>
#include <cppunit/portability/FloatingPoint.h>
#include <cppunit/portability/Stream.h>
#include <cppunit/tools/StringHelper.h>
#include <stdio.h>
#include <float.h> // For struct assertion_traits<double>
#if CPPUNIT_HAS_CONCEPTS
# include <concepts>
# include <limits>
# include <optional>
#endif // CPPUNIT_HAS_CONCEPTS

// Work around "passing 'T' chooses 'int' over 'unsigned int'" warnings when T
// is an enum type:
#if defined __GNUC__ && (__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 6))
#pragma GCC system_header
#endif


CPPUNIT_NS_BEGIN


namespace detail
{

/*! \brief (Implementation) Compares two floating point numbers.
 */
bool doublesEqual( double expected,
                   double actual,
                   double epsilon );

} // namespace detail


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


#if CPPUNIT_HAS_CONCEPTS

namespace detail
{


/*! \brief Used in asserts were less comparison is required.
 */
template <typename T, typename U>
concept less_comparable_with =
  requires (T t, U u)
  {
    { t < u };
  };

static_assert( detail::less_comparable_with<const char*, const void*> );

/*! \brief Used in asserts were less or equal comparison is required.
 *
 * The point to have dedicated concept for <= instead of combining
 * checks for < and == is that assert is going to call <= specifically
 * and this is what this concept is checking.
 */
template <typename T, typename U>
concept less_equal_comparable_with =
  requires (T t, U u)
  {
    { t <= u };
  };

static_assert( detail::less_equal_comparable_with<int, long> );


/*! \brief Used to check that assert argument can be serialized to
 * string (for error reporting).
 */
template <typename T>
concept assertion_traits_serializable =
  requires (T t)
  {
    { assertion_traits<T>::toString(t) };
  };

static_assert( detail::assertion_traits_serializable<int> );
static_assert( detail::assertion_traits_serializable<std::string> );


/*! \brief Used in asserts to deny their use with floating point numbers.
 * Both arguments has to be anything but floating point numbers for a
 * function with this contrain to accept such input.
 */
template <typename ExpectedT, typename ActualT>
concept not_floating_points =
  !std::floating_point<ExpectedT>
  && !std::floating_point<ActualT>;

static_assert( detail::not_floating_points<int, long> );
static_assert( !detail::not_floating_points<float, double> );


/*! \brief Used to overload asserts for floating point numbers comparison.
 * Accepts arguments if either of them is floating point number.
 */
template <typename ExpectedT, typename ActualT>
concept relaxed_floating_points =
  // Allow mixing floating point numbers with integers for this overload.
  // This overload will fit if at least one of the arguments is floating point.
  ( std::floating_point<ExpectedT> || std::floating_point<ActualT> )
  // Both arguments should be convertible to double
  // since assert_traits<double> is going to be used on them
  && std::convertible_to<ExpectedT, double>
  && std::convertible_to<ActualT, double>;

static_assert( !detail::relaxed_floating_points<int, long> );
static_assert( detail::relaxed_floating_points<int, float> );
static_assert( detail::relaxed_floating_points<double, long> );
static_assert( detail::relaxed_floating_points<double, float> );
static_assert( detail::relaxed_floating_points<double, double> );


/*! \brief Pointer-like String literals that can be null,
 * e.g. const char *.
 */
template <typename T>
concept nullable_string_literal =
  std::convertible_to<T, std::string>
  && std::is_pointer_v<std::remove_reference_t<T>>;

static_assert( detail::nullable_string_literal<const char*> );


/*! \brief Array-like strings literals that can't be null,
 * e.g. const char [4].
 */
template <typename T>
concept nonnull_string_literal =
  std::convertible_to<T, std::string>
  && std::is_array_v<std::remove_reference_t<T>>;

static_assert( detail::nonnull_string_literal<decltype("abc")> );


/*! \brief String literals, nullable or not.
 */
template <typename T>
concept string_literal =
  detail::nullable_string_literal<T>
  || detail::nonnull_string_literal<T>;

static_assert( detail::string_literal<const char*> );
static_assert( detail::string_literal<decltype("abc")> );


/*! \brief Used in asserts to deny their use with string literals.
 */
template <typename ExpectedT, typename ActualT>
concept not_string_literals =
  !detail::string_literal<ExpectedT>
  && !detail::string_literal<ActualT>;

static_assert( detail::not_string_literals<int, long> );
static_assert( detail::not_string_literals<float, double> );
// Note that if both arguments are std::string, they do not fit string
// literals concept and compared using regular assertEqual()
// implementation by operator ==.
static_assert( detail::not_string_literals<std::string, std::string> );
// Nulls are also compared using regular assertEquals() and operator==.
static_assert( detail::not_string_literals<nullptr_t, nullptr_t> );


/*! \brief Used in asserts to overload implementation for use with
 * string literals.
 */
template <typename ExpectedT, typename ActualT>
concept relaxed_string_literals =
  ( detail::string_literal<ExpectedT> || detail::string_literal<ActualT> )
  && std::convertible_to<ExpectedT, std::string>
  && std::convertible_to<ActualT, std::string>;

static_assert( !detail::relaxed_string_literals<std::string, std::string> );
static_assert( !detail::relaxed_string_literals<nullptr_t, nullptr_t> );
// If any argument is literal, both arguments should fit concept
static_assert( detail::relaxed_string_literals<decltype("abc"), const char*> );
static_assert( detail::relaxed_string_literals<decltype("abc"), std::string> );
static_assert( detail::relaxed_string_literals<std::string, const char*> );
static_assert( detail::relaxed_string_literals<const char*, nullptr_t> );


/*! \brief This is used to check that lambdas (and other executables)
 * passed to asserts fit expected interface.
 */
template <typename T>
concept executable =
  // Executable should be a function that takes no arguments
  std::invocable<T>;

static_assert( detail::executable<decltype([] {})> );
static_assert( detail::executable<void (*) ()> );


/*! \brief Check if type can be tested for true/false as if it was
 * explicitly converted to bool.
 *
 * For instance, std::unique_ptr can be explicitly converted to bool,
 * but can not be implicitly converted to bool.
 * This explicit convertion is for backward compatibility when
 * std::unique_ptr or other expression could be converted to bool by
 * compiler when assert macro was expanded into a function call that
 * explicitly accepted bool argument.
 *
 * Note that nullptr requires special handling to be converted to bool
 * and doesn't fit this concept.
 */
template <typename T>
concept explicitly_boolean_testable =
  requires (T t)
  {
    { !!t } -> std::convertible_to<bool>; // !! - explicit conversion
  };

static_assert( detail::explicitly_boolean_testable<bool> );
static_assert( detail::explicitly_boolean_testable<const char*> );
static_assert( detail::explicitly_boolean_testable<std::optional<int>> );
static_assert( !detail::explicitly_boolean_testable<nullptr_t> );


/*! \brief Used to pass something returning bool to assert function.
 */
template <typename T>
concept boolean_supplier =
  detail::executable<T>
  // Invoke result can be explicitly converted to bool
  && detail::explicitly_boolean_testable<std::invoke_result_t<T>>;

static_assert( detail::boolean_supplier<decltype([] { return true; })> );
static_assert( detail::boolean_supplier<bool (*) ()> );
static_assert( detail::boolean_supplier<const char* (*) ()> );


} // namespace detail

#endif // CPPUNIT_HAS_CONCEPTS


namespace detail
{

// XXX: Base asserts.

// This is a more generalized version of assertTrue( bool ).
// The extra step with boolean supplier is needed to invoke boolean
// expression inside assert (as opposed to outside of assert).
// For example, imagine if assertTrue( bool condition ) is used like so:
//
//   assertTrue( f() )
//
// In this form f() is going to be invoked before assertTrue() and
// its result is going to be passed to assertTrue() and then asserted.
// However if f() throws, assertTrue() is not going to be called at all.
// Form like this:
//
//   assertTrue( [] { return f(); } )
//
// Is going to invoke f() inside assert function and exception can be
// handled if needed. Or potentially handled in another assert in case
// of nested asserts. More realistic example: CPPUNIT_ASSERT( x == y )
// where == is a call to operator==() and this is going to be rewritten
// similarly to:
//
//   assertTrue( [&] { return !!( x == y ); } )
//
// Note explicit conversion to bool using !!.
/*! \brief (Implementation) Asserts that condition is true.
 * Use CPPUNIT_ASSERT or CPPUNIT_ASSERT_MESSAGES instead of this function.
 * \sa Asserter::failIf(), CPPUNIT_ASSERT, CPPUNIT_ASSERT_MESSAGE.
 */
template <typename Supplier>
#if CPPUNIT_HAS_CONCEPTS
  requires ( detail::boolean_supplier<Supplier> )
#endif // CPPUNIT_HAS_CONCEPTS
void assertTrue( const Supplier& condition,
                 const Message& message,
                 const SourceLine& sourceLine )
{
  const bool value = ( !!condition() );
  CPPUNIT_NS::Asserter::failIf( !value,
                                message,
                                sourceLine );
}


#if CPPUNIT_HAS_CONCEPTS

/*! \brief Overload for nulls for backward compatibility.
 * Basically for CPPUNIT_ASSERT( nullptr ) to work.
 */
template <typename Supplier>
  requires ( detail::executable<Supplier>
  && std::same_as<std::invoke_result_t<Supplier>, nullptr_t> )
void assertTrue( const Supplier&, const Message& m, const SourceLine& sl ) {
  assertTrue( [] () constexpr { return false; }, m, sl );
}

#endif // CPPUNIT_HAS_CONCEPTS


#if !CPPUNIT_HAS_CONCEPTS
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
#endif // !CPPUNIT_HAS_CONCEPTS


/*! \brief (Implementation) Asserts that two objects of the same type are equals.
 * Use CPPUNIT_ASSERT_EQUAL instead of this function.
 * \sa assertion_traits, Asserter::failNotEqual().
 * \sa CPPUNIT_ASSERT_EQUAL, CPPUNIT_ASSERT_EQUAL_MESSAGE.
 */
#if !CPPUNIT_HAS_CONCEPTS
template <typename ExpectedT>
#else
template <typename ExpectedT, typename ActualT>
  requires ( std::equality_comparable_with<ExpectedT, ActualT>
  && detail::assertion_traits_serializable<ExpectedT>
  && detail::assertion_traits_serializable<ActualT>
  // Floats require special handling
  && detail::not_floating_points<ExpectedT, ActualT>
  // String literals require special handling
  && detail::not_string_literals<ExpectedT, ActualT> )
#endif // !CPPUNIT_HAS_CONCEPTS
void assertEquals( const ExpectedT& expected,
                   const ActualT& actual,
                   const AdditionalMessage& message,
                   const SourceLine& sourceLine )
{
  // This is the same thing that assertion_trats::equal() was doing
  // with the exception that assertion_trats::equal() was calling
  // operator==() on arguments of the same type, but template with two
  // type arguments can call operator== on arguments of different types.
  // In C++20 arguments of different types are intended, in C++11 it's
  // supposed to call operator==() on arguments of the same type because
  // ExpectedT == ActualT since C++11 template is still with one type argument.
  if ( !(expected == actual ) )
  {
    Asserter::failNotEqual( assertion_traits<ExpectedT>::toString( expected ),
                            assertion_traits<ActualT>::toString( actual ),
                            sourceLine,
                            message );
  }
}


/*! \brief (Implementation) Asserts that an object is less than another one of the same type
 * Use CPPUNIT_ASSERT_LESS, CPPUNIT_ASSERT_GREATER instead of this function.
 * \sa assertion_traits, Asserter::failNotLess().
 */
#if !CPPUNIT_HAS_CONCEPTS
template <typename ExpectedT>
#else
template <typename ExpectedT, typename ActualT>
  requires ( detail::less_comparable_with<ActualT, ExpectedT>
  && detail::assertion_traits_serializable<ExpectedT>
  && detail::assertion_traits_serializable<ActualT>
  && detail::not_string_literals<ExpectedT, ActualT> )
#endif // !CPPUNIT_HAS_CONCEPTS
void assertLess( const ExpectedT& expected,
                 const ActualT& actual,
                 const AdditionalMessage& message,
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
#if !CPPUNIT_HAS_CONCEPTS
template <typename ExpectedT>
#else
template <typename ExpectedT, typename ActualT>
  requires ( detail::less_comparable_with<ExpectedT, ActualT>
  && detail::assertion_traits_serializable<ExpectedT>
  && detail::assertion_traits_serializable<ActualT>
  && detail::not_string_literals<ExpectedT, ActualT> )
#endif // !CPPUNIT_HAS_CONCEPTS
void assertGreater( const ExpectedT& expected,
                    const ActualT& actual,
                    const AdditionalMessage& message,
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
#if !CPPUNIT_HAS_CONCEPTS
template <typename ExpectedT>
#else
template <typename ExpectedT, typename ActualT>
  requires ( detail::less_equal_comparable_with<ActualT, ExpectedT>
  && detail::assertion_traits_serializable<ExpectedT>
  && detail::assertion_traits_serializable<ActualT>
  // C++11 version accepts floats here, but this isn't entirely correct.
  // At very least for floating point numbers it should be overload
  // that accepts epsilon like assertEqual() and CPPUNIT_ASSERT_DOUBLES_EQUAL() do.
  // So in C++20: deny floating point args in favor of macros:
  //   CPPUNIT_ASSERT_DOUBLES_LESS_EQUAL( double, double, epsilon )
  //   CPPUNIT_ASSERT_DOUBLES_GREATER_EQUAL( double, double, epsilon )
  // And underlying assertDoubleLessEqual() and assertDoubleGreaterEqual().
  // Another option is to provide overload for CPPUNIT_ASSERT_LESSEQUAL()
  // that takes floating point numbers and compare them with some default epsilon.
  && detail::not_floating_points<ExpectedT, ActualT>
  && detail::not_string_literals<ExpectedT, ActualT> )
#endif // !CPPUNIT_HAS_CONCEPTS
void assertLessEqual( const ExpectedT& expected,
                      const ActualT& actual,
                      const AdditionalMessage& message,
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


/*! \brief (Implementation) Asserts that two objects of the same type are equals.
 * Use CPPUNIT_ASSERT_LESSEQUAL, CPPUNIT_ASSERT_GREATEREQUAL instead of this function.
 * \sa assertion_traits, Asserter::failNotLessEqual().
 */
#if !CPPUNIT_HAS_CONCEPTS
template <typename ExpectedT>
#else
template <typename ExpectedT, typename ActualT>
  requires ( detail::less_equal_comparable_with<ExpectedT, ActualT>
  && detail::assertion_traits_serializable<ExpectedT>
  && detail::assertion_traits_serializable<ActualT>
  // Same constraints for floats as in assertLessEqual()
  && detail::not_floating_points<ExpectedT, ActualT>
  && detail::not_string_literals<ExpectedT, ActualT> )
#endif // !CPPUNIT_HAS_CONCEPTS
void assertGreaterEqual( const ExpectedT& expected,
                         const ActualT& actual,
                         const AdditionalMessage& message,
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


// Discard stuff used for transition from templates with one type argument
// two templates with two type arguments
#if !CPPUNIT_HAS_CONCEPTS
# undef ActualT
#endif // !CPPUNIT_HAS_CONCEPTS


// XXX: Explicit floating point numbers support.
// Explicit as in CPPUNIT_ASSERT_* is explicitly using those.

// It was assertDoubleEquals(double, double, double, etc) before.
// This template is not doing exactly the same thing because compiler
// was implicitly converting types when calling non-template
// assertDoubleEquals().
// For example assertDoubleEquals(int, float, epsilon) would compile
// in C++11 because first two arguments would be implicitly
// converted to double.
// It should work about the same way due to both arguments being
// converted to double when passed to doublesEqual() or
// assertion_traits<double>::toString().
// It no longer depends on compiler doing implicit convertions and
// doesn't require explicit actions (forcing template parameter
// type as in assertDoubleEquals<double, double>) in macro text.
/*! \brief (Implementation) Asserts that two double are equals given a tolerance.
 * Use CPPUNIT_ASSERT_DOUBLES_EQUAL instead of this function.
 * \sa Asserter::failNotEqual().
 * \sa CPPUNIT_ASSERT_DOUBLES_EQUAL for detailed semantic of the assertion.
 */
template <typename ExpectedT, typename ActualT>
#if CPPUNIT_HAS_CONCEPTS
  requires ( std::convertible_to<ExpectedT, double>
  && std::convertible_to<ActualT, double> )
#endif // CPPUNIT_HAS_CONCEPTS
void assertDoubleEquals( const ExpectedT& expected,
                         const ActualT& actual,
                         double epsilon,
                         const AdditionalMessage& message,
                         const SourceLine& sourceLine )
{
  const bool equal = detail::doublesEqual( expected, actual, epsilon );

  AdditionalMessage msg( "Delta   : " +
                         assertion_traits<double>::toString( epsilon ) );
  msg.addDetail( message );

  Asserter::failNotEqualIf( !equal,
                            assertion_traits<double>::toString( expected ),
                            assertion_traits<double>::toString( actual ),
                            sourceLine,
                            msg,
                            "double equality assertion failed" );
}


/*! \brief (Implementation) Less or equal implementation for floating point numbers
 * \sa Asserter::failNotLessEqual(), CPPUNIT_ASSERT_DOUBLES_LESSEQUAL.
 */
template <typename ExpectedT, typename ActualT>
#if CPPUNIT_HAS_CONCEPTS
  requires ( detail::less_comparable_with<ActualT, ExpectedT>
  && std::convertible_to<ExpectedT, double>
  && std::convertible_to<ActualT, double> )
#endif // CPPUNIT_HAS_CONCEPTS
void assertDoubleLessEqual( const ExpectedT& expected,
                            const ActualT& actual,
                            double epsilon,
                            const AdditionalMessage& message,
                            const SourceLine& sourceLine )
{
  AdditionalMessage msg( "Delta   : " +
                         assertion_traits<double>::toString( epsilon ) );
  msg.addDetail( message );

  if ( !( actual < expected )
  && !( detail::doublesEqual( expected, actual, epsilon ) ) )
  {
    Asserter::failNotLessEqual( assertion_traits<double>::toString( expected ),
                                assertion_traits<double>::toString( actual ),
                                sourceLine,
                                msg );
  }
}


/*! \brief (Implementation) Greater or equal implementation for floating point numbers
 * \sa Asserter::failNotGreaterEqual(), CPPUNIT_ASSERT_DOUBLES_GREATEREQUAL.
 */
template <typename ExpectedT, typename ActualT>
#if CPPUNIT_HAS_CONCEPTS
  requires ( detail::less_comparable_with<ExpectedT, ActualT>
  && std::convertible_to<ExpectedT, double>
  && std::convertible_to<ActualT, double> )
#endif // CPPUNIT_HAS_CONCEPTS
void assertDoubleGreaterEqual( const ExpectedT& expected,
                               const ActualT& actual,
                               double epsilon,
                               const AdditionalMessage& message,
                               const SourceLine& sourceLine )
{
  AdditionalMessage msg( "Delta   : " +
                         assertion_traits<double>::toString( epsilon ) );
  msg.addDetail( message );

  if ( !( expected < actual )
  && !( detail::doublesEqual( expected, actual, epsilon ) ) )
  {
    Asserter::failNotGreaterEqual( assertion_traits<double>::toString( expected ),
                                   assertion_traits<double>::toString( actual ),
                                   sourceLine,
                                   msg );
  }
}


// XXX: Implicit floating point numbers support.
// Implicit as in CPPUNIT_ASSERT_EQUAL() is going to be overloaded
// to implicitly use those for floating point number arguments.


#if CPPUNIT_HAS_CONCEPTS

/*! \brief Epsilon used for asserts floating point number comparison is
 * involved without setting epsilon explicitly.
 */
inline double CPPUNIT_DEFAULT_EPSILON = std::numeric_limits<float>::min();

// Normally to compare floating point numbers assertDoubleEquals()
// should be used with epsilon, but technically it is possible to do
// comparison correctly using some default epsilon.
// Additionally this overload can be marked [[deprecated]] to encourage
// usage of assertDoubleEquals() and print a human-readable message
// at the same time.

/*! \brief (Implementation) Overload for floating point numbers that
 * uses default epsilon for comparison.
 * \sa assertEquals, assertDoubleEquals, CPPUNIT_DEFAULT_EPSILON.
 * \sa CPPUNIT_ASSERT_EQUAL.
 */
template <typename ExpectedT, typename ActualT>
  requires ( detail::relaxed_floating_points<ExpectedT, ActualT> )
void assertEquals( const ExpectedT& e, const ActualT& a,
                   const AdditionalMessage& m, const SourceLine& sl ) {
  assertDoubleEquals( e, a, CPPUNIT_DEFAULT_EPSILON, m, sl );
}

template <typename ExpectedT, typename ActualT>
  requires ( detail::relaxed_floating_points<ActualT, ExpectedT> )
void assertLessEqual( const ExpectedT& e, const ActualT& a,
                      const AdditionalMessage& m, const SourceLine& sl ) {
  assertDoubleLessEqual( e, a, CPPUNIT_DEFAULT_EPSILON, m, sl );
}

template <typename ExpectedT, typename ActualT>
  requires ( detail::relaxed_floating_points<ExpectedT, ActualT> )
void assertGreaterEqual( const ExpectedT& e, const ActualT& a,
                         const AdditionalMessage& m, const SourceLine& sl ) {
  assertDoubleGreaterEqual( e, a, CPPUNIT_DEFAULT_EPSILON, m, sl );
}

#endif // CPPUNIT_HAS_CONCEPTS


// XXX: Strings support.


#if CPPUNIT_HAS_CONCEPTS

/*! \brief (Implementation) Convert stringl literal (may be null) to
 * optional (may be std::nullopt).
 */
template <typename ValueT>
  requires ( std::convertible_to<ValueT, std::string> )
constexpr
std::optional<std::string> stringLiteralToOptional( const ValueT &value )
{
  if constexpr ( detail::nullable_string_literal<ValueT>
  || std::same_as<ValueT, nullptr_t> )
  {
    if ( value == nullptr )
    {
      return std::nullopt;
    }
  }

  return std::make_optional( std::string(value) );
}


/*! \brief (Implementation) Overload for string literals comparison.
 *
 * String literals are going to be compared as optionals. Same applies
 * to all asserts in this category.
 */
template <typename ExpectedT, typename ActualT>
  requires ( detail::relaxed_string_literals<ExpectedT, ActualT> )
void assertEquals( const ExpectedT& e, const ActualT& a,
                   const AdditionalMessage& m, const SourceLine& sl ) {
  assertEquals( stringLiteralToOptional( e ), stringLiteralToOptional( a ), m, sl );
}

template <typename ExpectedT, typename ActualT>
  requires ( detail::relaxed_string_literals<ExpectedT, ActualT> )
void assertLess( const ExpectedT& e, const ActualT& a,
                 const AdditionalMessage& m, const SourceLine& sl ) {
  assertLess( stringLiteralToOptional( e ), stringLiteralToOptional( a ), m, sl );
}

template <typename ExpectedT, typename ActualT>
  requires ( detail::relaxed_string_literals<ExpectedT, ActualT> )
void assertGreater( const ExpectedT& e, const ActualT& a,
                    const AdditionalMessage& m, const SourceLine& sl ) {
  assertGreater( stringLiteralToOptional( e ), stringLiteralToOptional( a ), m, sl );
}

template <typename ExpectedT, typename ActualT>
  requires ( detail::relaxed_string_literals<ExpectedT, ActualT> )
void assertLessEqual( const ExpectedT& e, const ActualT& a,
                      const AdditionalMessage& m, const SourceLine& sl ) {
  assertLessEqual( stringLiteralToOptional( e ), stringLiteralToOptional( a ), m, sl );
}

template <typename ExpectedT, typename ActualT>
  requires ( detail::relaxed_string_literals<ExpectedT, ActualT> )
void assertGreaterEqual( const ExpectedT& e, const ActualT& a,
                         const AdditionalMessage& m, const SourceLine& sl ) {
  assertGreaterEqual( stringLiteralToOptional( e ), stringLiteralToOptional( a ), m, sl );
}

#endif // CPPUNIT_HAS_CONCEPTS


// XXX: Exceptions support.


/** \brief (Implementation) Asserts that executable is throwing exception
 * of expected type. Use CPPUNIT_ASSERT_THROW instead of this function.
 * \sa CPPUNIT_ASSERT_THROW, CPPUNIT_ASSERT_THROW_MESSAGE.
 */
template <typename ExceptionT,
          typename Executable>
#if CPPUNIT_HAS_CONCEPTS
  requires ( detail::executable<Executable> )
#endif // CPPUNIT_HAS_CONCEPTS
void assertThrows( const Executable& executable,
                   const AdditionalMessage& message,
                   const SourceLine& sourceLine )
{
  CPPUNIT_NS::Message cpputMsg_( "expected exception thrown" );
  cpputMsg_.addDetail( message );
  cpputMsg_.addDetail( "Expected: " +
                       CPPUNIT_NS::TypeInfoHelper::getClassName( typeid(ExceptionT) ) );

  bool cpputCorrectExceptionThrown_ = false;
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


/** \brief (Implementation) Asserts that executable does not throw exception.
 * Use CPPUNIT_ASSERT_NO_THROW instead of this function.
 * \sa CPPUNIT_ASSERT_NO_THROW, CPPUNIT_ASSERT_NO_THROW_MESSAGE.
 */
template <typename Executable>
#if CPPUNIT_HAS_CONCEPTS
  requires ( detail::executable<Executable> )
#endif // CPPUNIT_HAS_CONCEPTS
void assertDoesNotThrow( const Executable& executable,
                         const AdditionalMessage& message,
                         const SourceLine& sourceLine )
{
  CPPUNIT_NS::Message cpputMsg_( "unexpected exception caught" );
  cpputMsg_.addDetail( message );

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


} // namespace detail


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
  ( CPPUNIT_NS::detail::assertTrue( [&] { return ( condition ); },                \
                                 CPPUNIT_NS::Message( "assertion failed",         \
                                                      "Expression: " #condition), \
                                 CPPUNIT_SOURCELINE() ) )
#else
#define CPPUNIT_ASSERT(condition)                                            \
  ( CPPUNIT_NS::detail::assertTrue( [&] { return ( condition ); } (),        \
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
  ( CPPUNIT_NS::detail::assertTrue( [&] { return ( condition ); },         \
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
 * - (C++11) They are exactly of the same type
 * - They are serializable into a std::strstream using operator <<.
 * - They can be compared using operator ==. 
 *
 * The last two requirements (serialization and comparison) can be
 * removed by specializing the CppUnit::assertion_traits.
 */
#define CPPUNIT_ASSERT_EQUAL(expected,actual)          \
  ( CPPUNIT_NS::detail::assertEquals( (expected),      \
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
 * - (C++11) They are exactly of the same type
 * - They are serializable into a std::strstream using operator <<.
 * - They can be compared using operator ==. 
 *
 * The last two requirements (serialization and comparison) can be
 * removed by specializing the CppUnit::assertion_traits.
 */
#define CPPUNIT_ASSERT_EQUAL_MESSAGE(message,expected,actual)      \
  ( CPPUNIT_NS::detail::assertEquals( (expected),                  \
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
 * - (C++11) They are exactly of the same type
 * - They are serializable into a std::strstream using operator <<.
 * - They can be compared using operator <. 
 *
 * The last two requirements (serialization and comparison) can be
 * removed by specializing the CppUnit::assertion_traits.
 *
 * \sa CPPUNIT_ASSERT_GREATER
 */
#define CPPUNIT_ASSERT_LESS(expected, actual)          \
  ( CPPUNIT_NS::detail::assertLess( (expected),        \
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
 * - (C++11) They are exactly of the same type
 * - They are serializable into a std::strstream using operator <<.
 * - They can be compared using operator<. 
 *
 * The last two requirements (serialization and comparison) can be
 * removed by specializing the CppUnit::assertion_traits.
 *
 * \sa CPPUNIT_ASSERT_LESS
 */
#define CPPUNIT_ASSERT_GREATER(expected, actual)       \
  ( CPPUNIT_NS::detail::assertGreater( (expected),     \
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
 * - (C++11) They are exactly of the same type
 * - They are serializable into a std::strstream using operator <<.
 * - They can be compared using operator <=. 
 *
 * The last two requirements (serialization and comparison) can be
 * removed by specializing the CppUnit::assertion_traits.
 *
 * \sa CPPUNIT_ASSERT_GREATEREQUAL
 */
#define CPPUNIT_ASSERT_LESSEQUAL(expected, actual)               \
  ( CPPUNIT_NS::detail::assertLessEqual( (expected),             \
                                   (actual),                \
                                   "",                      \
                                   CPPUNIT_SOURCELINE() ) )


/** Asserts that actual is less or equal than expected for values
 * convertible to floating point number.
 * \ingroup Assertions
 *
 * \sa CPPUNIT_ASSERT_LESSEQUAL, CPPUNIT_ASSERT_DOUBLES_GREATEREQUAL
 */
# define CPPUNIT_ASSERT_DOUBLES_LESSEQUAL(expected, actual, epsilon) \
  ( CPPUNIT_NS::detail::assertDoubleLessEqual( (expected),           \
                                               (actual),             \
                                               (epsilon),            \
                                               "",                   \
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
 * - (C++11) They are exactly of the same type
 * - They are serializable into a std::strstream using operator <<.
 * - They can be compared using operator<=. 
 *
 * The last two requirements (serialization and comparison) can be
 * removed by specializing the CppUnit::assertion_traits.
 *
 * \sa CPPUNIT_ASSERT_LESSEQUAL
 */
#define CPPUNIT_ASSERT_GREATEREQUAL(expected, actual)            \
  ( CPPUNIT_NS::detail::assertGreaterEqual( (expected),          \
                                      (actual),              \
                                      "",                    \
                                      CPPUNIT_SOURCELINE() ) )


/** Asserts that actual is greater than expected for values
 * convertible to floating point number.
 * \ingroup Assertions
 *
 * \sa CPPUNIT_ASSERT_GREATEREQUAL, CPPUNIT_ASSERT_DOUBLES_LESSEQUAL
 */
# define CPPUNIT_ASSERT_DOUBLES_GREATEREQUAL(expected, actual, epsilon) \
  ( CPPUNIT_NS::detail::assertDoubleGreaterEqual( (expected),           \
                                                  (actual),             \
                                                  (epsilon),            \
                                                  "",                   \
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
  ( CPPUNIT_NS::detail::assertDoubleEquals( (expected),            \
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
  ( CPPUNIT_NS::detail::assertDoubleEquals( (expected),                      \
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
   CPPUNIT_ASSERT_THROW_MESSAGE( CPPUNIT_NS::AdditionalMessage(),       \
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
  ( CPPUNIT_NS::detail::assertThrows<ExceptionType>( [&] { expression; },     \
                                                     CPPUNIT_NS::message_to_string(message), \
                                                     CPPUNIT_SOURCELINE() ) )


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
   CPPUNIT_ASSERT_NO_THROW_MESSAGE( CPPUNIT_NS::AdditionalMessage(),       \
                                    expression )


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
  ( CPPUNIT_NS::detail::assertDoesNotThrow( [&] { expression; },              \
                                            CPPUNIT_NS::message_to_string(message), \
                                            CPPUNIT_SOURCELINE() ) )


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
