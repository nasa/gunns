/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/fluid/conductor/GunnsFluidSimpleQd.o))
***************************************************************************************************/

#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidSimpleQd.hh"

/// @details  Acts as a redundant test on UnitConversion class.
const double UtGunnsFluidSimpleQd::PI = 3.14159265358979;

/// @details  Test identification number.
int UtGunnsFluidSimpleQd::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Simple Quick-Disconnect link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidSimpleQd::UtGunnsFluidSimpleQd()
    :
    CppUnit::TestFixture(),
    tTypes(),
    tFractions(),
    tFluidProperties(0),
    tFluidConfig(0),
    tFluidInput0(0),
    tFluidInput1(0),
    tLinks(),
    tName(""),
    tNodes(),
    tNodeList(),
    tPort0(0),
    tPort1(0),
    tMaxConductivity(0.0),
    tExpansionScaleFactor(0.0),
    tThermalLength(0.0),
    tThermalDiameter(0.0),
    tSurfaceRoughness(0.0),
    tConfigData(0),
    tBlockageFlag(false),
    tBlockage(0.0),
    tState(GunnsFluidSimpleQd::DISCONNECTED),
    tWallTemperature(0.0),
    tInputData(0),
    tArticle(0)
//    tTimeStep(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Simple Quick-Disconnect link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidSimpleQd::~UtGunnsFluidSimpleQd()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleQd::setUp()
{
    /// - Define nominal port fluids.
    tFluidProperties      = new DefinedFluidProperties();
    tTypes[0]             = FluidProperties::GUNNS_N2;
    tTypes[1]             = FluidProperties::GUNNS_O2;
    tFractions[0]         = 0.5;
    tFractions[1]         = 0.5;
    tFluidConfig          = new PolyFluidConfigData(tFluidProperties, tTypes, N_FLUIDS);
    tFluidInput0          = new PolyFluidInputData(283.0,
                                                   109.0,
                                                     0.0,
                                                     0.0,
                                                     tFractions);
    tFluidInput1          = new PolyFluidInputData(320.0,
                                                   111.0,
                                                     0.0,
                                                     0.0,
                                                     tFractions);

    /// - Initialize the nodes.
    tNodes[0].initialize("UtNode1", tFluidConfig);
    tNodes[1].initialize("UtNode2", tFluidConfig);
    tNodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput0);
    tNodes[1].getContent()->initialize(*tFluidConfig, *tFluidInput1);

    tNodes[0].resetFlows();
    tNodes[1].resetFlows();

    /// - Initialize the nodes list.
    tNodeList.mNodes      = tNodes;
    tNodeList.mNumNodes   = N_NODES;

    /// - Define the nominal configuration data.
    tName                 = "nominal";
    tMaxConductivity      = 0.0063;
    tExpansionScaleFactor = 0.4;
    tThermalLength        = 1.0;
    tThermalDiameter      = 0.01;
    tSurfaceRoughness     = 2.1336E-6;
    tConfigData           = new GunnsFluidSimpleQdConfigData(tName,
                                                             &tNodeList,
                                                             tMaxConductivity,
                                                             tExpansionScaleFactor,
                                                             tThermalLength,
                                                             tThermalDiameter,
                                                             tSurfaceRoughness);

    /// - Define the nominal input data
    tBlockageFlag         = true;
    tBlockage             = 0.1;
    tState                = GunnsFluidSimpleQd::CONNECTED;
    tWallTemperature      = 300.0;
    tInputData            = new GunnsFluidSimpleQdInputData(tBlockageFlag,
                                                            tBlockage,
                                                            tState,
                                                            tWallTemperature);

    /// - Define the nominal port mapping.
    tPort0                = 0;
    tPort1                = 1;

    /// - Default construct the nominal test article.
    tArticle              = new FriendlyGunnsFluidSimpleQd;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleQd::tearDown()
{
    /// - Deletes in reverse order of news in setUp.
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete tFluidInput1;
    delete tFluidInput0;
    delete tFluidConfig;
    delete tFluidProperties;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the construction of GUNNS Simple Quick-Disconnect link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleQd::testConfig()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(tName                 == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductivity      == tConfigData->mMaxConductivity);
    CPPUNIT_ASSERT(tExpansionScaleFactor == tConfigData->mExpansionScaleFactor);
    CPPUNIT_ASSERT(tThermalLength        == tConfigData->mThermalLength);
    CPPUNIT_ASSERT(tThermalDiameter      == tConfigData->mThermalDiameter);
    CPPUNIT_ASSERT(tSurfaceRoughness     == tConfigData->mSurfaceRoughness);

    /// @test    Configuration data default construction.
    GunnsFluidSimpleQdConfigData defaultConfig;
    CPPUNIT_ASSERT(0                     == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0                     == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mMaxConductivity);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mThermalLength);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mThermalDiameter);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mSurfaceRoughness);

    /// @test    Configuration data copy construction.
    GunnsFluidSimpleQdConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tName                 == copyConfig.mName);
    CPPUNIT_ASSERT(tNodes                == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductivity      == copyConfig.mMaxConductivity);
    CPPUNIT_ASSERT(tExpansionScaleFactor == copyConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(tThermalLength        == copyConfig.mThermalLength);
    CPPUNIT_ASSERT(tThermalDiameter      == copyConfig.mThermalDiameter);
    CPPUNIT_ASSERT(tSurfaceRoughness     == copyConfig.mSurfaceRoughness);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of GUNNS Simple Quick-Disconnect link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleQd::testInput()
{
    UT_RESULT;

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(tBlockageFlag                    == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tBlockage                        == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tState                           == tInputData->mState);
    CPPUNIT_ASSERT(tWallTemperature                 == tInputData->mWallTemperature);

    /// @test    Input data default construction.
    GunnsFluidSimpleQdInputData defaultInput;
    CPPUNIT_ASSERT(false                            == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                              == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(GunnsFluidSimpleQd::DISCONNECTED == defaultInput.mState);
    CPPUNIT_ASSERT(0.0                              == defaultInput.mWallTemperature);

    /// @test    Input data copy construction.
    GunnsFluidSimpleQdInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tBlockageFlag                    == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tBlockage                        == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(tState                           == copyInput.mState);
    CPPUNIT_ASSERT(tWallTemperature                 == copyInput.mWallTemperature);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Simple Quick-Disconnect link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleQd::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default construction of class & base class attributes.
    CPPUNIT_ASSERT(0.0                              == tArticle->mThermalDiameter);
    CPPUNIT_ASSERT(0.0                              == tArticle->mThermalSurfaceArea);
    CPPUNIT_ASSERT(0.0                              == tArticle->mThermalROverD);
    CPPUNIT_ASSERT(GunnsFluidSimpleQd::DISCONNECTED == tArticle->mState);
    CPPUNIT_ASSERT(0.0                              == tArticle->mWallTemperature);
    CPPUNIT_ASSERT(0.0                              == tArticle->mWallHeatFlux);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsFluidSimpleQd* article = new GunnsFluidSimpleQd();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Simple Quick-Disconnect link model nominal initialization without
///           exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleQd::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    FriendlyGunnsFluidSimpleQd article;
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Base class initialization.
    CPPUNIT_ASSERT(tName                 == article.mName);
    CPPUNIT_ASSERT(&tNodes[0]            == article.mNodes[0]);
    CPPUNIT_ASSERT(&tNodes[1]            == article.mNodes[1]);
    CPPUNIT_ASSERT(tMaxConductivity      == article.mMaxConductivity);
    CPPUNIT_ASSERT(tExpansionScaleFactor == article.mExpansionScaleFactor);
    /// @test    Terms initialized from configuration data.
    const double expectedArea   = tThermalLength * PI * tThermalDiameter;
    const double expectedROverD = tSurfaceRoughness / tThermalDiameter;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedArea,   article.mThermalSurfaceArea, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedROverD, article.mThermalROverD,      DBL_EPSILON);
    CPPUNIT_ASSERT(tThermalDiameter      == article.mThermalDiameter);

    /// @test    Terms initialized from input data.
    CPPUNIT_ASSERT(tState                == article.mState);
    CPPUNIT_ASSERT(tWallTemperature      == article.mWallTemperature);

    /// @test    Initialized state data.
    CPPUNIT_ASSERT(0.0                   == article.mWallHeatFlux);

    /// @test    Internal fluid initialization.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getTemperature(),
                                 article.getInternalFluid()->getTemperature(), DBL_EPSILON);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(article.mInitFlag);

    /// @test    Verify restartModel functionality
    article.restartModel();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,article.mEffectiveConductivity,DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,article.mSystemConductance,DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Simple Quick-Disconnect link model nominal initialization without
///           exceptions and with the QD configured to not use thermal convection.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleQd::testNoConvectionInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    FriendlyGunnsFluidSimpleQd article;
    tConfigData->mThermalDiameter = 0.0;
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Base class initialization.
    CPPUNIT_ASSERT(tName                 == article.mName);
    CPPUNIT_ASSERT(&tNodes[0]            == article.mNodes[0]);
    CPPUNIT_ASSERT(&tNodes[1]            == article.mNodes[1]);
    CPPUNIT_ASSERT(tMaxConductivity      == article.mMaxConductivity);
    CPPUNIT_ASSERT(tExpansionScaleFactor == article.mExpansionScaleFactor);

    /// @test    Terms initialized from configuration data.
    CPPUNIT_ASSERT(0.0                   == article.mThermalSurfaceArea);
    CPPUNIT_ASSERT(0.0                   == article.mThermalROverD);
    CPPUNIT_ASSERT(0.0                   == article.mThermalDiameter);

    /// @test    Terms initialized from input data.
    CPPUNIT_ASSERT(tState                == article.mState);
    CPPUNIT_ASSERT(tWallTemperature      == article.mWallTemperature);

    /// @test    Initialized state data.
    CPPUNIT_ASSERT(0.0                   == article.mWallHeatFlux);

    /// @test    Internal fluid not instantiated.
    CPPUNIT_ASSERT(0 == article.mInternalFluid);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(article.mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Simple Quick-Disconnect link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleQd::testInitializationExceptions()
{
    UT_RESULT;

    /// - Default construct a test article.
    FriendlyGunnsFluidSimpleQd article;

    /// @test    Initialization exception (from the base class) on no name.
    tConfigData->mName = "";
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mName = tName;

    /// @test    Initialization exception on invalid config data: max conductivity < 0.
    tConfigData->mMaxConductivity = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mMaxConductivity = tMaxConductivity;

    /// @test    Initialization exception on invalid config data: expansion scale factor < 0.
    tConfigData->mExpansionScaleFactor = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mExpansionScaleFactor = tExpansionScaleFactor;

    /// @test    Initialization exception on invalid input data: wall temperature < 0.
    tInputData->mWallTemperature = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Simple Quick-Disconnect link model getter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleQd::testAccessors()
{
    UT_RESULT;

    /// @test    The getWallHeatFlux method.
    tArticle->mWallHeatFlux = 1.0;
    CPPUNIT_ASSERT( 1.0 == tArticle->getWallHeatFlux());

    /// @test    The getState method.
    tArticle->mState = tArticle->CONNECTED;
    CPPUNIT_ASSERT(tArticle->CONNECTED == tArticle->getState());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Simple Quick-Disconnect link model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleQd::testModifiers()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    setState method.
    tArticle->setState(tArticle->DISCONNECTED);
    CPPUNIT_ASSERT(tArticle->DISCONNECTED == tArticle->mState);
    tArticle->setState(tArticle->CONNECTED);
    CPPUNIT_ASSERT(tArticle->CONNECTED == tArticle->mState);

    /// @test    The thermal surface area setter with good value.
    tArticle->setThermalSurfaceArea(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1, tArticle->mThermalSurfaceArea, 0.0);

    /// @test    The thermal surface area setter with out of range value.
    tArticle->setThermalSurfaceArea(-0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mThermalSurfaceArea, 0.0);

    /// @test    The wall temperature setter with good value.
    tArticle->setWallTemperature(280.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(280.0, tArticle->mWallTemperature, 0.0);

    /// @test    The wall temperature setter with out of range value.
    tArticle->setWallTemperature(-0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mWallTemperature, 0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Simple Quick-Disconnect link model update state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleQd::testUpdateState()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Conductivity when connected.
    tArticle->step(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMaxConductivity * (1.0 - tBlockage),
                                 tArticle->mEffectiveConductivity, DBL_EPSILON);

    /// @test    Conductivity when disconnected.
    tArticle->mState = GunnsFluidSimpleQd::DISCONNECTED;
    tArticle->step(0.0);
    CPPUNIT_ASSERT(0.0 == tArticle->mEffectiveConductivity);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Simple Quick-Disconnect link model update fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleQd::testUpdateFluid()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test   Update fluid with flowrate too small.
    tArticle->updateFluid(0.0, 0.5 * DBL_EPSILON);
    CPPUNIT_ASSERT(0.0 == tArticle->mWallHeatFlux);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getTemperature(),
                                 tArticle->mInternalFluid->getTemperature(), DBL_EPSILON);

    /// @test   Update fluid with nominal flowrate.
    tArticle->updateFluid(0.0, 0.01);
    CPPUNIT_ASSERT(tArticle->mWallHeatFlux < 0.0);
    CPPUNIT_ASSERT(tArticle->mInternalFluid->getTemperature() > tNodes[0].getContent()->getTemperature());
    CPPUNIT_ASSERT(tArticle->mInternalFluid->getTemperature() < tWallTemperature);

    UT_PASS_FINAL;
}
