#ifndef CPPUNIT_TESTLEAF_H
#define CPPUNIT_TESTLEAF_H

#include <cppunit/Test.h>


CPPUNIT_NS_BEGIN


/*! \brief A single test object.
 *
 * Base class for single test case: a test that doesn't have any children.
 *
 */
class CPPUNIT_API TestLeaf: public Test
{
public:
  TestLeaf();

  /*! Returns 1 as the default number of test cases invoked by run().
   * 
   * You may override this method when many test cases are invoked (RepeatedTest
   * for example).
   * 
   * \return 1.
   * \see Test::countTestCases().
   */
  int countTestCases() const;

  /*! Returns the number of child of this test case: 0.
   *
   * You should never override this method: a TestLeaf as no children by
   * definition.
   *
   * \return 0.
   */
  int getChildTestCount() const;

  /*! Always throws std::out_of_range.
   * \see Test::doGetChildTestAt().
   */
  Test *doGetChildTestAt( int index ) const;

  /*! Set the test to be expected to fail.
   *
   * Basically reverses the test logic, i.e. the test fails, if it passes!
   *
   * Can be useful for test driven development, to mark failing tests before
   * fixing the corresponding bugs.
   */
  void setExpectedFailure();

  /*! Is the text expected to fail?
   *
   * \return true, if the test is expected to fail.
   */
  bool expectedFailure() const;

private:
  bool m_expectedFailure;
};

CPPUNIT_NS_END

#endif // CPPUNIT_TESTLEAF_H
