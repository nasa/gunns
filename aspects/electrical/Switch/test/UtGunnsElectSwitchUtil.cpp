/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
 (
    (aspects/electrical/Switch/GunnsElectSwitchUtil.o)
    (software/exceptions/TsInitializationException.o)
    (software/exceptions/TsOutOfBoundsException.o)
 )
 ***************************************************************************************************/
#include <iostream>
#include "UtGunnsElectSwitchUtil.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

/// @details default constructor for the test object
UtGunnsElectSwitchUtil::UtGunnsElectSwitchUtil()
:
tConfigData(),
tempConfigData(),
tInputData(),
tempInputData(),
tArticle(),
tSwitchOnResistance(0.0668),
tOverCurrentProtection(true),
tUnderVoltProtection(true),
tMinVoltage(105.0),
tIsTwoPortSwitch(false),
tPortAssigned(0),
tTripPriority(1),
tSwitchMalfFlag(false),
tSwitchMalf(0),
tSwitchIsClosed(false),
tPosTripLimit(3.5),
tNegTripLimit(0.1),
tTolerance(0.1)
{
}

/// @details default destructor of the test object
UtGunnsElectSwitchUtil::~UtGunnsElectSwitchUtil() {
}

/// @details clean up variables after each unit test.
void UtGunnsElectSwitchUtil::tearDown() {
    delete tArticle;
    delete tInputData;
    delete tConfigData;
}

/// @details create and assign data for the test
void UtGunnsElectSwitchUtil::setUp() {
    // config data
    tSwitchOnResistance = 0.0668;   // type V RPCM 3.5 amp switch
    tOverCurrentProtection = true;  // switch will trip from over current
    tUnderVoltProtection = true;    // switch will trip from under voltage
    tMinVoltage = 105.0;            // type V RPCM
    tIsTwoPortSwitch = false;       // this instance does not power another RPCM
    tPortAssigned = 0;              // default value for a non two port switch
    tTripPriority = 1;              // trip allowed after first minor step

    // input data
    tSwitchMalfFlag = false;            // no malf active by default
    tSwitchMalf = 0;                // no malf active by default
    tSwitchIsClosed = false;        // default switch state is open
    tPosTripLimit = 3.5;            // type V RPCM trip setpoint
    tNegTripLimit = -0.1;

    tConfigData = new GunnsElectSwitchUtilConfigData (tSwitchOnResistance, tOverCurrentProtection,
            tUnderVoltProtection, tMinVoltage, tIsTwoPortSwitch, tPortAssigned, tTripPriority);

    tConfigData->init(tSwitchOnResistance, tOverCurrentProtection, tUnderVoltProtection,
            tMinVoltage, tIsTwoPortSwitch, tPortAssigned, tTripPriority);

    tInputData = new GunnsElectSwitchUtilInputData (tSwitchMalfFlag, tSwitchMalf, tSwitchIsClosed, tPosTripLimit, tNegTripLimit);

    tInputData->init(tSwitchMalfFlag, tSwitchMalf, tSwitchIsClosed, tPosTripLimit, tNegTripLimit);

    tArticle = new FriendlyGunnsElectSwitchUtil();

    try {
        tArticle->initialize(*tConfigData, *tInputData);

    } catch (TsInitializationException &e) {
        std::cout << "UtGunnsElectSwitchUtil Setup method initialization exception -- failed" << std::endl;
    } catch (...) {
        std::cout << "UtGunnsElectSwitchUtil Setup method initialization exception catch all exception -- failed" << std::endl;
    }
}

/// @details test for construction of config details
void UtGunnsElectSwitchUtil::testConfigData() {
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsElectSwitchUtil 01: testConfigData ..........................";

    // Config data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSwitchOnResistance, tConfigData->mDefaultSwitchResistance, tTolerance);
    CPPUNIT_ASSERT(tConfigData->mOverCurrentProtection);
    CPPUNIT_ASSERT(tConfigData->mUnderVoltProtection);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMinVoltage, tConfigData->mMinVoltage, tTolerance);
    CPPUNIT_ASSERT(!tConfigData->mIsTwoPortSwitch);
    CPPUNIT_ASSERT_EQUAL(tPortAssigned, tConfigData->mPortAssigned);
    CPPUNIT_ASSERT(tTripPriority == tConfigData->mTripPriority);

    /// @test   Check copy config construction
    GunnsElectSwitchUtilConfigData copyConfig(*tConfigData);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSwitchOnResistance, copyConfig.mDefaultSwitchResistance, tTolerance);
    CPPUNIT_ASSERT(copyConfig.mOverCurrentProtection);
    CPPUNIT_ASSERT(copyConfig.mUnderVoltProtection);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMinVoltage, copyConfig.mMinVoltage, tTolerance);
    CPPUNIT_ASSERT(!copyConfig.mIsTwoPortSwitch);
    CPPUNIT_ASSERT_EQUAL(tPortAssigned, copyConfig.mPortAssigned);
    CPPUNIT_ASSERT(tTripPriority == copyConfig.mTripPriority);

    std::cout << "... Pass";
}

/// @details test for construction of input data
void UtGunnsElectSwitchUtil::testInputData() {
    std::cout << "\n UtGunnsElectSwitchUtil 02: testInputData ...........................";

    /// @test Test construction of input data
    CPPUNIT_ASSERT(!tInputData->mSwitchIsAutoClosed);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tPosTripLimit, tInputData->mPosTripLimit, tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNegTripLimit, tInputData->mNegTripLimit, tTolerance);

    /// @test test copy constructor for input data
    GunnsElectSwitchUtilInputData copyInput(*tInputData);

    CPPUNIT_ASSERT(!copyInput.mSwitchIsAutoClosed);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tPosTripLimit, copyInput.mPosTripLimit, tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNegTripLimit, copyInput.mNegTripLimit, tTolerance);

    std::cout << "... Pass";
}

/// @details test for default construction
void UtGunnsElectSwitchUtil::testDefaultConstruction() {
    std::cout << "\n UtGunnsElectSwitchUtil 03: testDefaultConstruction .................";

    FriendlyGunnsElectSwitchUtil article;

    article.initialize(*tConfigData, *tInputData);

    std::cout << "... Pass";
}

/// @details test for construction of config data
void UtGunnsElectSwitchUtil::testInitialization() {
    std::cout << "\n UtGunnsElectSwitchUtil 04: testInitialization ......................";

    /// @test   config and input values
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSwitchOnResistance, tArticle->mDefaultSwitchResistance, tTolerance);
    CPPUNIT_ASSERT(tArticle->mOverCurrentProtection);
//    CPPUNIT_ASSERT(tArticle->mUnderVoltProtection);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMinVoltage, tArticle->mMinVoltage, tTolerance);
    CPPUNIT_ASSERT(!tArticle->mIsTwoPortSwitch);
    CPPUNIT_ASSERT_EQUAL(tPortAssigned, tConfigData->mPortAssigned);
    CPPUNIT_ASSERT(tTripPriority == tArticle->mTripPriority);

    CPPUNIT_ASSERT(!tArticle->isClosed());
    CPPUNIT_ASSERT(!tArticle->isTripped());
    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());
    CPPUNIT_ASSERT(!tArticle->isTwoPortSwitch());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->getCurrent(), tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->getConductance(), tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->getPowerDissipation(), tTolerance);

    CPPUNIT_ASSERT(tArticle->mSwitchCardName == "NONE");

    std::cout << "... Pass";

}

/// @details test for close command response
void UtGunnsElectSwitchUtil::testCloseCmd() {
    std::cout << "\n UtGunnsElectSwitchUtil 05: testCloseCmd ............................";

    tArticle->mSwitchCommandedClosed = true;
    tArticle->updateSwitchState(true);

    CPPUNIT_ASSERT(tArticle->mSwitchIsClosed);
    CPPUNIT_ASSERT(!tArticle->isTripped());

    std::cout << "... Pass";
}

/// @details test for open command response
void UtGunnsElectSwitchUtil::testOpenCmd() {
    std::cout << "\n UtGunnsElectSwitchUtil 06: testOpenCmd .............................";

    tArticle->mSwitchCommandedClosed = true;
    tArticle->updateSwitchState(true);
    CPPUNIT_ASSERT(tArticle->mSwitchIsClosed);

    tArticle->mSwitchCommandedClosed = false;
    tArticle->updateSwitchState(true);

    CPPUNIT_ASSERT(!tArticle->mSwitchIsClosed);
    CPPUNIT_ASSERT(!tArticle->isTripped());

    std::cout << "... Pass";
}

/// @details test for nominal flow through a closed switch
void UtGunnsElectSwitchUtil::testNominalCurrentFlow() {
    std::cout << "\n UtGunnsElectSwitchUtil 07: testNominalCurrentFlow ..................";

    tArticle->mSwitchCommandedClosed = true;
    tArticle->updateSwitchState(true);
    CPPUNIT_ASSERT(tArticle->mSwitchIsClosed);

    //tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
    tArticle->updateSwitchFlow(3.0, 3.0, 124.5, 1, false);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, tArticle->getCurrent(), tTolerance);
    CPPUNIT_ASSERT(tArticle->mSwitchIsClosed);
    CPPUNIT_ASSERT(!tArticle->isTripped());
    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());

    std::cout << "... Pass";
}

/// @details test for no current through open switch
void UtGunnsElectSwitchUtil::testNoCurrentWhenOpen() {
    std::cout << "\n UtGunnsElectSwitchUtil 08: testNoCurrentWhenOpen ...................";

    //tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
    tArticle->updateSwitchFlow(3.0, 3.0, 124.5, 1, false);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->getCurrent(), tTolerance);
    CPPUNIT_ASSERT(!tArticle->mSwitchIsClosed);
    CPPUNIT_ASSERT(!tArticle->isTripped());
    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());

    std::cout << "... Pass";
}

/// @details test over current trip
void UtGunnsElectSwitchUtil::testOverCurrentTrip() {
    std::cout << "\n UtGunnsElectSwitchUtil 09: testOverCurrentTrip .....................";

    tArticle->mSwitchCommandedClosed = true;
    tArticle->updateSwitchState(true);
    CPPUNIT_ASSERT(tArticle->mSwitchIsClosed);

    //tArticle->updateSwitchFlow(3.6, 124.5, 124.5, 1, false, true);
    tArticle->updateSwitchFlow(3.6, 3.6, 124.5, 1, false);
    CPPUNIT_ASSERT(!tArticle->mSwitchIsClosed);
    CPPUNIT_ASSERT(tArticle->isTripped());
    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());

    std::cout << "... Pass";

}

/// @details test undervoltage trip
//void UtGunnsElectSwitchUtil::testUnderVoltTrip() {
//    std::cout << "\n UtGunnsElectSwitchUtil 10: testUnderVoltTrip .......................";
//
//    tArticle->mSwitchCommandedClosed = true;
//    tArticle->updateSwitchState(true);
//    CPPUNIT_ASSERT(tArticle->mSwitchIsClosed);
//
//    tArticle->updateSwitchFlow(3.0, 104.0, 104.0, 1, false, true);
//    CPPUNIT_ASSERT(!tArticle->mSwitchIsClosed);
//    CPPUNIT_ASSERT(tArticle->isTripped());
//    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());
//
//    std::cout << "... Pass";
//
//}

/// @details test getPowerDissipation
void UtGunnsElectSwitchUtil::testGetPowerDissipation() {
    std::cout << "\n UtGunnsElectSwitchUtil 11: testGetPowerDissipation .................";

    tArticle->mSwitchCommandedClosed = true;
    tArticle->updateSwitchState(true);
    CPPUNIT_ASSERT(tArticle->mSwitchIsClosed);

    //tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
    tArticle->updateSwitchFlow(3.0, 3.0, 124.5, 1, false);
    CPPUNIT_ASSERT(tArticle->mSwitchIsClosed);
    CPPUNIT_ASSERT(!tArticle->isTripped());
    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());
    CPPUNIT_ASSERT_DOUBLES_EQUAL((3.0 * 3.0 * tSwitchOnResistance), tArticle->getPowerDissipation(), tTolerance);

    std::cout << "... Pass";
}

/// @details test getCurrent
void UtGunnsElectSwitchUtil::testGetCurrent() {
    std::cout << "\n UtGunnsElectSwitchUtil 12: testGetCurrent ..........................";

    tArticle->mSwitchCommandedClosed = true;
    tArticle->updateSwitchState(true);
    CPPUNIT_ASSERT(tArticle->mSwitchIsClosed);

    //tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
    tArticle->updateSwitchFlow(3.0,3.0, 124.5, 1, false);
    CPPUNIT_ASSERT(tArticle->mSwitchIsClosed);
    CPPUNIT_ASSERT(!tArticle->isTripped());
    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, tArticle->getCurrent(), tTolerance);

    std::cout << "... Pass";
}

/// @details test getConductance
void UtGunnsElectSwitchUtil::testGetConductance() {
    std::cout << "\n UtGunnsElectSwitchUtil 13: testGetConductance ......................";

    tArticle->mSwitchCommandedClosed = true;
    tArticle->updateSwitchState(true);
    CPPUNIT_ASSERT(tArticle->mSwitchIsClosed);

    //tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
    tArticle->updateSwitchFlow(3.0, 3.0, 124.5, 1, false);
    CPPUNIT_ASSERT(tArticle->mSwitchIsClosed);
    CPPUNIT_ASSERT(!tArticle->isTripped());
    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());
    CPPUNIT_ASSERT_DOUBLES_EQUAL((1/tSwitchOnResistance), tArticle->getConductance(), tTolerance);

    std::cout << "... Pass";
}

/// @details test isClosed function
void UtGunnsElectSwitchUtil::testIsClosed() {
    std::cout << "\n UtGunnsElectSwitchUtil 14: testIsClosed ............................";

    tArticle->mSwitchCommandedClosed = true;
    tArticle->updateSwitchState(true);
    CPPUNIT_ASSERT(tArticle->isClosed());

    tArticle->mSwitchCommandedClosed = false;
    tArticle->updateSwitchState(true);
    CPPUNIT_ASSERT(!tArticle->isClosed());

    std::cout << "... Pass";

}

/// @details test isTripped function
void UtGunnsElectSwitchUtil::testIsTripped() {
    std::cout << "\n UtGunnsElectSwitchUtil 15: testIsTripped ...........................";

    tArticle->mSwitchCommandedClosed = true;
    tArticle->updateSwitchState(true);
    CPPUNIT_ASSERT(tArticle->isClosed());

    //tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
    tArticle->updateSwitchFlow(3.0, 3.0, 124.5, 1, false);
    CPPUNIT_ASSERT(tArticle->isClosed());
    CPPUNIT_ASSERT(!tArticle->isTripped());
    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());

    //tArticle->updateSwitchFlow(3.6, 124.5, 124.5, 1, false, true);
    tArticle->updateSwitchFlow(3.6, 3.6, 124.5, 1, false);
    CPPUNIT_ASSERT(!tArticle->isClosed());
    CPPUNIT_ASSERT(tArticle->isTripped());
    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());

    std::cout << "... Pass";
}

///@ details test isWaitingToTrip
void UtGunnsElectSwitchUtil::testIsWaitingToTripPos() {
    std::cout << "\n UtGunnsElectSwitchUtil 16: testIsWaitingToTripPos ..................";

    // config data
    tSwitchOnResistance = 0.0668;   // type V RPCM 3.5 amp switch
    tOverCurrentProtection = true;  // switch will trip from over current
    tUnderVoltProtection = true;    // switch will trip from under voltage
    tMinVoltage = 105.0;            // type V RPCM
    tIsTwoPortSwitch = true ;       // this instance does power another RPCM
    tPortAssigned = 1;
    tTripPriority = 3;              // trip allowed at third minor step

    // input data
    tSwitchIsClosed = false;        // Switch is open by default
    tPosTripLimit = 3.5;            // type V RPCM trip setpoint
    tNegTripLimit = -0.1;

    tempConfigData = new GunnsElectSwitchUtilConfigData (tSwitchOnResistance, tOverCurrentProtection,
            tUnderVoltProtection, tMinVoltage, tIsTwoPortSwitch, tPortAssigned, tTripPriority);

    tempInputData = new GunnsElectSwitchUtilInputData (tSwitchMalfFlag, tSwitchMalf, tSwitchIsClosed, tPosTripLimit, tNegTripLimit);

    tArticle->initialize(*tempConfigData, *tempInputData);

    tArticle->mSwitchCommandedClosed = true;
    tArticle->updateSwitchState(true);
    CPPUNIT_ASSERT(tArticle->isClosed());

    //tArticle->updateSwitchFlow(3.6, 124.5, 124.5, 1, false, true);
    tArticle->updateSwitchFlow(3.6, 3.6, 124.5, 1, false);
    CPPUNIT_ASSERT(tArticle->isClosed());
    CPPUNIT_ASSERT(!tArticle->isTripped());
    CPPUNIT_ASSERT(tArticle->isWaitingToTrip());

    delete tempConfigData;
    delete tempInputData;

    std::cout << "... Pass";
}

///@ details test isWaitingToTrip
void UtGunnsElectSwitchUtil::testIsWaitingToTripNeg() {
    std::cout << "\n UtGunnsElectSwitchUtil 17: testIsWaitingToTripNeg ..................";

    // config data
    tSwitchOnResistance = 0.0668;   // type V RPCM 3.5 amp switch
    tOverCurrentProtection = true;  // switch will trip from over current
    tUnderVoltProtection = true;    // switch will trip from under voltage
    tMinVoltage = 105.0;            // type V RPCM
    tIsTwoPortSwitch = true;        // this instance does power another RPCM
    tPortAssigned = 1;
    tTripPriority = 3;              // trip allowed at third minor step

    // input data
    tSwitchIsClosed = false;        // switch is open by default
    tPosTripLimit = 3.5;            // type V RPCM trip setpoint
    tNegTripLimit = -0.1;

    tempConfigData = new GunnsElectSwitchUtilConfigData (tSwitchOnResistance, tOverCurrentProtection,
            tUnderVoltProtection, tMinVoltage, tIsTwoPortSwitch, tPortAssigned, tTripPriority);

    tempInputData = new GunnsElectSwitchUtilInputData (tSwitchMalfFlag, tSwitchMalf, tSwitchIsClosed, tPosTripLimit, tNegTripLimit);

    tArticle->initialize(*tempConfigData, *tempInputData);

    tArticle->mSwitchCommandedClosed = true;
    tArticle->updateSwitchState(true);
    CPPUNIT_ASSERT(tArticle->isClosed());

    //tArticle->updateSwitchFlow(-1.0, 124.5, 124.5, 1, false, true);
    tArticle->updateSwitchFlow(-1.0,-1.0, 124.5, 1, false);
    CPPUNIT_ASSERT(tArticle->isClosed());
    CPPUNIT_ASSERT(!tArticle->isTripped());
    CPPUNIT_ASSERT(tArticle->isWaitingToTrip());

    delete tempConfigData;
    delete tempInputData;

    std::cout << "... Pass";
}
/// @details test isTwoPortSwitch function
void UtGunnsElectSwitchUtil::testIsTwoPortSwitch() {
    std::cout << "\n UtGunnsElectSwitchUtil 18: testIsTwoPortSwitch .....................";

    // config data
    tSwitchOnResistance = 0.0668;   // type V RPCM 3.5 amp switch
    tOverCurrentProtection = true;  // switch will trip from over current
    tUnderVoltProtection = true;    // switch will trip from under voltage
    tMinVoltage = 105.0;            // type V RPCM
    tIsTwoPortSwitch = true;        // this instance does not power another RPCM
    tPortAssigned = 5;              // output port for this two port switch
    tTripPriority = 1;              // trip allowed after first minor step

    // input data
    tSwitchIsClosed = false;        // switch is open by default
    tPosTripLimit = 3.5;            // type V RPCM trip setpoint
    tNegTripLimit = -0.1;

    tConfigData->init(tSwitchOnResistance, tOverCurrentProtection, tUnderVoltProtection, tMinVoltage,
            tIsTwoPortSwitch, tPortAssigned, tTripPriority);

    tempConfigData = new GunnsElectSwitchUtilConfigData (tSwitchOnResistance, tOverCurrentProtection,
             tUnderVoltProtection, tMinVoltage, tIsTwoPortSwitch, tPortAssigned, tTripPriority);

     tempInputData = new GunnsElectSwitchUtilInputData (tSwitchMalfFlag, tSwitchMalf, tSwitchIsClosed, tPosTripLimit, tNegTripLimit);

    tArticle->initialize(*tempConfigData, *tempInputData);


    CPPUNIT_ASSERT(tArticle->isTwoPortSwitch());
    CPPUNIT_ASSERT_EQUAL(tPortAssigned, tArticle->mPortAssigned);

    delete tempConfigData;
    delete tempInputData;

    std::cout << "... Pass";
}

/// @details test malf fails closed
void UtGunnsElectSwitchUtil::testMalfFailClosed() {
    std::cout << "\n UtGunnsElectSwitchUtil 19: testMalfFailClosed ......................";

    //switch is open and fails closed
    CPPUNIT_ASSERT(!tArticle->isClosed());
    CPPUNIT_ASSERT(!tArticle->isTripped());
    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());

    tArticle->setMalfFailClosed(true);
    CPPUNIT_ASSERT(tArticle->mMalfFailClosed);
    tArticle->updateSwitchState(true);

    CPPUNIT_ASSERT(tArticle->isClosed());
    CPPUNIT_ASSERT(!tArticle->isTripped());
    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());

    //switch is now closed and won't trip open

    //tArticle->updateSwitchFlow(3.6, 124.5, 124.5, 1, false, true);
    tArticle->updateSwitchFlow(3.6, 3.6, 124.5, 1, false);
    CPPUNIT_ASSERT(tArticle->isClosed());
    CPPUNIT_ASSERT(!tArticle->isTripped());
    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());

    tArticle->setMalfFailClosed();
    CPPUNIT_ASSERT(!tArticle->mMalfFailClosed);

     std::cout << "... Pass";
}

/// @details test malf fails open
void UtGunnsElectSwitchUtil::testMalfFailOpen() {
    std::cout << "\n UtGunnsElectSwitchUtil 20: testMalfFailOpen ........................";

    //close switch, malf it open, won't close again
    tArticle->mSwitchCommandedClosed = true;
    tArticle->updateSwitchState(true);

    CPPUNIT_ASSERT(tArticle->isClosed());
    CPPUNIT_ASSERT(!tArticle->isTripped());
    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());

    tArticle->setMalfFailOpen(true);
    CPPUNIT_ASSERT(tArticle->mMalfFailOpen);
    tArticle->updateSwitchState(true);

    CPPUNIT_ASSERT(!tArticle->isClosed());
    CPPUNIT_ASSERT(!tArticle->isTripped());
    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());

    tArticle->mSwitchCommandedClosed = true;
    tArticle->updateSwitchState(true);

    CPPUNIT_ASSERT(!tArticle->isClosed());
    CPPUNIT_ASSERT(!tArticle->isTripped());
    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());

    tArticle->setMalfFailOpen();
    CPPUNIT_ASSERT(!tArticle->mMalfFailOpen);

    std::cout << "... Pass";
}

/// @details test under volt trip flag
//void UtGunnsElectSwitchUtil::testIsVoltTrip() {
//    std::cout << "\n UtGunnsElectSwitchUtil 21: testIsVoltTrip ..........................";
//
//    tArticle->mSwitchCommandedClosed = true;
//    tArticle->updateSwitchState(true);
//    CPPUNIT_ASSERT(tArticle->isClosed());
//
//    tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
//    CPPUNIT_ASSERT(tArticle->isClosed());
//    CPPUNIT_ASSERT(!tArticle->isTripped());
//    CPPUNIT_ASSERT(!tArticle->isVoltTrip());
//
//    tArticle->updateSwitchFlow(3.0, 104.0, 104.0, 1, false, true);
//    CPPUNIT_ASSERT(!tArticle->isClosed());
//    CPPUNIT_ASSERT(tArticle->isTripped());
//    CPPUNIT_ASSERT(tArticle->isVoltTrip());
//    CPPUNIT_ASSERT(!tArticle->isPosTrip());
//    CPPUNIT_ASSERT(!tArticle->isNegTrip());
//
//    std::cout << "... Pass";
//}

/// @details test positive current trip
void UtGunnsElectSwitchUtil::testIsPosTrip() {
    std::cout << "\n UtGunnsElectSwitchUtil 22: testIsPosTrip ...........................";

    tArticle->mSwitchCommandedClosed = true;
    tArticle->updateSwitchState(true);
    CPPUNIT_ASSERT(tArticle->isClosed());

    //tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
    tArticle->updateSwitchFlow(3.0,3.0, 124.5, 1, false);
    CPPUNIT_ASSERT(tArticle->isClosed());
    CPPUNIT_ASSERT(!tArticle->isTripped());
    CPPUNIT_ASSERT(!tArticle->isPosTrip());

    //tArticle->updateSwitchFlow(3.6, 124.5, 124.5, 1, false, true);
    tArticle->updateSwitchFlow(3.6, 3.6, 124.5, 1, false);
    CPPUNIT_ASSERT(!tArticle->isClosed());
    CPPUNIT_ASSERT(tArticle->isTripped());
    CPPUNIT_ASSERT(tArticle->isJustTripped());
    CPPUNIT_ASSERT(tArticle->isPosTrip());
//    CPPUNIT_ASSERT(!tArticle->isVoltTrip());
    CPPUNIT_ASSERT(!tArticle->isNegTrip());

    std::cout << "... Pass";
}

/// @details test negative current trip
void UtGunnsElectSwitchUtil::testIsNegTrip() {
    std::cout << "\n UtGunnsElectSwitchUtil 23: testIsNegTrip ...........................";

    tArticle->mSwitchCommandedClosed = true;
    tArticle->updateSwitchState(true);
    CPPUNIT_ASSERT(tArticle->isClosed());

    //tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
    tArticle->updateSwitchFlow(3.0, 3.0, 124.5, 1, false);
    CPPUNIT_ASSERT(tArticle->isClosed());
    CPPUNIT_ASSERT(!tArticle->isTripped());
    CPPUNIT_ASSERT(!tArticle->isNegTrip());

    //tArticle->updateSwitchFlow(-1.0, 124.5, 124.5, 1, false, true);
    tArticle->updateSwitchFlow(-1.0, -1.0, 124.5, 1, false);
    CPPUNIT_ASSERT(!tArticle->isClosed());
    CPPUNIT_ASSERT(tArticle->isTripped());
    CPPUNIT_ASSERT(tArticle->isNegTrip());
    CPPUNIT_ASSERT(!tArticle->isPosTrip());
//    CPPUNIT_ASSERT(!tArticle->isVoltTrip());

    std::cout << "... Pass";
}

/// @details test Malf scale resistance lower than default
void UtGunnsElectSwitchUtil::testMalfSwitchResistanceLow() {
    std::cout << "\n UtGunnsElectSwitchUtil 24: testMalfSwitchResistanceLow .............";

    tArticle->mSwitchCommandedClosed = true;
    tArticle->updateSwitchState(true);
    CPPUNIT_ASSERT(tArticle->isClosed());

    //tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
    tArticle->updateSwitchFlow(3.0, 3.0, 124.5, 1, false);
    CPPUNIT_ASSERT(tArticle->isClosed());
    CPPUNIT_ASSERT(!tArticle->isTripped());
    CPPUNIT_ASSERT(!tArticle->isNegTrip());
    CPPUNIT_ASSERT_DOUBLES_EQUAL((3.0 * 3.0 * tSwitchOnResistance), tArticle->getPowerDissipation(), tTolerance);

    tArticle->setMalfFailResistance(true, 50.0);
    CPPUNIT_ASSERT(true  == tArticle->mMalfFailResistance);
    CPPUNIT_ASSERT(50.0  == tArticle->mMalfFailResistPercent);
    tArticle->updateSwitchState(true);
    //tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
    tArticle->updateSwitchFlow(3.0, 3.0, 124.5, 1, false);
    CPPUNIT_ASSERT(tArticle->isClosed());
    CPPUNIT_ASSERT_DOUBLES_EQUAL((0.5 * (3.0 * 3.0 * tSwitchOnResistance)), tArticle->getPowerDissipation(), tTolerance);

    tArticle->setMalfFailResistance();
    CPPUNIT_ASSERT(false == tArticle->mMalfFailResistance);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfFailResistPercent);

    std::cout << "... Pass";
}

/// @details test Malf scale resistance higher than default
void UtGunnsElectSwitchUtil::testMalfSwitchResistanceHigh() {
    std::cout << "\n UtGunnsElectSwitchUtil 25: testMalfSwitchResistanceHigh ............";

    tArticle->mSwitchCommandedClosed = true;
    tArticle->updateSwitchState(true);
    CPPUNIT_ASSERT(tArticle->isClosed());

    //tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
    tArticle->updateSwitchFlow(3.0, 3.0, 124.5, 1, false);
    CPPUNIT_ASSERT(tArticle->isClosed());
    CPPUNIT_ASSERT(!tArticle->isTripped());
    CPPUNIT_ASSERT(!tArticle->isNegTrip());
    CPPUNIT_ASSERT_DOUBLES_EQUAL((3.0 * 3.0 * tSwitchOnResistance), tArticle->getPowerDissipation(), tTolerance);

    tArticle->mMalfFailResistance = true;
    tArticle->mMalfFailResistPercent = 200.0;
    tArticle->updateSwitchState(true);
    //tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
    tArticle->updateSwitchFlow(3.0, 3.0, 124.5, 1, false);
    CPPUNIT_ASSERT(tArticle->isClosed());
    CPPUNIT_ASSERT_DOUBLES_EQUAL((2.0 * (3.0 * 3.0 * tSwitchOnResistance)), tArticle->getPowerDissipation(), tTolerance);

    std::cout << "... Pass";
}

/// @details test trip reset function
void UtGunnsElectSwitchUtil::testTripReset() {
    std::cout << "\n UtGunnsElectSwitchUtil 26: testTripReset ...........................";

    tArticle->mSwitchCommandedClosed = true;
    tArticle->updateSwitchState(true);
    CPPUNIT_ASSERT(tArticle->isClosed());

    //tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
    tArticle->updateSwitchFlow(3.0, 3.0, 124.5, 1, false);
    CPPUNIT_ASSERT(tArticle->isClosed());
    CPPUNIT_ASSERT(!tArticle->isTripped());
    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());

    //tArticle->updateSwitchFlow(3.6, 124.5, 124.5, 1, false, true);
    tArticle->updateSwitchFlow(3.6, 3.6, 124.5, 1, false);
    CPPUNIT_ASSERT(!tArticle->isClosed());
    CPPUNIT_ASSERT(tArticle->isTripped());
    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());

    //tArticle->mTripReset = true;  // no longer needed because opening the switch should clear trips
    tArticle->mSwitchCommandedClosed = false;
    tArticle->updateSwitchState(true);
    CPPUNIT_ASSERT(!tArticle->isClosed());
    CPPUNIT_ASSERT(!tArticle->isTripped());
    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());

    std::cout << "... Pass";
}

/// @details check initialize procedure sanity checks
void UtGunnsElectSwitchUtil::testBadInitData() {
    std::cout << "\n UtGunnsElectSwitchUtil 27: testBadInitData .........................";

    // set up with bad default resistance
    tConfigData->init(-1.0, tOverCurrentProtection, tUnderVoltProtection, tMinVoltage, tIsTwoPortSwitch, tPortAssigned, tTripPriority);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData), TsInitializationException);

    // set up bad two port switch output port
    tConfigData->init(tSwitchOnResistance, tOverCurrentProtection, tUnderVoltProtection, tMinVoltage, true, 0, tTripPriority);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData), TsInitializationException);

    // set up bad user load switch output port
    tConfigData->init(tSwitchOnResistance, tOverCurrentProtection, tUnderVoltProtection, tMinVoltage, false, 2, tTripPriority);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData), TsInitializationException);

    // set up bad trip priority for any switch
    tConfigData->init(tSwitchOnResistance, tOverCurrentProtection, tUnderVoltProtection, tMinVoltage, false, 2, 0);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData), TsInitializationException);

    // set up bad trip priority for any switch
    tConfigData->init(tSwitchOnResistance, tOverCurrentProtection, tUnderVoltProtection, tMinVoltage, true, 2, 0);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData), TsInitializationException);

   // set up bad trip priority for user load switch
    tConfigData->init(tSwitchOnResistance, tOverCurrentProtection, tUnderVoltProtection, tMinVoltage, false, 0, 3);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData), TsInitializationException);

    // set up bad positive trip limit
    tConfigData->init(tSwitchOnResistance, tOverCurrentProtection, tUnderVoltProtection, tMinVoltage, false, 0, 1);
    tInputData->init(false, 0, true, -3.5, -3.5);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData), TsInitializationException);

    // set up bad negative trip limit
    tInputData->init(false, 0, true, 3.5, 3.5);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData), TsInitializationException);

    std::cout << "... Pass";
}

/// @details test malfunction that changes positive trip setpoint
void UtGunnsElectSwitchUtil::testMalfPosTripLimit() {
    std::cout << "\n UtGunnsElectSwitchUtil 28: testMalfPosTripLimit ....................";

    // close the switch, test it's closed
    tArticle->mSwitchCommandedClosed = true;
    tArticle->updateSwitchState(true);
    CPPUNIT_ASSERT(tArticle->isClosed());

    // pass a valid current through the switch, stays closed
    //tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
    tArticle->updateSwitchFlow(3.0, 3.0, 124.5, 1, false);
    CPPUNIT_ASSERT(tArticle->isClosed());
    CPPUNIT_ASSERT(!tArticle->isTripped());
    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());

    // lower the trip setpoint and flow same valid current
    tArticle->setMalfPosTripSetpoint(true, 2.0);
    CPPUNIT_ASSERT(true  == tArticle->mMalfPosTripSetpointFail);
    CPPUNIT_ASSERT(2.0   == tArticle->mMalfPosTripSetpointFailValue);
    tArticle->updateSwitchState(true);
    //tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
    tArticle->updateSwitchFlow(3.0, 3.0, 124.5, 1, false);

    // switch is tripped
    CPPUNIT_ASSERT(!tArticle->isClosed());
    CPPUNIT_ASSERT(tArticle->isTripped());
    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());

    tArticle->setMalfPosTripSetpoint();
    CPPUNIT_ASSERT(false == tArticle->mMalfPosTripSetpointFail);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfPosTripSetpointFailValue);

    std::cout << "... Pass";
}

/// @details test malfunction that changes negative trip setpoint
void UtGunnsElectSwitchUtil::testMalfNegTripLimit() {
    std::cout << "\n UtGunnsElectSwitchUtil 29: testMalfNegTripLimit ....................";

    // clear out old config and init data
    delete tConfigData;
    delete tInputData;

    tConfigData = new GunnsElectSwitchUtilConfigData (tSwitchOnResistance, tOverCurrentProtection,
            tUnderVoltProtection, tMinVoltage, tIsTwoPortSwitch, tPortAssigned, tTripPriority);
    tInputData = new GunnsElectSwitchUtilInputData (tSwitchMalfFlag, tSwitchMalf, tSwitchIsClosed, tPosTripLimit, -3.5);

    tArticle->initialize(*tConfigData, *tInputData);


   // close the switch, test it's closed
    tArticle->mSwitchCommandedClosed = true;
    tArticle->updateSwitchState(true);
    CPPUNIT_ASSERT(tArticle->isClosed());

    // pass a valid current through the switch, stays closed
    //tArticle->updateSwitchFlow(-3.0, 124.5, 124.5, 1, false, true);
    tArticle->updateSwitchFlow(-3.0,-3.0, 124.5, 1, false);
    CPPUNIT_ASSERT(tArticle->isClosed());
    CPPUNIT_ASSERT(!tArticle->isTripped());
    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());

    // lower the trip setpoint and flow same valid current
    tArticle->setMalfNegTripSetpoint(true, -2.0);
    CPPUNIT_ASSERT(true  == tArticle->mMalfNegTripSetpointFail);
    CPPUNIT_ASSERT(-2.0  == tArticle->mMalfNegTripSetpointFailValue);

    tArticle->updateSwitchState(true);
    //tArticle->updateSwitchFlow(-3.0, 124.5, 124.5, 1, false, true);
    tArticle->updateSwitchFlow(-3.0,-3.0, 124.5, 1, false);

    // switch is tripped
    CPPUNIT_ASSERT(!tArticle->isClosed());
    CPPUNIT_ASSERT(tArticle->isTripped());
    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());

    tArticle->setMalfNegTripSetpoint();
    CPPUNIT_ASSERT(false == tArticle->mMalfNegTripSetpointFail);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfNegTripSetpointFailValue);

    std::cout << "... Pass";
}

void UtGunnsElectSwitchUtil::testGetPortAssigned() {
    std::cout << "\n UtGunnsElectSwitchUtil 30: testGetPortAssigned .....................";

    // clear out old config and init data
    delete tConfigData;
    delete tInputData;

    tConfigData = new GunnsElectSwitchUtilConfigData (tSwitchOnResistance, tOverCurrentProtection,
            tUnderVoltProtection, tMinVoltage, true, 2, tTripPriority);
    tInputData = new GunnsElectSwitchUtilInputData (tSwitchMalfFlag, tSwitchMalf, tSwitchIsClosed, tPosTripLimit, tNegTripLimit);

    tArticle->initialize(*tConfigData, *tInputData);

    // close the switch, test it's closed
     tArticle->mSwitchCommandedClosed = true;
     tArticle->updateSwitchState(true);
     CPPUNIT_ASSERT(tArticle->isClosed());

     CPPUNIT_ASSERT_EQUAL(2, tArticle->getPortAssigned());

     std::cout << "... Pass";
}

void UtGunnsElectSwitchUtil::testPowerSupplyValid() {
    std::cout << "\n UtGunnsElectSwitchUtil 31: testPowerSupplyValid.....................";

    // close the switch, check that it is closed
    tArticle->mSwitchCommandedClosed = true;
    tArticle->updateSwitchState(true);
    CPPUNIT_ASSERT(tArticle->isClosed());

    // update the switch with power supply marked 'invalid' (< minimum)
    tArticle->updateSwitchState(false);

    // test that the switch is now open
    CPPUNIT_ASSERT(!tArticle->isClosed());

    std::cout << "... Pass";
}

void UtGunnsElectSwitchUtil::testgetOutputVolts() {
    std::cout << "\n UtGunnsElectSwitchUtil 32: testGetOutputVolts.......................";

    // close the switch
    tArticle->mSwitchCommandedClosed = true;
    tArticle->updateSwitchState(true);
    CPPUNIT_ASSERT(tArticle->mSwitchIsClosed);

    //tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
    tArticle->updateSwitchFlow(3.0,3.0, 124.5, 1, false);
    CPPUNIT_ASSERT(tArticle->mSwitchIsClosed);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(124.5, tArticle->getOutputVolts(), tTolerance);
    CPPUNIT_ASSERT(!tArticle->isTripped());
    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());

    // open the switch
    tArticle->mSwitchCommandedClosed = false;
    tArticle->updateSwitchState(true);
    CPPUNIT_ASSERT(!tArticle->mSwitchIsClosed);

    //tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
    tArticle->updateSwitchFlow(3.0, 3.0, 124.5, 1, false);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->getOutputVolts(), tTolerance);
    CPPUNIT_ASSERT(!tArticle->isTripped());
    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());


    std::cout << "... Pass";
    std::cout << "\n -----------------------------------------------------------------------------";

}
