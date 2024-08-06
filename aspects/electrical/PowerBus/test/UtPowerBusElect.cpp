/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    (
    (aspects/electrical/PowerBus/PowerBusElect.o)
    (software/exceptions/TsInitializationException.o)
    (math/UnitConversion.o)
    (aspects/electrical/PowerBus/PowerBusElect.o)
    (aspects/electrical/UserLoad/UserLoadBase.o)
    (aspects/electrical/UserLoad/ResistiveLoad.o)
    (aspects/electrical/UserLoad/ConstantPowerLoad.o)

     )

 PROGRAMMERS:
 - ((Carlo Bocatto) (L-3) (Initial) (2013-05))
***************************************************************************************************/
#include "software/exceptions/TsConfigurationException.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "aspects/electrical/PowerBus/PowerBusElect.hh"
#include "math/UnitConversion.hh"
#include "UtPowerBusElect.hh"
#include <math.h>

EpsTestLoads::EpsTestLoads()
:
//test loads for ideal PowerBus
testLoad1(),
testLoad2(),
testLoad3()
{
}


EpsTestLoads::~EpsTestLoads()
{
}


EpsTestLoadsConfig::EpsTestLoadsConfig():
        testLoad1Config(ResistiveLoadConfigData("TestLoad1",RESISTIVE_LOAD,-120,25,100)),
        testLoad2Config(ConstantPowerLoadConfigData("TestLoad2",CONSTANT_POWER_LOAD,-120,576,5)),
        testLoad3Config(ResistiveLoadConfigData("TestLoad3",RESISTIVE_LOAD,-120,25,100))
{
}


EpsTestLoadsConfig::~EpsTestLoadsConfig()
{
}

EpsTestLoadsInput::EpsTestLoadsInput():
        testLoad1Input(ResistiveLoadInputData(0,0.0,1)),
        testLoad2Input(ConstantPowerLoadInputData(0,0.0,1)),
        testLoad3Input(ResistiveLoadInputData(0,0.0,1))
{
}


EpsTestLoadsInput::~EpsTestLoadsInput()
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtPowerBusElect class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtPowerBusElect::UtPowerBusElect() :
        tObject(),
        //tCfObject(),
        tConfigData(),
        tInputData(),
        tObjectPlug0("UtPowerBusElect_tObjectPlug0"),
        testPB1Socket(),
        testPB1SocketConfig("UtPowerBusElect.testPB1Socket", &tNodeList,200, true),
        testPB1SocketInput(false, 0.0),
        tLoads(),
        tLoadsConfig(),
        tLoadsInput(),
        tNetworkLoads(),
        tLinks(),
        tNodeList(),
        tPortMap(),
        tNumPorts(),
        tName(),
        tNumLoads(),
        tMalfBlockageFlag(),
        tMalfBlockageValue(),
        tMaxConductance(),
        tMinConductance(),
        tTolerance(0.01),
        tTimeStep()

{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtPowerBusElect class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtPowerBusElect::~UtPowerBusElect()
{
//do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPowerBusElect::setUp()
{
    tName = "testPowerBus";
    tNumLoads = 3;
    tNumPorts = 1;

    tPortMap = new int [tNumPorts];

    tPortMap[0] = 0;

    tNodeList.mNodes = tNodes;
    tNodeList.mNumNodes = testNumNodes;

    tMaxConductance = 1.3E5;
    tMinConductance = 1;

    //create test instance
    tObject = new FriendlyPowerBus();

    //Config Data for the Power Bus
    tConfigData = new PowerBusElectConfigData (tName,
                                            &tNodeList,
                                            tNumLoads,
                                            &tObjectPlug0,
                                            true);

    //Input Data afor the Power Bus
    tInputData = new PowerBusElectInputData (tMalfBlockageFlag,
                                            tMalfBlockageValue,
                                            tMaxConductance,
                                            tMinConductance,
                                            TestPoweBusSocketList::testPB1Socket);

    tObjectPlug0.addSocket(testPB1Socket);

    //setup loads
    tLoads.testLoad1.initialize(tLoadsConfig.testLoad1Config,tLoadsInput.testLoad1Input,tNetworkLoads,0,0);
    tLoads.testLoad2.initialize(tLoadsConfig.testLoad2Config,tLoadsInput.testLoad2Input,tNetworkLoads,0,1);
    tLoads.testLoad3.initialize(tLoadsConfig.testLoad3Config,tLoadsInput.testLoad3Input,tNetworkLoads,0,2);

    int cardId = 0;
    int loadSwitchId = 0;

    //Assignment process for good load data
    for(unsigned int i=0; i < tNetworkLoads.size(); i++) {
        cardId = tNetworkLoads[i]->getCardId();
        loadSwitchId = tNetworkLoads[i]->getLoadSwitchId();
        tBusLoads[cardId][loadSwitchId] = tNetworkLoads[i];
    }

    // Attempts to initialize data throws errors if there are any problems with initialize
    try {
        testPB1Socket.initialize(testPB1SocketConfig, testPB1SocketInput, tLinks, tPortMap[0], 2);
        tObject->initialize(*tConfigData, *tInputData, tLinks, tBusLoads[0], tPortMap);
    }
    catch (TsInitializationException &e) {
        std::cout << "UT_PowerBus Setup method initialization exception -- failed" << e.getMessage() << std::endl;
    }
    catch (...) {
        std::cout << "EVERYTHING ELSE" << std::endl;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPowerBusElect::tearDown()
{
    delete tInputData;
    delete tConfigData;
    delete tObject;
    delete [] tPortMap;

}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of Power Bus Elect link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPowerBusElect::testCDName()
{
    std::cout<<"............Config Data Tests..........................\n";

        ///set expected name
        std::string expectedName = "testPowerBus";
        //test name of power bus is equal to expected name.
        CPPUNIT_ASSERT_EQUAL(expectedName,tConfigData->mName);
        std::cout << "...........................................Passed \n";
}
void UtPowerBusElect::testCDNumLoads()
{
        //set expected number of loads
        int expectedLoads = 3;
        //test numLoads is equal to the expected number
        CPPUNIT_ASSERT_EQUAL(expectedLoads, tConfigData->mNumLoads);
        std::cout << "...........................................Passed \n";
}

void UtPowerBusElect::testCDCopyConstructor()
{
    //change config data
    tConfigData->mName = "copiedPowerBus";
    tConfigData->mNumLoads = 2;

    //copy Data into new object
    PowerBusElectConfigData newConfigData (*tConfigData);

    //test data copied correctly
    CPPUNIT_ASSERT_EQUAL(tConfigData->mName,newConfigData.mName);

    CPPUNIT_ASSERT_EQUAL(tConfigData->mNumLoads,newConfigData.mNumLoads);

    std::cout << "...........................................Passed \n";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of Power Bus Elect link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////

void  UtPowerBusElect::testIDBlockFlag()
{
    std::cout<<"............Input Data Tests..........................\n";
    //set expected BlockageFlag
    bool expectedFlag = false;
    CPPUNIT_ASSERT_EQUAL(expectedFlag,tInputData->mMalfBlockageFlag);
    std::cout << "...........................................Passed \n";
}
void  UtPowerBusElect::testIDBlockValue()
{
    //set expected BlockageValue
    double expectedValue = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedValue,tInputData->mMalfBlockageValue,tTolerance);
    std::cout << "...........................................Passed \n";
}
void  UtPowerBusElect::testIDMaxConductance()
{
    //set expected maxConductance
    double expectedConductance = 1.3E5;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedConductance,tInputData->mMaxConductance,tTolerance);
    std::cout << "...........................................Passed \n";

}
void UtPowerBusElect::testIDCopyConstructor()
{
    //change input data
    tInputData->mMalfBlockageFlag = true;
    tInputData->mMalfBlockageValue = 8.0;
    tInputData->mMaxConductance = 1.2E4;
    tInputData->mMinConductance = 1;

    //copy input data into new object
    PowerBusElectInputData newInputData(*tInputData);

    //test data copied correctly
    CPPUNIT_ASSERT_EQUAL(tInputData->mMalfBlockageFlag,newInputData.mMalfBlockageFlag);

    CPPUNIT_ASSERT_EQUAL(tInputData->mMalfBlockageValue,newInputData.mMalfBlockageValue);

    CPPUNIT_ASSERT_EQUAL(tInputData->mMaxConductance,newInputData.mMaxConductance);

    CPPUNIT_ASSERT_EQUAL(tInputData->mMinConductance,newInputData.mMinConductance);


    std::cout << "...........................................Passed \n";

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for initialize method of Power Bus Elect link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPowerBusElect::testGoodDataInitialize()
{
    std::cout<<"............Initialization Tests..........................\n";


    CPPUNIT_ASSERT_NO_THROW(tObject->initialize(*tConfigData, *tInputData, tLinks, tBusLoads[0], tPortMap));
    std::cout << "............................................Passed \n";

}

void UtPowerBusElect::testZeroMinConductance()
{
    //set MinConductance to 0;
    tInputData->mMinConductance = 0;
    double expectedMinConductance = 1.0E-8;

    tObject->initialize(*tConfigData, *tInputData, tLinks, tBusLoads[0], tPortMap);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMinConductance,tInputData->mMinConductance,tTolerance);

    std::cout << "............................................Passed \n";
}
void UtPowerBusElect::testBadNumLoadsInitialize()
{
    //test to ensure that Initialization Exception will be thrown if numLoads = 0
    tConfigData->mNumLoads = 0;
    CPPUNIT_ASSERT_THROW(tObject->initialize(*tConfigData, *tInputData, tLinks, tBusLoads[0], tPortMap),TsInitializationException);
    std::cout << "...........................................Passed \n";
}
void UtPowerBusElect::testBadMaxConductanceInitialize()
{
    //test to ensure that Initialization Exception will be thrown if max conductance < 0
    tInputData->mMaxConductance = 0;
    CPPUNIT_ASSERT_THROW(tObject->initialize(*tConfigData, *tInputData, tLinks, tBusLoads[0], tPortMap),TsInitializationException);
    std::cout << "...........................................Passed \n";
}
void UtPowerBusElect::testInitializeAddedVector()
{
    int expectedValue = 1;
    PowerBusElect* busA = new PowerBusElect();
    std::vector<GunnsBasicLink*> emptyVector;

    tObject->initialize(*tConfigData, *tInputData, emptyVector, tBusLoads[0], tPortMap);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedValue,emptyVector.size(),tTolerance);
    std::cout << "...........................................Passed \n";

    delete busA;
}

void UtPowerBusElect::testNullLoadsPointer()
{
    //set a null user load base
    UserLoadBase** nullDblPtr=0;
    CPPUNIT_ASSERT_THROW(tObject->initialize(*tConfigData, *tInputData, tLinks, nullDblPtr, tPortMap),TsInitializationException);
    std::cout << "...........................................Passed \n";
}

void UtPowerBusElect::testInitFlag()
{
    bool expectedFlag = true;

    CPPUNIT_ASSERT_EQUAL(expectedFlag,tObject->mInitFlag);

    std::cout << "...........................................Passed \n";

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for default construction method of Power Bus Elect link model .
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPowerBusElect::testDefConstBusVoltage()
{
    std::cout<<"............Default Construction Tests..........................\n";


    //test initial bus voltage
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,tObject->mBusVoltage,tTolerance);
    std::cout << "...........................................Passed \n";
}
void UtPowerBusElect::testDefConstInputCurrent()
{
    //test input current
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,tObject->mFlux,tTolerance);
    std::cout << "...........................................Passed \n";
}
void UtPowerBusElect::testDefConstResLoadConductance()
{
//test Resistive Load Conductance
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,tObject->mResLoadsConductance,tTolerance);
    std::cout << "...........................................Passed \n";
}
void UtPowerBusElect::testDefConstPwrLoadConductance()
{
    //test Constant Power Load Conductance
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,tObject->mCPowerLoadsConductance,tTolerance);
    std::cout << "...........................................Passed \n";
}
void UtPowerBusElect::testDefConstActiveResLoadConductance()
{
    //test active ResitiveLoad Conductance
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,tObject->mActiveResLoadsConductance,tTolerance);
    std::cout << "...........................................Passed \n";
}
void UtPowerBusElect::testDefConstActivePwrLoadconductance()
{
    //test active Constant PowerLoad Conductance
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,tObject->mActiveCPowerLoadsConductance,tTolerance);
    std::cout << "...........................................Passed \n";
}

void UtPowerBusElect::testDefConstNonLinear()
{
    //test Is NonLinear
    CPPUNIT_ASSERT_EQUAL(true,(tObject->mNonLinear));
    std::cout << "...........................................Passed \n";

}
////////////////////////////////////////////////////////////////////////////////////////////////////////
///// @details  Tests  to ensure that power bus loads > 0.
////////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPowerBusElect::testZeroLoads()
{
        //set numLoads = 0
    tConfigData->mNumLoads = 0;
    //test to ensure error message is thrown when numLoads = 0
    CPPUNIT_ASSERT_THROW(tObject->initialize(*tConfigData, *tInputData, tLinks, tBusLoads[0], tPortMap), TsInitializationException);
    std::cout << "...........................................Passed \n";
}
void UtPowerBusElect::testOneLoad()
{
    //set numLoads = 1
    tConfigData->mNumLoads=1;
    //test to ensure no error message is thrown when numLoads = 1
    CPPUNIT_ASSERT_NO_THROW(tObject->initialize(*tConfigData, *tInputData, tLinks, tBusLoads[0], tPortMap));
    std::cout << "...........................................Passed \n";
}
void UtPowerBusElect::testLargeLoadNumber()
{
    //set numLoads = 10,000
    tConfigData->mNumLoads=10,000;
    CPPUNIT_ASSERT_NO_THROW(tObject->initialize(*tConfigData, *tInputData, tLinks, tBusLoads[0], tPortMap));
    std::cout << "...........................................Passed \n";
}
void UtPowerBusElect::testNegativeNumberOfLoads()
{
    //setNumLoads = -5
    tConfigData->mNumLoads = -5;

    //test that error message is thrown when numLoads<0
    CPPUNIT_ASSERT_THROW(tObject->initialize(*tConfigData, *tInputData, tLinks, tBusLoads[0], tPortMap), TsInitializationException);
    std::cout << "...........................................Passed \n";
}


//Update tests
void UtPowerBusElect::testSteppedBusVoltage()
{
    std::cout<<"............Stepped Model Tests..........................\n";
    //set voltage at port 0 as the input voltage to the bus
    double expectedInputVoltage = 120;
    tObject->mPotentialVector[0] = expectedInputVoltage;

    //step the power bus
    UtPowerBusElect::stepTheModel();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedInputVoltage,tObject->mBusVoltage,tTolerance);
    std::cout << "...........................................Passed \n";
}
void UtPowerBusElect::testOverMaxResLoadConductance(){
    //set MaxConductance
    tObject->mMaxConductance = .004; // 1/25 = .04

    //set Max Res Load Conductance


    UtPowerBusElect::stepTheModel();

    //test to ensure the Active Res Load is set to Max Res Load Conductance
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tObject->mActiveResLoadsConductance,tObject->mMaxConductance,tTolerance);
    std::cout << "...........................................Passed \n";
}
void UtPowerBusElect::testUnderMinResLoadConductamce(){
    //set Max Res Load Conductance
    tObject->mMinConductance = 3;

    UtPowerBusElect::stepTheModel();

    //test to ensure the Active Res Load is set to Max Res Load Conductance
    // 0.0 because of logic found in line PowerBusElect.cpp 366
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,tObject->mActiveResLoadsConductance,tTolerance);
    std::cout << "...........................................Passed \n";
}
void UtPowerBusElect::testOverMaxPwrConductance(){
    //set Max Conductance
    tObject->mMaxConductance = 1.4E5;
    //set Max Load Conductance
    tObject->mCPowerLoadsConductance = 1200000000;

    UtPowerBusElect::stepTheModel();

    //test to ensure the Active Res Load is set to Max Res Load Conductance
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tObject->mActiveCPowerLoadsConductance,tObject->mMaxConductance,tTolerance);
    std::cout << "...........................................Passed \n";

}
void UtPowerBusElect::testBusLoadsHaveExpectedValues()
{
    //set default input powerBusvoltage
    double expectedInputVoltage = 120;
    tObject->mPotentialVector[0] = expectedInputVoltage;

    //declare expected values
    double expectedResistance1 = 25;
    double expectedResistance3 = 25;
    double expectedPower = 576;

    UtPowerBusElect::stepTheModel();

    //test resistance of Loads 1
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedResistance1,tLoads.testLoad1.getResistance(),tTolerance);
    //test resistance of Loads 3
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedResistance3,tLoads.testLoad3.getResistance(),tTolerance);

    //test power of Load 2
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower,tLoads.testLoad2.getPower(),tTolerance);

    std::cout << "...........................................Passed \n";
}
void UtPowerBusElect::testSteppedLoadVoltage()
{
    //set voltage at port 0 as the input voltage to the bus
    double expectedVoltage = 120;
    tObject->mPotentialVector[0] = expectedVoltage;

    //step the power bus
    UtPowerBusElect::stepTheModel();

    //Load 1  = Resistive Load
    //get the load power
    double load1Power=tLoads.testLoad1.getPower();
    //get load resistance
    double load1Resistance  = tLoads.testLoad1.getResistance();
    //calculate load voltage
    double load1Voltage = sqrt((load1Power*load1Resistance));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVoltage,load1Voltage,tTolerance);

    //Load 2  = Constant Power Load
    //get the load power
    double load2Power=tLoads.testLoad2.getPower();
    //get load resistance
    double load2Resistance  = tLoads.testLoad2.getResistance();
    //calculate load voltage
    double load2Voltage = sqrt((load2Power*load2Resistance));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVoltage,load2Voltage,tTolerance);

    //Load 3 = Resistive Load
    //get the load power
    double load3Power=tLoads.testLoad3.getPower();
    //get load resistance
    double load3Resistance  = tLoads.testLoad3.getResistance();
    //calculate load voltage
    double load3Voltage = sqrt((load3Power*load3Resistance));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVoltage,load3Voltage,tTolerance);


    std::cout << "...........................................Passed \n";
}

void UtPowerBusElect::testSteppedBusInputCurrentFlowsToLoadsCorrectly()
{
    //set input voltage
    double expectedVoltage = 120;
    tObject->mPotentialVector[0] = expectedVoltage;

    //set input current
    double expectedCurrent = 14.4;
    tObject->mFlux = expectedCurrent;

    double expectedIndividaulCurrents = expectedCurrent/3;

    UtPowerBusElect::stepTheModel();


    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIndividaulCurrents,tLoads.testLoad1.getCurrent(),tTolerance);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIndividaulCurrents,tLoads.testLoad2.getCurrent(),tTolerance);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIndividaulCurrents,tLoads.testLoad3.getCurrent(),tTolerance);

    //total currents to ensure that currents add up to expected value

    double totaledLoadCurrents = tLoads.testLoad1.getCurrent()+tLoads.testLoad1.getCurrent()+tLoads.testLoad1.getCurrent();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCurrent,totaledLoadCurrents,tTolerance);

    std::cout << "...........................................Passed \n";
}

void UtPowerBusElect::testPositveFluxComputeFlows(){
    tObject->mPotentialVector[0] = 120.0;
    UtPowerBusElect::stepTheModel();
    tObject->computeFlows(0.1);

    //test input current
    CPPUNIT_ASSERT_DOUBLES_EQUAL(14.4,tObject->mFlux,tTolerance);

    //test Potential Drop
    CPPUNIT_ASSERT_DOUBLES_EQUAL(120,tObject->mPotentialDrop,tTolerance);
    CPPUNIT_ASSERT_EQUAL(false, tObject->mSealed);

    std::cout << "...........................................Passed \n";
}

void UtPowerBusElect::testSteppedIsNonLinear(){
    //this test is here mostly for line coverage

    CPPUNIT_ASSERT_EQUAL(true, tObject->isNonLinear());

    std::cout << "...........................................Passed \n";

}

////////////////////////////////////////////////////////////////////////////////////////////////////////
///// @details  Test to ensure the power bus step method works properly
////////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPowerBusElect::stepTheModel()
{
    tObject->step(0.1);    //for minor step == 1 run regular step

    for(int i=1;i<=10;i++)
    {
        tObject->minorStep(0.1,i);


    }
}

void UtPowerBusElect::testDisconnectionRequestToSocket(){
    tObject->mPotentialVector[0] = 120.0;

    // disconnect the plug from the socket
    tObject->mPlug[0]->setDisconnectionRequest(0);


    tObject->step(0.1);
    tObject->computeFlows(0.1);

    CPPUNIT_ASSERT_EQUAL(true, tObject->mSealed);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tObject->mAdmittanceMatrix[0],tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tObject->mTotalCPowerLoadsPower,tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tObject->mResLoadsConductance,tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(120.0, tObject->mBusVoltage,tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tObject->mFlux,tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tObject->mPower,tTolerance);


    std::cout << "...........................................Passed \n";
}


void UtPowerBusElect::testConnectionRequestToSocket(){
    tObject->mPotentialVector[0] = 120.0;

    // socket is already connected, first disconnect
    // disconnect the plug from the socket
    tObject->mPlug[0]->setDisconnectionRequest(0);

    tObject->step(0.1);
    tObject->computeFlows(0.1);

    CPPUNIT_ASSERT_EQUAL(true, tObject->mSealed);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tObject->mAdmittanceMatrix[0],tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tObject->mTotalCPowerLoadsPower,tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tObject->mResLoadsConductance,tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tObject->mFlux,tTolerance);

    tObject->mPlug[0]->setConnectionRequest(0);
    tObject->step(0.1);
    tObject->computeFlows(0.1);
    CPPUNIT_ASSERT_EQUAL(false, tObject->mSealed);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.04, tObject->mCPowerLoadsConductance,tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(576, tObject->mTotalCPowerLoadsPower,tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.08, tObject->mResLoadsConductance,tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.12, tObject->mAdmittanceMatrix[0],tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(14.4, tObject->mFlux,tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1728.0, tObject->mPower,tTolerance);

    std::cout << "...........................................Passed \n";
}
