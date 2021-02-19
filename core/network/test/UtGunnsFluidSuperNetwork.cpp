/**
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
 ((core/network/GunnsFluidSuperNetwork.o))
*/

#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidSuperNetwork.hh"

/// @details  Test identification number.
int UtGunnsFluidSuperNetwork::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name (--) Instance name for H&S messages.
///
/// @details  Default constructs this test article.
////////////////////////////////////////////////////////////////////////////////////////////////////
FriendlyGunnsFluidSubNetwork::FriendlyGunnsFluidSubNetwork(const std::string& name)
    :
    GunnsNetworkBase(name, N_NODES, netNodes),
    netInternalFluidConfig(0),
    netNodes()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this test article.
////////////////////////////////////////////////////////////////////////////////////////////////////
FriendlyGunnsFluidSubNetwork::~FriendlyGunnsFluidSubNetwork()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name (--) Instance name for H&S messages.
///
/// @details  Implements the pure virtual base method for testability.  This mimics the recommended
///           functions of a real fluid network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void FriendlyGunnsFluidSubNetwork::initNodes(const std::string& name)
{
    GunnsFluidNode* nodes = static_cast<GunnsFluidNode*>(netNodeList.mNodes);
    nodes[NODE_0 + netSuperNodesOffset].initialize(name + createNodeName(NODE_0 + netSuperNodesOffset), netInternalFluidConfig);
    nodes[NODE_1 + netSuperNodesOffset].initialize(name + createNodeName(NODE_1 + netSuperNodesOffset), netInternalFluidConfig);
    nodes[NODE_2 + netSuperNodesOffset].initialize(name + createNodeName(NODE_2 + netSuperNodesOffset), netInternalFluidConfig);
    if (!netIsSubNetwork) {
        nodes[VACUUM].initialize(name + ".GROUND", netInternalFluidConfig);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Implements the pure virtual base method for testability.  This mimics some of the
///           recommended functions of a real fluid network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void FriendlyGunnsFluidSubNetwork::initNetwork()
{
    GunnsConfigData solverConfig(mName + ".netSolver", 1.0, 1.0, 1, 1);
    if (!netIsSubNetwork) {
        netSolver.initializeFluidNodes(netNodeList);
        netSolver.initialize(solverConfig, netLinks);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS network base class unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidSuperNetwork::UtGunnsFluidSuperNetwork()
    :
    CppUnit::TestFixture(),
    tName(""),
    tArticle(0),
    tSubNetworkA("tSubNetworkA"),
    tSubNetworkB("tSubNetworkB")
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS network base class unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidSuperNetwork::~UtGunnsFluidSuperNetwork()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSuperNetwork::setUp()
{
    /// - Default construct the nominal test article.
    tName    = "tArticle";
    tArticle = new FriendlyGunnsFluidSuperNetwork(tName);

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSuperNetwork::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete tArticle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS network base class default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSuperNetwork::testDefaultConstruction()
{
    UT_RESULT_FIRST;

    /// @test Default construction values.
    CPPUNIT_ASSERT(0                    == tArticle->netNodes);
    CPPUNIT_ASSERT(0                    == tArticle->netSolver.getPotentialVector());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS super network base class registerSuperNodes method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSuperNetwork::testRegisterSuperNodes()
{
    UT_RESULT;

    /// - Create some fluid networks.
    FriendlyGunnsFluidSubNetwork subNetworkA("subNetworkA");
    FriendlyGunnsFluidSubNetwork subNetworkB("subNetworkB");

    /// @test results of the registerSuperNodes method.
    tArticle->addSubNetwork(&subNetworkA);
    tArticle->addSubNetwork(&subNetworkB);
    tArticle->registerSuperNodes();

    CPPUNIT_ASSERT(2*FriendlyGunnsFluidSubNetwork::VACUUM+1 == tArticle->netNodeList.mNumNodes);
    CPPUNIT_ASSERT(2*FriendlyGunnsFluidSubNetwork::VACUUM+1 == subNetworkA.netNodeList.mNumNodes);
    CPPUNIT_ASSERT(2*FriendlyGunnsFluidSubNetwork::VACUUM+1 == subNetworkB.netNodeList.mNumNodes);
    CPPUNIT_ASSERT(0                                        != tArticle->netNodeList.mNodes);
    CPPUNIT_ASSERT(tArticle->netNodeList.mNodes             == subNetworkA.netNodeList.mNodes);
    CPPUNIT_ASSERT(tArticle->netNodeList.mNodes             == subNetworkB.netNodeList.mNodes);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS super network base class initialize method with sub-networks.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSuperNetwork::testInitialize()
{
    UT_RESULT;

    /// - Set up fluid sub-networks with identical fluid config.
    FriendlyGunnsFluidSubNetwork subNetworkA("subNetworkA");
    FriendlyGunnsFluidSubNetwork subNetworkB("subNetworkB");

    DefinedFluidProperties fluidProperties;
    FluidProperties::FluidType fluidTypes[1] = {FluidProperties::GUNNS_N2};
    PolyFluidConfigData fluidConfig(&fluidProperties, fluidTypes, 1);
    subNetworkA.netInternalFluidConfig = &fluidConfig;
    subNetworkB.netInternalFluidConfig = &fluidConfig;

    tArticle->addSubNetwork(&subNetworkA);
    tArticle->addSubNetwork(&subNetworkB);
    tArticle->registerSuperNodes();

    /// @test results of super-network and sub-networks initialization.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize());

    /// @test fluid nodes initialization.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(101.325, tArticle->netNodes[0].getPotential(), 0.001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(101.325, tArticle->netNodes[1].getPotential(), 0.001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(101.325, tArticle->netNodes[2].getPotential(), 0.001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(101.325, tArticle->netNodes[3].getPotential(), 0.001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(101.325, tArticle->netNodes[4].getPotential(), 0.001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(101.325, tArticle->netNodes[5].getPotential(), 0.001);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->netNodes[6].getPotential());

    CPPUNIT_ASSERT(std::string("tArticle.Node_0") == tArticle->netNodes[0].getName());
    CPPUNIT_ASSERT(std::string("tArticle.Node_1") == tArticle->netNodes[1].getName());
    CPPUNIT_ASSERT(std::string("tArticle.Node_2") == tArticle->netNodes[2].getName());
    CPPUNIT_ASSERT(std::string("tArticle.Node_3") == tArticle->netNodes[3].getName());
    CPPUNIT_ASSERT(std::string("tArticle.Node_4") == tArticle->netNodes[4].getName());
    CPPUNIT_ASSERT(std::string("tArticle.Node_5") == tArticle->netNodes[5].getName());
    CPPUNIT_ASSERT(std::string("tArticle.VACUUM") == tArticle->netNodes[6].getName());

    /// - Call registerJoints for code coverage.
    tArticle->registerJoints();

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS super network base class initialize method error handling.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSuperNetwork::testInitializeErrors()
{
    UT_RESULT;

    /// @test createJointNetwork throws exception.
    CPPUNIT_ASSERT_THROW(tArticle->createJointNetwork(0), TsInitializationException);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS fluid super network class catching missing fluid configurations in the
///           sub-networks.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSuperNetwork::testMissingFluidConfig()
{
    UT_RESULT;

    FriendlyGunnsFluidSubNetwork subNetworkA("subNetworkA");
    FriendlyGunnsFluidSubNetwork subNetworkB("subNetworkB");
    DefinedFluidProperties       properties;

    tArticle->addSubNetwork(&subNetworkA);
    tArticle->addSubNetwork(&subNetworkB);
    tArticle->registerSuperNodes();

    /// @test 2nd network missing fluid config.
    FluidProperties::FluidType fluidTypes[1] = {FluidProperties::GUNNS_N2};
    PolyFluidConfigData fluidConfig(&properties, fluidTypes, 1);
    subNetworkA.netInternalFluidConfig = &fluidConfig;
    subNetworkB.netInternalFluidConfig = 0;
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize());
    CPPUNIT_ASSERT(std::string("") == tArticle->netNodes[6].getName());

    /// @test 1st network missing fluid config.
    subNetworkA.netInternalFluidConfig = 0;
    subNetworkB.netInternalFluidConfig = &fluidConfig;
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize());
    CPPUNIT_ASSERT(std::string("") == tArticle->netNodes[6].getName());

    /// @test both networks missing fluid config.
    subNetworkA.netInternalFluidConfig = 0;
    subNetworkB.netInternalFluidConfig = 0;
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize());
    CPPUNIT_ASSERT(std::string("") == tArticle->netNodes[6].getName());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS fluid super network class catching sub-networks having different
///           number of fluid types.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSuperNetwork::testFluidConfigSize()
{
    UT_RESULT;

    FriendlyGunnsFluidSubNetwork subNetworkA("subNetworkA");
    FriendlyGunnsFluidSubNetwork subNetworkB("subNetworkB");
    DefinedFluidProperties       properties;

    tArticle->addSubNetwork(&subNetworkA);
    tArticle->addSubNetwork(&subNetworkB);
    tArticle->registerSuperNodes();

    /// @test networks have different number of fluid types.
    FluidProperties::FluidType fluidTypes1[1] = {FluidProperties::GUNNS_N2};
    FluidProperties::FluidType fluidTypes2[2] = {FluidProperties::GUNNS_N2,
                                                 FluidProperties::GUNNS_O2};
    PolyFluidConfigData fluidConfig1(&properties, fluidTypes1, 1);
    PolyFluidConfigData fluidConfig2(&properties, fluidTypes2, 2);
    subNetworkA.netInternalFluidConfig = &fluidConfig1;
    subNetworkB.netInternalFluidConfig = &fluidConfig2;
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize());
    CPPUNIT_ASSERT(std::string("") == tArticle->netNodes[6].getName());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS fluid super network class catching sub-networks having different
///           fluid types.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSuperNetwork::testFluidConfigTypes()
{
    UT_RESULT;

    FriendlyGunnsFluidSubNetwork subNetworkA("subNetworkA");
    FriendlyGunnsFluidSubNetwork subNetworkB("subNetworkB");
    DefinedFluidProperties       properties;

    tArticle->addSubNetwork(&subNetworkA);
    tArticle->addSubNetwork(&subNetworkB);
    tArticle->registerSuperNodes();

    /// @test networks have different fluid types.
    FluidProperties::FluidType fluidTypesA[1] = {FluidProperties::GUNNS_N2};
    FluidProperties::FluidType fluidTypesB[1] = {FluidProperties::GUNNS_O2};
    PolyFluidConfigData fluidConfigA(&properties, fluidTypesA, 1);
    PolyFluidConfigData fluidConfigB(&properties, fluidTypesB, 1);
    subNetworkA.netInternalFluidConfig = &fluidConfigA;
    subNetworkB.netInternalFluidConfig = &fluidConfigB;
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize());
    CPPUNIT_ASSERT(std::string("") == tArticle->netNodes[6].getName());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS fluid super network class catching some but not all sub-networks
///           having trace compounds configurations.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSuperNetwork::testMissingFluidConfigTc()
{
    UT_RESULT;

    FriendlyGunnsFluidSubNetwork subNetworkA("subNetworkA");
    FriendlyGunnsFluidSubNetwork subNetworkB("subNetworkB");
    DefinedFluidProperties       properties;
    DefinedChemicalCompounds     compounds;

    tArticle->addSubNetwork(&subNetworkA);
    tArticle->addSubNetwork(&subNetworkB);
    tArticle->registerSuperNodes();

    FluidProperties::FluidType fluidTypes[1] = {FluidProperties::GUNNS_N2};
    ChemicalCompound::Type     tcTypes[1]    = {ChemicalCompound::LIOH};
    GunnsFluidTraceCompoundsConfigData tcConfig(tcTypes, 1, "tcConfig");

    {
        /// @test 2nd network missing trace compounds config.
        PolyFluidConfigData fluidConfigA(&properties, fluidTypes, 1, &tcConfig);
        PolyFluidConfigData fluidConfigB(&properties, fluidTypes, 1, 0);
        subNetworkA.netInternalFluidConfig = &fluidConfigA;
        subNetworkB.netInternalFluidConfig = &fluidConfigB;
        CPPUNIT_ASSERT_NO_THROW(tArticle->initialize());
        CPPUNIT_ASSERT(std::string("") == tArticle->netNodes[6].getName());
    } {
        /// @test 1st network missing trace compounds config.
        PolyFluidConfigData fluidConfigA(&properties, fluidTypes, 1, 0);
        PolyFluidConfigData fluidConfigB(&properties, fluidTypes, 1, &tcConfig);
        subNetworkA.netInternalFluidConfig = &fluidConfigA;
        subNetworkB.netInternalFluidConfig = &fluidConfigB;
        CPPUNIT_ASSERT_NO_THROW(tArticle->initialize());
        CPPUNIT_ASSERT(std::string("") == tArticle->netNodes[6].getName());
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS fluid super network class catching sub-networks with different number
///           of trace compounds.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSuperNetwork::testFluidConfigTcSize()
{
    UT_RESULT;

    FriendlyGunnsFluidSubNetwork subNetworkA("subNetworkA");
    FriendlyGunnsFluidSubNetwork subNetworkB("subNetworkB");
    DefinedFluidProperties       properties;
    DefinedChemicalCompounds     compounds;

    tArticle->addSubNetwork(&subNetworkA);
    tArticle->addSubNetwork(&subNetworkB);
    tArticle->registerSuperNodes();

    FluidProperties::FluidType fluidTypes[1] = {FluidProperties::GUNNS_N2};
    ChemicalCompound::Type     tcTypes1[1]   = {ChemicalCompound::LIOH};
    ChemicalCompound::Type     tcTypes2[2]   = {ChemicalCompound::LIOH,
                                                ChemicalCompound::LIOHH2O};
    GunnsFluidTraceCompoundsConfigData tcConfig1(tcTypes1, 1, "tcConfig1");
    GunnsFluidTraceCompoundsConfigData tcConfig2(tcTypes2, 2, "tcConfig2");

    /// @test sub-network having different number of trace compounds.
    PolyFluidConfigData fluidConfigA(&properties, fluidTypes, 1, &tcConfig1);
    PolyFluidConfigData fluidConfigB(&properties, fluidTypes, 1, &tcConfig2);
    subNetworkA.netInternalFluidConfig = &fluidConfigA;
    subNetworkB.netInternalFluidConfig = &fluidConfigB;
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize());
    CPPUNIT_ASSERT(std::string("") == tArticle->netNodes[6].getName());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS fluid super network class catches sub-networks with different trace
///           compounds.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSuperNetwork::testFluidConfigTcTypes()
{
    UT_RESULT;

    FriendlyGunnsFluidSubNetwork subNetworkA("subNetworkA");
    FriendlyGunnsFluidSubNetwork subNetworkB("subNetworkB");
    DefinedFluidProperties       properties;
    DefinedChemicalCompounds     compounds;

    tArticle->addSubNetwork(&subNetworkA);
    tArticle->addSubNetwork(&subNetworkB);
    tArticle->registerSuperNodes();

    FluidProperties::FluidType fluidTypes[1] = {FluidProperties::GUNNS_N2};
    ChemicalCompound::Type     tcTypesA[1]   = {ChemicalCompound::LIOH};
    ChemicalCompound::Type     tcTypesB[1]   = {ChemicalCompound::LIOHH2O};
    GunnsFluidTraceCompoundsConfigData tcConfigA(tcTypesA, 1, "tcConfig1");
    GunnsFluidTraceCompoundsConfigData tcConfigB(tcTypesB, 1, "tcConfig2");

    /// @test sub-network having different number of trace compounds.
    PolyFluidConfigData fluidConfigA(&properties, fluidTypes, 1, &tcConfigA);
    PolyFluidConfigData fluidConfigB(&properties, fluidTypes, 1, &tcConfigB);
    subNetworkA.netInternalFluidConfig = &fluidConfigA;
    subNetworkB.netInternalFluidConfig = &fluidConfigB;
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize());
    CPPUNIT_ASSERT(std::string("") == tArticle->netNodes[6].getName());

    UT_PASS_FINAL;
}
