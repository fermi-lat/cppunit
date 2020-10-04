#ifndef CPPUNIT_EXTENSIONS_TESTFIXTUREFACTORY_H
#define CPPUNIT_EXTENSIONS_TESTFIXTUREFACTORY_H

#include <cppunit/Portability.h>


CPPUNIT_NS_BEGIN


class TestFixture;

/*! \brief Abstract TestFixture factory (Implementation).
 *
 * Implementation detail. Use by HelperMacros to handle TestFixture hierarchy.
 */
template<class TestFixtureType>
class TestFixtureFactory
{
public:
  /*! \brief Returns a new TestFixture instance.
   * \return A new fixture instance. The fixture instance is returned by
   *         the TestFixtureFactory passed on construction. The actual type 
   *         is that of the fixture on which the static method suite() 
   *         was called.
   */
  virtual TestFixtureType *makeFixture()
  {
    return new TestFixtureType();
  }
};


CPPUNIT_NS_END


#endif // CPPUNIT_EXTENSIONS_TESTFIXTUREFACTORY_H

