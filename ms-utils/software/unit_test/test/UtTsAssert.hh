#ifndef UtTsAssert_EXISTS
#define UtTsAssert_EXISTS

/**

 @ingroup

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 @details
 - PURPOSE:
 - ()

 - REQUIREMENTS:
 - (())

 - REFERENCE:
 - (())

 - ASSUMPTIONS AND LIMITATIONS:
 - (())

 - LIBRARY DEPENDENCY:
 (
 - (UtTsAssert.o)
 )

 - PROGRAMMERS:
 (
 - ((Ryan Prentice) (L-3) (Feb 15, 2013) (TS21) (Initial implementation))
 )
 **************************************************************************************************/

// System includes
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

// TS includes
#include "software/SimCompatibility/TsSimCompatibility.hh"

// Models includes


class UtTsAssert : public CppUnit::TestFixture {
public:

    UtTsAssert();
    virtual ~UtTsAssert();

    virtual void setUp();

    virtual void tearDown();

    CPPUNIT_TEST_SUITE(UtTsAssert);

    CPPUNIT_TEST(testAutoDeltaExpectedPosAndGreaterThanOne);
    CPPUNIT_TEST(testAutoDeltaExpectedNegAndGreaterThanOne);
    CPPUNIT_TEST(testAutoDeltaExpectedPosAndLessThanOne);
    CPPUNIT_TEST(testAutoDeltaExpectedNegAndLessThanOne);
    CPPUNIT_TEST(testAutoDeltaExpectedZero);
    CPPUNIT_TEST(testAutoDeltaExpectedZero);
    CPPUNIT_TEST(testAutoDeltaExpectedInf);

    CPPUNIT_TEST_SUITE_END();

private:

    void testAutoDeltaExpectedPosAndGreaterThanOne();
    void testAutoDeltaExpectedNegAndGreaterThanOne();
    void testAutoDeltaExpectedPosAndLessThanOne();
    void testAutoDeltaExpectedNegAndLessThanOne();
    void testAutoDeltaExpectedZero();
    void testAutoDeltaExpectedNan();
    void testAutoDeltaExpectedInf();
};

#endif /* UtTsAssert_EXISTS */
