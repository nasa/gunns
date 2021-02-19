#ifndef UtTsFanSpeedSensors_EXISTS
#define UtTsFanSpeedSensors_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_ISS_FLUID_COMMON_ISS_FAN_SPEED_SENSORS Fan Speed Sensors
/// @ingroup   UT_ISS_FLUID_COMMON
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details   Unit Tests for the Fan Speed Sensors model.
///
///@{
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <string>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "../TsFanSpeedSensors.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from TsFanSpeedSensors and befriend UtTsFanSpeedSensors.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyTsFanSpeedSensors : public TsFanSpeedSensors
{
    public:
        FriendlyTsFanSpeedSensors();
        virtual ~FriendlyTsFanSpeedSensors();
        friend class UtTsFanSpeedSensors;
};
inline FriendlyTsFanSpeedSensors::FriendlyTsFanSpeedSensors() : TsFanSpeedSensors() {};
inline FriendlyTsFanSpeedSensors::~FriendlyTsFanSpeedSensors() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fan Speed Sensors unit tests.
///
/// @details  This class provides the unit tests for the OFan Speed Sensors model within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtTsFanSpeedSensors : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Fan Speed Sensors unit test.
        UtTsFanSpeedSensors();
        /// @brief    Default destructs this Fan Speed Sensors unit test.
        virtual ~UtTsFanSpeedSensors();
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
        /// @brief    Tests accessor methods.
        void testAccessors();
        /// @brief    Tests modifier methods.
        void testModifiers();
        /// @brief    Tests update method.
        void testUpdate();
        /// @brief    Tests sense method.
        void testSense();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();
    private:
        CPPUNIT_TEST_SUITE(UtTsFanSpeedSensors);
        CPPUNIT_TEST(testConfigAndInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testUpdate);
        CPPUNIT_TEST(testSense);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST_SUITE_END();
        SensorAnalogConfigData        mSensorConfig;   /**< (--)  Nominal speed sensor configuration data. */
        TsFanSpeedSensorsConfigData* mConfig;         /**< (--)  Pointer to nominal configuration data. */
        SensorAnalogInputData         mSensorAInput;   /**< (--)  Nominal speed sensor configuration data. */
        SensorAnalogInputData         mSensorBInput;   /**< (--)  Nominal speed sensor configuration data. */
        SensorAnalogInputData         mSensorCInput;   /**< (--)  Nominal speed sensor configuration data. */
        TsFanSpeedSensorsInputData*  mInput;          /**< (--)  Pointer to nominal input data. */
        std::string                   mName;           /**< (--)  Object name. */
        FriendlyTsFanSpeedSensors*   mArticle;        /**< (--)  Pointer to article under test. */
        double                        mTimeStep;       /**< (s)   Nominal time step. */
        double                        mTolerance;      /**< (--)  Nominal tolerance for comparison of expected and returned values. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtTsFanSpeedSensors(const UtTsFanSpeedSensors&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtTsFanSpeedSensors& operator =(const UtTsFanSpeedSensors&);
};

/// @}

#endif
