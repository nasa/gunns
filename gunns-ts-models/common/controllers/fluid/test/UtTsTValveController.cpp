/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((common/controllers/fluid/TsTValveController.o))
***************************************************************************************************/

#include <iostream>

#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtTsTValveController.hh"

/// @details  Test identification number.
int UtTsTValveController::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this TS21 Fluid Controller Manual T-Valve (3 way, 3 position)
///           Controller Model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsTValveController::UtTsTValveController()
    :
    CppUnit::TestFixture(),
    mName(""),
    mConfig(0),
    mInput(0),
    mArticle(0),
    mTimeStep(0.0),
    mTolerance(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this TS21 Fluid Controller Manual T-Valve (3 way, 3 position)
///           Controller Model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsTValveController::~UtTsTValveController()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsTValveController::setUp()
{
    /// - Define the nominal initialization data.
    mName            = "Test";
    mConfig          = new TsTValveControllerConfigData(TsValveControllerConfigData(0.1, 0.9, 0.0, 1.0),
                                                        TsValveControllerConfigData(-0.1, 1.1, 0.0, 1.0));
    mInput           = new TsTValveControllerInputData(TsTValveController::A_PATH,
                                                       TsValveControllerInputData(0.9, false, 0.0),
                                                       TsValveControllerInputData(-0.1, false, 0.0));

    /// - Default construct the nominal test article.
    mArticle         = new FriendlyTsTValveController;

    /// - Define the nominal voltage, time step and comparison tolerance.
    mTimeStep        = 0.1;
    mTolerance       = 1.0e-06;

    /// - Increment the test identification number.
    ++TEST_ID;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsTValveController::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete mArticle;
    delete mConfig;
    delete mInput;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for TS21 Fluid Controller Manual T-Valve (3 way, 3 position) Controller Model
///           configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsTValveController::testConfigAndInput()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1,                                 mConfig->mAPath.mMinCmdPosition,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.9,                                 mConfig->mAPath.mMaxCmdPosition,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                 mConfig->mAPath.mMinFluidPosition,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,                                 mConfig->mAPath.mMaxFluidPosition,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.1,                                mConfig->mBPath.mMinCmdPosition,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.1,                                 mConfig->mBPath.mMaxCmdPosition,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                 mConfig->mBPath.mMinFluidPosition,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,                                 mConfig->mBPath.mMaxFluidPosition,        0.0);

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(TsTValveController::A_PATH                      == mInput->mPosition);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.9,                                 mInput->mAPath.mCmdPosition,              0.0);
    CPPUNIT_ASSERT(                                                  !mInput->mAPath.mManualPositionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                 mInput->mAPath.mManualPositionValue,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.1,                                mInput->mBPath.mCmdPosition,              0.0);
    CPPUNIT_ASSERT(                                                  !mInput->mBPath.mManualPositionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                 mInput->mBPath.mManualPositionValue,      0.0);

    /// @test    Configuration data default construction.
    TsTValveControllerConfigData defaultConfig;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                 defaultConfig.mAPath.mMinCmdPosition,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                 defaultConfig.mAPath.mMaxCmdPosition,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                 defaultConfig.mAPath.mMinFluidPosition,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                 defaultConfig.mAPath.mMaxFluidPosition,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                 defaultConfig.mBPath.mMinCmdPosition,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                 defaultConfig.mBPath.mMaxCmdPosition,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                 defaultConfig.mBPath.mMinFluidPosition,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                 defaultConfig.mBPath.mMaxFluidPosition,   0.0);

    /// @test    Input data default construction.
    TsTValveControllerInputData defaultInput;
    CPPUNIT_ASSERT(TsTValveController::NONE                        == defaultInput.mPosition);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                 defaultInput.mAPath.mCmdPosition,         0.0);
    CPPUNIT_ASSERT(                                                  !defaultInput.mAPath.mManualPositionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                 defaultInput.mAPath.mManualPositionValue, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                 defaultInput.mBPath.mCmdPosition,         0.0);
    CPPUNIT_ASSERT(                                                  !defaultInput.mBPath.mManualPositionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                 defaultInput.mBPath.mManualPositionValue, 0.0);

    /// @test    Configuration data copy construction.
    TsTValveControllerConfigData copyConfig(*mConfig);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mAPath.mMinCmdPosition,     copyConfig.mAPath.mMinCmdPosition,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mAPath.mMaxCmdPosition,     copyConfig.mAPath.mMaxCmdPosition,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mAPath.mMinFluidPosition,   copyConfig.mAPath.mMinFluidPosition,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mAPath.mMaxFluidPosition,   copyConfig.mAPath.mMaxFluidPosition,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mBPath.mMinCmdPosition,     copyConfig.mBPath.mMinCmdPosition,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mBPath.mMaxCmdPosition,     copyConfig.mBPath.mMaxCmdPosition,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mBPath.mMinFluidPosition,   copyConfig.mBPath.mMinFluidPosition,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfig->mBPath.mMaxFluidPosition,   copyConfig.mBPath.mMaxFluidPosition,      0.0);

    /// @test    Input data copy construction.
    TsTValveControllerInputData copyInput(*mInput);
    CPPUNIT_ASSERT(mInput->mPosition                               == copyInput.mPosition);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInput->mAPath.mCmdPosition,         copyInput.mAPath.mCmdPosition,            0.0);
    CPPUNIT_ASSERT(mInput->mAPath.mManualPositionFlag              == copyInput.mAPath.mManualPositionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInput->mAPath.mManualPositionValue, copyInput.mAPath.mManualPositionValue,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInput->mBPath.mCmdPosition,         copyInput.mBPath.mCmdPosition,            0.0);
    CPPUNIT_ASSERT(mInput->mBPath.mManualPositionFlag              == copyInput.mBPath.mManualPositionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInput->mBPath.mManualPositionValue, copyInput.mBPath.mManualPositionValue,    0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for TS21 Fluid Controller Manual T-Valve (3 way, 3 position) Controller Model
///           default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsTValveController::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Attributes.
    CPPUNIT_ASSERT(""                       == mArticle->mName);
    CPPUNIT_ASSERT(TsTValveController::NONE == mArticle->mPosition);
    CPPUNIT_ASSERT(                           !mArticle->mBPath.isInitialized());
    CPPUNIT_ASSERT(                           !mArticle->mAPath.isInitialized());
    CPPUNIT_ASSERT(                           !mArticle->mInitialized);

    /// @test    New/delete for code coverage.
    TsTValveController* article = new TsTValveController();
    CPPUNIT_ASSERT_NO_THROW(delete article);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for TS21 Fluid Controller Manual T-Valve (3 way, 3 position) Controller Model
///           nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsTValveController::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    mArticle->initialize(*mConfig, *mInput, mName);

    /// @test    Attributes.
    CPPUNIT_ASSERT(mName                      == mArticle->mName);
    CPPUNIT_ASSERT(TsTValveController::A_PATH == mArticle->mPosition);
    CPPUNIT_ASSERT(                              mArticle->mBPath.isInitialized());
    CPPUNIT_ASSERT(                              mArticle->mAPath.isInitialized());
    CPPUNIT_ASSERT(                              mArticle->mInitialized);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for TS21 Fluid Controller Manual T-Valve (3 way, 3 position) Controller Model
///           accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsTValveController::testAccessors()
{
    UT_RESULT;

    /// @test     isInitialized before initialization.
    CPPUNIT_ASSERT(                                                    !mArticle->isInitialized());

    /// - Initialize default constructed test article with nominal initialization data.
    mArticle->initialize(*mConfig, *mInput, mName);

    /// @test     isInitialized after initialization.
    CPPUNIT_ASSERT(                                                     mArticle->isInitialized());

    /// @test     getPosition.
    mArticle->mPosition = TsTValveController::NONE;
    CPPUNIT_ASSERT(TsTValveController::NONE                          == mArticle->getPosition());
    mArticle->mPosition = TsTValveController::A_PATH;
    CPPUNIT_ASSERT(TsTValveController::A_PATH                        == mArticle->getPosition());
    mArticle->mPosition = TsTValveController::B_PATH;
    CPPUNIT_ASSERT(TsTValveController::B_PATH                        == mArticle->getPosition());
    mArticle->mPosition = static_cast<TsTValveController::PositionType>(-1);
    CPPUNIT_ASSERT(static_cast<TsTValveController::PositionType>(-1) == mArticle->getPosition());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for TS21 Fluid Controller Manual T-Valve (3 way, 3 position) Controller Model
///           modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsTValveController::testModifiers()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    mArticle->initialize(*mConfig, *mInput, mName);

    /// @test    setPosition
    mArticle->setPosition(TsTValveController::NONE);
    CPPUNIT_ASSERT(TsTValveController::NONE                          == mArticle->mPosition);
    mArticle->setPosition(TsTValveController::A_PATH);
    CPPUNIT_ASSERT(TsTValveController::A_PATH                        == mArticle->mPosition);
    mArticle->setPosition(TsTValveController::B_PATH);
    CPPUNIT_ASSERT(TsTValveController::B_PATH                        == mArticle->mPosition);
    mArticle->setPosition(static_cast<TsTValveController::PositionType>(-1));
    CPPUNIT_ASSERT(static_cast<TsTValveController::PositionType>(-1) == mArticle->mPosition);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Tests for TS21 Fluid Controller Manual T-Valve (3 way, 3 position) Controller Model
///          update.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsTValveController::testUpdate()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    mArticle->initialize(*mConfig, *mInput, mName);

    /// @test select NONE
    mArticle->setPosition(TsTValveController::NONE);
    mArticle->update(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1,  mArticle->mAPath.getPosition(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.1, mArticle->mBPath.getPosition(), 0.0);

    /// @test select A
    mArticle->setPosition(TsTValveController::A_PATH);
    mArticle->update(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.9,  mArticle->mAPath.getPosition(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.1, mArticle->mBPath.getPosition(), 0.0);

    /// @test select B
    mArticle->setPosition(TsTValveController::B_PATH);
    mArticle->update(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1,  mArticle->mAPath.getPosition(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.1,  mArticle->mBPath.getPosition(), 0.0);

    /// @test select invalid (no change)
    mArticle->update(mTimeStep);
    mArticle->setPosition(static_cast<TsTValveController::PositionType>(-1));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1,  mArticle->mAPath.getPosition(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.1,  mArticle->mBPath.getPosition(), 0.0);
    mArticle->update(mTimeStep);
    mArticle->mPosition = static_cast<TsTValveController::PositionType>(-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1,  mArticle->mAPath.getPosition(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.1,  mArticle->mBPath.getPosition(), 0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for TS21 Fluid Controller Manual T-Valve (3 way, 3 position) Controller Model
///           initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsTValveController::testInitializationExceptions()
{
    UT_RESULT;

    /// @test    Initialization exception on empty name.
    CPPUNIT_ASSERT_THROW(mArticle->initialize(*mConfig, *mInput, ""),    TsInitializationException);
    CPPUNIT_ASSERT(!mArticle->mInitialized);

    /// @test  Exception on path A config data valve maximum position < valve minimum position.
    mConfig->mAPath.mMaxCmdPosition = mConfig->mAPath.mMinCmdPosition - FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(mArticle->initialize(*mConfig, *mInput, mName), TsInitializationException);
    CPPUNIT_ASSERT(!mArticle->isInitialized());
    mConfig->mAPath.mMaxCmdPosition = 1.0;

    /// @test  Exception on path B input data manual position  > max position.
    mInput->mBPath.mManualPositionFlag  = true;
    mInput->mBPath.mManualPositionValue = mConfig->mBPath.mMaxCmdPosition + FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(mArticle->initialize(*mConfig, *mInput, mName), TsInitializationException);
    CPPUNIT_ASSERT(!mArticle->isInitialized());
    mInput->mBPath.mManualPositionFlag  = false;
    mInput->mBPath.mManualPositionValue = 0.0;

    UT_PASS_LAST;
}
