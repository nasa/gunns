/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/fluid/potential/GunnsFluidImpeller.o))
***************************************************************************************************/

#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidImpeller.hh"

/// @details  Test identification number.
int UtGunnsFluidImpeller::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Impeller link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidImpeller::UtGunnsFluidImpeller()
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
    mReferenceDensity(0.0),
    mReferenceSpeed(0.0),
    mThermalLength(0.0),
    mThermalDiameter(0.0),
    mSurfaceRoughness(0.0),
    mPowerCurveCoefficient(0.0),
    mThermalSurfaceArea(0.0),
    mThermalROverD(0.0),
    mConfigData(0),
    mMalfBlockageFlag(false),
    mMalfBlockageValue(0.0),
    mSourcePressure(0.0),
    mMotorSpeed(0.0),
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
/// @details  Default destructs this GUNNS Fluid Impeller link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidImpeller::~UtGunnsFluidImpeller()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidImpeller::setUp()
{
    /// - Define nominal port fluids.
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

    /// - Initialize the nodes list.
    mNodeList.mNodes      = mNodes;
    mNodeList.mNumNodes   = N_NODES;

    /// - Define the nominal configuration data.
    mName                 = "nominal";
    mMaxConductivity      = 20.0;
    mExpansionScaleFactor = 0.5;
    mSourcePressure       = 10.0;
    mReferenceDensity     = mNodes[0].getContent()->getDensity();
    mReferenceSpeed       = 1000.0;
    mThermalLength        = 0.3;
    mThermalDiameter      = 0.4;
    mSurfaceRoughness     = 2.13360E-06;
    mConfigData           = new GunnsFluidImpellerConfigData(mName,
                                                             &mNodeList,
                                                             mMaxConductivity,
                                                             mExpansionScaleFactor,
                                                             mReferenceDensity,
                                                             mReferenceSpeed,
                                                             mThermalLength,
                                                             mThermalDiameter,
                                                             mSurfaceRoughness);

    /// - Compute the nominal configuration derived data.
    mPowerCurveCoefficient = mSourcePressure / (mReferenceDensity * mReferenceSpeed * mReferenceSpeed);
    mThermalROverD         = mSurfaceRoughness / mThermalDiameter;
    mThermalSurfaceArea    = UnitConversion::PI_UTIL * mThermalLength * mThermalDiameter;

    /// - Define the nominal input data
    mMalfBlockageFlag     = false;
    mMalfBlockageValue    = 0.01;
    mMotorSpeed           = 1000.0;
    mWallTemperature      = 300.0;
    mWallHeatFlux         = 0.0;
    mInputData            = new GunnsFluidImpellerInputData(mMalfBlockageFlag,
                                                            mMalfBlockageValue,
                                                            mSourcePressure,
                                                            mMotorSpeed,
                                                            mWallTemperature);

    /// - Define the nominal port mapping.
    mPort0                = 0;
    mPort1                = 1;

    /// - Default construct the nominal test article.
    mArticle              = new FriendlyGunnsFluidImpeller;

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
void UtGunnsFluidImpeller::tearDown()
{
    /// - Deletes in reverse order of news in setUp.
    delete mArticle;
    delete mInputData;
    delete mConfigData;
    delete mFluidInput1;
    delete mFluidInput0;
    delete mFluidConfig;
    delete mFluidProperties;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of GUNNS Fluid Impeller link model configuration and input
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidImpeller::testConfigAndInput()
{
    UT_RESULT_INITIAL("GUNNS Fluid Potential Links");

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(mName                                          == mConfigData->mName);
    CPPUNIT_ASSERT(mNodes                                         == mConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity,                   mConfigData->mMaxConductivity,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mExpansionScaleFactor,              mConfigData->mExpansionScaleFactor,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mReferenceDensity,                  mConfigData->mReferenceDensity,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mReferenceSpeed,                    mConfigData->mReferenceSpeed,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalLength,                     mConfigData->mThermalLength,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalDiameter,                   mConfigData->mThermalDiameter,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSurfaceRoughness,                  mConfigData->mSurfaceRoughness,      0.0);

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(mMalfBlockageFlag                              == mInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfBlockageValue,                 mInputData->mMalfBlockageValue,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSourcePressure,                    mInputData->mSourcePressure,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMotorSpeed,                        mInputData->mMotorSpeed,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mWallTemperature,                   mInputData->mWallTemperature,        0.0);

    /// @test    Configuration data default construction.
    GunnsFluidImpellerConfigData defaultConfig;
    CPPUNIT_ASSERT(0                                              == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0                                              == defaultConfig.mNodeList);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mMaxConductivity,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mExpansionScaleFactor, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mReferenceDensity,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mReferenceSpeed,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mThermalLength,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mThermalDiameter,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mSurfaceRoughness,     0.0);

    /// @test    Input data default construction.
    GunnsFluidImpellerInputData   defaultInput;
    CPPUNIT_ASSERT(                                                 !defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultInput.mMalfBlockageValue,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultInput.mSourcePressure,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultInput.mMotorSpeed,            0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultInput.mWallTemperature,       0.0);

    /// @test    Configuration data copy construction.
    GunnsFluidImpellerConfigData copyConfig(*mConfigData);
    CPPUNIT_ASSERT(mConfigData->mName                             == copyConfig.mName);
    CPPUNIT_ASSERT(mConfigData->mNodeList->mNodes                 == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mMaxConductivity,      copyConfig.mMaxConductivity,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mExpansionScaleFactor, copyConfig.mExpansionScaleFactor,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mReferenceDensity,     copyConfig.mReferenceDensity,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mReferenceSpeed,       copyConfig.mReferenceSpeed,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mThermalLength,        copyConfig.mThermalLength,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mThermalDiameter,      copyConfig.mThermalDiameter,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mSurfaceRoughness,     copyConfig.mSurfaceRoughness,        0.0);

    /// @test    Input data copy construction.
    GunnsFluidImpellerInputData   copyInput(*mInputData);
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag                  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfBlockageValue,     copyInput.mMalfBlockageValue,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mSourcePressure,        copyInput.mSourcePressure,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMotorSpeed,            copyInput.mMotorSpeed,               0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mWallTemperature,       copyInput.mWallTemperature,          0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Impeller link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidImpeller::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default construction configuration data.
    CPPUNIT_ASSERT(""                                             == mArticle->mName);
    CPPUNIT_ASSERT(0                                              == mArticle->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                mArticle->mMaxConductivity,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                mArticle->mExpansionScaleFactor,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                mArticle->mPowerCurveCoefficient,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                mArticle->mThermalDiameter,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                mArticle->mThermalSurfaceArea,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                mArticle->mThermalROverD,            0.0);

    /// @test    Default construction input data.
    CPPUNIT_ASSERT(                                                 !mArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                mArticle->mMalfBlockageValue,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                mArticle->mMotorSpeed,               0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                mArticle->mWallTemperature,          0.0);

    /// @test    Default construction state data.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                mArticle->mWallHeatFlux,             0.0);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(                                                 !mArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsFluidImpeller* article = new GunnsFluidImpeller();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Impeller link model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidImpeller::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    FriendlyGunnsFluidImpeller article;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    Nominal configuration data.
    CPPUNIT_ASSERT(mName                                      == article.mName);
    CPPUNIT_ASSERT(&mNodes[0]                                 == article.mNodes[0]);
    CPPUNIT_ASSERT(&mNodes[1]                                 == article.mNodes[1]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity,               article.mMaxConductivity,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mExpansionScaleFactor,          article.mExpansionScaleFactor,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mPowerCurveCoefficient,         article.mPowerCurveCoefficient, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalDiameter,               article.mThermalDiameter,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalSurfaceArea,            article.mThermalSurfaceArea,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalROverD,                 article.mThermalROverD,         0.0);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag              == article.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfBlockageValue, article.mMalfBlockageValue,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMotorSpeed,                    article.mMotorSpeed,            0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mWallTemperature,               article.mWallTemperature,       0.0);

    /// @test    Nominal state data.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mWallHeatFlux,                  article.mWallHeatFlux,          0.0);


    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(article.mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Impeller link model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidImpeller::testAccessors()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test   Nothing, there are no accessors (yet).

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Impeller link model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidImpeller::testModifiers()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
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
/// @details  Tests for GUNNS Fluid Impeller link model update state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidImpeller::testUpdateState()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test   Update state at reference speed and density.
    mArticle->updateState(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSourcePressure,
                                 mArticle->mSourcePressure,
                                 mTolerance);

    /// @test   Update state at zero speed and reference density.
    mArticle->mMotorSpeed = 0.0;
    mArticle->updateState(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mSourcePressure,
                                 mTolerance);

    /// @test   Update state at non-zero, non-reference speed and reference density.
    const double impellerSpeed          = 1200.0;
    const double expectedSourcePressure = mSourcePressure * impellerSpeed / mReferenceSpeed *
                                          impellerSpeed / mReferenceSpeed;
    mArticle->mMotorSpeed            = impellerSpeed;
    mArticle->updateState(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSourcePressure,
                                 mArticle->mSourcePressure,
                                 mTolerance);

    /// @test   Update state at reference speed and higher than reference density.
    mArticle->mMotorSpeed            = mMotorSpeed;
    mArticle->mInternalFluid->setTemperature(mNodes[0].getContent()->getTemperature() - 50.0);
    mArticle->updateState(mTimeStep);
    CPPUNIT_ASSERT(mSourcePressure < mArticle->mSourcePressure);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Impeller link model update fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidImpeller::testUpdateFluid()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test   Update fluid with flowrate too small.
    mArticle->updateFluid(mTimeStep, 0.5 * DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mWallHeatFlux,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNodes[0].getContent()->getTemperature(),
                                 mArticle->mInternalFluid->getTemperature(),
                                 mTolerance);

    /// @test   Update fluid with nominal time step, flowrate and temperature.
    mArticle->updateFluid(mTimeStep, mFlowRate);
    CPPUNIT_ASSERT(mArticle->mWallHeatFlux < 0.0);
    CPPUNIT_ASSERT(mArticle->mInternalFluid->getTemperature() > 283.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Impeller link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidImpeller::testInitializationExceptions()
{
    UT_RESULT;

    /// - Default construct a test article.
    GunnsFluidImpeller article;

    /// @test    Initialization exception on no name.
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

    /// @test    Initialization exception on invalid config data: reference density too small.
    mConfigData->mReferenceDensity = DBL_EPSILON * 0.5;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mReferenceDensity = mReferenceDensity;

    /// @test    Initialization exception on invalid config data: reference speed too small.
    mConfigData->mReferenceSpeed = DBL_EPSILON * 0.5;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mReferenceSpeed = mReferenceSpeed;

    /// @test    Initialization exception on invalid input data: mBlockage < 0.
    mInputData->mMalfBlockageValue = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mInputData->mMalfBlockageValue = mMalfBlockageValue;

    /// @test    Initialization exception on invalid input data: source pressure too small.
    mInputData->mSourcePressure = DBL_EPSILON * 0.5;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mInputData->mSourcePressure = mSourcePressure;

    /// @test    Initialization exception on invalid input data: impeller speed < 0.
    mInputData->mMotorSpeed = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mInputData->mMotorSpeed = mMotorSpeed;

    /// @test    Initialization exception on invalid input data: impeller temperature < 0.
    mInputData->mWallTemperature = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mInputData->mWallTemperature = mWallTemperature;

    UT_PASS_LAST;
}
