/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/fluid/conductor/GunnsFluidPipe.o))
***************************************************************************************************/

#include "core/GunnsFluidUtils.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidPipe.hh"

/// @details  Test identification number.
int UtGunnsFluidPipe::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Pipe link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidPipe::UtGunnsFluidPipe()
    :
    CppUnit::TestFixture(),
    mTypes(),
    mFractions(),
    mFluidProperties(0),
    mFluidConfig(0),
    mFluidInput0(0),
    mFluidInput1(0),
    mLinks(),
    mName(""),
    mNodes(),
    mNodeList(),
    mPort0(0),
    mPort1(0),
    mMaxConductivity(0.0),
    mExpansionScaleFactor(0.0),
    mThermalLength(0.0),
    mThermalDiameter(0.0),
    mSurfaceRoughness(0.0),
    mThermalSurfaceArea(0.0),
    mThermalROverD(0.0),
    mConfigData(0),
    mMalfBlockageFlag(false),
    mMalfBlockageValue(0.0),
    mWallTemperature(0.0),
    mInputData(0),
    mArticle(0),
    mWallHeatFlux(0.0),
    mFlowRate(0),
    mTimeStep(0.0),
    mTolerance(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Pipe link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidPipe::~UtGunnsFluidPipe()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPipe::setUp()
{
    /// - Define the nominal port fluids.
    mFluidProperties      = new DefinedFluidProperties();
    mTypes[0]             = FluidProperties::GUNNS_N2;
    mTypes[1]             = FluidProperties::GUNNS_O2;
    mFractions[0]         = 0.5;
    mFractions[1]         = 0.5;
    mFluidConfig          = new PolyFluidConfigData(mFluidProperties, mTypes, N_FLUIDS);
    mFluidInput0          = new PolyFluidInputData(283.0,
                                                   109.0,
                                                     0.0,
                                                     0.0,
                                                     mFractions);
    mFluidInput1          = new PolyFluidInputData(283.0,
                                                   108.0,
                                                     0.0,
                                                     0.0,
                                                     mFractions);

    /// - Initialize the nodes.
    mNodes[0].initialize("UtNode1", mFluidConfig);
    mNodes[1].initialize("UtNode2", mFluidConfig);
    mNodes[0].getContent()->initialize(*mFluidConfig, *mFluidInput0);
    mNodes[1].getContent()->initialize(*mFluidConfig, *mFluidInput1);

    mNodes[0].resetFlows();
    mNodes[1].resetFlows();

    /// - Initialize the nodes list.
    mNodeList.mNodes      = mNodes;
    mNodeList.mNumNodes   = N_NODES;

    /// - Define the nominal configuration data.
    mName                 = "nominal";
    mMaxConductivity      = 20.0;
    mExpansionScaleFactor = 0.5;
    mThermalLength         =  0.2;
    mThermalDiameter       =  0.5;
    mSurfaceRoughness      =  1.0e-06;
    mThermalSurfaceArea    =  mThermalLength * mThermalDiameter * UnitConversion::PI_UTIL;
    mThermalROverD         =  mSurfaceRoughness / mThermalDiameter;
    mConfigData            = new GunnsFluidPipeConfigData(mName,
                                                          &mNodeList,
                                                          mMaxConductivity,
                                                          mExpansionScaleFactor,
                                                          mThermalLength,
                                                          mThermalDiameter,
                                                          mSurfaceRoughness);

    /// - Define the nominal input data.
    mMalfBlockageFlag     = false;
    mMalfBlockageValue    = 0.3;
    mWallTemperature      = 270.0;
    mInputData            = new GunnsFluidPipeInputData(mMalfBlockageFlag,
                                                        mMalfBlockageValue,
                                                        mWallTemperature);

    /// - Define the nominal port mapping.
    mPort0                = 0;
    mPort1                = 1;

    /// - Default construct the nominal test article.
    mArticle              = new FriendlyGunnsFluidPipe;

    /// - Define the nominal initial state data
    mWallHeatFlux         = 0.0;

    /// - Define the nominal flow rate, time step and comparison tolerance.
    mFlowRate             = 0.1;
    mTimeStep             = 0.1;
    mTolerance            = 1.0e-08;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPipe::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete mArticle;
    delete mInputData;
    delete mConfigData;
    delete mFluidInput1;
    delete mFluidInput0;
    delete mFluidConfig;
    delete mFluidProperties;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of GUNNS Fluid Pipe link model configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPipe::testConfigAndInput()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(mName                                           == mConfigData->mName);
    CPPUNIT_ASSERT(mNodes                                          == mConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity,                    mConfigData->mMaxConductivity,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mExpansionScaleFactor,               mConfigData->mExpansionScaleFactor, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalLength,                      mConfigData->mThermalLength,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalDiameter,                    mConfigData->mThermalDiameter,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSurfaceRoughness,                   mConfigData->mSurfaceRoughness,     0.0);

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(mMalfBlockageFlag                               == mInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfBlockageValue,                  mInputData->mMalfBlockageValue,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mWallTemperature,                    mInputData->mWallTemperature,       0.0);

    /// @test    Configuration data default construction.
    GunnsFluidPipeConfigData defaultConfig;
    CPPUNIT_ASSERT(0                                              == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0                                              == defaultConfig.mNodeList);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mMaxConductivity,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mExpansionScaleFactor, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mThermalLength,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mThermalDiameter,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mSurfaceRoughness,     0.0);

    /// @test    Input data default construction.
    GunnsFluidPipeInputData   defaultInput;
    CPPUNIT_ASSERT(                                                 !defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultInput.mMalfBlockageValue,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultInput.mWallTemperature,       0.0);

    /// @test    Configuration data copy construction.
    GunnsFluidPipeConfigData copyConfig(*mConfigData);
    CPPUNIT_ASSERT(mConfigData->mName                             == copyConfig.mName);
    CPPUNIT_ASSERT(mConfigData->mNodeList->mNodes                 == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mMaxConductivity,      copyConfig.mMaxConductivity,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mExpansionScaleFactor, copyConfig.mExpansionScaleFactor,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mThermalLength,        copyConfig.mThermalLength,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mThermalDiameter,      copyConfig.mThermalDiameter,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mSurfaceRoughness,     copyConfig.mSurfaceRoughness,        0.0);

    /// @test    Input data copy construction.
    GunnsFluidPipeInputData   copyInput(*mInputData);
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag                  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfBlockageValue,     copyInput.mMalfBlockageValue,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mWallTemperature,       copyInput.mWallTemperature,          0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Pipe link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPipe::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default construction configuration data.
    CPPUNIT_ASSERT(""                   == mArticle->mName);
    CPPUNIT_ASSERT(0                    == mArticle->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,      mArticle->mMaxConductivity,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,      mArticle->mExpansionScaleFactor,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,      mArticle->mThermalDiameter,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,      mArticle->mThermalSurfaceArea,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,      mArticle->mThermalROverD,         0.0);

    /// @test    Default construction input data.
    CPPUNIT_ASSERT(                       !mArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,      mArticle->mMalfBlockageValue,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,      mArticle->mWallTemperature,       0.0);

    /// @test    Default construction state data.
    CPPUNIT_ASSERT(0                    == mArticle->mInternalFluid);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,      mArticle->mWallHeatFlux,          0.0);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(!mArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsFluidPipe* article = new GunnsFluidPipe();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Pipe link model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPipe::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    FriendlyGunnsFluidPipe article;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    Nominal configuration data.
    CPPUNIT_ASSERT(mName                              == article.mName);
    CPPUNIT_ASSERT(&mNodes[0]                         == article.mNodes[0]);
    CPPUNIT_ASSERT(&mNodes[1]                         == article.mNodes[1]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity,       article.mMaxConductivity,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mExpansionScaleFactor,  article.mExpansionScaleFactor,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalDiameter,       article.mThermalDiameter,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalSurfaceArea,    article.mThermalSurfaceArea,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalROverD,         article.mThermalROverD,         0.0);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT(mMalfBlockageFlag                  == article.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfBlockageValue,     article.mMalfBlockageValue,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mWallTemperature,       article.mWallTemperature,       0.0);

    /// @test    Nominal state data.
    CPPUNIT_ASSERT(0                                  != article.mInternalFluid);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mWallHeatFlux,          article.mWallHeatFlux,          0.0);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(                                      article.mInitFlag);

    /// @test    Nominal zero case.
    mConfigData->mThermalDiameter = 0.0;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    CPPUNIT_ASSERT(0.0                   == article.mThermalROverD);

    UT_PASS;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Pipe link model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPipe::testAccessors()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    Test getter method for pipe heat flux.
    mArticle->mWallHeatFlux = 5.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, mArticle->getPipeHeatFlux(), 0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Pipe link model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPipe::testModifiers()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    The thermal surface area setter with good value.
    mArticle->setThermalSurfaceArea(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1, mArticle->mThermalSurfaceArea, 0.0);

    /// @test    The thermal surface area setter with out of range value.
    mArticle->setThermalSurfaceArea(-0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mThermalSurfaceArea, 0.0);

    /// @test    The wall temperature setter with good value.
    mArticle->setWallTemperature(280.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(280.0, mArticle->mWallTemperature, 0.0);

    /// @test    The wall temperature setter with out of range value.
    mArticle->setWallTemperature(-0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mWallTemperature, 0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Pipe link model update state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPipe::testUpdateState()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test     Update state.
    mArticle->updateState(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mMaxConductivity,
                                 mArticle->mEffectiveConductivity,
                                 mTolerance);
    /// - Check restartModel capabilities
    mArticle->restartModel();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mArticle->mEffectiveConductivity,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mArticle->mSystemConductance,mTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Pipe link model update fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPipe::testUpdateFluid()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test     Update fluid with positive flowrate.
    mArticle->updateFluid(mTimeStep, mFlowRate);

    CPPUNIT_ASSERT(mArticle->mWallTemperature <
                              mArticle->mInternalFluid->getTemperature());
    CPPUNIT_ASSERT(0.0 < mArticle->mWallHeatFlux);

    /// @test     Update fluid with negative flowrate.
    mArticle->mFlowRate = - mArticle->mFlowRate;
    mArticle->updateFluid(mTimeStep, mFlowRate);

    CPPUNIT_ASSERT(mArticle->mWallTemperature <
                              mArticle->mInternalFluid->getTemperature());
    CPPUNIT_ASSERT(0.0 < mArticle->mWallHeatFlux);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Pipe link model update fluid (no flow).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPipe::testUpdateFluidNoFLow()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

//    /// @test     Update fluid with zero time step.
//    mArticle->updateFluid(0.0, mFlowRate);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mPipeHeatFlux, mTolerance);

    /// @test     Update fluid with zero flow rate.
    mArticle->updateFluid(mTimeStep, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mWallHeatFlux, mTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Pipe link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPipe::testInitializationExceptions()
{
    UT_RESULT;

    /// - Default construct a test article
    GunnsFluidPipe article;

    /// @test    Initialization exception on invalid config data: no name.
    mConfigData->mName = "";
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1), TsInitializationException);
    mConfigData->mName = mName;

    /// @test    Initialization exception on invalid config data: max conductivity < 0.
    mConfigData->mMaxConductivity = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1), TsInitializationException);
    mConfigData->mMaxConductivity = mMaxConductivity;

    /// @test    Initialization exception on invalid config data: expansion scale factor < 0.
    mConfigData->mExpansionScaleFactor = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1), TsInitializationException);
    mConfigData->mExpansionScaleFactor = mExpansionScaleFactor;

    /// @test    Initialization exception on invalid input data: blockage < 0.
    mInputData->mMalfBlockageValue = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1), TsInitializationException);
    mInputData->mMalfBlockageValue = mMalfBlockageValue;

    /// @test    Initialization exception on invalid input data: blockage > 1.
    mInputData->mMalfBlockageValue = 1.0 + FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1), TsInitializationException);
    mInputData->mMalfBlockageValue = mMalfBlockageValue;

    /// @test    Initialization exception on invalid input data: pipe temperature < 0.
    mInputData->mWallTemperature = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1), TsInitializationException);
    mInputData->mWallTemperature = mWallTemperature;

    UT_PASS_LAST;
}
