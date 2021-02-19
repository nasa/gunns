/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((core/GunnsBasicFlowController.o))
***************************************************************************************************/

#include "strings/UtResult.hh"

#include "UtGunnsBasicFlowController.hh"

/// @details  Test identification number.
int UtGunnsBasicFlowController::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Basic Flow Controller link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsBasicFlowController::UtGunnsBasicFlowController()
    :
    CppUnit::TestFixture(),
    tLinks(),
    tName(""),
    tNodes(),
    tNodeList(),
    tPort0(0),
    tPort1(0),
    tDefaultConductivity(0.0),
    tNonLinear(0.0),
    tEnableReverseControl(0.0),
    tConfigData(0),
    tMalfBlockageFlag(false),
    tMalfBlockageValue(0.0),
    tMalfFluxFlag(false),
    tMalfFluxValue(0.0),
    tFluxCommand(0.0),
    tInputData(0),
    tArticle(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Basic Flow Controller link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsBasicFlowController::~UtGunnsBasicFlowController()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////

void UtGunnsBasicFlowController::setUp()
{
    /// - Initialize the nodes.
    tNodes[0].initialize("UtNode1", 100.0);
    tNodes[1].initialize("UtNode2", 0.0);

    /// - Initialize the nodes list.
    tNodeList.mNodes      = tNodes;
    tNodeList.mNumNodes   = N_NODES;

    /// - Define the nominal configuration data.
    tName                 = "nominal";
    tDefaultConductivity  = 20.0;
    tNonLinear            = true;
    tEnableReverseControl = true;
    tConfigData           = new GunnsBasicFlowControllerConfigData(tName,
                                                                   &tNodeList,
                                                                   tDefaultConductivity,
                                                                   tNonLinear,
                                                                   tEnableReverseControl);

    /// - Define the nominal input data.
    tMalfBlockageFlag     = true;
    tMalfBlockageValue    = 0.5;
    tMalfFluxFlag         = true;
    tMalfFluxValue        = 0.3;
    tFluxCommand          = 2.0;
    tInputData            = new GunnsBasicFlowControllerInputData(tMalfBlockageFlag,
                                                                  tMalfBlockageValue,
                                                                  tMalfFluxFlag,
                                                                  tMalfFluxValue,
                                                                  tFluxCommand);

    /// - Define the nominal port mapping.
    tPort0                = 0;
    tPort1                = 1;

    /// - Default construct the nominal test article.
    tArticle              = new FriendlyGunnsBasicFlowController;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicFlowController::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete tArticle;
    delete tInputData;
    delete tConfigData;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Basic Flow Controller link model construction of configuration.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicFlowController::testConfig()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(tName                              == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                             == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tDefaultConductivity               == tConfigData->mDefaultConductivity);
    CPPUNIT_ASSERT(tNonLinear                         == tConfigData->mNonLinear);
    CPPUNIT_ASSERT(tEnableReverseControl              == tConfigData->mEnableReverseControl);

    /// @test    Configuration data default construction.
    GunnsBasicFlowControllerConfigData defaultConfig;
    CPPUNIT_ASSERT(0                                  == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0                                  == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0                                == defaultConfig.mDefaultConductivity);
    CPPUNIT_ASSERT(0.0                                == defaultConfig.mNonLinear);
    CPPUNIT_ASSERT(false                              == defaultConfig.mEnableReverseControl);

       /// @test    Configuration data copy construction.
    GunnsBasicFlowControllerConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tConfigData->mName                 == copyConfig.mName);
    CPPUNIT_ASSERT(tConfigData->mNodeList->mNodes     == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tConfigData->mDefaultConductivity  == copyConfig.mDefaultConductivity);
    CPPUNIT_ASSERT(tConfigData->mNonLinear            == copyConfig.mNonLinear);
    CPPUNIT_ASSERT(tConfigData->mEnableReverseControl == copyConfig.mEnableReverseControl);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Basic Flow Controller link model construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicFlowController::testInput()
{
    UT_RESULT;

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(tMalfBlockageFlag              == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue             == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tMalfFluxFlag                  == tInputData->mMalfFluxFlag);
    CPPUNIT_ASSERT(tMalfFluxValue                 == tInputData->mMalfFluxValue);
    CPPUNIT_ASSERT(tFluxCommand                   == tInputData->mFluxCommand);

    /// @test    Input data default construction.
    GunnsBasicFlowControllerInputData defaultInput;
    CPPUNIT_ASSERT(false                          == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                            == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(false                          == defaultInput.mMalfFluxFlag);
    CPPUNIT_ASSERT(0.0                            == defaultInput.mMalfFluxValue);
    CPPUNIT_ASSERT(0.0                            == defaultInput.mFluxCommand);

    /// @test    Input data copy construction.
    GunnsBasicFlowControllerInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageValue == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(tInputData->mMalfFluxFlag      == copyInput.mMalfFluxFlag);
    CPPUNIT_ASSERT(tInputData->mMalfFluxValue     == copyInput.mMalfFluxValue);
    CPPUNIT_ASSERT(tInputData->mFluxCommand       == copyInput.mFluxCommand);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Basic Flow Controller link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicFlowController::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default of attributes.
    CPPUNIT_ASSERT(""    == tArticle->mName);
    CPPUNIT_ASSERT(0     == tArticle->mNodes);
    CPPUNIT_ASSERT(0.0   == tArticle->mDefaultConductivity);
    CPPUNIT_ASSERT(false == tArticle->mMalfFluxFlag);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfFluxValue);
    CPPUNIT_ASSERT(false == tArticle->mNonLinear);
    CPPUNIT_ASSERT(false == tArticle->mEnableReverseControl);
    CPPUNIT_ASSERT(0.0   == tArticle->mFluxCommand);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(false == tArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsBasicFlowController* article = new GunnsBasicFlowController();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Basic Flow Controller link model nominal initialization without
///           exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicFlowController::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    FriendlyGunnsBasicFlowController article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1));

    /// @test    Nominal configuration data.
    CPPUNIT_ASSERT(tName                 == article.mName);
    CPPUNIT_ASSERT(&tNodes[0]            == article.mNodes[0]);
    CPPUNIT_ASSERT(&tNodes[1]            == article.mNodes[1]);
    CPPUNIT_ASSERT(tDefaultConductivity  == article.mDefaultConductivity);
    CPPUNIT_ASSERT(tNonLinear            == article.mNonLinear);
    CPPUNIT_ASSERT(tEnableReverseControl == article.mEnableReverseControl);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT(tMalfFluxFlag         == article.mMalfFluxFlag);
    CPPUNIT_ASSERT(tMalfFluxValue        == article.mMalfFluxValue);
    CPPUNIT_ASSERT(tFluxCommand          == article.mFluxCommand);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(article.mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Basic Flow Controller link model restart.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicFlowController::testRestart()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Base class attributes are reset.
    tArticle->mEffectiveConductivity = 10.0;
    tArticle->restart();
    CPPUNIT_ASSERT(0.0 == tArticle->mEffectiveConductivity);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Basic Flow Controller link model step.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicFlowController::testUpdateState()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test   Step with nominal forward flow.
    tArticle->setMalfFlux();
    tNodes[1].resetContentState();
    tArticle->mPotentialVector[0] = tNodes[0].getPotential();
    tArticle->mPotentialVector[1] = tNodes[1].getPotential();

    double expectedFlux = tFluxCommand;
    double expectedG = expectedFlux / (tNodes[0].getPotential() - tNodes[1].getPotential());

    tArticle->minorStep(0.1, 1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mEffectiveConductivity, DBL_EPSILON);

    /// @test   Step with the malfunction rate, controlled reverse flow.
    tArticle->setMalfFlux(true, 1.0);
    tArticle->setPort(0, 1);
    tArticle->setPort(1, 0);
    tArticle->mPotentialVector[0] = tNodes[1].getPotential();
    tArticle->mPotentialVector[1] = tNodes[0].getPotential();

    expectedFlux = 1.0;
    expectedG = expectedFlux / (tNodes[0].getPotential() - tNodes[1].getPotential());

    tArticle->step(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mEffectiveConductivity, DBL_EPSILON);

    /// @test   Step with zero delta-potential, limited to maximum conductivity.
    tArticle->mPotentialVector[1] = tNodes[1].getPotential();
    expectedG = tDefaultConductivity;

    tArticle->step(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mEffectiveConductivity, DBL_EPSILON);

    /// @test   Blocked reverse flow.
    tArticle->mEnableReverseControl = false;
    tArticle->mPotentialVector[0] = tNodes[1].getPotential();
    tArticle->mPotentialVector[1] = tNodes[0].getPotential();
    expectedG = 0.0;

    tArticle->step(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mEffectiveConductivity, DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Basic Flow Controller link model access methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicFlowController::testAccessors()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    The isNonLinear method.
    CPPUNIT_ASSERT(tNonLinear == tArticle->isNonLinear());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Basic Flow Controller link model modifier methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicFlowController::testModifiers()
{
    UT_RESULT;

    /// @test    Flow rate command setter method with nominal value.
    tArticle->setFluxCommand(10.0);
    CPPUNIT_ASSERT(10.0 == tArticle->mFluxCommand);

    /// @test    Flow rate command setter method with default value.
    tArticle->setFluxCommand();
    CPPUNIT_ASSERT(0.0  == tArticle->mFluxCommand);

    /// @test    Flow rate malfunction activation.
    tArticle->setMalfFlux(true, 5.0);
    CPPUNIT_ASSERT(       tArticle->mMalfFluxFlag);
    CPPUNIT_ASSERT(5.0 == tArticle->mMalfFluxValue);

    /// @test    Flow rate malfunction default value.
    tArticle->setMalfFlux();
    CPPUNIT_ASSERT(      !tArticle->mMalfFluxFlag);
    CPPUNIT_ASSERT(0.0 == tArticle->mMalfFluxValue);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Basic Flow Controller link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicFlowController::testInitializationExceptions()
{
    UT_RESULT;

    /// - There is currently nothing to test.

    UT_PASS_LAST;
}
