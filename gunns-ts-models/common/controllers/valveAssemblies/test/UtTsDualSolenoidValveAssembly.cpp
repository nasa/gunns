/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((../TsDualSolenoidValveAssembly.o))
***************************************************************************************************/

#include <iostream>

#include "software/exceptions/TsInitializationException.hh"

#include "UtTsDualSolenoidValveAssembly.hh"
#include "aspects/electrical/UserLoad/UserLoadBase.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Dual Solenoid Valve Assembly model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsDualSolenoidValveAssembly::UtTsDualSolenoidValveAssembly()
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
/// @details  Default destructs this Dual Solenoid Valve Assembly model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsDualSolenoidValveAssembly::~UtTsDualSolenoidValveAssembly()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsDualSolenoidValveAssembly::setUp()
{
    /// - Define the nominal configuration data.
    cController.mMinCmdPosition      = 0.0;
    cController.mMaxCmdPosition      = 1.0;
    cController.mMinFluidPosition    = 0.0;
    cController.mMaxFluidPosition    = 1.0;
    cController.mLatch               = TsDualSolenoidValveController::LATCHING;
    cSensorOpen.mOffValue            = false;
    cSensorOpen.mTarget              = 1.0;
    cSensorOpen.mTolerance           = 0.1;
    cSensorClosed.mOffValue          = false;
    cSensorClosed.mTarget            = 0.0;
    cSensorClosed.mTolerance         = 0.1;
    tConfig                          = new TsDualSolenoidValveAssemblyConfigData(cController,
                                                                                 cSensorOpen,
                                                                                 cSensorClosed);

    /// - Define the nominal input data.
    iController.mCmdPosition         = 0.0;
    iController.mManualPositionFlag  = false;
    iController.mManualPositionValue = 0.0;
    iController.mOpenSolenoidCmd     = false;
    iController.mCloseSolenoidCmd    = false;
    iSensorOpen.mPowerFlag           = true;
    iSensorOpen.mTruthInput          = false;
    iSensorOpen.mTruthInputAnalog    = 0.0;
    iSensorClosed.mPowerFlag         = true;
    iSensorClosed.mTruthInput        = false;
    iSensorClosed.mTruthInputAnalog  = 0.0;
    tInput                           = new TsDualSolenoidValveAssemblyInputData(iController,
                                                                                iSensorOpen,
                                                                                iSensorClosed);

    /// - Default construct the nominal test article.
    tName                = "Test";
    tArticle             = new FriendlyTsDualSolenoidValveAssembly();

    /// - Define the nominal time step and comparison tolerance.
    tTimeStep            = 0.1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsDualSolenoidValveAssembly::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete tArticle;
    delete tInput;
    delete tConfig;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Dual Solenoid Valve Assembly model construction of configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsDualSolenoidValveAssembly::testConfigAndInput()
{
    std::cout << "................................................................................";
    std::cout << "\n.Dual Solenoid Valve Assembly 01: Configuration And Input Data Test.....";

    /// @test    Configuration data default construction.
    TsDualSolenoidValveAssemblyConfigData defaultConfig;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         defaultConfig.mController.mMinCmdPosition,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         defaultConfig.mController.mMaxCmdPosition,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         defaultConfig.mController.mMinFluidPosition,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         defaultConfig.mController.mMaxFluidPosition,   0.0);
    CPPUNIT_ASSERT(TsDualSolenoidValveController::LATCHING == defaultConfig.mController.mLatch);
    CPPUNIT_ASSERT(                                          !defaultConfig.mSensorOpen.mOffValue);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         defaultConfig.mSensorOpen.mTarget,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         defaultConfig.mSensorOpen.mTolerance,          0.0);
    CPPUNIT_ASSERT(                                          !defaultConfig.mSensorClosed.mOffValue);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         defaultConfig.mSensorClosed.mTarget,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         defaultConfig.mSensorClosed.mTolerance,        0.0);

    /// @test    Input data default construction.
    TsDualSolenoidValveAssemblyInputData  defaultInput;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         defaultInput.mController.mCmdPosition,         0.0);
    CPPUNIT_ASSERT(                                          !defaultInput.mController.mManualPositionFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         defaultInput.mController.mManualPositionValue, 0.0);
    CPPUNIT_ASSERT(                                          !defaultInput.mController.mOpenSolenoidCmd);
    CPPUNIT_ASSERT(                                          !defaultInput.mController.mCloseSolenoidCmd);
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
    CPPUNIT_ASSERT(iController.mOpenSolenoidCmd            == tInput->mController.mOpenSolenoidCmd);
    CPPUNIT_ASSERT(iController.mCloseSolenoidCmd           == tInput->mController.mCloseSolenoidCmd);
    CPPUNIT_ASSERT(iSensorOpen.mPowerFlag                  == tInput->mSensorOpen.mPowerFlag);
    CPPUNIT_ASSERT(iSensorOpen.mTruthInput                 == tInput->mSensorOpen.mTruthInput);
    CPPUNIT_ASSERT(iSensorOpen.mTruthInputAnalog           == tInput->mSensorOpen.mTruthInputAnalog);
    CPPUNIT_ASSERT(iSensorClosed.mPowerFlag                == tInput->mSensorClosed.mPowerFlag);
    CPPUNIT_ASSERT(iSensorClosed.mTruthInput               == tInput->mSensorClosed.mTruthInput);
    CPPUNIT_ASSERT(iSensorClosed.mTruthInputAnalog         == tInput->mSensorClosed.mTruthInputAnalog);

    /// @test    Configuration data copy construction.
    TsDualSolenoidValveAssemblyConfigData copyConfig(*tConfig);
    CPPUNIT_ASSERT(cController.mMinCmdPosition             == copyConfig.mController.mMinCmdPosition);
    CPPUNIT_ASSERT(cController.mMaxCmdPosition             == copyConfig.mController.mMaxCmdPosition);
    CPPUNIT_ASSERT(cController.mMinFluidPosition           == copyConfig.mController.mMinFluidPosition);
    CPPUNIT_ASSERT(cController.mMaxFluidPosition           == copyConfig.mController.mMaxFluidPosition);
    CPPUNIT_ASSERT(cController.mLatch                      == copyConfig.mController.mLatch);
    CPPUNIT_ASSERT(cSensorOpen.mOffValue                   == copyConfig.mSensorOpen.mOffValue);
    CPPUNIT_ASSERT(cSensorOpen.mTarget                     == copyConfig.mSensorOpen.mTarget);
    CPPUNIT_ASSERT(cSensorOpen.mTolerance                  == copyConfig.mSensorOpen.mTolerance);
    CPPUNIT_ASSERT(cSensorClosed.mOffValue                 == copyConfig.mSensorClosed.mOffValue);
    CPPUNIT_ASSERT(cSensorClosed.mTarget                   == copyConfig.mSensorClosed.mTarget);
    CPPUNIT_ASSERT(cSensorClosed.mTolerance                == copyConfig.mSensorClosed.mTolerance);

    /// @test    Input data copy construction.
    TsDualSolenoidValveAssemblyInputData   copyInput(*tInput);
    CPPUNIT_ASSERT(iController.mCmdPosition                == copyInput.mController.mCmdPosition);
    CPPUNIT_ASSERT(iController.mManualPositionFlag         == copyInput.mController.mManualPositionFlag);
    CPPUNIT_ASSERT(iController.mManualPositionValue        == copyInput.mController.mManualPositionValue);
    CPPUNIT_ASSERT(iController.mOpenSolenoidCmd            == copyInput.mController.mOpenSolenoidCmd);
    CPPUNIT_ASSERT(iController.mCloseSolenoidCmd           == copyInput.mController.mCloseSolenoidCmd);
    CPPUNIT_ASSERT(iSensorOpen.mPowerFlag                  == copyInput.mSensorOpen.mPowerFlag);
    CPPUNIT_ASSERT(iSensorOpen.mTruthInput                 == copyInput.mSensorOpen.mTruthInput);
    CPPUNIT_ASSERT(iSensorOpen.mTruthInputAnalog           == copyInput.mSensorOpen.mTruthInputAnalog);
    CPPUNIT_ASSERT(iSensorClosed.mPowerFlag                == copyInput.mSensorClosed.mPowerFlag);
    CPPUNIT_ASSERT(iSensorClosed.mTruthInput               == copyInput.mSensorClosed.mTruthInput);
    CPPUNIT_ASSERT(iSensorClosed.mTruthInputAnalog         == copyInput.mSensorClosed.mTruthInputAnalog);

    /// @test    Configuration data assignment operation.
    TsDualSolenoidValveAssemblyConfigData assignConfig;
    assignConfig = *tConfig;
    CPPUNIT_ASSERT(cController.mMinCmdPosition             == assignConfig.mController.mMinCmdPosition);
    CPPUNIT_ASSERT(cController.mMaxCmdPosition             == assignConfig.mController.mMaxCmdPosition);
    CPPUNIT_ASSERT(cController.mMinFluidPosition           == assignConfig.mController.mMinFluidPosition);
    CPPUNIT_ASSERT(cController.mMaxFluidPosition           == assignConfig.mController.mMaxFluidPosition);
    CPPUNIT_ASSERT(cController.mLatch                      == assignConfig.mController.mLatch);
    CPPUNIT_ASSERT(cSensorOpen.mOffValue                   == assignConfig.mSensorOpen.mOffValue);
    CPPUNIT_ASSERT(cSensorOpen.mTarget                     == assignConfig.mSensorOpen.mTarget);
    CPPUNIT_ASSERT(cSensorOpen.mTolerance                  == assignConfig.mSensorOpen.mTolerance);
    CPPUNIT_ASSERT(cSensorClosed.mOffValue                 == assignConfig.mSensorClosed.mOffValue);
    CPPUNIT_ASSERT(cSensorClosed.mTarget                   == assignConfig.mSensorClosed.mTarget);
    CPPUNIT_ASSERT(cSensorClosed.mTolerance                == assignConfig.mSensorClosed.mTolerance);

    /// @test    Input data assignment operation.
    TsDualSolenoidValveAssemblyInputData assignInput;
    assignInput = *tInput;
    CPPUNIT_ASSERT(iController.mCmdPosition                == assignInput.mController.mCmdPosition);
    CPPUNIT_ASSERT(iController.mManualPositionFlag         == assignInput.mController.mManualPositionFlag);
    CPPUNIT_ASSERT(iController.mManualPositionValue        == assignInput.mController.mManualPositionValue);
    CPPUNIT_ASSERT(iController.mOpenSolenoidCmd            == assignInput.mController.mOpenSolenoidCmd);
    CPPUNIT_ASSERT(iController.mCloseSolenoidCmd           == assignInput.mController.mCloseSolenoidCmd);
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
    CPPUNIT_ASSERT(iController.mOpenSolenoidCmd            == assignInput.mController.mOpenSolenoidCmd);
    CPPUNIT_ASSERT(iController.mCloseSolenoidCmd           == assignInput.mController.mCloseSolenoidCmd);
    CPPUNIT_ASSERT(iSensorOpen.mPowerFlag                  == assignInput.mSensorOpen.mPowerFlag);
    CPPUNIT_ASSERT(iSensorOpen.mTruthInput                 == assignInput.mSensorOpen.mTruthInput);
    CPPUNIT_ASSERT(iSensorOpen.mTruthInputAnalog           == assignInput.mSensorOpen.mTruthInputAnalog);
    CPPUNIT_ASSERT(iSensorClosed.mPowerFlag                == assignInput.mSensorClosed.mPowerFlag);
    CPPUNIT_ASSERT(iSensorClosed.mTruthInput               == assignInput.mSensorClosed.mTruthInput);
    CPPUNIT_ASSERT(iSensorClosed.mTruthInputAnalog         == assignInput.mSensorClosed.mTruthInputAnalog);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Dual Solenoid Valve Assembly model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsDualSolenoidValveAssembly::testDefaultConstruction()
{
    std::cout << "\n.Dual Solenoid Valve Assembly 02: Default Construction Test.............";

    /// @test    Default values of attributes.
    CPPUNIT_ASSERT(LoadOFF                                 == tArticle->mController.getOpenSolenoidLoad());
    CPPUNIT_ASSERT(LoadOFF                                 == tArticle->mController.getCloseSolenoidLoad());
    CPPUNIT_ASSERT(                                          !tArticle->mSensorOpen.getSensedOutput());
    CPPUNIT_ASSERT(                                          !tArticle->mSensorClosed.getSensedOutput());
    CPPUNIT_ASSERT(                                          !tArticle->mInitialized);

    /// @test    New/delete for code coverage.
    TsDualSolenoidValveAssembly* article = new TsDualSolenoidValveAssembly();
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Dual Solenoid Valve Assembly model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsDualSolenoidValveAssembly::testNominalInitialization()
{
    std::cout << "\n.Dual Solenoid Valve Assembly 03: Nominal Initialization Test...........";

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfig, *tInput, tName);

    /// @test    Nominal values of attributes.
    CPPUNIT_ASSERT(LoadOFF                                 == tArticle->mController.getOpenSolenoidLoad());
    CPPUNIT_ASSERT(LoadOFF                                 == tArticle->mController.getCloseSolenoidLoad());
    CPPUNIT_ASSERT(                                          !tArticle->mSensorOpen.getSensedOutput());
    CPPUNIT_ASSERT(                                           tArticle->mSensorClosed.getSensedOutput());
    CPPUNIT_ASSERT(                                           tArticle->mInitialized);

    /// @test    Reinitialization.
    tArticle->initialize(*tConfig, *tInput, tName);
    CPPUNIT_ASSERT(LoadOFF                                 == tArticle->mController.getOpenSolenoidLoad());
    CPPUNIT_ASSERT(LoadOFF                                 == tArticle->mController.getCloseSolenoidLoad());
    CPPUNIT_ASSERT(                                          !tArticle->mSensorOpen.getSensedOutput());
    CPPUNIT_ASSERT(                                           tArticle->mSensorClosed.getSensedOutput());
    CPPUNIT_ASSERT(                                           tArticle->mInitialized);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Dual Solenoid Valve Controller model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsDualSolenoidValveAssembly::testInitializationFailure()
{
    std::cout << "\n.Dual Solenoid Valve Assembly 04: Initialization Failure Test...........";

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
/// @details  Tests for Dual Solenoid Valve Assembly model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsDualSolenoidValveAssembly::testAccessors()
{
    std::cout << "\n.Dual Solenoid Valve Assembly 05: Accessors Test........................";

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
/// @details  Tests for Dual Solenoid Valve Assembly model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsDualSolenoidValveAssembly::testModifiers()
{
    std::cout << "\n.Dual Solenoid Valve Assembly 06: Modifiers Test........................";

    /// - Setter methods are tested in testUpdateNominal below.

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Dual Solenoid Valve Assembly model update state (nominal).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsDualSolenoidValveAssembly::testUpdateNominal()
{
    std::cout << "\n.Dual Solenoid Valve Assembly 07: Update State Nominal Test.............";

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfig, *tInput, tName);

    /// @test    Set commands to open the valve and verify.
    tArticle->setOpenSolenoidCmd(true);
    tArticle->setCloseSolenoidCmd(false);
    tArticle->update(tTimeStep);
    CPPUNIT_ASSERT( tArticle->getOpenSensed());
    CPPUNIT_ASSERT(!tArticle->getCloseSensed());

    /// @test    Set commands to close the valve and verify.
    tArticle->setOpenSolenoidCmd(false);
    tArticle->setCloseSolenoidCmd(true);
    tArticle->update(tTimeStep);
    CPPUNIT_ASSERT(!tArticle->getOpenSensed());
    CPPUNIT_ASSERT( tArticle->getCloseSensed());

    /// @test    Send args to the update with arguments method and verify.
    tArticle->update(true, false, tTimeStep);
    CPPUNIT_ASSERT( tArticle->getOpenSensed());
    CPPUNIT_ASSERT(!tArticle->getCloseSensed());

    tArticle->update(false, true, tTimeStep);
    CPPUNIT_ASSERT(!tArticle->getOpenSensed());
    CPPUNIT_ASSERT( tArticle->getCloseSensed());

    std::cout << "... Pass" << std::endl;
}
