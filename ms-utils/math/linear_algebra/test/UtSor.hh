#ifndef UtSor_EXISTS
#define UtSor_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_UTILITIES_MATH_LINEAR_ALGEBRA_SOR Successive Over-Relaxation Unit Tests
/// @ingroup  UT_UTILITIES_MATH_LINEAR_ALGEBRA
///
/// @details  Unit Tests for the Sor class.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "math/linear_algebra/Sor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Utilities unit tests.
////
/// @details  This class provides the unit tests for the Sor class within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtSor : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Utilities unit test.
        UtSor();
        /// @brief    Default destructs this Utilities unit test.
        virtual ~UtSor();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests the isPositiveDefinite method.
        void testPositiveDefinite();
        /// @brief    Tests [A]{x} = {b} using Sor.
        void testNormalSolution();
        /// @brief    Tests an unconditioned matrix.
        void testUnConditioned();
        /// @brief    Tests a singular matrix.
        void testSingular();
    private:
        CPPUNIT_TEST_SUITE(UtSor);
        CPPUNIT_TEST(testPositiveDefinite);
        CPPUNIT_TEST(testNormalSolution);
        CPPUNIT_TEST(testUnConditioned);
        CPPUNIT_TEST(testSingular);
        CPPUNIT_TEST_SUITE_END();

        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtSor(const UtSor& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtSor& operator =(const UtSor& that);
};

///@}

#endif
