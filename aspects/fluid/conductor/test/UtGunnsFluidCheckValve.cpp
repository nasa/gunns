/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/fluid/conductor/GunnsFluidCheckValve.o))
***************************************************************************************************/

#include "core/GunnsFluidUtils.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidCheckValve.hh"

/// @details  Test identification number.
int UtGunnsFluidCheckValve::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Check Valve link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidCheckValve::UtGunnsFluidCheckValve()
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
    mRateLimit(0.0),
    mThermalLength(0.0),
    mThermalDiameter(0.0),
    mSurfaceRoughness(0.0),
    mThermalSurfaceArea(0.0),
    mThermalROverD(0.0),
    mClosePressure(0.0),
    mOpenPressure(0.0),
    mConfigData(0),
    mMalfBlockageFlag(false),
    mMalfBlockageValue(0.0),
    mPosition(0.0),
    mMalfLeakThruFlag(false),
    mMalfLeakThruValue(0.0),
    mWallTemperature(0.0),
    mMalfStuckFlag(false),
    mMalfFailToFlag(false),
    mMalfFailToValue(0.0),
    mInputData(0),
    mArticle(0),
    mPreviousLeakRate(0.0),
    mLeakConductivity(0.0),
    mWallHeatFlux(0.0),
    mFlowRate(0),
    mTimeStep(0.0),
    mTolerance(0.0)
 {
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Check Valve link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidCheckValve::~UtGunnsFluidCheckValve()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCheckValve::setUp()
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
    mNodeList.mNodes      = mNodes;
    mNodeList.mNumNodes   = N_NODES;

    /// - Define the nominal configuration data.
    mName                 = "nominal";
    mMaxConductivity      = 20.0;
    mExpansionScaleFactor =  0.5;
    mThermalLength        = 1.0;
    mThermalDiameter      = 0.01;
    mSurfaceRoughness     = 2.1336E-6;
    mThermalSurfaceArea   =  mThermalLength * mThermalDiameter * UnitConversion::PI_UTIL;
    mThermalROverD        =  mSurfaceRoughness / mThermalDiameter;
    mRateLimit            =  1.0;
    mClosePressure        =  0.1;
    mOpenPressure         =  1.0;
    mConfigData           = new GunnsFluidCheckValveConfigData(mName,
                                                               &mNodeList,
                                                               mMaxConductivity,
                                                               mExpansionScaleFactor,
                                                               mThermalLength,
                                                               mThermalDiameter,
                                                               mSurfaceRoughness,
                                                               mRateLimit,
                                                               mClosePressure,
                                                               mOpenPressure);

    /// - Define the nominal input data.
    mMalfBlockageFlag     = false;
    mMalfBlockageValue    = 0.3;
    mPosition             = 1.0;
    mMalfLeakThruFlag     = false;
    mMalfLeakThruValue    = 0.0;
    mPreviousLeakRate     = 0.0;
    mLeakConductivity     = 0.0;
    mWallTemperature      = 300.0;
    mMalfStuckFlag        = false;
    mMalfFailToFlag       = false;
    mMalfFailToValue      = 0.0;
    mInputData            = new GunnsFluidCheckValveInputData(mMalfBlockageFlag,
                                                              mMalfBlockageValue,
                                                              mPosition,
                                                              mMalfLeakThruFlag,
                                                              mMalfLeakThruValue,
                                                              mWallTemperature,
                                                              mMalfStuckFlag,
                                                              mMalfFailToFlag,
                                                              mMalfFailToValue);

    /// - Define the nominal port mapping.
    mPort0                = 0;
    mPort1                = 1;

    /// - Default construct the nominal test article.
    mArticle              = new FriendlyGunnsFluidCheckValve;

    /// - Define the nominal flow rate, time step and comparison tolerance.
    mFlowRate             = 0.3;
    mTimeStep             = 0.1;
    mTolerance            = 1.0e-08;

    /// - Increment the test identification number.
    ++TEST_ID;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCheckValve::tearDown()
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
/// @details  Tests for GUNNS Fluid Check Valve link model construction of configuration and input
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCheckValve::testConfigAndInput()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(mName                                          == mConfigData->mName);
    CPPUNIT_ASSERT(mNodes                                         == mConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity,                   mConfigData->mMaxConductivity,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mExpansionScaleFactor,              mConfigData->mExpansionScaleFactor,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mRateLimit,                         mConfigData->mRateLimit,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mClosePressure,                     mConfigData->mClosePressure,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mOpenPressure,                      mConfigData->mOpenPressure,          0.0);

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(mMalfBlockageFlag                              == mInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfBlockageValue,                 mInputData->mMalfBlockageValue,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mPosition,                          mInputData->mPosition,               0.0);
    CPPUNIT_ASSERT(mMalfLeakThruFlag                              == mInputData->mMalfLeakThruFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfLeakThruValue,                 mInputData->mMalfLeakThruValue,      0.0);
    CPPUNIT_ASSERT(mMalfStuckFlag                                 == mInputData->mMalfStuckFlag);
    CPPUNIT_ASSERT(mMalfFailToFlag                                == mInputData->mMalfFailToFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfFailToValue,                   mInputData->mMalfFailToValue,        0.0);

    /// @test    Configuration data default construction.
    GunnsFluidCheckValveConfigData defaultConfig;
    CPPUNIT_ASSERT(0                                              == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0                                              == defaultConfig.mNodeList);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mMaxConductivity,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mExpansionScaleFactor, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mRateLimit,            0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mClosePressure,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mOpenPressure,         0.0);

    /// @test    Input data default construction.
    GunnsFluidCheckValveInputData   defaultInput;
    CPPUNIT_ASSERT(                                                 !defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultInput.mMalfBlockageValue,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultInput.mPosition,              0.0);
    CPPUNIT_ASSERT(                                                 !defaultInput.mMalfLeakThruFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultInput.mMalfLeakThruValue,     0.0);
    CPPUNIT_ASSERT(                                                 !defaultInput.mMalfStuckFlag);
    CPPUNIT_ASSERT(                                                 !defaultInput.mMalfFailToFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultInput.mMalfFailToValue,       0.0);

    /// @test    Configuration data copy construction.
    GunnsFluidCheckValveConfigData copyConfig(*mConfigData);
    CPPUNIT_ASSERT(mConfigData->mName                             == copyConfig.mName);
    CPPUNIT_ASSERT(mConfigData->mNodeList->mNodes                 == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mMaxConductivity,      copyConfig.mMaxConductivity,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mExpansionScaleFactor, copyConfig.mExpansionScaleFactor,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mRateLimit,            copyConfig.mRateLimit,               0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mClosePressure,                     copyConfig.mClosePressure,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mOpenPressure,                      copyConfig.mOpenPressure,            0.0);

    /// @test    Input data copy construction.
    GunnsFluidCheckValveInputData   copyInput(*mInputData);
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag                  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfBlockageValue,     copyInput.mMalfBlockageValue,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mPosition,              copyInput.mPosition,                 0.0);
    CPPUNIT_ASSERT(mInputData->mMalfLeakThruFlag                  == copyInput.mMalfLeakThruFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfLeakThruValue,     copyInput.mMalfLeakThruValue,        0.0);
    CPPUNIT_ASSERT(mInputData->mMalfStuckFlag                     == copyInput.mMalfStuckFlag);
    CPPUNIT_ASSERT(mInputData->mMalfFailToFlag                    == copyInput.mMalfFailToFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfFailToValue,       copyInput.mMalfFailToValue,          0.0);

    UT_PASS;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Check Valve link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCheckValve::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default construction configuration data.
    CPPUNIT_ASSERT(             "" == mArticle->mName);
    CPPUNIT_ASSERT(              0 == mArticle->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mMaxConductivity,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mExpansionScaleFactor, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mRateLimit,            0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mClosePressure,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mOpenPressure,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mThermalDiameter,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mThermalSurfaceArea,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mThermalROverD,        0.0);

    /// @test    Default construction input data.
    CPPUNIT_ASSERT(                  !mArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mMalfBlockageValue,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mPosition,             0.0);
    CPPUNIT_ASSERT(                  !mArticle->mMalfLeakThruFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mMalfLeakThruValue,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mWallTemperature,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mWallHeatFlux,         0.0);
    CPPUNIT_ASSERT(                  !mArticle->mMalfStuckFlag);
    CPPUNIT_ASSERT(                  !mArticle->mMalfFailToFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mMalfFailToValue,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mPreviousLeakRate,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mLeakConductivity,     0.0);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(                  !mArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsFluidCheckValve* article = new GunnsFluidCheckValve();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Check Valve link model nominal initialization without
///           exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCheckValve::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    FriendlyGunnsFluidCheckValve article;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    Nominal configuration data.
    CPPUNIT_ASSERT(mName                             == article.mName);
    CPPUNIT_ASSERT(&mNodes[0]                        == article.mNodes[0]);
    CPPUNIT_ASSERT(&mNodes[1]                        == article.mNodes[1]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity,      article.mMaxConductivity,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mExpansionScaleFactor, article.mExpansionScaleFactor, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mRateLimit,            article.mRateLimit,            0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mClosePressure,        article.mClosePressure,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mOpenPressure,         article.mOpenPressure,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalSurfaceArea,   article.mThermalSurfaceArea,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalROverD,        article.mThermalROverD,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalDiameter,      article.mThermalDiameter,      DBL_EPSILON);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT(mMalfBlockageFlag                 == article.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfBlockageValue,    article.mMalfBlockageValue,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mPosition,             article.mPosition,             0.0);
    CPPUNIT_ASSERT(mMalfLeakThruFlag ==                 article.mMalfLeakThruFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfLeakThruValue,    article.mMalfLeakThruValue,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mWallTemperature,      article.mWallTemperature,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                   article.mWallHeatFlux,         0.0);
    CPPUNIT_ASSERT(mMalfStuckFlag                    == article.mMalfStuckFlag);
    CPPUNIT_ASSERT(mMalfFailToFlag                   == article.mMalfFailToFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfFailToValue,      article.mMalfFailToValue,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mPreviousLeakRate,     article.mPreviousLeakRate,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mLeakConductivity,     article.mLeakConductivity,     0.0);


    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(                                     article.mInitFlag);

    ///  -  Ensure Restart model functionality
    article.restartModel();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,article.mEffectiveConductivity,DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,article.mSystemConductance,DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Check Valve link model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCheckValve::testAccessors()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    Nothing (this derived class does not add or override any accessors).

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Check Valve link model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCheckValve::testModifiers()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    The stuck malf is set by the access method.
    mArticle->setMalfStuck(true);
    CPPUNIT_ASSERT(true  == mArticle->mMalfStuckFlag);

    /// @test    The stuck malf is reset by the access method.
    mArticle->setMalfStuck();
    CPPUNIT_ASSERT(false == mArticle->mMalfStuckFlag);

    /// @test    The fail to position malf is set by the access method.
    mArticle->setMalfFailTo(true, 1.0);
    CPPUNIT_ASSERT(true  == mArticle->mMalfFailToFlag);
    CPPUNIT_ASSERT(1.0   == mArticle->mMalfFailToValue);

    /// @test    The fail to position malf is reset by the access method.
    mArticle->setMalfFailTo();
    CPPUNIT_ASSERT(false == mArticle->mMalfFailToFlag);
    CPPUNIT_ASSERT(0.0   == mArticle->mMalfFailToValue);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Check Valve link model update state (nominal).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCheckValve::testUpdateStateNominal()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// - Effectively disable rate limiting.
    mArticle->mRateLimit = 1.0 / mTimeStep;

    /// @test    With valve closed and sufficient positive delta pressure so it full opens.
    mArticle->mPosition = 0.0;
    mArticle->mPotentialDrop = mOpenPressure;
    double expected          = 1.0;
    mArticle->updateState(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->mPosition, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity * expected,
                                 mArticle->mEffectiveConductivity,
                                 mTolerance);

    /// @test    With insufficient positive delta pressure so it partially opens.
    mArticle->mPotentialDrop = mClosePressure + 0.75 * (mOpenPressure - mClosePressure);
    expected                 = 0.75;
    mArticle->updateState(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->mPosition, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity * expected,
                                 mArticle->mEffectiveConductivity,
                                 mTolerance);

    /// @test    With insufficient negative delta pressure so it partially closes.
    mArticle->mPotentialDrop = mClosePressure + 0.25 * (mOpenPressure - mClosePressure);
    expected                 = 0.25;
    mArticle->updateState(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->mPosition, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity * expected,
                                 mArticle->mEffectiveConductivity,
                                 mTolerance);

    /// @test    With sufficient negative delta pressure so it closes.
    mArticle->mPotentialDrop = mClosePressure;
    expected                 = 0.0;
    mArticle->updateState(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->mPosition, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity * expected,
                                 mArticle->mEffectiveConductivity,
                                 mTolerance);

    /// @test    With insufficient negative delta pressure it partially closes.
    mArticle->mPotentialDrop = mClosePressure + 0.25 * (mOpenPressure - mClosePressure);
    expected                 = 0.25;
    mArticle->updateState(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->mPosition, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity * expected,
                                 mArticle->mEffectiveConductivity,
                                 mTolerance);

    /// @test    With insufficient positive delta pressure so it partially opens.
    mArticle->mPotentialDrop = mClosePressure + 0.75 * (mOpenPressure - mClosePressure);
    expected                 = 0.75;
    mArticle->updateState(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->mPosition, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity * expected,
                                 mArticle->mEffectiveConductivity,
                                 mTolerance);

    /// @test    With sufficient positive delta pressure so it fully opens.
    mArticle->mPotentialDrop = mOpenPressure;
    expected                 = 1.0;
    mArticle->updateState(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->mPosition, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity * expected,
                                 mArticle->mEffectiveConductivity,
                                 mTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Check Valve link model update state (rate limiting).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCheckValve::testUpdateStateRateLimited()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    Rate limiting with closed valve and delta p larger than rate limit * dt.
    mArticle->mRateLimit     = 0.25;
    mArticle->mPosition      = 0.0;
    mArticle->mPotentialDrop = mOpenPressure;
    double expected = mArticle->mRateLimit * mTimeStep;
    mArticle->updateState(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->mPosition, mTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Check Valve link model update state (open and close pressures
///           both zero).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCheckValve::testUpdateStateNoRange()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// - Effectively disable rate limiting and set open and closed pressure to zero.
    mArticle->mRateLimit     = 1.0 / mTimeStep;
    mArticle->mOpenPressure  = 0.0;
    mArticle->mClosePressure = 0.0;

    /// @test    With valve closed and a very small positive delta pressure so it fully opens.
    mArticle->mPosition      = 0.0;
    mArticle->mPotentialDrop = +DBL_EPSILON;
    double expected          = 1.0;
    mArticle->updateState(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->mPosition, mTolerance);

    /// @test    With valve open and a very small negative delta pressure so it closes.
    mArticle->mPotentialDrop = -DBL_EPSILON;
    expected = 0.0;
    mArticle->updateState(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->mPosition, mTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Check Valve link model update state (open and close pressures
///           both zero).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCheckValve::testUpdateStateMalfunction()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mConfigData->mClosePressure = 12.0;
    mConfigData->mOpenPressure  = 16.0;
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mNodes[0].getContent()->setPressure(112.0);
    mNodes[1].getContent()->setPressure(100.0);
    mNodes[0].getContent()->setTemperature(283.0);
    mNodes[1].getContent()->setTemperature(283.0);
    mArticle->mRateLimit     = 1.0 / mTimeStep;

    mArticle->mPotentialDrop      = 12.0;
    const double leakRate         = 1.0;
    const double leakConductivity = GunnsFluidUtils::predictConductivity(leakRate,
                                                                         mArticle->mMinLinearizationPotential,
                                                                         mNodes[0].getOutflow(),
                                                                         mNodes[1].getOutflow());

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

    mConfigData->mClosePressure = mClosePressure;
    mConfigData->mOpenPressure  = mOpenPressure;

    /// - Create handle fail to position criteria
    mArticle->mMalfStuckFlag = false;
    mArticle->mMalfFailToFlag = true;
    mArticle->mMalfFailToValue = 0.4;
    mArticle->updateState(mTimeStep);

    /// @test     Test <1.0 position setting for handle failure
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.4, mArticle->mPosition, 0.0);


    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Check Valve link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCheckValve::testInitializationExceptions()
{
    UT_RESULT;

    /// - Default construct a test article.
    GunnsFluidCheckValve article;

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

    /// @test    Initialization exception on invalid config data: mRateLimit < 0.
    mConfigData->mRateLimit  = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mRateLimit  = mRateLimit;

    /// @test    Initialization exception on invalid config data: mOpenPosition < mClosePosition.
    mConfigData->mClosePressure  = +DBL_EPSILON * 0.5;
    mConfigData->mOpenPressure   = -DBL_EPSILON * 0.5;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mClosePressure  = mClosePressure;
    mConfigData->mOpenPressure   = mOpenPressure;

    /// @test    Initialization exception on invalid input data: mMalfBlockageValue < 0.
    mInputData->mMalfBlockageValue = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mInputData->mMalfBlockageValue = mMalfBlockageValue;

    /// @test    Initialization exception on invalid input data: mMalfBlockageValue > 1.
    mInputData->mMalfBlockageValue = 1.0 + FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mInputData->mMalfBlockageValue = mMalfBlockageValue;

    /// @test    Initialization exception on invalid input data: wall temperature < 0.
    mInputData->mWallTemperature = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mInputData->mWallTemperature = mWallTemperature;

    /// @test    Initialization exception on invalid input data: mPosition < 0.
    mInputData->mPosition = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mInputData->mPosition = mPosition;

    /// @test    Initialization exception on invalid input data: mPosition > 1.
    mInputData->mPosition = 1.0 + FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mInputData->mPosition = mPosition;

    /// @test    Initialization exception on invalid input data: mMalfFailToValue < 0.
    mInputData->mMalfFailToValue = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mInputData->mMalfFailToValue = mMalfFailToValue;

    /// @test    Initialization exception on invalid input data: mMalfFailToValue > 1.
    mInputData->mMalfFailToValue = 1.0 + FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mInputData->mMalfFailToValue = mMalfFailToValue;

    UT_PASS_LAST;
}
