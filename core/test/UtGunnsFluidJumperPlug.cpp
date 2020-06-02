/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((core/GunnsFluidJumperPlug.o))
***************************************************************************************************/
#include "UtGunnsFluidJumperPlug.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsFluidJumperPlug class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidJumperPlug::UtGunnsFluidJumperPlug()
    :
    tName(),
    tArticle(),
    tNodes(),
    tNodeList(),
    tLinks(),
    tSocket1(),
    tSocket2(),
    tSocket3()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsFluidJumperPlug class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidJumperPlug::~UtGunnsFluidJumperPlug()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidJumperPlug::tearDown()
{
    /// - Deletes for new's in setUp
    delete tArticle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidJumperPlug::setUp()
{
    /// - Create the test article
    tName = "Test Article";
    tArticle = new FriendlyGunnsFluidJumperPlug(tName);
    tNodeList.mNumNodes = 3;
    tNodeList.mNodes    = tNodes;

    /// - Initialize the sockets
    GunnsFluidSocketConfigData socketConfig("Test Socket", &tNodeList, 0.0, false);
    GunnsFluidSocketInputData  socketInput (false, 0.0);

    tSocket1.initialize(socketConfig, socketInput, tLinks, 0, 2);
    tSocket2.initialize(socketConfig, socketInput, tLinks, 1, 2);
    tSocket3.initialize(socketConfig, socketInput, tLinks, 0, 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the constructor methods of the Fluid Jumper Plug class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidJumperPlug::testConstructors()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsFluidJumperPlug . 01: testConstructors ......................";

    /// - Check default construction
    CPPUNIT_ASSERT(tName == tArticle->mName);
    CPPUNIT_ASSERT(0     != tArticle->mSockets);
    CPPUNIT_ASSERT(0     == tArticle->mNumSockets);
    CPPUNIT_ASSERT(0     == tArticle->mConnectedNode);

    /// - Verify allocation of sockets array.
    GunnsBasicLink** expectedAddress = &tArticle->mSockets[0] + (FriendlyGunnsFluidJumperPlug::NONE - 1);
    GunnsBasicLink** actualAddress   = &tArticle->mSockets[FriendlyGunnsFluidJumperPlug::NONE - 1];
    CPPUNIT_ASSERT(expectedAddress == actualAddress);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests disconnecting from sockets with the Fluid Jumper Plug class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidJumperPlug::testConnections()
{
    std::cout << "\n UtGunnsFluidJumperPlug . 02: testConnections .......................";

    /// - Add the sockets to the list and initialize the plug to a connection.
    tArticle->addSocket(tSocket1);
    tArticle->addSocket(tSocket2);
    tArticle->addSocket(tSocket3);
    tArticle->initialize(2, 0);

    /// - Verify a good disconnection request.  The result should have the jumper mapped to the
    ///   socket's ambient node, and the socket indicates this jumper is disconnected.  Here we are
    ///   requesting to disconnect from tSocket1, which returns ambient Node 2.
    tArticle->setDisconnectionRequest(0);
    CPPUNIT_ASSERT(2 == tArticle->update(tArticle->mConnectedNode));
    CPPUNIT_ASSERT(2 == tArticle->mConnectedNode);
    CPPUNIT_ASSERT(FriendlyGunnsFluidJumperPlug::NONE     == tArticle->getActiveConnection());
    CPPUNIT_ASSERT(FriendlyGunnsFluidJumperPlug::NONE     == tArticle->getDisconnectionRequest());
    CPPUNIT_ASSERT(!tSocket1.isConnected(tArticle));

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests direct connection command.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidJumperPlug::testDirectConnection()
{
    std::cout << "\n UtGunnsFluidJumperPlug . 03: testDirectConnection ..................";

    /// - Add the sockets to the list and initialize the plug to a connection.
    tArticle->addSocket(tSocket1);
    tArticle->addSocket(tSocket2);
    tArticle->addSocket(tSocket3);
    tArticle->initialize(2, 0);
    CPPUNIT_ASSERT(0 == tArticle->mConnectedNode);
    CPPUNIT_ASSERT(FriendlyGunnsFluidJumperPlug::SOCKET_1 == tArticle->getActiveConnection());
    CPPUNIT_ASSERT(FriendlyGunnsFluidJumperPlug::NONE     == tArticle->getDisconnectionRequest());

    /// - Set the direct connection command to get the plug to swap sockets in one pass.  We are
    ///   connecting to SOCKET_2, which controls access to node 1.
    tArticle->setDirectConnectionRequest(1);
    CPPUNIT_ASSERT(1 == tArticle->update(tArticle->mConnectedNode));
    CPPUNIT_ASSERT(1 == tArticle->mConnectedNode);
    CPPUNIT_ASSERT(FriendlyGunnsFluidJumperPlug::SOCKET_2 == tArticle->getActiveConnection());
    CPPUNIT_ASSERT(FriendlyGunnsFluidJumperPlug::NONE     == tArticle->getConnectionRequest());
    CPPUNIT_ASSERT(FriendlyGunnsFluidJumperPlug::NONE     == tArticle->getDisconnectionRequest());
    CPPUNIT_ASSERT(FriendlyGunnsFluidJumperPlug::SOCKET_2 == tArticle->getDirectConnectionRequest());
    CPPUNIT_ASSERT(tSocket2.isConnected(tArticle));

    /// - Verify repeated commands do nothing.
    CPPUNIT_ASSERT(1 == tArticle->update(tArticle->mConnectedNode));
    CPPUNIT_ASSERT(1 == tArticle->mConnectedNode);
    CPPUNIT_ASSERT(FriendlyGunnsFluidJumperPlug::SOCKET_2 == tArticle->getActiveConnection());
    CPPUNIT_ASSERT(FriendlyGunnsFluidJumperPlug::NONE     == tArticle->getConnectionRequest());
    CPPUNIT_ASSERT(FriendlyGunnsFluidJumperPlug::NONE     == tArticle->getDisconnectionRequest());
    CPPUNIT_ASSERT(FriendlyGunnsFluidJumperPlug::SOCKET_2 == tArticle->getDirectConnectionRequest());
    CPPUNIT_ASSERT(tSocket2.isConnected(tArticle));

    /// - Verify plug remains disconnected if the desired socket already has another connection.
    ///   We'll connect a fake plug to tSocket3, and then try to connect our test jumper to it.
    FriendlyGunnsFluidJumperPlug fakeJumper("fake jumper");
    tSocket3.requestConnection(&fakeJumper);
    tArticle->setDirectConnectionRequest(2);
    CPPUNIT_ASSERT(2 == tArticle->update(tArticle->mConnectedNode));
    CPPUNIT_ASSERT(2 == tArticle->mConnectedNode);
    CPPUNIT_ASSERT(FriendlyGunnsFluidJumperPlug::NONE     == tArticle->getActiveConnection());
    CPPUNIT_ASSERT(FriendlyGunnsFluidJumperPlug::NONE     == tArticle->getConnectionRequest());
    CPPUNIT_ASSERT(FriendlyGunnsFluidJumperPlug::NONE     == tArticle->getDisconnectionRequest());
    CPPUNIT_ASSERT(FriendlyGunnsFluidJumperPlug::SOCKET_3 == tArticle->getDirectConnectionRequest());
    CPPUNIT_ASSERT(!tSocket2.isConnected(tArticle));
    CPPUNIT_ASSERT(!tSocket3.isConnected(tArticle));
    CPPUNIT_ASSERT( tSocket3.isConnected(&fakeJumper));

    /// - Verify the direct connection command lockout.  Command a direct connection back to
    ///   tSocket2 and verify the plug remains unconnected and the direct request is reset.
    tArticle->mDirectConnectionDisable = true;
    tArticle->setDirectConnectionRequest(1);
    CPPUNIT_ASSERT(2 == tArticle->update(tArticle->mConnectedNode));
    CPPUNIT_ASSERT(2 == tArticle->mConnectedNode);
    CPPUNIT_ASSERT(FriendlyGunnsFluidJumperPlug::NONE     == tArticle->getActiveConnection());
    CPPUNIT_ASSERT(FriendlyGunnsFluidJumperPlug::NONE     == tArticle->getConnectionRequest());
    CPPUNIT_ASSERT(FriendlyGunnsFluidJumperPlug::NONE     == tArticle->getDisconnectionRequest());
    CPPUNIT_ASSERT(-99                                    == tArticle->getDirectConnectionRequest());
    CPPUNIT_ASSERT(!tSocket2.isConnected(tArticle));

    std::cout << "... Pass";
}
