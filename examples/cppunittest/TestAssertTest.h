#ifndef TESTASSERTTEST_H
#define TESTASSERTTEST_H

#include <cppunit/extensions/HelperMacros.h>


class TestAssertTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( TestAssertTest );
  CPPUNIT_TEST( testAssertThrow );
  CPPUNIT_TEST( testAssertNoThrow );
  CPPUNIT_TEST( testAssertAssertionFail );
  CPPUNIT_TEST( testAssertAssertionPass );
  CPPUNIT_TEST( testAssert );
  CPPUNIT_TEST( testAssertEqual );
  CPPUNIT_TEST( testAssertLess );
  CPPUNIT_TEST( testAssertGreater );
  CPPUNIT_TEST( testAssertLessEqual );
  CPPUNIT_TEST( testAssertGreaterEqual );
  CPPUNIT_TEST( testAssertMessageTrue );
  CPPUNIT_TEST( testAssertMessageFalse );
  CPPUNIT_TEST( testAssertDoubleEquals );
  CPPUNIT_TEST( testAssertDoubleEqualsPrecision );
  CPPUNIT_TEST( testAssertDoubleNonFinite );
  CPPUNIT_TEST( testAssertDoubleEqualsMixedTypes );
  CPPUNIT_TEST( testDoubleLessEqualAsserts );
  CPPUNIT_TEST( testDoubleGreaterEqualAsserts );
  CPPUNIT_TEST( testFail );
  CPPUNIT_TEST( testAssertExceptionDetail );
#if CPPUNIT_HAS_CONCEPTS
  CPPUNIT_TEST( testConceptsAsserts );
#endif // CPPUNIT_HAS_CONCEPTS
  CPPUNIT_TEST_SUITE_END();

public:
  TestAssertTest();

  virtual ~TestAssertTest();

  virtual void setUp();
  virtual void tearDown();

  void testAssertThrow();
  void testAssertNoThrow();
  void testAssertAssertionFail();
  void testAssertAssertionPass();

  void testBasicAssertions();

  void testAssert();
  
  void testAssertEqual();
  void testAssertLess();
  void testAssertGreater();
  void testAssertLessEqual();
  void testAssertGreaterEqual();

  void testAssertMessageTrue();
  void testAssertMessageFalse();

  void testAssertDoubleEquals();
  void testAssertDoubleEqualsPrecision();
  void testAssertDoubleNonFinite();
  void testAssertDoubleEqualsMixedTypes();
  void testDoubleLessEqualAsserts();
  void testDoubleGreaterEqualAsserts();

  void testAssertLongEquals();
  void testAssertLongNotEquals();

  void testFail();

  void testAssertExceptionDetail();

#if CPPUNIT_HAS_CONCEPTS
  void testConceptsAsserts();
#endif // CPPUNIT_HAS_CONCEPTS

private:
  TestAssertTest( const TestAssertTest &copy );
  void operator =( const TestAssertTest &copy );

  void checkDoubleNotEquals( double expected, 
                             double actual, 
                             double delta );

  void checkMessageContains( CPPUNIT_NS::Exception *e,
                             std::string expectedMessage );

  void throwsException();
  void doesNotThrowException();

private:
};

#endif  // TESTASSERTTEST_H
