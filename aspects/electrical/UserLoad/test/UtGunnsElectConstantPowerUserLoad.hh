#ifndef UtGunnsElectConstantPowerUserLoad_EXISTS
#define UtGunnsElectConstantPowerUserLoad_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_CONSTAT_POWER_USER_LOAD_SPOTTER   Electrical Constant-Power User Load Spotter Unit Tests
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2021 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Electrical Constant-Power User Load Spotter.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/electrical/UserLoad/GunnsElectConstantPowerUserLoad.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsElectConstantPowerUserLoad and befriend UtGunnsElectConstantPowerUserLoad.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsElectConstantPowerUserLoad : public GunnsElectConstantPowerUserLoad
{
    public:
        FriendlyGunnsElectConstantPowerUserLoad(GunnsElectConstantPowerUserLoadConfigData* configData = 0,
                                                GunnsElectConstantPowerUserLoadInputData*  inputData  = 0);
        virtual ~FriendlyGunnsElectConstantPowerUserLoad();
        friend class UtGunnsElectConstantPowerUserLoad;
};
inline FriendlyGunnsElectConstantPowerUserLoad::FriendlyGunnsElectConstantPowerUserLoad(
        GunnsElectConstantPowerUserLoadConfigData* configData,
        GunnsElectConstantPowerUserLoadInputData*  inputData) : GunnsElectConstantPowerUserLoad(configData, inputData) {;}
inline FriendlyGunnsElectConstantPowerUserLoad::~FriendlyGunnsElectConstantPowerUserLoad() {;}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test implementation of GunnsNetworkSpotterConfigData.
///
/// @details  Derives from GunnsNetworkSpotterConfigData and is used to test that a dynamic_cast of
///           this type to the GunnsElectConstantPowerUserLoadConfigData test article type fails.
////////////////////////////////////////////////////////////////////////////////////////////////////
class BadGunnsElectConstantPowerUserLoadConfigData : public GunnsNetworkSpotterConfigData
{
    public:
        BadGunnsElectConstantPowerUserLoadConfigData(const std::string& name) : GunnsNetworkSpotterConfigData(name) {}
        virtual ~BadGunnsElectConstantPowerUserLoadConfigData() {}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test implementation of GunnsNetworkSpotterInputData.
///
/// @details  Derives from GunnsNetworkSpotterInputData and is used to test that a dynamic_cast of
///           this type to the GunnsElectConstantPowerUserLoadInputData test article type fails.
////////////////////////////////////////////////////////////////////////////////////////////////////
class BadGunnsElectConstantPowerUserLoadInputData : public GunnsNetworkSpotterInputData
{
    public:
        BadGunnsElectConstantPowerUserLoadInputData() {}
        virtual ~BadGunnsElectConstantPowerUserLoadInputData() {}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Valve unit tests.
////
/// @details  This class provides the unit tests for the GUNNS Electrical Constant-Power User Load
///           Spotter within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsElectConstantPowerUserLoad: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Check Valve unit test.
        UtGunnsElectConstantPowerUserLoad();
        /// @brief    Default constructs this Check Valve unit test.
        virtual ~UtGunnsElectConstantPowerUserLoad();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests config data.
        void testConfig();
        /// @brief    Tests input data.
        void testInput();
        /// @brief    Tests default construction.
        void testDefaultConstruction();
        /// @brief    Tests nominal construction.
        void testNominalConstruction();
        /// @brief    Tests initialize method.
        void testNominalInitialization();
        /// @brief    Tests initLoad method.
        void testNominalInitLoad();
        /// @brief    Tests accessor methods.
        void testAccessors();
        /// @brief    Tests step methods.
        void testStep();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();
        /// @brief    Tests duty cycle function.
        void testDutyCycle();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsElectConstantPowerUserLoad);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testNominalInitLoad);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testDutyCycle);
        CPPUNIT_TEST_SUITE_END();
        std::string                                tName;              /**< (--)  Nominal name. */
        double                                     tUnderVoltageLimit; /**< (v)   Nominal config data. */
        double                                     tPowerNormal;       /**< (W)   Nominal config data. */
        double                                     tPowerStandby;      /**< (W)   Nominal config data. */
        double                                     tFuseCurrentLimit;  /**< (amp) Nominal config data. */
        double                                     tDutyCycleFraction; /**< (--)  Nominal config data. */
        double                                     tDutyCyclePeriod;   /**< (s)   Nominal config data. */
        GunnsElectConstantPowerUserLoadConfigData* tConfigData;        /**< (--)  Pointer to nominal configuration data. */
        int                                        tInitialMode;       /**< (--)  Nominal input data. */
        double                                     tInitialVoltage;    /**< (--)  Nominal input data. */
        double                                     tDutyCycleTimer;    /**< (s)   Nominal input data. */
        GunnsElectConstantPowerUserLoadInputData*  tInputData;         /**< (--)  Pointer to nominal input data. */
        FriendlyGunnsElectConstantPowerUserLoad*   tArticle;           /**< (--)  Pointer to the article under test. */
        static int                                 TEST_ID;            /**< (--)  Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsElectConstantPowerUserLoad(const UtGunnsElectConstantPowerUserLoad&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsElectConstantPowerUserLoad& operator =(const UtGunnsElectConstantPowerUserLoad&);
};

///@}

#endif
