/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/fluid/source/GunnsFluidSeparatorLiquid.o))
***************************************************************************************************/

#include "core/GunnsFluidUtils.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidSeparatorLiquid.hh"

/// @details  Test identification number.
int UtGunnsFluidSeparatorLiquid::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Separator Liquid link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidSeparatorLiquid::UtGunnsFluidSeparatorLiquid()
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
    mLiquidType(FluidProperties::NO_FLUID),
    mLiquidIndex(-1),
    mConfigData(0),
    mMalfBlockageFlag(false),
    mMalfBlockageValue(0.0),
    mTransferTemperature(0.0),
    mTransferFlowRate(0.0),
    mSourcePressure(0.0),
    mInputData(0),
    mArticle(0),
    mFlowRate(0.0),
    mTimeStep(0.0),
    mTolerance(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Separator Liquid link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidSeparatorLiquid::~UtGunnsFluidSeparatorLiquid()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////

void UtGunnsFluidSeparatorLiquid::setUp()
{
    /// - Define the nominal port fluids.
    mFluidProperties      = new DefinedFluidProperties();
    mTypes[0]             = FluidProperties::GUNNS_WATER;
    mTypes[1]             = FluidProperties::GUNNS_O2;
    mFractions[0]         = 1.0;
    mFractions[1]         = 0.0;
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
    mMaxConductivity      = 1.0;
    mExpansionScaleFactor = 0.0;
    mLiquidType           = FluidProperties::GUNNS_WATER;
    mLiquidIndex          = mNodes[0].getContent()->find(mLiquidType);
    mConfigData           = new GunnsFluidSeparatorLiquidConfigData(mName,
                                                                    &mNodeList,
                                                                    mMaxConductivity,
                                                                    mExpansionScaleFactor,
                                                                    mLiquidType);

    /// - Define the nominal input data.
    mMalfBlockageFlag     = false;
    mMalfBlockageValue    = 0.3;
    mTransferTemperature  = 270.0;
    mTransferFlowRate     = 0.5;
    mSourcePressure       = 111.0;
    mInputData            = new GunnsFluidSeparatorLiquidInputData(mMalfBlockageFlag,
                                                                   mMalfBlockageValue,
                                                                   mSourcePressure,
                                                                   mTransferTemperature);

    /// - Define the nominal port mapping.
    mPort0                = 1;
    mPort1                = 0;

    /// - Default construct the nominal test article.
    mArticle              = new FriendlyGunnsFluidSeparatorLiquid;

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
void UtGunnsFluidSeparatorLiquid::tearDown()
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
/// @details  Tests for GUNNS Fluid Separator Liquid link model construction of configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSeparatorLiquid::testConfigAndInput()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(mName                        == mConfigData->mName);
    CPPUNIT_ASSERT(mNodes                       == mConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(mMaxConductivity             == mConfigData->mMaxConductivity);
    CPPUNIT_ASSERT(mExpansionScaleFactor        == mConfigData->mExpansionScaleFactor);
    CPPUNIT_ASSERT(FluidProperties::GUNNS_WATER == mConfigData->mLiquidType);

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(mMalfBlockageFlag == mInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfBlockageValue,
                                 mInputData->mMalfBlockageValue,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSourcePressure,
                                 mInputData->mSourcePressure,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mTransferTemperature,
                                 mInputData->mTransferTemperature,
                                 mTolerance);

    /// @test    Configuration data default construction.
    GunnsFluidSeparatorLiquidConfigData defaultConfig;
    CPPUNIT_ASSERT(0   == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0   == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mMaxConductivity);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(FluidProperties::NO_FLUID == defaultConfig.mLiquidType);

    /// @test    Input data default construction.
    GunnsFluidSeparatorLiquidInputData   defaultInput;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 defaultInput.mMalfBlockageValue,
                                 mTolerance);
    CPPUNIT_ASSERT(!defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 defaultInput.mMalfBlockageValue,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 defaultInput.mSourcePressure,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 defaultInput.mTransferTemperature,
                                 mTolerance);

    /// @test    Configuration data copy construction.
    GunnsFluidSeparatorLiquidConfigData copyConfig(*mConfigData);
    CPPUNIT_ASSERT(mConfigData->mName                 == copyConfig.mName);
    CPPUNIT_ASSERT(mConfigData->mNodeList->mNodes     == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(mConfigData->mMaxConductivity      == copyConfig.mMaxConductivity);
    CPPUNIT_ASSERT(mConfigData->mExpansionScaleFactor == copyConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(mConfigData->mLiquidType           == copyConfig.mLiquidType);

    /// @test    Input data copy construction.
    GunnsFluidSeparatorLiquidInputData   copyInput(*mInputData);
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfBlockageValue,
                                 copyInput.mMalfBlockageValue,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mSourcePressure,
                                 copyInput.mSourcePressure,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mTransferTemperature,
                                 copyInput.mTransferTemperature,
                                 mTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Separator Liquid link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSeparatorLiquid::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default construction configuration data.
    CPPUNIT_ASSERT("" == mArticle->mName);
    CPPUNIT_ASSERT(0  == mArticle->mNodes);
    CPPUNIT_ASSERT(FluidProperties::NO_FLUID == mArticle->mLiquidType);

    /// @test    Default construction input data.
    CPPUNIT_ASSERT(!mArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mMalfBlockageValue,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mSourcePressure,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mTransferTemperature,
                                 mTolerance);

    /// @test    Default construction state data.
    CPPUNIT_ASSERT(-1 == mArticle->mLiquidIndex);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(!mArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsFluidSeparatorLiquid* article = new GunnsFluidSeparatorLiquid();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Separator Liquid link model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSeparatorLiquid::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    FriendlyGunnsFluidSeparatorLiquid article;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    Nominal configuration data.
    CPPUNIT_ASSERT(mName  == article.mName);
    CPPUNIT_ASSERT(&mNodes[mPort0] == article.mNodes[0]);
    CPPUNIT_ASSERT(&mNodes[mPort1] == article.mNodes[1]);
    CPPUNIT_ASSERT(mConfigData->mLiquidType  == article.mLiquidType);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfBlockageValue,
                                 article.mMalfBlockageValue,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mSourcePressure,
                                 article.mSourcePressure,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mTransferTemperature,
                                 article.mTransferTemperature,
                                 mTolerance);

    /// @test    Nominal state data.
    CPPUNIT_ASSERT(mLiquidIndex == article.mLiquidIndex);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(article.mInitFlag);

    /// @test    Verify RestartModel Functionality
    article.mEffectiveConductivity = 1.0;
    article.mSystemConductance     = 1.0;
    article.mTuneMassFlow          = 1.0;
    article.mTuneVolFlow           = 1.0;
    article.mTuneDeltaT            = 1.0;

    article.restartModel();

    CPPUNIT_ASSERT(0.0 == article.mEffectiveConductivity);
    CPPUNIT_ASSERT(0.0 == article.mSystemConductance);
    CPPUNIT_ASSERT(0.0 == article.mTuneMassFlow);
    CPPUNIT_ASSERT(0.0 == article.mTuneVolFlow);
    CPPUNIT_ASSERT(0.0 == article.mTuneDeltaT);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Separator Liquid link model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSeparatorLiquid::testAccessors()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    Nothing (this derived class does not add or override any accessors).

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Separator Liquid link model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSeparatorLiquid::testModifiers()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    Nothing (this derived class does not add or override any modifiers).

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Separator Liquid link model update fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSeparatorLiquid::testUpdateFluidNominal()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->mTransferTemperature  = 270.0;

    mArticle->updateFluid(mTimeStep, mFlowRate);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(270.0,
                                 mArticle->mInternalFluid->getTemperature(),
                                 mTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Separator Liquid link model update fluid with no flow.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSeparatorLiquid::testProcessOutputs()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->mFlux = 1.0;

    mArticle->processOutputs();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNodes[1].getContent()->getMWeight(),
                                 mArticle->mFlowRate,
                                 mTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Separator Liquid link model specific port mapping rules.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSeparatorLiquid::testPortMapping()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    {
        /// @test    Fail if port 1 is the vacuum boundary node.
        const bool returned = mArticle->checkSpecificPortRules(1,1);
        CPPUNIT_ASSERT(!returned);
    } {
        /// @test    Fail if port 0 is not the vacuum boundary node.
        const bool returned = mArticle->checkSpecificPortRules(0,0);
        CPPUNIT_ASSERT(!returned);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Separator Liquid link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSeparatorLiquid::testInitializationExceptions()
{
    UT_RESULT;

    /// - Default construct a test article.
    GunnsFluidSeparatorLiquid article;

    /// @test    Initialization exception on invalid config data: no name.
    mConfigData->mName = "";
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mName = mName;

    /// @test    Initialization exception on invalid config data: incompatible liquid type.
    mConfigData->mLiquidType = FluidProperties::GUNNS_PG50;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mLiquidType = FluidProperties::GUNNS_WATER;

    /// @test    Initialization exception on invalid config data: liquid type not in PolyFluid.
    mConfigData->mLiquidType = FluidProperties::GUNNS_AMMONIA;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mLiquidType = FluidProperties::GUNNS_WATER;

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

    /// @test    Initialization exception on invalid input data: transfer temperature < 0.
    mInputData->mTransferTemperature = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mInputData->mTransferTemperature = mTransferTemperature;

    UT_PASS_LAST;
}
