/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    (
        (aspects/thermal/GunnsThermalPotential.o)
    )
***************************************************************************************************/

#include "UtGunnsThermalPotential.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsThermalPotential class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsThermalPotential::UtGunnsThermalPotential()
    :
    tConfigData(),
    tInputData(),
    tArticle(),
    tLinkName(),
    tNodes(),
    tNodeList(),
    tNetworkLinks(),
    tInitialConductivity(),
    tInitialPotential(),
    tMalfBlockageValue(),
    tMalfBlockageFlag(),
    tPort0(),
    tPort1(),
    tTimeStep(),
    tTolerance()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsThermalPotential class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsThermalPotential::~UtGunnsThermalPotential()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalPotential::tearDown()
{
    /// - Deletes for news in setup.
    delete tInputData;
    delete tConfigData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalPotential::setUp()
{
    /// - Declare the standard Gunns link test data
    tLinkName            = "Test Thermal Potential";
    tNodeList.mNumNodes  = 2;
    tNodeList.mNodes     = tNodes;
    tInitialConductivity = 0.1;
    tInitialPotential    = -0.5;
    tMalfBlockageValue   = 0.2;
    /// - For ThermalPotential, vacuum = 1 and is set at port0
    tPort0               = 1;
    tPort1               = 0;

    /// - Define nominal configuration data
    tConfigData = new GunnsThermalPotentialConfigData(tLinkName, &tNodeList,
                                                      tInitialConductivity);

    /// - Define nominal input data
    tInputData =  new GunnsThermalPotentialInputData(true, tMalfBlockageValue, tInitialPotential);

    /// - Initialize the test Article
    tArticle.initialize(*tConfigData, *tInputData, tNetworkLinks, tPort0, tPort1);

    /// - Declare the nominal test data
    tTolerance = 1.0e-8;
    tTimeStep = 0.1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalPotential::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsThermalPotential 01: testConfig ..........................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tLinkName == tConfigData->mName);

    /// - Check default config construction
    GunnsThermalPotentialConfigData defaultConfig;
    CPPUNIT_ASSERT("unnamed potential" == defaultConfig.mName);

    /// - Check copy config construction
    GunnsThermalPotentialConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tLinkName == copyConfig.mName);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalPotential::testInput()
{
    std::cout << "\n UtGunnsThermalPotential 02: testInput ...........................";

    /// - Check nominal input data
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tInitialPotential  == tInputData->mSourcePotential);

    /// - Check default input data
    GunnsThermalPotentialInputData defaultInput;
    CPPUNIT_ASSERT(!defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0 == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0 == defaultInput.mSourcePotential);

    /// - Check copy input data
    GunnsThermalPotentialInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageValue == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(tInitialPotential              == copyInput.mSourcePotential);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalPotential::testDefaultConstruction()
{
    std::cout << "\n UtGunnsThermalPotential 03: testDefaultConstruction .............";

    /// - Construct an uninitialized test article
    FriendlyGunnsThermalPotential article;

    /// @test input data
    CPPUNIT_ASSERT(0.0 == article.mSourcePotential);
    CPPUNIT_ASSERT(0.0 == article.mDefaultConductivity);
    CPPUNIT_ASSERT(0.0 == article.mMalfBlockageValue);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalPotential::testInitialization()
{
    std::cout << "\n UtGunnsThermalPotential 04: testInitialization ..................";

    /// @test config data
    CPPUNIT_ASSERT(tInitialConductivity == tArticle.mDefaultConductivity);
    CPPUNIT_ASSERT(tInitialPotential    == tArticle.mSourcePotential);
    CPPUNIT_ASSERT(tMalfBlockageValue   == tArticle.mMalfBlockageValue);

    /// @test init flag
    CPPUNIT_ASSERT(tArticle.mInitFlag);


    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests processOutputs method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalPotential::testProcessOutputs()
{
    std::cout << "\n UtGunnsThermalPotential 05: testProcessOutputs ..................";

    /// - Set mFlux and mHeatFluxIntoNode to different values
    tArticle.mFlux = 4;
    tArticle.mHeatFluxIntoNode = 8;

    /// @test HeatFluxIntoNode is not equal to mFlux nor its negative
    CPPUNIT_ASSERT(tArticle.mHeatFluxIntoNode !=  tArticle.mFlux);
    CPPUNIT_ASSERT(tArticle.mHeatFluxIntoNode != -tArticle.mFlux);

    /// - Call processOutputs
    tArticle.processOutputs();

    /// @test mHeatFluxIntoNode is assigned the negative of mFlux by the = operator
    CPPUNIT_ASSERT(tArticle.mHeatFluxIntoNode == -tArticle.mFlux);

    /// - Call Step()
    tArticle.step(tTimeStep);

    /// - Call processOutputs();
    tArticle.processOutputs();

    /// @test Again
    CPPUNIT_ASSERT(tArticle.mHeatFluxIntoNode == -tArticle.mFlux);

    std::cout << "... Pass";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests checkSpecificPortRules method.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalPotential::testSpecificPortRules()
{
    std::cout << "\n UtGunnsThermalPotential 06: testSpecificPortRules................";

    /// @test for invalid node number input
    CPPUNIT_ASSERT(false == tArticle.checkSpecificPortRules(tPort0, tNodeList.mNumNodes - 1));
    CPPUNIT_ASSERT(false == tArticle.checkSpecificPortRules(tPort1, tNodeList.mNumNodes));

    /// @test rules for validity
    CPPUNIT_ASSERT(true  == tArticle.checkSpecificPortRules(tPort1, tNodeList.mNumNodes - 1));
    CPPUNIT_ASSERT(true  == tArticle.checkSpecificPortRules(tPort0, tNodeList.mNumNodes));

    /// - Create a test article
    FriendlyGunnsThermalPotential article;

    /// @test Exception is thrown when article is initialized with bad data, which in this
    /// context means that port0 and port1 values are switched
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tNetworkLinks, tPort1,
                         tPort0), TsInitializationException);

    std::cout << "... Pass";
}
