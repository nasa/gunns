/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/fluid/conductor/GunnsFluidValve.o))
***************************************************************************************************/

#include "core/GunnsFluidUtils.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidValve.hh"

/// @details  Acts as a redundant test on UnitConversion class.
const double UtGunnsFluidValve::PI = 3.14159265358979;

/// @details  Test identification number.
int UtGunnsFluidValve::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Valve link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidValve::UtGunnsFluidValve()
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
/// @details  Default destructs this GUNNS Fluid Valve link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidValve::~UtGunnsFluidValve()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////

void UtGunnsFluidValve::setUp()
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
    mConfigData           = new GunnsFluidValveConfigData(mName,
                                                          &mNodeList,
                                                          mMaxConductivity,
                                                          mExpansionScaleFactor,
                                                          mThermalLength,
                                                          mThermalDiameter,
                                                          mSurfaceRoughness);

    /// - Define the nominal input data.
    mMalfBlockageFlag     = false;
    mMalfBlockageValue    = 0.3;
    mPosition             = 1.0;
    mMalfLeakThruFlag     = false;
    mMalfLeakThruValue    = 0.0;
    mPreviousLeakRate     = 0.0;
    mLeakConductivity     = 0.0;
    mWallTemperature      = 300.0;
    mInputData            = new GunnsFluidValveInputData(mMalfBlockageFlag,
                                                         mMalfBlockageValue,
                                                         mPosition,
                                                         mMalfLeakThruFlag,
                                                         mMalfLeakThruValue,
                                                         mWallTemperature);

    /// - Define the nominal port mapping.
    mPort0                = 0;
    mPort1                = 1;

    /// - Default construct the nominal test article.
    mArticle              = new FriendlyGunnsFluidValve;

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
void UtGunnsFluidValve::tearDown()
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
/// @details  Tests for GUNNS Fluid Valve link model construction of configuration.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidValve::testConfig()
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
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalLength,
                                 mConfigData->mThermalLength,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalDiameter,
                                 mConfigData->mThermalDiameter,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSurfaceRoughness,
                                 mConfigData->mSurfaceRoughness,
                                 0.0);

    /// @test    Configuration data default construction.
    GunnsFluidValveConfigData defaultConfig;
    CPPUNIT_ASSERT(0 == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0 == defaultConfig.mNodeList);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 defaultConfig.mMaxConductivity,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 defaultConfig.mExpansionScaleFactor,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 defaultConfig.mThermalLength,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 defaultConfig.mThermalDiameter,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 defaultConfig.mSurfaceRoughness,
                                 0.0);

       /// @test    Configuration data copy construction.
    GunnsFluidValveConfigData copyConfig(*mConfigData);
    CPPUNIT_ASSERT(mConfigData->mName             == copyConfig.mName);
    CPPUNIT_ASSERT(mConfigData->mNodeList->mNodes == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mMaxConductivity,
                                 copyConfig.mMaxConductivity,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mExpansionScaleFactor,
                                 copyConfig.mExpansionScaleFactor,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mThermalLength,
                                 copyConfig.mThermalLength,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mThermalDiameter,
                                 copyConfig.mThermalDiameter,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mSurfaceRoughness,
                                 copyConfig.mSurfaceRoughness,
                                 0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Valve link model construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidValve::testInput()
{
    UT_RESULT;

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
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mWallTemperature,
                                 mInputData->mWallTemperature,
                                 0.0);

    /// @test    Input data default construction.
    GunnsFluidValveInputData   defaultInput;
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
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 defaultInput.mWallTemperature,
                                 0.0);

    /// @test    Input data copy construction.
    GunnsFluidValveInputData   copyInput(*mInputData);
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
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mWallTemperature,
                                 copyInput.mWallTemperature,
                                 0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Valve link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidValve::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default of attributes.
    CPPUNIT_ASSERT("" == mArticle->mName);
    CPPUNIT_ASSERT(0  == mArticle->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mMaxConductivity,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mExpansionScaleFactor,
                                 0.0);
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
                                 mArticle->mWallHeatFlux,
                                 0.0);
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
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mWallTemperature,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mPreviousLeakRate,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mLeakConductivity,
                                 0.0);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(!mArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsFluidValve* article = new GunnsFluidValve();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Valve link model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidValve::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    FriendlyGunnsFluidValve article;
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

    /// @test    Internal fluid initialization.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNodes[0].getContent()->getTemperature(),
                                 article.getInternalFluid()->getTemperature(),
                                 DBL_EPSILON);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(article.mInitFlag);

    /// @test    Test restartModel functionality
    article.restartModel();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,article.mEffectiveConductivity,DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,article.mSystemConductance,DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Valve link model nominal initialization without
///           exceptions and with the valve configured to not use thermal convection.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidValve::testNoConvectionInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    FriendlyGunnsFluidValve article;
    mConfigData->mThermalDiameter = 0.0;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    Base class initialization.
    CPPUNIT_ASSERT(mName                 == article.mName);
    CPPUNIT_ASSERT(&mNodes[0]            == article.mNodes[0]);
    CPPUNIT_ASSERT(&mNodes[1]            == article.mNodes[1]);
    CPPUNIT_ASSERT(mMaxConductivity      == article.mMaxConductivity);
    CPPUNIT_ASSERT(mExpansionScaleFactor == article.mExpansionScaleFactor);

    /// @test    Terms initialized from configuration data.
    CPPUNIT_ASSERT(0.0                   == article.mThermalSurfaceArea);
    CPPUNIT_ASSERT(0.0                   == article.mThermalROverD);
    CPPUNIT_ASSERT(0.0                   == article.mThermalDiameter);

    /// @test    Terms initialized from input data.
    CPPUNIT_ASSERT(mWallTemperature      == article.mWallTemperature);

    /// @test    Initialized state data.
    CPPUNIT_ASSERT(0.0                   == article.mWallHeatFlux);

    /// @test    Internal fluid not instantiated.
    CPPUNIT_ASSERT(0 == article.mInternalFluid);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(article.mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Valve link model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidValve::testAccessors()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

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
    } {
        /// @test    The getWallHeatFlux method.
        mArticle->mWallHeatFlux = 1.0;
        CPPUNIT_ASSERT( 1.0 == mArticle->getWallHeatFlux());
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Valve link model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidValve::testModifiers()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

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
        /// @test    The leak-thru malfunction setter with activation values.
        mArticle->setMalfLeakThru(true, 1.0);
        CPPUNIT_ASSERT(true  == mArticle->mMalfLeakThruFlag);
        CPPUNIT_ASSERT(1.0   == mArticle->mMalfLeakThruValue);
    } {
        /// @test    The leak-thru malfunction setter with defaults.
        mArticle->setMalfLeakThru();
        CPPUNIT_ASSERT(false == mArticle->mMalfLeakThruFlag);
        CPPUNIT_ASSERT(0.0   == mArticle->mMalfLeakThruValue);
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
/// @details  Tests for GUNNS Fluid Valve link model update state nominal.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidValve::testUpdateStateNominal()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

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
void UtGunnsFluidValve::testUpdateStateMalfunction()
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
/// @details  Tests for GUNNS Fluid Valve link model update fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidValve::testUpdateFluid()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test   Update fluid with flowrate too small.
    mArticle->updateFluid(0.0, 0.5 * DBL_EPSILON);
    CPPUNIT_ASSERT(0.0 == mArticle->mWallHeatFlux);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNodes[0].getOutflow()->getTemperature(),
                                 mArticle->mInternalFluid->getTemperature(), DBL_EPSILON);

    /// @test   Update fluid with nominal flowrate.
    mArticle->updateFluid(0.0, 0.01);
    CPPUNIT_ASSERT(mArticle->mWallHeatFlux < 0.0);
    CPPUNIT_ASSERT(mArticle->mInternalFluid->getTemperature() > mNodes[0].getOutflow()->getTemperature());
    CPPUNIT_ASSERT(mArticle->mInternalFluid->getTemperature() < mWallTemperature);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Valve link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidValve::testInitializationExceptions()
{
    UT_RESULT;

    /// - Default construct a test article.
    GunnsFluidValve article;

    /// @test    Initialization exception invalid config data:no name.
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

    /// @test    Initialization exception on invalid input data: wall temperature < 0.
    mInputData->mWallTemperature = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);

    UT_PASS_LAST;
}
