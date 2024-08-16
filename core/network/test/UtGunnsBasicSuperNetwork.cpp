/**
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
 ((core/network/GunnsBasicSuperNetwork.o))
*/

#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsBasicSuperNetwork.hh"

/// @details  Test identification number.
int UtGunnsBasicSuperNetwork::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] type (--) Records the given type argument for verification
///
/// @details  A custom implementation for the unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsNetworkBase* FriendlyGunnsBasicSuperNetworkWithJoints::createJointNetwork(const int type)
{
    tType = type;
    return new FriendlyGunnsNetworkBase("test");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  A custom implementation for the unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void FriendlyGunnsBasicSuperNetworkWithJoints::registerJoints()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS network base class unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsBasicSuperNetwork::UtGunnsBasicSuperNetwork()
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
UtGunnsBasicSuperNetwork::~UtGunnsBasicSuperNetwork()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSuperNetwork::setUp()
{
    /// - Default construct the nominal test article.
    tName    = "tArticle";
    tArticle = new FriendlyGunnsBasicSuperNetwork(tName);

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSuperNetwork::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete tArticle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS network base class default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSuperNetwork::testDefaultConstruction()
{
    UT_RESULT_FIRST;

    /// @test Default construction values.
    CPPUNIT_ASSERT(0                    == tArticle->netNodes);
    CPPUNIT_ASSERT(0                    == tArticle->netSolver.getPotentialVector());
    CPPUNIT_ASSERT("tArticle.netSolver" == tArticle->netSolverConfig.mName);
    CPPUNIT_ASSERT(1.0e-6               == tArticle->netSolverConfig.mConvergenceTolerance);
    CPPUNIT_ASSERT(1.0e-6               == tArticle->netSolverConfig.mMinLinearizationPotential);
    CPPUNIT_ASSERT(1                    == tArticle->netSolverConfig.mMinorStepLimit);
    CPPUNIT_ASSERT(1                    == tArticle->netSolverConfig.mDecompositionLimit);
    CPPUNIT_ASSERT(0                    == tArticle->netNodeList.mNumNodes);
    CPPUNIT_ASSERT(0                    == tArticle->netNodeList.mNodes);
    CPPUNIT_ASSERT(0                    == tArticle->mLinks.size());
    CPPUNIT_ASSERT(0                    == tArticle->mSubnets.size());
    CPPUNIT_ASSERT(false                == tArticle->netMutexEnabled);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS super network base class addSubNetwork method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSuperNetwork::testAddSubNetwork()
{
    UT_RESULT;

    /// @test handles exception from calling with null sub-network argument.
    GunnsNetworkBase* sub = 0;
    CPPUNIT_ASSERT_NO_THROW(tArticle->addSubNetwork(sub));

    /// @test adding the first network to the super.
    tArticle->addSubNetwork(&tSubNetworkA);
    CPPUNIT_ASSERT(1                                  == tArticle->mSubnets.size());
    CPPUNIT_ASSERT(&tSubNetworkA                      == tArticle->mSubnets[0]);
    CPPUNIT_ASSERT(0                                  == tSubNetworkA.netSuperNodesOffset);
    CPPUNIT_ASSERT(FriendlyGunnsNetworkBase::GROUND   == tArticle->netNodeList.mNumNodes);

    /// @test adding the second network to the super.
    tArticle->addSubNetwork(&tSubNetworkB);
    CPPUNIT_ASSERT(2                                  == tArticle->mSubnets.size());
    CPPUNIT_ASSERT(&tSubNetworkB                      == tArticle->mSubnets[1]);
    CPPUNIT_ASSERT(3                                  == tSubNetworkB.netSuperNodesOffset);
    CPPUNIT_ASSERT(2*FriendlyGunnsNetworkBase::GROUND == tArticle->netNodeList.mNumNodes);

    /// @test re-adding the first network.
    tArticle->addSubNetwork(&tSubNetworkA);
    CPPUNIT_ASSERT(2                                  == tArticle->mSubnets.size());
    CPPUNIT_ASSERT(&tSubNetworkA                      == tArticle->mSubnets[0]);
    CPPUNIT_ASSERT(&tSubNetworkB                      == tArticle->mSubnets[1]);
    CPPUNIT_ASSERT(0                                  == tSubNetworkA.netSuperNodesOffset);
    CPPUNIT_ASSERT(3                                  == tSubNetworkB.netSuperNodesOffset);
    CPPUNIT_ASSERT(2*FriendlyGunnsNetworkBase::GROUND == tArticle->netNodeList.mNumNodes);

    /// @test handles exception from adding a sub-network after register nodes.
    tArticle->registerSuperNodes();
    CPPUNIT_ASSERT_NO_THROW(tArticle->addSubNetwork(&tSubNetworkA));

    /// @test getSubnets method.
    CPPUNIT_ASSERT(&tArticle->mSubnets                == &tArticle->getSubnets());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS super network base class addSubNetwork overloaded method for adding
///           a super-network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSuperNetwork::testAddSuperNetwork()
{
    UT_RESULT;

    FriendlyGunnsBasicSuperNetwork superSuperArticle("superSuperArticle");

    /// @test adding a super-network to another super-network.
    tArticle->addSubNetwork(&tSubNetworkA);
    tArticle->addSubNetwork(&tSubNetworkB);
    tArticle->registerSuperNodes();

    /// @test adding a sub-network that is already in another super-network.
    FriendlyGunnsBasicSuperNetwork anotherSuper("anotherSuper");
    FriendlyGunnsNetworkBase       anotherSub("anotherSub");
    anotherSuper.addSubNetwork(&anotherSub);

    CPPUNIT_ASSERT_NO_THROW(superSuperArticle.addSubNetwork(tArticle));
    CPPUNIT_ASSERT_NO_THROW(superSuperArticle.addSubNetwork(&anotherSub));
    CPPUNIT_ASSERT_NO_THROW(superSuperArticle.registerSuperNodes());

    CPPUNIT_ASSERT(3                                  == superSuperArticle.mSubnets.size());
    CPPUNIT_ASSERT(0                                  == tArticle->mSubnets.size());
    CPPUNIT_ASSERT(0                                  == anotherSuper.mSubnets.size());

    //// @test handles exception from adding a super-network after register nodes.
    CPPUNIT_ASSERT_NO_THROW(superSuperArticle.addSubNetwork(tArticle));

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS super network base class registerSuperNodes method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSuperNetwork::testRegisterSuperNodes()
{
    UT_RESULT;

    /// @test results of the registerSuperNodes method.
    tArticle->addSubNetwork(&tSubNetworkA);
    tArticle->addSubNetwork(&tSubNetworkB);
    tArticle->registerSuperNodes();

    CPPUNIT_ASSERT(2*FriendlyGunnsNetworkBase::GROUND+1 == tArticle->netNodeList.mNumNodes);
    CPPUNIT_ASSERT(2*FriendlyGunnsNetworkBase::GROUND+1 == tSubNetworkA.netNodeList.mNumNodes);
    CPPUNIT_ASSERT(2*FriendlyGunnsNetworkBase::GROUND+1 == tSubNetworkB.netNodeList.mNumNodes);
    CPPUNIT_ASSERT(0                                    != tArticle->netNodeList.mNodes);
    CPPUNIT_ASSERT(tArticle->netNodeList.mNodes         == tSubNetworkA.netNodeList.mNodes);
    CPPUNIT_ASSERT(tArticle->netNodeList.mNodes         == tSubNetworkB.netNodeList.mNodes);

    /// @test handles attempting to register twice.
    CPPUNIT_ASSERT_NO_THROW(tArticle->registerSuperNodes());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS super network base class initialize method with sub-networks.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSuperNetwork::testInitialize()
{
    UT_RESULT;

    /// - Set up subnetworks.
    tArticle->addSubNetwork(&tSubNetworkA);
    tArticle->addSubNetwork(&tSubNetworkB);
    tArticle->registerSuperNodes();

    /// @test results of super-network and sub-networks initialization.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize());

    /// @test nodes initialization.
    CPPUNIT_ASSERT_EQUAL(294.261, tArticle->netNodes[0].getPotential());
    CPPUNIT_ASSERT_EQUAL(294.261, tArticle->netNodes[1].getPotential());
    CPPUNIT_ASSERT_EQUAL(294.261, tArticle->netNodes[2].getPotential());
    CPPUNIT_ASSERT_EQUAL(294.261, tArticle->netNodes[3].getPotential());
    CPPUNIT_ASSERT_EQUAL(294.261, tArticle->netNodes[4].getPotential());
    CPPUNIT_ASSERT_EQUAL(294.261, tArticle->netNodes[5].getPotential());
    CPPUNIT_ASSERT_EQUAL(0.0,     tArticle->netNodes[6].getPotential());

    CPPUNIT_ASSERT(std::string("tArticle.Node_0") == tArticle->netNodes[0].getName());
    CPPUNIT_ASSERT(std::string("tArticle.Node_1") == tArticle->netNodes[1].getName());
    CPPUNIT_ASSERT(std::string("tArticle.Node_2") == tArticle->netNodes[2].getName());
    CPPUNIT_ASSERT(std::string("tArticle.Node_3") == tArticle->netNodes[3].getName());
    CPPUNIT_ASSERT(std::string("tArticle.Node_4") == tArticle->netNodes[4].getName());
    CPPUNIT_ASSERT(std::string("tArticle.Node_5") == tArticle->netNodes[5].getName());
    CPPUNIT_ASSERT(std::string("tArticle.GROUND") == tArticle->netNodes[6].getName());

    /// @test links initialization.
    CPPUNIT_ASSERT(2                              == tArticle->mLinks.size());
    CPPUNIT_ASSERT_EQUAL(0,       tSubNetworkA.mLink.getNodeMap()[0]);
    CPPUNIT_ASSERT_EQUAL(1,       tSubNetworkA.mLink.getNodeMap()[1]);
    CPPUNIT_ASSERT_EQUAL(3,       tSubNetworkB.mLink.getNodeMap()[0]);
    CPPUNIT_ASSERT_EQUAL(4,       tSubNetworkB.mLink.getNodeMap()[1]);

    /// @test solver initialization.
    CPPUNIT_ASSERT_EQUAL(294.261, tArticle->netSolver.getPotentialVector()[0]);
    CPPUNIT_ASSERT_EQUAL(294.261, tArticle->netSolver.getPotentialVector()[1]);
    CPPUNIT_ASSERT_EQUAL(294.261, tArticle->netSolver.getPotentialVector()[2]);
    CPPUNIT_ASSERT_EQUAL(294.261, tArticle->netSolver.getPotentialVector()[3]);
    CPPUNIT_ASSERT_EQUAL(294.261, tArticle->netSolver.getPotentialVector()[4]);
    CPPUNIT_ASSERT_EQUAL(294.261, tArticle->netSolver.getPotentialVector()[5]);

    /// @test mutex initializationa: the network should leave the mutex unlocked after unit.
    CPPUNIT_ASSERT(0 == pthread_mutex_trylock(&tArticle->netMutex));
    pthread_mutex_unlock(&tArticle->netMutex);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS super network base class initialize method error handling.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSuperNetwork::testInitializeErrors()
{
    UT_RESULT;

    /// - Set up subnetworks.
    tArticle->addSubNetwork(&tSubNetworkA);
    tArticle->addSubNetwork(&tSubNetworkB);

    /// @test init does nothing if the super-network nodes have not yet been registered.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize());
    CPPUNIT_ASSERT_EQUAL(0, tSubNetworkA.mLink.mInitCount);

    /// - Do a first nominal init.
    tArticle->registerSuperNodes();
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize());
    CPPUNIT_ASSERT_EQUAL(1, tSubNetworkA.mLink.mInitCount);
    CPPUNIT_ASSERT_EQUAL(1, tSubNetworkA.mInitNetworkCount);

    /// @test super-network catches initialization exceptions.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize());
    CPPUNIT_ASSERT_EQUAL(2, tSubNetworkA.mInitNetworkCount);

    /// @test super-network catches std::exception.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize());
    CPPUNIT_ASSERT_EQUAL(3, tSubNetworkA.mInitNetworkCount);

    /// @test super-network catches any random throw.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize());
    CPPUNIT_ASSERT_EQUAL(4, tSubNetworkA.mInitNetworkCount);

    /// @test createJointNetwork throws exception.
    CPPUNIT_ASSERT_THROW(tArticle->createJointNetwork(0), TsInitializationException);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS super network base class initialize method error handling when there
///           is a duplicated sub-network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSuperNetwork::testInitializeDuplicateSub()
{
    UT_RESULT;

    /// - Set up subnetworks with one of them added twice.
    tArticle->addSubNetwork(&tSubNetworkA);
    tArticle->addSubNetwork(&tSubNetworkB);
    tArticle->addSubNetwork(&tSubNetworkB);
    tArticle->registerSuperNodes();

    /// @test super-network catches duplicated network.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize());
    CPPUNIT_ASSERT_EQUAL(1, tSubNetworkA.mInitNetworkCount);
    CPPUNIT_ASSERT_EQUAL(1, tSubNetworkB.mInitNetworkCount);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS super network base class initialize method with no sub-networks.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSuperNetwork::testInitializeEmpty()
{
    UT_RESULT;

    /// @test init does nothing if there are no subnetworks.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize());
    CPPUNIT_ASSERT_EQUAL(0, tSubNetworkA.mLink.mInitCount);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS super network base class restart method with sub-networks.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSuperNetwork::testRestart()
{
    UT_RESULT;

    /// - Set up subnetworks & init.
    tArticle->addSubNetwork(&tSubNetworkA);
    tArticle->addSubNetwork(&tSubNetworkB);
    tArticle->registerSuperNodes();
    tArticle->initialize();

    /// @test restart method restarts the solver & links.
    tArticle->restart();
    CPPUNIT_ASSERT_EQUAL(1, tSubNetworkA.mLink.mRestartCount);
    CPPUNIT_ASSERT_EQUAL(1, tSubNetworkB.mLink.mRestartCount);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS super network base class restart method error handling.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSuperNetwork::testRestartErrors()
{
    UT_RESULT;

    /// - Set up subnetworks & init.
    tArticle->addSubNetwork(&tSubNetworkA);
    tArticle->addSubNetwork(&tSubNetworkB);
    tArticle->registerSuperNodes();
    tArticle->initialize();

    /// @test restart method handles std::exception.
    CPPUNIT_ASSERT_NO_THROW(tArticle->restart());
    CPPUNIT_ASSERT_NO_THROW(tArticle->restart());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS super network base class restart with no sub-networks.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSuperNetwork::testRestartEmpty()
{
    UT_RESULT;

    /// - Init with no sub-networks.
    tArticle->initialize();

    /// @test restart method does nothing if there are no sub-networks.
    CPPUNIT_ASSERT_NO_THROW(tArticle->restart());
    CPPUNIT_ASSERT_EQUAL(0, tSubNetworkA.mLink.mRestartCount);
    CPPUNIT_ASSERT_EQUAL(0, tSubNetworkB.mLink.mRestartCount);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS super network base class update method with sub-networks.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSuperNetwork::testUpdate()
{
    UT_RESULT;

    /// - Set up subnetworks & init.
    tArticle->addSubNetwork(&tSubNetworkA);
    tArticle->addSubNetwork(&tSubNetworkB);
    tArticle->registerSuperNodes();
    tArticle->initialize();

    /// @test update method cycles the spotters and solver.
    CPPUNIT_ASSERT_NO_THROW(tArticle->update(1.0));
    CPPUNIT_ASSERT_EQUAL(1, tSubNetworkA.mLink.mStepCount);
    CPPUNIT_ASSERT_EQUAL(1, tSubNetworkB.mLink.mStepCount);

    /// @test updating with mutex locking enabled and verify network leaves it unlocked when
    ///       finished.
    tArticle->setMutexEnabled(true);
    CPPUNIT_ASSERT_NO_THROW(tArticle->update(1.0));
    CPPUNIT_ASSERT_EQUAL(2, tSubNetworkA.mLink.mStepCount);

    pthread_mutex_t* mutex = tArticle->getMutex();
    CPPUNIT_ASSERT(0 == pthread_mutex_trylock(mutex));
    pthread_mutex_unlock(mutex);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS super network base class update method error handling.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSuperNetwork::testUpdateErrors()
{
    UT_RESULT;

    /// - Set up subnetworks, init & first nominal update.
    tArticle->addSubNetwork(&tSubNetworkA);
    tArticle->addSubNetwork(&tSubNetworkB);
    tArticle->registerSuperNodes();
    tArticle->initialize();
    tArticle->update(1.0);

    /// @test update method handles std::exception.
    CPPUNIT_ASSERT_NO_THROW(tArticle->update(1.0));

    /// @test update method handles any random throw.
    CPPUNIT_ASSERT_NO_THROW(tArticle->update(1.0));

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS super network base class update method with no sub-networks.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSuperNetwork::testUpdateEmpty()
{
    UT_RESULT;

    /// - Init with no sub-networks.
    tArticle->initialize();

    /// @test update method does nothing if there are no sub-networks.
    CPPUNIT_ASSERT_NO_THROW(tArticle->update(1.0));
    CPPUNIT_ASSERT_EQUAL(0, tSubNetworkA.mLink.mRestartCount);
    CPPUNIT_ASSERT_EQUAL(0, tSubNetworkB.mLink.mRestartCount);

    UT_PASS;
}

#include "UtGunnsMultiLinkConnector.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS super network base class joinLocations method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSuperNetwork::testJoinLocations()
{
    UT_RESULT;

    /// - Set up test sub-network and connector spotters.
    FriendlyGunnsNetworkBase network("network");
    GunnsMultiLinkConnector  location1(&network, 1);
    GunnsMultiLinkConnector  location2(&network, 1);
    GunnsMultiLinkConnector  location3(&network, 1);
    GunnsMultiLinkConnector  location4(&network, 1);
    GunnsMultiLinkConnector  location5(&network, 0);
    FriendlyGunnsBasicSuperNetworkWithJoints article("article");

    /// @test throw for conflicting joint types.
    CPPUNIT_ASSERT_THROW(article.joinLocations(&location1, &location5), TsInitializationException);
    CPPUNIT_ASSERT(0            == article.mJoints.size());

    /// @test nominal joining of 2 locations via a joint network.
    CPPUNIT_ASSERT_NO_THROW(article.joinLocations(&location1, &location2));
    CPPUNIT_ASSERT(1            == article.mJoints.size());
    CPPUNIT_ASSERT(0            != article.mJoints[0]->network);
    CPPUNIT_ASSERT(&location1   == article.mJoints[0]->location1);
    CPPUNIT_ASSERT(&location2   == article.mJoints[0]->location2);

    /// @test nominal joining of 3rd location to 1st location.
    CPPUNIT_ASSERT_NO_THROW(article.joinLocations(&location3, &location1));
    CPPUNIT_ASSERT(2            == article.mJoints.size());
    CPPUNIT_ASSERT(article.mJoints[0]->network == article.mJoints[1]->network);
    CPPUNIT_ASSERT(&location3   == article.mJoints[1]->location1);
    CPPUNIT_ASSERT(&location1   == article.mJoints[1]->location2);

    /// @test nominal joining of 4th location to 2nd location.
    CPPUNIT_ASSERT_NO_THROW(article.joinLocations(&location2, &location4));
    CPPUNIT_ASSERT(3            == article.mJoints.size());
    CPPUNIT_ASSERT(article.mJoints[0]->network == article.mJoints[2]->network);
    CPPUNIT_ASSERT(&location2   == article.mJoints[2]->location1);
    CPPUNIT_ASSERT(&location4   == article.mJoints[2]->location2);

    /// @test throw for both locations already added.
    CPPUNIT_ASSERT_THROW(article.joinLocations(&location1, &location4), TsInitializationException);

    /// - Set up subnetworks.
    article.addSubNetwork(&tSubNetworkA);
    article.addSubNetwork(&tSubNetworkB);
    article.registerSuperNodes();

    /// @test results of super-network and sub-networks initialization.
    CPPUNIT_ASSERT_NO_THROW(article.initialize());

    /// @test create a GunnSubNetworkJoint for constructor code coverage.
    GunnsSubNetworkJoint joint(0, 0, 0);

    /// - Call GunnsBasicSuperNetwork::registerJoints for code coverage.
    FriendlyGunnsBasicSuperNetwork article2("article2");
    article2.registerJoints();

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS super network base class getJointIndex method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSuperNetwork::testGetJointIndex()
{
    UT_RESULT;

    /// - Set up test sub-network and connector spotters.
    FriendlyGunnsNetworkBase network("network");
    GunnsMultiLinkConnector  location1(&network, 1);
    GunnsMultiLinkConnector  location2(&network, 1);
    FriendlyGunnsBasicSuperNetworkWithJoints article("article");
    CPPUNIT_ASSERT_NO_THROW(article.joinLocations(&location1, &location2));
    article.mJointNetworks[0]->setJointIndex(2);

    /// - Initialize the connector spotters.
    GunnsMultiLinkConnectorConfigData location1Config("location1");
    GunnsMultiLinkConnectorConfigData location2Config("location2");
    GunnsMultiLinkConnectorInputData  locationsInput;
    location1.initialize(&location1Config, &locationsInput);
    location2.initialize(&location2Config, &locationsInput);
    location1.setJointNetwork(article.mJointNetworks[0]);
    location2.setJointNetwork(article.mJointNetworks[0]);

    /// @test getJointIndex adds map keys for new connector names.
    CPPUNIT_ASSERT(2            == article.getJointIndex("location1"));
    CPPUNIT_ASSERT(1            == article.mJointsIndeces.size());
    CPPUNIT_ASSERT(2            == article.getJointIndex("location2"));
    CPPUNIT_ASSERT(2            == article.mJointsIndeces.size());

    /// @test getJointIndex doesn't add map keys for connector names already in the map.
    CPPUNIT_ASSERT(2            == article.getJointIndex("location2"));
    CPPUNIT_ASSERT(2            == article.mJointsIndeces.size());

    /// @test getJointIndex throws exception if connect name not found.
    CPPUNIT_ASSERT_THROW(article.getJointIndex("location3"), TsOutOfBoundsException);

    UT_PASS_LAST;
}
