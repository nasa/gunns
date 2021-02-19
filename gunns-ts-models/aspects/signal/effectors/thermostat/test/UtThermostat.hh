#ifndef UtThermostat_EXISTS
#define UtThermostat_EXISTS
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_ASPECTS_SIGNAL_EFFECTORS_THERMOSTAT_Thermostat    Thermostat Unit Tests
/// @ingroup  UT_ASPECTS_SIGNAL_EFFECTORS_THERMOSTAT
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Thermostat model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include "aspects/signal/effectors/thermostat/Thermostat.hh"
#include <string>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from Thermostat and befriend UtThermostat.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyThermostat : public Thermostat
{
    public:
        FriendlyThermostat();
        virtual ~FriendlyThermostat();
        friend class UtThermostat;
};
inline FriendlyThermostat::FriendlyThermostat() : Thermostat() {};
inline FriendlyThermostat::~FriendlyThermostat() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief   unit tests.
///
/// @details  This class provides the unit tests for the Heater
///           Controller model within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtThermostat: public CppUnit::TestFixture
{
    public:
        /// @brief  Default constructs this Heater unit test.
        UtThermostat();

        /// @brief  Default destructs this Heater unit test.
        virtual ~UtThermostat();

        /// @brief  Executes before each test.
        virtual void setUp();

        /// @brief  Executes after each test.
        void tearDown();

        /// @brief  Tests config and input data.
        void testConfigAndInput();

        /// @brief  Tests default construction.
        void testDefaultConstruction();

        /// @brief  Tests initialize method.
        void testNominalInitialization();

        /// @brief  Tests initialize method exceptions.
        void testInitializationExceptions();

        /// @brief  Tests accessor methods.
        void testAccessors();

        /// @brief  Tests modifier methods.
        void testModifiers();

        /// @brief  Tests nominal heater command by comparing sensor readings with nominal setpoints.
        void testNominalBehavior();

        /// @brief  Tests the behavior of the Thermostat malfunctions.
        void testMalfunctionSetpts();

        /// @brief  Tests the behavior of the Thermostat with setpoints malf active, biased low.
        void testMalfunctionSetptsCold();

        /// @brief  Tests the behavior of the Thermostat with setpoints malf active, biased high.
        void testMalfunctionSetptsHot();

        /// @details  Tests the logic specific to a multi-sensor:AND thermostat.
        void testMultiSensorAnd();

        /// @details  Tests the logic specific to a multi-sensor:OR thermostat.
        void testMultiSensorOr();

    protected:
        FriendlyThermostat*  tArticle;  /**< (--)  Pointer to the Thermostat under test */

        std::string tName;   /**< (--)  Name of test article */
        double tTimeStep;    /**< (s)   Nominal time step */
        double tTolerance;   /**< (--)  Tolerance for comparison of expected/returned values */

        ThermostatConfigData* tConfig;       /**< (--)  Pointer to Nominal config data */
        ThermostatInputData*  tInput;         /**< (--)  Pointer to Nominal input data */

        SensorAnalog* tSensor;  /**< (--)  Array of dynamically-allocated temperature sensors */

        double tNominalSetptLow;   /**< (K)  Temperature at which to command heater on */
        double tNominalSetptHigh;  /**< (K)  Temperature at which to command heater off */

        /// Malfunctions
        bool tMalfCmdOnFlag;            /**< (--)  Heater command on malfunction flag */
        bool tMalfCmdOffFlag;           /**< (--)  Heater command off malfunction flag */
        bool tMalfSetptLowFlag;         /**< (--)  Override temp setpoint-low malfunction flag */
        bool tMalfSetptHighFlag;        /**< (--)  Override temp setpoint-high malfunction flag */

        /// - Specify the number of sensors.
        int tNumSensors;          /**< (--)  Number of sensors in the test article */

        /// Temperature samples
        double middle;  /**< (K)  Temperature value in the middle of the setpoint range */
        double tooCold; /**< (K)  Temperature value under the low setpoint */
        double tooHot;  /**< (K)  Temperature value above the high setpoint */

        /// @brief  Runs assertion checks for a given article at a given sensor temperature.
        void assertAtTemperature(const double temperature, const UserLoadMode expected,
                const char* testDescription);

        /// @details  An intermediary method that sets all sensors to a given value.
        void setAllSensors(const double temperature);

        /// @details  An intermediary method that sets one sensor to a given value.
        void setOneSensor(const double temperature, const int index = 0);

    private:
        CPPUNIT_TEST_SUITE(UtThermostat);
        CPPUNIT_TEST(testConfigAndInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testNominalBehavior);
        CPPUNIT_TEST(testMalfunctionSetpts);
        CPPUNIT_TEST(testMultiSensorAnd);
        CPPUNIT_TEST(testMultiSensorOr);
        CPPUNIT_TEST_SUITE_END();

    private:
        /// @details  Copy constructor unavailable since declared private and not implemented.
        UtThermostat(const UtThermostat&);
        /// @details  Assignment operator unavailable since declared private and not implemented.
        UtThermostat& operator=(const UtThermostat&);
};

///@}

#endif
