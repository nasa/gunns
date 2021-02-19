#ifndef UtBrentMethod_EXISTS
#define UtBrentMethod_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_UTILITIES_MATH_ROOT_FINDING_BRENT_METHOD Brent's Method Unit Tests
/// @ingroup  UT_UTILITIES_MATH_ROOT_FINDING
///
/// @details  Unit Tests for the BrentMethod class.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "math/root_finding/BrentMethod.hh"

class UtBrentMethodCurve1 : public BrentMethod
{
    public:
        UtBrentMethodCurve1() {}
        virtual ~UtBrentMethodCurve1() {}
        virtual double evaluate(const double x);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Utilities unit tests.
////
/// @details  This class provides the unit tests for the BrentMethod class within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtBrentMethod : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Utilities unit test.
        UtBrentMethod();
        /// @brief    Default destructs this Utilities unit test.
        virtual ~UtBrentMethod();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests Brent's for a nominal function.
        void testNominal();
        /// @brief    Tests Brent's for error conditions.
        void testErrors();
    private:
        CPPUNIT_TEST_SUITE(UtBrentMethod);
        CPPUNIT_TEST(testNominal);
        CPPUNIT_TEST(testErrors);
        CPPUNIT_TEST_SUITE_END();

        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtBrentMethod(const UtBrentMethod& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtBrentMethod& operator =(const UtBrentMethod& that);
};

///@}

#endif
