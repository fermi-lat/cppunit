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
#if CPPUNIT_HAS_CONCEPTS
  CPPUNIT_TEST( testAssertNullPointers );
#endif // CPPUNIT_HAS_CONCEPTS
  CPPUNIT_TEST( testAssertLess );
  CPPUNIT_TEST( testAssertGreater );
  CPPUNIT_TEST( testAssertLessEqual );
  CPPUNIT_TEST( testAssertGreaterEqual );
#if CPPUNIT_HAS_CONCEPTS && CPPUNIT_ENABLE_IMPLICIT_DOUBLES_EQUAL
  CPPUNIT_TEST( testAssertEqualDoublesOverload );
  CPPUNIT_TEST( testAssertLessEqualDoublesOverload );
  CPPUNIT_TEST( testAssertGreaterEqualDoublesOverload );
#endif // CPPUNIT_HAS_CONCEPTS && CPPUNIT_ENABLE_IMPLICIT_DOUBLES_EQUAL
#if CPPUNIT_HAS_CONCEPTS && CPPUNIT_ENABLE_STRING_LITERALS
  CPPUNIT_TEST( testAssertEqualStringsOverload );
  CPPUNIT_TEST( testAssertLessStringsOverload );
  CPPUNIT_TEST( testAssertGreaterStringsOverload );
  CPPUNIT_TEST( testAssertLessEqualStringsOverload );
  CPPUNIT_TEST( testAssertGreaterEqualStringsOverload );
#endif // CPPUNIT_HAS_CONCEPTS && CPPUNIT_ENABLE_STRING_LITERALS
  CPPUNIT_TEST( testAssertMessageTrue );
  CPPUNIT_TEST( testAssertMessageFalse );
  CPPUNIT_TEST( testAssertDoubleEquals );
  CPPUNIT_TEST( testAssertDoubleEqualsPrecision );
  CPPUNIT_TEST( testAssertDoubleNonFinite );
  CPPUNIT_TEST( testAssertDoubleEqualsMixedTypes );
  CPPUNIT_TEST( testAssertDoubleLessEqualAsserts );
  CPPUNIT_TEST( testAssertDoubleGreaterEqualAsserts );
  CPPUNIT_TEST( testFail );
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
#if CPPUNIT_HAS_CONCEPTS
  void testAssertNullPointers();
#endif // CPPUNIT_HAS_CONCEPTS
  void testAssertLess();
  void testAssertGreater();
  void testAssertLessEqual();
  void testAssertGreaterEqual();
#if CPPUNIT_HAS_CONCEPTS && CPPUNIT_ENABLE_IMPLICIT_DOUBLES_EQUAL
  void testAssertEqualDoublesOverload();
  void testAssertLessEqualDoublesOverload();
  void testAssertGreaterEqualDoublesOverload();
#endif // CPPUNIT_HAS_CONCEPTS && CPPUNIT_ENABLE_IMPLICIT_DOUBLES_EQUAL
#if CPPUNIT_HAS_CONCEPTS && CPPUNIT_ENABLE_STRING_LITERALS
  void testAssertEqualStringsOverload();
  void testAssertLessStringsOverload();
  void testAssertGreaterStringsOverload();
  void testAssertLessEqualStringsOverload();
  void testAssertGreaterEqualStringsOverload();
#endif // CPPUNIT_HAS_CONCEPTS && CPPUNIT_ENABLE_STRING_LITERALS

  void testAssertMessageTrue();
  void testAssertMessageFalse();

  void testAssertDoubleEquals();
  void testAssertDoubleEqualsPrecision();
  void testAssertDoubleNonFinite();
  void testAssertDoubleEqualsMixedTypes();
  void testAssertDoubleLessEqualAsserts();
  void testAssertDoubleGreaterEqualAsserts();

  void testAssertLongEquals();
  void testAssertLongNotEquals();

  void testFail();

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
