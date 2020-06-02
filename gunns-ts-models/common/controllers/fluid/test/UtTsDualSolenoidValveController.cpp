/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((../TsDualSolenoidValveController.o))
***************************************************************************************************/

#include <iostream>

#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtTsDualSolenoidValveController.hh"
#include "aspects/electrical/UserLoad/UserLoadBase.hh"

/// @details  Test identification number.
int UtTsDualSolenoidValveController::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Dual Solenoid Valve Controller model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsDualSolenoidValveController::UtTsDualSolenoidValveController()
    :
    CppUnit::TestFixture(),
    tMinCmdPosition(0.0),
    tMaxCmdPosition(0.0),
    tMinFluidPosition(0.0),
    tMaxFluidPosition(0.0),
    tLatch(TsDualSolenoidValveController::LATCHING),
    tConfig(0),
    tCmdPosition(0.0),
    tManualPositionFlag(false),
    tManualPositionValue(0.0),
    tOpenSolenoidCmd(false),
    tCloseSolenoidCmd(false),
    tInput(0),
    tName(""),
    tArticle(0),
    tTimeStep(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Dual Solenoid Valve Controller model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsDualSolenoidValveController::~UtTsDualSolenoidValveController()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsDualSolenoidValveController::setUp()
{
    /// - Define the nominal configuration data.
    tMinCmdPosition      = -80.0;
    tMaxCmdPosition      = 80.0;
    tMinFluidPosition    = 0.1;
    tMaxFluidPosition    = 0.9;
    tLatch               = TsDualSolenoidValveController::NORMALLY_CLOSED;
    tConfig              = new TsDualSolenoidValveControllerConfigData(tMinCmdPosition,
                                                                       tMaxCmdPosition,
                                                                       tMinFluidPosition,
                                                                       tMaxFluidPosition,
                                                                       tLatch);

    /// - Define the nominal input data.
    tCmdPosition         = 40.0;
    tManualPositionFlag  = false;
    tManualPositionValue = 80.0;
    tOpenSolenoidCmd     = true;
    tCloseSolenoidCmd    = false;
    tInput               = new TsDualSolenoidValveControllerInputData(tCmdPosition,
                                                                      tManualPositionFlag,
                                                                      tManualPositionValue,
                                                                      tOpenSolenoidCmd,
                                                                      tCloseSolenoidCmd);

    /// - Default construct the nominal test article.
    tName                = "Test";
    tArticle             = new FriendlyTsDualSolenoidValveController();

    /// - Define the nominal time step and comparison tolerance.
    tTimeStep            = 0.1;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsDualSolenoidValveController::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete tArticle;
    delete tInput;
    delete tConfig;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Dual Solenoid Valve Controller model construction of configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsDualSolenoidValveController::testConfigAndInput()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data default construction.
    TsDualSolenoidValveControllerConfigData defaultConfig;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                             defaultConfig.mMinCmdPosition,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                             defaultConfig.mMaxCmdPosition,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                             defaultConfig.mMinFluidPosition,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                             defaultConfig.mMaxFluidPosition,      0.0);
    CPPUNIT_ASSERT(TsDualSolenoidValveController::LATCHING     == defaultConfig.mLatch);

    /// @test    Input data default construction.
    TsDualSolenoidValveControllerInputData  defaultInput;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                             defaultInput.mCmdPosition,            0.0);
    CPPUNIT_ASSERT(                                              !defaultInput.mManualPositionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                             defaultInput.mManualPositionValue,    0.0);
    CPPUNIT_ASSERT(                                              !defaultInput.mOpenSolenoidCmd);
    CPPUNIT_ASSERT(                                              !defaultInput.mCloseSolenoidCmd);

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMinCmdPosition,                 tConfig->mMinCmdPosition,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMaxCmdPosition,                 tConfig->mMaxCmdPosition,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMinFluidPosition,               tConfig->mMinFluidPosition,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMaxFluidPosition,               tConfig->mMaxFluidPosition,           0.0);
    CPPUNIT_ASSERT(tLatch                                      == tConfig->mLatch);

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCmdPosition,                    tInput->mCmdPosition,                 0.0);
    CPPUNIT_ASSERT(tManualPositionFlag                         == tInput->mManualPositionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tManualPositionValue,            tInput->mManualPositionValue,         0.0);
    CPPUNIT_ASSERT(tOpenSolenoidCmd                            == tInput->mOpenSolenoidCmd);
    CPPUNIT_ASSERT(tCloseSolenoidCmd                           == tInput->mCloseSolenoidCmd);

    /// @test    Configuration data copy construction.
    TsDualSolenoidValveControllerConfigData copyConfig(*tConfig);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tConfig->mMinCmdPosition,        copyConfig.mMinCmdPosition,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tConfig->mMaxCmdPosition,        copyConfig.mMaxCmdPosition,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tConfig->mMinFluidPosition,      copyConfig.mMinFluidPosition,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tConfig->mMaxFluidPosition,      copyConfig.mMaxFluidPosition,         0.0);
    CPPUNIT_ASSERT(tConfig->mLatch                             == copyConfig.mLatch);

    /// @test    Input data copy construction.
    TsDualSolenoidValveControllerInputData   copyInput(*tInput);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tInput->mCmdPosition,            copyInput.mCmdPosition,               0.0);
    CPPUNIT_ASSERT(tInput->mManualPositionFlag                 == copyInput.mManualPositionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tInput->mManualPositionValue,    copyInput.mManualPositionValue,       0.0);
    CPPUNIT_ASSERT(tInput->mOpenSolenoidCmd                    == copyInput.mOpenSolenoidCmd);
    CPPUNIT_ASSERT(tInput->mCloseSolenoidCmd                   == copyInput.mCloseSolenoidCmd);

    /// @test    Configuration data assignment operation.
    TsDualSolenoidValveControllerConfigData assignConfig;
    assignConfig = *tConfig;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tConfig->mMinCmdPosition,        assignConfig.mMinCmdPosition,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tConfig->mMaxCmdPosition,        assignConfig.mMaxCmdPosition,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tConfig->mMinFluidPosition,      assignConfig.mMinFluidPosition,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tConfig->mMaxFluidPosition,      assignConfig.mMaxFluidPosition,       0.0);
    CPPUNIT_ASSERT(tConfig->mLatch                             == assignConfig.mLatch);

    /// @test    Input data assignment operation.
    TsDualSolenoidValveControllerInputData assignInput;
    assignInput = *tInput;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tInput->mCmdPosition,            assignInput.mCmdPosition,             0.0);
    CPPUNIT_ASSERT(tInput->mManualPositionFlag                 == assignInput.mManualPositionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tInput->mManualPositionValue,    assignInput.mManualPositionValue,     0.0);
    CPPUNIT_ASSERT(tInput->mOpenSolenoidCmd                    == assignInput.mOpenSolenoidCmd);
    CPPUNIT_ASSERT(tInput->mCloseSolenoidCmd                   == assignInput.mCloseSolenoidCmd);

    /// @test    Configuration data assignment operation (to self).
//    assignConfig = assignConfig;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tConfig->mMinCmdPosition,        assignConfig.mMinCmdPosition,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tConfig->mMaxCmdPosition,        assignConfig.mMaxCmdPosition,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tConfig->mMinFluidPosition,      assignConfig.mMinFluidPosition,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tConfig->mMaxFluidPosition,      assignConfig.mMaxFluidPosition,       0.0);
    CPPUNIT_ASSERT(tConfig->mLatch                             == assignConfig.mLatch);

    /// @test    Input data assignment operation (to self).
    assignInput  = assignInput;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tInput->mCmdPosition,            assignInput.mCmdPosition,             0.0);
    CPPUNIT_ASSERT(tInput->mManualPositionFlag                 == assignInput.mManualPositionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tInput->mManualPositionValue,    assignInput.mManualPositionValue,     0.0);
    CPPUNIT_ASSERT(tInput->mOpenSolenoidCmd                    == assignInput.mOpenSolenoidCmd);
    CPPUNIT_ASSERT(tInput->mCloseSolenoidCmd                   == assignInput.mCloseSolenoidCmd);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Dual Solenoid Valve Controller model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsDualSolenoidValveController::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default values of attributes.
    CPPUNIT_ASSERT(""                                       == tArticle->mName);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                          tArticle->mMinCmdPosition,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                          tArticle->mMaxCmdPosition,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                          tArticle->mFluidBias,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                          tArticle->mFluidScale,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                          tArticle->mCmdPosition,            0.0);
    CPPUNIT_ASSERT(                                           !tArticle->mMalfValveStuckFlag);
    CPPUNIT_ASSERT(                                           !tArticle->mMalfValveFailToFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                          tArticle->mMalfValveFailToValue,   0.0);
    CPPUNIT_ASSERT(                                           !tArticle->mManualPositionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                          tArticle->mManualPositionValue,    0.0);
    CPPUNIT_ASSERT(                                           !tArticle->mMalfManualFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                          tArticle->mFluidPosition,          0.0);
    CPPUNIT_ASSERT(TsDualSolenoidValveController::LATCHING  == tArticle->mLatch);
    CPPUNIT_ASSERT(                                           !tArticle->mOpenSolenoidCmd);
    CPPUNIT_ASSERT(                                           !tArticle->mCloseSolenoidCmd);
    CPPUNIT_ASSERT(LoadOFF                                  == tArticle->mOpenSolenoidLoad);
    CPPUNIT_ASSERT(LoadOFF                                  == tArticle->mCloseSolenoidLoad);
    CPPUNIT_ASSERT(                                           !tArticle->mMalfOpenSolenoidFail);
    CPPUNIT_ASSERT(                                           !tArticle->mMalfCloseSolenoidFail);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                          tArticle->mMidCmdPosition,         0.0);
    CPPUNIT_ASSERT(                                           !tArticle->mInitialized);

    /// @test    New/delete for code coverage.
    TsDualSolenoidValveController* article = new TsDualSolenoidValveController();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Dual Solenoid Valve Controller model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsDualSolenoidValveController::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfig, *tInput, tName);

    /// @test    Nominal values of attributes.
    const double expectedMidCmd = 0.5 * (tMinCmdPosition + tMaxCmdPosition);
    CPPUNIT_ASSERT(tName                               == tArticle->mName);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMinCmdPosition,         tArticle->mMinCmdPosition,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMaxCmdPosition,         tArticle->mMaxCmdPosition,         0.0);
    CPPUNIT_ASSERT(tLatch                              == tArticle->mLatch);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCmdPosition,            tArticle->mCmdPosition,            0.0);
    CPPUNIT_ASSERT(                                      !tArticle->mMalfValveStuckFlag);
    CPPUNIT_ASSERT(                                      !tArticle->mMalfValveFailToFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                     tArticle->mMalfValveFailToValue,   0.0);
    CPPUNIT_ASSERT(tManualPositionFlag                 == tArticle->mManualPositionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tManualPositionValue,    tArticle->mManualPositionValue,    0.0);
    CPPUNIT_ASSERT(                                      !tArticle->mMalfManualFlag);
    CPPUNIT_ASSERT(tOpenSolenoidCmd                    == tArticle->mOpenSolenoidCmd);
    CPPUNIT_ASSERT(tCloseSolenoidCmd                   == tArticle->mCloseSolenoidCmd);
    CPPUNIT_ASSERT(LoadOFF                             == tArticle->mOpenSolenoidLoad);
    CPPUNIT_ASSERT(LoadOFF                             == tArticle->mCloseSolenoidLoad);
    CPPUNIT_ASSERT(                                      !tArticle->mMalfOpenSolenoidFail);
    CPPUNIT_ASSERT(                                      !tArticle->mMalfCloseSolenoidFail);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMidCmd,          tArticle->mMidCmdPosition,   DBL_EPSILON);
    CPPUNIT_ASSERT(                                       tArticle->mInitialized);

    /// @test    Reinitialization.
    tArticle->initialize(*tConfig, *tInput, tName);
    CPPUNIT_ASSERT(tName                               == tArticle->mName);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMinCmdPosition,         tArticle->mMinCmdPosition,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMaxCmdPosition,         tArticle->mMaxCmdPosition,         0.0);
    CPPUNIT_ASSERT(tLatch                              == tArticle->mLatch);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCmdPosition,            tArticle->mCmdPosition,            0.0);
    CPPUNIT_ASSERT(                                      !tArticle->mMalfValveStuckFlag);
    CPPUNIT_ASSERT(                                      !tArticle->mMalfValveFailToFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                     tArticle->mMalfValveFailToValue,   0.0);
    CPPUNIT_ASSERT(tManualPositionFlag                 == tArticle->mManualPositionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tManualPositionValue,    tArticle->mManualPositionValue,    0.0);
    CPPUNIT_ASSERT(                                      !tArticle->mMalfManualFlag);
    CPPUNIT_ASSERT(tOpenSolenoidCmd                    == tArticle->mOpenSolenoidCmd);
    CPPUNIT_ASSERT(tCloseSolenoidCmd                   == tArticle->mCloseSolenoidCmd);
    CPPUNIT_ASSERT(LoadOFF                             == tArticle->mOpenSolenoidLoad);
    CPPUNIT_ASSERT(LoadOFF                             == tArticle->mCloseSolenoidLoad);
    CPPUNIT_ASSERT(                                      !tArticle->mMalfOpenSolenoidFail);
    CPPUNIT_ASSERT(                                      !tArticle->mMalfCloseSolenoidFail);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMidCmd,          tArticle->mMidCmdPosition,   DBL_EPSILON);
    CPPUNIT_ASSERT(                                       tArticle->mInitialized);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Dual Solenoid Valve Controller model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsDualSolenoidValveController::testAccessors()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfig, *tInput, tName);

    /// @test     getOpenSolenoidLoad.
    tArticle->mOpenSolenoidLoad = LoadON;
    CPPUNIT_ASSERT(LoadON  == tArticle->getOpenSolenoidLoad());
    tArticle->mOpenSolenoidLoad = LoadOFF;
    CPPUNIT_ASSERT(LoadOFF == tArticle->getOpenSolenoidLoad());

    /// @test     getCloseSolenoidLoad.
    tArticle->mCloseSolenoidLoad = LoadON;
    CPPUNIT_ASSERT(LoadON  == tArticle->getCloseSolenoidLoad());
    tArticle->mCloseSolenoidLoad = LoadOFF;
    CPPUNIT_ASSERT(LoadOFF == tArticle->getCloseSolenoidLoad());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Dual Solenoid Valve Controller model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsDualSolenoidValveController::testModifiers()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfig, *tInput, tName);

    /// @test     setOpenSolenoidCmd.
    tArticle->setOpenSolenoidCmd(false);
    CPPUNIT_ASSERT(false == tArticle->mOpenSolenoidCmd);
    tArticle->setOpenSolenoidCmd(true);
    CPPUNIT_ASSERT(true  == tArticle->mOpenSolenoidCmd);

    /// @test     setCloseSolenoidCmd.
    tArticle->setCloseSolenoidCmd(false);
    CPPUNIT_ASSERT(false == tArticle->mCloseSolenoidCmd);
    tArticle->setCloseSolenoidCmd(true);
    CPPUNIT_ASSERT(true  == tArticle->mCloseSolenoidCmd);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Dual Solenoid Valve Controller model update state (nominal).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsDualSolenoidValveController::testUpdateNominal()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfig, *tInput, tName);

    /// @test    Update with no solenoids driven, verify valve goes to the nearest latching
    ///          position (closed).
    tArticle->setOpenSolenoidCmd(false);
    tArticle->setCloseSolenoidCmd(false);
    tArticle->update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMinCmdPosition, tArticle->getPosition(), 0.0);
    CPPUNIT_ASSERT(LoadOFF == tArticle->getOpenSolenoidLoad());
    CPPUNIT_ASSERT(LoadOFF == tArticle->getCloseSolenoidLoad());

    /// @test    Update with no solenoids driven, verify valve goes to the nearest latching
    ///          position (open).
    tArticle->mLatch = TsDualSolenoidValveController::NORMALLY_OPEN;
    tArticle->mCmdPosition = -40.0;
    tArticle->update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMaxCmdPosition, tArticle->getPosition(), 0.0);
    CPPUNIT_ASSERT(LoadOFF == tArticle->getOpenSolenoidLoad());
    CPPUNIT_ASSERT(LoadOFF == tArticle->getCloseSolenoidLoad());

    /// @test    Update with no solenoids driven, verify valve goes to the nearest latching
    ///          position (either/closed).
    tArticle->mLatch = TsDualSolenoidValveController::LATCHING;
    tArticle->mCmdPosition = -40.0;
    tArticle->update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMinCmdPosition, tArticle->getPosition(), 0.0);
    CPPUNIT_ASSERT(LoadOFF == tArticle->getOpenSolenoidLoad());
    CPPUNIT_ASSERT(LoadOFF == tArticle->getCloseSolenoidLoad());

    /// @test    Update with no solenoids driven, verify valve goes to the nearest latching
    ///          position (either/open).
    tArticle->mLatch = TsDualSolenoidValveController::LATCHING;
    tArticle->mCmdPosition = 1.0;
    tArticle->update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMaxCmdPosition, tArticle->getPosition(), 0.0);
    CPPUNIT_ASSERT(LoadOFF == tArticle->getOpenSolenoidLoad());
    CPPUNIT_ASSERT(LoadOFF == tArticle->getCloseSolenoidLoad());

    /// @test    Update with closed solenoid driven, verify valve goes to closed.
    tArticle->setOpenSolenoidCmd(false);
    tArticle->setCloseSolenoidCmd(true);
    tArticle->mCmdPosition = 1.0;
    tArticle->update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMinCmdPosition, tArticle->getPosition(), 0.0);
    CPPUNIT_ASSERT(LoadOFF == tArticle->getOpenSolenoidLoad());
    CPPUNIT_ASSERT(LoadON  == tArticle->getCloseSolenoidLoad());

    /// @test    Update with open solenoid driven, verify valve goes to open.
    tArticle->setOpenSolenoidCmd(true);
    tArticle->setCloseSolenoidCmd(false);
    tArticle->mCmdPosition = 0.0;
    tArticle->update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMaxCmdPosition, tArticle->getPosition(), 0.0);
    CPPUNIT_ASSERT(LoadON  == tArticle->getOpenSolenoidLoad());
    CPPUNIT_ASSERT(LoadOFF == tArticle->getCloseSolenoidLoad());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Dual Solenoid Valve Controller model update state (both solenoids driven).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsDualSolenoidValveController::testUpdateDualDrive()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfig, *tInput, tName);

    /// @test    Update with both solenoids driven, verify valve goes to the nearest latching
    ///          position (closed).
    tArticle->setOpenSolenoidCmd(true);
    tArticle->setCloseSolenoidCmd(true);
    tArticle->update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMinCmdPosition, tArticle->getPosition(), 0.0);
    CPPUNIT_ASSERT(LoadON == tArticle->getOpenSolenoidLoad());
    CPPUNIT_ASSERT(LoadON == tArticle->getCloseSolenoidLoad());

    /// @test    Update with no solenoids driven, verify valve goes to the nearest latching
    ///          position (open).
    tArticle->mLatch = TsDualSolenoidValveController::NORMALLY_OPEN;
    tArticle->mCmdPosition = -40.0;
    tArticle->update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMaxCmdPosition, tArticle->getPosition(), 0.0);
    CPPUNIT_ASSERT(LoadON == tArticle->getOpenSolenoidLoad());
    CPPUNIT_ASSERT(LoadON == tArticle->getCloseSolenoidLoad());

    /// @test    Update with no solenoids driven, verify valve goes to the nearest latching
    ///          position (either/closed).
    tArticle->mLatch = TsDualSolenoidValveController::LATCHING;
    tArticle->mCmdPosition = -40.0;
    tArticle->update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMinCmdPosition, tArticle->getPosition(), 0.0);
    CPPUNIT_ASSERT(LoadON == tArticle->getOpenSolenoidLoad());
    CPPUNIT_ASSERT(LoadON == tArticle->getCloseSolenoidLoad());

    /// @test    Update with no solenoids driven, verify valve goes to the nearest latching
    ///          position (either/open).
    tArticle->mLatch = TsDualSolenoidValveController::LATCHING;
    tArticle->mCmdPosition = 1.0;
    tArticle->update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMaxCmdPosition, tArticle->getPosition(), 0.0);
    CPPUNIT_ASSERT(LoadON == tArticle->getOpenSolenoidLoad());
    CPPUNIT_ASSERT(LoadON == tArticle->getCloseSolenoidLoad());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Dual Solenoid Valve Controller model update state (malfunctions).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsDualSolenoidValveController::testUpdateMalfunctions()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfig, *tInput, tName);

    /// @test    Update with open solenoid driven but failed, verify valve goes to the nearest
    ///          latching position (closed).
    tArticle->setOpenSolenoidCmd(true);
    tArticle->setCloseSolenoidCmd(false);
    tArticle->mMalfOpenSolenoidFail = true;
    tArticle->update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMinCmdPosition, tArticle->getPosition(), 0.0);
    CPPUNIT_ASSERT(LoadOFF == tArticle->getOpenSolenoidLoad());
    CPPUNIT_ASSERT(LoadOFF == tArticle->getCloseSolenoidLoad());

    /// @test    Reset the drive and verify valve remains in place.
    tArticle->setOpenSolenoidCmd(false);
    tArticle->update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMinCmdPosition, tArticle->getPosition(), 0.0);
    CPPUNIT_ASSERT(LoadOFF == tArticle->getOpenSolenoidLoad());
    CPPUNIT_ASSERT(LoadOFF == tArticle->getCloseSolenoidLoad());

    /// @test    Reset the malf and verify valve remains in place.
    tArticle->mMalfOpenSolenoidFail = false;
    tArticle->update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMinCmdPosition, tArticle->getPosition(), 0.0);
    CPPUNIT_ASSERT(LoadOFF == tArticle->getOpenSolenoidLoad());
    CPPUNIT_ASSERT(LoadOFF == tArticle->getCloseSolenoidLoad());

    /// @test    Update with close solenoid driven but failed, verify valve goes to the nearest
    ///          latching position (open).
    tArticle->setOpenSolenoidCmd(false);
    tArticle->setCloseSolenoidCmd(true);
    tArticle->mMalfCloseSolenoidFail = true;
    tArticle->mLatch = TsDualSolenoidValveController::NORMALLY_OPEN;
    tArticle->update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMaxCmdPosition, tArticle->getPosition(), 0.0);
    CPPUNIT_ASSERT(LoadOFF == tArticle->getOpenSolenoidLoad());
    CPPUNIT_ASSERT(LoadOFF == tArticle->getCloseSolenoidLoad());

    /// @test    Reset the drive and verify valve remains in place.
    tArticle->setCloseSolenoidCmd(false);
    tArticle->update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMaxCmdPosition, tArticle->getPosition(), 0.0);
    CPPUNIT_ASSERT(LoadOFF == tArticle->getOpenSolenoidLoad());
    CPPUNIT_ASSERT(LoadOFF == tArticle->getCloseSolenoidLoad());

    /// @test    Reset the malf and verify valve remains in place.
    tArticle->mMalfCloseSolenoidFail = false;
    tArticle->update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMaxCmdPosition, tArticle->getPosition(), 0.0);
    CPPUNIT_ASSERT(LoadOFF == tArticle->getOpenSolenoidLoad());
    CPPUNIT_ASSERT(LoadOFF == tArticle->getCloseSolenoidLoad());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Dual Solenoid Valve Controller model initialization with exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsDualSolenoidValveController::testInitializationExceptions()
{
    UT_RESULT;

    /// @test    Exception is thrown if both solenoid commands are set in input data.
    tInput->mOpenSolenoidCmd  = true;
    tInput->mCloseSolenoidCmd = true;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfig, *tInput, tName), TsInitializationException);
    CPPUNIT_ASSERT(!tArticle->mInitialized);

    UT_PASS_LAST;
}
