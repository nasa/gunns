/**
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((aspects/electrical/SolarArray/GunnsElectPvRegConv.o)
   (software/exceptions/TsInitializationException.o))
*/
#include "software/exceptions/TsInitializationException.hh"
#include <iostream>
#include "strings/UtResult.hh"
#include "UtGunnsElectPvRegConv.hh"
#include "math/MsMath.hh"

/// @details  Test identification number.
int UtGunnsElectPvRegConv::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsElectPvRegConv class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectPvRegConv::UtGunnsElectPvRegConv()
    :
    tLinks(),
    tNodes(),
    tNodeList(),
    tPort0(0),
    tName(""),
    tConfigData(0),
    tInputData(0),
    tArticle(0),
    tVoltageConvLimit(0.0),
    tVoltageConvEfficiency(0.0),
    tSensorIin(),
    tSensorVin(),
    tSensorIout(),
    tSensorVout(),
    tInOverCurrentTrip(0.0),
    tInOverVoltageTrip(0.0),
    tOutOverCurrentTrip(0.0),
    tOutOverVoltageTrip(0.0),
    tOutUnderVoltageTrip(0.0),
    tTripPriority(0),
    tArray(0),
    tArrayConfig(0),
    tArrayInput(0),
    tVoltageSetpoint(0.0),
    tVoltageSetpointDelta(0.0),
    tPowered(false),
    tEnabled(false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsElectPvRegConv class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectPvRegConv::~UtGunnsElectPvRegConv()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvRegConv::setUp()
{
    tName = "tArticle";

    /// - Define the nominal port mapping.
    tPort0 = 0;

    /// - Initialize the nodes list.
    tNodeList.mNodes    = tNodes;
    tNodeList.mNumNodes = N_NODES;

    const unsigned int numSections         = 3;
    const unsigned int numStrings          = 12;
    const unsigned int numStringsBySection = numStrings / numSections;

    /// - Initialize the sensors.
    GunnsSensorAnalogWrapperConfigData sensorConfig("tSensorVin", 0.0, 1.0E10);
    GunnsSensorAnalogWrapperInputData  sensorInput;
    tSensorVin.initialize(&sensorConfig, &sensorInput);

    sensorConfig.mName = "tSensorIin";
    tSensorIin.initialize(&sensorConfig, &sensorInput);

    sensorConfig.mName = "tSensorVout";
    tSensorVout.initialize(&sensorConfig, &sensorInput);

    sensorConfig.mName = "tSensorIout";
    tSensorIout.initialize(&sensorConfig, &sensorInput);

    /// - Create and initialize a nominal array.  We use the same config & input data as in
    ///   UtGunnsElectPvArray.
    tArrayConfig       = new GunnsElectPvArrayConfigData("tArray", &tNodeList, numSections,
                                                         numStrings, 0.8, 0.75, false, 31.636, 0.7,
                                                         0.5, 5, 20, 0.05, 1.0, 0.017, 200.0, 0.6,
                                                         294.0, -0.003, 0.00065);
    tArrayInput        = new GunnsElectPvArrayInputData(31.626, 0.0, 1.0, 284.0);
    tArray             = new FriendlyGunnsElectPvArray;
    tArray->initialize(*tArrayConfig, *tArrayInput, tLinks, tPort0);

    /// - Define the nominal configuration data.
    tVoltageConvLimit      = 1.5;
    tVoltageConvEfficiency = 0.9;
    tInOverCurrentTrip     = 5.0;
    tInOverVoltageTrip     = 11.2;
    tOutOverCurrentTrip    = 15.0;
    tOutOverVoltageTrip    = 11.0;
    tOutUnderVoltageTrip   = 5.0;
    tTripPriority          = 2;
    tConfigData            = new GunnsElectPvRegConvConfigData(tName,
                                                               &tNodeList,
                                                               tVoltageConvLimit,
                                                               tVoltageConvEfficiency,
                                                               tArray,
                                                               &tSensorIin,
                                                               &tSensorVin,
                                                               &tSensorIout,
                                                               &tSensorVout,
                                                               tInOverCurrentTrip,
                                                               tInOverVoltageTrip,
                                                               tOutOverCurrentTrip,
                                                               tOutOverVoltageTrip,
                                                               tOutUnderVoltageTrip,
                                                               tTripPriority);

    /// - Define the nominal input data.
    tVoltageSetpoint      = 10.0;
    tVoltageSetpointDelta = 1.0;
    tPowered              = true;
    tEnabled              = true;
    tInputData            = new GunnsElectPvRegConvInputData(tVoltageSetpoint,
                                                             tVoltageSetpointDelta,
                                                             tPowered,
                                                             tEnabled);

    /// - Default construct the nominal test article.
    tArticle           = new FriendlyGunnsElectPvRegConv;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvRegConv::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete tArray;
    delete tArrayInput;
    delete tArrayConfig;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of Photovoltaic Array Converter Regulator Link configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvRegConv::testConfig()
{
    UT_RESULT_FIRST;

    /// @test    Configuration nominal construction.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tVoltageConvLimit,      tConfigData->mVoltageConvLimit,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tVoltageConvEfficiency, tConfigData->mVoltageConvEfficiency, 0.0);
    CPPUNIT_ASSERT(tArray              == tConfigData->mArray);
    CPPUNIT_ASSERT(&tSensorIin         == tConfigData->mInCurrentSensor);
    CPPUNIT_ASSERT(&tSensorVin         == tConfigData->mInVoltageSensor);
    CPPUNIT_ASSERT(&tSensorIout        == tConfigData->mOutCurrentSensor);
    CPPUNIT_ASSERT(&tSensorVout        == tConfigData->mOutVoltageSensor);
    CPPUNIT_ASSERT(tInOverCurrentTrip  == tConfigData->mInOverCurrentTrip);
    CPPUNIT_ASSERT(tInOverVoltageTrip  == tConfigData->mInOverVoltageTrip);
    CPPUNIT_ASSERT(tOutOverCurrentTrip == tConfigData->mOutOverCurrentTrip);
    CPPUNIT_ASSERT(tOutOverVoltageTrip == tConfigData->mOutOverVoltageTrip);
    CPPUNIT_ASSERT(tTripPriority       == tConfigData->mTripPriority);

    /// @test    Configuration data default construction.
    GunnsElectPvRegConvConfigData defaultConfig;
    CPPUNIT_ASSERT(0.0 == defaultConfig.mVoltageConvLimit);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mVoltageConvEfficiency);
    CPPUNIT_ASSERT(0   == defaultConfig.mArray);
    CPPUNIT_ASSERT(0   == defaultConfig.mInCurrentSensor);
    CPPUNIT_ASSERT(0   == defaultConfig.mInVoltageSensor);
    CPPUNIT_ASSERT(0   == defaultConfig.mOutCurrentSensor);
    CPPUNIT_ASSERT(0   == defaultConfig.mOutVoltageSensor);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mInOverCurrentTrip);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mInOverVoltageTrip);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mOutOverCurrentTrip);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mOutOverVoltageTrip);
    CPPUNIT_ASSERT(0   == defaultConfig.mTripPriority);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of Photovoltaic Array Converter Regulator Link input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvRegConv::testInput()
{
    UT_RESULT;

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tVoltageSetpoint,      tInputData->mVoltageSetpoint,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tVoltageSetpointDelta, tInputData->mVoltageSetpointDelta, 0.0);
    CPPUNIT_ASSERT(tPowered == tInputData->mPowered);
    CPPUNIT_ASSERT(tEnabled == tInputData->mEnabled);

    /// @test    Input data default construction.
    GunnsElectPvRegConvInputData defaultInput;
    CPPUNIT_ASSERT(0.0   == defaultInput.mVoltageSetpoint);
    CPPUNIT_ASSERT(0.0   == defaultInput.mVoltageSetpointDelta);
    CPPUNIT_ASSERT(false == defaultInput.mPowered);
    CPPUNIT_ASSERT(false == defaultInput.mEnabled);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the constructor of the GunnsElectPvRegConv class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvRegConv::testConstruction()
{
    UT_RESULT;

    /// @test    Default construction.
    CPPUNIT_ASSERT(false                    == tArticle->mMalfVoltageBiasFlag);
    CPPUNIT_ASSERT(0.0                      == tArticle->mMalfVoltageBiasValue);
    CPPUNIT_ASSERT(0.0                      == tArticle->mVoltageConvLimit);
    CPPUNIT_ASSERT(0.0                      == tArticle->mVoltageConvEfficiency);
    CPPUNIT_ASSERT(0                        == tArticle->mArray);
    CPPUNIT_ASSERT(0.0                      == tArticle->mVoltageSetpoint);
    CPPUNIT_ASSERT(false                    == tArticle->mPowered);
    CPPUNIT_ASSERT(false                    == tArticle->mEnabled);
    CPPUNIT_ASSERT(false                    == tArticle->mResetTrips);
    CPPUNIT_ASSERT(0                        == tArticle->mSensors.mInCurrent);
    CPPUNIT_ASSERT(0                        == tArticle->mSensors.mInVoltage);
    CPPUNIT_ASSERT(0                        == tArticle->mSensors.mOutCurrent);
    CPPUNIT_ASSERT(0                        == tArticle->mSensors.mOutVoltage);
    CPPUNIT_ASSERT(false                    == tArticle->mTrips.isTripped());
    CPPUNIT_ASSERT(GunnsElectPvRegConv::OFF == tArticle->mState);
    CPPUNIT_ASSERT(0.0                      == tArticle->mRegulatedVoltage);
    CPPUNIT_ASSERT(0.0                      == tArticle->mInputPower);
    CPPUNIT_ASSERT(0.0                      == tArticle->mOutputPower);
    CPPUNIT_ASSERT(0.0                      == tArticle->mWasteHeat);
    CPPUNIT_ASSERT(0.0                      == tArticle->mStateAdmittance[0]);
    CPPUNIT_ASSERT(0.0                      == tArticle->mStateAdmittance[1]);
    CPPUNIT_ASSERT(0.0                      == tArticle->mStateAdmittance[2]);
    CPPUNIT_ASSERT(0.0                      == tArticle->mStateAdmittance[3]);
    CPPUNIT_ASSERT(0.0                      == tArticle->mStateSource[0]);
    CPPUNIT_ASSERT(0.0                      == tArticle->mStateSource[1]);
    CPPUNIT_ASSERT(0.0                      == tArticle->mStateSource[2]);
    CPPUNIT_ASSERT(0.0                      == tArticle->mStateSource[3]);
    CPPUNIT_ASSERT(0.0                      == tArticle->mMaxRegCurrent);
    CPPUNIT_ASSERT(false                    == tArticle->mStateUpmodeLatch);
    CPPUNIT_ASSERT(""                       == tArticle->mName);

    /// @test    New/delete for code coverage.
    GunnsElectPvRegConv* testArticle = new GunnsElectPvRegConv();
    delete testArticle;

    UT_PASS;
}

#include "UtGunnsElectPvArray.hh"
#include "UtGunnsElectPvSection.hh"
#include "aspects/electrical/TripLogic/test/UtGunnsTripLogic.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Photovoltaic Array Converter Regulator Link nominal initialization without
///           exceptions, supplying a custom strings load order list.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvRegConv::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));

    /// @test    Nominal config data.
    CPPUNIT_ASSERT(tVoltageConvLimit        == tArticle->mVoltageConvLimit);
    CPPUNIT_ASSERT(tVoltageConvEfficiency   == tArticle->mVoltageConvEfficiency);
    CPPUNIT_ASSERT(tArray                   == tArticle->mArray);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT(tVoltageSetpoint         == tArticle->mVoltageSetpoint);
    CPPUNIT_ASSERT(tVoltageSetpoint         == tArticle->mVoltageSetpoint);
    CPPUNIT_ASSERT(tPowered                 == tArticle->mPowered);
    CPPUNIT_ASSERT(tEnabled                 == tArticle->mEnabled);

    /// @test    Sensors package.
    CPPUNIT_ASSERT(&tSensorIin.mSensor      == tArticle->mSensors.mInCurrent);
    CPPUNIT_ASSERT(&tSensorVin.mSensor      == tArticle->mSensors.mInVoltage);
    CPPUNIT_ASSERT(&tSensorIout.mSensor     == tArticle->mSensors.mOutCurrent);
    CPPUNIT_ASSERT(&tSensorVout.mSensor     == tArticle->mSensors.mOutVoltage);

    /// @test    Trips package.
    GunnsBasicLink::SolutionResult result;
    CPPUNIT_ASSERT(false                    == tArticle->mTrips.isTripped());
    CPPUNIT_ASSERT(true == tArticle->mTrips.mInOverVoltage.checkForTrip(result, tInOverVoltageTrip + 0.01, tTripPriority));
    CPPUNIT_ASSERT(true == tArticle->mTrips.mInOverCurrent.checkForTrip(result, tInOverCurrentTrip + 0.01, tTripPriority));
    CPPUNIT_ASSERT(true == tArticle->mTrips.mOutOverVoltage.checkForTrip(result, tOutOverVoltageTrip + 0.01, tTripPriority));
    CPPUNIT_ASSERT(true == tArticle->mTrips.mOutOverCurrent.checkForTrip(result, tOutOverCurrentTrip + 0.01, tTripPriority));
    CPPUNIT_ASSERT(true == tArticle->mTrips.mOutUnderVoltage.checkForTrip(result, tOutUnderVoltageTrip - 0.01, tTripPriority));

    /// @test    Nominal state data.
    CPPUNIT_ASSERT(GunnsElectPvRegConv::OFF == tArticle->mState);
    CPPUNIT_ASSERT(0.0                      == tArticle->mRegulatedVoltage);
    CPPUNIT_ASSERT(0.0                      == tArticle->mInputPower);
    CPPUNIT_ASSERT(0.0                      == tArticle->mOutputPower);
    CPPUNIT_ASSERT(0.0                      == tArticle->mWasteHeat);
    CPPUNIT_ASSERT(0.0                      == tArticle->mMaxRegCurrent);
    CPPUNIT_ASSERT(0.0                      == tArticle->mStateAdmittance[0]);
    CPPUNIT_ASSERT(0.0                      == tArticle->mStateAdmittance[1]);
    CPPUNIT_ASSERT(0.0                      == tArticle->mStateAdmittance[2]);
    CPPUNIT_ASSERT(0.0                      == tArticle->mStateAdmittance[3]);
    CPPUNIT_ASSERT(0.0                      == tArticle->mStateSource[0]);
    CPPUNIT_ASSERT(0.0                      == tArticle->mStateSource[1]);
    CPPUNIT_ASSERT(0.0                      == tArticle->mStateSource[2]);
    CPPUNIT_ASSERT(0.0                      == tArticle->mStateSource[3]);
    CPPUNIT_ASSERT(false                    == tArticle->mStateUpmodeLatch);
    CPPUNIT_ASSERT(tName                    == tArticle->mName);
    CPPUNIT_ASSERT(true                     == tArticle->mInitFlag);

    /// @test    Array interface.
    FriendlyGunnsElectPvArray* array = static_cast<FriendlyGunnsElectPvArray*>(tArray);
    CPPUNIT_ASSERT(true                     == array->mCommonStringsOutput);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Photovoltaic Array Converter Regulator Link nominal initialization with exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvRegConv::testInitializationErrors()
{
    UT_RESULT;

    /// @test    Exception thrown for bad voltage conversion limit.
    tConfigData->mVoltageConvLimit = 0.999;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0), TsInitializationException);
    tConfigData->mVoltageConvLimit = tVoltageConvLimit;

    /// @test    Exception thrown for bad voltage conversion efficiency.
    tConfigData->mVoltageConvEfficiency = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0), TsInitializationException);
    tConfigData->mVoltageConvEfficiency = 1.001;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0), TsInitializationException);
    tConfigData->mVoltageConvEfficiency = tVoltageConvEfficiency;

    /// @test    Exception thrown for null array pointer.
    tConfigData->mArray = 0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0), TsInitializationException);
    tConfigData->mArray = tArray;

    /// @test    Exception thrown for uninitialized array.
    FriendlyGunnsElectPvArray badArray;
    tConfigData->mArray = &badArray;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0), TsInitializationException);
    tConfigData->mArray = tArray;

    /// @test    Exception thrown from section for bad voltage setpoint.
    tInputData->mVoltageSetpoint = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0), TsInitializationException);
    tInputData->mVoltageSetpoint = tVoltageSetpoint;

    CPPUNIT_ASSERT(false == tArticle->mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Array Converter Regulator Link restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvRegConv::testRestart()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));

    /// @test    Restart method clears non-config and non-checkpointed data.
    tArticle->mState              = GunnsElectPvRegConv::SAG;
    tArticle->mRegulatedVoltage   = 1.0;
    tArticle->mInputPower         = 1.0;
    tArticle->mOutputPower        = 1.0;
    tArticle->mWasteHeat          = 1.0;
    tArticle->mMaxRegCurrent      = 1.0;
    tArticle->mStateAdmittance[0] = 1.0;
    tArticle->mStateAdmittance[1] = 1.0;
    tArticle->mStateAdmittance[2] = 1.0;
    tArticle->mStateAdmittance[3] = 1.0;
    tArticle->mStateSource[0]     = 1.0;
    tArticle->mStateSource[1]     = 1.0;
    tArticle->mStateSource[2]     = 1.0;
    tArticle->mStateSource[3]     = 1.0;
    tArticle->mStateUpmodeLatch   = true;
    tArticle->restart();
    CPPUNIT_ASSERT(GunnsElectPvRegConv::OFF == tArticle->mState);
    CPPUNIT_ASSERT(0.0                      == tArticle->mRegulatedVoltage);
    CPPUNIT_ASSERT(0.0                      == tArticle->mInputPower);
    CPPUNIT_ASSERT(0.0                      == tArticle->mOutputPower);
    CPPUNIT_ASSERT(0.0                      == tArticle->mWasteHeat);
    CPPUNIT_ASSERT(0.0                      == tArticle->mMaxRegCurrent);
    CPPUNIT_ASSERT(0.0                      == tArticle->mStateAdmittance[0]);
    CPPUNIT_ASSERT(0.0                      == tArticle->mStateAdmittance[1]);
    CPPUNIT_ASSERT(0.0                      == tArticle->mStateAdmittance[2]);
    CPPUNIT_ASSERT(0.0                      == tArticle->mStateAdmittance[3]);
    CPPUNIT_ASSERT(0.0                      == tArticle->mStateSource[0]);
    CPPUNIT_ASSERT(0.0                      == tArticle->mStateSource[1]);
    CPPUNIT_ASSERT(0.0                      == tArticle->mStateSource[2]);
    CPPUNIT_ASSERT(0.0                      == tArticle->mStateSource[3]);
    CPPUNIT_ASSERT(false                    == tArticle->mStateUpmodeLatch);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Array Converter Regulator Link step and updateState methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvRegConv::testStep()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));

    {
        /// @test    Regulated voltage w/o setpoint malf, nominal max outputs, initial OFF->REG
        ///          transition, state [A] & {w} values, outputs in REG state.
        const double expectedVreg = tVoltageSetpoint + tVoltageSetpointDelta;
        tArticle->mPotentialVector[0] = expectedVreg - 1.0e-8;

        tArray->step(0.0);
        const double cornerV        = tArray->getIvCornerVoltage() * tVoltageConvLimit;
        const double cornerI        = tArray->getIvCornerCurrent() * tVoltageConvEfficiency / tVoltageConvLimit;
        const double potential      = tArray->getOpenCircuitVoltage() * tVoltageConvLimit;
        const double currentLimit   = tArray->getShortCircuitCurrent() / tVoltageConvLimit;
        const double expectedAoff   = 1.0e-8;
        const double expectedAshort = (currentLimit - cornerI) / cornerV;
        const double expectedAsag   = cornerI / (potential - cornerV);
        const double expectedAreg   = 1.0e+8;
        const double expectedWoff   = 0.0;
        const double expectedWshort = currentLimit;
        const double expectedWsag   = potential    * expectedAsag;
        const double expectedWreg   = expectedVreg * expectedAreg;

        tArticle->step(0.0);

        const double expectedVarr = expectedVreg / tVoltageConvLimit;
        double p, g;
        tArray->predictLoadAtVoltage(p, g, expectedVarr);
        const double expectedImax = p * tVoltageConvEfficiency / (expectedVarr * tVoltageConvLimit);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVreg,   tArticle->mRegulatedVoltage,    DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAoff,   tArticle->mStateAdmittance[0],  DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAreg,   tArticle->mStateAdmittance[1],  DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAsag,   tArticle->mStateAdmittance[2],  DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAshort, tArticle->mStateAdmittance[3],  DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedWoff,   tArticle->mStateSource[0],      DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedWreg,   tArticle->mStateSource[1],      DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedWsag,   tArticle->mStateSource[2],      DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedWshort, tArticle->mStateSource[3],      DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedImax,   tArticle->mMaxRegCurrent,       DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAreg,   tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedWreg,   tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(GunnsElectPvRegConv::REG == tArticle->mState);
        CPPUNIT_ASSERT(true                     == tArticle->mStateUpmodeLatch);
        CPPUNIT_ASSERT(true                     == tArticle->needAdmittanceUpdate());

        /// @test    Array loading, flux, output & input power in REG state.
        const double expectedFlux = -(expectedVreg - 1.0e-8) * expectedAreg + expectedWreg;
        const double expectedPout = (expectedVreg - 1.0e-8) * expectedFlux;
        const double expectedPin  = expectedPout / tVoltageConvEfficiency;

        tArticle->step(0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,   tArticle->mFlux,                DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPout,   tArticle->mOutputPower,         DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPin,    tArticle->mInputPower,          DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPin,    tArray->getTerminal().mPower,   DBL_EPSILON);
    } {
        /// @test    Regulated voltage with setpoint malf, transition to OFF when disabled,
        ///          outputs in OFF state.
        tArticle->mMalfVoltageBiasFlag  = true;
        tArticle->mMalfVoltageBiasValue = 1.0;
        tArticle->mEnabled              = false;
        tArticle->step(0.0);

        const double expectedVreg = tVoltageSetpoint + tVoltageSetpointDelta + 1.0;
        const double expectedFlux = 0.0;
        const double expectedPout = 0.0;
        const double expectedPin  = 0.0;
        const double expectedAoff = 1.0e-8;
        const double expectedWoff = 0.0;

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVreg,   tArticle->mRegulatedVoltage,    DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAoff,   tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedWoff,   tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(GunnsElectPvRegConv::OFF == tArticle->mState);
        CPPUNIT_ASSERT(true                     == tArticle->needAdmittanceUpdate());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,   tArticle->mFlux,                DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPout,   tArticle->mOutputPower,         DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPin,    tArticle->mInputPower,          DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPin,    tArray->getTerminal().mPower,   DBL_EPSILON);
    } {
        /// @test    Transition from REG -> SAG.
        tArray->mSections[0].setSourceExposedFraction(0.5);
        tArray->mSections[1].setSourceExposedFraction(0.5);
        tArray->mSections[2].setSourceExposedFraction(0.5);
        tArray->step(0.0);

        const double cornerV      = tArray->getIvCornerVoltage() * tVoltageConvLimit;
        const double cornerI      = tArray->getIvCornerCurrent() * tVoltageConvEfficiency / tVoltageConvLimit;
        const double potential    = tArray->getOpenCircuitVoltage() * tVoltageConvLimit;
        const double expectedAsag = cornerI / (potential - cornerV);
        const double expectedWsag = potential * expectedAsag;

        tArticle->mMalfVoltageBiasFlag  = false;
        tArticle->mEnabled              = true;
        tArticle->step(0.0);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAsag,   tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedWsag,   tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(GunnsElectPvRegConv::SAG == tArticle->mState);
        CPPUNIT_ASSERT(true                     == tArticle->needAdmittanceUpdate());
    } {
        /// @test    Array unlit, low-limit on regulated voltage.
        tArray->mSections[0].setSourceExposedFraction(0.0);
        tArray->mSections[1].setSourceExposedFraction(0.0);
        tArray->mSections[2].setSourceExposedFraction(0.0);
        tArray->mSections[0].setSourceFluxMagnitude(0.0);
        tArray->mSections[1].setSourceFluxMagnitude(0.0);
        tArray->mSections[2].setSourceFluxMagnitude(0.0);
        tArray->step(0.0);

        tArticle->mMalfVoltageBiasFlag  = true;
        tArticle->mMalfVoltageBiasValue = -20.0;
        tArticle->step(0.0);

        const double expectedVreg = DBL_EPSILON;

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVreg,  tArticle->mRegulatedVoltage,    DBL_EPSILON);
        CPPUNIT_ASSERT(GunnsElectPvRegConv::OFF == tArticle->mState);
        CPPUNIT_ASSERT(true                     == tArticle->needAdmittanceUpdate());
    } {
        /// - Force a trip.
        GunnsBasicLink::SolutionResult result;
        tArticle->mTrips.mInOverCurrent.checkForTrip(result, 1000.0, tTripPriority);
        CPPUNIT_ASSERT(true                     == tArticle->mTrips.isTripped());

        /// @test    Transition to off & reset trips when unpowered.
        tArticle->mPowered = false;
        tArticle->step(0.0);

        CPPUNIT_ASSERT(false                    == tArticle->mTrips.isTripped());
        CPPUNIT_ASSERT(GunnsElectPvRegConv::OFF == tArticle->mState);
    } {
        /// @test    Link port assignment control.
        tArticle->mUserPortSelect     = 0;
        tArticle->mUserPortSetControl = GunnsBasicLink::GROUND;
        tArticle->step(0.0);

        CPPUNIT_ASSERT(GunnsBasicLink::READY == tArticle->mUserPortSetControl);
        CPPUNIT_ASSERT(1                     == tArticle->mNodeMap[0]);
    } {
        /// - Force a trip.
        GunnsBasicLink::SolutionResult result;
        tArticle->mTrips.mInOverCurrent.checkForTrip(result, 1000.0, tTripPriority);

        /// @test    Transition to OFF when tripped.
        tArticle->mPowered = true;
        tArticle->mState   = GunnsElectPvRegConv::REG;
        tArticle->step(0.0);

        CPPUNIT_ASSERT(true                     == tArticle->mTrips.isTripped());
        CPPUNIT_ASSERT(GunnsElectPvRegConv::OFF == tArticle->mState);
    } {
        tArray->mSections[0].setSourceExposedFraction(1.0);
        tArray->mSections[1].setSourceExposedFraction(1.0);
        tArray->mSections[2].setSourceExposedFraction(1.0);
        tArray->mSections[0].setSourceFluxMagnitude(31.626);
        tArray->mSections[1].setSourceFluxMagnitude(31.626);
        tArray->mSections[2].setSourceFluxMagnitude(31.626);
        tArray->step(0.0);

        /// @test    Reset trips from command.
        tArticle->mResetTrips = true;

        tArticle->step(0.0);

        CPPUNIT_ASSERT(false                    == tArticle->mTrips.isTripped());
    } {
        /// @test    State input limited to valid range.
        tArticle->mState = static_cast<GunnsElectPvRegConv::PvRegStates>(17);
        tArticle->mPowered = false;
        tArticle->step(0.0);
        CPPUNIT_ASSERT(GunnsElectPvRegConv::OFF == tArticle->mState);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the Array Converter Regulator Link minorStep method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvRegConv::testMinorStep()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));

    const double expectedVreg = tVoltageSetpoint + tVoltageSetpointDelta;
    tArticle->mPotentialVector[0] = expectedVreg - 1.0e-8;
    tArray->step(0.0);
    tArticle->step(0.0);
    CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());

    {
        /// @test    [A] and {w} outputs are updated in minorStep for SAG state.
        tArticle->mState = GunnsElectPvRegConv::SAG;

        tArticle->minorStep(0.0, 2);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mStateAdmittance[GunnsElectPvRegConv::SAG],
                                     tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mStateSource[GunnsElectPvRegConv::SAG],
                                     tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());
    } {
        /// @test    [A] and {w} outputs are updated in minorStep for OFF state.
        tArticle->mState = GunnsElectPvRegConv::OFF;

        tArticle->minorStep(0.0, 3);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mStateAdmittance[GunnsElectPvRegConv::OFF],
                                     tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mStateSource[GunnsElectPvRegConv::OFF],
                                     tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());
    } {
        /// @test    [A] and {w} outputs are updated in minorStep for SHORT state.
        tArticle->mState = GunnsElectPvRegConv::SHORT;

        tArticle->minorStep(0.0, 4);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mStateAdmittance[GunnsElectPvRegConv::SHORT],
                                     tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mStateSource[GunnsElectPvRegConv::SHORT],
                                     tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());
    } {
        /// @test    [A] and {w} outputs are updated in minorStep for REG state.
        tArticle->mState = GunnsElectPvRegConv::REG;

        tArticle->minorStep(0.0, 5);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mStateAdmittance[GunnsElectPvRegConv::REG],
                                     tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mStateSource[GunnsElectPvRegConv::REG],
                                     tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the Array Converter Regulator Link getter and setter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvRegConv::testAccessors()
{
    UT_RESULT;

    /// @test    Link is non-linear.
    CPPUNIT_ASSERT(true == tArticle->isNonLinear());

    /// @test    Can set and get the voltage setpoint.
    tArticle->setVoltageSetpoint(5.0);
    CPPUNIT_ASSERT(5.0 == tArticle->getVoltageSetpoint());

    /// @test    Can set voltage setpoint delta.
    tArticle->setVoltageSetpointDelta(2.0);
    CPPUNIT_ASSERT(2.0 == tArticle->mVoltageSetpointDelta);

    /// @test    Can set the enabled flag.
    tArticle->setEnabled(true);
    CPPUNIT_ASSERT(true == tArticle->mEnabled);

    /// @test    Can get maximum regulated current.
    tArticle->mMaxRegCurrent = 15.0;
    CPPUNIT_ASSERT(15.0 == tArticle->getMaxRegCurrent());

    /// @test    Can get the trip logic object.
    CPPUNIT_ASSERT(&tArticle->mTrips == tArticle->getTrips());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the confirmSolutionAcceptable method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvRegConv::testConfirmSolutionAcceptable()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));

    /// - Step the article and array to update realistic states.
    tArray->step(0.0);
    tArticle->step(0.0);
    CPPUNIT_ASSERT(GunnsElectPvRegConv::REG == tArticle->mState);

    /// @test    State transitions only reject after solution is converged.
    tArticle->mPotentialVector[0] = 1.0;
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM  == tArticle->confirmSolutionAcceptable(0, 1));
    CPPUNIT_ASSERT(GunnsElectPvRegConv::SHORT == tArticle->mState);
    tArticle->minorStep(0.0, 2);
    tArticle->mStateUpmodeLatch = false;
    tArticle->mPotentialVector[0] = 1.0e4;
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM  == tArticle->confirmSolutionAcceptable(0, 2));
    CPPUNIT_ASSERT(0.0 == tArticle->mSensors.mOutVoltage->getSensedOutput());
    CPPUNIT_ASSERT(0.0 == tArticle->mSensors.mOutCurrent->getSensedOutput());
    CPPUNIT_ASSERT(0.0 == tArticle->mSensors.mInVoltage->getSensedOutput());
    CPPUNIT_ASSERT(0.0 == tArticle->mSensors.mInCurrent->getSensedOutput());
    tArticle->minorStep(0.0, 3);
    tArticle->mStateUpmodeLatch = false;
    tArticle->mPotentialVector[0] = 1.0;
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT     == tArticle->confirmSolutionAcceptable(1, 3));
    CPPUNIT_ASSERT(GunnsElectPvRegConv::SHORT == tArticle->mState);

    /// @test    Sensor updates after solution is converged.
    double expectedSensedVin  = tArray->getTerminal().mVoltage;
    double expectedSensedIin  = tArray->getTerminal().mCurrent;
    double expectedSensedVout = 1.0;
    double expectedSensedIout = tArticle->mFlux;
    double actualSensedVin    = tArticle->mSensors.mInVoltage->getSensedOutput();
    double actualSensedIin    = tArticle->mSensors.mInCurrent->getSensedOutput();
    double actualSensedVout   = tArticle->mSensors.mOutVoltage->getSensedOutput();
    double actualSensedIout   = tArticle->mSensors.mOutCurrent->getSensedOutput();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSensedVin,  actualSensedVin,  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSensedIin,  actualSensedIin,  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSensedVout, actualSensedVout, FLT_EPSILON * expectedSensedVout);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSensedIout, actualSensedIout, FLT_EPSILON);

    /// @test    Trips occur on prioritized converged minor step.
    CPPUNIT_ASSERT(tOutOverCurrentTrip < tArticle->mSensors.mOutCurrent->getSensedOutput());
    CPPUNIT_ASSERT(false == tArticle->mTrips.isTripped());
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT     == tArticle->confirmSolutionAcceptable(2, 4));
    CPPUNIT_ASSERT(GunnsElectPvRegConv::OFF   == tArticle->mState);
    CPPUNIT_ASSERT(true  == tArticle->mTrips.isTripped());

    /// @test    State transition from SHORT->SAG but only when the state has not upmoded this major
    ///          step.
    tArray->mSections[0].setSourceExposedFraction(0.5);
    tArray->mSections[1].setSourceExposedFraction(0.5);
    tArray->mSections[2].setSourceExposedFraction(0.5);
    tArray->step(0.0);

    tArticle->mResetTrips = true;
    const double expectedVreg = tVoltageSetpoint + tVoltageSetpointDelta;
    tArticle->mPotentialVector[0] = expectedVreg + 10.0;
    tArticle->mState = GunnsElectPvRegConv::SHORT;
    tArticle->step(0.0);
    CPPUNIT_ASSERT(GunnsElectPvRegConv::SAG == tArticle->mState);
    CPPUNIT_ASSERT(true  == tArticle->mStateUpmodeLatch);

    tArticle->mState = GunnsElectPvRegConv::SHORT;
    tArticle->confirmSolutionAcceptable(0, 1);
    CPPUNIT_ASSERT(GunnsElectPvRegConv::SHORT == tArticle->mState);
    CPPUNIT_ASSERT(true  == tArticle->mStateUpmodeLatch);

    /// @test    Delays, then rejects on trip from the output current sensor.
    tArray->mSections[0].setSourceExposedFraction(1.0);
    tArray->mSections[1].setSourceExposedFraction(1.0);
    tArray->mSections[2].setSourceExposedFraction(1.0);
    tArray->step(0.0);

    tArticle->mTrips.mOutOverCurrent.initialize(0.1, tTripPriority, false);
    tArticle->mState = GunnsElectPvRegConv::REG;
    double outputVolts = expectedVreg - 1.0e-8;
    tArticle->mPotentialVector[0] = outputVolts;

    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM  == tArticle->confirmSolutionAcceptable(0, 1));
    CPPUNIT_ASSERT(GunnsElectPvRegConv::REG == tArticle->mState);
    CPPUNIT_ASSERT(GunnsBasicLink::DELAY    == tArticle->confirmSolutionAcceptable(1, 2));
    CPPUNIT_ASSERT(GunnsElectPvRegConv::REG == tArticle->mState);
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT   == tArticle->confirmSolutionAcceptable(2, 3));
    CPPUNIT_ASSERT(GunnsElectPvRegConv::OFF == tArticle->mState);

    /// @test    Delays, then rejects on trip when optional output current sensor is missing, but
    ///          the trip limit is still specified.
    tArticle->mTrips.resetTrips();
    tArticle->mState = GunnsElectPvRegConv::REG;
    tArticle->mSensors.mOutCurrent = 0;

    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM  == tArticle->confirmSolutionAcceptable(0, 1));
    CPPUNIT_ASSERT(GunnsElectPvRegConv::REG == tArticle->mState);
    CPPUNIT_ASSERT(GunnsBasicLink::DELAY    == tArticle->confirmSolutionAcceptable(1, 2));
    CPPUNIT_ASSERT(GunnsElectPvRegConv::REG == tArticle->mState);
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT   == tArticle->confirmSolutionAcceptable(2, 3));
    CPPUNIT_ASSERT(GunnsElectPvRegConv::OFF == tArticle->mState);

    /// @test    Doesn't trip when not enabled.
    tArticle->mTrips.resetTrips();
    tArticle->mState = GunnsElectPvRegConv::OFF;
    tArticle->mEnabled = false;

    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM   == tArticle->confirmSolutionAcceptable(0, 1));
    CPPUNIT_ASSERT(GunnsElectPvRegConv::OFF == tArticle->mState);
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM   == tArticle->confirmSolutionAcceptable(1, 2));
    CPPUNIT_ASSERT(GunnsElectPvRegConv::OFF == tArticle->mState);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the computeFlows method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvRegConv::testComputeFlows()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));

    /// - Step the article and array to update realistic states.
    tArray->step(0.0);

    const double expectedVreg = tVoltageSetpoint + tVoltageSetpointDelta;
    tArticle->mPotentialVector[0] = expectedVreg - 1.0e-8;
    tArticle->step(0.0);

    /// @test    Outputs.
    double inputVolts   = tArray->getTerminal().mVoltage;
    double outputVolts  = tArticle->mPotentialVector[0];
    double expectedPin  = tArray->getTerminal().mPower;
    double expectedPout = outputVolts * tArticle->mFlux;
    double expectedDp   = inputVolts - outputVolts;
    double expectedP    = expectedPout - expectedPin;
    double expectedHeat = -expectedP;

    tArticle->computeFlows(0.0);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDp,      tArticle->mPotentialDrop, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP,       tArticle->mPower,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPout,    tArticle->mOutputPower,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPin,     tArticle->mInputPower,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHeat,    tArticle->mWasteHeat,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mFlux, tNodes[0].getInflux(),    DBL_EPSILON);

    UT_PASS_LAST;
}
