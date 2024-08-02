/*
@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
 */
#include "UtConstantPowerLoad.hh"
#include <iostream>

#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsNumericalException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "strings/UtResult.hh"

/// @details  Test identification number.
int UtConstantPowerLoad::TEST_ID = 0;

/* CPPUNIT_ASSERT choices:
 http://cppunit.sourceforge.net/doc/lastest/group___assertions.html#ga0

 #define         CPPUNIT_ASSERT(condition)
 Assertions that a condition is true.

 #define         CPPUNIT_ASSERT_MESSAGE(message, condition)
 Assertion with a user specified message.

 #define         CPPUNIT_FAIL(message)
 Fails with the specified message.

 #define         CPPUNIT_ASSERT_EQUAL(expected, actual)
 Asserts that two values are equals.

 #define         CPPUNIT_ASSERT_EQUAL_MESSAGE(message, expected, actual)
 Asserts that two values are equals, provides additional messafe on failure.

 #define         CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, actual, delta)
 Macro for primitive value comparisons.

 #define         CPPUNIT_ASSERT_THROW(expression, ExceptionType)
 Asserts that the given expression throws an exception of the specified type.

 #define         CPPUNIT_ASSERT_NO_THROW(expression)
 Asserts that the given expression does not throw any exceptions.

 #define         CPPUNIT_ASSERT_ASSERTION_FAIL(assertion) \
                   CPPUNIT_ASSERT_THROW( assertion, CPPUNIT_NS::Exception )
 Asserts that an assertion fail.

 #define         CPPUNIT_ASSERT_ASSERTION_PASS(assertion)   CPPUNIT_ASSERT_NO_THROW( assertion )
 Asserts that an assertion pass.
 */

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtCostantPowerUserLoad class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtConstantPowerLoad::UtConstantPowerLoad() :
tConfigData(),
tInputData(),
tArticle(),
tloadName(""),
tCardId(0),
tLoadSwitchId(0),
tActualPower(0.0),
tCurrent(0.0),
tEquivalentResistance(0.0),
tUserLoadType(CONSTANT_POWER_LOAD),
tPowerNormal(25.0),
tPowerStandby(10.0),
tInitialPotential(),
tnetworkLoads(),
tUnderVoltageLimit(),
tFuseCurrentLimit(0.0),
tPowerValid(true),
tLoadOperMode(LoadON),
tInitialVoltage(0.0),
tInitFlag(false),
tTolerance(0),
tMalfOverrideCurrentFlag(false),
tMalfOverrideCurrentValue(0.0) {
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtCostantPowerUserLoad class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtConstantPowerLoad::~UtConstantPowerLoad() {
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConstantPowerLoad::tearDown() {
    /// - Deletes for news in setUp
    delete tArticle;
    delete tInputData;
    delete tConfigData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConstantPowerLoad::setUp() {
    /// - Declare the standard constant power user load data
    tloadName = "TestConstantPower UserLoad";
    tInitialPotential = 322;
    tUnderVoltageLimit = 90.0;
    tFuseCurrentLimit = 5.0;
    tUserLoadType = CONSTANT_POWER_LOAD;
    tPowerNormal = 30.0;
    tPowerStandby = 5.0;
    tLoadOperMode = LoadON;
    tInitialVoltage = 120.0;
    tCardId = 0;
    tLoadSwitchId = 0;
    tTolerance = 1.0e-08;
    tMalfOverrideCurrentFlag = false;
    tMalfOverrideCurrentValue = 200.0;
    tActualPower = 50.0;
    tCurrent = 40.0;
    tEquivalentResistance = 1.0E8; // constant value for maximum resistance


    /// @brief Define nominal configuration data
    tConfigData = new ConstantPowerLoadConfigData(tloadName, tUserLoadType,
            tUnderVoltageLimit, tPowerNormal, tPowerStandby, tFuseCurrentLimit);
    /// @brief create new input data
    tInputData = new ConstantPowerLoadInputData(tMalfOverrideCurrentFlag,
            tMalfOverrideCurrentValue, tLoadOperMode, tInitialVoltage);

    /// @brief Create new test Article
    tArticle = new FriendlyConstantPowerLoad();

    try {
        /// - Initialize the test article.
        tArticle->initialize(*tConfigData, *tInputData, tnetworkLoads, tCardId,
                tLoadSwitchId);

    } catch (TsInitializationException &e) {
        std::cout
        << "UTConstantPowerLoad Setup method initialization exception -- failed"
        << std::endl;
     } catch (...) {
         std::cout
         << "UTConstantPowerLoad Setup method catch all exception -- failed"
         << std::endl;
     }

     /// - Increment the test identification number.
     ++TEST_ID;

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConstantPowerLoad::testConfig() {

    UT_RESULT_FIRST;

    /// @test   Nominal config construction
    CPPUNIT_ASSERT(tloadName == tConfigData->mName);
    CPPUNIT_ASSERT(CONSTANT_POWER_LOAD == tConfigData->mUserLoadType);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tUnderVoltageLimit, tConfigData->mUnderVoltageLimit, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tPowerNormal,       tConfigData->mPowerNormal,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tPowerStandby,      tConfigData->mPowerStandby,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFuseCurrentLimit,  tConfigData->mFuseCurrentLimit,  0.0);

    /// @test   Default config construction
    ConstantPowerLoadConfigData defaultConfig;
    CPPUNIT_ASSERT("Unnamed Load" == defaultConfig.mName);
    CPPUNIT_ASSERT(CONSTANT_POWER_LOAD == defaultConfig.mUserLoadType);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(98.0, defaultConfig.mUnderVoltageLimit, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  defaultConfig.mPowerNormal,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  defaultConfig.mPowerStandby,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  defaultConfig.mFuseCurrentLimit,  0.0);

    /// @test   Check copy config construction
    ConstantPowerLoadConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tloadName == copyConfig.mName);
    CPPUNIT_ASSERT(CONSTANT_POWER_LOAD == copyConfig.mUserLoadType);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tUnderVoltageLimit, copyConfig.mUnderVoltageLimit, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tPowerNormal,       copyConfig.mPowerNormal,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tPowerStandby,      copyConfig.mPowerStandby,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFuseCurrentLimit,  copyConfig.mFuseCurrentLimit,  0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConstantPowerLoad::testInput() {

    UT_RESULT;

    /// - Check nominal input construction
    // Work on this block when Input data class is available
    CPPUNIT_ASSERT(!tInputData->mMalfOverrideCurrentFlag);
    CPPUNIT_ASSERT(tMalfOverrideCurrentValue == tInputData->mMalfOverrideCurrentValue);

    /// - Check copy input construction
    ConstantPowerLoadInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tInputData->mMalfOverrideCurrentFlag
            == copyInput.mMalfOverrideCurrentFlag);
    CPPUNIT_ASSERT(tInputData->mMalfOverrideCurrentValue == copyInput.mMalfOverrideCurrentValue);
    CPPUNIT_ASSERT(tInputData->mLoadOperMode == copyInput.mLoadOperMode);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConstantPowerLoad::testDefaultConstruction() {

    UT_RESULT;

    /// - Construct an uninitialized test article
    FriendlyConstantPowerLoad article;

    // try to initialize the data
    article.initialize(*tConfigData, *tInputData, tnetworkLoads, tCardId,
            tLoadSwitchId);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConstantPowerLoad::testInitialization() {

    UT_RESULT;

    /// @test   config and input values
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tPowerNormal,       tArticle->mPowerNormal,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tUnderVoltageLimit, tArticle->mUnderVoltageLimit, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFuseCurrentLimit,  tArticle->mFuseCurrentLimit,  0.0);

    CPPUNIT_ASSERT(tPowerStandby == tArticle->mPowerStandby);
    CPPUNIT_ASSERT(tloadName == tArticle->getName());
    CPPUNIT_ASSERT(tLoadOperMode == tArticle->mLoadOperMode);
    CPPUNIT_ASSERT(tUserLoadType == tArticle->getLoadType());
    CPPUNIT_ASSERT(tCardId == tArticle->getCardId());
    CPPUNIT_ASSERT(tLoadSwitchId == tArticle->getLoadSwitchId());
    CPPUNIT_ASSERT(false == tArticle->mFuseIsBlown);
    CPPUNIT_ASSERT(false == tArticle->isFuseBlown());

    /// @test   input load type
    CPPUNIT_ASSERT_MESSAGE("Load Type is equal.", tUserLoadType
            == tArticle->mUserLoadType);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test that bad data produces the appropriate exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConstantPowerLoad::testValidation() {

    UT_RESULT;

    /// @test empty object name
    tConfigData->mName = "";
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData,
            tnetworkLoads, tCardId, tLoadSwitchId), TsInitializationException);
    tConfigData->mName = tloadName;

    /// @test voltage validation
    tConfigData->mUserLoadType = 3; // something other than constant power or constant resistance

    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData,
            tnetworkLoads, tCardId, tLoadSwitchId), TsInitializationException);
    tConfigData->mUnderVoltageLimit = tUnderVoltageLimit;

    // reset load type to constant power
    tConfigData->mUserLoadType = CONSTANT_POWER_LOAD;

    /// @test validation of operation mode
    tInputData->mLoadOperMode = 4; // value other than the assigned modes

    /// - Initialize the test article.
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData,
            tnetworkLoads, tCardId, tLoadSwitchId), TsInitializationException);

    CPPUNIT_ASSERT(tLoadOperMode == tArticle->getLoadOperMode());
    CPPUNIT_ASSERT(tMalfOverrideCurrentFlag == tArticle->getOverrideCurrentFlag());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMalfOverrideCurrentValue,
              tArticle->getOverrideCurrentValue(), tTolerance);

    /// reset operation mode to ON
    tInputData->mLoadOperMode = LoadON;

    // test invalid normal power value
    tConfigData->mPowerNormal = -1.0;
    /// - Initialize the test article.
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData,
            tnetworkLoads, tCardId, tLoadSwitchId), TsInitializationException);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test that bad data produces the appropriate exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConstantPowerLoad::testStandbyPowerValidation() {

    UT_RESULT;

    // test invalid standby power value
    tConfigData->mPowerStandby = -1.0;
    /// - Initialize the test article.
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData,
            tnetworkLoads, tCardId, tLoadSwitchId), TsInitializationException);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests updateState() method, called within BasicLink::step().
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConstantPowerLoad::testUpdateState() {

    UT_RESULT;

    /// - Placeholder for last-step potential
    double tPreviousPotential = tInitialPotential;

    /// - Default net heat flux
    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// - call step()
    tArticle->step(tPreviousPotential);

    // manually reset the power value and calculate the load again
    tArticle->mPowerNormal = 45.0;

    /// - call updateState(), which calculates a new Potential[0]
    tArticle->step(tPreviousPotential);

    /// @test    With equal heat fluxes the temperature should not change.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tPreviousPotential, tArticle->mVoltage,
            tTolerance);

    /// - save previous Potential
    tPreviousPotential = tArticle->mVoltage;

    /// - call updateState(), which calculates a new Potential[0]
    tArticle->step(tPreviousPotential);

    CPPUNIT_ASSERT_MESSAGE("Net current calculated.", tArticle->getPower()
            >= tArticle->mPowerNormal);

    /// - save previous Potential
    tPreviousPotential = tArticle->mVoltage;

    /// - call which calculates a new Potential
    // reset the initial potential tInitialPotential to 135.0
    tInitialPotential = 135.0;

    tArticle->step(tInitialPotential);

    CPPUNIT_ASSERT_MESSAGE("Net current changed?.", tArticle->getPower()
            <= tArticle->mPowerNormal);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->getPower(), tArticle->mPowerNormal,
            tTolerance);

    // power calculated here is very high
    CPPUNIT_ASSERT_MESSAGE("Equivalent resistance is very low!",
            tArticle->getResistance() < tEquivalentResistance);

    // current calculated here is very high
    CPPUNIT_ASSERT_MESSAGE("Actual current here is very low!",
            tArticle->getCurrent() < tCurrent);

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
    tArticle->mPowerStandby = 30000.0;

    // step through to perform the calculations
    tArticle->step(tInitialPotential);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->getPower(), tArticle->mPowerStandby,
            tTolerance);

    // current calculated here is less than (50.0)
    CPPUNIT_ASSERT_MESSAGE("Actual current here is very high!",
            tArticle->getCurrent() > tCurrent);

    // power calculated here is less than (40.0)
    CPPUNIT_ASSERT_MESSAGE("Equivalent resistance is very low!",
            tArticle->getResistance() < tEquivalentResistance);

    tArticle->mLoadOperMode = LoadOFF;
    // step through to perform the calculations
    tArticle->step(tInitialPotential);

    //when load is off then power is 0.0 and resistance is max resistance
    // current calculated here is less than (50.0)
    CPPUNIT_ASSERT_MESSAGE("Actual power is zero!",
            tArticle->getPower() == 0.0);

    // power calculated here is less than (40.0)
    CPPUNIT_ASSERT_MESSAGE("Equivalent resistance is high!",
            tArticle->getResistance() == tEquivalentResistance);

    //reset load to ON and set the overwrite flag to true
    tArticle->mLoadOperMode = LoadON;
    tArticle->mMalfOverrideCurrentFlag = true;

    // step through to perform the calculations
    tArticle->step(tInitialPotential);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->getCurrent(),
            tArticle->mMalfOverrideCurrentValue, tTolerance);

    // current calculated here is less than (50.0)
    CPPUNIT_ASSERT_MESSAGE("Actual current here is very high!",
            tArticle->getCurrent() > tCurrent);

    // power calculated here is less than (40.0)
    CPPUNIT_ASSERT_MESSAGE("Equivalent resistance is very low!",
            tArticle->getResistance() < tEquivalentResistance);


    tArticle->step(20.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->getCurrent(),
               0.0, tTolerance);

    /// @test   Magic power voltage
    tArticle->mMagicPowerFlag = true;
    tArticle->mMagicPowerValue = 24.0;
    tArticle->step(tInitialPotential);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(24.0, tArticle->mVoltage, tTolerance);
    tArticle->mMagicPowerFlag = false;

    tArticle->mPowerValid = true;
    CPPUNIT_ASSERT(tArticle->getPowerValid());

    tArticle->mMalfOverrideCurrentFlag = false;
    tArticle->mPowerNormal = -1.0;

    CPPUNIT_ASSERT_THROW(tArticle->calculateConstantPowerLoad(), TsNumericalException);

    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// - Create a new article, and initialize with zero capacitance.
    FriendlyConstantPowerLoad article;
    article.initialize(*tConfigData, *tInputData, tnetworkLoads, tCardId,
            tLoadSwitchId);

    /// - call updateState()
    article.step(tPreviousPotential);

    /// @test   With zero capacitance, the Potential should not update, regardless of the
    ///         net heat flux.
    CPPUNIT_ASSERT_MESSAGE("Voltage not equal.", tInitialPotential
            <= article.mVoltage);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests ComputeActualPower() method to test actual power equal to zero
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConstantPowerLoad::testComputeActualPowerMalfOverrideCurrentValueEqualToZero() {

    UT_RESULT;

    // test invalid standby power value
    tArticle->mMalfOverrideCurrentFlag = true;
    tArticle->mMalfOverrideCurrentValue = 0.0;

    tArticle->calculateConstantPowerLoad();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->getPower(), 0.0, tTolerance);

    tArticle->computeActualPower();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->getResistance(), UserLoadBase::MAXIMUM_RESISTANCE, tTolerance);

    UT_PASS;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests ComputeActualPower() method to test overwrite power less than zero
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConstantPowerLoad::testCalculateConstantPowerLoadMalfOverrideCurrentValueLessThanZero() {

    UT_RESULT;

    // test invalid standby power value
    tArticle->mMalfOverrideCurrentFlag = true;
    tArticle->mMalfOverrideCurrentValue = -1.0;

    CPPUNIT_ASSERT_THROW(tArticle->calculateConstantPowerLoad(), TsNumericalException);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests ComputeActualPower() method to test normal power equal to zero
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConstantPowerLoad::testCalculateConstantPowerLoadNormalPowerEqualToZero() {

    UT_RESULT;

    // test invalid standby power value
    tArticle->mLoadOperMode = LoadON;
    tArticle->mPowerValid = true;
    tArticle->mMalfOverrideCurrentFlag = false;
    tArticle->mPowerNormal = 0.0;

    tArticle->calculateConstantPowerLoad();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->getPower(), 0.0, tTolerance);

    UT_PASS;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests ComputeActualPower() method to test normal power equal to zero
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConstantPowerLoad::testCalculateConstantPowerLoadStandByPowerEqualToZero() {

    UT_RESULT;

    // test invalid standby power value
    tArticle->mLoadOperMode = LoadSTANDBY;
    tArticle->mPowerValid = true;
    tArticle->mMalfOverrideCurrentFlag = false;
    tArticle->mPowerStandby = 0.0;

    tArticle->calculateConstantPowerLoad();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->getPower(), 0.0, tTolerance);

    UT_PASS;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests ComputeActualPower() method to test actual power equal to zero
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConstantPowerLoad::testComputeActualPowerMalfOverridePowerValueEqualToZero() {

    UT_RESULT;

    // test invalid standby power value
    tArticle->mMalfOverridePowerFlag = true;
    tArticle->mMalfOverridePower = 0.0;

    tArticle->calculateConstantPowerLoad();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->getPower(), 0.0, tTolerance);

    tArticle->computeActualPower();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->getResistance(), UserLoadBase::MAXIMUM_RESISTANCE, tTolerance);

    UT_PASS;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests ComputeActualPower() method to test overwrite power less than zero
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConstantPowerLoad::testCalculateConstantPowerLoadMalfOverridePowerValueLessThanZero() {

    UT_RESULT;

    // test invalid standby power value
    tArticle->mMalfOverridePowerFlag = true;
    tArticle->mMalfOverridePower = -10.0;

    CPPUNIT_ASSERT_THROW(tArticle->calculateConstantPowerLoad(), TsNumericalException);

    CPPUNIT_ASSERT_THROW(tArticle->step(tInitialPotential), TsNumericalException);

    UT_PASS;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests ComputeActualPower() method to test overwrite power less than zero
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConstantPowerLoad::testCalculateConstantPowerLoadMalfOverridePowerValueGreaterThanZero() {

    UT_RESULT;

    // test invalid standby power value
    tArticle->mMalfOverridePowerFlag = true;
    tArticle->mMalfOverridePower = 30.0;

    tArticle->calculateConstantPowerLoad();

    // current calculated here is less than (50.0)
    CPPUNIT_ASSERT_MESSAGE("Test Actual power is low!", tArticle->getPower() < tActualPower);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests ComputeActualPower() method to test setters
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConstantPowerLoad::testSetters() {

    UT_RESULT;

    const double result = 1234.5;
    tArticle->setPowerNormal(result);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mPowerNormal, result, tTolerance);
    tArticle->setLoadOperMode(LoadSTANDBY);
    CPPUNIT_ASSERT(LoadSTANDBY == tArticle->mLoadOperMode);

    UT_PASS_LAST;
}
