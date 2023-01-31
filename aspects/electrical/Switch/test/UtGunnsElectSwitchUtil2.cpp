/*
@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/
#include <iostream>
#include "UtGunnsElectSwitchUtil2.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "strings/UtResult.hh"

/// @details  Test identification number.
int UtGunnsElectSwitchUtil2::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructor for the test object.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectSwitchUtil2::UtGunnsElectSwitchUtil2()
    :
    tResistance(0.0),
    tTripPriority(0),
    tPosition(false),
    tPositionCommand(false),
    tResetTripsCommand(false),
    tInputUnderVoltageTripLimit(0.0),
    tInputUnderVoltageTripReset(0.0),
    tInputOverVoltageTripLimit(0.0),
    tInputOverVoltageTripReset(0.0),
    tPosOverCurrentTripLimit(0.0),
    tNegOverCurrentTripLimit(0.0),
    tConfigData(),
    tInputData(),
    tArticle()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructor for the test object.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectSwitchUtil2::~UtGunnsElectSwitchUtil2()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Create and assign data for each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectSwitchUtil2::setUp()
{
    /// - Create nominal config data.
    tResistance   = 0.0668;
    tTripPriority = 3;

    tConfigData = new GunnsElectSwitchUtil2ConfigData(tResistance, tTripPriority);

    /// - Create nominal input data.
    tPosition                   = true;
    tPositionCommand            = true;
    tResetTripsCommand          = true;
    tInputUnderVoltageTripLimit =  11.0;
    tInputUnderVoltageTripReset =  13.0;
    tInputOverVoltageTripLimit  =  14.0;
    tInputOverVoltageTripReset  =  12.0;
    tPosOverCurrentTripLimit    =  10.0;
    tNegOverCurrentTripLimit    = -10.0;

    tInputData = new GunnsElectSwitchUtil2InputData(tPosition,
                                                    tPositionCommand,
                                                    tResetTripsCommand,
                                                    tInputUnderVoltageTripLimit,
                                                    tInputUnderVoltageTripReset,
                                                    tInputOverVoltageTripLimit,
                                                    tInputOverVoltageTripReset,
                                                    tPosOverCurrentTripLimit,
                                                    tNegOverCurrentTripLimit);

    /// - Create the test article.
    tArticle = new FriendlyGunnsElectSwitchUtil2();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Clean up variables after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectSwitchUtil2::tearDown()
{
    delete tArticle;
    delete tInputData;
    delete tConfigData;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectSwitchUtil2::testConfigData()
{
    UT_RESULT_FIRST;

    /// @test    Nominal construction.
    CPPUNIT_ASSERT(tResistance   == tConfigData->mResistance);
    CPPUNIT_ASSERT(tTripPriority == tConfigData->mTripPriority);

    /// @test    Copy construction.
    GunnsElectSwitchUtil2ConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tResistance   == copyConfig.mResistance);
    CPPUNIT_ASSERT(tTripPriority == copyConfig.mTripPriority);

    /// @test    Default construction.
    GunnsElectSwitchUtil2ConfigData defaultConfig;
    CPPUNIT_ASSERT(0.0 == defaultConfig.mResistance);
    CPPUNIT_ASSERT(0   == defaultConfig.mTripPriority);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectSwitchUtil2::testInputData()
{
    UT_RESULT;

    /// @test    Nominal construction.
    CPPUNIT_ASSERT(tPosition                   == tInputData->mPosition);
    CPPUNIT_ASSERT(tPositionCommand            == tInputData->mPositionCommand);
    CPPUNIT_ASSERT(tResetTripsCommand          == tInputData->mResetTripsCommand);
    CPPUNIT_ASSERT(tInputUnderVoltageTripLimit == tInputData->mInputUnderVoltageTripLimit);
    CPPUNIT_ASSERT(tInputUnderVoltageTripReset == tInputData->mInputUnderVoltageTripReset);
    CPPUNIT_ASSERT(tInputOverVoltageTripLimit  == tInputData->mInputOverVoltageTripLimit);
    CPPUNIT_ASSERT(tInputOverVoltageTripReset  == tInputData->mInputOverVoltageTripReset);
    CPPUNIT_ASSERT(tPosOverCurrentTripLimit    == tInputData->mPosOverCurrentTripLimit);
    CPPUNIT_ASSERT(tNegOverCurrentTripLimit    == tInputData->mNegOverCurrentTripLimit);

    /// @test    Copy construction.
    GunnsElectSwitchUtil2InputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tPosition                   == copyInput.mPosition);
    CPPUNIT_ASSERT(tPositionCommand            == copyInput.mPositionCommand);
    CPPUNIT_ASSERT(tResetTripsCommand          == copyInput.mResetTripsCommand);
    CPPUNIT_ASSERT(tInputUnderVoltageTripLimit == copyInput.mInputUnderVoltageTripLimit);
    CPPUNIT_ASSERT(tInputUnderVoltageTripReset == copyInput.mInputUnderVoltageTripReset);
    CPPUNIT_ASSERT(tInputOverVoltageTripLimit  == copyInput.mInputOverVoltageTripLimit);
    CPPUNIT_ASSERT(tInputOverVoltageTripReset  == copyInput.mInputOverVoltageTripReset);
    CPPUNIT_ASSERT(tPosOverCurrentTripLimit    == copyInput.mPosOverCurrentTripLimit);
    CPPUNIT_ASSERT(tNegOverCurrentTripLimit    == copyInput.mNegOverCurrentTripLimit);

    /// @test    Default construction.
    GunnsElectSwitchUtil2InputData defaultInput;
    CPPUNIT_ASSERT(false == defaultInput.mPosition);
    CPPUNIT_ASSERT(false == defaultInput.mPositionCommand);
    CPPUNIT_ASSERT(false == defaultInput.mResetTripsCommand);
    CPPUNIT_ASSERT(0.0   == defaultInput.mInputUnderVoltageTripLimit);
    CPPUNIT_ASSERT(0.0   == defaultInput.mInputUnderVoltageTripReset);
    CPPUNIT_ASSERT(0.0   == defaultInput.mInputOverVoltageTripLimit);
    CPPUNIT_ASSERT(0.0   == defaultInput.mInputOverVoltageTripReset);
    CPPUNIT_ASSERT(0.0   == defaultInput.mPosOverCurrentTripLimit);
    CPPUNIT_ASSERT(0.0   == defaultInput.mNegOverCurrentTripLimit);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectSwitchUtil2::testDefaultConstruction()
{
    UT_RESULT;

    FriendlyGunnsElectSwitchUtil2 article;
    CPPUNIT_ASSERT(false == article.mResistance);
    CPPUNIT_ASSERT(0.0   == article.mInputVoltage);
    CPPUNIT_ASSERT(0.0   == article.mCurrent);
    CPPUNIT_ASSERT(false == article.mPosition);
    CPPUNIT_ASSERT(false == article.mPositionCommand);
    CPPUNIT_ASSERT(false == article.mResetTripsCommand);
    CPPUNIT_ASSERT(0.0   == article.mInputUnderVoltageTrip.getLimit());
    CPPUNIT_ASSERT(0.0   == article.mInputUnderVoltageReset.getLimit());
    CPPUNIT_ASSERT(0.0   == article.mInputOverVoltageTrip.getLimit());
    CPPUNIT_ASSERT(0.0   == article.mInputOverVoltageReset.getLimit());
    CPPUNIT_ASSERT(0.0   == article.mPosOverCurrentTrip.getLimit());
    CPPUNIT_ASSERT(0.0   == article.mNegOverCurrentTrip.getLimit());
    CPPUNIT_ASSERT(false == article.mWaitingToTrip);
    CPPUNIT_ASSERT(false == article.mJustTripped);
    CPPUNIT_ASSERT(""    == article.mName);

    /// @test    New/delete for code coverage.
    GunnsElectSwitchUtil2* article2 = new GunnsElectSwitchUtil2;
    delete article2;

    UT_PASS;
}

///// @details test for construction of config data
//void UtGunnsElectSwitchUtil2::testInitialization() {
//    std::cout << "\n UtGunnsElectSwitchUtil2 04: testInitialization ......................";
//
//    /// @test   config and input values
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSwitchOnResistance, tArticle->mDefaultSwitchResistance, tTolerance);
//    CPPUNIT_ASSERT(tArticle->mOverCurrentProtection);
////    CPPUNIT_ASSERT(tArticle->mUnderVoltProtection);
////    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMinVoltage, tArticle->mMinVoltage, tTolerance);
//    CPPUNIT_ASSERT(!tArticle->mIsTwoPortSwitch);
//    CPPUNIT_ASSERT_EQUAL(tPortAssigned, tConfigData->mPortAssigned);
//    CPPUNIT_ASSERT(tTripPriority == tArticle->mTripPriority);
//
//    CPPUNIT_ASSERT(!tArticle->isClosed());
//    CPPUNIT_ASSERT(!tArticle->isTripped());
//    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());
//    CPPUNIT_ASSERT(!tArticle->isTwoPortSwitch());
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->getCurrent(), tTolerance);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->getConductance(), tTolerance);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->getPowerDissipation(), tTolerance);
//
//    CPPUNIT_ASSERT(tArticle->mSwitchCardName == "NONE");
//
//    std::cout << "... Pass";
//
//}
//
///// @details test for close command response
//void UtGunnsElectSwitchUtil2::testCloseCmd() {
//    std::cout << "\n UtGunnsElectSwitchUtil2 05: testCloseCmd ............................";
//
//    tArticle->mSwitchCommandedClosed = true;
//    tArticle->updateSwitchState(true);
//
//    CPPUNIT_ASSERT(tArticle->mSwitchIsClosed);
//    CPPUNIT_ASSERT(!tArticle->isTripped());
//
//    std::cout << "... Pass";
//}
//
///// @details test for open command response
//void UtGunnsElectSwitchUtil2::testOpenCmd() {
//    std::cout << "\n UtGunnsElectSwitchUtil2 06: testOpenCmd .............................";
//
//    tArticle->mSwitchCommandedClosed = true;
//    tArticle->updateSwitchState(true);
//    CPPUNIT_ASSERT(tArticle->mSwitchIsClosed);
//
//    tArticle->mSwitchCommandedClosed = false;
//    tArticle->updateSwitchState(true);
//
//    CPPUNIT_ASSERT(!tArticle->mSwitchIsClosed);
//    CPPUNIT_ASSERT(!tArticle->isTripped());
//
//    std::cout << "... Pass";
//}
//
///// @details test for nominal flow through a closed switch
//void UtGunnsElectSwitchUtil2::testNominalCurrentFlow() {
//    std::cout << "\n UtGunnsElectSwitchUtil2 07: testNominalCurrentFlow ..................";
//
//    tArticle->mSwitchCommandedClosed = true;
//    tArticle->updateSwitchState(true);
//    CPPUNIT_ASSERT(tArticle->mSwitchIsClosed);
//
//    //tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
//    tArticle->updateSwitchFlow(3.0, 3.0, 124.5, 1, false);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, tArticle->getCurrent(), tTolerance);
//    CPPUNIT_ASSERT(tArticle->mSwitchIsClosed);
//    CPPUNIT_ASSERT(!tArticle->isTripped());
//    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());
//
//    std::cout << "... Pass";
//}
//
///// @details test for no current through open switch
//void UtGunnsElectSwitchUtil2::testNoCurrentWhenOpen() {
//    std::cout << "\n UtGunnsElectSwitchUtil2 08: testNoCurrentWhenOpen ...................";
//
//    //tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
//    tArticle->updateSwitchFlow(3.0, 3.0, 124.5, 1, false);
//
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->getCurrent(), tTolerance);
//    CPPUNIT_ASSERT(!tArticle->mSwitchIsClosed);
//    CPPUNIT_ASSERT(!tArticle->isTripped());
//    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());
//
//    std::cout << "... Pass";
//}
//
///// @details test over current trip
//void UtGunnsElectSwitchUtil2::testOverCurrentTrip() {
//    std::cout << "\n UtGunnsElectSwitchUtil2 09: testOverCurrentTrip .....................";
//
//    tArticle->mSwitchCommandedClosed = true;
//    tArticle->updateSwitchState(true);
//    CPPUNIT_ASSERT(tArticle->mSwitchIsClosed);
//
//    //tArticle->updateSwitchFlow(3.6, 124.5, 124.5, 1, false, true);
//    tArticle->updateSwitchFlow(3.6, 3.6, 124.5, 1, false);
//    CPPUNIT_ASSERT(!tArticle->mSwitchIsClosed);
//    CPPUNIT_ASSERT(tArticle->isTripped());
//    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());
//
//    std::cout << "... Pass";
//
//}
//
///// @details test undervoltage trip
////void UtGunnsElectSwitchUtil2::testUnderVoltTrip() {
////    std::cout << "\n UtGunnsElectSwitchUtil2 10: testUnderVoltTrip .......................";
////
////    tArticle->mSwitchCommandedClosed = true;
////    tArticle->updateSwitchState(true);
////    CPPUNIT_ASSERT(tArticle->mSwitchIsClosed);
////
////    tArticle->updateSwitchFlow(3.0, 104.0, 104.0, 1, false, true);
////    CPPUNIT_ASSERT(!tArticle->mSwitchIsClosed);
////    CPPUNIT_ASSERT(tArticle->isTripped());
////    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());
////
////    std::cout << "... Pass";
////
////}
//
///// @details test getPowerDissipation
//void UtGunnsElectSwitchUtil2::testGetPowerDissipation() {
//    std::cout << "\n UtGunnsElectSwitchUtil2 11: testGetPowerDissipation .................";
//
//    tArticle->mSwitchCommandedClosed = true;
//    tArticle->updateSwitchState(true);
//    CPPUNIT_ASSERT(tArticle->mSwitchIsClosed);
//
//    //tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
//    tArticle->updateSwitchFlow(3.0, 3.0, 124.5, 1, false);
//    CPPUNIT_ASSERT(tArticle->mSwitchIsClosed);
//    CPPUNIT_ASSERT(!tArticle->isTripped());
//    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());
//    CPPUNIT_ASSERT_DOUBLES_EQUAL((3.0 * 3.0 * tSwitchOnResistance), tArticle->getPowerDissipation(), tTolerance);
//
//    std::cout << "... Pass";
//}
//
///// @details test getCurrent
//void UtGunnsElectSwitchUtil2::testGetCurrent() {
//    std::cout << "\n UtGunnsElectSwitchUtil2 12: testGetCurrent ..........................";
//
//    tArticle->mSwitchCommandedClosed = true;
//    tArticle->updateSwitchState(true);
//    CPPUNIT_ASSERT(tArticle->mSwitchIsClosed);
//
//    //tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
//    tArticle->updateSwitchFlow(3.0,3.0, 124.5, 1, false);
//    CPPUNIT_ASSERT(tArticle->mSwitchIsClosed);
//    CPPUNIT_ASSERT(!tArticle->isTripped());
//    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, tArticle->getCurrent(), tTolerance);
//
//    std::cout << "... Pass";
//}
//
///// @details test getConductance
//void UtGunnsElectSwitchUtil2::testGetConductance() {
//    std::cout << "\n UtGunnsElectSwitchUtil2 13: testGetConductance ......................";
//
//    tArticle->mSwitchCommandedClosed = true;
//    tArticle->updateSwitchState(true);
//    CPPUNIT_ASSERT(tArticle->mSwitchIsClosed);
//
//    //tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
//    tArticle->updateSwitchFlow(3.0, 3.0, 124.5, 1, false);
//    CPPUNIT_ASSERT(tArticle->mSwitchIsClosed);
//    CPPUNIT_ASSERT(!tArticle->isTripped());
//    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());
//    CPPUNIT_ASSERT_DOUBLES_EQUAL((1/tSwitchOnResistance), tArticle->getConductance(), tTolerance);
//
//    std::cout << "... Pass";
//}
//
///// @details test isClosed function
//void UtGunnsElectSwitchUtil2::testIsClosed() {
//    std::cout << "\n UtGunnsElectSwitchUtil2 14: testIsClosed ............................";
//
//    tArticle->mSwitchCommandedClosed = true;
//    tArticle->updateSwitchState(true);
//    CPPUNIT_ASSERT(tArticle->isClosed());
//
//    tArticle->mSwitchCommandedClosed = false;
//    tArticle->updateSwitchState(true);
//    CPPUNIT_ASSERT(!tArticle->isClosed());
//
//    std::cout << "... Pass";
//
//}
//
///// @details test isTripped function
//void UtGunnsElectSwitchUtil2::testIsTripped() {
//    std::cout << "\n UtGunnsElectSwitchUtil2 15: testIsTripped ...........................";
//
//    tArticle->mSwitchCommandedClosed = true;
//    tArticle->updateSwitchState(true);
//    CPPUNIT_ASSERT(tArticle->isClosed());
//
//    //tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
//    tArticle->updateSwitchFlow(3.0, 3.0, 124.5, 1, false);
//    CPPUNIT_ASSERT(tArticle->isClosed());
//    CPPUNIT_ASSERT(!tArticle->isTripped());
//    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());
//
//    //tArticle->updateSwitchFlow(3.6, 124.5, 124.5, 1, false, true);
//    tArticle->updateSwitchFlow(3.6, 3.6, 124.5, 1, false);
//    CPPUNIT_ASSERT(!tArticle->isClosed());
//    CPPUNIT_ASSERT(tArticle->isTripped());
//    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());
//
//    std::cout << "... Pass";
//}
//
/////@ details test isWaitingToTrip
//void UtGunnsElectSwitchUtil2::testIsWaitingToTripPos() {
//    std::cout << "\n UtGunnsElectSwitchUtil2 16: testIsWaitingToTripPos ..................";
//
//    // config data
//    tSwitchOnResistance = 0.0668;   // type V RPCM 3.5 amp switch
//    tOverCurrentProtection = true;  // switch will trip from over current
//    tUnderVoltProtection = true;    // switch will trip from under voltage
//    tMinVoltage = 105.0;            // type V RPCM
//    tIsTwoPortSwitch = true ;       // this instance does power another RPCM
//    tPortAssigned = 1;
//    tTripPriority = 3;              // trip allowed at third minor step
//
//    // input data
//    tSwitchIsClosed = false;        // Switch is open by default
//    tPosTripLimit = 3.5;            // type V RPCM trip setpoint
//    tNegTripLimit = -0.1;
//
//    tempConfigData = new GunnsElectSwitchUtil2ConfigData (tSwitchOnResistance, tOverCurrentProtection,
//            tUnderVoltProtection, tMinVoltage, tIsTwoPortSwitch, tPortAssigned, tTripPriority);
//
//    tempInputData = new GunnsElectSwitchUtil2InputData (tSwitchMalfFlag, tSwitchMalf, tSwitchIsClosed, tPosTripLimit, tNegTripLimit);
//
//    tArticle->initialize(*tempConfigData, *tempInputData);
//
//    tArticle->mSwitchCommandedClosed = true;
//    tArticle->updateSwitchState(true);
//    CPPUNIT_ASSERT(tArticle->isClosed());
//
//    //tArticle->updateSwitchFlow(3.6, 124.5, 124.5, 1, false, true);
//    tArticle->updateSwitchFlow(3.6, 3.6, 124.5, 1, false);
//    CPPUNIT_ASSERT(tArticle->isClosed());
//    CPPUNIT_ASSERT(!tArticle->isTripped());
//    CPPUNIT_ASSERT(tArticle->isWaitingToTrip());
//
//    delete tempConfigData;
//    delete tempInputData;
//
//    std::cout << "... Pass";
//}
//
/////@ details test isWaitingToTrip
//void UtGunnsElectSwitchUtil2::testIsWaitingToTripNeg() {
//    std::cout << "\n UtGunnsElectSwitchUtil2 17: testIsWaitingToTripNeg ..................";
//
//    // config data
//    tSwitchOnResistance = 0.0668;   // type V RPCM 3.5 amp switch
//    tOverCurrentProtection = true;  // switch will trip from over current
//    tUnderVoltProtection = true;    // switch will trip from under voltage
//    tMinVoltage = 105.0;            // type V RPCM
//    tIsTwoPortSwitch = true;        // this instance does power another RPCM
//    tPortAssigned = 1;
//    tTripPriority = 3;              // trip allowed at third minor step
//
//    // input data
//    tSwitchIsClosed = false;        // switch is open by default
//    tPosTripLimit = 3.5;            // type V RPCM trip setpoint
//    tNegTripLimit = -0.1;
//
//    tempConfigData = new GunnsElectSwitchUtil2ConfigData (tSwitchOnResistance, tOverCurrentProtection,
//            tUnderVoltProtection, tMinVoltage, tIsTwoPortSwitch, tPortAssigned, tTripPriority);
//
//    tempInputData = new GunnsElectSwitchUtil2InputData (tSwitchMalfFlag, tSwitchMalf, tSwitchIsClosed, tPosTripLimit, tNegTripLimit);
//
//    tArticle->initialize(*tempConfigData, *tempInputData);
//
//    tArticle->mSwitchCommandedClosed = true;
//    tArticle->updateSwitchState(true);
//    CPPUNIT_ASSERT(tArticle->isClosed());
//
//    //tArticle->updateSwitchFlow(-1.0, 124.5, 124.5, 1, false, true);
//    tArticle->updateSwitchFlow(-1.0,-1.0, 124.5, 1, false);
//    CPPUNIT_ASSERT(tArticle->isClosed());
//    CPPUNIT_ASSERT(!tArticle->isTripped());
//    CPPUNIT_ASSERT(tArticle->isWaitingToTrip());
//
//    delete tempConfigData;
//    delete tempInputData;
//
//    std::cout << "... Pass";
//}
///// @details test isTwoPortSwitch function
//void UtGunnsElectSwitchUtil2::testIsTwoPortSwitch() {
//    std::cout << "\n UtGunnsElectSwitchUtil2 18: testIsTwoPortSwitch .....................";
//
//    // config data
//    tSwitchOnResistance = 0.0668;   // type V RPCM 3.5 amp switch
//    tOverCurrentProtection = true;  // switch will trip from over current
//    tUnderVoltProtection = true;    // switch will trip from under voltage
//    tMinVoltage = 105.0;            // type V RPCM
//    tIsTwoPortSwitch = true;        // this instance does not power another RPCM
//    tPortAssigned = 5;              // output port for this two port switch
//    tTripPriority = 1;              // trip allowed after first minor step
//
//    // input data
//    tSwitchIsClosed = false;        // switch is open by default
//    tPosTripLimit = 3.5;            // type V RPCM trip setpoint
//    tNegTripLimit = -0.1;
//
//    tConfigData->init(tSwitchOnResistance, tOverCurrentProtection, tUnderVoltProtection, tMinVoltage,
//            tIsTwoPortSwitch, tPortAssigned, tTripPriority);
//
//    tempConfigData = new GunnsElectSwitchUtil2ConfigData (tSwitchOnResistance, tOverCurrentProtection,
//             tUnderVoltProtection, tMinVoltage, tIsTwoPortSwitch, tPortAssigned, tTripPriority);
//
//     tempInputData = new GunnsElectSwitchUtil2InputData (tSwitchMalfFlag, tSwitchMalf, tSwitchIsClosed, tPosTripLimit, tNegTripLimit);
//
//    tArticle->initialize(*tempConfigData, *tempInputData);
//
//
//    CPPUNIT_ASSERT(tArticle->isTwoPortSwitch());
//    CPPUNIT_ASSERT_EQUAL(tPortAssigned, tArticle->mPortAssigned);
//
//    delete tempConfigData;
//    delete tempInputData;
//
//    std::cout << "... Pass";
//}
//
///// @details test malf fails closed
//void UtGunnsElectSwitchUtil2::testMalfFailClosed() {
//    std::cout << "\n UtGunnsElectSwitchUtil2 19: testMalfFailClosed ......................";
//
//    //switch is open and fails closed
//    CPPUNIT_ASSERT(!tArticle->isClosed());
//    CPPUNIT_ASSERT(!tArticle->isTripped());
//    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());
//
//    tArticle->setMalfFailClosed(true);
//    CPPUNIT_ASSERT(tArticle->mMalfFailClosed);
//    tArticle->updateSwitchState(true);
//
//    CPPUNIT_ASSERT(tArticle->isClosed());
//    CPPUNIT_ASSERT(!tArticle->isTripped());
//    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());
//
//    //switch is now closed and won't trip open
//
//    //tArticle->updateSwitchFlow(3.6, 124.5, 124.5, 1, false, true);
//    tArticle->updateSwitchFlow(3.6, 3.6, 124.5, 1, false);
//    CPPUNIT_ASSERT(tArticle->isClosed());
//    CPPUNIT_ASSERT(!tArticle->isTripped());
//    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());
//
//    tArticle->setMalfFailClosed();
//    CPPUNIT_ASSERT(!tArticle->mMalfFailClosed);
//
//     std::cout << "... Pass";
//}
//
///// @details test malf fails open
//void UtGunnsElectSwitchUtil2::testMalfFailOpen() {
//    std::cout << "\n UtGunnsElectSwitchUtil2 20: testMalfFailOpen ........................";
//
//    //close switch, malf it open, won't close again
//    tArticle->mSwitchCommandedClosed = true;
//    tArticle->updateSwitchState(true);
//
//    CPPUNIT_ASSERT(tArticle->isClosed());
//    CPPUNIT_ASSERT(!tArticle->isTripped());
//    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());
//
//    tArticle->setMalfFailOpen(true);
//    CPPUNIT_ASSERT(tArticle->mMalfFailOpen);
//    tArticle->updateSwitchState(true);
//
//    CPPUNIT_ASSERT(!tArticle->isClosed());
//    CPPUNIT_ASSERT(!tArticle->isTripped());
//    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());
//
//    tArticle->mSwitchCommandedClosed = true;
//    tArticle->updateSwitchState(true);
//
//    CPPUNIT_ASSERT(!tArticle->isClosed());
//    CPPUNIT_ASSERT(!tArticle->isTripped());
//    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());
//
//    tArticle->setMalfFailOpen();
//    CPPUNIT_ASSERT(!tArticle->mMalfFailOpen);
//
//    std::cout << "... Pass";
//}
//
///// @details test under volt trip flag
////void UtGunnsElectSwitchUtil2::testIsVoltTrip() {
////    std::cout << "\n UtGunnsElectSwitchUtil2 21: testIsVoltTrip ..........................";
////
////    tArticle->mSwitchCommandedClosed = true;
////    tArticle->updateSwitchState(true);
////    CPPUNIT_ASSERT(tArticle->isClosed());
////
////    tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
////    CPPUNIT_ASSERT(tArticle->isClosed());
////    CPPUNIT_ASSERT(!tArticle->isTripped());
////    CPPUNIT_ASSERT(!tArticle->isVoltTrip());
////
////    tArticle->updateSwitchFlow(3.0, 104.0, 104.0, 1, false, true);
////    CPPUNIT_ASSERT(!tArticle->isClosed());
////    CPPUNIT_ASSERT(tArticle->isTripped());
////    CPPUNIT_ASSERT(tArticle->isVoltTrip());
////    CPPUNIT_ASSERT(!tArticle->isPosTrip());
////    CPPUNIT_ASSERT(!tArticle->isNegTrip());
////
////    std::cout << "... Pass";
////}
//
///// @details test positive current trip
//void UtGunnsElectSwitchUtil2::testIsPosTrip() {
//    std::cout << "\n UtGunnsElectSwitchUtil2 22: testIsPosTrip ...........................";
//
//    tArticle->mSwitchCommandedClosed = true;
//    tArticle->updateSwitchState(true);
//    CPPUNIT_ASSERT(tArticle->isClosed());
//
//    //tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
//    tArticle->updateSwitchFlow(3.0,3.0, 124.5, 1, false);
//    CPPUNIT_ASSERT(tArticle->isClosed());
//    CPPUNIT_ASSERT(!tArticle->isTripped());
//    CPPUNIT_ASSERT(!tArticle->isPosTrip());
//
//    //tArticle->updateSwitchFlow(3.6, 124.5, 124.5, 1, false, true);
//    tArticle->updateSwitchFlow(3.6, 3.6, 124.5, 1, false);
//    CPPUNIT_ASSERT(!tArticle->isClosed());
//    CPPUNIT_ASSERT(tArticle->isTripped());
//    CPPUNIT_ASSERT(tArticle->isJustTripped());
//    CPPUNIT_ASSERT(tArticle->isPosTrip());
////    CPPUNIT_ASSERT(!tArticle->isVoltTrip());
//    CPPUNIT_ASSERT(!tArticle->isNegTrip());
//
//    std::cout << "... Pass";
//}
//
///// @details test negative current trip
//void UtGunnsElectSwitchUtil2::testIsNegTrip() {
//    std::cout << "\n UtGunnsElectSwitchUtil2 23: testIsNegTrip ...........................";
//
//    tArticle->mSwitchCommandedClosed = true;
//    tArticle->updateSwitchState(true);
//    CPPUNIT_ASSERT(tArticle->isClosed());
//
//    //tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
//    tArticle->updateSwitchFlow(3.0, 3.0, 124.5, 1, false);
//    CPPUNIT_ASSERT(tArticle->isClosed());
//    CPPUNIT_ASSERT(!tArticle->isTripped());
//    CPPUNIT_ASSERT(!tArticle->isNegTrip());
//
//    //tArticle->updateSwitchFlow(-1.0, 124.5, 124.5, 1, false, true);
//    tArticle->updateSwitchFlow(-1.0, -1.0, 124.5, 1, false);
//    CPPUNIT_ASSERT(!tArticle->isClosed());
//    CPPUNIT_ASSERT(tArticle->isTripped());
//    CPPUNIT_ASSERT(tArticle->isNegTrip());
//    CPPUNIT_ASSERT(!tArticle->isPosTrip());
////    CPPUNIT_ASSERT(!tArticle->isVoltTrip());
//
//    std::cout << "... Pass";
//}
//
///// @details test Malf scale resistance lower than default
//void UtGunnsElectSwitchUtil2::testMalfSwitchResistanceLow() {
//    std::cout << "\n UtGunnsElectSwitchUtil2 24: testMalfSwitchResistanceLow .............";
//
//    tArticle->mSwitchCommandedClosed = true;
//    tArticle->updateSwitchState(true);
//    CPPUNIT_ASSERT(tArticle->isClosed());
//
//    //tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
//    tArticle->updateSwitchFlow(3.0, 3.0, 124.5, 1, false);
//    CPPUNIT_ASSERT(tArticle->isClosed());
//    CPPUNIT_ASSERT(!tArticle->isTripped());
//    CPPUNIT_ASSERT(!tArticle->isNegTrip());
//    CPPUNIT_ASSERT_DOUBLES_EQUAL((3.0 * 3.0 * tSwitchOnResistance), tArticle->getPowerDissipation(), tTolerance);
//
//    tArticle->setMalfFailResistance(true, 50.0);
//    CPPUNIT_ASSERT(true  == tArticle->mMalfFailResistance);
//    CPPUNIT_ASSERT(50.0  == tArticle->mMalfFailResistPercent);
//    tArticle->updateSwitchState(true);
//    //tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
//    tArticle->updateSwitchFlow(3.0, 3.0, 124.5, 1, false);
//    CPPUNIT_ASSERT(tArticle->isClosed());
//    CPPUNIT_ASSERT_DOUBLES_EQUAL((0.5 * (3.0 * 3.0 * tSwitchOnResistance)), tArticle->getPowerDissipation(), tTolerance);
//
//    tArticle->setMalfFailResistance();
//    CPPUNIT_ASSERT(false == tArticle->mMalfFailResistance);
//    CPPUNIT_ASSERT(0.0   == tArticle->mMalfFailResistPercent);
//
//    std::cout << "... Pass";
//}
//
///// @details test Malf scale resistance higher than default
//void UtGunnsElectSwitchUtil2::testMalfSwitchResistanceHigh() {
//    std::cout << "\n UtGunnsElectSwitchUtil2 25: testMalfSwitchResistanceHigh ............";
//
//    tArticle->mSwitchCommandedClosed = true;
//    tArticle->updateSwitchState(true);
//    CPPUNIT_ASSERT(tArticle->isClosed());
//
//    //tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
//    tArticle->updateSwitchFlow(3.0, 3.0, 124.5, 1, false);
//    CPPUNIT_ASSERT(tArticle->isClosed());
//    CPPUNIT_ASSERT(!tArticle->isTripped());
//    CPPUNIT_ASSERT(!tArticle->isNegTrip());
//    CPPUNIT_ASSERT_DOUBLES_EQUAL((3.0 * 3.0 * tSwitchOnResistance), tArticle->getPowerDissipation(), tTolerance);
//
//    tArticle->mMalfFailResistance = true;
//    tArticle->mMalfFailResistPercent = 200.0;
//    tArticle->updateSwitchState(true);
//    //tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
//    tArticle->updateSwitchFlow(3.0, 3.0, 124.5, 1, false);
//    CPPUNIT_ASSERT(tArticle->isClosed());
//    CPPUNIT_ASSERT_DOUBLES_EQUAL((2.0 * (3.0 * 3.0 * tSwitchOnResistance)), tArticle->getPowerDissipation(), tTolerance);
//
//    std::cout << "... Pass";
//}
//
///// @details test trip reset function
//void UtGunnsElectSwitchUtil2::testTripReset() {
//    std::cout << "\n UtGunnsElectSwitchUtil2 26: testTripReset ...........................";
//
//    tArticle->mSwitchCommandedClosed = true;
//    tArticle->updateSwitchState(true);
//    CPPUNIT_ASSERT(tArticle->isClosed());
//
//    //tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
//    tArticle->updateSwitchFlow(3.0, 3.0, 124.5, 1, false);
//    CPPUNIT_ASSERT(tArticle->isClosed());
//    CPPUNIT_ASSERT(!tArticle->isTripped());
//    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());
//
//    //tArticle->updateSwitchFlow(3.6, 124.5, 124.5, 1, false, true);
//    tArticle->updateSwitchFlow(3.6, 3.6, 124.5, 1, false);
//    CPPUNIT_ASSERT(!tArticle->isClosed());
//    CPPUNIT_ASSERT(tArticle->isTripped());
//    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());
//
//    //tArticle->mTripReset = true;  // no longer needed because opening the switch should clear trips
//    tArticle->mSwitchCommandedClosed = false;
//    tArticle->updateSwitchState(true);
//    CPPUNIT_ASSERT(!tArticle->isClosed());
//    CPPUNIT_ASSERT(!tArticle->isTripped());
//    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());
//
//    std::cout << "... Pass";
//}
//
///// @details check initialize procedure sanity checks
//void UtGunnsElectSwitchUtil2::testBadInitData() {
//    std::cout << "\n UtGunnsElectSwitchUtil2 27: testBadInitData .........................";
//
//    // set up with bad default resistance
//    tConfigData->init(-1.0, tOverCurrentProtection, tUnderVoltProtection, tMinVoltage, tIsTwoPortSwitch, tPortAssigned, tTripPriority);
//    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData), TsInitializationException);
//
//    // set up bad two port switch output port
//    tConfigData->init(tSwitchOnResistance, tOverCurrentProtection, tUnderVoltProtection, tMinVoltage, true, 0, tTripPriority);
//    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData), TsInitializationException);
//
//    // set up bad user load switch output port
//    tConfigData->init(tSwitchOnResistance, tOverCurrentProtection, tUnderVoltProtection, tMinVoltage, false, 2, tTripPriority);
//    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData), TsInitializationException);
//
//    // set up bad trip priority for any switch
//    tConfigData->init(tSwitchOnResistance, tOverCurrentProtection, tUnderVoltProtection, tMinVoltage, false, 2, 0);
//    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData), TsInitializationException);
//
//    // set up bad trip priority for any switch
//    tConfigData->init(tSwitchOnResistance, tOverCurrentProtection, tUnderVoltProtection, tMinVoltage, true, 2, 0);
//    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData), TsInitializationException);
//
//   // set up bad trip priority for user load switch
//    tConfigData->init(tSwitchOnResistance, tOverCurrentProtection, tUnderVoltProtection, tMinVoltage, false, 0, 3);
//    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData), TsInitializationException);
//
//    // set up bad positive trip limit
//    tConfigData->init(tSwitchOnResistance, tOverCurrentProtection, tUnderVoltProtection, tMinVoltage, false, 0, 1);
//    tInputData->init(false, 0, true, -3.5, -3.5);
//    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData), TsInitializationException);
//
//    // set up bad negative trip limit
//    tInputData->init(false, 0, true, 3.5, 3.5);
//    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData), TsInitializationException);
//
//    std::cout << "... Pass";
//}
//
///// @details test malfunction that changes positive trip setpoint
//void UtGunnsElectSwitchUtil2::testMalfPosTripLimit() {
//    std::cout << "\n UtGunnsElectSwitchUtil2 28: testMalfPosTripLimit ....................";
//
//    // close the switch, test it's closed
//    tArticle->mSwitchCommandedClosed = true;
//    tArticle->updateSwitchState(true);
//    CPPUNIT_ASSERT(tArticle->isClosed());
//
//    // pass a valid current through the switch, stays closed
//    //tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
//    tArticle->updateSwitchFlow(3.0, 3.0, 124.5, 1, false);
//    CPPUNIT_ASSERT(tArticle->isClosed());
//    CPPUNIT_ASSERT(!tArticle->isTripped());
//    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());
//
//    // lower the trip setpoint and flow same valid current
//    tArticle->setMalfPosTripSetpoint(true, 2.0);
//    CPPUNIT_ASSERT(true  == tArticle->mMalfPosTripSetpointFail);
//    CPPUNIT_ASSERT(2.0   == tArticle->mMalfPosTripSetpointFailValue);
//    tArticle->updateSwitchState(true);
//    //tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
//    tArticle->updateSwitchFlow(3.0, 3.0, 124.5, 1, false);
//
//    // switch is tripped
//    CPPUNIT_ASSERT(!tArticle->isClosed());
//    CPPUNIT_ASSERT(tArticle->isTripped());
//    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());
//
//    tArticle->setMalfPosTripSetpoint();
//    CPPUNIT_ASSERT(false == tArticle->mMalfPosTripSetpointFail);
//    CPPUNIT_ASSERT(0.0   == tArticle->mMalfPosTripSetpointFailValue);
//
//    std::cout << "... Pass";
//}
//
///// @details test malfunction that changes negative trip setpoint
//void UtGunnsElectSwitchUtil2::testMalfNegTripLimit() {
//    std::cout << "\n UtGunnsElectSwitchUtil2 29: testMalfNegTripLimit ....................";
//
//    // clear out old config and init data
//    delete tConfigData;
//    delete tInputData;
//
//    tConfigData = new GunnsElectSwitchUtil2ConfigData (tSwitchOnResistance, tOverCurrentProtection,
//            tUnderVoltProtection, tMinVoltage, tIsTwoPortSwitch, tPortAssigned, tTripPriority);
//    tInputData = new GunnsElectSwitchUtil2InputData (tSwitchMalfFlag, tSwitchMalf, tSwitchIsClosed, tPosTripLimit, -3.5);
//
//    tArticle->initialize(*tConfigData, *tInputData);
//
//
//   // close the switch, test it's closed
//    tArticle->mSwitchCommandedClosed = true;
//    tArticle->updateSwitchState(true);
//    CPPUNIT_ASSERT(tArticle->isClosed());
//
//    // pass a valid current through the switch, stays closed
//    //tArticle->updateSwitchFlow(-3.0, 124.5, 124.5, 1, false, true);
//    tArticle->updateSwitchFlow(-3.0,-3.0, 124.5, 1, false);
//    CPPUNIT_ASSERT(tArticle->isClosed());
//    CPPUNIT_ASSERT(!tArticle->isTripped());
//    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());
//
//    // lower the trip setpoint and flow same valid current
//    tArticle->setMalfNegTripSetpoint(true, -2.0);
//    CPPUNIT_ASSERT(true  == tArticle->mMalfNegTripSetpointFail);
//    CPPUNIT_ASSERT(-2.0  == tArticle->mMalfNegTripSetpointFailValue);
//
//    tArticle->updateSwitchState(true);
//    //tArticle->updateSwitchFlow(-3.0, 124.5, 124.5, 1, false, true);
//    tArticle->updateSwitchFlow(-3.0,-3.0, 124.5, 1, false);
//
//    // switch is tripped
//    CPPUNIT_ASSERT(!tArticle->isClosed());
//    CPPUNIT_ASSERT(tArticle->isTripped());
//    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());
//
//    tArticle->setMalfNegTripSetpoint();
//    CPPUNIT_ASSERT(false == tArticle->mMalfNegTripSetpointFail);
//    CPPUNIT_ASSERT(0.0   == tArticle->mMalfNegTripSetpointFailValue);
//
//    std::cout << "... Pass";
//}
//
//void UtGunnsElectSwitchUtil2::testGetPortAssigned() {
//    std::cout << "\n UtGunnsElectSwitchUtil2 30: testGetPortAssigned .....................";
//
//    // clear out old config and init data
//    delete tConfigData;
//    delete tInputData;
//
//    tConfigData = new GunnsElectSwitchUtil2ConfigData (tSwitchOnResistance, tOverCurrentProtection,
//            tUnderVoltProtection, tMinVoltage, true, 2, tTripPriority);
//    tInputData = new GunnsElectSwitchUtil2InputData (tSwitchMalfFlag, tSwitchMalf, tSwitchIsClosed, tPosTripLimit, tNegTripLimit);
//
//    tArticle->initialize(*tConfigData, *tInputData);
//
//    // close the switch, test it's closed
//     tArticle->mSwitchCommandedClosed = true;
//     tArticle->updateSwitchState(true);
//     CPPUNIT_ASSERT(tArticle->isClosed());
//
//     CPPUNIT_ASSERT_EQUAL(2, tArticle->getPortAssigned());
//
//     std::cout << "... Pass";
//}
//
//void UtGunnsElectSwitchUtil2::testPowerSupplyValid() {
//    std::cout << "\n UtGunnsElectSwitchUtil2 31: testPowerSupplyValid.....................";
//
//    // close the switch, check that it is closed
//    tArticle->mSwitchCommandedClosed = true;
//    tArticle->updateSwitchState(true);
//    CPPUNIT_ASSERT(tArticle->isClosed());
//
//    // update the switch with power supply marked 'invalid' (< minimum)
//    tArticle->updateSwitchState(false);
//
//    // test that the switch is now open
//    CPPUNIT_ASSERT(!tArticle->isClosed());
//
//    std::cout << "... Pass";
//}
//
//void UtGunnsElectSwitchUtil2::testgetOutputVolts() {
//    std::cout << "\n UtGunnsElectSwitchUtil2 32: testGetOutputVolts.......................";
//
//    // close the switch
//    tArticle->mSwitchCommandedClosed = true;
//    tArticle->updateSwitchState(true);
//    CPPUNIT_ASSERT(tArticle->mSwitchIsClosed);
//
//    //tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
//    tArticle->updateSwitchFlow(3.0,3.0, 124.5, 1, false);
//    CPPUNIT_ASSERT(tArticle->mSwitchIsClosed);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(124.5, tArticle->getOutputVolts(), tTolerance);
//    CPPUNIT_ASSERT(!tArticle->isTripped());
//    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());
//
//    // open the switch
//    tArticle->mSwitchCommandedClosed = false;
//    tArticle->updateSwitchState(true);
//    CPPUNIT_ASSERT(!tArticle->mSwitchIsClosed);
//
//    //tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
//    tArticle->updateSwitchFlow(3.0, 3.0, 124.5, 1, false);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->getOutputVolts(), tTolerance);
//    CPPUNIT_ASSERT(!tArticle->isTripped());
//    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());
//
//
//    std::cout << "... Pass";
//    std::cout << "\n -----------------------------------------------------------------------------";
//
//}
