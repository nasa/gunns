///@copyright Copyright 2019 United States Government as represented by the Administrator of the
///           National Aeronautics and Space Administration.  All Rights Reserved.

#include "core/network/GunnsBasicSuperNetwork.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsNetworkBase.hh"

/// @details  Test identification number.
int UtGunnsNetworkBase::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this test link.
////////////////////////////////////////////////////////////////////////////////////////////////////
FriendlyGunnsNetworkBaseLink::FriendlyGunnsNetworkBaseLink()
    :
    GunnsBasicLink(2),
    mInitCount(0),
    mRestartCount(0),
    mStepCount(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this test link.
////////////////////////////////////////////////////////////////////////////////////////////////////
FriendlyGunnsNetworkBaseLink::~FriendlyGunnsNetworkBaseLink()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) Reference to Link Config Data
/// @param[in]     inputData    (--) Reference to Link Input Data
/// @param[in,out] networkLinks (--) Reference to the Network Link Vector
/// @param[in]     port0        (--) Port 0 Mapping
/// @param[in]     port1        (--) Port 1 Mapping
///
/// @throws   TsInitializationException
///
/// @details  Increments a call count.  On specific calls, throws exceptions to test
///           handling by the test article.
////////////////////////////////////////////////////////////////////////////////////////////////////
void FriendlyGunnsNetworkBaseLink::initialize(const GunnsBasicLinkConfigData& configData,
                                              const GunnsBasicLinkInputData&  inputData,
                                              std::vector<GunnsBasicLink*>&   networkLinks,
                                              const int                       port0,
                                              const int                       port1)
{
    mInitCount++;
    if (1 == mInitCount) {
        int ports[2] = {port0, port1};
        GunnsBasicLink::initialize(configData, inputData, networkLinks, ports);
    } else if (2 == mInitCount) {
        GUNNS_ERROR(TsInitializationException, "test", "test");
    } else if (3 == mInitCount) {
        std::exception e;
        throw e;
    } else if (4  == mInitCount) {
        throw 2;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt (s) Integration time step (not used).
///
/// @throws  TsInitializationException, 2
///
/// @details  Increments a call count.  On specific calls, throws exceptions to test
///           handling by the test article.
////////////////////////////////////////////////////////////////////////////////////////////////////
void FriendlyGunnsNetworkBaseLink::step(const double dt __attribute__((unused)))
{
    mStepCount++;
    if (2 == mStepCount) {
        GUNNS_ERROR(TsInitializationException, "test", "test");
    } else if (3 == mStepCount) {
        throw 2;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws  TsInitializationException
///
/// @details  Increments a call count.  On specific calls, throws exceptions to test
///           handling by the test article.
////////////////////////////////////////////////////////////////////////////////////////////////////
void FriendlyGunnsNetworkBaseLink::restartModel()
{
    mRestartCount++;
    if (2 == mRestartCount) {
        GUNNS_ERROR(TsInitializationException, "test", "test");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name (--) Instance name for H&S messages.
///
/// @details  Default constructs this test article.
////////////////////////////////////////////////////////////////////////////////////////////////////
FriendlyGunnsNetworkBase::FriendlyGunnsNetworkBase(const std::string& name)
    :
    GunnsNetworkBase(name, N_NODES, netNodes),
    netNodes(),
    mLink(),
    mInitNodesCount(0),
    mInitNetworkCount(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this test article.
////////////////////////////////////////////////////////////////////////////////////////////////////
FriendlyGunnsNetworkBase::~FriendlyGunnsNetworkBase()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name (--) Instance name for H&S messages.
///
/// @details  Implements the pure virtual base method for testability.  This mimics the recommended
///           functions of a real network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void FriendlyGunnsNetworkBase::initNodes(const std::string& name)
{
    mInitNodesCount++;
    netNodeList.mNodes[NODE_0 + netSuperNodesOffset].initialize(name + createNodeName(NODE_0 + netSuperNodesOffset), 294.261);
    netNodeList.mNodes[NODE_1 + netSuperNodesOffset].initialize(name + createNodeName(NODE_1 + netSuperNodesOffset), 294.261);
    netNodeList.mNodes[NODE_2 + netSuperNodesOffset].initialize(name + createNodeName(NODE_2 + netSuperNodesOffset), 294.261);
    if (!netIsSubNetwork) {
        netNodeList.mNodes[GROUND].initialize(name + ".GROUND", 0.0);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Implements the pure virtual base method for testability.  This mimics some of the
///           recommended functions of a real network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void FriendlyGunnsNetworkBase::initNetwork()
{
    mInitNetworkCount++;
    if (2 == mInitNetworkCount) {
        GUNNS_ERROR(TsInitializationException, "test", "test");
    } else if (3 == mInitNetworkCount) {
        std::exception e;
        throw e;
    } else if (4  == mInitNetworkCount) {
        throw 2;
    }

    GunnsBasicLinkConfigData linkConfig(mName + ".mLink", &netNodeList);
    GunnsBasicLinkInputData  linkInput(false, 0.0);
    mLink.initialize(linkConfig, linkInput, netLinks, 0+netSuperNodesOffset, 1+netSuperNodesOffset);

    GunnsConfigData solverConfig(mName + ".netSolver", 1.0, 1.0, 1, 1);
    if (!netIsSubNetwork) {
        netSolver.initializeNodes(netNodeList);
        netSolver.initialize(solverConfig, netLinks);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS network base class unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsNetworkBase::UtGunnsNetworkBase()
    :
    CppUnit::TestFixture(),
    tName(""),
    tArticle(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS network base class unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsNetworkBase::~UtGunnsNetworkBase()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsNetworkBase::setUp()
{
    /// - Default construct the nominal test article.
    tName    = "tArticle";
    tArticle = new FriendlyGunnsNetworkBase(tName);

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsNetworkBase::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete tArticle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS network base class default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsNetworkBase::testDefaultConstruction()
{
    UT_RESULT_INITIAL("GUNNS Generic Network Classes");

    /// @test Default construction values.
    CPPUNIT_ASSERT(FriendlyGunnsNetworkBase::N_NODES == tArticle->netNodeList.mNumNodes);
    CPPUNIT_ASSERT(tArticle->netNodes                == tArticle->netNodeList.mNodes);
    CPPUNIT_ASSERT(0                                 == tArticle->netLinks.size());
    CPPUNIT_ASSERT(tName                             == tArticle->mName);
    CPPUNIT_ASSERT(false                             == tArticle->netIsSubNetwork);
    CPPUNIT_ASSERT(0                                 == tArticle->netSuperNodesOffset);
    CPPUNIT_ASSERT(-1                                == tArticle->netJointIndex);
    CPPUNIT_ASSERT(FriendlyGunnsNetworkBase::N_NODES == tArticle->netNumLocalNodes);
    CPPUNIT_ASSERT(0                                 == tArticle->netSuperNetwork);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS network base class sub-network setup methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsNetworkBase::testSubNetworkSetup()
{
    UT_RESULT;

    /// @test setNodeOffset method.
    const int offset = 2;
    GunnsBasicSuperNetwork superNetwork("superNetwork");
    tArticle->setNodeOffset(offset, &superNetwork);
    CPPUNIT_ASSERT(offset              == tArticle->netSuperNodesOffset);
    CPPUNIT_ASSERT(&superNetwork       == tArticle->netSuperNetwork);

    /// @test getNodeOffset method.
    CPPUNIT_ASSERT(offset              == tArticle->getNodeOffset());

    /// @test getSuperNetwork method.
    CPPUNIT_ASSERT(&superNetwork       == tArticle->getSuperNetwork());

    /// @test getLinks throws exception before setNodeList has been called.
    CPPUNIT_ASSERT_THROW(tArticle->getLinks(), TsInitializationException);

    /// @test setNodeList method.
    const int superNodes = 10;
    GunnsBasicNode nodes[superNodes];
    GunnsNodeList nodeList(superNodes, nodes);
    tArticle->setNodeList(nodeList);
    CPPUNIT_ASSERT(superNodes          == tArticle->netNodeList.mNumNodes);
    CPPUNIT_ASSERT(nodes               == tArticle->netNodeList.mNodes);
    CPPUNIT_ASSERT(true                == tArticle->netIsSubNetwork);

    /// @test getNumLocalNodes method.
    CPPUNIT_ASSERT(FriendlyGunnsNetworkBase::N_NODES == tArticle->getNumLocalNodes());

    /// @test getLinks method in a sub-network.
    CPPUNIT_ASSERT(&tArticle->netLinks == tArticle->getLinks());

    /// @test getFluidConfig method.
    CPPUNIT_ASSERT(0                   == tArticle->getFluidConfig());

    /// @test getJointIndex method.
    CPPUNIT_ASSERT(-1                  == tArticle->getJointIndex());

    /// @test setJointIndex method.
    tArticle->setJointIndex(2);
    CPPUNIT_ASSERT(2                   == tArticle->netJointIndex);
    CPPUNIT_ASSERT(2                   == tArticle->getJointIndex());

    /// @test getName method.
    CPPUNIT_ASSERT("tArticle"          == tArticle->getName());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS network base class initialize method when a sub-network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsNetworkBase::testInitSubNetwork()
{
    UT_RESULT;

    /// @test initialize method does nothing when a sub-network.
    const int offset = 2;
    GunnsBasicSuperNetwork superNetwork("superNetwork");
    tArticle->setNodeOffset(offset, &superNetwork);

    const int superNodes = 10;
    GunnsBasicNode nodes[superNodes];
    GunnsNodeList nodeList(superNodes, nodes);
    tArticle->setNodeList(nodeList);

    tArticle->initialize();
    CPPUNIT_ASSERT(0 == tArticle->mInitNodesCount);
    CPPUNIT_ASSERT(0 == tArticle->mInitNetworkCount);

    /// @test mutex initializationa: the network should leave the mutex unlocked after unit.
    CPPUNIT_ASSERT(0 == pthread_mutex_trylock(&tArticle->netMutex));
    pthread_mutex_unlock(&tArticle->netMutex);

    /// @test initNodes and initNetwork methods when called directly.
    tArticle->initNodes("super");
    CPPUNIT_ASSERT(std::string("")             == nodes[0].getName());
    CPPUNIT_ASSERT(std::string("")             == nodes[1].getName());
    CPPUNIT_ASSERT(std::string("super.Node_2") == nodes[2].getName());
    CPPUNIT_ASSERT(std::string("super.Node_3") == nodes[3].getName());
    CPPUNIT_ASSERT(std::string("super.Node_4") == nodes[4].getName());
    CPPUNIT_ASSERT(std::string("")             == nodes[5].getName());
    CPPUNIT_ASSERT_NO_THROW(tArticle->initNetwork());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS network base class initialize method when a standalone network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsNetworkBase::testInitStandalone()
{
    UT_RESULT;

    /// @test initialize method when a standalone network.
    tArticle->initialize();
    CPPUNIT_ASSERT(1 == tArticle->mInitNodesCount);
    CPPUNIT_ASSERT(1 == tArticle->mInitNetworkCount);

    /// @test mutex initializationa: the network should leave the mutex unlocked after unit.
    CPPUNIT_ASSERT(0 == pthread_mutex_trylock(&tArticle->netMutex));
    pthread_mutex_unlock(&tArticle->netMutex);

    /// @test initialize method handles TsInitializationException.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize());

    /// @test initialize method handles std::exception.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize());

    /// @test initialize method handles any random throw.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS network base class restart method when a sub-network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsNetworkBase::testRestartSubNetwork()
{
    UT_RESULT;

    tArticle->initialize();
    CPPUNIT_ASSERT_NO_THROW(tArticle->update(1.0));
    CPPUNIT_ASSERT_EQUAL(1, tArticle->netSolver.getMajorStepCount());

    /// @test restart method does nothing when a sub-network.
    tArticle->netIsSubNetwork = true;
    tArticle->restart();
    CPPUNIT_ASSERT_EQUAL(1, tArticle->netSolver.getMajorStepCount());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS network base class restart method when a standalone network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsNetworkBase::testRestartStandalone()
{
    UT_RESULT;

    /// @test restart method restarts the solver & links when a standalone network.
    tArticle->initialize();
    tArticle->restart();
    CPPUNIT_ASSERT_EQUAL(1, tArticle->mLink.mRestartCount);

    /// @test restart method handles std::exception.
    CPPUNIT_ASSERT_NO_THROW(tArticle->restart());

    /// @test restart method handles any random throw.
    CPPUNIT_ASSERT_NO_THROW(tArticle->restart());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS network base class update method when a sub-network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsNetworkBase::testUpdateSubNetwork()
{
    UT_RESULT;

    /// @test update method does nothing when a sub-network.
    tArticle->initialize();
    tArticle->netIsSubNetwork = true;
    CPPUNIT_ASSERT_NO_THROW(tArticle->update(1.0));
    CPPUNIT_ASSERT_EQUAL(0, tArticle->mLink.mStepCount);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS network base class update method when a standalone network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsNetworkBase::testUpdateStandalone()
{
    UT_RESULT;

    /// @test update method updates the spotters and solver when a standalone network.
    tArticle->initialize();
    CPPUNIT_ASSERT_NO_THROW(tArticle->update(1.0));
    CPPUNIT_ASSERT_EQUAL(1, tArticle->mLink.mStepCount);

    /// @test update method handles std::exception.
    CPPUNIT_ASSERT_NO_THROW(tArticle->update(1.0));

    /// @test update method handles any random throw.
    /// @test updating with mutex locking enabled and verify network leaves it unlocked when
    ///       finished.
    tArticle->setMutexEnabled(true);
    CPPUNIT_ASSERT_NO_THROW(tArticle->update(1.0));
    pthread_mutex_t* mutex = tArticle->getMutex();
    CPPUNIT_ASSERT(0 == pthread_mutex_trylock(mutex));
    pthread_mutex_unlock(mutex);

    UT_PASS_LAST;
}
