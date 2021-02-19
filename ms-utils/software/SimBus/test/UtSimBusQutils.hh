#ifndef UtSimBusQutils_EXISTS
#define UtSimBusQutils_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_UTILITIES_SOFTWARE_SIM_BUS    Sim Bus Queue Utilities Unit Tests
/// @ingroup  UT_UTILITIES_SOFTWARE
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for Sim Bus Queue Utilities.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "software/SimBus/SimBusQutils.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Utilities unit tests.
////
/// @details  This class provides the unit tests for the SimBusQutils class within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtSimBusQutils : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Utilities unit test.
        UtSimBusQutils();
        /// @brief    Default destructs this Utilities unit test.
        virtual ~UtSimBusQutils();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        void testGetLatest();
        void testGetSum();
        void testGetAverage();
        void testIsPulseHigh();
    private:
        CPPUNIT_TEST_SUITE(UtSimBusQutils);
        CPPUNIT_TEST(testGetLatest);
        CPPUNIT_TEST(testGetSum);
        CPPUNIT_TEST(testGetAverage);
        CPPUNIT_TEST(testIsPulseHigh);
        CPPUNIT_TEST_SUITE_END();

        /// @brief  --  Array of type double for the test queue values
        double* tQueueDouble;
        /// @brief  --  Array of type int for the test queue values
        int*    tQueueInt;
        /// @brief  --  Array of type boolean for the test queue values
        bool* tQueueBool;
        /// @brief  --  Test queue size
        int     tQueueSize;
        /// @brief  --  Test queue extraction size
        int     tExtractionSize;

        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtSimBusQutils(const UtSimBusQutils& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtSimBusQutils& operator =(const UtSimBusQutils& that);
};

///@}

#endif
