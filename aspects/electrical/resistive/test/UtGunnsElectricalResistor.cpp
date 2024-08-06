/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    (
        (aspects/electrical/resistive/GunnsElectricalResistor.o)
    )
***************************************************************************************************/
#include "UtGunnsElectricalResistor.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsElectricalResistor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectricalResistor::UtGunnsElectricalResistor()
    :
    mConfigData(),
    mInputData(),
    mArticle(),
    mLinkName(),
    mResistance(),
    mVoltageDrop(),
    mCurrent(),
    mElectricalEfficiency(),
    mWasteHeat(),
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
/// @details  This is the default destructor for the UtGunnsElectricalResistor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectricalResistor::~UtGunnsElectricalResistor()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectricalResistor::tearDown()
{
    /// - Deletes for news in setUp
    delete mArticle;
    delete mInputData;
    delete mConfigData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectricalResistor::setUp()
{
    mLinkName             = "Test Electrical Resistor";
    mNodeList.mNumNodes   = 2;
    mNodeList.mNodes      = mNodes;
    mResistance           = 1000.0;
    mVoltageIn            = 28.5;
    mVoltageOut           = 28.5;
    mVoltageDrop          = mVoltageIn - mVoltageOut;
    mCurrent              = 0.3;
    mElectricalEfficiency = 0.5;
    mWasteHeat            = 105.2;
    mBlockage             = 0.2;
    mPort0                = 0;
    mPort1                = 1;

    /// - Define nominal configuration data
    mConfigData = new GunnsElectricalResistorConfigData(mLinkName, &mNodeList, mResistance,
                                                        mElectricalEfficiency);

    /// - Create nominal input data
    mInputData = new GunnsElectricalResistorInputData(true, mBlockage);

    /// - Create the test article
    mArticle = new FriendlyGunnsElectricalResistor;

    /// - Declare the nominal test data
    mTolerance = 1.0e-08;
    mTimeStep = 0.1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectricalResistor::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsElectricalResistor .. 01: testConfig ............................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(mLinkName            == mConfigData->mName);
    CPPUNIT_ASSERT(mNodes               == mConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(mResistance == mConfigData->mResistance);
    CPPUNIT_ASSERT(mElectricalEfficiency == mConfigData->mElectricalEfficiency);

    /// - Check default config construction
    GunnsElectricalResistorConfigData defaultConfig;
    CPPUNIT_ASSERT(""                   == defaultConfig.mName);
    CPPUNIT_ASSERT(0                    == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0                  == defaultConfig.mResistance);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mElectricalEfficiency);

    /// - Check copy config construction
    GunnsElectricalResistorConfigData copyConfig(*mConfigData);
    CPPUNIT_ASSERT(mLinkName            == copyConfig.mName);
    CPPUNIT_ASSERT(mNodes               == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(mResistance == copyConfig.mResistance);
    CPPUNIT_ASSERT(mElectricalEfficiency == copyConfig.mElectricalEfficiency);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the input data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectricalResistor::testInput()
{
    std::cout << "\n UtGunnsElectricalResistor .. 02: testInput .............................";

    /// - Check nominal input construction
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mBlockage, mInputData->mMalfBlockageValue, 0.0);

    /// - Check default input construction
    GunnsElectricalResistorInputData defaultInput;
    CPPUNIT_ASSERT(!defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0 == defaultInput.mMalfBlockageValue);

    /// - Check copy input construction
    GunnsElectricalResistorInputData copyInput(*mInputData);
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfBlockageValue, copyInput.mMalfBlockageValue, 0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectricalResistor::testDefaultConstruction()
{
    std::cout << "\n UtGunnsElectricalResistor .. 03: testDefaultConstruction ...............";

    /// @test config data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mResistance,   0.0);

    /// @test class member default construction
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mVoltageIn,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mVoltageOut,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mVoltageDrop,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mCurrent,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mElectricalEfficiency,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mWasteHeat,   0.0);

    /// @test init flag
    CPPUNIT_ASSERT(!mArticle->mInitFlag);

    /// @test new/delete for code coverage
    GunnsElectricalResistor* article = new GunnsElectricalResistor();
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectricalResistor::testNominalInitialization()
{
    std::cout << "\n UtGunnsElectricalResistor .. 04: testNominalInitialization .............";

    /// - Default construct and initialize (with nominal data) a test article
    FriendlyGunnsElectricalResistor article;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test config data
    CPPUNIT_ASSERT(mResistance == article.mResistance);
    CPPUNIT_ASSERT(mElectricalEfficiency == article.mElectricalEfficiency);

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
void UtGunnsElectricalResistor::testInitializationExceptions()
{
    std::cout << "\n UtGunnsElectricalResistor .. 05: testInitializationExceptions ..........";

    /// - Default construct a test article
    GunnsElectricalResistor article;

    /// @test for exception on invalid config data: conductivity < 0
    mConfigData->mResistance = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);

    /// @test for exception on invalid config data: 0.0 <= electrical efficiency <= 1.0
    mConfigData->mElectricalEfficiency = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);

    mConfigData->mElectricalEfficiency = 1.0 + DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Update State.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectricalResistor::testUpdateState()
{
    std::cout << "\n UtGunnsElectricalResistor 07: testUpdateState ......................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    mArticle->updateState(mTimeStep);

    /// - Check the potential drop, flux, and power
    const double expectedResistance  = mResistance;
    const double expectedConductance  = 1/mResistance;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedResistance,   mArticle->mResistance, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedConductance,  mArticle->mEffectiveConductivity, DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectricalResistor::testComputeFlows()
{
    std::cout << "\n UtGunnsElectricalResistor .. 07: testComputeFlows ......................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// - Set the potential difference across the link like Gunns would do.
    mArticle->mPotentialVector[0] = 28.0;
    mArticle->mPotentialVector[1] = 0.0;

    mArticle->step(mTimeStep);
    mArticle->computeFlows(mTimeStep);

    /// - Check the potential drop, flux, and power
    const double expectedVoltageIn     = mArticle->mPotentialVector[0];
    const double expectedVoltageOut    = mArticle->mPotentialVector[1];
    const double expectedVoltageDrop   = mArticle->mPotentialVector[0] - mArticle->mPotentialVector[1];
    const double expectedCurrent       = expectedVoltageDrop * (1.0/mResistance) * (1.0 - mBlockage);
    const double expectedPower         = -expectedCurrent * expectedVoltageDrop;
    const double expectedWasteHeat     = -(1.0 - mElectricalEfficiency) * expectedPower;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVoltageIn,   mArticle->mVoltageIn,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVoltageOut,  mArticle->mVoltageOut,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVoltageDrop, mArticle->mVoltageDrop, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCurrent,     mArticle->mCurrent,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower,       mArticle->mPower,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedWasteHeat,   mArticle->mWasteHeat,   DBL_EPSILON);

    std::cout << "... Pass";
}
