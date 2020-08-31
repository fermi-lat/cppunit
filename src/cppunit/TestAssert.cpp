#include <cppunit/TestAssert.h>
#include <cppunit/portability/FloatingPoint.h>

CPPUNIT_NS_BEGIN


bool
floatsEqual( double expected,
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


CPPUNIT_NS_END
