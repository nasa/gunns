/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    (
        (core/GunnsBasicCapacitor.o)
    )
***************************************************************************************************/
#include "UtGunnsBasicCapacitor.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsBasicCapacitor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsBasicCapacitor::UtGunnsBasicCapacitor()
    :
    mConfigData(),
    mInputData(),
    mArticle(),
    mName(),
    mInitialCapacitance(),
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
/// @details  This is the default destructor for the UtGunnsBasicCapacitor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsBasicCapacitor::~UtGunnsBasicCapacitor()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicCapacitor::tearDown()
{
    /// - Deletes for news in setUp
    delete mArticle;
    delete mInputData;
    delete mConfigData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicCapacitor::setUp()
{
    /// - Set the link default data
    mName                 = "Test Basic Capacitor";
    mNodeList.mNumNodes   = 2;
    mNodeList.mNodes      = mNodes;
    mInitialCapacitance   = 0.5;
    mInitialPotential     = 0.1;
    mPort0                = 0;
    mPort1                = 1;

    /// - Define configuration data
    mConfigData = new GunnsBasicCapacitorConfigData(mName, &mNodeList);

    /// - Define input data
    mInputData = new GunnsBasicCapacitorInputData(true, 0.5,
            mInitialCapacitance, mInitialPotential);

    /// - Create the test article
    mArticle = new FriendlyGunnsBasicCapacitor;

    /// - Declare the nominal test data
    mTolerance = 1.0e-08;
    mTimeStep = 0.1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicCapacitor::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsBasicCapacitor .. 01: testConfig ............................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(mName               == mConfigData->mName);
    CPPUNIT_ASSERT(mNodes              == mConfigData->mNodeList->mNodes);

    /// - Check default config construction
    GunnsBasicCapacitorConfigData defaultConfig;
    CPPUNIT_ASSERT(""   == defaultConfig.mName);
    CPPUNIT_ASSERT(0    == defaultConfig.mNodeList);

    /// - Check copy config construction
    GunnsBasicCapacitorConfigData copyConfig(*mConfigData);
    CPPUNIT_ASSERT(mName                 == copyConfig.mName);
    CPPUNIT_ASSERT(mNodes                == copyConfig.mNodeList->mNodes);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the input data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicCapacitor::testInput()
{
    std::cout << "\n UtGunnsBasicCapacitor .. 02: testInput .............................";

    /// - Check nominal input data
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, mInputData->mMalfBlockageValue, 0.0);
    CPPUNIT_ASSERT(mInitialCapacitance == mInputData->mCapacitance);
    CPPUNIT_ASSERT(mInitialPotential   == mInputData->mPotential);

    /// - Check default input data
    GunnsBasicCapacitorInputData defaultInput;
    CPPUNIT_ASSERT(!defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0 == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0 == defaultInput.mCapacitance);
    CPPUNIT_ASSERT(0.0 == defaultInput.mPotential);

    /// - Check copy input data
    GunnsBasicCapacitorInputData copyInput(*mInputData);
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfBlockageValue, copyInput.mMalfBlockageValue, 0.0);
    CPPUNIT_ASSERT(mInitialCapacitance == copyInput.mCapacitance);
    CPPUNIT_ASSERT(mInitialPotential   == copyInput.mPotential);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicCapacitor::testDefaultConstruction()
{
    std::cout << "\n UtGunnsBasicCapacitor .. 03: testDefaultConstruction ...............";

    /// @test config data
    CPPUNIT_ASSERT(0.0   == mArticle->mCapacitance);
    CPPUNIT_ASSERT(false == mArticle->mEditCapacitanceFlag);
    CPPUNIT_ASSERT(0.0   == mArticle->mEditCapacitance);

    /// @test init flag
    CPPUNIT_ASSERT(!mArticle->mInitFlag);

    /// @test new/delete for code coverage
    GunnsBasicCapacitor* article = new GunnsBasicCapacitor();
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicCapacitor::testNominalInitialization()
{
    std::cout << "\n UtGunnsBasicCapacitor .. 04: testNominalInitialization .............";

    /// - Default construct and initialize (with nominal data) a test article
    FriendlyGunnsBasicCapacitor article;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test config data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInitialCapacitance, article.mCapacitance, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInitialPotential, article.mPotentialVector[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInitialPotential, mNodes[0].getPotential(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, article.mMalfBlockageValue, 0.0);

    /// @test init flag
    CPPUNIT_ASSERT(article.mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for initialization exceptions on invalid properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicCapacitor::testInitializationExceptions()
{
    std::cout << "\n UtGunnsBasicCapacitor .. 05: testInitializationExceptions ..........";

    /// - Default construct a test article
    GunnsBasicCapacitor article;

    /// @test for exception on invalid input data: capacitance < 0
    mInputData->mCapacitance = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the step method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicCapacitor::testStep()
{
    std::cout << "\n UtGunnsBasicCapacitor .. 06: testStep ..............................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->step(mTimeStep);

    /// - during step if the active capacitance doesn't equal the capacitance then the active
    ///   capacitance will get reset to the current capacitance.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mAdmittanceMatrix[0] * mTimeStep,
            mArticle->mCapacitance, mTolerance);

    /// - Since the initial potential is greater than zero then the source vector should be
    ///   greater than zero.
    if (mArticle->mSourceVector[0] > 0.0) {
        CPPUNIT_ASSERT(true);
    } else {
        CPPUNIT_ASSERT(false);
    }

    /// - Test a capacitance edit
    const double capacitance = mArticle->mCapacitance;

    mArticle->editCapacitance(true, 2.0);
    mArticle->step(mTimeStep);

    CPPUNIT_ASSERT(false == mArticle->mEditCapacitanceFlag);
    CPPUNIT_ASSERT(2.0   == mArticle->getCapacitance());

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicCapacitor::testComputeFlows()
{
    std::cout << "\n UtGunnsBasicCapacitor .. 07: testComputeFlows ......................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    mArticle->mMalfBlockageFlag = false;
    mArticle->step(mTimeStep);

    /// - Set the potential difference across the link like Gunns would do.  We will test flux and
    ///   power when the ground is not perfect.
    mArticle->mPotentialVector[0] = 0.1;
    mArticle->mPotentialVector[1] = 0.01;

    mArticle->computeFlows(mTimeStep);

    /// - Check the potential drop, flux, and power
    const double expectedDrop  = mArticle->mPotentialVector[0] - mArticle->mPotentialVector[1];
    const double expectedFlux  = expectedDrop * mArticle->mCapacitance / mTimeStep
                               - mArticle->mSourceVector[0];
    const double expectedPower = -expectedFlux * expectedDrop;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDrop,  mArticle->mPotentialDrop, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,  mArticle->mFlux,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower, mArticle->mPower,         DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the step method when the dt argument is zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicCapacitor::testZeroDtStep()
{
    std::cout << "\n UtGunnsBasicCapacitor .. 08: testZeroDtStep ........................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->step(mTimeStep);
    mArticle->step(0.0);

    /// - With dt = 0, admittance matrix should be capacitance divided by epsilon.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mCapacitance / DBL_EPSILON,
            mArticle->mAdmittanceMatrix[0], mTolerance);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the access methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicCapacitor::testAccessMethods()
{
    std::cout << "\n UtGunnsBasicCapacitor .. 09: testAccessMethods .....................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->step(mTimeStep);
    mArticle->step(0.0);

    /// - Test getters
    mArticle->mCapacitance=5.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, mArticle->getCapacitance(),0.0);

    std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the access methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicCapacitor::testModifierMethods()
{
    std::cout << "\n UtGunnsBasicCapacitor .. 10: testModifierMethods ...................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->step(mTimeStep);
    mArticle->step(0.0);

    /// - Test getters
    mArticle->setCapacitance(2.5);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.5, mArticle->getCapacitance(),0.0);

    /// - Test the edit capacitance method.
    mArticle->editCapacitance(true, 42.0);
    CPPUNIT_ASSERT(true == mArticle->mEditCapacitanceFlag);
    CPPUNIT_ASSERT(42.0 == mArticle->mEditCapacitance);

    std::cout << "... Pass";
}
