#ifndef UtConverterSensors_EXISTS
#define UtConverterSensors_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_ELECTRICAL_CONVERTER_SENSORS_UNIT_TEST  Converter Sensors Unit Test
/// @ingroup  UT_TSM_GUNNS_ELECTRICAL_CONVERTER_SENSORS
/// -
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Converter Sensors Model
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "common/sensors/SensorAnalog.hh"
#include "aspects/electrical/Converter/ConverterSensors.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from ConverterSensors and befriend ConverterSensors.
/// -
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyConverterSensors : public ConverterSensors {
 public:
    FriendlyConverterSensors();
    virtual ~FriendlyConverterSensors();
    friend class UtConverterSensors;
};

inline FriendlyConverterSensors::FriendlyConverterSensors()
    :
    ConverterSensors() {
};

inline FriendlyConverterSensors::~FriendlyConverterSensors() {
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief   ConverterSensors unit tests.
//// -
/// @details  This class provides the unit tests for the ConverterSensors within the CPPunit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtConverterSensors : public CppUnit::TestFixture {
    public:
        /// @brief    Default constructs this Converter Sensors model unit test.
        UtConverterSensors();
        /// @brief    Default destructs this Converter Sensors model unit test.
        virtual ~UtConverterSensors();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests config and input data.
        void testConfigAndInput();
        /// @brief    Tests default construction.
        void testDefaultConstruction();
        /// @brief    Tests initialize method.
        void testNominalInitialization();
        /// @brief    Tests standby power exception  method.
        void testStandbyPowerException();
        /// @brief    Tests update  method.
        void testRegulatedVoltageException();
        /// @brief    Tests update  method.
        void testEfficiencyException();
        /// @brief    Tests accessor methods.
        void testAccessors();
        /// @brief    Tests modifier methods.
        void testModifiers();
        /// @brief    Tests update  method.
        void testStep();
        /// @brief    Tests update  method.
        void testConversion();
        /// @brief    Tests update  method.
        void testConverterOff();
        /// @brief    Tests update  method.
        void testOutputOverCurrentTrip();
        /// @brief    Tests update  method.
        void testOutputOverVoltageTrip();
        /// @brief    Tests update  method.
        void testMalfunctions();
        /// @brief    Tests update  method.
        void testResetTrips();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();
    private:
        CPPUNIT_TEST_SUITE(UtConverterSensors);
        CPPUNIT_TEST(testConfigAndInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testConversion);
        CPPUNIT_TEST(testConverterOff);
        CPPUNIT_TEST(testOutputOverCurrentTrip);
        CPPUNIT_TEST(testOutputOverVoltageTrip);
        CPPUNIT_TEST(testMalfunctions);
        /*
        CPPUNIT_TEST(testInitializationExceptions);
        */
        CPPUNIT_TEST_SUITE_END();

        double          mStandbyPower;
        double          mInputVoltage;
        double          mOutputPower;
        double          mRegulatedVoltage;
        double          mEfficiency;
        double          mOutputOverCurrentLimit;
        double          mOutputOverVoltageLimit;
        bool            mOutOverCurrentTripActive;
        bool            mOutOverVoltageTripActive;
        double          mInputOverVoltageLimit;
        double          mInputUnderVoltageLimit;
        bool            mInOverVoltageTripActive;
        bool            mInUnderVoltageTripActive;

        SensorAnalogConfigData  mOutVoltageSensorConfig;
        SensorAnalogConfigData  mOutCurrentSensorConfig;
        ConverterSensorsConfigData* mConfig;     /**< (--)  Pointer to nominal configuration data. */

        SensorAnalogInputData   mOutVoltageSensorInput;
        SensorAnalogInputData   mOutCurrentSensorInput;
        ConverterSensorsInputData*  mInput;      /**< (--)  Pointer to nominal input data.  */

        FriendlyConverterSensors* mArticle;  /**< (--)   Pointer to the unit under test. */

        double          mTimeStep;
        double          mTolerance;
        static int      TEST_ID;  /**< (--)  Test identification number. */

  private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtConverterSensors(const UtConverterSensors&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtConverterSensors& operator =(const UtConverterSensors&);
};

/// @}

#endif

