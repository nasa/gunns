#ifndef UtResistiveLoad_exists
#define UtResistiveLoad_exists

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_RESISTIVE_LOAD  Gunns Electrical Resistive Load Unit test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns User Load model
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>
#include "aspects/electrical/UserLoad/UserLoadBase.hh"
#include "aspects/electrical/UserLoad/ResistiveLoad.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from ResistiveLoad and befriend UtResistiveLoad.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyResistiveLoad : public ResistiveLoad
{
public:
    FriendlyResistiveLoad();
    virtual ~FriendlyResistiveLoad();
    friend class UtResistiveLoad;
};

inline FriendlyResistiveLoad::FriendlyResistiveLoad() : ResistiveLoad() {};
inline FriendlyResistiveLoad::~FriendlyResistiveLoad() {};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Constant Power User Load basic unit tests.
////
/// @details  This class provides the unit tests for the Base Valve within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtResistiveLoad: public CppUnit::TestFixture
{
public:
    UtResistiveLoad();

    virtual ~UtResistiveLoad();

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
    void testStanbyResistanceValidation();

    /// @brief    Tests appropriate exceptions are thrown during validation.
    void testValidation();


    /// @brief    Tests updateState() method, called within BasicLink::step().
    void testUpdateState();
    void testCalculateResistiveLoadMalfOverrideCurrentValueEqualToZero();
    void testCalculateResistiveLoadMalfOverrideCurrentValueLessThanZero();
    void testComputeActualPowerNormalResistanceEqualToZero();
    void testComputeActualPowerStandbyResistanceEqualToZero();
    void testCalculateResistiveLoadMalfOverridePowerValueEqualToZero();
    void testCalculateResistiveLoadMalfOverridePowerValueLessThanZero();
    void testCalculateResistiveLoadMalfOverridePowerValueGreaterThanZero();
    void testUpdateFuse();

private:
    CPPUNIT_TEST_SUITE(UtResistiveLoad);
    CPPUNIT_TEST(testConfig);
    CPPUNIT_TEST(testInput);
    CPPUNIT_TEST(testDefaultConstruction);
    CPPUNIT_TEST(testInitialization);
    CPPUNIT_TEST(testValidation);
    CPPUNIT_TEST(testStanbyResistanceValidation);
    CPPUNIT_TEST(testUpdateState);
    CPPUNIT_TEST(testCalculateResistiveLoadMalfOverrideCurrentValueEqualToZero);
    CPPUNIT_TEST(testCalculateResistiveLoadMalfOverrideCurrentValueLessThanZero);
    CPPUNIT_TEST(testComputeActualPowerNormalResistanceEqualToZero);
    CPPUNIT_TEST(testComputeActualPowerStandbyResistanceEqualToZero);
    CPPUNIT_TEST(testCalculateResistiveLoadMalfOverridePowerValueEqualToZero);
    CPPUNIT_TEST(testCalculateResistiveLoadMalfOverridePowerValueLessThanZero);
    CPPUNIT_TEST(testCalculateResistiveLoadMalfOverridePowerValueGreaterThanZero);
    CPPUNIT_TEST(testUpdateFuse);
    CPPUNIT_TEST_SUITE_END();

    /// --     Pointer to nominal configuration data
    ResistiveLoadConfigData* tConfigData;

    /// --     Pointer to nominal input data
    ResistiveLoadInputData*  tInputData;

    /// --     Test Article
    FriendlyResistiveLoad*   tArticle;

    /// --     load Name
    std::string  tloadName;

    /// @brief -- cardId
    int tCardId;

    /// @brief -- load switch id
    int tLoadSwitchId;

    /// -- total actual power
    double tActualPower;

    // -- calculated current for the data value
    double tCurrent;

    /// -- set to the enum value of the LOAD_TYPE
    int tUserLoadType; // -- enum value


    /// @brief  User Loads
    std::vector<UserLoadBase*>  tnetworkLoads;

    double tResistanceNormal;

    double tResistanceStandby;

    /// @brief Equivalent Resistance for this load
    double tEquivalentResistance; // -- the equivalent resistance set during model update.

    /// @brief  Link Capacitance
    double  tInitialPotential;

    /// @brief under voltage trip limit set point value
    double tUnderVoltageLimit; // minimum voltage at which this load trips
    double tFuseCurrentLimit; /**< (amp) Current above which the fuse blows. */

    bool tPowerValid; // -- boolean for min voltage value

    /// Initialization operation mode for the load
    int tLoadOperMode;   // -- enum value for Load off/on/standby

    double tInitialVoltage; /**< (v) Initial voltage input data. */

    /// @brief   -- Nominal tolerance for comparison of expected and returned values
    double  tTolerance;

    /// Flag to overwrite the constant resistance power value.
    bool tMalfOverrideCurrentFlag; // -- overwrite power flag

    /// The overwrite power value for resistance load. Default value set to zero.
    double tMalfOverrideCurrentValue;  // -- overwrite power value

    /// Test identification number
    static int TEST_ID;

    /// @brief Copy constructor unavailable since declared private and not implemented.
    UtResistiveLoad(const UtResistiveLoad& that);

    /// @brief Assignment operator unavailable since declared private and not implemented.
    UtResistiveLoad& operator =(const UtResistiveLoad& that);

};

///@}

#endif
