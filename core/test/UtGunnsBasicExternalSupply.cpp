/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    (
        (core/GunnsBasicExternalSupply.o)
    )
***************************************************************************************************/
#include "UtGunnsBasicExternalSupply.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsBasicExternalSupply class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsBasicExternalSupply::UtGunnsBasicExternalSupply()
    :
    tConfigData(),
    tInputData(),
    tArticle(),
    tLinkName(),
    tInitialDemand(),
    tNodes(),
    tNodeList(),
    tLinks(),
    tPort0(),
    tPort1(),
    tTimeStep()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsBasicExternalSupply class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsBasicExternalSupply::~UtGunnsBasicExternalSupply()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicExternalSupply::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tInputData;
    delete tConfigData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicExternalSupply::setUp()
{
    tLinkName             = "Test Basic External Supply";
    tNodeList.mNumNodes   = 3;
    tNodeList.mNodes      = tNodes;
    tInitialDemand        = 0.5;
    tPort0                = 0;
    tPort1                = 2;

    /// - Define nominal configuration data
    tConfigData = new GunnsBasicExternalSupplyConfigData(tLinkName, &tNodeList);

    /// - Define nominal input data
    tInputData = new GunnsBasicExternalSupplyInputData(true, 0.5, tInitialDemand);

    /// - Create the test article
    tArticle = new FriendlyGunnsBasicExternalSupply;

    /// - Declare the nominal test data
    tTimeStep = 0.1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicExternalSupply::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsBasicExternalSupply 01: testConfig ..........................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tLinkName == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes    == tConfigData->mNodeList->mNodes);

    /// - Check default config construction
    GunnsBasicExternalSupplyConfigData defaultConfig;
    CPPUNIT_ASSERT(""        == defaultConfig.mName);
    CPPUNIT_ASSERT(0         == defaultConfig.mNodeList);

    /// - Check copy config construction
    GunnsBasicExternalSupplyConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tLinkName == copyConfig.mName);
    CPPUNIT_ASSERT(tNodes    == copyConfig.mNodeList->mNodes);


    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the input data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicExternalSupply::testInput()
{
    std::cout << "\n UtGunnsBasicExternalSupply 02: testInput ...........................";

    /// - Check nominal input construction
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.5            == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tInitialDemand == tInputData->mSourceFlux);

    /// - Check default input construction
    GunnsBasicExternalSupplyInputData defaultInput;
    CPPUNIT_ASSERT(!defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0 == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0 == defaultInput.mSourceFlux);

    /// - Check copy input construction
    GunnsBasicExternalSupplyInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageValue == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(tInitialDemand                 == copyInput.mSourceFlux);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicExternalSupply::testDefaultConstruction()
{
    std::cout << "\n UtGunnsBasicExternalSupply 03: testDefaultConstruction .............";

    /// @test config data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSupplyPotential,      0.0);
    CPPUNIT_ASSERT(0 == tArticle->mDemandFlux_queue);
    CPPUNIT_ASSERT(0 == tArticle->mDemandFlux_queue_size);

    /// @test init flag
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    /// @test new/delete for code coverage
    GunnsBasicExternalSupply* article = new GunnsBasicExternalSupply();
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicExternalSupply::testNominalInitialization()
{
    std::cout << "\n UtGunnsBasicExternalSupply 04: testNominalInitialization ...........";

    /// - Default construct and initialize (with nominal data) a test article
    FriendlyGunnsBasicExternalSupply article;
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5,            article.mMalfBlockageValue, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tInitialDemand, article.mSourceFlux,        0.0);

    /// @test init flag
    CPPUNIT_ASSERT(article.mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for initialization exceptions on invalid properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicExternalSupply::testInitializationExceptions()
{
    std::cout << "\n UtGunnsBasicExternalSupply 05: testInitializationExceptions ........";

    /// - Check port rules - initialize with port 0 set to the ground node and verify the link fails
    ///   to initialize
    tPort0 = 2;
    tPort1 = 1;
    FriendlyGunnsBasicExternalSupply article;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    CPPUNIT_ASSERT(!article.mInitFlag);

    /// - Initialize with port 1 not set to the ground node and verify the link fails to initialize.
    tPort0 = 0;
    tPort1 = 1;
    FriendlyGunnsBasicExternalSupply article2;
    CPPUNIT_ASSERT_THROW(article2.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    CPPUNIT_ASSERT(!article2.mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the processInputs and processOutputs methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicExternalSupply::testIoMethods()
{
    std::cout << "\n UtGunnsBasicExternalSupply 06: testIoMethods .......................";

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Set the read data as if the sim bus set it, and verify the read method processes the data.
    double* tQueue = new double[4];
    tQueue[0] = 0.1;
    tQueue[1] = 0.2;
    tQueue[2] = 0.3;
    tQueue[3] = 0.4;

    tArticle->mDemandFlux_queue      = tQueue;
    tArticle->mDemandFlux_queue_size = 4;
    tArticle->processInputs();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.25, tArticle->mSourceFlux, 0.0);

    /// - Verify the processInputs method can be used without using a queue.
    tArticle->mDemandFlux_queue      = tQueue;
    tArticle->mDemandFlux_queue_size = 0;

    CPPUNIT_ASSERT_NO_THROW(tArticle->processInputs());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.25, tArticle->mSourceFlux, 0.0);

    delete [] tQueue;
    tArticle->mDemandFlux_queue      = 0;
    tArticle->mDemandFlux_queue_size = 4;

    CPPUNIT_ASSERT_NO_THROW(tArticle->processInputs());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.25, tArticle->mSourceFlux, 0.0);

    tArticle->mDemandFlux_queue      = 0;
    tArticle->mDemandFlux_queue_size = 0;

    CPPUNIT_ASSERT_NO_THROW(tArticle->processInputs());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.25, tArticle->mSourceFlux, 0.0);

    /// - Verify the write method grabs the supply node properties for write to the sim bus.
    tNodes[0].setPotential(100.0);
    tArticle->processOutputs();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(100.0, tArticle->mSupplyPotential, 0.0);

    std::cout << "... Pass";
}
