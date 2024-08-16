/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    (
        (aspects/electrical/resistive/GunnsResistiveLoad.o)
    )
***************************************************************************************************/
#include "UtGunnsResistiveLoad.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsResistiveLoad class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsResistiveLoad::UtGunnsResistiveLoad()
    :
    mConfigData(),
    mInputData(),
    mArticle(),
    mLinkName(),
    mResistance(),
    mUnderVoltageLimit(),
    mLoadSwitchResistance(),
    mPosTripLimit(),
    mNegTripLimit(),
    mTripPriority(),
    mInitialSwitchPosition(),
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
/// @details  This is the default destructor for the UtGunnsResistiveLoad class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsResistiveLoad::~UtGunnsResistiveLoad()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsResistiveLoad::tearDown()
{
    /// - Deletes for news in setUp
    delete mArticle;
    delete mInputData;
    delete mConfigData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsResistiveLoad::setUp()
{
    mLinkName              = "Test Resistive Load";
    mNodeList.mNumNodes    = 2;
    mNodeList.mNodes       = mNodes;
    mResistance            = 1000.0;
    mElectricalEfficiency  = 0.05;
    mUnderVoltageLimit     = 24.0;
    mLoadSwitchResistance  = 0.06;
    mPosTripLimit          = 3.0;
    mNegTripLimit          = -0.2;
    mTripPriority          = 1;
    mPowerValid            = true;
    mBlockage              = 0.2;
    mInitialSwitchPosition = true;
    mPort0                 = 0;
    mPort1                 = 1;

    /// - Define nominal configuration data
    mConfigData = new GunnsResistiveLoadConfigData(mLinkName, &mNodeList, mResistance,
                                                   mElectricalEfficiency, mUnderVoltageLimit, mLoadSwitchResistance,
                                                   mPosTripLimit, mNegTripLimit, mTripPriority);

    /// - Create nominal input data
    mInputData = new GunnsResistiveLoadInputData(true, mBlockage, mInitialSwitchPosition);

    /// - Create the test article
    mArticle = new FriendlyGunnsResistiveLoad;

    /// - Declare the nominal test data
    mTolerance = 1.0e-08;
    mTimeStep = 0.1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsResistiveLoad::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsResistiveLoad .. 01: testConfig ............................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(mLinkName             == mConfigData->mName);
    CPPUNIT_ASSERT(mNodes                == mConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(mResistance           == mConfigData->mResistance);
    CPPUNIT_ASSERT(mElectricalEfficiency == mConfigData->mElectricalEfficiency);
    CPPUNIT_ASSERT(mUnderVoltageLimit    == mConfigData->mUnderVoltageLimit);
    CPPUNIT_ASSERT(mLoadSwitchResistance == mConfigData->mLoadSwitchResistance);
    CPPUNIT_ASSERT(mLoadSwitchResistance == mConfigData->mLoadSwitchResistance);
    CPPUNIT_ASSERT(mPosTripLimit         == mConfigData->mPosTripLimit);
    CPPUNIT_ASSERT(mNegTripLimit         == mConfigData->mNegTripLimit);
    CPPUNIT_ASSERT(mTripPriority         == mConfigData->mTripPriority);

    /// - Check default config construction
    GunnsResistiveLoadConfigData defaultConfig;
    CPPUNIT_ASSERT(""                    == defaultConfig.mName);
    CPPUNIT_ASSERT(0                     == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mResistance);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mElectricalEfficiency);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mUnderVoltageLimit);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mLoadSwitchResistance);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mLoadSwitchResistance);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mPosTripLimit);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mNegTripLimit);
    CPPUNIT_ASSERT(0                     == defaultConfig.mTripPriority);

    /// - Check copy config construction
    GunnsResistiveLoadConfigData copyConfig(*mConfigData);
    CPPUNIT_ASSERT(mLinkName             == copyConfig.mName);
    CPPUNIT_ASSERT(mNodes                == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(mResistance           == copyConfig.mResistance);
    CPPUNIT_ASSERT(mElectricalEfficiency == copyConfig.mElectricalEfficiency);
    CPPUNIT_ASSERT(mUnderVoltageLimit    == copyConfig.mUnderVoltageLimit);
    CPPUNIT_ASSERT(mLoadSwitchResistance == copyConfig.mLoadSwitchResistance);
    CPPUNIT_ASSERT(mLoadSwitchResistance == copyConfig.mLoadSwitchResistance);
    CPPUNIT_ASSERT(mPosTripLimit         == copyConfig.mPosTripLimit);
    CPPUNIT_ASSERT(mNegTripLimit         == copyConfig.mNegTripLimit);
    CPPUNIT_ASSERT(mTripPriority         == copyConfig.mTripPriority);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the input data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsResistiveLoad::testInput()
{
    std::cout << "\n UtGunnsResistiveLoad .. 02: testInput .............................";

    /// - Check nominal input construction
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mBlockage, mInputData->mMalfBlockageValue, 0.0);
    CPPUNIT_ASSERT(mInputData->mInitialSwitchPosition);

    /// - Check default input construction
    GunnsResistiveLoadInputData defaultInput;
    CPPUNIT_ASSERT(!defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0 == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(defaultInput.mInitialSwitchPosition);

    /// - Check copy input construction
    GunnsResistiveLoadInputData copyInput(*mInputData);
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfBlockageValue, copyInput.mMalfBlockageValue, 0.0);
    CPPUNIT_ASSERT(mInputData->mInitialSwitchPosition == copyInput.mInitialSwitchPosition);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsResistiveLoad::testDefaultConstruction()
{
    std::cout << "\n UtGunnsResistiveLoad .. 03: testDefaultConstruction ...............";

    /// @test config data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mUnderVoltageLimit, 0.0);
    CPPUNIT_ASSERT(!mArticle->mPowerValid);

    /// @test init flag
    CPPUNIT_ASSERT(!mArticle->mInitFlag);

    /// @test new/delete for code coverage
    GunnsResistiveLoad* article = new GunnsResistiveLoad();
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsResistiveLoad::testNominalInitialization()
{
    std::cout << "\n UtGunnsResistiveLoad .. 04: testNominalInitialization .............";

    /// - Default construct and initialize (with nominal data) a test article
    FriendlyGunnsResistiveLoad article;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test config data
    CPPUNIT_ASSERT(mUnderVoltageLimit == article.mUnderVoltageLimit);

    /// @test load switch was initialized properly
    // Want to test that the conductance was set properly. Need to update the switch state b/c there's
    // no way to close the switch at initialization. Have to update the switch state.
    article.mLoadSwitch.updateSwitchState(true);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0/mLoadSwitchResistance, article.mLoadSwitch.getConductance(), mTolerance);

    /// @test init flag
    CPPUNIT_ASSERT(article.mInitFlag);

    /// - Initialize with input data and check it
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mBlockage,            mArticle->mMalfBlockageValue,     0.0);
    CPPUNIT_ASSERT(article.mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for initialization exceptions on invalid properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsResistiveLoad::testInitializationExceptions()
{
    std::cout << "\n UtGunnsResistiveLoad .. 05: testInitializationExceptions ..........";

    /// - Default construct a test article
    GunnsResistiveLoad article;

    /// @test for exception on invalid config data: conductivity < 0
    mConfigData->mUnderVoltageLimit = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Update State.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsResistiveLoad::testUpdateState()
{
    std::cout << "\n UtGunnsResistiveLoad 07: testUpdateState ......................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// - Set the potential difference across the link like Gunns would do.
    ///   now force the switch closed, and verify the load resistance is
    ///   nominal
    mArticle->mLoadSwitch.setSwitchCommandedClosed(true);
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mResistance, mArticle->getResistance(), mTolerance);

    /// - Now command the load switch closed and verify the load resistance becomes effectively zero.
    mArticle->mLoadSwitch.setSwitchCommandedClosed(false);
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0E12,      mArticle->getResistance(), mTolerance);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Process Outputs.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsResistiveLoad::testProcessOutputs()
{
    std::cout << "\n UtGunnsResistiveLoad 08: testProcessOutputs ......................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// - Set the potential difference across the link like Gunns would do.
    ///   Power should be valid in this case because the input voltage to the load
    ///   is higher than the under voltage limit.
    mArticle->mPotentialVector[0] = 28.0;
    mArticle->mPotentialVector[1] = 0.0;
    mArticle->step(mTimeStep);
    mArticle->computeFlows(mTimeStep);
    mArticle->processOutputs();

    CPPUNIT_ASSERT(mArticle->mPowerValid);

    /// - Set the potential difference across the link like Gunns would do.
    ///   Power should be invalid in this case because the input voltage to the load
    ///   is lower than the under voltage limit.
    mArticle->mPotentialVector[0] = 23.4;
    mArticle->mPotentialVector[1] = 0.0;
    mArticle->step(mTimeStep);
    mArticle->computeFlows(mTimeStep);
    mArticle->processOutputs();

    CPPUNIT_ASSERT(!mArticle->mPowerValid);

    std::cout << "... Pass";
}
