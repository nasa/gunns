/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/fluid/conductor/GunnsFluidLeak.o))
***************************************************************************************************/

#include "core/GunnsFluidUtils.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidLeak.hh"

/// @details  Test identification number.
int UtGunnsFluidLeak::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Leak link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidLeak::UtGunnsFluidLeak()
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
    mMalfLeakHoleFlag(false),
    mMalfLeakHoleValue(0.0),
    mMalfLeakRateFlag(false),
    mMalfLeakRateValue(0.0),
    mInputData(0),
    mArticle(0),
    mPreviousLeakHole(0.0),
    mPreviousLeakRate(0.0),
    mLeakConductivity(0.0),
    mFlowRate(0),
    mTimeStep(0.0),
    mTolerance(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Leak link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidLeak::~UtGunnsFluidLeak()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidLeak::setUp()
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
    mConfigData           = new GunnsFluidLeakConfigData(mName,
                                                         &mNodeList,
                                                         mMaxConductivity,
                                                         mExpansionScaleFactor);

    /// - Define the nominal input data.
    mMalfBlockageFlag     = false;
    mMalfBlockageValue    = 0.3;
    mMalfLeakHoleFlag     = false;
    mMalfLeakHoleValue    = 0.2;
    mMalfLeakRateFlag     = false;
    mMalfLeakRateValue    = 0.1;
    mPreviousLeakHole     = 0.0;
    mPreviousLeakRate     = 0.0;
    mLeakConductivity     = 0.0;
    mInputData            = new GunnsFluidLeakInputData(mMalfBlockageFlag,
                                                        mMalfBlockageValue,
                                                        mMalfLeakHoleFlag,
                                                        mMalfLeakHoleValue,
                                                        mMalfLeakRateFlag,
                                                        mMalfLeakRateValue);

    /// - Define the nominal port mapping.
    mPort0                = 0;
    mPort1                = 1;

    /// - Default construct the nominal test article.
    mArticle              = new FriendlyGunnsFluidLeak;

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
void UtGunnsFluidLeak::tearDown()
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
/// @details  Tests for construction of GUNNS Fluid Leak link model configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidLeak::testConfigAndInput()
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

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(mMalfBlockageFlag == mInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfBlockageValue,
                                 mInputData->mMalfBlockageValue,
                                 0.0);
    CPPUNIT_ASSERT(mMalfLeakHoleFlag == mInputData->mMalfLeakHoleFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfLeakHoleValue,
                                 mInputData->mMalfLeakHoleValue,
                                 0.0);
    CPPUNIT_ASSERT(mMalfLeakRateFlag == mInputData->mMalfLeakRateFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfLeakRateValue,
                                 mInputData->mMalfLeakRateValue,
                                 0.0);

    /// @test    Configuration data default construction.
    GunnsFluidLeakConfigData defaultConfig;
    CPPUNIT_ASSERT(0 == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0 == defaultConfig.mNodeList);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 defaultConfig.mMaxConductivity,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 defaultConfig.mExpansionScaleFactor,
                                 0.0);

    /// @test    Input data default construction.
    GunnsFluidLeakInputData   defaultInput;
    CPPUNIT_ASSERT(!defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 defaultInput.mMalfBlockageValue,
                                 0.0);
    CPPUNIT_ASSERT(!defaultInput.mMalfLeakHoleFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 defaultInput.mMalfLeakHoleValue,
                                 0.0);
    CPPUNIT_ASSERT(!defaultInput.mMalfLeakRateFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 defaultInput.mMalfLeakRateValue,
                                 0.0);

    /// @test    Configuration data copy construction.
    GunnsFluidLeakConfigData copyConfig(*mConfigData);
    CPPUNIT_ASSERT(mConfigData->mName             == copyConfig.mName);
    CPPUNIT_ASSERT(mConfigData->mNodeList->mNodes == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mMaxConductivity,
                                 copyConfig.mMaxConductivity,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mExpansionScaleFactor,
                                 copyConfig.mExpansionScaleFactor,
                                 0.0);

    /// @test    Input data copy construction.
    GunnsFluidLeakInputData   copyInput(*mInputData);
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfBlockageValue,
                                 copyInput.mMalfBlockageValue,
                                 0.0);
    CPPUNIT_ASSERT(mInputData->mMalfLeakHoleFlag == copyInput.mMalfLeakHoleFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfLeakHoleValue,
                                 copyInput.mMalfLeakHoleValue,
                                 0.0);
    CPPUNIT_ASSERT(mInputData->mMalfLeakRateFlag == copyInput.mMalfLeakRateFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfLeakRateValue,
                                 copyInput.mMalfLeakRateValue,
                                 0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Leak link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidLeak::testDefaultConstruction()
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

    /// @test    Default construction input data.
    CPPUNIT_ASSERT(!mArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mMalfBlockageValue,
                                 0.0);
    CPPUNIT_ASSERT(!mArticle->mMalfLeakHoleFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mMalfLeakHoleValue,
                                 0.0);
    CPPUNIT_ASSERT(!mArticle->mMalfLeakRateFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mMalfLeakRateValue,
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
    GunnsFluidLeak* article = new GunnsFluidLeak();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Leak link model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidLeak::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    FriendlyGunnsFluidLeak article;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    Nominal configuration data.
    CPPUNIT_ASSERT(mName      == article.mName);
    CPPUNIT_ASSERT(&mNodes[0] == article.mNodes[0]);
    CPPUNIT_ASSERT(&mNodes[1] == article.mNodes[1]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity,
                                 article.mMaxConductivity,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mExpansionScaleFactor,
                                 article.mExpansionScaleFactor,
                                 0.0);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT(mMalfBlockageFlag == article.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfBlockageValue,
                                 article.mMalfBlockageValue,
                                 0.0);
    CPPUNIT_ASSERT(mMalfLeakRateFlag == article.mMalfLeakHoleFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfLeakHoleValue,
                                 article.mMalfLeakHoleValue,
                                 0.0);
    CPPUNIT_ASSERT(mMalfLeakRateFlag == article.mMalfLeakRateFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfLeakRateValue,
                                 article.mMalfLeakRateValue,
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

    /// - Check restartModel capabilities
    article.restartModel();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,article.mEffectiveConductivity,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,article.mSystemConductance,mTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Leak link model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidLeak::testAccessors()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test   Nothing, there are no accessors (yet).

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Leak link model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidLeak::testModifiers()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    The leak hole size malf parameters are set.
    mArticle->setMalfLeakHole(true, 1.0);
    CPPUNIT_ASSERT(true  == mArticle->mMalfLeakHoleFlag);
    CPPUNIT_ASSERT(1.0   == mArticle->mMalfLeakHoleValue);

    /// @test    The leak hole size malf parameters are reset.
    mArticle->setMalfLeakHole();
    CPPUNIT_ASSERT(false == mArticle->mMalfLeakHoleFlag);
    CPPUNIT_ASSERT(0.0   == mArticle->mMalfLeakHoleValue);

    /// @test    The leak initial rate malf parameters are set.
    mArticle->setMalfLeakRate(true, 1.0);
    CPPUNIT_ASSERT(true  == mArticle->mMalfLeakRateFlag);
    CPPUNIT_ASSERT(1.0   == mArticle->mMalfLeakRateValue);

    /// @test    The leak initial rate malf parameters are reset.
    mArticle->setMalfLeakRate();
    CPPUNIT_ASSERT(false == mArticle->mMalfLeakRateFlag);
    CPPUNIT_ASSERT(0.0   == mArticle->mMalfLeakRateValue);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Leak link model update state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidLeak::testUpdateState()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// - Compute conductivity corresponding to a leak rate.
    const double leakRate         = 1.0;
    const double leakConductivity = GunnsFluidUtils::predictConductivity(leakRate,
                                                                         mArticle->mMinLinearizationPotential,
                                                                         mNodes[0].getOutflow(),
                                                                         mNodes[1].getOutflow());

    /// @test     Update state with no leak malfunction.
    mArticle->updateState(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfLeakRateValue,
                                 mArticle->mMalfLeakRateValue,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mPreviousLeakRate,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mLeakConductivity,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mEffectiveConductivity,
                                 mTolerance);

    /// @test     Update state with initial leak rate malfunction insertion.
    mArticle->mMalfLeakRateFlag  = true;
    mArticle->mMalfLeakRateValue = leakRate;
    mArticle->updateState(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(leakRate,
                                 mArticle->mMalfLeakRateValue,
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
                                 mArticle->mMalfLeakRateValue,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(leakRate,
                                 mArticle->mPreviousLeakRate,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(leakConductivity,
                                 mArticle->mEffectiveConductivity,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(leakConductivity,
                                 mArticle->mLeakConductivity,
                                 mTolerance);

    /// @test     Update state with leak rate malfunction removed.
    mArticle->mMalfLeakRateFlag  = false;
    mArticle->mMalfLeakRateValue = 0.0;
    mArticle->updateState(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mPreviousLeakRate,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mMalfLeakRateValue,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mEffectiveConductivity,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mLeakConductivity,
                                 mTolerance);

    /// @test     Update state with initial leak hole malfunction insertion.
    const double leakHole        = 0.01;
    mArticle->mMalfLeakHoleFlag  = true;
    mArticle->mMalfLeakHoleValue = leakHole;
    mArticle->updateState(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(leakHole,
                                 mArticle->mMalfLeakHoleValue,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(leakHole,
                                 mArticle->mPreviousLeakHole,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(leakHole,
                                 mArticle->mLeakConductivity,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(leakHole,
                                 mArticle->mEffectiveConductivity,
                                 mTolerance);

    /// @test     Update state with leak hole malfunction in progress.
    mArticle->updateState(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(leakHole,
                                 mArticle->mMalfLeakHoleValue,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(leakHole,
                                 mArticle->mPreviousLeakHole,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(leakHole,
                                 mArticle->mEffectiveConductivity,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(leakHole,
                                 mArticle->mLeakConductivity,
                                 mTolerance);

    /// @test     Update state with leak hole malfunction removed.
    mArticle->mMalfLeakHoleFlag  = false;
    mArticle->mMalfLeakHoleValue = 0.0;
    mArticle->updateState(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mPreviousLeakHole,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mMalfLeakHoleValue,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mEffectiveConductivity,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mLeakConductivity,
                                 mTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Leak link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidLeak::testInitializationExceptions()
{
    UT_RESULT;

    /// - Default construct a test article.
    GunnsFluidLeak article;

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

    /// @test    Initialization exception on invalid input data: mMalfLeakRateValue < 0.
    mInputData->mMalfLeakHoleValue = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mInputData->mMalfLeakHoleValue = mMalfLeakHoleValue;

    /// @test    Initialization exception on invalid input data: mMalfLeakRateValue < 0.
    mInputData->mMalfLeakRateValue = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mInputData->mMalfLeakRateValue = mMalfLeakRateValue;

    UT_PASS_LAST;
}
