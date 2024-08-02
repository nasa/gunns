/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((core/GunnsFluidSocket.o))
***************************************************************************************************/
#include "UtGunnsFluidSocket.hh"
#include "UtGunnsFluidJumperPlug.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsInvalidStateTransitionException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsFluidSocket class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidSocket::UtGunnsFluidSocket()
    :
    tArticle(),
    tLinkName(),
    tMaxConductivity(),
    tExpansionScaleFactor(),
    tSelfSealing(),
    tConfigData(),
    tBlockage(),
    tMalfLeakRateFlag(),
    tMalfLeakRateValue(),
    tInputData(),
    tNodes(),
    tNodeList(),
    tLinks(),
    tPort0(),
    tPort1(),
    tFluidProperties(),
    tFluidConfig(),
    tFluidInput1(),
    tFluidInput2(),
    tFractions()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsFluidSocket class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidSocket::~UtGunnsFluidSocket()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSocket::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete tFluidInput2;
    delete tFluidInput1;
    delete tFluidConfig;
    delete [] tFractions;
    delete tFluidProperties;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSocket::setUp()
{
    tLinkName             = "Test Fluid Socket";
    tNodeList.mNumNodes   = 2;
    tNodeList.mNodes      = tNodes;
    tMaxConductivity      = 5.0E-4;
    tExpansionScaleFactor = 0.4;
    tSelfSealing          = true;
    tBlockage             = 0.2;
    tMalfLeakRateFlag     = true;
    tMalfLeakRateValue    = 0.001;
    tPort0                = 0;
    tPort1                = 1;

    /// - Setup the fluid nodes
    tFluidProperties = new DefinedFluidProperties();
    FluidProperties::FluidType types[2];
    types[0]    = FluidProperties::GUNNS_N2;
    types[1]    = FluidProperties::GUNNS_O2;
    tFractions = new double[2];
    tFractions[0] = 0.5;
    tFractions[1] = 0.5;
    tFluidConfig = new PolyFluidConfigData(tFluidProperties, types, 2);
    tFluidInput1 = new PolyFluidInputData(290.0,                    //temperature
                                          700.728,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          tFractions);              //massFractions
    tFluidInput2 = new PolyFluidInputData(283.15,                   //temperature
                                          689.475,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          tFractions);              //massFractions

    /// - Have to initialize the nodes with the fluid configs (normally done by GUNNS)
    tNodes[0].initialize("UtTestNode0", tFluidConfig);
    tNodes[1].initialize("UtTestNode1", tFluidConfig);

    tNodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput1);
    tNodes[1].getContent()->initialize(*tFluidConfig, *tFluidInput2);

    /// - Define nominal configuration data
    tConfigData = new GunnsFluidSocketConfigData(tLinkName, &tNodeList, tMaxConductivity,
                                                 tExpansionScaleFactor, tSelfSealing);

    /// - Create nominal input data
    tInputData  = new GunnsFluidSocketInputData(true, tBlockage, tMalfLeakRateFlag,
                                                tMalfLeakRateValue);

    /// - Create the test article
    tArticle    = new FriendlyGunnsFluidSocket;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSocket::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsFluidSocket ..... 01: testConfig ............................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tLinkName             == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductivity      == tConfigData->mMaxConductivity);
    CPPUNIT_ASSERT(tExpansionScaleFactor == tConfigData->mExpansionScaleFactor);
    CPPUNIT_ASSERT(tSelfSealing          == tConfigData->mSelfSealing);

    /// - Check default config construction
    GunnsFluidSocketConfigData defaultConfig;
    CPPUNIT_ASSERT(""                    == defaultConfig.mName);
    CPPUNIT_ASSERT(0                     == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mMaxConductivity);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(false                 == defaultConfig.mSelfSealing);

    /// - Check copy config construction
    GunnsFluidSocketConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tLinkName             == copyConfig.mName);
    CPPUNIT_ASSERT(tNodes                == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductivity      == copyConfig.mMaxConductivity);
    CPPUNIT_ASSERT(tExpansionScaleFactor == copyConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(tSelfSealing          == copyConfig.mSelfSealing);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the input data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSocket::testInput()
{
    std::cout << "\n UtGunnsFluidSocket ..... 02: testInput .............................";

    /// - Check nominal input construction
    CPPUNIT_ASSERT(true               == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tBlockage          == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tMalfLeakRateFlag  == tInputData->mMalfLeakRateFlag);
    CPPUNIT_ASSERT(tMalfLeakRateValue == tInputData->mMalfLeakRateValue);

    /// - Check default input construction
    GunnsFluidSocketInputData defaultInput;
    CPPUNIT_ASSERT(false              == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(false              == defaultInput.mMalfLeakRateFlag);
    CPPUNIT_ASSERT(0.0                == defaultInput.mMalfLeakRateValue);

    /// - Check copy input construction
    GunnsFluidSocketInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageValue == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(tInputData->mMalfLeakRateFlag  == copyInput.mMalfLeakRateFlag);
    CPPUNIT_ASSERT(tInputData->mMalfLeakRateValue == copyInput.mMalfLeakRateValue);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSocket::testDefaultConstruction()
{
    std::cout << "\n UtGunnsFluidSocket ..... 03: testDefaultConstruction ...............";

    /// @test state data
    CPPUNIT_ASSERT(0.0   == tArticle->mEffectiveConductivity);
    CPPUNIT_ASSERT(0.0   == tArticle->mMaxConductivity);
    CPPUNIT_ASSERT(0.0   == tArticle->mSystemConductance);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfBlockageValue);
    CPPUNIT_ASSERT(false == tArticle->mSelfSealing);
    CPPUNIT_ASSERT(0     == tArticle->mConnection);
    CPPUNIT_ASSERT(false == tArticle->mHasConnection);
    CPPUNIT_ASSERT(false == tArticle->mMalfLeakRateFlag);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfLeakRateValue);
    CPPUNIT_ASSERT(0.0   == tArticle->mPreviousLeakRate);
    CPPUNIT_ASSERT(0.0   == tArticle->mLeakConductivity);

    /// @test init flag
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    /// @test new/delete for code coverage
    GunnsFluidSocket* article = new GunnsFluidSocket();
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSocket::testNominalInitialization()
{
    std::cout << "\n UtGunnsFluidSocket ..... 04: testNominalInitialization .............";

    /// - Initialize with input data and check it
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    CPPUNIT_ASSERT(tMaxConductivity     == tArticle->mEffectiveConductivity);
    CPPUNIT_ASSERT(tMaxConductivity     == tArticle->mMaxConductivity);
    CPPUNIT_ASSERT(tBlockage            == tArticle->mMalfBlockageValue);
    CPPUNIT_ASSERT(tSelfSealing         == tArticle->mSelfSealing);
    CPPUNIT_ASSERT(0                    == tArticle->mConnection);
    CPPUNIT_ASSERT(false                == tArticle->mHasConnection);
    CPPUNIT_ASSERT(tMalfLeakRateFlag    == tArticle->mMalfLeakRateFlag);
    CPPUNIT_ASSERT(tMalfLeakRateValue   == tArticle->mMalfLeakRateValue);
    CPPUNIT_ASSERT(0.0                  == tArticle->mPreviousLeakRate);
    CPPUNIT_ASSERT(0.0                  == tArticle->mLeakConductivity);
    CPPUNIT_ASSERT(tArticle->mInitFlag);

    /// - Verify functionality of untested restartModel
    tArticle->mEffectiveConductivity = 1.0;
    tArticle->mSystemConductance     = 1.0;

    tArticle->restartModel();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mEffectiveConductivity, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSystemConductance,     DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSocket::testInitializationExceptions()
{
    std::cout << "\n UtGunnsFluidSocket ..... 05: testInitializationExceptions ..........";

    /// - Verify an exception is thrown when leak malfunction value is < 0.
    tInputData->mMalfLeakRateValue = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the requestConnection method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSocket::testRequestConnection()
{
    std::cout << "\n UtGunnsFluidSocket ..... 06: testRequestConnection .................";

    /// - Create fake jumper plugs.
    FriendlyGunnsFluidJumperPlug plug1("plug1");
    FriendlyGunnsFluidJumperPlug plug2("plug2");

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
void UtGunnsFluidSocket::testRequestDisconnection()
{
    std::cout << "\n UtGunnsFluidSocket ..... 07: testRequestDisconnection ..............";

    /// - Create fake jumper plugs.
    FriendlyGunnsFluidJumperPlug plug1("plug1");
    FriendlyGunnsFluidJumperPlug plug2("plug2");

    /// - Verify that an uninitialized socket throws an exception if connection is requested.
    CPPUNIT_ASSERT_THROW(tArticle->requestDisconnection(&plug1), TsInitializationException);

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Connect plug1 and verify disconnection of plug2.  Since plug2 is not the one
    ///   connected, the socket should return the ambient node, and leave plug1 connected.
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
void UtGunnsFluidSocket::testAccessors()
{
    std::cout << "\n UtGunnsFluidSocket ..... 08: testAccessors .........................";

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Verify the isConnected method returns false if the requestor is not connected.
    FriendlyGunnsFluidJumperPlug plug("plug");
    CPPUNIT_ASSERT(!tArticle->isConnected(&plug));

    /// - Verify the isConnected method returns false if no plug is connected.
    CPPUNIT_ASSERT(!tArticle->isConnected());

    /// - Verify the isConnected method returns true if the requestor is connected.
    tArticle->requestConnection(&plug);
    CPPUNIT_ASSERT(tArticle->isConnected(&plug));

    /// - Verify the isConnected method returns true if any plug is connected.
    CPPUNIT_ASSERT(tArticle->isConnected());

    /// @test that the leak rate malfunction is set
    tArticle->setMalfLeakRate(true, 1.0);
    CPPUNIT_ASSERT(true  == tArticle->mMalfLeakRateFlag);
    CPPUNIT_ASSERT(1.0   == tArticle->mMalfLeakRateValue);

    /// @test that the leak rate malfunction is reset
    tArticle->setMalfLeakRate();
    CPPUNIT_ASSERT(false == tArticle->mMalfLeakRateFlag);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfLeakRateValue);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the interaction of the base class step method with the socket's updateState
///           method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSocket::testStep()
{
    std::cout << "\n UtGunnsFluidSocket ..... 09: testStep ..............................";

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Verify that the self-sealing socket with no connection and leak-thru malf active has the
    ///   leak-thru conductivity.
    tArticle->step(0.0);
    double leakConductivity = tArticle->mLeakConductivity;
    CPPUNIT_ASSERT(0.0 < leakConductivity);
    CPPUNIT_ASSERT(tArticle->mPreviousLeakRate == tMalfLeakRateValue);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(leakConductivity * (1.0 - tBlockage),
                                 tArticle->mEffectiveConductivity, DBL_EPSILON);

    /// - Verify that a non-self-sealing socket with no connection has the max conductivity.
    tArticle->mSelfSealing = false;
    tArticle->step(0.0);
    CPPUNIT_ASSERT(leakConductivity == tArticle->mLeakConductivity);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMaxConductivity * (1.0 - tBlockage),
                                 tArticle->mEffectiveConductivity, DBL_EPSILON);

    /// - Verify that a non-self-sealing socket with a connection has the leak-thru conductivity.
    FriendlyGunnsFluidJumperPlug plug("plug");
    tArticle->requestConnection(&plug);
    tArticle->step(0.0);
    CPPUNIT_ASSERT(leakConductivity == tArticle->mLeakConductivity);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(leakConductivity * (1.0 - tBlockage),
                                 tArticle->mEffectiveConductivity, DBL_EPSILON);

    /// - Verify that a self-sealing socket with a connection and no leak has no conductivity.
    tArticle->mSelfSealing      = true;
    tArticle->mMalfLeakRateFlag = false;
    tArticle->step(0.0);
    CPPUNIT_ASSERT(leakConductivity == tArticle->mLeakConductivity);
    CPPUNIT_ASSERT(0.0 == tArticle->mEffectiveConductivity);

    /// - Verify the leak conductivity is preserved when the malfunction is re-activated.
    tArticle->mMalfLeakRateFlag = true;
    tArticle->step(0.0);
    CPPUNIT_ASSERT(leakConductivity == tArticle->mLeakConductivity);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(leakConductivity * (1.0 - tBlockage),
                                 tArticle->mEffectiveConductivity, DBL_EPSILON);

    /// - Verify the leak conductivity is limited to max conductivity.
    tArticle->mMalfLeakRateValue = 1000.0;
    tArticle->step(0.0);
    CPPUNIT_ASSERT(tArticle->mPreviousLeakRate == 1000.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMaxConductivity * (1.0 - tBlockage),
                                 tArticle->mEffectiveConductivity, DBL_EPSILON);

    std::cout << "... Pass";
}
