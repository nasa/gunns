#ifndef UtGunnsThermalCapacitorHeatQueues_EXISTS
#define UtGunnsThermalCapacitorHeatQueues_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_THERMAL_CAP_HEAT_QUEUES    Gunns Thermal Capacitor Heat Queues Spotter Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2020 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Thermal Capacitor Heat Queues Spotter class
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/thermal/GunnsThermalCapacitorHeatQueues.hh"
#include "UtGunnsThermalCapacitor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsThermalCapacitorHeatQueues and befriend UtGunnsThermalCapacitorHeatQueues.
///
/// @details  Class derived from the unit under test.  It has a constructor with the same arguments
///           as the parent and a default destructor, but it befriends the unit test case driver
///           class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsThermalCapacitorHeatQueues : public GunnsThermalCapacitorHeatQueues
{
    public:
        FriendlyGunnsThermalCapacitorHeatQueues(GunnsThermalCapacitor& capacitor)
            : GunnsThermalCapacitorHeatQueues(capacitor) {};
        virtual ~FriendlyGunnsThermalCapacitorHeatQueues() {;}
        friend class UtGunnsThermalCapacitorHeatQueues;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test implementation of GunnsNetworkSpotterConfigData.
///
/// @details  Derives from GunnsNetworkSpotterConfigData and is used to test that a dynamic_cast of
///           this type to the GunnsThermalCapacitorHeatQueuesConfigData test article type can fail.
////////////////////////////////////////////////////////////////////////////////////////////////////
class BadGunnsThermalCapacitorHeatQueuesConfigData : public GunnsNetworkSpotterConfigData
{
    public:
        BadGunnsThermalCapacitorHeatQueuesConfigData(const std::string& name) : GunnsNetworkSpotterConfigData(name) {}
        virtual ~BadGunnsThermalCapacitorHeatQueuesConfigData() {}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test implementation of GunnsNetworkSpotterInputData.
///
/// @details  Derives from GunnsNetworkSpotterInputData and is used to test that a dynamic_cast of
///           this type to the GunnsThermalCapacitorHeatQueuesInputData test article type can fail.
////////////////////////////////////////////////////////////////////////////////////////////////////
class BadGunnsThermalCapacitorHeatQueuesInputData : public GunnsNetworkSpotterInputData
{
    public:
        BadGunnsThermalCapacitorHeatQueuesInputData() {}
        virtual ~BadGunnsThermalCapacitorHeatQueuesInputData() {}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Thermal Capacitor Heat Queues Spotter Unit Tests.
///
/// @details  This class provides the unit tests for the GunnsThermalCapacitorHeatQueues class within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsThermalCapacitorHeatQueues : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this GunnsThermalCapacitorHeatQueues unit test.
        UtGunnsThermalCapacitorHeatQueues();
        /// @brief    Default destructs this GunnsThermalCapacitorHeatQueues unit test.
        virtual ~UtGunnsThermalCapacitorHeatQueues();
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
        /// @brief    Tests the stepPreSolver method.
        void testPreSolver();
        /// @brief    Tests the stepPostSolver method.
        void testPostSolver();
        /// @brief    Tests the setter and getter methods.
        void testAccessors();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsThermalCapacitorHeatQueues);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testInitialize);
        CPPUNIT_TEST(testPreSolver);
        CPPUNIT_TEST(testPostSolver);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST_SUITE_END();
        FriendlyGunnsThermalCapacitorHeatQueues   tArticle;   /**< (--) Test article. */
        std::string                               tName;      /**< (--) Instance name. */
        GunnsThermalCapacitorHeatQueuesConfigData tConfig;    /**< (--) Nominal config data. */
        GunnsThermalCapacitorHeatQueuesInputData  tInput;     /**< (--) Nominal input data. */
        FriendlyGunnsThermalCapacitor             tCapacitor; /**< (--) Test thermal capacitor link. */
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsThermalCapacitorHeatQueues(const UtGunnsThermalCapacitorHeatQueues& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsThermalCapacitorHeatQueues& operator =(const UtGunnsThermalCapacitorHeatQueues& that);
};

///@}

#endif
