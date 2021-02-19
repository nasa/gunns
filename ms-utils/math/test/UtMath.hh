#ifndef UtMath_Exist
#define UtMath_Exist

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_TSM_UTILITIES_MATH_MATH    MsMath Unit Tests
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
/// @brief    MsMath unit tests.
///
/// @details  This class provides the unit tests for the MsMath class within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtMath : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Check Valve unit test.
        UtMath();
        /// @brief    Default destructs this Check Valve unit test.
        virtual ~UtMath();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Test anglePi method.
        void testAnglePi();
        /// @brief    Test angle2Pi method.
        void testAngle2Pi();
        /// @brief    Test angle180 method.
        void testAngle180();
        /// @brief    Test angle360 method.
        void testAngle360();
        /// @brief    Test protectedAsin method.
        void testProtectedAsin();
        /// @brief    Test protectedAcos method.
        void testProtectedAcos();
        /// @brief    Test protectedSqrt method.
        void testProtectedSqrt();
        /// @brief    Test protectedLog10 method.
        void testProtectedLog10();
        /// @brief    Test protectedLog method.
        void testProtectedLog();
        /// @brief    Test protectedDiv method.
        void testprotectedDiv();
        /// @brief    Test double limitRange method.
        void testLimitRangeDouble();
        /// @brief    Test int limitRange method.
        void testLimitRangeInt();
        /// @brief    Test double isInRange method.
        void testIsInRangeDouble();
        /// @brief    Test int isInRange method.
        void testIsInRangeInt();
        /// @brief    Test innerLimit method.
        void testInnerLimitDouble();
        /// @brief    Test innerLimit method.
        void testInnerLimitInt();
        /// @brief    Test round method.
        void testRounding();
        /// @brief    Test quantized method.
        void testQuantize();
        /// @brief    Test fastPow method.
        void testFastPow();
    private:
        CPPUNIT_TEST_SUITE(UtMath);
        CPPUNIT_TEST(testAnglePi);
        CPPUNIT_TEST(testAngle2Pi);
        CPPUNIT_TEST(testAngle180);
        CPPUNIT_TEST(testAngle360);
        CPPUNIT_TEST(testProtectedAsin);
        CPPUNIT_TEST(testProtectedAcos);
        CPPUNIT_TEST(testProtectedSqrt);
        CPPUNIT_TEST(testProtectedLog10);
        CPPUNIT_TEST(testProtectedLog);
        CPPUNIT_TEST(testprotectedDiv);
        CPPUNIT_TEST(testLimitRangeDouble);
        CPPUNIT_TEST(testLimitRangeInt);
        CPPUNIT_TEST(testIsInRangeDouble);
        CPPUNIT_TEST(testIsInRangeInt);
        CPPUNIT_TEST(testInnerLimitDouble);
        CPPUNIT_TEST(testInnerLimitInt);
        CPPUNIT_TEST(testRounding);
        CPPUNIT_TEST(testQuantize);
        CPPUNIT_TEST(testFastPow);
        CPPUNIT_TEST_SUITE_END();

        double     m_defaultTolerance;    /**< (--)  Tolerance for comparison of expected and returned values. */
        static int TEST_ID;               /**< (--)  Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtMath(const UtMath&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtMath& operator =(const UtMath&);
};

/// @}

#endif
