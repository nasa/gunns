#ifndef UtLambertW_EXISTS
#define UtLambertW_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_UTILITIES_MATH_ELEM_FUNCTIONS_LAMBERTW Lambert W Function Unit Tests
/// @ingroup  UT_UTILITIES_MATH_ELEM_FUNCTIONS
///
/// @copyright Copyright 2024 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the LambertW class.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "math/elementary_functions/LambertW.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Lambert W Function unit test input set.
////
/// @details  This class provides a data structure to hold a set of input and expected output values
///           used int testing the Lamber W Function implementation..
////////////////////////////////////////////////////////////////////////////////////////////////////
struct UtLambertWInputSet
{
    public:
        double mInput;
        double mExpected;
        double mConvergence;
        UtLambertWInputSet(const double input = 0.0, const double expected = 0.0, const double convergence = 0.0);
        virtual ~UtLambertWInputSet();
        UtLambertWInputSet(const UtLambertWInputSet& that);
        UtLambertWInputSet& operator =(const UtLambertWInputSet& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Lambert W Function unit tests.
////
/// @details  This class provides the unit tests for the LambertW class within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtLambertW : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Utilities unit test.
        UtLambertW();
        /// @brief    Default destructs this Utilities unit test.
        virtual ~UtLambertW();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests solveW0 for nominal inputs.
        void testW0Nominal();
        /// @brief    Tests solveW1 for nominal inputs.
        void testW1Nominal();
        /// @brief    Tests solveW0 for error conditions.
        void testW0Errors();
        /// @brief    Tests solveW1 for error conditions.
        void testW1Errors();
        /// @brief    Tests fastSolveW0 for nominal inputs.
        void testW0Fast();
        /// @brief    Tests fastSolveW1 for nominal inputs.
        void testW1Fast();

    private:
        CPPUNIT_TEST_SUITE(UtLambertW);
        CPPUNIT_TEST(testW0Nominal);
        CPPUNIT_TEST(testW0Errors);
        CPPUNIT_TEST(testW1Nominal);
        CPPUNIT_TEST(testW1Errors);
        CPPUNIT_TEST(testW0Fast);
        CPPUNIT_TEST(testW1Fast);
        CPPUNIT_TEST_SUITE_END();
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtLambertW(const UtLambertW& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtLambertW& operator =(const UtLambertW& that);
};

///@}

#endif
