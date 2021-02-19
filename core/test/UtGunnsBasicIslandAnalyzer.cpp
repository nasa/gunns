/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
 ((core/GunnsBasicIslandAnalyzer.o))
***************************************************************************************************/

#include "UtGunnsBasicIslandAnalyzer.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsBasicIslandAnalyzer class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsBasicIslandAnalyzer::UtGunnsBasicIslandAnalyzer()
    :
    tArticle(tNodeList),
    tNodes(),
    tNodeList(),
    tName("test article"),
    tConfig(tName),
    tInput()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsBasicIslandAnalyzer class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsBasicIslandAnalyzer::~UtGunnsBasicIslandAnalyzer()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicIslandAnalyzer::tearDown()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicIslandAnalyzer::setUp()
{
    tConfig.mName       = tName;
    tNodeList.mNumNodes = 3;
    tNodeList.mNodes    = tNodes;

    /// - Initialize the nodes with the initial potential and fluxes.
    tNodes[0].initialize("UtTestNode0", 100.0);
    tNodes[1].initialize("UtTestNode1", -10.0);

    tNodes[0].collectInflux(2.0);
    tNodes[1].collectInflux(4.0);
    tNodes[0].collectOutflux(1.0);
    tNodes[1].collectOutflux(2.0);

    tNodes[0].integrateFlows(0.0);
    tNodes[1].integrateFlows(0.0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the GunnsBasicIslandAnalyzerConfigData class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicIslandAnalyzer::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsBasicIslandAnalyzer 01: testConfig ..........................";

    /// - Test nominal config data construction.
    CPPUNIT_ASSERT(tName == tConfig.mName);

    /// - Test default config data construction.
    GunnsBasicIslandAnalyzerConfigData article(tName);
    CPPUNIT_ASSERT(tName == article.mName);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the GunnsBasicIslandAnalyzerInputData class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicIslandAnalyzer::testInput()
{
    std::cout << "\n UtGunnsBasicIslandAnalyzer 02: testInput ...........................";

    /// - Currently nothing to test.

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the default constructor of the GunnsBasicIslandAnalyzer class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicIslandAnalyzer::testDefaultConstruction()
{
    std::cout << "\n UtGunnsBasicIslandAnalyzer 03: testDefaultConstruction .............";

    /// @test state data
    CPPUNIT_ASSERT(""         ==  tArticle.mName);
    CPPUNIT_ASSERT(&tNodeList == &tArticle.mNodeList);
    CPPUNIT_ASSERT(0          ==  tArticle.mAttachedNode);
    CPPUNIT_ASSERT(0          ==  tArticle.mIslandSize);
    CPPUNIT_ASSERT(0          ==  tArticle.mIslandNodes);
    CPPUNIT_ASSERT(0.0        ==  tArticle.mIslandNetFlux);
    CPPUNIT_ASSERT(0.0        ==  tArticle.mHiPotential);
    CPPUNIT_ASSERT(0          ==  tArticle.mHiPotentialNode);
    CPPUNIT_ASSERT(0.0        ==  tArticle.mLoPotential);
    CPPUNIT_ASSERT(0          ==  tArticle.mLoPotentialNode);

    /// @test init flag
    CPPUNIT_ASSERT(false      ==  tArticle.mInitFlag);

    /// @test new/delete for code coverage
    GunnsBasicIslandAnalyzer* article = new GunnsBasicIslandAnalyzer(tNodeList);
    delete article;

    std::cout << "... Pass";
}

void UtGunnsBasicIslandAnalyzer::testInitialize()
{
    std::cout << "\n UtGunnsBasicIslandAnalyzer 04: testInitialize ......................";

    tArticle.initialize(&tConfig, &tInput);

    /// - Test nominal initialization.
    CPPUNIT_ASSERT(tName      ==  tArticle.mName);
    CPPUNIT_ASSERT(&tNodeList == &tArticle.mNodeList);
    CPPUNIT_ASSERT(-1         ==  tArticle.mAttachedNode);
    CPPUNIT_ASSERT(0          ==  tArticle.mIslandSize);
    CPPUNIT_ASSERT(false      ==  tArticle.mIslandNodes[0]);
    CPPUNIT_ASSERT(false      ==  tArticle.mIslandNodes[1]);
    CPPUNIT_ASSERT(0.0        ==  tArticle.mIslandNetFlux);
    CPPUNIT_ASSERT(0.0        ==  tArticle.mHiPotential);
    CPPUNIT_ASSERT(-1         ==  tArticle.mHiPotentialNode);
    CPPUNIT_ASSERT(0.0        ==  tArticle.mLoPotential);
    CPPUNIT_ASSERT(-1         ==  tArticle.mLoPotentialNode);

    /// @test init flag
    CPPUNIT_ASSERT(true       ==  tArticle.mInitFlag);

    /// - Test exception thrown from missing name.
    tConfig.mName = "";
    CPPUNIT_ASSERT_THROW(tArticle.initialize(&tConfig, &tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);
    tConfig.mName = tName;

    /// - Test exception thrown from null config data.
    const GunnsBasicIslandAnalyzerConfigData* nullConfig = 0;
    CPPUNIT_ASSERT_THROW(tArticle.initialize(nullConfig, &tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);

    /// - Test exception thrown from null input data.
    const GunnsBasicIslandAnalyzerInputData* nullInput = 0;
    CPPUNIT_ASSERT_THROW(tArticle.initialize(&tConfig, nullInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);

    /// - Test exception thrown on bad config data pointer type.
    BadGunnsBasicIslandAnalyzerConfigData* badConfig = new BadGunnsBasicIslandAnalyzerConfigData(tName);
    CPPUNIT_ASSERT_THROW(tArticle.initialize(badConfig, &tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);
    delete badConfig;

    /// - Test exception thrown on bad input data pointer type.
    BadGunnsBasicIslandAnalyzerInputData* badInput = new BadGunnsBasicIslandAnalyzerInputData();
    CPPUNIT_ASSERT_THROW(tArticle.initialize(&tConfig, badInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);
    delete badInput;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the stepPreSolver method of the GunnsBasicIslandAnalyzer class.
///           That method doesn't actually do anything so there are no asserts, but this is done for
///           complete code coverage.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicIslandAnalyzer::testPreSolver()
{
    std::cout << "\n UtGunnsBasicIslandAnalyzer 05: testPreSolver .......................";

    tArticle.initialize(&tConfig, &tInput);
    tArticle.stepPreSolver(0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the stepPostSolver method of the GunnsBasicIslandAnalyzer class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicIslandAnalyzer::testPostSolver()
{
    std::cout << "\n UtGunnsBasicIslandAnalyzer 06: testPostSolver ......................";

    tArticle.initialize(&tConfig, &tInput);

    /// - Test analysis outputs if no vectors are supplied to the nodes, which should be reset.
    tArticle.mAttachedNode = 0;
    tArticle.stepPostSolver(0.0);

    CPPUNIT_ASSERT_EQUAL(0,       tArticle.mIslandSize);
    CPPUNIT_ASSERT_EQUAL(false,   tArticle.mIslandNodes[0]);
    CPPUNIT_ASSERT_EQUAL(false,   tArticle.mIslandNodes[1]);
    CPPUNIT_ASSERT_EQUAL(false,   tArticle.mIslandNodes[2]);
    CPPUNIT_ASSERT_EQUAL(0.0,     tArticle.mIslandNetFlux);
    CPPUNIT_ASSERT_EQUAL(0.0,     tArticle.mHiPotential);
    CPPUNIT_ASSERT_EQUAL(-1,      tArticle.mHiPotentialNode);
    CPPUNIT_ASSERT_EQUAL(0.0,     tArticle.mLoPotential);
    CPPUNIT_ASSERT_EQUAL(-1,      tArticle.mLoPotentialNode);

    /// - Build an island vector and assign nodes
    std::vector<int> island;
    island.push_back(0);
    island.push_back(1);
    tNodes[0].setIslandVector(&island);
    tNodes[1].setIslandVector(&island);

    /// - Test analysis outputs attached to node 0.
    tArticle.mAttachedNode = 0;
    tArticle.stepPostSolver(0.0);

    CPPUNIT_ASSERT_EQUAL(2,       tArticle.mIslandSize);
    CPPUNIT_ASSERT_EQUAL(true,    tArticle.mIslandNodes[0]);
    CPPUNIT_ASSERT_EQUAL(true,    tArticle.mIslandNodes[1]);
    CPPUNIT_ASSERT_EQUAL(false,   tArticle.mIslandNodes[2]);
    CPPUNIT_ASSERT_EQUAL(3.0,     tArticle.mIslandNetFlux);
    CPPUNIT_ASSERT_EQUAL(100.0,   tArticle.mHiPotential);
    CPPUNIT_ASSERT_EQUAL(0,       tArticle.mHiPotentialNode);
    CPPUNIT_ASSERT_EQUAL(-10.0,   tArticle.mLoPotential);
    CPPUNIT_ASSERT_EQUAL(1,       tArticle.mLoPotentialNode);

    /// - Test analysis outputs attached to node -1, which should be reset because it is invalid.
    tArticle.mAttachedNode = -1;
    tArticle.stepPostSolver(0.0);

    CPPUNIT_ASSERT_EQUAL(0,       tArticle.mIslandSize);
    CPPUNIT_ASSERT_EQUAL(false,   tArticle.mIslandNodes[0]);
    CPPUNIT_ASSERT_EQUAL(false,   tArticle.mIslandNodes[1]);
    CPPUNIT_ASSERT_EQUAL(false,   tArticle.mIslandNodes[2]);
    CPPUNIT_ASSERT_EQUAL(0.0,     tArticle.mIslandNetFlux);
    CPPUNIT_ASSERT_EQUAL(0.0,     tArticle.mHiPotential);
    CPPUNIT_ASSERT_EQUAL(-1,      tArticle.mHiPotentialNode);
    CPPUNIT_ASSERT_EQUAL(0.0,     tArticle.mLoPotential);
    CPPUNIT_ASSERT_EQUAL(-1,      tArticle.mLoPotentialNode);

    /// - Test analysis outputs attached to node 1, which should be identical to node 0.
    tArticle.mAttachedNode = 1;
    tArticle.stepPostSolver(0.0);

    CPPUNIT_ASSERT_EQUAL(2,       tArticle.mIslandSize);
    CPPUNIT_ASSERT_EQUAL(true,    tArticle.mIslandNodes[0]);
    CPPUNIT_ASSERT_EQUAL(true,    tArticle.mIslandNodes[1]);
    CPPUNIT_ASSERT_EQUAL(false,   tArticle.mIslandNodes[2]);
    CPPUNIT_ASSERT_EQUAL(3.0,     tArticle.mIslandNetFlux);
    CPPUNIT_ASSERT_EQUAL(100.0,   tArticle.mHiPotential);
    CPPUNIT_ASSERT_EQUAL(0,       tArticle.mHiPotentialNode);
    CPPUNIT_ASSERT_EQUAL(-10.0,   tArticle.mLoPotential);
    CPPUNIT_ASSERT_EQUAL(1,       tArticle.mLoPotentialNode);

    /// - Test analysis outputs attached to node 2, which should be reset because it is ground.
    tArticle.mAttachedNode = 2;
    tArticle.stepPostSolver(0.0);

    CPPUNIT_ASSERT_EQUAL(0,       tArticle.mIslandSize);
    CPPUNIT_ASSERT_EQUAL(false,   tArticle.mIslandNodes[0]);
    CPPUNIT_ASSERT_EQUAL(false,   tArticle.mIslandNodes[1]);
    CPPUNIT_ASSERT_EQUAL(false,   tArticle.mIslandNodes[2]);
    CPPUNIT_ASSERT_EQUAL(0.0,     tArticle.mIslandNetFlux);
    CPPUNIT_ASSERT_EQUAL(0.0,     tArticle.mHiPotential);
    CPPUNIT_ASSERT_EQUAL(-1,      tArticle.mHiPotentialNode);
    CPPUNIT_ASSERT_EQUAL(0.0,     tArticle.mLoPotential);
    CPPUNIT_ASSERT_EQUAL(-1,      tArticle.mLoPotentialNode);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the setter & getter methods of the GunnsBasicIslandAnalyzer class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicIslandAnalyzer::testAccessors()
{
    std::cout << "\n UtGunnsBasicIslandAnalyzer 07: testAccessors .......................";

    tArticle.initialize(&tConfig, &tInput);

    /// - Build an island vector and assign nodes
    std::vector<int> island;
    island.push_back(0);
    island.push_back(1);
    tNodes[0].setIslandVector(&island);
    tNodes[1].setIslandVector(&island);

    /// - Test the setter & getter for attached node.
    tArticle.setAttachedNode(1);
    CPPUNIT_ASSERT_EQUAL(1, tArticle.mAttachedNode);
    CPPUNIT_ASSERT_EQUAL(1, tArticle.getAttachedNode());

    /// - Test analysis outputs attached to node 1.
    tArticle.stepPostSolver(0.0);

    /// - Test the getter for island size.
    CPPUNIT_ASSERT_EQUAL(2, tArticle.getIslandSize());

    /// - Test the getter for the island nodes array.
    CPPUNIT_ASSERT_EQUAL(true,                                      tArticle.getIslandNodes()[0]);
    CPPUNIT_ASSERT_EQUAL(true,                                      tArticle.getIslandNodes()[1]);
    CPPUNIT_ASSERT_EQUAL(false,                                     tArticle.getIslandNodes()[2]);

    /// - Test the getter for the island net flux.
    CPPUNIT_ASSERT_EQUAL(3.0,                                       tArticle.getIslandNetFlux());

    std::cout << "... Pass";
}
