#ifndef UtGunnsElectResistiveUserLoad_EXISTS
#define UtGunnsElectResistiveUserLoad_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_RESISTIVE_USER_LOAD_SPOTTER   Electrical Resistive User Load Spotter Unit Tests
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2021 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Electrical Resistive User Load Spotter.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/electrical/UserLoad/GunnsElectResistiveUserLoad.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsElectResistiveUserLoad and befriend UtGunnsElectResistiveUserLoad.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsElectResistiveUserLoad : public GunnsElectResistiveUserLoad
{
    public:
        FriendlyGunnsElectResistiveUserLoad(GunnsElectResistiveUserLoadConfigData* configData = 0,
                                            GunnsElectResistiveUserLoadInputData*  inputData  = 0);
        virtual ~FriendlyGunnsElectResistiveUserLoad();
        friend class UtGunnsElectResistiveUserLoad;
};
inline FriendlyGunnsElectResistiveUserLoad::FriendlyGunnsElectResistiveUserLoad(
        GunnsElectResistiveUserLoadConfigData* configData,
        GunnsElectResistiveUserLoadInputData*  inputData) : GunnsElectResistiveUserLoad(configData, inputData) {;}
inline FriendlyGunnsElectResistiveUserLoad::~FriendlyGunnsElectResistiveUserLoad() {;}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test implementation of GunnsNetworkSpotterConfigData.
///
/// @details  Derives from GunnsNetworkSpotterConfigData and is used to test that a dynamic_cast of
///           this type to the GunnsElectResistiveUserLoadConfigData test article type fails.
////////////////////////////////////////////////////////////////////////////////////////////////////
class BadGunnsElectResistiveUserLoadConfigData : public GunnsNetworkSpotterConfigData
{
    public:
        BadGunnsElectResistiveUserLoadConfigData(const std::string& name) : GunnsNetworkSpotterConfigData(name) {}
        virtual ~BadGunnsElectResistiveUserLoadConfigData() {}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test implementation of GunnsNetworkSpotterInputData.
///
/// @details  Derives from GunnsNetworkSpotterInputData and is used to test that a dynamic_cast of
///           this type to the GunnsElectResistiveUserLoadInputData test article type fails.
////////////////////////////////////////////////////////////////////////////////////////////////////
class BadGunnsElectResistiveUserLoadInputData : public GunnsNetworkSpotterInputData
{
    public:
        BadGunnsElectResistiveUserLoadInputData() {}
        virtual ~BadGunnsElectResistiveUserLoadInputData() {}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Valve unit tests.
////
/// @details  This class provides the unit tests for the GUNNS Electrical Resistive User Load
///           Spotter within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsElectResistiveUserLoad: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Check Valve unit test.
        UtGunnsElectResistiveUserLoad();
        /// @brief    Default constructs this Check Valve unit test.
        virtual ~UtGunnsElectResistiveUserLoad();
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
        CPPUNIT_TEST_SUITE(UtGunnsElectResistiveUserLoad);
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
        std::string                            tName;              /**< (--)  Nominal name. */
        double                                 tUnderVoltageLimit; /**< (v)   Nominal config data. */
        double                                 tResistanceNormal;  /**< (ohm) Nominal config data. */
        double                                 tResistanceStandby; /**< (ohm) Nominal config data. */
        double                                 tFuseCurrentLimit;  /**< (amp) Nominal config data. */
        double                                 tDutyCycleFraction; /**< (--)  Nominal config data. */
        double                                 tDutyCyclePeriod;   /**< (s)   Nominal config data. */
        GunnsElectResistiveUserLoadConfigData* tConfigData;        /**< (--)  Pointer to nominal configuration data. */
        int                                    tInitialMode;       /**< (--)  Nominal input data. */
        double                                 tInitialVoltage;    /**< (--)  Nominal input data. */
        double                                 tDutyCycleTimer;    /**< (s)   Nominal input data. */
        GunnsElectResistiveUserLoadInputData*  tInputData;         /**< (--)  Pointer to nominal input data. */
        FriendlyGunnsElectResistiveUserLoad*   tArticle;           /**< (--)  Pointer to the article under test. */
        static int                             TEST_ID;            /**< (--)  Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsElectResistiveUserLoad(const UtGunnsElectResistiveUserLoad&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsElectResistiveUserLoad& operator =(const UtGunnsElectResistiveUserLoad&);
};

///@}

#endif
