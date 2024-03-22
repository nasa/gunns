/// Copyright 2024 United States Government as represented by the Administrator of the
/// National Aeronautics and Space Administration.  All Rights Reserved.

#include "UtGunnsFluidFlowOrchestrator.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsFluidFlowOrchestrator class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidFlowOrchestrator::UtGunnsFluidFlowOrchestrator()
    :
    tNumLinks(NUMLINKS),
    tNumNodes(NUMNODES),
    tArticle(tNumLinks, tNumNodes),
    tLinksArray(0),
    tNodesArray(0),
    tSource1(),
    tSource2(),
    tSource3(),
    tCapacitor1(),
    tCapacitor2(),
    tNodes(),
    tName("test article"),
    tNetNodeList(),
    tNetLinks(),
    tFluidProperties(0),
    tFluidConfig(0),
    tFluidInput(0),
    tFractions(0),
    tSourceConfig(0),
    tSourceInput(0),
    tCapacitorConfig(0),
    tCapacitorInput(0),
    tLinkNodeMaps(0),
    tLinkNumPorts(0)
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsFluidFlowOrchestrator class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidFlowOrchestrator::~UtGunnsFluidFlowOrchestrator()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFlowOrchestrator::tearDown()
{
    delete [] tLinkNumPorts;
    delete [] tLinkNodeMaps;
    delete tCapacitorInput;
    delete tCapacitorConfig;
    delete tSourceInput;
    delete tSourceConfig;
    delete tFluidInput;
    delete tFluidConfig;
    delete [] tFractions;
    delete tFluidProperties;
    delete [] tNodesArray;
    delete [] tLinksArray;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFlowOrchestrator::setUp()
{
    /// - Build the link and node pointer arrays that the solver would pass to the orchestrator
    ///   during initialization.
    tLinksArray = new GunnsBasicLink*[NUMLINKS];
    tLinksArray[0] = &tSource1;
    tLinksArray[1] = &tSource2;
    tLinksArray[2] = &tSource3;
    tLinksArray[3] = &tCapacitor1;
    tLinksArray[4] = &tCapacitor2;
    tNodesArray = new GunnsBasicNode*[NUMNODES];
    for (int i=0; i<NUMNODES; ++i) {
        tNodesArray[i] = &tNodes[i];
    }

    tFluidProperties = new DefinedFluidProperties();
    FluidProperties::FluidType types[2];
    types[0]      = FluidProperties::GUNNS_N2;
    types[1]      = FluidProperties::GUNNS_O2;
    tFractions    = new double[2];
    tFractions[0] = 0.5;
    tFractions[1] = 0.5;
    tFluidConfig = new PolyFluidConfigData(tFluidProperties, types, 2);
    tFluidInput  = new PolyFluidInputData(283.15,                   //temperature
                                          700.728,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          tFractions);              //massFractions

    for (int i=0; i<NUMNODES; ++i) {
        tNodes[i].initialize("tNodes", tFluidConfig);
        tNodes[i].getContent()->initialize(*tFluidConfig, *tFluidInput);
        tNodes[i].resetFlows();
    }
    tNetNodeList.mNodes    = tNodes;
    tNetNodeList.mNumNodes = tNumNodes;

    /// - Create links config & input data.
    tSourceConfig    = new GunnsFluidSourceConfigData("Source", &tNetNodeList);
    tSourceInput     = new GunnsFluidSourceInputData(false, 0.0, 0.0);
    tCapacitorConfig = new GunnsFluidCapacitorConfigData("Capacitor", &tNetNodeList, 0.0);
    tCapacitorInput  = new GunnsFluidCapacitorInputData(false, 0.0, 1.0, tFluidInput);

    /// - Initialize the source links to create a flow loop nodes 0-1-2-0.
    tSource1.initialize(*tSourceConfig, *tSourceInput, tNetLinks, 0, 1);
    tSource2.initialize(*tSourceConfig, *tSourceInput, tNetLinks, 1, 2);
    tSource3.initialize(*tSourceConfig, *tSourceInput, tNetLinks, 2, 0);

    /// - Initialize the capacitor links to make nodes 0, 1 capacitive, 2 is non-capacitive.
    tCapacitor1.initialize(*tCapacitorConfig, *tCapacitorInput, tNetLinks, 0, 3);
    tCapacitor2.initialize(*tCapacitorConfig, *tCapacitorInput, tNetLinks, 1, 3);

    /// - Build the link node map and number of ports arrays that the solver would pass to the
    ///   orchestrator during initialization.
    tLinkNodeMaps = new int*[NUMLINKS];
    tLinkNumPorts = new int[NUMLINKS];
    for (int i=0; i<NUMLINKS; ++i) {
        tLinkNodeMaps[i] = tLinksArray[i]->getNodeMap();
        tLinkNumPorts[i] = tLinksArray[i]->getNumberPorts();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the default constructor of the GunnsFluidFlowOrchestrator class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFlowOrchestrator::testDefaultConstruction()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsFluidFlowOrchestrator 01: testDefaultConstruction ...........";

    /// - Base class attributes.
    CPPUNIT_ASSERT(tNumLinks == tArticle.mNumLinks);
    CPPUNIT_ASSERT(tNumNodes == tArticle.mNumNodes);

    /// - Test article attributes.
    CPPUNIT_ASSERT(0         == tArticle.mLinkStates);
    CPPUNIT_ASSERT(0         == tArticle.mNodeStates);
    CPPUNIT_ASSERT(0         == tArticle.mNumIncompleteLinks);

    /// - Dynamic construction/deletion for code coverage.
    GunnsFluidFlowOrchestrator* article = new GunnsFluidFlowOrchestrator(tNumLinks, tNumNodes);
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the initialize method of the GunnsFluidFlowOrchestrator class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFlowOrchestrator::testInitialize()
{
    std::cout << "\n UtGunnsFluidFlowOrchestrator 02: testInitialize ....................";

    CPPUNIT_ASSERT_NO_THROW(tArticle.initialize(tName, tLinksArray, tNodesArray, tLinkNodeMaps, tLinkNumPorts));

    /// - Test nominal initialization of base class.
    CPPUNIT_ASSERT(tNumLinks   == tArticle.mNumLinks);
    CPPUNIT_ASSERT(tNumNodes   == tArticle.mNumNodes);
    CPPUNIT_ASSERT(tName       == tArticle.mName);
    CPPUNIT_ASSERT(true        == tArticle.mInitFlag);
    CPPUNIT_ASSERT(true        == tArticle.isInitialized());

    /// - Test nominal initialization of test article.
    CPPUNIT_ASSERT(false                                  == tArticle.mLinkStates[tNumLinks-1]);
    CPPUNIT_ASSERT(GunnsFluidFlowOrchestrator::INCOMPLETE == tArticle.mNodeStates[tNumNodes-1]);
    CPPUNIT_ASSERT(0                                      == tArticle.mNumIncompleteLinks);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests errors during initialize of the GunnsFluidFlowOrchestrator class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFlowOrchestrator::testInitializeExceptions()
{
    std::cout << "\n UtGunnsFluidFlowOrchestrator 03: testInitializeExceptions ..........";

    /// - Nothing currently to test, this is a placeholder.

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the update method of the GunnsFluidFlowOrchestrator class when there
///           are no link flows.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFlowOrchestrator::testUpdateNoFlow()
{
    std::cout << "\n UtGunnsFluidFlowOrchestrator 04: testUpdateNoFlow ..................";

    tArticle.initialize(tName, tLinksArray, tNodesArray, tLinkNodeMaps, tLinkNumPorts);

    const double dt = 0.1;
    tArticle.update(dt);

    CPPUNIT_ASSERT(true                                 == tArticle.mLinkStates[0]);
    CPPUNIT_ASSERT(true                                 == tArticle.mLinkStates[1]);
    CPPUNIT_ASSERT(true                                 == tArticle.mLinkStates[2]);
    CPPUNIT_ASSERT(true                                 == tArticle.mLinkStates[3]);
    CPPUNIT_ASSERT(true                                 == tArticle.mLinkStates[4]);
    CPPUNIT_ASSERT(GunnsFluidFlowOrchestrator::COMPLETE == tArticle.mNodeStates[0]);
    CPPUNIT_ASSERT(GunnsFluidFlowOrchestrator::COMPLETE == tArticle.mNodeStates[1]);
    CPPUNIT_ASSERT(GunnsFluidFlowOrchestrator::COMPLETE == tArticle.mNodeStates[2]);
    CPPUNIT_ASSERT(GunnsFluidFlowOrchestrator::COMPLETE == tArticle.mNodeStates[3]);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the update method of the GunnsFluidFlowOrchestrator class with a
///           nominal flow case.  Here we'll have some of the nodes overflowing, but not all.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFlowOrchestrator::testUpdateNominal()
{
    std::cout << "\n UtGunnsFluidFlowOrchestrator 05: testUpdateNominal .................";

    tArticle.initialize(tName, tLinksArray, tNodesArray, tLinkNodeMaps, tLinkNumPorts);

    const double dt = 0.1;

    /// - Edit node 1 to be smaller volume than 0.
    tCapacitor2.editVolume(true, 0.1);

    /// - Set mass flow rate around the loop that overflows node 1 but not 0.
    const double node0Mass = tNodes[0].getContent()->getMass();
    const double mdot = 0.5 * node0Mass / dt;

    tSource1.setFlowDemand(mdot);
    tSource2.setFlowDemand(mdot);
    tSource3.setFlowDemand(mdot);

    tSource1.step(dt);
    tSource2.step(dt);
    tSource3.step(dt);
    tCapacitor1.step(dt);
    tCapacitor2.step(dt);

    tArticle.update(dt);

    /// - Test that all nodes & links completed.
    CPPUNIT_ASSERT(true                                 == tArticle.mLinkStates[0]);
    CPPUNIT_ASSERT(true                                 == tArticle.mLinkStates[1]);
    CPPUNIT_ASSERT(true                                 == tArticle.mLinkStates[2]);
    CPPUNIT_ASSERT(true                                 == tArticle.mLinkStates[3]);
    CPPUNIT_ASSERT(true                                 == tArticle.mLinkStates[4]);
    CPPUNIT_ASSERT(GunnsFluidFlowOrchestrator::COMPLETE == tArticle.mNodeStates[0]);
    CPPUNIT_ASSERT(GunnsFluidFlowOrchestrator::COMPLETE == tArticle.mNodeStates[1]);
    CPPUNIT_ASSERT(GunnsFluidFlowOrchestrator::COMPLETE == tArticle.mNodeStates[2]);
    CPPUNIT_ASSERT(GunnsFluidFlowOrchestrator::COMPLETE == tArticle.mNodeStates[3]);

    /// - Test that we got overflows in the expected nodes.
    CPPUNIT_ASSERT(false                                == tNodes[0].isOverflowing(dt));
    CPPUNIT_ASSERT(true                                 == tNodes[1].isOverflowing(dt));
    CPPUNIT_ASSERT(true                                 == tNodes[2].isOverflowing(dt));
    CPPUNIT_ASSERT(false                                == tNodes[3].isOverflowing(dt));

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the update method of the GunnsFluidFlowOrchestrator class with an
///           overflow loop case.  All of the nodes are overflowing.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFlowOrchestrator::testUpdateOverflowLoop()
{
    std::cout << "\n UtGunnsFluidFlowOrchestrator 06: testUpdateOverflowLoop ............";

    tArticle.initialize(tName, tLinksArray, tNodesArray, tLinkNodeMaps, tLinkNumPorts);

    const double dt = 0.1;

    /// - Set mass flow rate around the loop that overflows nodes 0 and 1.
    const double node0Mass = tNodes[0].getContent()->getMass();
    const double mdot = 2.0 * node0Mass / dt;

    tSource1.setFlowDemand(mdot);
    tSource2.setFlowDemand(mdot);
    tSource3.setFlowDemand(mdot);

    tSource1.step(dt);
    tSource2.step(dt);
    tSource3.step(dt);
    tCapacitor1.step(dt);
    tCapacitor2.step(dt);

    tArticle.update(dt);

    /// - Test that all nodes & links completed.
    CPPUNIT_ASSERT(true                                 == tArticle.mLinkStates[0]);
    CPPUNIT_ASSERT(true                                 == tArticle.mLinkStates[1]);
    CPPUNIT_ASSERT(true                                 == tArticle.mLinkStates[2]);
    CPPUNIT_ASSERT(true                                 == tArticle.mLinkStates[3]);
    CPPUNIT_ASSERT(true                                 == tArticle.mLinkStates[4]);
    CPPUNIT_ASSERT(GunnsFluidFlowOrchestrator::COMPLETE == tArticle.mNodeStates[0]);
    CPPUNIT_ASSERT(GunnsFluidFlowOrchestrator::COMPLETE == tArticle.mNodeStates[1]);
    CPPUNIT_ASSERT(GunnsFluidFlowOrchestrator::COMPLETE == tArticle.mNodeStates[2]);
    CPPUNIT_ASSERT(GunnsFluidFlowOrchestrator::COMPLETE == tArticle.mNodeStates[3]);

    /// - Test that we got overflows in the expected nodes.
    CPPUNIT_ASSERT(true                                 == tNodes[0].isOverflowing(dt));
    CPPUNIT_ASSERT(true                                 == tNodes[1].isOverflowing(dt));
    CPPUNIT_ASSERT(true                                 == tNodes[2].isOverflowing(dt));
    CPPUNIT_ASSERT(false                                == tNodes[3].isOverflowing(dt));

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the update method of the GunnsFluidFlowOrchestrator where it has to
///           escape an infinite loop.  This is the same as test 6 above except we force the nodes
///           completion flag to always fail, triggering the loop abort logic.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFlowOrchestrator::testUpdateAbort()
{
    std::cout << "\n UtGunnsFluidFlowOrchestrator 07: testUpdateAbort ...................";

    tArticle.initialize(tName, tLinksArray, tNodesArray, tLinkNodeMaps, tLinkNumPorts);

    const double dt = 0.1;

    /// - Set mass flow rate around the loop that overflows nodes 0 and 1.
    const double node0Mass = tNodes[0].getContent()->getMass();
    const double mdot = 2.0 * node0Mass / dt;

    tSource1.setFlowDemand(mdot);
    tSource2.setFlowDemand(mdot);
    tSource3.setFlowDemand(mdot);

    tSource1.step(dt);
    tSource2.step(dt);
    tSource3.step(dt);
    tCapacitor1.step(dt);
    tCapacitor2.step(dt);

    /// - Test the loop abort logic.
    tArticle.triggerAbort = true;
    CPPUNIT_ASSERT_THROW(tArticle.update(dt), TsOutOfBoundsException);

    std::cout << "... Pass";
}
