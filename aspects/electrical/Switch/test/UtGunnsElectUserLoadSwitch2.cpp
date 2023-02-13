/*
@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/

#include "UtGunnsElectUserLoadSwitch2.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "common/sensors/TsNoise.hh"
#include "strings/UtResult.hh"

/// @details  Test identification number.
int UtGunnsElectUserLoadSwitch2::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Electrical User Load Switch Variant 2 link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectUserLoadSwitch2::UtGunnsElectUserLoadSwitch2()
    :
    CppUnit::TestFixture(),
    tLinks(),
    tName(""),
    tNodes(),
    tNodeList(),
    tPorts(),
    tSwitchResistance(0.0),
    tSwitchTripPriority(0),
    tCurrentSensorMinRange(0.0),
    tCurrentSensorMaxRange(0.0),
    tInputVoltageSensorMinRange(0.0),
    tInputVoltageSensorMaxRange(0.0),
    tOutputVoltageSensorMinRange(0.0),
    tOutputVoltageSensorMaxRange(0.0),
    tConfigData(0),
    tMalfBlockageFlag(false),
    tMalfBlockageValue(0.0),
    tSwitchIsClosed(false),
    tInputUnderVoltageTripLimit(0.0),
    tInputUnderVoltageTripReset(0.0),
    tInputOverVoltageTripLimit(0.0),
    tInputOverVoltageTripReset(0.0),
    tSwitchPosTripLimit(0.0),
    tSwitchNegTripLimit(0.0),
    tLoadsOverrideActive(false),
    tLoadsOverrideVoltage(0.0),
    tInputData(0),
    tArticle(0),
    tLoadRConfigData("", 0.0, 0.0, 0.0, 0.0),
    tLoadRInputData(0, 0.0),
    tLoadR(&tLoadRConfigData, &tLoadRInputData),
    tLoadR2ConfigData("", 0.0, 0.0, 0.0, 0.0),
    tLoadR2InputData(0, 0.0),
    tLoadR2(&tLoadR2ConfigData, &tLoadR2InputData),
    tLoadCpConfigData("", 0.0, 0.0, 0.0, 0.0),
    tLoadCpInputData(0, 0.0),
    tLoadCp(&tLoadCpConfigData, &tLoadCpInputData)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Electrical User Load Switch Variant 2 link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectUserLoadSwitch2::~UtGunnsElectUserLoadSwitch2()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////

void UtGunnsElectUserLoadSwitch2::setUp()
{
    /// - Initialize the nodes.
    tNodes[0].initialize("UtNode0", 120.0);
    tNodes[1].initialize("UtNode0", 121.9);
    tNodes[2].initialize("UtNode1",   0.0);

    /// - Initialize the nodes list.
    tNodeList.mNodes    = tNodes;
    tNodeList.mNumNodes = N_NODES;

    /// - Define the nominal configuration data.
    tName                        = "nominal";
    tSwitchResistance            =   0.1;
    tSwitchTripPriority          =   2;
    tCurrentSensorMinRange       = -10.0;
    tCurrentSensorMaxRange       =  10.0;
    tInputVoltageSensorMinRange  =   0.0;
    tInputVoltageSensorMaxRange  = 200.0;
    tOutputVoltageSensorMinRange =  -1.0;
    tOutputVoltageSensorMaxRange = 180.0;
    tConfigData                  = new GunnsElectUserLoadSwitch2ConfigData(tName,
                                                                          &tNodeList,
                                                                           tSwitchResistance,
                                                                           tSwitchTripPriority,
                                                                           tCurrentSensorMinRange,
                                                                           tCurrentSensorMaxRange,
                                                                           tInputVoltageSensorMinRange,
                                                                           tInputVoltageSensorMaxRange,
                                                                           tOutputVoltageSensorMinRange,
                                                                           tOutputVoltageSensorMaxRange);

    /// - Define the nominal input data.
    tMalfBlockageFlag           = true;
    tMalfBlockageValue          = 0.3;
    tSwitchIsClosed             = false;
    tInputUnderVoltageTripLimit = 100.0;
    tInputUnderVoltageTripReset = 105.0;
    tInputOverVoltageTripLimit  = 130.0;
    tInputOverVoltageTripReset  = 130.0;
    tSwitchPosTripLimit         =   5.0;
    tSwitchNegTripLimit         =  -1.0;
    tLoadsOverrideActive        = false;
    tLoadsOverrideVoltage       = 122.0;
    tInputData                  = new GunnsElectUserLoadSwitch2InputData(tMalfBlockageFlag,
                                                                         tMalfBlockageValue,
                                                                         tSwitchIsClosed,
                                                                         tInputUnderVoltageTripLimit,
                                                                         tInputUnderVoltageTripReset,
                                                                         tInputOverVoltageTripLimit,
                                                                         tInputOverVoltageTripReset,
                                                                         tSwitchPosTripLimit,
                                                                         tSwitchNegTripLimit,
                                                                         tLoadsOverrideActive,
                                                                         tLoadsOverrideVoltage);

    /// - Define the nominal port mapping.
    tPorts[0] = 0;
    tPorts[1] = 2;

    /// - Default construct the nominal test article.
    tArticle            = new FriendlyGunnsElectUserLoadSwitch2;

    /// - Define the nominal user loads config & input data.
    tLoadRConfigData.mName               = "tLoadR";
    tLoadRConfigData.mUnderVoltageLimit  = 80.0;
    tLoadRConfigData.mResistanceNormal   = 10.0;
    tLoadRConfigData.mResistanceStandby  = 100.0;
    tLoadRInputData.mInitialMode         = LoadON;
    tLoadRInputData.mInitialVoltage      = 120.0;

    tLoadR2ConfigData.mName               = "tLoadR2";
    tLoadR2ConfigData.mUnderVoltageLimit  = 80.0;
    tLoadR2ConfigData.mResistanceNormal   = 10.0;
    tLoadR2ConfigData.mResistanceStandby  = 100.0;
    tLoadR2ConfigData.mFuseCurrentLimit   = 10.0;
    tLoadR2InputData.mInitialMode         = LoadON;
    tLoadR2InputData.mInitialVoltage      = 120.0;

    tLoadCpConfigData.mName              = "tLoadCp";
    tLoadCpConfigData.mUnderVoltageLimit = 15.0;
    tLoadCpConfigData.mPowerNormal       = 100.0;
    tLoadCpConfigData.mPowerStandby      = 10.0;
    tLoadCpInputData.mInitialMode        = LoadON;
    tLoadCpInputData.mInitialVoltage     = 120.0;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectUserLoadSwitch2::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete tArticle;
    delete tInputData;
    delete tConfigData;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical User Load Switch Variant 2 link construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectUserLoadSwitch2::testConfig()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(tName                        == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                       == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(0.0                          == tConfigData->mDefaultConductivity);
    CPPUNIT_ASSERT(tSwitchResistance            == tConfigData->mSwitch.mResistance);
    CPPUNIT_ASSERT(tSwitchTripPriority          == tConfigData->mSwitch.mTripPriority);
    CPPUNIT_ASSERT(tCurrentSensorMinRange       == tConfigData->mCurrentSensor.mMinRange);
    CPPUNIT_ASSERT(tCurrentSensorMaxRange       == tConfigData->mCurrentSensor.mMaxRange);
    CPPUNIT_ASSERT(TsNoise::getNoiseFunction()  == tConfigData->mCurrentSensor.mNoiseFunction);
    CPPUNIT_ASSERT(tInputVoltageSensorMinRange  == tConfigData->mInputVoltageSensor.mMinRange);
    CPPUNIT_ASSERT(tInputVoltageSensorMaxRange  == tConfigData->mInputVoltageSensor.mMaxRange);
    CPPUNIT_ASSERT(TsNoise::getNoiseFunction()  == tConfigData->mInputVoltageSensor.mNoiseFunction);
    CPPUNIT_ASSERT(tOutputVoltageSensorMinRange == tConfigData->mOutputVoltageSensor.mMinRange);
    CPPUNIT_ASSERT(tOutputVoltageSensorMaxRange == tConfigData->mOutputVoltageSensor.mMaxRange);
    CPPUNIT_ASSERT(TsNoise::getNoiseFunction()  == tConfigData->mOutputVoltageSensor.mNoiseFunction);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical User Load Switch Variant 2 link construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectUserLoadSwitch2::testInput()
{
    UT_RESULT;

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(tMalfBlockageFlag           == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue          == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tSwitchIsClosed             == tInputData->mSwitch.mPosition);
    CPPUNIT_ASSERT(tSwitchIsClosed             == tInputData->mSwitch.mPositionCommand);
    CPPUNIT_ASSERT(false                       == tInputData->mSwitch.mResetTripsCommand);
    CPPUNIT_ASSERT(tInputUnderVoltageTripLimit == tInputData->mSwitch.mInputUnderVoltageTripLimit);
    CPPUNIT_ASSERT(tInputUnderVoltageTripReset == tInputData->mSwitch.mInputUnderVoltageTripReset);
    CPPUNIT_ASSERT(tInputOverVoltageTripLimit  == tInputData->mSwitch.mInputOverVoltageTripLimit);
    CPPUNIT_ASSERT(tInputOverVoltageTripReset  == tInputData->mSwitch.mInputOverVoltageTripReset);
    CPPUNIT_ASSERT(tSwitchPosTripLimit         == tInputData->mSwitch.mPosOverCurrentTripLimit);
    CPPUNIT_ASSERT(tSwitchNegTripLimit         == tInputData->mSwitch.mNegOverCurrentTripLimit);
    CPPUNIT_ASSERT(tLoadsOverrideActive        == tInputData->mLoadsOverrideActive);
    CPPUNIT_ASSERT(tLoadsOverrideVoltage       == tInputData->mLoadsOverrideVoltage);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical User Load Switch Variant 2 link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectUserLoadSwitch2::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default of attributes.
    CPPUNIT_ASSERT(""    == tArticle->mName);
    CPPUNIT_ASSERT(0     == tArticle->mNodes);
    CPPUNIT_ASSERT(false == tArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0   == tArticle->mDefaultConductivity);
    CPPUNIT_ASSERT(false == tArticle->mSwitch.mMalfFailClosed);
    CPPUNIT_ASSERT(false == tArticle->mCurrentSensor.mMalfFailOffFlag);
    CPPUNIT_ASSERT(false == tArticle->mInputVoltageSensor.mMalfFailOffFlag);
    CPPUNIT_ASSERT(false == tArticle->mOutputVoltageSensor.mMalfFailOffFlag);
    CPPUNIT_ASSERT(false == tArticle->mShort.mMalfConstantPowerFlag);
    CPPUNIT_ASSERT(0.0   == tArticle->mLoadsVoltage);
    CPPUNIT_ASSERT(0.0   == tArticle->mLoadsPower);
    CPPUNIT_ASSERT(false == tArticle->mLoadsOverrideActive);
    CPPUNIT_ASSERT(0.0   == tArticle->mLoadsOverrideVoltage);
    CPPUNIT_ASSERT(0     == tArticle->mUserLoads.size());

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsElectUserLoadSwitch2* article = new GunnsElectUserLoadSwitch2();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical User Load Switch Variant 2 link model nominal initialization
///           with user loads.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectUserLoadSwitch2::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize the user loads and add them to the test article.
    tLoadR .initLoad();
    tLoadCp.initLoad();
    CPPUNIT_ASSERT_NO_THROW(tArticle->addUserLoad(&tLoadR));
    CPPUNIT_ASSERT_NO_THROW(tArticle->addUserLoad(&tLoadCp));

    /// - Initialize test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPorts[0], tPorts[1]));

    /// @test    Nominal configuration data.
    CPPUNIT_ASSERT(tName              == tArticle->mName);
    CPPUNIT_ASSERT(&tNodes[tPorts[0]] == tArticle->mNodes[0]);
    CPPUNIT_ASSERT(&tNodes[tPorts[1]] == tArticle->mNodes[1]);
    CPPUNIT_ASSERT(0.0                == tArticle->mDefaultConductivity);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT_EQUAL(tMalfBlockageFlag,   tArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfBlockageValue,  tArticle->mMalfBlockageValue);
    CPPUNIT_ASSERT(false                   == tArticle->mSwitch.getPositionCommand());
    CPPUNIT_ASSERT(false                   == tArticle->mSwitch.getPosition());
    CPPUNIT_ASSERT(tLoadsOverrideActive    == tArticle->mLoadsOverrideActive);
    CPPUNIT_ASSERT_EQUAL(tLoadsOverrideVoltage, tArticle->mLoadsOverrideVoltage);

    /// @test    Nominal state data.
    CPPUNIT_ASSERT_EQUAL(0.0,                 tArticle->mEffectiveConductivity);
    CPPUNIT_ASSERT_EQUAL(0.0,                 tArticle->mLoadsVoltage);
    CPPUNIT_ASSERT(2                       == tArticle->mUserLoads.size());
    CPPUNIT_ASSERT(&tLoadR                 == tArticle->mUserLoads[0]);
    CPPUNIT_ASSERT(&tLoadCp                == tArticle->mUserLoads[1]);

    /// @test    Nominal initialization flags.
    CPPUNIT_ASSERT(tArticle->mSwitch.isInitialized());
    CPPUNIT_ASSERT(tArticle->mCurrentSensor.isInitialized());
    CPPUNIT_ASSERT(tArticle->mInputVoltageSensor.isInitialized());
    CPPUNIT_ASSERT(tArticle->mOutputVoltageSensor.isInitialized());
    CPPUNIT_ASSERT(tArticle->mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical User Load Switch Variant 2 link model access methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectUserLoadSwitch2::testAccessors()
{
    UT_RESULT;

    /// @test  isNonLinear method.
    CPPUNIT_ASSERT(tArticle->isNonLinear());
    tArticle->mLoadsOverrideActive = true;
    CPPUNIT_ASSERT(not tArticle->isNonLinear());

    /// @test  getLoadsPower method.
    tArticle->mLoadsPower = 10.0;
    CPPUNIT_ASSERT_EQUAL(10.0, tArticle->getLoadsPower());

    /// @test  setLoadsOverride method.
    tArticle->setLoadsOverride(true, 12.0);
    CPPUNIT_ASSERT_EQUAL(true, tArticle->mLoadsOverrideActive);
    CPPUNIT_ASSERT_EQUAL(12.0, tArticle->mLoadsOverrideVoltage);

    tArticle->setLoadsOverride();
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mLoadsOverrideActive);
    CPPUNIT_ASSERT_EQUAL(0.0,   tArticle->mLoadsOverrideVoltage);

    /// @test  getUserLoad
    CPPUNIT_ASSERT(0 == tArticle->getUserLoad(0));
    tLoadR .initLoad();
    tLoadCp.initLoad();
    CPPUNIT_ASSERT_NO_THROW(tArticle->addUserLoad(&tLoadR));
    CPPUNIT_ASSERT_NO_THROW(tArticle->addUserLoad(&tLoadCp));
    CPPUNIT_ASSERT(tArticle->mUserLoads[0] == tArticle->getUserLoad(0));
    CPPUNIT_ASSERT(tArticle->mUserLoads[1] == tArticle->getUserLoad(1));

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical User Load Switch Variant 2 link model step method with user
///           loads.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectUserLoadSwitch2::testStep()
{
    UT_RESULT;

    /// - Initialize the user loads and add them to the test article.
    tLoadR .initLoad();
    tLoadCp.initLoad();
    CPPUNIT_ASSERT_NO_THROW(tArticle->addUserLoad(&tLoadR));
    CPPUNIT_ASSERT_NO_THROW(tArticle->addUserLoad(&tLoadCp));

    /// - Initialize test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPorts[0], tPorts[1]));

    tArticle->mPotentialVector[0] = tNodes[tPorts[0]].getPotential();
    tArticle->mPotentialVector[1] = tNodes[tPorts[1]].getPotential();

    {
        /// @test Switch closed.
        tArticle->mSwitch.setPositionCommand(true);
        tArticle->mShort.setMalfResistance(true, 100.0, 0.0);
        const double timestep  = 0.1;
        CPPUNIT_ASSERT_NO_THROW(tArticle->step(timestep));
        CPPUNIT_ASSERT(tArticle->mSwitch.getPosition());
        double expectedLoadsV  = tNodes[tPorts[0]].getPotential();
        double expectedSwitchR = tSwitchResistance;
        double expectedLoadRR  = tLoadRConfigData.mResistanceNormal;
        double expectedLoadCpR = expectedLoadsV * expectedLoadsV / tLoadCpConfigData.mPowerNormal;
        double expectedShortG  = 1.0 / 100.0;
        double expectedLoadsG  = 100.0 * DBL_EPSILON + 1.0 / expectedLoadRR + 1.0 / expectedLoadCpR + expectedShortG;
        double expectedG       = 1.0 / (expectedSwitchR + 1.0 / expectedLoadsG);
        double expectedA       = expectedG * (1.0 - tInputData->mMalfBlockageValue);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSwitchR, tArticle->mSwitch.getResistance(),  DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedLoadRR,  tLoadR.getLoad()->getResistance(),  DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedLoadCpR, tLoadCp.getLoad()->getResistance(), DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedLoadsV,  tArticle->mLoadsVoltage,            DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG,       tArticle->mEffectiveConductivity,   DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedA,       tArticle->mAdmittanceMatrix[0],     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,             tArticle->mSourceVector[0],         DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,             tArticle->mSourceVector[1],         DBL_EPSILON);
        CPPUNIT_ASSERT(true == tLoadR.getLoad()->getPowerValid());
        CPPUNIT_ASSERT(true == tLoadCp.getLoad()->getPowerValid());
        CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());

        /// @test Another step with updated current to get the voltage drop across the switch,
        ///       and sensor updates with drift.
        const double flux = expectedA * (tNodes[tPorts[0]].getPotential() - tNodes[tPorts[1]].getPotential());
        tArticle->mFlux = flux;
        tArticle->mShort.setMalfResistance();
        tArticle->mCurrentSensor.mMalfDriftFlag       = true;
        tArticle->mCurrentSensor.mMalfDriftRate       = 1.0;
        tArticle->mInputVoltageSensor.mMalfDriftFlag  = true;
        tArticle->mInputVoltageSensor.mMalfDriftRate  = 1.0;
        tArticle->mOutputVoltageSensor.mMalfDriftFlag = true;
        tArticle->mOutputVoltageSensor.mMalfDriftRate = 1.0;
        CPPUNIT_ASSERT_NO_THROW(tArticle->step(timestep));
        CPPUNIT_ASSERT(tArticle->mSwitch.getPosition());
        expectedLoadsV -= flux * expectedSwitchR;
        expectedLoadCpR = expectedLoadsV * expectedLoadsV / tLoadCpConfigData.mPowerNormal;
        expectedLoadsG  = 100.0 * DBL_EPSILON + 1.0 / expectedLoadRR + 1.0 / expectedLoadCpR;
        expectedG       = 1.0 / (expectedSwitchR + 1.0 / expectedLoadsG);
        expectedA       = expectedG * (1.0 - tInputData->mMalfBlockageValue);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSwitchR, tArticle->mSwitch.getResistance(),  DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedLoadRR,  tLoadR.getLoad()->getResistance(),  DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedLoadCpR, tLoadCp.getLoad()->getResistance(), DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedLoadsV,  tArticle->mLoadsVoltage,            DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG,       tArticle->mEffectiveConductivity,   DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedA,       tArticle->mAdmittanceMatrix[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(true == tLoadR.getLoad()->getPowerValid());
        CPPUNIT_ASSERT(true == tLoadCp.getLoad()->getPowerValid());
        CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());

        double expectedSensedI    = flux                          + tArticle->mCurrentSensor.mMalfDriftRate       * timestep;
        double expectedSensedVin  = tArticle->mPotentialVector[0] + tArticle->mInputVoltageSensor.mMalfDriftRate  * timestep;
        double expectedSensedVout = expectedLoadsV                + tArticle->mOutputVoltageSensor.mMalfDriftRate * timestep;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSensedI,    static_cast<double>(tArticle->mCurrentSensor.getSensedOutput()),       1.0e-3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSensedVin,  static_cast<double>(tArticle->mInputVoltageSensor.getSensedOutput()),  1.0e-3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSensedVout, static_cast<double>(tArticle->mOutputVoltageSensor.getSensedOutput()), 1.0e-3);

        /// @test Repeated admittance, and sensor drift is not updated in minorStep().
        CPPUNIT_ASSERT_NO_THROW(tArticle->minorStep(timestep, 0));
        CPPUNIT_ASSERT(false == tArticle->needAdmittanceUpdate());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSensedI,    static_cast<double>(tArticle->mCurrentSensor.getSensedOutput()),       1.0e-3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSensedVin,  static_cast<double>(tArticle->mInputVoltageSensor.getSensedOutput()),  1.0e-3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSensedVout, static_cast<double>(tArticle->mOutputVoltageSensor.getSensedOutput()), 1.0e-3);
    } {
        /// @test Switch open.
        tArticle->mSwitch.setPositionCommand(false);
        CPPUNIT_ASSERT_NO_THROW(tArticle->step(0.0));
        CPPUNIT_ASSERT(!tArticle->mSwitch.getPosition());
        double expectedLoadsV  = tNodes[tPorts[1]].getPotential();
        double expectedG       = 0.0;
        double expectedA       = expectedG * (1.0 - tInputData->mMalfBlockageValue);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedLoadsV,  tArticle->mLoadsVoltage,            DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG,       tArticle->mEffectiveConductivity,   DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedA,       tArticle->mAdmittanceMatrix[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(false == tLoadR.getLoad()->getPowerValid());
        CPPUNIT_ASSERT(false == tLoadCp.getLoad()->getPowerValid());
        CPPUNIT_ASSERT(true  == tArticle->needAdmittanceUpdate());
    } {
        /// @test Run-time port mapping commands, ground input port.
        tArticle->mUserPortSelect     = 0;
        tArticle->mUserPortSetControl = GunnsBasicLink::GROUND;
        tArticle->mPotentialVector[0] = 0.0;
        tArticle->mPotentialVector[1] = 0.0;
        tArticle->mSwitch.setPositionCommand(true);
        CPPUNIT_ASSERT_NO_THROW(tArticle->step(0.0));
        CPPUNIT_ASSERT(2         == tArticle->mNodeMap[0]);
        CPPUNIT_ASSERT(tPorts[1] == tArticle->mNodeMap[1]);

        /// @test Switch closed with zero input & output volts.
        CPPUNIT_ASSERT(tArticle->mSwitch.getPosition());
        double expectedLoadsV  = 0.0;
        double expectedSwitchR = tSwitchResistance;
        double expectedLoadRR  = UserLoadBase::MAXIMUM_RESISTANCE;
        double expectedLoadCpR = UserLoadBase::MAXIMUM_RESISTANCE;
        double expectedLoadsG  = 100.0 * DBL_EPSILON + 1.0 / expectedLoadRR + 1.0 / expectedLoadCpR;
        double expectedG       = 1.0 / (expectedSwitchR + 1.0 / expectedLoadsG);
        double expectedA       = expectedG * (1.0 - tInputData->mMalfBlockageValue);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSwitchR, tArticle->mSwitch.getResistance(), DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedLoadRR,  tLoadR.getLoad()->getResistance(),  DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedLoadCpR, tLoadCp.getLoad()->getResistance(), DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedLoadsV,  tArticle->mLoadsVoltage,            DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG,       tArticle->mEffectiveConductivity,   DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedA,       tArticle->mAdmittanceMatrix[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());

        /// @test Switch closed with negative current.
        tArticle->mPotentialVector[0] = -100.0;
        tArticle->mFlux               = -1.0;
        CPPUNIT_ASSERT_NO_THROW(tArticle->step(0.0));
        CPPUNIT_ASSERT(tArticle->mSwitch.getPosition());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSwitchR, tArticle->mSwitch.getResistance(), DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedLoadRR,  tLoadR.getLoad()->getResistance(),  DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedLoadCpR, tLoadCp.getLoad()->getResistance(), DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedLoadsV,  tArticle->mLoadsVoltage,            DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG,       tArticle->mEffectiveConductivity,   DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedA,       tArticle->mAdmittanceMatrix[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(false == tArticle->needAdmittanceUpdate());
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical User Load Switch Variant 2 link model step method without
///           user loads.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectUserLoadSwitch2::testStepNoLoads()
{
    UT_RESULT;

    /// - Initialize test article with nominal initialization data, no user loads, and reverse input
    ///   voltage.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPorts[1], tPorts[0]));

    tArticle->mPotentialVector[0] = tNodes[tPorts[0]].getPotential();
    tArticle->mPotentialVector[1] = tNodes[tPorts[1]].getPotential();

    {
        /// @test Switch closed, reverse voltage.
        tArticle->mSwitch.setPositionCommand(true);
        CPPUNIT_ASSERT_NO_THROW(tArticle->step(0.0));
        CPPUNIT_ASSERT(tArticle->mSwitch.getPosition());
        double expectedLoadsV  = 0.0;
        double expectedSwitchR = tSwitchResistance;
        double expectedG       = 1.0 / expectedSwitchR;
        double expectedA       = expectedG * (1.0 - tInputData->mMalfBlockageValue);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSwitchR, tArticle->mSwitch.getResistance(), DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedLoadsV,  tArticle->mLoadsVoltage,            DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG,       tArticle->mEffectiveConductivity,   DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedA,       tArticle->mAdmittanceMatrix[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical User Load Switch Variant 2 link model step method without
///           user loads and output connected to the Ground node.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectUserLoadSwitch2::testStepNoLoadsIsolated()
{
    UT_RESULT;

    /// - Initialize test article with nominal initialization data, no user loads, and reverse input
    ///   voltage.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPorts[0], tPorts[1]));

    tArticle->mPotentialVector[0] = tNodes[tPorts[0]].getPotential();
    tArticle->mPotentialVector[1] = tNodes[tPorts[1]].getPotential();

    {
        /// @test Switch is isolated when closed, forward voltage to Ground node.
        tArticle->mSwitch.setPositionCommand(true);
        CPPUNIT_ASSERT_NO_THROW(tArticle->step(0.0));
        CPPUNIT_ASSERT(tArticle->mSwitch.getPosition());
        double expectedLoadsV  = 0.0;
        double expectedSwitchR = tSwitchResistance;
        double expectedG       = 1.0 / GunnsBasicLink::mConductanceLimit;
        double expectedA       = expectedG * (1.0 - tInputData->mMalfBlockageValue);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSwitchR, tArticle->mSwitch.getResistance(), DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedLoadsV,  tArticle->mLoadsVoltage,            DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG,       tArticle->mEffectiveConductivity,   DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedA,       tArticle->mAdmittanceMatrix[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical User Load Switch Variant 2 link model step method with user
///           loads and the loads voltage override is active.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectUserLoadSwitch2::testStepLoadsOverride()
{
    UT_RESULT;

    /// - Initialize the user loads and add them to the test article.
    tLoadR .initLoad();
    tLoadCp.initLoad();
    CPPUNIT_ASSERT_NO_THROW(tArticle->addUserLoad(&tLoadR));
    CPPUNIT_ASSERT_NO_THROW(tArticle->addUserLoad(&tLoadCp));

    /// - Initialize test article with nominal initialization data and loads override active.
    tInputData->mLoadsOverrideActive = true;
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPorts[0], tPorts[1]));

    tArticle->mPotentialVector[0] = tNodes[tPorts[0]].getPotential();
    tArticle->mPotentialVector[1] = tNodes[tPorts[1]].getPotential();

    {
        /// @test Switch closed.
        tArticle->mSwitch.setPositionCommand(true);
        CPPUNIT_ASSERT_NO_THROW(tArticle->step(0.0));
        CPPUNIT_ASSERT(!tArticle->mSwitch.getPosition());
        double expectedLoadsV  = tLoadsOverrideVoltage;
        double expectedSwitchR = 1.0 / DBL_EPSILON;
        double expectedLoadRR  = tLoadRConfigData.mResistanceNormal;
        double expectedLoadCpR = expectedLoadsV * expectedLoadsV / tLoadCpConfigData.mPowerNormal;
        double expectedA       = 1.0E8;  // GunnsElectUserLoadSwitch2::mLoadsOverrideG
        double expectedW       = tLoadsOverrideVoltage * expectedA;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSwitchR, tArticle->mSwitch.getResistance(), DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedLoadRR,  tLoadR.getLoad()->getResistance(),  DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedLoadCpR, tLoadCp.getLoad()->getResistance(), DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedLoadsV,  tArticle->mLoadsVoltage,            DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,             tArticle->mEffectiveConductivity,   DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,             tArticle->mAdmittanceMatrix[0],     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedA,       tArticle->mAdmittanceMatrix[3],     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,             tArticle->mSourceVector[0],         DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW,       tArticle->mSourceVector[1],         DBL_EPSILON);
        CPPUNIT_ASSERT(true == tLoadR.getLoad()->getPowerValid());
        CPPUNIT_ASSERT(true == tLoadCp.getLoad()->getPowerValid());
        CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical User Load Switch Variant 2 link model computeFlows method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectUserLoadSwitch2::testComputeFlows()
{
    UT_RESULT;

    /// - Initialize the user loads and add them to the test article.
    tLoadRInputData.mInitialMode = LoadSTANDBY;
    tLoadR .initLoad();
    tLoadCp.initLoad();
    CPPUNIT_ASSERT_NO_THROW(tArticle->addUserLoad(&tLoadR));
    CPPUNIT_ASSERT_NO_THROW(tArticle->addUserLoad(&tLoadCp));

    /// - Initialize test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPorts[0], tPorts[1]));

    tArticle->mPotentialVector[0] = tNodes[tPorts[0]].getPotential();
    tArticle->mPotentialVector[1] = tNodes[tPorts[1]].getPotential();

    {
        /// @test Normal calculation of power and loads power, and sensor drift should not update.
        tArticle->mCurrentSensor.mMalfDriftFlag       = true;
        tArticle->mCurrentSensor.mMalfDriftRate       = 1.0;
        tArticle->mInputVoltageSensor.mMalfDriftFlag  = true;
        tArticle->mInputVoltageSensor.mMalfDriftRate  = 1.0;
        tArticle->mOutputVoltageSensor.mMalfDriftFlag = true;
        tArticle->mOutputVoltageSensor.mMalfDriftRate = 1.0;
        tArticle->mSwitch.setPositionCommand(true);
        tArticle->step(0.0);
        tArticle->minorStep(0.0, 2);
        tArticle->confirmSolutionAcceptable(2, 2);
        CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == tArticle->confirmSolutionAcceptable(2, 2));

        double expectedLoadsV  = tNodes[tPorts[0]].getPotential();
        double expectedSwitchR = tSwitchResistance;
        double expectedLoadRR  = tLoadRConfigData.mResistanceStandby;
        double expectedLoadCpR = expectedLoadsV * expectedLoadsV / tLoadCpConfigData.mPowerNormal;
        double expectedLoadsG  = 100.0 * DBL_EPSILON + 1.0 / expectedLoadRR + 1.0 / expectedLoadCpR;
        double expectedG       = 1.0 / (expectedSwitchR + 1.0 / expectedLoadsG);
        double expectedA       = expectedG * (1.0 - tInputData->mMalfBlockageValue);
        double expectedFlux    = expectedA * (tNodes[tPorts[0]].getPotential() - tNodes[tPorts[1]].getPotential());
        double expectedPower   = -expectedFlux * tNodes[tPorts[0]].getPotential();
        double expectedLoadPwr = -expectedPower - expectedFlux * expectedFlux * tSwitchResistance;

        CPPUNIT_ASSERT_NO_THROW(tArticle->computeFlows(0.0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,      tArticle->mFlux,                         DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower,     tArticle->mPower,                        DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedLoadPwr,   tArticle->mLoadsPower,                   DBL_EPSILON);

        double expectedSensedI    = expectedFlux;
        double expectedSensedVin  = tArticle->mPotentialVector[0];
        double expectedSensedVout = expectedLoadsV;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSensedI,    static_cast<double>(tArticle->mCurrentSensor.getSensedOutput()),       1.0e-3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSensedVin,  static_cast<double>(tArticle->mInputVoltageSensor.getSensedOutput()),  1.0e-3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSensedVout, static_cast<double>(tArticle->mOutputVoltageSensor.getSensedOutput()), 1.0e-3);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical User Load Switch Variant 2 link model computeFlows method
///           with user loads and the loads override mode active.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectUserLoadSwitch2::testComputeFlowsOverrideLoads()
{
    UT_RESULT;

    //TODO sensors

    /// - Initialize the user loads and add them to the test article.
    tLoadRInputData.mInitialMode = LoadSTANDBY;
    tLoadR .initLoad();
    tLoadCp.initLoad();
    CPPUNIT_ASSERT_NO_THROW(tArticle->addUserLoad(&tLoadR));
    CPPUNIT_ASSERT_NO_THROW(tArticle->addUserLoad(&tLoadCp));

    /// - Initialize test article with nominal initialization data and loads override active.
    tInputData->mLoadsOverrideActive = true;
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPorts[0], tPorts[1]));

    tArticle->mPotentialVector[0] = tNodes[tPorts[0]].getPotential();
    tArticle->mPotentialVector[1] = tNodes[tPorts[1]].getPotential();

    {
        /// @test Normal calculation of power and loads power.
        tArticle->mSwitch.setPositionCommand(true);
        tArticle->step(0.0);

        double expectedLoadsV  = tLoadsOverrideVoltage;
        double expectedLoadPwr = tLoadR.getLoad()->getPower() + tLoadCp.getLoad()->getPower();
        double expectedFlux    = expectedLoadPwr / expectedLoadsV;

        CPPUNIT_ASSERT_NO_THROW(tArticle->computeFlows(0.0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,             tArticle->mPotentialDrop,                DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,    tArticle->mFlux,                         DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedLoadPwr, tArticle->mPower,                        DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedLoadPwr, tArticle->mLoadsPower,                   DBL_EPSILON);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical User Load Switch Variant 2 link model computeFlows method
///           with no user loads, connected to a non-Ground output node, and the loads override mode
///           active.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectUserLoadSwitch2::testComputeFlowsOverrideNonGround()
{
    UT_RESULT;

    /// - Initialize test article with nominal initialization data and loads override active.
    tInputData->mLoadsOverrideActive = true;
    tPorts[1] = 1;
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPorts[0], tPorts[1]));

    tArticle->mPotentialVector[0] = tNodes[tPorts[0]].getPotential();
    tArticle->mPotentialVector[1] = tNodes[tPorts[1]].getPotential();

    {
        /// @test Normal calculation of power and loads power.
        tArticle->mSwitch.setPositionCommand(true);
        tArticle->step(0.0);

        double expectedDp   = -tNodes[tPorts[1]].getPotential();
        double expectedG    = 1.0E8;   // GunnsElectUserLoadSwitch2::mLoadsOverrideG
        double expectedFlux = (tLoadsOverrideVoltage - tNodes[tPorts[1]].getPotential()) * expectedG;
        double expectedPwr  = tNodes[tPorts[1]].getPotential() * expectedFlux;

        CPPUNIT_ASSERT_NO_THROW(tArticle->computeFlows(0.0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDp,   tArticle->mPotentialDrop,                DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mFlux,                         FLT_EPSILON * expectedFlux);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPwr,  tArticle->mPower,                        FLT_EPSILON * expectedPwr);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPwr,  tArticle->mLoadsPower,                   FLT_EPSILON * expectedPwr);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical User Load Switch Variant 2 link model checkSpecificPortRules
///           method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectUserLoadSwitch2::testPortRules()
{
    UT_RESULT;

    /// - Initialize the user loads and add them to the test article.
    tLoadR .initLoad();
    tLoadCp.initLoad();
    CPPUNIT_ASSERT_NO_THROW(tArticle->addUserLoad(&tLoadR));
    CPPUNIT_ASSERT_NO_THROW(tArticle->addUserLoad(&tLoadCp));

    /// @test port 1 cannot be connected to a non-Ground when there are user loads.
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPorts[1], tPorts[0]),
                         TsInitializationException);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical User Load Switch Variant 2 link model non-linear link
///           functions including switch trips.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectUserLoadSwitch2::testTripLogic()
{
    UT_RESULT;

    /// - Initialize the user loads and add them to the test article.  Configure the resistive load
    ///   such that it won't cause a trip in standby mode but will cause one in normal mode.
    tLoadRConfigData.mResistanceNormal = 1.2;
    tLoadRInputData.mInitialMode  = LoadSTANDBY;
    tLoadR2InputData.mInitialMode = LoadSTANDBY;
    tLoadR .initLoad();
    tLoadR2.initLoad();
    tLoadCp.initLoad();
    CPPUNIT_ASSERT_NO_THROW(tArticle->addUserLoad(&tLoadR));
    CPPUNIT_ASSERT_NO_THROW(tArticle->addUserLoad(&tLoadR2));
    CPPUNIT_ASSERT_NO_THROW(tArticle->addUserLoad(&tLoadCp));

    /// - Initialize test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPorts[0], tPorts[1]));

    tArticle->mPotentialVector[0] = tNodes[tPorts[0]].getPotential();
    tArticle->mPotentialVector[1] = tNodes[tPorts[1]].getPotential();

    {
        /// @test nominal operation, confirming a converged network solution with no trip.
        tArticle->mSwitch.setPositionCommand(true);
        tArticle->step(0.0);
        CPPUNIT_ASSERT(GunnsBasicLink::DELAY == tArticle->confirmSolutionAcceptable(0, 1));
        double expectedFlux = 0.0;
        CPPUNIT_ASSERT( tArticle->mSwitch.getPosition());
        CPPUNIT_ASSERT(!tArticle->mSwitch.isTripped());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,  tArticle->mFlux,            DBL_EPSILON);

        tArticle->minorStep(0.0, 2);
        CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == tArticle->confirmSolutionAcceptable(1, 2));
        expectedFlux = tArticle->mAdmittanceMatrix[0] * tNodes[tPorts[0]].getPotential();
        CPPUNIT_ASSERT( tArticle->mSwitch.getPosition());
        CPPUNIT_ASSERT(!tArticle->mSwitch.isTripped());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,  tArticle->mFlux,            DBL_EPSILON);

        tArticle->minorStep(0.0, 3);
        CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == tArticle->confirmSolutionAcceptable(2, 3));
        expectedFlux = tArticle->mAdmittanceMatrix[0] * tNodes[tPorts[0]].getPotential();
        CPPUNIT_ASSERT( tArticle->mSwitch.getPosition());
        CPPUNIT_ASSERT(!tArticle->mSwitch.isTripped());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,  tArticle->mFlux,            DBL_EPSILON);

        tArticle->minorStep(0.0, 4);
        CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == tArticle->confirmSolutionAcceptable(3, 4));
        expectedFlux = tArticle->mAdmittanceMatrix[0] * tNodes[tPorts[0]].getPotential();
        CPPUNIT_ASSERT( tArticle->mSwitch.getPosition());
        CPPUNIT_ASSERT(!tArticle->mSwitch.isTripped());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,  tArticle->mFlux,            DBL_EPSILON);
    } {
        /// @test fuse blows before the switch trips, rejecting the solution on first converged step.
        tLoadR2.getLoad()->setLoadOperMode(LoadON);
        tArticle->mFlux = 0.0;
        tArticle->step(0.0);
        CPPUNIT_ASSERT(GunnsBasicLink::DELAY == tArticle->confirmSolutionAcceptable(0, 1));
        double expectedFlux = 0.0;
        CPPUNIT_ASSERT(!tLoadR2.getLoad()->isFuseBlown());
        CPPUNIT_ASSERT( tArticle->mSwitch.getPosition());
        CPPUNIT_ASSERT(!tArticle->mSwitch.isTripped());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,  tArticle->mFlux,            DBL_EPSILON);

        tArticle->mPotentialVector[0] = 121.0;
        tArticle->minorStep(0.0, 2);
        CPPUNIT_ASSERT(!tLoadR2.getLoad()->isFuseBlown());
        CPPUNIT_ASSERT(GunnsBasicLink::REJECT == tArticle->confirmSolutionAcceptable(1, 2));
        CPPUNIT_ASSERT( tLoadR2.getLoad()->isFuseBlown());
        CPPUNIT_ASSERT( tArticle->mSwitch.getPosition());
        CPPUNIT_ASSERT(!tArticle->mSwitch.isTripped());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,  tArticle->mFlux,            DBL_EPSILON);
    } {
        /// @test switch trip due to sensor bias, rejecting the solution once.
        tArticle->mCurrentSensor.mMalfBiasFlag  = true;
        tArticle->mCurrentSensor.mMalfBiasValue = 200.0;
        tArticle->mPotentialVector[0] = tNodes[tPorts[0]].getPotential();
        tArticle->mFlux = 0.0;
        tArticle->step(0.0);
        CPPUNIT_ASSERT(GunnsBasicLink::DELAY == tArticle->confirmSolutionAcceptable(0, 1));
        double expectedFlux = 0.0;
        CPPUNIT_ASSERT( tArticle->mSwitch.getPosition());
        CPPUNIT_ASSERT(!tArticle->mSwitch.isTripped());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,  tArticle->mFlux,            DBL_EPSILON);

        tArticle->minorStep(0.0, 2);
        CPPUNIT_ASSERT(GunnsBasicLink::DELAY == tArticle->confirmSolutionAcceptable(1, 2));
        expectedFlux = tArticle->mAdmittanceMatrix[0] * tNodes[tPorts[0]].getPotential();
        CPPUNIT_ASSERT( tArticle->mSwitch.getPosition());
        CPPUNIT_ASSERT(!tArticle->mSwitch.isTripped());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,  tArticle->mFlux,            DBL_EPSILON);

        tArticle->minorStep(0.0, 3);
        CPPUNIT_ASSERT(GunnsBasicLink::REJECT == tArticle->confirmSolutionAcceptable(2, 3));
        expectedFlux = tArticle->mAdmittanceMatrix[0] * tNodes[tPorts[0]].getPotential();
        CPPUNIT_ASSERT(!tArticle->mSwitch.getPosition());
        CPPUNIT_ASSERT( tArticle->mSwitch.isTripped());
        CPPUNIT_ASSERT( tArticle->mSwitch.getPosOverCurrentTrip()->isTripped());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,  tArticle->mFlux,            DBL_EPSILON);

        tArticle->minorStep(0.0, 4);
        CPPUNIT_ASSERT(GunnsBasicLink::DELAY == tArticle->confirmSolutionAcceptable(0, 4));
        CPPUNIT_ASSERT(!tArticle->mSwitch.getPosition());
        CPPUNIT_ASSERT( tArticle->mSwitch.isTripped());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,  tArticle->mFlux,            DBL_EPSILON);

        tArticle->minorStep(0.0, 5);
        CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == tArticle->confirmSolutionAcceptable(1, 5));
        expectedFlux = 0.0;
        CPPUNIT_ASSERT(!tArticle->mSwitch.getPosition());
        CPPUNIT_ASSERT( tArticle->mSwitch.isTripped());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,  tArticle->mFlux,            DBL_EPSILON);

        tArticle->minorStep(0.0, 6);
        CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == tArticle->confirmSolutionAcceptable(2, 6));
        CPPUNIT_ASSERT(!tArticle->mSwitch.getPosition());
        CPPUNIT_ASSERT( tArticle->mSwitch.isTripped());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,  tArticle->mFlux,            DBL_EPSILON);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical User Load Switch Variant 2 link model restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectUserLoadSwitch2::testRestart()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPorts[0], tPorts[1]);

    tArticle->mEffectiveConductivity = 1.0;
    tArticle->mLoadsVoltage          = 1.0;
    tArticle->mLoadsPower            = 1.0;

    tArticle->restart();

    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mEffectiveConductivity);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mLoadsVoltage);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mLoadsPower);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical User Load Switch Variant 2 link model initialization
///           exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectUserLoadSwitch2::testInitializationExceptions()
{
    UT_RESULT;

    /// @test    Initialization exception invalid config data: no name.
    tConfigData->mName = "";
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPorts[0], tPorts[1]),
                         TsInitializationException);
    CPPUNIT_ASSERT(!tArticle->isInitialized());
    tConfigData->mName = tName;

    /// @test    Initialization exception from the switch.
    tConfigData->mSwitch.mResistance = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPorts[0], tPorts[1]),
                         TsInitializationException);
    CPPUNIT_ASSERT(!tArticle->isInitialized());
    tConfigData->mSwitch.mResistance = tSwitchResistance;

    /// @test    Initialization exception from a sensor.
    tConfigData->mInputVoltageSensor.mMaxRange = -99.9;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPorts[0], tPorts[1]),
                         TsInitializationException);
    CPPUNIT_ASSERT(!tArticle->isInitialized());
    tConfigData->mInputVoltageSensor.mMaxRange = tInputVoltageSensorMaxRange;

    /// @test    Initialization exception from adding a user load after link is initialized.
    CPPUNIT_ASSERT_NO_THROW(tArticle->addUserLoad(&tLoadR));
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPorts[0], tPorts[1]));
    CPPUNIT_ASSERT(tArticle->isInitialized());
    CPPUNIT_ASSERT_THROW(tArticle->addUserLoad(&tLoadCp), TsInitializationException);

    UT_PASS_LAST;
}
