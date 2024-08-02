#ifndef UtRandom_Exist
#define UtRandom_Exist

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_TSM_UTILITIES_MATH_MATH    Math Unit Tests
/// @ingroup   UT_TSM_UTILITIES_MATH
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details   Unit Tests for the Math class.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Math unit tests.
///
/// @details  This class provides the unit tests for the Math class within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtRandom : public CppUnit::TestFixture
{
public:
    UtRandom();
    virtual ~UtRandom();

    void setUp();
    void tearDown();

    /// @brief    Tests TS_random::randomDouble()
    void testRandomDouble1();
    void testRandomDouble2();
    void testRandomDouble3();
    void testRandomInt1();
    void testRandomInt2();
    void testUniqueRandomDouble();
    void testValueFound();

private:
    CPPUNIT_TEST_SUITE(UtRandom);
    CPPUNIT_TEST( testRandomDouble1 );
    CPPUNIT_TEST( testRandomDouble2 );
    CPPUNIT_TEST( testRandomDouble3 );
    CPPUNIT_TEST( testRandomInt1 );
    CPPUNIT_TEST( testRandomInt2 );
    CPPUNIT_TEST( testUniqueRandomDouble );
    CPPUNIT_TEST( testValueFound );
    CPPUNIT_TEST_SUITE_END();

    int        ITERATION_COUNT;
    static int TEST_ID;               /**< (--)  Test identification number. */
    ////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Copy constructor unavailable since declared private and not implemented.
    ////////////////////////////////////////////////////////////////////////////////////////////
    UtRandom(
        const UtRandom & other );
    ////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Assignment operator unavailable since declared private and not implemented.
    ////////////////////////////////////////////////////////////////////////////////////////////
    UtRandom & operator =(
        const UtRandom & other );
};

/// @}

#endif
