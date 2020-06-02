/**
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((aspects/electrical/Diode/GunnsElectRealDiode.o)
   (core/GunnsBasicLink.o)
   (software/exceptions/TsInitializationException.o))
*/
#include "UtGunnsElectRealDiode.hh"
#include "software/exceptions/TsInitializationException.hh"
#include <iostream>
#include "strings/UtResult.hh"

/// @details  Test identification number.
int UtGunnsElectRealDiode::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsElectRealDiode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectRealDiode::UtGunnsElectRealDiode()
    :
    tLinks(),
    tName(""),
    tNodes(),
    tNodeList(),
    tPort0(0),
    tPort1(0),
    tConfigData(0),
    tInputData(0),
    tArticle(0),
    tForwardConductance(0.0),
    tReverseConductance(0.0),
    tVoltageDrop(0.0),
    tMalfBlockageFlag(false),
    tMalfBlockageValue(0.0),
    tReverseBias(false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsElectRealDiode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectRealDiode::~UtGunnsElectRealDiode()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectRealDiode::setUp()
{
    tName = "tArticle";

    /// - Initialize the nodes list.
    tNodeList.mNodes    = tNodes;
    tNodeList.mNumNodes = N_NODES;

    /// - Define the nominal configuration data.
    tForwardConductance = 10.0;
    tReverseConductance = 0.000000001;
    tVoltageDrop        = 0.7;
    tConfigData         = new GunnsElectRealDiodeConfigData(tName,
                                                            &tNodeList,
                                                            tForwardConductance,
                                                            tReverseConductance,
                                                            tVoltageDrop);

    /// - Define the nominal input data.
    tMalfBlockageFlag   = true;
    tMalfBlockageValue  = 0.1;
    tReverseBias        = true;
    tInputData          = new GunnsElectRealDiodeInputData(tMalfBlockageFlag,
                                                           tMalfBlockageValue,
                                                           tReverseBias);

    /// - Define the nominal port mapping.
    tPort0              = 0;
    tPort1              = 1;

    /// - Default construct the nominal test article.
    tArticle            = new FriendlyGunnsElectRealDiode;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectRealDiode::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tInputData;
    delete tConfigData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of Diode link model configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectRealDiode::testConfigAndInput()
{
    UT_RESULT_FIRST;

    /// @test    Configuration nominal construction.
    CPPUNIT_ASSERT(tName  == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tForwardConductance, tConfigData->mDefaultConductivity,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tReverseConductance, tConfigData->mReverseConductivity,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tVoltageDrop,        tConfigData->mVoltageDrop,          0.0);

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(tMalfBlockageFlag  == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tReverseBias       == tInputData->mReverseBias);
    CPPUNIT_ASSERT(0.0                == tInputData->mSourcePotential);

    /// @test    Configuration data default construction.
    GunnsElectRealDiodeConfigData defaultConfig;
    CPPUNIT_ASSERT("" == defaultConfig.mName);
    CPPUNIT_ASSERT(0  == defaultConfig.mNodeList);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                 defaultConfig.mDefaultConductivity, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                 defaultConfig.mReverseConductivity, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                 defaultConfig.mVoltageDrop,         0.0);

    /// @test    Input data default construction.
    GunnsElectRealDiodeInputData defaultInput;
    CPPUNIT_ASSERT(false == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0   == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(false == defaultInput.mReverseBias);
    CPPUNIT_ASSERT(0.0   == defaultInput.mSourcePotential);

    /// @test    Configuration data copy construction.
    GunnsElectRealDiodeConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tName  == copyConfig.mName);
    CPPUNIT_ASSERT(tNodes == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tForwardConductance, copyConfig.mDefaultConductivity,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tReverseConductance, copyConfig.mReverseConductivity,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tVoltageDrop,        copyConfig.mVoltageDrop,            0.0);

    /// @test    Input data copy construction.
    GunnsElectRealDiodeInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tMalfBlockageFlag  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(tReverseBias       == copyInput.mReverseBias);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the constructor of the GunnsElectRealDiode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectRealDiode::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default construction configuration data.
    CPPUNIT_ASSERT(""    == tArticle->mName);
    CPPUNIT_ASSERT(0     == tArticle->mNodes);

    /// @test    Default construction state data.
    CPPUNIT_ASSERT(0.0   == tArticle->mEffectiveConductivity);
    CPPUNIT_ASSERT(0.0   == tArticle->mReverseConductivity);
    CPPUNIT_ASSERT(0.0   == tArticle->mVoltageDrop);
    CPPUNIT_ASSERT(false == tArticle->mReverseBias);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(false == tArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsElectRealDiode* testArticle = new GunnsElectRealDiode();
    delete testArticle;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Diode link model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectRealDiode::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1));

    /// @test    Nominal state data.
    CPPUNIT_ASSERT(tName        == tArticle->mName);
    CPPUNIT_ASSERT(tNodes       == tArticle->mNodeList->mNodes);
    CPPUNIT_ASSERT(tReverseBias == tArticle->mReverseBias);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tForwardConductance, tArticle->mDefaultConductivity, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tReverseConductance, tArticle->mReverseConductivity, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tVoltageDrop,        tArticle->mVoltageDrop,         0.0);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(true         == tArticle->mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Diode link model nominal initialization with exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectRealDiode::testInitializationErrors()
{
    UT_RESULT;

    /// @test    Exception thrown for bad reverse conductivity.
    tConfigData->mReverseConductivity = -0.01;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle->mInitFlag);
    tConfigData->mReverseConductivity = tReverseConductance;

    /// @test    Exception thrown for bad voltage drop.
    tConfigData->mVoltageDrop = -0.01;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle->mInitFlag);
    tConfigData->mVoltageDrop = tVoltageDrop;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Diode link model restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectRealDiode::testRestart()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1));

    /// @test    Restart method clears non-config and non-checkpointed data.
    tArticle->mEffectiveConductivity = 1.0;
    tArticle->mSystemConductance     = 2.0;
    tArticle->restart();
    CPPUNIT_ASSERT(0.0 == tArticle->mEffectiveConductivity);
    CPPUNIT_ASSERT(0.0 == tArticle->mSystemConductance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Diode link model step and updateState methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectRealDiode::testStep()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1));

    {
        /// - @test    Outputs in forward bias.
        tArticle->mReverseBias        = false;
        tArticle->mPotentialVector[0] = 0.0;
        tArticle->mPotentialVector[1] = 1.0;
        const double expectedG    = tForwardConductance * (1.0 - tMalfBlockageValue);
        const double expectedV    = -tVoltageDrop * expectedG;
        const bool   expectedBias = false;
        tArticle->step(0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, tArticle->mSourceVector[1],     DBL_EPSILON);
        CPPUNIT_ASSERT(expectedBias == tArticle->mReverseBias);
    } {
        /// - @test    Outputs in reverse bias.
        tArticle->mReverseBias        = true;
        tArticle->mPotentialVector[0] = 1.0;
        tArticle->mPotentialVector[1] = 0.0;
        const double expectedG    = tReverseConductance * (1.0 - tMalfBlockageValue);
        const double expectedV    = 0.0;
        const bool   expectedBias = true;
        tArticle->step(0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, tArticle->mSourceVector[1],     DBL_EPSILON);
        CPPUNIT_ASSERT(expectedBias == tArticle->mReverseBias);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Diode link model minorStep and updateState methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectRealDiode::testMinorStep()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1));

    {
        /// - @test    Outputs in forward bias.
        tArticle->mReverseBias        = false;
        tArticle->mPotentialVector[0] = 0.0;
        tArticle->mPotentialVector[1] = 1.0;
        const double expectedG    = tForwardConductance * (1.0 - tMalfBlockageValue);
        const double expectedV    = -tVoltageDrop * expectedG;
        const bool   expectedBias = false;
        tArticle->minorStep(0.0, 2);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, tArticle->mSourceVector[1],     DBL_EPSILON);
        CPPUNIT_ASSERT(expectedBias == tArticle->mReverseBias);
    } {
        /// - @test    Outputs in reverse bias.
        tArticle->mReverseBias        = true;
        tArticle->mPotentialVector[0] = 1.0;
        tArticle->mPotentialVector[1] = 0.0;
        const double expectedG    = tReverseConductance * (1.0 - tMalfBlockageValue);
        const double expectedV    = 0.0;
        const bool   expectedBias = true;
        tArticle->minorStep(0.0, 3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, tArticle->mSourceVector[1],     DBL_EPSILON);
        CPPUNIT_ASSERT(expectedBias == tArticle->mReverseBias);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the Diode getter and setter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectRealDiode::testAccessors()
{
    UT_RESULT;

    /// @test    Link is non-linear.
    CPPUNIT_ASSERT(true  == tArticle->isNonLinear());

    /// @test    Can set reverse conductivity.
    tArticle->setReverseConductivity(5.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, tArticle->mReverseConductivity, 0.0);

    /// @test    Can set & get voltage drop.
    tArticle->setVoltageDrop(9.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.0, tArticle->mVoltageDrop,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.0, tArticle->getVoltageDrop(),     0.0);

    /// @test    Can get reversed bias flag.
    CPPUNIT_ASSERT(false == tArticle->isReversedBias());
    tArticle->mReverseBias = true;
    CPPUNIT_ASSERT(true  == tArticle->isReversedBias());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the confirmSolutionAcceptable method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectRealDiode::testConfirmSolutionAcceptable()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1));

    {
        /// @test    Remains in forward bias.
        tArticle->mPotentialVector[0] = 1.71;
        tArticle->mPotentialVector[1] = 1.0;
        const GunnsBasicLink::SolutionResult expectedResult = GunnsBasicLink::CONFIRM;
        const bool                           expectedBias   = false;
        const bool result = tArticle->confirmSolutionAcceptable(1, 1);
        CPPUNIT_ASSERT(expectedResult == expectedResult);
        CPPUNIT_ASSERT(expectedBias   == tArticle->mReverseBias);
    } {
        /// @test    Switches to reverse bias.
        tArticle->mPotentialVector[0] = 1.69;
        tArticle->mPotentialVector[1] = 1.0;
        const GunnsBasicLink::SolutionResult expectedResult = GunnsBasicLink::REJECT;
        const bool                           expectedBias   = true;
        const bool result = tArticle->confirmSolutionAcceptable(1, 1);
        CPPUNIT_ASSERT(expectedResult == expectedResult);
        CPPUNIT_ASSERT(expectedBias   == tArticle->mReverseBias);
    } {
        /// @test    Remains in reverse bias.
        tArticle->mPotentialVector[0] = 1.69;
        tArticle->mPotentialVector[1] = 1.0;
        const GunnsBasicLink::SolutionResult expectedResult = GunnsBasicLink::CONFIRM;
        const bool                           expectedBias   = true;
        const bool result = tArticle->confirmSolutionAcceptable(1, 1);
        CPPUNIT_ASSERT(expectedResult == expectedResult);
        CPPUNIT_ASSERT(expectedBias   == tArticle->mReverseBias);
    } {
        /// @test    Switches to forward bias.
        tArticle->mPotentialVector[0] = 1.71;
        tArticle->mPotentialVector[1] = 1.0;
        const GunnsBasicLink::SolutionResult expectedResult = GunnsBasicLink::REJECT;
        const bool                           expectedBias   = false;
        const bool result = tArticle->confirmSolutionAcceptable(1, 1);
        CPPUNIT_ASSERT(expectedResult == expectedResult);
        CPPUNIT_ASSERT(expectedBias   == tArticle->mReverseBias);
    }

    UT_PASS_LAST;
}
