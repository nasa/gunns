/*
@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
 */
#include "aspects/electrical/UserLoad/UserLoadBase.hh"
#include "UtResistiveLoad.hh"
#include <iostream>

#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsNumericalException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "strings/UtResult.hh"

/// @details  Test identification number.
int UtResistiveLoad::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsThermalCapacitor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtResistiveLoad::UtResistiveLoad()
:
tConfigData(),
tInputData(),
tArticle(),
tloadName(""),
tCardId(0),
tLoadSwitchId(0),
tActualPower(0.0),
tCurrent(0.0),
tUserLoadType(RESISTIVE_LOAD),
tnetworkLoads(),
tResistanceNormal(10000000),
tResistanceStandby(100000),
tEquivalentResistance(0.0),
tInitialPotential(0.0),
tUnderVoltageLimit(0.0),
tFuseCurrentLimit(0.0),
tPowerValid(true),
tLoadOperMode(LoadON),
tInitialVoltage(0.0),
tTolerance(0),
tMalfOverrideCurrentFlag(false),
tMalfOverrideCurrentValue(0.0)
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsThermalCapacitor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtResistiveLoad::~UtResistiveLoad()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtResistiveLoad::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tInputData;
    delete tConfigData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtResistiveLoad::setUp()
{
    /// - Declare the standard user load test data
    /// - Declare the ResistiveLoad specific test data
    tloadName          = "TestResistive Load";
    tInitialPotential  = 322;
    tUnderVoltageLimit = 90.0;
    tFuseCurrentLimit  = 5.0;
    tUserLoadType = RESISTIVE_LOAD;
    tResistanceNormal = 100000000.0;  // value is 1.0E8
    tResistanceStandby = 1000000.0;   // value is 1.0E6
    tLoadOperMode = LoadON;
    tInitialVoltage = 120.0;
    tCardId = 0;
    tLoadSwitchId = 0;
    tTolerance = 1.0e-08;
    tMalfOverrideCurrentFlag = false;
    tMalfOverrideCurrentValue = 12.0;
    tActualPower = 40.0; // initialize actual power data
    tCurrent = 50.0;  // set it to 50 amps

    /// - Define nominal configuration data
    tConfigData = new ResistiveLoadConfigData(tloadName, tUserLoadType,
            tUnderVoltageLimit, tResistanceNormal, tResistanceStandby, tFuseCurrentLimit);
    tInputData = new ResistiveLoadInputData(tMalfOverrideCurrentFlag, tMalfOverrideCurrentValue,
            tLoadOperMode, tInitialVoltage);
    /// - Create new test Article
    tArticle = new FriendlyResistiveLoad();

    try {
        /// - Initialize the test article.
        tArticle->initialize(*tConfigData, *tInputData, tnetworkLoads, tCardId, tLoadSwitchId);

    } catch (TsInitializationException &e) {
        std::cout << "UTResistiveLoad Setup method initialization exception -- failed" << std::endl;
    } /* catch (TsOutOfBoundsException &e) {
        std::cout << "UTResistiveLoad Setup method out of bound exception -- failed" << std::endl;
    }*/ catch (...) {
        std::cout << "UTResistiveLoad Setup method catch all exception -- failed" << std::endl;
    }

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtResistiveLoad::testConfig()
{
    UT_RESULT_FIRST;

    /// @test   Nominal config construction
    CPPUNIT_ASSERT(tloadName == tConfigData->mName);
    CPPUNIT_ASSERT(RESISTIVE_LOAD == tConfigData->mUserLoadType);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tUnderVoltageLimit, tConfigData->mUnderVoltageLimit, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tResistanceNormal,  tConfigData->mResistanceNormal,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tResistanceStandby, tConfigData->mResistanceStandby, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFuseCurrentLimit,  tConfigData->mFuseCurrentLimit,  0.0);

    /// @test   Check default config construction
    ResistiveLoadConfigData defaultConfig;
    CPPUNIT_ASSERT("" != defaultConfig.mName);
    CPPUNIT_ASSERT(RESISTIVE_LOAD == defaultConfig.mUserLoadType);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(98.0,  defaultConfig.mUnderVoltageLimit, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0E6, defaultConfig.mResistanceNormal,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0E8, defaultConfig.mResistanceStandby, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,   defaultConfig.mFuseCurrentLimit,  0.0);

    /// @test   Check copy config construction
    ResistiveLoadConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tloadName == copyConfig.mName);
    CPPUNIT_ASSERT(RESISTIVE_LOAD == copyConfig.mUserLoadType);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tUnderVoltageLimit, copyConfig.mUnderVoltageLimit, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tResistanceNormal,  copyConfig.mResistanceNormal,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tResistanceStandby, copyConfig.mResistanceStandby, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFuseCurrentLimit,  copyConfig.mFuseCurrentLimit,  0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtResistiveLoad::testInput()
{
    UT_RESULT;

    /// - Check nominal input construction
    /// - Check nominal input construction
    // Work on this block when Input data class is available
    CPPUNIT_ASSERT(!tInputData->mMalfOverrideCurrentFlag);
    CPPUNIT_ASSERT(tMalfOverrideCurrentValue  == tInputData->mMalfOverrideCurrentValue);

    /// - Check copy input construction
    // not being tested until input data is available
    ResistiveLoadInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tInputData->mMalfOverrideCurrentFlag  == copyInput.mMalfOverrideCurrentFlag);
    CPPUNIT_ASSERT(tInputData->mMalfOverrideCurrentValue == copyInput.mMalfOverrideCurrentValue);
    CPPUNIT_ASSERT(tInputData->mLoadOperMode == copyInput.mLoadOperMode);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtResistiveLoad::testDefaultConstruction()
{
    UT_RESULT;

    /// - Construct an uninitialized test article
    FriendlyResistiveLoad article;

    // try to initialize the data
    article.initialize(*tConfigData, *tInputData, tnetworkLoads, tCardId, tLoadSwitchId);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtResistiveLoad::testInitialization()
{
    UT_RESULT;

    /// @test   config and input values
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tResistanceNormal,  tArticle->mResistanceNormal,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tUnderVoltageLimit, tArticle->mUnderVoltageLimit, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFuseCurrentLimit,  tArticle->mFuseCurrentLimit,  0.0);

    CPPUNIT_ASSERT(tResistanceStandby == tArticle->mResistanceStandby);
    CPPUNIT_ASSERT(tloadName == tArticle->getName());
    CPPUNIT_ASSERT(tLoadOperMode == tArticle->mLoadOperMode);
    CPPUNIT_ASSERT(tUserLoadType == tArticle->getLoadType());
    CPPUNIT_ASSERT(tCardId == tArticle->getCardId());
    CPPUNIT_ASSERT(tLoadSwitchId == tArticle->getLoadSwitchId());
    CPPUNIT_ASSERT(false == tArticle->mFuseIsBlown);
    CPPUNIT_ASSERT(false == tArticle->isFuseBlown());

    /// @test   input load type
    CPPUNIT_ASSERT_MESSAGE("Load Type is equal.",
            tUserLoadType == tArticle->mUserLoadType);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test that bad data produces the appropriate exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtResistiveLoad::testValidation()
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
    // test invalid normal power value
    tConfigData->mResistanceNormal = -1.0;
    /// - Initialize the test article.
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData,
            tnetworkLoads, tCardId, tLoadSwitchId), TsInitializationException);

    // test invalid standby power value
    tConfigData->mResistanceStandby = -1.0;
    /// - Initialize the test article.
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData,
            tnetworkLoads, tCardId, tLoadSwitchId), TsInitializationException);

    // test invalid standby power value
    std::string lName = "\0";
    tConfigData->mName = lName;
    /// - Initialize the test article.
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData,
            tnetworkLoads, tCardId, tLoadSwitchId), TsInitializationException);

    UT_PASS;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test that bad data produces the appropriate exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtResistiveLoad::testStanbyResistanceValidation()
{
    UT_RESULT;

    // test invalid standby power value
    tConfigData->mResistanceStandby = -1.0;
    /// - Initialize the test article.
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData,
            tnetworkLoads, tCardId, tLoadSwitchId), TsInitializationException);

    UT_PASS;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests updateState() method, called within BasicLink::step().
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtResistiveLoad::testUpdateState()
{
    UT_RESULT;

    /// - Placeholder for last-step potential
    double tPreviousPotential = tInitialPotential;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// - call step()
    tArticle->step(tPreviousPotential);
    // manually reset the resistance value and calculate the load again
    tArticle->mResistanceNormal = 300.0;
    /// - call updateState(), which calculates a new Potential
    tArticle->step(tPreviousPotential);
    /// @test  With equal heat fluxes the temperature should not change.
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
    CPPUNIT_ASSERT_MESSAGE("Actual power is very high!", tArticle->getPower() > tActualPower);
    // current calculated here is very high
    CPPUNIT_ASSERT_MESSAGE("Actual current here is very low!", tArticle->getCurrent() < tCurrent);

    /// @test fuse blow malfunction.
    tArticle->setMalfBlowFuse(true);
    CPPUNIT_ASSERT(true == tArticle->mMalfBlowFuse);
    tArticle->step(tInitialPotential);
    CPPUNIT_ASSERT(true == tArticle->mFuseIsBlown);
    CPPUNIT_ASSERT(true == tArticle->isFuseBlown());
    CPPUNIT_ASSERT(0.0  == tArticle->mVoltage);
    tArticle->setMalfBlowFuse(false);
    tArticle->resetFuse();
    CPPUNIT_ASSERT(false == tArticle->mMalfBlowFuse);
    CPPUNIT_ASSERT(false == tArticle->mFuseIsBlown);
    CPPUNIT_ASSERT(false == tArticle->isFuseBlown());

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
    // current calculated here is less than (50.0)
    CPPUNIT_ASSERT_MESSAGE("Test Actual power is low!", tArticle->getPower() > tActualPower);
    // power calculated here is less than (40.0)
    CPPUNIT_ASSERT_MESSAGE("Actual current here is very low!", tArticle->getCurrent() < tCurrent);

    tArticle->mPowerValid = false;

    tArticle->calculateResistiveLoad();
    tArticle->mPowerValid = true;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// - Create a new article, and initialize with current input and config values.
    FriendlyResistiveLoad article;
    article.initialize(*tConfigData, *tInputData, tnetworkLoads, tCardId, tLoadSwitchId);
    /// - call step()
    article.step(tPreviousPotential);

    /// @test   With zero capacitance, the Potential should not update, regardless of the
    CPPUNIT_ASSERT_MESSAGE("Voltage not equal.", tInitialPotential <= article.mVoltage);

    UT_PASS;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests ComputeActualPower() method to test actual power equal to zero
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtResistiveLoad::testCalculateResistiveLoadMalfOverrideCurrentValueEqualToZero() {

    UT_RESULT;

    // test invalid standby power value
    tArticle->mMalfOverrideCurrentFlag = true;
    tArticle->mMalfOverrideCurrentValue = 0.0;

    tArticle->calculateResistiveLoad();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(UserLoadBase::MAXIMUM_RESISTANCE, tArticle->mEquivalentResistance, FLT_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests ComputeActualPower() method to test actual power equal to zero
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtResistiveLoad::testCalculateResistiveLoadMalfOverrideCurrentValueLessThanZero() {

    UT_RESULT;

    // test invalid standby power value, and malf parameters set by the setter method
    tArticle->setMalfOverrideCurrent(true, -10.0);
    CPPUNIT_ASSERT(true  == tArticle->mMalfOverrideCurrentFlag);
    CPPUNIT_ASSERT(-10.0 == tArticle->mMalfOverrideCurrentValue);

    CPPUNIT_ASSERT_THROW(tArticle->calculateResistiveLoad(), TsNumericalException);

    // test malf parameters reset by the setter method
    tArticle->setMalfOverrideCurrent();
    CPPUNIT_ASSERT(false == tArticle->mMalfOverrideCurrentFlag);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfOverrideCurrentValue);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests ComputeActualPower() method to test actual power equal to zero
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtResistiveLoad::testComputeActualPowerNormalResistanceEqualToZero() {

    UT_RESULT;

    // test invalid standby power value
    tArticle->mLoadOperMode = LoadON;
    tArticle->mPowerValid = true;
    tArticle->mMalfOverrideCurrentFlag = false;
    tArticle->mResistanceNormal = 0.0;

    tArticle->calculateResistiveLoad();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(UserLoadBase::MAXIMUM_RESISTANCE, tArticle->mEquivalentResistance, FLT_EPSILON);
    tArticle->computeActualPower();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(UserLoadBase::MAXIMUM_RESISTANCE, tArticle->mEquivalentResistance, FLT_EPSILON);

    // check for less than zero value.
    tArticle->mEquivalentResistance = -20.0;
    CPPUNIT_ASSERT_THROW(tArticle->computeActualPower(), TsNumericalException);

    UT_PASS;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests ComputeActualPower() method to test actual power equal to zero
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtResistiveLoad::testComputeActualPowerStandbyResistanceEqualToZero() {

    UT_RESULT;

    // test invalid standby power value
    tArticle->mLoadOperMode = LoadSTANDBY;
    tArticle->mPowerValid = true;
    tArticle->mMalfOverrideCurrentFlag = false;
    tArticle->mResistanceStandby = 0.0;

    tArticle->calculateResistiveLoad();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(UserLoadBase::MAXIMUM_RESISTANCE, tArticle->mEquivalentResistance, FLT_EPSILON);
    tArticle->computeActualPower();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(UserLoadBase::MAXIMUM_RESISTANCE, tArticle->mEquivalentResistance, FLT_EPSILON);

    /// @test   zero equivalent resistance in computeActualPower
    tArticle->mEquivalentResistance = 0.0;
    tArticle->mPrintMessageOnce     = false; // for code coverage
    tArticle->computeActualPower();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(UserLoadBase::DEFAULT_RESISTANCE, tArticle->mEquivalentResistance, 0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests ComputeActualPower() method to test actual power equal to zero
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtResistiveLoad::testCalculateResistiveLoadMalfOverridePowerValueEqualToZero() {

    UT_RESULT;

    // test invalid standby power value
    tArticle->mMalfOverridePowerFlag = true;
    tArticle->mMalfOverridePower = 0.0;

    tArticle->calculateResistiveLoad();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(UserLoadBase::MAXIMUM_RESISTANCE, tArticle->mEquivalentResistance, FLT_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests ComputeActualPower() method to test actual power equal to zero
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtResistiveLoad::testCalculateResistiveLoadMalfOverridePowerValueLessThanZero() {

    UT_RESULT;

    // test invalid standby power value
    tArticle->mMalfOverridePowerFlag = true;
    tArticle->mMalfOverridePower = -10.0;

    CPPUNIT_ASSERT_THROW(tArticle->calculateResistiveLoad(), TsNumericalException);

    CPPUNIT_ASSERT_THROW(tArticle->step(tInitialPotential), TsNumericalException);

    UT_PASS;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests ComputeActualPower() method to test actual power equal to zero
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtResistiveLoad::testCalculateResistiveLoadMalfOverridePowerValueGreaterThanZero() {

    UT_RESULT;

    // test invalid standby power value, and malf parameters are set by the setter method
    tArticle->setMalfOverridePower(true, 30.0);
    CPPUNIT_ASSERT(true  == tArticle->mMalfOverridePowerFlag);
    CPPUNIT_ASSERT(30.0  == tArticle->mMalfOverridePower);

    tArticle->calculateResistiveLoad();

    // current calculated here is less than (50.0)
    CPPUNIT_ASSERT_MESSAGE("Test Actual power is low!", tArticle->getPower() < tActualPower);

    // test malf parameters are reset by the setter method
    tArticle->setMalfOverridePower();
    CPPUNIT_ASSERT(false == tArticle->mMalfOverridePowerFlag);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfOverridePower);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests updateFuse method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtResistiveLoad::testUpdateFuse() {

    UT_RESULT;

    /// @test fuse current limit = 0.
    tArticle->mFuseIsBlown = true;
    tArticle->mFuseCurrentLimit = 0.0;
    tArticle->mEquivalentResistance = 1.0;

    /// @test fuse already blown.
    tArticle->mFuseCurrentLimit = 5.0;
    CPPUNIT_ASSERT(false == tArticle->updateFuse(100.0));
    CPPUNIT_ASSERT(true  == tArticle->isFuseBlown());

    /// @test fuse not blown, current < limit.
    tArticle->mFuseIsBlown = false;
    tArticle->mEquivalentResistance = 100.0;
    CPPUNIT_ASSERT(false == tArticle->updateFuse(100.0));
    CPPUNIT_ASSERT(false == tArticle->isFuseBlown());

    /// @test fuse not blown, current > limit.
    tArticle->mEquivalentResistance = 1.0;
    CPPUNIT_ASSERT(true == tArticle->updateFuse(100.0));
    CPPUNIT_ASSERT(true == tArticle->isFuseBlown());

    UT_PASS_LAST;
}
