/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((core/GunnsBasicJumper.o))
***************************************************************************************************/
#include "UtGunnsBasicJumper.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsBasicJumper class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsBasicJumper::UtGunnsBasicJumper()
    :
    tArticle(),
    tLinkName(),
    tDefaultConductivity(),
    tJumperPlug0(),
    tJumperPlug1(),
    tSelfSealing0(),
    tSelfSealing1(),
    tConfigData(),
    tBlockage(),
    tConnection0(),
    tConnection1(),
    tInputData(),
    tNodes(),
    tNodeList(),
    tLinks(),
    tPort0(),
    tPort1(),
    tSocket0A(),
    tSocket0B(),
    tSocket0C(),
    tSocket1A(),
    tSocket1B(),
    tSocket1C()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsBasicJumper class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsBasicJumper::~UtGunnsBasicJumper()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicJumper::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete tJumperPlug1;
    delete tJumperPlug0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicJumper::setUp()
{
    tLinkName             = "Test Basic Jumper";
    tNodeList.mNumNodes   = 3;
    tNodeList.mNodes      = tNodes;
    tDefaultConductivity  = 0.5;
    tSelfSealing0         = true;
    tSelfSealing1         = true;
    tBlockage             = 0.2;
    tConnection0          = 0;
    tConnection1          = 3;
    tPort0                = 0;
    tPort1                = 1;

    /// - Set up plugs
    tJumperPlug0 = new FriendlyGunnsBasicJumperPlug("Test Plug 0");
    tJumperPlug1 = new FriendlyGunnsBasicJumperPlug("Test Plug 1");
    tJumperPlug0->addSocket(tSocket0A);
    tJumperPlug0->addSocket(tSocket0B);
    tJumperPlug0->addSocket(tSocket0C);
    tJumperPlug1->addSocket(tSocket1A);
    tJumperPlug1->addSocket(tSocket1B);
    tJumperPlug1->addSocket(tSocket1C);

    /// - Initialize the sockets
    GunnsBasicSocketConfigData socketConfig("Test Socket", &tNodeList, 0.0, false);
    GunnsBasicSocketInputData  socketInput (false, 0.0);

    tSocket0A.initialize(socketConfig, socketInput, tLinks, 0, 2);
    tSocket0B.initialize(socketConfig, socketInput, tLinks, 1, 2);
    tSocket0C.initialize(socketConfig, socketInput, tLinks, 1, 0);
    tSocket1A.initialize(socketConfig, socketInput, tLinks, 0, 2);
    tSocket1B.initialize(socketConfig, socketInput, tLinks, 2, 2);
    tSocket1C.initialize(socketConfig, socketInput, tLinks, 1, 0);

    /// - Define nominal configuration data
    tConfigData = new GunnsBasicJumperConfigData(tLinkName, &tNodeList, tDefaultConductivity,
                                                 tJumperPlug0, tJumperPlug1, tSelfSealing0,
                                                 tSelfSealing1);

    /// - Create nominal input data
    tInputData = new GunnsBasicJumperInputData(true, tBlockage, tConnection0, tConnection1);

    /// - Create the test article
    tArticle = new FriendlyGunnsBasicJumper;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicJumper::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsBasicJumper ..... 01: testConfig ............................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tLinkName            == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes               == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tDefaultConductivity == tConfigData->mDefaultConductivity);
    CPPUNIT_ASSERT(tJumperPlug0         == tConfigData->mPlug0);
    CPPUNIT_ASSERT(tJumperPlug1         == tConfigData->mPlug1);
    CPPUNIT_ASSERT(tSelfSealing0        == tConfigData->mSelfSealing0);
    CPPUNIT_ASSERT(tSelfSealing1        == tConfigData->mSelfSealing1);

    /// - Check default config construction
    GunnsBasicJumperConfigData defaultConfig;
    CPPUNIT_ASSERT(""                   == defaultConfig.mName);
    CPPUNIT_ASSERT(0                    == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0                  == defaultConfig.mDefaultConductivity);
    CPPUNIT_ASSERT(0                    == defaultConfig.mPlug0);
    CPPUNIT_ASSERT(0                    == defaultConfig.mPlug1);
    CPPUNIT_ASSERT(false                == defaultConfig.mSelfSealing0);
    CPPUNIT_ASSERT(false                == defaultConfig.mSelfSealing1);

    /// - Check copy config construction
    GunnsBasicJumperConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tLinkName            == copyConfig.mName);
    CPPUNIT_ASSERT(tNodes               == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tDefaultConductivity == copyConfig.mDefaultConductivity);
    CPPUNIT_ASSERT(tJumperPlug0         == copyConfig.mPlug0);
    CPPUNIT_ASSERT(tJumperPlug1         == copyConfig.mPlug1);
    CPPUNIT_ASSERT(tSelfSealing0        == copyConfig.mSelfSealing0);
    CPPUNIT_ASSERT(tSelfSealing1        == copyConfig.mSelfSealing1);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the input data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicJumper::testInput()
{
    std::cout << "\n UtGunnsBasicJumper ..... 02: testInput .............................";

    /// - Check nominal input construction
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tBlockage, tInputData->mMalfBlockageValue, 0.0);

    /// - Check default input construction
    GunnsBasicJumperInputData defaultInput;
    CPPUNIT_ASSERT(!defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0 == defaultInput.mMalfBlockageValue);

    /// - Check copy input construction
    GunnsBasicJumperInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tInputData->mMalfBlockageValue, copyInput.mMalfBlockageValue, 0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicJumper::testDefaultConstruction()
{
    std::cout << "\n UtGunnsBasicJumper ..... 03: testDefaultConstruction ...............";

    /// @test state data
    CPPUNIT_ASSERT(0.0   == tArticle->mEffectiveConductivity);
    CPPUNIT_ASSERT(0.0   == tArticle->mDefaultConductivity);
    CPPUNIT_ASSERT(0.0   == tArticle->mSystemConductance);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfBlockageValue);
    CPPUNIT_ASSERT(0     == tArticle->mPlug[0]);
    CPPUNIT_ASSERT(0     == tArticle->mPlug[1]);
    CPPUNIT_ASSERT(false == tArticle->mSelfSealing0);
    CPPUNIT_ASSERT(false == tArticle->mSelfSealing1);

    /// @test init flag
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    /// @test new/delete for code coverage
    GunnsBasicJumper* article = new GunnsBasicJumper();
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicJumper::testNominalInitialization()
{
    std::cout << "\n UtGunnsBasicJumper ..... 04: testNominalInitialization .............";

    /// - Initialize with input data and check it.  In this case we'll configure the jumper to not
    ///   have a plug on port 1, so we can see both cases of a port and no port.
    tConfigData->mPlug1 = 0;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    CPPUNIT_ASSERT(tDefaultConductivity == tArticle->mEffectiveConductivity);
    CPPUNIT_ASSERT(tDefaultConductivity == tArticle->mDefaultConductivity);
    CPPUNIT_ASSERT(tBlockage            == tArticle->mMalfBlockageValue);
    CPPUNIT_ASSERT(tJumperPlug0         == tArticle->mPlug[0]);
    CPPUNIT_ASSERT(0                    == tArticle->mPlug[1]);
    CPPUNIT_ASSERT(tSelfSealing0        == tArticle->mSelfSealing0);
    CPPUNIT_ASSERT(tSelfSealing1        == tArticle->mSelfSealing1);

    /// - Verify the initial socket connections
    CPPUNIT_ASSERT(0      == tArticle->mNodeMap[0]);
    CPPUNIT_ASSERT(tPort1 == tArticle->mNodeMap[1]);
    CPPUNIT_ASSERT(FriendlyGunnsBasicJumperPlug::SOCKET_1 == tJumperPlug0->getActiveConnection());

    CPPUNIT_ASSERT(tArticle->mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test that a bad initialization of one of the plugs results in the link not finishing
///           initialization.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicJumper::testInitializationExceptions()
{
    std::cout << "\n UtGunnsBasicJumper ..... 05: testInitializationExceptions ..........";

    /// - Verify an exception is thrown if the initial socket assignment > no connection.
    tInputData->mConnection0 = 4;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the interaction of the base class step method with the jumper's updateState
///           method and the jumper plug objects.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicJumper::testStep()
{
    std::cout << "\n UtGunnsBasicJumper ..... 06: testStep ..............................";

    /// - Initialize default test article with nominal initialization data.
    tInputData->mConnection0 = 0;
    tInputData->mConnection1 = 2;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Verify that a jumper with both plugs connected has the correct conductivity.
    tArticle->step(0.0);
    const double expectedAdmittance = tDefaultConductivity * (1.0 - tBlockage);
    CPPUNIT_ASSERT(tDefaultConductivity == tArticle->mEffectiveConductivity);
    CPPUNIT_ASSERT(expectedAdmittance   == tArticle->mSystemConductance);
    CPPUNIT_ASSERT(expectedAdmittance   == tArticle->mAdmittanceMatrix[0]);

    /// - Verify that a jumper with self-sealing plug 0 that is disconnected has no conductivity.
    tJumperPlug0->setDisconnectionRequest(0);
    tArticle->step(0.0);
    CPPUNIT_ASSERT(0.0                  == tArticle->mEffectiveConductivity);
    CPPUNIT_ASSERT(0.0                  == tArticle->mSystemConductance);
    CPPUNIT_ASSERT(0.0                  == tArticle->mAdmittanceMatrix[0]);

    /// - Verify that a jumper with a non-self-sealing plug 0 that is disconnected has conductivity.
    tArticle->mSelfSealing0 = false;
    tArticle->step(0.0);
    CPPUNIT_ASSERT(tDefaultConductivity == tArticle->mEffectiveConductivity);
    CPPUNIT_ASSERT(expectedAdmittance   == tArticle->mSystemConductance);
    CPPUNIT_ASSERT(expectedAdmittance   == tArticle->mAdmittanceMatrix[0]);

    /// - Verify that a jumper with missing plug 0 has conductivity.
    tArticle->mPlug[0] = 0;
    tArticle->step(0.0);
    CPPUNIT_ASSERT(tDefaultConductivity == tArticle->mEffectiveConductivity);
    CPPUNIT_ASSERT(expectedAdmittance   == tArticle->mSystemConductance);
    CPPUNIT_ASSERT(expectedAdmittance   == tArticle->mAdmittanceMatrix[0]);
    tArticle->mPlug[0] = tJumperPlug0;

    /// - Verify that a jumper with self-sealing plug 1 that is disconnected has no conductivity.
    tJumperPlug1->setDisconnectionRequest(2);
    tArticle->step(0.0);
    CPPUNIT_ASSERT(0.0                  == tArticle->mEffectiveConductivity);
    CPPUNIT_ASSERT(0.0                  == tArticle->mSystemConductance);
    CPPUNIT_ASSERT(0.0                  == tArticle->mAdmittanceMatrix[0]);

    /// - Verify that a jumper with a non-self-sealing plug 1 that is disconnected has conductivity.
    tArticle->mSelfSealing1 = false;
    tArticle->step(0.0);
    CPPUNIT_ASSERT(tDefaultConductivity == tArticle->mEffectiveConductivity);
    CPPUNIT_ASSERT(expectedAdmittance   == tArticle->mSystemConductance);
    CPPUNIT_ASSERT(expectedAdmittance   == tArticle->mAdmittanceMatrix[0]);

    /// - Verify that a jumper with missing plug 1 has conductivity.
    tArticle->mPlug[1] = 0;
    tArticle->step(0.0);
    CPPUNIT_ASSERT(tDefaultConductivity == tArticle->mEffectiveConductivity);
    CPPUNIT_ASSERT(expectedAdmittance   == tArticle->mSystemConductance);
    CPPUNIT_ASSERT(expectedAdmittance   == tArticle->mAdmittanceMatrix[0]);
    tArticle->mPlug[1] = tJumperPlug1;

    /// - Verify that a jumper with both plugs connected to the same node has no conductivity.
    tJumperPlug0->setConnectionRequest(0);
    tJumperPlug1->setConnectionRequest(0);
    tArticle->step(0.0);
    CPPUNIT_ASSERT(0.0                  == tArticle->mEffectiveConductivity);
    CPPUNIT_ASSERT(0.0                  == tArticle->mSystemConductance);
    CPPUNIT_ASSERT(0.0                  == tArticle->mAdmittanceMatrix[0]);

    std::cout << "... Pass";
}
