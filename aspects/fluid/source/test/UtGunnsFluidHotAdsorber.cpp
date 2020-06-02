/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/fluid/source/GunnsFluidHotAdsorber.o))
***************************************************************************************************/

#include "core/GunnsFluidUtils.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidHotAdsorber.hh"

/// @details  Test identification number.
int UtGunnsFluidHotAdsorber::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Hot Adsorber link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidHotAdsorber::UtGunnsFluidHotAdsorber()
    :
    CppUnit::TestFixture(),
    mFluidProperties(0),
    mTypes(),
    mFractions(),
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
    mGasType(FluidProperties::NO_FLUID),
    mGasIndex(-1),
    mEfficiency(0.0),
    mMaxAdsorbtionRate(0.0),
    mMaxAdsorbedMass(0.0),
    mDesorbtionRate(0.0),
    mThermalLength(0.0),
    mThermalDiameter(0.0),
    mSurfaceRoughness(0.0),
    mThermalSurfaceArea(0.0),
    mThermalROverD(0.0),
    mEfficiencyBias(0.0),
    mEfficiencyScaleFactor(0.0),
    mConfigData(0),
    mMalfBlockageFlag(false),
    mMalfBlockageValue(0.0),
    mMalfEfficiencyFlag(false),
    mMalfEfficiencyValue(0.0),
    mDesorbtionCycle(false),
    mAdsorbedMass(0.0),
    mWallTemperature(0.0),
    mInputData(0),
    mFluidTemperature(0.0),
    mWallHeatFlux(0.0),
    mMass(0.0),
    mSorbtionFlowRate(0.0),
    mArticle(0),
    mFlowRate(0.0),
    mTimeStep(0.0),
    mTolerance(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Hot Adsorber link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidHotAdsorber::~UtGunnsFluidHotAdsorber()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////

void UtGunnsFluidHotAdsorber::setUp()
{
    /// - Define the nominal port fluids.
    mFluidProperties       = new DefinedFluidProperties();
    mTypes[0]              = FluidProperties::GUNNS_CO2;
    mTypes[1]              = FluidProperties::GUNNS_O2;
    mFractions[0]          = 0.1;
    mFractions[1]          = 0.9;
    mFluidConfig           = new PolyFluidConfigData(mFluidProperties, mTypes, N_FLUIDS);
    mFluidInput0           = new PolyFluidInputData(283.0,
                                                    109.0,
                                                      0.0,
                                                      0.0,
                                                    mFractions);
    mFluidInput1           = new PolyFluidInputData(283.0,
                                                    111.0,
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
    mNodeList.mNodes       = mNodes;
    mNodeList.mNumNodes    = N_NODES;

    /// - Define the nominal port mapping.
    mPort0                 = 0;
    mPort1                 = 1;

    /// - Define the nominal configuration data.
    mName                  = "nominal";
    mMaxConductivity       = 1.5;
    mExpansionScaleFactor  = 0.5;
    mGasType               = FluidProperties::GUNNS_CO2;
    mGasIndex              = mNodes[0].getContent()->find(mGasType);
    mEfficiency            = 0.8;
    mMaxAdsorbtionRate     = 0.01;
    mMaxAdsorbedMass       = 0.5;
    mDesorbtionRate        = 0.02;
    mThermalLength         = 1.0;
    mThermalDiameter       = 1.0;
    mSurfaceRoughness      = 1.0e-06;
    mThermalSurfaceArea    = UnitConversion::PI_UTIL * mThermalDiameter * mThermalLength;
    mThermalROverD         = mSurfaceRoughness / mThermalDiameter;
    mEfficiencyBias        = 1.0;
    mEfficiencyScaleFactor = 0.0;
    mConfigData            = new GunnsFluidHotAdsorberConfigData(mName,
                                                                 &mNodeList,
                                                                 mMaxConductivity,
                                                                 mExpansionScaleFactor,
                                                                 mGasType,
                                                                 mEfficiency,
                                                                 mMaxAdsorbtionRate,
                                                                 mMaxAdsorbedMass,
                                                                 mDesorbtionRate,
                                                                 mThermalLength,
                                                                 mThermalDiameter,
                                                                 mSurfaceRoughness,
                                                                 mEfficiencyBias,
                                                                 mEfficiencyScaleFactor);

    /// - Define the nominal input data.
    mMalfBlockageFlag      = false;
    mMalfBlockageValue     = 0.3;
    mDesorbtionCycle       = false;
    mAdsorbedMass          = 0.4;
    mWallTemperature       = 283.0;
    mMalfEfficiencyFlag    = false;
    mMalfEfficiencyValue   = 0.5;
    mInputData             = new GunnsFluidHotAdsorberInputData(mMalfBlockageFlag,
                                                                mMalfBlockageValue,
                                                                mDesorbtionCycle,
                                                                mAdsorbedMass,
                                                                mWallTemperature,
                                                                0,
                                                                0,
                                                                0,
                                                                mMalfEfficiencyFlag,
                                                                mMalfEfficiencyValue);

    /// - Define the nominal port mapping.
    mPort0                 = 1;
    mPort1                 = 0;

    /// - Default construct the nominal test article.
    mFluidTemperature      = 0.5 * (mWallTemperature + mFluidInput0->mTemperature);
    mWallHeatFlux          = 0.0;
    mMass                  = 0.0;
    mSorbtionFlowRate      = 0.0;
    mArticle               = new FriendlyGunnsFluidHotAdsorber;

    /// - Define the nominal flow rate, time step and comparison tolerance.
    mFlowRate              = 0.1;
    mTimeStep              = 1.0;
    mTolerance             = 1.0e-06;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHotAdsorber::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete mArticle;
    delete mInputData;
    delete mConfigData;
    delete mFluidInput1;
    delete mFluidInput0;
    delete mFluidConfig;
    delete mFluidProperties;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Hot Adsorber link model construction of configuration and input
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHotAdsorber::testConfigAndInput()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(mName                                          == mConfigData->mName);
    CPPUNIT_ASSERT(mNodes                                         == mConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity,                   mConfigData->mMaxConductivity,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mExpansionScaleFactor,              mConfigData->mExpansionScaleFactor,  0.0);
    CPPUNIT_ASSERT(FluidProperties::GUNNS_CO2                     == mConfigData->mGasType);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mEfficiency,                        mConfigData->mEfficiency,            0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxAdsorbtionRate,                 mConfigData->mMaxAdsorbtionRate,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxAdsorbedMass,                   mConfigData->mMaxAdsorbedMass,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mDesorbtionRate,                    mConfigData->mDesorbtionRate,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalLength,                     mConfigData->mThermalLength,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalDiameter,                   mConfigData->mThermalDiameter,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSurfaceRoughness,                  mConfigData->mSurfaceRoughness,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mEfficiencyBias,                    mConfigData->mEfficiencyBias,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mEfficiencyScaleFactor,             mConfigData->mEfficiencyScaleFactor, 0.0);

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(mMalfBlockageFlag                              == mInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfBlockageValue,                 mInputData->mMalfBlockageValue,      0.0);
    CPPUNIT_ASSERT(mDesorbtionCycle                               == mInputData->mDesorbtionCycle);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mAdsorbedMass,                      mInputData->mAdsorbedMass,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mWallTemperature,                   mInputData->mWallTemperature,        0.0);
    CPPUNIT_ASSERT(0                                              == mInputData->mTcEfficiency);
    CPPUNIT_ASSERT(0                                              == mInputData->mTcMaxAdsorbedMass);
    CPPUNIT_ASSERT(0                                              == mInputData->mTcAdsorbedMass);
    CPPUNIT_ASSERT(mMalfEfficiencyFlag                            == mInputData->mMalfEfficiencyFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfEfficiencyValue,               mInputData->mMalfEfficiencyValue,    0.0);

    /// @test    Configuration data default construction.
    GunnsFluidHotAdsorberConfigData defaultConfig;
    CPPUNIT_ASSERT(0                                              == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0                                              == defaultConfig.mNodeList);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mMaxConductivity,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mExpansionScaleFactor, 0.0);
    CPPUNIT_ASSERT(FluidProperties::NO_FLUID                      == defaultConfig.mGasType);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mEfficiency,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mMaxAdsorbtionRate,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mMaxAdsorbedMass,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mDesorbtionRate,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mThermalLength,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mThermalDiameter,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mSurfaceRoughness,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mEfficiencyBias,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mEfficiencyScaleFactor,0.0);

    /// @test    Input data default construction.
    GunnsFluidHotAdsorberInputData   defaultInput;
    CPPUNIT_ASSERT(                                                 !defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultInput.mMalfBlockageValue,     0.0);
    CPPUNIT_ASSERT(                                                 !defaultInput.mDesorbtionCycle);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultInput.mAdsorbedMass,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultInput.mWallTemperature,       0.0);
    CPPUNIT_ASSERT(0                                              == defaultInput.mTcEfficiency);
    CPPUNIT_ASSERT(0                                              == defaultInput.mTcMaxAdsorbedMass);
    CPPUNIT_ASSERT(0                                              == defaultInput.mTcAdsorbedMass);
    CPPUNIT_ASSERT(                                                 !defaultInput.mMalfEfficiencyFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultInput.mMalfEfficiencyValue,   0.0);

    /// @test    Configuration data copy construction.
    GunnsFluidHotAdsorberConfigData copyConfig(*mConfigData);
    CPPUNIT_ASSERT(mConfigData->mName                             == copyConfig.mName);
    CPPUNIT_ASSERT(mConfigData->mNodeList->mNodes                 == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mMaxConductivity,      copyConfig.mMaxConductivity,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mExpansionScaleFactor, copyConfig.mExpansionScaleFactor,    0.0);
    CPPUNIT_ASSERT(mConfigData->mGasType                          == copyConfig.mGasType);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mEfficiency,           copyConfig.mEfficiency,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mMaxAdsorbtionRate,    copyConfig.mMaxAdsorbtionRate,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mMaxAdsorbedMass,      copyConfig.mMaxAdsorbedMass,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mDesorbtionRate,       copyConfig.mDesorbtionRate,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mThermalLength,        copyConfig.mThermalLength,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mThermalDiameter,      copyConfig.mThermalDiameter,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mSurfaceRoughness,     copyConfig.mSurfaceRoughness,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mEfficiencyBias,       copyConfig.mEfficiencyBias,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mEfficiencyScaleFactor,copyConfig.mEfficiencyScaleFactor,   0.0);

    /// @test    Input data copy construction.
    GunnsFluidHotAdsorberInputData   copyInput(*mInputData);
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag                  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfBlockageValue,     copyInput.mMalfBlockageValue,        0.0);
    CPPUNIT_ASSERT(mInputData->mDesorbtionCycle                   == copyInput.mDesorbtionCycle);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mAdsorbedMass,          copyInput.mAdsorbedMass,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mWallTemperature,       copyInput.mWallTemperature,          0.0);
    CPPUNIT_ASSERT(0                                              == copyInput.mTcEfficiency);
    CPPUNIT_ASSERT(0                                              == copyInput.mTcMaxAdsorbedMass);
    CPPUNIT_ASSERT(0                                              == copyInput.mTcAdsorbedMass);
    CPPUNIT_ASSERT(mInputData->mMalfEfficiencyFlag                == copyInput.mMalfEfficiencyFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfEfficiencyValue,   copyInput.mMalfEfficiencyValue,      0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Hot Adsorber link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHotAdsorber::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default construction configuration data.
    CPPUNIT_ASSERT(""                        == mArticle->mName);
    CPPUNIT_ASSERT(0                         == mArticle->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           mArticle->mMaxConductivity,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           mArticle->mExpansionScaleFactor,  0.0);
    CPPUNIT_ASSERT(FluidProperties::NO_FLUID == mArticle->mGasType);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           mArticle->mEfficiency,            0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           mArticle->mMaxAdsorbtionRate,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           mArticle->mMaxAdsorbedMass,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           mArticle->mDesorbtionRate,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           mArticle->mThermalDiameter,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           mArticle->mThermalSurfaceArea,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           mArticle->mThermalROverD,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           mArticle->mEfficiencyBias,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           mArticle->mEfficiencyScaleFactor, 0.0);

    /// @test    Default construction input data.
    CPPUNIT_ASSERT(                            !mArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           mArticle->mMalfBlockageValue,     0.0);
    CPPUNIT_ASSERT(                            !mArticle->mDesorbtionCycle);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           mArticle->mAdsorbedMass,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           mArticle->mWallTemperature,       0.0);
    CPPUNIT_ASSERT(                            !mArticle->mMalfEfficiencyFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           mArticle->mMalfEfficiencyValue,  0.0);

    /// @test    Default construction state data.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           mArticle->mFluidTemperature,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           mArticle->mWallHeatFlux,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           mArticle->mMass,                  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           mArticle->mSorbtionFlowRate,      0.0);
    CPPUNIT_ASSERT(0                         == mArticle->mSorbtionFluid);
    CPPUNIT_ASSERT(0                         == mArticle->mInternalFluid);
    CPPUNIT_ASSERT(0                         == mArticle->mGasIndex);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(!mArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsFluidHotAdsorber* article = new GunnsFluidHotAdsorber();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Hot Adsorber link model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHotAdsorber::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    FriendlyGunnsFluidHotAdsorber article;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    Nominal configuration data.
    CPPUNIT_ASSERT(mName                                      == article.mName);
    CPPUNIT_ASSERT(&mNodes[mPort0]                            == article.mNodes[0]);
    CPPUNIT_ASSERT(&mNodes[mPort1]                            == article.mNodes[1]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity,               article.mMaxConductivity,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mExpansionScaleFactor,          article.mExpansionScaleFactor,  0.0);
    CPPUNIT_ASSERT(mConfigData->mGasType                      == article.mGasType);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mEfficiency,                    article.mEfficiency,            0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxAdsorbtionRate,             article.mMaxAdsorbtionRate,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxAdsorbedMass,               article.mMaxAdsorbedMass,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mDesorbtionRate,                article.mDesorbtionRate,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalDiameter,               article.mThermalDiameter,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalSurfaceArea,            article.mThermalSurfaceArea,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalROverD,                 article.mThermalROverD,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mEfficiencyBias,                article.mEfficiencyBias,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mEfficiencyScaleFactor,         article.mEfficiencyScaleFactor, 0.0);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag              == article.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfBlockageValue, article.mMalfBlockageValue,     0.0);
    CPPUNIT_ASSERT(mDesorbtionCycle                           == article.mDesorbtionCycle);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mAdsorbedMass,                  article.mAdsorbedMass,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mWallTemperature,               article.mWallTemperature,      0.0);
    CPPUNIT_ASSERT(mMalfEfficiencyFlag                        == article.mMalfEfficiencyFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfEfficiencyValue,           article.mMalfEfficiencyValue,  0.0);

    /// @test    Nominal state data.
    CPPUNIT_ASSERT(0                                          != article.mInternalFluid);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                            article.mMass,                 0.0);
    CPPUNIT_ASSERT(0                                          != article.mSorbtionFluid);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                            article.mSorbtionFlowRate,     0.0);
    CPPUNIT_ASSERT(mGasIndex                                  == article.mGasIndex);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(mFluidTemperature,              article.mFluidTemperature,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mWallHeatFlux,                  article.mWallHeatFlux,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMass,                          article.mMass,                  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSorbtionFlowRate,              article.mSorbtionFlowRate,      0.0);
    CPPUNIT_ASSERT(0                                          != article.mSorbtionFluid);
    CPPUNIT_ASSERT(0                                          != article.mInternalFluid);
    CPPUNIT_ASSERT(mGasIndex                                  == article.mGasIndex);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(article.mInitFlag);

    /// @test    Test RestartMethods for functionality
    article.mFluidTemperature = 1.0;
    article.mMass = 1.0;
    article.mSorbtionFlowRate = 1.0;

    article.restartModel();

    CPPUNIT_ASSERT(0.0 == article.mFluidTemperature);
    CPPUNIT_ASSERT(0.0 == article.mMass);
    CPPUNIT_ASSERT(0.0 == article.mSorbtionFlowRate);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Hot Adsorber link model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHotAdsorber::testAccessors()
{
    UT_RESULT;

    /// @test     isInitialized before initialization.
    CPPUNIT_ASSERT(                       !mArticle->isInitialized());

    /// @test     getAdsorbedMass before initialization.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,      mArticle->getAdsorbedMass(), 0.0);

    /// - Initialize default constructed test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test     isInitialized after initialization.
    CPPUNIT_ASSERT(                        mArticle->isInitialized());

    /// @test     getAdsorbedMass after initialization.
    const double expected   = 1.234;
    mArticle->mAdsorbedMass = expected;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->getAdsorbedMass(), 0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Hot Adsorber link model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHotAdsorber::testModifiers()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    Nothing (this derived class does not add or override any modifiers).

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Hot Adsorber link model update fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHotAdsorber::testUpdateFluidNominal()
{
    UT_RESULT;

    {
        /// @test     Nominal initialization data.
        mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
        const double expectedMass     = mNodes[0].getOutflow()->getMassFraction(mGasIndex) * 1.0 * mFlowRate * mTimeStep;
        const double expectedAdsorbed = mArticle->mAdsorbedMass + expectedMass;
        const double expectedRate     = -expectedMass / mTimeStep;
        mArticle->mFlowRate           = mFlowRate;
        mArticle->updateFluid(mTimeStep, 0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAdsorbed,           mArticle->mAdsorbedMass,                    mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,               mArticle->mMass,                            mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRate,               mArticle->mSorbtionFlowRate,                mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mFluidInput0->mTemperature, mArticle->mSorbtionFluid->getTemperature(), mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,               mArticle->mSorbtionFluid->getMass(),        mTolerance);
    } {
        /// @test    adsorbtion temperature dependence.
        mConfigData->mEfficiencyBias        = 0.25;
        mConfigData->mEfficiencyScaleFactor = 1.0 / (4.0 * 283.0);
        mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
        const double expectedMass     = mNodes[0].getOutflow()->getMassFraction(mGasIndex) * 0.5 * mFlowRate * mTimeStep;
        const double expectedAdsorbed = mArticle->mAdsorbedMass + expectedMass;
        const double expectedRate     = -expectedMass / mTimeStep;
        mArticle->mFlowRate           = mFlowRate;
        mArticle->updateFluid(mTimeStep, 0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRate,               mArticle->mSorbtionFlowRate,                mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAdsorbed,           mArticle->mAdsorbedMass,                    mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,               mArticle->mMass,                            mTolerance);
        mConfigData->mEfficiencyBias        =  1.0;
        mConfigData->mEfficiencyScaleFactor =  0.0;
    } {
        /// @test     Max removal rate constraint.
        mConfigData->mMaxAdsorbtionRate = 0.001;
        mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
        const double expectedRate     = -0.001;
        const double expectedMass     = -expectedRate * mTimeStep;;
        const double expectedAdsorbed = mArticle->mAdsorbedMass + expectedMass;
        mArticle->mFlowRate           = mFlowRate;
        mArticle->updateFluid(mTimeStep, 0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAdsorbed,           mArticle->mAdsorbedMass,                    mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,               mArticle->mMass,                            mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRate,               mArticle->mSorbtionFlowRate,                mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mFluidInput0->mTemperature, mArticle->mSorbtionFluid->getTemperature(), mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,               mArticle->mSorbtionFluid->getMass(),        mTolerance);
        mConfigData->mMaxAdsorbtionRate = mMaxAdsorbtionRate;
    } {
        /// @test     Capacity constraint.
        mInputData->mAdsorbedMass     = mConfigData->mMaxAdsorbedMass - 0.001;
        mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
        const double expectedMass     = 0.001;
        const double expectedAdsorbed = mConfigData->mMaxAdsorbedMass;
        const double expectedRate     = -0.001 / mTimeStep;
        mArticle->mFlowRate           = mFlowRate;
        mArticle->updateFluid(mTimeStep, 0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAdsorbed,           mArticle->mAdsorbedMass,                    mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,               mArticle->mMass,                            mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRate,               mArticle->mSorbtionFlowRate,                mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mFluidInput0->mTemperature, mArticle->mSorbtionFluid->getTemperature(), mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,               mArticle->mSorbtionFluid->getMass(),        mTolerance);
        mInputData->mAdsorbedMass     = mAdsorbedMass;
    } {
        /// @test     Nominal initialization data with reverse flow, efficiency malf active.
        mInputData->mMalfEfficiencyFlag = true;
        mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
        const double expectedMass     = mNodes[0].getOutflow()->getMassFraction(mGasIndex) * mMalfEfficiencyValue * mFlowRate * mTimeStep;
        const double expectedAdsorbed = mArticle->mAdsorbedMass + expectedMass;
        const double expectedRate     = -expectedMass / mTimeStep;
        mArticle->mFlowRate           = -mFlowRate;
        mArticle->updateFluid(mTimeStep, 0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAdsorbed,           mArticle->mAdsorbedMass,                    mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,               mArticle->mMass,                            mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRate,               mArticle->mSorbtionFlowRate,                mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mFluidInput0->mTemperature, mArticle->mSorbtionFluid->getTemperature(), mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,               mArticle->mSorbtionFluid->getMass(),        mTolerance);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Hot Adsorber link model update fluid with no flow.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHotAdsorber::testUpdateFluidOffNominal()
{
    UT_RESULT;

    /// @test    No flow adsorbtion.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->mFlowRate                          = 0.0;
    mArticle->updateFluid(mTimeStep, 1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mAdsorbedMass,    mArticle->mAdsorbedMass,     mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              mArticle->mMass,             mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              mArticle->mSorbtionFlowRate, mTolerance);

    /// @test    Zero time step adsorbtion.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->mFlowRate                          = mFlowRate;
    mArticle->updateFluid(0.0, 2.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mAdsorbedMass,    mArticle->mAdsorbedMass,     mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              mArticle->mMass,             mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              mArticle->mSorbtionFlowRate, mTolerance);

    /// @test    Too low flow adsorbtion.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->mFlowRate                          = -DBL_EPSILON;
    mArticle->updateFluid(mTimeStep, 3.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mAdsorbedMass,    mArticle->mAdsorbedMass,     mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              mArticle->mMass,             mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              mArticle->mSorbtionFlowRate, mTolerance);

    const double expectedMass                    = -mDesorbtionRate * mTimeStep;
    const double expectedAdsorbed                =  mAdsorbedMass - mDesorbtionRate * mTimeStep;
    const double expectedRate                    = -expectedMass / mTimeStep;

    /// @test    No flow desorbtion.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->mFlowRate                          =  0.0;
    mInputData->mDesorbtionCycle                 = true;
    mArticle->updateFluid(mTimeStep, 1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mAdsorbedMass,    mArticle->mAdsorbedMass,     mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              mArticle->mMass,             mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              mArticle->mSorbtionFlowRate, mTolerance);

    /// @test    Zero time step desorbtion.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->mFlowRate                          = mFlowRate;
    mArticle->mDesorbtionCycle                   = true;
    mArticle->updateFluid(0.0, 2.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mAdsorbedMass,    mArticle->mAdsorbedMass,     mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              mArticle->mMass,             mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              mArticle->mSorbtionFlowRate, mTolerance);

    /// @test    Too low flow desorbtion.
    mConfigData->mEfficiencyBias                 = -1.0;
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->mFlowRate                          = -DBL_EPSILON;
    mInputData->mDesorbtionCycle                 = true;
    mArticle->updateFluid(mTimeStep, 3.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAdsorbed, mArticle->mAdsorbedMass,     mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,     mArticle->mMass,             mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRate,     mArticle->mSorbtionFlowRate, mTolerance);
    mInputData->mDesorbtionCycle                 = false;
    mConfigData->mEfficiencyBias                 = 1.0;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Hot Adsorber link model update fluid in desorbtion cycle.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHotAdsorber::testUpdateFluidDesorbtion()
{
    UT_RESULT;

    {
        /// @test    Nominal.
        mConfigData->mEfficiencyBias  = -1.0;
        const double expectedMass     = -mDesorbtionRate * mTimeStep;
        const double expectedAdsorbed =  mAdsorbedMass - mDesorbtionRate * mTimeStep;
        const double expectedRate     =  -expectedMass / mTimeStep;
        mInputData->mDesorbtionCycle  = true;
        mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
        mArticle->mFlowRate           =  mFlowRate;
        mArticle->updateFluid(mTimeStep, 0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAdsorbed, mArticle->mAdsorbedMass,     mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,     mArticle->mMass,             mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRate,     mArticle->mSorbtionFlowRate, mTolerance);
    } {
        /// @test    desorbtion temperature dependence.
        mConfigData->mEfficiencyBias        = -0.25;
        mConfigData->mEfficiencyScaleFactor = -1.0 / (4.0 * 283.0);
        const double expectedMass           = -mDesorbtionRate * mTimeStep * 0.5;
        const double expectedAdsorbed       =  mAdsorbedMass - mDesorbtionRate * mTimeStep * 0.5;
        const double expectedRate           =  -expectedMass / mTimeStep;
        mInputData->mDesorbtionCycle        = true;
        mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
        mArticle->mFlowRate                 =  mFlowRate;
        mArticle->updateFluid(mTimeStep, 0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAdsorbed, mArticle->mAdsorbedMass,     mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,     mArticle->mMass,             mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRate,     mArticle->mSorbtionFlowRate, mTolerance);
        mConfigData->mEfficiencyBias        = -1.0;
        mConfigData->mEfficiencyScaleFactor =  0.0;
    } {
        /// @test    Availability constraint.
        const double expectedMass     = -0.0001;
        const double expectedAdsorbed =  0.0;
        const double expectedRate     = -expectedMass / mTimeStep;
        mInputData->mAdsorbedMass     = -expectedMass;
        mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
        mArticle->mFlowRate           =  mFlowRate;
        mArticle->updateFluid(mTimeStep, 0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAdsorbed, mArticle->mAdsorbedMass,     mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,     mArticle->mMass,             mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRate,     mArticle->mSorbtionFlowRate, mTolerance);
        mConfigData->mEfficiencyBias  = 1.0;
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Hot Adsorber link model specific port mapping rules.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHotAdsorber::testPortMapping()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    {
        /// @test    No fail if port 1 is the vacuum boundary node.
        const bool returned = mArticle->checkSpecificPortRules(1,1);
        CPPUNIT_ASSERT(returned);
    } {
        /// @test    No fail if port 0 is not the vacuum boundary node.
        const bool returned = mArticle->checkSpecificPortRules(0,0);
        CPPUNIT_ASSERT(returned);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Hot Adsorber link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHotAdsorber::testInitializationExceptions()
{
    UT_RESULT;

    /// - Default construct a test article.
    GunnsFluidHotAdsorber article;

    /// @test    Initialization exception on invalid config data: no name.
    mConfigData->mName = "";
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mName = mName;

    /// @test    Initialization exception on invalid config data: max conductivity < 0.
    mConfigData->mMaxConductivity = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mMaxConductivity = mMaxConductivity;

    /// @test    Initialization exception on invalid config data: expansion scale factor < 0.
    mConfigData->mExpansionScaleFactor = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mExpansionScaleFactor = mExpansionScaleFactor;

    /// @test    Initialization exception on invalid config data: gas type not in atmosphere.
    mConfigData->mGasType = FluidProperties::GUNNS_CH4;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mGasType = FluidProperties::GUNNS_CO2;

    /// @test    Initialization exception on invalid config data: adsorption efficiency < 0.0.
    mConfigData->mEfficiency = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mEfficiency = mEfficiency;

    /// @test    Initialization exception on invalid config data: adsorption efficiency > 1.0.
    mConfigData->mEfficiency = 1.0 + FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mEfficiency = mEfficiency;

    /// @test    Initialization exception on invalid config data: maximum adsorbed mass < 0.0.
    mConfigData->mMaxAdsorbtionRate = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mMaxAdsorbtionRate = mMaxAdsorbtionRate;

    /// @test    Initialization exception on invalid config data: maximum adsorption rate < 0.0.
    mConfigData->mMaxAdsorbedMass = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mMaxAdsorbedMass = mMaxAdsorbedMass;

    /// @test    Initialization exception on invalid config data: desorbtion rate < 0.0.
    mConfigData->mDesorbtionRate = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mDesorbtionRate = mDesorbtionRate;

    /// @test    Initialization exception on invalid input data: blockage malfunction value < 0.
    mInputData->mMalfBlockageValue = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mInputData->mMalfBlockageValue = mMalfBlockageValue;

    /// @test    Initialization exception on invalid input data: blockage malfunction value > 1.
    mInputData->mMalfBlockageValue = 1.0 + FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mInputData->mMalfBlockageValue = mMalfBlockageValue;

    /// @test    Initialization exception on invalid input data: mass in adsorber < 0.
    mInputData->mAdsorbedMass = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mInputData->mAdsorbedMass = mAdsorbedMass;

    /// @test    Initialization exception on invalid config data: wall temperature < 0.0.
    mInputData->mWallTemperature = -0.5 * DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mInputData->mWallTemperature = mWallTemperature;

    UT_PASS_LAST;
}
