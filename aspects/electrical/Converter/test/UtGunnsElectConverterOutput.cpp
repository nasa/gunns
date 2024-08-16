/**
@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((aspects/electrical/Converter/GunnsElectConverterInput.o)
   (software/exceptions/TsInitializationException.o))
*/
#include "software/exceptions/TsInitializationException.hh"
#include <iostream>
#include "strings/UtResult.hh"
#include "math/MsMath.hh"
#include "UtGunnsElectConverterOutput.hh"

/// @details  Test identification number.
int UtGunnsElectConverterOutput::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsElectConverterOutput class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectConverterOutput::UtGunnsElectConverterOutput()
    :
    tLinks(),
    tNodes(),
    tNodeList(),
    tPort0(0),
    tName(""),
    tConfigData(0),
    tInputData(0),
    tArticle(0),
    tSensorVout(),
    tSensorIout(),
    tRegulatorType(),
    tOutputConductance(0.0),
    tConverterEfficiency(0.0),
    tTripPriority(0),
    tOutOverVoltageTrip(0.0),
    tOutUnderVoltageTrip(0.0),
    tOutOverCurrentTrip(0.0),
    tEnableLimit(false),
    tMalfBlockageFlag(false),
    tMalfBlockageValue(0.0),
    tEnabled(false),
    tInputVoltage(0.0),
    tInputPower(0.0),
    tSetpoint(0.0),
    tInputConfigData(0),
    tInputInputData(0),
    tInputLink()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsElectConverterOutput class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectConverterOutput::~UtGunnsElectConverterOutput()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectConverterOutput::setUp()
{
    tName = "tArticle";

    /// - Define the nominal port mapping.
    tPort0 = 0;

    /// - Initialize the nodes list.
    tNodeList.mNodes    = tNodes;
    tNodeList.mNumNodes = N_NODES;

    /// - Initialize the sensors.
    {
        GunnsSensorAnalogWrapperConfigData sensorConfig("tSensorVout", 0.0, 1.0E10);
        GunnsSensorAnalogWrapperInputData  sensorInput;
        tSensorVout.initialize(&sensorConfig, &sensorInput);
    } {
        GunnsSensorAnalogWrapperConfigData sensorConfig("tSensorIout", 0.0, 1.0E10);
        GunnsSensorAnalogWrapperInputData  sensorInput;
        tSensorIout.initialize(&sensorConfig, &sensorInput);
    }

    /// - Define the nominal configuration data.
    tRegulatorType       = GunnsElectConverterOutput::TRANSFORMER;
    tOutputConductance   = 10.0;
    tConverterEfficiency = 0.9;
    tTripPriority        = 2;
    tOutOverVoltageTrip  = 130.0;
    tOutUnderVoltageTrip = 90.0;
    tOutOverCurrentTrip  = 100.0;
    tEnableLimit         = true;
    tConfigData          = new GunnsElectConverterOutputConfigData(tName,
                                                                  &tNodeList,
                                                                  tRegulatorType,
                                                                  tOutputConductance,
                                                                  tConverterEfficiency,
                                                                  &tSensorVout,
                                                                  &tSensorIout,
                                                                  tTripPriority,
                                                                  tOutOverVoltageTrip,
                                                                  tOutOverCurrentTrip,
                                                                  &tInputLink,
                                                                  tEnableLimit,
                                                                  tOutUnderVoltageTrip);

    /// - Define the nominal input data.
    tMalfBlockageFlag  = true;
    tMalfBlockageValue = 0.1;
    tEnabled           = true;
    tInputVoltage      = 120.0;
    tInputPower        = 10.0;
    tSetpoint          = 1.0;
    tInputData         = new GunnsElectConverterOutputInputData(tMalfBlockageFlag,
                                                                tMalfBlockageValue,
                                                                tEnabled,
                                                                tInputVoltage,
                                                                tInputPower,
                                                                tSetpoint);

    /// - Default construct the nominal test article.
    tArticle = new FriendlyGunnsElectConverterOutput;

    /// - Define nominal input link config data.
    tInputConfigData = new GunnsElectConverterInputConfigData("tInputLink",
                                                              &tNodeList,
                                                              0,
                                                              0,
                                                              3,
                                                              0.0,
                                                              140.0);

    /// - Define nominal input link input data.
    tInputInputData = new GunnsElectConverterInputInputData(false, 0.0, true, 0.0);

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectConverterOutput::tearDown()
{
    /// - Deletes for news in setUp
    delete tInputInputData;
    delete tInputConfigData;
    delete tArticle;
    delete tInputData;
    delete tConfigData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of Electrical Converter Output Link configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectConverterOutput::testConfig()
{
    UT_RESULT_FIRST;

    /// @test    Configuration nominal construction.
    CPPUNIT_ASSERT(tRegulatorType        == tConfigData->mRegulatorType);
    CPPUNIT_ASSERT(tOutputConductance    == tConfigData->mOutputConductance);
    CPPUNIT_ASSERT(tConverterEfficiency  == tConfigData->mConverterEfficiency);
    CPPUNIT_ASSERT(&tSensorVout          == tConfigData->mOutputVoltageSensor);
    CPPUNIT_ASSERT(&tSensorIout          == tConfigData->mOutputCurrentSensor);
    CPPUNIT_ASSERT(tTripPriority         == tConfigData->mTripPriority);
    CPPUNIT_ASSERT(tOutOverVoltageTrip   == tConfigData->mOutputOverVoltageTripLimit);
    CPPUNIT_ASSERT(tOutOverCurrentTrip   == tConfigData->mOutputOverCurrentTripLimit);
    CPPUNIT_ASSERT(&tInputLink           == tConfigData->mInputLink);
    CPPUNIT_ASSERT(true                  == tConfigData->mEnableLimiting);
    CPPUNIT_ASSERT(tOutUnderVoltageTrip  == tConfigData->mOutputUnderVoltageTripLimit);
    CPPUNIT_ASSERT(4                     == tConfigData->mStateFlipsLimit);

    /// @test    Configuration data default construction.
    GunnsElectConverterOutputConfigData defaultConfig;
    CPPUNIT_ASSERT(GunnsElectConverterOutput::VOLTAGE == defaultConfig.mRegulatorType);
    CPPUNIT_ASSERT(0.0                                == defaultConfig.mOutputConductance);
    CPPUNIT_ASSERT(0.0                                == defaultConfig.mConverterEfficiency);
    CPPUNIT_ASSERT(0                                  == defaultConfig.mOutputVoltageSensor);
    CPPUNIT_ASSERT(0                                  == defaultConfig.mOutputCurrentSensor);
    CPPUNIT_ASSERT(0                                  == defaultConfig.mTripPriority);
    CPPUNIT_ASSERT(0.0                                == defaultConfig.mOutputOverVoltageTripLimit);
    CPPUNIT_ASSERT(0.0                                == defaultConfig.mOutputOverCurrentTripLimit);
    CPPUNIT_ASSERT(0                                  == defaultConfig.mInputLink);
    CPPUNIT_ASSERT(false                              == defaultConfig.mEnableLimiting);
    CPPUNIT_ASSERT(0.0                                == defaultConfig.mOutputUnderVoltageTripLimit);
    CPPUNIT_ASSERT(4                                  == defaultConfig.mStateFlipsLimit);

    /// @test    Configuration data copy construction.
    GunnsElectConverterOutputConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tRegulatorType        == copyConfig.mRegulatorType);
    CPPUNIT_ASSERT(tOutputConductance    == copyConfig.mOutputConductance);
    CPPUNIT_ASSERT(tConverterEfficiency  == copyConfig.mConverterEfficiency);
    CPPUNIT_ASSERT(&tSensorVout          == copyConfig.mOutputVoltageSensor);
    CPPUNIT_ASSERT(&tSensorIout          == copyConfig.mOutputCurrentSensor);
    CPPUNIT_ASSERT(tTripPriority         == copyConfig.mTripPriority);
    CPPUNIT_ASSERT(tOutOverVoltageTrip   == copyConfig.mOutputOverVoltageTripLimit);
    CPPUNIT_ASSERT(tOutOverCurrentTrip   == copyConfig.mOutputOverCurrentTripLimit);
    CPPUNIT_ASSERT(&tInputLink           == copyConfig.mInputLink);
    CPPUNIT_ASSERT(tEnableLimit          == copyConfig.mEnableLimiting);
    CPPUNIT_ASSERT(tOutUnderVoltageTrip  == copyConfig.mOutputUnderVoltageTripLimit);
    CPPUNIT_ASSERT(4                     == copyConfig.mStateFlipsLimit);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of Electrical Converter Output Link input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectConverterOutput::testInput()
{
    UT_RESULT;

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(tMalfBlockageFlag  == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tEnabled           == tInputData->mEnabled);
    CPPUNIT_ASSERT(tInputVoltage      == tInputData->mInputVoltage);
    CPPUNIT_ASSERT(tInputPower        == tInputData->mInputPower);
    CPPUNIT_ASSERT(tSetpoint          == tInputData->mSetpoint);

    /// @test    Input data default construction.
    GunnsElectConverterOutputInputData defaultInput;
    CPPUNIT_ASSERT(false == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0   == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(false == defaultInput.mEnabled);
    CPPUNIT_ASSERT(0.0   == defaultInput.mInputVoltage);
    CPPUNIT_ASSERT(0.0   == defaultInput.mInputPower);
    CPPUNIT_ASSERT(0.0   == defaultInput.mSetpoint);

    /// @test    Input data copy construction.
    GunnsElectConverterOutputInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tMalfBlockageFlag  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(tEnabled           == copyInput.mEnabled);
    CPPUNIT_ASSERT(tInputVoltage      == copyInput.mInputVoltage);
    CPPUNIT_ASSERT(tInputPower        == copyInput.mInputPower);
    CPPUNIT_ASSERT(tSetpoint          == copyInput.mSetpoint);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the constructor of the GunnsElectConverterInput class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectConverterOutput::testConstruction()
{
    UT_RESULT;

    /// @test    Default construction.
    CPPUNIT_ASSERT(GunnsElectConverterOutput::VOLTAGE == tArticle->mRegulatorType);
    CPPUNIT_ASSERT(0.0                                == tArticle->mOutputConductance);
    CPPUNIT_ASSERT(0.0                                == tArticle->mConverterEfficiency);
    CPPUNIT_ASSERT(0                                  == tArticle->mOutputVoltageSensor);
    CPPUNIT_ASSERT(0                                  == tArticle->mOutputCurrentSensor);
    CPPUNIT_ASSERT(0                                  == tArticle->mInputLink);
    CPPUNIT_ASSERT(false                              == tArticle->mEnableLimiting);
    CPPUNIT_ASSERT(0                                  == tArticle->mStateFlipsLimit);
    CPPUNIT_ASSERT(false                              == tArticle->mEnabled);
    CPPUNIT_ASSERT(0.0                                == tArticle->mInputVoltage);
    CPPUNIT_ASSERT(false                              == tArticle->mInputVoltageValid);
    CPPUNIT_ASSERT(false                              == tArticle->mOutputPowerAvailable);
    CPPUNIT_ASSERT(0.0                                == tArticle->mSetpoint);
    CPPUNIT_ASSERT(false                              == tArticle->mResetTrips);
    CPPUNIT_ASSERT(0.0                                == tArticle->mInputPower);
    CPPUNIT_ASSERT(false                              == tArticle->mInputPowerValid);
    CPPUNIT_ASSERT(0.0                                == tArticle->mOutputChannelLoss);
    CPPUNIT_ASSERT(0.0                                == tArticle->mLoadResistance);
    CPPUNIT_ASSERT(0.0                                == tArticle->mTotalPowerLoss);
    CPPUNIT_ASSERT(false                              == tArticle->mOutputOverVoltageTrip.isTripped());
    CPPUNIT_ASSERT(false                              == tArticle->mOutputUnderVoltageTrip.isTripped());
    CPPUNIT_ASSERT(false                              == tArticle->mOutputOverCurrentTrip.isTripped());
    CPPUNIT_ASSERT(false                              == tArticle->mLeadsInterface);
    CPPUNIT_ASSERT(false                              == tArticle->mReverseBiasState);
    CPPUNIT_ASSERT(0                                  == tArticle->mReverseBiasFlips);
    CPPUNIT_ASSERT(false                              == tArticle->mLimitState);
    CPPUNIT_ASSERT(0                                  == tArticle->mLimitStateFlips);
    CPPUNIT_ASSERT(0.0                                == tArticle->mSourceVoltage);
    CPPUNIT_ASSERT(""                                 == tArticle->mName);

    /// @test    New/delete for code coverage.
    GunnsElectConverterOutput* testArticle = new GunnsElectConverterOutput();
    delete testArticle;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Electrical Converter Output Link nominal initialization without
///           exceptions, supplying a custom strings load order list.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectConverterOutput::testNominalInitialization()
{
    UT_RESULT;

    /// @test    Initialize the input link.
    CPPUNIT_ASSERT_NO_THROW(tInputLink.initialize(*tInputConfigData, *tInputInputData, tLinks, 1));

    /// - Initialize default constructed test article with nominal initialization data.
    tConfigData->mInputLink = 0;
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));

    /// @test    Nominal config data.
    CPPUNIT_ASSERT(tRegulatorType       == tArticle->mRegulatorType);
    CPPUNIT_ASSERT(tOutputConductance   == tArticle->mOutputConductance);
    CPPUNIT_ASSERT(tConverterEfficiency == tArticle->mConverterEfficiency);
    CPPUNIT_ASSERT(0                    == tArticle->mInputLink);
    CPPUNIT_ASSERT(tEnableLimit         == tArticle->mEnableLimiting);
    CPPUNIT_ASSERT(4                    == tArticle->mStateFlipsLimit);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT(tMalfBlockageFlag  == tArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue == tArticle->mMalfBlockageValue);
    CPPUNIT_ASSERT(tEnabled           == tArticle->mEnabled);
    CPPUNIT_ASSERT(tInputVoltage      == tArticle->mInputVoltage);
    CPPUNIT_ASSERT(tInputPower        == tArticle->mInputPower);
    CPPUNIT_ASSERT(tSetpoint          == tArticle->mSetpoint);

    /// @test    Sensors.
    CPPUNIT_ASSERT(&tSensorVout.mSensor == tArticle->mOutputVoltageSensor);
    CPPUNIT_ASSERT(&tSensorIout.mSensor == tArticle->mOutputCurrentSensor);

    /// @test    Trips package.
    GunnsBasicLink::SolutionResult result;
    CPPUNIT_ASSERT(false == tArticle->mOutputOverVoltageTrip .isTripped());
    CPPUNIT_ASSERT(false == tArticle->mOutputUnderVoltageTrip.isTripped());
    CPPUNIT_ASSERT(false == tArticle->mOutputOverCurrentTrip .isTripped());
    CPPUNIT_ASSERT(true  == tArticle->mOutputOverVoltageTrip .checkForTrip(result, tOutOverVoltageTrip  + 0.01, tTripPriority));
    CPPUNIT_ASSERT(true  == tArticle->mOutputUnderVoltageTrip.checkForTrip(result, tOutUnderVoltageTrip - 0.01, tTripPriority));
    CPPUNIT_ASSERT(true  == tArticle->mOutputOverCurrentTrip .checkForTrip(result, tOutOverCurrentTrip  + 0.01, tTripPriority));

    /// @test    Nominal state data.
    CPPUNIT_ASSERT(true  == tArticle->mOutputPowerAvailable);
    CPPUNIT_ASSERT(false == tArticle->mResetTrips);
    CPPUNIT_ASSERT(0.0   == tArticle->mOutputChannelLoss);
    CPPUNIT_ASSERT(0.0   == tArticle->mTotalPowerLoss);
    CPPUNIT_ASSERT(false == tArticle->mReverseBiasState);
    CPPUNIT_ASSERT(tName == tArticle->mName);
    CPPUNIT_ASSERT(true  == tArticle->mInitFlag);

    /// @test    Re-init with nominal input link provided, no sensors, and reverse bias.
    tConfigData->mInputLink           = &tInputLink;
    tConfigData->mOutputVoltageSensor = 0;
    tConfigData->mOutputCurrentSensor = 0;
    tConfigData->mRegulatorType       = GunnsElectConverterOutput::VOLTAGE;
    tInputData->mSetpoint             = 100.0;
    tArticle->mOutputVoltageSensor    = 0;
    tArticle->mOutputCurrentSensor    = 0;
    tNodes[0].setPotential(120.0);
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));
    CPPUNIT_ASSERT(&tInputLink == tArticle->mInputLink);
    CPPUNIT_ASSERT(0           == tArticle->mOutputVoltageSensor);
    CPPUNIT_ASSERT(0           == tArticle->mOutputCurrentSensor);
    CPPUNIT_ASSERT(tArticle    == tInputLink.mOutputLink);
    CPPUNIT_ASSERT(false       == tArticle->mLeadsInterface);
    CPPUNIT_ASSERT(true        == tArticle->mReverseBiasState);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Electrical Converter Output Link nominal initialization with exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectConverterOutput::testInitializationErrors()
{
    UT_RESULT;

    /// @test    Exception thrown for output conductance < DBL_EPSILON.
    tConfigData->mOutputConductance = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0), TsInitializationException);
    tConfigData->mOutputConductance = tOutputConductance;

    /// @test    Exception thrown for output efficiency < DBL_EPSILON.
    tConfigData->mConverterEfficiency = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0), TsInitializationException);

    /// @test    Exception thrown for output efficiency > 1.
    tConfigData->mConverterEfficiency = 1.00001;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0), TsInitializationException);
    tConfigData->mConverterEfficiency = tConverterEfficiency;

    /// @test    Exception thrown for input voltage < 0.
    tInputData->mInputVoltage = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0), TsInitializationException);
    tInputData->mInputVoltage = tInputVoltage;

    /// @test    Exception thrown for setpoint < 0.
    tInputData->mSetpoint = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0), TsInitializationException);
    tInputData->mSetpoint = tSetpoint;

    /// @test    Exception thrown for node list mismatch with input link.
    GunnsBasicNode otherNodes[N_NODES];
    GunnsNodeList  otherNodeList;
    otherNodeList.mNodes         = otherNodes;
    otherNodeList.mNumNodes      = N_NODES;
    tInputConfigData->mNodeList = &otherNodeList;
    CPPUNIT_ASSERT_NO_THROW(tInputLink.initialize(*tInputConfigData, *tInputInputData, tLinks, 1));
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0), TsInitializationException);

    CPPUNIT_ASSERT(false == tArticle->mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Electrical Converter Output Link restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectConverterOutput::testRestart()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));

    /// @test    Restart method clears non-config and non-checkpointed data.
    tArticle->mInputVoltageValid = false;
    tArticle->mInputPowerValid   = false;
    tArticle->mResetTrips        = true;
    tArticle->mOutputChannelLoss = 0.0;
    tArticle->mReverseBiasState  = true;
    tArticle->mReverseBiasFlips  = 1;
    tArticle->mLimitStateFlips   = 1;
    tArticle->restart();
    CPPUNIT_ASSERT(true  == tArticle->mInputVoltageValid);
    CPPUNIT_ASSERT(true  == tArticle->mInputPowerValid);
    CPPUNIT_ASSERT(false == tArticle->mResetTrips);
    CPPUNIT_ASSERT(0.0   == tArticle->mOutputChannelLoss);
    CPPUNIT_ASSERT(false == tArticle->mReverseBiasState);
    CPPUNIT_ASSERT(0     == tArticle->mReverseBiasFlips);
    CPPUNIT_ASSERT(0     == tArticle->mLimitStateFlips);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Electrical Converter Output Link step method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectConverterOutput::testStep()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    tConfigData->mInputLink = 0;
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));

    {
        /// @test    Step and minorStep (TRANSFORMER mode) with normal inputs.
        /// @test    Bias flip and current limit flip counters reset.
        tArticle->mReverseBiasFlips = 1;
        tArticle->mLimitStateFlips  = 1;
        double expectedG = (1.0 - tMalfBlockageValue) * tOutputConductance;
        double expectedW = expectedG * tInputVoltage * tSetpoint;
        tArticle->step(0.0);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());
        CPPUNIT_ASSERT(0    == tArticle->mReverseBiasFlips);
        CPPUNIT_ASSERT(0    == tArticle->mLimitStateFlips);

        /// @test    Reset trips when commanded.
        GunnsBasicLink::SolutionResult result;
        CPPUNIT_ASSERT(true == tArticle->mOutputOverVoltageTrip .checkForTrip(result, tOutOverVoltageTrip  + 0.01, tTripPriority));
        CPPUNIT_ASSERT(true == tArticle->mOutputUnderVoltageTrip.checkForTrip(result, tOutUnderVoltageTrip - 0.01, tTripPriority));
        CPPUNIT_ASSERT(true == tArticle->mOutputOverCurrentTrip .checkForTrip(result, tOutOverCurrentTrip  + 0.01, tTripPriority));
        CPPUNIT_ASSERT(true == tArticle->mOutputOverVoltageTrip .isTripped());
        CPPUNIT_ASSERT(true == tArticle->mOutputUnderVoltageTrip.isTripped());
        CPPUNIT_ASSERT(true == tArticle->mOutputOverCurrentTrip .isTripped());
        tArticle->mResetTrips = true;
        tArticle->step(0.0);

        CPPUNIT_ASSERT(false == tArticle->mOutputOverVoltageTrip .isTripped());
        CPPUNIT_ASSERT(false == tArticle->mOutputUnderVoltageTrip.isTripped());
        CPPUNIT_ASSERT(false == tArticle->mOutputOverCurrentTrip .isTripped());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(false == tArticle->needAdmittanceUpdate());
        CPPUNIT_ASSERT(false == tArticle->mResetTrips);

        /// @test    Trips not reset when not commanded.
        CPPUNIT_ASSERT(true == tArticle->mOutputOverVoltageTrip .checkForTrip(result, tOutOverVoltageTrip  + 0.01, tTripPriority));
        CPPUNIT_ASSERT(true == tArticle->mOutputUnderVoltageTrip.checkForTrip(result, tOutUnderVoltageTrip - 0.01, tTripPriority));
        CPPUNIT_ASSERT(true == tArticle->mOutputOverCurrentTrip .checkForTrip(result, tOutOverCurrentTrip  + 0.01, tTripPriority));
        CPPUNIT_ASSERT(true == tArticle->mOutputOverVoltageTrip .isTripped());
        CPPUNIT_ASSERT(true == tArticle->mOutputUnderVoltageTrip.isTripped());
        CPPUNIT_ASSERT(true == tArticle->mOutputOverCurrentTrip .isTripped());
        expectedG = 0.0;
        expectedW = 0.0;
        tArticle->step(0.0);

        CPPUNIT_ASSERT(true == tArticle->mOutputOverVoltageTrip .isTripped());
        CPPUNIT_ASSERT(true == tArticle->mOutputUnderVoltageTrip.isTripped());
        CPPUNIT_ASSERT(true == tArticle->mOutputOverCurrentTrip .isTripped());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(true  == tArticle->needAdmittanceUpdate());
        CPPUNIT_ASSERT(false == tArticle->mResetTrips);
    } {
        /// @test    User port commands.
        tArticle->mUserPortSelect     = 0;
        tArticle->mUserPortSelectNode = 1;
        tArticle->mUserPortSetControl = GunnsBasicLink::EXECUTE;
        tArticle->step(0.0);
        CPPUNIT_ASSERT(GunnsBasicLink::READY == tArticle->mUserPortSetControl);
        CPPUNIT_ASSERT(1                     == tArticle->mNodeMap[0]);
    } {
        /// @test    Resets trips in the input link.
        tInputLink.initialize(*tInputConfigData, *tInputInputData, tLinks, 1);
        tArticle->mInputLink = &tInputLink;

        tInputLink.mInputOverVoltageTrip.mMalfForceTrip = true;
        GunnsBasicLink::SolutionResult result;
        CPPUNIT_ASSERT(true == tInputLink.mInputOverVoltageTrip.checkForTrip(result, 1.0, 3));
        CPPUNIT_ASSERT(true == tInputLink.mInputOverVoltageTrip.isTripped());
        tInputLink.mInputOverVoltageTrip.mMalfForceTrip = false;
        tArticle->mResetTrips = true;

        tArticle->step(0.0);
        CPPUNIT_ASSERT(false == tInputLink.mInputOverVoltageTrip.isTripped());
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the Electrical Converter Output Link minorStep method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectConverterOutput::testMinorStep()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data and a
    ///   registered output link.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);
    tInputLink.initialize(*tInputConfigData, *tInputInputData, tLinks, 1);
    CPPUNIT_ASSERT(true == tArticle->mLeadsInterface);

    {
        /// @test    Step and minorStep (TRANSFORMER mode) with input voltage from the input link.
        double nodeV     = 120.0;
        double expectedI = 0.0;
        double expectedR = 1.0;
        double expectedG = tOutputConductance * (1.0 - tMalfBlockageValue);
        double expectedW = expectedG * nodeV * tSetpoint;
        tInputLink.mPotentialVector[0] = nodeV;
        tArticle->step(0.0);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, tArticle->mFlux,                DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedR, tArticle->mLoadResistance,      DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());
        CPPUNIT_ASSERT(true == tArticle->mOutputPowerAvailable);

        /// @test    Step and minorStep (VOLTAGE mode) with input voltage from the input link, and
        ///          repeated admittance.
        tArticle->mPotentialVector[0] = nodeV - 1.0;
        double setpoint = 100.0;
        expectedG       = tOutputConductance * (1.0 - tMalfBlockageValue);
        expectedI       = expectedW - tArticle->mPotentialVector[0] * expectedG;
        expectedR       = tArticle->mPotentialVector[0] / expectedI;
        expectedW       = expectedG * setpoint;
        tArticle->mRegulatorType = GunnsElectConverterOutput::VOLTAGE;
        tArticle->mSetpoint      = setpoint;
        tArticle->step(0.0);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, tArticle->mFlux,                DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedR, tArticle->mLoadResistance,      DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(false == tArticle->needAdmittanceUpdate());

        /// @test    Step and minorStep (CURRENT mode) with input voltage from the input link.
        expectedI = fmax(0.0, expectedW - tArticle->mPotentialVector[0] * expectedG);
        expectedR = 1.0;
        expectedG = FLT_EPSILON;
        setpoint  = 2.0;
        expectedW = setpoint * (1.0 - tMalfBlockageValue);
        tArticle->mRegulatorType = GunnsElectConverterOutput::CURRENT;
        tArticle->mSetpoint      = setpoint;
        tArticle->step(0.0);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, tArticle->mFlux,                DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedR, tArticle->mLoadResistance,      DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());

        /// @test    Step and minorStep (POWER mode) with input voltage from the input link.
        setpoint = 20.0;
        tArticle->mRegulatorType      = GunnsElectConverterOutput::POWER;
        tArticle->mSetpoint           = setpoint;
        tArticle->mPotentialVector[0] = nodeV;
        expectedI = expectedW - tArticle->mPotentialVector[0] * expectedG;
        expectedR = tArticle->mPotentialVector[0] / expectedI;
        expectedW = (1.0 - tMalfBlockageValue) * sqrt(setpoint / expectedR);
        tArticle->step(0.0);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, tArticle->mFlux,                DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedR, tArticle->mLoadResistance,      DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(false == tArticle->needAdmittanceUpdate());

        /// @test    minorStep (VOLTAGE mode) in the reverse bias state.
        setpoint  = 100.0;
        expectedG = 0.0;
        expectedW = 0.0;
        tArticle->mRegulatorType    = GunnsElectConverterOutput::VOLTAGE;
        tArticle->mSetpoint         = setpoint;
        tArticle->mReverseBiasState = true;
        tArticle->minorStep(0.0, 0);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());

        /// @test    minorStep when disabled.
        tArticle->mEnabled = false;
        tArticle->minorStep(0.0, 0);
        CPPUNIT_ASSERT(0.0 == tArticle->mAdmittanceMatrix[0]);

        /// @test    minorStep when input voltage is zero.
        tInputLink.mPotentialVector[0] = 0.0;
        tArticle->mEnabled             = true;
        tArticle->minorStep(0.0, 0);
        CPPUNIT_ASSERT(0.0  == tArticle->mAdmittanceMatrix[0]);
        CPPUNIT_ASSERT(true == tArticle->mOutputPowerAvailable);

    } {
        /// @test    step and minorStep when connected to the Ground node.
        tArticle->mUserPortSelect      = 0;
        tArticle->mUserPortSetControl  = GunnsBasicLink::GROUND;
        tArticle->mInputVoltage        = 120.0;
        tArticle->mAdmittanceMatrix[0] = 1.0;
        tArticle->mSourceVector[0]     = 1.0;
        tArticle->step(0.0);

        CPPUNIT_ASSERT(0.0 == tArticle->mInputVoltage);
        CPPUNIT_ASSERT(0.0 == tArticle->mAdmittanceMatrix[0]);
        CPPUNIT_ASSERT(0.0 == tArticle->mSourceVector[0]);
    } {
        FriendlyGunnsElectConverterOutput article2;
        article2.initialize(*tConfigData, *tInputData, tLinks, tPort0);
        tInputLink.initialize(*tInputConfigData, *tInputInputData, tLinks, 1);

        /// @test    minorStep (TRANSFORMER mode) in current limiting state.
        double nodeV     = 120.0;
        tInputLink.mPotentialVector[0] = nodeV;
        double expectedI = tOutOverCurrentTrip;
        double expectedR = 1.0;
        double expectedG = FLT_EPSILON;
        double expectedW = expectedI;
        article2.setLimitingState(GunnsElectConverterOutput::LIMIT_OC);
        article2.step(0.0);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedR, article2.mLoadResistance,      DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, article2.mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, article2.mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(true == article2.needAdmittanceUpdate());
        CPPUNIT_ASSERT(true == article2.mOutputPowerAvailable);

        /// @test    minorStep (VOLTAGE mode) in current limiting state.
        double setpoint         = 100.0;
        article2.mSetpoint      = setpoint;
        article2.mRegulatorType = GunnsElectConverterOutput::VOLTAGE;
        article2.step(0.0);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedR, article2.mLoadResistance,      DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, article2.mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, article2.mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(false == article2.needAdmittanceUpdate());
        CPPUNIT_ASSERT(true  == article2.mOutputPowerAvailable);

        /// @test    minorStep (CURRENT mode) in overvoltage limiting state.
        article2.mRegulatorType = GunnsElectConverterOutput::CURRENT;
        article2.setLimitingState(GunnsElectConverterOutput::LIMIT_OV);
        expectedG               = tOutputConductance * (1.0 - tMalfBlockageValue);
        expectedW               = expectedG * tOutOverVoltageTrip;
        article2.step(0.0);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, article2.mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, article2.mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(true == article2.needAdmittanceUpdate());
        CPPUNIT_ASSERT(true == article2.mOutputPowerAvailable);

        /// @test    minorStep (POWER mode) in undervoltage limiting state.
        article2.mRegulatorType = GunnsElectConverterOutput::POWER;
        article2.setLimitingState(GunnsElectConverterOutput::LIMIT_UV);
        expectedW               = expectedG * tOutUnderVoltageTrip;
        article2.step(0.0);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, article2.mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, article2.mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(false == article2.needAdmittanceUpdate());
        CPPUNIT_ASSERT(true  == article2.mOutputPowerAvailable);

        /// @test    minorStep (CURRENT mode) in overcurrent limiting state.
        article2.mRegulatorType = GunnsElectConverterOutput::CURRENT;
        article2.setLimitingState(GunnsElectConverterOutput::LIMIT_OC);
        expectedG = FLT_EPSILON;
        expectedW = tOutOverCurrentTrip * (1.0 - tMalfBlockageValue);
        article2.step(0.0);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, article2.mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, article2.mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(true == article2.needAdmittanceUpdate());
        CPPUNIT_ASSERT(true == article2.mOutputPowerAvailable);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the Electrical Converter Output Link computeInputPower method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectConverterOutput::testComputeInputPower()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);

    /// @test    Normal input power.
    double setpoint     = 100.0;
    double nodeV        =  99.0;
    double conductance  = tOutputConductance;
    double source       = conductance * setpoint;
    double expectedFlux      = source - nodeV * conductance;
    double expectedPower     = expectedFlux * nodeV;
    double expectedOutLoss   = expectedFlux * expectedFlux / tOutputConductance;
    double expectedPowerIn   = (expectedPower + expectedOutLoss) / tConverterEfficiency;
    double expectedTotalLoss = expectedPowerIn - expectedPower;
    double actualPower       = 0.0;
    tArticle->mSourceVector[0]     = source;
    tArticle->mAdmittanceMatrix[0] = conductance;
    tArticle->mPotentialVector[0]  = nodeV;

    CPPUNIT_ASSERT(true  == tArticle->computeInputPower(actualPower));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPowerIn,   actualPower,                   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tInputPower,       tArticle->mInputPower,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,      tArticle->mFlux,               DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower,     tArticle->mPower,              DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedOutLoss,   tArticle->mOutputChannelLoss,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTotalLoss, tArticle->mTotalPowerLoss,     DBL_EPSILON);

    /// @test    Power valid flag not set.
    tArticle->mInputPowerValid = false;
    CPPUNIT_ASSERT(false == tArticle->computeInputPower(actualPower));
    CPPUNIT_ASSERT(0.0   == actualPower);
    CPPUNIT_ASSERT(tArticle->mInputPower == tArticle->mInputPower);
    CPPUNIT_ASSERT(0.0   == tArticle->mPower);
    CPPUNIT_ASSERT(0.0   == tArticle->mOutputChannelLoss);
    CPPUNIT_ASSERT(0.0   == tArticle->mTotalPowerLoss);

    /// @test    On Ground node.
    tArticle->mInputPowerValid    = true;
    tArticle->mUserPortSelect     = 0;
    tArticle->mUserPortSetControl = GunnsBasicLink::GROUND;
    tArticle->step(0.0);
    CPPUNIT_ASSERT(true == tArticle->computeInputPower(actualPower));
    CPPUNIT_ASSERT(0.0  == actualPower);
    CPPUNIT_ASSERT(0.0  == tArticle->mInputPower);
    CPPUNIT_ASSERT(0.0  == tArticle->mPower);
    CPPUNIT_ASSERT(0.0  == tArticle->mOutputChannelLoss);
    CPPUNIT_ASSERT(0.0  == tArticle->mTotalPowerLoss);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the Electrical Converter Output Link getter and setter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectConverterOutput::testAccessors()
{
    UT_RESULT;

    /// @test    Link is non-linear.
    CPPUNIT_ASSERT(true == tArticle->isNonLinear());

    /// @test    Can set & get the enabled flag.
    tArticle->setEnabled(true);
    CPPUNIT_ASSERT(true == tArticle->mEnabled);
    CPPUNIT_ASSERT(true == tArticle->getEnabled());

    /// @test    Can set the input voltage.
    tArticle->setInputVoltage(120.0);
    CPPUNIT_ASSERT(120.0 == tArticle->mInputVoltage);

    /// @test    Can set and get the setpoint.
    tArticle->setSetpoint(2.0);
    CPPUNIT_ASSERT(2.0 == tArticle->getSetpoint());

    /// @test    Get the trip logics.
    CPPUNIT_ASSERT(&tArticle->mOutputOverVoltageTrip  == tArticle->getOutputOverVoltageTrip());
    CPPUNIT_ASSERT(&tArticle->mOutputUnderVoltageTrip == tArticle->getOutputUnderVoltageTrip());
    CPPUNIT_ASSERT(&tArticle->mOutputOverCurrentTrip  == tArticle->getOutputOverCurrentTrip());

    /// @test    Get the input power.
    tArticle->mInputPower = 42.0;
    CPPUNIT_ASSERT(42.0 == tArticle->getInputPower());

    /// @test    Get the input power valid flag.
    tArticle->mInputPowerValid = true;
    CPPUNIT_ASSERT(true  == tArticle->getInputPowerValid());
    tArticle->mInputPowerValid = false;
    CPPUNIT_ASSERT(false == tArticle->getInputPowerValid());

    /// @test    Set & get the current limiting state.
    tArticle->setLimitingState(GunnsElectConverterOutput::LIMIT_OC);
    CPPUNIT_ASSERT(true  == tArticle->getLimitingState());
    tArticle->setLimitingState(GunnsElectConverterOutput::NO_LIMIT);
    CPPUNIT_ASSERT(false == tArticle->getLimitingState());

    /// @test    Set output conductance.
    tArticle->setOutputConductance(1.0);
    CPPUNIT_ASSERT(1.0 == tArticle->mOutputConductance);

    /// @test    resetLastMinorStep.
    CPPUNIT_ASSERT(true == tArticle->resetLastMinorStep(0, 0));
    CPPUNIT_ASSERT(false == tArticle->mInputPowerValid);

    /// @test    function calls for line coverage.
    FriendlyGunnsElectConverterOutput article2;
    CPPUNIT_ASSERT(true == article2.isVoltageRegulator());

    article2.setMalfBlockage(true, 0.5);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, article2.applyBlockage(1.0), DBL_EPSILON);
    article2.setMalfBlockage();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, article2.applyBlockage(1.0), DBL_EPSILON);

    GunnsBasicLink::SolutionResult result = GunnsBasicLink::CONFIRM;
    article2.rejectWithLimitState(result, GunnsElectConverterOutput::LIMIT_OC);
    CPPUNIT_ASSERT(1 == article2.mLimitStateFlips);

    CPPUNIT_ASSERT(0.0 == article2.computeVoltageControlSetpoint());

    article2.mRegulatorType  = GunnsElectConverterOutput::POWER;
    article2.mLoadResistance = 1.0;
    article2.setSetpoint(1.0);
    CPPUNIT_ASSERT(1.0 == article2.computeCurrentControlSetpoint());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the confirmSolutionAcceptable method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectConverterOutput::testConfirmSolutionAcceptable()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    tConfigData->mInputLink      = 0;
    tConfigData->mEnableLimiting = false;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);

    /// @test    Confirms on Ground node.
    tArticle->mUserPortSelect     = 0;
    tArticle->mUserPortSetControl = GunnsBasicLink::GROUND;
    tArticle->step(0.0);
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == tArticle->confirmSolutionAcceptable(0, 1));
    tArticle->mInputVoltage       = tInputVoltage;
    tArticle->mUserPortSelect     = 0;
    tArticle->mUserPortSetControl = GunnsBasicLink::DEFAULT;
    tArticle->step(0.0);

    /// @test    Does not flip bias to reverse until network is converged.
    tArticle->mPotentialVector[0] = tInputVoltage + 1.0;
    tArticle->mInputPower         = 1.0;
    CPPUNIT_ASSERT(false == tArticle->mReverseBiasState);
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == tArticle->confirmSolutionAcceptable(0, 1));
    CPPUNIT_ASSERT(false == tArticle->mReverseBiasState);
    CPPUNIT_ASSERT(0     == tArticle->mReverseBiasFlips);
    CPPUNIT_ASSERT(true  == tArticle->mInputPowerValid);

    /// @test    Zeroes input power and rejects when bias changes from forward to reverse on
    ///          converged minor step.
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT == tArticle->confirmSolutionAcceptable(1, 2));
    CPPUNIT_ASSERT(0.0   == tArticle->mInputPower);
    CPPUNIT_ASSERT(true  == tArticle->mReverseBiasState);
    CPPUNIT_ASSERT(1     == tArticle->mReverseBiasFlips);
    CPPUNIT_ASSERT(false == tArticle->mInputPowerValid);

    /// @test    Zeroes input power and rejects when bias changes from reverse to forward.
    tArticle->mPotentialVector[0] = tInputVoltage;
    tArticle->mInputPower         = 1.0;
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT == tArticle->confirmSolutionAcceptable(0, 1));
    CPPUNIT_ASSERT(0.0   == tArticle->mInputPower);
    CPPUNIT_ASSERT(false == tArticle->mReverseBiasState);
    CPPUNIT_ASSERT(false == tArticle->mInputPowerValid);

    /// @test    Confirms when network isn't converged.
    tArticle->mInputPowerValid = true;
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == tArticle->confirmSolutionAcceptable(0, 1));
    CPPUNIT_ASSERT(true == tArticle->mInputPowerValid);

    /// @test    Confirms when sensors output values that don't cause trips.
    tSensorVout.mSensor.mMalfFailToFlag  = true;
    tSensorVout.mSensor.mMalfFailToValue = 120.0;
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == tArticle->confirmSolutionAcceptable(tTripPriority-1, 1));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(120.0, tSensorVout.mSensor.getSensedOutput(), DBL_EPSILON);
    CPPUNIT_ASSERT(true == tArticle->mInputPowerValid);

    /// @test    Confirms when link is disabled.
    tSensorVout.mSensor.mMalfFailToFlag  = false;
    tArticle->mEnabled = false;
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == tArticle->confirmSolutionAcceptable(tTripPriority, 1));
    CPPUNIT_ASSERT(true == tArticle->mInputPowerValid);
    tArticle->mEnabled = true;

    /// - Can't flip to reverse bias a fifth time during major step.
    tArticle->mPotentialVector[0] = 200.0;
    tArticle->mReverseBiasFlips   = 4;
    CPPUNIT_ASSERT(4     == tArticle->mReverseBiasFlips);
    CPPUNIT_ASSERT(false == tArticle->mReverseBiasState);

    /// - Flip to reverse bias and set up output overvolt trip condition.
    tArticle->mReverseBiasFlips = 0;
    tArticle->mReverseBiasState = false;
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT == tArticle->confirmSolutionAcceptable(1, 2));
    CPPUNIT_ASSERT(1     == tArticle->mReverseBiasFlips);
    CPPUNIT_ASSERT(false == tArticle->mInputPowerValid);

    /// @test    Delays in trip-able condition but trip priority not yet met.
    CPPUNIT_ASSERT(GunnsBasicLink::DELAY  == tArticle->confirmSolutionAcceptable(tTripPriority-1, 1));
    CPPUNIT_ASSERT(!tArticle->mOutputOverVoltageTrip.isTripped());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(200.0, tSensorVout.mSensor.getSensedOutput(), DBL_EPSILON);
    CPPUNIT_ASSERT(true == tArticle->mInputPowerValid);

    /// @test    Rejects due to overvolt trip from sensor.
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT == tArticle->confirmSolutionAcceptable(tTripPriority, 1));
    CPPUNIT_ASSERT(tArticle->mOutputOverVoltageTrip.isTripped());
    CPPUNIT_ASSERT(false == tArticle->mInputPowerValid);
    tArticle->mOutputOverVoltageTrip.resetTrip();

    /// @test    Rejects due to overvolt trip with no sensor.
    tArticle->mOutputVoltageSensor = 0;
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT == tArticle->confirmSolutionAcceptable(tTripPriority, 1));
    CPPUNIT_ASSERT(tArticle->mOutputOverVoltageTrip.isTripped());
    CPPUNIT_ASSERT(false == tArticle->mInputPowerValid);
    tArticle->mOutputOverVoltageTrip.resetTrip();

    /// - Flip to forward bias and set up overcurrent trip condition.
    tArticle->mPotentialVector[0]  = 120.0;
    tArticle->mSourceVector[0]     = 200.0;
    tArticle->mAdmittanceMatrix[0] = 0.0;
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT == tArticle->confirmSolutionAcceptable(0, 1));
    CPPUNIT_ASSERT(false == tArticle->mInputPowerValid);

    /// @test    Rejects due to overcurrent trip from sensor.
    tArticle->mInputPowerValid = true;
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT == tArticle->confirmSolutionAcceptable(tTripPriority, 1));
    CPPUNIT_ASSERT(tArticle->mOutputOverCurrentTrip.isTripped());
    CPPUNIT_ASSERT(false == tArticle->mInputPowerValid);
    tArticle->mOutputOverCurrentTrip.resetTrip();

    /// @test    Rejects due to overcurrent trip with no sensor.
    tArticle->mInputPowerValid     = true;
    tArticle->mOutputCurrentSensor = 0;
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT == tArticle->confirmSolutionAcceptable(tTripPriority, 1));
    CPPUNIT_ASSERT(tArticle->mOutputOverCurrentTrip.isTripped());
    CPPUNIT_ASSERT(false == tArticle->mInputPowerValid);
    tArticle->mOutputOverCurrentTrip.resetTrip();
    CPPUNIT_ASSERT(false == tArticle->mOutputOverCurrentTrip.isTripped());

    /// @test    Rejects due to undervolt trip from sensor.
    tArticle->mInputPowerValid     = true;
    tArticle->mOutputVoltageSensor = &tSensorVout.mSensor;
    tArticle->mSourceVector[0]     = 0.0;
    tArticle->mPotentialVector[0]  = 80.0;
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT == tArticle->confirmSolutionAcceptable(tTripPriority, 1));
    CPPUNIT_ASSERT(tArticle->mOutputUnderVoltageTrip.isTripped());
    CPPUNIT_ASSERT(false == tArticle->mInputPowerValid);
    tArticle->mOutputUnderVoltageTrip.resetTrip();
    CPPUNIT_ASSERT(false == tArticle->mOutputOverCurrentTrip.isTripped());

    /// @test    Rejects due to undervolt trip with no sensor.
    tArticle->mInputPowerValid = true;
    tArticle->mOutputVoltageSensor = 0;
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT == tArticle->confirmSolutionAcceptable(tTripPriority, 1));
    CPPUNIT_ASSERT(tArticle->mOutputUnderVoltageTrip.isTripped());
    CPPUNIT_ASSERT(false == tArticle->mInputPowerValid);
    tArticle->mOutputUnderVoltageTrip.resetTrip();
    CPPUNIT_ASSERT(false == tArticle->mOutputOverCurrentTrip.isTripped());

    /// @test    Rejects due to invalid voltage from the input link.
    tArticle->mPotentialVector[0] = 120.0;
    tArticle->mInputPowerValid    = true;
    tArticle->mInputVoltageValid  = false;
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT == tArticle->confirmSolutionAcceptable(tTripPriority, 1));
    CPPUNIT_ASSERT(true == tArticle->mInputPowerValid);
    tArticle->mInputVoltageValid = true;
    CPPUNIT_ASSERT(false == tArticle->mOutputOverCurrentTrip.isTripped());

    /// @test    Rejects due to entering current limiting state.
    tArticle->mPotentialVector[0]  = 120.0;
    tArticle->mSourceVector[0]     = 200.0;
    tArticle->mAdmittanceMatrix[0] = 0.0;
    tArticle->mEnableLimiting      = true;
    tArticle->mInputPowerValid     = true;
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT == tArticle->confirmSolutionAcceptable(tTripPriority, 1));
    CPPUNIT_ASSERT(false == tArticle->mOutputOverCurrentTrip.isTripped());
    CPPUNIT_ASSERT(true  == tArticle->mLimitState);
    CPPUNIT_ASSERT(1     == tArticle->mLimitStateFlips);
    CPPUNIT_ASSERT(false == tArticle->mInputPowerValid);

    /// @test    Rejects due to leaving current limiting state.
    tArticle->mPotentialVector[0] = 200.0;
    tArticle->mLimitStateFlips    = 0;
    tArticle->mReverseBiasFlips   = 0;
    tArticle->mInputPowerValid    = true;
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT == tArticle->confirmSolutionAcceptable(1, 1));
    CPPUNIT_ASSERT(false == tArticle->mOutputOverVoltageTrip.isTripped());
    CPPUNIT_ASSERT(false == tArticle->mLimitState);
    CPPUNIT_ASSERT(0     == tArticle->mLimitStateFlips);
    CPPUNIT_ASSERT(false == tArticle->mInputPowerValid);
    CPPUNIT_ASSERT(true  == tArticle->mReverseBiasState);
    CPPUNIT_ASSERT(1     == tArticle->mReverseBiasFlips);

    /// @test    Does not enter current limiting state a 5th time.
    tArticle->mPotentialVector[0] = 120.0;
    tArticle->mLimitStateFlips    = 4;
    tArticle->mReverseBiasState   = false;
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == tArticle->confirmSolutionAcceptable(1, 1));
    CPPUNIT_ASSERT(false == tArticle->mLimitState);
    CPPUNIT_ASSERT(4     == tArticle->mLimitStateFlips);
    CPPUNIT_ASSERT(true  == tArticle->mInputPowerValid);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the confirmSolutionAcceptable method for current source regulator type.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectConverterOutput::testConfirmSolutionCurrentSource()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    tConfigData->mRegulatorType  = GunnsElectConverterOutput::CURRENT;
    tConfigData->mInputLink      = 0;
    tConfigData->mEnableLimiting = true;
    tInputData->mSetpoint        = 10.0;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);

    /// @test    Rejects due to entering under-voltage limiting state from no limit.
    tArticle->mAdmittanceMatrix[0] = FLT_EPSILON;
    tArticle->mSourceVector[0]     = tSetpoint;
    tArticle->mPotentialVector[0]  = 0.1;
    tArticle->mSourceVoltage       = 0.0;
    tArticle->mLimitStateFlips     = 0;
    double expectedFlux            = std::max(0.0, tSetpoint - 0.1 * FLT_EPSILON);
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT              == tArticle->confirmSolutionAcceptable(1, 1));
    CPPUNIT_ASSERT(false                               == tArticle->mOutputUnderVoltageTrip.isTripped());
    CPPUNIT_ASSERT(GunnsElectConverterOutput::LIMIT_UV == tArticle->mLimitState);
    CPPUNIT_ASSERT(1                                   == tArticle->mLimitStateFlips);
    CPPUNIT_ASSERT(false                               == tArticle->mInputPowerValid);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mFlux, DBL_EPSILON);

    /// @test    Rejects due to switching to reverse bias due to output voltage restored.
    tArticle->mInputPowerValid     = true;
    tArticle->mAdmittanceMatrix[0] = tOutputConductance;
    tArticle->mSourceVector[0]     = tOutputConductance * tOutUnderVoltageTrip;
    tArticle->mPotentialVector[0]  = 120.0;
    tArticle->mSourceVoltage       = tOutUnderVoltageTrip;
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT              == tArticle->confirmSolutionAcceptable(1, 1));
    CPPUNIT_ASSERT(true                                == tArticle->mReverseBiasState);
    CPPUNIT_ASSERT(1                                   == tArticle->mReverseBiasFlips);
    CPPUNIT_ASSERT(GunnsElectConverterOutput::LIMIT_UV == tArticle->mLimitState);

    /// @test    Rejects due to leaving under-voltage limiting state to no limit.
    tArticle->mInputPowerValid     = true;
    tArticle->mAdmittanceMatrix[0] = 0.0;
    tArticle->mSourceVector[0]     = 0.0;
    tArticle->mPotentialVector[0]  = 120.0;
    tArticle->mSourceVoltage       = 0.0;
    expectedFlux                   = 0.0;
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT              == tArticle->confirmSolutionAcceptable(1, 1));
    CPPUNIT_ASSERT(GunnsElectConverterOutput::NO_LIMIT == tArticle->mLimitState);
    CPPUNIT_ASSERT(1                                   == tArticle->mLimitStateFlips);
    CPPUNIT_ASSERT(false                               == tArticle->mInputPowerValid);
    CPPUNIT_ASSERT(false                               == tArticle->mReverseBiasState);
    CPPUNIT_ASSERT(1                                   == tArticle->mReverseBiasFlips);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mFlux, DBL_EPSILON);

    /// - Re-enter under-voltage limit state.
    tArticle->mAdmittanceMatrix[0] = FLT_EPSILON;
    tArticle->mSourceVector[0]     = tSetpoint;
    tArticle->mPotentialVector[0]  = 0.1;
    tArticle->mSourceVoltage       = 0.0;
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT              == tArticle->confirmSolutionAcceptable(1, 1));
    CPPUNIT_ASSERT(GunnsElectConverterOutput::LIMIT_UV == tArticle->mLimitState);
    CPPUNIT_ASSERT(2                                   == tArticle->mLimitStateFlips);

    /// @test    Rejects due to leaving under-voltage limiting state to no limit due to output
    ///          current restored.
    tArticle->mInputPowerValid     = true;
    tArticle->mAdmittanceMatrix[0] = tOutputConductance;
    tArticle->mSourceVector[0]     = tOutputConductance * tOutUnderVoltageTrip;
    tArticle->mPotentialVector[0]  = 89.9;
    tArticle->mSourceVoltage       = tOutUnderVoltageTrip;
    expectedFlux                   = tOutputConductance * tOutUnderVoltageTrip - tOutputConductance * 89.9;
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT              == tArticle->confirmSolutionAcceptable(1, 1));
    CPPUNIT_ASSERT(GunnsElectConverterOutput::NO_LIMIT == tArticle->mLimitState);
    CPPUNIT_ASSERT(2                                   == tArticle->mLimitStateFlips);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mFlux, DBL_EPSILON);

    /// @test    Rejects due to entering over-voltage limiting state from no limit.
    tArticle->mPotentialVector[0]  = 200.0;
    tArticle->mSourceVoltage       = 0.0;
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT              == tArticle->confirmSolutionAcceptable(1, 1));
    CPPUNIT_ASSERT(false                               == tArticle->mOutputOverVoltageTrip.isTripped());
    CPPUNIT_ASSERT(GunnsElectConverterOutput::LIMIT_OV == tArticle->mLimitState);
    CPPUNIT_ASSERT(3                                   == tArticle->mLimitStateFlips);
    CPPUNIT_ASSERT(false                               == tArticle->mInputPowerValid);
    CPPUNIT_ASSERT(false                               == tArticle->mReverseBiasState);
    CPPUNIT_ASSERT(1                                   == tArticle->mReverseBiasFlips);

    /// @test    Rejects due to entering under-voltage limiting state from over-volt limit due to
    ///          output under-voltage.
    tArticle->mPotentialVector[0]  = 0.1;
    tArticle->mSourceVoltage       = tOutOverVoltageTrip;
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT              == tArticle->confirmSolutionAcceptable(1, 1));
    CPPUNIT_ASSERT(GunnsElectConverterOutput::LIMIT_UV == tArticle->mLimitState);
    CPPUNIT_ASSERT(4                                   == tArticle->mLimitStateFlips);
    CPPUNIT_ASSERT(false                               == tArticle->mInputPowerValid);

    /// @test    Rejects due to switching to reverse bias due to high output voltage.
    tArticle->mPotentialVector[0]  = 200.0;
    tArticle->mSourceVoltage       = tOutUnderVoltageTrip;
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT              == tArticle->confirmSolutionAcceptable(1, 1));
    CPPUNIT_ASSERT(true                                == tArticle->mReverseBiasState);
    CPPUNIT_ASSERT(2                                   == tArticle->mReverseBiasFlips);
    CPPUNIT_ASSERT(GunnsElectConverterOutput::LIMIT_UV == tArticle->mLimitState);

    /// @test    Rejects due to entering over-voltage limiting state from under-volt limit.
    tArticle->mAdmittanceMatrix[0] = 0.0;
    tArticle->mSourceVector[0]     = 0.0;
    tArticle->mSourceVoltage       = 0.0;
    tArticle->mInputPowerValid     = true;
    tArticle->mLimitStateFlips     = 3;
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT              == tArticle->confirmSolutionAcceptable(1, 1));
    CPPUNIT_ASSERT(GunnsElectConverterOutput::LIMIT_OV == tArticle->mLimitState);
    CPPUNIT_ASSERT(4                                   == tArticle->mLimitStateFlips);
    CPPUNIT_ASSERT(false                               == tArticle->mInputPowerValid);
    CPPUNIT_ASSERT(true                                == tArticle->mReverseBiasState);

    /// @test    Rejects due to switching to forward bias due to low output voltage.
    tArticle->mSourceVoltage       = tOutOverVoltageTrip;
    tArticle->mPotentialVector[0]  = tOutUnderVoltageTrip;
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT              == tArticle->confirmSolutionAcceptable(1, 1));
    CPPUNIT_ASSERT(false                               == tArticle->mReverseBiasState);
    CPPUNIT_ASSERT(2                                   == tArticle->mReverseBiasFlips);
    CPPUNIT_ASSERT(GunnsElectConverterOutput::LIMIT_OV == tArticle->mLimitState);
    CPPUNIT_ASSERT(4                                   == tArticle->mLimitStateFlips);

    /// @test    Rejects due to entering under-voltage limiting state from over-volt limit due to
    ///          excess output current.
    tArticle->mAdmittanceMatrix[0] = tOutputConductance;
    tArticle->mSourceVector[0]     = tOutputConductance * tOutOverVoltageTrip;
    tArticle->mLimitStateFlips     = 3;
    tArticle->mInputPowerValid     = true;
    expectedFlux                   = tOutputConductance * tOutOverVoltageTrip - tOutputConductance * tOutUnderVoltageTrip;
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT              == tArticle->confirmSolutionAcceptable(1, 1));
    CPPUNIT_ASSERT(GunnsElectConverterOutput::LIMIT_UV == tArticle->mLimitState);
    CPPUNIT_ASSERT(4                                   == tArticle->mLimitStateFlips);
    CPPUNIT_ASSERT(false                               == tArticle->mInputPowerValid);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mFlux, DBL_EPSILON);

    /// @test    Rejects due to entering the over-current limiting state from under-volt limit due
    ///          to excess output current.
    tArticle->mAdmittanceMatrix[0] = tOutputConductance;
    tArticle->mSourceVector[0]     = tOutputConductance * tOutUnderVoltageTrip;
    tArticle->mPotentialVector[0]  = 0.0;
    tArticle->mLimitStateFlips     = 3;
    tArticle->mInputPowerValid     = true;
    expectedFlux = tArticle->mSourceVector[0];
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT              == tArticle->confirmSolutionAcceptable(1, 1));
    CPPUNIT_ASSERT(GunnsElectConverterOutput::LIMIT_OC == tArticle->mLimitState);
    CPPUNIT_ASSERT(4                                   == tArticle->mLimitStateFlips);
    CPPUNIT_ASSERT(false                               == tArticle->mInputPowerValid);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mFlux, DBL_EPSILON);

    /// @test    Rejects due to entering the over-voltage limiting state from over-current limit due
    ///          to excess output voltage.
    tArticle->mAdmittanceMatrix[0] = FLT_EPSILON;
    tArticle->mSourceVector[0]     = tOutOverCurrentTrip;
    tArticle->mPotentialVector[0]  = 200.0;
    tArticle->mLimitStateFlips     = 3;
    tArticle->mInputPowerValid     = true;
    expectedFlux = tOutOverCurrentTrip - 200.0 * FLT_EPSILON;
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT              == tArticle->confirmSolutionAcceptable(1, 1));
    CPPUNIT_ASSERT(GunnsElectConverterOutput::LIMIT_OV == tArticle->mLimitState);
    CPPUNIT_ASSERT(4                                   == tArticle->mLimitStateFlips);
    CPPUNIT_ASSERT(false                               == tArticle->mInputPowerValid);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mFlux, DBL_EPSILON);

    /// @test    Rejects due to entering the no-limiting state from over-current limit due
    ///          to excess output voltage.
    tArticle->mLimitState = GunnsElectConverterOutput::LIMIT_OC;
    tArticle->mPotentialVector[0]  = 120.0;
    tArticle->mLimitStateFlips     = 3;
    tArticle->mReverseBiasState    = false;
    tArticle->mInputPowerValid     = true;
    expectedFlux = tOutOverCurrentTrip - 120.0 * FLT_EPSILON;
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT              == tArticle->confirmSolutionAcceptable(1, 1));
    CPPUNIT_ASSERT(GunnsElectConverterOutput::NO_LIMIT == tArticle->mLimitState);
    CPPUNIT_ASSERT(3                                   == tArticle->mLimitStateFlips);
    CPPUNIT_ASSERT(false                               == tArticle->mInputPowerValid);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mFlux, DBL_EPSILON);

    /// @test    Can't enter limit state > 4 times.
    tArticle->mLimitState = GunnsElectConverterOutput::NO_LIMIT;
    tArticle->mAdmittanceMatrix[0] = FLT_EPSILON;
    tArticle->mSourceVector[0]     = tSetpoint;
    tArticle->mPotentialVector[0]  = 0.1;
    tArticle->mLimitStateFlips     = 4;
    tArticle->mSourceVoltage       = 0.0;
    tArticle->mInputPowerValid     = true;
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM             == tArticle->confirmSolutionAcceptable(1, 1));
    CPPUNIT_ASSERT(GunnsElectConverterOutput::NO_LIMIT == tArticle->mLimitState);
    CPPUNIT_ASSERT(4                                   == tArticle->mLimitStateFlips);
    CPPUNIT_ASSERT(true                                == tArticle->mInputPowerValid);

    /// @test    Rejects due to invalid voltage from the input link.
    FriendlyGunnsElectConverterOutput article2;
    tConfigData->mInputLink = &tInputLink;
    article2.initialize(*tConfigData, *tInputData, tLinks, tPort0);
    tInputLink.initialize(*tInputConfigData, *tInputInputData, tLinks, 1);
    tInputLink.mInputVoltageValid = false;
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT == article2.confirmSolutionAcceptable(1, 1));

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the computeFlows method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectConverterOutput::testComputeFlows()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);

    /// @test    Output as voltage source.
    double setpoint     = 100.0;
    double nodeV        =  99.0;
    double conductance  = tOutputConductance;
    double source       = conductance * setpoint;
    double expectedFlux = source - nodeV * conductance;
    tArticle->mSourceVector[0]     = source;
    tArticle->mAdmittanceMatrix[0] = conductance;
    tArticle->mPotentialVector[0]  = nodeV;

    tArticle->computeFlows(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-nodeV,       tArticle->mPotentialDrop, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mFlux,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tNodes[0].getInflux(),    DBL_EPSILON);
    tNodes[0].resetFlows();

    /// @test    Output as current source.
    setpoint     = 2.0;
    nodeV        = 125.0;
    conductance  = 0.0;
    source       = setpoint;
    expectedFlux = setpoint;
    tArticle->mSourceVector[0]     = source;
    tArticle->mAdmittanceMatrix[0] = conductance;
    tArticle->mPotentialVector[0]  = nodeV;

    tArticle->computeFlows(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-nodeV,       tArticle->mPotentialDrop, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mFlux,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tNodes[0].getInflux(),    DBL_EPSILON);
    tNodes[0].resetFlows();

    /// @test    Negative flux gets limited to zero.
    source       = -1.0;
    expectedFlux = 0.0;
    tArticle->mSourceVector[0] = source;

    tArticle->computeFlows(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mFlux,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tNodes[0].getInflux(), DBL_EPSILON);
    tNodes[0].resetFlows();

    /// @test    On Ground node.
    tArticle->mUserPortSelect     = 0;
    tArticle->mUserPortSetControl = GunnsBasicLink::GROUND;
    tArticle->step(0.0);

    tArticle->mFlux          = 5.0;
    tArticle->mPotentialDrop = -120.0;
    tArticle->computeFlows(0.0);
    CPPUNIT_ASSERT(0.0 == tArticle->mPotentialDrop);
    CPPUNIT_ASSERT(0.0 == tArticle->mFlux);
    CPPUNIT_ASSERT(0.0 == tNodes[0].getInflux());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the getControlVoltage method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectConverterOutput::testControlVoltage()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);

    /// @test    Nominal control voltage output for a transformer.
    double expectedV = tInputVoltage * tSetpoint;
    tArticle->step(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, tArticle->getControlVoltage(), DBL_EPSILON);

    /// @test    Nominal control voltage output for a voltage regulator.
    tArticle->mRegulatorType = GunnsElectConverterOutput::VOLTAGE;
    tArticle->setSetpoint(tInputVoltage);
    expectedV = tInputVoltage;
    tArticle->step(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, tArticle->getControlVoltage(), DBL_EPSILON);

    /// @test    No control voltage when completely blocked.
    tArticle->setMalfBlockage(true, 1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->getControlVoltage(), DBL_EPSILON);
    tArticle->setMalfBlockage();

    /// @test    No control voltage with zero source voltage.
    tArticle->mSourceVoltage = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->getControlVoltage(), DBL_EPSILON);
    tArticle->mSourceVoltage = tInputVoltage;

    /// @test    No control voltage with zero output conductance.
    tArticle->mOutputConductance = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->getControlVoltage(), DBL_EPSILON);
    tArticle->mOutputConductance = tOutputConductance;

    /// @test    Control voltage for a current regulator that is limit enabled and can undervolt limit.
    tArticle->mRegulatorType = GunnsElectConverterOutput::CURRENT;
    tArticle->mSourceVoltage = tOutUnderVoltageTrip - 1.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tOutUnderVoltageTrip, tArticle->getControlVoltage(), DBL_EPSILON);

    /// @test    Control voltage for a current regulator that is overvoltage limiting.
    tArticle->mSourceVoltage = tOutOverVoltageTrip;
    tArticle->mLimitState = GunnsElectConverterOutput::LIMIT_OV;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mSourceVoltage, tArticle->getControlVoltage(), DBL_EPSILON);

    /// @test    Control voltage for a current regulator that is undervoltage limiting.
    tArticle->mSourceVoltage = tOutUnderVoltageTrip + 0.001;
    tArticle->mLimitState = GunnsElectConverterOutput::LIMIT_UV;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mSourceVoltage, tArticle->getControlVoltage(), DBL_EPSILON);

    UT_PASS_FINAL;
}
