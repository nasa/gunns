/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    (
        (core/GunnsBasicExternalDemand.o)
    )
***************************************************************************************************/
#include "UtGunnsBasicExternalDemand.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsBasicExternalDemand class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsBasicExternalDemand::UtGunnsBasicExternalDemand()
    :
    tConfigData(),
    tInputData(),
    tArticle(),
    tLinkName(),
    tInitialConductivity(),
    tInitialPotential(),
    tFilterMinConductivity(),
    tFilterMinDeltaP(),
    tFilterCapacitanceGain(),
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
/// @details  This is the default destructor for the UtGunnsBasicExternalDemand class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsBasicExternalDemand::~UtGunnsBasicExternalDemand()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicExternalDemand::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tInputData;
    delete tConfigData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicExternalDemand::setUp()
{
    tLinkName              = "Test Basic External Demand";
    tNodeList.mNumNodes    = 3;
    tNodeList.mNodes       = tNodes;
    tInitialConductivity   = 0.1;
    tInitialPotential      =-0.5;
    tFilterMinConductivity = 1.0;
    tFilterMinDeltaP       = 1.0E-8;
    tFilterCapacitanceGain = 0.1;
    tPort0                 = 2;
    tPort1                 = 0;
    tTimeStep              = 1.0;

    /// - Define nominal configuration data
    tConfigData = new GunnsBasicExternalDemandConfigData(tLinkName, &tNodeList,
                                                         tInitialConductivity,
                                                         tFilterMinConductivity,
                                                         tFilterMinDeltaP,
                                                         tFilterCapacitanceGain);

    /// - Define nominal input data
    tInputData = new GunnsBasicExternalDemandInputData(true, 0.5, tInitialPotential);

    /// - Create the test article
    tArticle = new FriendlyGunnsBasicExternalDemand;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicExternalDemand::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsBasicExternalDemand 01: testConfig ..........................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tLinkName              == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                 == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tInitialConductivity   == tConfigData->mDefaultConductivity);
    CPPUNIT_ASSERT(tFilterMinConductivity == tConfigData->mFilterMinConductivity);
    CPPUNIT_ASSERT(tFilterMinDeltaP       == tConfigData->mFilterMinDeltaP);
    CPPUNIT_ASSERT(tFilterCapacitanceGain == tConfigData->mFilterCapacitanceGain);

    /// - Check default config construction
    GunnsBasicExternalDemandConfigData defaultConfig;
    CPPUNIT_ASSERT(""                     == defaultConfig.mName);
    CPPUNIT_ASSERT(0                      == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0                    == defaultConfig.mDefaultConductivity);
    CPPUNIT_ASSERT(1.0                    == defaultConfig.mFilterMinConductivity);
    CPPUNIT_ASSERT(1.0E-8                 == defaultConfig.mFilterMinDeltaP);
    CPPUNIT_ASSERT(0.05                   == defaultConfig.mFilterCapacitanceGain);

    /// - Check copy config construction
    GunnsBasicExternalDemandConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tLinkName              == copyConfig.mName);
    CPPUNIT_ASSERT(tNodes                 == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tInitialConductivity   == copyConfig.mDefaultConductivity);
    CPPUNIT_ASSERT(tFilterMinConductivity == copyConfig.mFilterMinConductivity);
    CPPUNIT_ASSERT(tFilterMinDeltaP       == copyConfig.mFilterMinDeltaP);
    CPPUNIT_ASSERT(tFilterCapacitanceGain == copyConfig.mFilterCapacitanceGain);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the input data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicExternalDemand::testInput()
{
    std::cout << "\n UtGunnsBasicExternalDemand 02: testInput ...........................";

    /// - Check nominal input data
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.5                   == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tInitialPotential     == tInputData->mSourcePotential);

    /// - Check default input data
    GunnsBasicExternalDemandInputData defaultInput;
    CPPUNIT_ASSERT(!defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0 == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0 == defaultInput.mSourcePotential);

    /// - Check copy input data
    GunnsBasicExternalDemandInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageValue == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(tInitialPotential              == copyInput.mSourcePotential);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicExternalDemand::testDefaultConstruction()
{
    std::cout << "\n UtGunnsBasicExternalDemand 03: testDefaultConstruction .............";

    /// @test config data
    CPPUNIT_ASSERT(0.0 == tArticle->mFilterMinConductivity);
    CPPUNIT_ASSERT(0.0 == tArticle->mFilterMinDeltaP);
    CPPUNIT_ASSERT(0.0 == tArticle->mAvgDemand);
    CPPUNIT_ASSERT(0.0 == tArticle->mAvgSupplyP);
    CPPUNIT_ASSERT(0.0 == tArticle->mAvgSupplyDeltaP);
    CPPUNIT_ASSERT(0.0 == tArticle->mEstimatedCapacitance);
    CPPUNIT_ASSERT(0.0 == tArticle->mFilterCapacitanceGain);
    CPPUNIT_ASSERT(0.0 == tArticle->mSupplyPotential);
    CPPUNIT_ASSERT(0.0 == tArticle->mDemandFlux);

    /// @test init flag
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    /// @test new/delete for code coverage
    GunnsBasicExternalDemand* article = new GunnsBasicExternalDemand();
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicExternalDemand::testNominalInitialization()
{
    std::cout << "\n UtGunnsBasicExternalDemand 04: testNominalInitialization ...........";

    /// - Default construct and initialize (with nominal data) a test article
    FriendlyGunnsBasicExternalDemand article;
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test config data
    CPPUNIT_ASSERT(tInitialPotential      == article.mSourcePotential);
    CPPUNIT_ASSERT(tFilterMinConductivity == article.mFilterMinConductivity);
    CPPUNIT_ASSERT(tFilterMinDeltaP       == article.mFilterMinDeltaP);
    CPPUNIT_ASSERT(tFilterCapacitanceGain == article.mFilterCapacitanceGain);
    CPPUNIT_ASSERT(0.0                    == article.mAvgDemand);
    CPPUNIT_ASSERT(0.0                    == article.mAvgSupplyP);
    CPPUNIT_ASSERT(0.0                    == article.mAvgSupplyDeltaP);
    CPPUNIT_ASSERT(0.0                    == article.mEstimatedCapacitance);
    CPPUNIT_ASSERT(0.5                    == article.mMalfBlockageValue);

    /// @test init flag
    CPPUNIT_ASSERT(article.mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for initialization exceptions on invalid properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicExternalDemand::testInitializationExceptions()
{
    std::cout << "\n UtGunnsBasicExternalDemand 05: testInitializationExceptions ........";

    /// - Check port rules - initialize with port 1 set to the ground node and verify the link fails
    ///   to initialize
    tPort0 = 1;
    tPort1 = 2;
    FriendlyGunnsBasicExternalDemand article;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    CPPUNIT_ASSERT(!article.mInitFlag);

    /// - Initialize with port 0 not set to the ground node and verify the link fails to initialize.
    tPort0 = 0;
    tPort1 = 1;
    FriendlyGunnsBasicExternalDemand article2;
    CPPUNIT_ASSERT_THROW(article2.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    CPPUNIT_ASSERT(!article2.mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the processInputs and processOutputs methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicExternalDemand::testIoMethods()
{
    std::cout << "\n UtGunnsBasicExternalDemand 06: testIoMethods .......................";

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Set the read data as if the sim bus set it, and verify the read method processes the data.
    tArticle->mSupplyPotential = 100.0;
    tArticle->processInputs();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(100.0, tArticle->mSourcePotential, 0.0);

    /// - Verify the write method grabs the supply node properties for write to the sim bus.
    tArticle->mFlux = 1.0;
    tArticle->processOutputs();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tArticle->mDemandFlux, 0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicExternalDemand::testStep()
{
    std::cout << "\n UtGunnsBasicExternalDemand 07: testStep ............................";

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Make the first step with supply potential zero.
    tArticle->mSupplyPotential = 0.0;
    tArticle->processInputs();
    tArticle->step(tTimeStep);

    /// - Check the filtered link conductivity after the 1st step.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mAvgDemand,                          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mAvgSupplyDeltaP,                    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mAvgSupplyP,                         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mEstimatedCapacitance,               DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tArticle->mEffectiveConductivity,              DBL_EPSILON);

    /// - Step again with an increasing supply potential.  Set the link's flux to simulate network
    ///   flow.
    tArticle->mSupplyPotential = 100.0;
    tArticle->processInputs();
    tArticle->mFlux = 100.0;
    tArticle->step(tTimeStep);

    /// - Check the filtered link conductivity after the 2nd step.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5*tArticle->mFlux, tArticle->mAvgDemand,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5*100.0,           tArticle->mAvgSupplyDeltaP,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5*100.0,           tArticle->mAvgSupplyP,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-.1, tArticle->mEstimatedCapacitance,             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tArticle->mEffectiveConductivity,            DBL_EPSILON);

    /// - Check the base class was stepped.
    CPPUNIT_ASSERT(tArticle->mSourceVector[1] > 0.0);

    /// - Step again with dt = 0 and verify the result.
    tArticle->step(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFilterMinConductivity, tArticle->mEffectiveConductivity, 0.0);

    std::cout << "... Pass";
}
