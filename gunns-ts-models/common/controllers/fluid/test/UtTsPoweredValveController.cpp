/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((../TsPoweredValveController.o))
***************************************************************************************************/

#include <iostream>

#include "strings/UtResult.hh"

#include "software/exceptions/TsInitializationException.hh"

#include "UtTsPoweredValveController.hh"

/// @details  Test identification number.
int UtTsPoweredValveController::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Powered Valve Controller model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsPoweredValveController::UtTsPoweredValveController()
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
    mInput(0),
    mName(""),
    mFluidPosition(0.0),
    mPower(0.0),
    mArticle(0),
    mTimeStep(0.0),
    mTolerance(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Powered Valve Controller model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsPoweredValveController::~UtTsPoweredValveController()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPoweredValveController::setUp()
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
    mEotInterrupt           = true;
    mLatch                  = TsPoweredValveController::NORMALLY_CLOSED;
    mConfig                 = new TsPoweredValveControllerConfigData(mMinCmdPosition,
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
                                                                     mLatch);

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
    mCmd                    = 10.0;
    mInput                  = new TsPoweredValveControllerInputData(mCmdPosition,
                                                                    mManualPositionFlag,
                                                                    mManualPositionValue,
                                                                    mSupplyVoltageFlag,
                                                                    mEnabledFlag,
                                                                    mCmd);

    /// - Default construct the nominal test article.
    mName                   = "Test";
    mFluidPosition          = mFluidBias + mFluidScale * mCmdPosition;
    mPower                  = mMovePower;
    mArticle                = new FriendlyTsPoweredValveController();

    /// - Define the nominal time step and comparison tolerance.
    mTimeStep               = 0.1;
    mTolerance              = 1.0e-06;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPoweredValveController::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete mArticle;
    delete mInput;
    delete mConfig;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Powered Valve Controller model construction of configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPoweredValveController::testConfigAndInput()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data default construction.
    TsPoweredValveControllerConfigData defaultConfig;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                             defaultConfig.mMinCmdPosition,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                             defaultConfig.mMaxCmdPosition,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                             defaultConfig.mMinFluidPosition,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                             defaultConfig.mMaxFluidPosition,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                             defaultConfig.mTransitTime,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                             defaultConfig.mRefCmd,                0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                             defaultConfig.mHoldPower,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                             defaultConfig.mMovePower,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                             defaultConfig.mStuckPower,            0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                             defaultConfig.mRefVoltage,            0.0);
    CPPUNIT_ASSERT(TsPoweredValveController::LATCHING          == defaultConfig.mLatch);

    /// @test    Input data default construction.
    TsPoweredValveControllerInputData   defaultInput;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                             defaultInput.mCmdPosition,            0.0);
    CPPUNIT_ASSERT(                                              !defaultInput.mManualPositionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                             defaultInput.mManualPositionValue,    0.0);
    CPPUNIT_ASSERT(                                              !defaultInput.mSupplyVoltageFlag);
    CPPUNIT_ASSERT(                                              !defaultInput.mEnabledFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                             defaultInput.mCmd,                    0.0);

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

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mCmdPosition,                    mInput->mCmdPosition,                 0.0);
    CPPUNIT_ASSERT(mManualPositionFlag                         == mInput->mManualPositionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mManualPositionValue,            mInput->mManualPositionValue,         0.0);
    CPPUNIT_ASSERT(mSupplyVoltageFlag                          == mInput->mSupplyVoltageFlag);
    CPPUNIT_ASSERT(mEnabledFlag                                == mInput->mEnabledFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mCmd,                            mInput->mCmd,                         0.0);

    /// @test    Configuration data copy construction.
    TsPoweredValveControllerConfigData copyConfig(*mConfig);
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

    /// @test    Input data copy construction.
    TsPoweredValveControllerInputData   copyInput(*mInput);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInput->mCmdPosition,            copyInput.mCmdPosition,               0.0);
    CPPUNIT_ASSERT(mInput->mManualPositionFlag                 == copyInput.mManualPositionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInput->mManualPositionValue,    copyInput.mManualPositionValue,       0.0);
    CPPUNIT_ASSERT(mInput->mSupplyVoltageFlag                  == copyInput.mSupplyVoltageFlag);
    CPPUNIT_ASSERT(mInput->mEnabledFlag                        == copyInput.mEnabledFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInput->mCmd,                    copyInput.mCmd,                       0.0);

    /// @test    Configuration data assignment operation.
    TsPoweredValveControllerConfigData assignConfig;
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

    /// @test    Input data assignment operation.
    TsPoweredValveControllerInputData assignInput;
    assignInput = *mInput;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInput->mCmdPosition,            assignInput.mCmdPosition,             0.0);
    CPPUNIT_ASSERT(mInput->mManualPositionFlag                 == assignInput.mManualPositionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInput->mManualPositionValue,    assignInput.mManualPositionValue,     0.0);
    CPPUNIT_ASSERT(mInput->mSupplyVoltageFlag                  == assignInput.mSupplyVoltageFlag);
    CPPUNIT_ASSERT(mInput->mEnabledFlag                        == assignInput.mEnabledFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInput->mCmd,                    assignInput.mCmd,                     0.0);

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

    /// @test    Input data assignment operation (to self).
    assignInput  = assignInput;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInput->mCmdPosition,            assignInput.mCmdPosition,             0.0);
    CPPUNIT_ASSERT(mInput->mManualPositionFlag                 == assignInput.mManualPositionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInput->mManualPositionValue,    assignInput.mManualPositionValue,     0.0);
    CPPUNIT_ASSERT(mInput->mSupplyVoltageFlag                  == assignInput.mSupplyVoltageFlag);
    CPPUNIT_ASSERT(mInput->mEnabledFlag                        == assignInput.mEnabledFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInput->mCmd,                    assignInput.mCmd,                     0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Powered Valve Controller model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPoweredValveController::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default values of attributes.
    CPPUNIT_ASSERT(""                                  == mArticle->mName);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                     mArticle->mMinCmdPosition,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                     mArticle->mMaxCmdPosition,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                     mArticle->mFluidBias,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                     mArticle->mFluidScale,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                     mArticle->mCmdScale,               0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                     mArticle->mHoldPower,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                     mArticle->mMovePower,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                     mArticle->mStuckPower,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                     mArticle->mHoldResistance,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                     mArticle->mMoveResistance,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                     mArticle->mStuckResistance,        0.0);
    CPPUNIT_ASSERT(                                      !mArticle->mEotInterrupt);
    CPPUNIT_ASSERT(TsPoweredValveController::LATCHING  == mArticle->mLatch);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                     mArticle->mCmdPosition,            0.0);
    CPPUNIT_ASSERT(                                      !mArticle->mMalfValveStuckFlag);
    CPPUNIT_ASSERT(                                      !mArticle->mMalfValveFailToFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                     mArticle->mMalfValveFailToValue,   0.0);
    CPPUNIT_ASSERT(                                      !mArticle->mManualPositionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                     mArticle->mManualPositionValue,    0.0);
    CPPUNIT_ASSERT(                                      !mArticle->mMalfManualFlag);
    CPPUNIT_ASSERT(                                      !mArticle->mMalfPowerGoodFlag);
    CPPUNIT_ASSERT(                                      !mArticle->mMalfDriveStuckFlag);
    CPPUNIT_ASSERT(                                      !mArticle->mMalfDriveFailToFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                     mArticle->mMalfDriveFailToValue,   0.0);
    CPPUNIT_ASSERT(                                      !mArticle->mSupplyVoltageFlag);
    CPPUNIT_ASSERT(                                      !mArticle->mEnabledFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                     mArticle->mCmd,                    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                     mArticle->mFluidPosition,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                     mArticle->mPower,                  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                     mArticle->mResistance,             0.0);
    CPPUNIT_ASSERT(                                      !mArticle->mInitialized);

    /// @test    New/delete for code coverage.
    TsPoweredValveController* article = new TsPoweredValveController();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Powered Valve Controller model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPoweredValveController::testNominalInitialization()
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
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMoveResistance,         mArticle->mResistance,             0.0);
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
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMoveResistance,         mArticle->mResistance,             0.0);
    CPPUNIT_ASSERT(                                       mArticle->mInitialized);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Powered Valve Controller model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPoweredValveController::testAccessors()
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

    /// @test     getPower.
    for (int i = -1; i < 12; ++i) {
        const double expected = i * 0.1;
        mArticle->mPower = expected;
        const double returned = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    }

    /// @test     getResistance.
    for (int i = -1; i < 12; ++i) {
        const double expected = i * 0.1;
        mArticle->mResistance = expected;
        const double returned = mArticle->getResistance();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    }

    /// @test     getSupplyVoltageFlag
    mArticle->mSupplyVoltageFlag = false;
    CPPUNIT_ASSERT(false == mArticle->getSupplyVoltageFlag());

    /// @test     isInitialized.
    CPPUNIT_ASSERT(                   mArticle->isInitialized());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Powered Valve Controller model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPoweredValveController::testModifiers()
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

    /// @test    setSupplyVoltageFlag.
    mArticle->setSupplyVoltageFlag(true);
    CPPUNIT_ASSERT(mArticle->mSupplyVoltageFlag);
    mArticle->setSupplyVoltageFlag(false);
    CPPUNIT_ASSERT(!mArticle->mSupplyVoltageFlag);

    /// @test    setEnabledFlag.
    mArticle->setEnabledFlag(true);
    CPPUNIT_ASSERT(mArticle->mEnabledFlag);
    mArticle->setEnabledFlag(false);
    CPPUNIT_ASSERT(!mArticle->mEnabledFlag);

    /// @test    setCmd.
    for (int i = -11; i < 12; ++i) {
        const double expected = i * 0.1;
        mArticle->setCmd(expected);
        const double returned = mArticle->mCmd;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Powered Valve Controller model update state (nominal).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPoweredValveController::testUpdateNominal()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    mArticle->initialize(*mConfig, *mInput, mName);

    /// @test    update with nominal position and no faults.
    const double expectedP       = mArticle->getPosition() + mCmdScale * mRefCmd * mTimeStep;
    const double expectedW       = mMovePower;
    mArticle->setSupplyVoltageFlag(true);
    mArticle->setEnabledFlag(true);
    mArticle->setCmd(mRefCmd);
    mArticle->update(mTimeStep);
    const double returnedP       = mArticle->getPosition();
    const double returnedW       = mArticle->getPower();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, returnedP, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, returnedW, mTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Powered Valve Controller model update state (off-nominal).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPoweredValveController::testUpdateOffNominal()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    mArticle->initialize(*mConfig, *mInput, mName);

    {
        /// @test    update with zero time step.
        const double expectedP         = mArticle->getPosition();
        const double expectedW         = mMovePower;
        mArticle->setSupplyVoltageFlag(true);
        mArticle->setEnabledFlag(true);
        mArticle->setCmd(10.0);
        mArticle->update(0.0);
        const double returnedP         = mArticle->getPosition();
        const double returnedW         = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, returnedP, mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, returnedW, mTolerance);
    } {
        /// @test    update with no supply voltage (normally open).
        const double expectedP         = mMaxCmdPosition;
        const double expectedW         = 0.0;
        mArticle->mLatch               = TsPoweredValveController::NORMALLY_OPEN;
        mArticle->setSupplyVoltageFlag(false);
        mArticle->setEnabledFlag(true);
        mArticle->setCmd(10.0);
        mArticle->update(mTimeStep);
        const double returnedP         = mArticle->getPosition();
        const double returnedW         = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, returnedP, mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, returnedW, mTolerance);
    } {
        /// @test    update with no supply voltage (normally closed).
        const double expectedP         = mMinCmdPosition;
        const double expectedW         = 0.0;
        mArticle->mLatch               = TsPoweredValveController::NORMALLY_CLOSED;
        mArticle->setSupplyVoltageFlag(false);
        mArticle->setEnabledFlag(true);
        mArticle->setCmd(10.0);
        mArticle->update(mTimeStep);
        const double returnedP         = mArticle->getPosition();
        const double returnedW         = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, returnedP, mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, returnedW, mTolerance);
    } {
        /// @test    update with no supply voltage (latching).
        mArticle->mCmdPosition         = mCmdPosition;
        const double expectedP         = mArticle->getPosition();
        const double expectedW         = 0.0;
        mArticle->mLatch               = TsPoweredValveController::LATCHING;
        mArticle->setSupplyVoltageFlag(false);
        mArticle->setEnabledFlag(true);
        mArticle->setCmd(10.0);
        mArticle->update(mTimeStep);
        const double returnedP         = mArticle->getPosition();
        const double returnedW         = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, returnedP, mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, returnedW, mTolerance);
    } {
        /// @test    update with no enable.
        const double expectedP         = mCmdPosition;
        const double expectedW         = 0.0;
        mArticle->mLatch               = TsPoweredValveController::LATCHING;
        mArticle->setSupplyVoltageFlag(true);
        mArticle->setEnabledFlag(false);
        mArticle->setCmd(10.0);
        mArticle->update(mTimeStep);
        const double returnedP         = mArticle->getPosition();
        const double returnedW         = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, returnedP, mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, returnedW, mTolerance);
    } {
        /// @test    update with manual command.
        const double expectedP         = mManualPositionValue;
        const double expectedW         = mHoldPower;
        mArticle->mManualPositionFlag  = true;
        mArticle->setSupplyVoltageFlag(true);
        mArticle->setEnabledFlag(true);
        mArticle->setCmd(10.0);
        mArticle->update(mTimeStep);
        const double returnedP         = mArticle->getPosition();
        const double returnedW         = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, returnedP, mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, returnedW, mTolerance);
    } {
        /// @test    update with below valid range position and no faults.
        const double expectedP         = mMinCmdPosition;
        const double expectedW         = mHoldPower;
        mArticle->mManualPositionFlag  = true;
        mArticle->mManualPositionValue = mMinCmdPosition - 0.1;
        mArticle->update(mTimeStep);
        const double returnedP         = mArticle->getPosition();
        const double returnedW         = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, returnedP, mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, returnedW, mTolerance);
    } {
        /// @test    update with above valid range position and no faults.
        const double expectedP         = mMaxCmdPosition;
        const double expectedW         = mHoldPower;
        mArticle->mManualPositionFlag  = true;
        mArticle->mManualPositionValue = mMaxCmdPosition + 0.1;
        mArticle->update(mTimeStep);
        const double returnedP         = mArticle->getPosition();
        const double returnedW         = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, returnedP, mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, returnedW, mTolerance);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Powered Valve Controller model update state (malfunctions).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPoweredValveController::testUpdateMalfunctions()
{
    UT_RESULT;

    {
        /// @test    Stuck valve malfunction.
        mArticle->initialize(*mConfig, *mInput, mName);
        const double expectedW            = mStuckPower;
        mArticle->mMalfValveStuckFlag     = true;
        mArticle->setSupplyVoltageFlag(true);
        mArticle->setEnabledFlag(true);
        mArticle->setCmd(10.0);
        mArticle->update(mTimeStep);
        const double returnedW            = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mCmdPosition,            mArticle->getPosition(), mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW,               returnedW,               mTolerance);
    } {
        /// @test    Valve fail-to malfunction.
        mArticle->initialize(*mConfig, *mInput, mName);
        mArticle->mMalfValveFailToFlag    = true;
        mArticle->mManualPositionFlag     = true;
        mArticle->mMalfValveFailToValue   = mMalfValveFailToValue;
        mArticle->update(mTimeStep);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfValveFailToValue, mArticle->getPosition(), mTolerance);
    } {
        /// @test    Valve fail-to malfunction outside valid range.
        mArticle->initialize(*mConfig, *mInput, mName);
        mArticle->mMalfValveFailToFlag    = true;
        mArticle->mMalfValveFailToValue   = mMaxCmdPosition + 0.1;
        const double expected             = mMaxCmdPosition;
        mArticle->mManualPositionFlag     = true;
        mArticle->mManualPositionValue    = mCmdPosition - 0.1;
        mArticle->update(mTimeStep);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,                mArticle->getPosition(), mTolerance);
    } {
        /// @test    Stuck motor/solenoid malfunction.
        mArticle->initialize(*mConfig, *mInput, mName);
        const double expectedW            = mStuckPower;
        mArticle->mMalfDriveStuckFlag     = true;
        mArticle->setSupplyVoltageFlag(true);
        mArticle->setEnabledFlag(true);
        mArticle->setCmd(10.0);
        mArticle->update(mTimeStep);
        const double returnedW            = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mCmdPosition,            mArticle->getPosition(), mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW,               returnedW,               mTolerance);
    } {
        /// @test    Motor/solenoid fail-to malfunction.
        mArticle->initialize(*mConfig, *mInput, mName);
        mArticle->mMalfDriveFailToFlag    = true;
        mArticle->mMalfDriveFailToValue   = mMalfDriveFailToValue;
        mArticle->mCmd                    = 10.0;
        mArticle->update(mTimeStep);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfDriveFailToValue, mArticle->getPosition(), mTolerance);
    } {
        /// @test    Motor/solenoid fail-to malfunction outside valid range.
        mArticle->initialize(*mConfig, *mInput, mName);
        mArticle->mMalfDriveFailToFlag    = true;
        mArticle->mMalfDriveFailToValue   = mMaxCmdPosition + 0.1;
        const double expected             = mMaxCmdPosition;
        mArticle->setCmd(10.0);
        mArticle->update(mTimeStep);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,                mArticle->getPosition(), mTolerance);
    } {
        /// @test    Manual malfunction.
        mArticle->initialize(*mConfig, *mInput, mName);
        mArticle->mMalfValveFailToFlag    = false;
        mArticle->mMalfManualFlag         = true;
        mArticle->mManualPositionFlag     = true;
        const double expected             = mArticle->getPosition();
        mArticle->setCmd(0.0);
        mArticle->update(mTimeStep);
        const double returned = mArticle->getPosition();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,                returned,                mTolerance);
    } {
        /// @test    Power good malfunction.
        mArticle->initialize(*mConfig, *mInput, mName);
        const double expectedP            = mArticle->getPosition() + mCmdScale * mRefCmd * mTimeStep;
        const double expectedW            = mMovePower;
        mArticle->mCmdPosition            = mCmdPosition;
        mArticle->mMalfPowerGoodFlag      = true;
        mArticle->setSupplyVoltageFlag(false);
        mArticle->setEnabledFlag(true);
        mArticle->setCmd(mRefCmd);
        mArticle->update(mTimeStep);
        const double returnedP            = mArticle->getPosition();
        const double returnedW            = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, returnedP, mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, returnedW, mTolerance);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Powered Valve Controller model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPoweredValveController::testInitializationExceptions()
{
    UT_RESULT;

    /// - Default construct a test article.
    TsPoweredValveController article;

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

    /// @test  Exception on on stuck power < 0.
    mConfig->mStuckPower = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfig, *mInput, mName), TsInitializationException);
    CPPUNIT_ASSERT(!article.isInitialized());
    mConfig->mStuckPower = mHoldPower;

    /// @test  Exception on on move power < 0.
    mConfig->mMovePower = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfig, *mInput, mName), TsInitializationException);
    CPPUNIT_ASSERT(!article.isInitialized());
    mConfig->mMovePower = mHoldPower;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Powered Valve Controller model update state and update power at hardstops.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPoweredValveController::testUpdateAtHardStops()
{
    UT_RESULT;

    {
        /// @test    update at Open hardstop - positive mRefCmd.
        mInput->mCmdPosition = mConfig->mMaxCmdPosition;
        /// - Initialize default constructed test article with initialization data.
        mArticle->initialize(*mConfig, *mInput, mName);

        const double expectedP       = mConfig->mMaxCmdPosition;
        const double expectedW       = mStuckPower;
        mArticle->setSupplyVoltageFlag(true);
        mArticle->setEnabledFlag(true);
        mArticle->setCmd(mRefCmd);
        mArticle->update(mTimeStep);
        const double returnedP       = mArticle->getPosition();
        const double returnedW       = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, returnedP, mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, returnedW, mTolerance);
    } {
        /// @test    update at Close hardstop - positive mRefCmd.
        mArticle->mCmdPosition = mConfig->mMinCmdPosition;
        const double expectedP       = mConfig->mMinCmdPosition;
        const double expectedW       = mStuckPower;
        mArticle->setSupplyVoltageFlag(true);
        mArticle->setEnabledFlag(true);
        mArticle->setCmd(-mRefCmd);
        mArticle->update(mTimeStep);
        const double returnedP       = mArticle->getPosition();
        const double returnedW       = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, returnedP, mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, returnedW, mTolerance);
    } {
        /// @test    update at Open hardstop - negative mRefCmd.
        mRefCmd = -1.0;
        mConfig->mRefCmd = mRefCmd;
        mInput->mCmdPosition = mConfig->mMaxCmdPosition;
        /// - Initialize default constructed test article with initialization data.
        mArticle->initialize(*mConfig, *mInput, mName);

        const double expectedP       = mConfig->mMaxCmdPosition;
        const double expectedW       = mStuckPower;
        mArticle->setSupplyVoltageFlag(true);
        mArticle->setEnabledFlag(true);
        mArticle->setCmd(mRefCmd);
        mArticle->update(mTimeStep);
        const double returnedP       = mArticle->getPosition();
        const double returnedW       = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, returnedP, mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, returnedW, mTolerance);
    } {
        /// @test    update at Close hardstop - negative mRefCmd.
        mArticle->mCmdPosition = mConfig->mMinCmdPosition;
        const double expectedP       = mConfig->mMinCmdPosition;
        const double expectedW       = mStuckPower;
        mArticle->setSupplyVoltageFlag(true);
        mArticle->setEnabledFlag(true);
        mArticle->setCmd(-mRefCmd);
        mArticle->update(mTimeStep);
        const double returnedP       = mArticle->getPosition();
        const double returnedW       = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, returnedP, mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, returnedW, mTolerance);
    }

    UT_PASS_LAST;
}

