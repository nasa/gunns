/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    (
        (core/GunnsBasicPotential.o)
    )
***************************************************************************************************/
#include "UtGunnsBasicPotential.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsBasicPotential class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsBasicPotential::UtGunnsBasicPotential()
    :
    mConfigData(),
    mInputData(),
    mArticle(),
    mLinkName(),
    mInitialConductivity(),
    mInitialPotential(),
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
/// @details  This is the default destructor for the UtGunnsBasicPotential class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsBasicPotential::~UtGunnsBasicPotential()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicPotential::tearDown()
{
    /// - Deletes for news in setUp
    delete mArticle;
    delete mInputData;
    delete mConfigData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicPotential::setUp()
{
    mLinkName             = "Test Basic Potential";
    mNodeList.mNumNodes   = 2;
    mNodeList.mNodes      = mNodes;
    mInitialConductivity  = 0.1;
    mInitialPotential     =-0.5;
    mPort0                = 0;
    mPort1                = 1;

    /// - Define nominal configuration data
    mConfigData = new GunnsBasicPotentialConfigData(mLinkName, &mNodeList,
                                                    mInitialConductivity);

    /// - Define nominal input data
    mInputData = new GunnsBasicPotentialInputData(true, 0.5, mInitialPotential);

    /// - Create the test article
    mArticle = new FriendlyGunnsBasicPotential;

    /// - Declare the nominal test data
    mTolerance = 1.0e-08;
    mTimeStep = 0.1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicPotential::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsBasicPotential .. 01: testConfig ............................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(mLinkName            == mConfigData->mName);
    CPPUNIT_ASSERT(mNodes               == mConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(mInitialConductivity == mConfigData->mDefaultConductivity);

    /// - Check default config construction
    GunnsBasicPotentialConfigData defaultConfig;
    CPPUNIT_ASSERT(""                   == defaultConfig.mName);
    CPPUNIT_ASSERT(0                    == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0                  == defaultConfig.mDefaultConductivity);

    /// - Check copy config construction
    GunnsBasicPotentialConfigData copyConfig(*mConfigData);
    CPPUNIT_ASSERT(mLinkName            == copyConfig.mName);
    CPPUNIT_ASSERT(mNodes               == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(mInitialConductivity == copyConfig.mDefaultConductivity);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the input data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicPotential::testInput()
{
    std::cout << "\n UtGunnsBasicPotential .. 02: testInput .............................";

    /// - Check nominal input data
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, mInputData->mMalfBlockageValue, 0.0);
    CPPUNIT_ASSERT(mInitialPotential    == mInputData->mSourcePotential);

    /// - Check default input data
    GunnsBasicPotentialInputData defaultInput;
    CPPUNIT_ASSERT(!defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultInput.mMalfBlockageValue, 0.0);
    CPPUNIT_ASSERT(0.0 == defaultInput.mSourcePotential);

    /// - Check copy input data
    GunnsBasicPotentialInputData copyInput(*mInputData);
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfBlockageValue, copyInput.mMalfBlockageValue, 0.0);
    CPPUNIT_ASSERT(mInitialPotential    == copyInput.mSourcePotential);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicPotential::testDefaultConstruction()
{
    std::cout << "\n UtGunnsBasicPotential .. 03: testDefaultConstruction ...............";

    /// @test config data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mSourcePotential,
                                 mTolerance);

    /// @test init flag
    CPPUNIT_ASSERT(!mArticle->mInitFlag);

    /// @test new/delete for code coverage
    GunnsBasicPotential* article = new GunnsBasicPotential();
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicPotential::testNominalInitialization()
{
    std::cout << "\n UtGunnsBasicPotential .. 04: testNominalInitialization .............";

    /// - Default construct and initialize (with nominal data) a test article
    FriendlyGunnsBasicPotential article;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test config data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInitialConductivity, article.mEffectiveConductivity, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInitialPotential, article.mSourcePotential, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, article.mMalfBlockageValue, 0.0);

    /// @test init flag
    CPPUNIT_ASSERT(article.mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for initialization exceptions on invalid properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicPotential::testInitializationExceptions()
{
    std::cout << "\n UtGunnsBasicPotential .. 05: testInitializationExceptions ..........";

    /// - Currently there is nothing to test.

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicPotential::testStep()
{
    std::cout << "\n UtGunnsBasicPotential .. 06: testStep ..............................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->step(mTimeStep);

    /// - during step if the active conductance doesn't equal the conductance then the
    ///   active conductance will get reset to the current conductance.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mAdmittanceMatrix[0], mArticle->mSystemConductance,
            mTolerance);

    /// - Since the Source Potential is greater than zero and the effective conductivity is
    ///   greater than zero then the source vector of the link should be greater than zero too.
    if (mArticle->mSourceVector[0] > 0.0) {
        CPPUNIT_ASSERT(true);
    } else {
        CPPUNIT_ASSERT(false);
    }

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicPotential::testComputeFlows()
{
    std::cout << "\n UtGunnsBasicPotential .. 07: testComputeFlows ......................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    mArticle->mSourcePotential = 1.0;
    mArticle->mDefaultConductivity = 1.0;

    mArticle->step(mTimeStep);

    /// - Set the potential difference across the link like Gunns would do.
    mArticle->mPotentialVector[0] = 0.0;
    mArticle->mPotentialVector[1] = 0.9;

    mArticle->computeFlows(mTimeStep);

    /// - Check the potential drop, flux, and power
    const double expectedDrop  = mArticle->mPotentialVector[0] - mArticle->mPotentialVector[1];
    const double expectedFlux  = (expectedDrop + mArticle->mSourcePotential)
            * mArticle->mDefaultConductivity * (1.0 - mArticle->mMalfBlockageValue);
    const double expectedPower = -expectedFlux * expectedDrop;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDrop,  mArticle->mPotentialDrop, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,  mArticle->mFlux,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower, mArticle->mPower,         DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test setter & getter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicPotential::testAccessMethods()
{
    std::cout << "\n UtGunnsBasicPotential .. 08: testAccessMethods .....................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// - Test the setSourcePotential method.
    mArticle->setSourcePotential(42.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(42.0, mArticle->mSourcePotential, 0.0);

    std::cout << "... Pass";
}
