/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    (
        (aspects/thermal/network/ThermalNetwork.o)
    )
***************************************************************************************************/
#include "UtThermalNetwork.hh"
#include <iostream>
#include "math/UnitConversion.hh"
#include <cmath> //needed for fabs()
#include "software/exceptions/TsParseException.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "simulation/hs/TsHsUtMacro.hh" // needed for TEST_HS macro
#include "core/network/GunnsBasicSuperNetwork.hh"
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtThermalNetwork class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtThermalNetwork::UtThermalNetwork()
    :
    tArticle(0),
    tName(),
    parserFriendly(),
    tNodeFile(),
    tCondFile(),
    tRadFile(),
    tHtrFile(),
    tPanFile(),
    tEtcFile(),
    tThermInputFile(),
    tNetworkConfig(tName, 1.0, 1.0, 10),
    tNodes(0),
    tNumberNodes(0),
    tCapacitanceLinks(0),
    tConductionLinks(0),
    tRadiationLinks(0),
    tHeaters(0),
    tPanels(0),
    tPotentialLinks(0),
    tSources(0),
    tCapacitanceConfigData(0),
    tConductionConfigData(0),
    tRadiationConfigData(0),
    tHeaterConfigData(0),
    tPanelConfigData(0),
    tPotentialConfigData(0),
    tSourceConfigData(0),
    tCapacitanceInputData(0),
    tConductionInputData(0),
    tRadiationInputData(0),
    tHeaterInputData(0),
    tPanelInputData(0),
    tPotentialInputData(0),
    tSourceInputData(0),
    tTimeStep(0),
    tTolerance(0)
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtThermalNetwork class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtThermalNetwork::~UtThermalNetwork()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermalNetwork::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermalNetwork::setUp()
{
    tName = "Test ThermalNetwork";

    /// - Declare the config-files.
    tNodeFile = "ThermNodes_base.xml";
    tCondFile = "ThermLinksCond_base.xml";
    tRadFile = "ThermLinksRad_base.xml";
    tHtrFile = "HtrRegistry_base.xml";
    tPanFile = "ThermLinksPan_base.xml";
    tEtcFile = "ThermLinksEtc_base.xml";

    /// - Declare the thermal override input-file.
    tThermInputFile = ".xml";

    /// - Declare the nominal test data
    tTolerance = 1.0e-8;
    tTimeStep = 30.0;

    /// - Instantiate a test article.
    tArticle = new FriendlyThermalNetwork(tName);

    /// - Simulate default_data job.
    tArticle->mConfig.cNodeFile = tNodeFile;
    tArticle->mConfig.cCondFile = tCondFile;
    tArticle->mConfig.cRadFile = tRadFile;
    tArticle->mConfig.cHtrFile = tHtrFile;
    tArticle->mConfig.cPanFile = tPanFile;
    tArticle->mConfig.cEtcFile = tEtcFile;

    /// - Initialize the test article.
    tArticle->initialize();

    /// - Load the Friendly parser with the same files as the test article, so we can
    ///   compare the two (we only have access to parserFriendly).
    parserFriendly.mNodeFile = tNodeFile;
    parserFriendly.mCondFile = tCondFile;
    parserFriendly.mRadFile = tRadFile;
    parserFriendly.mHtrFile = tHtrFile;
    parserFriendly.mPanFile = tPanFile;
    parserFriendly.mEtcFile = tEtcFile;
    parserFriendly.initialize(tName);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for appropriate configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermalNetwork::testConfig()
{
    std::cout << "\n ----------------------------------------------------------------------------";
    const char* test = "ThermalNetwork 01: Testing config ...................................";
    std::cout << "\n " << test;
    TEST_HS(test);

    /// - Nominally construct config.
    ThermalNetworkConfigData config(tNodeFile,tCondFile,tRadFile,tHtrFile,tPanFile);
    /// @test   Default config data.
    CPPUNIT_ASSERT_MESSAGE("cNodeFile", tNodeFile == config.cNodeFile);
    CPPUNIT_ASSERT_MESSAGE("cCondFile", tCondFile == config.cCondFile);
    CPPUNIT_ASSERT_MESSAGE( "cRadFile", tRadFile == config.cRadFile);
    CPPUNIT_ASSERT_MESSAGE( "cHtrFile", tHtrFile == config.cHtrFile);
    CPPUNIT_ASSERT_MESSAGE( "cPanFile", tPanFile == config.cPanFile);
    CPPUNIT_ASSERT_MESSAGE( "cEtcFile", "" == config.cEtcFile);

    /// - Default construct config.
    ThermalNetworkConfigData configDefault;
    /// @test   Default config data.
    CPPUNIT_ASSERT_MESSAGE("cNodeFile", "" == configDefault.cNodeFile );
    CPPUNIT_ASSERT_MESSAGE("cCondFile", "" == configDefault.cCondFile );
    CPPUNIT_ASSERT_MESSAGE( "cRadFile", "" == configDefault.cRadFile );
    CPPUNIT_ASSERT_MESSAGE( "cHtrFile", "" == configDefault.cHtrFile );
    CPPUNIT_ASSERT_MESSAGE( "cPanFile", "" == configDefault.cPanFile );
    CPPUNIT_ASSERT_MESSAGE( "cEtcFile", "" == configDefault.cEtcFile );

    /// - Copy construct config.
    ThermalNetworkConfigData configCopy(config);
    /// @test   Copied config data.
    CPPUNIT_ASSERT_MESSAGE("cNodeFile", tNodeFile == config.cNodeFile);
    CPPUNIT_ASSERT_MESSAGE("cCondFile", tCondFile == config.cCondFile);
    CPPUNIT_ASSERT_MESSAGE( "cRadFile", tRadFile == config.cRadFile);
    CPPUNIT_ASSERT_MESSAGE( "cHtrFile", tHtrFile == config.cHtrFile);
    CPPUNIT_ASSERT_MESSAGE( "cPanFile", tPanFile == config.cPanFile);
    CPPUNIT_ASSERT_MESSAGE( "cEtcFile", "" == config.cEtcFile);

    std::cout << " Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for appropriate input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermalNetwork::testInput()
{
    const char* test = "ThermalNetwork 02: Testing input ....................................";
    std::cout << "\n " << test;
    TEST_HS(test);

    /// - Nominally construct input.
    ThermalNetworkInputData input(tThermInputFile);
    /// @test   Default input data.
    CPPUNIT_ASSERT_MESSAGE("tThermInputFile", tThermInputFile == input.iThermInputFile);

    /// - Default construct input.
    ThermalNetworkInputData inputDefault;
    /// @test   Default input data.
    CPPUNIT_ASSERT_MESSAGE("tThermInputFile", "" == inputDefault.iThermInputFile );

    /// - Copy construct input.
    ThermalNetworkInputData inputCopy(input);
    /// @test   Copied input data.
    CPPUNIT_ASSERT_MESSAGE("tThermInputFile", tThermInputFile == inputCopy.iThermInputFile);

    std::cout << " Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for appropriate construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermalNetwork::testNominalConstruction()
{
    const char* test = "ThermalNetwork 03: Test for Nominal Construction .................";
    std::cout << "\n " << test;
    TEST_HS(test);

    /// - default construct an article
    FriendlyThermalNetwork article(tName);

    /// @test  Default-constructed article's pointers are successfully initialized to zero
    CPPUNIT_ASSERT(tName == article.mName);
    CPPUNIT_ASSERT(0 == article.mHtrPowerElectrical);
    CPPUNIT_ASSERT(false == article.mMalfHtrMiswireFlag);
    CPPUNIT_ASSERT(0 == article.mMalfHtrIndexValue);
    CPPUNIT_ASSERT(0 == article.numCapEditGroups);
    CPPUNIT_ASSERT(0 == article.mCapEditScaleFactor);
    CPPUNIT_ASSERT(0 == article.mCapEditScalePrev);
    CPPUNIT_ASSERT(0 == article.pNodes);
    CPPUNIT_ASSERT(0 == article.indexSpaceNode);
    CPPUNIT_ASSERT(0 == article.mCapacitanceLinks);
    CPPUNIT_ASSERT(0 == article.mPotentialLinks);
    CPPUNIT_ASSERT(0 == article.mConductionLinks);
    CPPUNIT_ASSERT(0 == article.mRadiationLinks);
    CPPUNIT_ASSERT(0 == article.mSources);
    CPPUNIT_ASSERT(0 == article.mHeaters);
    CPPUNIT_ASSERT(0 == article.mPanels);
    CPPUNIT_ASSERT(0 == article.mCapacitanceConfigData);
    CPPUNIT_ASSERT(0 == article.mPotentialConfigData);
    CPPUNIT_ASSERT(0 == article.mConductionConfigData);
    CPPUNIT_ASSERT(0 == article.mRadiationConfigData);
    CPPUNIT_ASSERT(0 == article.mSourceConfigData);
    CPPUNIT_ASSERT(0 == article.mHeaterConfigData);
    CPPUNIT_ASSERT(0 == article.mPanelConfigData);
    CPPUNIT_ASSERT(0 == article.mCapacitanceInputData);
    CPPUNIT_ASSERT(0 == article.mPotentialInputData);
    CPPUNIT_ASSERT(0 == article.mConductionInputData);
    CPPUNIT_ASSERT(0 == article.mRadiationInputData);
    CPPUNIT_ASSERT(0 == article.mSourceInputData);
    CPPUNIT_ASSERT(0 == article.mHeaterInputData);
    CPPUNIT_ASSERT(0 == article.mPanelInputData);
    CPPUNIT_ASSERT(0 == article.netNumLocalNodes);
    CPPUNIT_ASSERT(0 == article.numLinksCap);
    CPPUNIT_ASSERT(0 == article.numLinksPot);
    CPPUNIT_ASSERT(0 == article.numLinksCond);
    CPPUNIT_ASSERT(0 == article.numLinksRad);
    CPPUNIT_ASSERT(0 == article.numLinksSrc);
    CPPUNIT_ASSERT(0 == article.numLinksHtr);
    CPPUNIT_ASSERT(0 == article.numLinksPan);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for appropriate initialization of ThermalNetwork. Since initialize() takes up to
///           three config-files as arguments, there are several different combinations to test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermalNetwork::testInitialize()
{
    const char* test = "ThermalNetwork 04: Test Initialization routine ...................";
    std::cout << "\n " << test;
    TEST_HS(test);

    /// @test  The nominal test article has the correct link counts.
    CPPUNIT_ASSERT_EQUAL(parserFriendly.numLinksCap,  tArticle->numLinksCap);
    CPPUNIT_ASSERT_EQUAL(parserFriendly.numLinksPot,  tArticle->numLinksPot);
    CPPUNIT_ASSERT_EQUAL(parserFriendly.numLinksCond, tArticle->numLinksCond);
    CPPUNIT_ASSERT_EQUAL(parserFriendly.numLinksRad,  tArticle->numLinksRad);
    CPPUNIT_ASSERT_EQUAL(parserFriendly.numLinksSrc,  tArticle->numLinksSrc);
    CPPUNIT_ASSERT_EQUAL(parserFriendly.numLinksHtr,  tArticle->numLinksHtr);
    CPPUNIT_ASSERT_EQUAL(parserFriendly.numLinksPan,  tArticle->numLinksPan);
    CPPUNIT_ASSERT(parserFriendly.vCapEditGroupList.size() == tArticle->numCapEditGroups);

    /// @test  Initialization of capacitance edit group controls
    for(int i = 0; i < tArticle->numCapEditGroups; ++i)
    {
        /// @test Capacitance edit scale factor controls by edit group.
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE( "Capacitance edit factor, group: " + parserFriendly.vCapEditGroupList.at(i),
           1.0, tArticle->mCapEditScaleFactor[i], tTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE( "Capacitance edit previous, group: " + parserFriendly.vCapEditGroupList.at(i),
           1.0, tArticle->mCapEditScalePrev[i], tTolerance);
    }

    /// @test  Initialization of island analyzer spotter.
    CPPUNIT_ASSERT_EQUAL(-1, tArticle->netIslandAnalyzer.getAttachedNode());

    /// @test  Initialization of GUNNS solver.
    FriendlyGunnsSolver* solver = static_cast<FriendlyGunnsSolver*>(&tArticle->netSolver);
    CPPUNIT_ASSERT_EQUAL(1, solver->mDecompositionLimit);

    /// @test  Repeat initialization doesn't cause an explosion.
    CPPUNIT_ASSERT_NO_THROW_MESSAGE("repeat initialize", tArticle->initialize() );

    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// - Default construct a test article
    FriendlyThermalNetwork article0("article0");

    /// - Call initialize() with zero config-files.
    CPPUNIT_ASSERT_NO_THROW_MESSAGE("initialize(), no config-files", article0.initialize() );

    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// - Default construct a test article
    FriendlyThermalNetwork article1("article1");

    /// - Simulate default_data job, only setting some files.
    article1.mConfig.cNodeFile = tNodeFile;
    article1.mConfig.cCondFile = tCondFile;
    article1.mConfig.cRadFile = tRadFile;

    /// - Call initialize().
    CPPUNIT_ASSERT_NO_THROW_MESSAGE("nominal initialize()", article1.initialize() );

    /// @test  The internal-link only test article should have the correct link counts.
    CPPUNIT_ASSERT_EQUAL(parserFriendly.numLinksCap,  article1.numLinksCap);
    CPPUNIT_ASSERT_EQUAL(0,  article1.numLinksPot);
    CPPUNIT_ASSERT_EQUAL(parserFriendly.numLinksCond, article1.numLinksCond);
    CPPUNIT_ASSERT_EQUAL(parserFriendly.numLinksRad,  article1.numLinksRad);
    CPPUNIT_ASSERT_EQUAL(0, article1.numLinksSrc);
    CPPUNIT_ASSERT_EQUAL(0, article1.numLinksHtr);
    CPPUNIT_ASSERT_EQUAL(0, article1.numLinksPan);

    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// - Default construct a test article
    FriendlyThermalNetwork article2("article2");

    /// - Simulate default_data job, only setting some files.
    article2.mConfig.cNodeFile = tNodeFile;
    article2.mConfig.cCondFile = tCondFile;
    article2.mConfig.cRadFile = tRadFile;
    article2.mConfig.cHtrFile = tHtrFile;
    article2.mConfig.cEtcFile = tEtcFile;
    article2.numCapEditGroups = 2;

    /// - Call initialize().
    CPPUNIT_ASSERT_NO_THROW_MESSAGE("nominal initialize()", article2.initialize() );

    /// @test  The internal/htr test article should have the correct link counts.
    CPPUNIT_ASSERT_EQUAL(parserFriendly.numLinksCap,  article2.numLinksCap);
    CPPUNIT_ASSERT_EQUAL(parserFriendly.numLinksCond, article2.numLinksCond);
    CPPUNIT_ASSERT_EQUAL(parserFriendly.numLinksRad,  article2.numLinksRad);
    CPPUNIT_ASSERT_EQUAL(parserFriendly.numLinksHtr,  article2.numLinksHtr);
    CPPUNIT_ASSERT_EQUAL(0, article2.numLinksPan);
    CPPUNIT_ASSERT_EQUAL(parserFriendly.numLinksPot,  article2.numLinksPot);
    CPPUNIT_ASSERT_EQUAL(parserFriendly.numLinksSrc,  article2.numLinksSrc);

    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// - Default construct another test article
    FriendlyThermalNetwork article3("article3");

    /// - Simulate default_data job, only setting some files.
    article3.mConfig.cNodeFile = tNodeFile;
    article3.mConfig.cCondFile = tCondFile;
    article3.mConfig.cRadFile = tRadFile;
    article3.mConfig.cPanFile = tPanFile;

    /// - Call initialize().
    CPPUNIT_ASSERT_NO_THROW_MESSAGE("nominal initialize()", article3.initialize() );

    /// @test  The internal/pan test article should have the correct link counts.
    CPPUNIT_ASSERT_EQUAL(parserFriendly.numLinksCap, article3.numLinksCap);
    CPPUNIT_ASSERT_EQUAL(parserFriendly.numLinksCond,  article3.numLinksCond);
    CPPUNIT_ASSERT_EQUAL(parserFriendly.numLinksRad,   article3.numLinksRad);
    CPPUNIT_ASSERT_EQUAL(0, article3.numLinksHtr);
    CPPUNIT_ASSERT_EQUAL(parserFriendly.numLinksPan, article3.numLinksPan);
    CPPUNIT_ASSERT_EQUAL(0, article3.numLinksPot);
    CPPUNIT_ASSERT_EQUAL(0, article3.numLinksSrc);

    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// - Default construct another test article
    FriendlyThermalNetwork article4("article4");

    /// - Initialize with a node-file that contains temperatures below absolute zero.
    article4.mConfig.cNodeFile = "ThermNodes_nonnumeric.xml";
    /// - Make sure the TsInitializationException is caught.
    CPPUNIT_ASSERT_NO_THROW( article4.initialize() );

    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// - Default construct another test article
    FriendlyThermalNetwork article5;

    /// - Initialize with a node-file that does not define a <capEditing> groups.
    article5.mConfig.cNodeFile = "ThermNodes_noCapEditing.xml";
    /// - Make sure nothing blows up.
    CPPUNIT_ASSERT_NO_THROW( article5.initialize("article5") );
    CPPUNIT_ASSERT("article5" == article5.mName);
    CPPUNIT_ASSERT_NO_THROW( article5.update(tTimeStep) );

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for appropriate building of link configuration data. The test makes sure that
///           the config data matches up with the data vectors in the parser object.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermalNetwork::testConfigBuild()
{
    const char* test = "ThermalNetwork 05: Test for correct Config Build .................";
    std::cout << "\n " << test;
    TEST_HS(test);

    /// - Declare a stringstream to store the test's index number, so a failure can be traced to a
    ///   specific index number with each for loop.
    std::stringstream out;

    for(int i = 0; i < tArticle->numLinksCap; ++i)
    {
        /// - Store the index into the stringstream.
        out.str("");
        out << i;

        /// @test Capacitance links: edit-group identifier
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Cap mEditCapacitanceGroup, index: " + out.str(),
            parserFriendly.vCapEditGroupIdentifiers.at(i),
            tArticle->mCapacitanceConfigData[i]->mEditCapacitanceGroup);
    }

    for(int i = 0; i < tArticle->numLinksCond; ++i)
    {
        /// - Store the index into the stringstream.
        out.str("");
        out << i;

        /// @test Conduction links: conduction coefficient
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE( "Conduction coefficient, index: " + out.str(),
            parserFriendly.vCondConductivities.at(i),
            tArticle->mConductionConfigData[i]->mDefaultConductivity,
            tTolerance);

        /// @test Conduction links: conduction coefficient
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE( "Conduction coefficient, index: " + out.str(),
            parserFriendly.vCondConductivities.at(i),
            tArticle->mConductionConfigData[i]->mDefaultConductivity,
            tTolerance);
    }

    for(int i = 0; i < tArticle->numLinksRad; ++i)
    {
        /// - Store the index into the stringstream.
        out.str("");
        out << i;

        /// @test Radiation links: radiation coefficient
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE( "Radiation coefficient, index: " + out.str(),
           parserFriendly.vRadCoefficients.at(i),
           tArticle->mRadiationConfigData[i]->mDefaultConductivity / UnitConversion::STEFAN_BOLTZMANN_CONST_SI,
           tTolerance);
    }

    for(int i = 0; i < tArticle->numLinksSrc; ++i)
    {
        /// - Store the index into the stringstream.
        out.str("");
        out << i;

        /// - The flux-distribution-fraction vector is a different size for each source, since sources
        ///   have a variable number of ports.
        for(int ii = 0; ii < tArticle->mSourceConfigData[i]->cFluxDistributionFractions.size(); ++ii)
        {
            /// @test Sources: power-draw fractions
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE( "Source efficiency, index: " + out.str(),
               parserFriendly.vSrcFracs.at(i).at(ii),
               tArticle->mSourceConfigData[i]->cFluxDistributionFractions.at(ii),
               tTolerance);
        }
    }

    for(int i = 0; i < tArticle->numLinksHtr; ++i)
    {
        /// - Store the index into the stringstream.
        out.str("");
        out << i;

        /// @test Heaters: efficiency
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE( "Heater tuning scalar, index: " + out.str(),
           parserFriendly.vHtrScalars.at(i),
           tArticle->mHeaterConfigData[i]->cTuningScalar,
           tTolerance);

        /// - The flux-distribution-fraction vector is a different size for each heater, since heaters
        ///   have a variable number of ports.
        for(int ii = 0; ii < tArticle->mHeaterConfigData[i]->cFluxDistributionFractions.size(); ++ii)
        {
            /// @test Heaters: power-draw fractions
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE( "Heater efficiency, index: " + out.str(),
               parserFriendly.vHtrFracs.at(i).at(ii),
               tArticle->mHeaterConfigData[i]->cFluxDistributionFractions.at(ii),
               tTolerance);
        }
    }

    for(int i = 0; i < tArticle->numLinksPan; ++i)
    {
        /// - Store the index into the stringstream.
        out.str("");
        out << i;

        /// @test Panels: absorptivity
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE( "Panel absorptivity, index: " + out.str(),
           parserFriendly.vPanAbsorptivities.at(i),
           tArticle->mPanelConfigData[i]->cAbsorptivity,
           tTolerance);

        /// @test Panels: surface area
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE( "Panel surface area, index: " + out.str(),
           parserFriendly.vPanAreas.at(i),
           tArticle->mPanelConfigData[i]->cSurfaceArea,
           tTolerance);

        /// - The flux-distribution-fraction vector is a different size for each panel, since panels
        ///   have a variable number of ports.
        for(int ii = 0; ii < tArticle->mPanelConfigData[i]->cFluxDistributionFractions.size(); ++ii)
        {
            /// @test Panels: power-draw fractions
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE( "Panel efficiency, index: " + out.str(),
               parserFriendly.vPanFracs.at(i).at(ii),
               tArticle->mPanelConfigData[i]->cFluxDistributionFractions.at(ii),
               tTolerance);
        }
    }
    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for appropriate building of link configuration data. The test makes sure that
///           the link input data matches up with the default input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermalNetwork::testInputBuild()
{
    const char* test = "ThermalNetwork 06: Test for correct Input Build ..................";
    std::cout << "\n " << test;
    TEST_HS(test);

    /// - Declare a stringstream to store the test's index number, so a failure can be traced to a
    ///   specific index number with each for loop.
    std::stringstream out;

    for(int i = 0; i < tArticle->numLinksCap; ++i)
    {
        /// - Store the index into the stringstream.
        out.str("");
        out << i;

        /// @test Capacitance links: capacitance
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE( "Cap mCapacitance, index: " + out.str(),
            parserFriendly.vCapCapacitances.at(i),
            tArticle->mCapacitanceInputData[i]->mCapacitance,
            tTolerance);

        /// @test Capacitance links: initial temperature
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE( "Cap mPotential, index: " + out.str(),
            parserFriendly.vCapTemperatures.at(i),
            tArticle->mCapacitanceInputData[i]->mPotential,
            tTolerance);

        /// @test Capacitance links: mMalfBlockageFlag
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Cap mMalfBlockageFlag, index: " + out.str(),
            false, tArticle->mCapacitanceInputData[i]->mMalfBlockageFlag);

        /// @test Capacitance links: mMalfBlockageValue
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE( "Cap mMalfBlockageValue, index: " + out.str(),
            0.0, tArticle->mCapacitanceInputData[i]->mMalfBlockageValue, tTolerance);
    }

    for(int i = 0; i < tArticle->numLinksPot; ++i)
    {
        /// - Store the index into the stringstream.
        out.str("");
        out << i;

        /// @test Potential links: temperature setting
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE( "Potential temperature, index: " + out.str(),
           parserFriendly.vPotTemperatures.at(i),
           tArticle->mPotentialInputData[i]->mSourcePotential,
           tTolerance);

        /// @test Potential links: mMalfBlockageFlag
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Potential mMalfBlockageFlag, index: " + out.str(),
            false, tArticle->mPotentialInputData[i]->mMalfBlockageFlag);

        /// @test Potential links: mMalfBlockageValue
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE( "Potential mMalfBlockageValue, index: " + out.str(),
            0.0, tArticle->mPotentialInputData[i]->mMalfBlockageValue, tTolerance);
    }

    for(int i = 0; i < tArticle->numLinksCond; ++i)
    {
        /// - Store the index into the stringstream.
        out.str("");
        out << i;

        /// @test Conduction links: mMalfBlockageFlag
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Conduction mMalfBlockageFlag, index: " + out.str(),
            false, tArticle->mConductionInputData[i]->mMalfBlockageFlag);

        /// @test Conduction links: mMalfBlockageValue
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE( "Conduction mMalfBlockageValue, index: " + out.str(),
            0.0, tArticle->mConductionInputData[i]->mMalfBlockageValue, tTolerance);
    }

    for(int i = 0; i < tArticle->numLinksRad; ++i)
    {
        /// - Store the index into the stringstream.
        out.str("");
        out << i;

        /// @test Radiation links: mMalfBlockageFlag
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Radiation mMalfBlockageFlag, index: " + out.str(),
            false, tArticle->mRadiationInputData[i]->mMalfBlockageFlag);

        /// @test Conduction links: mMalfBlockageValue
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE( "Radiation mMalfBlockageValue, index: " + out.str(),
            0.0, tArticle->mRadiationInputData[i]->mMalfBlockageValue, tTolerance);
    }

    for(int i = 0; i < tArticle->numLinksSrc; ++i)
    {
        /// - Store the index into the stringstream.
        out.str("");
        out << i;

        /// @test Sources: flux
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE( "Source flux, index: " + out.str(),
           parserFriendly.vSrcInits.at(i),
           tArticle->mSourceInputData[i]->iDemandedFlux,
           tTolerance);

        /// @test Source links: mMalfBlockageFlag
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Source mMalfBlockageFlag, index: " + out.str(),
            false, tArticle->mSourceInputData[i]->mMalfBlockageFlag);

        /// @test Source links: mMalfBlockageValue
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE( "Source mMalfBlockageValue, index: " + out.str(),
            0.0, tArticle->mSourceInputData[i]->mMalfBlockageValue, tTolerance);

        /// @test Source links: iMalfFluxOverrideFlag
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Source mMalfBlockageFlag, index: " + out.str(),
            false, tArticle->mSourceInputData[i]->iMalfFluxOverrideFlag);

        /// @test Source links: iMalfFluxOverrideValue
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE( "Source mMalfBlockageValue, index: " + out.str(),
            0.0, tArticle->mSourceInputData[i]->iMalfFluxOverrideValue, tTolerance);
    }

    for(int i = 0; i < tArticle->numLinksHtr; ++i)
    {
        /// - Store the index into the stringstream.
        out.str("");
        out << i;

        /// @test Heater links: mMalfBlockageFlag
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Heater mMalfBlockageFlag, index: " + out.str(),
            false, tArticle->mHeaterInputData[i]->mMalfBlockageFlag);

        /// @test Heater links: mMalfBlockageValue
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE( "Heater mMalfBlockageValue, index: " + out.str(),
            0.0, tArticle->mHeaterInputData[i]->mMalfBlockageValue, tTolerance);

        /// @test Heater links: iMalfFluxOverrideFlag
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Heater mMalfBlockageFlag, index: " + out.str(),
            false, tArticle->mHeaterInputData[i]->iMalfFluxOverrideFlag);

        /// @test Heater links: iMalfFluxOverrideValue
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE( "Heater mMalfBlockageValue, index: " + out.str(),
            0.0, tArticle->mHeaterInputData[i]->iMalfFluxOverrideValue, tTolerance);

    }

    for(int i = 0; i < tArticle->numLinksPan; ++i)
    {
        /// - Store the index into the stringstream.
        out.str("");
        out << i;

        /// @test Panels: initial demandedFlux
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE( "Panel initial demandedFlux, index: " + out.str(),
            0.0, tArticle->mPanelInputData[i]->iDemandedFlux, tTolerance);

        /// @test Panel links: mMalfBlockageFlag
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Panel mMalfBlockageFlag, index: " + out.str(),
            false, tArticle->mPanelInputData[i]->mMalfBlockageFlag);

        /// @test Panel links: mMalfBlockageValue
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE( "Panel mMalfBlockageValue, index: " + out.str(),
            0.0, tArticle->mPanelInputData[i]->mMalfBlockageValue, tTolerance);

        /// @test Panel links: iMalfFluxOverrideFlag
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Panel mMalfBlockageFlag, index: " + out.str(),
            false, tArticle->mPanelInputData[i]->iMalfFluxOverrideFlag);

        /// @test Panel links: iMalfFluxOverrideValue
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE( "Panel mMalfBlockageValue, index: " + out.str(),
            0.0, tArticle->mPanelInputData[i]->iMalfFluxOverrideValue, tTolerance);
    }
    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for appropriate link initialization. The test makes sure that the NodeMap
///           for each link matches up with the port numbers described in the config-files.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermalNetwork::testLinksBuild()
{
    const char* test = "ThermalNetwork 07: Test for correct Links Build (node connection) ";
    std::cout << "\n " << test;
    TEST_HS(test);

    /// - Declare a stringstream to store the test's index number, so a failure can be traced to a
    ///   specific index number with each for loop.
    std::stringstream out;

    /// - Capacitance links
    for(int i = 0; i < tArticle->numLinksCap; ++i)
    {
        /// - Store the index into the stringstream.
        out.str("");
        out << i;

        /// @test Capacitance links: port 0
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Capacitance port0, index: " + out.str(),
            parserFriendly.getMapLocation( parserFriendly.vNodeNames.at(i) ),
            tArticle->mCapacitanceLinks[i].getNodeMap()[0]);

        /// @test Capacitance links: port 1
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Capacitance port1, index: " + out.str(),
            tArticle->netNumLocalNodes-1,
            tArticle->mCapacitanceLinks[i].getNodeMap()[1]);
    }
    /// - Potential links
    for(int i = 0; i < tArticle->numLinksPot; ++i)
    {
        /// - Store the index into the stringstream.
        out.str("");
        out << i;

        /// @test Potential links: port 0
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Potential port0, index: " + out.str(),
            tArticle->netNumLocalNodes-1,
            tArticle->mPotentialLinks[i].getNodeMap()[0]);

        /// @test Potential links: port 1
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Potential port1, index: " + out.str(),
            parserFriendly.vPotPorts.at(i),
            tArticle->mPotentialLinks[i].getNodeMap()[1]);
    }
    /// - Conduction links
    for(int i = 0; i < tArticle->numLinksCond; ++i)
    {
        /// - Store the index into the stringstream.
        out.str("");
        out << i;

        /// @test Conduction links: port 0
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Conduction port0, index: " + out.str(),
            parserFriendly.vCondPorts0.at(i),
            tArticle->mConductionLinks[i].getNodeMap()[0]);

        /// @test Conduction links: port 1
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Conduction port1, index: " + out.str(),
            parserFriendly.vCondPorts1.at(i),
            tArticle->mConductionLinks[i].getNodeMap()[1]);
    }
    /// - Radiation links
    for(int i = 0; i < tArticle->numLinksRad; ++i)
    {
        /// - Store the index into the stringstream.
        out.str("");
        out << i;

        /// @test Radiation links: port 0
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Radiation port0, index: " + out.str(),
            parserFriendly.vRadPorts0.at(i),
            tArticle->mRadiationLinks[i].getNodeMap()[0]);

        /// @test Radiation links: port 1
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "failure at Radiation port1 index: " + out.str(),
            parserFriendly.vRadPorts1.at(i),
            tArticle->mRadiationLinks[i].getNodeMap()[1]);
    }
    /// - Source links
    for(int i = 0; i < tArticle->numLinksSrc; ++i)
    {
        /// - Store the index into the stringstream.
        out.str("");
        out << i;

        /// - Sources have a variable number of ports.
        int numports = tArticle->mSources[i].getNumberPorts();
        for(int ii = 0; ii < numports; ++ii)
        {
            /// @test Heaters: port map
            CPPUNIT_ASSERT_EQUAL_MESSAGE( "Source port, index: " + out.str(),
               parserFriendly.vSrcPorts.at(i).at(ii),
               tArticle->mSources[i].getNodeMap()[ii]);
        }
    }
    /// - Heater links
    for(int i = 0; i < tArticle->numLinksHtr; ++i)
    {
        /// - Store the index into the stringstream.
        out.str("");
        out << i;

        /// - Heaters have a variable number of ports.
        int numports = tArticle->mHeaters[i].getNumberPorts();
        for(int ii = 0; ii < numports; ++ii)
        {
            /// @test Heaters: port map
            CPPUNIT_ASSERT_EQUAL_MESSAGE( "Heater port, index: " + out.str(),
               parserFriendly.vHtrPorts.at(i).at(ii),
               tArticle->mHeaters[i].getNodeMap()[ii]);
        }
    }
    /// - ThermalPanel links
    for(int i = 0; i < tArticle->numLinksPan; ++i)
    {
        /// - Store the index into the stringstream.
        out.str("");
        out << i;

        /// - Panels have a variable number of ports.
        int numports = tArticle->mPanels[i].getNumberPorts();
        for(int ii = 0; ii < numports; ++ii)
        {
            /// @test Heaters: port map
            CPPUNIT_ASSERT_EQUAL_MESSAGE( "Heater port, index: " + out.str(),
               parserFriendly.vPanPorts.at(i).at(ii),
               tArticle->mPanels[i].getNodeMap()[ii]);
        }
    }

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for correct handling of null pointers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermalNetwork::testValidate()
{
    const char* test = "ThermalNetwork 08: Test Validation routine .......................";
    std::cout << "\n " << test;
    TEST_HS(test);

    /// @test  Zero for node list number of nodes
    const int numNodes = tArticle->netNodeList.mNumNodes;
    tArticle->netNodeList.mNumNodes = 0;
    CPPUNIT_ASSERT_THROW(tArticle->validate(), TsInitializationException);
    tArticle->netNodeList.mNumNodes = numNodes;

    /// @test  Null pointer to Network's BasicNode object
    tArticle->netNodeList.mNodes = 0;
    CPPUNIT_ASSERT_THROW(tArticle->validate(), TsInitializationException);
    tArticle->netNodeList.mNodes = tArticle->pNodes;

    /// @test  Nonpositive node count
    tNumberNodes = tArticle->netNumLocalNodes;
    tArticle->netNumLocalNodes = 0;
    CPPUNIT_ASSERT_THROW(tArticle->validate(), TsInitializationException);
    tArticle->netNumLocalNodes = tNumberNodes;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for correct handling of null pointers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermalNetwork::testSetHeaterPowers()
{
    const char* test = "ThermalNetwork 09: Test setHeaterPowers routine ..................";
    std::cout << "\n " << test;
    TEST_HS(test);

    /// - To avoid memory errors, make sure enough heaters have been properly constructed.
    if (tArticle->numLinksHtr > 2)
    {
        /// - Set trial power values
        double power0 = 43.5;
        double power1 = 19.0;

        /// - Set mHtrPowerElectrical array, to simulate it be written by the simbus.
        tArticle->mHtrPowerElectrical[0] = power0;
        tArticle->mHtrPowerElectrical[1] = power1;

        /// @test  Initial power values.
        CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mHeaters[0].getPowerElectrical(), 0.0, tTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mHeaters[1].getPowerElectrical(), 0.0, tTolerance);

        /// - Call the setHeaterPowers() method.
        CPPUNIT_ASSERT_NO_THROW_MESSAGE("nominal setHeaterPowers()", tArticle->setHeaterPowers() );

        /// @test  Check updated power values.
        CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mHeaters[0].getPowerElectrical(), power0, tTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mHeaters[1].getPowerElectrical(), power1, tTolerance);

        /// - Trip the miswire malfunction.
        tArticle->mMalfHtrMiswireFlag = true;

        /// - Call the setHeaterPowers() method.
        CPPUNIT_ASSERT_NO_THROW_MESSAGE("malf'd setHeaterPowers()", tArticle->setHeaterPowers() );

        /// @test  Power values should be unchanged, since no indicies were crossed.
        CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mHeaters[0].getPowerElectrical(), power0, tTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mHeaters[1].getPowerElectrical(), power1, tTolerance);

        /// - Cross the indicies in the mMalfHtrIndexValue array.
        tArticle->mMalfHtrIndexValue[0] = 1;
        tArticle->mMalfHtrIndexValue[1] = 0;

        /// - Call the setHeaterPowers() method.
        CPPUNIT_ASSERT_NO_THROW_MESSAGE("malf'd setHeaterPowers()", tArticle->setHeaterPowers() );

        /// @test  Power values should be crossed.
        CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mHeaters[0].getPowerElectrical(), power1, tTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mHeaters[1].getPowerElectrical(), power0, tTolerance);

        /// - Set mHtrPowerElectrical array, to simulate it be written by the simbus.
        tArticle->mMalfHtrIndexValue[0] = -10;
        tArticle->mMalfHtrIndexValue[1] = 265;

        /// - Call the setHeaterPowers() method.
        CPPUNIT_ASSERT_NO_THROW_MESSAGE("malf'd setHeaterPowers()", tArticle->setHeaterPowers() );

        /// @test  Power values should be back to their default.
        CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mHeaters[0].getPowerElectrical(), power0, tTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mHeaters[1].getPowerElectrical(), power1, tTolerance);

    } else
    {
        CPPUNIT_ASSERT_MESSAGE( "To test properly, more heaters need to be created.", false);
    }

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for appropriate updating of ThermalNetwork.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermalNetwork::testUpdate()
{
    const char* test = "ThermalNetwork 10: Test Update function ..........................";
    std::cout << "\n " << test;
    TEST_HS(test);

    /// - Default construct a test article.
    FriendlyThermalNetwork article(tName);

    /// - Check that exception is caught if network not first initialized.
    CPPUNIT_ASSERT_NO_THROW_MESSAGE("update before initialize", article.update(tTimeStep) );

    /// - Set the network to find islands and set the island analyzer to a valid node.
    tArticle->setIslandMode(Gunns::SOLVE);
    tArticle->netIslandAnalyzer.setAttachedNode(0);

    /// - Call the update() method.
    CPPUNIT_ASSERT_NO_THROW_MESSAGE("nominal update()", tArticle->update(tTimeStep) );

    /// - Declare a stringstream to store the test's index number, so a failure can be traced to a
    ///   specific index number with each for loop.
    std::stringstream out;

    /// - Check the temperature status of each capacitance link after the update() call.
    for(int i = 0; i < tArticle->numLinksCap; ++i)
    {
        /// - Store the index into the stringstream.
        out.str("");
        out << i;

        /// - Some analysis values.
        bool isPotentialFound = false;
        const double initial = parserFriendly.vCapTemperatures.at(i);
        const double final = tArticle->mCapacitanceLinks[i].getTemperature();

        /// - Check the temperature status of each capacitance link after the update() call.
        for(int j = 0; j < parserFriendly.numLinksPot; ++j)
        {
            /// - Check if there is a potential link on the same node.
            if ( parserFriendly.vPotPorts.at(j) == parserFriendly.vCapPorts.at(i) )
            {
                /// @test  Capacitance links that are coupled with a potential link have a
                ///        temperature that should not change.
                CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(
                        "Capacitor temperature (w/ potential), index: " + out.str(),
                        initial, final, tTolerance);
                isPotentialFound = true;
                break;
            }
        }
        if (false == isPotentialFound)
        {
            /// - If no potential was found on the node, make sure the temperature has changed some.
            if (tTolerance > fabs(final-initial) )
            {
                /// - Print values and test a failure.
                TEST_HS("Node name: " << tArticle->pNodes[i].getName());
                TEST_HS("   Initial T: " << initial);
                TEST_HS("   Updated T: " << final);
                CPPUNIT_ASSERT_MESSAGE( "Capacitor temperature, index: " + out.str(), false);
            }
        }
    }

    /// - Check the island analyzer outputs.
    CPPUNIT_ASSERT(0 < tArticle->netIslandAnalyzer.getIslandSize());

    std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests access to link members.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermalNetwork::testAccess()
{
    const char* test = "ThermalNetwork 11: Test access to needed link members.............";
    std::cout << "\n " << test;
    TEST_HS(test);

    /// - Construct a test article that is not friendly.
    ThermalNetwork article(tName);

    /// - Simulate default_data job.
    article.mConfig.cNodeFile = tNodeFile;
    article.mConfig.cCondFile = tCondFile;
    article.mConfig.cRadFile = tRadFile;
    article.mConfig.cHtrFile = tHtrFile;
    article.mConfig.cPanFile = tPanFile;
    article.mConfig.cEtcFile = tEtcFile;

    /// - Initialize the test article.
    article.initialize();

    /// @test  You should be able to access the temperature of each capacitor.
    CPPUNIT_ASSERT_NO_THROW_MESSAGE("access to getTemperature()", article.mCapacitanceLinks[0].getTemperature() );

    /// - This should fail because "numLinksCap" is protected.
//    article.numLinksCap;

    /// @test  GUNNS access to solver mode setter methods.
    FriendlyGunnsSolver* solver = static_cast<FriendlyGunnsSolver*>(&tArticle->netSolver);
    tArticle->setDummyMode();
    CPPUNIT_ASSERT(Gunns::DUMMY == solver->mSolverMode);
    tArticle->setSlaveMode();
    CPPUNIT_ASSERT(Gunns::SLAVE == solver->mSolverMode);
    tArticle->setNormalMode();
    CPPUNIT_ASSERT(Gunns::NORMAL == solver->mSolverMode);
    tArticle->setIslandMode(Gunns::SOLVE);
    CPPUNIT_ASSERT(Gunns::SOLVE  == solver->mIslandMode);

    /// @test  The heater miswire malfunction parameters are set by the access method.
    int index[3] = {2, 0, 1};
    tArticle->setMalfHtrMiswire(true, index);
    CPPUNIT_ASSERT(true  == tArticle->mMalfHtrMiswireFlag);
    CPPUNIT_ASSERT(2     == tArticle->mMalfHtrIndexValue[0]);
    CPPUNIT_ASSERT(0     == tArticle->mMalfHtrIndexValue[1]);
    CPPUNIT_ASSERT(1     == tArticle->mMalfHtrIndexValue[2]);

    /// @test  The heater miswire malfunction parameters are reset by the access method.
    tArticle->setMalfHtrMiswire();
    CPPUNIT_ASSERT(false == tArticle->mMalfHtrMiswireFlag);
    CPPUNIT_ASSERT(0     == tArticle->mMalfHtrIndexValue[0]);
    CPPUNIT_ASSERT(1     == tArticle->mMalfHtrIndexValue[1]);
    CPPUNIT_ASSERT(2     == tArticle->mMalfHtrIndexValue[2]);

    /// @test  The heater miswire malfunction flag is set by the access method but the index array
    ///        is not supplied.
    tArticle->setMalfHtrMiswire(true, index);
    tArticle->setMalfHtrMiswire(true);
    CPPUNIT_ASSERT(true  == tArticle->mMalfHtrMiswireFlag);
    CPPUNIT_ASSERT(0     == tArticle->mMalfHtrIndexValue[0]);
    CPPUNIT_ASSERT(1     == tArticle->mMalfHtrIndexValue[1]);
    CPPUNIT_ASSERT(2     == tArticle->mMalfHtrIndexValue[2]);

    std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests ability to temporarily edit a group of capacitance links.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermalNetwork::testCapacitanceEdit()
{
    const char* test = "ThermalNetwork 12: Test capacitor link group edits................";
    std::cout << "\n " << test;
    TEST_HS(test);

    /// @test  The test article has at least one capacitance edit group.
    CPPUNIT_ASSERT_MESSAGE("Test article does not test a multiple amount of Cap-edit groups.",
            tArticle->numCapEditGroups >= 2 );

    /// - Build an array of original capacitance values.
    double originalCapacitance[tArticle->numLinksCap];
    for(int i = 0; i < tArticle->numLinksCap; ++i)
    {
        originalCapacitance[i] = tArticle->mCapacitanceLinks[i].getCapacitance();
    }

    /// - Step the article with no changes to the group edits and verify no change.
    tArticle->update(tTimeStep);
    for(int i = 0; i < tArticle->numLinksCap; ++i)
    {
        /// @test  Capacitance value should be unchanged.
        CPPUNIT_ASSERT_DOUBLES_EQUAL(originalCapacitance[i],
                tArticle->mCapacitanceLinks[i].getCapacitance(), tTolerance);
    }

    /// - Increase a group edit scale factor.
    tArticle->mCapEditScaleFactor[0] = 11.5;

    /// - Step the article and test cap values.
    tArticle->update(tTimeStep);
    for(int i = 0; i < tArticle->numLinksCap; ++i)
    {
        if (0 == tArticle->mCapacitanceLinks[i].getEditCapacitanceGroup())
        {
            /// @test  Capacitance value should scaled by 11.5.
            CPPUNIT_ASSERT_DOUBLES_EQUAL(11.5 * originalCapacitance[i],
                    tArticle->mCapacitanceLinks[i].getCapacitance(), tTolerance);
        } else
        {
            /// @test  Capacitance value should be unchanged.
            CPPUNIT_ASSERT_DOUBLES_EQUAL(originalCapacitance[i],
                    tArticle->mCapacitanceLinks[i].getCapacitance(), tTolerance);
        }
    }

    /// - Return the scale factor to one.
    tArticle->mCapEditScaleFactor[0] = 1.0;

    /// - Step the article and test cap values.
    tArticle->update(tTimeStep);
    for(int i = 0; i < tArticle->numLinksCap; ++i)
    {
        /// @test  Capacitance value should be back to the original.
        CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mCapacitanceLinks[i].getCapacitance(),
                originalCapacitance[i], tTolerance);
    }

    /// - Change the other capacitance group scale factor to zero.
    tArticle->mCapEditScaleFactor[1] = 0.0;

    /// - Step the article and test cap values.
    tArticle->update(tTimeStep);
    for(int i = 0; i < tArticle->numLinksCap; ++i)
    {
        if (1 == tArticle->mCapacitanceLinks[i].getEditCapacitanceGroup())
        {
            /// @test  Capacitance value should be near zero.
            CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                    tArticle->mCapacitanceLinks[i].getCapacitance(), tTolerance);
        } else
        {
            /// @test  Capacitance value should be unchanged.
            CPPUNIT_ASSERT_DOUBLES_EQUAL(originalCapacitance[i],
                    tArticle->mCapacitanceLinks[i].getCapacitance(), tTolerance);
        }
    }

    /// - Return the scale factor to one.
    tArticle->mCapEditScaleFactor[1] = 1.0;

    /// - Step the article and test cap values.
    tArticle->update(tTimeStep);
    for(int i = 0; i < tArticle->numLinksCap; ++i)
    {
        /// @test  Capacitance value should be back to the original.
        CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mCapacitanceLinks[i].getCapacitance(),
                originalCapacitance[i], tTolerance);
    }

    std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the ThermalNetwork as a sub-network in a super-network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermalNetwork::testSuperNetwork()
{
    const char* test = "ThermalNetwork 13: Test in super-network..........................";
    std::cout << "\n " << test;
    TEST_HS(test);

    /// - Configure 2 instances of the test network.
    FriendlyThermalNetwork article1("article1");
    article1.mConfig.cNodeFile = tNodeFile;
    article1.mConfig.cCondFile = tCondFile;
    article1.mConfig.cRadFile  = tRadFile;
    article1.mConfig.cHtrFile  = tHtrFile;
    article1.mConfig.cPanFile  = tPanFile;
    article1.mConfig.cEtcFile  = tEtcFile;

    FriendlyThermalNetwork article2("article2");
    article2.mConfig.cNodeFile = tNodeFile;
    article2.mConfig.cCondFile = tCondFile;
    article2.mConfig.cRadFile  = tRadFile;
    article2.mConfig.cHtrFile  = tHtrFile;
    article2.mConfig.cPanFile  = tPanFile;
    article2.mConfig.cEtcFile  = tEtcFile;

    /// - Add these to the super-network.
    GunnsBasicSuperNetwork superNet("superNet");
    superNet.addSubNetwork(&article1);
    superNet.addSubNetwork(&article2);
    superNet.registerSuperNodes();

    /// @test  Super-network initialization.
    superNet.initialize();

    const int expectedSubNetCaps = parserFriendly.numLinksCap;
    const int expectedNumNodes = 2 * (expectedSubNetCaps + 1) + 1;
    CPPUNIT_ASSERT_EQUAL(expectedNumNodes, superNet.netNodeList.mNumNodes);
    CPPUNIT_ASSERT_EQUAL(0,                              article1.getNodeOffset());
    CPPUNIT_ASSERT_EQUAL(parserFriendly.numLinksCap + 1, article2.getNodeOffset());
    CPPUNIT_ASSERT(superNet.netNodeList.mNodes[0].isInitialized());
    CPPUNIT_ASSERT(superNet.netNodeList.mNodes[expectedNumNodes-1].isInitialized());
    FriendlyGunnsSolver* solver = static_cast<FriendlyGunnsSolver*>(&superNet.netSolver);
    CPPUNIT_ASSERT(solver->mInitFlag);
    CPPUNIT_ASSERT_EQUAL(expectedNumNodes - 1, solver->getNetworkSize());
    const int expectedNumLinks = 2 * (parserFriendly.numLinksCap
                                    + parserFriendly.numLinksCond
                                    + parserFriendly.numLinksRad
                                    + parserFriendly.numLinksHtr
                                    + parserFriendly.numLinksPan
                                    + parserFriendly.numLinksPot
                                    + parserFriendly.numLinksSrc);
    CPPUNIT_ASSERT_EQUAL(expectedNumLinks, solver->getNumLinks());
    CPPUNIT_ASSERT_EQUAL(0,                      article1.mCapacitanceLinks[0].getNodeMap()[0]);
    CPPUNIT_ASSERT_EQUAL(expectedSubNetCaps - 1, article1.mCapacitanceLinks[expectedSubNetCaps - 1].getNodeMap()[0]);
    CPPUNIT_ASSERT_EQUAL(expectedSubNetCaps + 1, article2.mCapacitanceLinks[0].getNodeMap()[0]);
    CPPUNIT_ASSERT_EQUAL(expectedNumNodes   - 3, article2.mCapacitanceLinks[expectedSubNetCaps - 1].getNodeMap()[0]);

    /// @test  Super-network update.
    solver->setIslandMode(Gunns::SOLVE);
    article1.netIslandAnalyzer.setAttachedNode(0);
    article2.netIslandAnalyzer.setAttachedNode(0);
    superNet.update(tTimeStep);
    CPPUNIT_ASSERT(0   < article1.netIslandAnalyzer.getIslandSize());
    CPPUNIT_ASSERT(0   < article2.netIslandAnalyzer.getIslandSize());
    CPPUNIT_ASSERT(0.0 < solver->mPotentialVector[0]);
    CPPUNIT_ASSERT_EQUAL(1, solver->mMajorStepCount);

    /// @test  Super-network and sub-network restart.
    article1.mCapEditScaleFactor[0] = -1.0;
    article2.mCapEditScaleFactor[0] = -1.0;
    CPPUNIT_ASSERT(0 < article1.numCapEditGroups);
    CPPUNIT_ASSERT(0 < article2.numCapEditGroups);
    CPPUNIT_ASSERT_NO_THROW(superNet.restart());
    CPPUNIT_ASSERT_NO_THROW(article1.restart());
    CPPUNIT_ASSERT_NO_THROW(article2.restart());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(DBL_EPSILON, article1.mCapEditScaleFactor[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(DBL_EPSILON, article2.mCapEditScaleFactor[0], 0.0);

    std::cout << "... Pass";
}
