/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((../TsFanSpeedSensors.o))
***************************************************************************************************/

#include <iostream>

#include "software/exceptions/TsInitializationException.hh"

#include "UtTsFanSpeedSensors.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Iss Fan Speed Sensors model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsFanSpeedSensors::UtTsFanSpeedSensors()
    :
    CppUnit::TestFixture(),
    mSensorConfig(),
    mConfig(0),
    mSensorAInput(),
    mSensorBInput(),
    mSensorCInput(),
    mInput(0),
    mName(""),
    mArticle(0),
    mTimeStep(0.0),
    mTolerance(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Iss Fan Speed Sensors model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsFanSpeedSensors::~UtTsFanSpeedSensors()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsFanSpeedSensors::setUp()
{
    /// - Define the nominal configuration data.
    mSensorConfig.mMaxRange   = +10000.0;
    mSensorConfig.mMinRange   = -10000.0;
    mConfig                   = new TsFanSpeedSensorsConfigData(mSensorConfig);

    /// - Define the nominal input data.
    mSensorAInput.mPowerFlag  = true;
    mSensorAInput.mTruthInput = 1000.0;
    mSensorBInput.mPowerFlag  = true;
    mSensorBInput.mTruthInput = 1001.0;
    mSensorCInput.mPowerFlag  = true;
    mSensorCInput.mTruthInput = 1002.0;
    mInput                  = new TsFanSpeedSensorsInputData(mSensorAInput, mSensorBInput, mSensorCInput);

    /// - Default construct the nominal test article.
    mName                   = "Test";
    mArticle                = new FriendlyTsFanSpeedSensors();

    /// - Define the nominal time step and comparison tolerance.
    mTimeStep               = 0.1;
    mTolerance              = 1.0e-06;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsFanSpeedSensors::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete mArticle;
    delete mInput;
    delete mConfig;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Iss Fan Speed Sensors model construction of configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsFanSpeedSensors::testConfigAndInput()
{
    std::cout << "\n.Iss Fan Speed Sensors 01: Configuration And Input Data Test...............";

    /// @test    Configuration data default construction.
    TsFanSpeedSensorsConfigData  defaultConfig;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                          defaultConfig.mSensor.mMaxRange,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                          defaultConfig.mSensor.mMinRange,   0.0);

    /// @test    Input data default construction.
    TsFanSpeedSensorsInputData   defaultInput;
    CPPUNIT_ASSERT(                                           !defaultInput.mSensorA.mPowerFlag);
    CPPUNIT_ASSERT(                                           !defaultInput.mSensorB.mPowerFlag);
    CPPUNIT_ASSERT(                                           !defaultInput.mSensorC.mPowerFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                          defaultInput.mSensorA.mTruthInput, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                          defaultInput.mSensorB.mTruthInput, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                          defaultInput.mSensorC.mTruthInput, 0.0);

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSensorConfig.mMaxRange,      mConfig->mSensor.mMaxRange,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSensorConfig.mMinRange,      mConfig->mSensor.mMinRange,        0.0);

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(mSensorAInput.mPowerFlag                 == mInput->mSensorA.mPowerFlag);
    CPPUNIT_ASSERT(mSensorBInput.mPowerFlag                 == mInput->mSensorB.mPowerFlag);
    CPPUNIT_ASSERT(mSensorCInput.mPowerFlag                 == mInput->mSensorC.mPowerFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSensorAInput.mTruthInput,    mInput->mSensorA.mTruthInput,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSensorBInput.mTruthInput,    mInput->mSensorB.mTruthInput,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSensorCInput.mTruthInput,    mInput->mSensorC.mTruthInput,      0.0);

    /// @test    Configuration data copy construction.
    TsFanSpeedSensorsConfigData  copyConfig(*mConfig);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mSensor.mMaxRange,   copyConfig.mSensor.mMaxRange,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mSensor.mMinRange,   copyConfig.mSensor.mMinRange,      0.0);

    /// @test    Input data copy construction.
    TsFanSpeedSensorsInputData   copyInput(*mInput);
    CPPUNIT_ASSERT(mInput->mSensorA.mPowerFlag              == copyInput.mSensorA.mPowerFlag);
    CPPUNIT_ASSERT(mInput->mSensorB.mPowerFlag              == copyInput.mSensorB.mPowerFlag);
    CPPUNIT_ASSERT(mInput->mSensorC.mPowerFlag              == copyInput.mSensorC.mPowerFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInput->mSensorA.mTruthInput, copyInput.mSensorA.mTruthInput,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInput->mSensorB.mTruthInput, copyInput.mSensorB.mTruthInput,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInput->mSensorC.mTruthInput, copyInput.mSensorC.mTruthInput,    0.0);

    /// @test    Configuration data assignment operation.
    TsFanSpeedSensorsConfigData  assignConfig;
    assignConfig = *mConfig;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mSensor.mMaxRange,   assignConfig.mSensor.mMaxRange,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mSensor.mMinRange,   assignConfig.mSensor.mMinRange,    0.0);

    /// @test    Input data assignment operation.
    TsFanSpeedSensorsInputData   assignInput;
    assignInput  = *mInput;
    CPPUNIT_ASSERT(mInput->mSensorA.mPowerFlag              == assignInput.mSensorA.mPowerFlag);
    CPPUNIT_ASSERT(mInput->mSensorB.mPowerFlag              == assignInput.mSensorB.mPowerFlag);
    CPPUNIT_ASSERT(mInput->mSensorC.mPowerFlag              == assignInput.mSensorC.mPowerFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInput->mSensorA.mTruthInput, assignInput.mSensorA.mTruthInput,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInput->mSensorB.mTruthInput, assignInput.mSensorB.mTruthInput,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInput->mSensorC.mTruthInput, assignInput.mSensorC.mTruthInput,  0.0);

    /// @test    Configuration data assignment operation (to self).
    assignConfig = assignConfig;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mSensor.mMaxRange,   assignConfig.mSensor.mMaxRange,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mSensor.mMinRange,   assignConfig.mSensor.mMinRange,    0.0);

    /// @test    Input data assignment operation (to self).
    assignInput  = assignInput;
    CPPUNIT_ASSERT(mInput->mSensorA.mPowerFlag              == assignInput.mSensorA.mPowerFlag);
    CPPUNIT_ASSERT(mInput->mSensorB.mPowerFlag              == assignInput.mSensorB.mPowerFlag);
    CPPUNIT_ASSERT(mInput->mSensorC.mPowerFlag              == assignInput.mSensorC.mPowerFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInput->mSensorA.mTruthInput, assignInput.mSensorA.mTruthInput,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInput->mSensorB.mTruthInput, assignInput.mSensorB.mTruthInput,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInput->mSensorC.mTruthInput, assignInput.mSensorC.mTruthInput,  0.0);

    std::cout << " Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Iss Fan Speed Sensors model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsFanSpeedSensors::testDefaultConstruction()
{
    std::cout << "\n.Iss Fan Speed Sensors 02: Default Construction Test.......................";

    /// @test    Default values of attributes.
    CPPUNIT_ASSERT(""               == mArticle->mName);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  mArticle->mSensorA.getSensedOutput(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  mArticle->mSensorB.getSensedOutput(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  mArticle->mSensorC.getSensedOutput(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  mArticle->mAverageSpeed,              0.0);
    CPPUNIT_ASSERT(                   !mArticle->mInitialized);

    /// @test    New/delete for code coverage.
    TsFanSpeedSensors* article = new TsFanSpeedSensors();
    delete article;

    std::cout << " Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Iss Fan Speed Sensors model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsFanSpeedSensors::testNominalInitialization()
{
    std::cout << "\n.Iss Fan Speed Sensors 03: Nominal Initialization Test.....................";

    /// - Initialize default constructed test article with nominal initialization data.
    mArticle->initialize(*mConfig, *mInput, mName);

    /// @test    Nominal values of attributes.
    CPPUNIT_ASSERT(mName                                  == mArticle->mName);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSensorAInput.mTruthInput,  mArticle->mSensorA.getSensedOutput(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSensorBInput.mTruthInput,  mArticle->mSensorB.getSensedOutput(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSensorCInput.mTruthInput,  mArticle->mSensorC.getSensedOutput(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSensorBInput.mTruthInput,  mArticle->mAverageSpeed,              0.0);
    CPPUNIT_ASSERT(                                          mArticle->mInitialized);

    /// @test    Reinitialization.
    mArticle->initialize(*mConfig, *mInput, mName);
    CPPUNIT_ASSERT(mName                                  == mArticle->mName);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSensorAInput.mTruthInput,  mArticle->mSensorA.getSensedOutput(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSensorBInput.mTruthInput,  mArticle->mSensorB.getSensedOutput(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSensorCInput.mTruthInput,  mArticle->mSensorC.getSensedOutput(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSensorBInput.mTruthInput,  mArticle->mAverageSpeed,              0.0);
    CPPUNIT_ASSERT(                                          mArticle->mInitialized);

    std::cout << " Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Iss Fan Speed Sensors model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsFanSpeedSensors::testAccessors()
{
    std::cout << "\n.Iss Fan Speed Sensors 04: Accessors Test..................................";

    /// - Initialize default constructed test article with nominal initialization data.
    mArticle->initialize(*mConfig, *mInput, mName);

    /// @test     getTruthInput.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSensorAInput.mTruthInput, mArticle->getTruthInput(),   0.0);

    /// @test     getSensedOutput.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSensorBInput.mTruthInput, mArticle->getSensedOutput(), 0.0);

    /// @test     isInitialized.
    CPPUNIT_ASSERT(                                         mArticle->isInitialized());

    std::cout << " Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Iss Fan Speed Sensors model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsFanSpeedSensors::testModifiers()
{
    std::cout << "\n.Iss Fan Speed Sensors 05: Modifiers Test..................................";

    /// - Initialize default constructed test article with nominal initialization data.
    mArticle->initialize(*mConfig, *mInput, mName);

    /// @test    setPowerFlag.
    mArticle->setPowerFlag(false);
    mArticle->mSensorA.update(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSensorConfig.mOffValue, mArticle->mSensorA.getSensedOutput(), 0.0);
    mArticle->mSensorB.update(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSensorConfig.mOffValue, mArticle->mSensorB.getSensedOutput(), 0.0);
    mArticle->mSensorC.update(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSensorConfig.mOffValue, mArticle->mSensorC.getSensedOutput(), 0.0);

    /// @test    setTruthInput.
    const double expected = 1234.5;
    mArticle->setTruthInput(expected);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->mSensorA.getTruthInput(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->mSensorB.getTruthInput(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->mSensorC.getTruthInput(), 0.0);

    std::cout << " Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Iss Fan Speed Sensors model update state (nominal).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsFanSpeedSensors::testUpdate()
{
    std::cout << "\n.Iss Fan Speed Sensors 06: Update State Nominal Test.......................";

    /// - Initialize default constructed test article with nominal initialization data.
    mArticle->initialize(*mConfig, *mInput, mName);
    const double expected = 1234.5;
    mArticle->setTruthInput(expected);

    /// @test    update with power and non-zero time step.
    mArticle->update(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->getSensedOutput(), 0.0);

    /// @test    update with zero time step.
    mArticle->update(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->getSensedOutput(), 0.0);

    /// @test    update with no power.
    mArticle->setPowerFlag(false);
    mArticle->update(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSensorConfig.mOffValue, mArticle->getSensedOutput(), 0.0);


    std::cout << " Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Iss Fan Speed Sensors model update state (off-nominal).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsFanSpeedSensors::testSense()
{
    std::cout << "\n.Iss Fan Speed Sensors 07: Update State Off-Nominal Test...................";

    /// - Initialize default constructed test article with nominal initialization data.
    mArticle->initialize(*mConfig, *mInput, mName);

    /// @test    sense with power and non-zero time step.
    const double expected = 1234.5;
    mArticle->sense(mTimeStep, true, expected);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->getSensedOutput(), 0.0);

    /// @test    sense with zero time step.
    mArticle->sense(0.0, true, expected);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->getSensedOutput(), 0.0);

    /// @test    sense with no power.
    mArticle->sense(mTimeStep, false, expected);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSensorConfig.mOffValue, mArticle->getSensedOutput(), 0.0);

    std::cout << " Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Iss Fan Speed Sensors model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsFanSpeedSensors::testInitializationExceptions()
{
    std::cout << "\n.Iss Fan Speed Sensors 08: Initialization Exceptions Test..................";

    /// - Default construct a test article.
    TsFanSpeedSensors article;

    /// @test  Exception on empty name.
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfig, *mInput, ""), TsInitializationException);
    CPPUNIT_ASSERT(!article.isInitialized());

    /// @test  Exception from sensor.
    mSensorConfig.mMinRange   = +10000.0;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfig, *mInput, ""), TsInitializationException);
    CPPUNIT_ASSERT(!article.isInitialized());

    std::cout << " Pass." << std::endl;
}
