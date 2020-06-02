/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((core/GunnsBasicJumperPlug.o))
***************************************************************************************************/
#include "UtGunnsBasicJumperPlug.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsBasicJumperPlug class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsBasicJumperPlug::UtGunnsBasicJumperPlug()
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
/// @details  This is the default destructor for the UtGunnsBasicJumperPlug class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsBasicJumperPlug::~UtGunnsBasicJumperPlug()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicJumperPlug::tearDown()
{
    /// - Deletes for new's in setUp
    delete tArticle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicJumperPlug::setUp()
{
    /// - Create the test article
    tName = "Test Article";
    tArticle = new FriendlyGunnsBasicJumperPlug(tName);
    tNodeList.mNumNodes = 3;
    tNodeList.mNodes    = tNodes;

    /// - Initialize the sockets
    GunnsBasicSocketConfigData socketConfig("Test Socket", &tNodeList, 0.0, false);
    GunnsBasicSocketInputData  socketInput (false, 0.0);

    tSocket1.initialize(socketConfig, socketInput, tLinks, 0, 2);
    tSocket2.initialize(socketConfig, socketInput, tLinks, 1, 2);
    tSocket3.initialize(socketConfig, socketInput, tLinks, 0, 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the constructor methods of the Basic Jumper Plug class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicJumperPlug::testConstructors()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsBasicJumperPlug . 01: testConstructors ......................";

    /// - Check default construction
    CPPUNIT_ASSERT(tName == tArticle->mName);
    CPPUNIT_ASSERT(0     != tArticle->mSockets);
    CPPUNIT_ASSERT(0     == tArticle->mNumSockets);
    CPPUNIT_ASSERT(0     == tArticle->mConnectedNode);
    CPPUNIT_ASSERT(0     == tArticle->mDirectConnectionRequest);
    CPPUNIT_ASSERT(false == tArticle->mDirectConnectionDisable);

    /// - Verify allocation of sockets array.
    GunnsBasicLink** expectedAddress = &tArticle->mSockets[0] + (FriendlyGunnsBasicJumperPlug::NONE - 1);
    GunnsBasicLink** actualAddress   = &tArticle->mSockets[FriendlyGunnsBasicJumperPlug::NONE - 1];
    CPPUNIT_ASSERT(expectedAddress == actualAddress);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the addSocket method of the Basic Jumper Plug class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicJumperPlug::testAddSocket()
{
    std::cout << "\n UtGunnsBasicJumperPlug . 02: testAddSocket .........................";

    /// - Add the sockets and verify their address in the socket list.
    tArticle->addSocket(tSocket1);
    CPPUNIT_ASSERT(&tSocket1 == tArticle->mSockets[0]);
    CPPUNIT_ASSERT(1         == tArticle->mNumSockets);

    tArticle->addSocket(tSocket2);
    CPPUNIT_ASSERT(&tSocket2 == tArticle->mSockets[1]);
    CPPUNIT_ASSERT(2         == tArticle->mNumSockets);

    tArticle->addSocket(tSocket3);
    CPPUNIT_ASSERT(&tSocket3 == tArticle->mSockets[2]);
    CPPUNIT_ASSERT(3         == tArticle->mNumSockets);

    /// - Add one socket too many and verify an exception is thrown.
    CPPUNIT_ASSERT_THROW(tArticle->addSocket(tSocket1), TsInitializationException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions of the Basic Jumper Plug class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicJumperPlug::testNominalInitialization()
{
    std::cout << "\n UtGunnsBasicJumperPlug . 03: testNominalInitialization .............";

    /// - Add the sockets to the list.
    tArticle->addSocket(tSocket1);
    tArticle->addSocket(tSocket2);
    tArticle->addSocket(tSocket3);

    /// - Initialize with no initial connection and verify the return value.
    CPPUNIT_ASSERT(2                                      == tArticle->initialize(2, 3));
    CPPUNIT_ASSERT(2                                      == tArticle->mConnectedNode);
    CPPUNIT_ASSERT(-99                                    == tArticle->mDirectConnectionRequest);
    CPPUNIT_ASSERT(false                                  == tArticle->mDirectConnectionDisable);
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getActiveConnection());

    /// - Initialize with initial connection and verify the return value.
    CPPUNIT_ASSERT(1                                      == tArticle->initialize(2, 1));
    CPPUNIT_ASSERT(1                                      == tArticle->mConnectedNode);
    CPPUNIT_ASSERT(-99                                    == tArticle->mDirectConnectionRequest);
    CPPUNIT_ASSERT(false                                  == tArticle->mDirectConnectionDisable);
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::SOCKET_2 == tArticle->getActiveConnection());

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the various initialization exceptions of the Basic Jumper Plug class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicJumperPlug::testInitializationExceptions()
{
    std::cout << "\n UtGunnsBasicJumperPlug . 04: testInitializationExceptions ..........";

    /// - Add two sockets to the list.
    tArticle->addSocket(tSocket1);
    tArticle->addSocket(tSocket2);

    /// - Verify an exception is thrown if the Plug is missing a socket.
    CPPUNIT_ASSERT_THROW(tArticle->initialize(2, 1),  TsInitializationException);

    /// - Finish adding sockets to the list.
    tArticle->addSocket(tSocket3);

    /// - Verify an exception is thrown if the initial socket assignment > no connection.
    CPPUNIT_ASSERT_THROW(tArticle->initialize(2, 4),  TsInitializationException);

    /// - Verify an exception is thrown if the initial socket assignment < 0.
    CPPUNIT_ASSERT_THROW(tArticle->initialize(2, -1), TsInitializationException);

    /// - Verify an exception is thrown if the initial plug connection is denied.
    FriendlyGunnsBasicJumperPlug fakeJumper("fake jumper");
    tSocket2.requestConnection(&fakeJumper);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(2, 1),  TsInitializationException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the getSocket method of the Basic Jumper Plug class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicJumperPlug::testGetSocket()
{
    std::cout << "\n UtGunnsBasicJumperPlug . 05: testGetSocket .........................";

    /// - Add the sockets to the list.
    tArticle->addSocket(tSocket1);
    tArticle->addSocket(tSocket2);
    tArticle->addSocket(tSocket3);


    /// - Test requesting socket index -1 throws exception.
    CPPUNIT_ASSERT_THROW(tArticle->getSocket(-1), TsOutOfBoundsException);

    /// - Test requesting socket index 0.
    CPPUNIT_ASSERT(&tSocket1 == tArticle->getSocket(0));

    /// - Test requesting socket index 1.
    CPPUNIT_ASSERT(&tSocket2 == tArticle->getSocket(1));

    /// - Test requesting socket index 2.
    CPPUNIT_ASSERT(&tSocket3 == tArticle->getSocket(2));

    /// - Test requesting socket index 3 throws exception.
    CPPUNIT_ASSERT_THROW(tArticle->getSocket(3), TsOutOfBoundsException);

    /// - Test requesting socket index with null pointer throws exception.
    tArticle->mSockets[2] = 0;
    CPPUNIT_ASSERT_THROW(tArticle->getSocket(2), TsOutOfBoundsException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the getter methods of the Basic Jumper Plug class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicJumperPlug::testAccessors()
{
    std::cout << "\n UtGunnsBasicJumperPlug . 06: testAccessors .........................";

    tArticle->mActiveConnection        = FriendlyGunnsBasicJumperPlug::SOCKET_1;
    tArticle->mLastDisconnection       = FriendlyGunnsBasicJumperPlug::SOCKET_3;
    tArticle->mConnectionRequest       = FriendlyGunnsBasicJumperPlug::SOCKET_2;
    tArticle->mDisconnectionRequest    = FriendlyGunnsBasicJumperPlug::SOCKET_3;
    tArticle->mDirectConnectionRequest = FriendlyGunnsBasicJumperPlug::SOCKET_2;
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::SOCKET_1 == tArticle->getActiveConnection());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::SOCKET_3 == tArticle->getLastDisconnection());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::SOCKET_2 == tArticle->getConnectionRequest());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::SOCKET_3 == tArticle->getDisconnectionRequest());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::SOCKET_2 == tArticle->getDirectConnectionRequest());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getNoConnection());
    CPPUNIT_ASSERT(tName                                  == tArticle->getName());

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the setter methods of the Basic Jumper Plug class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicJumperPlug::testModifiers()
{
    std::cout << "\n UtGunnsBasicJumperPlug . 07: testModifiers .........................";

    tArticle->setActiveConnection       (FriendlyGunnsBasicJumperPlug::SOCKET_1);
    tArticle->setLastDisconnection      (FriendlyGunnsBasicJumperPlug::SOCKET_3);
    tArticle->setConnectionRequest      (FriendlyGunnsBasicJumperPlug::SOCKET_2);
    tArticle->setDisconnectionRequest   (FriendlyGunnsBasicJumperPlug::SOCKET_3);
    tArticle->setDirectConnectionRequest(FriendlyGunnsBasicJumperPlug::SOCKET_2);
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::SOCKET_1 == tArticle->mActiveConnection);
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::SOCKET_3 == tArticle->mLastDisconnection);
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::SOCKET_2 == tArticle->mConnectionRequest);
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::SOCKET_3 == tArticle->mDisconnectionRequest);
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::SOCKET_2 == tArticle->mDirectConnectionRequest);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests connecting to sockets with the Basic Jumper Plug class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicJumperPlug::testConnection()
{
    std::cout << "\n UtGunnsBasicJumperPlug . 08: testConnection ........................";

    /// - Add the sockets to the list and initialize the plug to no connection.
    tArticle->addSocket(tSocket1);
    tArticle->addSocket(tSocket2);
    tArticle->addSocket(tSocket3);
    tArticle->initialize(0, 3);
    tArticle->setLastDisconnection(0);
    CPPUNIT_ASSERT(0 == tArticle->mConnectedNode);
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getActiveConnection());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getConnectionRequest());

    /// - Verify nothing happens if the plug connection request = no connection, and that it follows
    ///   the given node number if there is no connection change.
    tArticle->setConnectionRequest(3);
    CPPUNIT_ASSERT(2 == tArticle->update(2));
    CPPUNIT_ASSERT(2 == tArticle->mConnectedNode);
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getActiveConnection());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getConnectionRequest());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::SOCKET_1 == tArticle->getLastDisconnection());

    /// - Verify nothing happens if the connection request is denied by the socket.  We'll
    ///   connect a fake plug to tSocket2, and then try to connect our test jumper to it.
    FriendlyGunnsBasicJumperPlug fakeJumper("fake jumper");
    tSocket2.requestConnection(&fakeJumper);
    tArticle->setConnectionRequest(1);
    CPPUNIT_ASSERT(2 == tArticle->update(tArticle->mConnectedNode));
    CPPUNIT_ASSERT(2 == tArticle->mConnectedNode);
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getActiveConnection());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getConnectionRequest());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::SOCKET_1 == tArticle->getLastDisconnection());
    CPPUNIT_ASSERT(!tSocket2.isConnected(tArticle));
    CPPUNIT_ASSERT( tSocket2.isConnected(&fakeJumper));

    /// - Verify a good connection request.  Here we are connecting to tSocket1, which controls
    ///   access to Node 0.
    tArticle->setConnectionRequest(0);
    CPPUNIT_ASSERT(0 == tArticle->update(tArticle->mConnectedNode));
    CPPUNIT_ASSERT(0 == tArticle->mConnectedNode);
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::SOCKET_1 == tArticle->getActiveConnection());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getConnectionRequest());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getLastDisconnection());
    CPPUNIT_ASSERT(tSocket1.isConnected(tArticle));

    /// - Verify no connection happens if the plug is already connected to a socket.
    tArticle->setConnectionRequest(2);
    CPPUNIT_ASSERT(0 == tArticle->update(tArticle->mConnectedNode));
    CPPUNIT_ASSERT(0 == tArticle->mConnectedNode);
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::SOCKET_1 == tArticle->getActiveConnection());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getConnectionRequest());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getLastDisconnection());
    CPPUNIT_ASSERT( tSocket1.isConnected(tArticle));
    CPPUNIT_ASSERT(!tSocket3.isConnected(tArticle));

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests disconnecting from sockets with the Basic Jumper Plug class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicJumperPlug::testDisconnection()
{
    std::cout << "\n UtGunnsBasicJumperPlug . 09: testDisconnection .....................";

    /// - Add the sockets to the list and initialize the plug to a connection.
    tArticle->addSocket(tSocket1);
    tArticle->addSocket(tSocket2);
    tArticle->addSocket(tSocket3);
    tArticle->initialize(2, 0);
    CPPUNIT_ASSERT(0 == tArticle->mConnectedNode);
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::SOCKET_1 == tArticle->getActiveConnection());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getDisconnectionRequest());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getLastDisconnection());

    /// - Verify nothing happens if the plug connection request = no connection.
    tArticle->setDisconnectionRequest(3);
    CPPUNIT_ASSERT(0 == tArticle->update(tArticle->mConnectedNode));
    CPPUNIT_ASSERT(0 == tArticle->mConnectedNode);
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::SOCKET_1 == tArticle->getActiveConnection());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getDisconnectionRequest());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getLastDisconnection());

    /// - Verify nothing happens if attempting to disconnect from a different socket than the
    ///   current connected socket.
    tArticle->setDisconnectionRequest(2);
    CPPUNIT_ASSERT(0 == tArticle->update(tArticle->mConnectedNode));
    CPPUNIT_ASSERT(0 == tArticle->mConnectedNode);
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::SOCKET_1 == tArticle->getActiveConnection());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getDisconnectionRequest());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getLastDisconnection());

    /// - Verify a good disconnection request.  The result should have the jumper mapped to the
    ///   socket's ambient node, and the socket indicates this jumper is disconnected.  Here we are
    ///   requesting to disconnect from tSocket1, which returns ambient Node 2.
    tArticle->setDisconnectionRequest(0);
    CPPUNIT_ASSERT(2 == tArticle->update(tArticle->mConnectedNode));
    CPPUNIT_ASSERT(2 == tArticle->mConnectedNode);
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getActiveConnection());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getDisconnectionRequest());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::SOCKET_1 == tArticle->getLastDisconnection());
    CPPUNIT_ASSERT(!tSocket1.isConnected(tArticle));

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests simultaneous requests to disconnect and connect.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicJumperPlug::testQuickSwap()
{
    std::cout << "\n UtGunnsBasicJumperPlug . 10: testQuickSwap .........................";

    /// - Add the sockets to the list and initialize the plug to a connection.
    tArticle->addSocket(tSocket1);
    tArticle->addSocket(tSocket2);
    tArticle->addSocket(tSocket3);
    tArticle->initialize(2, 0);
    CPPUNIT_ASSERT(0 == tArticle->mConnectedNode);
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::SOCKET_1 == tArticle->getActiveConnection());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getDisconnectionRequest());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getLastDisconnection());

    /// - Set both the disconnect and connect requests at the same time to get the plug to swap
    ///   sockets in one pass.  We are connecting to SOCKET_2, which controls access to node 1.
    tArticle->setDisconnectionRequest(tArticle->getActiveConnection());
    tArticle->setConnectionRequest(1);
    CPPUNIT_ASSERT(1 == tArticle->update(tArticle->mConnectedNode));
    CPPUNIT_ASSERT(1 == tArticle->mConnectedNode);
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::SOCKET_2 == tArticle->getActiveConnection());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getConnectionRequest());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getDisconnectionRequest());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getLastDisconnection());
    CPPUNIT_ASSERT(tSocket2.isConnected(tArticle));

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests direct connection command.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicJumperPlug::testDirectConnection()
{
    std::cout << "\n UtGunnsBasicJumperPlug . 11: testDirectConnection ..................";

    /// - Add the sockets to the list and initialize the plug to a connection.
    tArticle->addSocket(tSocket1);
    tArticle->addSocket(tSocket2);
    tArticle->addSocket(tSocket3);
    tArticle->initialize(2, 0);
    CPPUNIT_ASSERT(0 == tArticle->mConnectedNode);
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::SOCKET_1 == tArticle->getActiveConnection());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getDisconnectionRequest());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getLastDisconnection());

    /// - Set the direct connection command to get the plug to swap sockets in one pass.  We are
    ///   connecting to SOCKET_2, which controls access to node 1.
    tArticle->setDirectConnectionRequest(1);
    CPPUNIT_ASSERT(1 == tArticle->update(tArticle->mConnectedNode));
    CPPUNIT_ASSERT(1 == tArticle->mConnectedNode);
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::SOCKET_2 == tArticle->getActiveConnection());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getConnectionRequest());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getDisconnectionRequest());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::SOCKET_2 == tArticle->getDirectConnectionRequest());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getLastDisconnection());
    CPPUNIT_ASSERT(tSocket2.isConnected(tArticle));

    /// - Verify repeated commands do nothing.
    CPPUNIT_ASSERT(1 == tArticle->update(tArticle->mConnectedNode));
    CPPUNIT_ASSERT(1 == tArticle->mConnectedNode);
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::SOCKET_2 == tArticle->getActiveConnection());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getConnectionRequest());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getDisconnectionRequest());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::SOCKET_2 == tArticle->getDirectConnectionRequest());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getLastDisconnection());
    CPPUNIT_ASSERT(tSocket2.isConnected(tArticle));

    /// - Verify plug remains disconnected if the desired socket already has another connection.
    ///   We'll connect a fake plug to tSocket3, and then try to connect our test jumper to it.
    FriendlyGunnsBasicJumperPlug fakeJumper("fake jumper");
    tSocket3.requestConnection(&fakeJumper);
    tArticle->setDirectConnectionRequest(2);
    CPPUNIT_ASSERT(2 == tArticle->update(tArticle->mConnectedNode));
    CPPUNIT_ASSERT(2 == tArticle->mConnectedNode);
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getActiveConnection());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getConnectionRequest());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getDisconnectionRequest());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::SOCKET_3 == tArticle->getDirectConnectionRequest());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::SOCKET_2 == tArticle->getLastDisconnection());
    CPPUNIT_ASSERT(!tSocket2.isConnected(tArticle));
    CPPUNIT_ASSERT(!tSocket3.isConnected(tArticle));
    CPPUNIT_ASSERT( tSocket3.isConnected(&fakeJumper));

    /// - Verify the direct connection command lockout.  Command a direct connection back to
    ///   tSocket2 and verify the plug remains unconnected and the direct request is reset.
    tArticle->mDirectConnectionDisable = true;
    tArticle->setDirectConnectionRequest(1);
    CPPUNIT_ASSERT(2 == tArticle->update(tArticle->mConnectedNode));
    CPPUNIT_ASSERT(2 == tArticle->mConnectedNode);
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getActiveConnection());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getConnectionRequest());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::NONE     == tArticle->getDisconnectionRequest());
    CPPUNIT_ASSERT(-99                                    == tArticle->getDirectConnectionRequest());
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::SOCKET_2 == tArticle->getLastDisconnection());
    CPPUNIT_ASSERT(!tSocket2.isConnected(tArticle));

    std::cout << "... Pass";
}
