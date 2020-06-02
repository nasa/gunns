#ifndef UtGunnsElectPvRegTrips_EXISTS
#define UtGunnsElectPvRegTrips_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_PHOTOVOLTAIC_REGULATOR_TRIPS    Photovoltaic Regulator Trips Package Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Photovoltaic Regulator Trips Package
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/electrical/SolarArray/GunnsElectPvRegTrips.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Photovoltaic Regulator Trips Package unit tests.
////
/// @details  This class provides the unit tests for the GunnsElectPvRegTrips within the CPPUnit
///           framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsElectPvRegTrips: public CppUnit::TestFixture
{
    public:
        /// @brief  Nominal constructor
        UtGunnsElectPvRegTrips();
        /// @brief  Nominal destructs
        virtual ~UtGunnsElectPvRegTrips();
        /// @brief  Executes before each test.
        void setUp();
        /// @brief  Executes after each test.
        void tearDown();
        /// @brief  Tests construction methods.
        void testConstruction();
        /// @brief  Tests the isTripped method.
        void testIsTripped();
        /// @brief  Tests the resetTrip method.
        void testResetTrip();

    private:
        /// @brief  Sets up the suite of tests for the GunnsElectPvRegTripsric unit testing.
        CPPUNIT_TEST_SUITE(UtGunnsElectPvRegTrips);
        CPPUNIT_TEST(testConstruction);
        CPPUNIT_TEST(testIsTripped);
        CPPUNIT_TEST(testResetTrip);
        CPPUNIT_TEST_SUITE_END();
        GunnsElectPvRegTrips* tArticle;       /**< (--) Test article. */
        static int            TEST_ID;        /**< (--) Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsElectPvRegTrips(const UtGunnsElectPvRegTrips& that);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsElectPvRegTrips& operator =(const UtGunnsElectPvRegTrips& that);
};

///@}

#endif
