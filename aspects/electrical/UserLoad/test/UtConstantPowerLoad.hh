#ifndef UtConstantPowerLoad_exists
#define UtConstantPowerLoad_exists

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_CONSTANT_POWER_USER_LOAD  Gunns Electrical Constant Power User Load Unit test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2021 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns User Load model
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "aspects/electrical/UserLoad/ConstantPowerLoad.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from ConstantPowerLoad and befriend UtConstantPowerLoad.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyConstantPowerLoad : public ConstantPowerLoad
{
public:
    FriendlyConstantPowerLoad();
    virtual ~FriendlyConstantPowerLoad();
    friend class UtConstantPowerLoad;
};

inline FriendlyConstantPowerLoad::FriendlyConstantPowerLoad() : ConstantPowerLoad() {};
inline FriendlyConstantPowerLoad::~FriendlyConstantPowerLoad() {};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Constant Power User Load basic unit tests.
////
/// @details  This class provides the unit tests for the Base Valve within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtConstantPowerLoad: public CppUnit::TestFixture
{
public:
    UtConstantPowerLoad();

    virtual ~UtConstantPowerLoad();

    /// @brief    Executes before each test.
    void setUp();

    /// @brief    Executes after each test.
    void tearDown();

    /// @brief    Tests default construction.
    void testDefaultConstruction();

    /// @brief    Tests initialize() method.
    void testInitialization();

    /// @brief    Tests initialize() method.
    void testConfig();

    /// @brief    Tests initialize() method.
    void testInput();

    /// @brief    Tests appropriate exceptions are thrown during validation.
    void testValidation();

    /// @brief    Tests appropriate exceptions are thrown during validation.
    void testStandbyPowerValidation();

    /// @brief    Tests updateState() method, called within BasicLink::step().
    void testUpdateState();

    /// @brief    Tests ComputeActualPower() method.
    void testComputeActualPowerMalfOverrideCurrentValueEqualToZero();

    /// @brief    Tests CalculateConstantPowerLoad() method, when overwrite power malfunction is set
    void testCalculateConstantPowerLoadMalfOverrideCurrentValueLessThanZero();

    void testCalculateConstantPowerLoadNormalPowerEqualToZero();
    void testCalculateConstantPowerLoadStandByPowerEqualToZero();
    void testComputeActualPowerMalfOverridePowerValueEqualToZero();
    void testCalculateConstantPowerLoadMalfOverridePowerValueLessThanZero();
    void testCalculateConstantPowerLoadMalfOverridePowerValueGreaterThanZero();
    void testSetters();

private:
    CPPUNIT_TEST_SUITE(UtConstantPowerLoad);
    CPPUNIT_TEST(testConfig);
    CPPUNIT_TEST(testInput);
    CPPUNIT_TEST(testDefaultConstruction);
    CPPUNIT_TEST(testInitialization);
    CPPUNIT_TEST(testValidation);
    CPPUNIT_TEST(testStandbyPowerValidation);
    CPPUNIT_TEST(testUpdateState);
    CPPUNIT_TEST(testComputeActualPowerMalfOverrideCurrentValueEqualToZero);
    CPPUNIT_TEST(testCalculateConstantPowerLoadMalfOverrideCurrentValueLessThanZero);
    CPPUNIT_TEST(testCalculateConstantPowerLoadNormalPowerEqualToZero);
    CPPUNIT_TEST(testCalculateConstantPowerLoadStandByPowerEqualToZero);
    CPPUNIT_TEST(testComputeActualPowerMalfOverridePowerValueEqualToZero);
    CPPUNIT_TEST(testCalculateConstantPowerLoadMalfOverridePowerValueLessThanZero);
    CPPUNIT_TEST(testCalculateConstantPowerLoadMalfOverridePowerValueGreaterThanZero);
    CPPUNIT_TEST(testSetters);
    CPPUNIT_TEST_SUITE_END();

    ///  Pointer to nominal configuration data
    ConstantPowerLoadConfigData* tConfigData;

    /// Pointer to nominal input data
    ConstantPowerLoadInputData*  tInputData;

    ///  Test Article
    FriendlyConstantPowerLoad*   tArticle;

    ///  load Name
    std::string  tloadName;

    /// @brief -- cardId
    int tCardId;

    /// @brief -- load switch id
    int tLoadSwitchId;

    /// total actual power
    double tActualPower;

    /// set the current value
    double tCurrent;

    /// set equivalence resistance
    double tEquivalentResistance;

    /// -- set to the enum value of the LOAD_TYPE
    int tUserLoadType; // -- enum value

    /// set the normal power value
    double tPowerNormal;

    /// set the stanby power value
    double tPowerStandby;

    /// @brief   -- Blockage malfunction fractional value (0-1)
    /// double  tMalfBlockageValue;


    /// --     Link Capacitance
    double  tInitialPotential;

    /// @brief   --   User Loads
    std::vector<UserLoadBase*>  tnetworkLoads;

    /// -- under voltage trip limit set point value
    double tUnderVoltageLimit; // minimum voltage at which this load trips
    double tFuseCurrentLimit; /**< (amp) Current above which the fuse blows. */

    bool tPowerValid; // -- boolean for min voltage value
    bool tLoadOperMode;   // -- boolean value for Load On/Off
    double tInitialVoltage; /**< (v) Initial voltage input data. */

    /// -- Initialization status flag, T = good
    bool tInitFlag;   // -- Link Init Flag

    /// @brief   -- Nominal tolerance for comparison of expected and returned values
    double  tTolerance;

    /// Flag to overwrite the constant resistance power value.
    bool tMalfOverrideCurrentFlag; // -- overwrite power flag

    /// The overwrite power value for resistance load. Default value set to zero.
    double tMalfOverrideCurrentValue;  // -- overwrite power value

    /// Test identification number
    static int TEST_ID;

    /// @brief Copy constructor unavailable since declared private and not implemented.
    UtConstantPowerLoad(const UtConstantPowerLoad& that);

    /// @brief Assignment operator unavailable since declared private and not implemented.
    UtConstantPowerLoad& operator =(const UtConstantPowerLoad& that);

};

///@}

#endif
