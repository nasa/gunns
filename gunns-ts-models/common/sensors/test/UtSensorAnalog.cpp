/****************************** TRICK HEADER ******************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
 ((common/sensors/SensorAnalog.o))

PROGRAMMERS:
 ((Jason Harvey) (L-3 Communications) (2012-04) (TS21) (Initial))

 **************************************************************************************************/

#include <cmath>
#include "UtSensorAnalog.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "common/sensors/TsNoise.hh"
#include "math/UnitConversion.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
UtSensorAnalog::UtSensorAnalog()
    :
    tName(),
    tMinRange(),
    tMaxRange(),
    tOffValue(),
    tNominalBias(),
    tNominalScale(),
    tNominalNoiseScale(),
    tNominalNoiseMult(),
    tNominalResolution(),
    tNominalNoiseFunction(),
    tUnitConversion(),
    tNominalConfig(),
    tPowerFlag(),
    tTruthInput(),
    tNominalInput(),
    tTimeStep()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
UtSensorAnalog::~UtSensorAnalog()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  unit test infrastructure
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorAnalog::setUp()
{
    /// - Define nominal config data.
    tName                 = "Test Sensor";
    tMinRange             =  1.0;
    tMaxRange             = 49.0;
    tOffValue             =  5.0;
    tNominalBias          =  0.1;
    tNominalScale         =  0.99;
    tNominalNoiseScale    =  1.0;
    tNominalNoiseMult     =  0.01;
    tNominalResolution    =  0.2;
    tNominalNoiseFunction = UtSensorAnalogNoise::testNoise;
    tUnitConversion       = UnitConversion::NO_CONVERSION;
    tNominalConfig = new SensorAnalogConfigData(tMinRange,
                                                tMaxRange,
                                                tOffValue,
                                                tNominalBias,
                                                tNominalScale,
                                                tNominalNoiseScale,
                                                tNominalResolution,
                                                tNominalNoiseFunction,
                                                tUnitConversion,
                                                tNominalNoiseMult);

    /// - Define nominal input data.
    tPowerFlag  = true;
    tTruthInput = 10.0;
    tNominalInput = new SensorAnalogInputData(tPowerFlag, tTruthInput);

    /// - Set other test data.
    tTimeStep = 0.1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  unit test infrastructure
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorAnalog::tearDown()
{
    delete tNominalInput;
    delete tNominalConfig;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the constructors & destructors of the Analog Sensor config data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorAnalog::testConfigData()
{
    std::cout << "\n--------------------------------------------------------------------------------";
    std::cout << "\n UtSensorAnalog ....... 01: testConfigData.............................";

    /// - Test default construction of a test config data article.
    SensorAnalogConfigData defaultConfig;
    CPPUNIT_ASSERT(0.0                           == defaultConfig.mMinRange);
    CPPUNIT_ASSERT(0.0                           == defaultConfig.mMaxRange);
    CPPUNIT_ASSERT(0.0                           == defaultConfig.mOffValue);
    CPPUNIT_ASSERT(0.0                           == defaultConfig.mNominalBias);
    CPPUNIT_ASSERT(1.0                           == defaultConfig.mNominalScale);
    CPPUNIT_ASSERT(0.0                           == defaultConfig.mNominalNoiseScale);
    CPPUNIT_ASSERT(0.0                           == defaultConfig.mNominalResolution);
    CPPUNIT_ASSERT(0                             == defaultConfig.mNoiseFunction);
    CPPUNIT_ASSERT(UnitConversion::NO_CONVERSION == defaultConfig.mUnitConversion);

    /// - Test nominal construction of a test config data article.
    tUnitConversion       = UnitConversion::K_TO_F;
    SensorAnalogConfigData nominalConfig(tMinRange, tMaxRange, tOffValue, tNominalBias,
                                         tNominalScale, tNominalNoiseScale, tNominalResolution,
                                         tNominalNoiseFunction, tUnitConversion);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMinRange,          nominalConfig.mMinRange,          FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMaxRange,          nominalConfig.mMaxRange,          FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tOffValue,          nominalConfig.mOffValue,          FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNominalBias,       nominalConfig.mNominalBias,       FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNominalScale,      nominalConfig.mNominalScale,      FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNominalNoiseScale, nominalConfig.mNominalNoiseScale, FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNominalResolution, nominalConfig.mNominalResolution, FLT_EPSILON);
    CPPUNIT_ASSERT(tNominalNoiseFunction == nominalConfig.mNoiseFunction);
    CPPUNIT_ASSERT(tUnitConversion       == nominalConfig.mUnitConversion);

    /// - Test copy construction of a test config data article.
    SensorAnalogConfigData copyConfig(nominalConfig);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMinRange,          copyConfig.mMinRange,          FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMaxRange,          copyConfig.mMaxRange,          FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tOffValue,          copyConfig.mOffValue,          FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNominalBias,       copyConfig.mNominalBias,       FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNominalScale,      copyConfig.mNominalScale,      FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNominalNoiseScale, copyConfig.mNominalNoiseScale, FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNominalResolution, copyConfig.mNominalResolution, FLT_EPSILON);
    CPPUNIT_ASSERT(tNominalNoiseFunction == copyConfig.mNoiseFunction);
    CPPUNIT_ASSERT(tUnitConversion       == copyConfig.mUnitConversion);

    /// - Test assignment of a test config data article.
    defaultConfig = nominalConfig;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMinRange,          defaultConfig.mMinRange,          FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMaxRange,          defaultConfig.mMaxRange,          FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tOffValue,          defaultConfig.mOffValue,          FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNominalBias,       defaultConfig.mNominalBias,       FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNominalScale,      defaultConfig.mNominalScale,      FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNominalNoiseScale, defaultConfig.mNominalNoiseScale, FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNominalResolution, defaultConfig.mNominalResolution, FLT_EPSILON);
    CPPUNIT_ASSERT(tNominalNoiseFunction == defaultConfig.mNoiseFunction);
    CPPUNIT_ASSERT(tUnitConversion       == defaultConfig.mUnitConversion);

    /// - Test self-assignment of a test config data article.
    CPPUNIT_ASSERT(&defaultConfig == &(defaultConfig = defaultConfig));

    /// - Test setNoiseFunction.
    typedef double (*f_ptr)();
    f_ptr ptr = TsNoise::getNoiseFunction();
    defaultConfig.setNoiseFunction(ptr);
    CPPUNIT_ASSERT(TsNoise::getNoise == defaultConfig.mNoiseFunction);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the constructors & destructors of the Analog Sensor input data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorAnalog::testInputData()
{
    std::cout << "\n UtSensorAnalog ....... 02: testInputData..............................";

    /// - Test default construction of a test input data article.
    SensorAnalogInputData defaultInput;
    CPPUNIT_ASSERT(false == defaultInput.mPowerFlag);
    CPPUNIT_ASSERT(false == defaultInput.mMalfFailToFlag);
    CPPUNIT_ASSERT(false == defaultInput.mMalfFailOffFlag);
    CPPUNIT_ASSERT(false == defaultInput.mMalfFailStuckFlag);
    CPPUNIT_ASSERT(false == defaultInput.mMalfIgnorePower);
    CPPUNIT_ASSERT(false == defaultInput.mMalfPerfectSensor);
    CPPUNIT_ASSERT(0.0   == defaultInput.mTruthInput);
    CPPUNIT_ASSERT(0.0   == defaultInput.mMalfFailToValue);
    CPPUNIT_ASSERT(false == defaultInput.mMalfScaleFlag);
    CPPUNIT_ASSERT(0.0   == defaultInput.mMalfScaleValue);
    CPPUNIT_ASSERT(false == defaultInput.mMalfBiasFlag);
    CPPUNIT_ASSERT(0.0   == defaultInput.mMalfBiasValue);
    CPPUNIT_ASSERT(false == defaultInput.mMalfDriftFlag);
    CPPUNIT_ASSERT(0.0   == defaultInput.mMalfDriftRate);
    CPPUNIT_ASSERT(false == defaultInput.mMalfNoiseFlag);
    CPPUNIT_ASSERT(0.0   == defaultInput.mMalfNoiseScale);
    CPPUNIT_ASSERT(false == defaultInput.mMalfResolutionFlag);
    CPPUNIT_ASSERT(0.0   == defaultInput.mMalfResolutionValue);
    CPPUNIT_ASSERT(0.0   == defaultInput.mDrift);

    /// - Test nominal construction of a test input data article.
    SensorAnalogInputData nominalInput(true, 10.0);
    CPPUNIT_ASSERT(true  == nominalInput.mPowerFlag);
    CPPUNIT_ASSERT(false  == nominalInput.mMalfFailToFlag);
    CPPUNIT_ASSERT(false  == nominalInput.mMalfFailOffFlag);
    CPPUNIT_ASSERT(false  == nominalInput.mMalfFailStuckFlag);
    CPPUNIT_ASSERT(false  == nominalInput.mMalfIgnorePower);
    CPPUNIT_ASSERT(false  == nominalInput.mMalfPerfectSensor);
    CPPUNIT_ASSERT(false  == nominalInput.mMalfScaleFlag);
    CPPUNIT_ASSERT(false  == nominalInput.mMalfBiasFlag);
    CPPUNIT_ASSERT(false  == nominalInput.mMalfDriftFlag);
    CPPUNIT_ASSERT(false  == nominalInput.mMalfNoiseFlag);
    CPPUNIT_ASSERT(false  == nominalInput.mMalfResolutionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(10.0, nominalInput.mTruthInput,          FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  nominalInput.mMalfFailToValue,     FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  nominalInput.mMalfScaleValue,      FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  nominalInput.mMalfBiasValue,       FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  nominalInput.mMalfDriftRate,       FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  nominalInput.mMalfNoiseScale,      FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  nominalInput.mMalfResolutionValue, FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  nominalInput.mDrift,               FLT_EPSILON);

    /// - Test copy construction of a test config data article.
    nominalInput.mMalfFailToFlag      = true;
    nominalInput.mMalfFailOffFlag     = true;
    nominalInput.mMalfFailStuckFlag   = true;
    nominalInput.mMalfIgnorePower     = true;
    nominalInput.mMalfPerfectSensor   = true;
    nominalInput.mMalfScaleFlag       = true;
    nominalInput.mMalfBiasFlag        = true;
    nominalInput.mMalfDriftFlag       = true;
    nominalInput.mMalfNoiseFlag       = true;
    nominalInput.mMalfResolutionFlag  = true;
    nominalInput.mMalfFailToValue     = 9.0;
    nominalInput.mMalfScaleValue      = 8.0;
    nominalInput.mMalfBiasValue       = 7.0;
    nominalInput.mMalfDriftRate       = 6.0;
    nominalInput.mMalfNoiseScale      = 5.0;
    nominalInput.mMalfResolutionValue = 4.0;
    nominalInput.mDrift               = 3.0;
    SensorAnalogInputData copyInput(nominalInput);
    CPPUNIT_ASSERT(true  == copyInput.mPowerFlag);
    CPPUNIT_ASSERT(true  == copyInput.mMalfFailToFlag);
    CPPUNIT_ASSERT(true  == copyInput.mMalfFailOffFlag);
    CPPUNIT_ASSERT(true  == copyInput.mMalfFailStuckFlag);
    CPPUNIT_ASSERT(true  == copyInput.mMalfIgnorePower);
    CPPUNIT_ASSERT(true  == copyInput.mMalfPerfectSensor);
    CPPUNIT_ASSERT(true  == copyInput.mMalfScaleFlag);
    CPPUNIT_ASSERT(true  == copyInput.mMalfBiasFlag);
    CPPUNIT_ASSERT(true  == copyInput.mMalfDriftFlag);
    CPPUNIT_ASSERT(true  == copyInput.mMalfNoiseFlag);
    CPPUNIT_ASSERT(true  == copyInput.mMalfResolutionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(10.0, copyInput.mTruthInput,          FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.0,  copyInput.mMalfFailToValue,     FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(8.0,  copyInput.mMalfScaleValue,      FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0,  copyInput.mMalfBiasValue,       FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.0,  copyInput.mMalfDriftRate,       FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0,  copyInput.mMalfNoiseScale,      FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0,  copyInput.mMalfResolutionValue, FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0,  copyInput.mDrift,               FLT_EPSILON);

    /// - Test assignment of a test input data article.
    defaultInput = nominalInput;
    CPPUNIT_ASSERT(true  == defaultInput.mPowerFlag);
    CPPUNIT_ASSERT(true  == defaultInput.mMalfFailToFlag);
    CPPUNIT_ASSERT(true  == defaultInput.mMalfFailOffFlag);
    CPPUNIT_ASSERT(true  == defaultInput.mMalfFailStuckFlag);
    CPPUNIT_ASSERT(true  == defaultInput.mMalfIgnorePower);
    CPPUNIT_ASSERT(true  == defaultInput.mMalfPerfectSensor);
    CPPUNIT_ASSERT(true  == defaultInput.mMalfScaleFlag);
    CPPUNIT_ASSERT(true  == defaultInput.mMalfBiasFlag);
    CPPUNIT_ASSERT(true  == defaultInput.mMalfDriftFlag);
    CPPUNIT_ASSERT(true  == defaultInput.mMalfNoiseFlag);
    CPPUNIT_ASSERT(true  == defaultInput.mMalfResolutionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(10.0, defaultInput.mTruthInput,          FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.0,  defaultInput.mMalfFailToValue,     FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(8.0,  defaultInput.mMalfScaleValue,      FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0,  defaultInput.mMalfBiasValue,       FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.0,  defaultInput.mMalfDriftRate,       FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0,  defaultInput.mMalfNoiseScale,      FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0,  defaultInput.mMalfResolutionValue, FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0,  defaultInput.mDrift,               FLT_EPSILON);

    /// - Test self-assignment of a test input data article.
    CPPUNIT_ASSERT(&defaultInput == &(defaultInput = defaultInput));

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the construction of the Analog Sensor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorAnalog::testConstruction()
{
    std::cout << "\n UtSensorAnalog ....... 03: testConstruction...........................";

    /// - Test default construction of the test article base class.
    FriendlySensorAnalog article;
    CPPUNIT_ASSERT(""                            == article.mName);
    CPPUNIT_ASSERT(false                         == article.mPowerFlag);
    CPPUNIT_ASSERT(false                         == article.mDegradedFlag);
    CPPUNIT_ASSERT(false                         == article.mMalfFailToFlag);
    CPPUNIT_ASSERT(false                         == article.mMalfFailOffFlag);
    CPPUNIT_ASSERT(false                         == article.mMalfFailStuckFlag);
    CPPUNIT_ASSERT(false                         == article.mMalfIgnorePower);
    CPPUNIT_ASSERT(false                         == article.mMalfPerfectSensor);
    CPPUNIT_ASSERT(false                         == article.mInitFlag);

    /// - Test default construction of the test article.
    CPPUNIT_ASSERT(0.0                           == article.mMinRange);
    CPPUNIT_ASSERT(0.0                           == article.mMaxRange);
    CPPUNIT_ASSERT(0.0                           == article.mOffValue);
    CPPUNIT_ASSERT(0.0                           == article.mNominalBias);
    CPPUNIT_ASSERT(0.0                           == article.mNominalScale);
    CPPUNIT_ASSERT(0.0                           == article.mNominalNoiseScale);
    CPPUNIT_ASSERT(0.0                           == article.mNominalResolution);
    CPPUNIT_ASSERT(0.0                           == article.mTruthInput);
    CPPUNIT_ASSERT(0.0                           == article.mMalfFailToValue);
    CPPUNIT_ASSERT(false                         == article.mMalfScaleFlag);
    CPPUNIT_ASSERT(0.0                           == article.mMalfScaleValue);
    CPPUNIT_ASSERT(false                         == article.mMalfBiasFlag);
    CPPUNIT_ASSERT(0.0                           == article.mMalfBiasValue);
    CPPUNIT_ASSERT(false                         == article.mMalfDriftFlag);
    CPPUNIT_ASSERT(0.0                           == article.mMalfDriftRate);
    CPPUNIT_ASSERT(false                         == article.mMalfNoiseFlag);
    CPPUNIT_ASSERT(0.0                           == article.mMalfNoiseScale);
    CPPUNIT_ASSERT(false                         == article.mMalfResolutionFlag);
    CPPUNIT_ASSERT(0.0                           == article.mMalfResolutionValue);
    CPPUNIT_ASSERT(0.0                           == article.mDrift);
    CPPUNIT_ASSERT(0.0                           == article.mTruthOutput);
    CPPUNIT_ASSERT(0.0                           == article.mSensedOutput);
    CPPUNIT_ASSERT(0                             == article.mNoiseFunction);
    CPPUNIT_ASSERT(UnitConversion::NO_CONVERSION == article.mUnitConversion);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests nominal initialization of the Analog Sensor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorAnalog::testInitialize()
{
    std::cout << "\n UtSensorAnalog ....... 04: testInitialize.............................";

    /// - Test nominal initialization of the test article base class.
    FriendlySensorAnalog article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName.c_str()));

    CPPUNIT_ASSERT(tName                 == article.mName);
    CPPUNIT_ASSERT(tPowerFlag            == article.mPowerFlag);
    CPPUNIT_ASSERT(false                 == article.mDegradedFlag);
    CPPUNIT_ASSERT(false                 == article.mMalfFailToFlag);
    CPPUNIT_ASSERT(false                 == article.mMalfFailOffFlag);
    CPPUNIT_ASSERT(false                 == article.mMalfFailStuckFlag);
    CPPUNIT_ASSERT(false                 == article.mMalfIgnorePower);
    CPPUNIT_ASSERT(false                 == article.mMalfPerfectSensor);

    /// - Test nominal initialization of the test article.
    CPPUNIT_ASSERT(tMinRange             == article.mMinRange);
    CPPUNIT_ASSERT(tMaxRange             == article.mMaxRange);
    CPPUNIT_ASSERT(tOffValue             == article.mOffValue);
    CPPUNIT_ASSERT(tNominalBias          == article.mNominalBias);
    CPPUNIT_ASSERT(tNominalScale         == article.mNominalScale);
    CPPUNIT_ASSERT(tNominalNoiseScale    == article.mNominalNoiseScale);
    CPPUNIT_ASSERT(tNominalNoiseFunction == article.mNoiseFunction);
    CPPUNIT_ASSERT(tUnitConversion       == article.mUnitConversion);
    CPPUNIT_ASSERT(tNominalResolution    == article.mNominalResolution);
    CPPUNIT_ASSERT(tTruthInput           == article.mTruthInput);
    CPPUNIT_ASSERT(tTruthInput           == article.mTruthOutput);
    CPPUNIT_ASSERT(0.0                   == article.mMalfFailToValue);
    CPPUNIT_ASSERT(false                 == article.mMalfScaleFlag);
    CPPUNIT_ASSERT(0.0                   == article.mMalfScaleValue);
    CPPUNIT_ASSERT(false                 == article.mMalfBiasFlag);
    CPPUNIT_ASSERT(0.0                   == article.mMalfBiasValue);
    CPPUNIT_ASSERT(false                 == article.mMalfDriftFlag);
    CPPUNIT_ASSERT(0.0                   == article.mMalfDriftRate);
    CPPUNIT_ASSERT(false                 == article.mMalfNoiseFlag);
    CPPUNIT_ASSERT(0.0                   == article.mMalfNoiseScale);
    CPPUNIT_ASSERT(false                 == article.mMalfResolutionFlag);
    CPPUNIT_ASSERT(0.0                   == article.mMalfResolutionValue);
    CPPUNIT_ASSERT(0.0                   == article.mDrift);
    CPPUNIT_ASSERT(true                  == article.mInitFlag);

    /// - Based on the config & input data, verify the initial sensor output.
    float expected =  tTruthInput * tNominalScale + tNominalBias + tNominalNoiseScale;
    expected = tNominalResolution * static_cast<double>(round(expected/tNominalResolution));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mSensedOutput, FLT_EPSILON * expected);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests initialization exceptions of the Analog Sensor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorAnalog::testInitializeExceptions()
{
    std::cout << "\n UtSensorAnalog ....... 05: testInitializeExceptions...................";

    /// - Verify exception is thrown when minimum range >= maximum range.
    FriendlySensorAnalog article;
    tNominalConfig->mMinRange = 49.0;
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName.c_str()),
                         TsInitializationException);
    tNominalConfig->mMinRange = tMinRange;

    /// - Verify exception is thrown when off value is not between minimum range & maximum range.
    tNominalConfig->mOffValue = 0.0;
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName.c_str()),
                         TsInitializationException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the various setter & getter methods of the Analog Sensor class.
///
/// @details Verifies the following TS21 requirements as they pertain to the sensor model:
///          - R.TS210-0021: TS21 vehicle subsystem models shall provide sensed and actual data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorAnalog::testAccessors()
{
    std::cout << "\n UtSensorAnalog ....... 06: testAccessors..............................";

    /// - Set up a test article with nominal config & input data.
    FriendlySensorAnalog article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName.c_str()));

    /// - Test the methods to set and get the input truth value.
    article.setTruthInput(20.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(20.0, article.getTruthInput(),   FLT_EPSILON);

    /// - Test the method to get the sensed output.
    article.mSensedOutput = 21.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(21.0, article.getSensedOutput(), FLT_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the processInputs method of the Analog Sensor class including numerical underflow
///         and units conversion function.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorAnalog::testInput()
{
    std::cout << "\n UtSensorAnalog ....... 07: testInput..................................";

    /// - Set up a test article with nominal config & input data and an input units conversion.
    FriendlySensorAnalog article;
    tNominalConfig->mUnitConversion = UnitConversion::K_TO_F;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName.c_str()));

    /// - Give the sensor an extremely small truth input and verify it is zeroed before applying the
    ///   units conversion.  This should result in absolute zero expressed in deg. F.
    article.setTruthInput(FLT_MIN * 0.1);
    double expected = -UnitConversion::ZERO_F_IN_R;
    article.processInput();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,     article.mSensedOutput, -FLT_EPSILON * expected);

    /// - Give the sensor a normal truth input and verify units conversion is correctly applied.
    ///   This should result in 32 deg. F.
    article.setTruthInput(273.15);
    expected = UnitConversion::convertDegKToDegF(273.15);
    article.processInput();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,     article.mSensedOutput, FLT_EPSILON * 273.15);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the applyScale method of the Analog Sensor class including nominal operation and
///         scale malfunction.
///
/// @details Verifies the following TS21 requirements as they pertain to the sensor model:
///          - R.TS210-0009: TS21 vehicle subsystem sensor models shall have fail, scale, noise,
///                          drift and bias malfunctions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorAnalog::testScale()
{
    std::cout << "\n UtSensorAnalog ....... 08: testScale..................................";

    /// - Set up a test article with nominal config & input data.
    FriendlySensorAnalog article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName.c_str()));

    /// - Verify the scale output with no malfunction.
    article.mSensedOutput = 10.0;
    const float expected  = 10.0 * tNominalScale;
    article.applyScale();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,     article.mSensedOutput, FLT_EPSILON);

    /// - Verify the scale output with malfunction added.
    article.mSensedOutput    = 10.0;
    article.mMalfScaleFlag   = true;
    article.mMalfScaleValue  = 0.5;
    const float expectedMalf = expected * 0.5;
    article.applyScale();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMalf, article.mSensedOutput, FLT_EPSILON);

    /// - Remove the malfunction and verify original result.
    article.mSensedOutput   = 10.0;
    article.mMalfScaleFlag  = false;
    article.applyScale();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,     article.mSensedOutput, FLT_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the applyBias method of the Analog Sensor class including nominal operation and
///         bias malfunction.
///
/// @details Verifies the following TS21 requirements as they pertain to the sensor model:
///          - R.TS210-0009: TS21 vehicle subsystem sensor models shall have fail, scale, noise,
///                          drift and bias malfunctions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorAnalog::testBias()
{
    std::cout << "\n UtSensorAnalog ....... 09: testBias...................................";

    /// - Set up a test article with nominal config & input data.
    FriendlySensorAnalog article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName.c_str()));

    /// - Verify the bias output with no malfunction.
    article.mSensedOutput = 10.0;
    const float expected  = 10.0 + tNominalBias;
    article.applyBias();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,     article.mSensedOutput, FLT_EPSILON);

    /// - Verify the scale output with malfunction added.
    article.mSensedOutput  = 10.0;
    article.mMalfBiasFlag  = true;
    article.mMalfBiasValue = 0.5;
    const float expectedMalf = expected + 0.5;
    article.applyBias();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMalf, article.mSensedOutput, FLT_EPSILON);

    /// - Remove the malfunction and verify original result.
    article.mSensedOutput = 10.0;
    article.mMalfBiasFlag  = false;
    article.applyBias();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,     article.mSensedOutput, FLT_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the applyDrift method of the Analog Sensor class including nominal operation and
///         drift malfunction.
///
/// @details Verifies the following TS21 requirements as they pertain to the sensor model:
///          - R.TS210-0009: TS21 vehicle subsystem sensor models shall have fail, scale, noise,
///                          drift and bias malfunctions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorAnalog::testDrift()
{
    std::cout << "\n UtSensorAnalog ....... 10: testDrift..................................";

    /// - Set up a test article with nominal config & input data.
    FriendlySensorAnalog article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName.c_str()));

    /// - Verify the drift output with no malfunction.
    article.mSensedOutput  = 10.0;
    const float expected   = 10.0;
    article.applyDrift(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,  article.mSensedOutput, FLT_EPSILON);

    /// - Verify the drift output with malfunction active.
    article.mSensedOutput  = 10.0;
    article.mMalfDriftFlag = true;
    article.mMalfDriftRate = 1.0;
    const float expected1  = expected + tTimeStep;
    article.applyDrift(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected1, article.mSensedOutput, FLT_EPSILON);

    /// - Verify a 2nd drift frame with a different drift rate.
    article.mSensedOutput  = 10.0;
    article.mMalfDriftFlag = true;
    article.mMalfDriftRate = 1.5;
    const float expected2  = expected1 + 1.5 * tTimeStep;
    article.applyDrift(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected2, article.mSensedOutput, FLT_EPSILON);

    /// - Verify drift is zeroed when the malfunction is removed.
    article.mSensedOutput  = 10.0;
    article.mMalfDriftFlag = false;
    article.applyDrift(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,  article.mSensedOutput, FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,       article.mDrift,        0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the applyNoise method of the Analog Sensor class including nominal operation and
///         noise malfunction.  This only tests the application of the noise generator output to the
///         sensor output, not the actual noise generator.  We assume the noise generator, which is
///         an external method, is already verified.
///
/// @details Verifies the following TS21 requirements as they pertain to the sensor model:
///          - R.TS210-0009: TS21 vehicle subsystem sensor models shall have fail, scale, noise,
///                          drift and bias malfunctions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorAnalog::testNoise()
{
    std::cout << "\n UtSensorAnalog ....... 11: testNoise..................................";

    /// - Set up a test article with nominal config & input data.
    FriendlySensorAnalog article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName.c_str()));

    /// - Verify the noise output with no malfunction.
    article.mSensedOutput        = 10.0;
    article.mMalfNoiseScale      = 0.5;
    article.mMalfNoiseMultiplier = 0.1;
    float expected = 10.0 + tNominalNoiseScale * UtSensorAnalogNoise::testNoise()
                   + tNominalNoiseMult * UtSensorAnalogNoise::testNoise() * fabs(10.0 - tOffValue);
    article.applyNoise();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mSensedOutput, FLT_EPSILON);

    /// - Verify the noise output with malfunction.
    article.mSensedOutput        = 10.0;
    article.mMalfNoiseFlag       = true;
    expected                     = 10.0 + 0.5 * UtSensorAnalogNoise::testNoise()
                                 + 0.1 * UtSensorAnalogNoise::testNoise() * fabs(10.0 - tOffValue);
    article.applyNoise();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mSensedOutput, FLT_EPSILON);

    /// - Verify the noise output with noise scales zero.
    article.mSensedOutput        = 10.0;
    article.mMalfNoiseScale      = 0.0;
    article.mMalfNoiseMultiplier = 0.0;
    expected                     = 10.0;
    article.applyNoise();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mSensedOutput, FLT_EPSILON);

    /// - Set up a new test article with no noise function supplied.
    FriendlySensorAnalog article2;
    tNominalConfig->mNoiseFunction = 0;
    CPPUNIT_ASSERT_NO_THROW(article2.initialize(*tNominalConfig, *tNominalInput, tName.c_str()));

    /// - Verify the noise output with no noise function.
    article2.mSensedOutput       = 10.0;
    article2.mMalfNoiseFlag      = true;
    article2.mMalfNoiseScale     = 0.5;
    article.mMalfNoiseMultiplier = 0.1;
    expected                     = 10.0;
    article2.applyNoise();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article2.mSensedOutput, FLT_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the applyResolution method of the Analog sensor class including nominal and
///         malfunction.
///
/// @details Verifies the following TS21 requirements as they pertain to the sensor model:
///          - R.TS210-0009: TS21 vehicle subsystem sensor models shall have fail, scale, noise,
///                          drift and bias malfunctions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorAnalog::testResolution()
{
    std::cout << "\n UtSensorAnalog ....... 12: testResolution.............................";

    /// - Set up a test article with nominal config & input data.
    FriendlySensorAnalog article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName.c_str()));

    /// - Verify the resolution output with no malfunction.  The nominal quantize scale is set to
    ///   0.2, so this case should round downwards.
    article.mSensedOutput   = 10.05;
    float expected          = 10.0;
    article.applyResolution();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mSensedOutput, FLT_EPSILON);

    /// - Verify the rounding up case.
    article.mSensedOutput   = 10.15;
    expected                = 10.2;
    article.applyResolution();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mSensedOutput, FLT_EPSILON);

    /// - Verify a negative value case, rounding down.
    article.mSensedOutput   = -10.15;
    expected                = -10.2;
    article.applyResolution();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mSensedOutput, FLT_EPSILON);

    /// - Verify a negative value case, rounding up.
    article.mSensedOutput   = -10.05;
    expected                = -10.0;
    article.applyResolution();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mSensedOutput, FLT_EPSILON);

    /// - Verify a negative value gets used as positive.
    article.mSensedOutput         = -10.15;
    article.mNominalResolution    =  -0.2;
    expected                      = -10.2;
    article.applyResolution();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mSensedOutput, FLT_EPSILON);

    /// - Verify zero value.
    article.mSensedOutput         = 10.15;
    article.mNominalResolution    =  0.0;
    expected                      = 10.15;
    article.applyResolution();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mSensedOutput, FLT_EPSILON);

    /// - Verify malfunction overrides the nominal value.
    article.mSensedOutput         = 10.15;
    article.mNominalResolution    =  0.2;
    article.mMalfResolutionFlag   = true;
    article.mMalfResolutionValue  =  0.18;
    expected                      = 10.08;  // int (10.15/0.18) * 0.18
    article.applyResolution();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mSensedOutput, FLT_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the update method of the Analog Sensor class under nominal operation with good
///         power and no malfs.
///
/// @details Verifies the following TS21 requirements as they pertain to the sensor model:
///          - R.TS210-0021: TS21 vehicle subsystem models shall provide sensed and actual data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorAnalog::testUpdateNominal()
{
    std::cout << "\n UtSensorAnalog ....... 13: testUpdateNominal..........................";

    /// - Set up a test article with nominal config & input data.
    FriendlySensorAnalog article;
    tNominalConfig->mNominalResolution = 0.18;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName.c_str()));

    /// - Based on the nominal config & input data, verify the sensor output.
    float expected = tTruthInput * tNominalScale + tNominalBias + tNominalNoiseScale;
    expected = 0.18 * static_cast<double>(round(expected/0.18));
    article.update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mSensedOutput, FLT_EPSILON * expected);
    CPPUNIT_ASSERT(false == article.mDegradedFlag);

    /// - Verify the lower output limit.
    article.setTruthInput(tMinRange - 5.0);
    expected = tMinRange;
    article.update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mSensedOutput, FLT_EPSILON * expected);

    /// - Verify the upper output limit.
    article.setTruthInput(tMaxRange + 5.0);
    expected = tMaxRange;
    article.update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mSensedOutput, FLT_EPSILON * expected);

    /// - Verify protection against underflow in the input.
    article.setTruthInput(DBL_MIN);
    article.mMinRange          =-1.0;
    article.mNominalResolution = 0.0;
    article.mNominalBias       = 0.0;
    article.mNominalScale      = 1.0;
    article.mNominalNoiseScale = 0.0;
    article.mNominalNoiseMult  = 0.0;
    expected = 0.0;
    article.update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mSensedOutput, 0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the update method of the Analog Sensor class under nominal operation with no power
///         and no malfs.
///
/// @details Verifies the following TS21 requirements as they pertain to the sensor model:
///          - R.TS210-0021: TS21 vehicle subsystem models shall provide sensed and actual data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorAnalog::testUpdateNoPower()
{
    std::cout << "\n UtSensorAnalog ....... 14: testUpdateNoPower..........................";

    /// - Set up a test article with nominal config & input data.
    FriendlySensorAnalog article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName.c_str()));

    /// - Based on the nominal config & input data, verify the sensor output is equal to the
    ///   configured off value.
    float expected = tOffValue;
    article.setPowerFlag(false);
    article.update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mSensedOutput, FLT_EPSILON);
    CPPUNIT_ASSERT(false == article.mDegradedFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the fail-to malfunction in the Analog Sensor class.
///
/// @details Verifies the following TS21 requirements as they pertain to the sensor model:
///          - R.TS210-0009: TS21 vehicle subsystem sensor models shall have fail, scale, noise,
///                          drift and bias malfunctions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorAnalog::testMalfFailTo()
{
    std::cout << "\n UtSensorAnalog ....... 15: testMalfFailTo.............................";

    /// - Set up a test article with nominal config & input data.
    FriendlySensorAnalog article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName.c_str()));

    /// - Verify sensed output of sensor when failed to a specific value.
    article.mMalfFailToFlag  = true;
    article.mMalfFailToValue = 27.0;
    article.update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(27.0, article.getSensedOutput(), FLT_EPSILON);
    CPPUNIT_ASSERT(true == article.mDegradedFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the stuck malfunction in the Analog Sensor class.
///
/// @details Verifies the following TS21 requirements as they pertain to the sensor model:
///          - R.TS210-0009: TS21 vehicle subsystem sensor models shall have fail, scale, noise,
///                          drift and bias malfunctions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorAnalog::testMalfStuck()
{
    std::cout << "\n UtSensorAnalog ....... 16: testMalfStuck..............................";

    /// - Set up a test article with nominal config & input data.
    FriendlySensorAnalog article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName.c_str()));

    /// - Verify nominal sensed output of sensor.
    float expected = tTruthInput * tNominalScale + tNominalBias + tNominalNoiseScale;
    expected = tNominalResolution * static_cast<double>(round(expected/tNominalResolution));
    article.update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mSensedOutput, FLT_EPSILON * expected);

    /// - Verify sensed output of sensor for a different truth input but the stuck malf active.
    article.setTruthInput(20.0);
    article.mMalfFailStuckFlag = true;
    article.update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mSensedOutput, FLT_EPSILON * expected);
    CPPUNIT_ASSERT(true  == article.mDegradedFlag);

    /// - Remove the malf and verify the sensor goes back to the new truth value.
    article.mMalfFailStuckFlag = false;
    expected = article.getTruthInput() * tNominalScale + tNominalBias + tNominalNoiseScale;
    expected = tNominalResolution * static_cast<double>(round(expected/tNominalResolution));
    article.update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mSensedOutput, FLT_EPSILON * expected);
    CPPUNIT_ASSERT(false == article.mDegradedFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the perfect sensor malfunction in the Analog Sensor class.
///
/// @details Verifies the following TS21 requirements as they pertain to the sensor model:
///          - R.TS210-0009: TS21 vehicle subsystem sensor models shall have fail, scale, noise,
///                          drift and bias malfunctions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorAnalog::testMalfPerfect()
{
    std::cout << "\n UtSensorAnalog ....... 17: testMalfPerfect............................";

    /// - Set up a test article with nominal config & input data.
    FriendlySensorAnalog article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName.c_str()));

    /// - Verify nominal sensed output of sensor with power off and no malfs.
    float expected = tOffValue;
    article.setPowerFlag(false);
    article.update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mSensedOutput, FLT_EPSILON);

    /// - Verify the perfect sensor malf overrides power.
    article.mMalfPerfectSensor = true;
    article.update(tTimeStep);
    expected = tTruthInput;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mSensedOutput, FLT_EPSILON);
    CPPUNIT_ASSERT(false == article.mDegradedFlag);

    /// - Verify the perfect sensor malf overrides the stuck malf.
    article.mMalfFailStuckFlag = true;
    article.setTruthInput(20.0);
    expected = 20.0;
    article.update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mSensedOutput, FLT_EPSILON);
    CPPUNIT_ASSERT(false == article.mDegradedFlag);

    /// - Verify the perfect sensor malf overrides the fail-to malf.
    article.mMalfFailStuckFlag = false;
    article.mMalfFailToFlag = true;
    article.mMalfFailToValue = 35.0;
    article.update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mSensedOutput, FLT_EPSILON);
    CPPUNIT_ASSERT(false == article.mDegradedFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests interactions of malfunctions in the Analog Sensor class that have not been covered
///         in previous tests.
///
/// @details Verifies the following TS21 requirements as they pertain to the sensor model:
///          - R.TS210-0021: TS21 vehicle subsystem models shall provide sensed and actual data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorAnalog::testMalfInteractions()
{
    std::cout << "\n UtSensorAnalog ....... 18: testMalfInteractions.......................";

    /// - Set up a test article with nominal config & input data.
    FriendlySensorAnalog article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName.c_str()));

    /// - Verify nominal sensed output of sensor with power off and no malfs.
    float expected = tOffValue;
    article.setPowerFlag(false);
    article.update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mSensedOutput, FLT_EPSILON);

    /// - Verify the fail-to malf overrides the stuck malf.
    article.setTruthInput(20.0);
    article.setPowerFlag(true);
    article.mMalfFailStuckFlag = true;
    article.mMalfFailToFlag    = true;
    article.mMalfFailToValue   = 15.0;
    expected                   = 15.0;
    article.update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.getSensedOutput(), FLT_EPSILON);
    CPPUNIT_ASSERT(true  == article.mDegradedFlag);

    /// - Verify that power overrides the fail-to malf.
    article.mMalfFailStuckFlag = false;
    article.mMalfFailOffFlag   = true;
    expected                   = tOffValue;
    article.update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.getSensedOutput(), FLT_EPSILON);
    CPPUNIT_ASSERT(true  == article.mDegradedFlag);

    /// - Verify that power overrides the stuck malf.
    article.mMalfFailStuckFlag = true;
    article.update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.getSensedOutput(), FLT_EPSILON);
    CPPUNIT_ASSERT(true  == article.mDegradedFlag);

    /// - Set up a new test article with nominal config & input data.
    FriendlySensorAnalog article2;
    CPPUNIT_ASSERT_NO_THROW(article2.initialize(*tNominalConfig, *tNominalInput, tName.c_str()));

    /// - Verify the interaction of the scale, bias, drift, noise & quantization malfunctions.
    article2.mMalfScaleFlag       = true;
    article2.mMalfScaleValue      = 1.2;
    article2.mMalfBiasFlag        = true;
    article2.mMalfBiasValue       = -0.6;
    article2.mMalfDriftFlag       = true;
    article2.mMalfDriftRate       = 0.05;
    article2.mMalfNoiseFlag       = true;
    article2.mMalfNoiseScale      = 5.0;
    article2.mMalfResolutionFlag  = true;
    article2.mMalfResolutionValue = 0.175;
    expected = tTruthInput * tNominalScale * 1.2 + tNominalBias - 0.6 + 0.05 * tTimeStep +
               tNominalNoiseScale * 5.0;
    expected = 0.175 * static_cast<double>(round(expected/0.175));
    article2.update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article2.getSensedOutput(), FLT_EPSILON * expected);
    CPPUNIT_ASSERT(true  == article.mDegradedFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests compatibility of the Analog Sensor class with established external noise
///         functions.  This method is intended to be updated with new noise functions as they
///         become available.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorAnalog::testExternalNoiseFunctions()
{
    std::cout << "\n UtSensorAnalog ....... 19: testExternalNoiseFunctions.................";

    /// - Set up a test article with nominal config & input data, and pointing to the TsNoise
    ///   getNoise C code function.  Remove the quantization so it doesn't mask the noise.
    FriendlySensorAnalog article;
    tNominalConfig->mNoiseFunction     = TsNoise::getNoise;
    tNominalConfig->mNominalResolution = 0.0;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName.c_str()));

    /// - Step the sensor to call the TS_noise function.  We just need to verify this configuration
    ///   compiles and runs, but check each value to make sure it is within the noise scale.
    float expected = tTruthInput * tNominalScale + tNominalBias;
    article.update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mSensedOutput, tNominalNoiseScale);
    article.update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mSensedOutput, tNominalNoiseScale);
    article.update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mSensedOutput, tNominalNoiseScale);
    article.update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mSensedOutput, tNominalNoiseScale);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Test the sense convenience method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorAnalog::testSense()
{
    std::cout << "\n UtSensorAnalog ....... 20: testSense..................................";

    /// - Set up a test article with nominal config & input data.
    FriendlySensorAnalog article;
    tNominalConfig->mNominalResolution = 0.18;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName.c_str()));

    /// - Based on the nominal config & input data, verify the sensor output.
    float expected = tTruthInput * tNominalScale + tNominalBias + tNominalNoiseScale;
    expected = 0.18 * static_cast<double>(round(expected/0.18));
    const float result = article.sense(tTimeStep, true, tTruthInput);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, result, FLT_EPSILON * expected);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Test update of the truth output with units conversion.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorAnalog::testTruthOutput()
{
    std::cout << "\n UtSensorAnalog ....... 21: testTruthOutput............................";

    /// - Set up a test article with nominal config & input data, add a units conversion.
    FriendlySensorAnalog article;
    tNominalConfig->mUnitConversion = UnitConversion::K_TO_F;
    tNominalInput->mTruthInput      = 273.15;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName.c_str()));

    /// - Verify truth output from the update method.  Note this also double-checks K to F to known
    ///   value at freezing point of water.
    article.update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(32.0, article.mTruthOutput, FLT_EPSILON);

    /// - Verify truth output from the sense method.
    article.sense(0.0, false, tNominalInput->mTruthInput);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(32.0, article.mTruthOutput, FLT_EPSILON);

    /// - Verify malfunctions do not interfere with the truth output.
    article.mMalfFailOffFlag   = true;
    article.update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(32.0, article.mTruthOutput, FLT_EPSILON);
    article.sense(0.0, false, tNominalInput->mTruthInput);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(32.0, article.mTruthOutput, FLT_EPSILON);

    std::cout << "... Pass.";
    std::cout << "\n--------------------------------------------------------------------------------";
}
