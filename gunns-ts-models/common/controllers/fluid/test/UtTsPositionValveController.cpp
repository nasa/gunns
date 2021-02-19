/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((../TsPositionValveController.o))
***************************************************************************************************/

#include <iostream>

#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtTsPositionValveController.hh"
#include "UtTsValveControllerAsserts.hh"

/// @details  Test identification number.
int UtTsPositionValveController::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Position Valve Controller model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsPositionValveController::UtTsPositionValveController()
    :
    CppUnit::TestFixture(),
    mMinCmdPosition(0.0),
    mMaxCmdPosition(0.0),
    mMinFluidPosition(0.0),
    mMaxFluidPosition(0.0),
    mFluidBias(0.0),
    mFluidScale(0.0),
    mTransitTime(0.0),
    mRefCmd(0.0),
    mCmdScale(0.0),
    mHoldPower(0.0),
    mMovePower(0.0),
    mStuckPower(0.0),
    mRefVoltage(0.0),
    mHoldResistance(0.0),
    mMoveResistance(0.0),
    mStuckResistance(0.0),
    mLatch(TsPoweredValveController::LATCHING),
    mEotInterrupt(false),
    mTolerance(0.0),
    mConfig(0),
    mCmdPosition(0.0),
    mMalfValveStuckFlag(false),
    mMalfValveFailToFlag(false),
    mMalfValveFailToValue(0.0),
    mManualPositionFlag(false),
    mManualPositionValue(0.0),
    mMalfManualFlag(false),
    mMalfPowerGoodFlag(false),
    mMalfDriveStuckFlag(false),
    mMalfDriveFailToFlag(false),
    mMalfDriveFailToValue(0.0),
    mSupplyVoltageFlag(false),
    mEnabledFlag(false),
    mCmd(0.0),
    mCommand(),
    mSensed(0.0),
    mInput(0),
    mName(""),
    mFluidPosition(0.0),
    mPower(0.0),
    mArticle(0),
    mTimeStep(0.0),
    mTol(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Position Valve Controller model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsPositionValveController::~UtTsPositionValveController()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPositionValveController::setUp()
{
    /// - Define the nominal configuration data.
    mMinCmdPosition         = -80.0;
    mMaxCmdPosition         = 80.0;
    mMinFluidPosition       = 0.1;
    mMaxFluidPosition       = 0.9;
    mFluidBias              = 0.5;
    mFluidScale             = 0.005;
    mTransitTime            = 10.0;
    mRefCmd                 = 1.0;
    mCmdScale               = (mMaxCmdPosition - mMinCmdPosition) / (mRefCmd * mTransitTime);
    mHoldPower              = 1.0;
    mMovePower              = 2.0;
    mStuckPower             = 3.0;
    mRefVoltage             = 100.0;
    mHoldResistance         = mRefVoltage * mRefVoltage / mHoldPower;
    mMoveResistance         = mRefVoltage * mRefVoltage / mMovePower;
    mStuckResistance        = mRefVoltage * mRefVoltage / mStuckPower;
    mLatch                  = TsPoweredValveController::NORMALLY_CLOSED;
    mEotInterrupt           = true;
    mTolerance              = 0.01;
    mConfig                 = new TsPositionValveControllerConfigData(mMinCmdPosition,
                                                                      mMaxCmdPosition,
                                                                      mMinFluidPosition,
                                                                      mMaxFluidPosition,
                                                                      mTransitTime,
                                                                      mRefCmd,
                                                                      mHoldPower,
                                                                      mMovePower,
                                                                      mStuckPower,
                                                                      mRefVoltage,
                                                                      mEotInterrupt,
                                                                      mLatch,
                                                                      mTolerance);

    /// - Define the nominal input data.
    mCmdPosition            = 20.0;
    mMalfValveStuckFlag     = false;
    mMalfValveFailToFlag    = false;
    mMalfValveFailToValue   = -20.0;
    mManualPositionFlag     = false;
    mManualPositionValue    = 40.0;
    mMalfManualFlag         = false;
    mMalfPowerGoodFlag      = false;
    mMalfDriveStuckFlag     = false;
    mMalfDriveFailToFlag    = false;
    mMalfDriveFailToValue   = -40.0;
    mSupplyVoltageFlag      = true;
    mEnabledFlag            = true;
    mCmd                    = 0.0;
    mCommand                = TsPositionValveCmd(mEnabledFlag, mCmdPosition);
    mSensed                 = mCmdPosition;
    mInput                  = new TsPoweredValveControllerInputData(mCmdPosition,
                                                                    mManualPositionFlag,
                                                                    mManualPositionValue,
                                                                    mSupplyVoltageFlag,
                                                                    mEnabledFlag,
                                                                    mCmd);

    /// - Default construct the nominal test article.
    mName                   = "Test";
    mFluidPosition          = mFluidBias + mFluidScale * mCmdPosition;
    mPower                  = mHoldPower;
    mArticle                = new FriendlyTsPositionValveController();

    /// - Define the nominal time step and comparison tolerance.
    mTimeStep               = 0.1;
    mTol                    = 1.0e-06;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPositionValveController::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete mArticle;
    delete mInput;
    delete mConfig;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Powered Valve Controller model construction of configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPositionValveController::testConfig()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data default construction.
    TsPositionValveControllerConfigData defaultConfig;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                             defaultConfig.mMinCmdPosition,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                             defaultConfig.mMaxCmdPosition,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                             defaultConfig.mMinFluidPosition,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                             defaultConfig.mMaxFluidPosition,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                             defaultConfig.mTransitTime,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,                             defaultConfig.mRefCmd,                0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                             defaultConfig.mHoldPower,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                             defaultConfig.mMovePower,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                             defaultConfig.mStuckPower,            0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                             defaultConfig.mRefVoltage,            0.0);
    CPPUNIT_ASSERT(TsPoweredValveController::LATCHING          == defaultConfig.mLatch);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                             defaultConfig.mTolerance,             0.0);

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMinCmdPosition,                 mConfig->mMinCmdPosition,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxCmdPosition,                 mConfig->mMaxCmdPosition,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMinFluidPosition,               mConfig->mMinFluidPosition,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxFluidPosition,               mConfig->mMaxFluidPosition,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mTransitTime,                    mConfig->mTransitTime,                0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mRefCmd,                         mConfig->mRefCmd,                     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mHoldPower,                      mConfig->mHoldPower,                  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMovePower,                      mConfig->mMovePower,                  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mStuckPower,                     mConfig->mStuckPower,                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mRefVoltage,                     mConfig->mRefVoltage,                 0.0);
    CPPUNIT_ASSERT(mLatch                                      == mConfig->mLatch);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mTolerance,                      mConfig->mTolerance,                  0.0);

    /// @test    Configuration data copy construction.
    TsPositionValveControllerConfigData copyConfig(*mConfig);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mMinCmdPosition,        copyConfig.mMinCmdPosition,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mMaxCmdPosition,        copyConfig.mMaxCmdPosition,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mMinFluidPosition,      copyConfig.mMinFluidPosition,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mMaxFluidPosition,      copyConfig.mMaxFluidPosition,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mTransitTime,           copyConfig.mTransitTime,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mRefCmd,                copyConfig.mRefCmd,                   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mHoldPower,             copyConfig.mHoldPower,                0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mMovePower,             copyConfig.mMovePower,                0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mStuckPower,            copyConfig.mStuckPower,               0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mRefVoltage,            copyConfig.mRefVoltage,               0.0);
    CPPUNIT_ASSERT(mConfig->mLatch                             == copyConfig.mLatch);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mTolerance,             copyConfig.mTolerance,                0.0);

    /// @test    Configuration data assignment operation.
    TsPositionValveControllerConfigData assignConfig;
    assignConfig = *mConfig;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mMinCmdPosition,        assignConfig.mMinCmdPosition,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mMaxCmdPosition,        assignConfig.mMaxCmdPosition,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mMinFluidPosition,      assignConfig.mMinFluidPosition,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mMaxFluidPosition,      assignConfig.mMaxFluidPosition,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mTransitTime,           assignConfig.mTransitTime,            0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mRefCmd,                assignConfig.mRefCmd,                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mHoldPower,             assignConfig.mHoldPower,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mMovePower,             assignConfig.mMovePower,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mStuckPower,            assignConfig.mStuckPower,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mRefVoltage,            assignConfig.mRefVoltage,             0.0);
    CPPUNIT_ASSERT(mConfig->mLatch                             == assignConfig.mLatch);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mTolerance,             assignConfig.mTolerance,              0.0);

    /// @test    Configuration data assignment operation (to self).
    assignConfig = assignConfig;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mMinCmdPosition,        assignConfig.mMinCmdPosition,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mMaxCmdPosition,        assignConfig.mMaxCmdPosition,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mMinFluidPosition,      assignConfig.mMinFluidPosition,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mMaxFluidPosition,      assignConfig.mMaxFluidPosition,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mTransitTime,           assignConfig.mTransitTime,            0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mRefCmd,                assignConfig.mRefCmd,                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mHoldPower,             assignConfig.mHoldPower,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mMovePower,             assignConfig.mMovePower,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mStuckPower,            assignConfig.mStuckPower,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mRefVoltage,            assignConfig.mRefVoltage,             0.0);
    CPPUNIT_ASSERT(mConfig->mLatch                             == assignConfig.mLatch);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mTolerance,             assignConfig.mTolerance,              0.0);

    UT_PASS;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Position Valve Controller model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPositionValveController::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default values of attributes.
    CPPUNIT_ASSERT(""                                      == mArticle->mName);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         mArticle->mMinCmdPosition,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         mArticle->mMaxCmdPosition,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         mArticle->mFluidBias,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         mArticle->mFluidScale,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         mArticle->mCmdScale,               0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         mArticle->mHoldPower,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         mArticle->mMovePower,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         mArticle->mStuckPower,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         mArticle->mHoldResistance,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         mArticle->mMoveResistance,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         mArticle->mStuckResistance,        0.0);
    CPPUNIT_ASSERT(                                          !mArticle->mEotInterrupt);
    CPPUNIT_ASSERT(TsPoweredValveController::LATCHING      == mArticle->mLatch);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         mArticle->mTolerance,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         mArticle->mCmdPosition,            0.0);
    CPPUNIT_ASSERT(                                          !mArticle->mMalfValveStuckFlag);
    CPPUNIT_ASSERT(                                          !mArticle->mMalfValveFailToFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         mArticle->mMalfValveFailToValue,   0.0);
    CPPUNIT_ASSERT(                                          !mArticle->mManualPositionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         mArticle->mManualPositionValue,    0.0);
    CPPUNIT_ASSERT(                                          !mArticle->mMalfManualFlag);
    CPPUNIT_ASSERT(                                          !mArticle->mMalfPowerGoodFlag);
    CPPUNIT_ASSERT(                                          !mArticle->mMalfDriveStuckFlag);
    CPPUNIT_ASSERT(                                          !mArticle->mMalfDriveFailToFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         mArticle->mMalfDriveFailToValue,   0.0);
    CPPUNIT_ASSERT(                                          !mArticle->mSupplyVoltageFlag);
    CPPUNIT_ASSERT(                                          !mArticle->mEnabledFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         mArticle->mCmd,                    0.0);
    ASSERT_POSITION_VALVE_CMD_VALUES_EQUAL(false, 0.0,        mArticle->mCommand,                0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         mArticle->mFluidPosition,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         mArticle->mPower,                  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         mArticle->mResistance,             0.0);
    CPPUNIT_ASSERT(                                          !mArticle->mInitialized);

    /// @test    New/delete for code coverage.
    TsPositionValveController* article = new TsPositionValveController();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Position Valve Controller model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPositionValveController::testNominalInitialization()
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
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mCmdScale,               mArticle->mCmdScale,               0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mHoldPower,              mArticle->mHoldPower,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMovePower,              mArticle->mMovePower,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mStuckPower,             mArticle->mStuckPower,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mHoldResistance,         mArticle->mHoldResistance,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMoveResistance,         mArticle->mMoveResistance,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mStuckResistance,        mArticle->mStuckResistance,        0.0);
    CPPUNIT_ASSERT(mEotInterrupt                       == mArticle->mEotInterrupt);
    CPPUNIT_ASSERT(mLatch                              == mArticle->mLatch);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mTolerance,              mArticle->mTolerance,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mCmdPosition,            mArticle->mCmdPosition,            0.0);
    CPPUNIT_ASSERT(false                               == mArticle->mMalfValveStuckFlag);
    CPPUNIT_ASSERT(false                               == mArticle->mMalfValveFailToFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                     mArticle->mMalfValveFailToValue,   0.0);
    CPPUNIT_ASSERT(mManualPositionFlag                 == mArticle->mManualPositionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mManualPositionValue,    mArticle->mManualPositionValue,    0.0);
    CPPUNIT_ASSERT(false                               == mArticle->mMalfManualFlag);
    CPPUNIT_ASSERT(false                               == mArticle->mMalfPowerGoodFlag);
    CPPUNIT_ASSERT(false                               == mArticle->mMalfDriveStuckFlag);
    CPPUNIT_ASSERT(false                               == mArticle->mMalfDriveFailToFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                     mArticle->mMalfDriveFailToValue,   0.0);
    CPPUNIT_ASSERT(mSupplyVoltageFlag                  == mArticle->mSupplyVoltageFlag);
    CPPUNIT_ASSERT(mEnabledFlag                        == mArticle->mEnabledFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mCmd,                    mArticle->mCmd,                    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mFluidPosition,          mArticle->mFluidPosition,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mPower,                  mArticle->mPower,                  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mHoldResistance,         mArticle->mResistance,             0.0);
    CPPUNIT_ASSERT(                                       mArticle->mInitialized);

    /// @test    Reinitialization.
    mArticle->initialize(*mConfig, *mInput, mName);
    CPPUNIT_ASSERT(mName                               == mArticle->mName);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMinCmdPosition,         mArticle->mMinCmdPosition,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxCmdPosition,         mArticle->mMaxCmdPosition,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mFluidBias,              mArticle->mFluidBias,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mFluidScale,             mArticle->mFluidScale,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mCmdScale,               mArticle->mCmdScale,               0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mHoldPower,              mArticle->mHoldPower,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMovePower,              mArticle->mMovePower,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mStuckPower,             mArticle->mStuckPower,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mHoldResistance,         mArticle->mHoldResistance,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMoveResistance,         mArticle->mMoveResistance,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mStuckResistance,        mArticle->mStuckResistance,        0.0);
    CPPUNIT_ASSERT(mEotInterrupt                       == mArticle->mEotInterrupt);
    CPPUNIT_ASSERT(mLatch                              == mArticle->mLatch);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mTolerance,              mArticle->mTolerance,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mCmdPosition,            mArticle->mCmdPosition,            0.0);
    CPPUNIT_ASSERT(false                               == mArticle->mMalfValveStuckFlag);
    CPPUNIT_ASSERT(false                               == mArticle->mMalfValveFailToFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                     mArticle->mMalfValveFailToValue,   0.0);
    CPPUNIT_ASSERT(mManualPositionFlag                 == mArticle->mManualPositionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mManualPositionValue,    mArticle->mManualPositionValue,    0.0);
    CPPUNIT_ASSERT(false                               == mArticle->mMalfManualFlag);
    CPPUNIT_ASSERT(false                               == mArticle->mMalfPowerGoodFlag);
    CPPUNIT_ASSERT(false                               == mArticle->mMalfDriveStuckFlag);
    CPPUNIT_ASSERT(false                               == mArticle->mMalfDriveFailToFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                     mArticle->mMalfDriveFailToValue,   0.0);
    CPPUNIT_ASSERT(mSupplyVoltageFlag                  == mArticle->mSupplyVoltageFlag);
    CPPUNIT_ASSERT(mEnabledFlag                        == mArticle->mEnabledFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mCmd,                    mArticle->mCmd,                    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mFluidPosition,          mArticle->mFluidPosition,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mPower,                  mArticle->mPower,                  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mHoldResistance,         mArticle->mResistance,             0.0);
    CPPUNIT_ASSERT(                                       mArticle->mInitialized);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Position Valve Controller model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPositionValveController::testAccessors()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    mArticle->initialize(*mConfig, *mInput, mName);

    /// @test     getPosition.
    for (int i = -1; i < 12; ++i) {
        const double expected  = i * 0.1;
        mArticle->mCmdPosition = expected;
        const double returned  = mArticle->getPosition();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTol);
    }

    /// @test     getPower.
    for (int i = -1; i < 12; ++i) {
        const double expected = i * 0.1;
        mArticle->mPower = expected;
        const double returned = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTol);
    }

    /// @test     getResistance.
    for (int i = -1; i < 12; ++i) {
        const double expected = i * 0.1;
        mArticle->mResistance = expected;
        const double returned = mArticle->getResistance();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTol);
    }

    /// @test     isInitialized.
    CPPUNIT_ASSERT(                   mArticle->isInitialized());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Position Valve Controller model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPositionValveController::testModifiers()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    mArticle->initialize(*mConfig, *mInput, mName);

    /// @test    setPosition.
    for (int i = -1; i < 12; ++i) {
        const double expected = i * 0.1;
        mArticle->setPosition(expected);
        const double returned = mArticle->mCmdPosition;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTol);
    }

    /// @test    setSupplyVoltageFlag.
    mArticle->setSupplyVoltageFlag(true);
    CPPUNIT_ASSERT(mArticle->mSupplyVoltageFlag);
    mArticle->setSupplyVoltageFlag(false);
    CPPUNIT_ASSERT(!mArticle->mSupplyVoltageFlag);

    /// @test    setEnabledFlag.
    mArticle->setEnabledFlag(true);
    CPPUNIT_ASSERT(mArticle->mCommand.mEnable);
    mArticle->setEnabledFlag(false);
    CPPUNIT_ASSERT(!mArticle->mCommand.mEnable);

    /// @test    setSpeedCommand.
    mArticle->setPositionCommand(3.7);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.7, mArticle->mCommand.mPosition, mTolerance);

    /// @test    setCommand.
    mArticle->setCommand(TsPositionValveCmd(true, 3.7));
    ASSERT_POSITION_VALVE_CMD_VALUES_EQUAL(true, 3.7, mArticle->mCommand, mTolerance);
    mArticle->setCommand(TsPositionValveCmd(false, -3.7));
    ASSERT_POSITION_VALVE_CMD_VALUES_EQUAL(false, -3.7, mArticle->mCommand, mTolerance);

    /// @test    setCmd.
    for (int i = -11; i < 12; ++i) {
        const double expected = i * 0.1;
        mArticle->setCmd(expected);
        const double returned = mArticle->mCmd;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTol);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Position Valve Controller model update state (nominal).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPositionValveController::testUpdateNominal()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    mArticle->initialize(*mConfig, *mInput, mName);

    /// @test    update with nominal position and no faults.
    const double expectedP       = mCmdPosition + 0.01;
    const double expectedW       = mMovePower;
    mArticle->setSupplyVoltageFlag(true);
    mArticle->setCommand(TsPositionValveCmd(true, expectedP));
    mArticle->update(mTimeStep);
    const double returnedP       = mArticle->getPosition();
    const double returnedW       = mArticle->getPower();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, returnedP, mTol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, returnedW, mTol);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Position Valve Controller model update state (off-nominal).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPositionValveController::testUpdateOffNominal()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    mArticle->initialize(*mConfig, *mInput, mName);

    {
        /// @test    update with zero time step.
        const double expectedP         = mCmdPosition;
        const double expectedW         = mHoldPower;
        mArticle->setSupplyVoltageFlag(true);
        mArticle->setCommand(TsPositionValveCmd(true, 0.5));
        mArticle->update(0.0);
        const double returnedP         = mArticle->getPosition();
        const double returnedW         = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, returnedP, mTol);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, returnedW, mTol);
    } {
        /// @test    update with no supply voltage (normally open).
        const double expectedP         = mMaxCmdPosition;
        const double expectedW         = 0.0;
        mArticle->mLatch               = TsPoweredValveController::NORMALLY_OPEN;
        mArticle->setSupplyVoltageFlag(false);
        mArticle->setCommand(TsPositionValveCmd(true, 0.5));
        mArticle->update(mTimeStep);
        const double returnedP         = mArticle->getPosition();
        const double returnedW         = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, returnedP, mTol);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, returnedW, mTol);
    } {
        /// @test    update with no supply voltage (normally closed).
        const double expectedP         = mMinCmdPosition;
        const double expectedW         = 0.0;
        mArticle->mLatch               = TsPoweredValveController::NORMALLY_CLOSED;
        mArticle->setSupplyVoltageFlag(false);
        mArticle->setCommand(TsPositionValveCmd(true, 0.5));
        mArticle->update(mTimeStep);
        const double returnedP         = mArticle->getPosition();
        const double returnedW         = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, returnedP, mTol);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, returnedW, mTol);
    } {
        /// @test    update with no supply voltage (latching).
        mArticle->mCmdPosition         = mCmdPosition;
        const double expectedP         = mArticle->getPosition();
        const double expectedW         = 0.0;
        mArticle->mLatch               = TsPoweredValveController::LATCHING;
        mArticle->setSupplyVoltageFlag(false);
        mArticle->setCommand(TsPositionValveCmd(true, 0.5));
        mArticle->update(mTimeStep);
        const double returnedP         = mArticle->getPosition();
        const double returnedW         = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, returnedP, mTol);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, returnedW, mTol);
    } {
        /// @test    update with no enable.
        const double expectedP         = mCmdPosition;
        const double expectedW         = 0.0;
        mArticle->mLatch               = TsPoweredValveController::LATCHING;
        mArticle->setSupplyVoltageFlag(true);
        mArticle->setCommand(TsPositionValveCmd(false, 0.5));
        mArticle->update(mTimeStep);
        const double returnedP         = mArticle->getPosition();
        const double returnedW         = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, returnedP, mTol);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, returnedW, mTol);
    } {
        /// @test    update with manual command.
        const double expectedP         = mManualPositionValue;
        const double expectedW         = mHoldPower;
        mArticle->mManualPositionFlag  = true;
        mArticle->setSupplyVoltageFlag(true);
        mArticle->setCommand(TsPositionValveCmd(true, 0.5));
        mArticle->update(mTimeStep);
        const double returnedP         = mArticle->getPosition();
        const double returnedW         = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, returnedP, mTol);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, returnedW, mTol);
    } {
        /// @test    update with below valid range position and no faults.
        const double expectedP         = mMinCmdPosition;
        const double expectedW         = mHoldPower;
        mArticle->mManualPositionFlag  = true;
        mArticle->mManualPositionValue = mMinCmdPosition - 0.1;
        mArticle->update(mTimeStep);
        const double returnedP         = mArticle->getPosition();
        const double returnedW         = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, returnedP, mTol);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, returnedW, mTol);
    } {
        /// @test    update with above valid range position and no faults.
        const double expectedP         = mMaxCmdPosition;
        const double expectedW         = mHoldPower;
        mArticle->mManualPositionFlag  = true;
        mArticle->mManualPositionValue = mMaxCmdPosition + 0.1;
        mArticle->update(mTimeStep);
        const double returnedP         = mArticle->getPosition();
        const double returnedW         = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, returnedP, mTol);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, returnedW, mTol);
    } {
        /// @test    update with open eot interruption
        const double expectedP       = mArticle->getPosition();
        const double expectedW       = mHoldPower;
        mArticle->setSupplyVoltageFlag(true);
        mArticle->setCommand(TsPositionValveCmd(true, mMaxCmdPosition));
        mArticle->setSensed(1.0);
        mArticle->update(mTimeStep);
        const double returnedP       = mArticle->getPosition();
        const double returnedW       = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, returnedP, mTol);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, returnedW, mTol);
    } {
        /// @test    update with close eot interruption
        const double expectedP       = mArticle->getPosition();
        const double expectedW       = mHoldPower;
        mArticle->setSupplyVoltageFlag(true);
        mArticle->setCommand(TsPositionValveCmd(true, mMinCmdPosition));
        mArticle->setSensed(1.0);
        mArticle->update(mTimeStep);
        const double returnedP       = mArticle->getPosition();
        const double returnedW       = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, returnedP, mTol);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, returnedW, mTol);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Position Valve Controller model update state (malfunctions).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPositionValveController::testUpdateMalfunctions()
{
    UT_RESULT;

    {
        /// @test    Stuck valve malfunction.
        mArticle->initialize(*mConfig, *mInput, mName);
        const double expectedW            = mStuckPower;
        mArticle->mMalfValveStuckFlag     = true;
        mArticle->setSupplyVoltageFlag(true);
        mArticle->setCommand(TsPositionValveCmd(true, 10.0));
        mArticle->update(mTimeStep);
        const double returnedW            = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mCmdPosition,            mArticle->getPosition(), mTol);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW,               returnedW,               mTol);
    } {
        /// @test    Valve fail-to malfunction.
        mArticle->initialize(*mConfig, *mInput, mName);
        mArticle->mMalfValveFailToFlag    = true;
        mArticle->mMalfValveFailToValue   = mMalfValveFailToValue;
        mArticle->mManualPositionFlag     = true;
        mArticle->mManualPositionValue    = mMalfValveFailToValue - 0.1;
        mArticle->update(mTimeStep);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfValveFailToValue, mArticle->getPosition(), mTol);
    } {
        /// @test    Valve fail-to malfunction outside valid range.
        mArticle->initialize(*mConfig, *mInput, mName);
        mArticle->mMalfValveFailToFlag    = true;
        mArticle->mMalfValveFailToValue   = mMaxCmdPosition + 0.1;
        const double expected             = mMaxCmdPosition;
        mArticle->mManualPositionFlag     = true;
        mArticle->mManualPositionValue    = mCmdPosition - 0.1;
        mArticle->update(mTimeStep);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,                mArticle->getPosition(), mTol);
    } {
        /// @test    Stuck motor/solenoid malfunction.
        mArticle->initialize(*mConfig, *mInput, mName);
        const double expectedW            = mStuckPower;
        mArticle->mMalfDriveStuckFlag     = true;
        mArticle->setSupplyVoltageFlag(true);
        mArticle->setCommand(TsPositionValveCmd(true, 10.0));
        mArticle->update(mTimeStep);
        const double returnedW            = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mCmdPosition,            mArticle->getPosition(), mTol);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW,               returnedW,               mTol);
    } {
        /// @test    Motor/solenoid fail-to malfunction.
        mArticle->initialize(*mConfig, *mInput, mName);
        mArticle->mMalfDriveFailToFlag    = true;
        mArticle->mMalfDriveFailToValue   = mMalfDriveFailToValue;
        mArticle->setCommand(TsPositionValveCmd(true, 10.0));
        mArticle->update(mTimeStep);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfDriveFailToValue, mArticle->getPosition(), mTol);
    } {
        /// @test    Motor/solenoid fail-to malfunction outside valid range.
        mArticle->initialize(*mConfig, *mInput, mName);
        mArticle->mMalfDriveFailToFlag    = true;
        mArticle->mMalfDriveFailToValue   = mMaxCmdPosition + 0.1;
        const double expected             = mMaxCmdPosition;
        mArticle->setCommand(TsPositionValveCmd(true, 10.0));
        mArticle->update(mTimeStep);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,                mArticle->getPosition(), mTol);
    } {
        /// @test    Manual malfunction.
        mArticle->initialize(*mConfig, *mInput, mName);
        mArticle->mMalfValveFailToFlag    = false;
        mArticle->mMalfManualFlag         = true;
        mArticle->mManualPositionFlag     = true;
        mArticle->mManualPositionValue    = 0.333;
        const double expected = mArticle->getPosition();
        mArticle->setCommand(TsPositionValveCmd(true, expected));
        mArticle->update(mTimeStep);
        const double returned = mArticle->getPosition();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,                returned,                mTol);
    } {
        /// @test    Power good malfunction.
        mArticle->initialize(*mConfig, *mInput, mName);
        const double expectedP            = mCmdPosition + mCmdScale * mRefCmd * mTimeStep;
        const double expectedW            = mMovePower;
        mArticle->mCmdPosition            = mCmdPosition;
        mArticle->mMalfPowerGoodFlag      = true;
        mArticle->setSupplyVoltageFlag(false);
        mArticle->setCommand(TsPositionValveCmd(true, expectedP));
        mArticle->update(mTimeStep);
        const double returnedP            = mArticle->getPosition();
        const double returnedW            = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, returnedP, mTol);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, returnedW, mTol);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Position Valve Controller model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPositionValveController::testInitializationExceptions()
{
    UT_RESULT;

    /// - Default construct a test article.
    TsPositionValveController article;

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

    /// @test  Exception on on transit time < 0.
    mConfig->mTransitTime = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfig, *mInput, mName), TsInitializationException);
    CPPUNIT_ASSERT(!article.isInitialized());
    mConfig->mTransitTime = mTransitTime;

    /// @test  Exception on reference command too small.
    mConfig->mRefCmd = 0.0;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfig, *mInput, mName), TsInitializationException);
    CPPUNIT_ASSERT(!article.isInitialized());
    mConfig->mRefCmd = mRefCmd;

    /// @test  Exception on on hold power < 0.
    mConfig->mHoldPower = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfig, *mInput, mName), TsInitializationException);
    CPPUNIT_ASSERT(!article.isInitialized());
    mConfig->mHoldPower = mHoldPower;

    UT_PASS_FINAL;
}
