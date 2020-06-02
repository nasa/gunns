/****************************** TRICK HEADER ******************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
 ((common/sensors/SensorBooleanAi.o))

PROGRAMMERS:
 ((Jason Harvey) (L-3 Communications) (2012-04) (TS21) (Initial))

 **************************************************************************************************/

#include "UtSensorBooleanAi.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
UtSensorBooleanAi::UtSensorBooleanAi()
    :
    tArticle(),
    tName()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
UtSensorBooleanAi::~UtSensorBooleanAi()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  unit test infrastructure
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorBooleanAi::setUp()
{
    /// - Define nominal article name.
    tName = "Test Sensor";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  unit test infrastructure
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorBooleanAi::tearDown()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the constructors & destructors of the Analog Input Boolean Sensor config data
///         class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorBooleanAi::testConfigData()
{
    std::cout << "\n--------------------------------------------------------------------------------";
    std::cout << "\n UtSensorBooleanAi .... 01: testConfigData.............................";

    /// - Test default construction of a test config data article.
    SensorBooleanAiConfigData defaultConfig;
    CPPUNIT_ASSERT(false == defaultConfig.mOffValue);
    CPPUNIT_ASSERT(0.0   == defaultConfig.mTarget);
    CPPUNIT_ASSERT(0.0   == defaultConfig.mTolerance);

    /// - Test nominal construction of a test config data article.
    SensorBooleanAiConfigData nominalConfig(true, 0.9, 0.1);
    CPPUNIT_ASSERT(true          == nominalConfig.mOffValue);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.9, nominalConfig.mTarget,    FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1, nominalConfig.mTolerance, FLT_EPSILON);

    /// - Test copy construction of a test config data article.
    SensorBooleanAiConfigData copyConfig(nominalConfig);
    CPPUNIT_ASSERT(true          == copyConfig.mOffValue);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.9, copyConfig.mTarget,       FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1, copyConfig.mTolerance,    FLT_EPSILON);

    /// - Test assignment of a test config data article.
    defaultConfig = nominalConfig;
    CPPUNIT_ASSERT(true          == defaultConfig.mOffValue);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.9, defaultConfig.mTarget,       FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1, defaultConfig.mTolerance,    FLT_EPSILON);

    /// - Test self-assignment of a test config data article.
    CPPUNIT_ASSERT(&defaultConfig == &(defaultConfig = defaultConfig));

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the constructors & destructors of the Analog Input Boolean Sensor input data
///         class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorBooleanAi::testInputData()
{
    std::cout << "\n UtSensorBooleanAi .... 02: testInputData..............................";

    /// - Test default construction of a test input data article.
    SensorBooleanAiInputData defaultInput;
    CPPUNIT_ASSERT(false == defaultInput.mPowerFlag);
    CPPUNIT_ASSERT(false == defaultInput.mMalfFailToFlag);
    CPPUNIT_ASSERT(false == defaultInput.mMalfFailOffFlag);
    CPPUNIT_ASSERT(false == defaultInput.mMalfFailStuckFlag);
    CPPUNIT_ASSERT(false == defaultInput.mMalfIgnorePower);
    CPPUNIT_ASSERT(false == defaultInput.mMalfPerfectSensor);
    CPPUNIT_ASSERT(false == defaultInput.mTruthInput);
    CPPUNIT_ASSERT(false == defaultInput.mMalfFailToValue);
    CPPUNIT_ASSERT(0.0   == defaultInput.mTruthInputAnalog);

    /// - Test nominal construction of a test input data article.
    SensorBooleanAiInputData nominalInput(true, true, 0.5);
    CPPUNIT_ASSERT(true  == nominalInput.mPowerFlag);
    CPPUNIT_ASSERT(false == nominalInput.mMalfFailToFlag);
    CPPUNIT_ASSERT(false == nominalInput.mMalfFailOffFlag);
    CPPUNIT_ASSERT(false == nominalInput.mMalfFailStuckFlag);
    CPPUNIT_ASSERT(false == nominalInput.mMalfIgnorePower);
    CPPUNIT_ASSERT(false == nominalInput.mMalfPerfectSensor);
    CPPUNIT_ASSERT(true  == nominalInput.mTruthInput);
    CPPUNIT_ASSERT(false == nominalInput.mMalfFailToValue);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, nominalInput.mTruthInputAnalog, FLT_EPSILON);

    /// - Test copy construction of a test config data article.
    nominalInput.mMalfFailToFlag    = true;
    nominalInput.mMalfFailOffFlag   = true;
    nominalInput.mMalfFailStuckFlag = true;
    nominalInput.mMalfIgnorePower   = true;
    nominalInput.mMalfPerfectSensor = true;
    nominalInput.mMalfFailToValue   = true;
    SensorBooleanAiInputData copyInput(nominalInput);
    CPPUNIT_ASSERT(true  == copyInput.mPowerFlag);
    CPPUNIT_ASSERT(true  == copyInput.mMalfFailToFlag);
    CPPUNIT_ASSERT(true  == copyInput.mMalfFailOffFlag);
    CPPUNIT_ASSERT(true  == copyInput.mMalfFailStuckFlag);
    CPPUNIT_ASSERT(true  == copyInput.mMalfIgnorePower);
    CPPUNIT_ASSERT(true  == copyInput.mMalfPerfectSensor);
    CPPUNIT_ASSERT(true  == copyInput.mTruthInput);
    CPPUNIT_ASSERT(true  == copyInput.mMalfFailToValue);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, copyInput.mTruthInputAnalog,    FLT_EPSILON);

    /// - Test assignment of a test input data article.
    defaultInput = nominalInput;
    CPPUNIT_ASSERT(true  == defaultInput.mPowerFlag);
    CPPUNIT_ASSERT(true  == defaultInput.mMalfFailToFlag);
    CPPUNIT_ASSERT(true  == defaultInput.mMalfFailOffFlag);
    CPPUNIT_ASSERT(true  == defaultInput.mMalfFailStuckFlag);
    CPPUNIT_ASSERT(true  == defaultInput.mMalfIgnorePower);
    CPPUNIT_ASSERT(true  == defaultInput.mMalfPerfectSensor);
    CPPUNIT_ASSERT(true  == defaultInput.mTruthInput);
    CPPUNIT_ASSERT(true  == defaultInput.mMalfFailToValue);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, defaultInput.mTruthInputAnalog,    FLT_EPSILON);

    /// - Test self-assignment of a test input data article.
    CPPUNIT_ASSERT(&defaultInput == &(defaultInput = defaultInput));

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the construction of the Analog Input Boolean Sensor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorBooleanAi::testConstruction()
{
    std::cout << "\n UtSensorBooleanAi .... 03: testConstruction...........................";

    /// - Test default construction of the test article base classes.
    CPPUNIT_ASSERT(""    == tArticle.mName);
    CPPUNIT_ASSERT(false == tArticle.mPowerFlag);
    CPPUNIT_ASSERT(false == tArticle.mDegradedFlag);
    CPPUNIT_ASSERT(false == tArticle.mMalfFailToFlag);
    CPPUNIT_ASSERT(false == tArticle.mMalfFailOffFlag);
    CPPUNIT_ASSERT(false == tArticle.mMalfFailStuckFlag);
    CPPUNIT_ASSERT(false == tArticle.mMalfIgnorePower);
    CPPUNIT_ASSERT(false == tArticle.mMalfPerfectSensor);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);
    CPPUNIT_ASSERT(false == tArticle.mOffValue);
    CPPUNIT_ASSERT(false == tArticle.mTruthInput);
    CPPUNIT_ASSERT(false == tArticle.mMalfFailToValue);
    CPPUNIT_ASSERT(false == tArticle.mSensedOutput);

    /// - Test default construction of the test article.
    CPPUNIT_ASSERT(0.0   == tArticle.mTarget);
    CPPUNIT_ASSERT(0.0   == tArticle.mTolerance);
    CPPUNIT_ASSERT(0.0   == tArticle.mTruthInputAnalog);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests nominal initialization of the Analog Input Boolean Sensor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorBooleanAi::testInitialize()
{
    std::cout << "\n UtSensorBooleanAi .... 04: testInitialize.............................";

    /// - Define nominal config & input data.
    SensorBooleanAiConfigData nominalConfig(true, 0.9, 0.1);
    SensorBooleanAiInputData  nominalInput(true, true, 0.89);

    /// - Test nominal initialization of the test article base classes.
    CPPUNIT_ASSERT_NO_THROW(tArticle.initialize(nominalConfig, nominalInput, tName.c_str()));

    CPPUNIT_ASSERT(tName == tArticle.mName);
    CPPUNIT_ASSERT(true  == tArticle.mPowerFlag);
    CPPUNIT_ASSERT(false == tArticle.mDegradedFlag);
    CPPUNIT_ASSERT(false == tArticle.mMalfFailToFlag);
    CPPUNIT_ASSERT(false == tArticle.mMalfFailOffFlag);
    CPPUNIT_ASSERT(false == tArticle.mMalfFailStuckFlag);
    CPPUNIT_ASSERT(false == tArticle.mMalfIgnorePower);
    CPPUNIT_ASSERT(false == tArticle.mMalfPerfectSensor);
    CPPUNIT_ASSERT(true  == tArticle.mOffValue);
    CPPUNIT_ASSERT(true  == tArticle.mTruthInput);
    CPPUNIT_ASSERT(false == tArticle.mMalfFailToValue);
    CPPUNIT_ASSERT(true  == tArticle.mSensedOutput);

    /// - Test nominal initialization of the test article.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.9,  tArticle.mTarget,           FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1,  tArticle.mTolerance,        FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.89, tArticle.mTruthInputAnalog, FLT_EPSILON);
    CPPUNIT_ASSERT(true  == tArticle.mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests initialization exceptions of the Analog Input Boolean Sensor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorBooleanAi::testInitializeExceptions()
{
    std::cout << "\n UtSensorBooleanAi .... 05: testInitializeExceptions...................";

    /// - Verify exception is thrown when tolerance is < 0.
    SensorBooleanAiConfigData nominalConfig(true, 0.9, -0.01);
    SensorBooleanAiInputData  nominalInput(true, true, 0.89);
    CPPUNIT_ASSERT_THROW(tArticle.initialize(nominalConfig, nominalInput, tName.c_str()),
                         TsInitializationException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the various setter & getter methods of the Analog Input Boolean Sensor class.
///
/// @details Verifies the following TS21 requirements as they pertain to the sensor model:
///          - R.TS210-0021: TS21 vehicle subsystem models shall provide sensed and actual data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorBooleanAi::testAccessors()
{
    std::cout << "\n UtSensorBooleanAi .... 06: testAccessors..............................";

    /// - Set up a test article with nominal config & input data.
    SensorBooleanAiConfigData nominalConfig(false, 0.9, 0.1);
    SensorBooleanAiInputData  nominalInput(false, false, 0.5);
    tArticle.initialize(nominalConfig, nominalInput, tName.c_str());

    /// - Test the methods to set and get the input truth analog value.
    tArticle.setTruthInput(0.33);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.33, tArticle.getTruthInput(), FLT_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the update method of the Analog Input Boolean Sensor class under nominal
///         operation with good power and no malfs.  Since the sensor class reuses the base class
///         update method, we are really focusing on the conversion of the analog input into the
///         boolean output here.
///
/// @details Verifies the following TS21 requirements as they pertain to the sensor model:
///          - R.TS210-0021: TS21 vehicle subsystem models shall provide sensed and actual data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorBooleanAi::testUpdateNominal()
{
    std::cout << "\n UtSensorBooleanAi .... 07: testUpdateNominal..........................";

    /// - Set up a test article with nominal config & input data.
    SensorBooleanAiConfigData nominalConfig(false, 0.9, 0.1);
    SensorBooleanAiInputData  nominalInput(true, false, 0.5);
    tArticle.initialize(nominalConfig, nominalInput, tName.c_str());

    /// - The following tests verify the output of the sensor when the truth input is in different
    ///   regions relative to the target and tolerance values.
    ///
    /// - Verify: truth input < target - tolerance.
    tArticle.update(0.0);
    CPPUNIT_ASSERT(false == tArticle.getSensedOutput());

    /// - Verify: truth input = target - tolerance (within precision limits).
    tArticle.setTruthInput(0.8 + FLT_EPSILON);
    tArticle.update(0.0);
    CPPUNIT_ASSERT(true  == tArticle.getSensedOutput());

    /// - Verify: target - tolerance < truth input < target.
    tArticle.setTruthInput(0.85);
    tArticle.update(0.0);
    CPPUNIT_ASSERT(true  == tArticle.getSensedOutput());

    /// - Verify: target < truth input < target + tolerance.
    tArticle.setTruthInput(0.95);
    tArticle.update(0.0);
    CPPUNIT_ASSERT(true  == tArticle.getSensedOutput());

    /// - Verify: truth input = target + tolerance (within precision limits).
    tArticle.setTruthInput(1.0 - FLT_EPSILON);
    tArticle.update(0.0);
    CPPUNIT_ASSERT(true  == tArticle.getSensedOutput());

    /// - Verify: truth input > target + tolerance.
    tArticle.setTruthInput(1.05);
    tArticle.update(0.0);
    CPPUNIT_ASSERT(false == tArticle.getSensedOutput());

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Test the sense convenience method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorBooleanAi::testSense()
{
    std::cout << "\n UtSensorBooleanAi .... 08: testSense..................................";

    /// - Set up a test article with nominal config & input data.
    SensorBooleanAiConfigData nominalConfig(false, 0.9, 0.1);
    SensorBooleanAiInputData  nominalInput(true, false, 0.5);
    tArticle.initialize(nominalConfig, nominalInput, tName.c_str());

    /// - Verify: truth input = target - tolerance (within precision limits).
    CPPUNIT_ASSERT(tArticle.sense(0.0, true, 0.8 + FLT_EPSILON));

    std::cout << "... Pass.";
    std::cout << "\n--------------------------------------------------------------------------------";
}
