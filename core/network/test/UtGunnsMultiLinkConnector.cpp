/**
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
 ((core/GunnsMultiLinkConnector.o))
*/

#include "UtGunnsMultiLinkConnector.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

/// @details  Test identification number.
int UtGunnsMultiLinkConnector::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsMultiLinkConnector class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsMultiLinkConnector::UtGunnsMultiLinkConnector()
    :
    tArticle(),
    tNetwork("tNetwork"),
    tNodes(),
    tNodeList(),
    tJointType(),
    tName(""),
    tConfig(),
    tLink1(),
    tPort1(),
    tNode1(),
    tLink2(),
    tPort2(),
    tNode2(),
    tInput1(),
    tInput2()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsMultiLinkConnector class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsMultiLinkConnector::~UtGunnsMultiLinkConnector()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsMultiLinkConnector::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tInput2;
    delete tInput1;
    delete tConfig;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsMultiLinkConnector::setUp()
{
    tNodeList.mNumNodes = 3;
    tNodeList.mNodes    = tNodes;

    /// - Test spotter configuration.
    tName               = "tArticle";
    tConfig             = new GunnsMultiLinkConnectorConfigData(tName);

    tPort1              = 1;
    tNode1              = 0;
    tInput1             = new GunnsMultiLinkConnectorInputData(&tLink1, tPort1, tNode1);

    tPort2              = 0;
    tNode2              = 1;
    tInput2             = new GunnsMultiLinkConnectorInputData(&tLink2, tPort2, tNode2);

    /// - Create the test article.
    tJointType          = 1;
    tArticle            = new FriendlyGunnsMultiLinkConnector(&tNetwork, tJointType);

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the GunnsMultiLinkConnectorConfigData class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsMultiLinkConnector::testConfig()
{
    UT_RESULT_FIRST;

    /// - Test nominal config data construction.
    CPPUNIT_ASSERT(tName      == tConfig->mName);

    /// - Test default config data construction.
    GunnsMultiLinkConnectorConfigData defaultConfig(tName);
    CPPUNIT_ASSERT(tName      == defaultConfig.mName);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the GunnsMultiLinkConnectorInputData class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsMultiLinkConnector::testInput()
{
    UT_RESULT;

    /// - Test nominal input data construction.
    CPPUNIT_ASSERT(&tLink1 == tInput1->mLink);
    CPPUNIT_ASSERT(tPort1  == tInput1->mPort);
    CPPUNIT_ASSERT(tNode1  == tInput1->mNode);

    /// - Test default input data construction.
    GunnsMultiLinkConnectorInputData defaultInput;
    CPPUNIT_ASSERT(0       == defaultInput.mLink);
    CPPUNIT_ASSERT(0       == defaultInput.mPort);
    CPPUNIT_ASSERT(0       == defaultInput.mNode);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the default constructor of the GunnsMultiLinkConnector class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsMultiLinkConnector::testDefaultConstruction()
{
    UT_RESULT;

    /// @test state data
    CPPUNIT_ASSERT(""         == tArticle->mName);
    CPPUNIT_ASSERT(&tNetwork  == dynamic_cast<FriendlyGunnsNetworkBase*>(tArticle->getNetwork()));
    CPPUNIT_ASSERT(0          == tArticle->mJointNetwork);
    CPPUNIT_ASSERT(tJointType == tArticle->mJointType);
    CPPUNIT_ASSERT(0          == tArticle->mConnections.size());

    /// @test init flag
    CPPUNIT_ASSERT(false      == tArticle->mInitFlag);

    /// @test default construction without the joint type argument
    GunnsMultiLinkConnector article(&tNetwork);
    CPPUNIT_ASSERT(0          == article.getJointType());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the initialize method of the GunnsMultiLinkConnector class.  This
///           also tests its stepPostSolver method since it is called by initialize.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsMultiLinkConnector::testInitialize()
{
    UT_RESULT;

    tArticle->initialize(tConfig, tInput1);

    /// - Test nominal initialization.
    CPPUNIT_ASSERT(tName      == tArticle->mName);
    CPPUNIT_ASSERT(tJointType == tArticle->getJointType());

    /// @test init flag
    CPPUNIT_ASSERT(true       == tArticle->mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the initialize method of the GunnsMultiLinkConnector with errors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsMultiLinkConnector::testInitializeExceptions()
{
    UT_RESULT;

    /// - Test exception thrown from missing name.
    tConfig->mName = "";
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tConfig, tInput1), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle->mInitFlag);
    tConfig->mName = tName;

    /// - Test exception thrown from null config data.
    const GunnsMultiLinkConnectorConfigData* nullConfig = 0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(nullConfig, tInput1), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle->mInitFlag);

    /// - Test exception thrown from null input data.
    const GunnsMultiLinkConnectorInputData* nullInput = 0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tConfig, nullInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle->mInitFlag);

    /// - Test exception thrown on bad config data pointer type.
    BadGunnsMultiLinkConnectorConfigData* badConfig = new BadGunnsMultiLinkConnectorConfigData(tName);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(badConfig, tInput1), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle->mInitFlag);
    delete badConfig;

    /// - Test exception thrown on bad input data pointer type.
    BadGunnsMultiLinkConnectorInputData* badInput = new BadGunnsMultiLinkConnectorInputData();
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tConfig, badInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle->mInitFlag);
    delete badInput;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the stepPreSolver method of the GunnsMultiLinkConnector class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsMultiLinkConnector::testPreSolver()
{
    UT_RESULT;

    tArticle->initialize(tConfig, tInput1);
    CPPUNIT_ASSERT_NO_THROW(tArticle->stepPreSolver(0.0));

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the stepPostSolver method of the GunnsMultiLinkConnector class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsMultiLinkConnector::testPostSolver()
{
    UT_RESULT;

    tArticle->initialize(tConfig, tInput1);
    CPPUNIT_ASSERT_NO_THROW(tArticle->stepPostSolver(0.0));

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the addConnection method of the GunnsMultiLinkConnector class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsMultiLinkConnector::testAddConnections()
{
    UT_RESULT;

    tArticle->addConnection(&tLink1, tPort1, tNode1);
    CPPUNIT_ASSERT(1       == tArticle->mConnections.size());
    CPPUNIT_ASSERT(&tLink1 == tArticle->mConnections[0]->mLink);
    CPPUNIT_ASSERT(tPort1  == tArticle->mConnections[0]->mPort);
    CPPUNIT_ASSERT(tNode1  == tArticle->mConnections[0]->mNode);

    tArticle->addConnection(&tLink2, tPort2, tNode2);
    CPPUNIT_ASSERT(2       == tArticle->mConnections.size());
    CPPUNIT_ASSERT(&tLink2 == tArticle->mConnections[1]->mLink);
    CPPUNIT_ASSERT(tPort2  == tArticle->mConnections[1]->mPort);
    CPPUNIT_ASSERT(tNode2  == tArticle->mConnections[1]->mNode);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the connect method of the GunnsMultiLinkConnector class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsMultiLinkConnector::testConnect()
{
    UT_RESULT;

    tArticle->addConnection(&tLink1, tPort1, tNode1);
    tArticle->addConnection(&tLink2, tPort2, tNode2);

    /// - Initialize the test links.
    std::vector<GunnsBasicLink*> links;
    GunnsBasicConductorConfigData linkConfig("link", &tNodeList);
    GunnsBasicConductorInputData  linkInput;
    tLink1.initialize(linkConfig, linkInput, links, 2, 2);
    tLink2.initialize(linkConfig, linkInput, links, 2, 2);

    /// - Initialize the test article.
    tArticle->initialize(tConfig, tInput1);

    /// @test connect method with no node offset.
    CPPUNIT_ASSERT_NO_THROW(tArticle->connect(0));
    CPPUNIT_ASSERT(tNode1 == tLink1.getNodeMap()[tPort1]);
    CPPUNIT_ASSERT(tNode2 == tLink2.getNodeMap()[tPort2]);

    /// @test connect method with a node offset.
    CPPUNIT_ASSERT_NO_THROW(tArticle->connect(1));
    CPPUNIT_ASSERT(tNode1+1 == tLink1.getNodeMap()[tPort1]);
    CPPUNIT_ASSERT(tNode2+1 == tLink2.getNodeMap()[tPort2]);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the methods related to the joint network of the
///           GunnsMultiLinkConnector class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsMultiLinkConnector::testJointIndex()
{
    UT_RESULT;

    /// @test getJointIndex throws exception if there is no joint network.
    CPPUNIT_ASSERT_THROW(tArticle->getJointIndex(), TsInitializationException);

    /// @test setJointNetwork method.
    FriendlyGunnsNetworkBase network("network");
    tArticle->setJointNetwork(&network);
    CPPUNIT_ASSERT(&network == tArticle->mJointNetwork);

    /// @test getJointIndex throws exception if the joint network has negative joint index.
    CPPUNIT_ASSERT_THROW(tArticle->getJointIndex(), TsInitializationException);

    /// @test nominal result of getJointIndex.
    network.setJointIndex(2);
    CPPUNIT_ASSERT(2        == tArticle->getJointIndex());

    UT_PASS_LAST;
}
