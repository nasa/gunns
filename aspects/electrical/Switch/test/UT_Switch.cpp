/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
 (
    (aspects/electrical/Switch/Switch.o)
    (software/exceptions/TsInitializationException.o)
    (software/exceptions/TsOutOfBoundsException.o)
 )
 ***************************************************************************************************/
#include <iostream>
#include "UT_Switch.hh"
#include "aspects/electrical/Switch/Switch.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

/// @details default constructor for the test object
UT_Switch::UT_Switch()
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
UT_Switch::~UT_Switch() {
}

/// @details clean up variables after each unit test.
void UT_Switch::tearDown() {
    delete tArticle;
    delete tInputData;
    delete tConfigData;
}

/// @details create and assign data for the test
void UT_Switch::setUp() {
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

    tConfigData = new SwitchConfigData (tSwitchOnResistance, tOverCurrentProtection,
            tUnderVoltProtection, tMinVoltage, tIsTwoPortSwitch, tPortAssigned, tTripPriority);

    tConfigData->init(tSwitchOnResistance, tOverCurrentProtection, tUnderVoltProtection,
            tMinVoltage, tIsTwoPortSwitch, tPortAssigned, tTripPriority);

    tInputData = new SwitchInputData (tSwitchMalfFlag, tSwitchMalf, tSwitchIsClosed, tPosTripLimit, tNegTripLimit);

    tInputData->init(tSwitchMalfFlag, tSwitchMalf, tSwitchIsClosed, tPosTripLimit, tNegTripLimit);

    tArticle = new FriendlySwitch();

    try {
        tArticle->initialize(*tConfigData, *tInputData);

    } catch (TsInitializationException &e) {
        std::cout << "UT_Switch Setup method initialization exception -- failed" << std::endl;
    } catch (...) {
        std::cout << "UT_Switch Setup method initialization exception catch all exception -- failed" << std::endl;
    }
}

/// @details test for construction of config details
void UT_Switch::testConfigData() {
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UT_Switch  01: testConfigData ......................................";

    // Config data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSwitchOnResistance, tConfigData->mDefaultSwitchResistance, tTolerance);
    CPPUNIT_ASSERT(tConfigData->mOverCurrentProtection);
    CPPUNIT_ASSERT(tConfigData->mUnderVoltProtection);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMinVoltage, tConfigData->mMinVoltage, tTolerance);
    CPPUNIT_ASSERT(!tConfigData->mIsTwoPortSwitch);
    CPPUNIT_ASSERT_EQUAL(tPortAssigned, tConfigData->mPortAssigned);
    CPPUNIT_ASSERT(tTripPriority == tConfigData->mTripPriority);

    /// @test   Check copy config construction
    SwitchConfigData copyConfig(*tConfigData);

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
void UT_Switch::testInputData() {
    std::cout << "\n UT_Switch  02: testInputData .......................................";

    /// @test Test construction of input data
    CPPUNIT_ASSERT(!tInputData->mSwitchIsAutoClosed);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tPosTripLimit, tInputData->mPosTripLimit, tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNegTripLimit, tInputData->mNegTripLimit, tTolerance);

    /// @test test copy constructor for input data
    SwitchInputData copyInput(*tInputData);

    CPPUNIT_ASSERT(!copyInput.mSwitchIsAutoClosed);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tPosTripLimit, copyInput.mPosTripLimit, tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNegTripLimit, copyInput.mNegTripLimit, tTolerance);

    std::cout << "... Pass";
}

/// @details test for default construction
void UT_Switch::testDefaultConstruction() {
    std::cout << "\n UT_Switch  03: testDefaultConstruction .............................";

    FriendlySwitch article;

    article.initialize(*tConfigData, *tInputData);

    std::cout << "... Pass";
}

/// @details test for construction of config data
void UT_Switch::testInitialization() {
    std::cout << "\n UT_Switch  04: testInitialization ..................................";

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
void UT_Switch::testCloseCmd() {
    std::cout << "\n UT_Switch  05: testCloseCmd ........................................";

    tArticle->mSwitchCommandedClosed = true;
    tArticle->updateSwitchState(true);

    CPPUNIT_ASSERT(tArticle->mSwitchIsClosed);
    CPPUNIT_ASSERT(!tArticle->isTripped());

    std::cout << "... Pass";
}

/// @details test for open command response
void UT_Switch::testOpenCmd() {
    std::cout << "\n UT_Switch  06: testOpenCmd .........................................";

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
void UT_Switch::testNominalCurrentFlow() {
    std::cout << "\n UT_Switch  07: testNominalCurrentFlow ..............................";

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
void UT_Switch::testNoCurrentWhenOpen() {
    std::cout << "\n UT_Switch  08: testNoCurrentWhenOpen ...............................";

    //tArticle->updateSwitchFlow(3.0, 124.5, 124.5, 1, false, true);
    tArticle->updateSwitchFlow(3.0, 3.0, 124.5, 1, false);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->getCurrent(), tTolerance);
    CPPUNIT_ASSERT(!tArticle->mSwitchIsClosed);
    CPPUNIT_ASSERT(!tArticle->isTripped());
    CPPUNIT_ASSERT(!tArticle->isWaitingToTrip());

    std::cout << "... Pass";
}

/// @details test over current trip
void UT_Switch::testOverCurrentTrip() {
    std::cout << "\n UT_Switch  09: testOverCurrentTrip .................................";

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
//void UT_Switch::testUnderVoltTrip() {
//    std::cout << "\n UT_Switch  10: testUnderVoltTrip ...................................";
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
void UT_Switch::testGetPowerDissipation() {
    std::cout << "\n UT_Switch  11: testGetPowerDissipation .............................";

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
void UT_Switch::testGetCurrent() {
    std::cout << "\n UT_Switch  12: testGetCurrent ......................................";

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
void UT_Switch::testGetConductance() {
    std::cout << "\n UT_Switch  13: testGetConductance ..................................";

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
void UT_Switch::testIsClosed() {
     std::cout << "\n UT_Switch  14: testIsClosed ........................................";

    tArticle->mSwitchCommandedClosed = true;
    tArticle->updateSwitchState(true);
    CPPUNIT_ASSERT(tArticle->isClosed());

    tArticle->mSwitchCommandedClosed = false;
    tArticle->updateSwitchState(true);
    CPPUNIT_ASSERT(!tArticle->isClosed());

    std::cout << "... Pass";

}

/// @details test isTripped function
void UT_Switch::testIsTripped() {
    std::cout << "\n UT_Switch  15: testIsTripped .......................................";

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
void UT_Switch::testIsWaitingToTripPos() {
    std::cout << "\n UT_Switch  16: testIsWaitingToTripPos ..............................";

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

    tempConfigData = new SwitchConfigData (tSwitchOnResistance, tOverCurrentProtection,
            tUnderVoltProtection, tMinVoltage, tIsTwoPortSwitch, tPortAssigned, tTripPriority);

    tempInputData = new SwitchInputData (tSwitchMalfFlag, tSwitchMalf, tSwitchIsClosed, tPosTripLimit, tNegTripLimit);

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
void UT_Switch::testIsWaitingToTripNeg() {
    std::cout << "\n UT_Switch  17: testIsWaitingToTripNeg ..............................";

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

    tempConfigData = new SwitchConfigData (tSwitchOnResistance, tOverCurrentProtection,
            tUnderVoltProtection, tMinVoltage, tIsTwoPortSwitch, tPortAssigned, tTripPriority);

    tempInputData = new SwitchInputData (tSwitchMalfFlag, tSwitchMalf, tSwitchIsClosed, tPosTripLimit, tNegTripLimit);

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
void UT_Switch::testIsTwoPortSwitch() {
    std::cout << "\n UT_Switch  18: testIsTwoPortSwitch .................................";

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

    tempConfigData = new SwitchConfigData (tSwitchOnResistance, tOverCurrentProtection,
             tUnderVoltProtection, tMinVoltage, tIsTwoPortSwitch, tPortAssigned, tTripPriority);

     tempInputData = new SwitchInputData (tSwitchMalfFlag, tSwitchMalf, tSwitchIsClosed, tPosTripLimit, tNegTripLimit);

    tArticle->initialize(*tempConfigData, *tempInputData);


    CPPUNIT_ASSERT(tArticle->isTwoPortSwitch());
    CPPUNIT_ASSERT_EQUAL(tPortAssigned, tArticle->mPortAssigned);

    delete tempConfigData;
    delete tempInputData;

    std::cout << "... Pass";
}

/// @details test malf fails closed
void UT_Switch::testMalfFailClosed() {
    std::cout << "\n UT_Switch  19: testMalfFailClosed ..................................";

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
void UT_Switch::testMalfFailOpen() {
    std::cout << "\n UT_Switch  20: testMalfFailOpen ....................................";

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
//void UT_Switch::testIsVoltTrip() {
//    std::cout << "\n UT_Switch  21: testIsVoltTrip ......................................";
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
void UT_Switch::testIsPosTrip() {
    std::cout << "\n UT_Switch  22: testIsPosTrip .......................................";

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
void UT_Switch::testIsNegTrip() {
    std::cout << "\n UT_Switch  23: testIsNegTrip .......................................";

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
void UT_Switch::testMalfSwitchResistanceLow() {
    std::cout << "\n UT_Switch  24: testMalfSwitchResistanceLow .........................";

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
void UT_Switch::testMalfSwitchResistanceHigh() {
    std::cout << "\n UT_Switch  25: testMalfSwitchResistanceHigh ........................";

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
void UT_Switch::testTripReset() {
    std::cout << "\n UT_Switch  26: testTripReset .......................................";

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
void UT_Switch::testBadInitData() {
    std::cout << "\n UT_Switch  27: testBadInitData .....................................";

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
void UT_Switch::testMalfPosTripLimit() {
    std::cout << "\n UT_Switch  28: testMalfPosTripLimit ................................";

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
void UT_Switch::testMalfNegTripLimit() {
    std::cout << "\n UT_Switch  29: testMalfNegTripLimit ................................";

    // clear out old config and init data
    delete tConfigData;
    delete tInputData;

    tConfigData = new SwitchConfigData (tSwitchOnResistance, tOverCurrentProtection,
            tUnderVoltProtection, tMinVoltage, tIsTwoPortSwitch, tPortAssigned, tTripPriority);
    tInputData = new SwitchInputData (tSwitchMalfFlag, tSwitchMalf, tSwitchIsClosed, tPosTripLimit, -3.5);

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

void UT_Switch::testGetPortAssigned() {
    std::cout << "\n UT_Switch  30: testGetPortAssigned .................................";

    // clear out old config and init data
    delete tConfigData;
    delete tInputData;

    tConfigData = new SwitchConfigData (tSwitchOnResistance, tOverCurrentProtection,
            tUnderVoltProtection, tMinVoltage, true, 2, tTripPriority);
    tInputData = new SwitchInputData (tSwitchMalfFlag, tSwitchMalf, tSwitchIsClosed, tPosTripLimit, tNegTripLimit);

    tArticle->initialize(*tConfigData, *tInputData);

    // close the switch, test it's closed
     tArticle->mSwitchCommandedClosed = true;
     tArticle->updateSwitchState(true);
     CPPUNIT_ASSERT(tArticle->isClosed());

     CPPUNIT_ASSERT_EQUAL(2, tArticle->getPortAssigned());

     std::cout << "... Pass";
}

void UT_Switch::testPowerSupplyValid() {
    std::cout << "\n UT_Switch  31: testPowerSupplyValid.................................";

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

void UT_Switch::testgetOutputVolts() {
    std::cout << "\n UT_Switch  32: testGetOutputVolts...................................";

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
