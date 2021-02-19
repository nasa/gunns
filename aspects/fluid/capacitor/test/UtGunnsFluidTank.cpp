/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/fluid/capacitor/GunnsFluidTank.o))
***************************************************************************************************/

#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidTank.hh"

/// @details  Test identification number.
int UtGunnsFluidTank::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Tank link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidTank::UtGunnsFluidTank()
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
    mVolume(0.0),
    mShellTemperature(0.0),
    mBiasHeatFlux(0.0),
    mSurfaceArea(0.0),
    mShellRadius(0.0),
    mExpansionScaleFactor(0.0),
    mDpdtFilterGain(0.0),
    mThermalDampingMass(0.0),
    mEditFluxTarget(0.0),
    mConfigData(0),
    mInputData(0),
    mArticle(0),
    mFlowRate(0),
    mTimeStep(0.0),
    mTolerance(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Tank link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidTank::~UtGunnsFluidTank()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTank::setUp()
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
    mVolume               = 0.5;
    mExpansionScaleFactor = 0.5;
    mDpdtFilterGain       = 0.1;
    mThermalDampingMass   = 1.0;
    mEditFluxTarget       = 1.0E-7;
    mShellTemperature     = 300.0;
    mBiasHeatFlux         = 10.0;
    mShellRadius          = 0.0;
    mSurfaceArea          = 0.0;
    mConfigData           = new GunnsFluidTankConfigData(mName,
                                                         &mNodeList,
                                                         mExpansionScaleFactor,
                                                         mDpdtFilterGain,
                                                         mThermalDampingMass,
                                                         mEditFluxTarget,
                                                         mSurfaceArea,
                                                         mShellRadius);

    /// - Define the nominal input data.
    mInputData            = new GunnsFluidTankInputData(false, 0.0,
                                                        mVolume,
                                                        mFluidInput0,
                                                        mShellTemperature,
                                                        mBiasHeatFlux);

    /// - Define the nominal port mapping.
    mPort0                = 0;
    mPort1                = 1;

    /// - Default construct the nominal test article.
    mArticle              = new FriendlyGunnsFluidTank;

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
void UtGunnsFluidTank::tearDown()
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
/// @details  Tests for construction of GUNNS Fluid Tank link model configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTank::testConfigAndInput()
{
    UT_RESULT_INITIAL("GUNNS Fluid Capacitor Links");

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(mName                 == mConfigData->mName);
    CPPUNIT_ASSERT(mNodes                == mConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(mExpansionScaleFactor == mConfigData->mExpansionScaleFactor);
    CPPUNIT_ASSERT(mDpdtFilterGain       == mConfigData->mDpdtFilterGain);
    CPPUNIT_ASSERT(mThermalDampingMass   == mConfigData->mThermalDampingMass);
    CPPUNIT_ASSERT(mEditFluxTarget       == mConfigData->mEditFluxTarget);
    CPPUNIT_ASSERT(mSurfaceArea          == mConfigData->mSurfaceArea);
    CPPUNIT_ASSERT(mShellRadius          == mConfigData->mShellRadius);

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(mFluidInput0          == mInputData->mInitialFluidState);
    CPPUNIT_ASSERT(mVolume               == mInputData->mInitialVolume);
    CPPUNIT_ASSERT(mShellTemperature     == mInputData->mShellTemperature);
    CPPUNIT_ASSERT(mBiasHeatFlux         == mInputData->mBiasHeatFlux);

    /// @test    Configuration data default construction.
    GunnsFluidTankConfigData defaultConfig;
    CPPUNIT_ASSERT(""                    == defaultConfig.mName);
    CPPUNIT_ASSERT(0                     == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mDpdtFilterGain);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mThermalDampingMass);
    CPPUNIT_ASSERT(1.0E-6                == defaultConfig.mEditFluxTarget);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mSurfaceArea);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mShellRadius);

    /// @test    Input data default construction.
    GunnsFluidTankInputData defaultInput;
    CPPUNIT_ASSERT(0                     == defaultInput.mInitialFluidState);
    CPPUNIT_ASSERT(0.0                   == defaultInput.mInitialVolume);
    CPPUNIT_ASSERT(0.0                   == defaultInput.mShellTemperature);
    CPPUNIT_ASSERT(0.0                   == defaultInput.mBiasHeatFlux);

    /// @test    Configuration data copy construction.
    GunnsFluidTankConfigData copyConfig(*mConfigData);
    CPPUNIT_ASSERT(mName                 == copyConfig.mName);
    CPPUNIT_ASSERT(mNodes                == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(mExpansionScaleFactor == copyConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(mDpdtFilterGain       == copyConfig.mDpdtFilterGain);
    CPPUNIT_ASSERT(mThermalDampingMass   == copyConfig.mThermalDampingMass);
    CPPUNIT_ASSERT(mEditFluxTarget       == copyConfig.mEditFluxTarget);
    CPPUNIT_ASSERT(mSurfaceArea          == copyConfig.mSurfaceArea);
    CPPUNIT_ASSERT(mShellRadius          == copyConfig.mShellRadius);

    /// @test    Input data copy construction.
    GunnsFluidTankInputData copyInput(*mInputData);
    CPPUNIT_ASSERT(mFluidInput0          == copyInput.mInitialFluidState);
    CPPUNIT_ASSERT(mVolume               == copyInput.mInitialVolume);
    CPPUNIT_ASSERT(mShellTemperature     == copyInput.mShellTemperature);
    CPPUNIT_ASSERT(mBiasHeatFlux         == copyInput.mBiasHeatFlux);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Tank link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTank::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default construction configuration data.
    CPPUNIT_ASSERT(""    == mArticle->mName);
    CPPUNIT_ASSERT(0     == mArticle->mNodes);

    /// @test    Default construction state data.
    CPPUNIT_ASSERT(0.0   == mArticle->mCapacitance);
    CPPUNIT_ASSERT(0.0   == mArticle->mFlowRate);
    CPPUNIT_ASSERT(0     == mArticle->mInternalFluid);
    CPPUNIT_ASSERT(0     == mArticle->mNConstituents);
    CPPUNIT_ASSERT(false == mArticle->mEditTemperatureFlag);
    CPPUNIT_ASSERT(false == mArticle->mEditTemperaturePressureFlag);
    CPPUNIT_ASSERT(false == mArticle->mEditTemperaturePartialPressureFlag);
    CPPUNIT_ASSERT(0     == mArticle->mEditPartialPressureRateFlag);
    CPPUNIT_ASSERT(0.0   == mArticle->mEditTemperatureValue);
    CPPUNIT_ASSERT(0.0   == mArticle->mEditPressureValue);
    CPPUNIT_ASSERT(0     == mArticle->mEditPartialPressureValue);
    CPPUNIT_ASSERT(0     == mArticle->mEditPartialPressureRateValue);
    CPPUNIT_ASSERT(0.0   == mArticle->mEditFluxTarget);
    CPPUNIT_ASSERT(0.0   == mArticle->mHeatFluxFromShell);
    CPPUNIT_ASSERT(0.0   == mArticle->mHeatFluxToShell);
    CPPUNIT_ASSERT(0.0   == mArticle->mPreviousPressure);
    CPPUNIT_ASSERT(0.0   == mArticle->mDpdt);
    CPPUNIT_ASSERT(0.0   == mArticle->mDpdtFilterGain);
    CPPUNIT_ASSERT(0     == mArticle->mPartialPressure);
    CPPUNIT_ASSERT(0     == mArticle->mMassFraction);
    CPPUNIT_ASSERT(0     == mArticle->mMoleFraction);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(!mArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsFluidTank* article = new GunnsFluidTank();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Tank link model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTank::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    FriendlyGunnsFluidTank article;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    Nominal configuration data.
    CPPUNIT_ASSERT(mName == article.mName);
    CPPUNIT_ASSERT(mNodes == mConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mVolume,
                                 article.mNodes[0]->getVolume(),
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mExpansionScaleFactor,
                                 mConfigData->mExpansionScaleFactor,
                                 mTolerance);

    /// @test    Nominal state data.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(article.mPotentialVector[0],
                                 article.mNodes[0]->getPotential(),
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNodes[0].getContent()->getPressure(),
                                 article.mNodes[0]->getContent()->getPressure(),
                                 mTolerance);
    CPPUNIT_ASSERT(article.mNodes[0]->getMass() > 0.0);
    CPPUNIT_ASSERT(0                          != article.mInternalFluid);
    CPPUNIT_ASSERT(2                          == article.mNConstituents);
    CPPUNIT_ASSERT(false                      == article.mEditPartialPressureRateFlag[1]);
    CPPUNIT_ASSERT(0.0                        == article.mEditPartialPressureValue[1]);
    CPPUNIT_ASSERT(0.0                        == article.mEditPartialPressureRateValue[1]);
    CPPUNIT_ASSERT(0.0                        <  article.mPartialPressure[1]);
    CPPUNIT_ASSERT(0.0                        <  article.mMassFraction[1]);
    CPPUNIT_ASSERT(0.0                        <  article.mMoleFraction[1]);
    CPPUNIT_ASSERT(mThermalDampingMass        == mNodes[0].mThermalDampingMass);
    CPPUNIT_ASSERT(0.0                        == article.mHeatFluxFromShell);
    CPPUNIT_ASSERT(0.0                        == article.mHeatFluxToShell);
    CPPUNIT_ASSERT(mFluidInput0->mTemperature == article.mTemperature);
    CPPUNIT_ASSERT(mNodes[0].getPotential()   == article.mPreviousPressure);
    CPPUNIT_ASSERT(0.0                        == article.mDpdt);
    CPPUNIT_ASSERT(mDpdtFilterGain            == article.mDpdtFilterGain);
    CPPUNIT_ASSERT(mEditFluxTarget            == article.mEditFluxTarget);
    CPPUNIT_ASSERT(mSurfaceArea               == article.mSurfaceArea);
    CPPUNIT_ASSERT(mShellRadius               == article.mShellRadius);
    CPPUNIT_ASSERT(mShellTemperature          == article.mShellTemperature);
    CPPUNIT_ASSERT(mBiasHeatFlux              == article.mBiasHeatFlux);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(article.mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Tank link model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTank::testAccessors()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    Get volume.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mVolume, mArticle->getVolume(),   mTolerance);

    /// @test    Set up a heat flux from the tank shell, which will normally come from the sim bus,
    ///          and verify the getHeatFlux method.
    mArticle->mHeatFluxFromShell = 1.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0 + mBiasHeatFlux,     mArticle->getHeatFlux(), 0.0);

    /// @test    Get Dpdt.
    mArticle->mDpdt = 4.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, mArticle->getDpdt(),   0.0);

    /// @test    Get mBiasHeatFlux.
    double tempBiasHeatFlux = mArticle->mBiasHeatFlux;
    mArticle->mBiasHeatFlux = 2.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, mArticle->getBiasHeatFlux(),   0.0);
    mArticle->mBiasHeatFlux = tempBiasHeatFlux;

    /// @test    Get Partial Pressure.
    mArticle->mPartialPressure[0] = 5.0;
    mArticle->mPartialPressure[1] = 7.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, mArticle->getPartialPressure()[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0, mArticle->getPartialPressure()[1], 0.0);

    /// @test    Set temperature edit.
    mArticle->editTemperature(true, 290.0);
    CPPUNIT_ASSERT(true  == mArticle->mEditTemperatureFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(290.0, mArticle->mEditTemperatureValue, FLT_EPSILON);

    /// @test    Reset temperature edit.
    mArticle->editTemperature();
    CPPUNIT_ASSERT(false == mArticle->mEditTemperatureFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(290.0, mArticle->mEditTemperatureValue, FLT_EPSILON);

    /// @test    Set temperature and total pressure edit.
    mArticle->editTemperaturePressure(true, 290.0, 90.0);
    CPPUNIT_ASSERT(true  == mArticle->mEditTemperaturePressureFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(290.0, mArticle->mEditTemperatureValue, FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 90.0, mArticle->mEditPressureValue,    FLT_EPSILON);

    /// @test    Reset temperature and total pressure edit.
    mArticle->editTemperaturePressure();
    CPPUNIT_ASSERT(false == mArticle->mEditTemperaturePressureFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(290.0, mArticle->mEditTemperatureValue, FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 90.0, mArticle->mEditPressureValue,    FLT_EPSILON);

    /// @test    Set temperature and partial pressures edit.
    double pp[N_FLUIDS] = {70.0, 30.0};
    mArticle->editTemperaturePartialPressure(true, 290.0, pp);
    CPPUNIT_ASSERT(true  == mArticle->mEditTemperaturePartialPressureFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(290.0, mArticle->mEditTemperatureValue,        FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 70.0, mArticle->mEditPartialPressureValue[0], FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 30.0, mArticle->mEditPartialPressureValue[1], FLT_EPSILON);

    /// @test    Reset temperature and partial pressures edit.
    mArticle->editTemperaturePartialPressure();
    CPPUNIT_ASSERT(false == mArticle->mEditTemperaturePartialPressureFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(290.0, mArticle->mEditTemperatureValue,        FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 70.0, mArticle->mEditPartialPressureValue[0], FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 30.0, mArticle->mEditPartialPressureValue[1], FLT_EPSILON);

    /// @test    Temperature & partial pressures edit method does nothing to the temperature if no
    ///          value is supplied, and nothing to the partial pressures if no partial pressure
    ///          array is supplied.
    mArticle->editTemperaturePartialPressure(true, 0.0, 0);
    CPPUNIT_ASSERT(true  == mArticle->mEditTemperaturePartialPressureFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(290.0, mArticle->mEditTemperatureValue,        FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 70.0, mArticle->mEditPartialPressureValue[0], FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 30.0, mArticle->mEditPartialPressureValue[1], FLT_EPSILON);

    /// @test    Set partial pressure rates edit.
    mArticle->editPartialPressureRate(FluidProperties::GUNNS_O2, true, 70.0, 1.0);
    CPPUNIT_ASSERT(true  == mArticle->mEditPartialPressureRateFlag[1]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 70.0, mArticle->mEditPartialPressureValue[1],     FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, mArticle->mEditPartialPressureRateValue[1], FLT_EPSILON);

    /// @test    Partial pressure rate edit method does nothing if no constituent supplied.
    mArticle->editPartialPressureRate();
    CPPUNIT_ASSERT(true  == mArticle->mEditPartialPressureRateFlag[1]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 70.0, mArticle->mEditPartialPressureValue[1],     FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, mArticle->mEditPartialPressureRateValue[1], FLT_EPSILON);

    /// @test    Reset partial pressure rates edit for the constituent.
    mArticle->editPartialPressureRate(FluidProperties::GUNNS_O2);
    CPPUNIT_ASSERT(false == mArticle->mEditPartialPressureRateFlag[1]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 70.0, mArticle->mEditPartialPressureValue[1],     FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, mArticle->mEditPartialPressureRateValue[1], FLT_EPSILON);

    /// @test    Exception thrown on invalid fluid type arg to partial pressure rate edit.
    CPPUNIT_ASSERT_THROW(mArticle->editPartialPressureRate(FluidProperties::GUNNS_AMMONIA),
                         TsOutOfBoundsException);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Tank link model edit temperature and update fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTank::testEditTemperature()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// - Finish node initialization.
    mNodes[0].prepareForStart();
    mNodes[0].validate();

    /// @test    Edit temperature to 300 and pressure to 700.
    double expectedTemperature             = 300.0;
    double expectedPressure                = mNodes[0].getPotential();
    mArticle->mEditTemperatureValue        = expectedTemperature;
    mArticle->mEditTemperatureFlag         = true;

    /// @test    Update fluid.
    mArticle->updateFluid(mTimeStep, mFlowRate);
    mNodes[0].computeMassError();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTemperature,
                                 mArticle->mNodes[0]->getContent()->getTemperature(),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPressure,
                                 mArticle->mNodes[0]->getContent()->getPressure(),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPressure, mArticle->mPotentialVector[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              mNodes[0].mMassError,          DBL_EPSILON);
    CPPUNIT_ASSERT(false  == mArticle->mEditTemperatureFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Tank link model edit temperature and pressure and update fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTank::testEditTemperatureAndPressure()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// - Finish node initialization.
    mNodes[0].prepareForStart();
    mNodes[0].validate();

    /// @test    Edit temperature to 300 and pressure to 700.
    double expectedTemperature             = 300.0;
    double expectedPressure                = 700.0;
    mArticle->mEditTemperatureValue        = expectedTemperature;
    mArticle->mEditPressureValue           = expectedPressure;
    mArticle->mEditTemperaturePressureFlag = true;

    /// @test    Update fluid.
    mArticle->updateFluid(mTimeStep, mFlowRate);
    mNodes[0].computeMassError();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTemperature,
                                 mArticle->mNodes[0]->getContent()->getTemperature(),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPressure,
                                 mArticle->mNodes[0]->getContent()->getPressure(),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPressure, mArticle->mPotentialVector[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              mNodes[0].mMassError,          DBL_EPSILON);
    CPPUNIT_ASSERT(true == mArticle->mEditTemperaturePressureFlag);
    CPPUNIT_ASSERT(true == mArticle->mDisablePressureCorrection);
    CPPUNIT_ASSERT(true == mArticle->mOverrideVector[0]);

    /// @test    Check for proper shutdown of the edit when the user manually requests it.
    mArticle->mEditTemperaturePressureFlag = false;
    mArticle->updateFluid(mTimeStep, mFlowRate);
    CPPUNIT_ASSERT(false == mArticle->mDisablePressureCorrection);
    CPPUNIT_ASSERT(false == mArticle->mOverrideVector[0]);

    /// - Start a new edit with different values.
    expectedTemperature                    = 250.0;
    expectedPressure                       = 650.0;
    mArticle->mEditTemperatureValue        = expectedTemperature;
    mArticle->mEditPressureValue           = expectedPressure;
    mArticle->mEditTemperaturePressureFlag = true;

    mArticle->updateFluid(mTimeStep, mFlowRate);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTemperature,
                                 mArticle->mNodes[0]->getContent()->getTemperature(),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPressure,
                                 mArticle->mNodes[0]->getContent()->getPressure(),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPressure, mArticle->mPotentialVector[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              mNodes[0].mMassError,          DBL_EPSILON);
    CPPUNIT_ASSERT(true == mArticle->mEditTemperaturePressureFlag);
    CPPUNIT_ASSERT(true == mArticle->mDisablePressureCorrection);
    CPPUNIT_ASSERT(true == mArticle->mOverrideVector[0]);

    /// @test     Check for non-shutdown of the edit when temperature hasn't converged.
    mNodes[0].getContent()->setTemperature(expectedTemperature + 0.1);
    mArticle->updateFluid(mTimeStep, mFlowRate);
    mNodes[0].setPotential(expectedPressure);
    CPPUNIT_ASSERT(true == mArticle->mEditTemperaturePressureFlag);
    CPPUNIT_ASSERT(true == mArticle->mDisablePressureCorrection);
    CPPUNIT_ASSERT(true == mArticle->mOverrideVector[0]);

    /// @test     Check for non-shutdown of the edit when pressure hasn't converged.
    mNodes[0].setPotential(expectedPressure + 0.1);
    mArticle->updateFluid(mTimeStep, mFlowRate);
    CPPUNIT_ASSERT(true == mArticle->mEditTemperaturePressureFlag);
    CPPUNIT_ASSERT(true == mArticle->mDisablePressureCorrection);
    CPPUNIT_ASSERT(true == mArticle->mOverrideVector[0]);

    /// @test     Check for non-shutdown of the edit when node flows haven't stabilized.
    mNodes[0].mNetFlux = mFlowRate;
    mNodes[0].setPotential(expectedPressure);
    mArticle->updateFluid(mTimeStep, mFlowRate);
    CPPUNIT_ASSERT(true == mArticle->mEditTemperaturePressureFlag);
    CPPUNIT_ASSERT(true == mArticle->mDisablePressureCorrection);
    CPPUNIT_ASSERT(true == mArticle->mOverrideVector[0]);

    /// @test     Check for automatic shutdown of the edit.
    mNodes[0].mNetFlux = 0.0;
    mArticle->updateFluid(mTimeStep, mFlowRate);
    CPPUNIT_ASSERT(false == mArticle->mEditTemperaturePressureFlag);
    CPPUNIT_ASSERT(false == mArticle->mDisablePressureCorrection);
    CPPUNIT_ASSERT(false == mArticle->mOverrideVector[0]);

    /// @test     Start a new edit where they forgot to set the temperature or pressure targets and
    ///           verify the tank keeps the current temperature and pressure.  Verify edit shutdown
    ///           since node was already at target properties.
    expectedTemperature                    = mArticle->mNodes[0]->getContent()->getTemperature();
    expectedPressure                       = mArticle->mNodes[0]->getContent()->getPressure();
    mArticle->mEditTemperatureValue        = 0.0;
    mArticle->mEditPressureValue           = 0.0;
    mArticle->mEditTemperaturePressureFlag = true;

    mArticle->updateFluid(mTimeStep, mFlowRate);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTemperature, mArticle->mEditTemperatureValue, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPressure,    mArticle->mEditPressureValue,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTemperature,
                                 mArticle->mNodes[0]->getContent()->getTemperature(),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPressure,
                                 mArticle->mNodes[0]->getContent()->getPressure(),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPressure, mArticle->mPotentialVector[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              mNodes[0].mMassError,          DBL_EPSILON);
    CPPUNIT_ASSERT(false == mArticle->mEditTemperaturePressureFlag);
    CPPUNIT_ASSERT(false == mArticle->mDisablePressureCorrection);
    CPPUNIT_ASSERT(false == mArticle->mOverrideVector[0]);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Tank link model edit temperature and partial pressure and update
///           fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTank::testEditTemperatureAndPartialPressure()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// - Finish node initialization.
    mNodes[0].prepareForStart();
    mNodes[0].validate();

    /// @test    Edit temperature to 290 and partial pressure to (400, 300).
    double expectedTemperature                    = 290.0;
    double expectedPartialPressure[2]             = { 400.0, 300.0 };
    double expectedPressure                       = expectedPartialPressure[0]
                                                  + expectedPartialPressure[1];
    mArticle->mEditTemperatureValue               = expectedTemperature;
    mArticle->mEditPartialPressureValue[0]        = expectedPartialPressure[0];
    mArticle->mEditPartialPressureValue[1]        = expectedPartialPressure[1];
    mArticle->mEditTemperaturePartialPressureFlag = true;

    /// @test    Update fluid.
    mArticle->updateFluid(mTimeStep, mFlowRate);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTemperature,
                                 mArticle->mNodes[0]->getContent()->getTemperature(),
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPartialPressure[0],
                                 mArticle->mNodes[0]->getContent()->
                                 getPartialPressure(FluidProperties::GUNNS_N2),
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPartialPressure[1],
                                 mArticle->mNodes[0]->getContent()->
                                 getPartialPressure(FluidProperties::GUNNS_O2),
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPressure, mArticle->mPotentialVector[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              mNodes[0].mMassError,          DBL_EPSILON);
    CPPUNIT_ASSERT(true == mArticle->mEditTemperaturePartialPressureFlag);
    CPPUNIT_ASSERT(true == mArticle->mDisablePressureCorrection);
    CPPUNIT_ASSERT(true == mArticle->mOverrideVector[0]);

    /// @test    Check for proper shutdown of the edit when the user manually requests it.
    mArticle->mEditTemperaturePartialPressureFlag = false;
    mArticle->updateFluid(mTimeStep, mFlowRate);
    CPPUNIT_ASSERT(false == mArticle->mDisablePressureCorrection);
    CPPUNIT_ASSERT(false == mArticle->mOverrideVector[0]);

    /// - Start a new edit with different values.
    expectedTemperature                           = 250.0;
    expectedPartialPressure[0]                    = 450.0;
    expectedPartialPressure[0]                    = 350.0;
    expectedPressure                              = expectedPartialPressure[0]
                                                  + expectedPartialPressure[1];
    mArticle->mEditTemperatureValue               = expectedTemperature;
    mArticle->mEditPartialPressureValue[0]        = expectedPartialPressure[0];
    mArticle->mEditPartialPressureValue[1]        = expectedPartialPressure[1];
    mArticle->mEditTemperaturePartialPressureFlag = true;

    mArticle->updateFluid(mTimeStep, mFlowRate);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTemperature,
                                 mArticle->mNodes[0]->getContent()->getTemperature(),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPartialPressure[0],
                                 mArticle->mNodes[0]->getContent()->
                                 getPartialPressure(FluidProperties::GUNNS_N2),
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPartialPressure[1],
                                 mArticle->mNodes[0]->getContent()->
                                 getPartialPressure(FluidProperties::GUNNS_O2),
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPressure,
                                 mArticle->mNodes[0]->getContent()->getPressure(),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPressure, mArticle->mPotentialVector[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              mNodes[0].mMassError,          DBL_EPSILON);
    CPPUNIT_ASSERT(true == mArticle->mEditTemperaturePartialPressureFlag);
    CPPUNIT_ASSERT(true == mArticle->mDisablePressureCorrection);
    CPPUNIT_ASSERT(true == mArticle->mOverrideVector[0]);

    /// @test     Check for non-shutdown of the edit when temperature hasn't converged.
    mNodes[0].getContent()->setTemperature(expectedTemperature + 0.1);
    mArticle->updateFluid(mTimeStep, mFlowRate);
    mNodes[0].setPotential(expectedPressure);
    CPPUNIT_ASSERT(true == mArticle->mEditTemperaturePartialPressureFlag);
    CPPUNIT_ASSERT(true == mArticle->mDisablePressureCorrection);
    CPPUNIT_ASSERT(true == mArticle->mOverrideVector[0]);

    /// @test     Check for non-shutdown of the edit when pressure hasn't converged.
    mNodes[0].setPotential(expectedPressure + 0.1);
    mArticle->updateFluid(mTimeStep, mFlowRate);
    CPPUNIT_ASSERT(true == mArticle->mEditTemperaturePartialPressureFlag);
    CPPUNIT_ASSERT(true == mArticle->mDisablePressureCorrection);
    CPPUNIT_ASSERT(true == mArticle->mOverrideVector[0]);

    /// @test     Check for non-shutdown of the edit when node flows haven't stabilized.
    mNodes[0].mNetFlux = mFlowRate;
    mNodes[0].setPotential(expectedPressure);
    mArticle->updateFluid(mTimeStep, mFlowRate);
    CPPUNIT_ASSERT(true == mArticle->mEditTemperaturePartialPressureFlag);
    CPPUNIT_ASSERT(true == mArticle->mDisablePressureCorrection);
    CPPUNIT_ASSERT(true == mArticle->mOverrideVector[0]);

    /// @test     Check for automatic shutdown of the edit.
    mNodes[0].mNetFlux = 0.0;
    mArticle->updateFluid(mTimeStep, mFlowRate);
    CPPUNIT_ASSERT(false == mArticle->mEditTemperaturePartialPressureFlag);
    CPPUNIT_ASSERT(false == mArticle->mDisablePressureCorrection);
    CPPUNIT_ASSERT(false == mArticle->mOverrideVector[0]);

    /// @test     Start a new edit where they forgot to set the temperature or partial pressure
    ///           targets and verify the tank keeps the current temperature and pressures.  Verify
    ///           edit shutdown since node was already at target properties.
    mArticle->mEditTemperatureValue               = 0.0;
    mArticle->mEditPartialPressureValue[0]        = 0.0;
    mArticle->mEditPartialPressureValue[1]        = 0.0;
    mArticle->mEditTemperaturePartialPressureFlag = true;

    /// @test    Update fluid.
    mArticle->updateFluid(mTimeStep, mFlowRate);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTemperature,
                                 mArticle->mNodes[0]->getContent()->getTemperature(),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPartialPressure[0],
                                 mArticle->mNodes[0]->getContent()->
                                 getPartialPressure(FluidProperties::GUNNS_N2),
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPartialPressure[1],
                                 mArticle->mNodes[0]->getContent()->
                                 getPartialPressure(FluidProperties::GUNNS_O2),
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPressure,
                                 mArticle->mNodes[0]->getContent()->getPressure(),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPressure, mArticle->mPotentialVector[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              mNodes[0].mMassError,          DBL_EPSILON);
    CPPUNIT_ASSERT(false == mArticle->mEditTemperaturePartialPressureFlag);
    CPPUNIT_ASSERT(false == mArticle->mDisablePressureCorrection);
    CPPUNIT_ASSERT(false == mArticle->mOverrideVector[0]);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Tank link model edit partial pressure rate and update state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTank::testEditPartialPressureRate()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mInputData->mBiasHeatFlux = 0.0;
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// - Finish node initialization.
    mNodes[0].prepareForStart();
    mNodes[1].prepareForStart();
    mNodes[0].validate();
    mNodes[1].validate();

    /// - Initialize partial pressures for the test pass.
    mArticle->processOutputs();

    /// @test    Edit ppO2 up for one step.
    mArticle->mEditPartialPressureRateValue[1] = 1.0;
    double targetPartialPressure               = mArticle->mNodes[0]->getContent()->
                                                 getPartialPressure(FluidProperties::GUNNS_O2) +
                                                 mArticle->mEditPartialPressureRateValue[1] *
                                                 mTimeStep + FLT_EPSILON;
    mArticle->mEditPartialPressureValue[1]     = targetPartialPressure;
    mArticle->mEditPartialPressureRateFlag[1]  = true;

    /// @test    Update state with time step too small.
    mArticle->updateState(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mSourceVector[0], mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mSourceVector[1], mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mFlowRate, mTolerance);

    /// @test    Step with nominal time step.
    mArticle->step(mTimeStep);

    // - Use Ideal Gas Law to find mass of O2 to raise ppO2 at current temperature.  Adjust
    //   for the difference between the node and internal fluid MW:
    double expectedMW   = mFluidProperties->getProperties(FluidProperties::GUNNS_O2)->getMWeight();
    double expectedMdot = mNodes[0].getContent()->getDensity() * mNodes[0].getVolume() * -1.0
                        * expectedMW / mNodes[0].getContent()->getMWeight()
                        / mNodes[0].getPotential();
    double expectedFlux = mNodes[0].getPotential() * mArticle->mCapacitance / mTimeStep
                        - expectedMdot / expectedMW;

    /// @test    Check outputs.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(283.0, mArticle->mInternalFluid->getTemperature(),  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMW, mArticle->mInternalFluid->getMWeight(), FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot, mArticle->mFlowRate,                  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,   mNodes[0].mExpansionScaleFactor,             0.0);
    CPPUNIT_ASSERT(true == mArticle->mEditPartialPressureRateFlag[1]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, mArticle->mSourceVector[0],           FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mSourceVector[1],           FLT_EPSILON);

    // - Mimic the remaining steps that the Gunns solver will take prior to the next pass through
    //   our test article.
    mNodes[0].setPotential(mNodes[0].getPotential() + mArticle->mEditPartialPressureRateValue[1]
                                                      * mTimeStep);
    mArticle->computeFlows(mTimeStep);
    CPPUNIT_ASSERT(GunnsBasicLink::SINK == mArticle->mPortDirections[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == mArticle->mPortDirections[1]);
    mArticle->transportFlows(mTimeStep);
    mNodes[0].integrateFlows(mTimeStep);
    mNodes[0].resetFlows();
    mArticle->processOutputs();

    /// @test    Step again and verify the edit turns itself off.
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                   mArticle->mFlowRate,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mExpansionScaleFactor, mNodes[0].mExpansionScaleFactor, 0.0);
    CPPUNIT_ASSERT(false == mArticle->mEditPartialPressureRateFlag[1]);

    mArticle->computeFlows(mTimeStep);
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == mArticle->mPortDirections[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == mArticle->mPortDirections[1]);
    mArticle->transportFlows(mTimeStep);
    mNodes[0].integrateFlows(mTimeStep);
    mNodes[0].resetFlows();
    mArticle->processOutputs();

    /// @test    Now edit ppN2 down by the same amount.
    mArticle->mEditPartialPressureRateValue[0] = 1.0;
    targetPartialPressure                      = mArticle->mNodes[0]->getContent()->
                                                 getPartialPressure(FluidProperties::GUNNS_N2) -
                                                 mArticle->mEditPartialPressureRateValue[0] *
                                                 mTimeStep - FLT_EPSILON;
    mArticle->mEditPartialPressureValue[0]     = targetPartialPressure;
    mArticle->mEditPartialPressureRateFlag[0]  = true;

    expectedMW   = mFluidProperties->getProperties(FluidProperties::GUNNS_N2)->getMWeight();
    expectedMdot = mNodes[0].getContent()->getDensity() * mNodes[0].getVolume() * 1.0
                        * expectedMW / mNodes[0].getContent()->getMWeight()
                        / mNodes[0].getPotential();

    mArticle->step(mTimeStep);

    /// @test    Check outputs.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(283.0, mArticle->mInternalFluid->getTemperature(),  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMW, mArticle->mInternalFluid->getMWeight(), FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot, mArticle->mFlowRate,                  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,   mNodes[0].mExpansionScaleFactor,             0.0);
    CPPUNIT_ASSERT(true == mArticle->mEditPartialPressureRateFlag[0]);

    mNodes[0].setPotential(mNodes[0].getPotential() - mArticle->mEditPartialPressureRateValue[0]
                                                      * mTimeStep);
    mArticle->computeFlows(mTimeStep);
    CPPUNIT_ASSERT(GunnsBasicLink::SINK == mArticle->mPortDirections[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == mArticle->mPortDirections[1]);
    mArticle->transportFlows(mTimeStep);
    mNodes[0].integrateFlows(mTimeStep);
    mNodes[0].resetFlows();
    mArticle->processOutputs();

    /// @test    Step again and verify the edit turns itself off.
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                   mArticle->mFlowRate,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mExpansionScaleFactor, mNodes[0].mExpansionScaleFactor, 0.0);
    CPPUNIT_ASSERT(false == mArticle->mEditPartialPressureRateFlag[0]);

    /// @test    Edit canceled if supplied rate is zero.
    mArticle->mEditPartialPressureRateValue[1] = 0.0;
    mArticle->mEditPartialPressureValue[1]     = 0.0;
    mArticle->mEditPartialPressureRateFlag[1]  = true;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT(false == mArticle->mEditPartialPressureRateFlag[1]);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Tank link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTank::testInitializationExceptions()
{
    UT_RESULT;

    /// - Default construct a test article.
    GunnsFluidTank article;

    /// @test    Initialization exception on no name.
    mConfigData->mName = "";
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mName = mName;

    /// @test    Initialization exception on negative volume.
    mInputData->mInitialVolume = -0.5;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mInputData->mInitialVolume = mVolume;

    /// @test    Initialization exception on expansion scale factor < 0.
    mConfigData->mExpansionScaleFactor = -0.3;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);

    /// @test    Initialization exception on expansion scale factor > 1.
    mConfigData->mExpansionScaleFactor = 1.1;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mExpansionScaleFactor = mExpansionScaleFactor;

    /// @test    Initialization exception on dp/dt filter gain < 0.
    mConfigData->mDpdtFilterGain = -0.01;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);

    /// @test    Initialization exception on dp/dt filter gain > 1.
    mConfigData->mDpdtFilterGain = 1.01;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mDpdtFilterGain = mDpdtFilterGain;

    /// @test    Initialization exception on thermal damping mass < 0.
    mConfigData->mThermalDampingMass = -0.01;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mThermalDampingMass = mThermalDampingMass;

    /// @test    Initialization exception on edit flux target < 0.
    mConfigData->mEditFluxTarget = -0.01;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mEditFluxTarget = mEditFluxTarget;

    /// @test    Initialization exception on surface area < 0.
    mConfigData->mSurfaceArea = -0.01;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mSurfaceArea = mSurfaceArea;

    /// @test    Initialization exception on shell radius < 0.
    mConfigData->mShellRadius = -0.01;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mShellRadius = mShellRadius;

    /// @test    Initialization exception on null initial fluid state.
    PolyFluidInputData* nullInputData = 0;
    mInputData->mInitialFluidState = nullInputData;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mInputData->mInitialFluidState = mFluidInput0;

    /// @test    Initialization exception on shell temperature < 0.
    mInputData->mShellTemperature = -0.01;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mInputData->mShellTemperature = mShellTemperature;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the processing of output data after the GUNNS network has updated.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTank::testProcessOutputs()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// - Finish initalizing the tank node.
    mNodes[0].prepareForStart();
    mNodes[0].validate();

    const double moleFractN2 = mNodes[0].getContent()->getMoleFraction(FluidProperties::GUNNS_N2);
    const double massFractN2 = mNodes[0].getContent()->getMassFraction(FluidProperties::GUNNS_N2);
    const double moleFractO2 = mNodes[0].getContent()->getMoleFraction(FluidProperties::GUNNS_O2);
    const double massFractO2 = mNodes[0].getContent()->getMassFraction(FluidProperties::GUNNS_O2);
    const double ppN2 = moleFractN2 * mNodes[0].getPotential();
    const double ppO2 = moleFractO2 * mNodes[0].getPotential();
    const double temperature = mNodes[0].getContent()->getTemperature();

    /// - Call the method under test and verify outputs.
    mArticle->processOutputs();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(moleFractN2, mArticle->mMoleFraction[0],    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(moleFractO2, mArticle->mMoleFraction[1],    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(massFractN2, mArticle->mMassFraction[0],    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(massFractO2, mArticle->mMassFraction[1],    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(ppN2,        mArticle->mPartialPressure[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(ppO2,        mArticle->mPartialPressure[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature, mArticle->mTemperature,        DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the dP/dt filter.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTank::testDpdt()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// - Set a typical filter gain.
    mArticle->mDpdtFilterGain = 0.1;

    /// - First pass through the filter should have zero dP/dt if the node pressure isn't changing.
    mArticle->computeFlows(mTimeStep);
    mArticle->transportFlows(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                      mArticle->mDpdt,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNodes[0].getPotential(), mArticle->mPreviousPressure, 0.0);

    /// - Verify filter output in response to a node pressure change.
    mNodes[0].setPotential(mNodes[0].getPotential() + 1.0);

    mArticle->computeFlows(mTimeStep);
    mArticle->transportFlows(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0 * mArticle->mDpdtFilterGain / mTimeStep,
                                                           mArticle->mDpdt, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNodes[0].getPotential(), mArticle->mPreviousPressure, 0.0);

    /// - Test when dt is zero.
    mNodes[0].setPotential(mNodes[0].getPotential() + 1.0);

    mArticle->computeFlows(0.0);
    mArticle->transportFlows(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                      mArticle->mDpdt,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNodes[0].getPotential(), mArticle->mPreviousPressure, 0.0);

    /// - Test when filter gain is zero.
    mNodes[0].setPotential(mNodes[0].getPotential() + 1.0);
    mArticle->mDpdtFilterGain = 0.0;

    mArticle->computeFlows(mTimeStep);
    mArticle->transportFlows(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                      mArticle->mDpdt,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNodes[0].getPotential(), mArticle->mPreviousPressure, 0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Sets up the test article to own its own heat flux, and sets up a default shell
///           temperature.  Verifies the calculation of the heat flux from the shell.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTank::testOwnShellFlux()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    /// - Shell radius = (3/4 * V/pi)^1/3.  We hard-code the pi product as a redundant check on the
    ///   pi constant used by the test article.
    mShellRadius = pow(2.35619449019 * mVolume, (1.0/3.0));
    /// - Shell surface area = 4 pi r^2
    mSurfaceArea = 12.5663706144 * mShellRadius* mShellRadius;
    mConfigData->mShellRadius = mShellRadius;
    mConfigData->mSurfaceArea = mSurfaceArea;
    mInputData->mBiasHeatFlux = 0.0;
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// - Call computeFlows and verify the retrieved heat flux is correct.
    mArticle->computeFlows(0.0);
    mArticle->transportFlows(0.0);

    const double expectedFlux = mSurfaceArea * mNodes[0].getContent()->getThermalConductivity() *
            (mShellTemperature - mFluidInput0->mTemperature) / mShellRadius;

    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedFlux, mArticle->getHeatFlux(),     1.0E-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedFlux, mArticle->mHeatFluxToShell,  1.0E-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedFlux, mNodes[0].mUndampedHeatFlux, 1.0E-6);

    /// - Test shell heat fluxes are zeroed for node pressure < 1 kPa.
    mArticle->mPotentialVector[0] = 0.99;
    mArticle->computeFlows(0.0);
    mArticle->transportFlows(0.0);

    CPPUNIT_ASSERT_EQUAL(0.0, mArticle->getHeatFlux());
    CPPUNIT_ASSERT_EQUAL(0.0, mArticle->mHeatFluxToShell);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Tank link model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTank::testModifiers()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mBiasHeatFlux, mArticle->mBiasHeatFlux, 1.0E-6);

    mArticle->setBiasHeatFlux(mBiasHeatFlux + 2.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mBiasHeatFlux + 2.0, mArticle->mBiasHeatFlux, 1.0E-6);

    UT_PASS_LAST;
}
