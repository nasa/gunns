/// @copyright Copyright 2024 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.

#include "UtGunnsBasicFlowOrchestrator.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsBasicFlowOrchestrator class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsBasicFlowOrchestrator::UtGunnsBasicFlowOrchestrator()
    :
    tNumLinks(NUMLINKS),
    tNumNodes(NUMNODES),
    tArticle(tNumLinks, tNumNodes),
    tLinksArray(0),
    tNodesArray(0),
    tConductors(),
    tNodes(),
    tLinkNodeMaps(0),
    tLinkNumPorts(0),
    tName("test article")
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsBasicFlowOrchestrator class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsBasicFlowOrchestrator::~UtGunnsBasicFlowOrchestrator()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicFlowOrchestrator::tearDown()
{
    delete [] tLinkNumPorts;
    delete [] tLinkNodeMaps;
    delete [] tNodesArray;
    delete [] tLinksArray;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicFlowOrchestrator::setUp()
{
    /// - Build the link and node pointer arrays that the solver would pass to the orchestrator
    ///   during initialization.
    tLinksArray = new GunnsBasicLink*[NUMLINKS];
    for (int i=0; i<NUMLINKS; ++i) {
        tLinksArray[i] = &tConductors[i];
    }
    tNodesArray = new GunnsBasicNode*[NUMNODES];
    for (int i=0; i<NUMNODES; ++i) {
        tNodesArray[i] = &tNodes[i];
    }

    /// - Initialize the links.
    std::vector<GunnsBasicLink*> networkLinks;
    for (int i=0; i<NUMLINKS; ++i) {
        networkLinks.push_back(&tConductors[i]);
    }
    GunnsNodeList nodeList;
    nodeList.mNodes    = tNodes;
    nodeList.mNumNodes = tNumNodes;
    GunnsBasicConductorConfigData linkConfig("link", &nodeList, 1.0);
    GunnsBasicConductorInputData  linkInput;
    tConductors[0].initialize(linkConfig, linkInput, networkLinks, 0, 1);
    tConductors[1].initialize(linkConfig, linkInput, networkLinks, 1, 2);

    /// - Build the link node map and number of ports arrays that the solver would pass to the
    ///   orchestrator during initialization.
    tLinkNodeMaps = new int*[NUMLINKS];
    tLinkNumPorts = new int[NUMLINKS];
    for (int i=0; i<NUMLINKS; ++i) {
        tLinkNodeMaps[i] = tConductors[i].getNodeMap();
        tLinkNumPorts[i] = tConductors[i].getNumberPorts();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the default constructor of the GunnsBasicFlowOrchestrator class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicFlowOrchestrator::testDefaultConstruction()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsBasicFlowOrchestrator 01: testDefaultConstruction ...........";

    CPPUNIT_ASSERT(tNumLinks == tArticle.mNumLinks);
    CPPUNIT_ASSERT(tNumNodes == tArticle.mNumNodes);
    CPPUNIT_ASSERT(0         == tArticle.mLinks);
    CPPUNIT_ASSERT(0         == tArticle.mNodes);
    CPPUNIT_ASSERT(0         == tArticle.mLinkNodeMaps);
    CPPUNIT_ASSERT(0         == tArticle.mLinkNumPorts);
    CPPUNIT_ASSERT(""        == tArticle.mName);
    CPPUNIT_ASSERT(false     == tArticle.mInitFlag);

    /// - Dynamic construction/deletion for code coverage.
    GunnsBasicFlowOrchestrator* article = new GunnsBasicFlowOrchestrator(tNumLinks, tNumNodes);
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the initialize method of the GunnsBasicFlowOrchestrator class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicFlowOrchestrator::testInitialize()
{
    std::cout << "\n UtGunnsBasicFlowOrchestrator 02: testInitialize ....................";

    CPPUNIT_ASSERT_NO_THROW(tArticle.initialize(tName, tLinksArray, tNodesArray, tLinkNodeMaps, tLinkNumPorts));

    /// - Test nominal initialization.
    CPPUNIT_ASSERT(tLinksArray   == tArticle.mLinks);
    CPPUNIT_ASSERT(tNodesArray   == tArticle.mNodes);
    CPPUNIT_ASSERT(tNumLinks     == tArticle.mNumLinks);
    CPPUNIT_ASSERT(tNumNodes     == tArticle.mNumNodes);
    CPPUNIT_ASSERT(tLinkNodeMaps == tArticle.mLinkNodeMaps);
    CPPUNIT_ASSERT(tLinkNumPorts == tArticle.mLinkNumPorts);
    CPPUNIT_ASSERT(tName         == tArticle.mName);
    CPPUNIT_ASSERT(true          == tArticle.mInitFlag);
    CPPUNIT_ASSERT(true          == tArticle.isInitialized());

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests errors during initialize of the GunnsBasicFlowOrchestrator class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicFlowOrchestrator::testInitializeExceptions()
{
    std::cout << "\n UtGunnsBasicFlowOrchestrator 03: testInitializeExceptions ..........";

    /// - Test exception thrown from missing name.
    CPPUNIT_ASSERT_THROW(tArticle.initialize("", tLinksArray, tNodesArray, tLinkNodeMaps, tLinkNumPorts), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);
    CPPUNIT_ASSERT(false == tArticle.isInitialized());

    /// - Test exception thrown number of links < 1.
    tNumLinks = 0;
    CPPUNIT_ASSERT_THROW(tArticle.initialize(tName, tLinksArray, tNodesArray, tLinkNodeMaps, tLinkNumPorts), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);
    tNumLinks = NUMLINKS;

    /// - Test exception thrown number of nodes < 1.
    tNumNodes = 0;
    CPPUNIT_ASSERT_THROW(tArticle.initialize(tName, tLinksArray, tNodesArray, tLinkNodeMaps, tLinkNumPorts), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);
    tNumNodes = NUMNODES;

    /// - Test exception thrown for null links array.
    CPPUNIT_ASSERT_THROW(tArticle.initialize(tName, 0, tNodesArray, tLinkNodeMaps, tLinkNumPorts), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);

    /// - Test exception thrown for null nodes array.
    CPPUNIT_ASSERT_THROW(tArticle.initialize(tName, tLinksArray, 0, tLinkNodeMaps, tLinkNumPorts), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);

    /// - Test exception thrown for null link node maps array.
    CPPUNIT_ASSERT_THROW(tArticle.initialize(tName, tLinksArray, tNodesArray, 0, tLinkNumPorts), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);

    /// - Test exception thrown for null link number of ports array.
    CPPUNIT_ASSERT_THROW(tArticle.initialize(tName, tLinksArray, tNodesArray, tLinkNodeMaps, 0), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the update method of the GunnsBasicFlowOrchestrator class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicFlowOrchestrator::testUpdate()
{
    std::cout << "\n UtGunnsBasicFlowOrchestrator 04: testUpdate ........................";

    tArticle.initialize(tName, tLinksArray, tNodesArray, tLinkNodeMaps, tLinkNumPorts);

    /// - Initialize the links & nodes.
    tNodes[0].initialize("node", 10.0);
    tNodes[1].initialize("node",  5.0);
    tNodes[2].initialize("node",  0.0);

    tConductors[0].getPotentialVector()[0] = tNodes[0].getPotential();
    tConductors[0].getPotentialVector()[1] = tNodes[1].getPotential();
    tConductors[1].getPotentialVector()[0] = tNodes[1].getPotential();
    tConductors[1].getPotentialVector()[1] = tNodes[2].getPotential();

    /// - Step the links and compute their expected flux and expected node flux.
    const double dt               = 0.1;
    const double expectedFlux0    = tNodes[0].getPotential() - tNodes[1].getPotential();
    const double expectedFlux1    = tNodes[1].getPotential() - tNodes[2].getPotential();
    const double expectedNetFlux0 = -expectedFlux0;
    const double expectedNetFlux1 =  expectedFlux0 - expectedFlux1;
    /// - Expected net flux of the last node is zero because it is the Ground node.
    const double expectedNetFlux2 = 0.0;

    tConductors[0].step(dt);
    tConductors[1].step(dt);

    /// - Test update method.
    tArticle.update(dt);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux0,    tConductors[0].getFlux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux1,    tConductors[1].getFlux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNetFlux0, tNodes[0].getNetFlux(),   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNetFlux1, tNodes[1].getNetFlux(),   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNetFlux2, tNodes[2].getNetFlux(),   DBL_EPSILON);

    std::cout << "... Pass";
}
