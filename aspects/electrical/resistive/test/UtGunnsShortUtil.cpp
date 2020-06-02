/**
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((aspects/electrical/resistive/GunnsShortUtil.o))
*/

#include "UtGunnsShortUtil.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsShortUtil class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsShortUtil::UtGunnsShortUtil()
    :
    tArticle(),
    tMalfMinimumVoltage(0.0),
    tMalfConstantPowerFlag(false),
    tMalfConstantPowerValue(0.0),
    tMalfConstantCurrentFlag(false),
    tMalfConstantCurrentValue(0.0),
    tMalfInitialPowerFlag(false),
    tMalfInitialPowerValue(0.0),
    tMalfInitialCurrentFlag(false),
    tMalfInitialCurrentValue(0.0),
    tMalfResistanceFlag(false),
    tMalfResistanceValue(0.0),
    tInputData()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsShortUtil class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsShortUtil::~UtGunnsShortUtil()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsShortUtil::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tInputData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsShortUtil::setUp()
{
    /// - Create nominal input data
    tMalfMinimumVoltage       = 3.0;
    tMalfConstantPowerFlag    = true;
    tMalfConstantPowerValue   = 2.0;
    tMalfConstantCurrentFlag  = true;
    tMalfConstantCurrentValue = 1.0;
    tMalfInitialPowerFlag     = true;
    tMalfInitialPowerValue    = 4.0;
    tMalfInitialCurrentFlag   = true;
    tMalfInitialCurrentValue  = 5.0;
    tMalfResistanceFlag       = true;
    tMalfResistanceValue      = 10.0;
    tInputData = new GunnsShortUtilInputData(tMalfMinimumVoltage,
                                             tMalfConstantPowerFlag,
                                             tMalfConstantPowerValue,
                                             tMalfConstantCurrentFlag,
                                             tMalfConstantCurrentValue,
                                             tMalfInitialPowerFlag,
                                             tMalfInitialPowerValue,
                                             tMalfInitialCurrentFlag,
                                             tMalfInitialCurrentValue,
                                             tMalfResistanceFlag,
                                             tMalfResistanceValue);

    /// - Create the test article
    tArticle = new FriendlyGunnsShortUtil;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the input data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsShortUtil::testInput()
{
    std::cout << "\n ----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsShortUtil ...... 01: testInput .............................";

    /// - Check nominal input construction
    CPPUNIT_ASSERT_EQUAL(tMalfMinimumVoltage,       tInputData->mMalfMinimumVoltage);
    CPPUNIT_ASSERT_EQUAL(tMalfConstantPowerFlag,    tInputData->mMalfConstantPowerFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfConstantPowerValue,   tInputData->mMalfConstantPowerValue);
    CPPUNIT_ASSERT_EQUAL(tMalfConstantCurrentFlag,  tInputData->mMalfConstantCurrentFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfConstantCurrentValue, tInputData->mMalfConstantCurrentValue);
    CPPUNIT_ASSERT_EQUAL(tMalfInitialPowerFlag,     tInputData->mMalfInitialPowerFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfInitialPowerValue,    tInputData->mMalfInitialPowerValue);
    CPPUNIT_ASSERT_EQUAL(tMalfInitialCurrentFlag,   tInputData->mMalfInitialCurrentFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfInitialCurrentValue,  tInputData->mMalfInitialCurrentValue);
    CPPUNIT_ASSERT_EQUAL(tMalfResistanceFlag,       tInputData->mMalfResistanceFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfResistanceValue,      tInputData->mMalfResistanceValue);

    /// - Check default input construction
    GunnsShortUtilInputData defaultInput;
    CPPUNIT_ASSERT_EQUAL(0.0,                       defaultInput.mMalfMinimumVoltage);
    CPPUNIT_ASSERT_EQUAL(false,                     defaultInput.mMalfConstantPowerFlag);
    CPPUNIT_ASSERT_EQUAL(0.0,                       defaultInput.mMalfConstantPowerValue);
    CPPUNIT_ASSERT_EQUAL(false,                     defaultInput.mMalfConstantCurrentFlag);
    CPPUNIT_ASSERT_EQUAL(0.0,                       defaultInput.mMalfConstantCurrentValue);
    CPPUNIT_ASSERT_EQUAL(false,                     defaultInput.mMalfInitialPowerFlag);
    CPPUNIT_ASSERT_EQUAL(0.0,                       defaultInput.mMalfInitialPowerValue);
    CPPUNIT_ASSERT_EQUAL(false,                     defaultInput.mMalfInitialCurrentFlag);
    CPPUNIT_ASSERT_EQUAL(0.0,                       defaultInput.mMalfInitialCurrentValue);
    CPPUNIT_ASSERT_EQUAL(false,                     defaultInput.mMalfResistanceFlag);
    CPPUNIT_ASSERT_EQUAL(0.0,                       defaultInput.mMalfResistanceValue);

    /// - Check copy input construction
    GunnsShortUtilInputData copyInput(*tInputData);
    CPPUNIT_ASSERT_EQUAL(tMalfMinimumVoltage,       copyInput.mMalfMinimumVoltage);
    CPPUNIT_ASSERT_EQUAL(tMalfConstantPowerFlag,    copyInput.mMalfConstantPowerFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfConstantPowerValue,   copyInput.mMalfConstantPowerValue);
    CPPUNIT_ASSERT_EQUAL(tMalfConstantCurrentFlag,  copyInput.mMalfConstantCurrentFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfConstantCurrentValue, copyInput.mMalfConstantCurrentValue);
    CPPUNIT_ASSERT_EQUAL(tMalfInitialPowerFlag,     copyInput.mMalfInitialPowerFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfInitialPowerValue,    copyInput.mMalfInitialPowerValue);
    CPPUNIT_ASSERT_EQUAL(tMalfInitialCurrentFlag,   copyInput.mMalfInitialCurrentFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfInitialCurrentValue,  copyInput.mMalfInitialCurrentValue);
    CPPUNIT_ASSERT_EQUAL(tMalfResistanceFlag,       copyInput.mMalfResistanceFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfResistanceValue,      copyInput.mMalfResistanceValue);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsShortUtil::testDefaultConstruction()
{
    std::cout << "\n UtGunnsShortUtil ...... 02: testDefaultConstruction ...............";

    /// @test config data
    CPPUNIT_ASSERT_EQUAL(0.0,                       tArticle->mMalfMinimumVoltage);
    CPPUNIT_ASSERT_EQUAL(false,                     tArticle->mMalfConstantPowerFlag);
    CPPUNIT_ASSERT_EQUAL(0.0,                       tArticle->mMalfConstantPowerValue);
    CPPUNIT_ASSERT_EQUAL(false,                     tArticle->mMalfConstantCurrentFlag);
    CPPUNIT_ASSERT_EQUAL(0.0,                       tArticle->mMalfConstantCurrentValue);
    CPPUNIT_ASSERT_EQUAL(false,                     tArticle->mMalfInitialPowerFlag);
    CPPUNIT_ASSERT_EQUAL(0.0,                       tArticle->mMalfInitialPowerValue);
    CPPUNIT_ASSERT_EQUAL(false,                     tArticle->mMalfInitialCurrentFlag);
    CPPUNIT_ASSERT_EQUAL(0.0,                       tArticle->mMalfInitialCurrentValue);
    CPPUNIT_ASSERT_EQUAL(false,                     tArticle->mMalfResistanceFlag);
    CPPUNIT_ASSERT_EQUAL(0.0,                       tArticle->mMalfResistanceValue);
    CPPUNIT_ASSERT_EQUAL(0.0,                       tArticle->mShortConductance);
    CPPUNIT_ASSERT_EQUAL(0.0,                       tArticle->mPreviousCurrent);
    CPPUNIT_ASSERT_EQUAL(0.0,                       tArticle->mPreviousPower);

    /// @test new/delete for code coverage
    GunnsShortUtil* article = new GunnsShortUtil();
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsShortUtil::testNominalInitialization()
{
    std::cout << "\n UtGunnsShortUtil ...... 03: testNominalInitialization .............";

    /// - Initialize (with nominal data) the test article.
    tArticle->initialize(*tInputData);

    /// @test input data.
    CPPUNIT_ASSERT_EQUAL(tMalfMinimumVoltage,       tArticle->mMalfMinimumVoltage);
    CPPUNIT_ASSERT_EQUAL(tMalfConstantPowerFlag,    tArticle->mMalfConstantPowerFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfConstantPowerValue,   tArticle->mMalfConstantPowerValue);
    CPPUNIT_ASSERT_EQUAL(tMalfConstantCurrentFlag,  tArticle->mMalfConstantCurrentFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfConstantCurrentValue, tArticle->mMalfConstantCurrentValue);
    CPPUNIT_ASSERT_EQUAL(tMalfInitialPowerFlag,     tArticle->mMalfInitialPowerFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfInitialPowerValue,    tArticle->mMalfInitialPowerValue);
    CPPUNIT_ASSERT_EQUAL(tMalfInitialCurrentFlag,   tArticle->mMalfInitialCurrentFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfInitialCurrentValue,  tArticle->mMalfInitialCurrentValue);
    CPPUNIT_ASSERT_EQUAL(tMalfResistanceFlag,       tArticle->mMalfResistanceFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfResistanceValue,      tArticle->mMalfResistanceValue);

    /// @test state data.
    CPPUNIT_ASSERT_EQUAL(0.0,                       tArticle->mShortConductance);
    CPPUNIT_ASSERT_EQUAL(0.0,                       tArticle->mPreviousCurrent);
    CPPUNIT_ASSERT_EQUAL(0.0,                       tArticle->mPreviousPower);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for the computeShort method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsShortUtil::testComputeShort()
{
    std::cout << "\n UtGunnsShortUtil ...... 04: testComputeShort ......................";

    /// - Initialize (with nominal data) the test article.
    tArticle->initialize(*tInputData);

    const double voltage = 120.0;
    {
        /// @test the constant power malf with positive voltage.
        const double expectedG = tMalfConstantPowerValue / voltage / voltage;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->computeShort(voltage), DBL_EPSILON);
        CPPUNIT_ASSERT(true  == tArticle->isConstantLoad());
    } {
        /// @test the constant power malf with zero voltage.
        const double expectedG = tMalfConstantPowerValue / DBL_EPSILON;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->computeShort(0.0), DBL_EPSILON);
        CPPUNIT_ASSERT(true  == tArticle->isConstantLoad());
    } {
        /// @test the constant power malf with negative malf and negative voltage.
        const double expectedG = tMalfConstantPowerValue / voltage / voltage;
        tArticle->mMalfConstantPowerValue = -tMalfConstantPowerValue;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->computeShort(-voltage), DBL_EPSILON);
        CPPUNIT_ASSERT(true  == tArticle->isConstantLoad());
    } {
        /// @test the constant power malf with zero value.
        const double expectedG = 0.0;
        tArticle->mMalfConstantPowerValue = 0.0;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->computeShort(voltage), DBL_EPSILON);
        CPPUNIT_ASSERT(false == tArticle->isConstantLoad());
    } {
        /// @test the constant current malf with positive voltage.
        tArticle->mMalfConstantPowerFlag = false;
        const double expectedG = tMalfConstantCurrentValue / voltage;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->computeShort(voltage), DBL_EPSILON);
        CPPUNIT_ASSERT(true  == tArticle->isConstantLoad());
    } {
        /// @test the constant current malf with zero voltage.
        const double expectedG = tMalfConstantCurrentValue / DBL_EPSILON;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->computeShort(0.0), DBL_EPSILON);
        CPPUNIT_ASSERT(true  == tArticle->isConstantLoad());
    } {
        /// @test the constant current malf with negative malf and negative voltage.
        const double expectedG = tMalfConstantCurrentValue / voltage;
        tArticle->mMalfConstantCurrentValue = -tMalfConstantCurrentValue;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->computeShort(-voltage), DBL_EPSILON);
        CPPUNIT_ASSERT(true  == tArticle->isConstantLoad());
    } {
        /// @test the constant current malf with zero value.
        const double expectedG = 0.0;
        tArticle->mMalfConstantCurrentValue = 0.0;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->computeShort(voltage), DBL_EPSILON);
        CPPUNIT_ASSERT(false == tArticle->isConstantLoad());
    } {
        /// @test the initial power malf with positive voltage.
        tArticle->mMalfConstantCurrentFlag = false;
        tMalfInitialPowerValue             = 4.2;
        tArticle->mMalfInitialPowerValue   = tMalfInitialPowerValue;
        const double expectedG = tMalfInitialPowerValue / voltage / voltage;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->computeShort(voltage), DBL_EPSILON);
        CPPUNIT_ASSERT(false == tArticle->isConstantLoad());
    } {
        /// @test the initial power malf with an old value.
        const double expectedG = tMalfInitialPowerValue / voltage / voltage;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->computeShort(voltage/2.0), DBL_EPSILON);
        CPPUNIT_ASSERT(false == tArticle->isConstantLoad());
    } {
        /// @test the initial power malf with zero voltage.
        tMalfInitialPowerValue             = 3.3;
        tArticle->mMalfInitialPowerValue   = tMalfInitialPowerValue;
        const double expectedG = tMalfInitialPowerValue / DBL_EPSILON;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->computeShort(0.0), DBL_EPSILON);
        CPPUNIT_ASSERT(false == tArticle->isConstantLoad());
    } {
        /// @test the initial power malf with negative malf and negative voltage.
        tMalfInitialPowerValue             = -1.0;
        tArticle->mMalfInitialPowerValue   = tMalfInitialPowerValue;
        const double expectedG = -tMalfInitialPowerValue / voltage / voltage;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->computeShort(-voltage), DBL_EPSILON);
        CPPUNIT_ASSERT(false == tArticle->isConstantLoad());
    } {
        /// @test the initial current malf with positive voltage.
        tArticle->mMalfInitialPowerFlag    = false;
        tMalfInitialCurrentValue           = 4.2;
        tArticle->mMalfInitialCurrentValue = tMalfInitialCurrentValue;
        const double expectedG = tMalfInitialCurrentValue / voltage;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->computeShort(voltage), DBL_EPSILON);
        CPPUNIT_ASSERT(false == tArticle->isConstantLoad());
    } {
        /// @test the initial current malf with an old value.
        const double expectedG = tMalfInitialCurrentValue / voltage;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->computeShort(voltage/2.0), DBL_EPSILON);
        CPPUNIT_ASSERT(false == tArticle->isConstantLoad());
    } {
        /// @test the initial current malf with zero voltage.
        tMalfInitialCurrentValue           = 3.3;
        tArticle->mMalfInitialCurrentValue = tMalfInitialCurrentValue;
        const double expectedG = tMalfInitialCurrentValue / DBL_EPSILON;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->computeShort(0.0), DBL_EPSILON);
        CPPUNIT_ASSERT(false == tArticle->isConstantLoad());
    } {
        /// @test the initial current malf with negative malf and negative voltage.
        tMalfInitialCurrentValue           = -1.0;
        tArticle->mMalfInitialCurrentValue = tMalfInitialCurrentValue;
        const double expectedG = -tMalfInitialCurrentValue / voltage;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->computeShort(-voltage), DBL_EPSILON);
        CPPUNIT_ASSERT(false == tArticle->isConstantLoad());
    } {
        /// @test the resistance malf.
        tArticle->mMalfInitialCurrentFlag = false;
        const double expectedG = 1.0 / tMalfResistanceValue;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->computeShort(voltage), DBL_EPSILON);
        CPPUNIT_ASSERT(false == tArticle->isConstantLoad());
    } {
        /// @test the resistance malf with zero value.
        tMalfResistanceValue           = 0.0;
        tArticle->mMalfResistanceValue = tMalfResistanceValue;
        const double expectedG = 1.0 / DBL_EPSILON;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->computeShort(voltage), DBL_EPSILON);
        CPPUNIT_ASSERT(false == tArticle->isConstantLoad());
    } {
        /// @test with no malf.
        tArticle->mMalfResistanceFlag = false;
        const double expectedG = 0.0;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->computeShort(voltage), DBL_EPSILON);
        CPPUNIT_ASSERT(false == tArticle->isConstantLoad());
    }

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Process Outputs.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsShortUtil::testAccessors()
{
    std::cout << "\n UtGunnsShortUtil ...... 05: testAccessors .........................";

    /// @test set the constant power malf.
    tArticle->setMalfConstantPower(true, 9.0, -1.0);
    CPPUNIT_ASSERT_EQUAL(-1.0,  tArticle->mMalfMinimumVoltage);
    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mMalfConstantPowerFlag);
    CPPUNIT_ASSERT_EQUAL(9.0,   tArticle->mMalfConstantPowerValue);

    /// @test reset the constant power malf.
    tArticle->setMalfConstantPower();
    CPPUNIT_ASSERT_EQUAL(0.0,   tArticle->mMalfMinimumVoltage);
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mMalfConstantPowerFlag);
    CPPUNIT_ASSERT_EQUAL(0.0,   tArticle->mMalfConstantPowerValue);

    /// @test set the constant current malf.
    tArticle->setMalfConstantCurrent(true, 9.0, -1.0);
    CPPUNIT_ASSERT_EQUAL(-1.0,  tArticle->mMalfMinimumVoltage);
    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mMalfConstantCurrentFlag);
    CPPUNIT_ASSERT_EQUAL(9.0,   tArticle->mMalfConstantCurrentValue);

    /// @test reset the constant current malf.
    tArticle->setMalfConstantCurrent();
    CPPUNIT_ASSERT_EQUAL(0.0,   tArticle->mMalfMinimumVoltage);
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mMalfConstantCurrentFlag);
    CPPUNIT_ASSERT_EQUAL(0.0,   tArticle->mMalfConstantCurrentValue);

    /// @test set the initial power malf.
    tArticle->setMalfInitialPower(true, 9.0, -1.0);
    CPPUNIT_ASSERT_EQUAL(-1.0,  tArticle->mMalfMinimumVoltage);
    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mMalfInitialPowerFlag);
    CPPUNIT_ASSERT_EQUAL(9.0,   tArticle->mMalfInitialPowerValue);

    /// @test reset the initial power malf.
    tArticle->setMalfInitialPower();
    CPPUNIT_ASSERT_EQUAL(0.0,   tArticle->mMalfMinimumVoltage);
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mMalfInitialPowerFlag);
    CPPUNIT_ASSERT_EQUAL(0.0,   tArticle->mMalfInitialPowerValue);

    /// @test set the initial current malf.
    tArticle->setMalfInitialCurrent(true, 9.0, -1.0);
    CPPUNIT_ASSERT_EQUAL(-1.0,  tArticle->mMalfMinimumVoltage);
    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mMalfInitialCurrentFlag);
    CPPUNIT_ASSERT_EQUAL(9.0,   tArticle->mMalfInitialCurrentValue);

    /// @test reset the initial current malf.
    tArticle->setMalfInitialCurrent();
    CPPUNIT_ASSERT_EQUAL(0.0,   tArticle->mMalfMinimumVoltage);
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mMalfInitialCurrentFlag);
    CPPUNIT_ASSERT_EQUAL(0.0,   tArticle->mMalfInitialCurrentValue);

    /// @test set the resistance malf.
    tArticle->setMalfResistance(true, 9.0, -1.0);
    CPPUNIT_ASSERT_EQUAL(-1.0,  tArticle->mMalfMinimumVoltage);
    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mMalfResistanceFlag);
    CPPUNIT_ASSERT_EQUAL(9.0,   tArticle->mMalfResistanceValue);

    /// @test reset the resistance malf.
    tArticle->setMalfResistance();
    CPPUNIT_ASSERT_EQUAL(0.0,   tArticle->mMalfMinimumVoltage);
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mMalfResistanceFlag);
    CPPUNIT_ASSERT_EQUAL(0.0,   tArticle->mMalfResistanceValue);


//    /// - Initialize default test article with nominal initialization data
//    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
//
//    /// - Set the potential difference across the link like Gunns would do.
//    ///   Power should be valid in this case because the input voltage to the load
//    ///   is higher than the under voltage limit.
//    mArticle->mPotentialVector[0] = 28.0;
//    mArticle->mPotentialVector[1] = 0.0;
//    mArticle->step(mTimeStep);
//    mArticle->computeFlows(mTimeStep);
//    mArticle->processOutputs();
//
//    CPPUNIT_ASSERT(mArticle->mPowerValid);
//
//    /// - Set the potential difference across the link like Gunns would do.
//    ///   Power should be invalid in this case because the input voltage to the load
//    ///   is lower than the under voltage limit.
//    mArticle->mPotentialVector[0] = 23.4;
//    mArticle->mPotentialVector[1] = 0.0;
//    mArticle->step(mTimeStep);
//    mArticle->computeFlows(mTimeStep);
//    mArticle->processOutputs();
//
//    CPPUNIT_ASSERT(!mArticle->mPowerValid);

    std::cout << "... Pass";
}
