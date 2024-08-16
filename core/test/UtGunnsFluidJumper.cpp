/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((core/GunnsFluidJumper.o))
***************************************************************************************************/
#include "UtGunnsFluidJumper.hh"
#include "UtGunnsFluidJumperPlug.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsFluidJumper class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidJumper::UtGunnsFluidJumper()
    :
    tArticle(),
    tLinkName(),
    tMaxConductivity(),
    tExpansionScaleFactor(),
    tJumperPlug0(),
    tJumperPlug1(),
    tSelfSealing0(),
    tSelfSealing1(),
    tConfigData(),
    tBlockage(),
    tMalfLeakRateFlag(),
    tMalfLeakRateValue(),
    tConnection0(),
    tConnection1(),
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
    tFluidInput3(),
    tFractions(),
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
/// @details  This is the default destructor for the UtGunnsFluidJumper class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidJumper::~UtGunnsFluidJumper()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidJumper::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete tFluidInput3;
    delete tFluidInput2;
    delete tFluidInput1;
    delete tFluidConfig;
    delete [] tFractions;
    delete tFluidProperties;
    delete tJumperPlug1;
    delete tJumperPlug0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidJumper::setUp()
{
    tLinkName             = "Test Fluid Jumper";
    tNodeList.mNumNodes   = 3;
    tNodeList.mNodes      = tNodes;
    tMaxConductivity      = 5.0E-4;
    tExpansionScaleFactor = 0.4;
    tSelfSealing0         = true;
    tSelfSealing1         = true;
    tBlockage             = 0.2;
    tMalfLeakRateFlag     = true;
    tMalfLeakRateValue    = 0.001;
    tConnection0          = 0;
    tConnection1          = 3;
    tPort0                = 0;
    tPort1                = 1;

    /// - Set up plugs
    tJumperPlug0 = new FriendlyGunnsFluidJumperPlug("Test Plug 0");
    tJumperPlug1 = new FriendlyGunnsFluidJumperPlug("Test Plug 1");
    tJumperPlug0->addSocket(tSocket0A);
    tJumperPlug0->addSocket(tSocket0B);
    tJumperPlug0->addSocket(tSocket0C);
    tJumperPlug1->addSocket(tSocket1A);
    tJumperPlug1->addSocket(tSocket1B);
    tJumperPlug1->addSocket(tSocket1C);

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
    tFluidInput3 = new PolyFluidInputData(283.15,                   //temperature
                                          FLT_EPSILON,              //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          tFractions);              //massFractions

    /// - Have to initialize the nodes with the fluid configs (normally done by GUNNS)
    tNodes[0].initialize("UtTestNode0", tFluidConfig);
    tNodes[1].initialize("UtTestNode1", tFluidConfig);
    tNodes[2].initialize("UtTestNode2", tFluidConfig);

    tNodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput1);
    tNodes[1].getContent()->initialize(*tFluidConfig, *tFluidInput2);
    tNodes[2].getContent()->initialize(*tFluidConfig, *tFluidInput3);

    /// - Initialize the sockets
    GunnsFluidSocketConfigData socketConfig("Test Socket", &tNodeList, 1.0E-6, 0.0, false);
    GunnsFluidSocketInputData  socketInput (false, 0.0, false, 0.0);

    tSocket0A.initialize(socketConfig, socketInput, tLinks, 0, 2);
    tSocket0B.initialize(socketConfig, socketInput, tLinks, 1, 2);
    tSocket0C.initialize(socketConfig, socketInput, tLinks, 1, 0);
    tSocket1A.initialize(socketConfig, socketInput, tLinks, 0, 2);
    tSocket1B.initialize(socketConfig, socketInput, tLinks, 2, 2);
    tSocket1C.initialize(socketConfig, socketInput, tLinks, 1, 0);

    /// - Define nominal configuration data
    tConfigData = new GunnsFluidJumperConfigData(tLinkName, &tNodeList, tMaxConductivity,
                                                 tExpansionScaleFactor, tJumperPlug0, tJumperPlug1,
                                                 tSelfSealing0, tSelfSealing1);

    /// - Create nominal input data
    tInputData  = new GunnsFluidJumperInputData(true, tBlockage, tMalfLeakRateFlag,
                                                tMalfLeakRateValue, tConnection0, tConnection1);

    /// - Create the test article
    tArticle    = new FriendlyGunnsFluidJumper;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidJumper::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsFluidJumper ..... 01: testConfig ............................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tLinkName             == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductivity      == tConfigData->mMaxConductivity);
    CPPUNIT_ASSERT(tExpansionScaleFactor == tConfigData->mExpansionScaleFactor);
    CPPUNIT_ASSERT(tJumperPlug0          == tConfigData->mPlug0);
    CPPUNIT_ASSERT(tJumperPlug1          == tConfigData->mPlug1);
    CPPUNIT_ASSERT(tSelfSealing0         == tConfigData->mSelfSealing0);
    CPPUNIT_ASSERT(tSelfSealing1         == tConfigData->mSelfSealing1);

    /// - Check default config construction
    GunnsFluidJumperConfigData defaultConfig;
    CPPUNIT_ASSERT(""                    == defaultConfig.mName);
    CPPUNIT_ASSERT(0                     == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mMaxConductivity);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(0                     == defaultConfig.mPlug0);
    CPPUNIT_ASSERT(0                     == defaultConfig.mPlug1);
    CPPUNIT_ASSERT(false                 == defaultConfig.mSelfSealing0);
    CPPUNIT_ASSERT(false                 == defaultConfig.mSelfSealing1);

    /// - Check copy config construction
    GunnsFluidJumperConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tLinkName             == copyConfig.mName);
    CPPUNIT_ASSERT(tNodes                == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductivity      == copyConfig.mMaxConductivity);
    CPPUNIT_ASSERT(tExpansionScaleFactor == copyConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(tJumperPlug0          == copyConfig.mPlug0);
    CPPUNIT_ASSERT(tJumperPlug1          == copyConfig.mPlug1);
    CPPUNIT_ASSERT(tSelfSealing0         == copyConfig.mSelfSealing0);
    CPPUNIT_ASSERT(tSelfSealing1         == copyConfig.mSelfSealing1);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the input data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidJumper::testInput()
{
    std::cout << "\n UtGunnsFluidJumper ..... 02: testInput .............................";

    /// - Check nominal input construction
    CPPUNIT_ASSERT(true               == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tBlockage          == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tMalfLeakRateFlag  == tInputData->mMalfLeakRateFlag);
    CPPUNIT_ASSERT(tMalfLeakRateValue == tInputData->mMalfLeakRateValue);
    CPPUNIT_ASSERT(tConnection0       == tInputData->mConnection0);
    CPPUNIT_ASSERT(tConnection1       == tInputData->mConnection1);

    /// - Check default input construction
    GunnsFluidJumperInputData defaultInput;
    CPPUNIT_ASSERT(false              == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(false              == defaultInput.mMalfLeakRateFlag);
    CPPUNIT_ASSERT(0.0                == defaultInput.mMalfLeakRateValue);
    CPPUNIT_ASSERT(-1                 == defaultInput.mConnection0);
    CPPUNIT_ASSERT(-1                 == defaultInput.mConnection1);

    /// - Check copy input construction
    GunnsFluidJumperInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageValue == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(tInputData->mMalfLeakRateFlag  == copyInput.mMalfLeakRateFlag);
    CPPUNIT_ASSERT(tInputData->mMalfLeakRateValue == copyInput.mMalfLeakRateValue);
    CPPUNIT_ASSERT(tInputData->mConnection0       == copyInput.mConnection0);
    CPPUNIT_ASSERT(tInputData->mConnection1       == copyInput.mConnection1);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidJumper::testDefaultConstruction()
{
    std::cout << "\n UtGunnsFluidJumper ..... 03: testDefaultConstruction ...............";

    /// @test state data
    CPPUNIT_ASSERT(0.0   == tArticle->mEffectiveConductivity);
    CPPUNIT_ASSERT(0.0   == tArticle->mMaxConductivity);
    CPPUNIT_ASSERT(0.0   == tArticle->mSystemConductance);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfBlockageValue);
    CPPUNIT_ASSERT(0     == tArticle->mPlug[0]);
    CPPUNIT_ASSERT(0     == tArticle->mPlug[1]);
    CPPUNIT_ASSERT(false == tArticle->mSelfSealing0);
    CPPUNIT_ASSERT(false == tArticle->mSelfSealing1);
    CPPUNIT_ASSERT(false == tArticle->mMalfLeakRateFlag);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfLeakRateValue);
    CPPUNIT_ASSERT(0.0   == tArticle->mPreviousLeakRate);
    CPPUNIT_ASSERT(0.0   == tArticle->mLeakConductivity);

    /// @test init flag
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    /// @test new/delete for code coverage
    GunnsFluidJumper* article = new GunnsFluidJumper();
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidJumper::testNominalInitialization()
{
    std::cout << "\n UtGunnsFluidJumper ..... 04: testNominalInitialization .............";

    /// - Initialize with input data and check it.  In this case we'll configure the jumper to not
    ///   have a plug on port 1, so we can see both cases of a port and no port.
    tConfigData->mPlug1 = 0;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    CPPUNIT_ASSERT(tMaxConductivity     == tArticle->mEffectiveConductivity);
    CPPUNIT_ASSERT(tMaxConductivity     == tArticle->mMaxConductivity);
    CPPUNIT_ASSERT(tBlockage            == tArticle->mMalfBlockageValue);
    CPPUNIT_ASSERT(tJumperPlug0         == tArticle->mPlug[0]);
    CPPUNIT_ASSERT(0                    == tArticle->mPlug[1]);
    CPPUNIT_ASSERT(tSelfSealing0        == tArticle->mSelfSealing0);
    CPPUNIT_ASSERT(tSelfSealing1        == tArticle->mSelfSealing1);
    CPPUNIT_ASSERT(tMalfLeakRateFlag    == tArticle->mMalfLeakRateFlag);
    CPPUNIT_ASSERT(tMalfLeakRateValue   == tArticle->mMalfLeakRateValue);
    CPPUNIT_ASSERT(0.0                  == tArticle->mPreviousLeakRate);
    CPPUNIT_ASSERT(0.0                  == tArticle->mLeakConductivity);

    /// - Verify the initial socket connections
    CPPUNIT_ASSERT(0                    == tArticle->mNodeMap[0]);
    CPPUNIT_ASSERT(tPort1               == tArticle->mNodeMap[1]);
    CPPUNIT_ASSERT(FriendlyGunnsFluidJumperPlug::SOCKET_1 == tJumperPlug0->getActiveConnection());

    CPPUNIT_ASSERT(tArticle->mInitFlag);

    /// - Verify untested RestartModel functionality
    tArticle->mEffectiveConductivity = 1.0;
    tArticle->mSystemConductance     = 1.0;

    tArticle->restartModel();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mEffectiveConductivity, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSystemConductance,     DBL_EPSILON);
    CPPUNIT_ASSERT(tArticle->mPlug[0]->getConnectionRequest() == tArticle->mPlug[0]->getDisconnectionRequest());

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidJumper::testInitializationExceptions()
{
    std::cout << "\n UtGunnsFluidJumper ..... 05: testInitializationExceptions ..........";

    /// - Verify an exception is thrown when leak malfunction value is < 0.
    tInputData->mMalfLeakRateValue = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test setter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidJumper::testModifiers()
{
    std::cout << "\n UtGunnsFluidJumper ..... 06: testModifiers .........................";

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

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
/// @details  Tests the interaction of the base class step method with the jumper's updateState
///           method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidJumper::testStep()
{
    std::cout << "\n UtGunnsFluidJumper ..... 07: testStep ..............................";

    /// - Initialize default test article with nominal initialization data.
    tInputData->mConnection0 = 0;
    tInputData->mConnection1 = 2;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Verify that a jumper with both plugs connected has the correct conductivity.
    tArticle->step(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMaxConductivity * (1.0 - tBlockage),
                                 tArticle->mEffectiveConductivity, DBL_EPSILON);

    /// - Verify that a jumper with self-sealing plug 0 that is disconnected with the leak-thru malf
    ///   active has the leak conductivity.
    tJumperPlug0->setDisconnectionRequest(0);
    tArticle->step(0.0);
    double leakConductivity = tArticle->mLeakConductivity;
    CPPUNIT_ASSERT(0.0                  <  leakConductivity);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(leakConductivity * (1.0 - tBlockage),
                                 tArticle->mEffectiveConductivity, DBL_EPSILON);

    /// - Verify that a jumper with a non-self-sealing plug 0 that is disconnected has conductivity.
    tArticle->mSelfSealing0 = false;
    tArticle->step(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMaxConductivity * (1.0 - tBlockage),
                                 tArticle->mEffectiveConductivity, DBL_EPSILON);

    /// - Verify that a jumper with missing plug 0 has conductivity.
    tArticle->mPlug[0] = 0;
    tArticle->step(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMaxConductivity * (1.0 - tBlockage),
                                 tArticle->mEffectiveConductivity, DBL_EPSILON);
    tArticle->mPlug[0] = tJumperPlug0;

    /// - Verify that a jumper with self-sealing plug 1 that is disconnected has leak conductivity.
    tJumperPlug1->setDisconnectionRequest(2);
    tArticle->step(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(leakConductivity * (1.0 - tBlockage),
                                 tArticle->mEffectiveConductivity, DBL_EPSILON);

    /// - Verify that a jumper with a non-self-sealing plug 1 that is disconnected has conductivity.
    tArticle->mSelfSealing1 = false;
    tArticle->step(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMaxConductivity * (1.0 - tBlockage),
                                 tArticle->mEffectiveConductivity, DBL_EPSILON);

    /// - Verify that a jumper with missing plug 1 has conductivity.
    tArticle->mPlug[1] = 0;
    tArticle->step(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMaxConductivity * (1.0 - tBlockage),
                                 tArticle->mEffectiveConductivity, DBL_EPSILON);
    tArticle->mPlug[1] = tJumperPlug1;

    /// - Verify that a jumper with both plugs connected to the same node has no conductivity.
    tJumperPlug0->setConnectionRequest(0);
    tJumperPlug1->setConnectionRequest(0);
    tArticle->step(0.0);
    CPPUNIT_ASSERT(0.0 == tArticle->mEffectiveConductivity);

    std::cout << "... Pass";
}
