/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((core/GunnsBasicSocket.o))
***************************************************************************************************/
#include "UtGunnsBasicSocket.hh"
#include "UtGunnsBasicJumperPlug.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsInvalidStateTransitionException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsBasicSocket class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsBasicSocket::UtGunnsBasicSocket()
    :
    tArticle(),
    tLinkName(),
    tDefaultConductivity(),
    tSelfSealing(),
    tConfigData(),
    tBlockage(),
    tInputData(),
    tNodes(),
    tNodeList(),
    tLinks(),
    tPort0(),
    tPort1()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsBasicSocket class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsBasicSocket::~UtGunnsBasicSocket()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSocket::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tInputData;
    delete tConfigData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSocket::setUp()
{
    tLinkName             = "Test Basic Socket";
    tNodeList.mNumNodes   = 2;
    tNodeList.mNodes      = tNodes;
    tDefaultConductivity  = 0.5;
    tSelfSealing          = true;
    tBlockage             = 0.2;
    tPort0                = 0;
    tPort1                = 1;

    /// - Define nominal configuration data
    tConfigData = new GunnsBasicSocketConfigData(tLinkName, &tNodeList, tDefaultConductivity,
                                                 tSelfSealing);

    /// - Create nominal input data
    tInputData = new GunnsBasicSocketInputData(true, tBlockage);

    /// - Create the test article
    tArticle = new FriendlyGunnsBasicSocket;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSocket::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsBasicSocket ..... 01: testConfig ............................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tLinkName            == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes               == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tDefaultConductivity == tConfigData->mDefaultConductivity);
    CPPUNIT_ASSERT(tSelfSealing         == tConfigData->mSelfSealing);

    /// - Check default config construction
    GunnsBasicSocketConfigData defaultConfig;
    CPPUNIT_ASSERT(""                   == defaultConfig.mName);
    CPPUNIT_ASSERT(0                    == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0                  == defaultConfig.mDefaultConductivity);
    CPPUNIT_ASSERT(false                == defaultConfig.mSelfSealing);

    /// - Check copy config construction
    GunnsBasicSocketConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tLinkName            == copyConfig.mName);
    CPPUNIT_ASSERT(tNodes               == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tDefaultConductivity == copyConfig.mDefaultConductivity);
    CPPUNIT_ASSERT(tSelfSealing         == copyConfig.mSelfSealing);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the input data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSocket::testInput()
{
    std::cout << "\n UtGunnsBasicSocket ..... 02: testInput .............................";

    /// - Check nominal input construction
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tBlockage, tInputData->mMalfBlockageValue, 0.0);

    /// - Check default input construction
    GunnsBasicSocketInputData defaultInput;
    CPPUNIT_ASSERT(!defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0 == defaultInput.mMalfBlockageValue);

    /// - Check copy input construction
    GunnsBasicSocketInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tInputData->mMalfBlockageValue, copyInput.mMalfBlockageValue, 0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSocket::testDefaultConstruction()
{
    std::cout << "\n UtGunnsBasicSocket ..... 03: testDefaultConstruction ...............";

    /// @test state data
    CPPUNIT_ASSERT(0.0   == tArticle->mEffectiveConductivity);
    CPPUNIT_ASSERT(0.0   == tArticle->mDefaultConductivity);
    CPPUNIT_ASSERT(0.0   == tArticle->mSystemConductance);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfBlockageValue);
    CPPUNIT_ASSERT(false == tArticle->mSelfSealing);
    CPPUNIT_ASSERT(0     == tArticle->mConnection);
    CPPUNIT_ASSERT(false == tArticle->mHasConnection);

    /// @test init flag
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    /// @test new/delete for code coverage
    GunnsBasicSocket* article = new GunnsBasicSocket();
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSocket::testNominalInitialization()
{
    std::cout << "\n UtGunnsBasicSocket ..... 04: testNominalInitialization .............";

    /// - Initialize with input data and check it
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    CPPUNIT_ASSERT(tDefaultConductivity == tArticle->mEffectiveConductivity);
    CPPUNIT_ASSERT(tDefaultConductivity == tArticle->mDefaultConductivity);
    CPPUNIT_ASSERT(tBlockage            == tArticle->mMalfBlockageValue);
    CPPUNIT_ASSERT(tSelfSealing         == tArticle->mSelfSealing);
    CPPUNIT_ASSERT(0                    == tArticle->mConnection);
    CPPUNIT_ASSERT(false                == tArticle->mHasConnection);
    CPPUNIT_ASSERT(tArticle->mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the requestConnection method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSocket::testRequestConnection()
{
    std::cout << "\n UtGunnsBasicSocket ..... 05: testRequestConnection .................";

    /// - Create fake jumper plugs.
    FriendlyGunnsBasicJumperPlug plug1("plug1");
    FriendlyGunnsBasicJumperPlug plug2("plug2");

    /// - Verify that an uninitialized socket throws an exception if connection is requested.
    CPPUNIT_ASSERT_THROW(tArticle->requestConnection(&plug1), TsInitializationException);

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Verify that the socket, when not connected, accepts plug's connection request.
    CPPUNIT_ASSERT(tPort0 == tArticle->requestConnection(&plug1));
    CPPUNIT_ASSERT(&plug1 == tArticle->mConnection);
    CPPUNIT_ASSERT(true   == tArticle->mHasConnection);

    /// - Verify that the socket, when connected to another plug, denies another plug's request.
    CPPUNIT_ASSERT_THROW(tArticle->requestConnection(&plug2), TsInvalidStateTransitionException);
    CPPUNIT_ASSERT(&plug1 == tArticle->mConnection);
    CPPUNIT_ASSERT(true   == tArticle->mHasConnection);

    /// - Verify that the socket, when connected to the same plug, denies the plug's request.
    CPPUNIT_ASSERT_THROW(tArticle->requestConnection(&plug1), TsInvalidStateTransitionException);
    CPPUNIT_ASSERT(&plug1 == tArticle->mConnection);
    CPPUNIT_ASSERT(true   == tArticle->mHasConnection);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the requestDisconnection method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSocket::testRequestDisconnection()
{
    std::cout << "\n UtGunnsBasicSocket ..... 06: testRequestDisconnection ..............";

    /// - Create fake jumper plugs.
    FriendlyGunnsBasicJumperPlug plug1("plug1");
    FriendlyGunnsBasicJumperPlug plug2("plug2");

    /// - Verify that an uninitialized socket throws an exception if connection is requested.
    CPPUNIT_ASSERT_THROW(tArticle->requestDisconnection(&plug1), TsInitializationException);

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Connect plug1 and verify disconnection of plug2.  Since plug2 is not the one
    ///   connected, the socked should return the ambient node, and leave plug1 connected.
    tArticle->requestConnection(&plug1);
    CPPUNIT_ASSERT(tPort1 == tArticle->requestDisconnection(&plug2));
    CPPUNIT_ASSERT(&plug1 == tArticle->mConnection);
    CPPUNIT_ASSERT(true   == tArticle->mHasConnection);

    /// - Verify disconnection of plug1 when connected.
    CPPUNIT_ASSERT(tPort1 == tArticle->requestDisconnection(&plug1));
    CPPUNIT_ASSERT(0      == tArticle->mConnection);
    CPPUNIT_ASSERT(false  == tArticle->mHasConnection);

    /// - Verify disconnection of plug1 when not connected.
    CPPUNIT_ASSERT(tPort1 == tArticle->requestDisconnection(&plug1));
    CPPUNIT_ASSERT(0      == tArticle->mConnection);
    CPPUNIT_ASSERT(false  == tArticle->mHasConnection);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the getter methods of the socket class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSocket::testAccessors()
{
    std::cout << "\n UtGunnsBasicSocket ..... 07: testAccessors .........................";

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Verify the isConnected method returns false if the requestor is not connected.
    FriendlyGunnsBasicJumperPlug plug("plug");
    CPPUNIT_ASSERT(!tArticle->isConnected(&plug));

    /// - Verify the isConnected method returns false if no plug is connected.
    CPPUNIT_ASSERT(!tArticle->isConnected());

    /// - Verify the isConnected method returns true if the requestor is connected.
    tArticle->requestConnection(&plug);
    CPPUNIT_ASSERT(tArticle->isConnected(&plug));

    /// - Verify the isConnected method returns true if any plug is connected.
    CPPUNIT_ASSERT(tArticle->isConnected());

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the interaction of the base class step method with the socket's updateState
///           method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSocket::testStep()
{
    std::cout << "\n UtGunnsBasicSocket ..... 08: testStep ..............................";

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Verify that the self-sealing socket with no connection has no conductivity.
    tArticle->step(0.0);
    CPPUNIT_ASSERT(0.0 == tArticle->mEffectiveConductivity);
    CPPUNIT_ASSERT(0.0 == tArticle->mSystemConductance);
    CPPUNIT_ASSERT(0.0 == tArticle->mAdmittanceMatrix[0]);

    /// - Verify that a non-self-sealing socket with no connection has the correct conductivity.
    tArticle->mSelfSealing = false;
    tArticle->step(0.0);
    CPPUNIT_ASSERT(tDefaultConductivity                     == tArticle->mEffectiveConductivity);
    CPPUNIT_ASSERT(tDefaultConductivity * (1.0 - tBlockage) == tArticle->mSystemConductance);
    CPPUNIT_ASSERT(tDefaultConductivity * (1.0 - tBlockage) == tArticle->mAdmittanceMatrix[0]);

    /// - Verify that a non-self-sealing socket with a connection has no conductivity.
    FriendlyGunnsBasicJumperPlug plug("plug");
    tArticle->requestConnection(&plug);
    tArticle->step(0.0);
    CPPUNIT_ASSERT(0.0 == tArticle->mEffectiveConductivity);
    CPPUNIT_ASSERT(0.0 == tArticle->mSystemConductance);
    CPPUNIT_ASSERT(0.0 == tArticle->mAdmittanceMatrix[0]);

    /// - Verify that a self-sealing socket with a connection has no conductivity.
    tArticle->mSelfSealing = false;
    tArticle->step(0.0);
    CPPUNIT_ASSERT(0.0 == tArticle->mEffectiveConductivity);
    CPPUNIT_ASSERT(0.0 == tArticle->mSystemConductance);
    CPPUNIT_ASSERT(0.0 == tArticle->mAdmittanceMatrix[0]);

    std::cout << "... Pass";
}
