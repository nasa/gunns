/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    (
        (core/GunnsBasicSource.o)
    )
***************************************************************************************************/
#include "UtGunnsBasicSource.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsBasicSource class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsBasicSource::UtGunnsBasicSource()
    :
    mConfigData(),
    mInputData(),
    mArticle(),
    mLinkName(),
    mInitialDemand(),
    mNodes(),
    mNodeList(),
    mLinks(),
    mPort0(),
    mPort1(),
    mTimeStep(),
    mTolerance()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsBasicSource class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsBasicSource::~UtGunnsBasicSource()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSource::tearDown()
{
    /// - Deletes for news in setUp
    delete mArticle;
    delete mInputData;
    delete mConfigData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSource::setUp()
{
    mLinkName             = "Test Basic Source";
    mNodeList.mNumNodes   = 2;
    mNodeList.mNodes      = mNodes;
    mInitialDemand        = 0.5;
    mPort0                = 0;
    mPort1                = 1;

    /// - Define nominal configuration data
    mConfigData = new GunnsBasicSourceConfigData(mLinkName, &mNodeList);

    /// - Define nominal input data
    mInputData = new GunnsBasicSourceInputData(true, 0.5, mInitialDemand);

    /// - Create the test article
    mArticle = new FriendlyGunnsBasicSource;

    /// - Declare the nominal test data
    mTolerance = 1.0e-08;
    mTimeStep = 0.1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSource::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsBasicSource ..... 01: testConfig ............................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(mLinkName == mConfigData->mName);
    CPPUNIT_ASSERT(mNodes    == mConfigData->mNodeList->mNodes);

    /// - Check default config construction
    GunnsBasicSourceConfigData defaultConfig;
    CPPUNIT_ASSERT(""        == defaultConfig.mName);
    CPPUNIT_ASSERT(0         == defaultConfig.mNodeList);

    /// - Check copy config construction
    GunnsBasicSourceConfigData copyConfig(*mConfigData);
    CPPUNIT_ASSERT(mLinkName == copyConfig.mName);
    CPPUNIT_ASSERT(mNodes    == copyConfig.mNodeList->mNodes);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSource::testInput()
{
    std::cout << "\n UtGunnsBasicSource ..... 02: testInput .............................";

    /// - Check nominal input construction
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.5            == mInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(mInitialDemand == mInputData->mSourceFlux);

    /// - Check default input construction
    GunnsBasicSourceInputData defaultInput;
    CPPUNIT_ASSERT(!defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0 == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0 == defaultInput.mSourceFlux);

    /// - Check copy input construction
    GunnsBasicSourceInputData copyInput(*mInputData);
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(mInputData->mMalfBlockageValue == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(mInitialDemand                 == copyInput.mSourceFlux);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSource::testDefaultConstruction()
{
    std::cout << "\n UtGunnsBasicSource ..... 03: testDefaultConstruction ...............";

    /// @test config data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mSourceFlux,
                                 mTolerance);

    /// @test init flag
    CPPUNIT_ASSERT(!mArticle->mInitFlag);

    /// @test new/delete for code coverage
    GunnsBasicSource* article = new GunnsBasicSource();
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSource::testNominalInitialization()
{
    std::cout << "\n UtGunnsBasicSource ..... 04: testNominalInitialization .............";

    /// - Default construct and initialize (with nominal data) a test article
    FriendlyGunnsBasicSource article;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test config data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5,            article.mMalfBlockageValue, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInitialDemand, article.mSourceFlux,        0.0);

    /// @test init flag
    CPPUNIT_ASSERT(article.mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSource::testStep()
{
    std::cout << "\n UtGunnsBasicSource ..... 05: testStep ..............................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->step(mTimeStep);

    /// - during step the source Vector will be equal to the source flux times the blockage malf.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mSourceFlux * 0.5,
                                 mArticle->mSourceVector[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mSourceFlux * 0.5,
                                 mArticle->mFlux,            DBL_EPSILON);

    /// - Step again with the blockage malfunction inactive.
    mArticle->mMalfBlockageFlag  = false;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mSourceFlux, mArticle->mSourceVector[1], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mSourceFlux, mArticle->mFlux,            0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSource::testComputeFlows()
{
    std::cout << "\n UtGunnsBasicSource ..... 06: testComputeFlows ......................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// - Initialize the nodes potentials
    mNodes[0].setPotential(100.0);
    mNodes[1].setPotential(  0.0);
    mArticle->mPotentialVector[0] = 100.0;
    mArticle->mPotentialVector[1] =   0.0;
    mArticle->mMalfBlockageFlag = false;

    mArticle->step(mTimeStep);
    mArticle->computeFlows(mTimeStep);

    /// - Check potential drop and power across the link is updated
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNodes[0].getPotential() - mNodes[1].getPotential(),
            mArticle->mPotentialDrop, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-mInitialDemand * mArticle->mPotentialDrop,
            mArticle->mPower, DBL_EPSILON);

    /// - Check flux is transported to/from the nodes
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInitialDemand, mNodes[1].getInflux(),  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInitialDemand, mNodes[0].getOutflux(), 0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests setter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSource::testSetters()
{
    std::cout << "\n UtGunnsBasicSource ..... 07: testSetters ...........................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// - Set the source flux demand using the setter method and verify.
    mArticle->setFluxDemand(1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, mArticle->mSourceFlux, 0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests access methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicSource::testAccessMethods()
{
    std::cout << "\n UtGunnsBasicSource ..... 08: testAccessMethods .....................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// - Get the source flux demand using the getter method and verify.
    mArticle->setFluxDemand(5.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, mArticle->getFluxDemand(), 0.0);

    std::cout << "... Pass";
}
