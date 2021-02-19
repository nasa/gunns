/**
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
 ((core/GunnsSensorAnalogWrapper.o))
*/

#include "UtGunnsSensorAnalogWrapper.hh"
#include "common/sensors/test/UtSensorAnalog.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsSensorAnalogWrapper class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsSensorAnalogWrapper::UtGunnsSensorAnalogWrapper()
    :
    tArticle(),
    tName(""),
    tConfig(),
    tInput(),
    tMinRange(0.0),
    tMaxRange(0.0),
    tOffValue(0.0),
    tNominalBias(0.0),
    tNominalScale(0.0),
    tNominalNoiseScale(0.0),
    tNominalResolution(0.0),
    tNoiseFunction(),
    tUnitConversion(),
    tNominalNoiseMult(0.0),
    tPowerFlag(false),
    tTruthInput(0.0),
    tTimeStep()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsSensorAnalogWrapper class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsSensorAnalogWrapper::~UtGunnsSensorAnalogWrapper()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsSensorAnalogWrapper::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tConfig;
    delete tInput;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsSensorAnalogWrapper::setUp()
{
    /// - Define nominal config data.
    tName              = "Test Sensor";
    tMinRange          =  1.0;
    tMaxRange          = 49.0;
    tOffValue          =  5.0;
    tNominalBias       =  0.1;
    tNominalScale      =  0.99;
    tNominalNoiseScale =  1.0;
    tNominalNoiseMult  =  0.01;
    tNominalResolution =  0.2;
    tNoiseFunction     = UtSensorAnalogNoise::testNoise;
    tUnitConversion    = UnitConversion::NO_CONVERSION;
    tConfig = new GunnsSensorAnalogWrapperConfigData(tName,
                                                     tMinRange,
                                                     tMaxRange,
                                                     tOffValue,
                                                     tNominalBias,
                                                     tNominalScale,
                                                     tNominalNoiseScale,
                                                     tNominalResolution,
                                                     tNoiseFunction,
                                                     tUnitConversion,
                                                     tNominalNoiseMult);

    /// - Define nominal input data.
    tPowerFlag  = true;
    tTruthInput = 10.0;
    tInput      = new GunnsSensorAnalogWrapperInputData(tPowerFlag, tTruthInput);

    /// - Create the test article.
    tArticle = new FriendlyGunnsSensorAnalogWrapper();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the GunnsSensorAnalogWrapperConfigData class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsSensorAnalogWrapper::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsSensorAnalogWrapper 01: testConfig ..........................";

    /// - Test nominal config data construction.
    CPPUNIT_ASSERT(tName           == tConfig->mName);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMinRange,          tConfig->mSensor.mMinRange,          FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMaxRange,          tConfig->mSensor.mMaxRange,          FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tOffValue,          tConfig->mSensor.mOffValue,          FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNominalBias,       tConfig->mSensor.mNominalBias,       FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNominalScale,      tConfig->mSensor.mNominalScale,      FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNominalNoiseScale, tConfig->mSensor.mNominalNoiseScale, FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNominalNoiseMult,  tConfig->mSensor.mNominalNoiseMult,  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNominalResolution, tConfig->mSensor.mNominalResolution, FLT_EPSILON);
    CPPUNIT_ASSERT(tNoiseFunction  == tConfig->mSensor.mNoiseFunction);
    CPPUNIT_ASSERT(tUnitConversion == tConfig->mSensor.mUnitConversion);

    /// - Test default config data construction.
    GunnsSensorAnalogWrapperConfigData defaultConfig(tName);
    CPPUNIT_ASSERT(tName                         == tConfig->mName);
    CPPUNIT_ASSERT(0.0                           == defaultConfig.mSensor.mMinRange);
    CPPUNIT_ASSERT(0.0                           == defaultConfig.mSensor.mMaxRange);
    CPPUNIT_ASSERT(0.0                           == defaultConfig.mSensor.mOffValue);
    CPPUNIT_ASSERT(0.0                           == defaultConfig.mSensor.mNominalBias);
    CPPUNIT_ASSERT(1.0                           == defaultConfig.mSensor.mNominalScale);
    CPPUNIT_ASSERT(0.0                           == defaultConfig.mSensor.mNominalNoiseScale);
    CPPUNIT_ASSERT(0.0                           == defaultConfig.mSensor.mNominalResolution);
    CPPUNIT_ASSERT(TsNoise::getNoise             == defaultConfig.mSensor.mNoiseFunction);
    CPPUNIT_ASSERT(UnitConversion::NO_CONVERSION == defaultConfig.mSensor.mUnitConversion);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the GunnsSensorAnalogWrapperInputData class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsSensorAnalogWrapper::testInput()
{
    std::cout << "\n UtGunnsSensorAnalogWrapper 02: testInput ...........................";

    /// - Test nominal input data construction.
    CPPUNIT_ASSERT(tPowerFlag == tInput->mSensor.mPowerFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tTruthInput, tInput->mSensor.mTruthInput, FLT_EPSILON);

    /// - Test default config data construction.
    GunnsSensorAnalogWrapperInputData defaultInput;
    CPPUNIT_ASSERT(false == defaultInput.mSensor.mPowerFlag);
    CPPUNIT_ASSERT(0.0   == defaultInput.mSensor.mTruthInput);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the default constructor of the GunnsSensorAnalogWrapper class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsSensorAnalogWrapper::testDefaultConstruction()
{
    std::cout << "\n UtGunnsSensorAnalogWrapper 03: testDefaultConstruction .............";

    /// @test state data
    CPPUNIT_ASSERT(""    == tArticle->mName);
    CPPUNIT_ASSERT(0.0   == tArticle->mSensor.mDrift);
    CPPUNIT_ASSERT(false == tArticle->mStepPreSolverFlag);
    CPPUNIT_ASSERT(false == tArticle->mStepPostSolverFlag);

    /// @test init flag
    CPPUNIT_ASSERT(false ==  tArticle->mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the initialize method of the GunnsSensorAnalogWrapper class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsSensorAnalogWrapper::testInitialize()
{
    std::cout << "\n UtGunnsSensorAnalogWrapper 04: testInitialize ......................";

    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tConfig, tInput));

    /// - Test nominal initialization.
    CPPUNIT_ASSERT(tName       == tArticle->mName);
    CPPUNIT_ASSERT(true        == tArticle->mSensor.isInitialized());
    CPPUNIT_ASSERT(tTruthInput == tArticle->mSensor.getTruthInput());
    CPPUNIT_ASSERT(false       == tArticle->mStepPreSolverFlag);
    CPPUNIT_ASSERT(true        == tArticle->mStepPostSolverFlag);

    /// @test init flag
    CPPUNIT_ASSERT(true        == tArticle->mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the initialize method of the GunnsSensorAnalogWrapper with errors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsSensorAnalogWrapper::testInitializeExceptions()
{
    std::cout << "\n UtGunnsSensorAnalogWrapper 05: testInitializeExceptions ............";

    /// - Test exception thrown from missing name.
    tConfig->mName = "";
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tConfig, tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle->mInitFlag);
    tConfig->mName = tName;

    /// - Test exception thrown from null config data.
    const GunnsSensorAnalogWrapperConfigData* nullConfig = 0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(nullConfig, tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle->mInitFlag);

    /// - Test exception thrown from null input data.
    const GunnsSensorAnalogWrapperInputData* nullInput = 0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tConfig, nullInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle->mInitFlag);

    /// - Test exception thrown on bad config data pointer type.
    BadGunnsSensorAnalogWrapperConfigData* badConfig = new BadGunnsSensorAnalogWrapperConfigData(tName);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(badConfig, tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle->mInitFlag);
    delete badConfig;

    /// - Test exception thrown on bad input data pointer type.
    BadGunnsSensorAnalogWrapperInputData* badInput = new BadGunnsSensorAnalogWrapperInputData();
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tConfig, badInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle->mInitFlag);
    delete badInput;

    /// - Test exception thrown from bad sensor init.
    tConfig->mSensor.mMinRange = tMaxRange + 1.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tConfig, tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle->mInitFlag);
    tConfig->mSensor.mMinRange = tMinRange;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the stepPreSolver method of the GunnsSensorAnalogWrapper class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsSensorAnalogWrapper::testPreSolver()
{
    std::cout << "\n UtGunnsSensorAnalogWrapper 06: testPreSolver .......................";

    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tConfig, tInput));

    /// - Test sensor not stepped if the pre-solver flag is not set.
    tArticle->mSensor.mMalfDriftFlag = true;
    tArticle->mSensor.mMalfDriftRate = 1.0;
    tArticle->stepPreSolver(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,       tArticle->mSensor.mDrift, FLT_EPSILON);

    /// - Test sensor not stepped if both the pre-solver and post-solver flags are set.
    tArticle->setStepPreSolverFlag(true);
    tArticle->stepPreSolver(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,       tArticle->mSensor.mDrift, FLT_EPSILON);

    /// - Test sensor not stepped when the pre-solver flag is set and post-solver is not set.
    tArticle->setStepPostSolverFlag(false);
    tArticle->stepPreSolver(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tTimeStep, tArticle->mSensor.mDrift, FLT_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the stepPostSolver method of the GunnsSensorAnalogWrapper class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsSensorAnalogWrapper::testPostSolver()
{
    std::cout << "\n UtGunnsSensorAnalogWrapper 07: testPostSolver ......................";

    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tConfig, tInput));

    /// - Test sensor not stepped if the post-solver flag is not set.
    tArticle->mSensor.mMalfDriftFlag = true;
    tArticle->mSensor.mMalfDriftRate = 1.0;
    tArticle->setStepPostSolverFlag(false);
    tArticle->stepPostSolver(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,       tArticle->mSensor.mDrift, FLT_EPSILON);

    /// - Test sensor is stepped if the post-solver flag is set.
    tArticle->setStepPostSolverFlag(true);
    tArticle->stepPostSolver(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,       tArticle->mSensor.mDrift, FLT_EPSILON);

    /// - Test sensor stepped if both the pre-solver and post-solver flags are set.
    tArticle->mSensor.mDrift = 0.0;
    tArticle->setStepPreSolverFlag(true);
    tArticle->stepPostSolver(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tTimeStep, tArticle->mSensor.mDrift, FLT_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the setter & getter methods of the GunnsSensorAnalogWrapper class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsSensorAnalogWrapper::testAccessors()
{
    std::cout << "\n UtGunnsSensorAnalogWrapper 08: testAccessors .......................";

    /// - Test the setStepPreSolverFlag method.
    tArticle->setStepPreSolverFlag(true);
    CPPUNIT_ASSERT(true == tArticle->mStepPreSolverFlag);

    /// - Test the setStepPostSolverFlag method.
    tArticle->setStepPostSolverFlag(true);
    CPPUNIT_ASSERT(true == tArticle->mStepPostSolverFlag);

    std::cout << "... Pass";
}
