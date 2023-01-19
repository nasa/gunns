/*
@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/
#include "software/exceptions/TsInitializationException.hh"
#include <iostream>
#include "strings/UtResult.hh"
#include "math/MsMath.hh"
#include "UtGunnsElectConverterInput.hh"

/// @details  Test identification number.
int UtGunnsElectConverterInput::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsElectConverterInput class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectConverterInput::UtGunnsElectConverterInput()
    :
    tLinks(),
    tNodes(),
    tNodeList(),
    tPort0(0),
    tName(""),
    tConfigData(0),
    tInputData(0),
    tArticle(0),
    tSensorVin(),
    tSensorIin(),
    tTripPriority(0),
    tInOverVoltageTrip(0.0),
    tInUnderVoltageTrip(0.0),
    tEfficiencyTable(0),
    tMalfBlockageFlag(false),
    tMalfBlockageValue(0.0),
    tEnabled(false),
    tInputVoltage(0.0),
    tInputPower(0.0),
    tReferencePower(0.0),
    tOutputConfigData(0),
    tOutputInputData(0),
    tOutputLink()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsElectConverterInput class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectConverterInput::~UtGunnsElectConverterInput()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectConverterInput::setUp()
{
    tName = "tArticle";

    /// - Define the nominal port mapping.
    tPort0 = 0;

    /// - Initialize the nodes list.
    tNodeList.mNodes    = tNodes;
    tNodeList.mNumNodes = N_NODES;

    /// - Initialize the sensors.
    GunnsSensorAnalogWrapperConfigData sensorConfig("tSensorVin", 0.0, 1.0E10);
    GunnsSensorAnalogWrapperInputData  sensorInput;
    tSensorVin.initialize(&sensorConfig, &sensorInput);
    tSensorIin.initialize(&sensorConfig, &sensorInput);

    /// - Define the nominal configuration data.
    tTripPriority       = 2;
    tInUnderVoltageTrip = 100.0;
    tInOverVoltageTrip  = 130.0;
    const double powPoints[] = {0.0, 1.0};
    const double effPoints[] = {0.5, 1.0};
    tEfficiencyTable    = new TsLinearInterpolator(powPoints, effPoints, 2, 0.0, 1.0);
    tConfigData         = new GunnsElectConverterInputConfigData(tName,
                                                                 &tNodeList,
                                                                 &tSensorVin,
                                                                 &tSensorIin,
                                                                 tTripPriority,
                                                                 tInUnderVoltageTrip,
                                                                 tInOverVoltageTrip,
                                                                 tEfficiencyTable);

    /// - Define the nominal input data.
    tMalfBlockageFlag  = true;
    tMalfBlockageValue = 0.1;
    tEnabled           = true;
    tInputVoltage      = 120.0;
    tInputPower        = 10.0;
    tReferencePower    = 100.0;
    tInputData         = new GunnsElectConverterInputInputData(tMalfBlockageFlag,
                                                               tMalfBlockageValue,
                                                               tEnabled,
                                                               tInputVoltage,
                                                               tInputPower,
                                                               tReferencePower);

    /// - Default construct the nominal test article.
    tArticle = new FriendlyGunnsElectConverterInput;

    /// - Define nominal output link config data.
    tOutputConfigData = new GunnsElectConverterOutputConfigData("tOutputLink",
                                                                &tNodeList,
                                                                GunnsElectConverterOutput::CURRENT,
                                                                1.0,
                                                                1.0,
                                                                0,
                                                                0,
                                                                3,
                                                                1.0e6,
                                                                1.0e6,
                                                                tArticle);

    /// - Define nominal output link input data.
    tOutputInputData = new GunnsElectConverterOutputInputData(false, 0.0, true, 120.0, 1.0);

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectConverterInput::tearDown()
{
    /// - Deletes for news in setUp
    delete tOutputInputData;
    delete tOutputConfigData;
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete tEfficiencyTable;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of Electrical Converter Input Link configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectConverterInput::testConfig()
{
    UT_RESULT_FIRST;

    /// @test    Configuration nominal construction.
    CPPUNIT_ASSERT(&tSensorVin         == tConfigData->mInputVoltageSensor);
    CPPUNIT_ASSERT(&tSensorIin         == tConfigData->mInputCurrentSensor);
    CPPUNIT_ASSERT(tTripPriority       == tConfigData->mTripPriority);
    CPPUNIT_ASSERT(tInUnderVoltageTrip == tConfigData->mInputUnderVoltageTripLimit);
    CPPUNIT_ASSERT(tInOverVoltageTrip  == tConfigData->mInputOverVoltageTripLimit);
    CPPUNIT_ASSERT(tEfficiencyTable    == tConfigData->mEfficiencyTable);

    /// @test    Configuration data default construction.
    GunnsElectConverterInputConfigData defaultConfig;
    CPPUNIT_ASSERT(0   == defaultConfig.mInputVoltageSensor);
    CPPUNIT_ASSERT(0   == defaultConfig.mInputCurrentSensor);
    CPPUNIT_ASSERT(0   == defaultConfig.mTripPriority);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mInputUnderVoltageTripLimit);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mInputOverVoltageTripLimit);
    CPPUNIT_ASSERT(0   == defaultConfig.mEfficiencyTable);

    /// @test    Configuration data copy construction.
    GunnsElectConverterInputConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(&tSensorVin         == copyConfig.mInputVoltageSensor);
    CPPUNIT_ASSERT(&tSensorIin         == copyConfig.mInputCurrentSensor);
    CPPUNIT_ASSERT(tTripPriority       == copyConfig.mTripPriority);
    CPPUNIT_ASSERT(tInUnderVoltageTrip == copyConfig.mInputUnderVoltageTripLimit);
    CPPUNIT_ASSERT(tInOverVoltageTrip  == copyConfig.mInputOverVoltageTripLimit);
    CPPUNIT_ASSERT(tEfficiencyTable    == copyConfig.mEfficiencyTable);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of Electrical Converter Input Link input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectConverterInput::testInput()
{
    UT_RESULT;

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(tMalfBlockageFlag  == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tEnabled           == tInputData->mEnabled);
    CPPUNIT_ASSERT(tInputVoltage      == tInputData->mInputVoltage);
    CPPUNIT_ASSERT(tInputPower        == tInputData->mInputPower);
    CPPUNIT_ASSERT(tReferencePower    == tInputData->mReferencePower);

    /// @test    Input data default construction.
    GunnsElectConverterInputInputData defaultInput;
    CPPUNIT_ASSERT(false == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0   == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(false == defaultInput.mEnabled);
    CPPUNIT_ASSERT(0.0   == defaultInput.mInputVoltage);
    CPPUNIT_ASSERT(0.0   == defaultInput.mInputPower);
    CPPUNIT_ASSERT(0.0   == defaultInput.mReferencePower);

    /// @test    Input data copy construction.
    GunnsElectConverterInputInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tMalfBlockageFlag  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(tEnabled           == copyInput.mEnabled);
    CPPUNIT_ASSERT(tInputVoltage      == copyInput.mInputVoltage);
    CPPUNIT_ASSERT(tInputPower        == copyInput.mInputPower);
    CPPUNIT_ASSERT(tReferencePower    == copyInput.mReferencePower);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the constructor of the GunnsElectConverterInput class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectConverterInput::testConstruction()
{
    UT_RESULT;

    /// @test    Default construction.
    CPPUNIT_ASSERT(0     == tArticle->mInputVoltageSensor);
    CPPUNIT_ASSERT(0     == tArticle->mInputCurrentSensor);
    CPPUNIT_ASSERT(0     == tArticle->mEfficiencyTable);
    CPPUNIT_ASSERT(0     == tArticle->mOutputLink);
    CPPUNIT_ASSERT(false == tArticle->mEnabled);
    CPPUNIT_ASSERT(0.0   == tArticle->mInputPower);
    CPPUNIT_ASSERT(false == tArticle->mInputPowerValid);
    CPPUNIT_ASSERT(false == tArticle->mResetTrips);
    CPPUNIT_ASSERT(0.0   == tArticle->mReferencePower);
    CPPUNIT_ASSERT(0.0   == tArticle->mInputVoltage);
    CPPUNIT_ASSERT(false == tArticle->mInputVoltageValid);
    CPPUNIT_ASSERT(false == tArticle->mInputUnderVoltageTrip.isTripped());
    CPPUNIT_ASSERT(false == tArticle->mInputOverVoltageTrip.isTripped());
    CPPUNIT_ASSERT(false == tArticle->mLeadsInterface);
    CPPUNIT_ASSERT(false == tArticle->mOverloadedState);
    CPPUNIT_ASSERT(false == tArticle->mLastOverloadedState);
    CPPUNIT_ASSERT(""    == tArticle->mName);

    /// @test    New/delete for code coverage.
    GunnsElectConverterInput* testArticle = new GunnsElectConverterInput();
    delete testArticle;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Electrical Converter Input Link nominal initialization without
///           exceptions, supplying a custom strings load order list.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectConverterInput::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));

    /// @test    Efficiency table.
    CPPUNIT_ASSERT(tEfficiencyTable == tArticle->mEfficiencyTable);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT(tMalfBlockageFlag  == tArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue == tArticle->mMalfBlockageValue);
    CPPUNIT_ASSERT(tInputVoltage      == tArticle->mInputVoltage);
    CPPUNIT_ASSERT(tInputPower        == tArticle->mInputPower);
    CPPUNIT_ASSERT(tEnabled           == tArticle->mEnabled);
    CPPUNIT_ASSERT(tReferencePower    == tArticle->mReferencePower);

    /// @test    Sensors.
    CPPUNIT_ASSERT(&tSensorVin.mSensor == tArticle->mInputVoltageSensor);
    CPPUNIT_ASSERT(&tSensorIin.mSensor == tArticle->mInputCurrentSensor);

    /// @test    Trips package.
    GunnsBasicLink::SolutionResult result;
    CPPUNIT_ASSERT(false == tArticle->mInputUnderVoltageTrip.isTripped());
    CPPUNIT_ASSERT(false == tArticle->mInputOverVoltageTrip.isTripped());
    CPPUNIT_ASSERT(true  == tArticle->mInputUnderVoltageTrip.checkForTrip(result, tInUnderVoltageTrip - 0.01, tTripPriority));
    CPPUNIT_ASSERT(true  == tArticle->mInputOverVoltageTrip.checkForTrip(result, tInOverVoltageTrip + 0.01, tTripPriority));

    /// @test    Nominal state data.
    CPPUNIT_ASSERT(false == tArticle->mResetTrips);
    CPPUNIT_ASSERT(false == tArticle->mOverloadedState);
    CPPUNIT_ASSERT(tName == tArticle->mName);
    CPPUNIT_ASSERT(true  == tArticle->mInitFlag);
    CPPUNIT_ASSERT(tInputVoltage == tNodes[0].getPotential());

    /// @test    Register output link.
    CPPUNIT_ASSERT_NO_THROW(tOutputLink.initialize(*tOutputConfigData, *tOutputInputData, tLinks, 1));
    CPPUNIT_ASSERT_NO_THROW(tArticle->registerOutputLink(&tOutputLink));
    CPPUNIT_ASSERT(&tOutputLink == tArticle->mOutputLink);

    /// @test    Re-init with nominal output link provided, and no input sensors.
    tConfigData->mInputVoltageSensor = 0;
    tConfigData->mInputCurrentSensor = 0;
    tArticle->mInputVoltageSensor    = 0;
    tArticle->mInputCurrentSensor    = 0;
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));
    CPPUNIT_ASSERT(false == tArticle->mLeadsInterface);
    CPPUNIT_ASSERT(0     == tArticle->mInputVoltageSensor);
    CPPUNIT_ASSERT(0     == tArticle->mInputCurrentSensor);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Electrical Converter Input Link nominal initialization with exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectConverterInput::testInitializationErrors()
{
    UT_RESULT;

    /// @test    Exception thrown for under-volt trip limit > over-volt limit.
    tConfigData->mInputUnderVoltageTripLimit = tInOverVoltageTrip + 0.001;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0), TsInitializationException);
    tConfigData->mInputUnderVoltageTripLimit = tInUnderVoltageTrip;

    /// @test    Exception not thrown for no efficiency table and zero reference power.
    tConfigData->mEfficiencyTable = 0;
    tInputData->mReferencePower   = 0.0;
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));

    /// @test    Exception thrown for efficiency table with zero reference power.
    tConfigData->mEfficiencyTable = tEfficiencyTable;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0), TsInitializationException);

    /// @test    Exception thrown for efficiency table values out of range.
    const double badPowPoints[] = {0.0, 1.0};
    const double badEffPoints[] = {0.0, 1.0};
    TsLinearInterpolator* badEffTable = new TsLinearInterpolator(badPowPoints, badEffPoints, 2, 0.0, 1.0);
    tConfigData->mEfficiencyTable = badEffTable;
    tInputData->mReferencePower   = tReferencePower;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0), TsInitializationException);
    tConfigData->mEfficiencyTable = tEfficiencyTable;
    delete badEffTable;

    /// @test    Exception not thrown for under-volt trip limit > over-volt limit but over-volt
    ///          limit = 0.
    tConfigData->mInputOverVoltageTripLimit = 0.0;
    GunnsElectConverterInput article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0));
    tConfigData->mInputOverVoltageTripLimit = tInOverVoltageTrip;

    /// @test    Exception thrown for node list mismatch with output link.
    GunnsBasicNode otherNodes[N_NODES];
    GunnsNodeList  otherNodeList;
    otherNodeList.mNodes         = otherNodes;
    otherNodeList.mNumNodes      = N_NODES;
    tOutputConfigData->mNodeList = &otherNodeList;
    CPPUNIT_ASSERT_NO_THROW(tOutputLink.initialize(*tOutputConfigData, *tOutputInputData, tLinks, 1));
    tArticle->registerOutputLink(&tOutputLink);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0), TsInitializationException);

    CPPUNIT_ASSERT(false == tArticle->mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Electrical Converter Input Link restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectConverterInput::testRestart()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));

    /// @test    Restart method clears non-config and non-checkpointed data.
    tArticle->mResetTrips          = true;
    tArticle->mOverloadedState     = true;
    tArticle->mLastOverloadedState = true;
    tArticle->restart();
    CPPUNIT_ASSERT(false == tArticle->mResetTrips);
    CPPUNIT_ASSERT(false == tArticle->mOverloadedState);
    CPPUNIT_ASSERT(false == tArticle->mLastOverloadedState);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Electrical Converter Input Link step method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectConverterInput::testStep()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));

    {
        /// @test    Step and minorStep with normal inputs.
        double nodeV       = 120.0;
        double expectedG   = 0.0;
        double expectedEff = 0.9 * (0.5 + 0.5 * tInputPower / tReferencePower);
        double expectedPwr = tInputPower / expectedEff;
        double expectedQ   = expectedPwr - tInputPower;
        double expectedW   = -expectedPwr / nodeV;
        tArticle->mOverloadedState = true;
        tArticle->mPotentialVector[0] = nodeV;
        tArticle->step(0.0);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG,   tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedEff, tArticle->mConverterEfficiency, DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedQ,   tArticle->mTotalPowerLoss,      DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW,   tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(false == tArticle->needAdmittanceUpdate());
        CPPUNIT_ASSERT(false == tArticle->mOverloadedState);

        /// @test    Reset trips when commanded.
        GunnsBasicLink::SolutionResult result;
        CPPUNIT_ASSERT(true == tArticle->mInputUnderVoltageTrip.checkForTrip(result, tInUnderVoltageTrip - 0.01, tTripPriority));
        CPPUNIT_ASSERT(true == tArticle->mInputOverVoltageTrip.checkForTrip(result, tInOverVoltageTrip + 0.01, tTripPriority));
        CPPUNIT_ASSERT(true == tArticle->mInputUnderVoltageTrip.isTripped());
        CPPUNIT_ASSERT(true == tArticle->mInputOverVoltageTrip.isTripped());
        tArticle->mResetTrips = true;
        tArticle->step(0.0);

        CPPUNIT_ASSERT(false == tArticle->mInputUnderVoltageTrip.isTripped());
        CPPUNIT_ASSERT(false == tArticle->mInputOverVoltageTrip.isTripped());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG,   tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedEff, tArticle->mConverterEfficiency, DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW,   tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(false == tArticle->needAdmittanceUpdate());
        CPPUNIT_ASSERT(false == tArticle->mOverloadedState);
        CPPUNIT_ASSERT(false == tArticle->mResetTrips);

        /// @test    No efficiency table and no blockage malf.
        tArticle->setMalfBlockage();
        tArticle->mEfficiencyTable = 0;
        expectedEff = 1.0;
        expectedPwr = tInputPower / expectedEff;
        expectedQ   = expectedPwr - tInputPower;
        expectedW   = -expectedPwr / nodeV;
        tArticle->step(0.0);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG,   tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedEff, tArticle->mConverterEfficiency, DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW,   tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(false == tArticle->needAdmittanceUpdate());
        CPPUNIT_ASSERT(false == tArticle->mOverloadedState);
        CPPUNIT_ASSERT(false == tArticle->mResetTrips);

        /// @test    Trips not reset when not commanded.
        CPPUNIT_ASSERT(true == tArticle->mInputUnderVoltageTrip.checkForTrip(result, tInUnderVoltageTrip - 0.01, tTripPriority));
        CPPUNIT_ASSERT(true == tArticle->mInputOverVoltageTrip.checkForTrip(result, tInOverVoltageTrip + 0.01, tTripPriority));
        CPPUNIT_ASSERT(true == tArticle->mInputUnderVoltageTrip.isTripped());
        CPPUNIT_ASSERT(true == tArticle->mInputOverVoltageTrip.isTripped());
        expectedW = 0.0;
        tArticle->step(0.0);

        CPPUNIT_ASSERT(true == tArticle->mInputUnderVoltageTrip.isTripped());
        CPPUNIT_ASSERT(true == tArticle->mInputOverVoltageTrip.isTripped());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG,   tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedEff, tArticle->mConverterEfficiency, DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW,   tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(false == tArticle->needAdmittanceUpdate());
        CPPUNIT_ASSERT(false == tArticle->mResetTrips);
    } {
        /// @test    User port commands.
        tArticle->mUserPortSelect     = 0;
        tArticle->mUserPortSelectNode = 1;
        tArticle->mUserPortSetControl = GunnsBasicLink::EXECUTE;
        tArticle->step(0.0);
        CPPUNIT_ASSERT(GunnsBasicLink::READY == tArticle->mUserPortSetControl);
        CPPUNIT_ASSERT(1                     == tArticle->mNodeMap[0]);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the Electrical Converter Input Link minorStep method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectConverterInput::testMinorStep()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data and a
    ///   registered output link.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);
    tOutputLink.initialize(*tOutputConfigData, *tOutputInputData, tLinks, 1);
    CPPUNIT_ASSERT(true == tArticle->mLeadsInterface);

    {
        /// - Set output link state to generate the desired input power for the test.
        double inV  = 120.0;
        double outI = 0.1;
        tOutputLink.mAdmittanceMatrix[0] = 0.0;
        tOutputLink.mSourceVector[0]     = outI;
        tOutputLink.mPotentialVector[0]  = inV;

        /// @test    Step and minorStep with normal input load from the output link.
        double outP      = inV * outI;
        double outPloss  = outI * outI / tOutputConfigData->mOutputConductance;
        double expectedP = (outP + outPloss) / tOutputConfigData->mConverterEfficiency;
        double expectedG = 0.0;
        double expectedW = -expectedP / inV;
        tOutputLink.mInputPower       = expectedP;
        tArticle->mPotentialVector[0] = inV;
        tArticle->setMalfBlockage();
        tArticle->mEfficiencyTable = 0;
        tArticle->step(0.0);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mInputPower,          DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(inV,       tOutputLink.mInputVoltage,      DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(false == tArticle->needAdmittanceUpdate());
        CPPUNIT_ASSERT(false == tArticle->mOverloadedState);

        /// @test    Step and minorStep with zero input voltage.
        expectedW = 0.0;
        tArticle->mPotentialVector[0] = 0.0;
        tArticle->step(0.0);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(false == tArticle->needAdmittanceUpdate());
        CPPUNIT_ASSERT(false == tArticle->mOverloadedState);

        /// @test    minorStep in the overloaded state.
        tArticle->mPotentialVector[0] = inV;
        tArticle->mOverloadedState    = true;
        tArticle->minorStep(0.0, 0);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(false == tArticle->needAdmittanceUpdate());
        CPPUNIT_ASSERT(true  == tArticle->mOverloadedState);

        /// @test    minorStep when negative input voltage and not overloaded state.
        expectedW                     = -1.0;
        tArticle->mSourceVector[0]    = expectedW;
        tArticle->mPotentialVector[0] = -1.0;
        tArticle->mOverloadedState    = false;
        tArticle->minorStep(0.0, 0);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(false == tArticle->needAdmittanceUpdate());
        CPPUNIT_ASSERT(false == tArticle->mOverloadedState);

        /// @test    minorStep zeroes admittance.
        tArticle->mAdmittanceMatrix[0] = 1.0;
        tArticle->minorStep(0.0, 0);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(true  == tArticle->needAdmittanceUpdate());
        CPPUNIT_ASSERT(false == tArticle->mOverloadedState);

        /// @test    minorStep when output link has invalid power.
        tOutputLink.mInputPowerValid = false;
        tArticle->minorStep(0.0, 0);
        CPPUNIT_ASSERT(false == tArticle->mInputPowerValid);

        /// @test    minorStep gets mInputPowerValid from output link when we don't lead interface.
        tArticle->mInputPowerValid = true;
        tArticle->mLeadsInterface  = false;
        tArticle->minorStep(0.0, 0);
        CPPUNIT_ASSERT(false == tArticle->mInputPowerValid);

        /// @test    minorStep resets mInputPowerValid when there is no output link.
        tArticle->mLeadsInterface = false;
        tArticle->mOutputLink     = 0;
        tArticle->minorStep(0.0, 0);
        CPPUNIT_ASSERT(true == tArticle->mInputPowerValid);
    } {
        /// @test    step and minorStep when connected to the Ground node.
        tArticle->mUserPortSelect      = 0;
        tArticle->mUserPortSetControl  = GunnsBasicLink::GROUND;
        tArticle->mInputPower          = 1.0;
        tArticle->mAdmittanceMatrix[0] = 1.0;
        tArticle->mSourceVector[0]     = 1.0;
        tArticle->step(0.0);

        CPPUNIT_ASSERT(0.0 == tArticle->mInputPower);
        CPPUNIT_ASSERT(0.0 == tArticle->mAdmittanceMatrix[0]);
        CPPUNIT_ASSERT(0.0 == tArticle->mSourceVector[0]);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the Electrical Converter Input Link computeInputVoltage method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectConverterInput::testComputeInputVoltage()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);

    /// @test    Normal input voltage.
    double nodeV   = 120.0;
    double actualV = 0.0;
    tArticle->mPotentialVector[0] = nodeV;
    CPPUNIT_ASSERT(true  == tArticle->computeInputVoltage(actualV));
    CPPUNIT_ASSERT(nodeV == actualV);

    /// @test    Negative node voltage.
    nodeV = -1.0;
    tArticle->mPotentialVector[0] = nodeV;
    CPPUNIT_ASSERT(true  == tArticle->computeInputVoltage(actualV));
    CPPUNIT_ASSERT(0.0   == actualV);

    /// @test    Fully blocked.
    nodeV = 120.0;
    tArticle->mPotentialVector[0] = nodeV;
    tArticle->mMalfBlockageValue  = 1.0;
    tArticle->mOverloadedState    = false;
    CPPUNIT_ASSERT(true  == tArticle->computeInputVoltage(actualV));
    CPPUNIT_ASSERT(0.0   == actualV);
    tArticle->mMalfBlockageFlag = false;

    /// @test    Disabled.
    tArticle->mEnabled = false;
    CPPUNIT_ASSERT(true  == tArticle->computeInputVoltage(actualV));
    CPPUNIT_ASSERT(0.0   == actualV);
    CPPUNIT_ASSERT(false == tArticle->mOverloadedState);
    tArticle->mEnabled = true;

    /// @test    Over-volt tripped.
    GunnsBasicLink::SolutionResult result;
    CPPUNIT_ASSERT(true  == tArticle->mInputOverVoltageTrip.checkForTrip(result, tInOverVoltageTrip + 0.01, tTripPriority));
    CPPUNIT_ASSERT(true  == tArticle->computeInputVoltage(actualV));
    CPPUNIT_ASSERT(0.0   == actualV);
    tArticle->mInputOverVoltageTrip.resetTrip();

    /// @test    Under-volt tripped.
    CPPUNIT_ASSERT(true  == tArticle->mInputUnderVoltageTrip.checkForTrip(result, tInUnderVoltageTrip - 0.01, tTripPriority));
    CPPUNIT_ASSERT(true  == tArticle->computeInputVoltage(actualV));
    CPPUNIT_ASSERT(0.0   == actualV);
    tArticle->mInputUnderVoltageTrip.resetTrip();

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the Electrical Converter Input Link getter and setter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectConverterInput::testAccessors()
{
    UT_RESULT;

    /// @test    Link is non-linear.
    CPPUNIT_ASSERT(true == tArticle->isNonLinear());

    /// @test    Can set the enabled flag.
    tArticle->setEnabled(true);
    CPPUNIT_ASSERT(true == tArticle->mEnabled);

    /// @test    Can set the input power.
    tArticle->setInputPower(15.0);
    CPPUNIT_ASSERT(15.0 == tArticle->mInputPower);

    /// @test    Set the reference power.
    tArticle->setReferencePower(42.0);
    CPPUNIT_ASSERT(42.0 == tArticle->mReferencePower);

    /// @test    Get the trip logics.
    CPPUNIT_ASSERT(&tArticle->mInputOverVoltageTrip  == tArticle->getInputOverVoltageTrip());
    CPPUNIT_ASSERT(&tArticle->mInputUnderVoltageTrip == tArticle->getInputUnderVoltageTrip());

    /// @test    Get the input voltage.
    tArticle->mInputVoltage = 5.0;
    CPPUNIT_ASSERT(5.0 == tArticle->getInputVoltage());

    /// @test    Get the input voltage valid flag.
    tArticle->mInputVoltageValid = true;
    CPPUNIT_ASSERT(true == tArticle->getInputVoltageValid());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the confirmSolutionAcceptable method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectConverterInput::testConfirmSolutionAcceptable()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);

    /// - Set up a trip condition (under-volt):
    tArticle->mPotentialVector[0] = 1.0;

    /// @test    Confirms on Ground node.
    tArticle->mUserPortSelect     = 0;
    tArticle->mUserPortSetControl = GunnsBasicLink::GROUND;
    tArticle->step(0.0);
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == tArticle->confirmSolutionAcceptable(1, 1));

    /// @test    Confirms when network isn't converged.
    tArticle->mUserPortSelect     = 0;
    tArticle->mUserPortSetControl = GunnsBasicLink::DEFAULT;
    tArticle->step(0.0);
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == tArticle->confirmSolutionAcceptable(0, 1));
    CPPUNIT_ASSERT(true == tArticle->mInputVoltageValid);

    /// @test    Confirms when sensor outputs value that doens't cause trip.
    tSensorVin.mSensor.mMalfFailToFlag  = true;
    tSensorVin.mSensor.mMalfFailToValue = 120.0;
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == tArticle->confirmSolutionAcceptable(tTripPriority-1, 1));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(120.0, tSensorVin.mSensor.getSensedOutput(), DBL_EPSILON);
    CPPUNIT_ASSERT(true == tArticle->mInputVoltageValid);

    /// @test    Delays in trip-able condition but trip priority not yet met.
    tSensorVin.mSensor.mMalfFailToFlag  = false;
    CPPUNIT_ASSERT(GunnsBasicLink::DELAY == tArticle->confirmSolutionAcceptable(tTripPriority-1, 1));
    CPPUNIT_ASSERT(!tArticle->mInputUnderVoltageTrip.isTripped());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tSensorVin.mSensor.getSensedOutput(), DBL_EPSILON);
    CPPUNIT_ASSERT(true  == tArticle->mInputVoltageValid);
    CPPUNIT_ASSERT(false == tArticle->mOverloadedState);

    /// @test    Rejects due to undervolt trip from sensor.
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT == tArticle->confirmSolutionAcceptable(tTripPriority, 1));
    CPPUNIT_ASSERT(tArticle->mInputUnderVoltageTrip.isTripped());
    CPPUNIT_ASSERT(false == tArticle->mInputVoltageValid);
    tArticle->mInputUnderVoltageTrip.resetTrip();

    /// @test    Rejects due to overvolt trip from sensor.
    tArticle->mPotentialVector[0] = 131.0;
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT == tArticle->confirmSolutionAcceptable(tTripPriority, 1));
    CPPUNIT_ASSERT(tArticle->mInputOverVoltageTrip.isTripped());
    CPPUNIT_ASSERT(false == tArticle->mInputVoltageValid);
    tArticle->mInputOverVoltageTrip.resetTrip();

    /// @test    Rejects due to undervolt trip with no sensor.
    tArticle->mPotentialVector[0] = 1.0;
    tArticle->mInputVoltageSensor = 0;
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT == tArticle->confirmSolutionAcceptable(tTripPriority, 1));
    CPPUNIT_ASSERT(tArticle->mInputUnderVoltageTrip.isTripped());
    CPPUNIT_ASSERT(false == tArticle->mInputVoltageValid);
    tArticle->mInputUnderVoltageTrip.resetTrip();

    /// @test    Rejects due to overvolt trip with no sensor.
    tArticle->mPotentialVector[0] = 131.0;
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT == tArticle->confirmSolutionAcceptable(tTripPriority, 1));
    CPPUNIT_ASSERT(tArticle->mInputOverVoltageTrip.isTripped());
    CPPUNIT_ASSERT(false == tArticle->mInputVoltageValid);
    tArticle->mInputOverVoltageTrip.resetTrip();

    /// @test    Doesn't reject or enter overloaded state if the network converged on a negative
    ///          voltage but we have no input power.
    tArticle->mPotentialVector[0] = -1.0;
    tArticle->mInputPower         =  0.0;
    tArticle->mInputUnderVoltageTrip.mMalfInhibitTrip = true;
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == tArticle->confirmSolutionAcceptable(tTripPriority-1, 1));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mInputVoltage, DBL_EPSILON);
    CPPUNIT_ASSERT(true  == tArticle->mInputVoltageValid);
    CPPUNIT_ASSERT(false == tArticle->mOverloadedState);

    /// @test    Rejects due to entering overload state because the network converged on a negative
    ///          voltage but the undervolt trip failed and we have input power.
    tArticle->mInputPower         =  1.0;
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT == tArticle->confirmSolutionAcceptable(tTripPriority-1, 1));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mInputVoltage, DBL_EPSILON);
    CPPUNIT_ASSERT(false == tArticle->mInputVoltageValid);
    CPPUNIT_ASSERT(true  == tArticle->mOverloadedState);

    /// @test    Rejects due to invalid power.
    tArticle->mPotentialVector[0] = 120.0;
    tArticle->mInputPowerValid    = false;
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT == tArticle->confirmSolutionAcceptable(tTripPriority, 1));
    CPPUNIT_ASSERT(true  == tArticle->mInputVoltageValid);

    /// @test    Rejects due to invalid power from the output link.
    FriendlyGunnsElectConverterInput article2;
    article2.initialize(*tConfigData, *tInputData, tLinks, tPort0);
    tOutputConfigData->mInputLink = &article2;
    tOutputLink.initialize(*tOutputConfigData, *tOutputInputData, tLinks, 1);
    tOutputLink.mInputPowerValid = false;
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT == article2.confirmSolutionAcceptable(tTripPriority, 1));
    CPPUNIT_ASSERT(false == article2.mInputVoltageValid);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the resetLastMinorStep method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectConverterInput::testResetLastMinorStep()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);
    tArticle->mPotentialVector[0] = -1.0;

    /// @test    Does nothing when network is not converged.
    CPPUNIT_ASSERT(true == tArticle->resetLastMinorStep(0, 1));
    CPPUNIT_ASSERT(-1.0 == tArticle->mPotentialVector[0]);

    /// @test    Leaves positive potential vector alone.
    tArticle->mPotentialVector[0] = 1.0;
    CPPUNIT_ASSERT(true == tArticle->resetLastMinorStep(1, 1));
    CPPUNIT_ASSERT(1.0  == tArticle->mPotentialVector[0]);

    /// @test    Resets negative potential vector when network is converged.
    tArticle->mPotentialVector[0] = -1.0;
    CPPUNIT_ASSERT(true == tArticle->resetLastMinorStep(1, 1));
    CPPUNIT_ASSERT(0.0  == tArticle->mPotentialVector[0]);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the computeFlows method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectConverterInput::testComputeFlows()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);

    /// @test    Output as current source.
    double flux        = 1.0;
    double conductance = 0.0;
    double nodeV       = 120.0;
    double expectedP   = -flux * nodeV;
    tArticle->mSourceVector[0]     = -flux;
    tArticle->mAdmittanceMatrix[0] = conductance;
    tArticle->mPotentialVector[0]  = nodeV;

    tArticle->computeFlows(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(nodeV,     tArticle->mInputVoltage,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(nodeV,     tArticle->mPotentialDrop, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(flux,      tArticle->mFlux,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mPower,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(flux,      tNodes[0].getOutflux(),   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(flux,      tArticle->mInputCurrentSensor->getSensedOutput(), FLT_EPSILON);
    tNodes[0].resetFlows();

    /// @test    Output as conductor.
    conductance = GunnsBasicLink::mConductanceLimit;
    nodeV       = DBL_EPSILON;
    flux        = nodeV * conductance;
    expectedP   = -flux * nodeV;
    tArticle->mSourceVector[0]     = 0.0;
    tArticle->mAdmittanceMatrix[0] = conductance;
    tArticle->mPotentialVector[0]  = nodeV;

    tArticle->computeFlows(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(nodeV,     tArticle->mInputVoltage,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(nodeV,     tArticle->mPotentialDrop, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(flux,      tArticle->mFlux,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mPower,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(flux,      tNodes[0].getOutflux(),   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(flux,      tArticle->mInputCurrentSensor->getSensedOutput(), FLT_EPSILON);
    tNodes[0].resetFlows();

    /// @test    Doesn't transport negative flux, and update with no current sensor.
    conductance = 0.0;
    flux        = -1.0;
    expectedP   = -flux * nodeV;
    tArticle->mSourceVector[0]     = -flux;
    tArticle->mAdmittanceMatrix[0] = conductance;
    tArticle->mInputCurrentSensor  = 0;

    tArticle->computeFlows(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(flux,      tArticle->mFlux,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mPower,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,       tNodes[0].getOutflux(),   DBL_EPSILON);
    tNodes[0].resetFlows();

    /// @test    On Ground node.
    tArticle->mUserPortSelect     = 0;
    tArticle->mUserPortSetControl = GunnsBasicLink::GROUND;
    tArticle->step(0.0);

    tArticle->computeFlows(0.0);
    CPPUNIT_ASSERT(0.0 == tArticle->mInputVoltage);
    CPPUNIT_ASSERT(0.0 == tArticle->mPotentialDrop);
    CPPUNIT_ASSERT(0.0 == tArticle->mFlux);
    CPPUNIT_ASSERT(0.0 == tArticle->mPower);
    CPPUNIT_ASSERT(0.0 == tNodes[0].getOutflux());

    UT_PASS_LAST;
}
