/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
 LIBRARY DEPENDENCY:
    ((aspects/electrical/UserLoad/test/DerivedResistanceLoadExample.o))
 ***************************************************************************************************/
#include "aspects/electrical/UserLoad/UserLoadBase.hh"
#include "UtDerivedResistanceLoadExample.hh"
#include <iostream>

#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "strings/UtResult.hh"

/// @details  Test identification number.
int UtDerivedResistanceLoadExample::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsThermalCapacitor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtDerivedResistanceLoadExample::UtDerivedResistanceLoadExample()
:
tConfigData(),
tInputData(),
tArticle(),
tloadName(""),
tUserLoadType(RESISTIVE_LOAD),
tUnderVoltageLimit(0.0),
tResistanceNormal(10000000),
tResistanceStandby(100000),
tPrllResistance1(),
tPrllResistance2(),
tCardId(0),
tLoadSwitchId(0),
tActualPower(0.0),
tCurrent(0.0),
tnetworkLoads(),
tEquivalentResistance(0.0),
tInitialPotential(0.0),
tPowerValid(true),
tLoadOperMode(LoadON),
tTolerance(0),
tMalfOverrideCurrentFlag(),
tMalfOverrideCurrentValue()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsThermalCapacitor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtDerivedResistanceLoadExample::~UtDerivedResistanceLoadExample()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDerivedResistanceLoadExample::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tInputData;
    delete tConfigData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDerivedResistanceLoadExample::setUp()
{
    /// - Declare the standard user load test data

    /// - Declare the ResistiveLoad specific test data
    tloadName          = "TestDerivedResistance Load Example";
    tInitialPotential  = 322;
    tUnderVoltageLimit = 90.0;
    tUserLoadType = RESISTIVE_LOAD;
    tResistanceNormal = 100000000.0;  // value is 1.0E8
    tResistanceStandby = 1000000.0;   // value is 1.0E6
    tPrllResistance1 = 500000;
    tPrllResistance2 = 500000;

    tLoadOperMode = LoadON;
    tCardId = 0;
    tLoadSwitchId = 0;
    tTolerance = 1.0e-08;
    tMalfOverrideCurrentFlag = false;
    tMalfOverrideCurrentValue = 12.0;
    tActualPower = 40.0; // initialize actual power data
    tCurrent = 50.0;  // set it to 50 amps


    /// - Define nominal configuration data
    tConfigData = new DerivedResistanceLoadExampleConfigData(tloadName, tUserLoadType,
            tUnderVoltageLimit, tResistanceNormal, tResistanceStandby,
            tPrllResistance1, tPrllResistance2);
    tInputData = new DerivedResistanceLoadExampleInputData(tMalfOverrideCurrentFlag, tMalfOverrideCurrentValue, tLoadOperMode);

    /// - Create new test Article
    tArticle = new FriendlyDerivedResistanceLoadExample();


    try {
        /// - Initialize the test article.
        tArticle->initialize(*tConfigData, *tInputData, tnetworkLoads, tCardId, tLoadSwitchId);

    } catch (TsInitializationException &e) {
        std::cout << "UtDerivedResistanceLoadExample Setup method initialization exception -- failed" << std::endl;
    } catch (...) {
        std::cout << "UtDerivedResistanceLoadExample Setup method catch all exception -- failed" << std::endl;
    }

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDerivedResistanceLoadExample::testConfig()
{
    UT_RESULT_FIRST;

    /// @test   Nominal config construction
    CPPUNIT_ASSERT(tloadName == tConfigData->mName);

    /// @test   Nominal config construction
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tResistanceNormal, tConfigData->mResistanceNormal, tTolerance);

    /// @test   Nominal config construction
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tResistanceStandby, tConfigData->mResistanceStandby, tTolerance);


    /// @test   Check default config construction
    DerivedResistanceLoadExampleConfigData defaultConfig;
    CPPUNIT_ASSERT("" != defaultConfig.mName);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0E6, defaultConfig.mResistanceNormal, tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0E8, defaultConfig.mResistanceStandby, tTolerance);

    /// @test   Check copy config construction
    DerivedResistanceLoadExampleConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tloadName == copyConfig.mName);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tResistanceNormal, copyConfig.mResistanceNormal, tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tResistanceStandby, copyConfig.mResistanceStandby, tTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDerivedResistanceLoadExample::testInput()
{
    UT_RESULT;

    /// - Check nominal input construction
    /// - Check nominal input construction
    // Work on this block when Input data class is available
    CPPUNIT_ASSERT(!tInputData->mMalfOverrideCurrentFlag);
    CPPUNIT_ASSERT(tMalfOverrideCurrentValue  == tInputData->mMalfOverrideCurrentValue);


    /// - Check copy input construction
    // not being tested until input data is available
    DerivedResistanceLoadExampleInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tInputData->mMalfOverrideCurrentFlag  == copyInput.mMalfOverrideCurrentFlag);
    CPPUNIT_ASSERT(tInputData->mMalfOverrideCurrentValue == copyInput.mMalfOverrideCurrentValue);
    CPPUNIT_ASSERT(tInputData->mLoadOperMode == copyInput.mLoadOperMode);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDerivedResistanceLoadExample::testDefaultConstruction()
{
    UT_RESULT;

    /// - Construct an uninitialized test article
    FriendlyDerivedResistanceLoadExample article;

    // try to initialize the data
    article.initialize(*tConfigData, *tInputData, tnetworkLoads, tCardId, tLoadSwitchId);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDerivedResistanceLoadExample::testInitialization()
{
    UT_RESULT;

    /// @test   config and input values
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tResistanceNormal, tArticle->mResistanceNormal, tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tUnderVoltageLimit,   tArticle->mUnderVoltageLimit, tTolerance);

    CPPUNIT_ASSERT(tResistanceStandby == tArticle->mResistanceStandby);
    CPPUNIT_ASSERT(tloadName == tArticle->getName());
    CPPUNIT_ASSERT(tLoadOperMode == tArticle->mLoadOperMode);
    CPPUNIT_ASSERT(tUserLoadType == tArticle->getLoadType());
    CPPUNIT_ASSERT(tCardId == tArticle->getCardId());
    CPPUNIT_ASSERT(tLoadSwitchId == tArticle->getLoadSwitchId());


    /// @test   input load type
    CPPUNIT_ASSERT_MESSAGE("Load Type is equal.",
            tUserLoadType == tArticle->mUserLoadType);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test that bad data produces the appropriate exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDerivedResistanceLoadExample::testValidation()
{
    UT_RESULT;

    /// @test validation of load type
    tConfigData->mUserLoadType = 3; // something other than constant power or constant resistance

    /// - Initialize the test article.
    CPPUNIT_ASSERT_THROW(tArticle->initialize( *tConfigData, *tInputData, tnetworkLoads,
                                       tCardId, tLoadSwitchId), TsInitializationException);

    // reset load type to constant resistance
    tConfigData->mUserLoadType = RESISTIVE_LOAD;

    /// @test validation of operation mode
    tInputData->mLoadOperMode = 4; // value other than the assigned modes

    /// - Initialize the test article.
    CPPUNIT_ASSERT_THROW(tArticle->initialize( *tConfigData, *tInputData, tnetworkLoads,
                                       tCardId, tLoadSwitchId), TsInitializationException);

    // reset operation mode to ON
    tInputData->mLoadOperMode = LoadON;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests updateState() method, called within BasicLink::step().
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDerivedResistanceLoadExample::testUpdateState()
{
    UT_RESULT;

    /// - Placeholder for last-step potential
    double tPreviousPotential = tInitialPotential;

    CPPUNIT_ASSERT(tArticle->mResistanceNormal > tPrllResistance1);

    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// - call step()
    tArticle->step(tPreviousPotential);

    CPPUNIT_ASSERT(tArticle->mResistanceNormal < tPrllResistance1);

    // manually reset the resistance value and calculate the load again.
    // The resistance will be overwritten by updateLoad
    tArticle->mResistanceNormal = 300.0;


    /// - call updateState(), which calculates a new Potential[0]
    tArticle->step(tPreviousPotential);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(250000.0, tArticle->mResistanceNormal, tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(250000.0, tArticle->mEquivalentResistance, tTolerance);

    /// @test    With equal heat fluxes the temperature should not change.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tPreviousPotential, tArticle->mVoltage, tTolerance);

    /// - save previous Potential
    tPreviousPotential = tArticle->mVoltage;

    /// - call updateState(), which calculates a new Potential[0]
    tArticle->step(tPreviousPotential);

    CPPUNIT_ASSERT_MESSAGE("Net resistance calculated.",
            tArticle->getResistance() >= tArticle->mResistanceNormal);

    /// - save previous Potential
    tPreviousPotential = tArticle->mVoltage;

    /// - call updateState(), which calculates a new Potential[0]
    // reset the initial potential tInitialPotential to 135.0
    tInitialPotential = 135.0;

    // step through to perform the calculations
    tArticle->step(tInitialPotential);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->getResistance(), tArticle->mResistanceNormal, tTolerance);

    // power calculated here is very high
    CPPUNIT_ASSERT_MESSAGE("Actual power is low!", tArticle->getPower() < tActualPower);

    // current calculated here is very high
    CPPUNIT_ASSERT_MESSAGE("Actual current here is very low!", tArticle->getCurrent() < tCurrent);

    //set load to standby and calculate power and current
    tArticle->mLoadOperMode = LoadSTANDBY;
    tArticle->mResistanceStandby = 30000.0;

    // step through to perform the calculations
    tArticle->step(tInitialPotential);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->getResistance(), tArticle->mResistanceStandby, tTolerance);

    // power calculated here is less than (40.0)
    CPPUNIT_ASSERT_MESSAGE("Actual power is very low!", tArticle->getPower() < tActualPower);

    // current calculated here is less than (50.0)
    CPPUNIT_ASSERT_MESSAGE("Actual current here is very low!", tArticle->getCurrent() < tCurrent);

    tArticle->mLoadOperMode = LoadOFF;

    // step through to perform the calculations
    tArticle->step(tInitialPotential);

    //when load is off then power and current are very low as resistance is maximum
    // current calculated here is less than (50.0)
    CPPUNIT_ASSERT_MESSAGE("Actual power is very low!", tArticle->getPower() == 0.0);

    // power calculated here is less than (40.0)
    CPPUNIT_ASSERT_MESSAGE("Actual current here is very low!", tArticle->getCurrent() == 0.0);

    //reset load to ON and set the overwrite flag to true
    tArticle->mLoadOperMode = LoadON;
    tArticle->mMalfOverrideCurrentFlag = true;

    // step through to perform the calculations
    tArticle->step(tInitialPotential);

    CPPUNIT_ASSERT_MESSAGE("Override resistance calculated is less than normal value!",
            tArticle->getResistance() < tArticle->mResistanceNormal);

    //when load is off then power and current are very low as resistance is maximum
    // current calculated here is less than (50.0)
    CPPUNIT_ASSERT_MESSAGE("Actual power is very low!", tArticle->getPower() > tActualPower);

    // power calculated here is less than (40.0)
    CPPUNIT_ASSERT_MESSAGE("Actual current here is very low!", tArticle->getCurrent() < tCurrent);

    UT_PASS_LAST;
}



