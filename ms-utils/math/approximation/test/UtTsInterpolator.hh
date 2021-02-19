#ifndef UtTsInterpolator_EXISTS
#define UtTsInterpolator_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_UTILITIES_MATH_APPROXIMATION_INTERPOLATOR    Interpolator Unit Tests
/// @ingroup  UT_UTILITIES_MATH
///
/// @details  Unit Tests for the Interpolator classes.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "math/approximation/TsApproximation.hh"


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Curve fit unit tests.
////
/// @details  This class provides the unit tests for the Curve Fit classes within the CPPUnit
///           framework.
////////////////////////////////////////////////////////////////////////////////////////////////////

class UtTsInterpolator: public CppUnit::TestFixture
{
    private:
        CPPUNIT_TEST_SUITE(UtTsInterpolator);
        CPPUNIT_TEST(testTsLinear);
        CPPUNIT_TEST(testTsBilinear);
        CPPUNIT_TEST(testTsLinearException);
        CPPUNIT_TEST(testTsBilinearException);
        CPPUNIT_TEST(testInvalidRangeExceptions);
        CPPUNIT_TEST(testEmptyConstructors);
        CPPUNIT_TEST(testTsLinearInit1);
        CPPUNIT_TEST(testTsLinearInit2);
        CPPUNIT_TEST(testTsBilinearReverse);
        CPPUNIT_TEST_SUITE_END();
        /// @brief    -- Pointer to unit under test
        TsApproximation*   mArticle;
        /// @brief    -- Tolerance for comparison of expected and returned values
        double             mTolerance;
    public:
        /// @brief    Default constructs this interpolator model.
        UtTsInterpolator();
        /// @brief    Default destructs this curve fit unit-test.
        virtual ~UtTsInterpolator();
        void tearDown();
        void setUp();
        void testTsLinear();
        void testTsBilinear();
        void testTsLinearException();
        void testTsLinearInit1();
        void testTsLinearInit2();
        void testTsBilinearException();
        void testInvalidRangeExceptions();
        void testEmptyConstructors();
        void testTsBilinearReverse();
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtTsInterpolator(const UtTsInterpolator&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtTsInterpolator& operator =(const UtTsInterpolator&);
};
/// @}

#endif

