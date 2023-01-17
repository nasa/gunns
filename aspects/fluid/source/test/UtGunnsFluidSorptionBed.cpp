/*
@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/

#include "strings/UtResult.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
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
    tPort1(),
    tTimeStep(),
    tFluidProperties(),
    tCompoundProperties(),
    tTcTypes(),
    tFluidTcConfig(),
    tFluidConfig(),
    tFluidTcInput(),
    tFluidInput1(),
    tFluidInput2(),
    tFractions1(),
    tFractions2(),
    tCustomSorbant()
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
    delete tFluidInput2;
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
    tNodeList.mNumNodes = 4;
    tNodeList.mNodes    = tNodes;
    tPort0              = 0;
    tPort1              = 1;
    tTcTypes[0]         = ChemicalCompound::H2O;
    tTcTypes[1]         = ChemicalCompound::CO2;
    tTcTypes[2]         = ChemicalCompound::NH3;
    tTimeStep           = 0.1;

    /// - Setup some fluid nodes.  CO2 will be both a bulk constituent and a trace compound, so we
    ///   can test that the link handles such a case.
    tFluidProperties = new DefinedFluidProperties();
    FluidProperties::FluidType types[4];
    types[0]    = FluidProperties::GUNNS_N2;
    types[1]    = FluidProperties::GUNNS_O2;
    types[2]    = FluidProperties::GUNNS_CO2;
    types[3]    = FluidProperties::GUNNS_WATER;
    tFractions1[0] = 0.8;
    tFractions1[1] = 0.19;
    tFractions1[2] = 0.01;
    tFractions1[3] = 0.0;

    tFractions2[0] = 0.0;
    tFractions2[1] = 0.0;
    tFractions2[2] = 0.0;
    tFractions2[3] = 1.0;

    tCompoundProperties = new DefinedChemicalCompounds();
    tFluidTcConfig = new GunnsFluidTraceCompoundsConfigData(tTcTypes, 3, "tFluidTcConfig");
    tFluidConfig = new PolyFluidConfigData(tFluidProperties, types, 4, tFluidTcConfig);

    double tcConcentrations[3];
    tcConcentrations[0] = 5.0e-4;
    tcConcentrations[1] = 6.0e-6;
    tcConcentrations[2] = 1.0e-5;
    tFluidTcInput = new GunnsFluidTraceCompoundsInputData(tcConcentrations);

    tFluidInput1 = new PolyFluidInputData(283.15,                   //temperature
                                          700.728,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          tFractions1,              //massFractions
                                          tFluidTcInput);           //trace compounds

    tFluidInput2 = new PolyFluidInputData(283.15,                   //temperature
                                          100.0,                    //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          tFractions2,              //massFractions
                                          0);                       //no trace compounds

    /// - Have to initialize the nodes with the fluid configs (normally done by GUNNS)
    tNodes[0].initialize("UtTestNode0", tFluidConfig);
    tNodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput1);
    tNodes[0].resetFlows();

    tNodes[1].initialize("UtTestNode1", tFluidConfig);
    tNodes[1].getContent()->initialize(*tFluidConfig, *tFluidInput1);
    tNodes[1].resetFlows();

    /// - Liquid node for testing port rules.
    tNodes[2].initialize("UtTestNode2", tFluidConfig);
    tNodes[2].getContent()->initialize(*tFluidConfig, *tFluidInput2);
    tNodes[2].resetFlows();

    /// - Ground node.
    tNodes[3].initialize("UtTestNode3", tFluidConfig);
    tNodes[3].resetFlows();

    /// - Define nominal configuration data.  Segment 0 has a defined sorbant, and segment 1
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
    SorbateInteractingCompounds co2OffgasO2;
    co2OffgasNh3.mCompound    = ChemicalCompound::NH3;
    co2OffgasO2.mCompound     = ChemicalCompound::O2;  // not realistic, this is just for testing
    co2OffgasNh3.mInteraction = 1.0e-4;
    co2OffgasO2.mInteraction  = 5.0e-5;
    std::vector<SorbateInteractingCompounds> offgasCompounds;
    offgasCompounds.push_back(co2OffgasNh3);
    offgasCompounds.push_back(co2OffgasO2);

    tCustomSorbant = tConfigData->addCustomSorbant(500.0, 0.4, 400.0);
    tCustomSorbant->addSorbate(ChemicalCompound::H2O, 0,                  0,                1.767e+2, 2.787e-5,  1.093e+3, -1.190e-3,  2.213e+1, -50.2, 0.002);
    tCustomSorbant->addSorbate(ChemicalCompound::CO2, &blockingCompounds, &offgasCompounds, 7.678e-6, 5.164e-7,  2.330e+3, -3.053e-1,  2.386e+2, -40.0, 0.011375);
    tConfigData->addSegment(tCustomSorbant, 0.001, 200.0);

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

    /// @test copy constructor
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
    offgasCompounds.push_back(co2OffgasNh3);

    SorbantProperties* sorbant = tConfigData->addCustomSorbant(500.0, 0.4, 400.0);
    sorbant->addSorbate(ChemicalCompound::H2O, 0,                  0,                1.767e+2, 2.787e-5,  1.093e+3, -1.190e-3,  2.213e+1, -50.2, 0.002);
    sorbant->addSorbate(ChemicalCompound::CO2, &blockingCompounds, &offgasCompounds, 7.678e-6, 5.164e-7,  2.330e+3, -3.053e-1,  2.386e+2, -40.0, 0.011375);

    const SorbateProperties* sorbateCo2 = &sorbant->getSorbates()->at(1);

    /// - Note that normally you'd pass this the internal fluid of a link, not a node, but the
    ///   node's fluid is convenient here and suffices for this test.
    const double loading = 1.0e-4;
    const double volume  = 1.0;
    const double mwCo2   = 44.0095;
    const double expectedMass = loading * volume * mwCo2;
    CPPUNIT_ASSERT_NO_THROW(article.init(sorbateCo2, 2, 1, 1.0e-4, 1.0, tNodes[0].getContent()));
    CPPUNIT_ASSERT(sorbateCo2 == article.mProperties);
    CPPUNIT_ASSERT(sorbateCo2 == article.getProperties());
    CPPUNIT_ASSERT(2          == article.mFluidIndexes.mFluid);
    CPPUNIT_ASSERT(1          == article.mFluidIndexes.mTc);
    CPPUNIT_ASSERT(loading    == article.mLoading);
    CPPUNIT_ASSERT(1          == sorbateCo2->getBlockingCompounds()->size());
    CPPUNIT_ASSERT(1          == sorbateCo2->getOffgasCompounds()->size());
    CPPUNIT_ASSERT(1          == article.getOffgasIndexes()->size());
    CPPUNIT_ASSERT(-1         == article.getOffgasIndexes()->at(0).mFluid);
    CPPUNIT_ASSERT(2          == article.getOffgasIndexes()->at(0).mTc);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass, article.mLoadedMass, DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSorptionBed::testDefaultConstruction()
{
    UT_RESULT;

    /// @test state data
    CPPUNIT_ASSERT(0   == tArticle->mSegments);
    CPPUNIT_ASSERT(0   == tArticle->mNSegments);
    CPPUNIT_ASSERT(0.0 == tArticle->mVolume);
    CPPUNIT_ASSERT(0   == tArticle->mAdsorptionFluidRates);
    CPPUNIT_ASSERT(0   == tArticle->mAdsorptionTcRates);
    CPPUNIT_ASSERT(0   == tArticle->mAdsorbedFluidMasses);
    CPPUNIT_ASSERT(0   == tArticle->mAdsorbedTcMasses);

    /// @test init flag
    CPPUNIT_ASSERT(false == tArticle->mInitFlag);

    /// @test new/delete GunnsFluidSorptionBed for code coverage
    GunnsFluidSorptionBed* article = new GunnsFluidSorptionBed();
    delete article;

    /// @test new/delete GunnsFluidSorptionBedSegment for coverage
    GunnsFluidSorptionBedSegment* segment = new GunnsFluidSorptionBedSegment();
    delete segment;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSorptionBed::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize the test article with nominal data
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1));

    /// @test creation of internal fluid.
    CPPUNIT_ASSERT(0 != tArticle->mInternalFluid);
    CPPUNIT_ASSERT(4 == tArticle->mInternalFluid->getNConstituents());
    CPPUNIT_ASSERT(4 == tNodes[0].getFluidConfig()->mNTypes);

    /// @test initialization of bed segments.
    CPPUNIT_ASSERT(false                                == tArticle->mSegments[0].mMalfDegradeFlag);
    CPPUNIT_ASSERT(0.0                                  == tArticle->mSegments[1].mMalfDegradeFlag);
    CPPUNIT_ASSERT(false                                == tArticle->mSegments[0].mMalfDegradeValue);
    CPPUNIT_ASSERT(0.0                                  == tArticle->mSegments[1].mMalfDegradeValue);
    CPPUNIT_ASSERT(2                                    == tArticle->mSegments[0].getNSorbates());
    CPPUNIT_ASSERT(2                                    == tArticle->mSegments[1].getNSorbates());
    CPPUNIT_ASSERT(-1                                   == tArticle->mSegments[0].mSorbates[0].getFluidIndexes()->mFluid);
    CPPUNIT_ASSERT(0                                    == tArticle->mSegments[0].mSorbates[0].getFluidIndexes()->mTc);
    CPPUNIT_ASSERT(2                                    == tArticle->mSegments[0].mSorbates[1].getFluidIndexes()->mFluid);
    CPPUNIT_ASSERT(1                                    == tArticle->mSegments[0].mSorbates[1].getFluidIndexes()->mTc);
    CPPUNIT_ASSERT(-1                                   == tArticle->mSegments[1].mSorbates[0].getFluidIndexes()->mFluid);
    CPPUNIT_ASSERT(0                                    == tArticle->mSegments[1].mSorbates[0].getFluidIndexes()->mTc);
    CPPUNIT_ASSERT(0                                    == tArticle->mSegments[1].mSorbates[0].getOffgasIndexes()->size());
    CPPUNIT_ASSERT(2                                    == tArticle->mSegments[1].mSorbates[1].getProperties()->getOffgasCompounds()->size());
    CPPUNIT_ASSERT(2                                    == tArticle->mSegments[1].mSorbates[1].getOffgasIndexes()->size());
    CPPUNIT_ASSERT(2                                    == tArticle->mSegments[1].mSorbates[1].getFluidIndexes()->mFluid);
    CPPUNIT_ASSERT(1                                    == tArticle->mSegments[1].mSorbates[1].getFluidIndexes()->mTc);
    CPPUNIT_ASSERT(tConfigData->mSegments.at(0).mVolume == tArticle->mSegments[0].mVolume);
    CPPUNIT_ASSERT(tConfigData->mSegments.at(1).mVolume == tArticle->mSegments[1].mVolume);
    CPPUNIT_ASSERT(tInputData->mWallTemperature         == tArticle->mSegments[0].mTemperature);
    CPPUNIT_ASSERT(tInputData->mWallTemperature         == tArticle->mSegments[1].mTemperature);
    CPPUNIT_ASSERT(0.0                                  == tArticle->mSegments[0].mHeatFlux);
    CPPUNIT_ASSERT(0.0                                  == tArticle->mSegments[1].mHeatFlux);
    CPPUNIT_ASSERT(0.0                                  == tArticle->mSegments[0].mThermCap);
    CPPUNIT_ASSERT(0.0                                  == tArticle->mSegments[1].mThermCap);
    CPPUNIT_ASSERT(tArticle->mInternalFluid             == tArticle->mSegments[0].mFluid);
    CPPUNIT_ASSERT(tArticle->mInternalFluid             == tArticle->mSegments[1].mFluid);

    /// @test output loading arrays initialized from loadings in input data.
    double expectedCo2Fluid = 0.0;
    double expectedH2oTc    = 0.0;
    for (unsigned int i=0; i<tInputData->mLoading.size(); ++i) {
        if (ChemicalCompound::H2O == tInputData->mLoading.at(i).mSorbate) {
            /// - H2O is only a trace compound in this test.
            const unsigned int segment = tInputData->mLoading.at(i).mSegment;
            const double segVol = tConfigData->mSegments.at(segment).mVolume; //assuming no degrade malf
            const double segSorbVol = tConfigData->mSegments.at(segment).mProperties->computeVolume(segVol);
            const double mw = 18.0153; // independent check of compound properties MW H2O
            expectedH2oTc += tInputData->mLoading.at(i).mLoading * segSorbVol * mw;
        } else if (ChemicalCompound::CO2 == tInputData->mLoading.at(i).mSorbate) {
            /// - CO2 is both a trace compound and a bulk fluid, so the model only loads the bulk fluid.
            const unsigned int segment = tInputData->mLoading.at(i).mSegment;
            const double segVol = tConfigData->mSegments.at(segment).mVolume; //assuming no degrade malf
            const double segSorbVol = tConfigData->mSegments.at(segment).mProperties->computeVolume(segVol);
            const double mw = 44.0095; // independent check of compound properties MW CO2
            expectedCo2Fluid += tInputData->mLoading.at(i).mLoading * segSorbVol * mw;
        }
    }
    CPPUNIT_ASSERT(0 != tArticle->mAdsorptionFluidRates);
    CPPUNIT_ASSERT(0 != tArticle->mAdsorptionTcRates);
    CPPUNIT_ASSERT(0 != tArticle->mAdsorbedFluidMasses);
    CPPUNIT_ASSERT(0 != tArticle->mAdsorbedTcMasses);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tArticle->getAdsorbedFluidMass(FluidProperties::GUNNS_N2),    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tArticle->getAdsorbedFluidMass(FluidProperties::GUNNS_O2),    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCo2Fluid, tArticle->getAdsorbedFluidMass(FluidProperties::GUNNS_CO2),   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH2oTc,    tArticle->getAdsorbedTcMass(ChemicalCompound::H2O),           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tArticle->getAdsorbedTcMass(ChemicalCompound::CO2),           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tArticle->getAdsorptionFluidRate(FluidProperties::GUNNS_N2),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tArticle->getAdsorptionFluidRate(FluidProperties::GUNNS_O2),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tArticle->getAdsorptionFluidRate(FluidProperties::GUNNS_CO2), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tArticle->getAdsorptionTcRate(ChemicalCompound::H2O),         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tArticle->getAdsorptionTcRate(ChemicalCompound::CO2),         DBL_EPSILON);

    /// @test initialization flag.
    CPPUNIT_ASSERT(true               == tArticle->mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for exceptions thrown during initialization.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSorptionBed::testInitializationExceptions()
{
    UT_RESULT;

    /// @test exception thrown on bed wall temperature out of range.
    tInputData->mWallTemperature = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tInputData->mWallTemperature = tWallTemperature;

    /// @test exception thrown on a segment loading's segment value out of range.
    const unsigned int savedSegment = tInputData->mLoading.at(0).mSegment;
    tInputData->mLoading.at(0).mSegment = 2;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tInputData->mLoading.at(0).mSegment = savedSegment;

    /// @test exception thrown on segment loading's loading value out of range.
    const unsigned int savedLoading = tInputData->mLoading.at(0).mLoading;
    tInputData->mLoading.at(0).mLoading = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tInputData->mLoading.at(0).mLoading = savedLoading;

    /// @test exception thrown on a segment missing sorbant properties.
    const SorbantProperties* savedProperties = tConfigData->mSegments.at(0).mProperties;
    tConfigData->mSegments.at(0).mProperties = 0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mSegments.at(0).mProperties = savedProperties;

    /// @test exception thrown on a segment with zero volume.
    const double savedVolume = tConfigData->mSegments.at(0).mVolume;
    tConfigData->mSegments.at(0).mVolume = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mSegments.at(0).mVolume = savedVolume;

    /// @test exception thrown on a segment with zero heat transfer coefficient.
    const double savedHtc = tConfigData->mSegments.at(0).mHtc;
    tConfigData->mSegments.at(0).mHtc = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mSegments.at(0).mHtc = savedHtc;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSorptionBed::testRestart()
{
    UT_RESULT;

    /// - Initialize the test article with nominal data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test non-zero outputs after initialization.
    CPPUNIT_ASSERT(0.0 < tArticle->getAdsorbedFluidMass(FluidProperties::GUNNS_CO2));
    CPPUNIT_ASSERT(0.0 < tArticle->getAdsorbedTcMass(ChemicalCompound::H2O));
    CPPUNIT_ASSERT(0.0 < tArticle->mEffectiveConductivity);
    CPPUNIT_ASSERT(0.0 < tArticle->mSegments[0].mSorbates[1].mLoadedMass);

    /// - Set some non-checkpointed attributes.
    tArticle->mAdsorptionFluidRates[0] = 1.0;
    tArticle->mAdsorptionTcRates[0]    = 1.0;

    /// @test base class restart.
    tArticle->restart();
    CPPUNIT_ASSERT(0.0 == tArticle->mEffectiveConductivity);

    /// @test non-checkpointed and non-config data attributes are reset.
    CPPUNIT_ASSERT(0.0 == tArticle->getAdsorbedFluidMass(FluidProperties::GUNNS_CO2));
    CPPUNIT_ASSERT(0.0 == tArticle->getAdsorbedTcMass(ChemicalCompound::H2O));
    CPPUNIT_ASSERT(0.0 == tArticle->mAdsorptionFluidRates[0]);
    CPPUNIT_ASSERT(0.0 == tArticle->mAdsorptionTcRates[0]);
    CPPUNIT_ASSERT(0.0 == tArticle->mSegments[0].mSorbates[1].mLoadedMass);

    /// - Restart a bed sorbate directly for coverage
    tArticle->mSegments[0].mSorbates[0].restartModel();

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the UtGunnsFluidSorptionBed::updateLoadingEquil method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSorptionBed::testBedSorbateLoadingEquil()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    {
        /// @test nominal equilibrium loading.
        DefinedSorbantProperties definedSorbProps;
        const SorbantProperties* silGelB125 = definedSorbProps.getSorbant(SorbantProperties::SILICA_GEL_B125);
        const std::vector<SorbateProperties>* sorbates = silGelB125->getSorbates();

        const double pp                 =   0.1;
        const double temperature        = 290.0;
        const double expectEquilLoading = sorbates->at(1).computeLoadingEquil(pp, temperature);
        tArticle->mSegments[0].mSorbates[1].updateLoadingEquil(pp, temperature);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectEquilLoading, tArticle->mSegments[0].mSorbates[1].mLoadingEquil, DBL_EPSILON);

        /// @test equilibrium loading malfunction.
        tArticle->mSegments[0].mSorbates[1].mMalfLoadingEquilFlag  = true;
        tArticle->mSegments[0].mSorbates[1].mMalfLoadingEquilValue = 0.5;
        tArticle->mSegments[0].mSorbates[1].updateLoadingEquil(pp, temperature);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5 * expectEquilLoading, tArticle->mSegments[0].mSorbates[1].mLoadingEquil, DBL_EPSILON);
    } {
        /// @test equilibrium loading with blocking compounds
        const std::vector<SorbateProperties>* sorbates = tCustomSorbant->getSorbates();

        const double pp                 =   0.1;
        const double temperature        = 290.0;
        const double blockerFraction    =   0.5;
        tArticle->mSegments[1].mSorbates[0].mLoadingFraction = blockerFraction;
        const double expectedBlockingInteraction = 1.0; // as defined in setUp, h2oBlockingCo2
        const double blockingFactor     = 1.0 - blockerFraction * expectedBlockingInteraction;
        const double expectEquilLoading = sorbates->at(1).computeLoadingEquil(pp, temperature)
                                        * blockingFactor;
        tArticle->mSegments[1].mSorbates[1].updateLoadingEquil(pp, temperature);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectEquilLoading, tArticle->mSegments[1].mSorbates[1].mLoadingEquil, DBL_EPSILON);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the UtGunnsFluidSorptionBed::updateLoading and updateLoaadedMass methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSorptionBed::testBedSorbateUpdateLoading()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test UtGunnsFluidSorptionBed::updateLoading.
    DefinedSorbantProperties definedSorbProps;
    const SorbantProperties* silGelB125 = definedSorbProps.getSorbant(SorbantProperties::SILICA_GEL_B125);
    const std::vector<SorbateProperties>* sorbates = silGelB125->getSorbates();
    const double pp               =   0.1;
    const double temperature      = 290.0;
    const double loadingEquil     = sorbates->at(1).computeLoadingEquil(pp, temperature);
    {
        /// @test nominal equilibrium loading > loading, adsorb rate not limited.
        const double loading          = 0.75 * loadingEquil;
        const double expectedRate     = sorbates->at(1).computeLoadingRate(loadingEquil, loading);
        const double expectedLoading  = loading + expectedRate * tTimeStep;
        const double expectedFraction = expectedLoading / loadingEquil;
        tArticle->mSegments[0].mSorbates[1].mLoadingEquil = loadingEquil;
        tArticle->mSegments[0].mSorbates[1].mLoading      = loading;
        tArticle->mSegments[0].mSorbates[1].updateLoading(tTimeStep, 999.9, 999.9);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRate,     tArticle->mSegments[0].mSorbates[1].mLoadingRate,     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedLoading,  tArticle->mSegments[0].mSorbates[1].mLoading,         DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFraction, tArticle->mSegments[0].mSorbates[1].mLoadingFraction, DBL_EPSILON);
    } {
        /// @test nominal equilibrium loading > loading, adsorb rate limited by influx.
        const double loading          = 0.75 * loadingEquil;
        const double defaultRate      = sorbates->at(1).computeLoadingRate(loadingEquil, loading);
        const double influx           = 0.1 * defaultRate;
        const double expectedRate     = influx * 0.99; // GunnsFluidSorptionBedSorbate::mLimitAdsorbFraction
        const double expectedLoading  = loading + expectedRate * tTimeStep;
        const double expectedFraction = expectedLoading / loadingEquil;
        tArticle->mSegments[0].mSorbates[1].mLoadingEquil = loadingEquil;
        tArticle->mSegments[0].mSorbates[1].mLoading      = loading;
        tArticle->mSegments[0].mSorbates[1].updateLoading(tTimeStep, influx, 999.9);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRate,     tArticle->mSegments[0].mSorbates[1].mLoadingRate,     100.0 * DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedLoading,  tArticle->mSegments[0].mSorbates[1].mLoading,         100.0 * DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFraction, tArticle->mSegments[0].mSorbates[1].mLoadingFraction, FLT_EPSILON);
    } {
        /// @test nominal equilibrium loading < loading, desorb rate not limited.
        const double loading          = 1.25 * loadingEquil;
        const double expectedRate     = sorbates->at(1).computeLoadingRate(loadingEquil, loading);
        const double expectedLoading  = loading + expectedRate * tTimeStep;
        const double expectedFraction = expectedLoading / loadingEquil;
        tArticle->mSegments[0].mSorbates[1].mLoadingEquil = loadingEquil;
        tArticle->mSegments[0].mSorbates[1].mLoading      = loading;
        tArticle->mSegments[0].mSorbates[1].updateLoading(tTimeStep, 999.9, 999.9);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRate,     tArticle->mSegments[0].mSorbates[1].mLoadingRate,     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedLoading,  tArticle->mSegments[0].mSorbates[1].mLoading,         DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFraction, tArticle->mSegments[0].mSorbates[1].mLoadingFraction, DBL_EPSILON);
    } {
        /// @test nominal equilibrium loading < loading, desorb rate limited.
        const double loading          = 1.25 * loadingEquil;
        const double defaultRate      = sorbates->at(1).computeLoadingRate(loadingEquil, loading);
        const double desorbRateLimit  = -0.5 * defaultRate; // rate limit to function is positive...
        const double expectedRate     = -desorbRateLimit;   // ...actual expected rate is negative
        const double expectedLoading  = loading + expectedRate * tTimeStep;
        const double expectedFraction = expectedLoading / loadingEquil;
        tArticle->mSegments[0].mSorbates[1].mLoadingEquil = loadingEquil;
        tArticle->mSegments[0].mSorbates[1].mLoading      = loading;
        tArticle->mSegments[0].mSorbates[1].updateLoading(tTimeStep, 999.9, desorbRateLimit);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRate,     tArticle->mSegments[0].mSorbates[1].mLoadingRate,     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedLoading,  tArticle->mSegments[0].mSorbates[1].mLoading,         DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFraction, tArticle->mSegments[0].mSorbates[1].mLoadingFraction, DBL_EPSILON);

        /// @test GunnsFluidSorptionBedSorbate::updateLoadedMass.
        const double sorbantVol   = 2.0;
        const double expectedMass = expectedLoading * sorbantVol * 44.0095; // independent verif. of CO2 MW
        const double expectedSsorbRate = expectedRate * sorbantVol;
        tArticle->mSegments[0].mSorbates[1].updateLoadedMass(sorbantVol);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,      tArticle->mSegments[0].mSorbates[1].mLoadedMass,     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSsorbRate, tArticle->mSegments[0].mSorbates[1].mAdsorptionRate, DBL_EPSILON);

        /// @test GunnsFluidSorptionBedSorbate::computeHeatFlux.
        const double expectedQ = tArticle->mSegments[0].mSorbates[1].getProperties()->computeHeatFlux(tArticle->mSegments[0].mSorbates[1].mAdsorptionRate);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedQ, tArticle->mSegments[0].mSorbates[1].computeHeatFlux(), DBL_EPSILON);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for GunnsFluidSorptionBedSegment::update method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSorptionBed::testBedSegmentUpdate()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - We test segment 1 since it has a custom sorbant with blocking & offgas compounds.
    const double mdotIn = 0.001;
    double       mdot   = mdotIn;
    const double pIn    = 100.5;
    const double pOut   = 100.0;
    const double initH2oLoadEquil = tArticle->mSegments[1].mSorbates[0].mLoadingEquil;
    const double initCo2LoadEquil = tArticle->mSegments[1].mSorbates[1].mLoadingEquil;
    const double initH2oSorbRate  = tArticle->mSegments[1].mSorbates[0].mAdsorptionRate;
    const double initCo2SorbRate  = tArticle->mSegments[1].mSorbates[1].mAdsorptionRate;

    /// - Prepare the test article's internal fluid with the state of the supplying node.
    tArticle->mInternalFluid->setState(tNodes[0].getContent());
    for (int i=0; i<tNodes[0].getFluidConfig()->mNTypes; ++i) {
        tArticle->mInternalFluid->setMass(i, tNodes[0].getContent()->getMassFraction(i) * mdotIn);
    }

    /// - Duplicate the prepared internal fluid for comparisons.
    PolyFluid testFluid(*tArticle->mInternalFluid, "testFluid");

    /// @test sorbates got updated.
    const double degradation  = 0.5;
    tArticle->mSegments[1].setMalfDegrade(true, degradation);
    tArticle->mSegments[1].update(mdot, pIn, pOut, tTimeStep);

    const double h2oLoadEquil = tArticle->mSegments[1].mSorbates[0].mLoadingEquil;
    const double co2LoadEquil = tArticle->mSegments[1].mSorbates[1].mLoadingEquil;
    const double h2oSorbRate  = tArticle->mSegments[1].mSorbates[0].mAdsorptionRate;
    const double co2SorbRate  = tArticle->mSegments[1].mSorbates[1].mAdsorptionRate;
    CPPUNIT_ASSERT(initH2oLoadEquil != h2oLoadEquil);
    CPPUNIT_ASSERT(initCo2LoadEquil != co2LoadEquil);
    CPPUNIT_ASSERT(initH2oSorbRate  != h2oSorbRate);
    CPPUNIT_ASSERT(initCo2SorbRate  != co2SorbRate);

    /// @test heat of convection and reactions to fluid.
    const double expectedQconvect = GunnsFluidUtils::computeConvectiveHeatFlux(&testFluid, mdotIn, tConfigData->mSegments[1].mHtc, tWallTemperature);
    const double expectedQH2o     = tCustomSorbant->getSorbates()->at(0).computeHeatFlux(h2oSorbRate);
    const double expectedQCo2     = tCustomSorbant->getSorbates()->at(1).computeHeatFlux(co2SorbRate);
    const double expectedQ        = expectedQconvect + expectedQH2o + expectedQCo2;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedQ, tArticle->mSegments[1].mHeatFlux, DBL_EPSILON);

    /// @test updated segment thermal capacity as the sorbant plus the loaded sorbate capacities,
    ///       not including sorbates that are only trace comopunds (in this case, H2O).
    const double volSorbant = tCustomSorbant->computeVolume(tConfigData->mSegments.at(1).mVolume);
    const double tcSorbant  = tCustomSorbant->computeThermalCapacity(volSorbant);
    const double co2Cp      = testFluid.getProperties(FluidProperties::GUNNS_CO2)->getSpecificHeat(tWallTemperature);
    const double tcCo2      = co2Cp * tArticle->mSegments[1].mSorbates[1].mLoadedMass;
    const double expectedTc = tcSorbant + tcCo2;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTc, tArticle->mSegments[1].mThermCap, DBL_EPSILON);

    /// @test effects of degradation malfunction.
    const double degradedVol = volSorbant * (1.0 - degradation);
    const double expectedH2oLoadMass = tArticle->mSegments[1].mSorbates[0].mLoading * degradedVol * 18.0153;
    const double expectedCo2LoadMass = tArticle->mSegments[1].mSorbates[1].mLoading * degradedVol * 44.0095;
    const double expectedH2oSorbRate = tArticle->mSegments[1].mSorbates[0].mLoadingRate * degradedVol;
    const double expectedCo2SorbRate = tArticle->mSegments[1].mSorbates[1].mLoadingRate * degradedVol;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH2oLoadMass, tArticle->mSegments[1].mSorbates[0].mLoadedMass, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCo2LoadMass, tArticle->mSegments[1].mSorbates[1].mLoadedMass, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH2oSorbRate, h2oSorbRate, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCo2SorbRate, co2SorbRate, DBL_EPSILON);

    /// @test updated exit temperature.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(testFluid.getTemperature(), tArticle->mInternalFluid->getTemperature(), DBL_EPSILON);

    /// - Get bulk constituent mdots of inlet.
    const double mdotN2In  = mdotIn * testFluid.getMassFraction(FluidProperties::GUNNS_N2);
    const double mdotO2In  = mdotIn * testFluid.getMassFraction(FluidProperties::GUNNS_O2);
    const double mdotCo2In = mdotIn * testFluid.getMassFraction(FluidProperties::GUNNS_CO2);
    const double ndotIn    = mdotIn / testFluid.getMWeight();

    /// - Get trace compounds mdots of inlet.
    GunnsFluidTraceCompounds* testTc     = testFluid.getTraceCompounds();
    GunnsFluidTraceCompounds* internalTc = tArticle->mInternalFluid->getTraceCompounds();
    const double mdotH2oTcIn  = ndotIn * testTc->getMoleFraction(ChemicalCompound::H2O) * 18.0153;
    const double mdotCo2TcIn  = ndotIn * testTc->getMoleFraction(ChemicalCompound::CO2) * 44.0095;
    const double mdotNh3TcIn  = ndotIn * testTc->getMoleFraction(ChemicalCompound::NH3) * 17.0305; // independent check of compound properties MW NH3
    const double mdotSorbH2o  = tArticle->mSegments[1].mSorbates[0].mAdsorptionRate * 18.0153; // independent check of compound properties MW H2O
    const double mdotSorbCo2  = tArticle->mSegments[1].mSorbates[1].mAdsorptionRate * 44.0095; // independent check of compound properties MW CO2
    const double ndotSorbCo2  = mdotSorbCo2 / 44.0095;
    const double mdotH2oTcOut = mdotH2oTcIn - mdotSorbH2o;
    const double mdotCo2Out   = mdotCo2In + mdotCo2TcIn - mdotSorbCo2;
    const double mdotO2Out    = mdotO2In + ndotSorbCo2 * 31.9988 * 5.0e-5; // from SetUp, co2OffgasO2

    /// @test updated exit mdot.
    const double expectedMdotOut = mdotIn - (mdotSorbCo2 - mdotCo2TcIn) + (mdotO2Out - mdotO2In);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdotOut, mdot, DBL_EPSILON);

    /// @test updated exit mixture of bulk fluids.
    const double expectedXN2  = mdotN2In   / mdot;
    const double expectedXO2  = mdotO2Out  / mdot;
    const double expectedXCo2 = mdotCo2Out / mdot;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(mdotN2In,     tArticle->mInternalFluid->getConstituent(FluidProperties::GUNNS_N2)->getMass(),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mdotO2Out,    tArticle->mInternalFluid->getConstituent(FluidProperties::GUNNS_O2)->getMass(),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mdotCo2Out,   tArticle->mInternalFluid->getConstituent(FluidProperties::GUNNS_CO2)->getMass(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mdot,         tArticle->mInternalFluid->getMass(),                                             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedXN2,  tArticle->mInternalFluid->getMassFraction(FluidProperties::GUNNS_N2),            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedXO2,  tArticle->mInternalFluid->getMassFraction(FluidProperties::GUNNS_O2),            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedXCo2, tArticle->mInternalFluid->getMassFraction(FluidProperties::GUNNS_CO2),           DBL_EPSILON);

    /// @test updated exit mixture of trace compounds.
    const double mdotNh3TcOut = mdotNh3TcIn + ndotSorbCo2 * 17.0305 * 1.0e-4; // from SetUp, co2OffgasNh3

    CPPUNIT_ASSERT_DOUBLES_EQUAL(mdotNh3TcOut, internalTc->getMass(ChemicalCompound::NH3), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mdotH2oTcOut, internalTc->getMass(ChemicalCompound::H2O), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          internalTc->getMass(ChemicalCompound::CO2), DBL_EPSILON);

    const double ndotNh3TcOut = mdotNh3TcOut / 17.0305;
    const double ndotH2oTcOut = mdotH2oTcOut / 18.0153;
    const double expectedNNh3 = ndotNh3TcOut / tArticle->mInternalFluid->getMole();
    const double expectedNH2o = ndotH2oTcOut / tArticle->mInternalFluid->getMole();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNNh3, internalTc->getMoleFraction(ChemicalCompound::NH3), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNH2o, internalTc->getMoleFraction(ChemicalCompound::H2O), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          internalTc->getMoleFraction(ChemicalCompound::CO2), DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Transport Flows method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSorptionBed::testTransportFlows()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test bulk forward flow properties.
    const double Pin  = 101.0;
    const double Pout = 100.0;
    tNodes[0].setPotential(Pin);
    tNodes[1].setPotential(Pout);
    tNodes[0].integrateFlows(tTimeStep);
    tNodes[1].integrateFlows(tTimeStep);
    tArticle->mPotentialVector[0] = Pin;
    tArticle->mPotentialVector[1] = Pout;
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT(0.0 < tArticle->mAdmittanceMatrix[0]);
    tArticle->computeFlows(tTimeStep);
    tArticle->transportFlows(tTimeStep);

    double expectedDP   = Pin - Pout;
    double expectedFlux = tArticle->mAdmittanceMatrix[0] * expectedDP;
    double expectedMdot = expectedFlux * tNodes[0].getContent()->getMWeight();
    double expectedQ    = expectedMdot / tNodes[0].getContent()->getDensity();
    double expectedPwr  = -expectedQ * expectedDP * 1000.0; // kPa to Pa
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDP,   tArticle->mPotentialDrop, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mFlux,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot, tArticle->mFlowRate,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedQ,    tArticle->mVolFlowRate,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPwr,  tArticle->mPower,         DBL_EPSILON);

    /// - Sorption printouts for debugging.
//    std::cout << std::endl;
//    std::cout << tNodes[0].getContent()->getMoleFraction(FluidProperties::GUNNS_CO2) << " " << tNodes[0].getContent()->getTraceCompounds()->getMoleFraction(ChemicalCompound::CO2) << std::endl;
//    std::cout << tNodes[0].getContent()->getTraceCompounds()->getMoleFraction(ChemicalCompound::H2O) << std::endl;
//    std::cout << tArticle->mFlux << std::endl;
//    std::cout << tArticle->mSegments[0].mSorbates[0].mAdsorptionRate << " " << tArticle->mSegments[0].mSorbates[1].mAdsorptionRate << std::endl;
//    std::cout << tArticle->mSegments[1].mSorbates[0].mAdsorptionRate << " " << tArticle->mSegments[1].mSorbates[1].mAdsorptionRate << std::endl;
//    std::cout << tArticle->mInternalFluid->getMoleFraction(FluidProperties::GUNNS_CO2) << " " << tArticle->mInternalFluid->getTraceCompounds()->getMoleFraction(ChemicalCompound::CO2) << std::endl;
//    std::cout << tArticle->mInternalFluid->getTraceCompounds()->getMoleFraction(ChemicalCompound::H2O) << std::endl;

    /// @test chaining of outputs from previous segment to inputs of next segment, by checking that
    ///       downstream segments have less adsorption and heat flux than upstream, assuming similar
    ///       sorption properties and initial loading.
    CPPUNIT_ASSERT(fabs(tArticle->mSegments[0].mHeatFlux)                    > fabs(tArticle->mSegments[1].mHeatFlux));
    CPPUNIT_ASSERT(fabs(tArticle->mSegments[0].mSorbates[0].mAdsorptionRate) > fabs(tArticle->mSegments[1].mSorbates[0].mAdsorptionRate));
    CPPUNIT_ASSERT(fabs(tArticle->mSegments[0].mSorbates[1].mAdsorptionRate) > fabs(tArticle->mSegments[1].mSorbates[1].mAdsorptionRate));

    /// @test sorption outputs to node for forward flow.  This assumes we've already verified the
    ///       bed sorbate and bed segments updates, above.  This assumes all of the incoming CO2
    ///       trace compound gets adsorbed, which we expect in this test configuration.
    double expectedTcCo2AdsorbNdot   = expectedFlux * tNodes[0].getContent()->getTraceCompounds()->getMoleFraction(ChemicalCompound::CO2);
    double expectedBulkCo2AdsorbNdot = tArticle->mSegments[0].mSorbates[1].mAdsorptionRate
                                     + tArticle->mSegments[1].mSorbates[1].mAdsorptionRate
                                     - expectedTcCo2AdsorbNdot;
    double expectedTcH2oAdsorbNdot   = tArticle->mSegments[0].mSorbates[0].mAdsorptionRate
                                     + tArticle->mSegments[1].mSorbates[0].mAdsorptionRate;
    double expectedBulkO2DesorbNdot  = tArticle->mSegments[1].mSorbates[1].mAdsorptionRate * 5.0e-5; // from setUp, co2OffgasO2
    double approxSegFlow = expectedMdot
                         - expectedBulkCo2AdsorbNdot * 44.0095
                         + expectedBulkO2DesorbNdot * 31.9988;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tNodes[1].getInflow()->getTraceCompounds()->getMoleFraction(ChemicalCompound::CO2), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(approxSegFlow, tNodes[1].getInflux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tNodes[0].getInflux(), 0.0);

    /// @test source vector for forward flow.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                        tArticle->mSourceVector[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedBulkO2DesorbNdot - expectedBulkCo2AdsorbNdot,
                                                             tArticle->mSourceVector[1], DBL_EPSILON);

    /// @test outflow from the source node for forward flow.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot, tNodes[0].getOutflux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tNodes[1].getOutflux(), 0.0);

    /// @test output loading arrays.
    double expectedCo2Rate = 0.0;
    double expectedH2oRate = 0.0;
    double expectedCo2Mass = 0.0;
    double expectedH2oMass = 0.0;
    for (unsigned int i=0; i<2; ++i) {
            expectedH2oMass += tArticle->mSegments[i].mSorbates[0].mLoadedMass;
            expectedH2oRate += tArticle->mSegments[i].mSorbates[0].mAdsorptionRate * 18.0153;
            expectedCo2Mass += tArticle->mSegments[i].mSorbates[1].mLoadedMass;
            expectedCo2Rate += tArticle->mSegments[i].mSorbates[1].mAdsorptionRate * 44.0095;
    }
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tArticle->getAdsorbedFluidMass(FluidProperties::GUNNS_N2),    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tArticle->getAdsorbedFluidMass(FluidProperties::GUNNS_O2),    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCo2Mass,  tArticle->getAdsorbedFluidMass(FluidProperties::GUNNS_CO2),   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH2oMass,  tArticle->getAdsorbedTcMass(ChemicalCompound::H2O),           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tArticle->getAdsorbedTcMass(ChemicalCompound::CO2),           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tArticle->getAdsorptionFluidRate(FluidProperties::GUNNS_N2),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tArticle->getAdsorptionFluidRate(FluidProperties::GUNNS_O2),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCo2Rate,  tArticle->getAdsorptionFluidRate(FluidProperties::GUNNS_CO2), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH2oRate,  tArticle->getAdsorptionTcRate(ChemicalCompound::H2O),         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tArticle->getAdsorptionTcRate(ChemicalCompound::CO2),         DBL_EPSILON);

    /// - @test zero bulk inflow rate.  1st segment desorbs H2O, creating flow to port 1 node.
    tNodes[0].setPotential(Pin);
    tNodes[1].setPotential(Pin);
    tNodes[0].resetFlows();
    tNodes[1].resetFlows();
    tArticle->mPotentialVector[0] = Pin;
    tArticle->mPotentialVector[1] = Pin;

    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT(0.0 == tArticle->mAdmittanceMatrix[0]);
    tArticle->computeFlows(tTimeStep);
    tArticle->transportFlows(tTimeStep);

    expectedDP   = 0.0;
    expectedFlux = 0.0;
    expectedMdot = 0.0;
    expectedQ    = 0.0;
    expectedPwr  = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDP,   tArticle->mPotentialDrop, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mFlux,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot, tArticle->mFlowRate,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedQ,    tArticle->mVolFlowRate,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPwr,  tArticle->mPower,         DBL_EPSILON);

    CPPUNIT_ASSERT(0.0 == tArticle->mSegments[0].mSorbates[0].mAdsorptionRate);
    CPPUNIT_ASSERT(0.0 == tArticle->mSegments[0].mSorbates[1].mAdsorptionRate);
    CPPUNIT_ASSERT(0.0 == tArticle->mSegments[0].mHeatFlux);
    CPPUNIT_ASSERT(0.0 >  tArticle->mSegments[1].mSorbates[0].mAdsorptionRate);
    CPPUNIT_ASSERT(0.0 >  tArticle->mSegments[1].mSorbates[1].mAdsorptionRate);
    CPPUNIT_ASSERT(0.0 >  tArticle->mSegments[1].mHeatFlux);

    CPPUNIT_ASSERT(0.0 <  tNodes[1].getInflux());
    CPPUNIT_ASSERT(0.0 == tNodes[0].getInflux());
    CPPUNIT_ASSERT(0.0 == tNodes[1].getOutflux());
    CPPUNIT_ASSERT(0.0 == tNodes[0].getOutflux());

    expectedCo2Rate = 0.0;
    expectedH2oRate = 0.0;
    expectedCo2Mass = 0.0;
    expectedH2oMass = 0.0;
    for (unsigned int i=0; i<2; ++i) {
            expectedH2oMass += tArticle->mSegments[i].mSorbates[0].mLoadedMass;
            expectedH2oRate += tArticle->mSegments[i].mSorbates[0].mAdsorptionRate * 18.0153;
            expectedCo2Mass += tArticle->mSegments[i].mSorbates[1].mLoadedMass;
            expectedCo2Rate += tArticle->mSegments[i].mSorbates[1].mAdsorptionRate * 44.0095;
    }
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tArticle->getAdsorbedFluidMass(FluidProperties::GUNNS_N2),    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tArticle->getAdsorbedFluidMass(FluidProperties::GUNNS_O2),    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCo2Mass,  tArticle->getAdsorbedFluidMass(FluidProperties::GUNNS_CO2),   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH2oMass,  tArticle->getAdsorbedTcMass(ChemicalCompound::H2O),           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tArticle->getAdsorbedTcMass(ChemicalCompound::CO2),           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tArticle->getAdsorptionFluidRate(FluidProperties::GUNNS_N2),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tArticle->getAdsorptionFluidRate(FluidProperties::GUNNS_O2),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCo2Rate,  tArticle->getAdsorptionFluidRate(FluidProperties::GUNNS_CO2), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH2oRate,  tArticle->getAdsorptionTcRate(ChemicalCompound::H2O),         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tArticle->getAdsorptionTcRate(ChemicalCompound::CO2),         DBL_EPSILON);

    /// @test reverse flow bulk properties.
    tNodes[0].setPotential(Pout);
    tNodes[1].setPotential(Pin);
    tNodes[0].resetFlows();
    tNodes[1].resetFlows();
    tArticle->mPotentialVector[0] = Pout;
    tArticle->mPotentialVector[1] = Pin;

    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT(0.0 < tArticle->mAdmittanceMatrix[0]);
    tArticle->computeFlows(tTimeStep);
    tArticle->transportFlows(tTimeStep);

    expectedDP   = Pout - Pin;
    expectedFlux = tArticle->mAdmittanceMatrix[0] * expectedDP;
    expectedMdot = expectedFlux * tNodes[1].getOutflow()->getMWeight();
    expectedQ    = expectedMdot / tNodes[1].getOutflow()->getDensity();
    expectedPwr  = -expectedQ * expectedDP * 1000.0; // kPa to Pa

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDP,   tArticle->mPotentialDrop, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mFlux,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot, tArticle->mFlowRate,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedQ,    tArticle->mVolFlowRate,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPwr,  tArticle->mPower,         DBL_EPSILON);

    /// @test chaining of outputs from previous segment to inputs of next segment, reverse flow.
    CPPUNIT_ASSERT(fabs(tArticle->mSegments[1].mHeatFlux)                    > fabs(tArticle->mSegments[0].mHeatFlux));
    CPPUNIT_ASSERT(fabs(tArticle->mSegments[1].mSorbates[0].mAdsorptionRate) > fabs(tArticle->mSegments[0].mSorbates[0].mAdsorptionRate));
    CPPUNIT_ASSERT(fabs(tArticle->mSegments[1].mSorbates[1].mAdsorptionRate) > fabs(tArticle->mSegments[0].mSorbates[1].mAdsorptionRate));

    /// @test sorption outputs to node for reverse flow.
    expectedTcCo2AdsorbNdot   = -expectedFlux * tNodes[1].getContent()->getTraceCompounds()->getMoleFraction(ChemicalCompound::CO2);
    expectedBulkCo2AdsorbNdot = tArticle->mSegments[0].mSorbates[1].mAdsorptionRate
                              + tArticle->mSegments[1].mSorbates[1].mAdsorptionRate
                              - expectedTcCo2AdsorbNdot;
    expectedTcH2oAdsorbNdot   = tArticle->mSegments[0].mSorbates[0].mAdsorptionRate
                              + tArticle->mSegments[1].mSorbates[0].mAdsorptionRate;
    expectedBulkO2DesorbNdot  = tArticle->mSegments[1].mSorbates[1].mAdsorptionRate * 5.0e-5; // from setUp, co2OffgasO2
    approxSegFlow = -expectedMdot
                  - expectedBulkCo2AdsorbNdot * 44.0095
                  + expectedBulkO2DesorbNdot * 31.9988;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tNodes[0].getInflow()->getTraceCompounds()->getMoleFraction(ChemicalCompound::CO2), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(approxSegFlow, tNodes[0].getInflux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tNodes[1].getInflux(), 0.0);

    /// @test source vector for forward flow.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                        tArticle->mSourceVector[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedBulkO2DesorbNdot - expectedBulkCo2AdsorbNdot,
                                                             tArticle->mSourceVector[0], DBL_EPSILON);

    /// @test outflow from the source node for forward flow.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedMdot, tNodes[1].getOutflux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tNodes[0].getOutflux(), 0.0);

    /// @test output loading arrays.
    expectedCo2Rate = 0.0;
    expectedH2oRate = 0.0;
    expectedCo2Mass = 0.0;
    expectedH2oMass = 0.0;
    for (unsigned int i=0; i<2; ++i) {
            expectedH2oMass += tArticle->mSegments[i].mSorbates[0].mLoadedMass;
            expectedH2oRate += tArticle->mSegments[i].mSorbates[0].mAdsorptionRate * 18.0153;
            expectedCo2Mass += tArticle->mSegments[i].mSorbates[1].mLoadedMass;
            expectedCo2Rate += tArticle->mSegments[i].mSorbates[1].mAdsorptionRate * 44.0095;
    }
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tArticle->getAdsorbedFluidMass(FluidProperties::GUNNS_N2),    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tArticle->getAdsorbedFluidMass(FluidProperties::GUNNS_O2),    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCo2Mass,  tArticle->getAdsorbedFluidMass(FluidProperties::GUNNS_CO2),   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH2oMass,  tArticle->getAdsorbedTcMass(ChemicalCompound::H2O),           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tArticle->getAdsorbedTcMass(ChemicalCompound::CO2),           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tArticle->getAdsorptionFluidRate(FluidProperties::GUNNS_N2),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tArticle->getAdsorptionFluidRate(FluidProperties::GUNNS_O2),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCo2Rate,  tArticle->getAdsorptionFluidRate(FluidProperties::GUNNS_CO2), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH2oRate,  tArticle->getAdsorptionTcRate(ChemicalCompound::H2O),         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tArticle->getAdsorptionTcRate(ChemicalCompound::CO2),         DBL_EPSILON);

    /// - Verify volumetric flow rate when the source density is zero
    tArticle->mPotentialVector[0] = -0.6;
    tNodes[1].resetContentState();
    tNodes[1].resetFlows();
    tArticle->computeFlows(tTimeStep);
    tArticle->transportFlows(tTimeStep);
    CPPUNIT_ASSERT(0.0 == tArticle->mVolFlowRate);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for checkSpecificPortRules method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSorptionBed::testPortRules()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test link can map both ports to Ground node.
    tArticle->setPort(0, 3);
    CPPUNIT_ASSERT(3 == tArticle->getNodeMap()[0]);
    tArticle->setPort(1, 3);
    CPPUNIT_ASSERT(3 == tArticle->getNodeMap()[1]);

    /// @test link doesn't allow mapping to a liquid node.
    tArticle->setPort(0, 2);
    CPPUNIT_ASSERT(3 == tArticle->getNodeMap()[0]);
    tArticle->setPort(1, 2);
    CPPUNIT_ASSERT(3 == tArticle->getNodeMap()[1]);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for various exceptions during runtime.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSorptionBed::testRunExceptions()
{
    UT_RESULT;

    /// - Set up network node list with nodes having no trace compounds.
    tFluidConfig->mTraceCompounds = 0;
    tFluidInput1->mTraceCompounds = 0;

    GunnsFluidNode nodes[2];
    nodes[0].initialize("node0", tFluidConfig);
    nodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput1);
    nodes[0].resetFlows();
    nodes[1].initialize("node0", tFluidConfig);
    nodes[1].getContent()->initialize(*tFluidConfig, *tFluidInput1);
    nodes[1].resetFlows();

    tNodeList.mNumNodes = 2;
    tNodeList.mNodes    = nodes;

    /// - Initialize the test article with nominal data and nodes that have no trace compounds.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1));

    /// @test Throw error when getAdsorbedTcMass called and network has no trace compounds.
    CPPUNIT_ASSERT_THROW(tArticle->getAdsorbedTcMass(ChemicalCompound::CO2), TsOutOfBoundsException);

    /// @test Throw error when getAdsorptionTcRate called and network has no trace compounds.
    CPPUNIT_ASSERT_THROW(tArticle->getAdsorptionTcRate(ChemicalCompound::CO2), TsOutOfBoundsException);

    UT_PASS_LAST;
}
