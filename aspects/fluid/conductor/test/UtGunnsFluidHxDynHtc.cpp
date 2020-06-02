/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/fluid/conductor/GunnsFluidHxDynHtc.o))
***************************************************************************************************/

#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidHxDynHtc.hh"

/// @details  Test identification number.
int UtGunnsFluidHxDynHtc::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Heat Exchanger With Dynamic HTC link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidHxDynHtc::UtGunnsFluidHxDynHtc()
    :
    CppUnit::TestFixture(),
    tTypes(),
    tFractions(),
    tFluidProperties(0),
    tFluidConfig(0),
    tFluidInput(0),
    tLinks(),
    tName(""),
    tNodes(),
    tNodeList(),
    tPort0(0),
    tPort1(0),
    tMaxConductivity(0.0),
    tExpansionScaleFactor(0.0),
    tNumSegs(0),
    tHtcCoeff0(0.0),
    tHtcCoeff1(0.0),
    tHtcExponent(0.0),
    tHtcLimit(0.0),
    tConfigData(0),
    tMalfBlockageFlag(false),
    tMalfBlockageValue(0.0),
    tInitialSegmentTemperature(0.0),
    tInputData(0),
    tArticle(0)
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Heat Exchanger With Dynamic HTC link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidHxDynHtc::~UtGunnsFluidHxDynHtc()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHxDynHtc::setUp()
{
    /// - Define the nominal port fluids.
    tFluidProperties = new DefinedFluidProperties();
    tTypes[0]        = FluidProperties::GUNNS_PG50;
    tFractions[0]    = 1.0;
    tFluidConfig     = new PolyFluidConfigData(tFluidProperties, tTypes, N_FLUIDS);
    tFluidInput      = new PolyFluidInputData(295.0,
                                              100.0,
                                                0.0,
                                                0.0,
                                              tFractions);

    /// - Initialize the nodes.
    tNodes[0].initialize("UtNode1", tFluidConfig);
    tNodes[1].initialize("UtNode2", tFluidConfig);
    tNodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput);
    tNodes[1].getContent()->initialize(*tFluidConfig, *tFluidInput);

    /// - Initialize the nodes list.
    tNodeList.mNodes      = tNodes;
    tNodeList.mNumNodes   = N_NODES;

    /// - Define the nominal configuration data.
    tName                 = "nominal";
    tMaxConductivity      = 2.0;
    tExpansionScaleFactor = 0.5;
    tNumSegs              = 4;
    tHtcCoeff0            = 400.0;
    tHtcCoeff1            = -10.0;
    tHtcExponent          =   0.8;
    tHtcLimit             = 400.0;
    tConfigData           = new GunnsFluidHxDynHtcConfigData(tName,
                                                             &tNodeList,
                                                             tMaxConductivity,
                                                             tExpansionScaleFactor,
                                                             tNumSegs,
                                                             tHtcCoeff0,
                                                             tHtcCoeff1,
                                                             tHtcExponent,
                                                             tHtcLimit);

    /// - Define the nominal input data.
    tMalfBlockageFlag          = false;
    tMalfBlockageValue         = 0.0;
    tInitialSegmentTemperature = 280.0;
    tInputData                 = new GunnsFluidHxDynHtcInputData(tMalfBlockageFlag,
                                                                 tMalfBlockageValue,
                                                                 tInitialSegmentTemperature);

    /// - Define the nominal port mapping.
    tPort0                = 0;
    tPort1                = 1;

    /// - Default construct the nominal test article.
    tArticle              = new FriendlyGunnsFluidHxDynHtc;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHxDynHtc::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete tFluidInput;
    delete tFluidConfig;
    delete tFluidProperties;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of GUNNS Fluid Heat Exchanger With Dynamic HTC link model configuration and
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHxDynHtc::testConfigAndInput()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(tName                      == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                     == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductivity           == tConfigData->mMaxConductivity);
    CPPUNIT_ASSERT(tExpansionScaleFactor      == tConfigData->mExpansionScaleFactor);
    CPPUNIT_ASSERT(tNumSegs                   == tConfigData->mNumSegs);
    CPPUNIT_ASSERT(tHtcCoeff0                 == tConfigData->mHtcCoeff0);
    CPPUNIT_ASSERT(tHtcCoeff1                 == tConfigData->mHtcCoeff1);
    CPPUNIT_ASSERT(tHtcExponent               == tConfigData->mHtcExponent);
    CPPUNIT_ASSERT(tHtcLimit                  == tConfigData->mHtcLimit);

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(tMalfBlockageFlag          == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue         == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(1.0                        == tInputData->mHeatTransferCoefficient);
    CPPUNIT_ASSERT(tInitialSegmentTemperature == tInputData->mInitialSegmentTemperature);
    CPPUNIT_ASSERT(0                          == tInputData->mSegmentHtcOverrides);

    /// @test    Configuration data default construction.
    GunnsFluidHxDynHtcConfigData defaultConfig;
    CPPUNIT_ASSERT(0                          == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0                          == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0                        == defaultConfig.mMaxConductivity);
    CPPUNIT_ASSERT(0.0                        == defaultConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(0                          == defaultConfig.mNumSegs);
    CPPUNIT_ASSERT(0.0                        == defaultConfig.mHtcCoeff0);
    CPPUNIT_ASSERT(0.0                        == defaultConfig.mHtcCoeff1);
    CPPUNIT_ASSERT(0.0                        == defaultConfig.mHtcExponent);
    CPPUNIT_ASSERT(0.0                        == defaultConfig.mHtcLimit);
    CPPUNIT_ASSERT(0                          == defaultConfig.mSegsHtc.size());

    /// @test    Input data default construction.
    GunnsFluidHxDynHtcInputData defaultInput;
    CPPUNIT_ASSERT(false                      == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                        == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(1.0                        == defaultInput.mHeatTransferCoefficient);
    CPPUNIT_ASSERT(0.0                        == defaultInput.mInitialSegmentTemperature);
    CPPUNIT_ASSERT(0                          == defaultInput.mSegmentHtcOverrides);

    /// @test    Configuration data copy construction.
    tConfigData->addSegment(12.0, -1.0, 1.0, 20.0);
    GunnsFluidHxDynHtcConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tName                              == copyConfig.mName);
    CPPUNIT_ASSERT(tNodeList.mNodes                   == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductivity                   == copyConfig.mMaxConductivity);
    CPPUNIT_ASSERT(tExpansionScaleFactor              == copyConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(tNumSegs                           == copyConfig.mNumSegs);
    CPPUNIT_ASSERT(tHtcCoeff0                         == copyConfig.mHtcCoeff0);
    CPPUNIT_ASSERT(tHtcCoeff1                         == copyConfig.mHtcCoeff1);
    CPPUNIT_ASSERT(tHtcExponent                       == copyConfig.mHtcExponent);
    CPPUNIT_ASSERT(tHtcLimit                          == copyConfig.mHtcLimit);
    CPPUNIT_ASSERT(tConfigData->mSegsHtc[0].mCoeff0   == copyConfig.mSegsHtc[0].mCoeff0);
    CPPUNIT_ASSERT(tConfigData->mSegsHtc[0].mCoeff1   == copyConfig.mSegsHtc[0].mCoeff1);
    CPPUNIT_ASSERT(tConfigData->mSegsHtc[0].mExponent == copyConfig.mSegsHtc[0].mExponent);
    CPPUNIT_ASSERT(tConfigData->mSegsHtc[0].mLimit    == copyConfig.mSegsHtc[0].mLimit);

    /// @test    Input data copy construction.
    double tSegmentHtc[4]            = {0.0};
    tInputData->mSegmentHtcOverrides = tSegmentHtc;
    GunnsFluidHxDynHtcInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tMalfBlockageFlag          == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue         == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(1.0                        == copyInput.mHeatTransferCoefficient);
    CPPUNIT_ASSERT(tInitialSegmentTemperature == copyInput.mInitialSegmentTemperature);
    CPPUNIT_ASSERT(tSegmentHtc                == copyInput.mSegmentHtcOverrides);

    /// @test    For code coverage, segment Dynamic HTC nominal and copy construction.
    GunnsFluidHxDynHtcSegment* htcSeg = new GunnsFluidHxDynHtcSegment(1.0, 2.0, 3.0, 4.0);
    GunnsFluidHxDynHtcSegment* htcSegCopy = new GunnsFluidHxDynHtcSegment(*htcSeg);
    CPPUNIT_ASSERT(1.0 == htcSegCopy->mCoeff0);
    CPPUNIT_ASSERT(2.0 == htcSegCopy->mCoeff1);
    CPPUNIT_ASSERT(3.0 == htcSegCopy->mExponent);
    CPPUNIT_ASSERT(4.0 == htcSegCopy->mLimit);
    delete htcSegCopy;
    delete htcSeg;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Heat Exchanger With Dynamic HTC link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHxDynHtc::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default construction of base class.
    CPPUNIT_ASSERT(""    == tArticle->mName);

    /// @test    Default construction of test article.
    CPPUNIT_ASSERT(0     == tArticle->mSegsDynHtc);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(false == tArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    UtGunnsFluidHxDynHtc* article = new UtGunnsFluidHxDynHtc();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Heat Exchanger With Dynamic HTC link model nominal initialization without
///           exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHxDynHtc::testInitialization ()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    FriendlyGunnsFluidHxDynHtc article;
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Nominal configuration data.
    CPPUNIT_ASSERT(tName                          == article.mName);
    CPPUNIT_ASSERT(&tNodes[0]                     == article.mNodes[0]);
    CPPUNIT_ASSERT(&tNodes[1]                     == article.mNodes[1]);
    CPPUNIT_ASSERT(tMaxConductivity               == article.mMaxConductivity);
    CPPUNIT_ASSERT(tExpansionScaleFactor          == article.mExpansionScaleFactor);
    CPPUNIT_ASSERT(tNumSegs                       == article.mNumSegs);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT(tMalfBlockageFlag              == article.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue             == article.mMalfBlockageValue);
    double tSegHtc = tHtcCoeff0 / tNumSegs;
    for (int i = 0; i < tNumSegs; ++i) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(tSegHtc, article.mSegHtc[i], DBL_EPSILON);
        CPPUNIT_ASSERT(tInitialSegmentTemperature == article.mSegTemperature[i]);

        /// @test    Nominal state data.
        CPPUNIT_ASSERT(0.0                        == article.mSegEnergyGain[i]);
        CPPUNIT_ASSERT(false                      == article.mMalfSegDegradeFlag[i]);
        CPPUNIT_ASSERT(0.0                        == article.mMalfSegDegradeValue[i]);
    }
    CPPUNIT_ASSERT(0.0                            == article.mTotalEnergyGain);
    CPPUNIT_ASSERT(0.0                            == article.mDeltaTemperature);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(true                           == article.mInitFlag);

    /// - Initialize a new article using the segment HTC overrides.
    FriendlyGunnsFluidHxDynHtc article2;
    tConfigData->addSegment(1.0, -0.1, 0.1, 10.0);
    tConfigData->addSegment(2.0, -0.2, 0.2, 20.0);
    tConfigData->addSegment(3.0, -0.3, 0.3, 30.0);
    tConfigData->addSegment(4.0, -0.4, 0.4, 40.0);
    article2.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    CPPUNIT_ASSERT(tName                          == article2.mName);
    CPPUNIT_ASSERT(&tNodes[0]                     == article2.mNodes[0]);
    CPPUNIT_ASSERT(&tNodes[1]                     == article2.mNodes[1]);
    CPPUNIT_ASSERT(tMaxConductivity               == article2.mMaxConductivity);
    CPPUNIT_ASSERT(tExpansionScaleFactor          == article2.mExpansionScaleFactor);
    CPPUNIT_ASSERT(tNumSegs                       == article2.mNumSegs);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT(tMalfBlockageFlag              == article2.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue             == article2.mMalfBlockageValue);
    for (int i = 0; i < tNumSegs; ++i) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL((i+1.0),        article2.mSegsDynHtc[i].mCoeff0,   DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL((i+1.0) * -0.1, article2.mSegsDynHtc[i].mCoeff1,   DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL((i+1.0) *  0.1, article2.mSegsDynHtc[i].mExponent, DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL((i+1.0) * 10.0, article2.mSegsDynHtc[i].mLimit,    DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL((i+1.0),        article2.mSegHtc[i],               DBL_EPSILON);
        CPPUNIT_ASSERT(tInitialSegmentTemperature == article2.mSegTemperature[i]);

        /// @test    Nominal state data.
        CPPUNIT_ASSERT(0.0                        == article2.mSegEnergyGain[i]);
        CPPUNIT_ASSERT(false                      == article2.mMalfSegDegradeFlag[i]);
        CPPUNIT_ASSERT(0.0                        == article2.mMalfSegDegradeValue[i]);
    }
    CPPUNIT_ASSERT(0.0                            == article2.mTotalEnergyGain);
    CPPUNIT_ASSERT(0.0                            == article2.mDeltaTemperature);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(true                           == article2.mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Heat Exchanger With Dynamic HTC link model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHxDynHtc::testModifiers()
{
    UT_RESULT;

    /// @test    The addSegment function in the config data.
    tConfigData->addSegment(1.0, -0.1, 0.1, 10.0);
    CPPUNIT_ASSERT(1 == tConfigData->mSegsHtc.size());
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, tConfigData->mSegsHtc[0].mCoeff0,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.1, tConfigData->mSegsHtc[0].mCoeff1,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.1, tConfigData->mSegsHtc[0].mExponent, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(10.0, tConfigData->mSegsHtc[0].mLimit,    DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Heat Exchanger With Dynamic HTC link model
///           computeHeatTransferCoefficient method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHxDynHtc::testHtc()
{
    UT_RESULT;

    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Test segment heat transfer coefficients with no malfunction.
    double mdot            = 2.0;
    double expectedHtc     = (tHtcCoeff0 + tHtcCoeff1 * powf(mdot, tHtcExponent)) / tNumSegs;
    tArticle->mMalfHxDegradeFlag = false;
    tArticle->mFlowRate          = mdot;
    tArticle->computeHeatTransferCoefficient();

    for (int i = 0; i < tNumSegs; i++) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHtc, tArticle->mSegHtc[i], DBL_EPSILON);
    }

    /// @test    Test segment heat transfer coefficient with degrade malfunction.
    tArticle->mMalfHxDegradeFlag      = true;
    tArticle->mMalfHxDegradeValue     = 0.3;
    tArticle->mMalfSegDegradeFlag[1]  = true;
    tArticle->mMalfSegDegradeValue[1] = 0.5;
    tArticle->computeHeatTransferCoefficient();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHtc * tArticle->mMalfHxDegradeValue,     tArticle->mSegHtc[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHtc * tArticle->mMalfSegDegradeValue[1], tArticle->mSegHtc[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHtc * tArticle->mMalfHxDegradeValue,     tArticle->mSegHtc[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHtc * tArticle->mMalfHxDegradeValue,     tArticle->mSegHtc[3], DBL_EPSILON);

    /// @test    Limited to zero.
    tArticle->mMalfHxDegradeFlag      = false;
    tArticle->mMalfSegDegradeFlag[1]  = false;
    for (int i = 0; i < tNumSegs; i++) {
        tArticle->mSegsDynHtc[i].mCoeff0 = -1.0;
        tArticle->mSegsDynHtc[i].mCoeff1 = -1.0;
    }

    tArticle->computeHeatTransferCoefficient();

    for (int i = 0; i < tNumSegs; i++) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSegHtc[i], DBL_EPSILON);
    }

    /// @test    Upper limit.
    for (int i = 0; i < tNumSegs; i++) {
        tArticle->mSegsDynHtc[i].mCoeff0 = tHtcLimit;
        tArticle->mSegsDynHtc[i].mCoeff1 = 1.0;
    }

    tArticle->computeHeatTransferCoefficient();

    for (int i = 0; i < tNumSegs; i++) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(tHtcLimit / tNumSegs, tArticle->mSegHtc[i], DBL_EPSILON);
    }

    /// @test    Corner cases HTC exponent and flow rate ranges.
    mdot = FLT_EPSILON;
    tArticle->mFlowRate = mdot;
    tArticle->mSegsDynHtc[0].mExponent = 20.0;
    tArticle->mSegsDynHtc[0].mCoeff0   = 100.0;
    tArticle->mSegsDynHtc[0].mCoeff1   = 1.0E10;
    tArticle->mSegsDynHtc[0].mLimit    = 2000.0;
    expectedHtc = std::min(2000.0, (100.0 + 1.0E10 * powf(mdot, 20.0)));
    tArticle->computeHeatTransferCoefficient();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHtc,   tArticle->mSegHtc[0], DBL_EPSILON);

    mdot = 10.0;
    tArticle->mFlowRate = mdot;
    expectedHtc = std::min(2000.0, (100.0 + 1.0E10 * powf(mdot, 20.0)));
    tArticle->computeHeatTransferCoefficient();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHtc,   tArticle->mSegHtc[0], DBL_EPSILON);

    mdot = FLT_EPSILON;
    tArticle->mFlowRate = mdot;
    tArticle->mSegsDynHtc[0].mExponent = 0.05;
    expectedHtc = std::min(2000.0, (100.0 + 1.0E10 * powf(mdot, 20.0)));
    tArticle->computeHeatTransferCoefficient();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHtc,   tArticle->mSegHtc[0], DBL_EPSILON);

    mdot = 10.0;
    tArticle->mFlowRate = mdot;
    expectedHtc = std::min(2000.0, (100.0 + 1.0E10 * powf(mdot, 20.0)));
    tArticle->computeHeatTransferCoefficient();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHtc,   tArticle->mSegHtc[0], DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Heat Exchanger With Dynamic HTC link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHxDynHtc::testInitializationExceptions()
{
    UT_RESULT;

    /// - Default construct a test article.
    GunnsFluidHxDynHtc article;

    /// @test    Base class validation: invalid config data: number of segments < 1.
    tConfigData->mNumSegs = 0;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mNumSegs = tNumSegs;

    /// @test    Initialization exception on segment HTC overrides vector not matching # segments.
    tConfigData->addSegment(1.0, 1.0, 1.0, 1.0);
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);

    /// @test    Initialization exception on a segment override HTC exponent not in (0.05, 20).
    tConfigData->addSegment(1.0, 1.0, 1.0,  1.0);
    tConfigData->addSegment(1.0, 1.0, 1.0,  1.0);
    tConfigData->addSegment(1.0, 1.0, 0.04, 1.0);
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mSegsHtc.clear();
    tConfigData->addSegment(1.0, 1.0,  1.0,  1.0);
    tConfigData->addSegment(1.0, 1.0,  1.0,  1.0);
    tConfigData->addSegment(1.0, 1.0,  1.0,  1.0);
    tConfigData->addSegment(1.0, 1.0, 20.01, 1.0);
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mSegsHtc.clear();

    /// @test    Initialization exception on a segment override HTC limit < FLT_EPSILON.
    tConfigData->addSegment(1.0, 1.0, 1.0, 1.0);
    tConfigData->addSegment(1.0, 1.0, 1.0, 1.0);
    tConfigData->addSegment(1.0, 1.0, 1.0, 1.0);
    tConfigData->addSegment(1.0, 1.0, 1.0, 0.0);
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mSegsHtc.clear();

    /// @test    Initialization exception on overall HTC exponent not in (0.05, 20).
    tConfigData->mHtcExponent = 0.04;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mHtcExponent = 20.01;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mHtcExponent = tHtcExponent;

    /// @test    Initialization exception on overall HTC limit < FLT_EPSILON.
    tConfigData->mHtcLimit = 0.0;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mHtcLimit = tHtcLimit;

    UT_PASS_LAST;
}
