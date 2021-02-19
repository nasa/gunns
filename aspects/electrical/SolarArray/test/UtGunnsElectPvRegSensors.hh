#ifndef UtGunnsElectPvRegSensors_EXISTS
#define UtGunnsElectPvRegSensors_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_PHOTOVOLTAIC_REGULATOR_SENSORS    Photovoltaic Regulator Sensors Package Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Photovoltaic Regulator Sensors Package
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/electrical/SolarArray/GunnsElectPvRegSensors.hh"
#include "core/GunnsSensorAnalogWrapper.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Photovoltaic Regulator Sensors Package unit tests.
////
/// @details  This class provides the unit tests for the GunnsElectPvRegSensors within the CPPUnit
///           framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsElectPvRegSensors: public CppUnit::TestFixture
{
    public:
        /// @brief  Nominal constructor
        UtGunnsElectPvRegSensors();
        /// @brief  Nominal destructs
        virtual ~UtGunnsElectPvRegSensors();
        /// @brief  Executes before each test.
        void setUp();
        /// @brief  Executes after each test.
        void tearDown();
        /// @brief  Tests construction methods.
        void testConstruction();
        /// @brief  Tests the sensor connection methods.
        void testConnectSensors();

    private:
        /// @brief  Sets up the suite of tests for the GunnsElectPvRegSensorsric unit testing.
        CPPUNIT_TEST_SUITE(UtGunnsElectPvRegSensors);
        CPPUNIT_TEST(testConstruction);
        CPPUNIT_TEST(testConnectSensors);
        CPPUNIT_TEST_SUITE_END();
        GunnsElectPvRegSensors*  tArticle;      /**< (--) Test article. */
        GunnsSensorAnalogWrapper tSensorVin;    /**< (--) Swensor wrapper object. */
        GunnsSensorAnalogWrapper tSensorVout;   /**< (--) Swensor wrapper object. */
        GunnsSensorAnalogWrapper tSensorIin;    /**< (--) Swensor wrapper object. */
        GunnsSensorAnalogWrapper tSensorIout;   /**< (--) Swensor wrapper object. */
        static int               TEST_ID;       /**< (--) Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsElectPvRegSensors(const UtGunnsElectPvRegSensors& that);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsElectPvRegSensors& operator =(const UtGunnsElectPvRegSensors& that);
};

///@}

#endif
