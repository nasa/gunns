/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
(
   (aspects/fluid/conductor/GunnsFluidEqConductor.o)
)
***************************************************************************************************/

#include "software/exceptions/TsOutOfBoundsException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidEqConductor.hh"

/// @details  Test identification number.
int UtGunnsFluidEqConductor::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsFluidEqConductor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidEqConductor::UtGunnsFluidEqConductor()
    :
    tArticle(),
    tNodes(),
    tFluidProperties(),
    tLocalConfig(),
    tFluidInput1(),
    tFluidInput2(),
    tLinkName(),
    tUseNetworkCapacitance(),
    tConfigData(),
    tNodeList(),
    tLinks(),
    tInputData(),
    tPort0(),
    tPort1(),
    tTimeStep()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsFluidEqConductor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidEqConductor::~UtGunnsFluidEqConductor()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidEqConductor::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete tFluidInput2;
    delete tFluidInput1;
    delete tLocalConfig;
    delete tFluidProperties;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidEqConductor::setUp()
{
    tLinkName              = "Test Fluid Equivalent Conductor";
    tNodeList.mNumNodes    = 3;
    tNodeList.mNodes       = tNodes;
    tUseNetworkCapacitance = true;
    tPort0                 = 0;
    tPort1                 = 1;
    tTimeStep              = 0.1;

    /// - Setup some fluid nodes
    tFluidProperties = new DefinedFluidProperties();
    FluidProperties::FluidType types[2] = {FluidProperties::GUNNS_N2, FluidProperties::GUNNS_O2};
    tLocalConfig = new PolyFluidConfigData(tFluidProperties, types, 2);
    double fractions[2] = {0.5, 0.5};
    tFluidInput1 = new PolyFluidInputData(283.15,                   //temperature
                                          700.728,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          fractions);               //massFractions
    tFluidInput2 = new PolyFluidInputData(283.15,                   //temperature
                                          689.728,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          fractions);               //massFractions

    /// - Have to initialize the nodes with the fluid configs (normally done by GUNNS)
    tNodes[0].initialize("UtTestNode0", tLocalConfig);
    tNodes[1].initialize("UtTestNode1", tLocalConfig);
    tNodes[2].initialize("UtTestNode2", tLocalConfig);

    tNodes[0].getContent()->initialize(*tLocalConfig, *tFluidInput1);
    tNodes[1].getContent()->initialize(*tLocalConfig, *tFluidInput2);
    tNodes[2].getContent()->initialize(*tLocalConfig, *tFluidInput2);

    tNodes[0].setPotential(tFluidInput1->mPressure);
    tNodes[1].setPotential(tFluidInput2->mPressure);

    tNodes[0].resetFlows();
    tNodes[1].resetFlows();
    tNodes[2].resetFlows();

    /// - Define nominal configuration data
    tConfigData = new GunnsFluidEqConductorConfigData(tLinkName, &tNodeList, tUseNetworkCapacitance);

    /// - Define nominal input data
    tInputData = new GunnsFluidEqConductorInputData(false, 0.0);

    tArticle = new FriendlyGunnsFluidEqConductor;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidEqConductor::testConfig()
{
    UT_RESULT_FIRST;

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tLinkName              == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                 == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tUseNetworkCapacitance == tConfigData->mUseNetworkCapacitance);

    /// - Check default config construction
    GunnsFluidEqConductorConfigData defaultConfig;
    CPPUNIT_ASSERT(""                     == defaultConfig.mName);
    CPPUNIT_ASSERT(0                      == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(false                  == defaultConfig.mUseNetworkCapacitance);

    /// - Check copy config construction
    GunnsFluidEqConductorConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tLinkName              == copyConfig.mName);
    CPPUNIT_ASSERT(tNodes                 == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tUseNetworkCapacitance == copyConfig.mUseNetworkCapacitance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidEqConductor::testInput()
{
    UT_RESULT;

    /// - Check nominal config construction
    CPPUNIT_ASSERT(false == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0   == tInputData->mMalfBlockageValue);

    /// - Check default config construction
    GunnsFluidEqConductorInputData defaultInput;
    CPPUNIT_ASSERT(false == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0   == defaultInput.mMalfBlockageValue);

    /// - Check copy config construction
    GunnsFluidEqConductorInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageValue == copyInput.mMalfBlockageValue);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidEqConductor::testDefaultConstruction()
{
    UT_RESULT;

    /// @test class members
    CPPUNIT_ASSERT(false == tArticle->mUseNetworkCapacitance);
    CPPUNIT_ASSERT(0.0   == tArticle->mPort0DemandFlux);
    CPPUNIT_ASSERT(0.0   == tArticle->mPort0DemandTemperature);
    CPPUNIT_ASSERT(0.0   == tArticle->mPort1DemandFlux);
    CPPUNIT_ASSERT(0.0   == tArticle->mPort1DemandTemperature);
    CPPUNIT_ASSERT(0.0   == tArticle->mPort0SupplyCapacitance);
    CPPUNIT_ASSERT(0.0   == tArticle->mPort0SupplyPressure);
    CPPUNIT_ASSERT(0.0   == tArticle->mPort0SupplyTemperature);
    CPPUNIT_ASSERT(0     == tArticle->mPort0SupplyMassFractions),
    CPPUNIT_ASSERT(0.0   == tArticle->mPort1SupplyCapacitance);
    CPPUNIT_ASSERT(0.0   == tArticle->mPort1SupplyPressure);
    CPPUNIT_ASSERT(0.0   == tArticle->mPort1SupplyTemperature);
    CPPUNIT_ASSERT(0     == tArticle->mPort1SupplyMassFractions),
    CPPUNIT_ASSERT(0.0   == tArticle->mEquivalentConductance);
    CPPUNIT_ASSERT(0.0   == tArticle->mPort0LeakConductance);
    CPPUNIT_ASSERT(0.0   == tArticle->mPort1LeakConductance);
    CPPUNIT_ASSERT(0.0   == tArticle->mLastPotentialDrop);

    /// @test init flag
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    /// @test new/delete for code coverage
    GunnsFluidEqConductor* article = new GunnsFluidEqConductor();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidEqConductor::testNominalInitialization()
{
    UT_RESULT;

    /// - Default construct and initialize (with nominal data) a test article
    FriendlyGunnsFluidEqConductor article;
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Test class members.
    CPPUNIT_ASSERT(tUseNetworkCapacitance == article.mUseNetworkCapacitance);
    CPPUNIT_ASSERT(0.0 == tArticle->mPort0DemandFlux);
    CPPUNIT_ASSERT(0.0 == tArticle->mPort0DemandTemperature);
    CPPUNIT_ASSERT(0.0 == tArticle->mPort1DemandFlux);
    CPPUNIT_ASSERT(0.0 == tArticle->mPort1DemandTemperature);
    CPPUNIT_ASSERT(0.0 == tArticle->mPort0SupplyPressure);
    CPPUNIT_ASSERT(0.0 == tArticle->mPort0SupplyTemperature);
    CPPUNIT_ASSERT(0.0 == tArticle->mPort1SupplyPressure);
    CPPUNIT_ASSERT(0.0 == tArticle->mPort1SupplyTemperature);
    CPPUNIT_ASSERT(0.0 == tArticle->mEquivalentConductance);
    CPPUNIT_ASSERT(0.0 == tArticle->mPort0LeakConductance);
    CPPUNIT_ASSERT(0.0 == tArticle->mPort1LeakConductance);
    CPPUNIT_ASSERT(0.0 == tArticle->mLastPotentialDrop);

    /// - Test internal fluid & supply mass fraction arrays.
    CPPUNIT_ASSERT(article.mInternalFluid);
    CPPUNIT_ASSERT(article.mPort0SupplyMassFractions);
    CPPUNIT_ASSERT(article.mPort1SupplyMassFractions);
    CPPUNIT_ASSERT(0.0 == article.mPort0SupplyMassFractions[0]);
    CPPUNIT_ASSERT(0.0 == article.mPort0SupplyMassFractions[1]);
    CPPUNIT_ASSERT(0.0 == article.mPort1SupplyMassFractions[0]);
    CPPUNIT_ASSERT(0.0 == article.mPort1SupplyMassFractions[1]);

    /// - Test init flag
    CPPUNIT_ASSERT(article.mInitFlag);

    /// - Verify a double init works
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    CPPUNIT_ASSERT(article.mInitFlag);

    /// - Verify functionality of restartModel
    tArticle->mEquivalentConductance = 1.0;
    tArticle->mPort0LeakConductance  = 1.0;
    tArticle->mPort1LeakConductance  = 1.0;

    tArticle->restartModel();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,tArticle->mEquivalentConductance,DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,tArticle->mPort0LeakConductance,DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,tArticle->mPort1LeakConductance,DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for initialization exceptions on invalid properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidEqConductor::testInitializationExceptions()
{
    UT_RESULT;

    /// - There are no exceptions to check.

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the processInputs and processOutputs methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidEqConductor::testIoMethods()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Verify the write method grabs the supply node properties for write to the sim bus.
    tNodes[0].setNetworkCapacitance(1.0);
    tNodes[1].setNetworkCapacitance(2.0);
    tArticle->processOutputs();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getNetworkCapacitance(),
            tArticle->mPort0SupplyCapacitance, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getPotential(),
            tArticle->mPort0SupplyPressure, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getTemperature(),
            tArticle->mPort0SupplyTemperature, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getMassFraction(0),
            tArticle->mPort0SupplyMassFractions[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getMassFraction(1),
            tArticle->mPort0SupplyMassFractions[1], 0.0);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[1].getNetworkCapacitance(),
            tArticle->mPort1SupplyCapacitance, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[1].getPotential(),
            tArticle->mPort1SupplyPressure, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[1].getContent()->getTemperature(),
            tArticle->mPort1SupplyTemperature, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[1].getContent()->getMassFraction(0),
            tArticle->mPort1SupplyMassFractions[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[1].getContent()->getMassFraction(1),
            tArticle->mPort1SupplyMassFractions[1], 0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the step method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidEqConductor::testStep()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Step with zero potentials and verify no divide-by zero's.
    tArticle->step(tTimeStep);

    /// - Set last-pass values.
    double expectedPotentialDrop  = tNodes[0].getPotential() - tNodes[1].getPotential();
    tArticle->mLastPotentialDrop  = expectedPotentialDrop;

    /// - Verify a normal forward-flow case.
    tArticle->mPotentialVector[0] = tNodes[0].getPotential();
    tArticle->mPotentialVector[1] = tNodes[1].getPotential();
    tArticle->mPort0DemandFlux    =  1.0;
    tArticle->mPort1DemandFlux    = -1.0;
    double expectedEquivC         = 1.0 / expectedPotentialDrop;
    double expectedLeakC0         = 0.0;
    double expectedLeakC1         = 0.0;
    double request                = GunnsFluidEqConductor::mNetworkCapacitanceFlux;

    tArticle->step(tTimeStep);
    verifyStepOutputs(expectedEquivC, expectedLeakC0, expectedLeakC1, true);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(request, tNodes[0].getNetworkCapacitanceRequest(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(request, tNodes[1].getNetworkCapacitanceRequest(), DBL_EPSILON);

    /// - Verify a normal reverse-flow case, and with the use network capacitance flag off.
    tArticle->mPotentialVector[0] = tNodes[1].getPotential();
    tArticle->mPotentialVector[1] = tNodes[0].getPotential();
    tArticle->mPort0DemandFlux    = -1.0;
    tArticle->mPort1DemandFlux    =  1.0;
    tArticle->mAdmittanceUpdate   =  false;
    tArticle->mUseNetworkCapacitance =  false;
    tNodes[0].setNetworkCapacitanceRequest(0.0);
    tNodes[1].setNetworkCapacitanceRequest(0.0);
    expectedEquivC                = 1.0 / expectedPotentialDrop;
    expectedLeakC0                = 0.0;
    expectedLeakC1                = 0.0;

    tArticle->step(tTimeStep);
    verifyStepOutputs(expectedEquivC, expectedLeakC0, expectedLeakC1, false);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[0].getNetworkCapacitanceRequest(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[1].getNetworkCapacitanceRequest(), DBL_EPSILON);

    /// - Verify forward flow with a leak.
    tArticle->mPotentialVector[0] = tNodes[0].getPotential();
    tArticle->mPotentialVector[1] = tNodes[1].getPotential();
    tArticle->mPort0DemandFlux    =  1.0;
    tArticle->mPort1DemandFlux    = -0.5;
    tArticle->mAdmittanceUpdate   =  false;
    expectedEquivC                = 0.75 / expectedPotentialDrop;
    expectedLeakC0                = 0.0;
    expectedLeakC1                = 0.5 / tNodes[1].getPotential();

    tArticle->step(tTimeStep);
    verifyStepOutputs(expectedEquivC, expectedLeakC0, expectedLeakC1, true);

    /// - Verify reverse flow with a leak.
    tArticle->mPotentialVector[0] = tNodes[1].getPotential();
    tArticle->mPotentialVector[1] = tNodes[0].getPotential();
    tArticle->mPort0DemandFlux    = -0.5;
    tArticle->mPort1DemandFlux    =  1.0;
    tArticle->mAdmittanceUpdate   =  false;
    expectedEquivC                = 0.75 / expectedPotentialDrop;
    expectedLeakC0                = 0.5 / tNodes[1].getPotential();
    expectedLeakC1                = 0.0;

    tArticle->step(tTimeStep);
    verifyStepOutputs(expectedEquivC, expectedLeakC0, expectedLeakC1, true);

    /// - Verify blockage with a port 0 leak.
    tArticle->mPotentialVector[0] = tNodes[0].getPotential();
    tArticle->mPotentialVector[1] = tNodes[1].getPotential();
    tArticle->mPort0DemandFlux    =  1.0;
    tArticle->mPort1DemandFlux    =  0.0;
    tArticle->mAdmittanceUpdate   =  false;
    expectedEquivC                = 0.0;
    expectedLeakC0                = 1.0 / tNodes[0].getPotential();
    expectedLeakC1                = 0.0;

    tArticle->step(tTimeStep);
    verifyStepOutputs(expectedEquivC, expectedLeakC0, expectedLeakC1, true);

    /// - Verify blockage with a port 1 leak.
    tArticle->mPotentialVector[0] = tNodes[0].getPotential();
    tArticle->mPotentialVector[1] = tNodes[1].getPotential();
    tArticle->mPort0DemandFlux    =  0.0;
    tArticle->mPort1DemandFlux    =  1.0;
    tArticle->mAdmittanceUpdate   =  false;
    expectedEquivC                = 0.0;
    expectedLeakC0                = 0.0;
    expectedLeakC1                = 1.0 / tNodes[1].getPotential();

    tArticle->step(tTimeStep);
    verifyStepOutputs(expectedEquivC, expectedLeakC0, expectedLeakC1, true);

    /// - Verify blockage with leaks on both ports.
    tArticle->mPotentialVector[0] = tNodes[0].getPotential();
    tArticle->mPotentialVector[1] = tNodes[1].getPotential();
    tArticle->mPort0DemandFlux    =  0.5;
    tArticle->mPort1DemandFlux    =  1.0;
    tArticle->mAdmittanceUpdate   =  false;
    expectedEquivC                = 0.0;
    expectedLeakC0                = 0.5 / tNodes[0].getPotential();
    expectedLeakC1                = 1.0 / tNodes[1].getPotential();

    tArticle->step(tTimeStep);
    verifyStepOutputs(expectedEquivC, expectedLeakC0, expectedLeakC1, true);

    /// - Set the input data for flow source in the external network and verify an exception is
    ///   thrown.
    tArticle->mPotentialVector[0] = tNodes[0].getPotential();
    tArticle->mPotentialVector[1] = tNodes[1].getPotential();
    tArticle->mPort0DemandFlux    = -0.5;
    tArticle->mPort1DemandFlux    = -1.0;

    //TODO restore when the flow source case is fixed.
//    CPPUNIT_ASSERT_THROW(tArticle->step(tTimeStep), TsOutOfBoundsException);
    tArticle->step(tTimeStep);
    verifyStepOutputs(0.0, 0.0, 0.0, true);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidEqConductor::testComputeFlows()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Verify the outputs of a forward flux between the link ports with a leak.
    tArticle->mPotentialVector[0]     = tNodes[0].getPotential();
    tArticle->mPotentialVector[1]     = tNodes[1].getPotential();
    double expectedPotentialDrop      = tNodes[0].getPotential() - tNodes[1].getPotential();
    tArticle->mPotentialDrop          = 9.0;
    tArticle->mEquivalentConductance  = 1.0 / expectedPotentialDrop;
    tArticle->mPort0LeakConductance   = 0.0;
    tArticle->mPort1LeakConductance   = 0.1 * tArticle->mEquivalentConductance;
    tArticle->mPort0DemandTemperature = 290.0;
    tArticle->mPort1DemandTemperature = 300.0;

    tArticle->computeFlows(tTimeStep);
    tArticle->transportFlows(tTimeStep);

    /// - Verify the computeFlows method outputs
    double expectedFlowRate      = 1.0 * tNodes[0].getOutflow()->getMWeight();
    double expectedVolFlowRate   = expectedFlowRate / tNodes[0].getOutflow()->getDensity();
    double expectedPower         = -expectedVolFlowRate * 1000.0 * expectedPotentialDrop;
    double expectedTemperature   = tArticle->mPort1DemandTemperature;
    double expectedLeakFlow      = tArticle->mPort1LeakConductance * tNodes[1].getPotential()
                                 * tNodes[1].getOutflow()->getMWeight();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.0,                   tArticle->mLastPotentialDrop, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPotentialDrop, tArticle->mPotentialDrop,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,                   tArticle->mFlux,              DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlowRate,      tArticle->mFlowRate,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVolFlowRate,   tArticle->mVolFlowRate,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower,         tArticle->mPower,             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTemperature,   tArticle->mInternalFluid->getTemperature(),
                                                                                      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                   tNodes[0].getInflux(),        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlowRate,      tNodes[0].getOutflux(),       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlowRate,      tNodes[1].getInflux(),        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedLeakFlow,      tNodes[1].getOutflux(),       DBL_EPSILON);

    /// - Set link ports to swap nodes, and verify setPort gets called by step.
    tArticle->mUserPortSelect     = 0;
    tArticle->mUserPortSetControl = GunnsBasicLink::GROUND;
    tArticle->step(tTimeStep);
    tArticle->mUserPortSelect     = 1;
    tArticle->mUserPortSelectNode = 0;
    tArticle->mUserPortSetControl = GunnsBasicLink::EXECUTE;
    tArticle->step(tTimeStep);
    tArticle->mUserPortSelect     = 0;
    tArticle->mUserPortSelectNode = 1;
    tArticle->mUserPortSetControl = GunnsBasicLink::EXECUTE;
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT(1 == tArticle->mNodeMap[0]);
    CPPUNIT_ASSERT(0 == tArticle->mNodeMap[1]);

    /// - Verify the outputs of a reverse flux between the link ports with a leak.
    tNodes[0].resetFlows();
    tNodes[1].resetFlows();
    tArticle->mPotentialVector[0]     = tNodes[1].getPotential();
    tArticle->mPotentialVector[1]     = tNodes[0].getPotential();
    expectedPotentialDrop             = tNodes[1].getPotential() - tNodes[0].getPotential();
    tArticle->mEquivalentConductance  = -1.0 / expectedPotentialDrop;
    tArticle->mPort0LeakConductance   = 0.1 * tArticle->mEquivalentConductance;
    tArticle->mPort1LeakConductance   = 0.0;
    tArticle->mPort0DemandTemperature = 300.0;
    tArticle->mPort1DemandTemperature = 290.0;

    tArticle->computeFlows(tTimeStep);
    tArticle->transportFlows(tTimeStep);

    /// - Verify the computeFlows method outputs
    expectedFlowRate      = -1.0 * tNodes[0].getOutflow()->getMWeight();
    expectedVolFlowRate   = expectedFlowRate / tNodes[0].getOutflow()->getDensity();
    expectedPower         = -expectedVolFlowRate * 1000.0 * expectedPotentialDrop;
    expectedTemperature   = tArticle->mPort0DemandTemperature;
    expectedLeakFlow      = tArticle->mPort0LeakConductance * tNodes[1].getPotential()
                                 * tNodes[1].getOutflow()->getMWeight();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedPotentialDrop,tArticle->mLastPotentialDrop, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPotentialDrop, tArticle->mPotentialDrop,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0,                  tArticle->mFlux,              DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlowRate,      tArticle->mFlowRate,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVolFlowRate,   tArticle->mVolFlowRate,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower,         tArticle->mPower,             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTemperature,   tArticle->mInternalFluid->getTemperature(),
                                                                                      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                   tNodes[0].getInflux(),        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedFlowRate,     tNodes[0].getOutflux(),       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedFlowRate,     tNodes[1].getInflux(),        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedLeakFlow,      tNodes[1].getOutflux(),       DBL_EPSILON);

    /// - Verify volumetric flow rate when the source density is zero
    tNodes[0].resetContentState();
    tNodes[0].resetFlows();
    tArticle->computeFlows(tTimeStep);
    tArticle->transportFlows(tTimeStep);

    CPPUNIT_ASSERT(0.0 == tArticle->mVolFlowRate);

    /// - Verify proper updating of portleakFlow values with outflow parameters
    tArticle->step(tTimeStep);
    tArticle->computeFlows(tTimeStep);
    double fractions[2];
    fractions[0] = 0.9;
    fractions[1] = 0.1;
    tNodes[0].getContent()->setMassAndMassFractions(1.0,fractions);
    tNodes[1].getContent()->setMassAndMassFractions(1.0,fractions);
    tNodes[0].resetFlows();
    tNodes[1].resetFlows();
    fractions[0] = 0.5;
    fractions[1] = 0.5;
    tNodes[0].getContent()->setMassAndMassFractions(1.0,fractions);
    tNodes[1].getContent()->setMassAndMassFractions(1.0,fractions);
    tArticle->transportFlows(tTimeStep);

    CPPUNIT_ASSERT(tNodes[0].getOutflow()->getMWeight() !=
                           tNodes[0].getContent()->getMWeight());

    CPPUNIT_ASSERT(tNodes[1].getOutflow()->getMWeight() !=
                           tNodes[1].getContent()->getMWeight());

    double tempPort0LeakFlow = 0.1 * (tArticle->mPort0LeakConductance *
                           tArticle->mPotentialVector[0]  * tNodes[0].getOutflow()->getMWeight());

    double tempPort1LeakFlow = 0.1 * (tArticle->mPort1LeakConductance *
                           tArticle->mPotentialVector[1] * tNodes[1].getOutflow()->getMWeight());

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getOutflux(), tempPort0LeakFlow, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[1].getOutflux(), tempPort1LeakFlow, DBL_EPSILON);

    /// - Verfy correct port direction with Positive, negative, and zero potential Vectors
    tArticle->mPotentialVector[0]     = tNodes[0].getPotential();
    tArticle->mPotentialVector[1]     = tNodes[1].getPotential();
    tArticle->mEquivalentConductance = 1.0;
    tArticle->computeFlows(tTimeStep);

    /// - Molar flux should be greater than zero because the port 0 potential vector is positive
    CPPUNIT_ASSERT( tArticle->mFlux > 0.0);

    /// - Confirm correct source port allocation with positive potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tArticle->mPortDirections[0]);

    /// - Confirm correct source port allocation with positive potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::SINK == tArticle->mPortDirections[1]);

    tArticle->mEquivalentConductance = -1.0;
    tArticle->computeFlows(tTimeStep);

    /// - Molar flux should be less than zero because the port 0 potential vector is negative
    CPPUNIT_ASSERT( tArticle->mFlux < 0.0);

    /// - Confirm correct source port allocation with negative potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::SINK == tArticle->mPortDirections[0]);

    /// - Confirm correct source port allocation with negative potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tArticle->mPortDirections[1]);

    tArticle->mEquivalentConductance = 0.0;
    tArticle->computeFlows(tTimeStep);

    /// - Molar flux should be equal to zero because the port 0 potential vector is zero
    CPPUNIT_ASSERT( tArticle->mFlux == 0.0);

    /// - Confirm correct source port allocation with zero potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tArticle->mPortDirections[0]);

    /// - Confirm correct source port allocation with zero potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tArticle->mPortDirections[1]);

    UT_PASS_LAST;

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] equivC (--) Expected equivalent conductance
/// @param[in] leakC0 (--) Expected port 0 leak conductance
/// @param[in] leakC0 (--) Expected port 1 leak conductance
/// @param[in] flag   (--) Expected new admittance flag
///
/// @details  Verifies the outputs of the link step method compare with given arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidEqConductor::verifyStepOutputs(const double equivC,
                                                const double leakC0,
                                                const double leakC1,
                                                const bool   flag)
{
    CPPUNIT_ASSERT_DOUBLES_EQUAL(equivC,          tArticle->mEquivalentConductance,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(leakC0,          tArticle->mPort0LeakConductance,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(leakC1,          tArticle->mPort1LeakConductance,     DBL_EPSILON);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(equivC + leakC0, tArticle->mAdmittanceMatrix[0],      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-equivC,         tArticle->mAdmittanceMatrix[1],      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-equivC,         tArticle->mAdmittanceMatrix[2],      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(equivC + leakC1, tArticle->mAdmittanceMatrix[3],      DBL_EPSILON);

    CPPUNIT_ASSERT(flag == tArticle->mAdmittanceUpdate);
}
