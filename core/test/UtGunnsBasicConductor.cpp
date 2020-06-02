/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    (
        (core/GunnsBasicConductor.o)
    )
***************************************************************************************************/
#include "UtGunnsBasicConductor.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsBasicConductor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsBasicConductor::UtGunnsBasicConductor()
    :
    mConfigData(),
    mInputData(),
    mArticle(),
    mLinkName(),
    mInitialConductivity(),
    mBlockage(),
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
/// @details  This is the default destructor for the UtGunnsBasicConductor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsBasicConductor::~UtGunnsBasicConductor()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicConductor::tearDown()
{
    /// - Deletes for news in setUp
    delete mArticle;
    delete mInputData;
    delete mConfigData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicConductor::setUp()
{
    mLinkName             = "Test Basic Conductor";
    mNodeList.mNumNodes   = 2;
    mNodeList.mNodes      = mNodes;
    mInitialConductivity  = 0.5;
    mBlockage             = 0.2;
    mPort0                = 0;
    mPort1                = 1;

    /// - Define nominal configuration data
    mConfigData = new GunnsBasicConductorConfigData(mLinkName, &mNodeList,
                                                    mInitialConductivity);

    /// - Create nominal input data
    mInputData = new GunnsBasicConductorInputData(true, mBlockage);

    /// - Create the test article
    mArticle = new FriendlyGunnsBasicConductor;

    /// - Declare the nominal test data
    mTolerance = 1.0e-08;
    mTimeStep = 0.1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicConductor::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsBasicConductor .. 01: testConfig ............................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(mLinkName            == mConfigData->mName);
    CPPUNIT_ASSERT(mNodes               == mConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(mInitialConductivity == mConfigData->mDefaultConductivity);

    /// - Check default config construction
    GunnsBasicConductorConfigData defaultConfig;
    CPPUNIT_ASSERT(""                   == defaultConfig.mName);
    CPPUNIT_ASSERT(0                    == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0                  == defaultConfig.mDefaultConductivity);

    /// - Check copy config construction
    GunnsBasicConductorConfigData copyConfig(*mConfigData);
    CPPUNIT_ASSERT(mLinkName            == copyConfig.mName);
    CPPUNIT_ASSERT(mNodes               == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(mInitialConductivity == copyConfig.mDefaultConductivity);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the input data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicConductor::testInput()
{
    std::cout << "\n UtGunnsBasicConductor .. 02: testInput .............................";

    /// - Check nominal input construction
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mBlockage, mInputData->mMalfBlockageValue, 0.0);

    /// - Check default input construction
    GunnsBasicConductorInputData defaultInput;
    CPPUNIT_ASSERT(!defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0 == defaultInput.mMalfBlockageValue);

    /// - Check copy input construction
    GunnsBasicConductorInputData copyInput(*mInputData);
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfBlockageValue, copyInput.mMalfBlockageValue, 0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicConductor::testDefaultConstruction()
{
    std::cout << "\n UtGunnsBasicConductor .. 03: testDefaultConstruction ...............";

    /// @test config data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mEffectiveConductivity, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mDefaultConductivity,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mSystemConductance,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mMalfBlockageValue,     0.0);

    /// @test init flag
    CPPUNIT_ASSERT(!mArticle->mInitFlag);

    /// @test new/delete for code coverage
    GunnsBasicConductor* article = new GunnsBasicConductor();
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicConductor::testNominalInitialization()
{
    std::cout << "\n UtGunnsBasicConductor .. 04: testNominalInitialization .............";

    /// - Default construct and initialize (with nominal data) a test article
    FriendlyGunnsBasicConductor article;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test config data
    CPPUNIT_ASSERT(mInitialConductivity == article.mDefaultConductivity);
    CPPUNIT_ASSERT(mInitialConductivity == article.mEffectiveConductivity);

    /// @test init flag
    CPPUNIT_ASSERT(article.mInitFlag);

    /// - Initialize with input data and check it
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInitialConductivity, mArticle->mEffectiveConductivity, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInitialConductivity, mArticle->mDefaultConductivity,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mBlockage,            mArticle->mMalfBlockageValue,     0.0);
    CPPUNIT_ASSERT(article.mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for initialization exceptions on invalid properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicConductor::testInitializationExceptions()
{
    std::cout << "\n UtGunnsBasicConductor .. 05: testInitializationExceptions ..........";

    /// - Default construct a test article
    GunnsBasicConductor article;

    /// @test for exception on invalid config data: conductivity < 0
    mConfigData->mDefaultConductivity = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicConductor::testStep()
{
    std::cout << "\n UtGunnsBasicConductor .. 06: testStep ..............................";

    /// - Initialize default test article with off nominal initialization data
    mConfigData->mDefaultConductivity = 1E32;
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->step(mTimeStep);

    /// - during step the conductance will be reset to the conductance limit.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(GunnsBasicLink::mConductanceLimit,
            mArticle->mSystemConductance, mTolerance);

    /// - during step if the active conductance doesn't equal the conductance then the active
    ///   conductance will get reset to the current conductance.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mAdmittanceMatrix[0], mArticle->mSystemConductance,
            mTolerance);

    /// - Step again with a normal conductivity and verify the contribution of the blockage term.
    mArticle->mEffectiveConductivity = 10.0;
    mArticle->mDefaultConductivity   = 10.0;
    mArticle->step(mTimeStep);
    const double expectedAdmittance = mArticle->mEffectiveConductivity * (1.0 - mBlockage);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(10.0, mArticle->mEffectiveConductivity, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAdmittance, mArticle->mAdmittanceMatrix[0], DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicConductor::testComputeFlows()
{
    std::cout << "\n UtGunnsBasicConductor .. 07: testComputeFlows ......................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// - Set the potential difference across the link like Gunns would do.
    mArticle->mPotentialVector[0] = 0.5;
    mArticle->mPotentialVector[1] = 0.4;

    mArticle->step(mTimeStep);
    mArticle->computeFlows(mTimeStep);

    /// - Check the potential drop, flux, and power
    const double expectedDrop  = mArticle->mPotentialVector[0] - mArticle->mPotentialVector[1];
    const double expectedFlux  = expectedDrop * mInitialConductivity * (1.0 - mBlockage);
    const double expectedPower = -expectedFlux * expectedDrop;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDrop,  mArticle->mPotentialDrop, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,  mArticle->mFlux,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower, mArticle->mPower,         DBL_EPSILON);

    /// - Verify zero flux when delta-potential is very small.
    mArticle->mPotentialVector[0] = 1.0;
    mArticle->mPotentialVector[1] = 1.0 + 10.0*DBL_EPSILON;
    mArticle->computeFlows(mTimeStep);

    CPPUNIT_ASSERT(0.0 == mArticle->mFlux);

    /// - Verify zero flux when all pressures are zero.
    mArticle->mPotentialVector[0] = 0.0;
    mArticle->mPotentialVector[1] = 0.0;
    mArticle->computeFlows(mTimeStep);

    CPPUNIT_ASSERT(0.0 == mArticle->mFlux);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test access methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicConductor::testAccessMethods()
{
    std::cout << "\n UtGunnsBasicConductor .. 08: testAccessMethods .....................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// - Set the potential difference across the link like Gunns would do.
    mArticle->mPotentialVector[0] = 0.5;
    mArticle->mPotentialVector[1] = 0.4;

    mArticle->step(mTimeStep);
    mArticle->computeFlows(mTimeStep);

    /// - Check access methods
    mArticle->mEffectiveConductivity = 5.0;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, mArticle->getEffectiveConductivity(), 0.0);

    mArticle->setDefaultConductivity(1.0);
    CPPUNIT_ASSERT(1.0 == mArticle->mDefaultConductivity);

    std::cout << "... Pass";
}
