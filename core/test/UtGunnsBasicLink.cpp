/*
@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/

#include "UtGunnsBasicLink.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsBasicLink class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsBasicLink::UtGunnsBasicLink()
    :
    mConfigData(),
    mInputData(),
    mArticle(),
    mLinkName(),
    mNodes(),
    mNodeList(),
    mLinks(),
    mPortMap(),
    mNumPorts(),
    mTimeStep(),
    mTolerance()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsBasicLink class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsBasicLink::~UtGunnsBasicLink()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicLink::tearDown()
{
    /// - Deletes for news in setUp
    delete mArticle;
    delete mInputData;
    delete mConfigData;
    delete [] mPortMap;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicLink::setUp()
{
    mLinkName = "New Link";
    mNumPorts = 2;
    mPortMap = new int[mNumPorts];
    mPortMap[0] = 0;
    mPortMap[1] = 1;
    mNodeList.mNumNodes = 2;
    mNodeList.mNodes    = mNodes;

    /// - Define nominal configuration data
    mConfigData = new GunnsBasicLinkConfigData(mLinkName, &mNodeList);

    /// - Define nominal input data with the blockage malfunction active
    mInputData  = new GunnsBasicLinkInputData(true, 0.5);

    /// - Create the test article
    mArticle = new FriendlyGunnsBasicLink(mNumPorts);

    /// - Declare the nominal test data
    mTolerance = 1.0e-08;
    mTimeStep = 0.1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicLink::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsBasicLink ....... 01: testConfig ............................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(mLinkName == mConfigData->mName);

    /// - Check default config construction
    GunnsBasicLinkConfigData defaultConfig;
    CPPUNIT_ASSERT("" == defaultConfig.mName);

    /// - Check copy config construction
    GunnsBasicLinkConfigData copyConfig(*mConfigData);
    CPPUNIT_ASSERT(mLinkName == copyConfig.mName);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicLink::testInput()
{
    std::cout << "\n UtGunnsBasicLink ....... 02: testInput .............................";

    /// - Check nominal input construction
    CPPUNIT_ASSERT(0                              == mInputData->mInitialNodeMap);
    CPPUNIT_ASSERT(true                           == mInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.5                            == mInputData->mMalfBlockageValue);

    /// - Check default input construction
    GunnsBasicLinkInputData defaultInput;
    CPPUNIT_ASSERT(0                              == defaultInput.mInitialNodeMap);
    CPPUNIT_ASSERT(false                          == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                            == defaultInput.mMalfBlockageValue);

    /// - Check copy input construction
    int* map = new int[mNumPorts];
    mInputData->mInitialNodeMap = map;
    GunnsBasicLinkInputData copyInput(*mInputData);
    CPPUNIT_ASSERT(map                            == copyInput.mInitialNodeMap);
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(mInputData->mMalfBlockageValue == copyInput.mMalfBlockageValue);
    delete [] map;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicLink::testDefaultConstruction()
{
    std::cout << "\n UtGunnsBasicLink ....... 03: testDefaultConstruction ...............";

    CPPUNIT_ASSERT(""                    == mArticle->mName);
    CPPUNIT_ASSERT(0                     == mArticle->mAdmittanceMatrix);
    CPPUNIT_ASSERT(0                     == mArticle->mNodes);
    CPPUNIT_ASSERT(0                     == mArticle->mNodeList);
    CPPUNIT_ASSERT(0                     == mArticle->mPotentialVector);
    CPPUNIT_ASSERT(0                     == mArticle->mSourceVector);
    CPPUNIT_ASSERT(0                     == mArticle->mNodeMap);
    CPPUNIT_ASSERT(0                     == mArticle->mOverrideVector);
    CPPUNIT_ASSERT(mNumPorts             == mArticle->mNumPorts);
    CPPUNIT_ASSERT(false                 == mArticle->mAdmittanceUpdate);
    CPPUNIT_ASSERT(0.0                   == mArticle->mFlux);
    CPPUNIT_ASSERT(0.0                   == mArticle->mPotentialDrop);
    CPPUNIT_ASSERT(0.0                   == mArticle->mPower);
    CPPUNIT_ASSERT(1.0                   == mArticle->mMinLinearizationPotential);
    CPPUNIT_ASSERT(false                 == mArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                   == mArticle->mMalfBlockageValue);
    CPPUNIT_ASSERT(false                 == mArticle->mInitFlag);
    CPPUNIT_ASSERT(-1                    == mArticle->mUserPortSelect);
    CPPUNIT_ASSERT(-1                    == mArticle->mUserPortSelectNode);
    CPPUNIT_ASSERT(GunnsBasicLink::READY == mArticle->mUserPortSetControl);

    /// @test new/delete the admittance map for code coverage
    GunnsBasicLinkAdmittanceMap* map = new GunnsBasicLinkAdmittanceMap();
    map->allocateMap("map", 4);
    map->freeMap();
    delete map;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicLink::testNominalInitialization()
{
    std::cout << "\n UtGunnsBasicLink ....... 04: testNominalInitialization .............";

    /// - Construct and initialize (with nominal data) a test article
    FriendlyGunnsBasicLink* article = new FriendlyGunnsBasicLink(mNumPorts);
    article->initialize(*mConfigData, *mInputData, mLinks, mPortMap);

    CPPUNIT_ASSERT(mLinkName == article->getName());
    CPPUNIT_ASSERT(article   == mLinks[0]);

    for (int i = 0; i < mNumPorts; ++i) {
        CPPUNIT_ASSERT(mPortMap[i] == article->mNodeMap[i]);
        CPPUNIT_ASSERT(mPortMap[i] == article->mDefaultNodeMap[i]);
    }

    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag == article->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfBlockageValue, article->mMalfBlockageValue, 0.0);
    CPPUNIT_ASSERT(-1                    == mArticle->mUserPortSelect);
    CPPUNIT_ASSERT(-1                    == mArticle->mUserPortSelectNode);
    CPPUNIT_ASSERT(GunnsBasicLink::READY == mArticle->mUserPortSetControl);

    /// @test init flag
    CPPUNIT_ASSERT(article->mInitFlag);

    delete article;

    /// - Initialize the default test article using the initial node map override array.
    int map[2] = {mPortMap[1], mPortMap[0]};
    mInputData->mInitialNodeMap = map;
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPortMap);
    CPPUNIT_ASSERT(mPortMap[1] == mArticle->mNodeMap[0]);
    CPPUNIT_ASSERT(mPortMap[0] == mArticle->mNodeMap[1]);
    CPPUNIT_ASSERT(mPortMap[1] == mArticle->mDefaultNodeMap[0]);
    CPPUNIT_ASSERT(mPortMap[0] == mArticle->mDefaultNodeMap[1]);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for initialization exceptions on invalid properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicLink::testInitializationExceptions()
{
    std::cout << "\n UtGunnsBasicLink ....... 05: testInitializationExceptions ..........";

    /// - Default construct a test article
    FriendlyGunnsBasicLink* article = new FriendlyGunnsBasicLink(mNumPorts);

    /// @test for exception on null port map
    int* portMap = 0;
    CPPUNIT_ASSERT_THROW(article->initialize(*mConfigData, *mInputData, mLinks, portMap),
                         TsInitializationException);
    portMap = new int[mNumPorts];

    /// @test for exception on invalid name
    mConfigData->mName = "";
    CPPUNIT_ASSERT_THROW(article->initialize(*mConfigData, *mInputData, mLinks, portMap),
                         TsInitializationException);
    mConfigData->mName = "New Link";

    /// @test for exception on invalid number of ports.
    FriendlyGunnsBasicLink* article2 = new FriendlyGunnsBasicLink(0);
    CPPUNIT_ASSERT_THROW(article2->initialize(*mConfigData, *mInputData, mLinks, portMap),
                         TsInitializationException);

    /// @test for exception on invalid port assignment.
    portMap[0] =  0;
    portMap[1] = -1;
    CPPUNIT_ASSERT_THROW(article->initialize(*mConfigData, *mInputData, mLinks, portMap),
                         TsInitializationException);

    /// @test for exception on duplicate port assignments.
    portMap[1] =  0;
    CPPUNIT_ASSERT_THROW(article->initialize(*mConfigData, *mInputData, mLinks, portMap),
                         TsInitializationException);

    /// @test for exception on invalid blockage value.
    portMap[1] =  1;
    mInputData->mMalfBlockageValue = -0.1;
    CPPUNIT_ASSERT_THROW(article->initialize(*mConfigData, *mInputData, mLinks, portMap),
                         TsInitializationException);

    mInputData->mMalfBlockageValue = 1.1;
    CPPUNIT_ASSERT_THROW(article->initialize(*mConfigData, *mInputData, mLinks, portMap),
                         TsInitializationException);

    /// @test for exception on null node pointers.
    mInputData->mMalfBlockageValue = 0.5;
    portMap[1] =  1;
    mConfigData->mNodeList->mNodes = 0;
    CPPUNIT_ASSERT_THROW(article->initialize(*mConfigData, *mInputData, mLinks, portMap),
                         TsInitializationException);

    /// @test for exception on null node list.
    mConfigData->mNodeList = 0;
    CPPUNIT_ASSERT_THROW(article->initialize(*mConfigData, *mInputData, mLinks, portMap),
                         TsInitializationException);

    delete article;
    delete article2;
    delete [] portMap;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicLink::testAccessors()
{
    std::cout << "\n UtGunnsBasicLink ....... 06: testAccessors .........................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPortMap);

    /// @test to make sure it returns the correct string name
    CPPUNIT_ASSERT_EQUAL(mLinkName, std::string(mArticle->getName()));

    /// @test to make sure it returns the proper node Map
    for (int i = 0; i < mNumPorts; ++i) {
        CPPUNIT_ASSERT_EQUAL(mPortMap[i], mArticle->getNodeMap()[i]);
    }

    /// @test to make sure it returns values in the admittance matrix
    for (int i = 0; i < mNumPorts * mNumPorts; ++i) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->getAdmittanceMatrix()[i], mTolerance);
    }

    /// @test to make sure it returns the source, potential and override vectors
    for (int i = 0; i < mNumPorts; ++i) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->getSourceVector()[i], mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->getPotentialVector()[i], mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->getOverrideVector()[i], mTolerance);
    }

    /// @test to make sure it returns the number of ports
    CPPUNIT_ASSERT_EQUAL(mNumPorts, mArticle->getNumberPorts());

    /// @test to make sure it returns the admittance update flag
    CPPUNIT_ASSERT_EQUAL(false, mArticle->needAdmittanceUpdate());

    /// @test to make sure it returns the delta Potential
    mArticle->mPotentialVector[1] = 120.0;
    mArticle->mPotentialVector[0] =   0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-120.0, mArticle->getDeltaPotential( 0,  1),  0.0);

    /// @test getDeltaPotential throws exceptions with invalid port arguments
    CPPUNIT_ASSERT_THROW(mArticle->getDeltaPotential(-1,  1), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle->getDeltaPotential( 2,  1), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle->getDeltaPotential( 0, -1), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle->getDeltaPotential( 0,  2), TsOutOfBoundsException);

    /// @test to make sure it computes the power
    mArticle->mFlux = 0.5;
    mArticle->computePower();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(60.0, mArticle->mPower, DBL_EPSILON);

    /// @test to make sure it returns the non-linear flag
    CPPUNIT_ASSERT_EQUAL(false, mArticle->isNonLinear());

    /// @test to make sure it returns the Initializer flag
    CPPUNIT_ASSERT_EQUAL(true, mArticle->isInitialized());

    /// @test to make sure it returns the solution acceptable flag
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == mArticle->confirmSolutionAcceptable(1, 1));

    /// @test to make sure it returns the reset last step flag
    CPPUNIT_ASSERT_EQUAL(true, mArticle->resetLastMinorStep(1, 1));

    /// @test to make sure getters work
    mArticle->mFlux = 5.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, mArticle->getFlux(), 0.0);

    mArticle->mPower = 5.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, mArticle->getPower(), 0.0);

    mArticle->mPotentialDrop = 5.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, mArticle->getPotentialDrop(), 0.0);

    /// @test the node map lock setters
    mArticle->lockNodeMap();
    CPPUNIT_ASSERT_EQUAL(true, mArticle->mNodeMapLocked);

    mArticle->unlockNodeMap();
    CPPUNIT_ASSERT_EQUAL(false, mArticle->mNodeMapLocked);

    /// - call the dummy placeholder methods that don't do or return anything, for code coverage.
    mArticle->updateState(0.0);
    mArticle->updateFlux(0.0, 0.0);
    mArticle->step(0.0);
    mArticle->minorStep(0.0, 2);
    mArticle->computeFlows(0.0);
    mArticle->processInputs();
    mArticle->processOutputs();

    /// @test checkValidNodeNumber
    CPPUNIT_ASSERT(false == mArticle->GunnsBasicLink::checkValidNodeNumber(-1));

    /// @test getPortDirection
    CPPUNIT_ASSERT(mArticle->mPortDirections == mArticle->GunnsBasicLink::getPortDirections());

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicLink::testModifiers()
{
    std::cout << "\n UtGunnsBasicLink ....... 07: testModifiers .........................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPortMap);

    /// @test for seeing if min linearization potential gets set
    const double minLinearP = 0.5;
    mArticle->setMinLinearizationPotential(minLinearP);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(minLinearP, mArticle->mMinLinearizationPotential, mTolerance);

    /// @test that the network name gets set
    char* name = new char[strlen(mLinkName.c_str())+1];
    strcpy(name, mLinkName.c_str());

    for (int i=0; i<strlen(mLinkName.c_str())+1; ++i) {
        CPPUNIT_ASSERT(name[i] == mArticle->mName[i]);
    }
    delete [] name;

    /// @test that the blockage malfunction is set
    mArticle->setMalfBlockage(true, 1.0);
    CPPUNIT_ASSERT(true  == mArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT(1.0   == mArticle->mMalfBlockageValue);

    /// @test that the blockage malfunction is reset
    mArticle->setMalfBlockage();
    CPPUNIT_ASSERT(false == mArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0   == mArticle->mMalfBlockageValue);

    /// @test that a port override is set from nominal values.
    mArticle->setPortOverride(0, 1.0);
    CPPUNIT_ASSERT(true  == mArticle->mOverrideVector[0]);
    CPPUNIT_ASSERT(1.0   == mArticle->mPotentialVector[0]);
    CPPUNIT_ASSERT(false == mArticle->mOverrideVector[1]);
    CPPUNIT_ASSERT(0.0   == mArticle->mPotentialVector[1]);

    mArticle->setPortOverride(1, 2.0);
    CPPUNIT_ASSERT(true  == mArticle->mOverrideVector[0]);
    CPPUNIT_ASSERT(1.0   == mArticle->mPotentialVector[0]);
    CPPUNIT_ASSERT(true  == mArticle->mOverrideVector[1]);
    CPPUNIT_ASSERT(2.0   == mArticle->mPotentialVector[1]);

    /// @test that a port override given bad port has no effect.
    mArticle->setPortOverride(2, 3.0);
    CPPUNIT_ASSERT(true  == mArticle->mOverrideVector[0]);
    CPPUNIT_ASSERT(1.0   == mArticle->mPotentialVector[0]);
    CPPUNIT_ASSERT(true  == mArticle->mOverrideVector[1]);
    CPPUNIT_ASSERT(2.0   == mArticle->mPotentialVector[1]);

    /// @test that a port override is reset.
    mArticle->resetPortOverride(0);
    CPPUNIT_ASSERT(false == mArticle->mOverrideVector[0]);
    CPPUNIT_ASSERT(1.0   == mArticle->mPotentialVector[0]);
    CPPUNIT_ASSERT(true  == mArticle->mOverrideVector[1]);
    CPPUNIT_ASSERT(2.0   == mArticle->mPotentialVector[1]);

    /// @test that a port override reset given bad port has no effect.
    mArticle->resetPortOverride(2);
    CPPUNIT_ASSERT(false == mArticle->mOverrideVector[0]);
    CPPUNIT_ASSERT(1.0   == mArticle->mPotentialVector[0]);
    CPPUNIT_ASSERT(true  == mArticle->mOverrideVector[1]);
    CPPUNIT_ASSERT(2.0   == mArticle->mPotentialVector[1]);

    /// @test that all port overrides are reset by default value.
    mArticle->resetPortOverride();
    CPPUNIT_ASSERT(false == mArticle->mOverrideVector[0]);
    CPPUNIT_ASSERT(1.0   == mArticle->mPotentialVector[0]);
    CPPUNIT_ASSERT(false == mArticle->mOverrideVector[1]);
    CPPUNIT_ASSERT(2.0   == mArticle->mPotentialVector[1]);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the checkPortRules method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicLink::testCheckPortRules()
{
    std::cout << "\n UtGunnsBasicLink ....... 08: testCheckPortRules ....................";

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPortMap);

    /// - Check for invalid port number requests.
    CPPUNIT_ASSERT(false == mArticle->checkPortRules(-1, 0));
    CPPUNIT_ASSERT(false == mArticle->checkPortRules( 2, 0));

    /// - Check for invalid node number requests.
    CPPUNIT_ASSERT(false == mArticle->checkPortRules( 0,-1));
    CPPUNIT_ASSERT(false == mArticle->checkPortRules( 1, 2));

    /// - Check for duplicate node requests.
    CPPUNIT_ASSERT(false == mArticle->checkPortRules( 1, 0));

    /// - Check nominal requests.
    CPPUNIT_ASSERT(true  == mArticle->checkPortRules( 0, 1));
    mArticle->setPort(0, 1);
    CPPUNIT_ASSERT(true  == mArticle->checkPortRules( 1, 0));

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the setPort method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicLink::testSetPort()
{
    std::cout << "\n UtGunnsBasicLink ....... 09: testSetPort ...........................";

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPortMap);

    /// - Swap port nodes and verify new node assignments.
    mArticle->setPort(0, 1);
    mArticle->setPort(1, 0);

    CPPUNIT_ASSERT(1 == mArticle->mNodeMap[0]);
    CPPUNIT_ASSERT(0 == mArticle->mNodeMap[1]);
    CPPUNIT_ASSERT(&mNodes[1] == mArticle->mNodes[0]);
    CPPUNIT_ASSERT(&mNodes[0] == mArticle->mNodes[1]);

    /// - Verify the admittance update flag has been set.
    CPPUNIT_ASSERT(true == mArticle->needAdmittanceUpdate());

    /// - Test the lock term prevents changes to node assignments.
    mArticle->lockNodeMap();
    bool result = mArticle->setPort(1, 1);
    CPPUNIT_ASSERT(false == result);
    CPPUNIT_ASSERT(0 == mArticle->mNodeMap[1]);
    CPPUNIT_ASSERT(&mNodes[0] == mArticle->mNodes[1]);

    mArticle->unlockNodeMap();
    result = mArticle->setPort(1, 1);
    CPPUNIT_ASSERT(true == result);
    CPPUNIT_ASSERT(1 == mArticle->mNodeMap[1]);
    CPPUNIT_ASSERT(&mNodes[1] == mArticle->mNodes[1]);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the transportFlux method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicLink::testTransportFlux()
{
    std::cout << "\n UtGunnsBasicLink ....... 10: testTransportFlux .....................";

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPortMap);

    /// - Test zero flux.
    mArticle->mFlux = 0.0;
    mArticle->transportFlux(0, 1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mNodes[0].getOutflux(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mNodes[1].getOutflux(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mNodes[0].getInflux(),  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mNodes[1].getInflux(),  0.0);

    mNodes[0].resetFlows();
    mNodes[1].resetFlows();

    /// - Test positive flux, flowing from port 0 to port 1.
    mArticle->mFlux = 1.0;
    mArticle->transportFlux(0, 1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, mNodes[0].getOutflux(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mNodes[1].getOutflux(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mNodes[0].getInflux(),  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, mNodes[1].getInflux(),  0.0);

    mNodes[0].resetFlows();
    mNodes[1].resetFlows();

    /// - Test negative flux, flowing from port 1 to port 0.
    mArticle->mFlux = -1.0;
    mArticle->transportFlux(0, 1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mNodes[0].getOutflux(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, mNodes[1].getOutflux(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, mNodes[0].getInflux(),  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mNodes[1].getInflux(),  0.0);

    mNodes[0].resetFlows();
    mNodes[1].resetFlows();

    /// - Test calls with invalid ports throw exceptions.
    CPPUNIT_ASSERT_THROW(mArticle->transportFlux(-1,  1), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle->transportFlux( 2,  1), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle->transportFlux( 0, -1), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle->transportFlux( 0,  2), TsOutOfBoundsException);

    /// - Call GunnsBasicLink::transportFlows for code coverage
    mArticle->GunnsBasicLink::transportFlows(0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the processUserPortCommand method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicLink::testUserPortCommand()
{
    std::cout << "\n UtGunnsBasicLink ....... 11: testUserPortCommand ...................";

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPortMap);

    /// - Use the user interface terms to swap port nodes and verify new node assignments.
    mArticle->mUserPortSelect     = 0;
    mArticle->mUserPortSelectNode = 1;
    mArticle->mUserPortSetControl = GunnsBasicLink::EXECUTE;
    mArticle->processUserPortCommand();

    CPPUNIT_ASSERT(-1                    == mArticle->mUserPortSelect);
    CPPUNIT_ASSERT(-1                    == mArticle->mUserPortSelectNode);
    CPPUNIT_ASSERT(GunnsBasicLink::READY == mArticle->mUserPortSetControl);
    CPPUNIT_ASSERT(1 == mArticle->mNodeMap[0]);
    CPPUNIT_ASSERT(&mNodes[1] == mArticle->mNodes[0]);

    mArticle->mUserPortSelect     = 1;
    mArticle->mUserPortSelectNode = 0;
    mArticle->mUserPortSetControl = GunnsBasicLink::EXECUTE;
    mArticle->processUserPortCommand();

    CPPUNIT_ASSERT(-1                    == mArticle->mUserPortSelect);
    CPPUNIT_ASSERT(-1                    == mArticle->mUserPortSelectNode);
    CPPUNIT_ASSERT(GunnsBasicLink::READY == mArticle->mUserPortSetControl);
    CPPUNIT_ASSERT(0 == mArticle->mNodeMap[1]);
    CPPUNIT_ASSERT(&mNodes[0] == mArticle->mNodes[1]);

    /// - Try a bad assignment and verify the user control indicates the error.
    mArticle->mUserPortSelect     = 2;
    mArticle->mUserPortSelectNode = 0;
    mArticle->mUserPortSetControl = GunnsBasicLink::EXECUTE;
    mArticle->processUserPortCommand();

    CPPUNIT_ASSERT(-1                     == mArticle->mUserPortSelect);
    CPPUNIT_ASSERT(-1                     == mArticle->mUserPortSelectNode);
    CPPUNIT_ASSERT(GunnsBasicLink::FAILED == mArticle->mUserPortSetControl);
    CPPUNIT_ASSERT(0 == mArticle->mNodeMap[1]);
    CPPUNIT_ASSERT(&mNodes[0] == mArticle->mNodes[1]);

    /// - Test the GROUND command.
    mArticle->mUserPortSelect     = 1;
    mArticle->mUserPortSetControl = GunnsBasicLink::GROUND;
    mArticle->processUserPortCommand();

    CPPUNIT_ASSERT(-1                    == mArticle->mUserPortSelect);
    CPPUNIT_ASSERT(-1                    == mArticle->mUserPortSelectNode);
    CPPUNIT_ASSERT(GunnsBasicLink::READY == mArticle->mUserPortSetControl);
    CPPUNIT_ASSERT(1 == mArticle->mNodeMap[1]);
    CPPUNIT_ASSERT(&mNodes[1] == mArticle->mNodes[1]);

    /// - Test the lock term prevents changes to node assignments.
    mArticle->lockNodeMap();
    mArticle->mUserPortSelect     = 1;
    mArticle->mUserPortSelectNode = 0;
    mArticle->mUserPortSetControl = GunnsBasicLink::EXECUTE;
    mArticle->processUserPortCommand();
    CPPUNIT_ASSERT(1 == mArticle->mNodeMap[1]);
    CPPUNIT_ASSERT(&mNodes[1] == mArticle->mNodes[1]);
    CPPUNIT_ASSERT(-1                    == mArticle->mUserPortSelect);
    CPPUNIT_ASSERT(-1                    == mArticle->mUserPortSelectNode);
    CPPUNIT_ASSERT(GunnsBasicLink::FAILED == mArticle->mUserPortSetControl);

    mArticle->unlockNodeMap();
    mArticle->mUserPortSelect     = 1;
    mArticle->mUserPortSelectNode = 0;
    mArticle->mUserPortSetControl = GunnsBasicLink::EXECUTE;
    mArticle->processUserPortCommand();
    CPPUNIT_ASSERT(0 == mArticle->mNodeMap[1]);
    CPPUNIT_ASSERT(&mNodes[0] == mArticle->mNodes[1]);
    CPPUNIT_ASSERT(-1                    == mArticle->mUserPortSelect);
    CPPUNIT_ASSERT(-1                    == mArticle->mUserPortSelectNode);
    CPPUNIT_ASSERT(GunnsBasicLink::READY == mArticle->mUserPortSetControl);

    /// - Test the DEFAULT command with a valid port.
    mArticle->mUserPortSelect     = 1;
    mArticle->mUserPortSetControl = GunnsBasicLink::DEFAULT;
    mArticle->processUserPortCommand();
    CPPUNIT_ASSERT(1 == mArticle->mNodeMap[1]);
    CPPUNIT_ASSERT(&mNodes[1] == mArticle->mNodes[1]);
    CPPUNIT_ASSERT(-1                    == mArticle->mUserPortSelect);
    CPPUNIT_ASSERT(-1                    == mArticle->mUserPortSelectNode);
    CPPUNIT_ASSERT(GunnsBasicLink::READY == mArticle->mUserPortSetControl);

    /// - Test the DEFAULT command with an invalid port.
    mArticle->mUserPortSelect     = 2;
    mArticle->mUserPortSetControl = GunnsBasicLink::DEFAULT;
    mArticle->processUserPortCommand();
    CPPUNIT_ASSERT(1 == mArticle->mNodeMap[1]);
    CPPUNIT_ASSERT(1 == mArticle->mNodeMap[1]);
    CPPUNIT_ASSERT(&mNodes[1] == mArticle->mNodes[1]);
    CPPUNIT_ASSERT(&mNodes[1] == mArticle->mNodes[1]);
    CPPUNIT_ASSERT(-1                    == mArticle->mUserPortSelect);
    CPPUNIT_ASSERT(-1                    == mArticle->mUserPortSelectNode);
    CPPUNIT_ASSERT(GunnsBasicLink::FAILED == mArticle->mUserPortSetControl);

    std::cout << "... Pass";
}
