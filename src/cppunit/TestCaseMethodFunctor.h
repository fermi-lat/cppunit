#ifndef CPPUNIT_TESTCASEMETHODFUNCTOR_H
#define CPPUNIT_TESTCASEMETHODFUNCTOR_H

#include <cppunit/Protector.h>
#include <cppunit/TestCase.h>

CPPUNIT_NS_BEGIN

/*! \brief Functor to call test case methods (Implementation).
 *
 * Implementation detail.
 */
class TestCaseMethodFunctor : public Functor
{
public:
  typedef void (TestCase::*Method)();

  TestCaseMethodFunctor( TestCase *target,
                         Method method )
    : m_target( target )
    , m_method( method )
  {
  }

  bool operator()() const
  {
    (m_target->*m_method)();
    return true;
  }

  bool isRunMethod() const
  {
    return m_method == &TestCase::runTest;
  }

private:
  TestCase *m_target;
  Method m_method;
};

CPPUNIT_NS_END

#endif // CPPUNIT_TESTCASEMETHODFUNCTOR_H
