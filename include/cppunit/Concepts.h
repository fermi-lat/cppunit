#ifndef CPPUNIT_CONCEPTS_H
#define CPPUNIT_CONCEPTS_H


#if HAVE_CXX2A

#include <concepts>


CPPUNIT_NS_BEGIN


namespace asserts
{


/*! \brief This is used to pass lambdas (and other executables) to asserts
 */
template <typename T>
concept executable =
  // Executable should be a function that takes no arguments
  std::invocable<T>;


/*! \brief Used in asserts were less comparison is required
 */
template <typename T, typename U>
concept less_testable_with =
  requires (T t, U u)
  {
    { t < u };
  };


/*! \brief Used in asserts that do not accept floating point arguments
 */
template <typename ExpectedT, typename ActualT>
concept deny_fp_args =
  !std::floating_point<ExpectedT>
  && !std::floating_point<ActualT>;


/*! \brief Used in asserts that take floating point numbers
 */
template <typename ExpectedT, typename ActualT, typename To>
concept args_convertible_to =
  std::convertible_to<ExpectedT, To>
  && std::convertible_to<ActualT, To>;


/*! \brief Used in asserts overloaded for floating point numbers comparison
 */
template <typename ExpectedT, typename ActualT>
concept fp_overload =
  // Allow mixing floating point numbers with integers for this overload.
  // This overload will fit if at least one of the arguments is floating point.
  ( std::floating_point<ExpectedT> || std::floating_point<ActualT> )
  // Both arguments should be convertible to double
  // since assert_traits<double> is going to be used on them
  && args_convertible_to<ExpectedT, ActualT, double>;


} // namespace asserts


CPPUNIT_NS_END

#endif // HAVE_CXX2A

#endif // CPPUNIT_CONCEPTS_H
