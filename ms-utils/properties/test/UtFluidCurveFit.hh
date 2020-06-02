#ifndef UtFluidCurveFit_EXISTS
#define UtFluidCurveFit_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_PROPERTIES_FLUID_CURVE_FIT    Fluid Curve Fit Unit Tests
/// @ingroup  UT_TSM_PROPERTIES
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Fluid-specific Curve Fit classes.
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
class UtFluidCurveFit: public CppUnit::TestFixture
{
    private:
        CPPUNIT_TEST_SUITE(UtFluidCurveFit);
        CPPUNIT_TEST(testTsat);
        CPPUNIT_TEST(testHvap);
        CPPUNIT_TEST(testInitializationException);
        CPPUNIT_TEST(testInvalidRangeExceptions);
        CPPUNIT_TEST(testInvalidRangeBounds);
        CPPUNIT_TEST(testTsatException);
        CPPUNIT_TEST(testHvapException);
        CPPUNIT_TEST(testFunctionPointer);
        CPPUNIT_TEST(testEmptyConstructors);
        CPPUNIT_TEST_SUITE_END();
        /// @brief    -- Pointer to unit under test
        TsApproximation*   mArticle;
        // @brief    -- Tolerance for comparison of expected and returned values
        double             mTolerance;
    public:
        /// @brief    Default constructs this curve fit unit-test.
        UtFluidCurveFit();
        /// @brief    Default destructs this curve fit unit-test.
        virtual ~UtFluidCurveFit();
        void tearDown();
        void setUp();
        void testTsat();
        void testHvap();
        void testInitializationException();
        void testInvalidRangeExceptions();
        void testInvalidRangeBounds();
        void testTsatException();
        void testHvapException();
        void testFunctionPointer();
        void testEmptyConstructors();
        typedef  double (TsApproximation::*EvaluateFunction)(const double x, const double y);
        double dummy(EvaluateFunction eval, TsApproximation* c);
    private:
          ////////////////////////////////////////////////////////////////////////////////////////////
          /// @details  Copy constructor unavailable since declared private and not implemented.
          ////////////////////////////////////////////////////////////////////////////////////////////
          UtFluidCurveFit(const UtFluidCurveFit&);
          ////////////////////////////////////////////////////////////////////////////////////////////
          /// @details  Assignment operator unavailable since declared private and not implemented.
          ////////////////////////////////////////////////////////////////////////////////////////////
          UtFluidCurveFit& operator =(const UtFluidCurveFit&);
};

/// @}

#endif

