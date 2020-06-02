/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    (
        (aspects/fluid/conductor/GunnsFluidPressureSensitiveValve.o)
    )
***************************************************************************************************/
// @reqt method R.TS222-0063 TS21 ECLSS models shall perform pressure regulator control.
// @reqt method R.TS222-0068 TS21 ECLSS models shall provide malfunctions to freeze valves at non-target positions.
// @reqt method R.TS222-0082 TS21 ECLSS models shall simulate the functionality of valves.
// @reqt method R.TS222-0083 TS21 ECLSS models shall malfunction valves.
// @reqt method R.TS228-0001 TS21 thermal models shall simulate the functionality of valves.
// @reqt method R.TS228-0029 TS21 thermal models shall provide valve malfunctions.
// @reqt method R.TS228-0032 TS21 thermal models shall provide pressure regulator malfunctions.

#include "core/GunnsFluidUtils.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidPressureSensitiveValve.hh"

/// @details  Test identification number.
int UtGunnsFluidPressureSensitiveValve::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsFluidPressureSensitiveValve class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidPressureSensitiveValve::UtGunnsFluidPressureSensitiveValve()
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
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsFluidPressureSensitiveValve class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidPressureSensitiveValve::~UtGunnsFluidPressureSensitiveValve()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPressureSensitiveValve::setUp()
{
    /// - Define the nominal port fluids.
    mFluidProperties       = new DefinedFluidProperties();
    mTypes[0]              = FluidProperties::GUNNS_N2;
    mTypes[1]              = FluidProperties::GUNNS_O2;
    mFractions[0]          = 0.5;
    mFractions[1]          = 0.5;
    mFluidConfig           = new PolyFluidConfigData(mFluidProperties, mTypes, N_FLUIDS);
    mFluidInput0           = new PolyFluidInputData(290.0,
                                                    700.728,
                                                      0.0,
                                                      0.0,
                                                    mFractions);
    mFluidInput1           = new PolyFluidInputData(283.15,
                                                    689.475,
                                                      0.0,
                                                      0.0,
                                                    mFractions);
    mFluidInput2           = new PolyFluidInputData(283.15,
                                                    701.0,
                                                      0.0,
                                                      0.0,
                                                    mFractions);
    mFluidInput3           = new PolyFluidInputData(283.15,
                                                    699.0,
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
    mConfigData            = new GunnsFluidPressureSensitiveValveConfigData(mName,
                                                                            &mNodeList,
                                                                            mMaxConductivity,
                                                                            mExpansionScaleFactor,
                                                                            mRateLimit,
                                                                            mThermalLength,
                                                                            mThermalDiameter,
                                                                            mSurfaceRoughness);

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
    mInputData             = new GunnsFluidPressureSensitiveValveInputData(mMalfBlockageFlag,
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
    mArticle               = new FriendlyGunnsFluidPressureSensitiveValve;

    /// - Define the nominal initial state data
    mPreviousLeakRate      = 0.0;
    mLeakConductivity      = 0.0;
    mTuneMode              = GunnsFluidUtils::OFF;
    mTuneMassFlow          = 0.0;
    mTuneVolFlow           = 0.0;
    mTuneDeltaT            = 0.0;
    mEffectiveConductivity = mMaxConductivity;
    mSystemConductance     = 0.0;
    mLastSystemConductance = 0.0;
    mControlPressure       = 0.0;
    mWallHeatFlux          = 0.0;

    /// - Define the nominal time step and comparison tolerance.
    mTimeStep              = 0.1;
    mTolerance             = 0.01;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPressureSensitiveValve::tearDown()
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
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPressureSensitiveValve::testConfigAndInput()
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
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfFailToValue,                    mInputData->mMalfFailToValue,     0.0);

    /// @test    Configuration data default construction.
    GunnsFluidPressureSensitiveValveConfigData defaultConfig;
    CPPUNIT_ASSERT(0                                              == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0                                              == defaultConfig.mNodeList);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mMaxConductivity,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mExpansionScaleFactor, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mRateLimit,            0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mThermalLength,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mThermalDiameter,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mSurfaceRoughness,     0.0);

    /// @test    Input data default construction.
    GunnsFluidPressureSensitiveValveInputData   defaultInput;
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
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultInput.mMalfFailToValue,0.0);

    /// @test    Configuration data copy construction.
    GunnsFluidPressureSensitiveValveConfigData copyConfig(*mConfigData);
    CPPUNIT_ASSERT(mConfigData->mName                             == copyConfig.mName);
    CPPUNIT_ASSERT(mConfigData->mNodeList->mNodes                 == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mMaxConductivity,      copyConfig.mMaxConductivity,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mExpansionScaleFactor, copyConfig.mExpansionScaleFactor,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mRateLimit,            copyConfig.mRateLimit,               0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mThermalLength,        copyConfig.mThermalLength,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mThermalDiameter,      copyConfig.mThermalDiameter,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mSurfaceRoughness,     copyConfig.mSurfaceRoughness,        0.0);

    /// @test    Input data copy construction.
    GunnsFluidPressureSensitiveValveInputData   copyInput(*mInputData);
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
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfFailToValue,       copyInput.mMalfFailToValue,   0.0);

    UT_PASS;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPressureSensitiveValve::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default construction configuration data.
    CPPUNIT_ASSERT(""                   == mArticle->mName);
    CPPUNIT_ASSERT(0                    == mArticle->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,      mArticle->mMaxConductivity,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,      mArticle->mExpansionScaleFactor,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,      mArticle->mRateLimit,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,      mArticle->mThermalDiameter,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,      mArticle->mThermalSurfaceArea,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,      mArticle->mThermalROverD,         0.0);

    /// @test    Default construction input data.
    CPPUNIT_ASSERT(                       !mArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,      mArticle->mMalfBlockageValue,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,      mArticle->mPosition,              0.0);
    CPPUNIT_ASSERT(                       !mArticle->mMalfLeakThruFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,      mArticle->mMalfLeakThruValue,     0.0);
    CPPUNIT_ASSERT(                       !mArticle->mMalfPressureBiasFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,      mArticle->mMalfPressureBiasValue, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,      mArticle->mSetPointPressureBias,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,      mArticle->mWallTemperature,       0.0);
    CPPUNIT_ASSERT(                       !mArticle->mMalfStuckFlag);
    CPPUNIT_ASSERT(                       !mArticle->mMalfFailToFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,      mArticle->mMalfFailToValue,0.0);

    /// @test    Default construction state data.
    CPPUNIT_ASSERT(0                    == mArticle->mInternalFluid);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,      mArticle->mPreviousLeakRate,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,      mArticle->mLeakConductivity,      0.0);
    CPPUNIT_ASSERT(GunnsFluidUtils::OFF == mArticle->mTuneMode);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,      mArticle->mTuneMassFlow,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,      mArticle->mTuneVolFlow,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,      mArticle->mTuneDeltaT,            0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,      mArticle->mEffectiveConductivity, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,      mArticle->mSystemConductance,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,      mArticle->mControlPressure,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,      mArticle->mWallHeatFlux,          0.0);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(!mArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsFluidPressureSensitiveValve* article = new GunnsFluidPressureSensitiveValve();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPressureSensitiveValve::testNominalInitialization()
{
    UT_RESULT;

    /// - Default construct and initialize (with nominal data) a test article
    FriendlyGunnsFluidPressureSensitiveValve article;
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
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfFailToValue,       article.mMalfFailToValue,0.0);

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
    CPPUNIT_ASSERT(                                      article.mInitFlag);

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

    /// @test   Catch case if mThermalDiameter is zero
    mConfigData->mThermalDiameter = 0.0;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3);

    CPPUNIT_ASSERT(mArticle->mThermalROverD == 0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Valve link model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPressureSensitiveValve::testAccessors()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3);

    {
        /// @test    For more than nominal full open position.
        const double expected = 2.00;
        mArticle->mPosition = expected;
        const double returned = mArticle->getPosition();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 0.0);
    } {
        /// @test    For nominal full open position.
        const double expected = 1.00;
        mArticle->mPosition = expected;
        const double returned = mArticle->getPosition();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 0.0);
    } {
        /// @test    For intermediate position.
        const double expected = 0.50;
        mArticle->mPosition = expected;
        const double returned = mArticle->getPosition();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 0.0);
    } {
        /// @test    For nominal full closed position.
        const double expected = 0.00;
        mArticle->mPosition = expected;
        const double returned = mArticle->getPosition();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 0.0);
    } {
        /// @test    For less than nominal full closed position.
        const double expected = -1.00;
        mArticle->mPosition = expected;
        const double returned = mArticle->getPosition();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 0.0);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Valve link model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPressureSensitiveValve::testModifiers()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3);

    {
        /// @test    For more than nominal full open position.
        const double position = 2.00;
        mArticle->setPosition(position);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(position, mArticle->mPosition, 0.0);
    } {
        /// @test    For nominal full open position.
        const double position = 1.00;
        mArticle->setPosition(position);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(position, mArticle->mPosition, 0.0);
    } {
        /// @test    For intermediate position.
        const double position = 0.50;
        mArticle->setPosition(position);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(position, mArticle->mPosition, 0.0);
    } {
        /// @test    For nominal full closed position.
        const double position = 0.00;
        mArticle->setPosition(position);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(position, mArticle->mPosition, 0.0);
    } {
        /// @test    For less than nominal full closed position.
        const double position = -1.00;
        mArticle->setPosition(position);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(position, mArticle->mPosition, 0.0);
    } {
        /// @test    The initial leak through rate malfunction parameters are set.
        mArticle->setMalfLeakThru(true, 1.0);
        CPPUNIT_ASSERT(true  == mArticle->mMalfLeakThruFlag);
        CPPUNIT_ASSERT(1.0   == mArticle->mMalfLeakThruValue);
    } {
        /// @test    The initial leak through rate malfunction parameters are reset.
        mArticle->setMalfLeakThru();
        CPPUNIT_ASSERT(false == mArticle->mMalfLeakThruFlag);
        CPPUNIT_ASSERT(0.0   == mArticle->mMalfLeakThruValue);
    } {
        /// @test    The control pressure bias malfunction parameters are set.
        mArticle->setMalfPressureBias(true, 1.0);
        CPPUNIT_ASSERT(true  == mArticle->mMalfPressureBiasFlag);
        CPPUNIT_ASSERT(1.0   == mArticle->mMalfPressureBiasValue);
    } {
        /// @test    The control pressure bias malfunction parameters are reset.
        mArticle->setMalfPressureBias();
        CPPUNIT_ASSERT(false == mArticle->mMalfPressureBiasFlag);
        CPPUNIT_ASSERT(0.0   == mArticle->mMalfPressureBiasValue);
    } {
        /// @test    The stuck at current position malfunction parameters are set.
        mArticle->setMalfStuck(true);
        CPPUNIT_ASSERT(true  == mArticle->mMalfStuckFlag);
    } {
        /// @test    The stuck at current position malfunction parameters are reset.
        mArticle->setMalfStuck();
        CPPUNIT_ASSERT(false == mArticle->mMalfStuckFlag);
    } {
        /// @test    The fail to position malfunction parameters are set.
        mArticle->setMalfFailTo(true, 1.0);
        CPPUNIT_ASSERT(true  == mArticle->mMalfFailToFlag);
        CPPUNIT_ASSERT(1.0   == mArticle->mMalfFailToValue);
    } {
        /// @test    The fail to position malfunction parameters are reset.
        mArticle->setMalfFailTo();
        CPPUNIT_ASSERT(false == mArticle->mMalfFailToFlag);
        CPPUNIT_ASSERT(0.0   == mArticle->mMalfFailToValue);
    } {
        /// @test    The thermal surface area setter with good value.
        const double surfaceArea = 0.1;
        mArticle->setThermalSurfaceArea(surfaceArea);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(surfaceArea, mArticle->mThermalSurfaceArea, 0.0);
    } {
        /// @test    The thermal surface area setter with out of range value.
        const double surfaceArea = -0.1;
        mArticle->setThermalSurfaceArea(surfaceArea);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mThermalSurfaceArea, 0.0);
    } {
        /// @test    The wall temperature setter with good value.
        const double wallTemperature = 280.0;
        mArticle->setWallTemperature(wallTemperature);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(wallTemperature, mArticle->mWallTemperature, 0.0);
    } {
        /// @test    The wall temperature setter with out of range value.
        const double wallTemperature = -0.1;
        mArticle->setWallTemperature(wallTemperature);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mWallTemperature, 0.0);
    }

    UT_PASS;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the step method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPressureSensitiveValve::testStep()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3);

    mArticle->mPotentialVector[2]   = mNodes[2].getOutflow()->getPressure();
    mArticle->mPotentialVector[3]   = mNodes[3].getOutflow()->getPressure();
    const double expected = mNodes[2].getOutflow()->getPressure() - mNodes[3].getOutflow()->getPressure();
    mArticle->step(mTimeStep);

    /// @test    Positive admittance and zero potential.
    CPPUNIT_ASSERT(mArticle->mAdmittanceMatrix[0] > 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mSourceVector[0], mTolerance);

    /// @test    Control pressure.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->mControlPressure, mTolerance);

    mArticle->step(mTimeStep);

    /// @test    Control pressure with pressure ports swapped.
    mArticle->mUserPortSetControl = GunnsBasicLink::EXECUTE;
    mArticle->mUserPortSelect = 2;
    mArticle->mUserPortSelectNode = 3;
    mArticle->step(mTimeStep);
    mArticle->mUserPortSetControl = GunnsBasicLink::EXECUTE;
    mArticle->mUserPortSelect = 3;
    mArticle->mUserPortSelectNode = 2;
    mArticle->mPotentialVector[2]   = mNodes[3].getOutflow()->getPressure();
    mArticle->mPotentialVector[3]   = mNodes[2].getOutflow()->getPressure();
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expected, mArticle->mControlPressure, mTolerance);

    /// @test     Step with pressure bias malfunction insertion.
    double expectedP = mArticle->mControlPressure + 1.0;
    mArticle->mMalfPressureBiasFlag  = true;
    mArticle->mMalfPressureBiasValue = 1.0;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP,
                                 mArticle->mControlPressure,
                                 mTolerance);

    /// @test     Step with set point pressure bias.
    expectedP = mArticle->mControlPressure - 1.0;
    mArticle->mSetPointPressureBias = 1.0;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP,
                                 mArticle->mControlPressure,
                                 mTolerance);

    /// @test     Step with blockage malfunction insertion.
    mArticle->mMalfBlockageFlag  = true;
    mArticle->mMalfBlockageValue = 1.0;
    mArticle->mPosition          = 1.0;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mEffectiveConductivity,
                                 mTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPressureSensitiveValve::testComputeFlows()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3);

    mArticle->step(mTimeStep);
    mArticle->computeFlows(mTimeStep);

    /// - Confirm correct null port allocation with zero potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == mArticle->mPortDirections[0]);
    
    /// - Confirm correct null port allocation with zero potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == mArticle->mPortDirections[1]);

    /// - Confirm correct null port allocation with zero potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == mArticle->mPortDirections[2]);
    
    /// - Confirm correct null port allocation with zero potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == mArticle->mPortDirections[3]);

    mArticle->transportFlows(mTimeStep);

    /// - Conductance Link should have zero flow rate because the potential vector is zero
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mFlowRate, mTolerance);

    /// - Confirm mVolFlowRate is zero with a zero potential vector (TransportFlows)
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mVolFlowRate, mTolerance);

    mArticle->mPotentialVector[0] = 0.8;
    mArticle->computeFlows(mTimeStep);

    /// - Confirm correct source port allocation with postive potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == mArticle->mPortDirections[0]);
    
    /// - Confirm correct Sink port allocation with postive potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::SINK == mArticle->mPortDirections[1]);

    /// - Confirm correct null port allocation with postive potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == mArticle->mPortDirections[2]);
    
    /// - Confirm correct null port allocation with postive potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == mArticle->mPortDirections[3]);

    /// - Nodal outflux scheduling should be equal to step molar flux for source node
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNodes[0].getScheduledOutflux(),mArticle->mFlux,DBL_EPSILON);

    mArticle->transportFlows(mTimeStep);

    /// - Confirm correct source port selection with Postive potential Vector (TransportFlows)
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, mArticle->determineSourcePort(mArticle->mFlux, 0, 1), DBL_EPSILON);
    
    /// - Conductance Link should have positive flow rate because the port 0 potential vector is positive
    CPPUNIT_ASSERT(mArticle->mFlowRate > 0.0);

    mArticle->mPotentialVector[0] = -0.6;
    mArticle->computeFlows(mTimeStep);

    /// - Confirm correct sink port allocation with negative potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::SINK == mArticle->mPortDirections[0]);
    
    /// - Confirm correct source port allocation with negative potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == mArticle->mPortDirections[1]);

    /// - Confirm correct null port allocation with negative potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == mArticle->mPortDirections[2]);
    
    /// - Confirm correct null port allocation with negative potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == mArticle->mPortDirections[3]);

    /// - Nodal outflux scheduling should be equal to step molar flux for source node
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNodes[1].getScheduledOutflux(), -mArticle->mFlux,DBL_EPSILON);
    
    mArticle->transportFlows(mTimeStep);

    /// - Confirm correct source port selection with negative potential Vector (TransportFlows)
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1, mArticle->determineSourcePort(mArticle->mFlux, 0, 1), DBL_EPSILON);
    
    /// - Conductance Link should have negative flow rate because the port 0 potential vector is negative
    CPPUNIT_ASSERT(mArticle->mFlowRate < 0.0);

    /// - Verify volumetric flow rate and hydraulic power
    const double expectedVolFlow = mArticle->mFlowRate / mNodes[1].getOutflow()->getDensity();
    const double expectedPower = -1000.0 * expectedVolFlow *
            (mArticle->mPotentialVector[0] - mArticle->mPotentialVector[1]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVolFlow, mArticle->mVolFlowRate, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower,   mArticle->mPower,       DBL_EPSILON);

    /// - Verify volumetric flow rate when the source density is zero
    mNodes[1].resetContentState();
    mNodes[1].resetFlows();
    mArticle->computeFlows(mTimeStep);
    mArticle->transportFlows(mTimeStep);

    CPPUNIT_ASSERT(0.0 == mArticle->mVolFlowRate);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPressureSensitiveValve::testComputeFlowsWithInternalFluid()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3);

    mArticle->createInternalFluid();
    mArticle->step(mTimeStep);
    mArticle->computeFlows(mTimeStep);
    mArticle->transportFlows(mTimeStep);

    /// - Conductance Link should have zero flow rate because the potential vector is zero
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mFlowRate, mTolerance);

    mArticle->mPotentialVector[0] = 0.5;
    mArticle->mPotentialVector[1] = 0.0;
    mArticle->computeFlows(mTimeStep);
    mNodes[0].getContent()->setTemperature(40.0);
    mNodes[0].getContent()->setPressure(500.0);
    mNodes[0].resetFlows();
    mNodes[0].getContent()->setTemperature(290.0);
    mNodes[0].getContent()->setPressure(700.728);

    /// - Check to ensure that content and outflow conditions are different
    CPPUNIT_ASSERT(mNodes[0].getOutflow()->getPressure()!= mNodes[0].getContent()->getPressure());
    mArticle->mExpansionScaleFactor = 0.0;
    mArticle->transportFlows(mTimeStep);

    mArticle->mInternalFluid->setTemperature(mNodes[0].getOutflow()->getTemperature());
    double tempWallHeatFlux = GunnsFluidUtils::computeConvectiveHeatFlux(mArticle->mInternalFluid,
                                                                   mArticle->mFlowRate,
                                                                   mArticle->mThermalROverD,
                                                                   mArticle->mThermalDiameter,
                                                                   mArticle->mThermalSurfaceArea,
                                                                   mArticle->mWallTemperature);
 
    /// - Internal Fluid should be at same pressure as outflow with zero Expansion Scale Factor
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNodes[0].getOutflow()->getPressure(),
            mArticle->getInternalFluid()->getPressure(),mTolerance);

    /// - Internal Fluid should be at same temperature as outflow with zero Expansion Scale Factor
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tempWallHeatFlux, mArticle->mWallHeatFlux, 0.01); 
   
    mNodes[0].resetFlows();
    mNodes[1].resetFlows();

    /// - Conductance Link should have positive flow rate because the port 0 potential vector is positive
    CPPUNIT_ASSERT(mArticle->mFlowRate > 0.0);

    mArticle->mPotentialVector[0] = -0.6;
    mArticle->computeFlows(mTimeStep);
    mNodes[1].getContent()->setTemperature(20.0);
    mNodes[1].getContent()->setPressure(20.0);
    mNodes[1].resetFlows();
    mNodes[1].getContent()->setTemperature(283.15);
    mNodes[1].getContent()->setPressure(689.475);

    /// - Check to ensure that content and outflow conditions are different
    CPPUNIT_ASSERT(mNodes[1].getOutflow()->getPressure() != 
                                                 mNodes[1].getContent()-> getPressure());

    mArticle->transportFlows(mTimeStep);

    mArticle->mInternalFluid->setTemperature(mNodes[1].getOutflow()->getTemperature());
    tempWallHeatFlux = GunnsFluidUtils::computeConvectiveHeatFlux(mArticle->mInternalFluid,
                                                                   mArticle->mFlowRate,
                                                                   mArticle->mThermalROverD,
                                                                   mArticle->mThermalDiameter,
                                                                   mArticle->mThermalSurfaceArea,
                                                                   mArticle->mWallTemperature);
 
    /// - Internal Fluid should be at same pressure as outflow with zero Expansion Scale Factor
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNodes[1].getOutflow()->getPressure(),mArticle->getInternalFluid()->
                                                 getPressure(),mTolerance);

    /// - Internal Fluid should be at same temperature as outflow with zero Expansion Scale Factor
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tempWallHeatFlux, mArticle->mWallHeatFlux, 0.01); 
   
    mNodes[0].resetFlows();
    mNodes[1].resetFlows();

    /// - Conductance Link should have negative flow rate because the port 0 potential vector is negative
    CPPUNIT_ASSERT(mArticle->mFlowRate < 0.0);

    /// - Test a case where the inlet node is vacuum on port 0 and verify it does not affect the
    ///   internal fluid  //TODO from GunnsFluidLink
//    mArticle->mUserPortSelect     = 0;
//    mArticle->mUserPortSelectNode = 1;
//    mArticle->mUserPortSetControl = GunnsBasicLink::EXECUTE;
//    mArticle->step(mTimeStep);
//
//    mArticle->mUserPortSelect     = 1;
//    mArticle->mUserPortSelectNode = 0;
//    mArticle->mUserPortSetControl = GunnsBasicLink::EXECUTE;
//    mArticle->step(mTimeStep);
//
//    mArticle->mPotentialVector[0] =  0.0;
//    mArticle->mPotentialVector[1] = -0.6;
//    mArticle->computeFlows(mTimeStep);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test tuning the link conductivity and expansion scale factor.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPressureSensitiveValve::testTuning()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3);
    
    mNodes[0].setPotential(mNodes[0].getOutflow()->getPressure());
    mNodes[1].setPotential(mNodes[1].getOutflow()->getPressure());
    mArticle->mPotentialVector[0] = mNodes[0].getPotential();
    mArticle->mPotentialVector[1] = mNodes[1].getPotential();

    double newT = GunnsFluidUtils::computeIsentropicTemperature(0.4, mNodes[0].getPotential(),
                                                                     mNodes[1].getPotential(),
                                                                     mNodes[0].getContent());
    mNodes[1].getContent()->setTemperature(newT);

    /// - Step once to get the default mdot and dT
    mArticle->step(mTimeStep);
    mArticle->computeFlows(mTimeStep);
    mArticle->transportFlows(mTimeStep);

    double defaultConductivity = mArticle->mEffectiveConductivity;
    double defaultScaleFactor  = mArticle->mExpansionScaleFactor;

    /// - Set the controls to tune the conductivity and expansion scale factor to half their default
    ///   values.

    /// - Tune the link to half its default mass flow rate.
    const double defaultMdot = mArticle->mFlowRate;
    mArticle->mTuneMassFlow  = fabs(0.5 * defaultMdot);
    mArticle->mTuneMode      = GunnsFluidUtils::MASS;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT(GunnsFluidUtils::OFF == mArticle->mTuneMode);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5 * defaultConductivity, mArticle->mEffectiveConductivity,
                                                            FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5 * defaultConductivity, mArticle->mMaxConductivity,
                                                            FLT_EPSILON);

    /// - Tune the link to double its default true volumetric flow rate for positive flow.
    double defaultVdot     = mArticle->mFlowRate / mNodes[0].getOutflow()->getDensity();
    mArticle->mTuneVolFlow = 2.0 * defaultVdot;
    mArticle->mTuneMode    = GunnsFluidUtils::TRUE_VOL;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT(GunnsFluidUtils::OFF == mArticle->mTuneMode);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0 * defaultConductivity, mArticle->mEffectiveConductivity,
                                                            FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0 * defaultConductivity, mArticle->mMaxConductivity,
                                                            FLT_EPSILON);

    /// - Set up the nodes to create backflow and re-adjust the link to the new pressures.
    mNodes[0].setPotential(675.0);
    mArticle->mPotentialVector[0] = 675.0;
    mNodes[0].resetFlows();
    mNodes[1].resetFlows();
    mArticle->mMaxConductivity = mMaxConductivity;
    mArticle->step(mTimeStep);
    mArticle->computeFlows(mTimeStep);
    mArticle->transportFlows(mTimeStep);

    /// - Tune the link to half its default true volumetric flow rate for negative flow.
    defaultVdot            = mArticle->mFlowRate / mNodes[1].getOutflow()->getDensity();
    mArticle->mTuneVolFlow = 0.5 * defaultVdot;
    mArticle->mTuneMode    = GunnsFluidUtils::TRUE_VOL;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT(GunnsFluidUtils::OFF == mArticle->mTuneMode);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5 * defaultConductivity, mArticle->mEffectiveConductivity,
                                                            FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5 * defaultConductivity, mArticle->mMaxConductivity,
                                                            FLT_EPSILON);

    /// - Restore the nodes to their original state and re-adjust the link again.
    mNodes[0].setPotential(700.728);
    mArticle->mPotentialVector[0] = 700.728;
    mArticle->mMaxConductivity = mMaxConductivity;
    mNodes[0].resetFlows();
    mNodes[1].resetFlows();
    mArticle->step(mTimeStep);
    mArticle->computeFlows(mTimeStep);
    mArticle->transportFlows(mTimeStep);

    /// - Tune the link to double its default standard volumetric flow rate for positive flow, using
    ///   1.2041 kg/m3 for the standard air density.
    const double defaultVdotStd = mArticle->mFlowRate / 1.2041;
    mArticle->mTuneVolFlow      = 2.0 * defaultVdotStd;
    mArticle->mTuneMode         = GunnsFluidUtils::STD_VOL;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT(GunnsFluidUtils::OFF == mArticle->mTuneMode);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0 * defaultConductivity, mArticle->mEffectiveConductivity,
                                                            FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0 * defaultConductivity, mArticle->mMaxConductivity,
                                                            FLT_EPSILON);

    /// - Tune the link to half its default expansion scale factor.
    const double defaultdT = mArticle->mNodes[1]->getOutflow()->getTemperature() -
                             mArticle->mNodes[0]->getOutflow()->getTemperature();

    mArticle->mTuneDeltaT  = 0.5 * defaultdT;
    mArticle->mTuneMode    = GunnsFluidUtils::DELTA_T;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT(GunnsFluidUtils::OFF == mArticle->mTuneMode);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5 * defaultScaleFactor,  mArticle->mExpansionScaleFactor,
                                                            FLT_EPSILON);

    const double expected    = mArticle->mEffectiveConductivity;

    mArticle->mTuneMode      = GunnsFluidUtils::OFF;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT(GunnsFluidUtils::OFF == mArticle->mTuneMode);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->mEffectiveConductivity,
                                                            FLT_EPSILON);

    mArticle->mTuneMode      = static_cast<GunnsFluidUtils::TuningMode>(-1);
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT(GunnsFluidUtils::OFF == mArticle->mTuneMode);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->mEffectiveConductivity,
                                                            FLT_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Valve link model update state nominal.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPressureSensitiveValve::testUpdateStateNominal()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3);

    {
        /// @test    For effective conductivity at more than nominal full open position.
        const double position = 2.00;
        mArticle->setPosition(position);
        mArticle->updateState(mTimeStep);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity * position,
                                     mArticle->mEffectiveConductivity,
                                     mTolerance);
    } {
        /// @test    For effective conductivity at nominal full open position.
        const double position = 1.00;
        mArticle->setPosition(position);
        mArticle->updateState(mTimeStep);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity * position,
                                     mArticle->mEffectiveConductivity,
                                     mTolerance);
    } {
        /// @test    For effective conductivity at intermediate position.
        const double position = 0.50;
        mArticle->setPosition(position);
        mArticle->updateState(mTimeStep);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity * position,
                                     mArticle->mEffectiveConductivity,
                                     mTolerance);
    } {
        /// @test    For effective conductivity at nominal full closed position.
        const double position = 0.00;
        mArticle->setPosition(position);
        mArticle->updateState(mTimeStep);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity * position,
                                     mArticle->mEffectiveConductivity,
                                     mTolerance);
    } {
        /// @test    For effective conductivity at less than nominal full closed position.
        const double position = -1.00;
        mArticle->setPosition(position);
        mArticle->updateState(mTimeStep);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mLeakConductivity,
                                     mArticle->mEffectiveConductivity,
                                     mTolerance);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Valve link model update state with malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPressureSensitiveValve::testUpdateStateMalfunction()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2, mPort3);

    const double leakRate         = 1.0;
    const double leakConductivity = GunnsFluidUtils::predictConductivity(leakRate,
                                                                         mArticle->mMinLinearizationPotential,
                                                                         mNodes[0].getOutflow(),
                                                                         mNodes[1].getOutflow());
    const double position         = 0.5 * leakConductivity / mArticle->mMaxConductivity;

    mArticle->setPosition(position);
    mArticle->updateState(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mMalfLeakThruValue,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mPreviousLeakRate,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mLeakConductivity,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5 * leakConductivity,
                                 mArticle->mEffectiveConductivity,
                                 mTolerance);

    /// @test     Update state with initial leak rate malfunction insertion.
    mArticle->mMalfLeakThruFlag  = true;
    mArticle->mMalfLeakThruValue = leakRate;
    mArticle->updateState(mTimeStep);
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

    /// @test     Update state with leak rate malfunction in progress.
    mArticle->updateState(mTimeStep);
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

    /// @test     Update state with leak rate malfunction removed.
    mArticle->mMalfLeakThruFlag  = false;
    mArticle->mMalfLeakThruValue = 0.0;
    mArticle->updateState(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mPreviousLeakRate,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mMalfLeakThruValue,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mLeakConductivity,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5 * leakConductivity,
                                 mArticle->mEffectiveConductivity,
                                 mTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test accessors and getters.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPressureSensitiveValve::testInitializationExceptions()
{
    UT_RESULT;

    /// - Default construct a test article.
    GunnsFluidPressureSensitiveValve article;

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
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, 0, 0, 2, 3), TsInitializationException);

    /// @test    Initialization exception on invalid arguments: duplicate nodes on ports 0 & 1
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, 1, 1, 2, 3), TsInitializationException);

    /// @test    Initialization exception on invalid arguments: duplicate nodes on ports 2 & 3
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, 0, 1, 2, 2), TsInitializationException);

    /// @test    Initialization exception on invalid arguments: duplicate nodes on ports 2 & 3
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, 0, 1, 3, 3), TsInitializationException);

    UT_PASS_LAST;
}
