/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((../TsOpenCloseValveAssembly.o))
***************************************************************************************************/

#include <iostream>

#include "software/exceptions/TsInitializationException.hh"

#include "UtTsOpenCloseValveAssembly.hh"
#include "aspects/electrical/UserLoad/UserLoadBase.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Open/Close Valve Assembly model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsOpenCloseValveAssembly::UtTsOpenCloseValveAssembly()
    :
    CppUnit::TestFixture(),
    cController(),
    cSensorOpen(),
    cSensorClosed(),
    tConfig(0),
    iController(),
    iSensorOpen(),
    iSensorClosed(),
    tInput(0),
    tName(""),
    tArticle(0),
    tTimeStep(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Open/Close Valve Assembly model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsOpenCloseValveAssembly::~UtTsOpenCloseValveAssembly()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpenCloseValveAssembly::setUp()
{
    /// - Define the nominal configuration data.
    cController.mMinCmdPosition      = 0.0;
    cController.mMaxCmdPosition      = 1.0;
    cController.mMinFluidPosition    = 0.0;
    cController.mMaxFluidPosition    = 1.0;
    cController.mTransitTime         = 0.1;
    cController.mRefCmd              = 1.0;
    cController.mHoldPower           = 10.0;
    cController.mMovePower           = 15.0;
    cController.mStuckPower          = 20.0;
    cController.mRefVoltage          = 120.0;
    cController.mEotInterrupt        = true;
    cController.mLatch               = TsPoweredValveController::LATCHING;
    cSensorOpen.mOffValue            = false;
    cSensorOpen.mTarget              = 1.0;
    cSensorOpen.mTolerance           = 0.1;
    cSensorClosed.mOffValue          = false;
    cSensorClosed.mTarget            = 0.0;
    cSensorClosed.mTolerance         = 0.1;
    tConfig                          = new TsOpenCloseValveAssemblyConfigData(cController,
                                                                              cSensorOpen,
                                                                              cSensorClosed);

    /// - Define the nominal input data.
    iController.mCmdPosition         = 0.0;
    iController.mManualPositionFlag  = false;
    iController.mManualPositionValue = 0.0;
    iController.mSupplyVoltageFlag   = true;
    iController.mEnabledFlag         = false;
    iController.mCmd                 = 1.0;
    iSensorOpen.mPowerFlag           = true;
    iSensorOpen.mTruthInput          = false;
    iSensorOpen.mTruthInputAnalog    = 0.0;
    iSensorClosed.mPowerFlag         = true;
    iSensorClosed.mTruthInput        = false;
    iSensorClosed.mTruthInputAnalog  = 0.0;
    tInput                           = new TsOpenCloseValveAssemblyInputData(iController,
                                                                             iSensorOpen,
                                                                             iSensorClosed);

    /// - Default construct the nominal test article.
    tName                = "Test";
    tArticle             = new FriendlyTsOpenCloseValveAssembly();

    /// - Define the nominal time step and comparison tolerance.
    tTimeStep            = 0.1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpenCloseValveAssembly::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete tArticle;
    delete tInput;
    delete tConfig;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Open/Close Valve Assembly model construction of configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpenCloseValveAssembly::testConfigAndInput()
{
    std::cout << "................................................................................";
    std::cout << "\n.Open/Close Valve Assembly 01: Configuration And Input Data Test........";

    /// @test    Configuration data default construction.
    TsOpenCloseValveAssemblyConfigData defaultConfig;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                    defaultConfig.mController.mMinCmdPosition,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                    defaultConfig.mController.mMaxCmdPosition,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                    defaultConfig.mController.mMinFluidPosition,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                    defaultConfig.mController.mMaxFluidPosition,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                    defaultConfig.mController.mTransitTime,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                    defaultConfig.mController.mRefCmd,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                    defaultConfig.mController.mHoldPower,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                    defaultConfig.mController.mMovePower,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                    defaultConfig.mController.mStuckPower,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                    defaultConfig.mController.mRefVoltage,         0.0);
    CPPUNIT_ASSERT(                                     !defaultConfig.mController.mEotInterrupt);
    CPPUNIT_ASSERT(TsPoweredValveController::LATCHING == defaultConfig.mController.mLatch);
    CPPUNIT_ASSERT(                                     !defaultConfig.mSensorOpen.mOffValue);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                    defaultConfig.mSensorOpen.mTarget,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                    defaultConfig.mSensorOpen.mTolerance,          0.0);
    CPPUNIT_ASSERT(                                     !defaultConfig.mSensorClosed.mOffValue);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                    defaultConfig.mSensorClosed.mTarget,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                    defaultConfig.mSensorClosed.mTolerance,        0.0);

    /// @test    Input data default construction.
    TsOpenCloseValveAssemblyInputData  defaultInput;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         defaultInput.mController.mCmdPosition,         0.0);
    CPPUNIT_ASSERT(                                          !defaultInput.mController.mManualPositionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         defaultInput.mController.mManualPositionValue, 0.0);
    CPPUNIT_ASSERT(                                          !defaultInput.mController.mSupplyVoltageFlag);
    CPPUNIT_ASSERT(                                          !defaultInput.mController.mEnabledFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         defaultInput.mController.mCmd,                 0.0);
    CPPUNIT_ASSERT(                                          !defaultInput.mSensorOpen.mPowerFlag);
    CPPUNIT_ASSERT(                                          !defaultInput.mSensorOpen.mTruthInput);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         defaultInput.mSensorOpen.mTruthInputAnalog,    0.0);
    CPPUNIT_ASSERT(                                          !defaultInput.mSensorClosed.mPowerFlag);
    CPPUNIT_ASSERT(                                          !defaultInput.mSensorClosed.mTruthInput);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         defaultInput.mSensorClosed.mTruthInputAnalog,  0.0);

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(cController.mMinCmdPosition             == tConfig->mController.mMinCmdPosition);
    CPPUNIT_ASSERT(cController.mMaxCmdPosition             == tConfig->mController.mMaxCmdPosition);
    CPPUNIT_ASSERT(cController.mMinFluidPosition           == tConfig->mController.mMinFluidPosition);
    CPPUNIT_ASSERT(cController.mMaxFluidPosition           == tConfig->mController.mMaxFluidPosition);
    CPPUNIT_ASSERT(cController.mTransitTime                == tConfig->mController.mTransitTime);
    CPPUNIT_ASSERT(cController.mRefCmd                     == tConfig->mController.mRefCmd);
    CPPUNIT_ASSERT(cController.mHoldPower                  == tConfig->mController.mHoldPower);
    CPPUNIT_ASSERT(cController.mMovePower                  == tConfig->mController.mMovePower);
    CPPUNIT_ASSERT(cController.mStuckPower                 == tConfig->mController.mStuckPower);
    CPPUNIT_ASSERT(cController.mRefVoltage                 == tConfig->mController.mRefVoltage);
    CPPUNIT_ASSERT(cController.mEotInterrupt               == tConfig->mController.mEotInterrupt);
    CPPUNIT_ASSERT(cController.mLatch                      == tConfig->mController.mLatch);
    CPPUNIT_ASSERT(cSensorOpen.mOffValue                   == tConfig->mSensorOpen.mOffValue);
    CPPUNIT_ASSERT(cSensorOpen.mTarget                     == tConfig->mSensorOpen.mTarget);
    CPPUNIT_ASSERT(cSensorOpen.mTolerance                  == tConfig->mSensorOpen.mTolerance);
    CPPUNIT_ASSERT(cSensorClosed.mOffValue                 == tConfig->mSensorClosed.mOffValue);
    CPPUNIT_ASSERT(cSensorClosed.mTarget                   == tConfig->mSensorClosed.mTarget);
    CPPUNIT_ASSERT(cSensorClosed.mTolerance                == tConfig->mSensorClosed.mTolerance);

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(iController.mCmdPosition                == tInput->mController.mCmdPosition);
    CPPUNIT_ASSERT(iController.mManualPositionFlag         == tInput->mController.mManualPositionFlag);
    CPPUNIT_ASSERT(iController.mManualPositionValue        == tInput->mController.mManualPositionValue);
    CPPUNIT_ASSERT(iController.mSupplyVoltageFlag          == tInput->mController.mSupplyVoltageFlag);
    CPPUNIT_ASSERT(iController.mEnabledFlag                == tInput->mController.mEnabledFlag);
    CPPUNIT_ASSERT(iController.mCmd                        == tInput->mController.mCmd);
    CPPUNIT_ASSERT(iSensorOpen.mPowerFlag                  == tInput->mSensorOpen.mPowerFlag);
    CPPUNIT_ASSERT(iSensorOpen.mTruthInput                 == tInput->mSensorOpen.mTruthInput);
    CPPUNIT_ASSERT(iSensorOpen.mTruthInputAnalog           == tInput->mSensorOpen.mTruthInputAnalog);
    CPPUNIT_ASSERT(iSensorClosed.mPowerFlag                == tInput->mSensorClosed.mPowerFlag);
    CPPUNIT_ASSERT(iSensorClosed.mTruthInput               == tInput->mSensorClosed.mTruthInput);
    CPPUNIT_ASSERT(iSensorClosed.mTruthInputAnalog         == tInput->mSensorClosed.mTruthInputAnalog);

    /// @test    Configuration data copy construction.
    TsOpenCloseValveAssemblyConfigData copyConfig(*tConfig);
    CPPUNIT_ASSERT(cController.mMinCmdPosition             == copyConfig.mController.mMinCmdPosition);
    CPPUNIT_ASSERT(cController.mMaxCmdPosition             == copyConfig.mController.mMaxCmdPosition);
    CPPUNIT_ASSERT(cController.mMinFluidPosition           == copyConfig.mController.mMinFluidPosition);
    CPPUNIT_ASSERT(cController.mMaxFluidPosition           == copyConfig.mController.mMaxFluidPosition);
    CPPUNIT_ASSERT(cController.mTransitTime                == copyConfig.mController.mTransitTime);
    CPPUNIT_ASSERT(cController.mRefCmd                     == copyConfig.mController.mRefCmd);
    CPPUNIT_ASSERT(cController.mHoldPower                  == copyConfig.mController.mHoldPower);
    CPPUNIT_ASSERT(cController.mMovePower                  == copyConfig.mController.mMovePower);
    CPPUNIT_ASSERT(cController.mStuckPower                 == copyConfig.mController.mStuckPower);
    CPPUNIT_ASSERT(cController.mRefVoltage                 == copyConfig.mController.mRefVoltage);
    CPPUNIT_ASSERT(cController.mEotInterrupt               == copyConfig.mController.mEotInterrupt);
    CPPUNIT_ASSERT(cController.mLatch                      == copyConfig.mController.mLatch);
    CPPUNIT_ASSERT(cSensorOpen.mOffValue                   == copyConfig.mSensorOpen.mOffValue);
    CPPUNIT_ASSERT(cSensorOpen.mTarget                     == copyConfig.mSensorOpen.mTarget);
    CPPUNIT_ASSERT(cSensorOpen.mTolerance                  == copyConfig.mSensorOpen.mTolerance);
    CPPUNIT_ASSERT(cSensorClosed.mOffValue                 == copyConfig.mSensorClosed.mOffValue);
    CPPUNIT_ASSERT(cSensorClosed.mTarget                   == copyConfig.mSensorClosed.mTarget);
    CPPUNIT_ASSERT(cSensorClosed.mTolerance                == copyConfig.mSensorClosed.mTolerance);

    /// @test    Input data copy construction.
    TsOpenCloseValveAssemblyInputData   copyInput(*tInput);
    CPPUNIT_ASSERT(iController.mCmdPosition                == copyInput.mController.mCmdPosition);
    CPPUNIT_ASSERT(iController.mManualPositionFlag         == copyInput.mController.mManualPositionFlag);
    CPPUNIT_ASSERT(iController.mManualPositionValue        == copyInput.mController.mManualPositionValue);
    CPPUNIT_ASSERT(iController.mSupplyVoltageFlag          == copyInput.mController.mSupplyVoltageFlag);
    CPPUNIT_ASSERT(iController.mEnabledFlag                == copyInput.mController.mEnabledFlag);
    CPPUNIT_ASSERT(iController.mCmd                        == copyInput.mController.mCmd);
    CPPUNIT_ASSERT(iSensorOpen.mPowerFlag                  == copyInput.mSensorOpen.mPowerFlag);
    CPPUNIT_ASSERT(iSensorOpen.mTruthInput                 == copyInput.mSensorOpen.mTruthInput);
    CPPUNIT_ASSERT(iSensorOpen.mTruthInputAnalog           == copyInput.mSensorOpen.mTruthInputAnalog);
    CPPUNIT_ASSERT(iSensorClosed.mPowerFlag                == copyInput.mSensorClosed.mPowerFlag);
    CPPUNIT_ASSERT(iSensorClosed.mTruthInput               == copyInput.mSensorClosed.mTruthInput);
    CPPUNIT_ASSERT(iSensorClosed.mTruthInputAnalog         == copyInput.mSensorClosed.mTruthInputAnalog);

    /// @test    Configuration data assignment operation.
    TsOpenCloseValveAssemblyConfigData assignConfig;
    assignConfig = *tConfig;
    CPPUNIT_ASSERT(cController.mMinCmdPosition             == assignConfig.mController.mMinCmdPosition);
    CPPUNIT_ASSERT(cController.mMaxCmdPosition             == assignConfig.mController.mMaxCmdPosition);
    CPPUNIT_ASSERT(cController.mMinFluidPosition           == assignConfig.mController.mMinFluidPosition);
    CPPUNIT_ASSERT(cController.mMaxFluidPosition           == assignConfig.mController.mMaxFluidPosition);
    CPPUNIT_ASSERT(cController.mTransitTime                == assignConfig.mController.mTransitTime);
    CPPUNIT_ASSERT(cController.mRefCmd                     == assignConfig.mController.mRefCmd);
    CPPUNIT_ASSERT(cController.mHoldPower                  == assignConfig.mController.mHoldPower);
    CPPUNIT_ASSERT(cController.mMovePower                  == assignConfig.mController.mMovePower);
    CPPUNIT_ASSERT(cController.mStuckPower                 == assignConfig.mController.mStuckPower);
    CPPUNIT_ASSERT(cController.mRefVoltage                 == assignConfig.mController.mRefVoltage);
    CPPUNIT_ASSERT(cController.mEotInterrupt               == assignConfig.mController.mEotInterrupt);
    CPPUNIT_ASSERT(cController.mLatch                      == assignConfig.mController.mLatch);
    CPPUNIT_ASSERT(cSensorOpen.mOffValue                   == assignConfig.mSensorOpen.mOffValue);
    CPPUNIT_ASSERT(cSensorOpen.mTarget                     == assignConfig.mSensorOpen.mTarget);
    CPPUNIT_ASSERT(cSensorOpen.mTolerance                  == assignConfig.mSensorOpen.mTolerance);
    CPPUNIT_ASSERT(cSensorClosed.mOffValue                 == assignConfig.mSensorClosed.mOffValue);
    CPPUNIT_ASSERT(cSensorClosed.mTarget                   == assignConfig.mSensorClosed.mTarget);
    CPPUNIT_ASSERT(cSensorClosed.mTolerance                == assignConfig.mSensorClosed.mTolerance);

    /// @test    Input data assignment operation.
    TsOpenCloseValveAssemblyInputData assignInput;
    assignInput = *tInput;
    CPPUNIT_ASSERT(iController.mCmdPosition                == assignInput.mController.mCmdPosition);
    CPPUNIT_ASSERT(iController.mManualPositionFlag         == assignInput.mController.mManualPositionFlag);
    CPPUNIT_ASSERT(iController.mManualPositionValue        == assignInput.mController.mManualPositionValue);
    CPPUNIT_ASSERT(iController.mSupplyVoltageFlag          == assignInput.mController.mSupplyVoltageFlag);
    CPPUNIT_ASSERT(iController.mEnabledFlag                == assignInput.mController.mEnabledFlag);
    CPPUNIT_ASSERT(iController.mCmd                        == assignInput.mController.mCmd);
    CPPUNIT_ASSERT(iSensorOpen.mPowerFlag                  == assignInput.mSensorOpen.mPowerFlag);
    CPPUNIT_ASSERT(iSensorOpen.mTruthInput                 == assignInput.mSensorOpen.mTruthInput);
    CPPUNIT_ASSERT(iSensorOpen.mTruthInputAnalog           == assignInput.mSensorOpen.mTruthInputAnalog);
    CPPUNIT_ASSERT(iSensorClosed.mPowerFlag                == assignInput.mSensorClosed.mPowerFlag);
    CPPUNIT_ASSERT(iSensorClosed.mTruthInput               == assignInput.mSensorClosed.mTruthInput);
    CPPUNIT_ASSERT(iSensorClosed.mTruthInputAnalog         == assignInput.mSensorClosed.mTruthInputAnalog);

    /// @test    Configuration data assignment operation (to self).
    assignConfig = assignConfig;
    CPPUNIT_ASSERT(cController.mMinCmdPosition             == assignConfig.mController.mMinCmdPosition);
    CPPUNIT_ASSERT(cController.mMaxCmdPosition             == assignConfig.mController.mMaxCmdPosition);
    CPPUNIT_ASSERT(cController.mMinFluidPosition           == assignConfig.mController.mMinFluidPosition);
    CPPUNIT_ASSERT(cController.mMaxFluidPosition           == assignConfig.mController.mMaxFluidPosition);
    CPPUNIT_ASSERT(cController.mTransitTime                == assignConfig.mController.mTransitTime);
    CPPUNIT_ASSERT(cController.mRefCmd                     == assignConfig.mController.mRefCmd);
    CPPUNIT_ASSERT(cController.mHoldPower                  == assignConfig.mController.mHoldPower);
    CPPUNIT_ASSERT(cController.mMovePower                  == assignConfig.mController.mMovePower);
    CPPUNIT_ASSERT(cController.mStuckPower                 == assignConfig.mController.mStuckPower);
    CPPUNIT_ASSERT(cController.mRefVoltage                 == assignConfig.mController.mRefVoltage);
    CPPUNIT_ASSERT(cController.mEotInterrupt               == assignConfig.mController.mEotInterrupt);
    CPPUNIT_ASSERT(cController.mLatch                      == assignConfig.mController.mLatch);
    CPPUNIT_ASSERT(cSensorOpen.mOffValue                   == assignConfig.mSensorOpen.mOffValue);
    CPPUNIT_ASSERT(cSensorOpen.mTarget                     == assignConfig.mSensorOpen.mTarget);
    CPPUNIT_ASSERT(cSensorOpen.mTolerance                  == assignConfig.mSensorOpen.mTolerance);
    CPPUNIT_ASSERT(cSensorClosed.mOffValue                 == assignConfig.mSensorClosed.mOffValue);
    CPPUNIT_ASSERT(cSensorClosed.mTarget                   == assignConfig.mSensorClosed.mTarget);
    CPPUNIT_ASSERT(cSensorClosed.mTolerance                == assignConfig.mSensorClosed.mTolerance);

    /// @test    Input data assignment operation (to self).
    assignInput  = assignInput;
    CPPUNIT_ASSERT(iController.mCmdPosition                == assignInput.mController.mCmdPosition);
    CPPUNIT_ASSERT(iController.mManualPositionFlag         == assignInput.mController.mManualPositionFlag);
    CPPUNIT_ASSERT(iController.mManualPositionValue        == assignInput.mController.mManualPositionValue);
    CPPUNIT_ASSERT(iController.mSupplyVoltageFlag          == assignInput.mController.mSupplyVoltageFlag);
    CPPUNIT_ASSERT(iController.mEnabledFlag                == assignInput.mController.mEnabledFlag);
    CPPUNIT_ASSERT(iController.mCmd                        == assignInput.mController.mCmd);
    CPPUNIT_ASSERT(iSensorOpen.mPowerFlag                  == assignInput.mSensorOpen.mPowerFlag);
    CPPUNIT_ASSERT(iSensorOpen.mTruthInput                 == assignInput.mSensorOpen.mTruthInput);
    CPPUNIT_ASSERT(iSensorOpen.mTruthInputAnalog           == assignInput.mSensorOpen.mTruthInputAnalog);
    CPPUNIT_ASSERT(iSensorClosed.mPowerFlag                == assignInput.mSensorClosed.mPowerFlag);
    CPPUNIT_ASSERT(iSensorClosed.mTruthInput               == assignInput.mSensorClosed.mTruthInput);
    CPPUNIT_ASSERT(iSensorClosed.mTruthInputAnalog         == assignInput.mSensorClosed.mTruthInputAnalog);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Open/Close Valve Assembly model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpenCloseValveAssembly::testDefaultConstruction()
{
    std::cout << "\n.Open/Close Valve Assembly 02: Default Construction Test................";

    /// @test    Default values of attributes.
    CPPUNIT_ASSERT(0.0                                     == tArticle->mController.getCmdScale());
    CPPUNIT_ASSERT(false                                   == tArticle->mController.getSupplyVoltageFlag());
    CPPUNIT_ASSERT(0.0                                     == tArticle->mController.getPower());
    CPPUNIT_ASSERT(0.0                                     == tArticle->mController.getResistance());
    CPPUNIT_ASSERT(                                          !tArticle->mSensorOpen.getSensedOutput());
    CPPUNIT_ASSERT(                                          !tArticle->mSensorClosed.getSensedOutput());
    CPPUNIT_ASSERT(                                          !tArticle->mInitialized);

    /// @test    New/delete for code coverage.
    TsOpenCloseValveAssembly* article = new TsOpenCloseValveAssembly();
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Open/Close Valve Assembly model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpenCloseValveAssembly::testNominalInitialization()
{
    std::cout << "\n.Open/Close Valve Assembly 03: Nominal Initialization Test..............";

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfig, *tInput, tName);

    /// @test    Nominal values of attributes.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(10.0,                        tArticle->mController.getCmdScale(), DBL_EPSILON);
    CPPUNIT_ASSERT(true                                    == tArticle->mController.getSupplyVoltageFlag());
    CPPUNIT_ASSERT(                                          !tArticle->mSensorOpen.getSensedOutput());
    CPPUNIT_ASSERT(                                           tArticle->mSensorClosed.getSensedOutput());
    CPPUNIT_ASSERT(                                           tArticle->mInitialized);

    /// @test    Reinitialization.
    tArticle->initialize(*tConfig, *tInput, tName);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(10.0,                        tArticle->mController.getCmdScale(), DBL_EPSILON);
    CPPUNIT_ASSERT(true                                    == tArticle->mController.getSupplyVoltageFlag());
    CPPUNIT_ASSERT(                                          !tArticle->mSensorOpen.getSensedOutput());
    CPPUNIT_ASSERT(                                           tArticle->mSensorClosed.getSensedOutput());
    CPPUNIT_ASSERT(                                           tArticle->mInitialized);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Open/Close Valve Controller model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpenCloseValveAssembly::testInitializationFailure()
{
    std::cout << "\n.Open/Close Valve Assembly 04: Initialization Failure Test..............";

    /// @test    Assembly fails to init if controller fails to init.
    tConfig->mController.mMinCmdPosition = 1000.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfig, *tInput, tName), TsInitializationException);
    CPPUNIT_ASSERT(                                          !tArticle->mInitialized);

    /// @test    Assembly fails to init if open sensor fails to init.
    tConfig->mController.mMinCmdPosition =  0.0;
    tConfig->mSensorOpen.mTolerance      = -1.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfig, *tInput, tName), TsInitializationException);
    CPPUNIT_ASSERT(                                          !tArticle->mInitialized);

    /// @test    Assembly fails to init if closed sensor fails to init.
    tConfig->mSensorClosed.mTolerance    =  0.1;
    tConfig->mSensorClosed.mTolerance    = -1.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfig, *tInput, tName), TsInitializationException);
    CPPUNIT_ASSERT(                                          !tArticle->mInitialized);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Open/Close Valve Assembly model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpenCloseValveAssembly::testAccessors()
{
    std::cout << "\n.Open/Close Valve Assembly 05: Accessors Test...........................";

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfig, *tInput, tName);

    /// @test     isInitialized.
    CPPUNIT_ASSERT(tArticle->isInitialized());

    /// @test     getOpenSensed.
    CPPUNIT_ASSERT(!tArticle->getOpenSensed());

    /// @test     getCloseSensed.
    CPPUNIT_ASSERT(tArticle->getCloseSensed());

    /// @test     getPosition.
    CPPUNIT_ASSERT(0.0 == tArticle->getPosition());

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Open/Close Valve Assembly model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpenCloseValveAssembly::testModifiers()
{
    std::cout << "\n.Open/Close Valve Assembly 06: Modifiers Test...........................";

    /// - Setter methods are tested in testUpdateNominal below.

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Open/Close Valve Assembly model update state (nominal).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpenCloseValveAssembly::testUpdateNominal()
{
    std::cout << "\n.Open/Close Valve Assembly 07: Update State Nominal Test................";

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfig, *tInput, tName);

    /// @test    Set commands to open the valve and verify.
    tArticle->setSupplyVoltageFlag(true);
    tArticle->setEnabledFlag(true);
    tArticle->setOpenCommand(true);
    tArticle->update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tArticle->mController.getPosition(), DBL_EPSILON);
    CPPUNIT_ASSERT( tArticle->getOpenSensed());
    CPPUNIT_ASSERT(!tArticle->getCloseSensed());

    /// @test    Set commands to close the valve and verify.
    tArticle->setOpenCommand(false);
    tArticle->setCloseCommand(true);
    tArticle->update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mController.getPosition(), DBL_EPSILON);
    CPPUNIT_ASSERT(!tArticle->getOpenSensed());
    CPPUNIT_ASSERT( tArticle->getCloseSensed());

    /// @test    Set a composite command to open the valve and verify.
    TsOpenCloseValveCmd composite(true, true, false);
    tArticle->setCommand(composite);
    tArticle->update(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tArticle->mController.getPosition(), DBL_EPSILON);
    CPPUNIT_ASSERT( tArticle->getOpenSensed());
    CPPUNIT_ASSERT(!tArticle->getCloseSensed());

    /// @test    Send args to the update with arguments method and verify.
    composite.mOpen  = false;
    composite.mClose = true;
    tArticle->update(true, composite, tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mController.getPosition(), DBL_EPSILON);
    CPPUNIT_ASSERT(!tArticle->getOpenSensed());
    CPPUNIT_ASSERT( tArticle->getCloseSensed());

    std::cout << "... Pass" << std::endl;
}
