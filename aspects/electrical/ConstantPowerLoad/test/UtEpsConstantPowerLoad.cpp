/**
@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/

#include "UtEpsConstantPowerLoad.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtEpsConstantPowerLoad class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtEpsConstantPowerLoad::UtEpsConstantPowerLoad()
    :
    mConfigData(),
    mInputData(),
    mArticle(),
    mLinkName(),
    mInitialConductivity(),
    mDefaultPower(),
    mMinimumVoltageLimit(),
    mNodes(),
    mNodeList(),
    mLinks(),
    mPort0(),
    mPort1(),
    mTimeStep(),
    mTolerance() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtEpsConstantPowerLoad class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtEpsConstantPowerLoad::~UtEpsConstantPowerLoad() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtEpsConstantPowerLoad::tearDown() {
    /// - Deletes for news in setUp
    delete mArticle;
    delete mInputData;
    delete mConfigData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtEpsConstantPowerLoad::setUp() {
    mLinkName             = "Test Basic ConstantLoad";
    mNodeList.mNumNodes   = 2;
    mNodeList.mNodes      = mNodes;
    mInitialConductivity  = 0.1;
    mDefaultPower         = 0.2;
    mMinimumVoltageLimit  = 0.4;
    mPort0                = 0;
    mPort1                = 1;

    /// - Define nominal configuration data
    mConfigData = new EpsConstantPowerLoadConfigData(
            mLinkName,
            &mNodeList,
            mInitialConductivity,
            mDefaultPower,
            mMinimumVoltageLimit);

    /// - Define nominal input data
    mInputData = new EpsConstantPowerLoadInputData(true, 0.5);

    mArticle = new FriendlyEpsConstantPowerLoad;

    /// - Declare the nominal test data
    mTolerance = 1.0e-08;
    mTimeStep = 0.1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtEpsConstantPowerLoad::testConfig() {
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtEpsConstantPowerLoad 01: testConfig ............................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(mLinkName             == mConfigData->mName);
    CPPUNIT_ASSERT(mNodes                == mConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(mInitialConductivity  == mConfigData->mDefaultConductivity);
    CPPUNIT_ASSERT(mDefaultPower         == mConfigData->mDefaultPower);
    CPPUNIT_ASSERT(mMinimumVoltageLimit == mConfigData->mMinimumVoltageLimit);

    /// - Check default config construction
    EpsConstantPowerLoadConfigData defaultConfig;
    CPPUNIT_ASSERT(""    == defaultConfig.mName);
    CPPUNIT_ASSERT(0     == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0   == defaultConfig.mDefaultConductivity);
    CPPUNIT_ASSERT(0.0   == defaultConfig.mDefaultPower);
    CPPUNIT_ASSERT(0.0   == defaultConfig.mMinimumVoltageLimit);


    /// - Check copy config construction
    EpsConstantPowerLoadConfigData copyConfig(*mConfigData);
    CPPUNIT_ASSERT(mLinkName             == copyConfig.mName);
    CPPUNIT_ASSERT(mNodes                == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(mInitialConductivity  == copyConfig.mDefaultConductivity);
    CPPUNIT_ASSERT(mDefaultPower         == copyConfig.mDefaultPower);
    CPPUNIT_ASSERT(mMinimumVoltageLimit == copyConfig.mMinimumVoltageLimit);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the input data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtEpsConstantPowerLoad::testInput() {
    std::cout << "\n UtEpsConstantPowerLoad 02: testInput .............................";

    /// - Check nominal input potential
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, mInputData->mMalfBlockageValue, 0.0);

    /// - Check default input potential
    EpsConstantPowerLoadInputData defaultInput;
    CPPUNIT_ASSERT(!defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultInput.mMalfBlockageValue, 0.0);

    /// - Check copy input potential
    EpsConstantPowerLoadInputData copyInput(*mInputData);
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfBlockageValue, copyInput.mMalfBlockageValue, 0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtEpsConstantPowerLoad::testDefaultConstruction() {
    std::cout << "\n UtEpsConstantPowerLoad 03: testDefaultConstruction ...............";

    /// @test the default constructed values
    CPPUNIT_ASSERT(0.0 == mArticle->mDesiredPower);
    CPPUNIT_ASSERT(0.0 == mArticle->mMinimumVoltageLimit);
    CPPUNIT_ASSERT(0.0 == mArticle->mPower);

    /// @test init flag
    CPPUNIT_ASSERT(!mArticle->mInitFlag);

    /// @test new/delete for code coverage
    EpsConstantPowerLoad* article = new EpsConstantPowerLoad();
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtEpsConstantPowerLoad::testNominalInitialization() {
    std::cout << "\n UtEpsConstantPowerLoad 04: testNominalInitialization .............";

    /// - Default construct and initialize (with nominal data) a test article
    FriendlyEpsConstantPowerLoad article;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test config data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mDefaultPower, article.mDesiredPower, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMinimumVoltageLimit, article.mMinimumVoltageLimit, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, article.mMalfBlockageValue, 0.0);

    /// @test init flag
    CPPUNIT_ASSERT(article.mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for initialization exceptions on invalid properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtEpsConstantPowerLoad::testInitializationExceptions() {
    std::cout << "\n UtEpsConstantPowerLoad 05: testInitializationExceptions ..........";

    /// - Default construct a test article
    EpsConstantPowerLoad article;

    /// @test for exception on invalid config data: default power < 0
    mConfigData->mDefaultPower = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mDefaultPower = mDefaultPower;

    /// @test for exception on invalid config data: minimum voltage limit < 0
    mConfigData->mMinimumVoltageLimit = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtEpsConstantPowerLoad::testAccessors() {
    std::cout << "\n UtEpsConstantPowerLoad 06: testAccessors .........................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test to make sure a constant load returns that it is a non-linear link
    const bool expected = true;
    const bool returned = mArticle->isNonLinear();
    CPPUNIT_ASSERT_EQUAL(expected, returned);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtEpsConstantPowerLoad::testModifiers() {
    std::cout << "\n UtEpsConstantPowerLoad 07: testModifiers .........................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test for more than nominal full open position
    const double power = 0.5;
    const double potential = 0.41;
    const double conductivity = power / potential / potential;
    mArticle->setPowerAndNominalPotential(power, potential);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(conductivity, mArticle->mEffectiveConductivity, mTolerance);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtEpsConstantPowerLoad::testStep() {
    std::cout << "\n UtEpsConstantPowerLoad 08: testStep ..............................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// - Load up a negative Potential vector
    mArticle->mPotentialVector[0] = 0.0;
    mArticle->mPotentialVector[1] = 0.1;
    double conductivity = 0.0;

    mArticle->step(mTimeStep);

    /// - during step the conductance will be reset to lower conductance limit.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(conductivity,
            mArticle->mEffectiveConductivity, mTolerance);

    /// - during step if the active conductance doesn't equal the conductance then the active
    ///   conductance will get reset to the current conductance.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mAdmittanceMatrix[0], mArticle->mSystemConductance,
            mTolerance);

    /// - Load up a positive Potential vector
    mArticle->mPotentialVector[0] = 1.5;
    mArticle->mPotentialVector[1] = 0.0;

    /// - Calculate conductivity based on (1 / R) = P / V^2
    conductivity = mDefaultPower / (1.5 * 1.5);

    mArticle->step(mTimeStep);

    /// - during step the effective conductivity will get recalculated from Joules and Ohms law.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(conductivity, mArticle->mEffectiveConductivity, mTolerance);

    /// - Call minorStep and verify computeFlows and step are called again.  Check for actual power
    ///   and conductivity to change.
    conductivity = mArticle->mEffectiveConductivity;
    double power = mArticle->mPower;

    mArticle->mPotentialVector[0] = 2.5;
    mArticle->minorStep(mTimeStep, 2);

    CPPUNIT_ASSERT(conductivity != mArticle->mEffectiveConductivity);
    CPPUNIT_ASSERT(power        != mArticle->mPower);

    /// - Test for potential less than the link minimum.
    mArticle->mPotentialVector[0] = 0.3;
    conductivity = mDefaultPower / (0.99 * 0.99 * 0.4 * 0.4);
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(conductivity, mArticle->mEffectiveConductivity, mTolerance);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtEpsConstantPowerLoad::testComputeFlows() {
    std::cout << "\n UtEpsConstantPowerLoad 09: testComputeFlows ......................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// - Set the potential difference across the link like Gunns would do.
    mArticle->mPotentialVector[0] = 0.5;
    mArticle->mPotentialVector[1] = 0.4;

    mArticle->step(mTimeStep);
    mArticle->computeFlows(mTimeStep);

    /// - Since the port 0 potential is 0.1 larger than port 1 check the potential drop
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1, mArticle->mPotentialDrop, mTolerance);

    /// - Check to make sure the flux across the link is greater than zero because port0
    ///   potential is larger.
    if (mArticle->mFlux > 0.0) {
        CPPUNIT_ASSERT(true);
    } else {
        CPPUNIT_ASSERT(false);
    }

    std::cout << "... Pass";
}
