#ifndef CPPUNIT_CONCEPTS_H
#define CPPUNIT_CONCEPTS_H


#if HAVE_CXX2A

#include <concepts>


CPPUNIT_NS_BEGIN


// This is used to pass lambdas (and other executables) to asserts
template <typename T>
concept executable =
  // Executable should be a function that takes no arguments
  std::invocable<T>;


// Used in asserts were less comparison is required
template <typename T, typename U>
concept less_testable_with =
  requires (T t, U u)
  {
    { t < u };
  };


CPPUNIT_NS_END

#endif // HAVE_CXX2A

#endif // CPPUNIT_CONCEPTS_H
