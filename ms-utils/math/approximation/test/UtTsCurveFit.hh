#ifndef UtTsCurveFit_EXISTS
#define UtTsCurveFit_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_UTILITIES_MATH_APPROXIMATION_CURVE_FIT    Curve Fit Unit Tests
/// @ingroup  UT_UTILITIES_MATH_APPROXIMATION
///
/// @details  Unit Tests for the Curve Fit classes.
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
class UtTsCurveFit: public CppUnit::TestFixture
{
    private:
        CPPUNIT_TEST_SUITE(UtTsCurveFit);
        CPPUNIT_TEST(testTsLinear);
        CPPUNIT_TEST(testQuadratic);
        CPPUNIT_TEST(testCubic);
        CPPUNIT_TEST(testQuartic);
        CPPUNIT_TEST(testQuintic);
        CPPUNIT_TEST(testExponential);
        CPPUNIT_TEST(testRational);
        CPPUNIT_TEST(testPower);
        CPPUNIT_TEST(testInvLinearFit);
        CPPUNIT_TEST(testInvQuadraticFit);
        CPPUNIT_TEST(testShowmateFit);
        CPPUNIT_TEST(testSutherlandFit);
        CPPUNIT_TEST(testQuotientFit);
        CPPUNIT_TEST(testProductFit);
        CPPUNIT_TEST(testQuadLinFit);
        CPPUNIT_TEST(testQuadLinInvFit);
        CPPUNIT_TEST(testQuadraticRootFit);
        CPPUNIT_TEST(testCoefficientAccessors);
        CPPUNIT_TEST(testInitializationException);
        CPPUNIT_TEST(testInvalidRangeExceptions);
        CPPUNIT_TEST(testInvalidRangeBounds);
        CPPUNIT_TEST(testExponentialException);
        CPPUNIT_TEST(testRationalException);
        CPPUNIT_TEST(testInvLinearException);
        CPPUNIT_TEST(testInvQuadraticException);
        CPPUNIT_TEST(testShowmateException);
        CPPUNIT_TEST(testSutherlandException);
        CPPUNIT_TEST(testQuotientException);
        CPPUNIT_TEST(testQuadLinInvException);
        CPPUNIT_TEST(testQuadraticRootException);
        CPPUNIT_TEST(testFunctionPointer);
        CPPUNIT_TEST(testEmptyConstructors);
        CPPUNIT_TEST_SUITE_END();
        /// @brief    -- Pointer to unit under test
        TsApproximation*   mArticle;
        // @brief    -- Tolerance for comparison of expected and returned values
        double             mTolerance;
    public:
        /// @brief    Default constructs this curve fit unit-test.
        UtTsCurveFit();
        /// @brief    Default destructs this curve fit unit-test.
        virtual ~UtTsCurveFit();
        void tearDown();
        void setUp();
        void testTsLinear();
        void testQuadratic();
        void testCubic();
        void testQuartic();
        void testQuintic();
        void testExponential();
        void testRational();
        void testPower();
        void testInvLinearFit();
        void testInvQuadraticFit();
        void testShowmateFit();
        void testSutherlandFit();
        void testQuotientFit();
        void testProductFit();
        void testQuadLinFit();
        void testQuadLinInvFit();
        void testQuadraticRootFit();
        void testCoefficientAccessors();
        void testInitializationException();
        void testInvalidRangeExceptions();
        void testInvalidRangeBounds();
        void testExponentialException();
        void testRationalException();
        void testInvLinearException();
        void testInvQuadraticException();
        void testShowmateException();
        void testSutherlandException();
        void testQuotientException();
        void testQuadLinInvException();
        void testQuadraticRootException();
        void testFunctionPointer();
        void testEmptyConstructors();
        typedef  double (TsApproximation::*EvaluateFunction)(const double x, const double y);
        double dummy(EvaluateFunction eval, TsApproximation* c);
    private:
          ////////////////////////////////////////////////////////////////////////////////////////////
          /// @details  Copy constructor unavailable since declared private and not implemented.
          ////////////////////////////////////////////////////////////////////////////////////////////
          UtTsCurveFit(const UtTsCurveFit&);
          ////////////////////////////////////////////////////////////////////////////////////////////
          /// @details  Assignment operator unavailable since declared private and not implemented.
          ////////////////////////////////////////////////////////////////////////////////////////////
          UtTsCurveFit& operator =(const UtTsCurveFit&);
};

/// @}

#endif

