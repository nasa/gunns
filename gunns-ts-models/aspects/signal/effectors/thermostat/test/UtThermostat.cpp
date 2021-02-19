/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    (
    (aspects/signal/effectors/thermostat/Thermostat.o)
    (common/sensors/SensorAnalog.o)
    (common/sensors/TsNoise.o)
    )
***************************************************************************************************/
#include <iostream>
#include "UtThermostat.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "common/sensors/SensorAnalog.hh"
#include "common/sensors/TsNoise.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Thermostat model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtThermostat::UtThermostat()
    :
    CppUnit::TestFixture(),
    tArticle(0),
    tName(""),
    tTimeStep(0.0),
    tTolerance(0.0),
    tConfig(0),
    tInput(0),
    tSensor(0),
    tNominalSetptLow(0.0),
    tNominalSetptHigh(0.0),
    tMalfCmdOnFlag(false),
    tMalfCmdOffFlag(false),
    tMalfSetptLowFlag(false),
    tMalfSetptHighFlag(false),
    tNumSensors(0),
    middle(0.0),
    tooCold(0.0),
    tooHot(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Thermostat model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtThermostat::~UtThermostat()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermostat::setUp()
{
    /// - Specify the name.
    tName = "test Thermostat";

    /// - Specify the number of sensors.
    tNumSensors = 1;

    /// - Define the nominal initialization data.
    tNominalSetptLow = 290;
    tNominalSetptHigh = 310;

    /// - Set malfunction parameters.
    tMalfCmdOnFlag  = false;
    tMalfCmdOffFlag = false;

    /// - Set temperature samples.
    middle  = (tNominalSetptLow + tNominalSetptHigh) / 2;
    tooHot  = tNominalSetptHigh + 100;
    tooCold = 0.0;

    /// - Define the nominal voltage, time step and comparison tolerance.
    tTimeStep  = 0.1;
    tTolerance = 1.0e-08;

    /// - Declare RTD sensor config and input.
    SensorAnalogConfigData rtdSensorConfig(0.0, 1000.0, 296.0);
    SensorAnalogInputData  rtdSensorInput(true, 296.0);

    /// - Load the sensor config with a noise function.
    rtdSensorConfig.mNoiseFunction = TsNoise::getNoise;

    /// - Instantiate the config and input data objects.
    tConfig = new ThermostatConfigData(rtdSensorConfig, tNominalSetptLow, tNominalSetptHigh,
            tNumSensors, ThermostatConfigData::SINGLE);
    tInput = new ThermostatInputData(rtdSensorInput, tMalfCmdOnFlag, tMalfCmdOffFlag,
            tMalfSetptLowFlag, tMalfSetptHighFlag, tNominalSetptLow, tNominalSetptHigh);

    /// - Default construct the test article.
    tArticle = new FriendlyThermostat;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermostat::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete tInput;
    delete tConfig;
    delete tArticle;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Thermostat model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermostat::testConfigAndInput()
{
    std::cout << "\n------------------------------------------------------------------------";
    std::cout << "\nThermostat 00: Config and Input Data Test ........";

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(tNumSensors == tConfig->cNumSensors);
    CPPUNIT_ASSERT(tNominalSetptLow  == tConfig->cNominalSetptLow);
    CPPUNIT_ASSERT(tNominalSetptHigh == tConfig->cNominalSetptHigh);

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(tMalfCmdOnFlag  == tInput->iMalfCmdOnFlag);
    CPPUNIT_ASSERT(tMalfCmdOffFlag == tInput->iMalfCmdOffFlag);
    CPPUNIT_ASSERT(tMalfSetptLowFlag == tInput->iMalfSetptLowFlag);
    CPPUNIT_ASSERT(tMalfSetptHighFlag == tInput->iMalfSetptHighFlag);
    CPPUNIT_ASSERT(tNominalSetptLow  == tInput->iMalfSetptLowValue);
    CPPUNIT_ASSERT(tNominalSetptHigh == tInput->iMalfSetptHighValue);

    /// @test    Config data default construction.
    SensorAnalogConfigData  rtdSensorConfig;
    ThermostatConfigData defaultConfig(rtdSensorConfig);
    CPPUNIT_ASSERT( 220.0 == defaultConfig.cNominalSetptLow );
    CPPUNIT_ASSERT( 240.0 == defaultConfig.cNominalSetptHigh );
    CPPUNIT_ASSERT( 1 == defaultConfig.cNumSensors );
    CPPUNIT_ASSERT( ThermostatConfigData::SINGLE == defaultConfig.cLogicType );

    /// @test    Input data default construction.
    SensorAnalogInputData  rtdSensorInput(true, 296.0);
    ThermostatInputData defaultInput(rtdSensorInput);
    CPPUNIT_ASSERT( !defaultInput.iMalfCmdOnFlag );
    CPPUNIT_ASSERT( !defaultInput.iMalfCmdOffFlag );
    CPPUNIT_ASSERT( !defaultInput.iIsKillSwitchEngaged);
    CPPUNIT_ASSERT( !defaultInput.iMalfSetptLowFlag);
    CPPUNIT_ASSERT( !defaultInput.iMalfSetptHighFlag);
    CPPUNIT_ASSERT(   0.0 == defaultInput.iMalfSetptLowValue);
    CPPUNIT_ASSERT(1000.0 == defaultInput.iMalfSetptHighValue);

    /// @test    Configuration data copy construction.
    ThermostatConfigData copyConfig(*tConfig);
    CPPUNIT_ASSERT(tNumSensors == copyConfig.cNumSensors);
    CPPUNIT_ASSERT(tNominalSetptLow  == copyConfig.cNominalSetptLow);
    CPPUNIT_ASSERT(tNominalSetptHigh == copyConfig.cNominalSetptHigh);

    /// @test    Input data copy construction.
    tInput->iIsKillSwitchEngaged = true;
    ThermostatInputData copyInput(*tInput);
    CPPUNIT_ASSERT(tMalfCmdOnFlag  == copyInput.iMalfCmdOnFlag);
    CPPUNIT_ASSERT(tMalfCmdOffFlag == copyInput.iMalfCmdOffFlag);
    CPPUNIT_ASSERT(true            == copyInput.iIsKillSwitchEngaged);
    CPPUNIT_ASSERT(tMalfSetptLowFlag == copyInput.iMalfSetptLowFlag);
    CPPUNIT_ASSERT(tMalfSetptHighFlag == copyInput.iMalfSetptHighFlag);
    CPPUNIT_ASSERT(tNominalSetptLow  == copyInput.iMalfSetptLowValue);
    CPPUNIT_ASSERT(tNominalSetptHigh == copyInput.iMalfSetptHighValue);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Thermostat model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermostat::testDefaultConstruction()
{
    std::cout << "\nThermostat 01: Default Construction Test .........";

    /// @test    Attributes.
    CPPUNIT_ASSERT(""  == tArticle->mName);
    CPPUNIT_ASSERT(      !tArticle->mInitialized);
    CPPUNIT_ASSERT(0   == tArticle->mTempSensor);
    CPPUNIT_ASSERT(0   == tArticle->mNumSensors);
    CPPUNIT_ASSERT(0.0 == tArticle->mNominalSetptHigh);
    CPPUNIT_ASSERT(0.0 == tArticle->mNominalSetptLow);

    /// @test    Malfunction attributes.
    CPPUNIT_ASSERT(      !tArticle->mMalfCmdOnFlag);
    CPPUNIT_ASSERT(      !tArticle->mMalfCmdOffFlag);
    CPPUNIT_ASSERT(      !tArticle->mMalfSetptLowFlag);
    CPPUNIT_ASSERT(      !tArticle->mMalfSetptHighFlag);
    CPPUNIT_ASSERT(0.0 == tArticle->mMalfSetptLowValue);
    CPPUNIT_ASSERT(0.0 == tArticle->mMalfSetptHighValue);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Thermostat model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermostat::testNominalInitialization()
{
    std::cout << "\nThermostat 02: Nominal Initialization Test .......";

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfig, *tInput, tName);

    /// @test    Attributes.
    CPPUNIT_ASSERT(tName == tArticle->mName);
    CPPUNIT_ASSERT(tMalfCmdOnFlag  == tArticle->mMalfCmdOnFlag);
    CPPUNIT_ASSERT(tMalfCmdOffFlag == tArticle->mMalfCmdOffFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mNominalTemperatureSetpt, (0.5 * (tNominalSetptLow + tNominalSetptHigh)), tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mTemperatureSetpt, tArticle->mNominalTemperatureSetpt, tTolerance);
    CPPUNIT_ASSERT(tArticle->mInitialized);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Thermostat model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermostat::testInitializationExceptions()
{
    std::cout << "\nThermostat 03: Initialization Exceptions Test ....";

    /// @test    Default data initialization exception on empty name.
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfig, *tInput, ""), TsInitializationException);

    /// @test    Initialization exception on a single-state logic type with more than one sensor.
    tConfig->cNumSensors = 4;
    tConfig->cLogicType = ThermostatConfigData::SINGLE;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfig, *tInput, tName), TsInitializationException);

    /// @test    Initialization exception on numTempSensors less than one.
    tConfig->cNumSensors = 0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfig, *tInput, tName), TsInitializationException);
    tConfig->cNumSensors = tNumSensors;

    /// @test    Initialization exception on low setpt greater than or equal to the high.
    tConfig->cNominalSetptLow = tConfig->cNominalSetptHigh;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfig, *tInput, tName), TsInitializationException);
    tConfig->cNominalSetptLow = tNominalSetptLow;

    /// @test    Initialization exception on low setpt greater than or equal to the high.
    tInput->iMalfSetptLowValue = tInput->iMalfSetptHighValue;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfig, *tInput, tName), TsInitializationException);
    tInput->iMalfSetptLowValue = tNominalSetptLow;

    /// @test  Exception on conflicting malfs.
    tInput->iMalfCmdOnFlag = true;
    tInput->iMalfCmdOffFlag = true;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfig, *tInput, tName), TsInitializationException);

    /// @test    Exception on update() of an uninitialized object.
    tArticle->mInitialized = false;
    CPPUNIT_ASSERT_THROW(tArticle->update(tTimeStep), TsInitializationException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Thermostat model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermostat::testAccessors()
{
    std::cout << "\nThermostat 04: Accessors Test ....................";

    /// @test    isInitialized before initialization.
    CPPUNIT_ASSERT( !tArticle->isInitialized() );
    /// @test    getNominalSetptLow before initialization.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->getNominalSetptLow(), 0.0, tTolerance);
    /// @test    getNominalSetptHigh before initialization.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->getNominalSetptHigh(), 0.0, tTolerance);

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfig, *tInput, tName);

    /// @test    isInitialized after initialization.
    CPPUNIT_ASSERT(tArticle->isInitialized());
    /// @test    getNominalSetptLow after initialization.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->getNominalSetptLow(), tNominalSetptLow, tTolerance);
    /// @test    getNominalSetptHigh after initialization.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->getNominalSetptHigh(), tNominalSetptHigh, tTolerance);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Thermostat model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermostat::testModifiers()
{
    std::cout << "\nThermostat 05: Modifiers Test ....................";

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfig, *tInput, tName);

    /// @test    setTemperatureSetpt.
    double expectedResult = tArticle->mTemperatureSetpt + 1.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mTemperatureSetpt, tArticle->mNominalTemperatureSetpt, tTolerance);
    tArticle->setTemperatureSetpt(expectedResult);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mTemperatureSetpt, expectedResult, tTolerance);

    // Reset for next test
    tArticle->setTemperatureSetpt(tArticle->mNominalTemperatureSetpt);
    tArticle->mCommandSetptBias = 0.0;

    /// @test    setCommandSetptBias.
    tArticle->setCommandSetptBias(-1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mCommandSetptBias, -1.0, tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mTemperatureSetpt, (tArticle->mNominalTemperatureSetpt + tArticle->mCommandSetptBias), tTolerance);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the nominal Thermostat behavior.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermostat::testNominalBehavior()
{
    std::cout << "\nThermostat 06: Nominal Behavior Test .............";

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfig, *tInput, tName);

    /// - Set the sensor to a cold value, test that the thermostat is commanding ON.
    assertAtTemperature(tooCold, LoadON, "N1. Temp too low but command not ON.");

    /// - CommandActual should be unchanged if the temperature is in the mid-range.
    assertAtTemperature(middle, LoadON,
            "N2. Temp changed to mid-range from cold, but command did not stay ON");

    /// - Set the sensor to a hot value, test that the thermostat is commanding OFF.
    assertAtTemperature(tooHot, LoadOFF, "N3. Temp too high but command not OFF.");

    /// - CommandActual should be unchanged if the temperature is in the mid-range.
    assertAtTemperature(middle, LoadOFF,
            "N4. Temp changed to mid-range from hot, but command did not stay OFF");


    /// - Modify temperature setpoint to force thermostat on.
    tArticle->setTemperatureSetpt(tNominalSetptHigh + 1.0);
    /// - Set the sensor to middle value, which is now too low. Thermostat should command on.
    assertAtTemperature(middle, LoadON, "N5. Temp too low but command not ON.");

    /// - Modify temperature setpoint to force thermostat off.
    tArticle->setTemperatureSetpt(tNominalSetptLow - 1.0);
    /// - Set the sensor to middle value, which is now too high. Thermostat should command off.
    assertAtTemperature(middle, LoadOFF, "N6. Temp too high but command not ON.");


    /// - Modify temperature setpoint command bias to force thermostat on.
    tArticle->setCommandSetptBias(tNominalSetptHigh - middle + 1.0);
    /// - Set the sensor to middle value, which is now too low. Thermostat should command on.
    assertAtTemperature(middle, LoadON, "N7. Temp too low but command not ON.");

    /// - Modify temperature setpoint command bias to force thermostat off.
    tArticle->setCommandSetptBias(tNominalSetptLow - middle - 1.0);
    /// - Set the sensor to middle value, which is now too high. Thermostat should command off.
    assertAtTemperature(middle, LoadOFF, "N8. Temp too high but command not ON.");

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the behavior of the Thermostat with malfunctions activated.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermostat::testMalfunctionSetpts()
{
    std::cout << "\nThermostat 07: Malfunctions Test .................";

    /// - Instantiate a test article with less-than-nominal malf setpoints.
    tInput->iMalfSetptLowValue  = tNominalSetptLow  + 1;
    tInput->iMalfSetptHighValue = tNominalSetptHigh + 1;
    tArticle->initialize(*tConfig, *tInput, "higher-than-nominal malf setpoints");

    testMalfunctionSetptsHot();

    /// - Instantiate a test article with less-than-nominal malf setpoints.
    tInput->iMalfSetptLowValue  = tNominalSetptLow  - 1;
    tInput->iMalfSetptHighValue = tNominalSetptHigh - 1;
    tArticle->initialize(*tConfig, *tInput, "less-than-nominal malf setpoints");

    testMalfunctionSetptsCold();

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the behavior of the Thermostat with setpoints malf active, biased low.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermostat::testMalfunctionSetptsCold()
{
    /// - Derive sample temperature values to test.
    double nominalMiddleMalfTooHot  = (tNominalSetptHigh + tArticle->mMalfSetptHighValue) / 2;
    double nominalTooColdMalfMiddle = ( tNominalSetptLow + tArticle->mMalfSetptLowValue ) / 2;

    /// - Set the sensor to a cold value, test that the thermostat is commanding ON when malf'd.
    tArticle->mMalfSetptHighFlag = true;
    assertAtTemperature(tooCold, LoadON,
            "C1. Temp is cold, but command is not ON.");

    /// - Set the sensor to a value that would command OFF a malf'd article but not a nominal one.
    tArticle->mMalfSetptHighFlag = false;
    assertAtTemperature(nominalMiddleMalfTooHot, LoadON,
            "C2. Temp still in nominal mid-range, but command not ON.");

    /// - Activate the malf, now the same temperature should trigger a command OFF.
    tArticle->mMalfSetptHighFlag = true;
    assertAtTemperature(nominalMiddleMalfTooHot, LoadOFF,
            "C3. Temp too high when malf'd, but command not OFF.");

    /// - Now, de-activate the malf. Even though the temperature has not changed, the thermostat
    ///   should flip back to the ON command, since the nominal command should be preserved.
    tArticle->mMalfSetptHighFlag = false;
    assertAtTemperature(nominalMiddleMalfTooHot, LoadON,
            "C4. Temp unchanged, but malf de-activated. Command should be back to nominal ON.");

    /// - Set the sensor to a middle range. The thermostat should be ON when when nominal.
    assertAtTemperature(middle, LoadON,
            "C5. Temp in very middle. Malf NOT active. Command should be ON.");

    /// - When the malf is activated, the malf'd command should default to the last nominal state.
    tArticle->mMalfSetptLowFlag = true;
    assertAtTemperature(middle, LoadON,
            "C6. Temp in very middle. Malf active. Command should be ON.");

    /// - A very high temperature reading should command OFF the thermostat, whether malf'd or nominal.
    assertAtTemperature(tooHot, LoadOFF,
            "C7. Temp too high. Command should be OFF.");

    /// - Set the sensor to a value that would command ON a nominal thermostat but not a malf'd one.
    assertAtTemperature(nominalTooColdMalfMiddle, LoadOFF,
            "C8. Temp too low for nominal, but Malf is active, so command should be OFF.");

    /// - When the malf is de-activated, the command should return to the nominal state. Even though
    ///   the thermostat was OFF in the last timestep, the temp had previously fallen below the nominal
    ///   setpt. Therefore, even at a middle-range temperature, the thermostat should trip ON when the
    ///   malf is de-activated.
    tArticle->mMalfSetptLowFlag = false;
    assertAtTemperature(middle, LoadON,
            "C9. Temp in very middle. Malf NOT active. Command should be ON.");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the behavior of the Thermostat with setpoints malf active, biased high.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermostat::testMalfunctionSetptsHot()
{
    /// - Derive sample temperature values to test.
    double nominalTooHotMalfMiddle  = (tNominalSetptHigh + tArticle->mMalfSetptHighValue) / 2;
    double nominalMiddleMalfTooCold = ( tNominalSetptLow + tArticle->mMalfSetptLowValue ) / 2;

    /// - Set the sensor to a cold value, test that the thermostat is commanding ON when malf'd.
    tArticle->mMalfSetptHighFlag = true;
    assertAtTemperature(tooCold, LoadON,
            "H1. Temp is cold, malf is Active. Command should be ON.");

    /// - Set the sensor to a value that would command OFF a nominal article but not a malf'd one.
    assertAtTemperature(nominalTooHotMalfMiddle, LoadON,
            "H2. Temp still in malf'd mid-range, but command not ON.");

    /// - De-Activate the malf, now the same temperature should trigger a command OFF.
    tArticle->mMalfSetptHighFlag = false;
    assertAtTemperature(nominalTooHotMalfMiddle, LoadOFF,
            "H3. Temp too high when nominal, but command not OFF.");

    /// - Now, activate the malf. Even though the last malf'd command was ON, the thermostat should
    ///   only remember the nominal state, and remain OFF.
    tArticle->mMalfSetptHighFlag = true;
    assertAtTemperature(nominalTooHotMalfMiddle, LoadOFF,
            "H4. Temp unchanged, but malf activated. Command should be residual OFF.");

    /// - Set the sensor to a temp that is too cold using malf'd setpoints.
    tArticle->mMalfSetptLowFlag = true;
    assertAtTemperature(nominalMiddleMalfTooCold, LoadON,
            "H5. Temp middle when nominal, too low when malf'd. Malf active, Command should be ON.");

    /// - When the malf is de-activated, the command should resort to its previous status.
    tArticle->mMalfSetptLowFlag = false;
    assertAtTemperature(nominalMiddleMalfTooCold, LoadON,
            "H6. Temp unchanged, but low setpoint malf de-activated. Command should be ON.");

    /// - When both malf's are de-activated, the command should resort to the nominal status.
    tArticle->mMalfSetptHighFlag = false;
    assertAtTemperature(nominalMiddleMalfTooCold, LoadOFF,
            "H7. Temp unchanged, but both malf's de-activated. Command should be OFF.");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the logic specific to a multi-sensor:AND thermostat.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermostat::testMultiSensorAnd()
{
    std::cout << "\nThermostat 08: Logic test, multi-sensor:AND ......";

    /// - Configure with multi-sensor:OR settings.
    tConfig->cNumSensors = 15;
    tConfig->cLogicType = ThermostatConfigData::AND;

    /// - Initialize the test article.
    tArticle->initialize(*tConfig, *tInput, tName);

    /// - Set all sensors to a mid-range temperature.
    setAllSensors(middle);
    /// @test   The initial state should be OFF.
    tArticle->update(tTimeStep);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("AND thermostat initially at OFF",
            LoadOFF, tArticle->getCommandActual() );

    /// - Set one sensor to a cold temperature.
    setOneSensor(tooCold, 0);
    /// @test   This should not trip an 'AND' thermostat.
    tArticle->update(tTimeStep);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("AND thermostat should stay OFF when one sensor is too cold.",
            LoadOFF, tArticle->getCommandActual() );

    /// - Set all sensors to a too-cold temperature.
    setAllSensors(tooCold);
    /// @test   The AND thermostat should switch ON once ALL sensors are under the low setpt.
    tArticle->update(tTimeStep);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("AND thermostat should be ON when all sensors too cold",
            LoadON, tArticle->getCommandActual() );

    /// - Set all sensors to a mid-range temperature.
    setAllSensors(middle);
    /// @test   The AND thermostat should stay ON when sensors are in-between setpoints.
    tArticle->update(tTimeStep);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("AND thermostat should stay ON when sensors read mid-range",
            LoadON, tArticle->getCommandActual() );

    /// - Set one sensor to a hot temperature.
    setOneSensor(tooHot, tNumSensors-1);
    /// @test   The 'AND' thermostat should trip OFF once any sensor is above the high setpt.
    tArticle->update(tTimeStep);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("AND thermostat should be OFF when one sensor reads too hot.",
            LoadOFF, tArticle->getCommandActual() );

    std::cout << "... Pass";
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the logic specific to a multi-sensor:AND thermostat.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermostat::testMultiSensorOr()
{
    std::cout << "\nThermostat 09: Logic test, multi-sensor:OR .......";

    /// - Configure with multi-sensor:OR settings.
    tConfig->cNumSensors = 15;
    tConfig->cLogicType = ThermostatConfigData::OR;

    /// - Initialize the test article.
    tArticle->initialize(*tConfig, *tInput, tName);

    /// - Set all sensors to a mid-range temperature.
    setAllSensors(middle);
    /// @test   The initial state should be OFF.
    tArticle->update(tTimeStep);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("OR thermostat initially at OFF",
            LoadOFF, tArticle->getCommandActual() );

    /// - Set one sensor to a cold temperature.
    setOneSensor(tooCold, 0);
    /// @test   This should trip an OR thermostat to LoadON.
    tArticle->update(tTimeStep);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("OR thermostat should turn ON when one sensor is too cold.",
            LoadON, tArticle->getCommandActual() );

    /// - Set another sensor to a hot temperature.
    setOneSensor(tooHot, tNumSensors-1);
    /// @test   This should have no effect on an OR thermostat. It should remain OFF.
    tArticle->update(tTimeStep);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("OR thermostat has only one sensor that reads too hot.",
            LoadON, tArticle->getCommandActual() );

    /// - Set all sensors to a too-hot temperature.
    setAllSensors(tooHot);
    /// @test   The OR thermostat should switch OFF once all sensors are above the high setpt.
    tArticle->update(tTimeStep);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("OR thermostat should be OFF when all sensors too hot",
            LoadOFF, tArticle->getCommandActual() );

    std::cout << "... Pass";
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  An intermediary method that runs assertion checks for a given article at a given
///           sensor temperature. Called by different test methods. The Command On/Off malfunctions
///           are tested here so that their behavior is verified at all the different temperature /
///           setpoint combinations. As is killswitch behavior.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermostat::assertAtTemperature(const double temperature, const UserLoadMode expected,
        const char* testDescription)
{
    /// - Loop through all sensors and set the temperature.
    setAllSensors(temperature);

    /// - Engage the kill switch. The malfs should take precedence.
    tArticle->mIsKillSwitchEngaged = true;

    /// - Set malfunction command ON.
    tArticle->mMalfCmdOnFlag = true;
    tArticle->update(tTimeStep);

    /// @test    CommandActual should be ON if the malf command ON is active.
    CPPUNIT_ASSERT_MESSAGE("Malf cmd:ON active but command is not ON.",
            LoadON == tArticle->getCommandActual());

    /// - Set malfunction command OFF. This should take precedence over the command ON malf.
    tArticle->mMalfCmdOffFlag = true;
    tArticle->update(tTimeStep);

    /// @test    CommandActual should be OFF if the malf command OFF is active.
    CPPUNIT_ASSERT_MESSAGE("Malf cmd:OFF active but command is not OFF.",
            LoadOFF == tArticle->getCommandActual());

    /// - Reset the malf flags (but leave the kill switch engaged) and update.
    tArticle->mMalfCmdOffFlag = false;
    tArticle->mMalfCmdOnFlag = false;
    tArticle->update(tTimeStep);

    /// @test    CommandActual should be OFF if the kill switch is engaged.
    CPPUNIT_ASSERT_MESSAGE("Kill switch engaged but command is not OFF.",
            LoadOFF == tArticle->getCommandActual());

    /// - Engage the kill switch and update.
    tArticle->mIsKillSwitchEngaged = false;
    tArticle->update(tTimeStep);

    /// @test    Thermostat should be commanding at the expected value.
    CPPUNIT_ASSERT_EQUAL_MESSAGE( testDescription, expected, tArticle->getCommandActual() );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  temperature  (K)  temperature to set on the sensor
///
/// @details    An intermediary method that sets all sensors to a given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermostat::setAllSensors(const double temperature)
{
    /// - Loop through all sensors.
    for (int i = 0; i < tArticle->mNumSensors; ++i)
    {
        /// - Set each sensor temperature.
        setOneSensor(temperature, i);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  temperature  (K)   temperature to set on the sensor
/// @param[in]  index        (--)  index of the specific sensor within the mTempSensor array
///
/// @details    An intermediary method that sets one sensor to a given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermostat::setOneSensor(const double temperature, const int index)
{
    /// - Set specific sensor temperature.
    tArticle->mTempSensor[index].setTruthInput(temperature);
}

