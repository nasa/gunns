/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((../TsToggleValveController.o))
***************************************************************************************************/

#include <iostream>

#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtTsToggleValveController.hh"
#include "UtTsValveControllerAsserts.hh"

/// @details  Test identification number.
int UtTsToggleValveController::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Toggle Valve Controller model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsToggleValveController::UtTsToggleValveController()
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
    mCommand(),
    mSensed(),
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
/// @details  Default destructs this Toggle Valve Controller model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsToggleValveController::~UtTsToggleValveController()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsToggleValveController::setUp()
{
    /// - Define the nominal configuration data.
    mMinCmdPosition         = -80.0;
    mMaxCmdPosition         = 80.0;
    mMinFluidPosition       = 0.1;
    mMaxFluidPosition       = 0.9;
    mFluidBias              = 0.5;
    mFluidScale             = 0.005;
    mTransitTime            = 0.1;
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
    mCmd                    = 0.0;
//    mCommand                = TsToggleValveCmd(mEnabledFlag, true);
    mSensed                 = TsOpenCloseValveSensed(mCmdPosition > mMaxCmdPosition - FLT_EPSILON, mCmdPosition < mMinCmdPosition + FLT_EPSILON);
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
    mArticle                = new FriendlyTsToggleValveController();

    /// - Define the nominal time step and comparison tolerance.
    mTimeStep               = 0.1;
    mTolerance              = 1.0e-06;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsToggleValveController::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete mArticle;
    delete mInput;
    delete mConfig;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Toggle Valve Controller model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsToggleValveController::testDefaultConstruction()
{
    UT_RESULT_FIRST;

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
    ASSERT_TOGGLE_VALVE_CMD_VALUES_EQUAL(false, false,        mArticle->mCommand);
    ASSERT_OPEN_CLOSE_VALVE_SENSED_VALUES_EQUAL(false, false, mArticle->mSensed);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         mArticle->mFluidPosition,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         mArticle->mPower,                  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         mArticle->mResistance,             0.0);
    CPPUNIT_ASSERT(                                          !mArticle->mInitialized);

    /// @test    New/delete for code coverage.
    TsToggleValveController* article = new TsToggleValveController();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Toggle Valve Controller model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsToggleValveController::testNominalInitialization()
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
/// @details  Tests for Toggle Valve Controller model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsToggleValveController::testAccessors()
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

    /// @test     isInitialized.
    CPPUNIT_ASSERT(                   mArticle->isInitialized());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Toggle Valve Controller model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsToggleValveController::testModifiers()
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
    CPPUNIT_ASSERT(mArticle->mCommand.mEnable);
    mArticle->setEnabledFlag(false);
    CPPUNIT_ASSERT(!mArticle->mCommand.mEnable);

    /// @test    setToggle.
    mArticle->setToggle(true);
    CPPUNIT_ASSERT(mArticle->mCommand.mToggle);
    mArticle->setToggle(false);
    CPPUNIT_ASSERT(!mArticle->mCommand.mToggle);

    /// @test    setOpenSensed.
    mArticle->setOpenSensed(true);
    CPPUNIT_ASSERT(mArticle->mSensed.mOpen);
    mArticle->setOpenSensed(false);
    CPPUNIT_ASSERT(!mArticle->mSensed.mOpen);

    /// @test    setCloseSensed.
    mArticle->setCloseSensed(true);
    CPPUNIT_ASSERT(mArticle->mSensed.mClose);
    mArticle->setCloseSensed(false);
    CPPUNIT_ASSERT(!mArticle->mSensed.mClose);

    /// @test    setSensed.
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            const bool open = static_cast<bool>(i);
            const bool close = static_cast<bool>(j);
            TsOpenCloseValveSensed expected(open, close);
            mArticle->setSensed(expected);
            ASSERT_OPEN_CLOSE_VALVE_SENSED_EQUAL(expected, mArticle->mSensed);
        }
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Toggle Valve Controller model update state (nominal).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsToggleValveController::testUpdateNominal()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    mArticle->initialize(*mConfig, *mInput, mName);

    {
        /// @test    update with toggle open, nominal position and no faults.
        mArticle->setPosition(mMaxCmdPosition);
        const double expectedP       = mMinCmdPosition;
        const double expectedW       = mMovePower;
        mArticle->setSupplyVoltageFlag(true);
        mArticle->setEnabledFlag(true);
        mArticle->setToggle(true);
        mArticle->update(mTimeStep);
        const double returnedP       = mArticle->getPosition();
        const double returnedW       = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, returnedP, mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, returnedW, mTolerance);
    } {
        /// @test    update with toggle close, nominal position and no faults.
        const double expectedP       = mMaxCmdPosition;
        const double expectedW       = mMovePower;
        mArticle->setSupplyVoltageFlag(true);
        mArticle->setEnabledFlag(true);
        mArticle->setToggle(false);
        mArticle->update(mTimeStep);
        mArticle->setEnabledFlag(true);
        mArticle->setToggle(true);
        mArticle->update(mTimeStep);
        const double returnedP       = mArticle->getPosition();
        const double returnedW       = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, returnedP, mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, returnedW, mTolerance);
    } {
        /// @test    update with toggle open, nominal position, no faults, no EOT interrupt.
        mArticle->mEotInterrupt = false;
        mArticle->setPosition(mMinCmdPosition);
        const double expectedP       = mMaxCmdPosition;
        const double expectedW       = mMovePower;
        mArticle->setEnabledFlag(true);
        mArticle->setToggle(false);
        mArticle->update(mTimeStep);
        mArticle->setSupplyVoltageFlag(true);
        mArticle->setEnabledFlag(true);
        mArticle->setToggle(true);
        mArticle->update(mTimeStep);
        const double returnedP       = mArticle->getPosition();
        const double returnedW       = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, returnedP, mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, returnedW, mTolerance);
    } {
        /// @test    update with toggle close, nominal position, no faults, no EOT interrupt.
        mArticle->mEotInterrupt = false;
        const double expectedP       = mMinCmdPosition;
        const double expectedW       = mMovePower;
        mArticle->setSupplyVoltageFlag(true);
        mArticle->setEnabledFlag(true);
        mArticle->setToggle(false);
        mArticle->update(mTimeStep);
        mArticle->setEnabledFlag(true);
        mArticle->setToggle(true);
        mArticle->update(mTimeStep);
        double returnedP       = mArticle->getPosition();
        double returnedW       = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, returnedP, mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, returnedW, mTolerance);
        mArticle->update(mTimeStep);
        returnedP       = mArticle->getPosition();
        returnedW       = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, returnedP, mTolerance);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Toggle Valve Controller model update state (off-nominal).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsToggleValveController::testUpdateOffNominal()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    mArticle->initialize(*mConfig, *mInput, mName);

    {
        /// @test    update with zero time step.
        const double expectedP         = mCmdPosition;
        const double expectedW         = mHoldPower;
        mArticle->setSupplyVoltageFlag(true);
        mArticle->setEnabledFlag(true);
        mArticle->setToggle(true);
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
        mArticle->setToggle(true);
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
        mArticle->setToggle(true);
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
        mArticle->setToggle(true);
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
        mArticle->setToggle(true);
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
        mArticle->setToggle(true);
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
/// @details  Tests for Toggle Valve Controller model update state (malfunctions).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsToggleValveController::testUpdateMalfunctions()
{
    UT_RESULT;

    {
        /// @test    Stuck valve malfunction.
        mArticle->initialize(*mConfig, *mInput, mName);
        const double expectedW            = mStuckPower;
        mArticle->mMalfValveStuckFlag     = true;
        mArticle->setSupplyVoltageFlag(true);
        mArticle->setEnabledFlag(true);
        mArticle->setToggle(true);
        mArticle->update(mTimeStep);
        const double returnedW            = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mCmdPosition,            mArticle->getPosition(), mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW,               returnedW,               mTolerance);
    } {
        /// @test    Valve fail-to malfunction.
        mArticle->initialize(*mConfig, *mInput, mName);
        mArticle->mMalfValveFailToFlag    = true;
        mArticle->mMalfValveFailToValue   = mMalfValveFailToValue;
        mArticle->mManualPositionFlag     = true;
        mArticle->mManualPositionValue    = mMalfValveFailToValue + 0.1;
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
        mArticle->setToggle(true);
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
        mArticle->setEnabledFlag(true);
        mArticle->setToggle(true);
        mArticle->update(mTimeStep);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,                mArticle->getPosition(), mTolerance);
    } {
        /// @test    Manual malfunction.
        mArticle->initialize(*mConfig, *mInput, mName);
        mArticle->mMalfValveFailToFlag    = false;
        mArticle->mMalfManualFlag         = true;
        mArticle->mManualPositionFlag     = true;
        const double expected             = mArticle->getPosition();
        mArticle->update(mTimeStep);
        const double returned = mArticle->getPosition();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,                returned,                mTolerance);
    } {
        /// @test    Power good malfunction.
        mArticle->initialize(*mConfig, *mInput, mName);
        const double expectedP            = mMinCmdPosition;
        const double expectedW            = mMovePower;
        mArticle->mCmdPosition            = mCmdPosition;
        mArticle->mMalfPowerGoodFlag      = true;
        mArticle->setSupplyVoltageFlag(false);
        mArticle->setEnabledFlag(true);
        mArticle->setToggle(true);
        mArticle->update(mTimeStep);
        const double returnedP            = mArticle->getPosition();
        const double returnedW            = mArticle->getPower();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, returnedP, mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, returnedW, mTolerance);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Toggle Valve Controller model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsToggleValveController::testInitializationExceptions()
{
    UT_RESULT;

    /// - Default construct a test article.
    TsToggleValveController article;

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

    UT_PASS_LAST;
}
