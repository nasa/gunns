/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((../TsPidController.o))
***************************************************************************************************/

#include <iostream>

#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtTsPidController.hh"

/// @details  Test identification number.
int UtTsPidController::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this PID Controller model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsPidController::UtTsPidController()
    :
    CppUnit::TestFixture(),
    tGainP(0.0),
    tGainI(0.0),
    tGainD(0.0),
    tInterval(0.0),
    tLimitLow(0.0),
    tLimitHigh(0.0),
    tConfigData(0),
    tInput(0.0),
    tSetpoint(false),
    tTimer(false),
    tIntegral(0.0),
    tOutput(false),
    tInputData(0),
    tName(""),
    tArticle(0),
    tTimeStep(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this PID Controller model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsPidController::~UtTsPidController()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPidController::setUp()
{
    /// - Define the nominal configuration data.
    tGainP      =  0.1;
    tGainI      =  0.2;
    tGainD      =  0.01;
    tInterval   =  2.0;
    tLimitLow   = -1.0;
    tLimitHigh  =  1.0;
    tConfigData = new TsPidControllerConfigData(tGainP,
                                                tGainI,
                                                tGainD,
                                                tInterval,
                                                tLimitLow,
                                                tLimitHigh);

    /// - Define the nominal input data.
    tInput     = -0.1;
    tSetpoint  =  0.5;
    tTimer     =  1.0;
    tIntegral  = -0.2;
    tOutput    =  0.3;
    tInputData = new TsPidControllerInputData(tInput,
                                              tSetpoint,
                                              tTimer,
                                              tIntegral,
                                              tOutput);

    /// - Default construct the nominal test article.
    tName    = "Test";
    tArticle = new FriendlyTsPidController();

    /// - Define the time step.
    tTimeStep = 0.1;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPidController::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete tArticle;
    delete tInputData;
    delete tConfigData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for PID Controller model construction of configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPidController::testConfigData()
{
    UT_RESULT_INITIAL("TS21 Common PID Controller");

    /// @test    Configuration data default construction.
    TsPidControllerConfigData defaultConfig;
    CPPUNIT_ASSERT_EQUAL(0.0, defaultConfig.mGainP);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultConfig.mGainI);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultConfig.mGainD);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultConfig.mInterval);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultConfig.mLimitLow);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultConfig.mLimitHigh);

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT_EQUAL(tGainP,     tConfigData->mGainP);
    CPPUNIT_ASSERT_EQUAL(tGainI,     tConfigData->mGainI);
    CPPUNIT_ASSERT_EQUAL(tGainD,     tConfigData->mGainD);
    CPPUNIT_ASSERT_EQUAL(tInterval,  tConfigData->mInterval);
    CPPUNIT_ASSERT_EQUAL(tLimitLow,  tConfigData->mLimitLow);
    CPPUNIT_ASSERT_EQUAL(tLimitHigh, tConfigData->mLimitHigh);

    /// @test    Configuration data copy construction.
    TsPidControllerConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT_EQUAL(tConfigData->mGainP,     copyConfig.mGainP);
    CPPUNIT_ASSERT_EQUAL(tConfigData->mGainI,     copyConfig.mGainI);
    CPPUNIT_ASSERT_EQUAL(tConfigData->mGainD,     copyConfig.mGainD);
    CPPUNIT_ASSERT_EQUAL(tConfigData->mInterval,  copyConfig.mInterval);
    CPPUNIT_ASSERT_EQUAL(tConfigData->mLimitLow,  copyConfig.mLimitLow);
    CPPUNIT_ASSERT_EQUAL(tConfigData->mLimitHigh, copyConfig.mLimitHigh);

    /// @test    Configuration data assignment operation.
    TsPidControllerConfigData assignConfig;
    assignConfig = *tConfigData;
    CPPUNIT_ASSERT_EQUAL(tConfigData->mGainP,     assignConfig.mGainP);
    CPPUNIT_ASSERT_EQUAL(tConfigData->mGainI,     assignConfig.mGainI);
    CPPUNIT_ASSERT_EQUAL(tConfigData->mGainD,     assignConfig.mGainD);
    CPPUNIT_ASSERT_EQUAL(tConfigData->mInterval,  assignConfig.mInterval);
    CPPUNIT_ASSERT_EQUAL(tConfigData->mLimitLow,  assignConfig.mLimitLow);
    CPPUNIT_ASSERT_EQUAL(tConfigData->mLimitHigh, assignConfig.mLimitHigh);

    /// @test    Configuration data assignment operation (to self).
    assignConfig = assignConfig;
    CPPUNIT_ASSERT_EQUAL(tConfigData->mGainP,     assignConfig.mGainP);
    CPPUNIT_ASSERT_EQUAL(tConfigData->mGainI,     assignConfig.mGainI);
    CPPUNIT_ASSERT_EQUAL(tConfigData->mGainD,     assignConfig.mGainD);
    CPPUNIT_ASSERT_EQUAL(tConfigData->mInterval,  assignConfig.mInterval);
    CPPUNIT_ASSERT_EQUAL(tConfigData->mLimitLow,  assignConfig.mLimitLow);
    CPPUNIT_ASSERT_EQUAL(tConfigData->mLimitHigh, assignConfig.mLimitHigh);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for PID Controller model construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPidController::testInputData()
{
    UT_RESULT;

    /// @test    Input data default construction.
    TsPidControllerInputData defaultInput;
    CPPUNIT_ASSERT_EQUAL(0.0, defaultInput.mInput);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultInput.mSetpoint);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultInput.mTimer);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultInput.mIntegral);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultInput.mOutput);

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT_EQUAL(tInput,    tInputData->mInput);
    CPPUNIT_ASSERT_EQUAL(tSetpoint, tInputData->mSetpoint);
    CPPUNIT_ASSERT_EQUAL(tTimer,    tInputData->mTimer);
    CPPUNIT_ASSERT_EQUAL(tIntegral, tInputData->mIntegral);
    CPPUNIT_ASSERT_EQUAL(tOutput,   tInputData->mOutput);

    /// @test    Input data copy construction.
    TsPidControllerInputData copyInput(*tInputData);
    CPPUNIT_ASSERT_EQUAL(tInputData->mInput,    copyInput.mInput);
    CPPUNIT_ASSERT_EQUAL(tInputData->mSetpoint, copyInput.mSetpoint);
    CPPUNIT_ASSERT_EQUAL(tInputData->mTimer,    copyInput.mTimer);
    CPPUNIT_ASSERT_EQUAL(tInputData->mIntegral, copyInput.mIntegral);
    CPPUNIT_ASSERT_EQUAL(tInputData->mOutput,   copyInput.mOutput);

    /// @test    Input data assignment operation.
    TsPidControllerInputData assignInput;
    assignInput = *tInputData;
    CPPUNIT_ASSERT_EQUAL(tInputData->mInput,    assignInput.mInput);
    CPPUNIT_ASSERT_EQUAL(tInputData->mSetpoint, assignInput.mSetpoint);
    CPPUNIT_ASSERT_EQUAL(tInputData->mTimer,    assignInput.mTimer);
    CPPUNIT_ASSERT_EQUAL(tInputData->mIntegral, assignInput.mIntegral);
    CPPUNIT_ASSERT_EQUAL(tInputData->mOutput,   assignInput.mOutput);

    /// @test    Input data assignment operation (to self).
    assignInput = assignInput;
    CPPUNIT_ASSERT_EQUAL(tInputData->mInput,    assignInput.mInput);
    CPPUNIT_ASSERT_EQUAL(tInputData->mSetpoint, assignInput.mSetpoint);
    CPPUNIT_ASSERT_EQUAL(tInputData->mTimer,    assignInput.mTimer);
    CPPUNIT_ASSERT_EQUAL(tInputData->mIntegral, assignInput.mIntegral);
    CPPUNIT_ASSERT_EQUAL(tInputData->mOutput,   assignInput.mOutput);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for PID Controller model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPidController::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default values of attributes.
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mGainP);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mGainI);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mGainD);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mInterval);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mLimitLow);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mLimitHigh);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mInput);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mSetpoint);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mTimer);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mIntegral);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mOutput);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mError);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mDerivative);
    CPPUNIT_ASSERT(""      == tArticle->mName);
    CPPUNIT_ASSERT(          !tArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    TsPidController* article = new TsPidController();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for PID Controller model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPidController::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tName);

    /// @test    Nominal values of attributes.
    const double expectedError = tInput - tSetpoint;
    CPPUNIT_ASSERT(tName             == tArticle->mName);
    CPPUNIT_ASSERT_EQUAL(tGainP,        tArticle->mGainP);
    CPPUNIT_ASSERT_EQUAL(tGainI,        tArticle->mGainI);
    CPPUNIT_ASSERT_EQUAL(tGainD,        tArticle->mGainD);
    CPPUNIT_ASSERT_EQUAL(tInterval,     tArticle->mInterval);
    CPPUNIT_ASSERT_EQUAL(tLimitLow,     tArticle->mLimitLow);
    CPPUNIT_ASSERT_EQUAL(tLimitHigh,    tArticle->mLimitHigh);
    CPPUNIT_ASSERT_EQUAL(tInput,        tArticle->mInput);
    CPPUNIT_ASSERT_EQUAL(tSetpoint,     tArticle->mSetpoint);
    CPPUNIT_ASSERT_EQUAL(tTimer,        tArticle->mTimer);
    CPPUNIT_ASSERT_EQUAL(tIntegral,     tArticle->mIntegral);
    CPPUNIT_ASSERT_EQUAL(tOutput,       tArticle->mOutput);
    CPPUNIT_ASSERT_EQUAL(expectedError, tArticle->mError);
    CPPUNIT_ASSERT_EQUAL(0.0,           tArticle->mDerivative);
    CPPUNIT_ASSERT(                     tArticle->mInitFlag);

    /// @test    Reinitialization.
    tArticle->initialize(*tConfigData, *tInputData, tName);
    CPPUNIT_ASSERT_EQUAL(tGainP,        tArticle->mGainP);
    CPPUNIT_ASSERT_EQUAL(tGainI,        tArticle->mGainI);
    CPPUNIT_ASSERT_EQUAL(tGainD,        tArticle->mGainD);
    CPPUNIT_ASSERT_EQUAL(tInterval,     tArticle->mInterval);
    CPPUNIT_ASSERT_EQUAL(tLimitLow,     tArticle->mLimitLow);
    CPPUNIT_ASSERT_EQUAL(tLimitHigh,    tArticle->mLimitHigh);
    CPPUNIT_ASSERT_EQUAL(tInput,        tArticle->mInput);
    CPPUNIT_ASSERT_EQUAL(tSetpoint,     tArticle->mSetpoint);
    CPPUNIT_ASSERT_EQUAL(tTimer,        tArticle->mTimer);
    CPPUNIT_ASSERT_EQUAL(tIntegral,     tArticle->mIntegral);
    CPPUNIT_ASSERT_EQUAL(tOutput,       tArticle->mOutput);
    CPPUNIT_ASSERT_EQUAL(expectedError, tArticle->mError);
    CPPUNIT_ASSERT_EQUAL(0.0,           tArticle->mDerivative);
    CPPUNIT_ASSERT(                     tArticle->mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for PID Controller model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPidController::testAccessors()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tName);

    /// @test     getOutput.
    CPPUNIT_ASSERT_EQUAL(tOutput, tArticle->getOutput());

    /// @test     isInitialized.
    CPPUNIT_ASSERT(tArticle->isInitialized());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for PID Controller model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPidController::testModifiers()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tName);

    /// @test    setSetpoint.
    tArticle->setSetpoint(2.0);
    CPPUNIT_ASSERT_EQUAL(2.0, tArticle->mSetpoint);

    /// @test    setInput.
    tArticle->setInput(3.0);
    CPPUNIT_ASSERT_EQUAL(3.0, tArticle->mInput);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for PID Controller model update method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPidController::testUpdate()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tName);

    /// @test    update with timer < interval, timer increments and output is unchanged.
    double expectedTimer = tTimer + tTimeStep;
    tArticle->update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTimer, tArticle->mTimer,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tOutput,       tArticle->mOutput, 0.0);

    /// @test    update with timer incrementing to interval, output is updated, timer is reset.
    double expectedError_0 = tInput - tSetpoint;
    double input           = 0.0;
    double timer           = tInterval - tTimeStep;
    tArticle->mInput = input;
    tArticle->mTimer = timer;
    tArticle->update(tTimeStep);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mTimer);
    double expectedError_1    = input - tSetpoint;
    double expectedIntegral   = tIntegral + expectedError_1 * tInterval;
    double expectedDerivative = (expectedError_1 - expectedError_0) / tInterval;
    double expectedOutput     = tOutput + expectedError_1 * tGainP + expectedIntegral * tGainI
                              + expectedDerivative * tGainD;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedError_1,    tArticle->mError,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIntegral,   tArticle->mIntegral,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDerivative, tArticle->mDerivative, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedOutput,     tArticle->mOutput,     DBL_EPSILON);

    /// @test   update with timer past the interval.  This occurs if user manually overrides the
    ///         timer or if interval is not a multiple of dt.
    input = 0.05;
    timer = tInterval * 1.5;
    tArticle->mInput = input;
    tArticle->mTimer = timer;
    tArticle->update(tTimeStep);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mTimer);
    double expectedError_2 = input - tSetpoint;
    expectedIntegral      += expectedError_2 * (timer + tTimeStep);
    expectedDerivative     = (expectedError_2 - expectedError_1) / (timer + tTimeStep);
    expectedOutput        += expectedError_2 * tGainP + expectedIntegral * tGainI
                           + expectedDerivative * tGainD;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedError_2,    tArticle->mError,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIntegral,   tArticle->mIntegral,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDerivative, tArticle->mDerivative, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedOutput,     tArticle->mOutput,     DBL_EPSILON);

    /// @test   Protecting underflows in integral and output.
    input = tSetpoint;
    timer = tInterval - tTimeStep;
    tArticle->mInput    = input;
    tArticle->mTimer    = timer;
    tArticle->mError    = 0.0;
    tArticle->mIntegral = -0.1 * DBL_EPSILON;
    tArticle->mOutput   =  0.1 * DBL_EPSILON;
    tArticle->update(tTimeStep);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mTimer);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mError);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mIntegral);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mDerivative);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mOutput);

    /// @test   Interval = 0, timer = 0, timestep = 0.
    tArticle->mInterval = 0.0;
    tArticle->mTimer    = 0.0;
    tArticle->update(0.0);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mTimer);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mError);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mIntegral);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mDerivative);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mOutput);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for PID Controller model update state (off-nominal).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPidController::testUpdateWithInput()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tName);

    /// @test    update with timer incrementing to interval, output is updated, timer is reset.
    double expectedError_0 = tInput - tSetpoint;
    double input           = 0.1;
    double timer           = tInterval - tTimeStep;
    tArticle->mTimer = timer;
    tArticle->update(tTimeStep, input);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mTimer);
    double expectedError_1    = input - tSetpoint;
    double expectedIntegral   = tIntegral + expectedError_1 * tInterval;
    double expectedDerivative = (expectedError_1 - expectedError_0) / tInterval;
    double expectedOutput     = tOutput + expectedError_1 * tGainP + expectedIntegral * tGainI
                              + expectedDerivative * tGainD;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedError_1,    tArticle->mError,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIntegral,   tArticle->mIntegral,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDerivative, tArticle->mDerivative, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedOutput,     tArticle->mOutput,     DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for PID Controller model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPidController::testInitializationExceptions()
{
    UT_RESULT;

    /// - Default construct a test article.
    TsPidController article;

    /// @test  Exception on empty name.
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, ""), TsInitializationException);
    CPPUNIT_ASSERT(!article.isInitialized());

    /// @test  Exception on update interval not > 0.
    tConfigData->mInterval = 0.0;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tName), TsInitializationException);
    CPPUNIT_ASSERT(!article.isInitialized());
    tConfigData->mInterval = tInterval;

    /// @test  Exception on upper limit <= lower limit.
    tConfigData->mLimitHigh = tLimitLow;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tName), TsInitializationException);
    CPPUNIT_ASSERT(!article.isInitialized());
    tConfigData->mLimitHigh = tLimitLow - DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tName), TsInitializationException);
    CPPUNIT_ASSERT(!article.isInitialized());
    tConfigData->mLimitHigh = tLimitHigh;

    UT_PASS_LAST;
}
