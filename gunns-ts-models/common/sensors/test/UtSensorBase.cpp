/****************************** TRICK HEADER ******************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
 ((common/sensors/SensorBase.o))

PROGRAMMERS:
 ((Jason Harvey) (L-3 Communications) (2012-04) (TS21) (Initial))

 **************************************************************************************************/

#include "UtSensorBase.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
UtSensorBase::UtSensorBase()
    :
    tArticle(),
    tName()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
UtSensorBase::~UtSensorBase()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  unit test infrastructure
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorBase::setUp()
{
    /// - Define nominal article name.
    tName = "Test Sensor";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  unit test infrastructure
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorBase::tearDown()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the constructors & destructors of the Sensor Base config data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorBase::testConfigData()
{
    std::cout << "\n--------------------------------------------------------------------------------";
    std::cout << "\n UtSensorBase ......... 01: testConfigData.............................";

    /// - Test assignment of a test config data article.
    SensorBaseConfigData defaultConfig, nominalConfig;
    defaultConfig = nominalConfig;
    // currently nothing to check

    /// - Test self-assignment of a test config data article.
    CPPUNIT_ASSERT(&defaultConfig == &(defaultConfig = defaultConfig));

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the constructors & destructors of the Sensor Base input data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorBase::testInputData()
{
    std::cout << "\n UtSensorBase ......... 02: testInputData..............................";

    /// - Test default construction of a test input data article.
    SensorBaseInputData defaultInput;
    CPPUNIT_ASSERT(false == defaultInput.mPowerFlag);
    CPPUNIT_ASSERT(false == defaultInput.mMalfFailToFlag);
    CPPUNIT_ASSERT(false == defaultInput.mMalfFailOffFlag);
    CPPUNIT_ASSERT(false == defaultInput.mMalfFailStuckFlag);
    CPPUNIT_ASSERT(false == defaultInput.mMalfIgnorePower);
    CPPUNIT_ASSERT(false == defaultInput.mMalfPerfectSensor);

    /// - Test nominal construction of a test input data article.
    SensorBaseInputData nominalInput(true);
    CPPUNIT_ASSERT(true  == nominalInput.mPowerFlag);
    CPPUNIT_ASSERT(false == nominalInput.mMalfFailToFlag);
    CPPUNIT_ASSERT(false == nominalInput.mMalfFailOffFlag);
    CPPUNIT_ASSERT(false == nominalInput.mMalfFailStuckFlag);
    CPPUNIT_ASSERT(false == nominalInput.mMalfIgnorePower);
    CPPUNIT_ASSERT(false == nominalInput.mMalfPerfectSensor);

    /// - Test copy construction of a test input data article.
    nominalInput.mMalfFailToFlag    = true;
    nominalInput.mMalfFailOffFlag   = true;
    nominalInput.mMalfFailStuckFlag = true;
    nominalInput.mMalfIgnorePower   = true;
    nominalInput.mMalfPerfectSensor = true;
    SensorBaseInputData copyInput(nominalInput);
    CPPUNIT_ASSERT(true  == copyInput.mPowerFlag);
    CPPUNIT_ASSERT(true  == copyInput.mMalfFailToFlag);
    CPPUNIT_ASSERT(true  == copyInput.mMalfFailOffFlag);
    CPPUNIT_ASSERT(true  == copyInput.mMalfFailStuckFlag);
    CPPUNIT_ASSERT(true  == copyInput.mMalfIgnorePower);
    CPPUNIT_ASSERT(true  == copyInput.mMalfPerfectSensor);

    /// - Test assignment of a test input data article.
    defaultInput = nominalInput;
    CPPUNIT_ASSERT(true  == defaultInput.mPowerFlag);
    CPPUNIT_ASSERT(true  == defaultInput.mMalfFailToFlag);
    CPPUNIT_ASSERT(true  == defaultInput.mMalfFailOffFlag);
    CPPUNIT_ASSERT(true  == defaultInput.mMalfFailStuckFlag);
    CPPUNIT_ASSERT(true  == defaultInput.mMalfIgnorePower);
    CPPUNIT_ASSERT(true  == defaultInput.mMalfPerfectSensor);

    /// - Test self-assignment of a test input data article.
    CPPUNIT_ASSERT(&defaultInput == &(defaultInput = defaultInput));

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the construction of the Sensor Base class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorBase::testConstruction()
{
    std::cout << "\n UtSensorBase ......... 03: testConstruction...........................";

    /// - Test default construction of a test article.
    CPPUNIT_ASSERT(""    == tArticle.mName);
    CPPUNIT_ASSERT(false == tArticle.mPowerFlag);
    CPPUNIT_ASSERT(false == tArticle.mDegradedFlag);
    CPPUNIT_ASSERT(false == tArticle.mMalfFailToFlag);
    CPPUNIT_ASSERT(false == tArticle.mMalfFailOffFlag);
    CPPUNIT_ASSERT(false == tArticle.mMalfFailStuckFlag);
    CPPUNIT_ASSERT(false == tArticle.mMalfIgnorePower);
    CPPUNIT_ASSERT(false == tArticle.mMalfPerfectSensor);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests nominal initialization of the Sensor Base class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorBase::testInitialize()
{
    std::cout << "\n UtSensorBase ......... 04: testInitialize.............................";

    /// - Define config & input data.
    SensorBaseConfigData nominalConfig;
    SensorBaseInputData  nominalInput(true);
    nominalInput.mMalfFailToFlag    = true;
    nominalInput.mMalfFailOffFlag   = true;
    nominalInput.mMalfFailStuckFlag = true;
    nominalInput.mMalfIgnorePower   = true;
    nominalInput.mMalfPerfectSensor = true;

    /// - Test nominal initialization of a test article.
    CPPUNIT_ASSERT_NO_THROW(tArticle.initialize(nominalConfig, nominalInput, tName.c_str()));

    CPPUNIT_ASSERT(tName == tArticle.mName);
    CPPUNIT_ASSERT(true  == tArticle.mPowerFlag);
    CPPUNIT_ASSERT(false == tArticle.mDegradedFlag);
    CPPUNIT_ASSERT(true  == tArticle.mMalfFailToFlag);
    CPPUNIT_ASSERT(true  == tArticle.mMalfFailOffFlag);
    CPPUNIT_ASSERT(true  == tArticle.mMalfFailStuckFlag);
    CPPUNIT_ASSERT(true  == tArticle.mMalfIgnorePower);
    CPPUNIT_ASSERT(true  == tArticle.mMalfPerfectSensor);
    CPPUNIT_ASSERT(true  == tArticle.mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests initialization exceptions of the Sensor Base class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorBase::testInitializeExceptions()
{
    std::cout << "\n UtSensorBase ......... 05: testInitializeExceptions...................";

    /// - Test an exception is thrown when initializing with no name supplied.
    SensorBaseConfigData nominalConfig;
    SensorBaseInputData  nominalInput(true);
    CPPUNIT_ASSERT_THROW(tArticle.initialize(nominalConfig, nominalInput, ""),
                         TsInitializationException);

    CPPUNIT_ASSERT(false == tArticle.mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the various setter & getter methods of the Sensor Base class.
///
/// @details Verifies the following TS21 requirements as they pertain to the sensor model:
///          - R.TS210-0011: TS21 vehicle subsystem models shall receive the current state of the
///                          electrical network system.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorBase::testAccessors()
{
    std::cout << "\n UtSensorBase ......... 06: testAccessors..............................";

    /// - Set up a test article with nominal config & input data.
    SensorBaseConfigData nominalConfig;
    SensorBaseInputData  nominalInput(false);
    tArticle.initialize(nominalConfig, nominalInput, tName.c_str());

    /// - Test the method to return the initialized flag.
    CPPUNIT_ASSERT(true == tArticle.isInitialized());

    /// - Set the sensor power flag via the setter method and verify.
    tArticle.setPowerFlag(true);
    CPPUNIT_ASSERT(true == tArticle.mPowerFlag);

    /// - Test the method to return the sensor name.
    CPPUNIT_ASSERT(tName == tArticle.getName());

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the determinePower method of the Sensor Base class.
///
/// @details Verifies the following TS21 requirements as they pertain to the sensor model:
///          - R.TS210-0004: TS21 vehicle subsystem models shall provide an override for nominal
///                          component functionality regardless of power availability.
///          - R.TS210-0005: TS21 vehicle subsystem models shall provide an override for preventing
///                          nominal component functionality regardless of power availability.
///          - R.TS210-0009: TS21 vehicle subsystem sensor models shall have fail, scale, noise,
///                          drift and bias malfunctions.
///          - R.TS210-0011: TS21 vehicle subsystem models shall receive the current state of the
///                          electrical network system.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorBase::testDeterminePower()
{
    std::cout << "\n UtSensorBase ......... 07: testDeterminePower.........................";

    /// - Set up a test article with nominal config & input data in a powered state.
    SensorBaseConfigData nominalConfig;
    SensorBaseInputData  nominalInput(true);
    tArticle.initialize(nominalConfig, nominalInput, tName.c_str());

    /// - Verify the determinePower method indicates the sensor is powered.
    CPPUNIT_ASSERT(true == tArticle.determinePower());

    /// - Set the sensor power flag false and verify determinePower indicates the sensor is off.
    tArticle.setPowerFlag(false);
    CPPUNIT_ASSERT(false == tArticle.determinePower());

    /// - Now set the ignore power malfunction and verify the sensor goes back to being on.
    tArticle.mMalfIgnorePower = true;
    CPPUNIT_ASSERT(true == tArticle.determinePower());

    /// - Now set the failed off malfunction and verify the sensor switches back off.
    tArticle.mMalfFailOffFlag = true;
    CPPUNIT_ASSERT(false == tArticle.determinePower());

    std::cout << "... Pass.";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests initialization of the mDegradedFlag attribute of the Sensor Base class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorBase::testDegradation()
{
    std::cout << "\n UtSensorBase ......... 08: testDegradation............................";

    /// - Set up a test article with nominal config & input data.
    SensorBaseConfigData nominalConfig;
    SensorBaseInputData  nominalInput(true);
    tArticle.initialize(nominalConfig, nominalInput, tName.c_str());

    /// - Verify no degrade indicated when no malfs are active.
    CPPUNIT_ASSERT(false == tArticle.mDegradedFlag);

    /// - Verify degrade indicated when any of the output malfs are active.
    nominalInput.mMalfFailOffFlag   = true;
    tArticle.initialize(nominalConfig, nominalInput, tName.c_str());
    CPPUNIT_ASSERT(true  == tArticle.mDegradedFlag);

    nominalInput.mMalfFailOffFlag   = false;
    nominalInput.mMalfFailToFlag    = true;
    tArticle.initialize(nominalConfig, nominalInput, tName.c_str());
    CPPUNIT_ASSERT(true  == tArticle.mDegradedFlag);

    nominalInput.mMalfFailToFlag    = false;
    nominalInput.mMalfFailStuckFlag = true;
    tArticle.initialize(nominalConfig, nominalInput, tName.c_str());
    CPPUNIT_ASSERT(true  == tArticle.mDegradedFlag);

    /// - Verify no degrade indicated when the perfect sensor malf is active and that it overrides
    ///   the other malfs.
    nominalInput.mMalfFailOffFlag   = true;
    nominalInput.mMalfFailToFlag    = true;
    nominalInput.mMalfPerfectSensor    = true;
    tArticle.initialize(nominalConfig, nominalInput, tName.c_str());
    CPPUNIT_ASSERT(false == tArticle.mDegradedFlag);

    std::cout << "... Pass.";
    std::cout << "\n--------------------------------------------------------------------------------";
}
