/****************************** TRICK HEADER ******************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
 ((common/sensors/SensorBooleanBi.o))

PROGRAMMERS:
 ((Jason Harvey) (L-3 Communications) (2012-04) (TS21) (Initial))

 **************************************************************************************************/

#include "UtSensorBooleanBi.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
UtSensorBooleanBi::UtSensorBooleanBi()
    :
    tArticle(),
    tName()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
UtSensorBooleanBi::~UtSensorBooleanBi()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  unit test infrastructure
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorBooleanBi::setUp()
{
    /// - Define nominal article name.
    tName = "Test Sensor";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  unit test infrastructure
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorBooleanBi::tearDown()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the constructors & destructors of the Boolean Input Boolean Sensor config data
///         class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorBooleanBi::testConfigData()
{
    std::cout << "\n--------------------------------------------------------------------------------";
    std::cout << "\n UtSensorBooleanBi .... 01: testConfigData.............................";

    /// - Test default construction of a test config data article.
    SensorBooleanBiConfigData defaultConfig;
    CPPUNIT_ASSERT(false == defaultConfig.mOffValue);

    /// - Test nominal construction of a test config data article.
    SensorBooleanBiConfigData nominalConfig(true);
    CPPUNIT_ASSERT(true  == nominalConfig.mOffValue);

    /// - Test copy construction of a test config data article.
    SensorBooleanBiConfigData copyConfig(nominalConfig);
    CPPUNIT_ASSERT(true  == copyConfig.mOffValue);

    /// - Test assignment of a test config data article.
    defaultConfig = nominalConfig;
    CPPUNIT_ASSERT(true  == defaultConfig.mOffValue);

    /// - Test self-assignment of a test config data article.
    CPPUNIT_ASSERT(&defaultConfig == &(defaultConfig = defaultConfig));

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the constructors & destructors of the Boolean Input Boolean Sensor input data
///         class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorBooleanBi::testInputData()
{
    std::cout << "\n UtSensorBooleanBi .... 02: testInputData..............................";

    /// - Test default construction of a test input data article.
    SensorBooleanBiInputData defaultInput;
    CPPUNIT_ASSERT(false == defaultInput.mPowerFlag);
    CPPUNIT_ASSERT(false == defaultInput.mMalfFailToFlag);
    CPPUNIT_ASSERT(false == defaultInput.mMalfFailOffFlag);
    CPPUNIT_ASSERT(false == defaultInput.mMalfFailStuckFlag);
    CPPUNIT_ASSERT(false == defaultInput.mMalfIgnorePower);
    CPPUNIT_ASSERT(false == defaultInput.mMalfPerfectSensor);
    CPPUNIT_ASSERT(false == defaultInput.mTruthInput);
    CPPUNIT_ASSERT(false == defaultInput.mMalfFailToValue);

    /// - Test nominal construction of a test input data article.
    SensorBooleanBiInputData nominalInput(true, true);
    CPPUNIT_ASSERT(true  == nominalInput.mPowerFlag);
    CPPUNIT_ASSERT(false == nominalInput.mMalfFailToFlag);
    CPPUNIT_ASSERT(false == nominalInput.mMalfFailOffFlag);
    CPPUNIT_ASSERT(false == nominalInput.mMalfFailStuckFlag);
    CPPUNIT_ASSERT(false == nominalInput.mMalfIgnorePower);
    CPPUNIT_ASSERT(false == nominalInput.mMalfPerfectSensor);
    CPPUNIT_ASSERT(true  == nominalInput.mTruthInput);
    CPPUNIT_ASSERT(false == nominalInput.mMalfFailToValue);

    /// - Test copy construction of a test config data article.
    nominalInput.mMalfFailToFlag    = true;
    nominalInput.mMalfFailOffFlag   = true;
    nominalInput.mMalfFailStuckFlag = true;
    nominalInput.mMalfIgnorePower   = true;
    nominalInput.mMalfPerfectSensor = true;
    nominalInput.mMalfFailToValue   = true;
    SensorBooleanBiInputData copyInput(nominalInput);
    CPPUNIT_ASSERT(true  == copyInput.mPowerFlag);
    CPPUNIT_ASSERT(true  == copyInput.mMalfFailToFlag);
    CPPUNIT_ASSERT(true  == copyInput.mMalfFailOffFlag);
    CPPUNIT_ASSERT(true  == copyInput.mMalfFailStuckFlag);
    CPPUNIT_ASSERT(true  == copyInput.mMalfIgnorePower);
    CPPUNIT_ASSERT(true  == copyInput.mMalfPerfectSensor);
    CPPUNIT_ASSERT(true  == copyInput.mTruthInput);
    CPPUNIT_ASSERT(true  == copyInput.mMalfFailToValue);

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

    /// - Test self-assignment of a test input data article.
    CPPUNIT_ASSERT(&defaultInput == &(defaultInput = defaultInput));

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the construction of the Boolean Input Boolean Sensor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorBooleanBi::testConstruction()
{
    std::cout << "\n UtSensorBooleanBi .... 03: testConstruction...........................";

    /// - Test default construction of the test article base class.
    CPPUNIT_ASSERT(""    == tArticle.mName);
    CPPUNIT_ASSERT(false == tArticle.mPowerFlag);
    CPPUNIT_ASSERT(false == tArticle.mDegradedFlag);
    CPPUNIT_ASSERT(false == tArticle.mMalfFailToFlag);
    CPPUNIT_ASSERT(false == tArticle.mMalfFailOffFlag);
    CPPUNIT_ASSERT(false == tArticle.mMalfFailStuckFlag);
    CPPUNIT_ASSERT(false == tArticle.mMalfIgnorePower);
    CPPUNIT_ASSERT(false == tArticle.mMalfPerfectSensor);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);

    /// - Test default construction of the test article.
    CPPUNIT_ASSERT(false == tArticle.mOffValue);
    CPPUNIT_ASSERT(false == tArticle.mTruthInput);
    CPPUNIT_ASSERT(false == tArticle.mMalfFailToValue);
    CPPUNIT_ASSERT(false == tArticle.mSensedOutput);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests nominal initialization of the Boolean Input Boolean Sensor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorBooleanBi::testInitialize()
{
    std::cout << "\n UtSensorBooleanBi .... 04: testInitialize.............................";

    /// - Define nominal config & input data.
    SensorBooleanBiConfigData nominalConfig(true);
    SensorBooleanBiInputData  nominalInput(true, true);

    /// - Test nominal initialization of the test article base class.
    CPPUNIT_ASSERT_NO_THROW(tArticle.initialize(nominalConfig, nominalInput, tName.c_str()));

    CPPUNIT_ASSERT(tName == tArticle.mName);
    CPPUNIT_ASSERT(true  == tArticle.mPowerFlag);
    CPPUNIT_ASSERT(false == tArticle.mDegradedFlag);
    CPPUNIT_ASSERT(false == tArticle.mMalfFailToFlag);
    CPPUNIT_ASSERT(false == tArticle.mMalfFailOffFlag);
    CPPUNIT_ASSERT(false == tArticle.mMalfFailStuckFlag);
    CPPUNIT_ASSERT(false == tArticle.mMalfIgnorePower);
    CPPUNIT_ASSERT(false == tArticle.mMalfPerfectSensor);

    /// - Test nominal initialization of the test article.
    CPPUNIT_ASSERT(true  == tArticle.mOffValue);
    CPPUNIT_ASSERT(true  == tArticle.mTruthInput);
    CPPUNIT_ASSERT(false == tArticle.mMalfFailToValue);
    CPPUNIT_ASSERT(true  == tArticle.mSensedOutput);
    CPPUNIT_ASSERT(true  == tArticle.mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests initialization exceptions of the Boolean Input Boolean Sensor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorBooleanBi::testInitializeExceptions()
{
    std::cout << "\n UtSensorBooleanBi .... 05: testInitializeExceptions...................";

    /// - The class currently has nothing to test.

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the various setter & getter methods of the Boolean Input Boolean Sensor class.
///
/// @details Verifies the following TS21 requirements as they pertain to the sensor model:
///          - R.TS210-0021: TS21 vehicle subsystem models shall provide sensed and actual data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorBooleanBi::testAccessors()
{
    std::cout << "\n UtSensorBooleanBi .... 06: testAccessors..............................";

    /// - Set up the test article with nominal config & input data.
    SensorBooleanBiConfigData nominalConfig(false);
    SensorBooleanBiInputData  nominalInput;
    tArticle.initialize(nominalConfig, nominalInput, tName.c_str());

    /// - Test the methods to set and get the input truth value.
    tArticle.setTruthInput(true);
    CPPUNIT_ASSERT(true == tArticle.getTruthInput());

    /// - Test the method to get the sensed output.
    tArticle.mSensedOutput = true;
    CPPUNIT_ASSERT(true == tArticle.getSensedOutput());

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the update method of the Boolean Input Boolean Sensor class under nominal
///         operation with good power and no malfs.
///
/// @details Verifies the following TS21 requirements as they pertain to the sensor model:
///          - R.TS210-0021: TS21 vehicle subsystem models shall provide sensed and actual data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorBooleanBi::testUpdateNominal()
{
    std::cout << "\n UtSensorBooleanBi .... 07: testUpdateNominal..........................";

    /// - Set up the test article with nominal config & input data.
    SensorBooleanBiConfigData nominalConfig(false);
    SensorBooleanBiInputData  nominalInput(true);
    tArticle.initialize(nominalConfig, nominalInput, tName.c_str());

    /// - Verify sensed output of the sensor for truth input = false.
    tArticle.update(0.0);
    CPPUNIT_ASSERT(false == tArticle.getSensedOutput());

    /// - Verify sensed output of the sensor for truth input = true.
    tArticle.setTruthInput(true);
    tArticle.update(0.0);
    CPPUNIT_ASSERT(true  == tArticle.getSensedOutput());

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the update method of the Boolean Input Boolean Sensor class under nominal
///         operation with no power and no malfs.
///
/// @details Verifies the following TS21 requirements as they pertain to the sensor model:
///          - R.TS210-0021: TS21 vehicle subsystem models shall provide sensed and actual data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorBooleanBi::testUpdateNoPower()
{
    std::cout << "\n UtSensorBooleanBi .... 08: testUpdateNoPower..........................";

    /// - Set up a test article with nominal config & input data.
    SensorBooleanBiConfigData nominalConfig(false);
    SensorBooleanBiInputData  nominalInput(false, true);
    tArticle.initialize(nominalConfig, nominalInput, tName.c_str());

    /// - Verify sensed output of the sensor for truth input = true.
    tArticle.update(0.0);
    CPPUNIT_ASSERT(false == tArticle.getSensedOutput());

    /// - Verify sensed output of the sensor for truth input = false.
    tArticle.setTruthInput(false);
    tArticle.update(0.0);
    CPPUNIT_ASSERT(false == tArticle.getSensedOutput());

    /// - Now try it with sensor configured to output true when off.
    tArticle.mOffValue = true;
    tArticle.update(0.0);
    CPPUNIT_ASSERT(true == tArticle.getSensedOutput());

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the fail-to malfunction in the Boolean Input Boolean Sensor class.
///
/// @details Verifies the following TS21 requirements as they pertain to the sensor model:
///          - R.TS210-0009: TS21 vehicle subsystem sensor models shall have fail, scale, noise,
///                          drift and bias malfunctions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorBooleanBi::testMalfFailTo()
{
    std::cout << "\n UtSensorBooleanBi .... 09: testMalfFailTo.............................";

    /// - Set up a test article with nominal config & input data.
    SensorBooleanBiConfigData nominalConfig(false);
    SensorBooleanBiInputData  nominalInput(true, true);
    tArticle.initialize(nominalConfig, nominalInput, tName.c_str());

    /// - Verify sensed output of sensor for truth input = true but failed to false.
    tArticle.mMalfFailToFlag  = true;
    tArticle.mMalfFailToValue = false;
    tArticle.update(0.0);
    CPPUNIT_ASSERT(false == tArticle.getSensedOutput());
    CPPUNIT_ASSERT(true  == tArticle.mDegradedFlag);

    /// - Verify sensed output of sensor for truth input = false but failed to true.
    tArticle.setTruthInput(false);
    tArticle.mMalfFailToValue = true;
    tArticle.update(0.0);
    CPPUNIT_ASSERT(true  == tArticle.getSensedOutput());
    CPPUNIT_ASSERT(true  == tArticle.mDegradedFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the stuck malfunction in the Boolean Input Boolean Sensor class.
///
/// @details Verifies the following TS21 requirements as they pertain to the sensor model:
///          - R.TS210-0009: TS21 vehicle subsystem sensor models shall have fail, scale, noise,
///                          drift and bias malfunctions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorBooleanBi::testMalfStuck()
{
    std::cout << "\n UtSensorBooleanBi .... 10: testMalfStuck..............................";

    /// - Set up a test article with nominal config & input data.
    SensorBooleanBiConfigData nominalConfig(false);
    SensorBooleanBiInputData  nominalInput(true, true);
    tArticle.initialize(nominalConfig, nominalInput, tName.c_str());

    /// - Verify nominal sensed output of sensor.
    tArticle.update(0.0);
    CPPUNIT_ASSERT(true  == tArticle.getSensedOutput());
    CPPUNIT_ASSERT(false == tArticle.mDegradedFlag);

    /// - Verify sensed output of sensor for truth input switched but the stuck malf active.
    tArticle.setTruthInput(false);
    tArticle.mMalfFailStuckFlag = true;
    tArticle.update(0.0);
    CPPUNIT_ASSERT(true  == tArticle.getSensedOutput());
    CPPUNIT_ASSERT(true  == tArticle.mDegradedFlag);

    /// - Remove the malf and verify the sensor goes back to the new truth value.
    tArticle.mMalfFailStuckFlag = false;
    tArticle.update(0.0);
    CPPUNIT_ASSERT(false == tArticle.getSensedOutput());
    CPPUNIT_ASSERT(false == tArticle.mDegradedFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the perfect sensor malfunction in the Boolean Input Boolean Sensor class.
///
/// @details Verifies the following TS21 requirements as they pertain to the sensor model:
///          - R.TS210-0009: TS21 vehicle subsystem sensor models shall have fail, scale, noise,
///                          drift and bias malfunctions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorBooleanBi::testMalfPerfect()
{
    std::cout << "\n UtSensorBooleanBi .... 11: testMalfPerfect............................";

    /// - Set up a test article with nominal config & input data.
    SensorBooleanBiConfigData nominalConfig(false);
    SensorBooleanBiInputData  nominalInput(false, true);
    tArticle.initialize(nominalConfig, nominalInput, tName.c_str());

    /// - Verify nominal sensed output of sensor with power off and no malfs.
    tArticle.update(0.0);
    CPPUNIT_ASSERT(false == tArticle.getSensedOutput());

    /// - Verify the perfect sensor malf overrides power.
    tArticle.mMalfPerfectSensor = true;
    tArticle.update(0.0);
    CPPUNIT_ASSERT(true  == tArticle.getSensedOutput());
    CPPUNIT_ASSERT(false == tArticle.mDegradedFlag);

    /// - Verify the perfect sensor malf overrides the stuck malf.
    tArticle.mMalfFailStuckFlag = true;
    tArticle.setTruthInput(false);
    tArticle.update(0.0);
    CPPUNIT_ASSERT(false == tArticle.getSensedOutput());
    CPPUNIT_ASSERT(false == tArticle.mDegradedFlag);

    /// - Verify the perfect sensor malf overrides the fail-to malf.
    tArticle.mMalfFailStuckFlag = false;
    tArticle.mMalfFailToFlag = true;
    tArticle.mMalfFailToValue = true;
    tArticle.update(0.0);
    CPPUNIT_ASSERT(false == tArticle.getSensedOutput());
    CPPUNIT_ASSERT(false == tArticle.mDegradedFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests interactions of malfunctions in the Boolean Input Boolean Sensor class that have
///         not been covered in previous tests.
///
/// @details Verifies the following TS21 requirements as they pertain to the sensor model:
///          - R.TS210-0009: TS21 vehicle subsystem sensor models shall have fail, scale, noise,
///                          drift and bias malfunctions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorBooleanBi::testMalfInteractions()
{
    std::cout << "\n UtSensorBooleanBi .... 12: testMalfInteractions.......................";

    /// - Set up a test article with nominal config & input data.
    SensorBooleanBiConfigData nominalConfig(false);
    SensorBooleanBiInputData  nominalInput(true, true);
    tArticle.initialize(nominalConfig, nominalInput, tName.c_str());

    /// - Verify nominal sensed output of sensor with power off and no malfs.
    tArticle.update(0.0);
    CPPUNIT_ASSERT(true == tArticle.getSensedOutput());

    /// - Verify the fail-to malf overrides the stuck malf.
    tArticle.setTruthInput(false);
    tArticle.mMalfFailStuckFlag = true;
    tArticle.mMalfFailToFlag    = true;
    tArticle.mMalfFailToValue   = false;
    tArticle.update(0.0);
    CPPUNIT_ASSERT(false == tArticle.getSensedOutput());
    CPPUNIT_ASSERT(true  == tArticle.mDegradedFlag);

    /// - Verify that power overrides the fail-to malf.
    tArticle.mMalfFailStuckFlag = false;
    tArticle.mMalfFailToValue   = true;
    tArticle.mMalfFailOffFlag   = true;
    tArticle.setTruthInput(true);
    tArticle.update(0.0);
    CPPUNIT_ASSERT(false == tArticle.getSensedOutput());
    CPPUNIT_ASSERT(true  == tArticle.mDegradedFlag);

    /// - Verify that power overrides the stuck malf.
    tArticle.setTruthInput(false);
    tArticle.mMalfFailStuckFlag = true;
    tArticle.update(0.0);
    CPPUNIT_ASSERT(false == tArticle.getSensedOutput());
    CPPUNIT_ASSERT(true  == tArticle.mDegradedFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Test the sense convenience method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorBooleanBi::testSense()
{
    std::cout << "\n UtSensorBooleanBi .... 13: testSense..................................";

    /// - Set up the test article with nominal config & input data.
    SensorBooleanBiConfigData nominalConfig(false);
    SensorBooleanBiInputData  nominalInput(true);
    tArticle.initialize(nominalConfig, nominalInput, tName.c_str());

    /// - Verify sensed output of the sensor for truth input = true.
    CPPUNIT_ASSERT(tArticle.sense(0.0, true, true));

    std::cout << "... Pass.";
    std::cout << "\n--------------------------------------------------------------------------------";
}
