#ifndef UtGunnsThermoelectricDevice_EXISTS
#define UtGunnsThermoelectricDevice_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup    UT_GUNNS_THERMAL_RADIATION  GUNNS Thermoelectric Device Unit test
/// @ingroup     UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details     Unit Tests for GUNNS Thermoelectric Device
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "aspects/thermal/GunnsThermoelectricDevice.hh"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsThermoelectricDevice and befriend UtGunnsThermoelectricDevice.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsThermoelectricDevice : public GunnsThermoelectricDevice
{
    public:
        FriendlyGunnsThermoelectricDevice();
        virtual ~FriendlyGunnsThermoelectricDevice();
        friend class UtGunnsThermoelectricDevice;
};
inline FriendlyGunnsThermoelectricDevice::FriendlyGunnsThermoelectricDevice() : GunnsThermoelectricDevice() {};
inline FriendlyGunnsThermoelectricDevice::~FriendlyGunnsThermoelectricDevice() {};

// Forward declare classes for pointer types.
class FriendlyGunnsThermoelectricEffect;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Thermoelectric Device unit tests.
///
/// @details  This class provides unit tests for the Thermoelectric Device within the CPPUnit
///           framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsThermoelectricDevice: public CppUnit::TestFixture
{
    public:
        /// @brief    Default Constructor.
        UtGunnsThermoelectricDevice();
        /// @brief    Default Destructor.
        virtual ~UtGunnsThermoelectricDevice();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests ConfigData construction.
        void testConfig();
        /// @brief    Tests InputData  construction.
        void testInput();
        /// @brief    Tests Default Construction.
        void testDefaultConstruction();
        /// @brief    Tests initialize method without exceptions.
        void testInitialization();
        /// @brief    Tests initialize method with exceptions.
        void testInitializationExceptions();
        /// @brief    Tests restart method.
        void testRestart();
        /// @brief    Tests step method.
        void testStep();
        /// @brief    Tests computeFlows method.
        void testComputeFlows();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsThermoelectricDevice);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST_SUITE_END();
        GunnsThermoelectricDeviceConfigData* tConfigData;                      /**< (--)    Pointer to nominal configuration data. */
        GunnsThermoelectricDeviceInputData*  tInputData;                       /**< (--)    Pointer to nominal input data. */
        FriendlyGunnsThermoelectricDevice*   tArticle;                         /**< (--)    Pointer to the test article. */
        FriendlyGunnsThermoelectricEffect*   tArticleEffect;                   /**< (--)    Pointer to the test article's thermoelectric effect object. */
        std::string                          tName;                            /**< (--)    Link Name. */
        GunnsBasicNode                       tNodes[2];                        /**< (--)    Network Nodes. */
        GunnsNodeList                        tNodeList;                        /**< (--)    Node List. */
        std::vector<GunnsBasicLink*>         tLinks;                           /**< (--)    Network Links.  */
        double                               tNumThermocouples;                /**< (--)    Nominal config data. */
        double                               tGeometryFactor;                  /**< (m)     Nominal config data. */
        double                               tElectricalResistivityCoeff[2];   /**< (ohm*m) Nominal config data. */
        double                               tSeebeckCoeff[3];                 /**< (v/K)   Nominal config data. */
        double                               tThermalConductivityCoeff[3];     /**< (W/m/K) Nominal config data. */
        double                               tEndPlateThermalConductance;      /**< (W/K)   Nominal config data. */
        double                               tMinTemperature;                  /**< (K)     Nominal config data. */
        double                               tMaxTemperature;                  /**< (K)     Nominal config data. */
        bool                                 tMalfBlockageFlag;                /**< (--)    Nominal input data. */
        double                               tMalfBlockageValue;               /**< (--)    Nominal input data. */
        bool                                 tMalfThermoelectricEffectsFlag;   /**< (--)    Nominal input data. */
        double                               tMalfThermoelectricEffectsScalar; /**< (--)    Nominal input data. */
        int                                  tPort0;                           /**< (--)    Nominal inlet port index. */
        int                                  tPort1;                           /**< (--)    Nominal outlet port index. */
        double                               tTimeStep;                        /**< (s)     Nominal time step. */
        static int                           TEST_ID;                          /**< (--)    Test identification number. */
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsThermoelectricDevice(const UtGunnsThermoelectricDevice& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsThermoelectricDevice& operator =(const UtGunnsThermoelectricDevice& that);
};

///@}

#endif
