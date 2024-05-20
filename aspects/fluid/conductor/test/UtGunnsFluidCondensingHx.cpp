/**
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/fluid/conductor/GunnsFluidCondensingHx.o))
*/

#include "UtGunnsFluidCondensingHx.hh"

#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"


/// @details  Test identification number.
int UtGunnsFluidCondensingHx::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Condensing HX unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidCondensingHx::UtGunnsFluidCondensingHx()
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
    tHxNumSegments(0),
    tHxDryHtcCoeff0(0.0),
    tHxDryHtcCoeff1(0.0),
    tHxDryHtcExponent(0.0),
    tHxDryHtcLimit(0.0),
    tConfigData(0),
    tMalfBlockageFlag(false),
    tMalfBlockageValue(0.0),
    tWallTemperature(0),
    tInputData(0),
    tArticle(0)
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Condensing HX unit
///           test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidCondensingHx::~UtGunnsFluidCondensingHx()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCondensingHx::setUp()
{
    /// - Define the nominal port fluids.
    tFluidProperties       = new DefinedFluidProperties();
    tTypes[0]              = FluidProperties::GUNNS_H2O;
    tTypes[1]              = FluidProperties::GUNNS_O2;
    tTypes[2]              = FluidProperties::GUNNS_N2;
    tFractions[0]          = 0.0091755;
    tFractions[1]          = 0.2335363;
    tFractions[2]          = 1.0 - tFractions[0] - tFractions[1];
    tFluidConfig           = new PolyFluidConfigData(tFluidProperties, tTypes, N_FLUIDS);
    tFluidInput            = new PolyFluidInputData(295.15,
                                                    100.18082,
                                                      0.0,
                                                      0.0,
                                                    tFractions);

    /// - Initialize the nodes, with the last node as the ground node.
    tNodes[0].initialize("UtNode1", tFluidConfig);
    tNodes[1].initialize("UtNode2", tFluidConfig);
    tNodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput);
    tNodes[1].getContent()->initialize(*tFluidConfig, *tFluidInput);
    tNodes[1].resetContentState();

    tNodes[0].resetFlows();
    tNodes[1].resetFlows();

    /// - Initialize the nodes list.
    tNodeList.mNodes       = tNodes;
    tNodeList.mNumNodes    = N_NODES;

    /// - Define the nominal configuration data.
    tName                  =  "nominal";
    tMaxConductivity       =  0.0120106;
    tHxNumSegments         =  4;
    tHxDryHtcCoeff0        =  1.0;
    tHxDryHtcCoeff1        =  1000.0;
    tHxDryHtcExponent      =  0.75;
    tHxDryHtcLimit         =  1200.0;
    tConfigData            = new GunnsFluidCondensingHxConfigData(tName,
                                                                  &tNodeList,
                                                                  tMaxConductivity,
                                                                  tHxNumSegments,
                                                                  tHxDryHtcCoeff0,
                                                                  tHxDryHtcCoeff1,
                                                                  tHxDryHtcExponent,
                                                                  tHxDryHtcLimit);

    /// - Define the nominal input data.
    tMalfBlockageFlag      = false;
    tMalfBlockageValue     = 0.0;
    tWallTemperature       = 286.0;
    tInputData             = new GunnsFluidCondensingHxInputData(tMalfBlockageFlag,
                                                                 tMalfBlockageValue,
                                                                 tWallTemperature);

    /// - Define the nominal port mapping.
    tPort0                 = 0;
    tPort1                 = 1;

    /// - Default construct the nominal test article.
    tArticle               = new FriendlyGunnsFluidCondensingHx;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCondensingHx::tearDown()
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
/// @details  Tests for construction of GUNNS Fluid Condensing HX
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCondensingHx::testConfig()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(tName                == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes               == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductivity     == tConfigData->mMaxConductivity);
    CPPUNIT_ASSERT(0.0                  == tConfigData->mExpansionScaleFactor);
    CPPUNIT_ASSERT(tHxNumSegments       == tConfigData->mNumSegments);
    CPPUNIT_ASSERT(tHxDryHtcCoeff0      == tConfigData->mHtcCoeff0);
    CPPUNIT_ASSERT(tHxDryHtcCoeff1      == tConfigData->mHtcCoeff1);
    CPPUNIT_ASSERT(tHxDryHtcExponent    == tConfigData->mHtcExponent);
    CPPUNIT_ASSERT(tHxDryHtcLimit       == tConfigData->mHtcLimit);

    /// @test    Configuration data default construction.
    GunnsFluidCondensingHxConfigData defaultConfig;
    CPPUNIT_ASSERT(0                    == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0                    == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0                  == defaultConfig.mMaxConductivity);
    CPPUNIT_ASSERT(0.0                  == defaultConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(0                    == defaultConfig.mNumSegments);
    CPPUNIT_ASSERT(0.0                  == defaultConfig.mHtcCoeff0);
    CPPUNIT_ASSERT(0.0                  == defaultConfig.mHtcCoeff1);
    CPPUNIT_ASSERT(0.0                  == defaultConfig.mHtcExponent);
    CPPUNIT_ASSERT(0.0                  == defaultConfig.mHtcLimit);

    /// @test    Configuration data copy construction.
    GunnsFluidCondensingHxConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tName                == copyConfig.mName);
    CPPUNIT_ASSERT(tNodes               == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductivity     == copyConfig.mMaxConductivity);
    CPPUNIT_ASSERT(0.0                  == copyConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(tHxNumSegments       == copyConfig.mNumSegments);
    CPPUNIT_ASSERT(tHxDryHtcCoeff0      == copyConfig.mHtcCoeff0);
    CPPUNIT_ASSERT(tHxDryHtcCoeff1      == copyConfig.mHtcCoeff1);
    CPPUNIT_ASSERT(tHxDryHtcExponent    == copyConfig.mHtcExponent);
    CPPUNIT_ASSERT(tHxDryHtcLimit       == copyConfig.mHtcLimit);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Condensing HX default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCondensingHx::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Base class default construction.
    CPPUNIT_ASSERT(""                   == tArticle->mName);
    CPPUNIT_ASSERT(0                    == tArticle->mNodes);
    CPPUNIT_ASSERT(0.0                  == tArticle->mMaxConductivity);
    CPPUNIT_ASSERT(0.0                  == tArticle->mExpansionScaleFactor);
    CPPUNIT_ASSERT(0                    == tArticle->mNumSegments);
    CPPUNIT_ASSERT(0.0                  == tArticle->mHtcCoeff0);

    /// @test    Default construction configuration data.
    CPPUNIT_ASSERT(0.0                  == tArticle->mHtcCoeff1);
    CPPUNIT_ASSERT(0.0                  == tArticle->mHtcExponent);
    CPPUNIT_ASSERT(0.0                  == tArticle->mHtcLimit);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(false                == tArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    UtGunnsFluidCondensingHx* article = new UtGunnsFluidCondensingHx();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Condensing HX nominal
///           initialization.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCondensingHx::testInitialization ()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test   Base class initialization.
    CPPUNIT_ASSERT(tName               == tArticle->mName);
    CPPUNIT_ASSERT(tNodes              == tArticle->mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductivity    == tArticle->mMaxConductivity);
    CPPUNIT_ASSERT(0.0                 == tArticle->mExpansionScaleFactor);
    CPPUNIT_ASSERT(tHxNumSegments      == tArticle->mNumSegments);
    CPPUNIT_ASSERT(tHxDryHtcCoeff0     == tArticle->mHtcCoeff0);

    /// @test    Nominal configuration data.
    CPPUNIT_ASSERT(tHxDryHtcCoeff1     == tArticle->mHtcCoeff1);
    CPPUNIT_ASSERT(tHxDryHtcExponent   == tArticle->mHtcExponent);
    CPPUNIT_ASSERT(tHxDryHtcLimit      == tArticle->mHtcLimit);
    CPPUNIT_ASSERT(0                   == tNodes[0].getOutflow()->find(FluidProperties::GUNNS_H2O));

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(true                == tArticle->mInitFlag);

    /// @test    Verify restartModel functionality
    tArticle->restartModel();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,tArticle->mEffectiveConductivity,DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,tArticle->mSystemConductance,DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,tArticle->mSegmentHtc,DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,tArticle->mSensibleHeat,DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,tArticle->mLatentHeat,DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,tArticle->mTotalHeat,DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,tArticle->mCondensationRate,DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Condensing HX computeHeatTransferCoefficient method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCondensingHx::testHtc()
{
    UT_RESULT;

    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Test segment heat transfer coefficient with all effects.
    double mdot = 0.1;
    tArticle->mFlowRate = mdot;
    tArticle->mMalfHxDegradeFlag  = true;
    tArticle->mMalfHxDegradeValue = 0.5;
    double dryHtc = tHxDryHtcCoeff0 + tHxDryHtcCoeff1 * std::pow(mdot, tHxDryHtcExponent);
    double expectedHtc = dryHtc * 0.5 / tHxNumSegments;
    tArticle->computeHeatTransferCoefficient();
    tArticle->degradeHeatTransferCoefficient();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHtc,   tArticle->mSegmentHtc, DBL_EPSILON);

    /// @test    Zero flow.
    mdot = 0.0;
    tArticle->mFlowRate = mdot;
    dryHtc = tHxDryHtcCoeff0;
    expectedHtc = dryHtc * 0.5 / tHxNumSegments;
    tArticle->computeHeatTransferCoefficient();
    tArticle->degradeHeatTransferCoefficient();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHtc,   tArticle->mSegmentHtc, DBL_EPSILON);

    /// @test    HTC zero limit.
    tArticle->mHtcCoeff0 = -10000.0;
    dryHtc = 0.0;
    expectedHtc = dryHtc * 0.5 / tHxNumSegments;
    tArticle->computeHeatTransferCoefficient();
    tArticle->degradeHeatTransferCoefficient();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHtc,   tArticle->mSegmentHtc, DBL_EPSILON);

    /// @test    HTC upper limit.
    tArticle->mHtcCoeff0 = 10000.0;
    dryHtc = tHxDryHtcLimit;
    expectedHtc = dryHtc * 0.5 / tHxNumSegments;
    tArticle->computeHeatTransferCoefficient();
    tArticle->degradeHeatTransferCoefficient();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHtc,   tArticle->mSegmentHtc, DBL_EPSILON);

    /// @test    Corner cases HTC exponent and flow rate ranges.
    mdot = FLT_EPSILON;
    tArticle->mFlowRate    = mdot;
    tArticle->mHtcExponent = 20.0;
    tArticle->mHtcCoeff0   = 100.0;
    tArticle->mHtcCoeff1   = 1.0E10;
    tArticle->mHtcLimit    = 2000.0;
    expectedHtc = std::min(2000.0, (100.0 + 1.0E10 * powf(mdot, 20.0))) * 0.5 / tHxNumSegments;
    tArticle->computeHeatTransferCoefficient();
    tArticle->degradeHeatTransferCoefficient();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHtc,   tArticle->mSegmentHtc, DBL_EPSILON);

    mdot = 10.0;
    tArticle->mFlowRate = mdot;
    expectedHtc = std::min(2000.0, (100.0 + 1.0E10 * powf(mdot, 20.0))) * 0.5 / tHxNumSegments;
    tArticle->computeHeatTransferCoefficient();
    tArticle->degradeHeatTransferCoefficient();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHtc,   tArticle->mSegmentHtc, DBL_EPSILON);

    mdot = FLT_EPSILON;
    tArticle->mFlowRate    = mdot;
    tArticle->mHtcExponent = 0.05;
    expectedHtc = std::min(2000.0, (100.0 + 1.0E10 * powf(mdot, 20.0))) * 0.5 / tHxNumSegments;
    tArticle->computeHeatTransferCoefficient();
    tArticle->degradeHeatTransferCoefficient();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHtc,   tArticle->mSegmentHtc, DBL_EPSILON);

    mdot = 10.0;
    tArticle->mFlowRate = mdot;
    expectedHtc = std::min(2000.0, (100.0 + 1.0E10 * powf(mdot, 20.0))) * 0.5 / tHxNumSegments;
    tArticle->computeHeatTransferCoefficient();
    tArticle->degradeHeatTransferCoefficient();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHtc,   tArticle->mSegmentHtc, DBL_EPSILON);

    /// @test    HeatExchanger nodal outflow properties to link input verification
    const double ppH2O = tArticle->mInternalFluid->getPartialPressure(FluidProperties::GUNNS_H2O);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(373.15,tNodes[0].getOutflow()->
                 getProperties(FluidProperties::GUNNS_H2O)->getSaturationTemperature(ppH2O),1.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Condensing HX
///           initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCondensingHx::testInitializationExceptions()
{
    UT_RESULT;

    /// @test    Initialization exception on invalid config data: num segments < 1.
    tConfigData->mNumSegments = 0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mNumSegments = tHxNumSegments;

    /// @test    Initialization exception on overall HTC exponent not in (0.05, 20).
    tConfigData->mHtcExponent = 0.04;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mHtcExponent = 20.01;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mHtcExponent = tHxDryHtcExponent;

    /// @test    Initialization exception on overall HTC limit < FLT_EPSILON.
    tConfigData->mHtcLimit = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mHtcLimit = tHxDryHtcLimit;

    /// @test    Initialization exception on invalid input data: default wall temperature < 0.
    tInputData->mWallTemperature = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tInputData->mWallTemperature = tWallTemperature;

    /// @test    Initialization exception on GUNNS_H2O not present in the network.
    tTypes[0] = FluidProperties::GUNNS_CO2;
    PolyFluidConfigData fluidConfig = PolyFluidConfigData(tFluidProperties, tTypes, N_FLUIDS);
    tNodes[0].initialize("UtNode1", &fluidConfig);
    tNodes[1].initialize("UtNode2", &fluidConfig);
    tNodes[0].getContent()->initialize(fluidConfig, *tFluidInput);
    tNodes[1].getContent()->initialize(fluidConfig, *tFluidInput);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);

    UT_PASS;
}
