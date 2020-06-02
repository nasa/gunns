#ifndef UtLaguerreMethod_EXISTS
#define UtLaguerreMethod_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_UTILITIES_MATH_ROOT_FINDING_LAGUERRE_METHOD Laguerre's Method Unit Tests
/// @ingroup  UT_UTILITIES_MATH_ROOT_FINDING
///
/// @details  Unit Tests for the LaguerreMethod class.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "math/root_finding/LaguerreMethod.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Utilities unit tests.
////
/// @details  This class provides the unit tests for the LaguerreMethod class within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtLaguerreMethod : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Laguerre's Method unit test.
        UtLaguerreMethod();
        /// @brief    Default destructs this Laguerre's Method unit test.
        virtual ~UtLaguerreMethod();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests Laguerre's method for returning roots of a real polynomial.
        void testReal();
        /// @brief    Tests Lageurre's method for returning roots of a complex polynomial.
        void testComplex();
        /// @brief    Tests Laguerre's method for error conditions.
        void testErrors();
    private:
        CPPUNIT_TEST_SUITE(UtLaguerreMethod);
        CPPUNIT_TEST(testReal);
        CPPUNIT_TEST(testComplex);
        CPPUNIT_TEST(testErrors);
        CPPUNIT_TEST_SUITE_END();

        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtLaguerreMethod(const UtLaguerreMethod& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtLaguerreMethod& operator =(const UtLaguerreMethod& that);
};

///@}

#endif
