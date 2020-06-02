/****************************** TRICK HEADER ******************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
 (
    (../TsOpticSmokeDetector.o)
    (common/sensors/TsNoise.o)
 )

 PROGRAMMERS:
 (
  ((Jose A. Perez) (L3-Comm) (Mar 23, 2012) (TS21) (initial))
 )
 **************************************************************************************************/

#include "UtTsOpticSmokeDetector.hh"
#include "common/sensors/TsNoise.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default class constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsOpticSmokeDetector::UtTsOpticSmokeDetector()
   :
   testArticle (),
   configData  (),
   inputData   (),
   test        (0),
   testC       (0),
   testI       (0),
   dt          (0.0),
   sdName      ()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsOpticSmokeDetector::~UtTsOpticSmokeDetector()
{
    // nothing to do
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpticSmokeDetector::setUp()
{
    dt                               = 0.1;
    sdName                           = "Smoke Detector Test";

    configData.obsMaxPercentage      = 104.12; // Obs.% JMEWS Data. Ref. D684-10508-02-02: Sec. 3.3 for ISS SD.
    configData.obsMinPercentage      = 0.0;    // Obs.% JMEWS Data. Ref. D684-10508-02-02: Sec. 3.3 for ISS SD.
    configData.obsSmokeContributer   = 20.0;   //Tunes the obscuration smoke contribution according to JMEWS.
    configData.scatterMaxPercentage  = 2.47;   // %/ft. JMEWS Data. Ref. D684-10508-02-02: Sec. 3.3 for ISS SD.
    configData.scatterMinPercentage  = 0.11;   // %/ft. JMEWS Data. Ref. D684-10508-02-02: Sec. 3.3 for ISS SD.
    configData.bitOnMaxScatterValue  = 1.42;   // %/ft. JMEWS Data. Ref. SSP 41002 Table 3.3.4.1.1.3-1 for ISS SD.
    configData.nominalObscuration    = 8.24;   // Obs.% JMEWS Data. Ref. D684-10508-02-02: Sec. 3.3 for ISS SD.
    configData.nominalScatter        = 0.16;   // %/ft. JMEWS Data. Ref. D684-10508-02-02: Sec. 3.3 for ISS SD.
    configData.bitRecoveryTime       = 1.5;    // sec.  D684-10508-02-02: Sec. 3.3 for ISS SD: "Quite Period".

    inputData.isPowered              = false;
    inputData.isLoadOn               = 1;
    inputData.isBitEnabled           = false;
    inputData.smokeLevelFactor       = 0.0;
    inputData.bitTimer               = 0.0;

    //obscuration sensor
    configData.obsSensorConfigD.mMinRange              = 0.0;    // Obs.%
    configData.obsSensorConfigD.mMaxRange              = 104.15; // Obs.%
    configData.obsSensorConfigD.mNominalNoiseScale     = 0.5;
    configData.obsSensorConfigD.mNoiseFunction         = TsNoise::getNoise;

    //scatter sensor
    configData.scatterSensorConfigD.mMinRange          = 0.0;    // %/ft
    configData.scatterSensorConfigD.mMaxRange          = 2.47;   // %/ft
    configData.scatterSensorConfigD.mNominalNoiseScale = 0.05;
    configData.scatterSensorConfigD.mNoiseFunction     = TsNoise::getNoise;

    test  = new TsOpticSmokeDetector();
    testC = new TsOpticSmokeDetectorConfigData();
    testI = new TsOpticSmokeDetectorInputData();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpticSmokeDetector::tearDown()
{
    delete test;
    delete testC;
    delete testI;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructor test.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpticSmokeDetector::DefaultConstructorTest()
{
    std::cout << "\n-------------------------------------------------------------------------------";
    std::cout << "\n Optic Smoke Detector: Default Constructor Test ";

    CPPUNIT_ASSERT_EQUAL(false, testArticle.isPowered);
    CPPUNIT_ASSERT_EQUAL(0,  testArticle.isLoadOn);
    CPPUNIT_ASSERT_EQUAL(false, testArticle.mInitialized);
    CPPUNIT_ASSERT_EQUAL(false, testArticle.isBitEnabled);
    CPPUNIT_ASSERT_EQUAL(false, testArticle.isBitInProcess);
    CPPUNIT_ASSERT_EQUAL(false, testArticle.malfTotalFail);
    CPPUNIT_ASSERT_EQUAL(false, testArticle.malfPowerToOn);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, testArticle.smokeLevelFactor, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, testArticle.obscurationPercentage, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, testArticle.scatterPercentage, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, testArticle.bitTimer, 0.01);
    CPPUNIT_ASSERT("" == testArticle.mName);

    std::cout << "\t... Pass";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test Initialize method.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpticSmokeDetector::InitializeMethodTest()
{
    std::cout << "\n Optic Smoke Detector: Initialize Method Test ";

    testArticle.initialize(configData, inputData, sdName);

    CPPUNIT_ASSERT_EQUAL(false, testArticle.isPowered);
    CPPUNIT_ASSERT_EQUAL(1,  testArticle.isLoadOn);
    CPPUNIT_ASSERT_EQUAL(false, testArticle.isBitEnabled);
    CPPUNIT_ASSERT_EQUAL(true,  testArticle.mInitialized);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,   testArticle.smokeLevelFactor, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,   testArticle.bitTimer, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(configData.obsMaxPercentage, testArticle.configD.obsMaxPercentage, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(configData.obsMinPercentage, testArticle.configD.obsMinPercentage, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(configData.obsSmokeContributer,  testArticle.configD.obsSmokeContributer, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(configData.scatterMaxPercentage, testArticle.configD.scatterMaxPercentage, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(configData.scatterMinPercentage, testArticle.configD.scatterMinPercentage, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(configData.bitOnMaxScatterValue, testArticle.configD.bitOnMaxScatterValue, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(configData.nominalObscuration,   testArticle.configD.nominalObscuration, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(configData.nominalScatter,  testArticle.configD.nominalScatter, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(configData.bitRecoveryTime, testArticle.configD.bitRecoveryTime, 0.01);

    std::cout << "\t... Pass";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Verify Exception is thrown when no smoke detector name was specified.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpticSmokeDetector::ConfigExceptionThrownNoNameSpecifiedTest()
{
    std::cout << "\n Optic Smoke Detector: Config Exception Thrown when no name was specified.";

    try {
//        testArticle.initialize(configData, inputData, "");
        CPPUNIT_ASSERT_THROW(testArticle.initialize(configData, inputData, ""), TsInitializationException);
        }
    catch (TsInitializationException &ne)
    {
        //Verify is the correct configuration exception.
        CPPUNIT_ASSERT_EQUAL(std::string(
                "No smoke detector name was specified."), ne.getCause());
    }

    std::cout << "\t... Pass";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Verify Exception is thrown when not configuring min and max obscuration values properly.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpticSmokeDetector::ConfigExceptionThrownMinObscurationGreaterThanMaxTest()
{
    std::cout << "\n Optic Smoke Detector: Config Exception Thrown when min obscuration is greater than max obscuration.";

    configData.obsMaxPercentage      = 50.0;
    configData.obsMinPercentage      = 51.0;
    try {
        testArticle.initialize(configData, inputData, sdName);
        CPPUNIT_ASSERT_THROW(testArticle.initialize(configData, inputData, sdName), TsInitializationException);
        }
    catch (TsInitializationException &ne)
    {
        //Verify is the correct configuration exception.
        CPPUNIT_ASSERT_EQUAL(std::string(
            ": obsMinPercentage input must be less than obsMaxPercentage input."), ne.getCause());
    }

    std::cout << "\t... Pass";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Verify Exception is thrown when not configuring min and max scatter values properly.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpticSmokeDetector::ConfigExceptionThrownMinScatterGreaterThanMaxTest()
{
    std::cout << "\n Optic Smoke Detector: Config Exception Thrown when min scatter is greater than max scatter.";

    configData.scatterMaxPercentage      = 50.0;
    configData.scatterMinPercentage      = 51.0;
    try {
        testArticle.initialize(configData, inputData, sdName);
        CPPUNIT_ASSERT_THROW(testArticle.initialize(configData, inputData, sdName), TsInitializationException);
        }
    catch (TsInitializationException &ne)
    {
        //Verify is the correct configuration exception.
        CPPUNIT_ASSERT_EQUAL(std::string(
            ": scatterMinPercentage input must be less than scatterMaxPercentage input."), ne.getCause());
    }

    std::cout << "\t... Pass";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Verify Exception is thrown when not configuring min and nominal obscuration properly.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpticSmokeDetector::ConfigExceptionThrownMinObscurationGreaterThanNominalTest()
{
    std::cout << "\n Optic Smoke Detector: Config Exception Thrown when min obscuration is greater than nominal obscuration.";

    configData.nominalObscuration    = 50.0;
    configData.obsMinPercentage      = 51.0;
    try {
        testArticle.initialize(configData, inputData, sdName);
        CPPUNIT_ASSERT_THROW(testArticle.initialize(configData, inputData, sdName), TsInitializationException);
        }
    catch (TsInitializationException &ne)
    {
        //Verify is the correct configuration exception.
        CPPUNIT_ASSERT_EQUAL(std::string(
            ": nominalObscuration input must be greater than obsMinPercentage input."), ne.getCause());
    }

    std::cout << "\t... Pass";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Verify Exception is thrown when not configuring min and nominal scatter properly.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpticSmokeDetector::ConfigExceptionThrownMinScatterGreaterThanNominalTest()
{
    std::cout << "\n Optic Smoke Detector: Config Exception Thrown when min scatter is greater than nominal scatter.";

    configData.nominalScatter            = 50.0;
    configData.scatterMinPercentage      = 51.0;
    configData.scatterMaxPercentage      = 55.0;
    try {
        testArticle.initialize(configData, inputData, sdName);
        CPPUNIT_ASSERT_THROW(testArticle.initialize(configData, inputData, sdName), TsInitializationException);
        }
    catch (TsInitializationException &ne)
    {
        //Verify is the correct configuration exception.
        CPPUNIT_ASSERT_EQUAL(std::string(
             ": nominalScatter input must be greater than scatterMinPercentage input."), ne.getCause());
    }

    std::cout << "\t... Pass";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Verify Exception is thrown when not configuring bitOnMaxScatterValue properly.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpticSmokeDetector::ConfigExceptionThrownBitOnMaxScatterValueNotCorrectValueTest()
{
    std::cout << "\n Optic Smoke Detector: Config Exception Thrown when bit on scatter value is entered incorrectly.";

    configData.scatterMaxPercentage      = 70.0;
    configData.nominalScatter            = 65.0;
    configData.bitOnMaxScatterValue      = 55.0;
    configData.scatterMinPercentage      = 60.0;

    try {
        testArticle.initialize(configData, inputData, sdName);
        CPPUNIT_ASSERT_THROW(testArticle.initialize(configData, inputData, sdName), TsInitializationException);
        }
    catch (TsInitializationException &ne)
    {
        //Verify is the correct configuration exception.
        CPPUNIT_ASSERT_EQUAL(std::string(
            ": bitOnMaxScatterValue input must be greater than scatterMinPercentage input."), ne.getCause());
    }

    std::cout << "\t... Pass";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Verify Exception is thrown when bit recovery time is less than tolerance.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpticSmokeDetector::ConfigExceptionThrownBitRecoveryTimeLessThanTolTest()
{
    std::cout << "\n Optic Smoke Detector: Config Exception Thrown when bit recovery time is less than tolerance.";

    configData.bitRecoveryTime       = -1.0;

    try {
        testArticle.initialize(configData, inputData, sdName);
        CPPUNIT_ASSERT_THROW(testArticle.initialize(configData, inputData, sdName), TsInitializationException);
        }
    catch (TsInitializationException &ne)
    {
        //Verify is the correct configuration exception.
        CPPUNIT_ASSERT_EQUAL(std::string(
            ": bitRecoveryTime input value cannot be negative, zero or be close to zero."), ne.getCause());
    }

    std::cout << "\t... Pass";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Verify Exception is thrown when smoke level's value is not entered correctly.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpticSmokeDetector::ConfigExceptionThrownSmokeLevelOutOfBoundsTest()
{
    std::cout << "\n Optic Smoke Detector: Config Exception Thrown when smoke level's value is not entered correctly.";

    inputData.smokeLevelFactor = -1.0;

    try {
        testArticle.initialize(configData, inputData, sdName);
        CPPUNIT_ASSERT_THROW(testArticle.initialize(configData, inputData, sdName), TsInitializationException);
        }
    catch (TsInitializationException &ne)
    {
        //Verify is the correct configuration exception.
        CPPUNIT_ASSERT_EQUAL(std::string(
            ": smokeLevelFactor input value can only be from 0.0 to 1.0."), ne.getCause());
    }

    inputData.smokeLevelFactor = 2.0;

    try {
        testArticle.initialize(configData, inputData, sdName);
        CPPUNIT_ASSERT_THROW(testArticle.initialize(configData, inputData, sdName), TsInitializationException);
        }
    catch (TsInitializationException &ne)
    {
        //Verify is the correct configuration exception.
        CPPUNIT_ASSERT_EQUAL(std::string(
            ": smokeLevelFactor input value can only be from 0.0 to 1.0."), ne.getCause());
    }

    std::cout << "\t... Pass";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Verify Exception is thrown when bit Timer's value is not entered correctly.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpticSmokeDetector::ConfigExceptionThrownBitTimerOutOfBoundsTest()
{
    std::cout << "\n Optic Smoke Detector: Config Exception Thrown when bit Timer's value is not entered correctly.";

    inputData.bitTimer = -1.0;

    try {
        testArticle.initialize(configData, inputData, sdName);
        CPPUNIT_ASSERT_THROW(testArticle.initialize(configData, inputData, sdName), TsInitializationException);
        }
    catch (TsInitializationException &ne)
    {
        //Verify is the correct configuration exception.
        CPPUNIT_ASSERT_EQUAL(std::string(
            ": bitTimer input value cannot be negative."), ne.getCause());
    }

    std::cout << "\t... Pass";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Verify Exception is thrown when calling update prior calling initialize.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpticSmokeDetector::InitExceptionThrownCallingUpdatePriorInitTest()
{
    std::cout << "\n Optic Smoke Detector: Init Exception Thrown when calling update prior calling initialize.";

    /// - Test deleted as the calling class is expected to check for object initialization.

    std::cout << "\t... Pass";
}


///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Run update at nominal conditions, no malfunctions. Verify outputs.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpticSmokeDetector::RunUpdateNominallyTest()
{
    std::cout << "\n Optic Smoke Detector: Verify outputs with nominal conditions.";

    inputData.isPowered = true;

    testArticle.initialize(configData, inputData, sdName);

    testArticle.update(dt);

    float obscurationValue = configData.nominalObscuration;
    float scatterValue     = configData.nominalScatter;
    float obsVoltValue     = obscurationValue * configData.obsPercentVoltSlope + configData.obsVoltIntercept;
    float scatVoltValue    = scatterValue * configData.scatPercentVoltSlope + configData.scatVoltIntercept;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(obscurationValue, testArticle.obscurationPercentage, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(scatterValue,     testArticle.scatterPercentage, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(obscurationValue, testArticle.getSensedObscurationValue(), 1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(scatterValue,     testArticle.getSensedScatterValue(), 0.05);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(obsVoltValue,     testArticle.obsSensedVoltage, 0.05);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(scatVoltValue,    testArticle.scatSensedVoltage, 0.05);

    std::cout << "\t... Pass";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Run update with Smoke and nominal conditions, no malfunctions. Verify outputs.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpticSmokeDetector::RunUpdateNominallyWithSmokeTest()
{
    std::cout << "\n Optic Smoke Detector: Verify outputs with nominal conditions and smoke added in to the system.";

    inputData.isPowered = true;
    inputData.smokeLevelFactor = 0.9;

    testArticle.initialize(configData, inputData, sdName);

    testArticle.update(dt);

    float obscurationValue = configData.nominalObscuration + (configData.obsSmokeContributer * inputData.smokeLevelFactor);
    float scatterValue     = configData.nominalScatter + (configData.scatterMaxPercentage - configData.nominalScatter) * inputData.smokeLevelFactor;
    float obsVoltValue     = obscurationValue * configData.obsPercentVoltSlope + configData.obsVoltIntercept;
    float scatVoltValue    = scatterValue * configData.scatPercentVoltSlope + configData.scatVoltIntercept;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(obscurationValue, testArticle.obscurationPercentage, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(scatterValue, testArticle.scatterPercentage, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(obscurationValue, testArticle.getSensedObscurationValue(), 1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(scatterValue, testArticle.getSensedScatterValue(), 0.05);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(obsVoltValue,     testArticle.obsSensedVoltage, 0.05);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(scatVoltValue,    testArticle.scatSensedVoltage, 0.05);

    std::cout << "\t... Pass";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Run update at with power malfunction conditions. Verify outputs.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpticSmokeDetector::RunUpdateWithPowerMalfunctionsTest()
{
    std::cout << "\n Optic Smoke Detector: Verify outputs with power malfunction conditions.";

    inputData.isPowered = false;

    testArticle.initialize(configData, inputData, sdName);

    //input malfunctions
    testArticle.malfPowerToOn = true;
    testArticle.malfTotalFail = false;

    testArticle.update(dt);

    float obscurationValue = configData.nominalObscuration;
    float scatterValue     = configData.nominalScatter;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(obscurationValue, testArticle.obscurationPercentage, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(scatterValue, testArticle.scatterPercentage, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(obscurationValue, testArticle.getSensedObscurationValue(), 1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(scatterValue, testArticle.getSensedScatterValue(), 0.05);
    CPPUNIT_ASSERT_EQUAL(1,  testArticle.isLoadOn);

    std::cout << "\t... Pass";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Run update with no power available. Verify outputs.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpticSmokeDetector::RunUpdateWithNoPowerTest()
{
    std::cout << "\n Optic Smoke Detector: Verify outputs when power is not available.";

    inputData.isPowered       = false;
    testArticle.malfPowerToOn = false;
    testArticle.malfTotalFail = false;
    configData.obsOffValue    = 53.45;
    configData.scatOffValue   = 0.1006;
    float obsVoltValue        = configData.obsOffValue * configData.obsPercentVoltSlope + configData.obsVoltIntercept;
    float scatVoltValue       = configData.scatOffValue * configData.scatPercentVoltSlope + configData.scatVoltIntercept;

    testArticle.initialize(configData, inputData, sdName);

    testArticle.update(dt);

    CPPUNIT_ASSERT_EQUAL(0,              testArticle.isLoadOn);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(configData.obsOffValue,  testArticle.obscurationPercentage, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(configData.scatOffValue, testArticle.scatterPercentage, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(configData.obsOffValue,  testArticle.getSensedObscurationValue(), 0.05);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(configData.scatOffValue, testArticle.getSensedScatterValue(), 0.05);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(obsVoltValue,     testArticle.obsSensedVoltage, 0.05);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(scatVoltValue,    testArticle.scatSensedVoltage, 0.05);

    std::cout << "\t... Pass";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Run update with smoke detector fail-as-a-whole command is set. Verify outputs.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpticSmokeDetector::RunUpdateWithFailCommandTest()
{
    std::cout << "\n Optic Smoke Detector: Verify outputs when smoke detector has failed as a whole.";

    inputData.isPowered = true;

    //Malf
    testArticle.malfTotalFail = true;
    testArticle.malfPowerToOn = false;

    testArticle.initialize(configData, inputData, sdName);

    testArticle.update(dt);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(53.45,   testArticle.obscurationPercentage, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1006,  testArticle.scatterPercentage, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(53.45,   testArticle.getSensedObscurationValue(), 0.05);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.10060, testArticle.getSensedScatterValue(), 0.05);

    std::cout << "\t... Pass";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Run update with obscuration sensor failed. Verify outputs.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpticSmokeDetector::RunUpdateWithFailedObscurationSensorTest()
{
    std::cout << "\n Optic Smoke Detector: Verify outputs when obscuration sensor has failed.";

    inputData.isPowered = true;
    //Malf
    inputData.obsSensorInputD.mMalfFailToValue = 0.0;
    inputData.obsSensorInputD.mMalfScaleFlag = true;

    testArticle.initialize(configData, inputData, sdName);

    testArticle.update(dt);

    float obscurationValue = configData.nominalObscuration;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(obscurationValue, testArticle.obscurationPercentage, 0.05);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, testArticle.getSensedObscurationValue(), 1.0);

    std::cout << "\t... Pass";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Run update with scatter sensor failed. Verify outputs.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpticSmokeDetector::RunUpdateWithFailedScatterSensorTest()
{
    std::cout << "\n Optic Smoke Detector: Verify outputs when scatter sensor has failed.";

    inputData.isPowered = true;
    //Malf
    inputData.scatterSensorInputD.mMalfFailToValue = 0.0;
    inputData.scatterSensorInputD.mMalfScaleFlag = true;

    testArticle.initialize(configData, inputData, sdName);

    testArticle.update(dt);

    float scatterValue     = configData.nominalScatter;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(scatterValue, testArticle.scatterPercentage, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, testArticle.getSensedScatterValue(), 0.05);

    std::cout << "\t... Pass";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Perform Active bit Test Sequence and verify outputs.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpticSmokeDetector::RunActiveBitTest()
{
    std::cout << "\n Optic Smoke Detector: Perform active bit test and verify outputs.";

    inputData.isLoadOn                  = 1;
    inputData.isPowered                 = true;
    inputData.isBitEnabled              = true;
    testArticle.malfTotalFail           = false;
    testArticle.malfPowerToOn           = false;

    testArticle.initialize(configData, inputData, sdName);

    for(int ii = 0; ii < 5; ii++)
    {
     testArticle.update(dt);
    }

    float obscurationValue = configData.obsMaxPercentage;
    float scatterValue     = configData.bitOnMaxScatterValue;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(obscurationValue, testArticle.obscurationPercentage, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(scatterValue, testArticle.scatterPercentage, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(obscurationValue, testArticle.getSensedObscurationValue(), 1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(scatterValue, testArticle.getSensedScatterValue(), 0.05);

    testArticle.isBitEnabled = false;
    for(int ii = 0; ii < 16; ii++)
    {
     testArticle.update(dt);
    }

    obscurationValue = configData.obsMaxPercentage;
    scatterValue     = configData.scatterMinPercentage;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(obscurationValue, testArticle.obscurationPercentage, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(scatterValue, testArticle.scatterPercentage, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(obscurationValue, testArticle.getSensedObscurationValue(), 1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(scatterValue, testArticle.getSensedScatterValue(), 0.06);

    testArticle.update(dt); //After bitTimer exceeds bit recovery time, sensor has finished active bit test.

    obscurationValue = configData.nominalObscuration;
    scatterValue     = configData.nominalScatter;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(obscurationValue, testArticle.obscurationPercentage, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(scatterValue, testArticle.scatterPercentage, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(obscurationValue, testArticle.getSensedObscurationValue(), 1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(scatterValue, testArticle.getSensedScatterValue(), 0.06);

    std::cout << "\t... Pass";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Perform Active bit Test Sequence with Smoke present and verify outputs.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpticSmokeDetector::RunActiveBitWithSmokeTest()
{
    std::cout << "\n Optic Smoke Detector: Perform active bit test with smoke present and verify outputs.";

    inputData.isPowered                 = true;
    inputData.isBitEnabled              = true;
    inputData.smokeLevelFactor          = 0.9;
    testArticle.malfTotalFail           = false;
    testArticle.malfPowerToOn           = false;

    testArticle.initialize(configData, inputData, sdName);

    for(int ii = 0; ii < 5; ii++)
    {
     testArticle.update(dt);
    }

    float obscurationValue = configData.obsMaxPercentage;
    float scatterValue     = configData.bitOnMaxScatterValue;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(obscurationValue, testArticle.obscurationPercentage, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(scatterValue, testArticle.scatterPercentage, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(obscurationValue, testArticle.getSensedObscurationValue(), 1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(scatterValue, testArticle.getSensedScatterValue(), 0.05);

    testArticle.isBitEnabled = false;
    for(int ii = 0; ii < 16; ii++)
    {
     testArticle.update(dt);
    }

    obscurationValue = configData.obsMaxPercentage;
    scatterValue     = configData.scatterMinPercentage;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(obscurationValue, testArticle.obscurationPercentage, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(scatterValue, testArticle.scatterPercentage, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(obscurationValue, testArticle.getSensedObscurationValue(), 1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(scatterValue, testArticle.getSensedScatterValue(), 0.05);

    testArticle.update(dt); //After bitTimer exceeds bit recovery time, sensor has finished active bit test, goes back to normal.

    obscurationValue = configData.nominalObscuration + (configData.obsSmokeContributer * inputData.smokeLevelFactor);
    scatterValue     = configData.nominalScatter + (configData.scatterMaxPercentage - configData.nominalScatter) * inputData.smokeLevelFactor;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(obscurationValue, testArticle.obscurationPercentage, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(scatterValue, testArticle.scatterPercentage, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(obscurationValue, testArticle.getSensedObscurationValue(), 1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(scatterValue, testArticle.getSensedScatterValue(), 0.05);

    std::cout << "\t... Pass";
}

