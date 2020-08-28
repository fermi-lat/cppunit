#include <cppunit/TestAssert.h>
#include <cppunit/portability/FloatingPoint.h>

CPPUNIT_NS_BEGIN


void 
assertEquals( double expected,
              double actual,
              double delta,
              const std::string &message,
              const SourceLine &sourceLine )
{
  AdditionalMessage msg( "Delta   : " + 
                         assertion_traits<double>::toString(delta) );
  msg.addDetail( AdditionalMessage(message) );

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

  Asserter::failNotEqualIf( !equal,
                            assertion_traits<double>::toString(expected),
                            assertion_traits<double>::toString(actual),
                            sourceLine, 
                            msg, 
                            "double equality assertion failed" );
}

void
assertDoubleEquals( double expected,
                    double actual,
                    double delta,
                    SourceLine sourceLine,
                    const std::string &message )
{
   return assertEquals( expected, actual, delta, message, sourceLine );
}


CPPUNIT_NS_END
