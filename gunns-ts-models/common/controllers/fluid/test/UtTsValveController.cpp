/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((../TsValveController.o))
***************************************************************************************************/

#include <iostream>

#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtTsValveController.hh"

/// @details  Test identification number.
int UtTsValveController::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Valve Controller model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsValveController::UtTsValveController()
    :
    CppUnit::TestFixture(),
    mMinCmdPosition(0.0),
    mMaxCmdPosition(0.0),
    mMinFluidPosition(0.0),
    mMaxFluidPosition(0.0),
    mFluidBias(0.0),
    mFluidScale(0.0),
    mConfig(0),
    mCmdPosition(0.0),
    mMalfValveStuckFlag(false),
    mMalfValveFailToFlag(false),
    mMalfValveFailToValue(0.0),
    mManualPositionFlag(false),
    mManualPositionValue(0.0),
    mMalfManualFlag(false),
    mInput(0),
    mName(""),
    mFluidPosition(0.0),
    mArticle(0),
    mTimeStep(0.0),
    mTolerance(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Valve Controller model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsValveController::~UtTsValveController()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsValveController::setUp()
{
    /// - Define the nominal configuration data.
    mMinCmdPosition         = -80.0;
    mMaxCmdPosition         = 80.0;
    mMinFluidPosition       = 0.1;
    mMaxFluidPosition       = 0.9;
    mFluidBias              = 0.5;
    mFluidScale             = 0.005;
    mConfig                 = new TsValveControllerConfigData(mMinCmdPosition,
                                                              mMaxCmdPosition,
                                                              mMinFluidPosition,
                                                              mMaxFluidPosition);

    /// - Define the nominal input data.
    mCmdPosition            = 20.0;
    mMalfValveStuckFlag     = false;
    mMalfValveFailToFlag    = false;
    mMalfValveFailToValue   = -20.0;
    mManualPositionFlag     = false;
    mManualPositionValue    = 40.0;
    mMalfManualFlag         = false;
    mInput                  = new TsValveControllerInputData(mCmdPosition,
                                                             mManualPositionFlag,
                                                             mManualPositionValue);

    /// - Default construct the nominal test article.
    mName                   = "Test";
    mFluidPosition          = 0.6;
    mArticle                = new FriendlyTsValveController();

    /// - Define the nominal voltage, time step and comparison tolerance.
    mTimeStep               = 0.1;
    mTolerance              = 1.0e-06;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsValveController::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete mArticle;
    delete mInput;
    delete mConfig;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Valve Controller model construction of configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsValveController::testConfigAndInput()
{
    UT_RESULT_INITIAL("TS21 Common Valve Controller");

    /// @test    Configuration data default construction.
    TsValveControllerConfigData defaultConfig;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                             defaultConfig.mMinCmdPosition,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                             defaultConfig.mMaxCmdPosition,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                             defaultConfig.mMinFluidPosition,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                             defaultConfig.mMaxFluidPosition,      0.0);

    /// @test    Input data default construction.
    TsValveControllerInputData   defaultInput;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                             defaultInput.mCmdPosition,            0.0);
    CPPUNIT_ASSERT(                                              !defaultInput.mManualPositionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                             defaultInput.mManualPositionValue,    0.0);

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMinCmdPosition,                 mConfig->mMinCmdPosition,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxCmdPosition,                 mConfig->mMaxCmdPosition,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMinFluidPosition,               mConfig->mMinFluidPosition,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxFluidPosition,               mConfig->mMaxFluidPosition,           0.0);

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mCmdPosition,                    mInput->mCmdPosition,                 0.0);
    CPPUNIT_ASSERT(mManualPositionFlag                         == mInput->mManualPositionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mManualPositionValue,            mInput->mManualPositionValue,         0.0);

    /// @test    Configuration data copy construction.
    TsValveControllerConfigData copyConfig(*mConfig);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mMinCmdPosition,        copyConfig.mMinCmdPosition,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mMaxCmdPosition,        copyConfig.mMaxCmdPosition,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mMinFluidPosition,      copyConfig.mMinFluidPosition,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mMaxFluidPosition,      copyConfig.mMaxFluidPosition,         0.0);

    /// @test    Input data copy construction.
    TsValveControllerInputData   copyInput(*mInput);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInput->mCmdPosition,            copyInput.mCmdPosition,               0.0);
    CPPUNIT_ASSERT(mInput->mManualPositionFlag                 == copyInput.mManualPositionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInput->mManualPositionValue,    copyInput.mManualPositionValue,       0.0);

    /// @test    Configuration data assignment operation.
    TsValveControllerConfigData assignConfig;
    assignConfig = *mConfig;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mMinCmdPosition,        assignConfig.mMinCmdPosition,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mMaxCmdPosition,        assignConfig.mMaxCmdPosition,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mMinFluidPosition,      assignConfig.mMinFluidPosition,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mMaxFluidPosition,      assignConfig.mMaxFluidPosition,       0.0);

    /// @test    Input data assignment operation.
    TsValveControllerInputData assignInput;
    assignInput = *mInput;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInput->mCmdPosition,            assignInput.mCmdPosition,             0.0);
    CPPUNIT_ASSERT(mInput->mManualPositionFlag                 == assignInput.mManualPositionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInput->mManualPositionValue,    assignInput.mManualPositionValue,     0.0);

    /// @test    Configuration data assignment operation (to self).
    assignConfig = assignConfig;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mMinCmdPosition,        assignConfig.mMinCmdPosition,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mMaxCmdPosition,        assignConfig.mMaxCmdPosition,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mMinFluidPosition,      assignConfig.mMinFluidPosition,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mMaxFluidPosition,      assignConfig.mMaxFluidPosition,       0.0);

    /// @test    Input data assignment operation (to self).
    assignInput  = assignInput;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInput->mCmdPosition,            assignInput.mCmdPosition,             0.0);
    CPPUNIT_ASSERT(mInput->mManualPositionFlag                 == assignInput.mManualPositionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInput->mManualPositionValue,    assignInput.mManualPositionValue,     0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Valve Controller model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsValveController::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default values of attributes.
    CPPUNIT_ASSERT(""               == mArticle->mName);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  mArticle->mMinCmdPosition,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  mArticle->mMaxCmdPosition,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  mArticle->mFluidBias,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  mArticle->mFluidScale,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  mArticle->mCmdPosition,            0.0);
    CPPUNIT_ASSERT(                   !mArticle->mMalfValveStuckFlag);
    CPPUNIT_ASSERT(                   !mArticle->mMalfValveFailToFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  mArticle->mMalfValveFailToValue,   0.0);
    CPPUNIT_ASSERT(                   !mArticle->mManualPositionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  mArticle->mManualPositionValue,    0.0);
    CPPUNIT_ASSERT(                   !mArticle->mMalfManualFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  mArticle->mFluidPosition,          0.0);
    CPPUNIT_ASSERT(                   !mArticle->mInitialized);

    /// @test    New/delete for code coverage.
    TsValveController* article = new TsValveController();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Valve Controller model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsValveController::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    mArticle->initialize(*mConfig, *mInput, mName);

    /// @test    Nominal values of attributes.
    CPPUNIT_ASSERT(mName                               == mArticle->mName);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMinCmdPosition,         mArticle->mMinCmdPosition,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxCmdPosition,         mArticle->mMaxCmdPosition,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mFluidBias,              mArticle->mFluidBias,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mFluidScale,             mArticle->mFluidScale,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mCmdPosition,            mArticle->mCmdPosition,            0.0);
    CPPUNIT_ASSERT(false                               == mArticle->mMalfValveStuckFlag);
    CPPUNIT_ASSERT(false                               == mArticle->mMalfValveFailToFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                     mArticle->mMalfValveFailToValue,   0.0);
    CPPUNIT_ASSERT(mManualPositionFlag                 == mArticle->mManualPositionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mManualPositionValue,    mArticle->mManualPositionValue,    0.0);
    CPPUNIT_ASSERT(false                     == mArticle->mMalfManualFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mFluidPosition,          mArticle->mFluidPosition,          0.0);
    CPPUNIT_ASSERT(                                       mArticle->mInitialized);

    /// @test    Reinitialization.
    mArticle->initialize(*mConfig, *mInput, mName);
    CPPUNIT_ASSERT(mName                               == mArticle->mName);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMinCmdPosition,         mArticle->mMinCmdPosition,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxCmdPosition,         mArticle->mMaxCmdPosition,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mFluidBias,              mArticle->mFluidBias,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mFluidScale,             mArticle->mFluidScale,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mCmdPosition,            mArticle->mCmdPosition,            0.0);
    CPPUNIT_ASSERT(false                               == mArticle->mMalfValveStuckFlag);
    CPPUNIT_ASSERT(false                               == mArticle->mMalfValveFailToFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                     mArticle->mMalfValveFailToValue,   0.0);
    CPPUNIT_ASSERT(mManualPositionFlag                 == mArticle->mManualPositionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mManualPositionValue,    mArticle->mManualPositionValue,    0.0);
    CPPUNIT_ASSERT(false                               == mArticle->mMalfManualFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mFluidPosition,          mArticle->mFluidPosition,          0.0);
    CPPUNIT_ASSERT(                                       mArticle->mInitialized);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Valve Controller model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsValveController::testAccessors()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    mArticle->initialize(*mConfig, *mInput, mName);

    /// @test     getPosition.
    for (int i = -1; i < 12; ++i) {
        const double expected  = i * 0.1;
        mArticle->mCmdPosition = expected;
        const double returned  = mArticle->getPosition();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    }

    /// @test     isStuck.
    mArticle->mMalfValveStuckFlag    = true;
    mArticle->update(mTimeStep);
    CPPUNIT_ASSERT(mArticle->isStuck());
    mArticle->mMalfValveStuckFlag    = false;
    mArticle->mMalfValveFailToFlag   = true;
    mArticle->update(mTimeStep);
    CPPUNIT_ASSERT(mArticle->isStuck());
    mArticle->mMalfValveStuckFlag    = false;
    mArticle->mMalfValveFailToFlag   = false;
    mArticle->update(mTimeStep);
    CPPUNIT_ASSERT(!mArticle->isStuck());

    /// @test     isLowerLimit and isUpperLimit.
    mArticle->mCmdPosition = -80.1;
    mArticle->update(mTimeStep);
    CPPUNIT_ASSERT( mArticle->isLowerLimit());
    CPPUNIT_ASSERT(!mArticle->isUpperLimit());
    mArticle->mCmdPosition =  80.1;
    mArticle->update(mTimeStep);
    CPPUNIT_ASSERT(!mArticle->isLowerLimit());
    CPPUNIT_ASSERT( mArticle->isUpperLimit());
    mArticle->mCmdPosition =  0.0;
    mArticle->update(mTimeStep);
    CPPUNIT_ASSERT(!mArticle->isLowerLimit());
    CPPUNIT_ASSERT(!mArticle->isUpperLimit());

    /// @test     isInitialized.
    CPPUNIT_ASSERT(                   mArticle->isInitialized());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Valve Controller model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsValveController::testModifiers()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    mArticle->initialize(*mConfig, *mInput, mName);

    /// @test    setPosition.
    for (int i = -1; i < 12; ++i) {
        const double expected = i * 0.1;
        mArticle->setPosition(expected);
        const double returned = mArticle->mCmdPosition;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    }

    /// @test    setManualPosition.
    mArticle->setManualPosition(true, 0.5);
    CPPUNIT_ASSERT(true  == mArticle->mManualPositionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5,  mArticle->mManualPositionValue, DBL_EPSILON);

    mArticle->setManualPosition(false, -1.3);
    CPPUNIT_ASSERT(false == mArticle->mManualPositionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.3, mArticle->mManualPositionValue, DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Valve Controller model update state (nominal).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsValveController::testUpdateNominal()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    mArticle->initialize(*mConfig, *mInput, mName);

    /// @test    update with nominal time step, nominal position and no faults.
    const double expected = mManualPositionValue;
    mArticle->mManualPositionFlag = true;
    mArticle->update(mTimeStep);
    const double returned = mArticle->getPosition();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Valve Controller model update state (off-nominal).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsValveController::testUpdateOffNominal()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    mArticle->initialize(*mConfig, *mInput, mName);

    {
        /// @test    update with no manual position flag and no faults.
        mArticle->mManualPositionFlag  = false;
        mArticle->mManualPositionValue = mArticle->getPosition() - 0.1;
        const double expected = mArticle->getPosition();
        mArticle->update(mTimeStep);
        const double returned = mArticle->getPosition();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        /// @test    update with zero time step, nominal position and no faults.
        mArticle->mManualPositionValue = mManualPositionValue;
        const double expected          = mManualPositionValue;
        mArticle->mManualPositionFlag  = true;
        mArticle->update(0.0);
        const double returned = mArticle->getPosition();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        /// @test    update with below valid range position and no faults.
        mArticle->mManualPositionValue = mMinCmdPosition - 0.1;
        const double expected = mMinCmdPosition;
        mArticle->update(mTimeStep);
        const double returned = mArticle->getPosition();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        /// @test    update with above valid range position and no faults.
        mArticle->mManualPositionValue = mMaxCmdPosition + 0.1;
        const double expected = mMaxCmdPosition;
        mArticle->update(mTimeStep);
        const double returned = mArticle->getPosition();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Valve Controller model update state (malfunctions).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsValveController::testUpdateMalfunctions()
{
    UT_RESULT;

    /// @test    Stuck malfunction.
    mArticle->initialize(*mConfig, *mInput, mName);
    mArticle->mMalfValveStuckFlag     = true;
    mArticle->mManualPositionFlag     = true;
    mArticle->mMalfValveFailToValue   = mCmdPosition - 0.1;
    mArticle->mManualPositionValue    = mCmdPosition + 0.1;
    mArticle->update(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mCmdPosition,            mArticle->getPosition(), mTolerance);

    /// @test    Fail-to malfunction.
    mArticle->initialize(*mConfig, *mInput, mName);
    mArticle->mMalfValveFailToFlag    = true;
    mArticle->mManualPositionFlag     = true;
    mArticle->mMalfValveFailToValue = mCmdPosition - 0.1;
    mArticle->mManualPositionValue    = mCmdPosition + 0.1;
    mArticle->update(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mMalfValveFailToValue, mArticle->getPosition(), mTolerance);

    /// @test    Fail-to malfunction outside valid range.
    mArticle->initialize(*mConfig, *mInput, mName);
    mArticle->mMalfValveFailToFlag    = true;
    mArticle->mMalfValveFailToValue   = 100.0;
    double expected                   = mMaxCmdPosition;
    mArticle->mManualPositionFlag     = true;
    mArticle->mManualPositionValue    = mMaxCmdPosition - 0.1;
    mArticle->update(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,                mArticle->getPosition(), mTolerance);

    /// @test    Manual malfunction.
    mArticle->mMalfValveFailToFlag    = false;
    mArticle->mMalfManualFlag         = true;;
    mArticle->mManualPositionFlag     = true;
    mArticle->mManualPositionValue    = mCmdPosition + 0.1;
    expected = mArticle->getPosition();
    mArticle->update(mTimeStep);
    const double returned = mArticle->getPosition();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,                returned,                mTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Valve Controller model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsValveController::testInitializationExceptions()
{
    UT_RESULT;

    /// - Default construct a test article.
    TsValveController article;

    /// @test  Exception on empty name.
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfig, *mInput, ""), TsInitializationException);
    CPPUNIT_ASSERT(!article.isInitialized());

    /// @test  Exception on valve maximum position <= valve minimum position.
    mConfig->mMaxCmdPosition = mMinCmdPosition;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfig, *mInput, mName), TsInitializationException);
    CPPUNIT_ASSERT(!article.isInitialized());
    mConfig->mMaxCmdPosition = mMaxCmdPosition;

    /// @test  Exception on valve maximum flow area fraction <= valve minimum flow area fraction.
    mConfig->mMaxFluidPosition = mMinFluidPosition;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfig, *mInput, mName), TsInitializationException);
    CPPUNIT_ASSERT(!article.isInitialized());
    mConfig->mMaxFluidPosition = mMaxFluidPosition;

    /// @test  Exception on valve position < min position.
    mInput->mCmdPosition = mMinCmdPosition - FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfig, *mInput, mName), TsInitializationException);
    CPPUNIT_ASSERT(!article.isInitialized());
    mInput->mCmdPosition = mCmdPosition;

    /// @test  Exception on valve position > max position.
    mInput->mCmdPosition = mMaxCmdPosition + FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfig, *mInput, mName), TsInitializationException);
    CPPUNIT_ASSERT(!article.isInitialized());
    mInput->mCmdPosition = mCmdPosition;

    /// @test  Exception on on manual position < min position.
    mInput->mManualPositionFlag  = true;;
    mInput->mManualPositionValue = mMinCmdPosition - FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfig, *mInput, mName), TsInitializationException);
    CPPUNIT_ASSERT(!article.isInitialized());
    mInput->mManualPositionValue = mManualPositionValue;

    /// @test  Exception on manual position  > max position.
    mInput->mManualPositionValue = mMaxCmdPosition + FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfig, *mInput, mName), TsInitializationException);
    CPPUNIT_ASSERT(!article.isInitialized());
    mInput->mManualPositionFlag  = false;;
    mInput->mManualPositionValue = mManualPositionValue;

    UT_PASS_LAST;
}
