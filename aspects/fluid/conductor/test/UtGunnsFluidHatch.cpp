/*
@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/

#include "core/GunnsFluidUtils.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidHatch.hh"

/// @details  Acts as a redundant test on UnitConversion class.
const double UtGunnsFluidHatch::PI = 3.14159265358979;

/// @details  Test identification number.
int UtGunnsFluidHatch::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Hatch link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidHatch::UtGunnsFluidHatch()
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
    mLength0(0.0),
    mLength1(0.0),
    mConfigData(0),
    mMalfBlockageFlag(false),
    mMalfBlockageValue(0.0),
    mPosition(0.0),
    mMalfLeakThruFlag(false),
    mMalfLeakThruValue(0.0),
    mInputData(0),
    mArticle(0),
    mPreviousLeakRate(0.0),
    mLeakConductivity(0.0),
    mFlowRate(0),
    mTimeStep(0.0),
    mTolerance(0.0),
    mThermalLength(0.0),
    mThermalDiameter(0.0),
    mSurfaceRoughness(0.0),
    mWallTemperature(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Hatch link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidHatch::~UtGunnsFluidHatch()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHatch::setUp()
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
    mNodes[0].initVolume(1.0);
    mNodes[1].initVolume(1.0);
    mNodes[0].resetFlows();
    mNodes[1].resetFlows();

    /// - Initialize the nodes list.
    mNodeList.mNodes      = mNodes;
    mNodeList.mNumNodes   = N_NODES;

    /// - Define the nominal configuration data.
    mName                 = "nominal";
    mMaxConductivity      =  1.5;
    mExpansionScaleFactor =  0.5;
    mThermalLength        = 1.0;
    mThermalDiameter      = 0.01;
    mSurfaceRoughness     = 2.1336E-6;
    mLength0              =  2.0;
    mLength1              =  4.0;
    mConfigData           = new GunnsFluidHatchConfigData(mName,
                                                          &mNodeList,
                                                          mMaxConductivity,
                                                          mExpansionScaleFactor,
                                                          mThermalLength,
                                                          mThermalDiameter,
                                                          mSurfaceRoughness,
                                                          mLength0,
                                                          mLength1);

    /// - Define the nominal input data.
    mMalfBlockageFlag     = false;
    mMalfBlockageValue    = 0.3;
    mPosition             = 1.0;
    mMalfLeakThruFlag     = false;
    mMalfLeakThruValue    = 0.0;
    mPreviousLeakRate     = 0.0;
    mLeakConductivity     = 0.0;
    mWallTemperature      = 300.0;
    mInputData            = new GunnsFluidHatchInputData(mMalfBlockageFlag,
                                                         mMalfBlockageValue,
                                                         mPosition,
                                                         mMalfLeakThruFlag,
                                                         mMalfLeakThruValue,
                                                         mWallTemperature);

    /// - Define the nominal port mapping.
    mPort0                = 0;
    mPort1                = 1;

    /// - Default construct the nominal test article.
    mArticle              = new FriendlyGunnsFluidHatch;

    /// - Define the nominal flow rate, time step and comparison tolerance.
    mFlowRate             = 0.3;
    mTimeStep             = 0.1;
    mTolerance            = 1.0e-08;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHatch::tearDown()
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
/// @details  Tests for GUNNS Fluid Hatch link model construction of configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHatch::testConfigAndInput()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(mName  == mConfigData->mName);
    CPPUNIT_ASSERT(mNodes == mConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity,
                                 mConfigData->mMaxConductivity,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mExpansionScaleFactor,
                                 mConfigData->mExpansionScaleFactor,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mLength0,
                                 mConfigData->mLength0,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mLength1,
                                 mConfigData->mLength1,
                                 0.0);

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(mMalfBlockageFlag == mInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfBlockageValue,
                                 mInputData->mMalfBlockageValue,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mPosition,
                                 mInputData->mPosition,
                                 0.0);
    CPPUNIT_ASSERT(mMalfLeakThruFlag == mInputData->mMalfLeakThruFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfLeakThruValue,
                                 mInputData->mMalfLeakThruValue,
                                 0.0);

    /// @test    Configuration data default construction.
    GunnsFluidHatchConfigData defaultConfig;
    CPPUNIT_ASSERT(0 == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0 == defaultConfig.mNodeList);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 defaultConfig.mMaxConductivity,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 defaultConfig.mExpansionScaleFactor,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 defaultConfig.mLength0,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 defaultConfig.mLength1,
                                 0.0);

    /// @test    Input data default construction.
    GunnsFluidHatchInputData   defaultInput;
    CPPUNIT_ASSERT(!defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 defaultInput.mMalfBlockageValue,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 defaultInput.mPosition,
                                 0.0);
    CPPUNIT_ASSERT(!defaultInput.mMalfLeakThruFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 defaultInput.mMalfLeakThruValue,
                                 0.0);

    /// @test    Configuration data copy construction.
    GunnsFluidHatchConfigData copyConfig(*mConfigData);
    CPPUNIT_ASSERT(mConfigData->mName             == copyConfig.mName);
    CPPUNIT_ASSERT(mConfigData->mNodeList->mNodes == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mMaxConductivity,
                                 copyConfig.mMaxConductivity,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mExpansionScaleFactor,
                                 copyConfig.mExpansionScaleFactor,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mLength0,
                                 copyConfig.mLength0,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mLength1,
                                 copyConfig.mLength1,
                                 0.0);

    /// @test    Input data copy construction.
    GunnsFluidHatchInputData   copyInput(*mInputData);
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfBlockageValue,
                                 copyInput.mMalfBlockageValue,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mPosition,
                                 copyInput.mPosition,
                                 0.0);
    CPPUNIT_ASSERT(mInputData->mMalfLeakThruFlag == copyInput.mMalfLeakThruFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfLeakThruValue,
                                 copyInput.mMalfLeakThruValue,
                                 0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Hatch link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHatch::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default construction configuration data.
    CPPUNIT_ASSERT("" == mArticle->mName);
    CPPUNIT_ASSERT(0  == mArticle->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mMaxConductivity,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mExpansionScaleFactor,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mLength0,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mLength1,
                                 0.0);

    /// @test    Default construction input data.
    CPPUNIT_ASSERT(!mArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mMalfBlockageValue,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mPosition,
                                 0.0);
    CPPUNIT_ASSERT(!mArticle->mMalfLeakThruFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mMalfLeakThruValue,
                                 0.0);

    /// @test    Test that optional thermal parameters are defaulting to zero
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mThermalDiameter,
                                 0.0);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                  mArticle->mThermalSurfaceArea,
                                  0.0);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                  mArticle->mThermalROverD,
                                  0.0);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                  mArticle->mWallTemperature,
                                  0.0);

    /// @test    Default construction state data.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mPreviousLeakRate,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mLeakConductivity,
                                 0.0);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(!mArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsFluidHatch* article = new GunnsFluidHatch();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Hatch link model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHatch::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    FriendlyGunnsFluidHatch article;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    Nominal configuration data.
    CPPUNIT_ASSERT(mName  == article.mName);
    CPPUNIT_ASSERT(&mNodes[0] == article.mNodes[0]);
    CPPUNIT_ASSERT(&mNodes[1] == article.mNodes[1]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity,
                                 article.mMaxConductivity,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mExpansionScaleFactor,
                                 article.mExpansionScaleFactor,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mLength0,
                                 article.mLength0,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mLength1,
                                 article.mLength1,
                                 0.0);
    const double expectedArea   = mThermalLength * PI * mThermalDiameter;
    const double expectedROverD = mSurfaceRoughness / mThermalDiameter;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedArea,
                                 article.mThermalSurfaceArea,
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedROverD,
                                 article.mThermalROverD,
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalDiameter,
                                 article.mThermalDiameter,
                                 DBL_EPSILON);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT(mMalfBlockageFlag == article.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfBlockageValue,
                                 article.mMalfBlockageValue,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mPosition,
                                 article.mPosition,
                                 0.0);
    CPPUNIT_ASSERT(mMalfLeakThruFlag == article.mMalfLeakThruFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfLeakThruValue,
                                 article.mMalfLeakThruValue,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mWallTemperature,
                                 article.mWallTemperature,
                                 0.0);

    /// @test    Nominal state data.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mPreviousLeakRate,
                                 article.mPreviousLeakRate,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mLeakConductivity,
                                 article.mLeakConductivity,
                                 0.0);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(article.mInitFlag);

    /// @test    Verify restartModel functionality
    article.mDiffusiveFlowRate  = 1.0;
    article.mConductiveHeatFlux = 1.0;

    article.restartModel();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,article.mDiffusiveFlowRate,DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,article.mConductiveHeatFlux,DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Hatch link model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHatch::testAccessors()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    Nothing (this derived class does not add or override any accessors).

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Hatch link model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHatch::testModifiers()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    Nothing (this derived class does not add or override any modifiers).

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Hatch link model update state nominal.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHatch::testUpdateStateNominal()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    {
        /// @test    For effective conductivity at more than nominal full open position.
        const double position = 2.00;
        mArticle->mPosition = position;
        mArticle->updateState(mTimeStep);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity * position,
                                     mArticle->mEffectiveConductivity,
                                     mTolerance);
    } {
        /// @test    For effective conductivity at nominal full open position.
        const double position = 1.00;
        mArticle->mPosition = position;
        mArticle->updateState(mTimeStep);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity * position,
                                     mArticle->mEffectiveConductivity,
                                     mTolerance);
    } {
        /// @test    For effective conductivity at intermediate position.
        const double position = 0.50;
        mArticle->mPosition = position;
        mArticle->updateState(mTimeStep);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity * position,
                                     mArticle->mEffectiveConductivity,
                                     mTolerance);
    } {
        /// @test    For effective conductivity at nominal full closed position.
        const double position = 0.00;
        mArticle->mPosition = position;
        mArticle->updateState(mTimeStep);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity * position,
                                     mArticle->mEffectiveConductivity,
                                     mTolerance);
    } {
        /// @test    For effective conductivity at less than nominal full closed position.
        const double position = -1.00;
        mArticle->mPosition = position;
        mArticle->updateState(mTimeStep);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mLeakConductivity,
                                     mArticle->mEffectiveConductivity,
                                     mTolerance);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Hatch link model update state with malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHatch::testUpdateStateMalfunction()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    const double leakRate         = 1.0;
    const double leakConductivity = GunnsFluidUtils::predictConductivity(leakRate,
                                                                         mArticle->mMinLinearizationPotential,
                                                                         mNodes[0].getOutflow(),
                                                                         mNodes[1].getOutflow());
    const double position         = 0.5 * leakConductivity / mArticle->mMaxConductivity;

    mArticle->mPosition = position;
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
/// @details  Tests for GUNNS Fluid Hatch link model update fluid with neither gas diffusion nor
///           heat conduction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHatch::testUpdateFluidNeither()
{
    UT_RESULT;

    /// - Initialize default test article with port fluids at same temperature and with same
    ///   mole fractions
    PolyFluidInputData fluidInput0(273.0, 109.0, 0.0, 0.0, mFractions);
    PolyFluidInputData fluidInput1(293.0, 109.0, 0.0, 0.0, mFractions);
    mNodes[0].initialize("UtNode1", mFluidConfig);
    mNodes[1].initialize("UtNode2", mFluidConfig);
    mNodes[0].getContent()->initialize(*mFluidConfig, fluidInput0);
    mNodes[1].getContent()->initialize(*mFluidConfig, fluidInput1);
    mNodes[0].resetFlows();
    mNodes[1].resetFlows();
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @todo     Compute real expected values.
    mArticle->mFlowRate = mFlowRate;
    mArticle->updateFluid(mTimeStep, mFlowRate);
    const double expected  = mArticle->mFlowRate - mFlowRate;
    const double expected0 = mNodes[0].getOutflow()->getTemperature();
    const double expected1 = mNodes[1].getOutflow()->getTemperature();

    /// @test     Mass flux and node fluid temperatures.
    mArticle->mFlowRate = mFlowRate;
    mArticle->updateFluid(mTimeStep, mFlowRate);
    const double returned  = mArticle->mFlowRate - mFlowRate;
    const double returned0 = mNodes[0].getOutflow()->getTemperature();
    const double returned1 = mNodes[1].getOutflow()->getTemperature();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,  returned,  mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected0, returned0, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected1, returned1, mTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Hatch link model update fluid with gas diffusion.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHatch::testUpdateFluidDiffuse()
{
    UT_RESULT;

    /// - Initialize default test article with port fluids at same temperature and pressure but with different
    ///   mole fractions
    double temp0 = 283.0;
    double temp1 = 283.5;
    double fractions0[2] = {0.4, 0.6};
    double fractions1[2] = {0.6, 0.4};
    PolyFluidInputData fluidInput0(temp0, 109.0, 0.0, 0.0, fractions0);
    PolyFluidInputData fluidInput1(temp1, 109.0, 0.0, 0.0, fractions1);
    mNodes[0].initialize("UtNode1", mFluidConfig);
    mNodes[1].initialize("UtNode2", mFluidConfig);
    mNodes[0].getContent()->initialize(*mFluidConfig, fluidInput0);
    mNodes[1].getContent()->initialize(*mFluidConfig, fluidInput1);
    mNodes[0].resetFlows();
    mNodes[1].resetFlows();
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    // Set bulk flow to 0.0. Varying bulk flow rates are tested inside UtGunnsFluidUtils::computeGasDiffusion method.
    // Exact diffusion flow rates are also checked inside UtGunnsFluidUtils::computeGasDiffusion method.
    mArticle->mFlowRate = 0.0;
    mArticle->updateFluid(mTimeStep, 0.0);
    CPPUNIT_ASSERT(mArticle->mDiffusiveFlowRate > 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(temp1, mArticle->mDiffusiveFluid->getTemperature(), mTolerance);

    /// - Test temperature for opposite direction of net diffusive flow.
    temp0 = 283.0;
    temp1 = 283.5;
    fractions0[0] = 0.6;
    fractions0[1] = 0.4;
    fractions1[0] = 0.4;
    fractions1[1] = 0.6;
    PolyFluidInputData fluidInput0a(temp0, 109.0, 0.0, 0.0, fractions0);
    PolyFluidInputData fluidInput1a(temp1, 109.0, 0.0, 0.0, fractions1);
    mNodes[0].initialize("UtNode1", mFluidConfig);
    mNodes[1].initialize("UtNode2", mFluidConfig);
    mNodes[0].getContent()->initialize(*mFluidConfig, fluidInput0a);
    mNodes[1].getContent()->initialize(*mFluidConfig, fluidInput1a);
    mNodes[0].resetFlows();
    mNodes[1].resetFlows();
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->mFlowRate = 0.0;
    mArticle->updateFluid(mTimeStep, 0.0);
    CPPUNIT_ASSERT(mArticle->mDiffusiveFlowRate < 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(temp0, mArticle->mDiffusiveFluid->getTemperature(), mTolerance);

    /// - Set temperature to be beyond limit for diffusion calculation (Node 1 temp lower than Node 0).
    temp0 = 283.0;
    temp1 = 263.0;
    // Return fractions to original values
    fractions0[0] = 0.4;
    fractions0[1] = 0.6;
    fractions1[0] = 0.6;
    fractions1[1] = 0.4;
    PolyFluidInputData fluidInput0b(temp0, 109.0, 0.0, 0.0, fractions0);
    PolyFluidInputData fluidInput1b(temp1, 109.0, 0.0, 0.0, fractions1);
    mNodes[0].initialize("UtNode1", mFluidConfig);
    mNodes[1].initialize("UtNode2", mFluidConfig);
    mNodes[0].getContent()->initialize(*mFluidConfig, fluidInput0b);
    mNodes[1].getContent()->initialize(*mFluidConfig, fluidInput1b);
    mNodes[0].resetFlows();
    mNodes[1].resetFlows();
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->mFlowRate = 0.0;
    mArticle->updateFluid(mTimeStep, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mDiffusiveFlowRate, mTolerance);

    /// - Set temperature to be beyond limit for diffusion calculation (Node 1 temp higher than Node 0).
    temp0 = 283.0;
    temp1 = 303.0;
    PolyFluidInputData fluidInput0c(temp0, 109.0, 0.0, 0.0, fractions0);
    PolyFluidInputData fluidInput1c(temp1, 109.0, 0.0, 0.0, fractions1);
    mNodes[0].initialize("UtNode1", mFluidConfig);
    mNodes[1].initialize("UtNode2", mFluidConfig);
    mNodes[0].getContent()->initialize(*mFluidConfig, fluidInput0c);
    mNodes[1].getContent()->initialize(*mFluidConfig, fluidInput1c);
    mNodes[0].resetFlows();
    mNodes[1].resetFlows();
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->mFlowRate = 0.0;
    mArticle->updateFluid(mTimeStep, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mDiffusiveFlowRate, mTolerance);

    /// - Set delta pressure to be beyond limit for diffusion calculation (Node 1 press higher than Node 0).
    double press0 = 101.0;
    double press1 = 101.011;
    PolyFluidInputData fluidInput0d(263.0, press0, 0.0, 0.0, fractions0);
    PolyFluidInputData fluidInput1d(263.0, press1, 0.0, 0.0, fractions1);
    mNodes[0].initialize("UtNode1", mFluidConfig);
    mNodes[1].initialize("UtNode2", mFluidConfig);
    mNodes[0].getContent()->initialize(*mFluidConfig, fluidInput0d);
    mNodes[1].getContent()->initialize(*mFluidConfig, fluidInput1d);
    mNodes[0].resetFlows();
    mNodes[1].resetFlows();
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->mPotentialDrop = press1 - press0;
    mArticle->mFlowRate = 0.0;
    mArticle->updateFluid(mTimeStep, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mDiffusiveFlowRate, mTolerance);

    /// - Set delta pressure to be beyond limit for diffusion calculation (Node 1 press lower than Node 0).
    press0 = 101.011;
    press1 = 101.0;
    PolyFluidInputData fluidInput0e(263.0, press0, 0.0, 0.0, fractions0);
    PolyFluidInputData fluidInput1e(263.0, press1, 0.0, 0.0, fractions1);
    mNodes[0].initialize("UtNode1", mFluidConfig);
    mNodes[1].initialize("UtNode2", mFluidConfig);
    mNodes[0].getContent()->initialize(*mFluidConfig, fluidInput0e);
    mNodes[1].getContent()->initialize(*mFluidConfig, fluidInput1e);
    mNodes[0].resetFlows();
    mNodes[1].resetFlows();
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->mPotentialDrop = press1 - press0;
    mArticle->mFlowRate = 0.0;
    mArticle->updateFluid(mTimeStep, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mDiffusiveFlowRate, mTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Hatch link model update fluid with heat conduction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHatch::testUpdateFluidConduct()
{
    UT_RESULT;

    /// - Initialize default test article with port fluids at different temperature but with same
    ///   mole fractions
    PolyFluidInputData fluidInput(273.0, 109.0, 0.0, 0.0, mFractions);
    mNodes[0].initialize("UtNode1", mFluidConfig);
    mNodes[1].initialize("UtNode2", mFluidConfig);
    mNodes[0].getContent()->initialize(*mFluidConfig, fluidInput);
    mNodes[1].getContent()->initialize(*mFluidConfig, fluidInput);
    mNodes[0].resetFlows();
    mNodes[1].resetFlows();
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @todo     Compute real expected values.
    mArticle->updateFluid(mTimeStep, 0.0);
    const double expected  = mArticle->mFlowRate;
    const double expected0 = 273.0;
    const double expected1 = 273.0;

    /// @test     Mass flux and node fluid temperatures.
    mArticle->updateFluid(mTimeStep, 0.0);
    const double returned  = mArticle->mFlowRate;
    const double returned0 = mNodes[0].getOutflow()->getTemperature();
    const double returned1 = mNodes[1].getOutflow()->getTemperature();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,  returned,  mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected0, returned0, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected1, returned1, mTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Hatch link model update fluid with both gas diffusion and
///           heat conduction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHatch::testUpdateFluidBoth()
{
    UT_RESULT;

    /// - Initialize default test article with port fluids at different temperature and with
    ///   different mole fractions
    double fractions0[2] = {0.4, 0.6};
    PolyFluidInputData fluidInput0(273.0, 109.0, 0.0, 0.0, fractions0);
    double fractions1[2] = {0.6, 0.4};
    PolyFluidInputData fluidInput1(293.0, 109.0, 0.0, 0.0, fractions1);
    mNodes[0].initialize("UtNode1", mFluidConfig);
    mNodes[1].initialize("UtNode2", mFluidConfig);
    mNodes[0].getContent()->initialize(*mFluidConfig, fluidInput0);
    mNodes[1].getContent()->initialize(*mFluidConfig, fluidInput1);
    mNodes[0].resetFlows();
    mNodes[1].resetFlows();
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @todo     Compute real expected value.
    mArticle->mFlowRate = mFlowRate;
    mArticle->updateFluid(mTimeStep, mFlowRate);
    const double expected  = mArticle->mFlowRate - mFlowRate;
    const double expected0 = mNodes[0].getOutflow()->getTemperature();
    const double expected1 = mNodes[1].getOutflow()->getTemperature();
    mNodes[0].getContent()->initialize(*mFluidConfig, fluidInput0);
    mNodes[1].getContent()->initialize(*mFluidConfig, fluidInput1);
    mNodes[0].resetFlows();
    mNodes[1].resetFlows();
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test     Mass flux and node fluid temperatures.
    mArticle->mFlowRate = mFlowRate;
    mArticle->updateFluid(mTimeStep, mFlowRate);
    const double returned  = mArticle->mFlowRate - mFlowRate;
    const double returned0 = mNodes[0].getOutflow()->getTemperature();
    const double returned1 = mNodes[1].getOutflow()->getTemperature();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,  returned,  mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected0, returned0, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected1, returned1, mTolerance);

    /// - Set up pointers to the nodes.
    FriendlyGunnsFluidNode* nodePtrs[3];
    nodePtrs[0] = static_cast<FriendlyGunnsFluidNode*>(&mNodes[0]);
    nodePtrs[1] = static_cast<FriendlyGunnsFluidNode*>(&mNodes[1]);
    nodePtrs[2] = static_cast<FriendlyGunnsFluidNode*>(&mNodes[2]);

    /// @test     Effects are zeroed if the time step is zero.
    mArticle->mFlowRate           = mFlowRate;
    mArticle->mDiffusiveFlowRate  = 1.0;
    mArticle->mConductiveHeatFlux = 1.0;
    mNodes[0].resetFlows();
    mNodes[1].resetFlows();
    mNodes[2].resetFlows();
    mArticle->updateFluid(0.0, mFlowRate);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mDiffusiveFlowRate,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mConductiveHeatFlux, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, nodePtrs[0]->mInflowHeatFlux,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, nodePtrs[1]->mInflowHeatFlux,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, nodePtrs[2]->mInflowHeatFlux,  0.0);

    /// @test     Effects are zeroed if the port 0 node is Vacuum.
    mArticle->setPort(0, 2, "unit test", false);
    CPPUNIT_ASSERT_EQUAL(GunnsBasicLink::READY, mArticle->mUserPortSetControl);

    mArticle->mFlowRate           = mFlowRate;
    mArticle->mDiffusiveFlowRate  = 1.0;
    mArticle->mConductiveHeatFlux = 1.0;
    mNodes[0].resetFlows();
    mNodes[1].resetFlows();
    mNodes[2].resetFlows();
    mArticle->updateFluid(mTimeStep, mFlowRate);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mDiffusiveFlowRate,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mConductiveHeatFlux, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, nodePtrs[0]->mInflowHeatFlux,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, nodePtrs[1]->mInflowHeatFlux,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, nodePtrs[2]->mInflowHeatFlux,  0.0);

    /// @test     Effects are zeroed if the port 1 node is Vacuum.
    mArticle->setPort(0, 0, "unit test", false);
    mArticle->setPort(1, 2, "unit test", false);
    CPPUNIT_ASSERT_EQUAL(GunnsBasicLink::READY, mArticle->mUserPortSetControl);

    mArticle->mFlowRate           = mFlowRate;
    mArticle->mDiffusiveFlowRate  = 1.0;
    mArticle->mConductiveHeatFlux = 1.0;
    mNodes[0].resetFlows();
    mNodes[1].resetFlows();
    mNodes[2].resetFlows();
    mArticle->updateFluid(mTimeStep, mFlowRate);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mDiffusiveFlowRate,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mConductiveHeatFlux, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, nodePtrs[0]->mInflowHeatFlux,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, nodePtrs[1]->mInflowHeatFlux,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, nodePtrs[2]->mInflowHeatFlux,  0.0);

    /// @test     Effects are zeroed if port 0 node has zero volume.
    mArticle->setPort(1, 1, "unit test", false);
    CPPUNIT_ASSERT_EQUAL(GunnsBasicLink::READY, mArticle->mUserPortSetControl);

    mNodes[0].setVolume(0.0);
    mArticle->mFlowRate           = mFlowRate;
    mArticle->mDiffusiveFlowRate  = 1.0;
    mArticle->mConductiveHeatFlux = 1.0;
    mNodes[0].resetFlows();
    mNodes[1].resetFlows();
    mNodes[2].resetFlows();
    mArticle->updateFluid(mTimeStep, mFlowRate);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mDiffusiveFlowRate,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mConductiveHeatFlux, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, nodePtrs[0]->mInflowHeatFlux,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, nodePtrs[1]->mInflowHeatFlux,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, nodePtrs[2]->mInflowHeatFlux,  0.0);
    mNodes[0].setVolume(1.0);

    /// @test     Effects are zeroed if port 1 node has zero volume.
    mNodes[1].setVolume(0.0);
    mArticle->mFlowRate           = mFlowRate;
    mArticle->mDiffusiveFlowRate  = 1.0;
    mArticle->mConductiveHeatFlux = 1.0;
    mNodes[0].resetFlows();
    mNodes[1].resetFlows();
    mNodes[2].resetFlows();
    mArticle->updateFluid(mTimeStep, mFlowRate);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mDiffusiveFlowRate,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mConductiveHeatFlux, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, nodePtrs[0]->mInflowHeatFlux,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, nodePtrs[1]->mInflowHeatFlux,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, nodePtrs[2]->mInflowHeatFlux,  0.0);
    mNodes[1].setVolume(1.0);

    /// @test    Check updateFluid's called functions Outflow Parameters
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mNodes[0].getContent()->setTemperature(50);
    mNodes[1].getContent()->setTemperature(60);
    mNodes[0].getContent()->setPressure(80);
    mNodes[1].getContent()->setPressure(90);
    mNodes[0].resetFlows();
    mNodes[1].resetFlows();
    mNodes[0].getContent()->setTemperature(283.0);
    mNodes[0].getContent()->setPressure(109.0);
    mNodes[1].getContent()->setTemperature(283.0);
    mNodes[1].getContent()->setPressure(111.0);
    mArticle->updateFluid(mTimeStep,mFlowRate);
    double tempDiffusiveFlowRate = GunnsFluidUtils::computeGasDiffusion(mArticle->mDiffusiveFluid,
                                                                  mArticle->mEffectiveConductivity,
                                                                  mNodes[0].getOutflow(),
                                                                  mNodes[1].getOutflow(),
                                                                  mArticle->mFlowRate,
                                                                  mArticle->mLength0,
                                                                  mArticle->mLength1);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tempDiffusiveFlowRate, mArticle->mDiffusiveFlowRate, DBL_EPSILON);

    double tempConductiveHeatFlux = GunnsFluidUtils::computeConductiveHeatFlux(
                                                                mArticle->mEffectiveConductivity,
                                                                mNodes[0].getOutflow(),
                                                                mNodes[1].getOutflow(),
                                                                mArticle->mLength0,
                                                                mArticle->mLength1);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tempConductiveHeatFlux, mArticle->mConductiveHeatFlux, DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Hatch link model transport fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHatch::testTransportFluid()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    Internal fluid and mass flow rate .
    const PolyFluid* expectedFluid = mArticle->mInternalFluid;
    const double expectedFlowRate  = mArticle->mFlowRate;
    mArticle->transportFluid(true);
    CPPUNIT_ASSERT(expectedFluid == mArticle->mInternalFluid);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlowRate,  mArticle->mFlowRate,  mTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Hatch link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHatch::testInitializationExceptions()
{
    UT_RESULT;

    /// - Default construct a test article.
    GunnsFluidHatch article;

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

    /// @test    Initialization exception on invalid config data: mLength0 + mLength1 < FLT_EPSILON.
    mConfigData->mLength0  = 0.25 * FLT_EPSILON;
    mConfigData->mLength1  = 0.25 * FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mLength0  = mLength0;
    mConfigData->mLength1  = mLength1;

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

    /// @test    Initialization exception on invalid input data: mMalfLeakThruValue < 0.
    mInputData->mMalfLeakThruValue = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mInputData->mMalfLeakThruValue = mMalfLeakThruValue;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Hatch link model restart.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHatch::testRestart()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// - Set non-config & non-checkpointed terms.
    mArticle->mEffectiveConductivity = 1.0;
    mArticle->mDiffusiveFlowRate     = 1.0;
    mArticle->mConductiveHeatFlux    = 1.0;

    /// @test    Terms are reset in this class and the base class.
    mArticle->restart();

    CPPUNIT_ASSERT_EQUAL(0.0, mArticle->mEffectiveConductivity);
    CPPUNIT_ASSERT_EQUAL(0.0, mArticle->mDiffusiveFlowRate);
    CPPUNIT_ASSERT_EQUAL(0.0, mArticle->mConductiveHeatFlux);

    UT_PASS_LAST;
}
