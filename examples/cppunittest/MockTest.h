#ifndef MOCKTEST_H
#define MOCKTEST_H

#include <cppunit/TestCase.h>
#include <functional>
#include <string>


/*! \class MockTest
 * \brief This class represents a mock test constructible
 * from a function.
 */
class MockTest : public CPPUNIT_NS::TestCase
{
public:
  using Function = std::function<void()>;

  MockTest( Function function, const std::string &name = "" )
    : CPPUNIT_NS::TestCase( name )
    , m_function( function )
  {
    CPPUNIT_ASSERT( m_function );
  }

protected:
  void runTest() override
  {
    m_function();
  }

private:
  const Function m_function;
};

#endif // MOCKTEST_H
