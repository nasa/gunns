/*
@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/

#include "strings/UtResult.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "UtGunnsFluidSourceBoundary.hh"

/// @details  Test identification number.
int UtGunnsFluidSourceBoundary::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsFluidSourceBoundary class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidSourceBoundary::UtGunnsFluidSourceBoundary()
    :
    tConfigData(),
    tInputData(),
    tArticle(),
    tFlipFlowSign(),
    tTraceCompoundsOnly(),
    tLinkName(),
    tInitialFlowDemand(),
    tTcRatesState(),
    tTcInput(),
    tNodes(),
    tNodeList(),
    tLinks(),
    tPort0(),
    tTimeStep(),
    tTolerance(),
    tFluidProperties(),
    tCompoundProperties(),
    tTcTypes(),
    tFluidTcConfig(),
    tFluidConfig(),
    tFluidTcInput(),
    tFluidInput1(),
    tFluidInput2(),
    tFractions1(),
    tFractions2()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsFluidSourceBoundary class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidSourceBoundary::~UtGunnsFluidSourceBoundary()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSourceBoundary::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete tFluidInput2;
    delete tFluidInput1;
    delete tFluidTcInput;
    delete tFluidConfig;
    delete tFluidTcConfig;
    delete tCompoundProperties;
    delete tFluidProperties;
    delete tTcInput;
    delete [] tTcRatesState;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSourceBoundary::setUp()
{
    tLinkName           = "Test Fluid Source Boundary";
    tNodeList.mNumNodes = 2;
    tNodeList.mNodes    = tNodes;
    tFlipFlowSign       = true;
    tTraceCompoundsOnly = false;
    tInitialFlowDemand  = -0.5;
    tTcRatesState       = new double[2];
    tTcRatesState[0]    = 1.0e-9;
    tTcRatesState[1]    = 2.0e-10;
    tTcInput            = new GunnsFluidTraceCompoundsInputData(tTcRatesState);
    tPort0              = 0;
    tTcTypes[0]         = ChemicalCompound::H2O;
    tTcTypes[1]         = ChemicalCompound::CO2;
    tTimeStep           = 0.1;
    tTolerance          = 0.01;

    /// - Setup some fluid nodes
    tFluidProperties = new DefinedFluidProperties();
    FluidProperties::FluidType types[2];
    types[0]    = FluidProperties::GUNNS_N2;
    types[1]    = FluidProperties::GUNNS_O2;
    tFractions1[0] = 0.5;
    tFractions1[1] = 0.5;

    tCompoundProperties = new DefinedChemicalCompounds();
    tFluidTcConfig = new GunnsFluidTraceCompoundsConfigData(tTcTypes, 2, "tFluidTcConfig");
    tFluidConfig = new PolyFluidConfigData(tFluidProperties, types, 2, tFluidTcConfig);

    double tcConcentrations[2];
    tcConcentrations[0] = 5.0e-5;
    tcConcentrations[1] = 6.0e-6;
    tFluidTcInput = new GunnsFluidTraceCompoundsInputData(tcConcentrations);

    tFluidInput1 = new PolyFluidInputData(283.15,                   //temperature
                                          700.728,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          tFractions1,              //massFractions
                                          tFluidTcInput);           //trace compounds

    tFractions2[0] = 0.0;
    tFractions2[1] = 1.0;
    tFluidInput2 = new PolyFluidInputData(300.0,                    //temperature
                                          689.728,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          tFractions2,              //massFractions
                                          tTcInput);                //trace compounds

    /// - Have to initialize the nodes with the fluid configs (normally done by GUNNS)
    tNodes[0].initialize("UtTestNode0", tFluidConfig);
    tNodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput1);
    tNodes[0].resetFlows();

    /// - Define nominal configuration data
    tConfigData = new GunnsFluidSourceBoundaryConfigData(tLinkName, &tNodeList, tFlipFlowSign,
                                                         tTraceCompoundsOnly);

    /// - Define default input data
    tInputData = new GunnsFluidSourceBoundaryInputData(true, 0.5, tInitialFlowDemand, tFluidInput2);

    /// - Default construct the nominal test article.
    tArticle = new FriendlyGunnsFluidSourceBoundary;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSourceBoundary::testConfig()
{
    UT_RESULT_FIRST;

    /// - Check nominal config construction
    tTraceCompoundsOnly = true;
    GunnsFluidSourceBoundaryConfigData nominalConfig(tLinkName, &tNodeList, tFlipFlowSign,
                                                     tTraceCompoundsOnly);
    CPPUNIT_ASSERT(tLinkName           == nominalConfig.mName);
    CPPUNIT_ASSERT(tNodes              == nominalConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tFlipFlowSign       == nominalConfig.mFlipFlowSign);
    CPPUNIT_ASSERT(tTraceCompoundsOnly == nominalConfig.mTraceCompoundsOnly);

    /// - Check default config construction
    GunnsFluidSourceBoundaryConfigData defaultConfig;
    CPPUNIT_ASSERT(""                  == defaultConfig.mName);
    CPPUNIT_ASSERT(0                   == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(false               == defaultConfig.mFlipFlowSign);
    CPPUNIT_ASSERT(false               == defaultConfig.mTraceCompoundsOnly);

    /// - Check copy config construction
    GunnsFluidSourceBoundaryConfigData copyConfig(nominalConfig);
    CPPUNIT_ASSERT(tLinkName           == copyConfig.mName);
    CPPUNIT_ASSERT(tNodes              == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tFlipFlowSign       == copyConfig.mFlipFlowSign);
    CPPUNIT_ASSERT(tTraceCompoundsOnly == copyConfig.mTraceCompoundsOnly);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSourceBoundary::testInput()
{
    UT_RESULT;

    /// - Check nominal input construction
    CPPUNIT_ASSERT(true               == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.5                == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tInitialFlowDemand == tInputData->mFlowDemand);
    CPPUNIT_ASSERT(tFluidInput2       == tInputData->mInternalFluid);
    CPPUNIT_ASSERT(tTcInput           == tInputData->mInternalFluid->mTraceCompounds);
    CPPUNIT_ASSERT(tTcRatesState[0]   == tInputData->mInternalFluid->mTraceCompounds->mState[0]);
    CPPUNIT_ASSERT(tTcRatesState[1]   == tInputData->mInternalFluid->mTraceCompounds->mState[1]);

    /// - Check default input construction
    GunnsFluidSourceBoundaryInputData defaultInput;
    CPPUNIT_ASSERT(false              == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0                == defaultInput.mFlowDemand);
    CPPUNIT_ASSERT(0                  == defaultInput.mInternalFluid);

    /// - Check copy input construction
    GunnsFluidSourceBoundaryInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(true               == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.5                == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(tInitialFlowDemand == copyInput.mFlowDemand);
    CPPUNIT_ASSERT(tFluidInput2       == copyInput.mInternalFluid);
    CPPUNIT_ASSERT(tTcInput           == copyInput.mInternalFluid->mTraceCompounds);
    CPPUNIT_ASSERT(tTcRatesState[0]   == copyInput.mInternalFluid->mTraceCompounds->mState[0]);
    CPPUNIT_ASSERT(tTcRatesState[1]   == copyInput.mInternalFluid->mTraceCompounds->mState[1]);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSourceBoundary::testDefaultConstruction()
{
    UT_RESULT;

    /// @test state data
    CPPUNIT_ASSERT(false == tArticle->mFlipFlowSign);
    CPPUNIT_ASSERT(false == tArticle->mTraceCompoundsOnly);
    CPPUNIT_ASSERT(0.0   == tArticle->mFlowDemand);
    CPPUNIT_ASSERT(0     == tArticle->mTraceCompoundRates);

    /// @test init flag
    CPPUNIT_ASSERT(false == tArticle->mInitFlag);

    /// @test new/delete for code coverage
    GunnsFluidSourceBoundary* article = new GunnsFluidSourceBoundary();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSourceBoundary::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize the test article with nominal data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);

    /// @test config, input & state data
    CPPUNIT_ASSERT(tFlipFlowSign      == tArticle->mFlipFlowSign);
    CPPUNIT_ASSERT(tInitialFlowDemand == tArticle->mFlowDemand);
    CPPUNIT_ASSERT(tArticle->mInternalFluid);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluidInput2->mTemperature,
                                 tArticle->mInternalFluid->getTemperature(),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFractions2[0],
                                 tArticle->mInternalFluid->getMassFraction(FluidProperties::GUNNS_N2),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFractions2[1],
                                 tArticle->mInternalFluid->getMassFraction(FluidProperties::GUNNS_O2),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT(tArticle->mTraceCompoundRates);
    CPPUNIT_ASSERT(tArticle->mTraceCompoundRates[0] == tTcRatesState[0]);
    CPPUNIT_ASSERT(tArticle->mTraceCompoundRates[1] == tTcRatesState[1]);

    /// @test init flag
    CPPUNIT_ASSERT(true               == tArticle->mInitFlag);

    /// @test trace compound rates are zeroed if no trace compounds in link input data
    tInputData->mInternalFluid->mTraceCompounds = 0;
    delete tArticle;
    tArticle = new FriendlyGunnsFluidSourceBoundary;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);

    CPPUNIT_ASSERT(tFlipFlowSign      == tArticle->mFlipFlowSign);
    CPPUNIT_ASSERT(tInitialFlowDemand == tArticle->mFlowDemand);
    CPPUNIT_ASSERT(tArticle->mInternalFluid);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluidInput2->mTemperature,
                                 tArticle->mInternalFluid->getTemperature(),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFractions2[0],
                                 tArticle->mInternalFluid->getMassFraction(FluidProperties::GUNNS_N2),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFractions2[1],
                                 tArticle->mInternalFluid->getMassFraction(FluidProperties::GUNNS_O2),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT(tArticle->mTraceCompoundRates);
    CPPUNIT_ASSERT(0.0 == tArticle->mTraceCompoundRates[0]);
    CPPUNIT_ASSERT(0.0 == tArticle->mTraceCompoundRates[1]);
    CPPUNIT_ASSERT(true               == tArticle->mInitFlag);

    /// @test initialization for no trace compounds in network
    tFluidConfig->mTraceCompounds = 0;
    GunnsFluidNode nodes[2];
    tNodeList.mNodes    = nodes;
    tFluidInput1->mTraceCompounds = 0;
    nodes[0].initialize("nodes_0", tFluidConfig);
    nodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput1);

    delete tArticle;
    tArticle = new FriendlyGunnsFluidSourceBoundary;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);

    CPPUNIT_ASSERT(tFlipFlowSign      == tArticle->mFlipFlowSign);
    CPPUNIT_ASSERT(tInitialFlowDemand == tArticle->mFlowDemand);
    CPPUNIT_ASSERT(tArticle->mInternalFluid);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluidInput2->mTemperature,
                                 tArticle->mInternalFluid->getTemperature(),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFractions2[0],
                                 tArticle->mInternalFluid->getMassFraction(FluidProperties::GUNNS_N2),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFractions2[1],
                                 tArticle->mInternalFluid->getMassFraction(FluidProperties::GUNNS_O2),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT(!tArticle->mTraceCompoundRates);
    CPPUNIT_ASSERT(true               == tArticle->mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for exceptions thrown during initialization.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSourceBoundary::testInitializationExceptions()
{
    UT_RESULT;

    /// @test exception thrown on no internal fluid provided.
    tInputData->mInternalFluid = 0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0),
                         TsInitializationException);
    tInputData->mInternalFluid = tFluidInput2;

    /// @test exception thrown on trace compounds only flag set when network has no trace compounds.
    tConfigData->mTraceCompoundsOnly = true;
    tFluidConfig->mTraceCompounds = 0;
    GunnsFluidNode nodes[2];
    tNodeList.mNodes    = nodes;
    tFluidInput1->mTraceCompounds = 0;
    nodes[0].initialize("nodes_0", tFluidConfig);
    nodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput1);

    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0),
                         TsInitializationException);
    tFluidConfig->mTraceCompounds = tFluidTcConfig;
    tFluidInput1->mTraceCompounds = tFluidTcInput;
    tNodeList.mNodes              = tNodes;

    /// @test exception thrown on trace compounds only flag but input data has no trace compounds.
    tInputData->mInternalFluid->mTraceCompounds = 0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0),
                         TsInitializationException);
    tInputData->mInternalFluid->mTraceCompounds = tTcInput;

    /// @test init flag
    CPPUNIT_ASSERT(false == tArticle->mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSourceBoundary::testRestart()
{
    UT_RESULT;

    /// - Initialize the test article with nominal data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);

    /// @test base class restart.
    tArticle->restart();

    /// - currently nothing to verify.

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test accessors and getters.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSourceBoundary::testAccessorsAndGetters()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);

    /// @test setFlowDemand & getFlowDemand methods.
    tArticle->setFlowDemand(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1, tArticle->getFlowDemand(), 0.0);

    /// @test setFlowState method.
    PolyFluid* fluid = tNodes[0].getContent();
    tArticle->setFlowState(fluid);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(fluid->getPressure(), tArticle->mInternalFluid->getPressure(), 0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the step method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSourceBoundary::testStep()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);

    /// - During this step the flow rate will be equal to the given flow demand reduced by the
    ///   blockage malfunction, and with the sign flipped.
    tArticle->step(tTimeStep);
    double expectedMdot = -0.5 * tInitialFlowDemand;
    double expectedFlux = expectedMdot / tArticle->mInternalFluid->getMWeight();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot, tArticle->mFlowRate,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mFlux,            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mSourceVector[0], DBL_EPSILON);

    /// - Step again with the blockage malfunction de-activated and flow sign not flipped.
    tArticle->mFlipFlowSign = false;
    tArticle->setMalfBlockage();
    tArticle->step(tTimeStep);
    expectedMdot = tInitialFlowDemand;
    expectedFlux = expectedMdot / tArticle->mInternalFluid->getMWeight();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot, tArticle->mFlowRate,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mFlux,            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mSourceVector[0], DBL_EPSILON);

    /// - Step again with zero molecular weight in the internal fluid.  We do this by hacking the
    ///   link's internal fluid to point to the Ground node's contents, which hasn't been init'd.
    PolyFluid* saveFluid     = tArticle->mInternalFluid;
    tArticle->mInternalFluid = tNodes[1].getContent();
    tArticle->step(tTimeStep);
    tArticle->mInternalFluid = saveFluid;
    expectedMdot = tInitialFlowDemand;
    expectedFlux = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot, tArticle->mFlowRate,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mFlux,            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mSourceVector[0], DBL_EPSILON);

    /// - Step again with trace compounds only set.
    tArticle->mTraceCompoundsOnly = true;
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mFlux,            DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows with flow into the node.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSourceBoundary::testComputeFlowsToNode()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data, forward flow and no
    ///   blockage.
    tInputData->mMalfBlockageFlag = false;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);

    tArticle->step(tTimeStep);
    tArticle->computeFlows(tTimeStep);

    CPPUNIT_ASSERT(GunnsBasicLink::SINK == tArticle->mPortDirections[0]);

    tArticle->transportFlows(tTimeStep);

    const double expectedP   =  tNodes[0].getPotential();
    const double expectedQ   = -tInitialFlowDemand / tArticle->getInternalFluid()->getDensity();
    const double expectedPwr = 1000.0 * expectedQ * expectedP;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedP,                 tArticle->mPotentialDrop,                DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedQ,                 tArticle->mVolFlowRate,                  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedPwr,               tArticle->mPower,                        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-tInitialFlowDemand,        tNodes[0].getInflux(),                   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluidInput2->mTemperature, tNodes[0].getInflow()->getTemperature(), FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,                       tNodes[0].getOutflux(),                  DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows with flow out of the node.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSourceBoundary::testComputeFlowsFromNode()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data, reverse flow and no
    ///   blockage.
    tInputData->mMalfBlockageFlag = false;
    tInputData->mFlowDemand       = -tInitialFlowDemand;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);

    tArticle->step(tTimeStep);
    tArticle->computeFlows(tTimeStep);

    CPPUNIT_ASSERT(GunnsBasicLink::SINK == tArticle->mPortDirections[0]);

    tArticle->transportFlows(tTimeStep);

    const double expectedP   = -tNodes[0].getPotential();
    const double expectedQ   = tInitialFlowDemand / tArticle->getInternalFluid()->getDensity();
    const double expectedPwr = 1000.0 * expectedQ * expectedP;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedP,                 tArticle->mPotentialDrop,                DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedQ,                 tArticle->mVolFlowRate,                  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedPwr,               tArticle->mPower,                        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( tInitialFlowDemand,        tNodes[0].getInflux(),                   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluidInput2->mTemperature, tNodes[0].getInflow()->getTemperature(), FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,                       tNodes[0].getOutflux(),                  DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows with zero flow demand.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSourceBoundary::testComputeFlowsZeroFlow()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data, zero flow and no
    ///   blockage.
    tInputData->mMalfBlockageFlag = false;
    tInputData->mFlowDemand       = 0.0;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);

    tArticle->step(tTimeStep);
    tArticle->computeFlows(tTimeStep);

    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tArticle->mPortDirections[0]);

    tArticle->transportFlows(tTimeStep);

    const double expectedP   = -tNodes[0].getPotential();
    const double expectedQ   = 0.0;
    const double expectedPwr = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedP,   tArticle->mPotentialDrop, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedQ,   tArticle->mVolFlowRate,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedPwr, tArticle->mPower,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,         tNodes[0].getInflux(),    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,         tNodes[0].getOutflux(),   DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows with zero internal fluid density.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSourceBoundary::testComputeFlowsZeroDensity()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data, forward flow and no
    ///   blockage.
    tInputData->mMalfBlockageFlag = false;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);

    tArticle->step(tTimeStep);

    /// - Force the internal fluid density to zero.  We do this by hacking the link's internal fluid
    ///   to point to the Ground node's contents, which hasn't been init'd.
    PolyFluid* saveFluid     = tArticle->mInternalFluid;
    tArticle->mInternalFluid = tNodes[1].getOutflow();
    tArticle->computeFlows(tTimeStep);
    tArticle->transportFlows(tTimeStep);
    tArticle->mInternalFluid = saveFluid;

    const double expectedQ   = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedQ, tArticle->mVolFlowRate, DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows with trace compounds only.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSourceBoundary::testComputeFlowsTcOnly()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data and trace compounds only.
    tConfigData->mTraceCompoundsOnly = true;
    tConfigData->mFlipFlowSign       = false;
    tInputData->mMalfBlockageFlag    = false;
    tInputData->mFlowDemand          = 1.0;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);

    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tArticle->mFlowRate, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mFlux,     DBL_EPSILON);

    tArticle->computeFlows(tTimeStep);

    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tArticle->mPortDirections[0]);

    tArticle->transportFlows(tTimeStep);

    FriendlyGunnsFluidSourceBoundaryNode* node = static_cast<FriendlyGunnsFluidSourceBoundaryNode*>(&tNodes[0]);
    const double mdotH2O = node->mTcInflow.mState[0];
    const double mdotCO2 = node->mTcInflow.mState[1];

    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,                              tArticle->mFlowRate, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                              tArticle->mFlux,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mTraceCompoundRates[0], mdotH2O,             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mTraceCompoundRates[1], mdotCO2,             DBL_EPSILON);

    UT_PASS_LAST;
}
