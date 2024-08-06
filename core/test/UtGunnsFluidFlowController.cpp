/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((core/GunnsFluidFlowController.o))
***************************************************************************************************/

#include "core/GunnsFluidUtils.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidFlowController.hh"

/// @details  Test identification number.
int UtGunnsFluidFlowController::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Flow Controller link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidFlowController::UtGunnsFluidFlowController()
    :
    CppUnit::TestFixture(),
    tTypes(),
    tFractions(),
    tFluidProperties(0),
    tFluidConfig(0),
    tFluidInput(0),
    tLinks(),
    tName(""),
    tNodes(),
    tNodeList(),
    tPort0(0),
    tPort1(0),
    tMaxConductivity(0.0),
    tExpansionScaleFactor(0.0),
    tFilterProportionalGain(0.0),
    tEnableReverseControl(0.0),
    tConfigData(0),
    tMalfBlockageFlag(false),
    tMalfBlockageValue(0.0),
    tMalfFlowRateFlag(false),
    tMalfFlowRateValue(0.0),
    tFlowRateCommand(0.0),
    tInputData(0),
    tArticle(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Flow Controller link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidFlowController::~UtGunnsFluidFlowController()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////

void UtGunnsFluidFlowController::setUp()
{
    /// - Define the nominal port fluids.
    tFluidProperties        = new DefinedFluidProperties();
    tTypes[0]               = FluidProperties::GUNNS_WATER;
    tFluidConfig            = new PolyFluidConfigData(tFluidProperties, tTypes, N_FLUIDS);

    tFractions[0]           = 1.0;
    tFluidInput             = new PolyFluidInputData(283.0,
                                                     111.0,
                                                       0.0,
                                                       0.0,
                                                     tFractions);

    /// - Initialize the nodes.
    tNodes[0].initialize("UtNode1", tFluidConfig);
    tNodes[1].initialize("UtNode2", tFluidConfig);
    tNodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput);
    tNodes[1].getContent()->initialize(*tFluidConfig, *tFluidInput);

    /// - Initialize the nodes list.
    tNodeList.mNodes        = tNodes;
    tNodeList.mNumNodes     = N_NODES;

    /// - Define the nominal configuration data.
    tName                   = "nominal";
    tMaxConductivity        = 20.0;
    tExpansionScaleFactor   = 1.0;
    tFilterProportionalGain = 0.5;
    tEnableReverseControl   = true;
    tConfigData             = new GunnsFluidFlowControllerConfigData(tName,
                                                                     &tNodeList,
                                                                     tMaxConductivity,
                                                                     tExpansionScaleFactor,
                                                                     tFilterProportionalGain,
                                                                     tEnableReverseControl);

    /// - Define the nominal input data.
    tMalfBlockageFlag       = false;
    tMalfBlockageValue      = 0.5;
    tMalfFlowRateFlag       = false;
    tMalfFlowRateValue      = 0.3;
    tFlowRateCommand        = 2.0;
    tInputData              = new GunnsFluidFlowControllerInputData(tMalfBlockageFlag,
                                                                    tMalfBlockageValue,
                                                                    tMalfFlowRateFlag,
                                                                    tMalfFlowRateValue,
                                                                    tFlowRateCommand);

    /// - Define the nominal port mapping.
    tPort0                  = 0;
    tPort1                  = 1;

    /// - Default construct the nominal test article.
    tArticle                = new FriendlyGunnsFluidFlowController;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFlowController::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete tFluidInput;
    delete tFluidConfig;
    delete tFluidProperties;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Flow Controller link model construction of configuration.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFlowController::testConfig()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(tName                                == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                               == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductivity                     == tConfigData->mMaxConductivity);
    CPPUNIT_ASSERT(tExpansionScaleFactor                == tConfigData->mExpansionScaleFactor);
    CPPUNIT_ASSERT(tFilterProportionalGain              == tConfigData->mFilterProportionalGain);
    CPPUNIT_ASSERT(tEnableReverseControl                == tConfigData->mEnableReverseControl);

    /// @test    Configuration data default construction.
    GunnsFluidFlowControllerConfigData defaultConfig;
    CPPUNIT_ASSERT(0                                    == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0                                    == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0                                  == defaultConfig.mMaxConductivity);
    CPPUNIT_ASSERT(0.0                                  == defaultConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(0.0                                  == defaultConfig.mFilterProportionalGain);
    CPPUNIT_ASSERT(false                                == defaultConfig.mEnableReverseControl);

       /// @test    Configuration data copy construction.
    GunnsFluidFlowControllerConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tConfigData->mName                   == copyConfig.mName);
    CPPUNIT_ASSERT(tConfigData->mNodeList->mNodes       == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tConfigData->mMaxConductivity        == copyConfig.mMaxConductivity);
    CPPUNIT_ASSERT(tConfigData->mExpansionScaleFactor   == copyConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(tConfigData->mFilterProportionalGain == copyConfig.mFilterProportionalGain);
    CPPUNIT_ASSERT(tConfigData->mEnableReverseControl   == copyConfig.mEnableReverseControl);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Flow Controller link model construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFlowController::testInput()
{
    UT_RESULT;

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(tMalfBlockageFlag              == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue             == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tMalfFlowRateFlag              == tInputData->mMalfFlowRateFlag);
    CPPUNIT_ASSERT(tMalfFlowRateValue             == tInputData->mMalfFlowRateValue);
    CPPUNIT_ASSERT(tFlowRateCommand               == tInputData->mFlowRateCommand);

    /// @test    Input data default construction.
    GunnsFluidFlowControllerInputData defaultInput;
    CPPUNIT_ASSERT(false                          == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                            == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(false                          == defaultInput.mMalfFlowRateFlag);
    CPPUNIT_ASSERT(0.0                            == defaultInput.mMalfFlowRateValue);
    CPPUNIT_ASSERT(0.0                            == defaultInput.mFlowRateCommand);

    /// @test    Input data copy construction.
    GunnsFluidFlowControllerInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageValue == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(tInputData->mMalfFlowRateFlag  == copyInput.mMalfFlowRateFlag);
    CPPUNIT_ASSERT(tInputData->mMalfFlowRateValue == copyInput.mMalfFlowRateValue);
    CPPUNIT_ASSERT(tInputData->mFlowRateCommand   == copyInput.mFlowRateCommand);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Flow Controller link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFlowController::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default of attributes.
    CPPUNIT_ASSERT(""    == tArticle->mName);
    CPPUNIT_ASSERT(0     == tArticle->mNodes);
    CPPUNIT_ASSERT(0.0   == tArticle->mMaxConductivity);
    CPPUNIT_ASSERT(0.0   == tArticle->mExpansionScaleFactor);
    CPPUNIT_ASSERT(false == tArticle->mMalfFlowRateFlag);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfFlowRateValue);
    CPPUNIT_ASSERT(0.0   == tArticle->mFilterProportionalGain);
    CPPUNIT_ASSERT(false == tArticle->mEnableReverseControl);
    CPPUNIT_ASSERT(0.0   == tArticle->mFlowRateCommand);
    CPPUNIT_ASSERT(0.0   == tArticle->mLastConductivity);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(false == tArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsFluidFlowController* article = new GunnsFluidFlowController();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Flow Controller link model nominal initialization without
///           exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFlowController::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    FriendlyGunnsFluidFlowController article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1));

    /// @test    Nominal configuration data.
    CPPUNIT_ASSERT(tName                          == article.mName);
    CPPUNIT_ASSERT(&tNodes[0]                     == article.mNodes[0]);
    CPPUNIT_ASSERT(&tNodes[1]                     == article.mNodes[1]);
    CPPUNIT_ASSERT(tMaxConductivity               == article.mMaxConductivity);
    CPPUNIT_ASSERT(tExpansionScaleFactor          == article.mExpansionScaleFactor);
    CPPUNIT_ASSERT(tFilterProportionalGain        == article.mFilterProportionalGain);
    CPPUNIT_ASSERT(tEnableReverseControl          == article.mEnableReverseControl);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT(tMalfFlowRateFlag              == article.mMalfFlowRateFlag);
    CPPUNIT_ASSERT(tMalfFlowRateValue             == article.mMalfFlowRateValue);
    CPPUNIT_ASSERT(tFlowRateCommand               == article.mFlowRateCommand);

    /// @test    Nominal state data.
    CPPUNIT_ASSERT(article.mEffectiveConductivity == article.mLastConductivity);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(article.mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Flow Controller link model restart.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFlowController::testRestart()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Base and this class attributes are reset.
    tArticle->mEffectiveConductivity = 10.0;
    tArticle->restart();
    CPPUNIT_ASSERT(0.0 == tArticle->mEffectiveConductivity);
    CPPUNIT_ASSERT(0.0 == tArticle->mLastConductivity);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Flow Controller link model step.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFlowController::testUpdateState()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test   Step with nominal forward flow.
    tNodes[1].resetContentState();

    tArticle->mPotentialVector[0] = tNodes[0].getContent()->getPressure();
    tArticle->mPotentialVector[1] = tNodes[1].getContent()->getPressure();

    double expectedMdot = tFlowRateCommand;
    double expectedG = GunnsFluidUtils::predictConductivity(expectedMdot, 0.0,
                                                            tNodes[0].getContent(),
                                                            tNodes[1].getContent());
    double lastG = tArticle->mLastConductivity;
    expectedG = lastG + tFilterProportionalGain * (expectedG - lastG);

    tArticle->step(0.1);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mEffectiveConductivity, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mLastConductivity,      DBL_EPSILON);

    /// @test   Step with the malfunction rate, forward flow, and gain limit limited to 1.
    tArticle->setMalfFlowRate(true, 1.0);
    tArticle->mFilterProportionalGain = 2.0;

    expectedMdot = 1.0;
    expectedG = GunnsFluidUtils::predictConductivity(expectedMdot, 0.0,
                                                     tNodes[0].getContent(),
                                                     tNodes[1].getContent());

    tArticle->step(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mEffectiveConductivity, FLT_EPSILON);

    /// @test   Step with gain limited to zero.
    tArticle->setMalfFlowRate(true, 10.0);
    tArticle->mFilterProportionalGain = -1.0;

    tArticle->step(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mEffectiveConductivity, FLT_EPSILON);

    /// @test   Step with conductance limited to maximum.
    tArticle->setMalfFlowRate(true, 1.0e10);
    tArticle->mFilterProportionalGain = 1.0;
    expectedG = tMaxConductivity;

    tArticle->step(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mEffectiveConductivity, DBL_EPSILON);

    /// @test   Step with filter preventing underflow.
    tArticle->setMalfFlowRate(true, 1.0e-20);
    expectedG = 0.0;

    tArticle->step(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mEffectiveConductivity, DBL_EPSILON);

    /// @test   Controlled reverse flow.
    tArticle->setPort(0, 1);
    tArticle->setPort(1, 0);
    tArticle->mPotentialVector[0] = tNodes[1].getContent()->getPressure();
    tArticle->mPotentialVector[1] = tNodes[0].getContent()->getPressure();
    tArticle->setMalfFlowRate();

    expectedMdot = tFlowRateCommand;
    expectedG = GunnsFluidUtils::predictConductivity(expectedMdot, 0.0,
                                                     tNodes[1].getContent(),
                                                     tNodes[0].getContent());

    tArticle->step(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mEffectiveConductivity, DBL_EPSILON);

    /// @test   Blocked reverse flow.
    tArticle->mEnableReverseControl = false;
    expectedG = 0.0;

    tArticle->step(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mEffectiveConductivity, DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Flow Controller link model access methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFlowController::testAccessors()
{
    UT_RESULT;

    /// - Currently there is nothing to test.

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Flow Controller link model modifier methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFlowController::testModifiers()
{
    UT_RESULT;

    /// @test    Flow rate command setter method with nominal value.
    tArticle->setFlowRateCommand(10.0);
    CPPUNIT_ASSERT(10.0 == tArticle->mFlowRateCommand);

    /// @test    Flow rate command setter method with default value.
    tArticle->setFlowRateCommand();
    CPPUNIT_ASSERT(0.0  == tArticle->mFlowRateCommand);

    /// @test    Flow rate malfunction activation.
    tArticle->setMalfFlowRate(true, 5.0);
    CPPUNIT_ASSERT(       tArticle->mMalfFlowRateFlag);
    CPPUNIT_ASSERT(5.0 == tArticle->mMalfFlowRateValue);

    /// @test    Flow rate malfunction default value.
    tArticle->setMalfFlowRate();
    CPPUNIT_ASSERT(      !tArticle->mMalfFlowRateFlag);
    CPPUNIT_ASSERT(0.0 == tArticle->mMalfFlowRateValue);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Flow Controller link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFlowController::testInitializationExceptions()
{
    UT_RESULT;

    /// - Default construct a test article.
    FriendlyGunnsFluidFlowController article;

    /// @test    Initialization exception invalid config data:no name.
    tConfigData->mName = "";
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mName = tName;

    /// @test    Initialization exception on invalid config data: filter gain < 0.
    tConfigData->mFilterProportionalGain = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);

    /// @test    Initialization exception on invalid config data: filter gain > 1.
    tConfigData->mFilterProportionalGain = 1.01;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mFilterProportionalGain = tFilterProportionalGain;

    CPPUNIT_ASSERT(!article.mInitFlag);

    UT_PASS_LAST;
}
