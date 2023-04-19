/**
@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((aspects/electrical/SolarArray/GunnsElectPvRegShunt.o)
   (software/exceptions/TsInitializationException.o))
*/
#include "software/exceptions/TsInitializationException.hh"
#include <iostream>
#include "strings/UtResult.hh"
#include "UtGunnsElectPvRegShunt.hh"
#include "math/MsMath.hh"

/// @details  Test identification number.
int UtGunnsElectPvRegShunt::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsElectPvRegShunt class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectPvRegShunt::UtGunnsElectPvRegShunt()
    :
    tLinks(),
    tNodes(),
    tNodeList(),
    tPort0(0),
    tPort1(0),
    tName(""),
    tConfigData(0),
    tInputData(0),
    tArticle(0),
    tOutputConductance(0.0),
    tShuntConductance(0.0),
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
    tPowered(false),
    tEnabled(false),
    tMinOperatePower(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsElectPvRegShunt class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectPvRegShunt::~UtGunnsElectPvRegShunt()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvRegShunt::setUp()
{
    tName = "tArticle";

    /// - Define the nominal port mapping.
    tPort0 = 0;
    tPort1 = 1;

    /// - Initialize the nodes list.
    tNodeList.mNodes    = tNodes;
    tNodeList.mNumNodes = N_NODES;

    const unsigned int numSections         = 3;
    const unsigned int numStrings          = 12;
    const unsigned int numStringsBySection = numStrings / numSections;

    /// - Initialize the sensors.
    GunnsSensorAnalogWrapperConfigData sensorConfig("tSensorVin", 0.0, 20.0);
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
    tOutputConductance   = 100.0;
    tShuntConductance    = 10.0;
    tInOverCurrentTrip   = 5.0;
    tInOverVoltageTrip   = 11.2;
    tOutOverCurrentTrip  = 15.0;
    tOutOverVoltageTrip  = 11.0;
    tOutUnderVoltageTrip = 5.0;
    tTripPriority        = 2;
    tConfigData          = new GunnsElectPvRegShuntConfigData(tName,
                                                              &tNodeList,
                                                              tOutputConductance,
                                                              tShuntConductance,
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

    /// - Configure the string load order.
    for (int section = numSections - 1; section > -1; --section) {
        for (int string = numStringsBySection - 1; string > -1; --string) {
            tConfigData->addStringLoadOrder(section, string);
        }
    }

    /// - Define the nominal input data.
    tVoltageSetpoint   = 10.0;
    tPowered           = true;
    tEnabled           = true;
    tMinOperatePower   = 100.0;
    tInputData         = new GunnsElectPvRegShuntInputData(tVoltageSetpoint,
                                                           tPowered,
                                                           tEnabled,
                                                           tMinOperatePower);

    /// - Default construct the nominal test article.
    tArticle           = new FriendlyGunnsElectPvRegShunt;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvRegShunt::tearDown()
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
/// @details  Tests for construction of Photovoltaic Array Shunting Regulator Link configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvRegShunt::testConfig()
{
    UT_RESULT_FIRST;

    /// @test    Configuration nominal construction.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tOutputConductance, tConfigData->mOutputConductance, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tShuntConductance,  tConfigData->mShuntConductance,  0.0);
    CPPUNIT_ASSERT(tArray                    == tConfigData->mArray);
    CPPUNIT_ASSERT(&tSensorIin               == tConfigData->mInCurrentSensor);
    CPPUNIT_ASSERT(&tSensorVin               == tConfigData->mInVoltageSensor);
    CPPUNIT_ASSERT(&tSensorIout              == tConfigData->mOutCurrentSensor);
    CPPUNIT_ASSERT(&tSensorVout              == tConfigData->mOutVoltageSensor);
    CPPUNIT_ASSERT(tInOverCurrentTrip        == tConfigData->mInOverCurrentTrip);
    CPPUNIT_ASSERT(tInOverVoltageTrip        == tConfigData->mInOverVoltageTrip);
    CPPUNIT_ASSERT(tOutOverCurrentTrip       == tConfigData->mOutOverCurrentTrip);
    CPPUNIT_ASSERT(tOutOverVoltageTrip       == tConfigData->mOutOverVoltageTrip);
    CPPUNIT_ASSERT(tTripPriority             == tConfigData->mTripPriority);
    CPPUNIT_ASSERT(tArrayConfig->mNumStrings == tConfigData->mStringLoadOrder.size());
    CPPUNIT_ASSERT(0                         == tConfigData->mStringLoadOrder.back().mSection);
    CPPUNIT_ASSERT(0                         == tConfigData->mStringLoadOrder.back().mString);

    /// @test    Configuration data default construction.
    GunnsElectPvRegShuntConfigData defaultConfig;
    CPPUNIT_ASSERT(0.0 == defaultConfig.mOutputConductance);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mShuntConductance);
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
    CPPUNIT_ASSERT(0   == defaultConfig.mStringLoadOrder.size());

    /// @test    Load order functions for code coverage.
    GunnsElectPvStringLoadOrder order1(1, 2);
    GunnsElectPvStringLoadOrder* order2 = new GunnsElectPvStringLoadOrder(3, 4);
    *order2 = order1;
    CPPUNIT_ASSERT(*order2 == order1);
    delete order2;

    GunnsElectPvStringLoadOrder* order3 = new GunnsElectPvStringLoadOrder(order1);
    CPPUNIT_ASSERT(*order3 == order1);
    delete order3;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of Photovoltaic Array Shunting Regulator Link input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvRegShunt::testInput()
{
    UT_RESULT;

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tVoltageSetpoint, tInputData->mVoltageSetpoint, 0.0);
    CPPUNIT_ASSERT(tPowered         == tInputData->mPowered);
    CPPUNIT_ASSERT(tEnabled         == tInputData->mEnabled);
    CPPUNIT_ASSERT(tMinOperatePower == tInputData->mMinOperatePower);

    /// @test    Input data default construction.
    GunnsElectPvRegShuntInputData defaultInput;
    CPPUNIT_ASSERT(0.0   == defaultInput.mVoltageSetpoint);
    CPPUNIT_ASSERT(false == defaultInput.mPowered);
    CPPUNIT_ASSERT(false == defaultInput.mEnabled);
    CPPUNIT_ASSERT(0.0   == defaultInput.mMinOperatePower);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the constructor of the GunnsElectPvRegShunt class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvRegShunt::testConstruction()
{
    UT_RESULT;

    /// @test    Default construction.
    CPPUNIT_ASSERT(false                     == tArticle->mMalfVoltageBiasFlag);
    CPPUNIT_ASSERT(0.0                       == tArticle->mMalfVoltageBiasValue);
    CPPUNIT_ASSERT(0.0                       == tArticle->mOutputConductance);
    CPPUNIT_ASSERT(0.0                       == tArticle->mShuntConductance);
    CPPUNIT_ASSERT(0                         == tArticle->mArray);
    CPPUNIT_ASSERT(0.0                       == tArticle->mStringLoadOrder.size());
    CPPUNIT_ASSERT(0.0                       == tArticle->mVoltageSetpoint);
    CPPUNIT_ASSERT(false                     == tArticle->mPowered);
    CPPUNIT_ASSERT(false                     == tArticle->mEnabled);
    CPPUNIT_ASSERT(0.0                       == tArticle->mMinOperatePower);
    CPPUNIT_ASSERT(false                     == tArticle->mResetTrips);
    CPPUNIT_ASSERT(0                         == tArticle->mSensors.mInCurrent);
    CPPUNIT_ASSERT(0                         == tArticle->mSensors.mInVoltage);
    CPPUNIT_ASSERT(0                         == tArticle->mSensors.mOutCurrent);
    CPPUNIT_ASSERT(0                         == tArticle->mSensors.mOutVoltage);
    CPPUNIT_ASSERT(false                     == tArticle->mTrips.isTripped());
    CPPUNIT_ASSERT(GunnsElectPvRegShunt::OFF == tArticle->mState);
    CPPUNIT_ASSERT(0.0                       == tArticle->mRegulatedVoltage);
    CPPUNIT_ASSERT(0.0                       == tArticle->mInputConductance);
    CPPUNIT_ASSERT(0.0                       == tArticle->mShuntPower);
    CPPUNIT_ASSERT(0.0                       == tArticle->mInputPower);
    CPPUNIT_ASSERT(0.0                       == tArticle->mOutputPower);
    CPPUNIT_ASSERT(0.0                       == tArticle->mWasteHeat);
    CPPUNIT_ASSERT(0.0                       == tArticle->mPvBulkPowerAvail);
    CPPUNIT_ASSERT(0.0                       == tArticle->mMaxRegCurrent);
    CPPUNIT_ASSERT(""                        == tArticle->mName);

    /// @test    New/delete for code coverage.
    GunnsElectPvRegShunt* testArticle = new GunnsElectPvRegShunt();
    delete testArticle;

    UT_PASS;
}

#include "UtGunnsElectPvSection.hh"
#include "aspects/electrical/TripLogic/test/UtGunnsTripLogic.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Photovoltaic Array Shunting Regulator Link nominal initialization without
///           exceptions, supplying a custom strings load order list.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvRegShunt::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1));

    /// @test    Nominal config data.
    CPPUNIT_ASSERT(tOutputConductance        == tArticle->mOutputConductance);
    CPPUNIT_ASSERT(tShuntConductance         == tArticle->mShuntConductance);
    CPPUNIT_ASSERT(tArray                    == tArticle->mArray);
    CPPUNIT_ASSERT(tArrayConfig->mNumStrings == tArticle->mStringLoadOrder.size());
    CPPUNIT_ASSERT(0                         == tArticle->mStringLoadOrder.back().mSection);
    CPPUNIT_ASSERT(0                         == tArticle->mStringLoadOrder.back().mString);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT(tVoltageSetpoint          == tArticle->mVoltageSetpoint);
    CPPUNIT_ASSERT(tPowered                  == tArticle->mPowered);
    CPPUNIT_ASSERT(tEnabled                  == tArticle->mEnabled);
    CPPUNIT_ASSERT(tMinOperatePower          == tArticle->mMinOperatePower);

    /// @test    Sensors package.
    CPPUNIT_ASSERT(&tSensorIin.mSensor       == tArticle->mSensors.mInCurrent);
    CPPUNIT_ASSERT(&tSensorVin.mSensor       == tArticle->mSensors.mInVoltage);
    CPPUNIT_ASSERT(&tSensorIout.mSensor      == tArticle->mSensors.mOutCurrent);
    CPPUNIT_ASSERT(&tSensorVout.mSensor      == tArticle->mSensors.mOutVoltage);

    /// @test    Trips package.
    GunnsBasicLink::SolutionResult result;
    CPPUNIT_ASSERT(false                     == tArticle->mTrips.isTripped());
    CPPUNIT_ASSERT(true == tArticle->mTrips.mInOverVoltage.checkForTrip(result, tInOverVoltageTrip + 0.01, tTripPriority));
    CPPUNIT_ASSERT(true == tArticle->mTrips.mInOverCurrent.checkForTrip(result, tInOverCurrentTrip + 0.01, tTripPriority));
    CPPUNIT_ASSERT(true == tArticle->mTrips.mOutOverVoltage.checkForTrip(result, tOutOverVoltageTrip + 0.01, tTripPriority));
    CPPUNIT_ASSERT(true == tArticle->mTrips.mOutOverCurrent.checkForTrip(result, tOutOverCurrentTrip + 0.01, tTripPriority));
    CPPUNIT_ASSERT(true == tArticle->mTrips.mOutUnderVoltage.checkForTrip(result, tOutUnderVoltageTrip - 0.01, tTripPriority));

    /// @test    Nominal state data.
    CPPUNIT_ASSERT(GunnsElectPvRegShunt::OFF == tArticle->mState);
    CPPUNIT_ASSERT(0.0                       == tArticle->mRegulatedVoltage);
    CPPUNIT_ASSERT(0.0                       == tArticle->mInputConductance);
    CPPUNIT_ASSERT(0.0                       == tArticle->mShuntPower);
    CPPUNIT_ASSERT(0.0                       == tArticle->mInputPower);
    CPPUNIT_ASSERT(0.0                       == tArticle->mOutputPower);
    CPPUNIT_ASSERT(0.0                       == tArticle->mWasteHeat);
    CPPUNIT_ASSERT(0.0                       == tArticle->mPvBulkPowerAvail);
    CPPUNIT_ASSERT(0.0                       == tArticle->mMaxRegCurrent);
    CPPUNIT_ASSERT(tName                     == tArticle->mName);
    CPPUNIT_ASSERT(true                      == tArticle->mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Photovoltaic Array Shunting Regulator Link nominal initialization without
///           exceptions, with default string load order.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvRegShunt::testDefaultLoadOrderInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data, using an
    ///   empty string load order vector.
    tConfigData->mStringLoadOrder.clear();
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1));

    /// @test    Nominal config data.
    CPPUNIT_ASSERT(tOutputConductance        == tArticle->mOutputConductance);
    CPPUNIT_ASSERT(tShuntConductance         == tArticle->mShuntConductance);
    CPPUNIT_ASSERT(tArray                    == tArticle->mArray);
    CPPUNIT_ASSERT(tArrayConfig->mNumStrings == tArticle->mStringLoadOrder.size());
    const unsigned int section = tArrayConfig->mNumSections - 1;
    const unsigned int string  = tArrayConfig->mNumStrings / tArrayConfig->mNumSections - 1;
    CPPUNIT_ASSERT(section                   == tArticle->mStringLoadOrder.back().mSection);
    CPPUNIT_ASSERT(string                    == tArticle->mStringLoadOrder.back().mString);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT(tVoltageSetpoint          == tArticle->mVoltageSetpoint);
    CPPUNIT_ASSERT(tEnabled                  == tArticle->mEnabled);

    /// @test    Nominal state data.
    CPPUNIT_ASSERT(GunnsElectPvRegShunt::OFF == tArticle->mState);
    CPPUNIT_ASSERT(0.0                       == tArticle->mRegulatedVoltage);
    CPPUNIT_ASSERT(0.0                       == tArticle->mInputConductance);
    CPPUNIT_ASSERT(0.0                       == tArticle->mShuntPower);
    CPPUNIT_ASSERT(0.0                       == tArticle->mInputPower);
    CPPUNIT_ASSERT(0.0                       == tArticle->mOutputPower);
    CPPUNIT_ASSERT(0.0                       == tArticle->mWasteHeat);
    CPPUNIT_ASSERT(0.0                       == tArticle->mPvBulkPowerAvail);
    CPPUNIT_ASSERT(0.0                       == tArticle->mMaxRegCurrent);
    CPPUNIT_ASSERT(tName                     == tArticle->mName);
    CPPUNIT_ASSERT(true                      == tArticle->mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Photovoltaic Array Shunting Regulator Link nominal initialization with exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvRegShunt::testInitializationErrors()
{
    UT_RESULT;

    /// @test    Exception thrown for bad output conductance.
    tConfigData->mOutputConductance = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mOutputConductance = tOutputConductance;

    /// @test    Exception thrown for bad shunt conductance.
    tConfigData->mShuntConductance = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mShuntConductance = tShuntConductance;

    /// @test    Exception thrown for null array pointer.
    tConfigData->mArray = 0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mArray = tArray;

    /// @test    Exception thrown for uninitialized array.
    FriendlyGunnsElectPvArray badArray;
    tConfigData->mArray = &badArray;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mArray = tArray;

    /// @test    Exception thrown for bad string load order vector length.
    tConfigData->addStringLoadOrder(0, 0);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mStringLoadOrder.pop_back();

    /// @test    Exception thrown for bad section # in string load order vector.
    tConfigData->mStringLoadOrder.pop_back();
    tConfigData->addStringLoadOrder(5, 0);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mStringLoadOrder.pop_back();
    tConfigData->addStringLoadOrder(0, 0);

    /// @test    Exception thrown for bad string # in string load order vector.
    tConfigData->mStringLoadOrder.pop_back();
    tConfigData->addStringLoadOrder(0, 67);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mStringLoadOrder.pop_back();
    tConfigData->addStringLoadOrder(0, 0);

    /// @test    Exception thrown for duplicate entry in string load order vector.
    tConfigData->mStringLoadOrder.pop_back();
    tConfigData->mStringLoadOrder.pop_back();
    tConfigData->addStringLoadOrder(0, 0);
    tConfigData->addStringLoadOrder(0, 0);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mStringLoadOrder.pop_back();
    tConfigData->mStringLoadOrder.pop_back();
    tConfigData->addStringLoadOrder(0, 1);
    tConfigData->addStringLoadOrder(0, 0);

    /// @test    Exception thrown from section for bad trip priority.
    tConfigData->mTripPriority = 0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mTripPriority = tTripPriority;

    /// @test    Exception thrown from section for bad voltage setpoint.
    tInputData->mVoltageSetpoint = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tInputData->mVoltageSetpoint = tVoltageSetpoint;

    CPPUNIT_ASSERT(false == tArticle->mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Array Shunting Regulator Link restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvRegShunt::testRestart()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1));

    /// @test    Restart method clears non-config and non-checkpointed data.
    tArticle->mState            = GunnsElectPvRegShunt::SAG;
    tArticle->mRegulatedVoltage = 1.0;
    tArticle->mInputConductance = 1.0;
    tArticle->mShuntPower       = 1.0;
    tArticle->mInputPower       = 1.0;
    tArticle->mOutputPower      = 1.0;
    tArticle->mWasteHeat        = 1.0;
    tArticle->mPvBulkPowerAvail = 1.0;
    tArticle->mMaxRegCurrent    = 1.0;
    tArticle->restart();
    CPPUNIT_ASSERT(GunnsElectPvRegShunt::OFF == tArticle->mState);
    CPPUNIT_ASSERT(0.0                       == tArticle->mRegulatedVoltage);
    CPPUNIT_ASSERT(0.0                       == tArticle->mInputConductance);
    CPPUNIT_ASSERT(0.0                       == tArticle->mShuntPower);
    CPPUNIT_ASSERT(0.0                       == tArticle->mInputPower);
    CPPUNIT_ASSERT(0.0                       == tArticle->mOutputPower);
    CPPUNIT_ASSERT(0.0                       == tArticle->mWasteHeat);
    CPPUNIT_ASSERT(0.0                       == tArticle->mPvBulkPowerAvail);
    CPPUNIT_ASSERT(0.0                       == tArticle->mMaxRegCurrent);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Array Shunting Regulator Link step and updateState methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvRegShunt::testStep()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1));

    {
        /// @test    Regulated voltage w/o setpoint malf, nominal max outputs, initial OFF->REG
        ///          transition, [A] & {w} outputs in REG state.
        tArray->step(0.0);
        tArticle->step(0.0);

        double expectedPbulk, expectedGin;
        const double expectedVreg = tVoltageSetpoint;
        tArray->predictLoadAtVoltage(expectedPbulk, expectedGin, expectedVreg);
        const double expectedImax = tArrayConfig->mNumStrings
                                  * tArray->mSections[0].mStrings[0].getTerminal().mCurrent;
        const double expectedAin  = expectedGin;
        const double expectedAout = tOutputConductance;
        const double expectedW    = expectedVreg * expectedAout;

        CPPUNIT_ASSERT(tMinOperatePower < tArticle->mPvBulkPowerAvail);
        CPPUNIT_ASSERT(GunnsElectPvRegShunt::REG == tArticle->mState);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVreg,  tArticle->mRegulatedVoltage,    DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPbulk, tArticle->mPvBulkPowerAvail,    DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGin,   tArticle->mInputConductance,    DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedImax,  tArticle->mMaxRegCurrent,       FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAin,   tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tArticle->mAdmittanceMatrix[1], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tArticle->mAdmittanceMatrix[2], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAout,  tArticle->mAdmittanceMatrix[3], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW,     tArticle->mSourceVector[1],     DBL_EPSILON);
        CPPUNIT_ASSERT(false                     == tArray->mSections[0].mStrings[0].isShunted());
        CPPUNIT_ASSERT(true                      == tArticle->needAdmittanceUpdate());
        CPPUNIT_ASSERT(true                      == tArticle->mOffToRegOccurred);
    } {
        /// @test    Regulated voltage with setpoint malf, transition to OFF when disabled,
        ///          [A] & {w} outputs in OFF state.
        tArticle->mMalfVoltageBiasFlag  = true;
        tArticle->mMalfVoltageBiasValue = 1.0;
        tArticle->mEnabled              = false;
        tArticle->step(0.0);

        double expectedPbulk, expectedGin;
        const double expectedVreg = tVoltageSetpoint + 1.0;
        tArray->predictLoadAtVoltage(expectedPbulk, expectedGin, expectedVreg);
        const double expectedAin  = expectedGin;
        const double expectedAout = 1.0 / GunnsBasicLink::mConductanceLimit;
        const double expectedW    = 0.0;

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVreg,  tArticle->mRegulatedVoltage,    DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAin,   tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tArticle->mAdmittanceMatrix[1], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tArticle->mAdmittanceMatrix[2], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAout,  tArticle->mAdmittanceMatrix[3], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW,     tArticle->mSourceVector[1],     DBL_EPSILON);
        CPPUNIT_ASSERT(GunnsElectPvRegShunt::OFF == tArticle->mState);
        CPPUNIT_ASSERT(true                      == tArticle->needAdmittanceUpdate());
        CPPUNIT_ASSERT(false                     == tArticle->mOffToRegOccurred);
    } {
        /// @test    Transition from REG -> OFF due to low light.
        tArray->mSections[0].setSourceExposedFraction(0.5);
        tArray->mSections[1].setSourceExposedFraction(0.5);
        tArray->mSections[2].setSourceExposedFraction(0.5);
        tArray->step(0.0);
        tArticle->step(0.0);

        CPPUNIT_ASSERT(GunnsElectPvRegShunt::OFF == tArticle->mState);
        CPPUNIT_ASSERT(true                      == tArticle->needAdmittanceUpdate());
        CPPUNIT_ASSERT(false                     == tArticle->mOffToRegOccurred);
    } {
        /// @test    Array unlit, low-limit on regulated voltage.
        tArray->mSections[0].setSourceExposedFraction(0.0);
        tArray->mSections[1].setSourceExposedFraction(0.0);
        tArray->mSections[2].setSourceExposedFraction(0.0);
        tArray->mSections[0].setSourceFluxMagnitude(0.0);
        tArray->mSections[1].setSourceFluxMagnitude(0.0);
        tArray->mSections[2].setSourceFluxMagnitude(0.0);
        tArray->step(0.0);

        tArticle->mMalfVoltageBiasValue = -20.0;
        tArticle->step(0.0);

        const double expectedVreg = DBL_EPSILON;

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVreg,  tArticle->mRegulatedVoltage,    DBL_EPSILON);
        CPPUNIT_ASSERT(GunnsElectPvRegShunt::OFF == tArticle->mState);
        CPPUNIT_ASSERT(true                      == tArticle->needAdmittanceUpdate());
        CPPUNIT_ASSERT(false                     == tArticle->mOffToRegOccurred);
    } {
        /// - Force a trip.
        GunnsBasicLink::SolutionResult result;
        tArticle->mTrips.mInOverCurrent.checkForTrip(result, 1000.0, tTripPriority);
        CPPUNIT_ASSERT(true                      == tArticle->mTrips.isTripped());

        /// @test    Transition to off & reset trips when unpowered.
        tArticle->mPowered = false;
        tArticle->step(0.0);

        CPPUNIT_ASSERT(false                     == tArticle->mTrips.isTripped());
        CPPUNIT_ASSERT(GunnsElectPvRegShunt::OFF == tArticle->mState);
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
        tArticle->mState   = GunnsElectPvRegShunt::REG;
        tArticle->step(0.0);

        CPPUNIT_ASSERT(true                      == tArticle->mTrips.isTripped());
        CPPUNIT_ASSERT(GunnsElectPvRegShunt::OFF == tArticle->mState);
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

        CPPUNIT_ASSERT(false                     == tArticle->mTrips.isTripped());
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the Array Shunting Regulator Link minorStep method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvRegShunt::testMinorStep()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1));

    {
        /// @test    [A] and {w} outputs are updated in minorStep for REG state.
        tArticle->mState = GunnsElectPvRegShunt::REG;
        tArticle->mRegulatedVoltage = 10.0;
        tArticle->mInputConductance = 1.0;
        const double expectedAin  = tArticle->mInputConductance;
        const double expectedAout = tOutputConductance;
        const double expectedW    = tArticle->mRegulatedVoltage * expectedAout;

        tArticle->minorStep(0.0, 2);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAin,   tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tArticle->mAdmittanceMatrix[1], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tArticle->mAdmittanceMatrix[2], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAout,  tArticle->mAdmittanceMatrix[3], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW,     tArticle->mSourceVector[1],     DBL_EPSILON);
        CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());
    } {
        /// @test    [A] and {w} outputs are updated in minorStep for SAG state.
        tArticle->mState = GunnsElectPvRegShunt::SAG;
        const double expectedAout = tOutputConductance;
        const double expectedW    = 0.0;

        tArticle->minorStep(0.0, 3);

        CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedAout, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedAout, tArticle->mAdmittanceMatrix[1], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedAout, tArticle->mAdmittanceMatrix[2], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedAout, tArticle->mAdmittanceMatrix[3], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW,     tArticle->mSourceVector[1],     DBL_EPSILON);
        CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());
    } {
        /// @test    [A] and {w} outputs are updated in minorStep for OFF state.
        tArticle->mState = GunnsElectPvRegShunt::OFF;
        const double expectedAin  = tArticle->mInputConductance;
        const double expectedAout = 1.0 / GunnsBasicLink::mConductanceLimit;
        const double expectedW    = 0.0;

        tArticle->minorStep(0.0, 4);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAin,   tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tArticle->mAdmittanceMatrix[1], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tArticle->mAdmittanceMatrix[2], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAout,  tArticle->mAdmittanceMatrix[3], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW,     tArticle->mSourceVector[1],     DBL_EPSILON);
        CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the Array Shunting Regulator Link getter and setter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvRegShunt::testAccessors()
{
    UT_RESULT;

    /// @test    Link is non-linear.
    CPPUNIT_ASSERT(true == tArticle->isNonLinear());

    /// @test    Can set and get the voltage setpoint.
    tArticle->setVoltageSetpoint(5.0);
    CPPUNIT_ASSERT(5.0 == tArticle->getVoltageSetpoint());

    /// @test    Can set the enabled flag.
    tArticle->setEnabled(true);
    CPPUNIT_ASSERT(true == tArticle->mEnabled);

    /// @test    Can get maximum regulated current.
    tArticle->mMaxRegCurrent = 15.0;
    CPPUNIT_ASSERT(15.0 == tArticle->getMaxRegCurrent());

    /// @test    Can set and get the minimum operate power.
    tArticle->setMinOperatePower(1000.0);
    CPPUNIT_ASSERT(1000.0 == tArticle->getMinOperatePower());

    /// @test    Can get the trip logic object.
    CPPUNIT_ASSERT(&tArticle->mTrips == tArticle->getTrips());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the confirmSolutionAcceptable method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvRegShunt::testConfirmSolutionAcceptable()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1));

    /// - Step the article and array to update realistic states.
    tArray->step(0.0);
    tArticle->step(0.0);
    CPPUNIT_ASSERT(GunnsElectPvRegShunt::REG == tArticle->mState);
    CPPUNIT_ASSERT(true                      == tArticle->mOffToRegOccurred);

    /// @test    REG state loads the array strings, remains in REG state since the strings have
    ///          sufficient power, and remaining strings are shunted.
    double inputVolts  = 11.0;
    double outputVolts = 9.9;
    double powerDemand = tVoltageSetpoint * tOutputConductance * (tVoltageSetpoint - outputVolts);
    tArticle->mPotentialVector[0] = inputVolts;
    tArticle->mPotentialVector[1] = outputVolts;

    tArray->mSections[0].mStrings[0].loadAtVoltage(tVoltageSetpoint);
    const double loadedStringP = tArray->mSections[0].mStrings[0].getTerminal().mPower;
    const double loadedStringG = tArray->mSections[0].mStrings[0].getTerminal().mConductance;

    tArray->mSections[0].mStrings[0].loadAtConductance(tShuntConductance);
    const double shuntedStringP = tArray->mSections[0].mStrings[0].getTerminal().mPower;

    const int numLoadedStrings   = ceil(powerDemand / loadedStringP);
    const int numShuntedStrings  = tArrayConfig->mNumStrings - numLoadedStrings;
    const int firstLoadedSection = tArrayConfig->mNumSections - 1;
    const int firstLoadedString  = tArrayConfig->mNumStrings / tArrayConfig->mNumSections - 1;
    tArray->mSections[firstLoadedSection].mStrings[firstLoadedString].setShunted(true);

    double expectedPin  = numLoadedStrings * loadedStringP;
    double expectedGin  = numLoadedStrings * loadedStringG;
    double expectedPsh  = numShuntedStrings * shuntedStringP;
    double expectedFlux = (tVoltageSetpoint - outputVolts) * tOutputConductance;

    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM   == tArticle->confirmSolutionAcceptable(1, 2));
    CPPUNIT_ASSERT(GunnsElectPvRegShunt::REG == tArticle->mState);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPin,   tArticle->mInputPower,       FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGin,   tArticle->mInputConductance, FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPsh,   tArticle->mShuntPower,       FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,  tArticle->mFlux,             FLT_EPSILON);
    CPPUNIT_ASSERT(false == tArray->mCommonStringsOutput);
    CPPUNIT_ASSERT(true  == tArray->mSections[0].mStrings[0].isShunted());
    CPPUNIT_ASSERT(false == tArray->mSections[firstLoadedSection].mStrings[firstLoadedString].isShunted());

    /// @test    Sensor updates
    double expectedSensedVin  = tArray->mSections[firstLoadedSection].mStrings[firstLoadedString].getTerminal().mVoltage;
    double expectedSensedIin  = tArray->mSections[firstLoadedSection].mStrings[firstLoadedString].getTerminal().mCurrent;
    double expectedSensedVout = outputVolts;
    double expectedSensedIout = expectedFlux;
    double actualSensedVin    = tArticle->mSensors.mInVoltage->getSensedOutput();
    double actualSensedIin    = tArticle->mSensors.mInCurrent->getSensedOutput();
    double actualSensedVout   = tArticle->mSensors.mOutVoltage->getSensedOutput();
    double actualSensedIout   = tArticle->mSensors.mOutCurrent->getSensedOutput();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSensedVin,  actualSensedVin,  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSensedIin,  actualSensedIin,  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSensedVout, actualSensedVout, FLT_EPSILON * expectedSensedVout);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSensedIout, actualSensedIout, FLT_EPSILON);

    /// @test    Transition from REG -> OFF due to insufficient array power, only after solution is
    ///          converged, and all strings are shunted.  This tests the scenario where
    ///          vehicle load > PV available power > minimum operate power, which must be limited
    ///          by the model from flipping between REG-OFF indefinitely.
    tArray->step(0.0);
    tArticle->step(0.0);
    CPPUNIT_ASSERT(GunnsElectPvRegShunt::REG == tArticle->mState);
    CPPUNIT_ASSERT(false                     == tArticle->mOffToRegOccurred);
    outputVolts = 9.0;
    powerDemand = tVoltageSetpoint * tOutputConductance * (tVoltageSetpoint - outputVolts);
    tArticle->mPotentialVector[1] = outputVolts;

    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM   == tArticle->confirmSolutionAcceptable(0, 1));
    CPPUNIT_ASSERT(GunnsElectPvRegShunt::REG == tArticle->mState);
    CPPUNIT_ASSERT(GunnsBasicLink::DELAY     == tArticle->confirmSolutionAcceptable(1, 2));
    CPPUNIT_ASSERT(GunnsElectPvRegShunt::REG == tArticle->mState);
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT    == tArticle->confirmSolutionAcceptable(2, 3));
    CPPUNIT_ASSERT(GunnsElectPvRegShunt::OFF == tArticle->mState);
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM   == tArticle->confirmSolutionAcceptable(0, 4));
    CPPUNIT_ASSERT(GunnsElectPvRegShunt::OFF == tArticle->mState);
    CPPUNIT_ASSERT(GunnsBasicLink::DELAY     == tArticle->confirmSolutionAcceptable(1, 5));
    CPPUNIT_ASSERT(GunnsElectPvRegShunt::OFF == tArticle->mState);
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT    == tArticle->confirmSolutionAcceptable(2, 6));
    CPPUNIT_ASSERT(GunnsElectPvRegShunt::REG == tArticle->mState);
    CPPUNIT_ASSERT(true                      == tArticle->mOffToRegOccurred);
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM   == tArticle->confirmSolutionAcceptable(0, 7));
    CPPUNIT_ASSERT(GunnsElectPvRegShunt::REG == tArticle->mState);
    CPPUNIT_ASSERT(GunnsBasicLink::DELAY     == tArticle->confirmSolutionAcceptable(1, 8));
    CPPUNIT_ASSERT(GunnsElectPvRegShunt::REG == tArticle->mState);
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT    == tArticle->confirmSolutionAcceptable(2, 9));
    CPPUNIT_ASSERT(GunnsElectPvRegShunt::OFF == tArticle->mState);
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM   == tArticle->confirmSolutionAcceptable(0, 10));
    CPPUNIT_ASSERT(GunnsElectPvRegShunt::OFF == tArticle->mState);
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM   == tArticle->confirmSolutionAcceptable(1, 11));
    CPPUNIT_ASSERT(GunnsElectPvRegShunt::OFF == tArticle->mState);
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM   == tArticle->confirmSolutionAcceptable(2, 12));
    CPPUNIT_ASSERT(GunnsElectPvRegShunt::OFF == tArticle->mState);

    /// @test    Confirming in OFF state shunts all strings.
    expectedPin  = 0.0;
    expectedGin  = 0.0;
    expectedPsh  = tArrayConfig->mNumStrings * shuntedStringP;
    expectedFlux = 0.0;
    CPPUNIT_ASSERT(true == tArray->mCommonStringsOutput);
    CPPUNIT_ASSERT(true == tArray->mSections[0].mStrings[0].isShunted());
    CPPUNIT_ASSERT(true == tArray->mSections[firstLoadedSection].mStrings[firstLoadedString].isShunted());

    /// @test    Transition from REG -> OFF due to back-voltage.
    outputVolts = tVoltageSetpoint + 0.01;
    tArticle->mPotentialVector[1] = outputVolts;
    tArticle->mState = GunnsElectPvRegShunt::REG;

    CPPUNIT_ASSERT(GunnsBasicLink::REJECT    == tArticle->confirmSolutionAcceptable(0, 1));
    CPPUNIT_ASSERT(GunnsElectPvRegShunt::OFF == tArticle->mState);

    /// @test    Delays, then rejects on trip from the output current sensor.
    inputVolts  = 11.0;
    outputVolts =  9.8;
    tArticle->mPotentialVector[0] = inputVolts;
    tArticle->mPotentialVector[1] = outputVolts;
    tArticle->mState = GunnsElectPvRegShunt::REG;

    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM   == tArticle->confirmSolutionAcceptable(0, 1));
    CPPUNIT_ASSERT(GunnsElectPvRegShunt::REG == tArticle->mState);
    CPPUNIT_ASSERT(GunnsBasicLink::DELAY     == tArticle->confirmSolutionAcceptable(1, 2));
    CPPUNIT_ASSERT(GunnsElectPvRegShunt::REG == tArticle->mState);
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT    == tArticle->confirmSolutionAcceptable(2, 3));
    CPPUNIT_ASSERT(GunnsElectPvRegShunt::OFF == tArticle->mState);

    /// @test    Delays, then rejects on trip when optional output current sensor is missing, but
    ///          the trip limit is still specified.
    tArticle->mTrips.resetTrips();
    tArticle->mState = GunnsElectPvRegShunt::REG;
    tArticle->mSensors.mOutCurrent = 0;

    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM   == tArticle->confirmSolutionAcceptable(0, 1));
    CPPUNIT_ASSERT(GunnsElectPvRegShunt::REG == tArticle->mState);
    CPPUNIT_ASSERT(GunnsBasicLink::DELAY     == tArticle->confirmSolutionAcceptable(1, 2));
    CPPUNIT_ASSERT(GunnsElectPvRegShunt::REG == tArticle->mState);
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT    == tArticle->confirmSolutionAcceptable(2, 3));
    CPPUNIT_ASSERT(GunnsElectPvRegShunt::OFF == tArticle->mState);

    /// @test    Updates sensors but doesn't trip when not enabled.
    outputVolts = 10.0;
    tArticle->mPotentialVector[1] = outputVolts;
    tArticle->mTrips.resetTrips();
    tArticle->mState = GunnsElectPvRegShunt::OFF;
    tArticle->mEnabled = false;

    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM   == tArticle->confirmSolutionAcceptable(0, 1));
    CPPUNIT_ASSERT(GunnsElectPvRegShunt::OFF == tArticle->mState);
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM   == tArticle->confirmSolutionAcceptable(1, 2));
    CPPUNIT_ASSERT(GunnsElectPvRegShunt::OFF == tArticle->mState);
    expectedSensedVout = outputVolts;
    actualSensedVout   = tArticle->mSensors.mOutVoltage->getSensedOutput();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSensedVout, actualSensedVout, FLT_EPSILON * expectedSensedVout);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the computeFlows method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvRegShunt::testComputeFlows()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1));

    /// - Step the article and array to update realistic states.
    tArray->step(0.0);
    tArticle->step(0.0);

    /// @test    Outputs in REG state.
    double inputVolts  = tVoltageSetpoint;
    double outputVolts = inputVolts - 0.1;
    double expectedPin = 100.0;
    double expectedPsh = 5.0;
    tArticle->mPotentialVector[0] = inputVolts;
    tArticle->mPotentialVector[1] = outputVolts;
    tArticle->mState              = GunnsElectPvRegShunt::REG;
    tArticle->mInputPower         = expectedPin;
    tArticle->mShuntPower         = expectedPsh;
    double expectedDp   = inputVolts - outputVolts;
    double expectedFlux = expectedDp * tOutputConductance;
    double expectedP    = -expectedFlux * expectedDp;
    double expectedPout = expectedFlux * outputVolts;
    double expectedHeat = expectedPsh - expectedP;

    tArticle->mFlux = expectedFlux;
    tArticle->computeFlows(0.0);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDp,   tArticle->mPotentialDrop, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP,    tArticle->mPower,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPout, tArticle->mOutputPower,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPin,  tArticle->mInputPower,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHeat, tArticle->mWasteHeat,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tNodes[0].getOutflux(),   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tNodes[1].getInflux(),    DBL_EPSILON);

    /// @test    Outputs in OFF state.
    outputVolts = inputVolts + 1.0;
    expectedPsh = 5.0;
    tArticle->mPotentialVector[0] = inputVolts;
    tArticle->mPotentialVector[1] = outputVolts;
    tArticle->mState              = GunnsElectPvRegShunt::OFF;
    tArticle->mInputPower         = 100.0;
    tArticle->mShuntPower         = expectedPsh;
    expectedDp   = inputVolts - outputVolts;
    expectedFlux = 0.0;
    expectedP    = 0.0;
    expectedPin  = 0.0;
    expectedPout = 0.0;
    expectedHeat = expectedPsh - expectedP;

    tNodes[0].resetFlows();
    tNodes[1].resetFlows();
    tArticle->mFlux = expectedFlux;
    tArticle->computeFlows(0.0);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDp,   tArticle->mPotentialDrop, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP,    tArticle->mPower,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPout, tArticle->mOutputPower,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPin,  tArticle->mInputPower,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHeat, tArticle->mWasteHeat,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tNodes[0].getOutflux(),   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tNodes[1].getInflux(),    DBL_EPSILON);

    UT_PASS_LAST;
}
