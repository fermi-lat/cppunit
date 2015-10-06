#include <cppunit/Exception.h>
#include <cppunit/TestLeaf.h>
#include <cppunit/TestFailure.h>
#include <cppunit/extensions/TypeInfoHelper.h>
#include "DefaultProtector.h"
#include "ProtectorContext.h"
#include "TestCaseMethodFunctor.h"


CPPUNIT_NS_BEGIN


bool 
DefaultProtector::protect( const Functor &functor,
                           const ProtectorContext &context )
{
  try
  {
    bool result = functor();

    const TestCaseMethodFunctor *test_functor = dynamic_cast< const TestCaseMethodFunctor* >( &functor );
    if ( !( test_functor && test_functor->isRunMethod() ) )
      return result;

    TestLeaf *test = dynamic_cast< TestLeaf* >( context.m_test );
    if ( result && test && test->expectedFailure() )
      reportUnexpectedPass( context );

    return result;
  }
  catch ( Exception &failure )
  {
    reportFailure( context, failure );
  }
  catch ( std::exception &e )
  {
    std::string shortDescription( "uncaught exception of type " );
#if CPPUNIT_USE_TYPEINFO_NAME
    shortDescription += TypeInfoHelper::getClassName( typeid(e) );
#else
    shortDescription += "std::exception (or derived).";
#endif
    Message message( shortDescription, e.what() );
    reportError( context, message );
  }
  catch ( ... )
  {
    reportError( context,
                 Message( "uncaught exception of unknown type") );
  }
  
  return false;
}


CPPUNIT_NS_END
