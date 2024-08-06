/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/fluid/conductor/GunnsFluidHeatExchanger.o))
***************************************************************************************************/

#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidHeatExchanger.hh"

/// @details  Test identification number.
int UtGunnsFluidHeatExchanger::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Heat Exchanger link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidHeatExchanger::UtGunnsFluidHeatExchanger()
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
    mNumSegs(0),
    mConfigData(0),
    mMalfBlockageFlag(false),
    mMalfBlockageValue(0.0),
    mHeatTransferCoefficient(0.0),
    mInitialSegmentTemperature(0.0),
    mInputData(0),
    mArticle(0),
    mSegEnergyGain(),
    mTotalEnergyGain(0.0),
    mFlowRate(0),
    mTimeStep(0.0),
    mTolerance(0.0)
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Heat Exchanger link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidHeatExchanger::~UtGunnsFluidHeatExchanger()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHeatExchanger::setUp()
{
    /// - Define the nominal port fluids.
    mFluidProperties      = new DefinedFluidProperties();
    mTypes[0]             = FluidProperties::GUNNS_PG50;
    mFractions[0]         = 1.0;
    mFluidConfig          = new PolyFluidConfigData(mFluidProperties, mTypes, N_FLUIDS);
    mFluidInput0          = new PolyFluidInputData(295.0,
                                                   100.0,
                                                     0.0,
                                                     0.0,
                                                   mFractions);
    mFluidInput1          = new PolyFluidInputData(295.0,
                                                   100.0,
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
    mMaxConductivity      = 2.0;
    mExpansionScaleFactor = 0.5;
    mNumSegs              = 4;
    mConfigData           = new GunnsFluidHeatExchangerConfigData(mName,
                                                                  &mNodeList,
                                                                  mMaxConductivity,
                                                                  mExpansionScaleFactor,
                                                                  mNumSegs);

    /// - Define the nominal input data.
    mMalfBlockageFlag          = false;
    mMalfBlockageValue         = 0.0;
    mHeatTransferCoefficient   = 1.0;
    mInitialSegmentTemperature = 280.0;
    mInputData                 = new GunnsFluidHeatExchangerInputData(mMalfBlockageFlag,
                                                                      mMalfBlockageValue,
                                                                      mHeatTransferCoefficient,
                                                                      mInitialSegmentTemperature);

    /// - Define the nominal port mapping.
    mPort0                = 0;
    mPort1                = 1;

    /// - Default construct the nominal test article.
    mArticle              = new FriendlyGunnsFluidHeatExchanger;

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
void UtGunnsFluidHeatExchanger::tearDown()
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
/// @details  Tests for construction of GUNNS Fluid Heat Exchanger link model configuration and
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHeatExchanger::testConfigAndInput()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(mName                      == mConfigData->mName);
    CPPUNIT_ASSERT(mNodes                     == mConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(mMaxConductivity           == mConfigData->mMaxConductivity);
    CPPUNIT_ASSERT(mExpansionScaleFactor      == mConfigData->mExpansionScaleFactor);
    CPPUNIT_ASSERT(mNumSegs                   == mConfigData->mNumSegs);

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(mMalfBlockageFlag          == mInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(mMalfBlockageValue         == mInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(mHeatTransferCoefficient   == mInputData->mHeatTransferCoefficient);
    CPPUNIT_ASSERT(mInitialSegmentTemperature == mInputData->mInitialSegmentTemperature);
    CPPUNIT_ASSERT(0                          == mInputData->mSegmentHtcOverrides);

    /// @test    Configuration data default construction.
    GunnsFluidHeatExchangerConfigData defaultConfig;
    CPPUNIT_ASSERT(0                          == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0                          == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0                        == defaultConfig.mMaxConductivity);
    CPPUNIT_ASSERT(0.0                        == defaultConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(0                          == defaultConfig.mNumSegs);

    /// @test    Input data default construction.
    GunnsFluidHeatExchangerInputData defaultInput;
    CPPUNIT_ASSERT(false                      == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                        == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0                        == defaultInput.mHeatTransferCoefficient);
    CPPUNIT_ASSERT(0.0                        == defaultInput.mInitialSegmentTemperature);
    CPPUNIT_ASSERT(0                          == defaultInput.mSegmentHtcOverrides);

    /// @test    Configuration data copy construction.
    GunnsFluidHeatExchangerConfigData copyConfig(*mConfigData);
    CPPUNIT_ASSERT(mName                      == copyConfig.mName);
    CPPUNIT_ASSERT(mNodeList.mNodes           == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(mMaxConductivity           == copyConfig.mMaxConductivity);
    CPPUNIT_ASSERT(mExpansionScaleFactor      == copyConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(mNumSegs                   == copyConfig.mNumSegs);

    /// @test    Input data copy construction.
    double tSegmentHtc[4]            = {0.0};
    mInputData->mSegmentHtcOverrides = tSegmentHtc;
    GunnsFluidHeatExchangerInputData copyInput(*mInputData);
    CPPUNIT_ASSERT(mMalfBlockageFlag          == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(mMalfBlockageValue         == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(mHeatTransferCoefficient   == copyInput.mHeatTransferCoefficient);
    CPPUNIT_ASSERT(mInitialSegmentTemperature == copyInput.mInitialSegmentTemperature);
    CPPUNIT_ASSERT(tSegmentHtc                == copyInput.mSegmentHtcOverrides);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Heat Exchanger link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHeatExchanger::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default construction configuration data.
    CPPUNIT_ASSERT(""    == mArticle->mName);
    CPPUNIT_ASSERT(0     == mArticle->mNodes);
    CPPUNIT_ASSERT(0.0   == mArticle->mMaxConductivity);
    CPPUNIT_ASSERT(0.0   == mArticle->mExpansionScaleFactor);
    CPPUNIT_ASSERT(0     == mArticle->mNumSegs);

    /// @test    Default construction input data.
    CPPUNIT_ASSERT(false == mArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0   == mArticle->mMalfBlockageValue);
    CPPUNIT_ASSERT(0     == mArticle->mSegHtc);
    CPPUNIT_ASSERT(0     == mArticle->mSegTemperature);

    /// @test    Default construction state data.
    CPPUNIT_ASSERT(0     == mArticle->mMalfSegDegradeFlag);
    CPPUNIT_ASSERT(0     == mArticle->mMalfSegDegradeValue);
    CPPUNIT_ASSERT(0     == mArticle->mSegEnergyGain);
    CPPUNIT_ASSERT(0.0   == mArticle->mTotalEnergyGain);
    CPPUNIT_ASSERT(0.0   == mArticle->mDeltaTemperature);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(false == mArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    UtGunnsFluidHeatExchanger* article = new UtGunnsFluidHeatExchanger();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Heat Exchanger link model nominal initialization without
///           exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHeatExchanger::testInitialization ()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    FriendlyGunnsFluidHeatExchanger article;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    Nominal configuration data.
    CPPUNIT_ASSERT(mName                          == article.mName);
    CPPUNIT_ASSERT(&mNodes[0]                     == article.mNodes[0]);
    CPPUNIT_ASSERT(&mNodes[1]                     == article.mNodes[1]);
    CPPUNIT_ASSERT(mMaxConductivity               == article.mMaxConductivity);
    CPPUNIT_ASSERT(mExpansionScaleFactor          == article.mExpansionScaleFactor);
    CPPUNIT_ASSERT(mNumSegs                       == article.mNumSegs);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT(mMalfBlockageFlag              == article.mMalfBlockageFlag);
    CPPUNIT_ASSERT(mMalfBlockageValue             == article.mMalfBlockageValue);
    double tSegHtc = mHeatTransferCoefficient / mNumSegs;
    for (int i = 0; i < mNumSegs; ++i) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(tSegHtc, article.mSegHtc[i], DBL_EPSILON);
        CPPUNIT_ASSERT(mInitialSegmentTemperature == article.mSegTemperature[i]);

        /// @test    Nominal state data.
        CPPUNIT_ASSERT(0.0                        == article.mSegEnergyGain[i]);
        CPPUNIT_ASSERT(false                      == article.mMalfSegDegradeFlag[i]);
        CPPUNIT_ASSERT(0.0                        == article.mMalfSegDegradeValue[i]);
    }
    CPPUNIT_ASSERT(0.0                            == article.mTotalEnergyGain);
    CPPUNIT_ASSERT(0.0                            == article.mDeltaTemperature);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(true                           == article.mInitFlag);

    /// - Initialize a new article with the segment heat transfer coefficient and temperature input
    ///   data arrays set.
    double tSegmentHtc[4]            = {0.0, -0.01,   1.0,  10.0};
    mInputData->mSegmentHtcOverrides = tSegmentHtc;

    FriendlyGunnsFluidHeatExchanger article2;
    article2.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(    0.0, article2.mSegHtc[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSegHtc, article2.mSegHtc[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(    1.0, article2.mSegHtc[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(   10.0, article2.mSegHtc[3], DBL_EPSILON);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(true                           == article2.mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Heat Exchanger link model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHeatExchanger::testAccessors()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// - @test getTotalEnergyGain method
    mArticle->mTotalEnergyGain = 2.2;
    CPPUNIT_ASSERT( 2.2 == mArticle->getTotalEnergyGain());

    /// - @test getDeltaTemperature method
    mArticle->mDeltaTemperature = 3.3;
    CPPUNIT_ASSERT( 3.3 == mArticle->getDeltaTemperature());

    /// - @test getNumSegs method
    CPPUNIT_ASSERT( mNumSegs == mArticle->getNumSegs());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Heat Exchanger link model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHeatExchanger::testModifiers()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    The HX heat transfer degradation malf parameters are set.
    mArticle->setMalfHxDegrade(true, 1.0);
    CPPUNIT_ASSERT(true  == mArticle->mMalfHxDegradeFlag);
    CPPUNIT_ASSERT(1.0   == mArticle->mMalfHxDegradeValue);

    /// @test    The HX heat transfer degradation malf parameters are reset.
    mArticle->setMalfHxDegrade();
    CPPUNIT_ASSERT(false == mArticle->mMalfHxDegradeFlag);
    CPPUNIT_ASSERT(0.0   == mArticle->mMalfHxDegradeValue);

    /// @test    The segment heat transfer degradation malf parameters are set.
    mArticle->setMalfSegDegrade(1, true, 1.0);
    CPPUNIT_ASSERT(true  == mArticle->mMalfSegDegradeFlag[1]);
    CPPUNIT_ASSERT(1.0   == mArticle->mMalfSegDegradeValue[1]);

    /// @test    The segment heat transfer degradation malf parameters are reset.
    mArticle->setMalfSegDegrade(1);
    CPPUNIT_ASSERT(false == mArticle->mMalfSegDegradeFlag[1]);
    CPPUNIT_ASSERT(0.0   == mArticle->mMalfSegDegradeValue[1]);

    /// @test    The segment heat transfer degradation malf takes no action if an invalid segment #
    ///          is specified.
    CPPUNIT_ASSERT_NO_THROW(mArticle->setMalfSegDegrade(-1, true, 1.0));
    CPPUNIT_ASSERT(false == mArticle->mMalfSegDegradeFlag[0]);
    CPPUNIT_ASSERT(0.0   == mArticle->mMalfSegDegradeValue[0]);

    CPPUNIT_ASSERT_NO_THROW(mArticle->setMalfSegDegrade(mNumSegs, true, 1.0));
    CPPUNIT_ASSERT(false == mArticle->mMalfSegDegradeFlag[mNumSegs-1]);
    CPPUNIT_ASSERT(0.0   == mArticle->mMalfSegDegradeValue[mNumSegs-1]);

    /// @test    The temperature override setter with good value.
    mArticle->setTemperatureOverride(280.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(280.0, mArticle->mTemperatureOverride, 0.0);

    /// @test    The temperature override setter with out of range value.
    mArticle->setTemperatureOverride(-0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mTemperatureOverride, 0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Heat Exchanger link model update fluid (no flow).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHeatExchanger::testNoFlow()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    FriendlyGunnsFluidHeatExchanger article;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    const double expected = mFluidInput0->mTemperature;

    /// @test         No flow.
    article.updateFluid(mTimeStep, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.getFluid()->getTemperature(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,      article.getTotalEnergyGain(),         0.0);

    /// @test         Zero time step.
    article.updateFluid(0.0, mFlowRate);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.getFluid()->getTemperature(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,      article.getTotalEnergyGain(),         0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Heat Exchanger link model update fluid (same temperature).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHeatExchanger::testTemperatures()
{
    UT_RESULT;

    /// - Use default constructed test article.
    FriendlyGunnsFluidHeatExchanger article;

    /// @test     Fluid temperature equal to segment temperature (no segment energy gain)
    mInputData->mInitialSegmentTemperature = 295.0;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    article.updateFluid(mTimeStep, mFlowRate);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mInitialSegmentTemperature,
                                 article.getFluid()->getTemperature(),
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 article.getTotalEnergyGain(),
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 article.mDeltaTemperature,
                                 0.0);

    /// @test     Fluid temperature greater than segment temperature  (positive energy gain)
    mInputData->mInitialSegmentTemperature = 270.0;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    article.updateFluid(mTimeStep, mFlowRate);
    CPPUNIT_ASSERT(article.getFluid()->getTemperature() < mFluidInput0->mTemperature);
    CPPUNIT_ASSERT(article.mTotalEnergyGain > 0.0);
    CPPUNIT_ASSERT(article.mDeltaTemperature < 0.0);

    /// @test     Fluid temperature less than segment temperature  (negative energy gain)
    mInputData->mInitialSegmentTemperature = 300.0;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    article.updateFluid(mTimeStep, mFlowRate);
    CPPUNIT_ASSERT(article.getFluid()->getTemperature() > mFluidInput0->mTemperature);
    CPPUNIT_ASSERT(article.mTotalEnergyGain < 0.0);
    CPPUNIT_ASSERT(article.mDeltaTemperature > 0.0);

    mInputData->mInitialSegmentTemperature = mInitialSegmentTemperature;

    /// @test     Fluid temperature override
    mInputData->mInitialSegmentTemperature = 300.0;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    article.updateFluid(mTimeStep, mFlowRate);
    article.mTemperatureOverride = 280.0;
    article.updateFluid(mTimeStep, mFlowRate);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(280.0,   article.getFluid()->getTemperature(), DBL_EPSILON);

    ///@test      Outflow fluid temperature
    mInputData->mInitialSegmentTemperature = 300.0;
    mNodes[0].getOutflow()->setTemperature(10.0);
    article.updateFluid(mTimeStep, mFlowRate);
    CPPUNIT_ASSERT(mNodes[0].getOutflow()->getTemperature()!= mNodes[0].getContent()->getTemperature());
    double tempOutflow = article.mInternalFluid->getTemperature()-article.mDeltaTemperature;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tempOutflow,mNodes[0].getOutflow()->getTemperature(),
                                                             DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Heat Exchanger link model update fluid (nominal & reverse flow).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHeatExchanger::testFlowDirections()
{
    UT_RESULT;

    /// - Use default constructed test article.
    FriendlyGunnsFluidHeatExchanger article;

    /// @test     Forward flow ()
    mFluidInput0->mTemperature = mInputData->mInitialSegmentTemperature + 20.0;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    article.updateFluid(mTimeStep, mFlowRate);
    mFluidInput0->mTemperature = mFluidInput1->mTemperature;
    CPPUNIT_ASSERT(article.getFluid()->getTemperature() < mFluidInput0->mTemperature);

    /// @test     Reverse flow ()
    mFluidInput1->mTemperature = mInputData->mInitialSegmentTemperature + 20.0;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    article.updateFluid(mTimeStep, -mFlowRate);
    mFluidInput1->mTemperature = mFluidInput0->mTemperature;
    CPPUNIT_ASSERT(article.getFluid()->getTemperature() < mFluidInput1->mTemperature);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Heat Exchanger link model
///           computeHeatTransferCoefficient method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHeatExchanger::testHtc()
{
    UT_RESULT;

    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    Test segment heat transfer coefficient with no malfunction.
    mArticle->mMalfHxDegradeFlag  = false;
    mArticle->computeHeatTransferCoefficient();

    for (int i = 0; i < mNumSegs; i++) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mSegHtcDefault[i], mArticle->mSegHtc[i], DBL_EPSILON);
    }

    /// @test    Test segment heat transfer coefficient with degrade malfunction.
    mArticle->mMalfHxDegradeFlag      = true;
    mArticle->mMalfHxDegradeValue     = 0.3;
    mArticle->mMalfSegDegradeFlag[1]  = true;
    mArticle->mMalfSegDegradeValue[1] = 0.5;
    mArticle->computeHeatTransferCoefficient();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mSegHtcDefault[0] * mArticle->mMalfHxDegradeValue,     mArticle->mSegHtc[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mSegHtcDefault[1] * mArticle->mMalfSegDegradeValue[1], mArticle->mSegHtc[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mSegHtcDefault[2] * mArticle->mMalfHxDegradeValue,     mArticle->mSegHtc[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mSegHtcDefault[3] * mArticle->mMalfHxDegradeValue,     mArticle->mSegHtc[3], DBL_EPSILON);

    /// @test    Limited to zero.
    mArticle->mMalfHxDegradeFlag      = false;
    mArticle->mMalfSegDegradeFlag[1]  = false;
    for (int i = 0; i < mNumSegs; i++) {
        mArticle->mSegHtcDefault[i] = -1.0E15;
    }

    mArticle->computeHeatTransferCoefficient();

    for (int i = 0; i < mNumSegs; i++) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,   mArticle->mSegHtc[i], DBL_EPSILON);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Heat Exchanger link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHeatExchanger::testInitializationExceptions()
{
    UT_RESULT;

    /// - Default construct a test article.
    GunnsFluidHeatExchanger article;

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

    /// @test    Initialization exception on invalid config data: number of segments < 1.
    mConfigData->mNumSegs = 0;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mNumSegs = mNumSegs;

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

    /// @test    Initialization exception on invalid input data: mMalfBlockageValue < FLT_EPSILON.
    mInputData->mHeatTransferCoefficient = DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mInputData->mHeatTransferCoefficient = mHeatTransferCoefficient;

    /// @test    Initialization exception on invalid input data: mMalfBlockageValue < FLT_EPSILON.
    mInputData->mInitialSegmentTemperature = DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mInputData->mInitialSegmentTemperature = mInitialSegmentTemperature;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Heat Exchanger link model restart.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHeatExchanger::testRestart()
{
    UT_RESULT;

    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// - Set some non-config & non-checkpointed attributes.
    mArticle->mSystemConductance = 1.0;
    mArticle->mTotalEnergyGain   = 2.0;
    mArticle->mDeltaTemperature  = 3.0;

    /// @test    Restart method.
    mArticle->restart();
    CPPUNIT_ASSERT(0.0 == mArticle->mSystemConductance);
    CPPUNIT_ASSERT(0.0 == mArticle->mTotalEnergyGain);
    CPPUNIT_ASSERT(0.0 == mArticle->mDeltaTemperature);

    UT_PASS_LAST;
}
