#ifndef UtGunnsSensorAnalogWrapper_EXISTS
#define UtGunnsSensorAnalogWrapper_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_SENSOR_ANALOG_WRAPPER    GUNNS Analog Sensor Wrapper Spotter Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Analog Sensor Wrapper Spotter class
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include "core/GunnsSensorAnalogWrapper.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsSensorAnalogWrapper and befriend UtGunnsSensorAnalogWrapper.
///
/// @details  Class derived from the unit under test.  It has a constructor with the same arguments
///           as the parent and a default destructor, but it befriends the unit test case driver
///           class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsSensorAnalogWrapper : public GunnsSensorAnalogWrapper
{
    public:
        FriendlyGunnsSensorAnalogWrapper() : GunnsSensorAnalogWrapper() {;}
        virtual ~FriendlyGunnsSensorAnalogWrapper() {;}
        friend class UtGunnsSensorAnalogWrapper;
        friend class UtGunnsElectPvRegSensors;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test implementation of GunnsNetworkSpotterConfigData.
///
/// @details  Derives from GunnsNetworkSpotterConfigData and is used to test that a dynamic_cast of
///           this type to the GunnsSensorAnalogWrapperConfigData test article type can fail.
////////////////////////////////////////////////////////////////////////////////////////////////////
class BadGunnsSensorAnalogWrapperConfigData : public GunnsNetworkSpotterConfigData
{
    public:
        BadGunnsSensorAnalogWrapperConfigData(const std::string& name) : GunnsNetworkSpotterConfigData(name) {}
        virtual ~BadGunnsSensorAnalogWrapperConfigData() {}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test implementation of GunnsNetworkSpotterInputData.
///
/// @details  Derives from GunnsNetworkSpotterInputData and is used to test that a dynamic_cast of
///           this type to the GunnsSensorAnalogWrapperInputData test article type can fail.
////////////////////////////////////////////////////////////////////////////////////////////////////
class BadGunnsSensorAnalogWrapperInputData : public GunnsNetworkSpotterInputData
{
    public:
        BadGunnsSensorAnalogWrapperInputData() {}
        virtual ~BadGunnsSensorAnalogWrapperInputData() {}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Analog Sensor Wrapper Spotter Unit Tests.
///
/// @details  This class provides the unit tests for the GunnsSensorAnalogWrapper class within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsSensorAnalogWrapper : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this GunnsSensorAnalogWrapper unit test.
        UtGunnsSensorAnalogWrapper();
        /// @brief    Default destructs this GunnsSensorAnalogWrapper unit test.
        virtual ~UtGunnsSensorAnalogWrapper();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests the config data class.
        void testConfig();
        /// @brief    Tests the input data class.
        void testInput();
        /// @brief    Tests default constructors.
        void testDefaultConstruction();
        /// @brief    Tests initialization.
        void testInitialize();
        /// @brief    Tests initialization exceptions.
        void testInitializeExceptions();
        /// @brief    Tests the stepPreSolver method.
        void testPreSolver();
        /// @brief    Tests the stepPostSolver method.
        void testPostSolver();
        /// @brief    Tests the setter and getter methods.
        void testAccessors();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsSensorAnalogWrapper);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testInitialize);
        CPPUNIT_TEST(testInitializeExceptions);
        CPPUNIT_TEST(testPreSolver);
        CPPUNIT_TEST(testPostSolver);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST_SUITE_END();

        FriendlyGunnsSensorAnalogWrapper*   tArticle;           /**< (--) Test article. */
        std::string                         tName;              /**< (--) Instance name. */
        GunnsSensorAnalogWrapperConfigData* tConfig;            /**< (--) Nominal config data. */
        GunnsSensorAnalogWrapperInputData*  tInput;             /**< (--) Nominal input data. */
        double                              tMinRange;          /**< (--) Nominal config data. */
        double                              tMaxRange;          /**< (--) Nominal config data. */
        double                              tOffValue;          /**< (--) Nominal config data. */
        double                              tNominalBias;       /**< (--) Nominal config data. */
        double                              tNominalScale;      /**< (--) Nominal config data. */
        double                              tNominalNoiseScale; /**< (--) Nominal config data. */
        double                              tNominalResolution; /**< (--) Nominal config data. */
        double                            (*tNoiseFunction)();  /**< (--) Nominal config data */
        UnitConversion::Type                tUnitConversion;    /**< (--) Nominal config data */
        double                              tNominalNoiseMult;  /**< (--) Nominal config data. */
        bool                                tPowerFlag;         /**< (--) Nominal input data. */
        double                              tTruthInput;        /**< (--) Nominal input data. */
        double                              tTimeStep;          /**< (--) Time step size for this test */

        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsSensorAnalogWrapper(const UtGunnsSensorAnalogWrapper& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsSensorAnalogWrapper& operator =(const UtGunnsSensorAnalogWrapper& that);
};

///@}

#endif
