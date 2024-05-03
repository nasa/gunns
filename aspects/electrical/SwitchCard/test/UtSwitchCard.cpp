/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
 (
 (aspects/electrical/SwitchCard/SwitchCardElect.o)
 (core/GunnsBasicLink.o)
 (software/exceptions/TsConfigurationException.o)
 (aspects/electrical/UserLoad/ConstantPowerLoad.o)
 (aspects/electrical/UserLoad/ResistiveLoad.o)
 (aspects/electrical/UserLoad/UserLoadBase.o)
 (math/UnitConversion.o)
 (UtSwitchCard.o)
 )

 PROGRAMMERS:
 - ((Philip Trottman) (USA) (Initial) (2012-02))
 ***************************************************************************************************/
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "math/UnitConversion.hh"
#include "UtSwitchCard.hh"
#include <iostream>

EpsTestLoads::EpsTestLoads()
:
testRpcmSwitch2(),
testRpcmSwitch4()
{}
EpsTestLoads::~EpsTestLoads() {}

EpsTestLoadsConfig::EpsTestLoadsConfig()
:
testRpcmSwitch2Config(ConstantPowerLoadConfigData("TestRpcmSwitch2Load", CONSTANT_POWER_LOAD,90,200,5)),
testRpcmSwitch4Config(ResistiveLoadConfigData("TestRpcmSwitch4Load", RESISTIVE_LOAD, 90,25,100))
{}
EpsTestLoadsConfig::~EpsTestLoadsConfig() {}

EpsTestLoadsInput::EpsTestLoadsInput()
:
testRpcmSwitch2Input(ConstantPowerLoadInputData(0, 0.0, 1)),
testRpcmSwitch4Input(ResistiveLoadInputData(0, 0.0, 1))
{}
EpsTestLoadsInput::~EpsTestLoadsInput() {}

UtSwitchCard::UtSwitchCard() :
        tArticle(),
        tConfigData(),
        tInputData(),
        tLoads(),
        tLoadsConfig(),
        tLoadsInput(),
        tNetworkLoads(),
        tCardLoads(),
        tLinks(),
        tNodeList(),
        tNodes(),
        tPortMap(),
        tNumPorts(),
        tName(""),
        tNumSwitches(),
        tNumTwoPortSwitches(),
        tPowerSupplyLoad(),
        tInputVoltSensorConfigData(),
        tInputCurrentSensorConfigData(),
        tSwitchVoltSensorConfigData(),
        tSwitchCurrentSensorConfigData(),
        tMalfBlockageFlag(),
        tMalfBlockageValue(),
        tSwitchPosTripLimit(),
        tSwitchNegTripLimit(),
        tMinInputVoltage(),
        tMaxConductance(),
        tInputVoltSensorInputData(),
        tInputCurrentSensorInputData(),
        tSwitchVoltSensorInputData(),
        tSwitchCurrentSensorInputData(),
        tTolerance(0.1),
        tTimeStep()
{
}
UtSwitchCard::~UtSwitchCard() {}

void UtSwitchCard::setUp() {

    tNumSwitches = 4;
    tNumTwoPortSwitches = 2;
    tPowerSupplyLoad = 3.7;
    tName = "testRPCM";

    tNumPorts = tNumTwoPortSwitches + 2;
    tPortMap = new int[tNumPorts];

    tPortMap[0] = UtSwitchCard::testSource;
    tPortMap[1] = UtSwitchCard::testRpcm1;
    tPortMap[2] = UtSwitchCard::testRpcm2;
    tPortMap[3] = UtSwitchCard::testGround;

    // initialize the test nodes
    tNodeList.mNodes = tNodes;
    tNodeList.mNumNodes = testNumNodes;
    tNodes[0].initialize("tNodes_0", 124.5);
    tNodes[1].initialize("tNodes_1", 124.5);
    tNodes[2].initialize("tNodes_2", 124.5);
    tNodes[3].initialize("tNodes_3",   0.0);

    tSwitchPosTripLimit = 25.0;
    tSwitchNegTripLimit = -0.1;

    tMinInputVoltage = 70.0;
    tMaxConductance = 1.3E5;

    tInputVoltSensorConfigData = SensorAnalogConfigData(0.0, 140.0, 0.0, 0.0, 1.0, 0.0, 0.001, 0, UnitConversion::NO_CONVERSION);
    tInputCurrentSensorConfigData = SensorAnalogConfigData(-125.0, 125.0, 0.0, 0.0, 1.0, 0.0, 0.001, 0, UnitConversion::NO_CONVERSION);
    tSwitchVoltSensorConfigData = SensorAnalogConfigData(0.0, 250.0, 0.0, 0.0, 1.0, 0.0, 0.001, 0, UnitConversion::NO_CONVERSION);
    tSwitchCurrentSensorConfigData = SensorAnalogConfigData(-250.0, 250.0, 0.0, 0.0, 1.0, 0.0, 0.001, 0, UnitConversion::NO_CONVERSION);

    tInputVoltSensorInputData = SensorAnalogInputData(true, 0.0);
    tInputCurrentSensorInputData = SensorAnalogInputData(true, 0.0);
    tSwitchVoltSensorInputData = SensorAnalogInputData(true, 0.0);
    tSwitchCurrentSensorInputData = SensorAnalogInputData(true, 0.0);
    // create the test instance
    tArticle = new FriendlySwitchCard();

    // config data for the switch card
    tConfigData = new SwitchCardElectConfigData (tName,
                                                 &tNodeList,
                                                 tNumSwitches,
                                                 tNumTwoPortSwitches,
                                                 tPowerSupplyLoad,
                                                 &tInputVoltSensorConfigData,
                                                 &tInputCurrentSensorConfigData,
                                                 &tSwitchVoltSensorConfigData,
                                                 &tSwitchCurrentSensorConfigData,
                                                 105.0,
                                                 190.0,
                                                 NULL);

    // config data for the switches
    tConfigData->mSwitchConfigData[0].init(0.0158, true, true, 105.0, true, 1, 2);
    tConfigData->mSwitchConfigData[1].init(0.0158, true, true, 105.0, false, 0, 1);
    tConfigData->mSwitchConfigData[2].init(0.0158, true, true, 105.0, true, 2, 2);
    tConfigData->mSwitchConfigData[3].init(0.0158, true, true, 105.0, false, 0, 1);

    tConfigData->mPorts[0] = testSource;
    tConfigData->mPorts[1] = testRpcm1;
    tConfigData->mPorts[2] = testRpcm2;

    // input data for the switch card
    tInputData = new SwitchCardElectInputData (tMalfBlockageFlag,
                                               tMalfBlockageValue,
                                               tNumSwitches,
                                               &tInputVoltSensorInputData,
                                               &tInputCurrentSensorInputData,
                                               &tSwitchVoltSensorInputData,
                                               &tSwitchCurrentSensorInputData,
                                               tMinInputVoltage,
                                               tMaxConductance);

    // input data for the switches
    for(int i = 0; i<tNumSwitches; i++) {
        tInputData->mSwitchInputData[i].init(false, 0, false, tSwitchPosTripLimit, tSwitchNegTripLimit);
    }

    //set up the user loads
    tLoads.testRpcmSwitch2.initialize(tLoadsConfig.testRpcmSwitch2Config, tLoadsInput.testRpcmSwitch2Input, tNetworkLoads, 0, 1);
    tLoads.testRpcmSwitch4.initialize(tLoadsConfig.testRpcmSwitch4Config, tLoadsInput.testRpcmSwitch4Input, tNetworkLoads, 0, 3);

    ///- Now go through the network loads and assign each load pointer to its appropriate card.
    int cardId = 0;
    int loadSwitchId = 0;

    //std::cout << std::endl << "In setUp mapping card loads .. " << std::endl;
    for (unsigned int i = 0; i < tNetworkLoads.size(); ++i) {
        cardId = tNetworkLoads[i]->getCardId();
        loadSwitchId = tNetworkLoads[i]->getLoadSwitchId();
        tCardLoads[cardId][loadSwitchId] = tNetworkLoads[i];
        //std::cout << "tNetworkLoad[" << i << "] mapped to tCardLoads[" << cardId << "]["
        //        << loadSwitchId << "]. Name: " << tNetworkLoads[i]->mName  << std::endl;
    }
    //std::cout << std::endl;
}

void UtSwitchCard::tearDown() {
    delete tArticle;
    delete tConfigData;
    delete tInputData;
    delete [] tPortMap;
}

void UtSwitchCard::testConfigData() {
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtSwitchCard  01: testConfigData ...................................";

    CPPUNIT_ASSERT_EQUAL(tNumSwitches, tConfigData->mNumSwitches);
    CPPUNIT_ASSERT_EQUAL(tNumTwoPortSwitches, tConfigData->mNumTwoPortSwitches);

    std::cout << "... Pass";
}

void UtSwitchCard::testInputData() {
    std::cout << "\n UtSwitchCard  02: testInputData ....................................";

    CPPUNIT_ASSERT_EQUAL(tNumSwitches, tInputData->mNumSwitches);

    /// - Test default constructor
    SwitchCardElectInputData article;
    CPPUNIT_ASSERT_EQUAL(false, article.mMalfBlockageFlag);
    CPPUNIT_ASSERT_EQUAL(0.0,   article.mMalfBlockageValue);
    CPPUNIT_ASSERT_EQUAL(1,     article.mNumSwitches);
    CPPUNIT_ASSERT(0 != article.mSwitchInputData);
    CPPUNIT_ASSERT_EQUAL(0,     article.mSwitchInputData[0].mSwitchMalfValue);
    CPPUNIT_ASSERT_EQUAL(3.5,   article.mSwitchInputData[0].mPosTripLimit);
    CPPUNIT_ASSERT(0 == article.mInputVoltSensorInputData);
    CPPUNIT_ASSERT(0 == article.mInputCurrentSensorInputData);
    CPPUNIT_ASSERT(0 == article.mSwitchVoltSensorInputData);
    CPPUNIT_ASSERT(0 == article.mSwitchCurrentSensorInputData);
    CPPUNIT_ASSERT_EQUAL(70.0,  article.mMinInputVoltage);
    CPPUNIT_ASSERT_EQUAL(1.3E5, article.mMaxConductance);

    CPPUNIT_ASSERT_EQUAL(tSwitchPosTripLimit, tInputData->mSwitchInputData[0].mPosTripLimit);

    SwitchCardElectInputData copyInput(*tInputData);

    CPPUNIT_ASSERT_EQUAL(tNumSwitches, copyInput.mNumSwitches);

    std::cout << "... Pass";
}

void UtSwitchCard::testDefaultConstruction() {
    std::cout << "\n UtSwitchCard  03: testDefaultConstruction ..........................";

    FriendlySwitchCard article;

    article.initialize(*tConfigData, *tInputData, tLinks, tCardLoads[0], tPortMap);

    std::cout << "... Pass";
}

void UtSwitchCard::testInitialization() {
    std::cout << "\n UtSwitchCard  04: testInitialization ...............................";

    tArticle->initialize(*tConfigData, *tInputData, tLinks, tCardLoads[0], tPortMap);

    CPPUNIT_ASSERT(tName == tArticle->mName);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getPotential(), tArticle->mBusVoltage, tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mTotalCPowerLoadsPower, tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mInputCurrent, tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mResLoadsConductance, tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mCPowerLoadsConductance, tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mActiveResLoadsConductance, tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mActiveCPowerLoadsConductance, tTolerance);
    CPPUNIT_ASSERT(!tArticle->mIsMinorStep);
    CPPUNIT_ASSERT(!tArticle->mAnySwitchTripped);
    CPPUNIT_ASSERT(tArticle->isNonLinear());

    std::cout << "... Pass";
}

void UtSwitchCard::testNoSwitchesClosed() {
    std::cout << "\n UtSwitchCard  05: testNoSwitchesClosed .............................";

    tArticle->initialize(*tConfigData, *tInputData, tLinks, tCardLoads[0], tPortMap);

    // set voltages
    tArticle->mPotentialVector[0] = 124.5;
    tArticle->mPotentialVector[1] = 0.0;
    tArticle->mPotentialVector[2] = 0.0;

    // step the model
    UtSwitchCard::stepTheModel();

//    tArticle->step(0.1);
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);
//    for (int i = 0; i < 10; i++) {
//        tArticle->minorStep(0.1, 1);
//    }
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);

    // check that all switches are open
    CPPUNIT_ASSERT(!tArticle->mSwitch[0].isClosed());
    CPPUNIT_ASSERT(!tArticle->mSwitch[1].isClosed());
    CPPUNIT_ASSERT(!tArticle->mSwitch[2].isClosed());
    CPPUNIT_ASSERT(!tArticle->mSwitch[3].isClosed());

    // check that userLoads are unpowered
    CPPUNIT_ASSERT(!tArticle->mLoad[1]->getPowerValid());
    CPPUNIT_ASSERT(!tArticle->mLoad[3]->getPowerValid());

    // check that the voltage at port 0 shows up as the RPCM bus voltage
    CPPUNIT_ASSERT_DOUBLES_EQUAL(124.5, tArticle->mBusVoltage, tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1, tArticle->mAdmittanceMatrix[0], tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mInputCurrent, tTolerance);

    std::cout << "... Pass";
}

void UtSwitchCard::testCloseSwitchOne() {
    std::cout << "\n UtSwitchCard  06: testCloseSwitchOne ...............................";

    tArticle->initialize(*tConfigData, *tInputData, tLinks, tCardLoads[0], tPortMap);

    // set voltages
    tArticle->mPotentialVector[0] = 124.5;
    tArticle->mPotentialVector[1] = 124.4;
    tArticle->mPotentialVector[2] = 0.0;

    tArticle->mSwitch[0].setSwitchCommandedClosed(true);
    tArticle->mSwitch[0].updateSwitchState(true);

    CPPUNIT_ASSERT(tArticle->mSwitch[0].isClosed());

    // step the model
    UtSwitchCard::stepTheModel();

    //    tArticle->step(0.1);
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);
//    for (int i=0; i < 10; i++) {
//        tArticle->minorStep(0.1, 1);
//    }
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);

    // check that the voltage at port 0 shows up as the RPCM bus voltage
    CPPUNIT_ASSERT_DOUBLES_EQUAL(124.5, tArticle->mBusVoltage, tTolerance);

    //  admittance matrix[0] with only switch 1 closed should be 1 / 0.0158 ohms = 63.3 mhos
    CPPUNIT_ASSERT_DOUBLES_EQUAL(63.3, tArticle->mAdmittanceMatrix[0], tTolerance);

    // a 0.1 volt drop across the switch (124.5 at input port 0 - 124.4 volts at output port 1)
    // with a switch resistance of 0.0158 ohms yields 0.1 / 0.0158 = 6.33 amps of current through the switch
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.3, tArticle->mInputCurrent, tTolerance);

    std::cout << "... Pass";
}

void UtSwitchCard::testCloseSwitchTwo() {
    std::cout << "\n UtSwitchCard  07: testCloseSwitchTwo ...............................";

    tArticle->initialize(*tConfigData, *tInputData, tLinks, tCardLoads[0], tPortMap);

   // set voltages
    tArticle->mPotentialVector[0] = 124.5;
    tArticle->mPotentialVector[1] = 0.0;
    tArticle->mPotentialVector[2] = 0.0;

    tArticle->mSwitch[1].setSwitchCommandedClosed(true);
    tArticle->mSwitch[1].updateSwitchState(true);

    CPPUNIT_ASSERT(tArticle->mSwitch[1].isClosed());

    // step the model
    UtSwitchCard::stepTheModel();
//    tArticle->step(0.1);
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);
//    for (int i=0; i < 10; i++) {
//        tArticle->minorStep(0.1, 1);
//        //std::cout << tArticle->mInputCurrent << " ";
//    }
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);


    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.6, tArticle->mInputCurrent, tTolerance);


    // check that the voltage at port 0 shows up as the RPCM bus voltage
    CPPUNIT_ASSERT_DOUBLES_EQUAL(124.5, tArticle->mBusVoltage, tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.01, tArticle->mAdmittanceMatrix[0], 1.0);

    // CP load is 200 watts, at 124.5 volts resistance = 124.5^2 / 200 = 77 ohms
    // this results in a current of 124.5 / 77 = 1.606 amps
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.6, tArticle->mInputCurrent, tTolerance);

    // check that the admittance matrix is flagged as needing an update from the new load
    // CPPUNIT_ASSERT(tArticle->needAdmittanceUpdate());

    std::cout << "... Pass";
}

void UtSwitchCard::testCloseSwitchThree() {
    std::cout << "\n UtSwitchCard  08: testCloseSwitchThree .............................";

    tArticle->initialize(*tConfigData, *tInputData, tLinks, tCardLoads[0], tPortMap);

    // set voltages
    tArticle->mPotentialVector[0] = 124.5;
    tArticle->mPotentialVector[1] = 0.0;
    tArticle->mPotentialVector[2] = 124.4;

    tArticle->mSwitch[2].setSwitchCommandedClosed(true);
    tArticle->mSwitch[2].updateSwitchState(true);

    CPPUNIT_ASSERT(tArticle->mSwitch[2].isClosed());

    // step the model
    UtSwitchCard::stepTheModel();

//    tArticle->step(0.1);
//    tArticle->computeFlows(0.1);
//   for (int i=0; i < 10; i++) {
//        tArticle->minorStep(0.1, 1);
//    }
//    tArticle->confirmSolutionAcceptable(1, 1);

    // check that the voltage at port 0 shows up as the RPCM bus voltage
    CPPUNIT_ASSERT_DOUBLES_EQUAL(124.5, tArticle->mBusVoltage, tTolerance);

    //  admittance matrix[0] with only switch 1 closed should be 1 / 0.0158 ohms = 63.3 mhos
    CPPUNIT_ASSERT_DOUBLES_EQUAL(63.3, tArticle->mAdmittanceMatrix[0], tTolerance);

    // a 0.1 volt drop across the switch (124.5 at input port 0 - 124.4 volts at output port 1)
    // with a switch resistance of 0.0158 ohms yields 0.1 / 0.0158 = 6.33 amps of current through the switch
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.3, tArticle->mInputCurrent, tTolerance);

    std::cout << "... Pass";
}

void UtSwitchCard::testCloseSwitchFour() {
    std::cout << "\n UtSwitchCard  09: testCloseSwitchFour ..............................";

    tArticle->initialize(*tConfigData, *tInputData, tLinks, tCardLoads[0], tPortMap);

    tArticle->mPotentialVector[0] = 124.5;
    tArticle->mPotentialVector[1] = 0.0;
    tArticle->mPotentialVector[2] = 0.0;

    tArticle->mSwitch[3].setSwitchCommandedClosed(true);
    tArticle->mSwitch[3].updateSwitchState(true);

    CPPUNIT_ASSERT(tArticle->mSwitch[3].isClosed());

    // step the model
    UtSwitchCard::stepTheModel();

//    tArticle->step(0.1);
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);
//    for (int i=0; i < 10; i++) {
//        tArticle->minorStep(0.1, 1);
//    }
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);

    // check that the voltage at port 0 shows up as the RPCM bus voltage
    CPPUNIT_ASSERT_DOUBLES_EQUAL(124.5, tArticle->mBusVoltage, tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.01, tArticle->mAdmittanceMatrix[0], 1.0);

    // 25 ohm resistive load results in a current of 124.5 / 25 = 4.98 amps
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.98, tArticle->mInputCurrent, tTolerance);


    std::cout << "... Pass";
}

void UtSwitchCard::testCloseAllSwitches() {
    std::cout << "\n UtSwitchCard  10: testCloseAllSwitches .............................";

    tArticle->initialize(*tConfigData, *tInputData, tLinks, tCardLoads[0], tPortMap);

    tArticle->mPotentialVector[0] = 124.5;
    tArticle->mPotentialVector[1] = 124.4;
    tArticle->mPotentialVector[2] = 124.4;

    // close all the switches
    for (int i = 0; i < tNumSwitches; i++) {
        tArticle->mSwitch[i].setSwitchCommandedClosed(true);
        tArticle->mSwitch[i].updateSwitchState(true);
    }

    // verify that all the switches are closed
    CPPUNIT_ASSERT(tArticle->mSwitch[0].isClosed());
    CPPUNIT_ASSERT(tArticle->mSwitch[1].isClosed());
    CPPUNIT_ASSERT(tArticle->mSwitch[2].isClosed());
    CPPUNIT_ASSERT(tArticle->mSwitch[3].isClosed());

    // step the model
    UtSwitchCard::stepTheModel();

    //    tArticle->step(0.1);
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);
//    for (int i=0; i < 10; i++) {
//        tArticle->minorStep(0.1, 1);
//    }
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);

    // verify that all the switches are still closed
    CPPUNIT_ASSERT(tArticle->mSwitch[0].isClosed());
    CPPUNIT_ASSERT(tArticle->mSwitch[1].isClosed());
    CPPUNIT_ASSERT(tArticle->mSwitch[2].isClosed());
    CPPUNIT_ASSERT(tArticle->mSwitch[3].isClosed());

    // check that the voltage at port 0 shows up as the RPCM bus voltage
    CPPUNIT_ASSERT_DOUBLES_EQUAL(124.5, tArticle->mBusVoltage, tTolerance);

    // input current should be the sum of all the switches:
    // 6.33 amps from switch 1 + 1.61 amps from switch 2 + 6.33 amps from switch 3 + 4.98 amps
    // from switch 4 = 19.25 amps total
    CPPUNIT_ASSERT_DOUBLES_EQUAL(19.25, tArticle->mInputCurrent, tTolerance);

    std::cout << "... Pass";
}

void UtSwitchCard::testOpenTwoPortSwitch() {
    std::cout << "\n UtSwitchCard  11: testOpenTwoPortSwitch ............................";

    tArticle->initialize(*tConfigData, *tInputData, tLinks, tCardLoads[0], tPortMap);

    ////////////////////////////////////////////////////////////////////////////////
    // start by repeating test 10
    tArticle->mPotentialVector[0] = 124.5;
    tArticle->mPotentialVector[1] = 124.4;
    tArticle->mPotentialVector[2] = 124.4;

    // close all the switches
    for (int i = 0; i < tNumSwitches; i++) {
        tArticle->mSwitch[i].setSwitchCommandedClosed(true);
        tArticle->mSwitch[i].updateSwitchState(true);
    }

    // verify that all the switches are closed
    CPPUNIT_ASSERT(tArticle->mSwitch[0].isClosed());
    CPPUNIT_ASSERT(tArticle->mSwitch[1].isClosed());
    CPPUNIT_ASSERT(tArticle->mSwitch[2].isClosed());
    CPPUNIT_ASSERT(tArticle->mSwitch[3].isClosed());

    // step the model
    UtSwitchCard::stepTheModel();

//    tArticle->step(0.1);
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);
//    for (int i=0; i < 10; i++) {
//        tArticle->minorStep(0.1, 1);
//    }
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(19.25, tArticle->mInputCurrent, tTolerance);
    //// end test 10
    ////////////////////////////////////////////////////////////////////////////////

    // now open switch 1
    tArticle->mSwitch[0].setSwitchCommandedClosed(false);
    tArticle->mSwitch[0].updateSwitchState(true);

    // verify that the switch is now open
    CPPUNIT_ASSERT(!tArticle->mSwitch[0].isClosed());
    CPPUNIT_ASSERT(tArticle->mSwitch[1].isClosed());
    CPPUNIT_ASSERT(tArticle->mSwitch[2].isClosed());
    CPPUNIT_ASSERT(tArticle->mSwitch[3].isClosed());

    // step the model
    UtSwitchCard::stepTheModel();

//    tArticle->step(0.1);
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);
//    for (int i=0; i < 10; i++) {
//        tArticle->minorStep(0.1, 1);
//    }
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);

    // input current should be 6.33 amps less than at the end of test 10
    CPPUNIT_ASSERT_DOUBLES_EQUAL(12.9, tArticle->mInputCurrent, tTolerance);

    std::cout << "... Pass";
}

void UtSwitchCard::testOpenLoadSwitch() {
    std::cout << "\n UtSwitchCard  12: testOpenLoadSwitch ...............................";

    tArticle->initialize(*tConfigData, *tInputData, tLinks, tCardLoads[0], tPortMap);

    ////////////////////////////////////////////////////////////////////////////////
    // start by repeating test 10
    tArticle->mPotentialVector[0] = 124.5;
    tArticle->mPotentialVector[1] = 124.4;
    tArticle->mPotentialVector[2] = 124.4;

    // close all the switches
    for (int i = 0; i < tNumSwitches; i++) {
        tArticle->mSwitch[i].setSwitchCommandedClosed(true);
        tArticle->mSwitch[i].updateSwitchState(true);
    }

    // verify that all the switches are closed
    CPPUNIT_ASSERT(tArticle->mSwitch[0].isClosed());
    CPPUNIT_ASSERT(tArticle->mSwitch[1].isClosed());
    CPPUNIT_ASSERT(tArticle->mSwitch[2].isClosed());
    CPPUNIT_ASSERT(tArticle->mSwitch[3].isClosed());

    // step the model
    UtSwitchCard::stepTheModel();

//    tArticle->step(0.1);
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);
//    for (int i=0; i < 10; i++) {
//        tArticle->minorStep(0.1, 1);
//    }
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(19.25, tArticle->mInputCurrent, tTolerance);
    //// end test 10
    ////////////////////////////////////////////////////////////////////////////////

    // now open switch 2
    tArticle->mSwitch[1].setSwitchCommandedClosed(false);
    tArticle->mSwitch[1].updateSwitchState(true);

    // verify that the switch is now open
    CPPUNIT_ASSERT(tArticle->mSwitch[0].isClosed());
    CPPUNIT_ASSERT(!tArticle->mSwitch[1].isClosed());
    CPPUNIT_ASSERT(tArticle->mSwitch[2].isClosed());
    CPPUNIT_ASSERT(tArticle->mSwitch[3].isClosed());

    // step the model
    UtSwitchCard::stepTheModel();

//    tArticle->step(0.1);
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);
//    for (int i=0; i < 10; i++) {
//        tArticle->minorStep(0.1, 1);
//    }
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);

    // input current should be 1.61 amps less than at the end of test 10
    CPPUNIT_ASSERT_DOUBLES_EQUAL(17.6, tArticle->mInputCurrent, tTolerance);

    std::cout << "... Pass";
}

void UtSwitchCard::testTripTwoPortSwitch() {
    std::cout << "\n UtSwitchCard  13: testTripTwoPortSwitch ............................";

    tArticle->initialize(*tConfigData, *tInputData, tLinks, tCardLoads[0], tPortMap);

    ////////////////////////////////////////////////////////////////////////////////
    ///// start by repeating test 10
    tArticle->mPotentialVector[0] = 124.5;
    tArticle->mPotentialVector[1] = 124.4;
    tArticle->mPotentialVector[2] = 124.4;

    // close all the switches
    for (int i = 0; i < tNumSwitches; i++) {
        tArticle->mSwitch[i].setSwitchCommandedClosed(true);
        tArticle->mSwitch[i].updateSwitchState(true);
    }

    // verify that all the switches are closed
    CPPUNIT_ASSERT(tArticle->mSwitch[0].isClosed());
    CPPUNIT_ASSERT(tArticle->mSwitch[1].isClosed());
    CPPUNIT_ASSERT(tArticle->mSwitch[2].isClosed());
    CPPUNIT_ASSERT(tArticle->mSwitch[3].isClosed());

    // step the model
    UtSwitchCard::stepTheModel();

//    tArticle->step(0.1);
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);
//    for (int i=0; i < 10; i++) {
//        tArticle->minorStep(0.1, 1);
//    }
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(19.25, tArticle->mInputCurrent, tTolerance);
    //// end test 10
    ////////////////////////////////////////////////////////////////////////////////

    // now trip switch 1 by lowering the voltage at the output node, increasing the current
    // across the switch above tSwitchPosTripLimit (25 amps).
    // A drop of (0.0158 ohms * 26 amps) = 0.41 volts should do it
    tArticle->mPotentialVector[0] = 124.5;
    tArticle->mPotentialVector[1] = 124.09;
    tArticle->mPotentialVector[2] = 124.4;

    // step the model
//    UtSwitchCard::stepTheModel();

    // slightly custom version of stepTheModel that updates both converged and minor step in confirmSolutionAcceptable
    tArticle->step(0.1);  // for minorStep == 1 run regular step
    for (int i = 1; i<=10; i++) {  // worst case, run until EPS minor step limit
        tArticle->minorStep(0.1, i);
        tArticle->confirmSolutionAcceptable(i, i);
    }

//   tArticle->step(0.1);
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);
//    for (int i=0; i < 10; i++) {
//        tArticle->minorStep(0.1, i);
//        tArticle->confirmSolutionAcceptable(i, i);
//    }
//    tArticle->computeFlows(0.1);


    // verify that switch 1 is now opened and tripped
    CPPUNIT_ASSERT(!tArticle->mSwitch[0].isClosed());
    CPPUNIT_ASSERT(tArticle->mSwitch[0].isTripped());

    //verify that none of the other switches are open
    CPPUNIT_ASSERT(tArticle->mSwitch[1].isClosed());
    CPPUNIT_ASSERT(tArticle->mSwitch[2].isClosed());
    CPPUNIT_ASSERT(tArticle->mSwitch[3].isClosed());

    // verify the total current is missing the -6.33 amps from switch 1
    CPPUNIT_ASSERT_DOUBLES_EQUAL(12.9, tArticle->mInputCurrent, tTolerance);

    std::cout << "... Pass";
}

void UtSwitchCard::testTripLoadSwitch() {
    std::cout << "\n UtSwitchCard  14: testTripLoadSwitch ...............................";

    tArticle->initialize(*tConfigData, *tInputData, tLinks, tCardLoads[0], tPortMap);

    ////////////////////////////////////////////////////////////////////////////////
    ///// start by repeating test 10
    tArticle->mPotentialVector[0] = 124.5;
    tArticle->mPotentialVector[1] = 124.4;
    tArticle->mPotentialVector[2] = 124.4;

    // close all the switches
    for (int i = 0; i < tNumSwitches; i++) {
        tArticle->mSwitch[i].setSwitchCommandedClosed(true);
        tArticle->mSwitch[i].updateSwitchState(true);
    }

    // verify that all the switches are closed
    CPPUNIT_ASSERT(tArticle->mSwitch[0].isClosed());
    CPPUNIT_ASSERT(tArticle->mSwitch[1].isClosed());
    CPPUNIT_ASSERT(tArticle->mSwitch[2].isClosed());
    CPPUNIT_ASSERT(tArticle->mSwitch[3].isClosed());

    // step the model
    UtSwitchCard::stepTheModel();

//    tArticle->step(0.1);
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);
//    for (int i=0; i < 10; i++) {
//        tArticle->minorStep(0.1, 1);
//    }
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(19.25, tArticle->mInputCurrent, tTolerance);
    //// end test 10
    ////////////////////////////////////////////////////////////////////////////////

    // now trip switch 2 by overwriting the constant power load value
    // 26 amps * 124.5 volts = 3,237 watts should do it
    // !!!  need a way to SET the overwritePower malf from here!
    // plan B:  lower the switch trip setpoint to below the 1.6 amps the load pulls
    // the logic is the same either way
    tArticle->mSwitch[1].setPosTripLimit(1.5);

    // step the model
    UtSwitchCard::stepTheModel();

//    tArticle->step(0.1);
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);
//    for (int i=0; i < 10; i++) {
//        tArticle->minorStep(0.1, 1);
//    }
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);

    // verify that switch 2 is now opened and tripped
    CPPUNIT_ASSERT(!tArticle->mSwitch[1].isClosed());
    CPPUNIT_ASSERT(tArticle->mSwitch[1].isTripped());

    //verify that none of the other switches are open
    CPPUNIT_ASSERT(tArticle->mSwitch[0].isClosed());
    CPPUNIT_ASSERT(tArticle->mSwitch[2].isClosed());
    CPPUNIT_ASSERT(tArticle->mSwitch[3].isClosed());

    // verify the total current is missing the 1.61 amps from switch 1
    CPPUNIT_ASSERT_DOUBLES_EQUAL(17.6, tArticle->mInputCurrent, tTolerance);


    std::cout << "... Pass";
}

void UtSwitchCard::testLossOfInputVolts() {
    std::cout << "\n UtSwitchCard  15: testLossOfInputVolts .............................";

    tArticle->initialize(*tConfigData, *tInputData, tLinks, tCardLoads[0], tPortMap);

    ////////////////////////////////////////////////////////////////////////////////
    ///// start by repeating test 10
    tArticle->mPotentialVector[0] = 124.5;
    tArticle->mPotentialVector[1] = 124.4;
    tArticle->mPotentialVector[2] = 124.4;

    // close all the switches
    for (int i = 0; i < tNumSwitches; i++) {
        tArticle->mSwitch[i].setSwitchCommandedClosed(true);
        tArticle->mSwitch[i].updateSwitchState(true);
    }

    // verify that all the switches are closed
    CPPUNIT_ASSERT(tArticle->mSwitch[0].isClosed());
    CPPUNIT_ASSERT(tArticle->mSwitch[1].isClosed());
    CPPUNIT_ASSERT(tArticle->mSwitch[2].isClosed());
    CPPUNIT_ASSERT(tArticle->mSwitch[3].isClosed());

    // step the model
    UtSwitchCard::stepTheModel();

//    tArticle->step(0.1);
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);
//    for (int i=0; i < 10; i++) {
//        tArticle->minorStep(0.1, 1);
//    }
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(19.25, tArticle->mInputCurrent, tTolerance);
    //// end test 10
    ////////////////////////////////////////////////////////////////////////////////

    // now kill the input voltage and all switches should open and input current should be zero
    tArticle->mPotentialVector[0] = 0.0;
    tArticle->mPotentialVector[1] = 0.0;
    tArticle->mPotentialVector[2] = 0.0;

    // step the model
    UtSwitchCard::stepTheModel();

//    tArticle->step(0.1);
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);
//    for (int i=0; i < 10; i++) {
//        tArticle->minorStep(0.1, 1);
//    }
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);

    // verify that all the switches are open
    CPPUNIT_ASSERT(!tArticle->mSwitch[0].isClosed());
    CPPUNIT_ASSERT(!tArticle->mSwitch[1].isClosed());
    CPPUNIT_ASSERT(!tArticle->mSwitch[2].isClosed());
    CPPUNIT_ASSERT(!tArticle->mSwitch[3].isClosed());

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mInputCurrent, tTolerance);

    std::cout << "... Pass";
}

void UtSwitchCard::testTripPriority() {
    std::cout << "\n UtSwitchCard  16: testTripPriority .................................";

    tArticle->initialize(*tConfigData, *tInputData, tLinks, tCardLoads[0], tPortMap);

    ////////////////////////////////////////////////////////////////////////////////
    ///// start by repeating test 6
    // set voltages
    tArticle->mPotentialVector[0] = 124.5;
    tArticle->mPotentialVector[1] = 124.4;
    tArticle->mPotentialVector[2] = 0.0;

    tArticle->mSwitch[0].setSwitchCommandedClosed(true);
    tArticle->mSwitch[0].updateSwitchState(true);

    CPPUNIT_ASSERT(tArticle->mSwitch[0].isClosed());

    // step the model
    UtSwitchCard::stepTheModel();

//    tArticle->step(0.1);
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);
//    for (int i=0; i < 10; i++) {
//        tArticle->minorStep(0.1, 1);
//    }
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);

    // check that the voltage at port 0 shows up as the RPCM bus voltage
    CPPUNIT_ASSERT_DOUBLES_EQUAL(124.5, tArticle->mBusVoltage, tTolerance);

    //  admittance matrix[0] with only switch 1 closed should be 1 / 0.0158 ohms = 63.3 mhos
    CPPUNIT_ASSERT_DOUBLES_EQUAL(63.3, tArticle->mAdmittanceMatrix[0], tTolerance);

    // a 0.1 volt drop across the switch (124.5 at input port 0 - 124.4 volts at output port 1)
    // with a switch resistance of 0.0158 ohms yields 0.1 / 0.0158 = 6.33 amps of current through the switch
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.3, tArticle->mInputCurrent, tTolerance);
    //// end test 6
    ////////////////////////////////////////////////////////////////////////////////

    // now trip the switch as in test 13, but with the minorStep value at 0
    // this should prevent the switch from tripping but flag it as waiting to
    // trip at minorStep 1
    tArticle->mPotentialVector[0] = 124.5;
    tArticle->mPotentialVector[1] = 124.09;
    tArticle->mPotentialVector[2] = 124.4;

    // step the model
    UtSwitchCard::stepTheModel();

//    tArticle->step(0.1);
//    tArticle->mCurrentMinorStep = 0;
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(0, 0);
//    for (int i=0; i < 10; i++) {
//        tArticle->minorStep(0.1, 0);
//    }
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(0, 0);

    // verify that switch 1 is not opened or tripped, but is waiting to trip
    CPPUNIT_ASSERT(tArticle->mSwitch[0].isClosed());
    CPPUNIT_ASSERT(!tArticle->mSwitch[0].isTripped());
    CPPUNIT_ASSERT(tArticle->mSwitch[0].isWaitingToTrip());

    std::cout << "... Pass";
}

void UtSwitchCard::testPowerSupplyFail() {
    std::cout << "\n UtSwitchCard  17: testPowerSupplyFail ..............................";

    tArticle->initialize(*tConfigData, *tInputData, tLinks, tCardLoads[0], tPortMap);

    ///////////////////////////////////////////
    // repeat test 6 to start with one switch closed
    // set voltages
    tArticle->mPotentialVector[0] = 124.5;
    tArticle->mPotentialVector[1] = 124.4;
    tArticle->mPotentialVector[2] = 0.0;

    tArticle->mSwitch[0].setSwitchCommandedClosed(true);
    tArticle->mSwitch[0].updateSwitchState(true);

    CPPUNIT_ASSERT(tArticle->mSwitch[0].isClosed());

    // step the model
    UtSwitchCard::stepTheModel();

//    tArticle->step(0.1);
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);
//    for (int i=0; i < 10; i++) {
//        tArticle->minorStep(0.1, 1);
//    }
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);

    // check that the voltage at port 0 shows up as the RPCM bus voltage
    CPPUNIT_ASSERT_DOUBLES_EQUAL(124.5, tArticle->mBusVoltage, tTolerance);

    //  admittance matrix[0] with only switch 1 closed should be 1 / 0.0158 ohms = 63.3 mhos
    CPPUNIT_ASSERT_DOUBLES_EQUAL(63.3, tArticle->mAdmittanceMatrix[0], tTolerance);

    // a 0.1 volt drop across the switch (124.5 at input port 0 - 124.4 volts at output port 1)
    // with a switch resistance of 0.0158 ohms yields 0.1 / 0.0158 = 6.33 amps of current through the switch
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.3, tArticle->mInputCurrent, tTolerance);

    // apply power supply fail malfunction
    tArticle->setMalfPowerSupplyFail(true);
    CPPUNIT_ASSERT(true  == tArticle->mMalfPowerSupplyFailFlag);

    // step the model
    UtSwitchCard::stepTheModel();

//    tArticle->step(0.1);
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);
//    for (int i=0; i < 10; i++) {
//        tArticle->minorStep(0.1, 1);
//    }
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);


    // check that the power supply valid is false and the switch is opened
    CPPUNIT_ASSERT(!tArticle->mTlmPowerSupplyValid);
    CPPUNIT_ASSERT(!tArticle->mSwitch[0].isClosed());

    // test the malfunction is reset by the setter
    tArticle->setMalfPowerSupplyFail();
    CPPUNIT_ASSERT(false == tArticle->mMalfPowerSupplyFailFlag);

    std::cout << "... Pass";
}

void UtSwitchCard::testMagicPower() {
    std::cout << "\n UtSwitchCard  18: testMagicPower ...................................";

    tArticle->initialize(*tConfigData, *tInputData, tLinks, tCardLoads[0], tPortMap);

    // set voltages
    tArticle->mPotentialVector[0] = 0.0;
    tArticle->mPotentialVector[1] = 0.0;
    tArticle->mPotentialVector[2] = 0.0;

    // step the model
    UtSwitchCard::stepTheModel();

//    tArticle->step(0.1);
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);
//    for (int i = 0; i < 10; i++) {
//        tArticle->minorStep(0.1, 1);
//    }
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);

    // check that all switches are open
    CPPUNIT_ASSERT(!tArticle->mSwitch[0].isClosed());
    CPPUNIT_ASSERT(!tArticle->mSwitch[1].isClosed());
    CPPUNIT_ASSERT(!tArticle->mSwitch[2].isClosed());
    CPPUNIT_ASSERT(!tArticle->mSwitch[3].isClosed());

    // check that userLoads are unpowered
    CPPUNIT_ASSERT(!tArticle->mLoad[1]->getPowerValid());
    CPPUNIT_ASSERT(!tArticle->mLoad[3]->getPowerValid());

    // try to close switch 1 with no power to the switchCard
    tArticle->mSwitch[0].setSwitchCommandedClosed(true);
    tArticle->step(0.1);

    // switch should not close
    CPPUNIT_ASSERT(!tArticle->mSwitch[0].isClosed());

    // override input with magic power term
    tArticle->mMagicPowerFlag = true;

    // step the model
    UtSwitchCard::stepTheModel();

//    tArticle->step(0.1);
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);
//    for (int i = 0; i < 10; i++) {
//        tArticle->minorStep(0.1, 1);
//    }
//    tArticle->computeFlows(0.1);
//    tArticle->confirmSolutionAcceptable(1, 1);

    // check that the voltage override shows up as the RPCM bus voltage
    CPPUNIT_ASSERT_DOUBLES_EQUAL(123.456, tArticle->mBusVoltage, tTolerance);

    // try to close switch 1 with no power to the switchCard
//    tArticle->mSwitch[0].setSwitchCommandedClosed(true);
//    tArticle->mSwitch[0].updateSwitchState(true);

    // switch should not close
//    CPPUNIT_ASSERT(tArticle->mSwitch[0].isClosed());
    std::cout << "... Pass";
}

void UtSwitchCard::testBadInitData() {
    std::cout << "\n UtSwitchCard  19: testBadInitData ..................................";

    tArticle->initialize(*tConfigData, *tInputData, tLinks, tCardLoads[0], tPortMap);

    // test exception for less than 1 total switches
    tConfigData->initData("testRPCM", -1, 2, 10.0);
    tInputData->init(-1, tMinInputVoltage, tMaxConductance);

    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tCardLoads[0], tPortMap), TsInitializationException);

    // test exception for more two port switches than total switches
    tConfigData->initData("testRPCM", 2, 4, 10.0);
    tInputData->init(2, tMinInputVoltage, tMaxConductance);

    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tCardLoads[0], tPortMap), TsInitializationException);

    // test exception for config and input data having different number of switches
    tConfigData->initData("testRPCM", 4, 2, 10.0);
    tInputData->init(6, tMinInputVoltage, tMaxConductance);

    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tCardLoads[0], tPortMap), TsInitializationException);

    // test exception for config having a negative power supply load
    tConfigData->initData("testRPCM", 4, 2, -10.0);
    tInputData->init(4, tMinInputVoltage, tMaxConductance);

    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tCardLoads[0], tPortMap), TsInitializationException);

    std::cout << "... Pass";
}

void UtSwitchCard::testInputUndervolt() {
    std::cout << "\n UtSwitchCard  20: testInputundervolt ...............................";

    tArticle->initialize(*tConfigData, *tInputData, tLinks, tCardLoads[0], tPortMap);

    ///////////////////////////////////////////
    // repeat test 6 to start with one switch closed
    // set voltages
    tArticle->mPotentialVector[0] = 124.5;
    tArticle->mPotentialVector[1] = 124.4;
    tArticle->mPotentialVector[2] = 0.0;

    tArticle->mSwitch[0].setSwitchCommandedClosed(true);
    tArticle->mSwitch[0].updateSwitchState(true);

    CPPUNIT_ASSERT(tArticle->mSwitch[0].isClosed());

    // step the model
    UtSwitchCard::stepTheModel();

    // check that the input undervolt fdir is enabled (not inhibited)
    CPPUNIT_ASSERT(!tArticle->mUndervoltFdirActive);

    // lower the input voltage so that the RPC trips open
    tArticle->mPotentialVector[0] = 104.5;
    tArticle->mPotentialVector[1] = 104.4;
    tArticle->mPotentialVector[2] = 0.0;

    // step the model
    UtSwitchCard::stepTheModel();

    // check that the closed switch is now open
    CPPUNIT_ASSERT(!tArticle->mSwitch[0].isClosed());

    CPPUNIT_ASSERT(tArticle->mUndervoltTrip);
    CPPUNIT_ASSERT(tArticle->mSwitch[0].isTripped());
    std::cout << "... Pass";

}

void UtSwitchCard::testInputOvervolt() {
    std::cout << "\n UtSwitchCard  21: testInputOvervolt ................................";

    tArticle->initialize(*tConfigData, *tInputData, tLinks, tCardLoads[0], tPortMap);

    ///////////////////////////////////////////
    // repeat test 6 to start with one switch closed
    // set voltages
    tArticle->mPotentialVector[0] = 124.5;
    tArticle->mPotentialVector[1] = 124.4;
    tArticle->mPotentialVector[2] = 0.0;

    tArticle->mSwitch[0].setSwitchCommandedClosed(true);
    tArticle->mSwitch[0].updateSwitchState(true);

    CPPUNIT_ASSERT(tArticle->mSwitch[0].isClosed());

    // step the model
    UtSwitchCard::stepTheModel();

    // enable the overvolt trip check by turning on the fault detection isolation and recovery function for switch 1
    tArticle->mOvervoltFdirActive[0] = true;

    // check that the input overvolt fdir is enabled
    CPPUNIT_ASSERT(tArticle->mOvervoltFdirActive[0]);

    // lower the input voltage so that the RPC trips open
    tArticle->mPotentialVector[0] = 194.5;
    tArticle->mPotentialVector[1] = 194.4;
    tArticle->mPotentialVector[2] = 0.0;

    // step the model
    UtSwitchCard::stepTheModel();

    // check that the closed switch is now open
    CPPUNIT_ASSERT(!tArticle->mSwitch[0].isClosed());

    CPPUNIT_ASSERT(tArticle->mOvervoltTrip);
    CPPUNIT_ASSERT(tArticle->mSwitch[0].isTripped());
    std::cout << "... Pass";

}

void UtSwitchCard::testClearUndervoltTrip() {
    std::cout << "\n UtSwitchCard  22: testCLearUndervoltTrip............................";

    tArticle->initialize(*tConfigData, *tInputData, tLinks, tCardLoads[0], tPortMap);

    // step the model as normal
    UtSwitchCard::stepTheModel();

    ///////////////////////////////////////////
    // repeat test 6 to start with one switch closed
    // set voltages
    tArticle->mPotentialVector[0] = 124.5;
    tArticle->mPotentialVector[1] = 124.4;
    tArticle->mPotentialVector[2] = 0.0;

    // step the model
    UtSwitchCard::stepTheModel();

    // command switch 1 closed
    tArticle->mSwitch[0].setSwitchCommandedClosed(true);
    tArticle->mSwitch[0].updateSwitchState(true);

    // step the model
    UtSwitchCard::stepTheModel();

    // check that switch 1 is closed
    CPPUNIT_ASSERT(tArticle->mSwitch[0].isClosed());

    // check the input undervolt flag is not set
    CPPUNIT_ASSERT(!tArticle->mUndervoltTrip);

    // adjust the input voltage to cause an undervolt but not shut off the device
    tArticle->mPotentialVector[0] = 101.1;
    tArticle->mPotentialVector[1] = 101.0;
    tArticle->mPotentialVector[2] = 0.0;

    // step the model
    UtSwitchCard::stepTheModel();

    // check that switch 1 is open
     CPPUNIT_ASSERT(!tArticle->mSwitch[0].isClosed());

    // restore the input voltage
    tArticle->mPotentialVector[0] = 124.5;
    tArticle->mPotentialVector[1] = 124.4;
    tArticle->mPotentialVector[2] = 0.0;

    // step the model
    UtSwitchCard::stepTheModel();

   // check that switch 1 is still open
    CPPUNIT_ASSERT(!tArticle->mSwitch[0].isClosed());

    // check the input undervolt flag is set
    CPPUNIT_ASSERT(tArticle->mTlmUndervoltTrip[0]);

    // set the undervolt clear flag
    tArticle->mClearUndervoltTrip = true;

    // step the model
    UtSwitchCard::stepTheModel();

    // check the input undervolt flag is not set
    CPPUNIT_ASSERT(!tArticle->mTlmUndervoltTrip[0]);

    std::cout << "... Pass";
    std::cout << "\n -----------------------------------------------------------------------------";
}

void UtSwitchCard::stepTheModel() {
    tArticle->step(0.1);  // for minorStep == 1 run regular step

    for (int i = 1; i<=10; i++) {  // worst case, run until EPS minor step limit
        tArticle->minorStep(0.1, i);
        tArticle->confirmSolutionAcceptable(1, i);
    }

}
