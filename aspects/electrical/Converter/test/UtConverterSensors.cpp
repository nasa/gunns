/*********************** TRICK HEADER *************************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((common/sensors/TsNoise.o)
     (ConverterSensors.o))
 **************************************************************************************************/

#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "common/sensors/TsNoise.hh"

#include "UtConverterSensors.hh"

/// @details  Test identification number.
int UtConverterSensors::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  UtConverterSensors constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
UtConverterSensors::UtConverterSensors()
    :
    CppUnit::TestFixture(),
    mStandbyPower(0.0),
    mInputVoltage(0.0),
    mOutputPower(0.0),
    mRegulatedVoltage(0.0),
    mEfficiency(0.0),
    mOutputOverCurrentLimit(0.0),
    mOutputOverVoltageLimit(0.0),
    mOutOverCurrentTripActive(false),
    mOutOverVoltageTripActive(false),
    mInputOverVoltageLimit(0.0),
    mInputUnderVoltageLimit(0.0),
    mInOverVoltageTripActive(false),
    mInUnderVoltageTripActive(false),
    mOutVoltageSensorConfig(),
    mOutCurrentSensorConfig(),
    mConfig(0),
    mOutVoltageSensorInput(),
    mOutCurrentSensorInput(),
    mInput(0),
    mArticle(0),
    mTimeStep(0.0),
    mTolerance(0.0) {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Default destructs this ISS EVAS Power Supply Assembly Converter Model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtConverterSensors::~UtConverterSensors() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterSensors::setUp() {
    mStandbyPower                                       = 5.0;
    mInputVoltage                                       = 120.0;
    mOutputPower                                        = 50.0;
    mRegulatedVoltage                                   = 18.5;
    mEfficiency                                         = 0.95;
    mOutputOverCurrentLimit                             = 10.50;
    mOutputOverVoltageLimit                             = 22.0;
    mOutOverCurrentTripActive                           = true;
    mOutOverVoltageTripActive                           = true;
    mInputOverVoltageLimit                              = 150.0;
    mInputUnderVoltageLimit                             = 90.0;
    mInOverVoltageTripActive                            = false;
    mInOverVoltageTripActive                            = false;

    mOutVoltageSensorConfig.mMaxRange                   = 250.0;
    mOutVoltageSensorConfig.mNominalResolution          = 0.01;
    mOutVoltageSensorConfig.mNominalScale               = 1.0;
    mOutVoltageSensorConfig.mNominalNoiseScale          = 0.555;
    mOutVoltageSensorConfig.mNoiseFunction              = TsNoise::getNoise;

    mOutCurrentSensorConfig.mMaxRange                   = 100.0;
    mOutCurrentSensorConfig.mNominalResolution          = 0.01;
    mOutCurrentSensorConfig.mNominalScale               = 1.0;
    mOutCurrentSensorConfig.mNominalNoiseScale          = 0.225;
    mOutCurrentSensorConfig.mNoiseFunction              = TsNoise::getNoise;

    mConfig                 = new ConverterSensorsConfigData(mStandbyPower,
                                                                      mOutVoltageSensorConfig,
                                                                      mOutCurrentSensorConfig);
    mInput                  = new ConverterSensorsInputData(mInputVoltage,
                                                                     mOutputPower,
                                                                     mRegulatedVoltage,
                                                                     mEfficiency,
                                                                     mOutputOverCurrentLimit,
                                                                     mOutputOverVoltageLimit,
                                                                     mOutOverCurrentTripActive,
                                                                     mOutOverVoltageTripActive,
                                                                     mInputOverVoltageLimit,
                                                                     mInputUnderVoltageLimit,
                                                                     mInOverVoltageTripActive,
                                                                     mInOverVoltageTripActive);
    mArticle                = new FriendlyConverterSensors;
    mTimeStep               = 0.1;
    mTolerance              = 1.0e-08;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterSensors::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete mArticle;
    delete mInput;
    delete mConfig;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Tests for ISS EVAS Power Supply Assembly Converter configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterSensors::testConfigAndInput()
{
    UT_RESULT_FIRST;

    /// - Check nominal config construction
    CPPUNIT_ASSERT(mStandbyPower == mConfig->mStandbyPower);


     /// - Check default config construction
    ConverterSensorsConfigData  defaultConfig;

    CPPUNIT_ASSERT(0.0 == defaultConfig.mStandbyPower);
    CPPUNIT_ASSERT(0.0 == static_cast<double>(defaultConfig.outVoltageSensor.mMaxRange));
    CPPUNIT_ASSERT(0.0 == static_cast<double>(defaultConfig.outVoltageSensor.mNominalResolution));
    CPPUNIT_ASSERT(1.0 == static_cast<double>(defaultConfig.outVoltageSensor.mNominalScale));
    CPPUNIT_ASSERT(0.0 == static_cast<double>(defaultConfig.outVoltageSensor.mNominalNoiseScale));
    CPPUNIT_ASSERT(0.0 == static_cast<double>(defaultConfig.outCurrentSensor.mMaxRange));
    CPPUNIT_ASSERT(0.0 == static_cast<double>(defaultConfig.outCurrentSensor.mNominalResolution ));
    CPPUNIT_ASSERT(1.0 == static_cast<double>(defaultConfig.outCurrentSensor.mNominalScale));
    CPPUNIT_ASSERT(0.0 == static_cast<double>(defaultConfig.outCurrentSensor.mNominalNoiseScale));

    /// - Check copy config construction
    ConverterSensorsConfigData  copyConfig(*mConfig);

    CPPUNIT_ASSERT(mStandbyPower == copyConfig.mStandbyPower);

    /// - Check creating new instance and then delete for code coverage
    ConverterSensorsConfigData * newConfig = new ConverterSensorsConfigData();
    delete newConfig;

    /// - Check nominal input construction
    CPPUNIT_ASSERT(mInputVoltage                == mInput->mInputVoltage);
    CPPUNIT_ASSERT(mOutputPower                 == mInput->mOutputPower);
    CPPUNIT_ASSERT(mRegulatedVoltage            == mInput->mRegulatedVoltage);
    CPPUNIT_ASSERT(mEfficiency                  == mInput->mEfficiency);
    CPPUNIT_ASSERT(mOutputOverCurrentLimit      == mInput->mOutputOverCurrentLimit);
    CPPUNIT_ASSERT(mOutputOverVoltageLimit      == mInput->mOutputOverVoltageLimit);
    CPPUNIT_ASSERT(mOutOverCurrentTripActive    == mInput->mOutOverCurrentTripActive);
    CPPUNIT_ASSERT(mOutOverVoltageTripActive    == mInput->mOutOverVoltageTripActive);
    CPPUNIT_ASSERT(mInputOverVoltageLimit       == mInput->mInputOverVoltageLimit);
    CPPUNIT_ASSERT(mInputUnderVoltageLimit      == mInput->mInputUnderVoltageLimit);
    CPPUNIT_ASSERT(mInOverVoltageTripActive     == mInput->mInOverVoltageTripActive);
    CPPUNIT_ASSERT(mInOverVoltageTripActive     == mInput->mInUnderVoltageTripActive);

    /// - Check default input construction
    ConverterSensorsInputData  defaultInput;

    CPPUNIT_ASSERT(0.0     == defaultInput.mInputVoltage);
    CPPUNIT_ASSERT(0.0     == defaultInput.mOutputPower);
    CPPUNIT_ASSERT(0.0     == defaultInput.mRegulatedVoltage);
    CPPUNIT_ASSERT(0.0     == defaultInput.mEfficiency);
    CPPUNIT_ASSERT(0.0     == defaultInput.mOutputOverCurrentLimit);
    CPPUNIT_ASSERT(0.0     == defaultInput.mOutputOverVoltageLimit);
    CPPUNIT_ASSERT(false   == defaultInput.mOutOverCurrentTripActive);
    CPPUNIT_ASSERT(false   == defaultInput.mOutOverVoltageTripActive);
    CPPUNIT_ASSERT(0.0     == defaultInput.mInputOverVoltageLimit);
    CPPUNIT_ASSERT(0.0     == defaultInput.mInputUnderVoltageLimit);
    CPPUNIT_ASSERT(false   == defaultInput.mInOverVoltageTripActive);
    CPPUNIT_ASSERT(false   == defaultInput.mInUnderVoltageTripActive);

    /// - Check copy input construction
    ConverterSensorsInputData   copyInput(*mInput);

    CPPUNIT_ASSERT(mInputVoltage              == copyInput.mInputVoltage);
    CPPUNIT_ASSERT(mOutputPower               == copyInput.mOutputPower);
    CPPUNIT_ASSERT(mRegulatedVoltage          == copyInput.mRegulatedVoltage);
    CPPUNIT_ASSERT(mEfficiency                == copyInput.mEfficiency);
    CPPUNIT_ASSERT(mOutputOverCurrentLimit    == copyInput.mOutputOverCurrentLimit);
    CPPUNIT_ASSERT(mOutputOverVoltageLimit    == copyInput.mOutputOverVoltageLimit);
    CPPUNIT_ASSERT(mOutOverCurrentTripActive  == copyInput.mOutOverCurrentTripActive);
    CPPUNIT_ASSERT(mOutOverVoltageTripActive  == copyInput.mOutOverVoltageTripActive);
    CPPUNIT_ASSERT(mInputOverVoltageLimit     == copyInput.mInputOverVoltageLimit);
    CPPUNIT_ASSERT(mInputUnderVoltageLimit    == copyInput.mInputUnderVoltageLimit);
    CPPUNIT_ASSERT(mInOverVoltageTripActive   == copyInput.mInOverVoltageTripActive);
    CPPUNIT_ASSERT(mInOverVoltageTripActive   == copyInput.mInUnderVoltageTripActive);

    /// - Check creating new instance and then delete for code coverage
    ConverterSensorsInputData * newInput = new ConverterSensorsInputData();
    delete newInput;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Test default construction
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterSensors::testDefaultConstruction() {
    UT_RESULT;

    /// @test default constructed values

    CPPUNIT_ASSERT(0.0      == mArticle->mStandbyPower);
    CPPUNIT_ASSERT(           !mArticle->outVoltageSensor.isInitialized());
    CPPUNIT_ASSERT(           !mArticle->outCurrentSensor.isInitialized());
    CPPUNIT_ASSERT(0.0      == mArticle->mInputVoltage);
    CPPUNIT_ASSERT(0.0      == mArticle->mOutputPower);
    CPPUNIT_ASSERT(0.0      == mArticle->mRegulatedVoltage);
    CPPUNIT_ASSERT(0.0      == mArticle->mEfficiency);
    CPPUNIT_ASSERT(0.0      == mArticle->mOutputOverCurrentLimit);
    CPPUNIT_ASSERT(0.0      == mArticle->mOutputOverVoltageLimit);
    CPPUNIT_ASSERT(false    == mArticle->mOutOverCurrentTripActive);
    CPPUNIT_ASSERT(false    == mArticle->mOutOverVoltageTripActive);
    CPPUNIT_ASSERT(0.0      == mArticle->mInputOverVoltageLimit);
    CPPUNIT_ASSERT(0.0      == mArticle->mInputUnderVoltageLimit);
    CPPUNIT_ASSERT(false    == mArticle->mOutOverCurrentTripActive);
    CPPUNIT_ASSERT(false    == mArticle->mOutOverVoltageTripActive);
    CPPUNIT_ASSERT(false    == mArticle->mOutputOverCurrentTrip);
    CPPUNIT_ASSERT(false    == mArticle->mOutputOverVoltageTrip);
    CPPUNIT_ASSERT(false    == mArticle->mInputOverVoltageTrip);
    CPPUNIT_ASSERT(false    == mArticle->mInputUnderVoltageTrip);
    CPPUNIT_ASSERT(false    == mArticle->mResetTrips);
    CPPUNIT_ASSERT(false    == mArticle->mTripOnSensedValue);
    CPPUNIT_ASSERT(false    == mArticle->mConverterOn);
    CPPUNIT_ASSERT(false    == mArticle->mBadEfficiency);
    CPPUNIT_ASSERT(0.0      == mArticle->mInputCurrent);
    CPPUNIT_ASSERT(0.0      == mArticle->mOutputCurrent);
    CPPUNIT_ASSERT(0.0      == mArticle->mOutputVoltage);
    CPPUNIT_ASSERT(0.0      == mArticle->mInputPower);
    CPPUNIT_ASSERT(0.0      == mArticle->mHeatDissipation);
    CPPUNIT_ASSERT(false    == mArticle->mMalfOverrideOutOverCurrentLimit);
    CPPUNIT_ASSERT(0.0      == mArticle->mMalfOverrideOutOverCurrentValue);
    CPPUNIT_ASSERT(false    == mArticle->mMalfOverrideOutOverVoltageLimit);
    CPPUNIT_ASSERT(0.0      == mArticle->mMalfOverrideOutOverVoltageValue);
    CPPUNIT_ASSERT(false    == mArticle->mMalfOverrideOutVoltage);
    CPPUNIT_ASSERT(0.0      == mArticle->mMalfOverrideOutVoltageValue);
    CPPUNIT_ASSERT(false    == mArticle->mMalfConverterFailed);
    CPPUNIT_ASSERT(0.0      == mArticle->mDefaultOutOverCurrentLimit);
    CPPUNIT_ASSERT(0.0      == mArticle->mDefaultOutOverVoltageLimit);
    CPPUNIT_ASSERT(0.0      == mArticle->mDefaultOutVoltage);
    CPPUNIT_ASSERT(false    == mArticle->mConverterFailed);
    CPPUNIT_ASSERT(false    == mArticle->mConverterOnCmd);
    CPPUNIT_ASSERT(false    == mArticle->mPrevConverterOnCmd);
    CPPUNIT_ASSERT(0.0      == mArticle->mOutputCurrentSensed);
    CPPUNIT_ASSERT(0.0      == mArticle->mOutputVoltageSensed);
    CPPUNIT_ASSERT(false    == mArticle->mInitFlag);

    /// @test new/delete for code coverage
    ConverterSensors* article = new ConverterSensors();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Test nominal initialization
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterSensors::testNominalInitialization() {
    UT_RESULT;

    /// - Default construct and initialize (with nominal data) a test article
    mArticle->initialize(*mConfig, *mInput);

    /// - Check nominal initialization data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mStandbyPower, mArticle->mStandbyPower, mTolerance);
    CPPUNIT_ASSERT_EQUAL(true, mArticle->outVoltageSensor.isInitialized());
    CPPUNIT_ASSERT_EQUAL(true, mArticle->outCurrentSensor.isInitialized());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputVoltage, mArticle->mInputVoltage, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mOutputPower, mArticle->mOutputPower, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mRegulatedVoltage, mArticle->mRegulatedVoltage, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mEfficiency, mArticle->mEfficiency, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mOutputOverCurrentLimit, mArticle->mOutputOverCurrentLimit, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mOutputOverVoltageLimit, mArticle->mOutputOverVoltageLimit, mTolerance);
    CPPUNIT_ASSERT_EQUAL(mOutOverCurrentTripActive, mArticle->mOutOverCurrentTripActive);
    CPPUNIT_ASSERT_EQUAL(mOutOverVoltageTripActive, mArticle->mOutOverVoltageTripActive);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputOverVoltageLimit, mArticle->mInputOverVoltageLimit, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputUnderVoltageLimit, mArticle->mInputUnderVoltageLimit, mTolerance);
    CPPUNIT_ASSERT_EQUAL(mInOverVoltageTripActive, mArticle->mInOverVoltageTripActive);
    CPPUNIT_ASSERT_EQUAL(mInUnderVoltageTripActive, mArticle->mInUnderVoltageTripActive);
    CPPUNIT_ASSERT_EQUAL(false, mArticle->mOutputOverCurrentTrip);
    CPPUNIT_ASSERT_EQUAL(false, mArticle->mOutputOverVoltageTrip);
    CPPUNIT_ASSERT_EQUAL(false, mArticle->mInputOverVoltageTrip);
    CPPUNIT_ASSERT_EQUAL(false, mArticle->mInputUnderVoltageTrip);
    CPPUNIT_ASSERT_EQUAL(false, mArticle->mResetTrips);
    CPPUNIT_ASSERT_EQUAL(false, mArticle->mTripOnSensedValue);
    CPPUNIT_ASSERT_EQUAL(false, mArticle->mConverterOn);
    CPPUNIT_ASSERT_EQUAL(false, mArticle->mBadEfficiency);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mInputCurrent, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mOutputCurrent, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mOutputVoltage, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mInputPower, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mHeatDissipation, mTolerance);
    CPPUNIT_ASSERT_EQUAL(false, mArticle->mMalfOverrideOutOverCurrentLimit);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mMalfOverrideOutOverCurrentValue, mTolerance);
    CPPUNIT_ASSERT_EQUAL(false, mArticle->mMalfOverrideOutOverVoltageLimit);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mMalfOverrideOutOverVoltageValue, mTolerance);
    CPPUNIT_ASSERT_EQUAL(false, mArticle->mMalfOverrideOutVoltage);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mMalfOverrideOutVoltageValue, mTolerance);
    CPPUNIT_ASSERT_EQUAL(false, mArticle->mMalfConverterFailed);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mOutputOverCurrentLimit, mArticle->mDefaultOutOverCurrentLimit, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mOutputOverVoltageLimit, mArticle->mDefaultOutOverVoltageLimit, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mRegulatedVoltage, mArticle->mDefaultOutVoltage, mTolerance);
    CPPUNIT_ASSERT_EQUAL(false, mArticle->mConverterFailed);
    CPPUNIT_ASSERT_EQUAL(false, mArticle->mConverterOnCmd);
    CPPUNIT_ASSERT_EQUAL(false, mArticle->mPrevConverterOnCmd);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mOutputCurrentSensed, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mOutputVoltageSensed, mTolerance);
    CPPUNIT_ASSERT_EQUAL(true, mArticle->mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Test getters
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterSensors::testAccessors() {
    UT_RESULT;

    /// - Initialization default test article with nominal data
    mArticle->initialize(*mConfig, *mInput);

    /// @test all the getters
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputVoltage, mArticle->getInputVoltage(), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,   mArticle->getOutputVoltage(), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,   mArticle->getOutputCurrent(), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mStandbyPower, mArticle->mStandbyPower, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->getInputPower(), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mOutputPower, mArticle->getOutputPower(), mTolerance);
    CPPUNIT_ASSERT_EQUAL(false, mArticle->isConverterOn());
    CPPUNIT_ASSERT_EQUAL(false, mArticle->isOutputOverCurrentTrip());
    CPPUNIT_ASSERT_EQUAL(false, mArticle->isOutputOverVoltageTrip());
    CPPUNIT_ASSERT_EQUAL(false, mArticle->isInputOverVoltageTrip());
    CPPUNIT_ASSERT_EQUAL(false, mArticle->isInputUnderVoltageTrip());
    CPPUNIT_ASSERT_EQUAL(false, mArticle->isTripOnSensedValue());
    CPPUNIT_ASSERT_EQUAL(false, mArticle->mBadEfficiency);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,   mArticle->getSensedOutputCurrent(), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,   mArticle->getSensedOutputVoltage(), mTolerance);
    CPPUNIT_ASSERT_EQUAL(false, mArticle->isConverterCmdOn());
    CPPUNIT_ASSERT_EQUAL(false, mArticle->isTripped());
    CPPUNIT_ASSERT_EQUAL(false, mArticle->isConverterFailed());
    CPPUNIT_ASSERT_EQUAL(true, mArticle->isInitialized());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Test setters
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterSensors::testModifiers() {
    UT_RESULT;

    /// - Default construct a test article
    FriendlyConverterSensors article;

    /// @test all the setters
    article.setConverterState(true);
    CPPUNIT_ASSERT_EQUAL(true, article.mConverterOn);
    CPPUNIT_ASSERT_EQUAL(true, article.isConverterOn());
    article.setInputVoltage(mInputVoltage);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputVoltage, article.mInputVoltage, mTolerance);
    article.setRegulatedVoltage(mRegulatedVoltage);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mRegulatedVoltage, article.mRegulatedVoltage, mTolerance);
    article.setEfficiency(mEfficiency);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mEfficiency, article.mEfficiency, mTolerance);
    article.setOutputOverCurrentLimit(mOutputOverCurrentLimit);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mOutputOverCurrentLimit, article.mOutputOverCurrentLimit, mTolerance);
    article.setOutputOverVoltageLimit(mOutputOverVoltageLimit);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mOutputOverVoltageLimit, article.mOutputOverVoltageLimit, mTolerance);
    article.mInputOverVoltageLimit = mInputOverVoltageLimit;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputOverVoltageLimit, article.mInputOverVoltageLimit, mTolerance);
    article.mInputUnderVoltageLimit = mInputUnderVoltageLimit;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputUnderVoltageLimit, article.mInputUnderVoltageLimit, mTolerance);
    article.mOutputVoltage = mRegulatedVoltage;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mRegulatedVoltage, article.mOutputVoltage, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mRegulatedVoltage, article.getOutputVoltage(), mTolerance);
    article.setOutOverCurrentTripActive(true);
    CPPUNIT_ASSERT_EQUAL(true, article.mOutOverCurrentTripActive);
    article.mOutOverVoltageTripActive = true;
    CPPUNIT_ASSERT_EQUAL(true, article.mOutOverVoltageTripActive);
    article.mInOverVoltageTripActive = true;
    CPPUNIT_ASSERT_EQUAL(true, article.mInOverVoltageTripActive);
    article.mInUnderVoltageTripActive = true;
    CPPUNIT_ASSERT_EQUAL(true, article.mInUnderVoltageTripActive);

    double loadPower = 100.0;
    mArticle->setLoad(loadPower);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(loadPower, mArticle->mOutputPower, mTolerance);

    bool flag = true;
    mArticle->setConverterOn(flag);
    CPPUNIT_ASSERT_EQUAL(flag, mArticle->mConverterOnCmd);
    mArticle->updateConverterState();
    CPPUNIT_ASSERT_EQUAL(flag, mArticle->isConverterOn());


    mArticle->mOutOverCurrentTripActive = true;
    mArticle->mOutOverVoltageTripActive = true;
    mArticle->mOutputVoltageSensed   = 32.0;
    mArticle->mOutputCurrentSensed   = 15.0;
    mArticle->setTrips();
    CPPUNIT_ASSERT(true == mArticle->mOutputOverCurrentTrip);
    CPPUNIT_ASSERT(true == mArticle->mOutputOverVoltageTrip);

    //mArticle->setConverterFailed(true);
    //CPPUNIT_ASSERT(                         mArticle->isConverterFailed());

    UT_PASS;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Test step process
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterSensors::testStep() {
    UT_RESULT;

    /// - Initialization default test article with nominal data
    mArticle->initialize(*mConfig, *mInput);
    CPPUNIT_ASSERT_EQUAL(true, mArticle->isInitialized());

    /// @test Once converter is on and no trip occurs, output voltage will be
    /// same as the regulated voltage
    mArticle->setConverterOn(true);
    mArticle->step(mTimeStep);

    /// - All the trips will be off and  converter will be on
    CPPUNIT_ASSERT_EQUAL(true,  mArticle->isConverterOn());
    CPPUNIT_ASSERT_EQUAL(false, mArticle->mBadEfficiency);
    CPPUNIT_ASSERT_EQUAL(false, mArticle->isTripOnSensedValue());
    CPPUNIT_ASSERT_EQUAL(false, mArticle->isOutputOverCurrentTrip());
    CPPUNIT_ASSERT_EQUAL(false, mArticle->isOutputOverVoltageTrip());
    CPPUNIT_ASSERT_EQUAL(false, mArticle->isInputOverVoltageTrip());
    CPPUNIT_ASSERT_EQUAL(false, mArticle->isInputUnderVoltageTrip());
    CPPUNIT_ASSERT_EQUAL(false, mArticle->isTripped());
    CPPUNIT_ASSERT_EQUAL(false, mArticle->isConverterFailed());

    /// - Output voltage same as regulated voltage
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mRegulatedVoltage, mArticle->getOutputVoltage(), mTolerance);

    mArticle->mInputVoltage = -DBL_EPSILON;
    mArticle->step(mTimeStep);

    /// - @test Input voltage < DBL_EPSILON
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->getInputCurrent(), mTolerance);
    mArticle->mInputVoltage = mInput->mInputVoltage;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Test conversion process
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterSensors::testConversion() {
    UT_RESULT;

    /// - Initialization default test article with nominal data
    mArticle->initialize(*mConfig, *mInput);
    CPPUNIT_ASSERT_EQUAL(true, mArticle->isInitialized());

    /// @test ConverterSensors's conversion process
    double loadPower = 70.0;
    double powerConsumption = mStandbyPower + (loadPower / mEfficiency);
    double inputCurrent = powerConsumption / mInputVoltage;
    double heat = 4.64605263157895;

    mArticle->setLoad(loadPower);

    /// Turn on converter, update and do conversion process
    mArticle->setConverterOn(true);
    mArticle->step(mTimeStep);

    /// - Check input and output power depending upon the default efficiency
    CPPUNIT_ASSERT_DOUBLES_EQUAL(powerConsumption, mArticle->getInputPower(), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(loadPower,        mArticle->getOutputPower(), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(inputCurrent,     mArticle->mInputCurrent, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(heat,             mArticle->mHeatDissipation, mTolerance);

    /// unloaded case (no load attached), update
    loadPower = 0.0;
    powerConsumption = mStandbyPower;
    inputCurrent = powerConsumption / mInputVoltage;
    heat = 2.5;

    mArticle->setLoad(loadPower);
    mArticle->step(mTimeStep);

    /// - Check input and output power depending upon the default efficiency
    CPPUNIT_ASSERT_DOUBLES_EQUAL(powerConsumption, mArticle->getInputPower(), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(loadPower,        mArticle->getOutputPower(), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(inputCurrent,     mArticle->mInputCurrent, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(heat,             mArticle->mHeatDissipation, mTolerance);

    UT_PASS;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Test converter disable mode
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterSensors::testConverterOff() {
    UT_RESULT;

    /// - Initialization default test article with nominal data
    mArticle->initialize(*mConfig, *mInput);
    CPPUNIT_ASSERT_EQUAL(true, mArticle->isInitialized());

    /// @test Once converter is off, output voltage will be zero
    double outCurrent = 0.0;
    double inPower = 0.0;
    double inCurrent = 0.0;
    double heat = 0.0;

    /// - Set converter On and disable it's output
    mArticle->setConverterOn(false);
    mArticle->step(mTimeStep);

    /// - Once converter is on and output is disable, power used by the
    /// converter will be standby power, assuming input voltaeg is provided within range
    CPPUNIT_ASSERT_DOUBLES_EQUAL(inPower,    mArticle->getInputPower(), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(inCurrent,  mArticle->mInputCurrent, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(outCurrent, mArticle->mOutputCurrent, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(heat,       mArticle->mHeatDissipation, mTolerance);

    UT_PASS;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Test output over current trip
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterSensors::testOutputOverCurrentTrip() {
    UT_RESULT;

    /// - Initialization default test article with nominal data
    mArticle->initialize(*mConfig, *mInput);
    CPPUNIT_ASSERT_EQUAL(true, mArticle->isInitialized());

    /// @test  Turn converter on and set out over current true true voltage to the converter
    mArticle->setConverterOn(true);
    CPPUNIT_ASSERT_EQUAL(true, mArticle->isConverterCmdOn());

    /// - set really high load for this converter and see if over current trip occurs
    double loadPower = 10000.0;
    mArticle->setLoad(loadPower);
    mArticle->step(mTimeStep);

    CPPUNIT_ASSERT_EQUAL(true,  mArticle->mOutputOverCurrentTrip);
    CPPUNIT_ASSERT_EQUAL(false, mArticle->mOutputOverVoltageTrip);
    CPPUNIT_ASSERT_EQUAL(false, mArticle->isConverterOn());
    CPPUNIT_ASSERT_EQUAL(true,  mArticle->isTripped());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,   mArticle->mHeatDissipation, mTolerance);

    /// @test  ResetTrips()
    mArticle->mResetTrips = true;

    /// - set nominal load and check the statuses
    mArticle->setLoad(mOutputPower);
    mArticle->step(mTimeStep);

    CPPUNIT_ASSERT_EQUAL(false, mArticle->mOutputOverCurrentTrip);
    CPPUNIT_ASSERT_EQUAL(false, mArticle->mOutputOverVoltageTrip);
    CPPUNIT_ASSERT_EQUAL(false, mArticle->isTripped());
    CPPUNIT_ASSERT_EQUAL(true,  mArticle->isConverterOn());

    /// @test out over current trip will occur.  The out over current limit less than sensed current
    /// and converter will be off
    double outCurrent = mOutputPower / mRegulatedVoltage;
    double currentTolerance = mOutCurrentSensorConfig.mNominalNoiseScale;
    double voltageTolerance = mOutVoltageSensorConfig.mNominalNoiseScale;
    mInput->mOutputOverCurrentLimit = DBL_EPSILON;

    mArticle->initialize(*mConfig, *mInput);
    CPPUNIT_ASSERT_EQUAL(true, mArticle->isInitialized());

    mArticle->step(mTimeStep);

    CPPUNIT_ASSERT_EQUAL(true,  mArticle->mOutputOverCurrentTrip);
    CPPUNIT_ASSERT_EQUAL(false, mArticle->mOutputOverVoltageTrip);
    CPPUNIT_ASSERT_EQUAL(false, mArticle->isConverterOn());
    CPPUNIT_ASSERT_EQUAL(true,  mArticle->isTripped());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(outCurrent,   mArticle->getSensedOutputCurrent(), currentTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mRegulatedVoltage,   mArticle->getSensedOutputVoltage(), voltageTolerance);

    UT_PASS;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Test output over voltage trip
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterSensors::testOutputOverVoltageTrip() {
    UT_RESULT;

    /// - Initialization default test article with nominal data
    mArticle->initialize(*mConfig, *mInput);
    CPPUNIT_ASSERT_EQUAL(true, mArticle->isInitialized());

    /// - Set output over voltage limit less than output voltage
    double outVoltage = DBL_EPSILON;
    mArticle->mDefaultOutOverVoltageLimit = outVoltage;

    /// - Turn converter on, and provide zero input voltage to the converter
    mArticle->setConverterOn(true);
    mArticle->step(mTimeStep);

    CPPUNIT_ASSERT_EQUAL(true, mArticle->isConverterCmdOn());
    CPPUNIT_ASSERT_EQUAL(false, mArticle->mOutputOverCurrentTrip);
    CPPUNIT_ASSERT_EQUAL(true, mArticle->mOutputOverVoltageTrip);
    CPPUNIT_ASSERT_EQUAL(true, mArticle->isTripped());
    CPPUNIT_ASSERT_EQUAL(false,  mArticle->isConverterOn());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->getOutputCurrent(), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->getOutputVoltage(), mTolerance);

    mArticle->cmdConverterOn(true);
    mArticle->step(mTimeStep);
    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Test malunctins
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterSensors::testMalfunctions() {
    UT_RESULT;

     /// - Initialization default test article with nominal data
    mArticle->initialize(*mConfig, *mInput);
    CPPUNIT_ASSERT_EQUAL(true, mArticle->isInitialized());

    /// @test Converter Failed Malfunction
    mArticle->mMalfConverterFailed = true;

    mArticle->setConverterOn(true);
    mArticle->step(mTimeStep);
    //CPPUNIT_ASSERT( mArticle->isConverterFailed());
    CPPUNIT_ASSERT(!mArticle->isConverterOn());

    mArticle->mConverterFailed = false;
    mArticle->mMalfConverterFailed = false;
    mArticle->step(mTimeStep);
    //CPPUNIT_ASSERT(!mArticle->isConverterFailed());
    CPPUNIT_ASSERT( mArticle->isConverterOn());

    /// @test Malfunction override output over current limit
    mArticle->setLoad(190);
    mArticle->mMalfOverrideOutOverCurrentLimit = true;
    mArticle->mMalfOverrideOutOverCurrentValue = 5.0;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT( mArticle->isTripped());
    CPPUNIT_ASSERT(!mArticle->isConverterOn());

    mArticle->mMalfOverrideOutOverCurrentLimit = false;
    mArticle->mMalfOverrideOutOverCurrentValue = 0.0;
    mArticle->setLoad(0);
    mArticle->mResetTrips = true;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT(!mArticle->isTripped());
    CPPUNIT_ASSERT( mArticle->isConverterOn());

    /// @test Malfunction override output over voltage limit
    mArticle->mMalfOverrideOutOverVoltageLimit = true;
    mArticle->mMalfOverrideOutOverVoltageValue = 12.5;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT( mArticle->isTripped());
    CPPUNIT_ASSERT(!mArticle->isConverterOn());

    mArticle->mMalfOverrideOutOverVoltageLimit = false;
    mArticle->mMalfOverrideOutOverVoltageValue = 0.0;
    mArticle->mResetTrips = true;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT(!mArticle->isTripped());
    CPPUNIT_ASSERT( mArticle->isConverterOn());

    /// @test Malfunction override output voltage
    mArticle->mMalfOverrideOutVoltage = true;
    mArticle->mMalfOverrideOutVoltageValue = 23.5;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT( mArticle->isTripped());
    CPPUNIT_ASSERT(!mArticle->isConverterOn());

    mArticle->mMalfOverrideOutVoltage = false;
    mArticle->mMalfOverrideOutVoltageValue = mRegulatedVoltage;
    mArticle->mResetTrips = true;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT(!mArticle->isTripped());
    CPPUNIT_ASSERT( mArticle->isConverterOn());

    UT_PASS_LAST;
}

/*
//
*/
