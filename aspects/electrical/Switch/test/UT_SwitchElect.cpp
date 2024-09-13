/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
 (
    (aspects/electrical/Switch/SwitchElect.o)
    (software/exceptions/TsInitializationException.o)
    (software/exceptions/TsOutOfBoundsException.o)
 )
 ***************************************************************************************************/
#include <iostream>
#include "UT_SwitchElect.hh"
#include "aspects/electrical/Switch/SwitchElect.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

/// @details default constructor for the test object
UT_SwitchElect::UT_SwitchElect()
:
tConfigData(),
tempConfigData(),
tInputData(),
tempInputData(),
tArticle(),
tLinks(),
tNodeList(),
tNodes(),
tPortMap(),
tNumPorts(),
tName(""),
tSwitchOnResistance(0.0668),
tOverCurrentProtection(true),
tUnderVoltProtection(true),
tMinVoltage(105.0),
tIsTwoPortSwitch(false),
tPortAssigned(0),
tTripPriority(1),
tMalfBlockageFlag(false),
tMalfBlockageValue(0),
tSwitchMalfFlag(0),
tSwitchMalf(0),
tSwitchIsClosed(false),
tPosTripLimit(3.5),
tNegTripLimit(0.1),
tTolerance(0.1)
{
}

/// @details default destructor of the test object
UT_SwitchElect::~UT_SwitchElect() {
}

/// @details clean up variables after each unit test.
void UT_SwitchElect::tearDown() {
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete [] tPortMap;

}

/// @details create and assign data for the test
void UT_SwitchElect::setUp() {
    // config data
    tName = "testSwitch";
    tSwitchOnResistance = 0.0668;   // type V RPCM 3.5 amp switch
    tOverCurrentProtection = true;  // switch will trip from over current
    tUnderVoltProtection = true;    // switch will trip from under voltage
    tMinVoltage = 105.0;            // type V RPCM
    tIsTwoPortSwitch = false;       // this instance does not power another RPCM
    tPortAssigned = 0;              // default value for a non two port switch
    tTripPriority = 1;              // trip allowed after first minor step

    // input data
    tMalfBlockageFlag = false;
    tMalfBlockageValue = 0;
    tSwitchMalfFlag = false;            // no malf active by default
    tSwitchMalf = 0;                // no malf active by default
    tSwitchIsClosed = false;        // default switch state is open
    tPosTripLimit = 3.5;            // type V RPCM trip setpoint
    tNegTripLimit = -0.1;

    tNumPorts = 2;
    tPortMap = new int[tNumPorts];

    tPortMap[0] = testSource;
    tPortMap[1] = testDestination;

    tNodeList.mNodes = tNodes;
    tNodeList.mNumNodes = testNumNodes;

    tArticle = new FriendlySwitchElect();

    tConfigData = new SwitchElectConfigData (tName, &tNodeList, GunnsElectSwitchUtilConfigData(tSwitchOnResistance,
                                                                                 tOverCurrentProtection,
                                                                                 tUnderVoltProtection,
                                                                                 tMinVoltage,
                                                                                 tIsTwoPortSwitch,
                                                                                 tPortAssigned,
                                                                                 tTripPriority));

    tInputData = new SwitchElectInputData (tMalfBlockageFlag, tMalfBlockageValue, GunnsElectSwitchUtilInputData(tSwitchMalfFlag,
                                                                                             tSwitchMalf,
                                                                                             tSwitchIsClosed,
                                                                                             tPosTripLimit,
                                                                                             tNegTripLimit));

    try {
        tArticle->initialize(*tConfigData, *tInputData, tLinks, 0, 1);

    } catch (TsInitializationException &e) {
        std::cout << "UT_Switch Setup method initialization exception -- failed" << std::endl;
    } catch (...) {
        std::cout << "UT_Switch Setup method initialization exception catch all exception -- failed" << std::endl;
    }
}

/// @details test for construction of config details
void UT_SwitchElect::testConfigData() {
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UT_SwitchElect  01: testConfigData .................................";

    // Config data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSwitchOnResistance, tConfigData->mSwitchConfigData.mDefaultSwitchResistance, tTolerance);
    CPPUNIT_ASSERT(tConfigData->mSwitchConfigData.mOverCurrentProtection);
    CPPUNIT_ASSERT(tConfigData->mSwitchConfigData.mUnderVoltProtection);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMinVoltage, tConfigData->mSwitchConfigData.mMinVoltage, tTolerance);
    CPPUNIT_ASSERT(!tConfigData->mSwitchConfigData.mIsTwoPortSwitch);
    CPPUNIT_ASSERT_EQUAL(tPortAssigned, tConfigData->mSwitchConfigData.mPortAssigned);
    CPPUNIT_ASSERT_EQUAL(tTripPriority, tConfigData->mSwitchConfigData.mTripPriority);

    /// @test   Check copy config construction
    SwitchElectConfigData copyConfig(*tConfigData);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSwitchOnResistance, copyConfig.mSwitchConfigData.mDefaultSwitchResistance, tTolerance);
    CPPUNIT_ASSERT(copyConfig.mSwitchConfigData.mOverCurrentProtection);
    CPPUNIT_ASSERT(copyConfig.mSwitchConfigData.mUnderVoltProtection);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMinVoltage, copyConfig.mSwitchConfigData.mMinVoltage, tTolerance);
    CPPUNIT_ASSERT(!copyConfig.mSwitchConfigData.mIsTwoPortSwitch);
    CPPUNIT_ASSERT_EQUAL(tPortAssigned, copyConfig.mSwitchConfigData.mPortAssigned);
    CPPUNIT_ASSERT(tTripPriority == copyConfig.mSwitchConfigData.mTripPriority);

    std::cout << "... Pass";
}

/// @details test for construction of input data
void UT_SwitchElect::testInputData() {
    std::cout << "\n UT_SwitchElect  02: testInputData ..................................";

    /// @test Test construction of input data
    CPPUNIT_ASSERT_EQUAL(tSwitchMalfFlag, tInputData->mSwitchInputData.mSwitchMalfFlag);
    CPPUNIT_ASSERT_EQUAL(tSwitchMalf,tInputData->mSwitchInputData.mSwitchMalfValue);
    CPPUNIT_ASSERT(!tInputData->mSwitchInputData.mSwitchIsAutoClosed);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tPosTripLimit, tInputData->mSwitchInputData.mPosTripLimit, tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNegTripLimit, tInputData->mSwitchInputData.mNegTripLimit, tTolerance);

    /// @test test copy constructor for input data
    SwitchElectInputData copyInput(*tInputData);

    CPPUNIT_ASSERT_EQUAL(tSwitchMalfFlag, copyInput.mSwitchInputData.mSwitchMalfFlag);
    CPPUNIT_ASSERT_EQUAL(tSwitchMalf, copyInput.mSwitchInputData.mSwitchMalfValue);
    CPPUNIT_ASSERT(!copyInput.mSwitchInputData.mSwitchIsAutoClosed);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tPosTripLimit, copyInput.mSwitchInputData.mPosTripLimit, tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNegTripLimit, copyInput.mSwitchInputData.mNegTripLimit, tTolerance);

    std::cout << "... Pass";
}

/// @details test for default construction
void UT_SwitchElect::testDefaultConstruction() {
    std::cout << "\n UT_SwitchElect  03: testDefaultConstruction ........................";

    FriendlySwitchElect article;

    article.initialize(*tConfigData, *tInputData, tLinks, 0, 1);

    std::cout << "... Pass";
}

/// @details test for construction of config data

void UT_SwitchElect::testUpdateState() {
    std::cout << "\n UT_SwitchElect  04: testUpdateState ................................";

    tArticle->updateState(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mEffectiveConductivity, tTolerance);

    std::cout << "... Pass";

}

void UT_SwitchElect::testUpdateFlux() {
    std::cout << "\n UT_SwitchElect  05: testUpdateFlux. ................................";

    tArticle->mPotentialVector[0] = 124.5;

    tArticle->updateFlux(0.1, 3.0);

    std::cout << "... Pass";

}

void UT_SwitchElect::testSwitchIsTripped() {
    std::cout << "\n UT_SwitchElect  06: testSwitchIsTripped. ...........................";

    // pass voltages into the switch
    tArticle->mPotentialVector[0] = 124.5;
    tArticle->mPotentialVector[1] = 124.5;

    CPPUNIT_ASSERT(!tArticle->isSwitchClosed());

    tArticle->updateFlux(0.1, 0.1);

    CPPUNIT_ASSERT(!tArticle->isSwitchTripped());


    std::cout << "... Pass";
}

void UT_SwitchElect::testSwitchIsClosed() {
    std::cout << "\n UT_SwitchElect  07: testSwitchIsClosed. ............................";


    std::cout << "... Pass";
}

void UT_SwitchElect::testSetSwitchCommandedClosed() {
    std::cout << "\n UT_SwitchElect  08: testSetSwitchCommandedClosed. ..................";


    std::cout << "... Pass";
}

void UT_SwitchElect::testSetSwitchTripReset() {
    std::cout << "\n UT_SwitchElect  09: testSetSwitchTripReset. ........................";


    std::cout << "... Pass";
}

void UT_SwitchElect::testGetCurrent() {
    std::cout << "\n UT_SwitchElect  10: testGetCurrent. ................................";


    std::cout << "... Pass";
}

void UT_SwitchElect::testGetVoltages() {
    std::cout << "\n UT_SwitchElect  10: testGetVoltages. ...............................";


    std::cout << "... Pass";
    std::cout << "\n -----------------------------------------------------------------------------";
}

