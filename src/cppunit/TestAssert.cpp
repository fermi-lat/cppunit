#include <cppunit/TestAssert.h>
#include <cppunit/portability/FloatingPoint.h>

CPPUNIT_NS_BEGIN

namespace detail
{

static bool
doublesEqual( double expected,
              double actual,
              double delta )
{
  bool equal;
  if ( floatingPointIsFinite(expected)  &&  floatingPointIsFinite(actual) )
      equal = fabs( expected - actual ) <= delta;
  else
  {
    // If expected or actual is not finite, it may be +inf, -inf or NaN (Not a Number).
    // Value of +inf or -inf leads to a true equality regardless of delta if both
    // expected and actual have the same value (infinity sign).
    // NaN Value should always lead to a failed equality.
    if ( floatingPointIsUnordered(expected)  ||  floatingPointIsUnordered(actual) )
    { 
       equal = false;  // expected or actual is a NaN
    }
    else // ordered values, +inf or -inf comparison
    {
       equal = expected == actual;
    }
  }

  return equal;
}


void
assertDoubleEquals( double expected,
                    double actual,
                    double epsilon,
                    const AdditionalMessage& message,
                    const SourceLine& sourceLine )
{
  const bool equal = doublesEqual( expected, actual, epsilon );

  AdditionalMessage msg( "Delta   : " +
                         assertion_traits<double>::toString(epsilon) );
  msg.addDetail( message );

  Asserter::failNotEqualIf( !equal,
                            assertion_traits<double>::toString(expected),
                            assertion_traits<double>::toString(actual),
                            sourceLine, 
                            msg, 
                            "double equality assertion failed" );
}


void
assertDoubleLessEqual( double expected,
                       double actual,
                       double epsilon,
                       const AdditionalMessage& message,
                       const SourceLine& sourceLine )
{
  AdditionalMessage msg( "Delta   : " +
                         assertion_traits<double>::toString(epsilon) );
  msg.addDetail( message );

  if ( !( actual < expected )
  && !( doublesEqual( expected, actual, epsilon ) ) )
  {
    Asserter::failNotLessEqual( assertion_traits<double>::toString(expected),
                                assertion_traits<double>::toString(actual),
                                sourceLine,
                                msg );
  }
}


void
assertDoubleGreaterEqual( double expected,
                          double actual,
                          double epsilon,
                          const AdditionalMessage& message,
                          const SourceLine& sourceLine )
{
  AdditionalMessage msg( "Delta   : " +
                         assertion_traits<double>::toString(epsilon) );
  msg.addDetail( message );

  if ( !( expected < actual )
  && !( doublesEqual( expected, actual, epsilon ) ) )
  {
    Asserter::failNotGreaterEqual( assertion_traits<double>::toString(expected),
                                   assertion_traits<double>::toString(actual),
                                   sourceLine,
                                   msg );
  }
}

} // namespace detail


CPPUNIT_NS_END
