/******************************** TRICK HEADER *****************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
 ()

 REQUIREMENTS:
 (())

 REFERENCE:
 (())

 ASSUMPTIONS AND LIMITATIONS:
 (())

 LIBRARY DEPENDENCY:
 (
 (UtTsAssert.o)
 )

 PROGRAMMERS:
 (
 ((Ryan Prentice) (L-3) (Feb 15, 2013) (TS21) (Initial implementation))
 )
 **************************************************************************************************/

// System includes

// TS includes

// Models includes
#include "UtTsAssert.hh"
#include "software/unit_test/TsAssert.hh"

UtTsAssert::UtTsAssert()
{
    // Intentionally left blank
}

UtTsAssert::~UtTsAssert()
{
    // Intentionally left blank
}

void UtTsAssert::setUp()
{
}

void UtTsAssert::tearDown()
{
}

void UtTsAssert::testAutoDeltaExpectedPosAndGreaterThanOne()
{
    double delta;
    double test;

    // Expected Value: positive and > 1
    test = 1000.0;
    delta = autoDelta(test, 0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(delta, __FLT_MIN__, __FLT_MIN__);

    delta = autoDelta(test, 1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(delta, 1000.0, 100.0);

    delta = autoDelta(test, 2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(delta, 100.0, 10.0);

    delta = autoDelta(test, 3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(delta, 10.0, 1.0);

    delta = autoDelta(test, 4);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(delta, 1.0, 0.1);

    delta = autoDelta(test, 5);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(delta, 0.1, 0.01);
}

void UtTsAssert::testAutoDeltaExpectedNegAndGreaterThanOne()
{
    double delta;
    double test;

    // Expected Value: negative and > 1
    test = -1000.0;
    delta = autoDelta(test, 0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(delta, __FLT_MIN__, __FLT_MIN__);

    delta = autoDelta(test, 1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(delta, 1000.0, 100.0);

    delta = autoDelta(test, 2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(delta, 100.0, 10.0);

    delta = autoDelta(test, 3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(delta, 10.0, 1.0);

    delta = autoDelta(test, 4);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(delta, 1.0, 0.1);

    delta = autoDelta(test, 5);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(delta, 0.1, 0.01);
}

void UtTsAssert::testAutoDeltaExpectedPosAndLessThanOne()
{
    double delta;
    double test;

    // Expected Value: positive and < 1
    test = 0.1;
    delta = autoDelta(test, 0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(delta, __FLT_MIN__, __FLT_MIN__);

    delta = autoDelta(test, 1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(delta, 0.1, 0.01);

    delta = autoDelta(test, 2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(delta, 0.01, 0.001);

    delta = autoDelta(test, 3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(delta, 0.001, 0.0001);

    delta = autoDelta(test, 4);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(delta, 0.0001, 0.00001);
}

void UtTsAssert::testAutoDeltaExpectedNegAndLessThanOne()
{
    double delta;
    double test;

    // Expected Value: positive and < 1
    test = -0.1;
    delta = autoDelta(test, 0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(delta, __FLT_MIN__, __FLT_MIN__);

    delta = autoDelta(test, 1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(delta, 0.1, 0.01);

    delta = autoDelta(test, 2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(delta, 0.01, 0.001);

    delta = autoDelta(test, 3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(delta, 0.001, 0.0001);

    delta = autoDelta(test, 4);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(delta, 0.0001, 0.00001);
}

void UtTsAssert::testAutoDeltaExpectedZero()
{
    double delta;
    double test;

    // Expected Value: 0
    test = 0.0;
    delta = autoDelta(test, 1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(delta, 0.1, 0.01);
}

void UtTsAssert::testAutoDeltaExpectedNan()
{
    double delta;
    double test;

    // Expected Value: NAN
    test = NAN;
    delta = autoDelta(test, 0);
    CPPUNIT_ASSERT(isnan(delta));
}

void UtTsAssert::testAutoDeltaExpectedInf()
{
    double delta;
    double test;

    // Expected Value: INF
    test = 1.0/0.0;
    delta = autoDelta(test, 0);
    CPPUNIT_ASSERT(isinf(delta));
}
