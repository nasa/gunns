/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/fluid/conductor/GunnsFluidRegulatorValve.o))
***************************************************************************************************/

#include "core/GunnsFluidUtils.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidRegulatorValve.hh"

/// @details  Test identification number.
int UtGunnsFluidRegulatorValve::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Pressure Regulator Valve link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidRegulatorValve::UtGunnsFluidRegulatorValve()
    :
    CppUnit::TestFixture(),
    mTypes(),
    mFractions(),
    mFluidProperties(0),
    mFluidConfig(0),
    mFluidInput0(0),
    mFluidInput1(0),
    mFluidInput2(0),
    mFluidInput3(0),
    mLinks(),
    mName(""),
    mNodes(),
    mNodeList(),
    mPort0(0),
    mPort1(0),
    mPort2(0),
    mPort3(0),
    mMaxConductivity(0.0),
    mExpansionScaleFactor(0.0),
    mRateLimit(0.0),
    mThermalLength(0.0),
    mThermalDiameter(0.0),
    mSurfaceRoughness(0.0),
    mThermalSurfaceArea(0.0),
    mThermalROverD(0.0),
    mReseatPressure(0.0),
    mCrackPressure(0.0),
    mFullOpenPressure(0.0),
    mPopPosition(0.0),
    mPopSlopeScale(0.0),
    mConfigData(0),
    mMalfBlockageFlag(false),
    mMalfBlockageValue(0.0),
    mPosition(0.0),
    mMalfLeakThruFlag(0.0),
    mMalfLeakThruValue(0.0),
    mMalfPressureBiasFlag(0.0),
    mMalfPressureBiasValue(0.0),
    mSetPointPressureBias(0.0),
    mWallTemperature(0.0),
    mMalfStuckFlag(false),
    mMalfFailToFlag(false),
    mMalfFailToValue(0.0),
    mInputData(0),
    mArticle(0),
    mPreviousLeakRate(0.0),
    mLeakConductivity(0.0),
    mTuneMode(GunnsFluidUtils::OFF),
    mTuneMassFlow(0.0),
    mTuneVolFlow(0.0),
    mTuneDeltaT(0.0),
    mEffectiveConductivity(0.0),
    mSystemConductance(0.0),
    mLastSystemConductance(0.0),
    mControlPressure(0.0),
    mWallHeatFlux(0.0),
    mTimeStep(0.0),
    mTolerance(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Pressure Regulator Valve link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidRegulatorValve::~UtGunnsFluidRegulatorValve()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidRegulatorValve::setUp()
{
    /// - Define the nominal port fluids.
    mFluidProperties       = new DefinedFluidProperties();
    mTypes[0]              = FluidProperties::GUNNS_N2;
    mTypes[1]              = FluidProperties::GUNNS_O2;
    mFractions[0]          = 0.5;
    mFractions[1]          = 0.5;
    mFluidConfig           = new PolyFluidConfigData(mFluidProperties, mTypes, N_FLUIDS);
    mFluidInput0           = new PolyFluidInputData(283.0,
                                                    111.0,
                                                      0.0,
                                                      0.0,
                                                    mFractions);
    mFluidInput1           = new PolyFluidInputData(283.0,
                                                    111.0,
                                                      0.0,
                                                      0.0,
                                                    mFractions);
    mFluidInput2           = new PolyFluidInputData(283.0,
                                                    111.0,
                                                      0.0,
                                                      0.0,
                                                    mFractions);
    mFluidInput3           = new PolyFluidInputData(283.0,
                                                    111.0,
                                                      0.0,
                                                      0.0,
                                                    mFractions);

    /// - Initialize the nodes.
    mNodes[0].initialize("UtNode1", mFluidConfig);
    mNodes[1].initialize("UtNode2", mFluidConfig);
    mNodes[2].initialize("UtNode3", mFluidConfig);
    mNodes[3].initialize("UtNode4", mFluidConfig);
    mNodes[0].getContent()->initialize(*mFluidConfig, *mFluidInput0);
    mNodes[1].getContent()->initialize(*mFluidConfig, *mFluidInput1);
    mNodes[2].getContent()->initialize(*mFluidConfig, *mFluidInput2);
    mNodes[3].getContent()->initialize(*mFluidConfig, *mFluidInput3);

    mNodes[0].resetFlows();
    mNodes[1].resetFlows();
    mNodes[2].resetFlows();
    mNodes[3].resetFlows();

    /// - Initialize the nodes list.
    mNodeList.mNodes       = mNodes;
    mNodeList.mNumNodes    = N_NODES + 1;

    /// - Define the nominal configuration data.
    mName                  = "nominal";
    mMaxConductivity       =  0.5;
    mExpansionScaleFactor  =  0.4;
    mRateLimit             =  1.0;
    mThermalLength         =  0.2;
    mThermalDiameter       =  0.5;
    mSurfaceRoughness      =  1.0e-06;
    mThermalSurfaceArea    =  mThermalLength * mThermalDiameter * UnitConversion::PI_UTIL;
    mThermalROverD         =  mSurfaceRoughness / mThermalDiameter;
    mReseatPressure        = 3.0;
    mCrackPressure         = 2.0;
    mFullOpenPressure      = 1.0;
    mPopPosition           = 0.10;
    mPopSlopeScale         = 4.0;
    mConfigData            = new GunnsFluidRegulatorValveConfigData(mName,
                                                                    &mNodeList,
                                                                    mMaxConductivity,
                                                                    mExpansionScaleFactor,
                                                                    mRateLimit,
                                                                    mThermalLength,
                                                                    mThermalDiameter,
                                                                    mSurfaceRoughness,
                                                                    mReseatPressure,
                                                                    mCrackPressure,
                                                                    mFullOpenPressure,
                                                                    mPopPosition,
                                                                    mPopSlopeScale);

    /// - Define the nominal input data.
    mMalfBlockageFlag      = false;
    mMalfBlockageValue     = 0.5;
    mPosition              = 1.0;
    mMalfLeakThruFlag      = false;
    mMalfLeakThruValue     = 0.01;
    mMalfPressureBiasFlag  = false;
    mMalfPressureBiasValue = 1.0;
    mSetPointPressureBias  = 0.0;
    mWallTemperature       = 290.0;
    mMalfStuckFlag         = false;
    mMalfFailToFlag        = false;
    mMalfFailToValue       = 0.0;
    mInputData             = new GunnsFluidRegulatorValveInputData(mMalfBlockageFlag,
                                                                   mMalfBlockageValue,
                                                                   mPosition,
                                                                   mMalfLeakThruFlag,
                                                                   mMalfLeakThruValue,
                                                                   mMalfPressureBiasFlag,
                                                                   mMalfPressureBiasValue,
                                                                   mSetPointPressureBias,
                                                                   mWallTemperature,
                                                                   mMalfStuckFlag,
                                                                   mMalfFailToFlag,
                                                                   mMalfFailToValue);

    /// - Define the nominal port mapping.
    mPort0                 = 0;
    mPort1                 = 1;
    mPort2                 = 2;
    mPort3                 = 3;

    /// - Default construct the nominal test article.
    mArticle          = new FriendlyGunnsFluidRegulatorValve;

    /// - Define the nominal initial state data
    mPreviousLeakRate     = 0.0;
    mLeakConductivity     = 0.0;
    mTuneMode              = GunnsFluidUtils::OFF;
    mTuneMassFlow          = 0.0;
    mTuneVolFlow           = 0.0;
    mTuneDeltaT            = 0.0;
    mEffectiveConductivity = mMaxConductivity;
    mSystemConductance     = 0.0;
    mLastSystemConductance = 0.0;
    mControlPressure       = 0.0;
    mWallHeatFlux          = 0.0;

    /// - Define the time step and comparison tolerance.
    mTimeStep             = 0.1;
    mTolerance            = 1.0e-08;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidRegulatorValve::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete mArticle;
    delete mInputData;
    delete mConfigData;
    delete mFluidInput3;
    delete mFluidInput2;
    delete mFluidInput1;
    delete mFluidInput0;
    delete mFluidConfig;
    delete mFluidProperties;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of GUNNS Fluid Pressure Regulator Valve link model
///           configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidRegulatorValve::testConfigAndInput()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(mName                                           == mConfigData->mName);
    CPPUNIT_ASSERT(mNodes                                          == mConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity,                    mConfigData->mMaxConductivity,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mExpansionScaleFactor,               mConfigData->mExpansionScaleFactor, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mRateLimit,                          mConfigData->mRateLimit,            0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalLength,                      mConfigData->mThermalLength,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalDiameter,                    mConfigData->mThermalDiameter,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSurfaceRoughness,                   mConfigData->mSurfaceRoughness,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mReseatPressure,                     mConfigData->mReseatPressure,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mCrackPressure,                      mConfigData->mCrackPressure,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mFullOpenPressure,                   mConfigData->mFullOpenPressure,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mPopPosition,                        mConfigData->mPopPosition,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mPopSlopeScale,                      mConfigData->mPopSlopeScale,        0.0);

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(mMalfBlockageFlag                               == mInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfBlockageValue,                  mInputData->mMalfBlockageValue,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mPosition,                           mInputData->mPosition,              0.0);
    CPPUNIT_ASSERT(mMalfLeakThruFlag                               == mInputData->mMalfLeakThruFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfLeakThruValue,                  mInputData->mMalfLeakThruValue,     0.0);
    CPPUNIT_ASSERT(mMalfPressureBiasFlag                           == mInputData->mMalfPressureBiasFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfPressureBiasValue,              mInputData->mMalfPressureBiasValue, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSetPointPressureBias,               mInputData->mSetPointPressureBias,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mWallTemperature,                    mInputData->mWallTemperature,       0.0);
    CPPUNIT_ASSERT(mMalfStuckFlag                                  == mInputData->mMalfStuckFlag);
    CPPUNIT_ASSERT(mMalfFailToFlag                                 == mInputData->mMalfFailToFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfFailToValue,                    mInputData->mMalfFailToValue,       0.0);

    /// @test    Configuration data default construction.
    GunnsFluidRegulatorValveConfigData defaultConfig;
    CPPUNIT_ASSERT(0                                              == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0                                              == defaultConfig.mNodeList);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mMaxConductivity,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mExpansionScaleFactor, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mRateLimit,            0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mThermalLength,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mThermalDiameter,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mSurfaceRoughness,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mReseatPressure,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mCrackPressure,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mFullOpenPressure,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mPopPosition,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mPopSlopeScale,        0.0);

    /// @test    Input data default construction.
    GunnsFluidRegulatorValveInputData   defaultInput;
    CPPUNIT_ASSERT(                                                 !defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultInput.mMalfBlockageValue,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultInput.mPosition,              0.0);
    CPPUNIT_ASSERT(                                                 !defaultInput.mMalfLeakThruFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultInput.mMalfLeakThruValue,     0.0);
    CPPUNIT_ASSERT(                                                 !defaultInput.mMalfPressureBiasFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultInput.mMalfPressureBiasValue, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultInput.mSetPointPressureBias,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultInput.mWallTemperature,       0.0);
    CPPUNIT_ASSERT(                                                 !defaultInput.mMalfStuckFlag);
    CPPUNIT_ASSERT(                                                 !defaultInput.mMalfFailToFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultInput.mMalfFailToValue,       0.0);

    /// @test    Configuration data copy construction.
    GunnsFluidRegulatorValveConfigData copyConfig(*mConfigData);
    CPPUNIT_ASSERT(mConfigData->mName                             == copyConfig.mName);
    CPPUNIT_ASSERT(mConfigData->mNodeList->mNodes                 == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mMaxConductivity,      copyConfig.mMaxConductivity,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mExpansionScaleFactor, copyConfig.mExpansionScaleFactor,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mRateLimit,            copyConfig.mRateLimit,               0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mThermalLength,        copyConfig.mThermalLength,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mThermalDiameter,      copyConfig.mThermalDiameter,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mSurfaceRoughness,     copyConfig.mSurfaceRoughness,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mReseatPressure,                    copyConfig.mReseatPressure,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mCrackPressure,                     copyConfig.mCrackPressure,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mFullOpenPressure,                  copyConfig.mFullOpenPressure,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mPopPosition,                       copyConfig.mPopPosition,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mPopSlopeScale,                     copyConfig.mPopSlopeScale,           0.0);

    /// @test    Input data copy construction.
    GunnsFluidRegulatorValveInputData   copyInput(*mInputData);
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag                  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfBlockageValue,     copyInput.mMalfBlockageValue,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mPosition,              copyInput.mPosition,                 0.0);
    CPPUNIT_ASSERT(mInputData->mMalfLeakThruFlag                  == copyInput.mMalfLeakThruFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfLeakThruValue,     copyInput.mMalfLeakThruValue,        0.0);
    CPPUNIT_ASSERT(mInputData->mMalfPressureBiasFlag              == copyInput.mMalfPressureBiasFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfPressureBiasValue, copyInput.mMalfPressureBiasValue,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mSetPointPressureBias,  copyInput.mSetPointPressureBias,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mWallTemperature,       copyInput.mWallTemperature,          0.0);
    CPPUNIT_ASSERT(mInputData->mMalfStuckFlag                     == copyInput.mMalfStuckFlag);
    CPPUNIT_ASSERT(mInputData->mMalfFailToFlag                    == copyInput.mMalfFailToFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfFailToValue,       copyInput.mMalfFailToValue,          0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Pressure Regulator Valve link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidRegulatorValve::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default construction configuration data.
    CPPUNIT_ASSERT(""                       == mArticle->mName);
    CPPUNIT_ASSERT(0                        == mArticle->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mMaxConductivity,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mExpansionScaleFactor,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mRateLimit,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mThermalDiameter,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mThermalSurfaceArea,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mThermalROverD,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mReseatPressure,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mCrackPressure,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mFullOpenPressure,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mPopPosition,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mPopSlopeScale,         0.0);

    /// @test    Default construction input data.
    CPPUNIT_ASSERT(                           !mArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mMalfBlockageValue,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mPosition,              0.0);
    CPPUNIT_ASSERT(                           !mArticle->mMalfLeakThruFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mMalfLeakThruValue,     0.0);
    CPPUNIT_ASSERT(                           !mArticle->mMalfPressureBiasFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mMalfPressureBiasValue, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mSetPointPressureBias,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mWallTemperature,       0.0);
    CPPUNIT_ASSERT(                           !mArticle->mMalfStuckFlag);
    CPPUNIT_ASSERT(                           !mArticle->mMalfFailToFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mMalfFailToValue,       0.0);

    /// @test    Default construction state data.
    CPPUNIT_ASSERT(0                        == mArticle->mInternalFluid);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mPreviousLeakRate,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mLeakConductivity,      0.0);
    CPPUNIT_ASSERT(GunnsFluidUtils::OFF     == mArticle->mTuneMode);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mTuneMassFlow,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mTuneVolFlow,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mTuneDeltaT,            0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mEffectiveConductivity, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mSystemConductance,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mControlPressure,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mWallHeatFlux,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mPopIncM,               0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mPopIncMInv,            0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mPopIncB,               0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mPopIncP,               0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mPopDecM,               0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mPopDecMInv,            0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mPopDecB,               0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mPopDecP,               0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mNomIncM,               0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mNomIncMInv,            0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mNomIncB,               0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mNomDecM,               0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mNomDecMInv,            0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mNomDecB,               0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mPreviousPressure,      0.0);
    CPPUNIT_ASSERT(GunnsFluidValve::OPENING == mArticle->mState);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(                           !mArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsFluidRegulatorValve* article = new GunnsFluidRegulatorValve();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Pressure Regulator Valve link model nominal initialization
///            without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidRegulatorValve::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    FriendlyGunnsFluidRegulatorValve article;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3);

    /// @test    Nominal configuration data.
    CPPUNIT_ASSERT(mName                              == article.mName);
    CPPUNIT_ASSERT(&mNodes[0]                         == article.mNodes[0]);
    CPPUNIT_ASSERT(&mNodes[1]                         == article.mNodes[1]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity,       article.mMaxConductivity,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mExpansionScaleFactor,  article.mExpansionScaleFactor,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mRateLimit,             article.mRateLimit,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalDiameter,       article.mThermalDiameter,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalSurfaceArea,    article.mThermalSurfaceArea,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalROverD,         article.mThermalROverD,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mReseatPressure,        article.mReseatPressure,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mCrackPressure,         article.mCrackPressure,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mFullOpenPressure,      article.mFullOpenPressure,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mPopPosition,           article.mPopPosition,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mPopSlopeScale,         article.mPopSlopeScale,         0.0);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT(mMalfBlockageFlag                  == article.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfBlockageValue,     article.mMalfBlockageValue,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mPosition,              article.mPosition,              0.0);
    CPPUNIT_ASSERT(mMalfLeakThruFlag                  == article.mMalfLeakThruFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfLeakThruValue,     article.mMalfLeakThruValue,     0.0);
    CPPUNIT_ASSERT(mMalfPressureBiasFlag              == article.mMalfPressureBiasFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfPressureBiasValue, article.mMalfPressureBiasValue, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSetPointPressureBias,  article.mSetPointPressureBias,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mWallTemperature,       article.mWallTemperature,       0.0);
    CPPUNIT_ASSERT(mMalfStuckFlag                     == article.mMalfStuckFlag);
    CPPUNIT_ASSERT(mMalfFailToFlag                    == article.mMalfFailToFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfFailToValue,       article.mMalfFailToValue,       0.0);

    /// @test    Nominal state data.
    CPPUNIT_ASSERT(0                                  != article.mInternalFluid);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mPreviousLeakRate,      article.mPreviousLeakRate,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mLeakConductivity,      article.mLeakConductivity,      0.0);
    CPPUNIT_ASSERT(mTuneMode                          == article.mTuneMode);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mTuneMassFlow,          article.mTuneMassFlow,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mTuneVolFlow,           article.mTuneVolFlow,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mTuneDeltaT,            article.mTuneDeltaT,            0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mEffectiveConductivity, article.mEffectiveConductivity, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSystemConductance,     article.mSystemConductance,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mControlPressure,       article.mControlPressure,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mWallHeatFlux,          article.mWallHeatFlux,          0.0);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(article.mInitFlag);

    /// @test    Verify restartModel functionality
    mEffectiveConductivity = 1.0;
    mSystemConductance     = 1.0;
    mTuneMassFlow          = 1.0;
    mControlPressure       = 1.0;
    mTuneDeltaT            = 1.0;
    mTuneVolFlow           = 1.0;

    article.restartModel();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article.mEffectiveConductivity, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article.mSystemConductance,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article.mTuneMassFlow,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article.mControlPressure,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article.mTuneDeltaT,            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article.mTuneVolFlow,           DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Pressure Regulator Valve link model derived initialization.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidRegulatorValve::testDerived()
{
    UT_RESULT;

    /// - Initialize (with nominal data) the default test article.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3);

    /// @test    Pop slopes.
    double expected = mPopSlopeScale / (mFullOpenPressure - mCrackPressure);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,
                                 mArticle->mPopDecM,
                                 mTolerance);
    expected        = mPopSlopeScale / (mFullOpenPressure - mReseatPressure);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,
                                 mArticle->mPopIncM,
                                 mTolerance);

    /// @test    Position 0 on decreasing pressure at crack pressure.
    double returned = mArticle->mPopDecB + mArticle->mPopDecM * mArticle->mCrackPressure;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 returned,
                                 mTolerance);

    /// @test    Position 0 on increasing pressure at reseat pressure.
    returned = mArticle->mPopIncB + mArticle->mPopIncM * mArticle->mReseatPressure;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 returned,
                                 mTolerance);

    /// @test    Position 1 on increasing pressure at full open pressure.
    returned = mArticle->mNomIncB + mArticle->mNomIncM * mArticle->mFullOpenPressure;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,
                                 returned,
                                 mTolerance);

    /// @test    Position 1 on decreasing pressure at full open pressure.
    returned = mArticle->mNomDecB + mArticle->mNomDecM * mArticle->mFullOpenPressure;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,
                                 returned,
                                 mTolerance);

    /// @test    Same pressure on increasing pressure at transition from pop to nominal.
    expected = (mArticle->mPopPosition - mArticle->mPopIncB) * mArticle->mPopIncMInv;
    returned = (mArticle->mPopPosition - mArticle->mNomIncB) * mArticle->mNomIncMInv;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,
                                 returned,
                                 mTolerance);

    /// @test    Same pressure on decreasing pressure at transition from nominal to pop.
    expected = (mArticle->mPopPosition - mArticle->mPopDecB) * mArticle->mPopDecMInv;
    returned = (mArticle->mPopPosition - mArticle->mNomDecB) * mArticle->mNomDecMInv;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,
                                 returned,
                                 mTolerance);

    /// @test    Valve state consistent with position.
    mInputData->mPosition = 0.0;
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3);
    CPPUNIT_ASSERT(mArticle->mState == GunnsFluidValve::CLOSED);
    mInputData->mPosition = 0.5;
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3);
    CPPUNIT_ASSERT(mArticle->mState == GunnsFluidValve::OPENING);
    mInputData->mPosition = 1.0;
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3);
    CPPUNIT_ASSERT(mArticle->mState == GunnsFluidValve::OPEN);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Pressure Regulator Valve link model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidRegulatorValve::testAccessors()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3);

    /// @test    Test state access method.
    mArticle->mState = GunnsFluidValve::TRANSITIONING;
    CPPUNIT_ASSERT(GunnsFluidValve::TRANSITIONING==mArticle->getState());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Pressure Regulator Valve link model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidRegulatorValve::testModifiers()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3);

    /// @test    Nothing (this derived class does not add or override any modifiers).

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Pressure Regulator Valve link model update state (nominal).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidRegulatorValve::testUpdateStateNominal()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3);

    /// - Set closed position.
    mArticle->mPosition = 0.0;
    mArticle->mState    = GunnsFluidValve::CLOSED;

    /// - Effectively disable rate limiting.
    mArticle->mRateLimit = 1.0 / mTimeStep;

    /// - Set node 3 to vacuum
    mArticle->mPotentialVector[3]   = 0.0;

    /// @test    Start above crack pressure so valve is closed.
    mArticle->mPotentialVector[2]   = mCrackPressure + FLT_EPSILON;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mPosition, mTolerance);
    CPPUNIT_ASSERT(GunnsFluidValve::CLOSED == mArticle->mState);

    /// @test    Decrement to just below crack pressure so valve is closed but opening.
    mArticle->mPotentialVector[2]   = mCrackPressure - FLT_EPSILON;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mPosition, mTolerance * 1.0e+05);
    CPPUNIT_ASSERT(GunnsFluidValve::OPENING == mArticle->mState);

    /// @test    Decrement to dec pop pressure so valve is opening at pop position.
    mArticle->mPotentialVector[2]   = (mArticle->mPopPosition - mArticle->mPopDecB) * mArticle->mPopDecMInv;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mPopPosition, mArticle->mPosition, mTolerance);
    CPPUNIT_ASSERT(GunnsFluidValve::OPENING == mArticle->mState);

    /// @test    Decrement to just above full open pressure so valve is opening and almost open.
    mArticle->mPotentialVector[2]   = mFullOpenPressure + FLT_EPSILON;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, mArticle->mPosition, mTolerance * 1.0e+05);
    CPPUNIT_ASSERT(GunnsFluidValve::OPENING == mArticle->mState);

    /// @test    Decrement to just below full open pressure so valve is open.
    mArticle->mPotentialVector[2]   = mFullOpenPressure - FLT_EPSILON;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, mArticle->mPosition, mTolerance);
    CPPUNIT_ASSERT(GunnsFluidValve::OPEN == mArticle->mState);

    /// @test    Increment to inc pop pressure so valve is at pop position and closing.
    mArticle->mPotentialVector[2]   = (mArticle->mPopPosition - mArticle->mPopIncB) * mArticle->mPopIncMInv;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mPopPosition, mArticle->mPosition, mTolerance);
    CPPUNIT_ASSERT(GunnsFluidValve::CLOSING == mArticle->mState);

    /// @test    Increment to just below reseat pressure so valve is closing and almost closed.
    mArticle->mPotentialVector[2]   = mReseatPressure - FLT_EPSILON;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mPosition, mTolerance * 1.0e+05);
    CPPUNIT_ASSERT(GunnsFluidValve::CLOSING == mArticle->mState);

    /// @test    Increment to just above reseat pressure so valve is closed.
    mArticle->mPotentialVector[2]   = mReseatPressure + FLT_EPSILON;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mPosition, mTolerance);
    CPPUNIT_ASSERT(GunnsFluidValve::CLOSED == mArticle->mState);

    /// @test    Default state case
    mArticle->mState    = static_cast<GunnsFluidValve::State>(-1);
    mArticle->mPosition = 0.5;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, mArticle->mPosition, mTolerance);
    CPPUNIT_ASSERT(-1 == mArticle->mState);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Pressure Regulator Valve link model update state (hysteresis).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidRegulatorValve::testUpdateStateHysteresis()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3);

    /// - Set closed position.
    mArticle->mPosition = 0.0;
    mArticle->mState    = GunnsFluidValve::CLOSED;

    /// - Effectively disable rate limiting.
    mArticle->mRateLimit = 1.0 / mTimeStep;

    /// - Set node 3 to vacuum
    mArticle->mPotentialVector[3]   = 0.0;

    /// @test    Start above crack pressure so valve is closed.
    mArticle->mPotentialVector[2]   = mCrackPressure + FLT_EPSILON;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mPosition, mTolerance);
    CPPUNIT_ASSERT(GunnsFluidValve::CLOSED == mArticle->mState);

    /// @test    Decrement to mCrackPressure - 0.01 so valve is opening.
    const double expected01 = mArticle->mPopDecB + mArticle->mPopDecM * (mCrackPressure - 0.01);
    mArticle->mPotentialVector[2]   = mCrackPressure - 0.01;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected01, mArticle->mPosition, mTolerance);
    CPPUNIT_ASSERT(GunnsFluidValve::OPENING == mArticle->mState);

    /// @test    Increment to mCrackPressure - 0.009 so valve is transitioning.
    mArticle->mPotentialVector[2]   = mCrackPressure - 0.009;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected01, mArticle->mPosition, mTolerance);
    CPPUNIT_ASSERT(GunnsFluidValve::TRANSITIONING == mArticle->mState);

    /// @test    Decrement to mCrackPressure - 0.0095 so valve is transitioning.
    mArticle->mPotentialVector[2]   = mCrackPressure - 0.0095;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected01, mArticle->mPosition, mTolerance);
    CPPUNIT_ASSERT(GunnsFluidValve::TRANSITIONING == mArticle->mState);

    /// @test    Increment to mReseatPressure - 0.005 so valve is closing.
    const double expected05 = mArticle->mPopIncB + mArticle->mPopIncM * (mReseatPressure - 0.005);
    mArticle->mPotentialVector[2]   = mReseatPressure - 0.005;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected05, mArticle->mPosition, mTolerance);
    CPPUNIT_ASSERT(GunnsFluidValve::CLOSING == mArticle->mState);

    /// @test    Decrement to 1.25 full open pressure so valve is opening.
    const double expected125 = mArticle->mNomDecB + mArticle->mNomDecM * 1.25 * mFullOpenPressure;
    mArticle->mPotentialVector[2]   = 1.25 * mFullOpenPressure;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected125, mArticle->mPosition, mTolerance);
    CPPUNIT_ASSERT(GunnsFluidValve::OPENING == mArticle->mState);

    /// @test    Increment to 1.255 full open pressure so valve is transitioning.
    mArticle->mPotentialVector[2]   = 1.255 * mFullOpenPressure;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected125, mArticle->mPosition, mTolerance);
    CPPUNIT_ASSERT(GunnsFluidValve::TRANSITIONING == mArticle->mState);

    /// @test    Decrement to 1.2525 full open pressure so valve is transitioning.
    mArticle->mPotentialVector[2]   = 1.2525 * mFullOpenPressure;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected125, mArticle->mPosition, mTolerance);
    CPPUNIT_ASSERT(GunnsFluidValve::TRANSITIONING == mArticle->mState);

    /// @test    Increment to 1.75 full open pressure so valve is closing.
    const double expected175 = mArticle->mNomIncB + mArticle->mNomIncM * 1.75 * mFullOpenPressure;
    mArticle->mPotentialVector[2]   = 1.750 * mFullOpenPressure;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected175, mArticle->mPosition, mTolerance);
    CPPUNIT_ASSERT(GunnsFluidValve::CLOSING == mArticle->mState);

    /// @test    Decrement to 1.25 full open pressure so valve is opening.
    mArticle->mPotentialVector[2]   = 1.25 * mFullOpenPressure;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected125, mArticle->mPosition, mTolerance);
    CPPUNIT_ASSERT(GunnsFluidValve::OPENING == mArticle->mState);

    /// @test    Increment to reseat pressure so valve closes.
    mArticle->mPotentialVector[2]   = mReseatPressure;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mPosition, mTolerance);
    CPPUNIT_ASSERT(GunnsFluidValve::CLOSING == mArticle->mState);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Pressure Regulator Valve link model update state (rate limiting).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidRegulatorValve::testUpdateStateRateLimited()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3);

    /// - Set node 3 to vacuum
    mArticle->mPotentialVector[3]   = 0.0;

    /// @test    Rate limiting with closed valve and rate limit at half pop position at pop
    ///          pressure.
    mArticle->mRateLimit = 0.5 * mPopPosition;
    mArticle->mPosition  = 0.0;
    mArticle->mPotentialVector[2]   = (mArticle->mPopPosition - mArticle->mPopDecB) * mArticle->mPopDecMInv;
    mArticle->step(mTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mRateLimit * mTimeStep,
                                 mArticle->mPosition,
                                 mTolerance);

    /// @test    Rate limiting with closing valve, rate limit at half and pressure well above
    ///          reseat.
    mArticle->mRateLimit = 0.5;
    mArticle->mPosition  = 1.0;
    mArticle->mState     = GunnsFluidValve::CLOSING;
    mArticle->mPotentialVector[2]   = 2.0 * mReseatPressure;
    mArticle->step(mTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0 - mArticle->mRateLimit * mTimeStep,
                                 mArticle->mPosition,
                                 mTolerance);
    CPPUNIT_ASSERT(mArticle->mState = GunnsFluidValve::CLOSING);

    /// @test    Rate limiting with opening valve, rate limit at half and pressure well below full
    ///          open.
    mArticle->mRateLimit = 0.5;
    mArticle->mPosition  = 0.0;
    mArticle->mState     = GunnsFluidValve::OPENING;
    mArticle->mPotentialVector[2]   = 0.5 * mFullOpenPressure;
    mArticle->step(mTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mRateLimit * mTimeStep,
                                 mArticle->mPosition,
                                 mTolerance);
    CPPUNIT_ASSERT(mArticle->mState = GunnsFluidValve::OPENING);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Pressure Regulator Valve link model update state (reverse flow).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidRegulatorValve::testUpdateStateReverse()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3);

    /// @test    Valve closed on reverse flow
    mArticle->mRateLimit = 0.5 * mPopPosition;
    mArticle->mPosition  = 0.0;
    mArticle->mPotentialVector[2]   = 120.0;
    mArticle->mPotentialVector[3]   = 110.0;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mPosition, mTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Pressure Regulator Valve link model update state (malfunction).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidRegulatorValve::testUpdateStateMalfunction()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3);

    /// - Set closed position.
    mArticle->mPosition = 0.0;
    mArticle->mState    = GunnsFluidValve::CLOSED;

    /// - Set node 1 pressure > node 0 pressure
    mArticle->mNodes[1]->getOutflow()->setPressure(mArticle->mNodes[0]->getOutflow()->getPressure() + 0.05);

    const double leakRate         = 1.0;
    const double leakConductivity = GunnsFluidUtils::predictConductivity(leakRate,
                                                                         mArticle->mMinLinearizationPotential,
                                                                         mArticle->mNodes[0]->getOutflow(),
                                                                         mArticle->mNodes[1]->getOutflow());

    /// @test    Update state with initial leak rate malfunction insertion.
    mArticle->mMalfLeakThruFlag  = true;
    mArticle->mMalfLeakThruValue = leakRate;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(leakRate,
                                 mArticle->mMalfLeakThruValue,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(leakRate,
                                 mArticle->mPreviousLeakRate,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(leakConductivity,
                                 mArticle->mLeakConductivity,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(leakConductivity,
                                 mArticle->mEffectiveConductivity,
                                 mTolerance);

    /// @test    Update state with initial Failure to update flag
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3);
    mArticle->mMalfStuckFlag  = false;
    mArticle->setMalfFailTo(true,0.6);
    mArticle->updateState(mTimeStep);
    
    CPPUNIT_ASSERT(mArticle->mPosition == 0.6);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Pressure Regulator Valve link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidRegulatorValve::testInitializationExceptions()
{
    UT_RESULT;

    /// - Default construct a test article.
    GunnsFluidRegulatorValve article;

    /// @test    Initialization exception on invalid config data: no name.
    mConfigData->mName = "";
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3), TsInitializationException);
    mConfigData->mName = mName;

    /// @test    Initialization exception on invalid config data: max conductivity < 0.
    mConfigData->mMaxConductivity = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3), TsInitializationException);
    mConfigData->mMaxConductivity = mMaxConductivity;

    /// @test    Initialization exception on invalid config data: expansion scale factor < 0.
    mConfigData->mExpansionScaleFactor = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3), TsInitializationException);
    mConfigData->mExpansionScaleFactor = mExpansionScaleFactor;

    /// @test    Initialization exception on invalid config data: mRateLimit < 0.
    mConfigData->mRateLimit  = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3), TsInitializationException);
    mConfigData->mRateLimit  = mRateLimit;

    /// @test    Initialization exception on invalid config data: mFullOpenPressure < 0.
    mConfigData->mFullOpenPressure  = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3), TsInitializationException);
    mConfigData->mFullOpenPressure  = mFullOpenPressure;

    /// @test     Initialization exception on invalid config data:
    ///           mCrackPressure >= mReseatPressure.
    mConfigData->mCrackPressure  = mReseatPressure;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3), TsInitializationException);
    mConfigData->mCrackPressure  = mCrackPressure;

    /// @test    Initialization exception on invalid config data:
    ///          mFullOpenPressure >= mCrackPressure.
    mConfigData->mFullOpenPressure  = mCrackPressure;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3), TsInitializationException);
    mConfigData->mFullOpenPressure  = mFullOpenPressure;

    /// @test    Initialization exception on invalid config data: pop position <= 0.
    mConfigData->mPopPosition  = 0.00;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3), TsInitializationException);
    mConfigData->mPopPosition  = mPopPosition;

    /// @test    Initialization exception on invalid config data: pop position >= 1.
    mConfigData->mPopPosition  = 1.00;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3), TsInitializationException);
    mConfigData->mPopPosition  = mPopPosition;

    /// @test    Initialization exception on invalid config data: pop slope scale factor < 1.
    mConfigData->mPopSlopeScale  = 0.99;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3), TsInitializationException);
    mConfigData->mPopSlopeScale  = mPopSlopeScale;

    /// @test    Initialization exception on invalid input data: mMalfBlockageValue < 0.
    mInputData->mMalfBlockageValue = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3), TsInitializationException);
    mInputData->mMalfBlockageValue = mMalfBlockageValue;

    /// @test    Initialization exception on invalid input data: mMalfBlockageValue > 1.
    mInputData->mMalfBlockageValue = 1.0 + FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3), TsInitializationException);
    mInputData->mMalfBlockageValue = mMalfBlockageValue;

    /// @test    Initialization exception on invalid input data: mPosition < 0.
    mInputData->mPosition = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3), TsInitializationException);
    mInputData->mPosition = mPosition;

    /// @test    Initialization exception on invalid input data: mPosition > 1.
    mInputData->mPosition = 1.0 + FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3), TsInitializationException);
    mInputData->mPosition = mPosition;

    /// @test    Initialization exception on invalid input data: mMalfLeakThruValue < 0.
    mInputData->mMalfLeakThruValue = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3), TsInitializationException);
    mInputData->mMalfLeakThruValue = mMalfLeakThruValue;

    /// @test    Initialization exception on invalid input data: mWallTemperature < 0.
    mInputData->mWallTemperature = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3), TsInitializationException);
    mInputData->mWallTemperature = mWallTemperature;

    /// @test    Initialization exception on invalid input data: mMalfFailToValue < 0.
    mInputData->mMalfFailToValue = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3),
                         TsInitializationException);
    mInputData->mMalfFailToValue = mMalfFailToValue;

    /// @test    Initialization exception on invalid input data: mMalfFailToValue > 1.
    mInputData->mMalfFailToValue = 1.0 + FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3),
                         TsInitializationException);
    mInputData->mMalfFailToValue = mMalfFailToValue;

    /// @test    Initialization exception on invalid arguments: duplicate nodes on ports 0 & 1
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort0, mPort2, mPort3), TsInitializationException);

    /// @test    Initialization exception on invalid arguments: duplicate nodes on ports 0 & 1
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort1, mPort1, mPort2, mPort3), TsInitializationException);

    /// @test    Initialization exception on invalid arguments: duplicate nodes on ports 2 & 3
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort2), TsInitializationException);

    /// @test    Initialization exception on invalid arguments: duplicate nodes on ports 2 & 3
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort3, mPort3), TsInitializationException);

    UT_PASS_LAST;
}
