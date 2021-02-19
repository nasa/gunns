#ifndef UtDerivedResistanceLoadExample_exists
#define UtDerivedResistanceLoadExample_exists

/************************** TRICK HEADER ***********************************************************
 * @defgroup UT_DERIVED_RESISTIVE_LOAD_EXAMPLE  Gunns Electrical Derived Resistive Load Example Unit test
 * @ingroup  UT_GUNNS
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
 LIBRARY DEPENDENCY:
    ((aspects/electrical/UserLoad/test/DerivedResistanceLoadExample.o))

* @details  Unit Tests for the Gunns User Load model
* @{
***************************************************************************************************/

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>
#include "aspects/electrical/UserLoad/ResistiveLoad.hh"
#include "aspects/electrical/UserLoad/test/DerivedResistanceLoadExample.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from ResistiveLoad and befriend UtDerivedResistanceLoadExample.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyDerivedResistanceLoadExample : public DerivedResistanceLoadExample
{
public:
    FriendlyDerivedResistanceLoadExample();
    virtual ~FriendlyDerivedResistanceLoadExample();
    friend class UtDerivedResistanceLoadExample;
};

inline FriendlyDerivedResistanceLoadExample::FriendlyDerivedResistanceLoadExample() : DerivedResistanceLoadExample() {};
inline FriendlyDerivedResistanceLoadExample::~FriendlyDerivedResistanceLoadExample() {};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Constant Power User Load basic unit tests.
////
/// @details  This class provides the unit tests for the Base Valve within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtDerivedResistanceLoadExample: public CppUnit::TestFixture
{
public:
    UtDerivedResistanceLoadExample();

    virtual ~UtDerivedResistanceLoadExample();

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

    /// @brief    Tests updateState() method, called within BasicLink::step().
    void testUpdateState();

private:
    CPPUNIT_TEST_SUITE(UtDerivedResistanceLoadExample);
    CPPUNIT_TEST(testConfig);
    CPPUNIT_TEST(testInput);
    CPPUNIT_TEST(testDefaultConstruction);
    CPPUNIT_TEST(testInitialization);
    CPPUNIT_TEST(testValidation);
    CPPUNIT_TEST(testUpdateState);
    CPPUNIT_TEST_SUITE_END();

    /// --     Pointer to nominal configuration data
    DerivedResistanceLoadExampleConfigData* tConfigData;

    /// --     Pointer to nominal input data
    DerivedResistanceLoadExampleInputData*  tInputData;

    /// --     Test Article
    FriendlyDerivedResistanceLoadExample*  tArticle;

    /// --     load Name
    std::string  tloadName;
    /// -- set to the enum value of the LOAD_TYPE
    int tUserLoadType;
    /// @brief under voltage trip limit set point value
    double tUnderVoltageLimit; // minimum voltage at which this load trips

    double tResistanceNormal;
    double tResistanceStandby;
    double tPrllResistance1;
    double tPrllResistance2;

    /// @brief -- cardId
    int tCardId;
    /// @brief -- load switch id
    int tLoadSwitchId;

    /// -- total actual power
    double tActualPower;

    // -- calculated current for the data value
    double tCurrent;


    /// @brief  User Loads
    std::vector<UserLoadBase*>  tnetworkLoads;

    /// @brief Equivalent Resistance for this load
    double tEquivalentResistance; // -- the equivalent resistance set during model update.

    /// @brief  Link Capacitance
    double  tInitialPotential;

    bool tPowerValid; // -- boolean for min voltage value

    /// Initialization operation mode for the load
    int tLoadOperMode;   // -- enum value for Load off/on/standby

    /// @brief   -- Nominal tolerance for comparison of expected and returned values
    double  tTolerance;

    /// Flag to overwrite the constant resistance power value.
    bool tMalfOverrideCurrentFlag; // -- overwrite power flag

    /// The overwrite power value for resistance load. Default value set to zero.
    double tMalfOverrideCurrentValue;  // -- overwrite power value

    /// Test identification number
    static int TEST_ID;

    /// @brief Copy constructor unavailable since declared private and not implemented.
    UtDerivedResistanceLoadExample(const UtDerivedResistanceLoadExample& that);

    /// @brief Assignment operator unavailable since declared private and not implemented.
    UtDerivedResistanceLoadExample& operator =(const UtDerivedResistanceLoadExample& that);

};

///@}

#endif
