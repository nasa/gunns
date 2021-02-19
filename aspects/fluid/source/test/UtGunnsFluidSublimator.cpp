/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/fluid/source/GunnsFluidSublimator.o))
***************************************************************************************************/

#include "core/GunnsFluidUtils.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidSublimator.hh"

/// @details  Test identification number.
int UtGunnsFluidSublimator::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Sublimator link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidSublimator::UtGunnsFluidSublimator()
    :
    CppUnit::TestFixture(),
    tTypes(),
    tFractionsFeed(),
    tFractionsVent(),
    tFluidProperties(0),
    tFluidConfig(0),
    tFluidInput0(0),
    tFluidInput1(0),
    tLinks(),
    tName(""),
    tNodes(),
    tNodeList(),
    tPort0(0),
    tPort1(0),
    tGasType(FluidProperties::NO_FLUID),
    tHeatOfVaporization(0.0),
    tHeatOfFusion(0.0),
    tTriplePointTemperature(0.0),
    tMaxIceMass(0.0),
    tIceCoverageFraction(0.0),
    tMaxThermalConductivity(0.0),
    tPlateConductivity(0.0),
    tThermalConductivityGain(0.0),
    tConfigData(0),
    tIceMass(0.0),
    tStructureTemperature(0.0),
    tInputData(0),
    tArticle(0),
    tTimeStep(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Sublimator link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidSublimator::~UtGunnsFluidSublimator()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////

void UtGunnsFluidSublimator::setUp()
{
    /// - Define the nominal port fluids.
    tFluidProperties        = new DefinedFluidProperties();
    tTypes[0]               = FluidProperties::GUNNS_WATER;
    tTypes[1]               = FluidProperties::GUNNS_H2O;
    tTypes[2]               = FluidProperties::GUNNS_O2;
    tFractionsFeed[0]       = 1.0;
    tFractionsFeed[1]       = 0.0;
    tFractionsFeed[2]       = 0.0;
    tFractionsVent[0]       = 0.0;
    tFractionsVent[1]       = 0.0;
    tFractionsVent[2]       = 1.0;
    tFluidConfig            = new PolyFluidConfigData(tFluidProperties, tTypes, N_FLUIDS);
    tFluidInput0            = new PolyFluidInputData(283.0,
                                                     109.0,
                                                       0.0,
                                                       0.0,
                                                     tFractionsFeed);
    tFluidInput1            = new PolyFluidInputData(274.0,
                                                       0.1,
                                                       0.0,
                                                       0.0,
                                                     tFractionsVent);

    /// - Initialize the nodes.  Nodes 0 & 2 are liquid, 1 & 3 are gas.
    tNodes[0].initialize("UtNode1", tFluidConfig);
    tNodes[1].initialize("UtNode2", tFluidConfig);
    tNodes[2].initialize("UtNode3", tFluidConfig);
    tNodes[3].initialize("UtNode4", tFluidConfig);
    tNodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput0);
    tNodes[1].getContent()->initialize(*tFluidConfig, *tFluidInput1);
    tNodes[2].getContent()->initialize(*tFluidConfig, *tFluidInput0);
    tNodes[3].getContent()->initialize(*tFluidConfig, *tFluidInput1);

    tNodes[0].resetFlows();
    tNodes[1].resetFlows();
    tNodes[2].resetFlows();
    tNodes[3].resetFlows();

    /// - Initialize the nodes list.
    tNodeList.mNodes         = tNodes;
    tNodeList.mNumNodes      = N_NODES;

    /// - Define the nominal configuration data.
    tName                    = "nominal";
    tGasType                 = FluidProperties::GUNNS_H2O;
    tHeatOfVaporization      = 2257.0;
    tHeatOfFusion            =  334.0;
    tTriplePointTemperature  =  273.15;
    tMaxIceMass              =    1.0;
    tIceCoverageFraction     =    0.1;
    tMaxThermalConductivity  =  100.0;
    tPlateConductivity       = 1.0E-7;
    tThermalConductivityGain =    0.01;
    tConfigData              = new GunnsFluidSublimatorConfigData(tName,
                                                                  &tNodeList,
                                                                  tGasType,
                                                                  tHeatOfVaporization,
                                                                  tHeatOfFusion,
                                                                  tTriplePointTemperature,
                                                                  tMaxIceMass,
                                                                  tIceCoverageFraction,
                                                                  tMaxThermalConductivity,
                                                                  tPlateConductivity,
                                                                  tThermalConductivityGain);

    /// - Define the nominal input data.
    tIceMass               =   0.0;
    tStructureTemperature  = 294.0;
    tInputData             = new GunnsFluidSublimatorInputData(tIceMass,
                                                               tStructureTemperature);

    /// - Define the nominal port mapping.
    tPort0                 = 0;
    tPort1                 = 1;

    /// - Default construct the nominal test article.
    tArticle               = new FriendlyGunnsFluidSublimator;

    /// - Define the nominal time step.
    tTimeStep              = 1.0;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSublimator::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete tFluidInput1;
    delete tFluidInput0;
    delete tFluidConfig;
    delete tFluidProperties;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Sublimator link model construction of configuration and input
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSublimator::testConfigAndInput()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(tName                                 == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                                == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tGasType                              == tConfigData->mGasType);
    CPPUNIT_ASSERT(tHeatOfVaporization                   == tConfigData->mHeatOfVaporization);
    CPPUNIT_ASSERT(tHeatOfFusion                         == tConfigData->mHeatOfFusion);
    CPPUNIT_ASSERT(tTriplePointTemperature               == tConfigData->mTriplePointTemperature);
    CPPUNIT_ASSERT(tMaxIceMass                           == tConfigData->mMaxIceMass);
    CPPUNIT_ASSERT(tIceCoverageFraction                  == tConfigData->mIceCoverageFraction);
    CPPUNIT_ASSERT(tMaxThermalConductivity               == tConfigData->mMaxThermalConductivity);
    CPPUNIT_ASSERT(tPlateConductivity                    == tConfigData->mPlateConductivity);
    CPPUNIT_ASSERT(tThermalConductivityGain              == tConfigData->mThermalConductivityGain);

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(                                        !tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                                   == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tIceMass                              == tInputData->mIceMass);
    CPPUNIT_ASSERT(tStructureTemperature                 == tInputData->mStructureTemperature);
    CPPUNIT_ASSERT(                                        !tInputData->mMalfFeedRateBiasFlag);
    CPPUNIT_ASSERT(0.0                                   == tInputData->mMalfFeedRateBiasValue);

    /// @test    Configuration data default construction.
    GunnsFluidSublimatorConfigData defaultConfig;
    CPPUNIT_ASSERT(0                                     == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0                                     == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(FluidProperties::NO_FLUID             == defaultConfig.mGasType);
    CPPUNIT_ASSERT(0.0                                   == defaultConfig.mHeatOfVaporization);
    CPPUNIT_ASSERT(0.0                                   == defaultConfig.mHeatOfFusion);
    CPPUNIT_ASSERT(0.0                                   == defaultConfig.mTriplePointTemperature);
    CPPUNIT_ASSERT(0.0                                   == defaultConfig.mMaxIceMass);
    CPPUNIT_ASSERT(0.0                                   == defaultConfig.mIceCoverageFraction);
    CPPUNIT_ASSERT(0.0                                   == defaultConfig.mMaxThermalConductivity);
    CPPUNIT_ASSERT(0.0                                   == defaultConfig.mPlateConductivity);
    CPPUNIT_ASSERT(0.0                                   == defaultConfig.mThermalConductivityGain);

    /// @test    Input data default construction.
    GunnsFluidSublimatorInputData defaultInput;
    CPPUNIT_ASSERT(                                        !defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                                   == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0                                   == defaultInput.mIceMass);
    CPPUNIT_ASSERT(0.0                                   == defaultInput.mStructureTemperature);
    CPPUNIT_ASSERT(                                        !defaultInput.mMalfFeedRateBiasFlag);
    CPPUNIT_ASSERT(0.0                                   == defaultInput.mMalfFeedRateBiasValue);

    /// @test    Configuration data copy construction.
    GunnsFluidSublimatorConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tConfigData->mName                    == copyConfig.mName);
    CPPUNIT_ASSERT(tConfigData->mNodeList->mNodes        == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tConfigData->mGasType                 == copyConfig.mGasType);
    CPPUNIT_ASSERT(tConfigData->mHeatOfVaporization      == copyConfig.mHeatOfVaporization);
    CPPUNIT_ASSERT(tConfigData->mHeatOfFusion            == copyConfig.mHeatOfFusion);
    CPPUNIT_ASSERT(tConfigData->mTriplePointTemperature  == copyConfig.mTriplePointTemperature);
    CPPUNIT_ASSERT(tConfigData->mMaxIceMass              == copyConfig.mMaxIceMass);
    CPPUNIT_ASSERT(tConfigData->mIceCoverageFraction     == copyConfig.mIceCoverageFraction);
    CPPUNIT_ASSERT(tConfigData->mMaxThermalConductivity  == copyConfig.mMaxThermalConductivity);
    CPPUNIT_ASSERT(tConfigData->mPlateConductivity       == copyConfig.mPlateConductivity);
    CPPUNIT_ASSERT(tConfigData->mThermalConductivityGain == copyConfig.mThermalConductivityGain);

    /// @test    Input data copy construction.
    tInputData->mMalfBlockageFlag      = true;
    tInputData->mMalfBlockageValue     = 1.0;
    tInputData->mMalfFeedRateBiasFlag  = true;
    tInputData->mMalfFeedRateBiasValue = 2.0;

    GunnsFluidSublimatorInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(                                        copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(1.0                                  == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(tInputData->mIceMass                 == copyInput.mIceMass);
    CPPUNIT_ASSERT(tInputData->mStructureTemperature    == copyInput.mStructureTemperature);
    CPPUNIT_ASSERT(                                        copyInput.mMalfFeedRateBiasFlag);
    CPPUNIT_ASSERT(2.0                                  == copyInput.mMalfFeedRateBiasValue);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Sublimator link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSublimator::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default construction state data.
    CPPUNIT_ASSERT(                            !tArticle->mMalfFeedRateBiasFlag);
    CPPUNIT_ASSERT(0.0                       == tArticle->mMalfFeedRateBiasValue);
    CPPUNIT_ASSERT(FluidProperties::NO_FLUID == tArticle->mGasType);
    CPPUNIT_ASSERT(0.0                       == tArticle->mHeatOfVaporization);
    CPPUNIT_ASSERT(0.0                       == tArticle->mHeatOfFusion);
    CPPUNIT_ASSERT(0.0                       == tArticle->mTriplePointTemperature);
    CPPUNIT_ASSERT(0.0                       == tArticle->mMaxIceMass);
    CPPUNIT_ASSERT(0.0                       == tArticle->mIceCoverageFraction);
    CPPUNIT_ASSERT(0.0                       == tArticle->mMaxThermalConductivity);
    CPPUNIT_ASSERT(0.0                       == tArticle->mPlateConductivity);
    CPPUNIT_ASSERT(0.0                       == tArticle->mThermalConductivityGain);
    CPPUNIT_ASSERT(0.0                       == tArticle->mIceMass);
    CPPUNIT_ASSERT(0.0                       == tArticle->mStructureTemperature);
    CPPUNIT_ASSERT(0.0                       == tArticle->mHeatBalance);
    CPPUNIT_ASSERT(0.0                       == tArticle->mHeatOfSublimation);
    CPPUNIT_ASSERT(0.0                       == tArticle->mThermalConductivity);
    CPPUNIT_ASSERT(0.0                       == tArticle->mVentRelativeHumidity);
    CPPUNIT_ASSERT(0.0                       == tArticle->mSystemConductance);
    CPPUNIT_ASSERT(false                     == tArticle->mLiquidBreakthrough);
    CPPUNIT_ASSERT(0                         == tArticle->mVentFluid);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsFluidSublimator* article = new GunnsFluidSublimator();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Sublimator link model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSublimator::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    FriendlyGunnsFluidSublimator article;
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Nominal configuration data.
    CPPUNIT_ASSERT(tName                                 == article.mName);
    CPPUNIT_ASSERT(&tNodes[tPort0]                       == article.mNodes[0]);
    CPPUNIT_ASSERT(&tNodes[tPort1]                       == article.mNodes[1]);
    CPPUNIT_ASSERT(tConfigData->mMaxIceMass              == article.mMaxIceMass);
    CPPUNIT_ASSERT(tConfigData->mMaxThermalConductivity  == article.mMaxThermalConductivity);
    CPPUNIT_ASSERT(tConfigData->mHeatOfVaporization      == article.mHeatOfVaporization);
    CPPUNIT_ASSERT(tConfigData->mHeatOfFusion            == article.mHeatOfFusion);
    CPPUNIT_ASSERT(tConfigData->mTriplePointTemperature  == article.mTriplePointTemperature);
    CPPUNIT_ASSERT(tConfigData->mIceCoverageFraction     == article.mIceCoverageFraction);
    CPPUNIT_ASSERT(tConfigData->mPlateConductivity       == article.mPlateConductivity);
    CPPUNIT_ASSERT(tConfigData->mGasType                 == article.mGasType);
    CPPUNIT_ASSERT(tConfigData->mThermalConductivityGain == article.mThermalConductivityGain);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT(0.0                                   == article.mMalfBlockageValue);
    CPPUNIT_ASSERT(tInputData->mIceMass                  == article.mIceMass);
    CPPUNIT_ASSERT(tInputData->mStructureTemperature     == article.mStructureTemperature);
    CPPUNIT_ASSERT(                                        !article.mMalfFeedRateBiasFlag);
    CPPUNIT_ASSERT(0.0                                   == article.mMalfFeedRateBiasValue);

    /// @test    Nominal derived data.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tConfigData->mHeatOfVaporization + tConfigData->mHeatOfFusion,
                                 article.mHeatOfSublimation, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tConfigData->mTriplePointTemperature,
                                 article.mVentFluid->getTemperature(), DBL_EPSILON);
    CPPUNIT_ASSERT(1.0 == article.mVentFluid->getMassFraction(tGasType));

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(article.mInitFlag);

    /// @test    Verify restartModel functionality
    article.mVentRelativeHumidity = 1.0;
    article.mSystemConductance    = 1.0;
    article.mLiquidBreakthrough   = 1.0;

    article.restartModel();

    CPPUNIT_ASSERT(0.0 == article.mVentRelativeHumidity);
    CPPUNIT_ASSERT(0.0 == article.mSystemConductance);
    CPPUNIT_ASSERT(0.0 == article.mLiquidBreakthrough);


    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Sublimator link model step method in nominal operation.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSublimator::testStepNominal()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Outputs of first step with nominal feed and zero ice mass, zero heat balance.
    tArticle->mPotentialVector[0] = tFluidInput0->mPressure;
    tArticle->mPotentialVector[1] = tFluidInput1->mPressure;
    tArticle->step(tTimeStep);

    /// - Calculate expected results.  We expect feed rate = plate vent rate, zero vent rate, ice
    ///   mass and thermal conductivity increasing.
    double plateDP = tFluidInput0->mPressure - tFluidInput1->mPressure;
    double plateConductivity = tPlateConductivity;
    double MW = tNodes[0].getContent()->getMWeight();
    double systemConductance = 1.0E-12 + plateConductivity
                             * sqrt(1000.0 * tNodes[0].getContent()->getDensity() / plateDP) / MW;
    double plateVentRate = systemConductance * plateDP * MW;
    double feedFlux = tFluidInput1->mPressure * systemConductance;
    double sublimationRate = 0.0;
    double ventFlux = sublimationRate / MW;

    const FluidProperties* propertiesH2O = tNodes[0].getContent()->getProperties(FluidProperties::GUNNS_H2O);
    double relativeHumidity = tNodes[1].getContent()->getPartialPressure(tGasType) /
            propertiesH2O->getSaturationPressure(tNodes[1].getContent()->getTemperature());
    double iceRate = (sublimationRate + plateVentRate * (1.0 - relativeHumidity)) *
            tHeatOfVaporization / tHeatOfFusion;
    double iceMass = iceRate * tTimeStep;

    double conductivity = tMaxThermalConductivity * (1.0 - relativeHumidity) *
                          iceMass / tMaxIceMass;
    double filteredConductivity = tThermalConductivityGain * conductivity;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(systemConductance,    tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(feedFlux,             tArticle->mSourceVector[0],     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(ventFlux,             tArticle->mSourceVector[1],     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(iceMass,              tArticle->mIceMass,             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(filteredConductivity, tArticle->mThermalConductivity, DBL_EPSILON);

    /// @test    Outputs of 2nd step with heat balance & partial plate ice coverage, user bias to
    ///          feed demand & blockage malf.  We expect feed rate = sublimation + plate vent +
    ///          bias, vent rate = sublimation w/blockage, ice mass & thermal conductivity
    ///          increasing.
    tArticle->mHeatBalance           = -100.0;
    tArticle->mMalfFeedRateBiasFlag  = true;
    tArticle->mMalfFeedRateBiasValue = 0.1;
    tArticle->mMalfBlockageFlag      = true;
    tArticle->mMalfBlockageValue     = 0.2;
    tArticle->step(tTimeStep);

    /// - Calculate expected results
    plateConductivity = tPlateConductivity * (1.0 - tArticle->mMalfBlockageValue) *
            (1.0 - iceMass / (tMaxIceMass * (1.0 - tArticle->mMalfBlockageValue) *
            tIceCoverageFraction));
    sublimationRate = -tArticle->mHeatBalance / (tHeatOfVaporization + tHeatOfFusion) / 1000.0;
    systemConductance = 1.0E-12 + plateConductivity
                      * sqrt(1000.0 * tNodes[0].getContent()->getDensity() / plateDP) / MW
                      + (sublimationRate + tArticle->mMalfFeedRateBiasValue) / tFluidInput0->mPressure / MW;
    plateVentRate = systemConductance * plateDP * MW;

    feedFlux = tFluidInput1->mPressure * systemConductance;
    ventFlux = sublimationRate / MW;

    iceRate = (sublimationRate + plateVentRate * (1.0 - relativeHumidity)) *
            tHeatOfVaporization / tHeatOfFusion;
    iceMass += iceRate * tTimeStep;

    filteredConductivity *= (1.0 - tThermalConductivityGain);
    conductivity = tMaxThermalConductivity * (1.0 - relativeHumidity) * iceMass / tMaxIceMass;
    filteredConductivity += tThermalConductivityGain * conductivity;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(systemConductance,    tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(feedFlux,             tArticle->mSourceVector[0],     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(ventFlux,             tArticle->mSourceVector[1],     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(iceMass,              tArticle->mIceMass,             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(filteredConductivity, tArticle->mThermalConductivity, DBL_EPSILON);

    /// @test    Outputs of 3rd step with heat balance & complete plate ice coverage.  We expect
    ///          feed rate = vent rate = sublimation, ice mass & thermal conductivity increasing.
    tArticle->mHeatBalance          = -200.0;
    tArticle->mIceMass              = 0.5;
    iceMass                         = 0.5;
    tArticle->mMalfBlockageFlag     = false;
    tArticle->mMalfFeedRateBiasFlag = false;
    tArticle->step(tTimeStep);

    /// - Calculate expected results
    plateVentRate = 0.0;
    sublimationRate = -tArticle->mHeatBalance / (tHeatOfVaporization + tHeatOfFusion) / 1000.0;
    systemConductance = 1.0E-12 + sublimationRate / tFluidInput0->mPressure / MW;
    ventFlux = sublimationRate / MW;
    feedFlux = tFluidInput1->mPressure * systemConductance;

    iceRate = (sublimationRate + plateVentRate * (1.0 - relativeHumidity)) *
            tHeatOfVaporization / tHeatOfFusion;
    iceMass += iceRate * tTimeStep;

    filteredConductivity *= (1.0 - tThermalConductivityGain);
    conductivity = tMaxThermalConductivity * (1.0 - relativeHumidity) * iceMass / tMaxIceMass;
    filteredConductivity += tThermalConductivityGain * conductivity;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(systemConductance,    tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(feedFlux,             tArticle->mSourceVector[0],     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(ventFlux,             tArticle->mSourceVector[1],     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(iceMass,              tArticle->mIceMass,             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(filteredConductivity, tArticle->mThermalConductivity, DBL_EPSILON);

    /// @test    Outputs of 4th step with heat balance & max ice mass w/ blockage.  We expect feed
    ///          rate = vent rate = sublimation, ice mass held at reduced max, and thermal
    ///          conductivity increasing.
    tArticle->mHeatBalance = -500.0;
    tArticle->mMalfBlockageFlag = true;
    tArticle->mIceMass = tMaxIceMass;
    tArticle->step(tTimeStep);

    /// - Calculate expected results
    sublimationRate = -tArticle->mHeatBalance / (tHeatOfVaporization + tHeatOfFusion) / 1000.0;
    systemConductance = 1.0E-12 + sublimationRate / tFluidInput0->mPressure / MW;
    ventFlux = sublimationRate / MW;
    feedFlux = tFluidInput1->mPressure * systemConductance;

    iceMass = tMaxIceMass * (1.0 - tArticle->mMalfBlockageValue);

    filteredConductivity *= (1.0 - tThermalConductivityGain);
    conductivity = tMaxThermalConductivity * (1.0 - relativeHumidity) * iceMass / tMaxIceMass;
    filteredConductivity += tThermalConductivityGain * conductivity;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(systemConductance,    tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(feedFlux,             tArticle->mSourceVector[0],     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(ventFlux,             tArticle->mSourceVector[1],     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(iceMass,              tArticle->mIceMass,             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(filteredConductivity, tArticle->mThermalConductivity, DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Sublimator link model step method, with feedwater turned off.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSublimator::testStepNoFeed()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Outputs of first step with heat balance, max ice, user feed bias, but no feed
    ///          pressure.
    tArticle->mPotentialVector[0] = tFluidInput1->mPressure;
    tArticle->mPotentialVector[1] = tFluidInput1->mPressure;
    tArticle->mIceMass = tMaxIceMass;
    tArticle->mHeatBalance = -500.0;
    tArticle->mThermalConductivity = tMaxThermalConductivity;
    tArticle->mMalfFeedRateBiasFlag = true;
    tArticle->mMalfFeedRateBiasValue = 0.1;
    tArticle->step(tTimeStep);

    /// - Calculate expected results.  We expect zero feed rate, vent rate = sublimation, ice mass
    ///   dropping at the sublimation rate, and thermal conductivity dropping.
    double MW = tNodes[0].getContent()->getMWeight();

    double sublimationRate = -tArticle->mHeatBalance / (tHeatOfVaporization + tHeatOfFusion)
                           / 1000.0;
    double systemConductance = 1.0E-12;

    double feedFlux = tFluidInput1->mPressure * systemConductance;
    double ventFlux = sublimationRate / MW;

    const FluidProperties* propertiesH2O = tNodes[0].getContent()->getProperties(FluidProperties::GUNNS_H2O);
    double relativeHumidity = tNodes[1].getContent()->getPartialPressure(tGasType) /
            propertiesH2O->getSaturationPressure(tNodes[1].getContent()->getTemperature());
    double iceRate = -sublimationRate;
    double iceMass = tMaxIceMass + iceRate * tTimeStep;

    double filteredConductivity = (1.0 - tThermalConductivityGain) * tMaxThermalConductivity;
    double conductivity = tMaxThermalConductivity * (1.0 - relativeHumidity) * iceMass
                        / tMaxIceMass;
    filteredConductivity += tThermalConductivityGain * conductivity;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(systemConductance,    tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(feedFlux,             tArticle->mSourceVector[0],     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(ventFlux,             tArticle->mSourceVector[1],     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(iceMass,              tArticle->mIceMass,             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(filteredConductivity, tArticle->mThermalConductivity, DBL_EPSILON);

    /// @test    Outputs of 2nd step with partial ice coverage.
    tArticle->mIceMass = 0.05;
    iceMass = 0.05;
    tArticle->mHeatBalance = -50.0;
    tArticle->step(tTimeStep);

    /// - Calculate expected results.  We expect zero feed rate, vent rate = sublimation, ice mass
    ///   dropping at the sublimation rate, and thermal conductivity dropping.
    sublimationRate = -tArticle->mHeatBalance / (tHeatOfVaporization + tHeatOfFusion) / 1000.0;

    ventFlux = sublimationRate / MW;

    iceRate = -sublimationRate;
    iceMass += iceRate * tTimeStep;

    filteredConductivity *= (1.0 - tThermalConductivityGain);
    conductivity = tMaxThermalConductivity * (1.0 - relativeHumidity) * iceMass / tMaxIceMass;
    filteredConductivity += tThermalConductivityGain * conductivity;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(systemConductance,    tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(feedFlux,             tArticle->mSourceVector[0],     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(ventFlux,             tArticle->mSourceVector[1],     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(iceMass,              tArticle->mIceMass,             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(filteredConductivity, tArticle->mThermalConductivity, DBL_EPSILON);

    /// @test    Outputs of 3rd step with zero ice.
    tArticle->mIceMass = 0.0;
    iceMass = 0.0;
    tArticle->mHeatBalance = -10.0;
    tArticle->mThermalConductivity = 1.0E-17;
    tArticle->step(tTimeStep);

    /// - Calculate expected results.  We expect zero feed & vent rate, zero ice and zero thermal
    ///   conductivity.
    feedFlux = tFluidInput1->mPressure * systemConductance;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(systemConductance,    tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(feedFlux,             tArticle->mSourceVector[0],     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                  tArticle->mSourceVector[1],     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                  tArticle->mIceMass,             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                  tArticle->mThermalConductivity, DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Sublimator link model step method with inadequate venting.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSublimator::testStepNoVent()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Outputs of first step with nominal feed and zero ice mass, zero heat balance, high
    ///          vent pressure.
    tNodes[1].setPotential(50.0);
    tArticle->mPotentialVector[0] = tFluidInput0->mPressure;
    tArticle->mPotentialVector[1] = tNodes[1].getContent()->getPressure();
    tArticle->step(tTimeStep);

    /// - Calculate expected results.  We expect feed rate = plate vent rate, zero vent rate, ice
    ///   mass and thermal conductivity remaining at zero.
    double plateDP = tArticle->mPotentialVector[0] - tArticle->mPotentialVector[1];
    double plateConductivity = tPlateConductivity;
    double MW = tNodes[0].getContent()->getMWeight();
    double systemConductance = 1.0E-12 + plateConductivity
                      * sqrt(1000.0 * tNodes[0].getContent()->getDensity() / plateDP) / MW;
    double feedFlux = tNodes[1].getContent()->getPressure() * systemConductance;

    double sublimationRate = 0.0;
    double ventFlux = sublimationRate / MW;

    const FluidProperties* propertiesH2O = tNodes[0].getContent()->getProperties(FluidProperties::GUNNS_H2O);
    double relativeHumidity = tNodes[1].getContent()->getPartialPressure(tGasType) /
            propertiesH2O->getSaturationPressure(tNodes[1].getContent()->getTemperature());
    double iceRate = 0.0;
    double iceMass = iceRate * tTimeStep;

    double conductivity = tMaxThermalConductivity * (1.0 - relativeHumidity) *
                          iceMass / tMaxIceMass;
    double filteredConductivity = tThermalConductivityGain * conductivity;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(systemConductance,    tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(feedFlux,             tArticle->mSourceVector[0],     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(ventFlux,             tArticle->mSourceVector[1],     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(iceMass,              tArticle->mIceMass,             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(filteredConductivity, tArticle->mThermalConductivity, DBL_EPSILON);

    /// @test    Outputs of 2nd step with heat balance & partial plate ice coverage, low vent total
    ///          pressure but high vapor pressure.
    tNodes[1].setPotential(1.0);
    double massFractions[3] = {0.0, 1.0, 0.0};
    tNodes[1].getContent()->setMassAndMassFractions(0.0, massFractions);
    tNodes[1].getContent()->setTemperature(tFluidInput1->mTemperature);
    tArticle->mPotentialVector[1] = tNodes[1].getContent()->getPressure();
    tArticle->mHeatBalance = -100.0;
    tArticle->mIceMass = 0.05;
    iceMass = 0.05;
    tArticle->mThermalConductivity = tMaxThermalConductivity;
    tArticle->step(tTimeStep);

    /// - Calculate expected results.  We expect feed rate = plate vent, zero vent, ice melting &
    ///   thermal conductivity decreasing.
    relativeHumidity = tNodes[1].getContent()->getPartialPressure(tGasType) /
            propertiesH2O->getSaturationPressure(tNodes[1].getContent()->getTemperature());
    relativeHumidity = MsMath::limitRange(0.0, relativeHumidity, 1.0);

    plateDP = tArticle->mPotentialVector[0] - tArticle->mPotentialVector[1];
    plateConductivity = tPlateConductivity * (1.0 - iceMass / (tMaxIceMass * tIceCoverageFraction));
    systemConductance = 1.0E-12 + plateConductivity
                      * sqrt(1000.0 * tNodes[0].getContent()->getDensity() / plateDP) / MW;

    feedFlux = tNodes[1].getContent()->getPressure() * systemConductance;

    double meltingRate = -tArticle->mHeatBalance / tHeatOfFusion / 1000.0;
    iceRate = -meltingRate;
    iceMass += iceRate * tTimeStep;

    filteredConductivity = tMaxThermalConductivity * (1.0 - tThermalConductivityGain);
    conductivity = tMaxThermalConductivity * (1.0 - relativeHumidity) * iceMass / tMaxIceMass;
    filteredConductivity += tThermalConductivityGain * conductivity;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(systemConductance,    tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(feedFlux,             tArticle->mSourceVector[0],     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(ventFlux,             tArticle->mSourceVector[1],     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(iceMass,              tArticle->mIceMass,             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(filteredConductivity, tArticle->mThermalConductivity, DBL_EPSILON);

    /// @test    Outputs of 3rd step with heat balance & zero ice, high vent vapor pressure.
    tArticle->mIceMass = 0.0;
    iceMass = 0.0;
    tArticle->step(tTimeStep);

    /// - Calculate expected results.  We expect no ice.
    plateConductivity = tPlateConductivity * (1.0 - iceMass / (tMaxIceMass * tIceCoverageFraction));
    systemConductance = 1.0E-12 + plateConductivity
                      * sqrt(1000.0 * tNodes[0].getContent()->getDensity() / plateDP) / MW;

    feedFlux = tNodes[1].getContent()->getPressure() * systemConductance;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(systemConductance,    tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(feedFlux,             tArticle->mSourceVector[0],     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(ventFlux,             tArticle->mSourceVector[1],     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(iceMass,              tArticle->mIceMass,             DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Sublimator link model computeFlows method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSublimator::testComputeFlows()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Vent & feed transport to/from the nodes.
    tArticle->mAdmittanceMatrix[0] = 0.1;
    tArticle->mPotentialVector[0]  = 15.0;
    tArticle->mPotentialVector[1]  = 0.001;
    tArticle->mSourceVector[0]     = 0.0001;
    tArticle->mSourceVector[1]     = 0.00001;
    tArticle->computeFlows(tTimeStep);
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tArticle->mPortDirections[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::SINK   == tArticle->mPortDirections[1]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mFlux, tNodes[0].getScheduledOutflux(),DBL_EPSILON);
    tArticle->transportFlows(tTimeStep);

    const double MW = tNodes[0].getOutflow()->getMWeight();

    const double dP       = tArticle->mPotentialVector[0] - tArticle->mPotentialVector[1];
    const double flux     = tArticle->mAdmittanceMatrix[0] * tArticle->mPotentialVector[0]
                          - tArticle->mSourceVector[0];
    const double feedFlow = flux * MW;
    const double ventFlow = tArticle->mSourceVector[1] * MW;
    const double volFlow  = feedFlow / tNodes[0].getOutflow()->getDensity();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(dP,       tArticle->mPotentialDrop, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(flux,     tArticle->mFlux, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(feedFlow, tArticle->mFlowRate, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(volFlow,  tArticle->mVolFlowRate, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(feedFlow, tNodes[0].getOutflux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(ventFlow, tNodes[1].getInflux(),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tTriplePointTemperature, tNodes[1].getInflow()->getTemperature(),
                                                                   FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,      tNodes[1].getInflow()->getMassFraction(tGasType),
                                                                   DBL_EPSILON);

    /// @test    Zero flow vent & feed transport to/from the nodes.
    tNodes[0].resetFlows();
    tNodes[1].resetFlows();
    tArticle->mAdmittanceMatrix[0] = 0.0;
    tArticle->mPotentialVector[0]  = 0.0;
    tArticle->mPotentialVector[1]  = 0.0;
    tArticle->mSourceVector[0]     = 0.0;
    tArticle->mSourceVector[1]     = 0.0;
    tArticle->computeFlows(tTimeStep);
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tArticle->mPortDirections[1]);
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tArticle->mPortDirections[0]);
    tArticle->transportFlows(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mPotentialDrop, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mFlux, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mFlowRate, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[0].getOutflux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[1].getInflux(),  DBL_EPSILON);

    /// @test    Zero feed node outflow density for code coverage.
    tNodes[0].getOutflow()->resetState();
    tArticle->transportFlows(tTimeStep);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mVolFlowRate);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Sublimator link model specific port mapping rules.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSublimator::testPortMapping()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Fail if port 0 is the vacuum boundary node.
    CPPUNIT_ASSERT(!tArticle->checkSpecificPortRules(0, 3));

    /// @test    Fail if port 0 is in gas phase.
    /// - First have to set port 1 to the ground node so we don't attempt to assign port 0 to the
    ///   same node.
    tArticle->setPort(1, 3);
    CPPUNIT_ASSERT(!tArticle->checkSpecificPortRules(0, 1));

    /// @test    Fail if port 1 is in liquid phase.
    /// - First have to set port 0 to another node.
    tArticle->setPort(0, 2);
    CPPUNIT_ASSERT(!tArticle->checkSpecificPortRules(1, 0));

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Sublimator link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSublimator::testInitializationExceptions()
{
    UT_RESULT;

    /// - Default construct a test article.
    GunnsFluidSublimator article;

    /// @test    Initialization exception on invalid config data: no name.
    tConfigData->mName = "";
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mName = tName;

    /// @test    Initialization exception on invalid config data: gas type not in atmosphere.
    tConfigData->mGasType = FluidProperties::GUNNS_CH4;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mGasType = FluidProperties::GUNNS_H2O;

    /// @test    Initialization exception on invalid config data: heat of fusion <= 0.0.
    tConfigData->mHeatOfFusion = 0.0;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mHeatOfFusion = tHeatOfFusion;

    /// @test    Initialization exception on invalid config data: heat of vaporization <= 0.0.
    tConfigData->mHeatOfVaporization = 0.0;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mHeatOfVaporization = tHeatOfVaporization;

    /// @test    Initialization exception on invalid config data: heat of fusion >= heat of
    ///          vaporization.
    tConfigData->mHeatOfVaporization = tHeatOfFusion;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mHeatOfVaporization = tHeatOfVaporization;

    /// @test    Initialization exception on invalid config data: freezing point <= 0.0.
    tConfigData->mTriplePointTemperature = 0.0;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mTriplePointTemperature = tTriplePointTemperature;

    /// @test    Initialization exception on invalid config data: max ice mass <= 0.0.
    tConfigData->mMaxIceMass = 0.0;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mMaxIceMass = tMaxIceMass;

    /// @test    Initialization exception on invalid config data: max thermal conductivity <= 0.0.
    tConfigData->mMaxThermalConductivity = 0.0;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mMaxThermalConductivity = tMaxThermalConductivity;

    /// @test    Initialization exception on invalid config data: ice coverage fraction < 0.0.
    tConfigData->mIceCoverageFraction = -0.01;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mIceCoverageFraction = 1.01;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mIceCoverageFraction = tIceCoverageFraction;

    /// @test    Initialization exception on invalid config data: plate conductivity <= 0.0.
    tConfigData->mPlateConductivity = 0.0;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mPlateConductivity = tPlateConductivity;

    /// @test    Initialization exception on invalid config data: thermal conductivity gain < 0.0.
    tConfigData->mThermalConductivityGain = -0.01;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);

    /// @test    Initialization exception on invalid config data: thermal conductivity gain > 1.0.
    tConfigData->mThermalConductivityGain = 1.01;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mThermalConductivityGain = 0.0;

    /// @test    Initialization exception on invalid input data: initial ice mass < 0.0.
    tInputData->mIceMass = -0.01;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tInputData->mIceMass = tIceMass;

    /// @test    Initialization exception on invalid input data: initial temperature < 0.0.
    tInputData->mStructureTemperature = -0.01;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tInputData->mStructureTemperature = tStructureTemperature;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the access methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSublimator::testAccessMethods()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Fail if getters to not return correct value.
    tArticle->mIceMass = 5.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, tArticle->getIceMass(), 0.0);

    /// @test    Feed rate bias malfunction is set.
    tArticle->setMalfFeedRateBias(true, 1.0);
    CPPUNIT_ASSERT(true  == tArticle->mMalfFeedRateBiasFlag);
    CPPUNIT_ASSERT(1.0   == tArticle->mMalfFeedRateBiasValue);

    /// @test    Feed rate bias malfunction is reset.
    tArticle->setMalfFeedRateBias();
    CPPUNIT_ASSERT(false == tArticle->mMalfFeedRateBiasFlag);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfFeedRateBiasValue);

    UT_PASS_LAST;
}
