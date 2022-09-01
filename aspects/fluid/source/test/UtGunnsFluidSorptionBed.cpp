/*
@copyright Copyright 2022 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/

#include "strings/UtResult.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "UtGunnsFluidSorptionBed.hh"

/// @details  Test identification number.
int UtGunnsFluidSorptionBed::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsFluidSorptionBed class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidSorptionBed::UtGunnsFluidSorptionBed()
    :
    tMaxConductivity(),
    tConfigData(),
    tWallTemperature(),
    tInputData(),
    tArticle(),
    tLinkName(),
    tTcInput(),
    tNodes(),
    tNodeList(),
    tLinks(),
    tPort0(),
    tTimeStep(),
    tFluidProperties(),
    tCompoundProperties(),
    tTcTypes(),
    tFluidTcConfig(),
    tFluidConfig(),
    tFluidTcInput(),
    tFluidInput1(),
    tFractions1()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsFluidSorptionBed class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidSorptionBed::~UtGunnsFluidSorptionBed()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSorptionBed::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete tFluidInput1;
    delete tFluidTcInput;
    delete tFluidConfig;
    delete tFluidTcConfig;
    delete tCompoundProperties;
    delete tFluidProperties;
    delete tTcInput;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSorptionBed::setUp()
{
    tLinkName           = "SorptionBed";
    tNodeList.mNumNodes = 2;
    tNodeList.mNodes    = tNodes;
    tPort0              = 0;
    tTcTypes[0]         = ChemicalCompound::H2O;
    tTcTypes[1]         = ChemicalCompound::CO2;
    tTcTypes[2]         = ChemicalCompound::NH3;
    tTimeStep           = 0.1;

    /// - Setup some fluid nodes.  CO2 will be both a bulk constituent and a trace compound, so we
    ///   can test that the link handles such a case.
    tFluidProperties = new DefinedFluidProperties();
    FluidProperties::FluidType types[3];
    types[0]    = FluidProperties::GUNNS_N2;
    types[1]    = FluidProperties::GUNNS_O2;
    types[2]    = FluidProperties::GUNNS_CO2;
    tFractions1[0] = 0.8;
    tFractions1[1] = 0.19;
    tFractions1[2] = 0.01;

    tCompoundProperties = new DefinedChemicalCompounds();
    tFluidTcConfig = new GunnsFluidTraceCompoundsConfigData(tTcTypes, 3, "tFluidTcConfig");
    tFluidConfig = new PolyFluidConfigData(tFluidProperties, types, 3, tFluidTcConfig);

    double tcConcentrations[3];
    tcConcentrations[0] = 5.0e-4;
    tcConcentrations[1] = 6.0e-4;
    tcConcentrations[2] = 1.0e-5;
    tFluidTcInput = new GunnsFluidTraceCompoundsInputData(tcConcentrations);

    tFluidInput1 = new PolyFluidInputData(283.15,                   //temperature
                                          700.728,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          tFractions1,              //massFractions
                                          tFluidTcInput);           //trace compounds

    /// - Have to initialize the nodes with the fluid configs (normally done by GUNNS)
    tNodes[0].initialize("UtTestNode0", tFluidConfig);
    tNodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput1);
    tNodes[0].resetFlows();

    tNodes[1].initialize("UtTestNode1", tFluidConfig);
    tNodes[1].getContent()->initialize(*tFluidConfig, *tFluidInput1);
    tNodes[1].resetFlows();

    /// - Define nominal configuration data.  Segment 1 has a defined sorbant, and segment 2
    ///   has a custom sorbant.
    tMaxConductivity = 0.001;
    tConfigData = new GunnsFluidSorptionBedConfigData(tLinkName, &tNodeList, tMaxConductivity);
    tConfigData->addSegment(SorbantProperties::SILICA_GEL_B125, 0.001, 200.0);

    /// - Define custom sorbant with blocking and offgas compounds.
    SorbateInteractingCompounds h2oBlockingCo2;
    h2oBlockingCo2.mCompound    = ChemicalCompound::H2O;
    h2oBlockingCo2.mInteraction = 1.0;
    std::vector<SorbateInteractingCompounds> blockingCompounds;
    blockingCompounds.push_back(h2oBlockingCo2);

    SorbateInteractingCompounds co2OffgasNh3;
    co2OffgasNh3.mCompound    = ChemicalCompound::NH3;
    co2OffgasNh3.mInteraction = 1.0e-4;
    std::vector<SorbateInteractingCompounds> offgasCompounds;
    blockingCompounds.push_back(co2OffgasNh3);

    SorbantProperties* seg2Sorbant = tConfigData->addCustomSorbant(500.0, 0.4, 400.0);
    seg2Sorbant->addSorbate(ChemicalCompound::H2O, 0,                  0,                1.767e+2, 2.787e-5,  1.093e+3, -1.190e-3,  2.213e+1, -50.2, 0.002);
    seg2Sorbant->addSorbate(ChemicalCompound::CO2, &blockingCompounds, &offgasCompounds, 7.678e-6, 5.164e-7,  2.330e+3, -3.053e-1,  2.386e+2, -40.0, 0.011375);
    tConfigData->addSegment(seg2Sorbant, 0.001, 200.0);

    /// - Define default input data
    tWallTemperature = 300.0;
    tInputData = new GunnsFluidSorptionBedInputData(false, 0.0, tWallTemperature);
    tInputData->addSegmentSorbateLoading(0, ChemicalCompound::H2O, 1.0e-6);
    tInputData->addSegmentSorbateLoading(0, ChemicalCompound::CO2, 2.0e-6);
    tInputData->addSegmentSorbateLoading(1, ChemicalCompound::H2O, 5.0e-7);
    tInputData->addSegmentSorbateLoading(1, ChemicalCompound::CO2, 5.0e-7);

    /// - Default construct the nominal test article.
    tArticle = new FriendlyGunnsFluidSorptionBed;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for sorption bed config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSorptionBed::testBedConfig()
{
    UT_RESULT_FIRST;

    /// @test default construction
    FriendlyGunnsFluidSorptionBedConfigData defaultConfig;
    CPPUNIT_ASSERT(""        == defaultConfig.mName);
    CPPUNIT_ASSERT(0         == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0       == defaultConfig.mMaxConductivity);
    CPPUNIT_ASSERT(0         == defaultConfig.mSegments.size());
    CPPUNIT_ASSERT(0         == defaultConfig.mDefinedSorbants);
    CPPUNIT_ASSERT(0         == defaultConfig.mCustomSorbants.size());

    /// @test nominal construction
    FriendlyGunnsFluidSorptionBedConfigData nominalConfig(tLinkName, &tNodeList, 1.0);
    CPPUNIT_ASSERT(tLinkName == nominalConfig.mName);
    CPPUNIT_ASSERT(tNodes    == nominalConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(1.0       == nominalConfig.mMaxConductivity);
    CPPUNIT_ASSERT(0         == nominalConfig.mSegments.size());
    CPPUNIT_ASSERT(0         == nominalConfig.mDefinedSorbants);
    CPPUNIT_ASSERT(0         == nominalConfig.mCustomSorbants.size());

    /// @test addCustomSorbant
    const double sorbantRho  = 1000.0;
    const double sorbantPor  = 0.5;
    const double sorbantCp   = 500.0;
    const double segmentVol  = 1.0;
    const double expectedVol = segmentVol * (1.0 - sorbantPor);
    const double expectedC   = expectedVol * sorbantCp * sorbantRho;
    SorbantProperties* customSorbant = nominalConfig.addCustomSorbant(sorbantRho, sorbantPor, sorbantCp);
    CPPUNIT_ASSERT(1             == nominalConfig.mCustomSorbants.size());
    CPPUNIT_ASSERT(customSorbant == &nominalConfig.mCustomSorbants.at(0));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVol, customSorbant->computeVolume(segmentVol),           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedC,   customSorbant->computeThermalCapacity(expectedVol), DBL_EPSILON);

    /// @test addSegment with defined sorbant type
    const double sorbantHtc = 300.0;
    nominalConfig.addSegment(SorbantProperties::SILICA_GEL_B125, segmentVol, sorbantHtc);
    CPPUNIT_ASSERT(1                          == nominalConfig.mSegments.size());
    CPPUNIT_ASSERT(0                          != nominalConfig.mDefinedSorbants);
    CPPUNIT_ASSERT(tLinkName + ".mSegments_0" == nominalConfig.mSegments.at(0).mName);
    CPPUNIT_ASSERT(segmentVol                 == nominalConfig.mSegments.at(0).mVolume);
    CPPUNIT_ASSERT(sorbantHtc                 == nominalConfig.mSegments.at(0).mHtc);
    CPPUNIT_ASSERT(nominalConfig.mDefinedSorbants->getSorbant(SorbantProperties::SILICA_GEL_B125) == nominalConfig.mSegments.at(0).mProperties);

    /// @test addSegment with custom sorbant type
    nominalConfig.addSegment(customSorbant, segmentVol, sorbantHtc);
    CPPUNIT_ASSERT(2                          == nominalConfig.mSegments.size());
    CPPUNIT_ASSERT(0                          != nominalConfig.mDefinedSorbants);
    CPPUNIT_ASSERT(tLinkName + ".mSegments_1" == nominalConfig.mSegments.at(1).mName);
    CPPUNIT_ASSERT(segmentVol                 == nominalConfig.mSegments.at(1).mVolume);
    CPPUNIT_ASSERT(sorbantHtc                 == nominalConfig.mSegments.at(1).mHtc);
    CPPUNIT_ASSERT(customSorbant              == nominalConfig.mSegments.at(1).mProperties);

    /// @test exception from addSegment with bad defined sorbant
    CPPUNIT_ASSERT_THROW(
            nominalConfig.addSegment(SorbantProperties::CUSTOM, segmentVol, sorbantHtc),
            TsInitializationException);

    /// @test exception from addSegment with bad custom sorbant
    SorbantProperties badSorbant(SorbantProperties::CUSTOM, 1.0, 1.0, 1.0);
    CPPUNIT_ASSERT_THROW(
            nominalConfig.addSegment(&badSorbant, segmentVol, sorbantHtc),
            TsInitializationException);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for sorption bed input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSorptionBed::testBedInput()
{
    UT_RESULT;

    /// @test default construction
    GunnsFluidSorptionBedInputData defaultInput;
    CPPUNIT_ASSERT(false == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0   == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0   == defaultInput.mWallTemperature);
    CPPUNIT_ASSERT(0     == defaultInput.mLoading.size());

    /// @test nominal construction
    GunnsFluidSorptionBedInputData nominalInput(true, 0.5, 300.0);
    CPPUNIT_ASSERT(true  == nominalInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.5   == nominalInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(300.0 == nominalInput.mWallTemperature);
    CPPUNIT_ASSERT(0     == nominalInput.mLoading.size());

    /// @test addSegmentSorbateLoading
    nominalInput.addSegmentSorbateLoading(6, ChemicalCompound::CO2, 1.0e-6);
    CPPUNIT_ASSERT(1                     == nominalInput.mLoading.size());
    CPPUNIT_ASSERT(6                     == nominalInput.mLoading.at(0).mSegment);
    CPPUNIT_ASSERT(ChemicalCompound::CO2 == nominalInput.mLoading.at(0).mSorbate);
    CPPUNIT_ASSERT(1.0e-6                == nominalInput.mLoading.at(0).mLoading);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for sorption bed segment config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSorptionBed::testSegmentConfig()
{
    UT_RESULT;

    /// @test nominal construction
    SorbantProperties sorbant(SorbantProperties::CUSTOM, 1.0, 1.0, 1.0);
    GunnsFluidSorptionBedSegmentConfigData nominalConfig("config", &sorbant, 1.0, 100.0);
    CPPUNIT_ASSERT("config" == nominalConfig.mName);
    CPPUNIT_ASSERT(&sorbant == nominalConfig.mProperties);
    CPPUNIT_ASSERT(1.0      == nominalConfig.mVolume);
    CPPUNIT_ASSERT(100.0    == nominalConfig.mHtc);

    /// @test copy construction
    GunnsFluidSorptionBedSegmentConfigData copyConfig(nominalConfig);
    CPPUNIT_ASSERT("config" == copyConfig.mName);
    CPPUNIT_ASSERT(&sorbant == copyConfig.mProperties);
    CPPUNIT_ASSERT(1.0      == copyConfig.mVolume);
    CPPUNIT_ASSERT(100.0    == copyConfig.mHtc);

    /// @test assignment operator doesn't actually do anything, this if for code coverage.
    copyConfig = nominalConfig;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for sorption bed segment input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSorptionBed::testSegmentInput()
{
    UT_RESULT;

    /// @test nominal construction
    GunnsFluidSorptionBedSegmentInputData nominalInput(3, ChemicalCompound::H2O, 0.001);
    CPPUNIT_ASSERT(3                     == nominalInput.mSegment);
    CPPUNIT_ASSERT(ChemicalCompound::H2O == nominalInput.mSorbate);
    CPPUNIT_ASSERT(0.001                 == nominalInput.mLoading);

    /// @test copy constructor TODO don't know why this isn't showing up in code coverage...?
    GunnsFluidSorptionBedSegmentInputData copyInput(nominalInput);
    CPPUNIT_ASSERT(3                     == copyInput.mSegment);
    CPPUNIT_ASSERT(ChemicalCompound::H2O == copyInput.mSorbate);
    CPPUNIT_ASSERT(0.001                 == copyInput.mLoading);

    /// @test assignment operator doesn't actually do anything, this if for code coverage.
    copyInput = nominalInput;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for the bed sorbate construction and initialization.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSorptionBed::testBedSorbateInit()
{
    UT_RESULT;

    /// @test default construction
    FriendlyGunnsFluidSorptionBedSorbate article;
    CPPUNIT_ASSERT(false == article.mMalfLoadingEquilFlag);
    CPPUNIT_ASSERT(0.0   == article.mMalfLoadingEquilValue);
    CPPUNIT_ASSERT(0.0   == article.mLoading);
    CPPUNIT_ASSERT(0.0   == article.mLoadingEquil);
    CPPUNIT_ASSERT(0.0   == article.mLoadingRate);
    CPPUNIT_ASSERT(0.0   == article.mLoadingFraction);
    CPPUNIT_ASSERT(0.0   == article.mAdsorptionRate);
    CPPUNIT_ASSERT(0.0   == article.mLoadedMass);
    CPPUNIT_ASSERT(0     == article.mProperties);
    CPPUNIT_ASSERT(0     == article.mFluidIndexes.mFluid);
    CPPUNIT_ASSERT(0     == article.mFluidIndexes.mTc);
    CPPUNIT_ASSERT(0     == article.mBlockingStates.size());
    CPPUNIT_ASSERT(0     == article.mBlockingCompoundIndex.size());
    CPPUNIT_ASSERT(0     == article.mOffgasIndexes.size());

    /// @test new/delete for code coverage
    GunnsFluidSorptionBedSorbate* newSorbate = new GunnsFluidSorptionBedSorbate;
    delete newSorbate;

    /// @test initialization with a custom sorbate with blocking and offgas compounds.
    SorbateInteractingCompounds h2oBlockingCo2;
    h2oBlockingCo2.mCompound    = ChemicalCompound::H2O;
    h2oBlockingCo2.mInteraction = 1.0;
    std::vector<SorbateInteractingCompounds> blockingCompounds;
    blockingCompounds.push_back(h2oBlockingCo2);

    SorbateInteractingCompounds co2OffgasNh3;
    co2OffgasNh3.mCompound    = ChemicalCompound::NH3;
    co2OffgasNh3.mInteraction = 1.0e-4;
    std::vector<SorbateInteractingCompounds> offgasCompounds;
    blockingCompounds.push_back(co2OffgasNh3);

    SorbantProperties* sorbant = tConfigData->addCustomSorbant(500.0, 0.4, 400.0);
    sorbant->addSorbate(ChemicalCompound::H2O, 0,                  0,                1.767e+2, 2.787e-5,  1.093e+3, -1.190e-3,  2.213e+1, -50.2, 0.002);
    sorbant->addSorbate(ChemicalCompound::CO2, &blockingCompounds, &offgasCompounds, 7.678e-6, 5.164e-7,  2.330e+3, -3.053e-1,  2.386e+2, -40.0, 0.011375);

    const SorbateProperties* sorbateCo2 = &sorbant->getSorbates()->at(1);
    /// - Note that normally you'd pass this the internal fluid of a link, not a node, but the
    ///   node's fluid is convenient here and suffices for this test.
    CPPUNIT_ASSERT_NO_THROW(article.init(sorbateCo2, 2, 1, 1.0e-4, tNodes[0].getContent()));
    CPPUNIT_ASSERT(sorbateCo2 == article.mProperties);
    CPPUNIT_ASSERT(2          == article.mFluidIndexes.mFluid);
    CPPUNIT_ASSERT(1          == article.mFluidIndexes.mTc);
    CPPUNIT_ASSERT(1.0e-4     == article.mLoading);
    //TODO assert offgas stuff

    //TODO loading <0 exception

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSorptionBed::testDefaultConstruction()
{
    UT_RESULT;

//    /// @test state data
//    CPPUNIT_ASSERT(false == tArticle->mFlipFlowSign);
//    CPPUNIT_ASSERT(false == tArticle->mTraceCompoundsOnly);
//    CPPUNIT_ASSERT(0.0   == tArticle->mFlowDemand);
//    CPPUNIT_ASSERT(0     == tArticle->mTraceCompoundRates);
//
//    /// @test init flag
//    CPPUNIT_ASSERT(false == tArticle->mInitFlag);
//
//    /// @test new/delete for code coverage
//    GunnsFluidSorptionBed* article = new GunnsFluidSorptionBed();
//    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSorptionBed::testNominalInitialization()
{
    UT_RESULT;

//    /// - Initialize the test article with nominal data
//    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);
//
//    /// @test config, input & state data
//    CPPUNIT_ASSERT(tFlipFlowSign      == tArticle->mFlipFlowSign);
//    CPPUNIT_ASSERT(tInitialFlowDemand == tArticle->mFlowDemand);
//    CPPUNIT_ASSERT(tArticle->mInternalFluid);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluidInput2->mTemperature,
//                                 tArticle->mInternalFluid->getTemperature(),
//                                 DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFractions2[0],
//                                 tArticle->mInternalFluid->getMassFraction(FluidProperties::GUNNS_N2),
//                                 DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFractions2[1],
//                                 tArticle->mInternalFluid->getMassFraction(FluidProperties::GUNNS_O2),
//                                 DBL_EPSILON);
//    CPPUNIT_ASSERT(tArticle->mTraceCompoundRates);
//    CPPUNIT_ASSERT(tArticle->mTraceCompoundRates[0] == tTcRatesState[0]);
//    CPPUNIT_ASSERT(tArticle->mTraceCompoundRates[1] == tTcRatesState[1]);
//
//    /// @test init flag
//    CPPUNIT_ASSERT(true               == tArticle->mInitFlag);
//
//    /// @test trace compound rates are zeroed if no trace compounds in link input data
//    tInputData->mInternalFluid->mTraceCompounds = 0;
//    delete tArticle;
//    tArticle = new FriendlyGunnsFluidSorptionBed;
//    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);
//
//    CPPUNIT_ASSERT(tFlipFlowSign      == tArticle->mFlipFlowSign);
//    CPPUNIT_ASSERT(tInitialFlowDemand == tArticle->mFlowDemand);
//    CPPUNIT_ASSERT(tArticle->mInternalFluid);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluidInput2->mTemperature,
//                                 tArticle->mInternalFluid->getTemperature(),
//                                 DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFractions2[0],
//                                 tArticle->mInternalFluid->getMassFraction(FluidProperties::GUNNS_N2),
//                                 DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFractions2[1],
//                                 tArticle->mInternalFluid->getMassFraction(FluidProperties::GUNNS_O2),
//                                 DBL_EPSILON);
//    CPPUNIT_ASSERT(tArticle->mTraceCompoundRates);
//    CPPUNIT_ASSERT(0.0 == tArticle->mTraceCompoundRates[0]);
//    CPPUNIT_ASSERT(0.0 == tArticle->mTraceCompoundRates[1]);
//    CPPUNIT_ASSERT(true               == tArticle->mInitFlag);
//
//    /// @test initialization for no trace compounds in network
//    tFluidConfig->mTraceCompounds = 0;
//    GunnsFluidNode nodes[2];
//    tNodeList.mNodes    = nodes;
//    tFluidInput1->mTraceCompounds = 0;
//    nodes[0].initialize("nodes_0", tFluidConfig);
//    nodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput1);
//
//    delete tArticle;
//    tArticle = new FriendlyGunnsFluidSorptionBed;
//    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);
//
//    CPPUNIT_ASSERT(tFlipFlowSign      == tArticle->mFlipFlowSign);
//    CPPUNIT_ASSERT(tInitialFlowDemand == tArticle->mFlowDemand);
//    CPPUNIT_ASSERT(tArticle->mInternalFluid);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluidInput2->mTemperature,
//                                 tArticle->mInternalFluid->getTemperature(),
//                                 DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFractions2[0],
//                                 tArticle->mInternalFluid->getMassFraction(FluidProperties::GUNNS_N2),
//                                 DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFractions2[1],
//                                 tArticle->mInternalFluid->getMassFraction(FluidProperties::GUNNS_O2),
//                                 DBL_EPSILON);
//    CPPUNIT_ASSERT(!tArticle->mTraceCompoundRates);
//    CPPUNIT_ASSERT(true               == tArticle->mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for exceptions thrown during initialization.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSorptionBed::testInitializationExceptions()
{
    UT_RESULT;

//    /// @test exception thrown on no internal fluid provided.
//    tInputData->mInternalFluid = 0;
//    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0),
//                         TsInitializationException);
//    tInputData->mInternalFluid = tFluidInput2;
//
//    /// @test exception thrown on trace compounds only flag set when network has no trace compounds.
//    tConfigData->mTraceCompoundsOnly = true;
//    tFluidConfig->mTraceCompounds = 0;
//    GunnsFluidNode nodes[2];
//    tNodeList.mNodes    = nodes;
//    tFluidInput1->mTraceCompounds = 0;
//    nodes[0].initialize("nodes_0", tFluidConfig);
//    nodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput1);
//
//    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0),
//                         TsInitializationException);
//    tFluidConfig->mTraceCompounds = tFluidTcConfig;
//    tFluidInput1->mTraceCompounds = tFluidTcInput;
//    tNodeList.mNodes              = tNodes;
//
//    /// @test exception thrown on trace compounds only flag but input data has no trace compounds.
//    tInputData->mInternalFluid->mTraceCompounds = 0;
//    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0),
//                         TsInitializationException);
//    tInputData->mInternalFluid->mTraceCompounds = tTcInput;
//
//    /// @test init flag
//    CPPUNIT_ASSERT(false == tArticle->mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSorptionBed::testRestart()
{
    UT_RESULT;

//    /// - Initialize the test article with nominal data
//    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);
//
//    /// @test base class restart.
//    tArticle->restart();
//
//    /// - currently nothing to verify.

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test accessors and getters.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSorptionBed::testAccessorsAndGetters()
{
    UT_RESULT;

//    /// - Initialize default test article with nominal initialization data
//    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);
//
//    /// @test setFlowDemand & getFlowDemand methods.
//    tArticle->setFlowDemand(0.1);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1, tArticle->getFlowDemand(), 0.0);
//
//    /// @test setFlowState method.
//    PolyFluid* fluid = tNodes[0].getContent();
//    tArticle->setFlowState(fluid);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(fluid->getPressure(), tArticle->mInternalFluid->getPressure(), 0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the step method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSorptionBed::testStep()
{
    UT_RESULT;

//    /// - Initialize default test article with nominal initialization data
//    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);
//
//    /// - During this step the flow rate will be equal to the given flow demand reduced by the
//    ///   blockage malfunction, and with the sign flipped.
//    tArticle->step(tTimeStep);
//    double expectedMdot = -0.5 * tInitialFlowDemand;
//    double expectedFlux = expectedMdot / tArticle->mInternalFluid->getMWeight();
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot, tArticle->mFlowRate,        DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mFlux,            DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mSourceVector[0], DBL_EPSILON);
//
//    /// - Step again with the blockage malfunction de-activated and flow sign not flipped.
//    tArticle->mFlipFlowSign = false;
//    tArticle->setMalfBlockage();
//    tArticle->step(tTimeStep);
//    expectedMdot = tInitialFlowDemand;
//    expectedFlux = expectedMdot / tArticle->mInternalFluid->getMWeight();
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot, tArticle->mFlowRate,        DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mFlux,            DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mSourceVector[0], DBL_EPSILON);
//
//    /// - Step again with zero molecular weight in the internal fluid.  We do this by hacking the
//    ///   link's internal fluid to point to the Ground node's contents, which hasn't been init'd.
//    PolyFluid* saveFluid     = tArticle->mInternalFluid;
//    tArticle->mInternalFluid = tNodes[1].getContent();
//    tArticle->step(tTimeStep);
//    tArticle->mInternalFluid = saveFluid;
//    expectedMdot = tInitialFlowDemand;
//    expectedFlux = 0.0;
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot, tArticle->mFlowRate,        DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mFlux,            DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mSourceVector[0], DBL_EPSILON);
//
//    /// - Step again with trace compounds only set.
//    tArticle->mTraceCompoundsOnly = true;
//    tArticle->step(tTimeStep);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mFlux,            DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows with flow into the node.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSorptionBed::testComputeFlowsToNode()
{
    UT_RESULT;

//    /// - Initialize default test article with nominal initialization data, forward flow and no
//    ///   blockage.
//    tInputData->mMalfBlockageFlag = false;
//    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);
//
//    tArticle->step(tTimeStep);
//    tArticle->computeFlows(tTimeStep);
//
//    CPPUNIT_ASSERT(GunnsBasicLink::SINK == tArticle->mPortDirections[0]);
//
//    tArticle->transportFlows(tTimeStep);
//
//    const double expectedP   =  tNodes[0].getPotential();
//    const double expectedQ   = -tInitialFlowDemand / tArticle->getInternalFluid()->getDensity();
//    const double expectedPwr = 1000.0 * expectedQ * expectedP;
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedP,                 tArticle->mPotentialDrop,                DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedQ,                 tArticle->mVolFlowRate,                  DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedPwr,               tArticle->mPower,                        DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(-tInitialFlowDemand,        tNodes[0].getInflux(),                   DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluidInput2->mTemperature, tNodes[0].getInflow()->getTemperature(), FLT_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,                       tNodes[0].getOutflux(),                  DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows with flow out of the node.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSorptionBed::testComputeFlowsFromNode()
{
    UT_RESULT;

//    /// - Initialize default test article with nominal initialization data, reverse flow and no
//    ///   blockage.
//    tInputData->mMalfBlockageFlag = false;
//    tInputData->mFlowDemand       = -tInitialFlowDemand;
//    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);
//
//    tArticle->step(tTimeStep);
//    tArticle->computeFlows(tTimeStep);
//
//    CPPUNIT_ASSERT(GunnsBasicLink::SINK == tArticle->mPortDirections[0]);
//
//    tArticle->transportFlows(tTimeStep);
//
//    const double expectedP   = -tNodes[0].getPotential();
//    const double expectedQ   = tInitialFlowDemand / tArticle->getInternalFluid()->getDensity();
//    const double expectedPwr = 1000.0 * expectedQ * expectedP;
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedP,                 tArticle->mPotentialDrop,                DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedQ,                 tArticle->mVolFlowRate,                  DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedPwr,               tArticle->mPower,                        DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL( tInitialFlowDemand,        tNodes[0].getInflux(),                   DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluidInput2->mTemperature, tNodes[0].getInflow()->getTemperature(), FLT_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,                       tNodes[0].getOutflux(),                  DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows with zero flow demand.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSorptionBed::testComputeFlowsZeroFlow()
{
    UT_RESULT;

//    /// - Initialize default test article with nominal initialization data, zero flow and no
//    ///   blockage.
//    tInputData->mMalfBlockageFlag = false;
//    tInputData->mFlowDemand       = 0.0;
//    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);
//
//    tArticle->step(tTimeStep);
//    tArticle->computeFlows(tTimeStep);
//
//    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tArticle->mPortDirections[0]);
//
//    tArticle->transportFlows(tTimeStep);
//
//    const double expectedP   = -tNodes[0].getPotential();
//    const double expectedQ   = 0.0;
//    const double expectedPwr = 0.0;
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedP,   tArticle->mPotentialDrop, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedQ,   tArticle->mVolFlowRate,   DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedPwr, tArticle->mPower,         DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,         tNodes[0].getInflux(),    DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,         tNodes[0].getOutflux(),   DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows with zero internal fluid density.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSorptionBed::testComputeFlowsZeroDensity()
{
    UT_RESULT;

//    /// - Initialize default test article with nominal initialization data, forward flow and no
//    ///   blockage.
//    tInputData->mMalfBlockageFlag = false;
//    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);
//
//    tArticle->step(tTimeStep);
//
//    /// - Force the internal fluid density to zero.  We do this by hacking the link's internal fluid
//    ///   to point to the Ground node's contents, which hasn't been init'd.
//    PolyFluid* saveFluid     = tArticle->mInternalFluid;
//    tArticle->mInternalFluid = tNodes[1].getOutflow();
//    tArticle->computeFlows(tTimeStep);
//    tArticle->transportFlows(tTimeStep);
//    tArticle->mInternalFluid = saveFluid;
//
//    const double expectedQ   = 0.0;
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedQ, tArticle->mVolFlowRate, DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows with trace compounds only.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSorptionBed::testComputeFlowsTcOnly()
{
    UT_RESULT;

//    /// - Initialize default test article with nominal initialization data and trace compounds only.
//    tConfigData->mTraceCompoundsOnly = true;
//    tConfigData->mFlipFlowSign       = false;
//    tInputData->mMalfBlockageFlag    = false;
//    tInputData->mFlowDemand          = 1.0;
//    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);
//
//    tArticle->step(tTimeStep);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tArticle->mFlowRate, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mFlux,     DBL_EPSILON);
//
//    tArticle->computeFlows(tTimeStep);
//
//    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tArticle->mPortDirections[0]);
//
//    tArticle->transportFlows(tTimeStep);
//
//    FriendlyGunnsFluidSorptionBedNode* node = static_cast<FriendlyGunnsFluidSorptionBedNode*>(&tNodes[0]);
//    const double mdotH2O = node->mTcInflow.mState[0];
//    const double mdotCO2 = node->mTcInflow.mState[1];
//
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,                              tArticle->mFlowRate, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                              tArticle->mFlux,     DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mTraceCompoundRates[0], mdotH2O,             DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mTraceCompoundRates[1], mdotCO2,             DBL_EPSILON);

    UT_PASS_LAST;
}
