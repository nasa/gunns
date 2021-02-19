/*
@copyright Copyright 2020 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/
#include "UtGunnsFluidPhaseChangeSource.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

/// @details  Test identification number.
int UtGunnsFluidPhaseChangeSource::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsFluidPhaseChangeSource class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidPhaseChangeSource::UtGunnsFluidPhaseChangeSource()
    :
    tLinkName(),
    tLiquidType(),
    tGasType(),
    tEfficiency(),
    tConfigData(),
    tMalfBlockageFlag(),
    tMalfBlockageValue(),
    tPowerInput(),
    tInputData(),
    tArticle(),
    tNodes(),
    tNodeList(),
    tLinks(),
    tPort0(),
    tPort1(),
    tTimeStep(),
    tFluidProperties(),
    tFluidConfig(),
    tFluidInput1(),
    tFluidInput2(),
    tFractions()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsFluidPhaseChangeSource class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidPhaseChangeSource::~UtGunnsFluidPhaseChangeSource()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPhaseChangeSource::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete tFluidInput2;
    delete tFluidInput1;
    delete [] tFractions;
    delete tFluidConfig;
    delete tFluidProperties;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPhaseChangeSource::setUp()
{
    tLinkName           = "Test Fluid Evaporation";
    tNodeList.mNumNodes = 5;
    tNodeList.mNodes    = tNodes;
    tPort0              = 0;
    tPort1              = 1;
    tTimeStep           = 0.1;

    /// - Setup some fluid nodes
    tFluidProperties = new DefinedFluidProperties();
    FluidProperties::FluidType types[3];
    types[0]     = FluidProperties::GUNNS_HE;
    types[1]     = FluidProperties::GUNNS_CH4;
    types[2]     = FluidProperties::GUNNS_METHANE;
    tFluidConfig = new PolyFluidConfigData(tFluidProperties, types, 3);

    /// - Have to initialize the nodes with the fluid configs (normally done by GUNNS)
    tNodes[0].initialize("UtTestNode0", tFluidConfig);
    tNodes[1].initialize("UtTestNode1", tFluidConfig);
    tNodes[2].initialize("UtTestNode2", tFluidConfig);
    tNodes[3].initialize("UtTestNode3", tFluidConfig);
    tNodes[4].initialize("UtTestNode4", tFluidConfig);

    /// - Saturation temperature for methane @ 101.325 kPa is 111.67 K.
    tFractions = new double[3];
    tFractions[0] = 0.0;
    tFractions[1] = 0.0;
    tFractions[2] = 1.0;
    tFluidInput1 = new PolyFluidInputData(100.0,                    //temperature
                                          101.325,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          tFractions);              //massFractions

    tNodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput1);
    tNodes[2].getContent()->initialize(*tFluidConfig, *tFluidInput1);
    tNodes[0].resetFlows();
    tNodes[2].resetFlows();

    tFractions[0] = 0.001;
    tFractions[1] = 0.999;
    tFractions[2] = 0.0;
    tFluidInput2 = new PolyFluidInputData(120.0,                    //temperature
                                          101.325,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          tFractions);              //massFractions

    tNodes[1].getContent()->initialize(*tFluidConfig, *tFluidInput2);
    tNodes[1].resetFlows();

    tFluidInput2->mMassFraction[0] = 0.0;
    tFluidInput2->mMassFraction[1] = 1.0;
    tNodes[3].getContent()->initialize(*tFluidConfig, *tFluidInput2);
    tNodes[3].resetFlows();

    /// - Define nominal configuration data
    tLiquidType       = FluidProperties::GUNNS_METHANE;
    tGasType          = FluidProperties::GUNNS_CH4;
    tEfficiency       = 0.9;
    tConfigData       = new GunnsFluidPhaseChangeSourceConfigData(tLinkName,
                                                                  &tNodeList,
                                                                  tLiquidType,
                                                                  tGasType,
                                                                  tEfficiency);

    /// - Define nominal input data
    tMalfBlockageFlag  = true;
    tMalfBlockageValue = 0.5;
    tPowerInput        = 1000.0;
    tInputData         = new GunnsFluidPhaseChangeSourceInputData(tMalfBlockageFlag,
                                                                  tMalfBlockageValue,
                                                                  tPowerInput);

    tArticle = new FriendlyGunnsFluidPhaseChangeSource;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPhaseChangeSource::testConfig()
{
    UT_RESULT_FIRST;

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tLinkName                 == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                    == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tLiquidType               == tConfigData->mLiquidType);
    CPPUNIT_ASSERT(tGasType                  == tConfigData->mGasType);
    CPPUNIT_ASSERT(tEfficiency               == tConfigData->mEfficiency);

    /// - Check default config construction
    GunnsFluidPhaseChangeSourceConfigData defaultConfig;
    CPPUNIT_ASSERT(""                        == defaultConfig.mName);
    CPPUNIT_ASSERT(0                         == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(FluidProperties::NO_FLUID == defaultConfig.mLiquidType);
    CPPUNIT_ASSERT(FluidProperties::NO_FLUID == defaultConfig.mGasType);
    CPPUNIT_ASSERT(0.0                       == defaultConfig.mEfficiency);

    /// - Check copy config construction
    GunnsFluidPhaseChangeSourceConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tLinkName                 == copyConfig.mName);
    CPPUNIT_ASSERT(tNodes                    == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tLiquidType               == copyConfig.mLiquidType);
    CPPUNIT_ASSERT(tGasType                  == copyConfig.mGasType);
    CPPUNIT_ASSERT(tEfficiency               == copyConfig.mEfficiency);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPhaseChangeSource::testInput()
{
    UT_RESULT;

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tMalfBlockageFlag  == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tPowerInput        == tInputData->mPowerInput);

    /// - Check default config construction
    GunnsFluidPhaseChangeSourceInputData defaultInput;
    CPPUNIT_ASSERT(false              == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0                == defaultInput.mPowerInput);

    /// - Check copy config construction
    GunnsFluidPhaseChangeSourceInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tMalfBlockageFlag  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(tPowerInput        == copyInput.mPowerInput);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPhaseChangeSource::testDefaultConstruction()
{
    UT_RESULT;

    /// @test proper default construction of class member data
    CPPUNIT_ASSERT(FluidProperties::NO_FLUID == tArticle->mLiquidType);
    CPPUNIT_ASSERT(FluidProperties::NO_FLUID == tArticle->mGasType);
    CPPUNIT_ASSERT(0.0                       == tArticle->mEfficiency);
    CPPUNIT_ASSERT(0                         == tArticle->mLiquidIndex);
    CPPUNIT_ASSERT(0                         == tArticle->mGasIndex);
    CPPUNIT_ASSERT(0.0                       == tArticle->mPowerInput);
    CPPUNIT_ASSERT(0                         == tArticle->mLiquidFluid);
    CPPUNIT_ASSERT(0                         == tArticle->mGasFluid);
    CPPUNIT_ASSERT(0.0                       == tArticle->mTsat);
    CPPUNIT_ASSERT(0.0                       == tArticle->mDh);
    CPPUNIT_ASSERT(0.0                       == tArticle->mL);

    /// @test init flag
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    /// @test new/delete for code coverage
    GunnsFluidPhaseChangeSource* article = new GunnsFluidPhaseChangeSource();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPhaseChangeSource::testNominalInitialization()
{
    UT_RESULT;

    /// - Default construct and initialize (with nominal data) a test article
    FriendlyGunnsFluidPhaseChangeSource article;
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Verify the parent method is called
    CPPUNIT_ASSERT(tLinkName          == article.getName());
    CPPUNIT_ASSERT(tMalfBlockageValue == article.mMalfBlockageValue);

    /// - Verify member variables are properly set
    DefinedFluidProperties definedFluidProps;
    const double MW = definedFluidProps.getProperties(FluidProperties::GUNNS_CH4)->getMWeight();
    CPPUNIT_ASSERT(tLiquidType                == article.mLiquidType);
    CPPUNIT_ASSERT(tGasType                   == article.mGasType);
    CPPUNIT_ASSERT(tEfficiency                == article.mEfficiency);
    CPPUNIT_ASSERT(2                          == article.mLiquidIndex);
    CPPUNIT_ASSERT(1                          == article.mGasIndex);
    CPPUNIT_ASSERT(tPowerInput                == article.mPowerInput);
    CPPUNIT_ASSERT(MW                         == article.mLiquidFluid->getMWeight());
    CPPUNIT_ASSERT(MW                         == article.mGasFluid->getMWeight());
    CPPUNIT_ASSERT(0.0                        == article.mTsat);
    CPPUNIT_ASSERT(0.0                        == article.mDh);
    CPPUNIT_ASSERT(0.0                        == article.mL);

    /// @test init flag
    CPPUNIT_ASSERT(article.mInitFlag);

    /// @test double initialization.
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    CPPUNIT_ASSERT(MW                         == article.mLiquidFluid->getMWeight());
    CPPUNIT_ASSERT(MW                         == article.mGasFluid->getMWeight());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test exceptions from the initialize method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPhaseChangeSource::testInitializationExceptions()
{
    UT_RESULT;

    /// @test for exception on liquid type not in network.
    tConfigData->mLiquidType = FluidProperties::GUNNS_WATER;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
            TsInitializationException);
    tConfigData->mLiquidType = tLiquidType;

    /// @test for exception on gas type not in network.
    tConfigData->mGasType = FluidProperties::GUNNS_H2O;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
            TsInitializationException);
    tConfigData->mGasType = tGasType;

    /// @test for exception on different liquid & gas compounds.
    tConfigData->mGasType = FluidProperties::GUNNS_HE;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
            TsInitializationException);
    tConfigData->mGasType = tGasType;

    /// @test for exception on wrong phase in the liquid type.
    tConfigData->mLiquidType = tGasType;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
            TsInitializationException);
    tConfigData->mLiquidType = tLiquidType;

    /// @test for exception on wrong phase in the gas type.
    tConfigData->mGasType = tLiquidType;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
            TsInitializationException);
    tConfigData->mGasType = tGasType;

    /// @test for exception on efficiency not in (-1, 1).
    tConfigData->mEfficiency = 1.001;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
            TsInitializationException);
    tConfigData->mEfficiency = -1.001;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
            TsInitializationException);
    tConfigData->mEfficiency = tEfficiency;

    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPhaseChangeSource::testRestart()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Set some stuff in the article that should be reset on restart.
    tArticle->mPower = 1.0;

    /// @test restart resets terms.
    tArticle->restart();

    CPPUNIT_ASSERT(0.0 == tArticle->mPower);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for the step method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPhaseChangeSource::testStep()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    tArticle->mPotentialVector[0] = tFluidInput1->mPressure;
    tArticle->mPotentialVector[1] = tFluidInput2->mPressure;

    DefinedFluidProperties definedFluidProps;
    const FluidProperties* gasProps = definedFluidProps.getProperties(tGasType);
    const FluidProperties* liqProps = definedFluidProps.getProperties(tLiquidType);
    const FluidProperties* heProps  = definedFluidProps.getProperties(FluidProperties::GUNNS_HE);

    /// @test nominal outputs for positive power direction: liquid to gas.
    double expectedPwr  = tPowerInput * tEfficiency * (1.0 - tMalfBlockageValue);
    double expectedTsat = gasProps->getSaturationTemperature(tFluidInput1->mPressure);
    double expectedDh   = liqProps->getSpecificEnthalpy(expectedTsat)
                        - liqProps->getSpecificEnthalpy(tFluidInput1->mTemperature);
    double expectedL    = liqProps->getHeatOfVaporization(expectedTsat) * 1000.0;
    double expectedMdot = expectedPwr / (expectedDh + expectedL);
    double expectedFlux = expectedMdot / gasProps->getMWeight();

    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedPwr,  tArticle->mPower,           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedTsat, tArticle->mTsat,            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedDh,   tArticle->mDh,              DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedL,    tArticle->mL,               DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedMdot, tArticle->mFlowRate,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedFlux, tArticle->mFlux,            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedFlux, tArticle->mSourceVector[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedFlux, tArticle->mSourceVector[1], DBL_EPSILON);

    /// @test nominal outputs for negative power direction: gas to liquid.
    tEfficiency *= -1.0;
    tArticle->mEfficiency = tEfficiency;
    expectedPwr  = tPowerInput * tEfficiency * (1.0 - tMalfBlockageValue);
    expectedTsat = liqProps->getSaturationTemperature(tFluidInput2->mPressure);
    expectedDh   = 0.999 * (gasProps->getSpecificEnthalpy(tFluidInput2->mTemperature)
                          - gasProps->getSpecificEnthalpy(expectedTsat))
                 + 0.001 * (heProps->getSpecificEnthalpy(tFluidInput2->mTemperature)
                          - heProps->getSpecificEnthalpy(expectedTsat));
    expectedL    = gasProps->getHeatOfVaporization(expectedTsat) * 1000.0;
    expectedMdot = expectedPwr / (expectedDh + expectedL);
    expectedFlux = expectedMdot / gasProps->getMWeight();

    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedPwr,  tArticle->mPower,           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedTsat, tArticle->mTsat,            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedDh,   tArticle->mDh,              FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedL,    tArticle->mL,               DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedMdot, tArticle->mFlowRate,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedFlux, tArticle->mFlux,            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedFlux, tArticle->mSourceVector[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedFlux, tArticle->mSourceVector[1], DBL_EPSILON);

    /// @test nominal outputs for zero power.
    tArticle->mPowerInput = 0.0;
    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mPower,           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTsat, tArticle->mTsat,            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDh,   tArticle->mDh,              FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedL,    tArticle->mL,               DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mFlowRate,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mFlux,            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mSourceVector[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mSourceVector[1], DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPhaseChangeSource::testComputeFlows()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    tArticle->mPotentialVector[0] = tFluidInput1->mPressure;
    tArticle->mPotentialVector[1] = tFluidInput2->mPressure;

    /// @test outputs with no flow.
    const double expectedDp = tFluidInput1->mPressure - tFluidInput2->mPressure;
    tArticle->computeFlows(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDp, tArticle->mPotentialDrop, DBL_EPSILON);
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tArticle->mPortDirections[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tArticle->mPortDirections[1]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[tPort0].getScheduledOutflux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[tPort1].getScheduledOutflux(), DBL_EPSILON);

    /// @test outputs with positive flow.
    tArticle->mFlux = 1.0;
    tArticle->computeFlows(tTimeStep);
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tArticle->mPortDirections[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::SINK   == tArticle->mPortDirections[1]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tNodes[tPort0].getScheduledOutflux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[tPort1].getScheduledOutflux(), DBL_EPSILON);

    /// @test outputs with negative flow and the source node isn't 100% the gas type.
    tNodes[tPort0].resetFlows();
    tNodes[tPort1].resetFlows();

    tArticle->mFlux = -1.0;
    tArticle->computeFlows(tTimeStep);
    CPPUNIT_ASSERT(GunnsBasicLink::SINK == tArticle->mPortDirections[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tArticle->mPortDirections[1]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[tPort0].getScheduledOutflux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[tPort1].getScheduledOutflux(), DBL_EPSILON);

    /// @test outputs with negative flow and the source node is 100% the gas type.
    tArticle->mUserPortSelect     = 1;
    tArticle->mUserPortSelectNode = 3;
    tArticle->mUserPortSetControl = GunnsBasicLink::EXECUTE;
    tArticle->step(tTimeStep);

    tArticle->mFlux = -1.0;
    tArticle->computeFlows(tTimeStep);
    CPPUNIT_ASSERT(GunnsBasicLink::SINK   == tArticle->mPortDirections[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tArticle->mPortDirections[1]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[tPort0].getScheduledOutflux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tNodes[3].getScheduledOutflux(),      DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for transport flow.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPhaseChangeSource::testTransportFlows()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    tArticle->mPotentialVector[0] = tFluidInput1->mPressure;
    tArticle->mPotentialVector[1] = tFluidInput2->mPressure;

    DefinedFluidProperties definedFluidProps;
    const FluidProperties* gasProps = definedFluidProps.getProperties(tGasType);
    const FluidProperties* liqProps = definedFluidProps.getProperties(tLiquidType);
    const FluidProperties* heProps  = definedFluidProps.getProperties(FluidProperties::GUNNS_HE);

    /// @test positive flow direction.
    tArticle->mFlowRate = 0.001;

    double expectedQ  = 0.001 / tNodes[tPort0].getContent()->getDensity();
    double expectedT  = gasProps->getSaturationTemperature(tFluidInput2->mPressure);
    double expectedMW = gasProps->getMWeight();
    tArticle->transportFlows(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedQ, tArticle->mVolFlowRate,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,       tNodes[tPort0].getInflux(),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.001,     tNodes[tPort0].getOutflux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.001,     tNodes[tPort1].getInflux(),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,       tNodes[tPort1].getOutflux(), DBL_EPSILON);
    PolyFluid* influid = tNodes[tPort1].getInflow();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedT,  influid->getTemperature(), FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMW, influid->getMWeight(),     FLT_EPSILON);

    /// @test negative flow direction and the source node isn't 100% the gas type.
    tNodes[tPort0].resetFlows();
    tNodes[tPort1].resetFlows();
    tArticle->mFlowRate = -0.001;

    expectedQ = -0.001 / tNodes[tPort1].getContent()->getDensity();
    expectedT = liqProps->getSaturationTemperature(tFluidInput1->mPressure);
    tArticle->transportFlows(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedQ, tArticle->mVolFlowRate,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.001,    tNodes[tPort0].getInflux(),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,      tNodes[tPort0].getOutflux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.001,    tNodes[tPort1].getInflux(),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,      tNodes[tPort1].getOutflux(), DBL_EPSILON);
    influid = tNodes[tPort0].getInflow();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedT,  influid->getTemperature(), FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMW, influid->getMWeight(),     FLT_EPSILON);

    /// @test negative flow direction and the source node is 100% the gas type.
    tNodes[tPort0].resetFlows();
    tNodes[tPort1].resetFlows();
    tArticle->mUserPortSelect     = 1;
    tArticle->mUserPortSelectNode = 3;
    tArticle->mUserPortSetControl = GunnsBasicLink::EXECUTE;
    tArticle->step(tTimeStep);
    tArticle->mFlowRate = -0.001;

    expectedQ = -0.001 / tNodes[3].getContent()->getDensity();
    tArticle->transportFlows(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedQ, tArticle->mVolFlowRate,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.001,     tNodes[tPort0].getInflux(),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,       tNodes[tPort0].getOutflux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,       tNodes[3].getInflux(),       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.001,     tNodes[3].getOutflux(),      DBL_EPSILON);
    influid = tNodes[tPort0].getInflow();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedT,  influid->getTemperature(), FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMW, influid->getMWeight(),     FLT_EPSILON);

    /// @test volume flow rate with zero input density.
    tNodes[3].getOutflow()->resetState();
    tArticle->transportFlows(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mVolFlowRate, DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test setter & getter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPhaseChangeSource::testAccessMethods()
{
    UT_RESULT;

    /// @test setPowerInput method.
    tArticle->setPowerInput(500.0);
    CPPUNIT_ASSERT(500.0 == tArticle->mPowerInput);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test port mapping and validity checks for liquid flow-thru links.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPhaseChangeSource::testPortMapping()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test Port 0 fails to move to Ground.
    tArticle->mUserPortSelect     = 0;
    tArticle->mUserPortSetControl = GunnsBasicLink::GROUND;

    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT_EQUAL(GunnsBasicLink::FAILED, tArticle->mUserPortSetControl);
    CPPUNIT_ASSERT_EQUAL(tPort0,                 tArticle->mNodeMap[0]);

    /// @test Port 1 fails to move to Ground.
    tArticle->mUserPortSelect     = 1;
    tArticle->mUserPortSetControl = GunnsBasicLink::GROUND;

    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT_EQUAL(GunnsBasicLink::FAILED, tArticle->mUserPortSetControl);
    CPPUNIT_ASSERT_EQUAL(tPort1,                 tArticle->mNodeMap[1]);

    /// @test Port 0 fails to move to a gas node.
    tArticle->mUserPortSelect     = 0;
    tArticle->mUserPortSelectNode = 3;
    tArticle->mUserPortSetControl = GunnsBasicLink::EXECUTE;

    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT_EQUAL(GunnsBasicLink::FAILED, tArticle->mUserPortSetControl);
    CPPUNIT_ASSERT_EQUAL(tPort0,                 tArticle->mNodeMap[0]);

    /// @test Port 1 fails to move to a liquid node.
    tArticle->mUserPortSelect     = 1;
    tArticle->mUserPortSelectNode = 2;
    tArticle->mUserPortSetControl = GunnsBasicLink::EXECUTE;

    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT_EQUAL(GunnsBasicLink::FAILED, tArticle->mUserPortSetControl);
    CPPUNIT_ASSERT_EQUAL(tPort1,                 tArticle->mNodeMap[1]);

    /// @test Normal move of Port 0 to a liquid node.
    tArticle->mUserPortSelect     = 0;
    tArticle->mUserPortSelectNode = 2;
    tArticle->mUserPortSetControl = GunnsBasicLink::EXECUTE;

    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT_EQUAL(GunnsBasicLink::READY, tArticle->mUserPortSetControl);
    CPPUNIT_ASSERT_EQUAL(2,                     tArticle->mNodeMap[0]);

    /// @test Normal move of Port 1 to a gas node.
    tArticle->mUserPortSelect     = 1;
    tArticle->mUserPortSelectNode = 3;
    tArticle->mUserPortSetControl = GunnsBasicLink::EXECUTE;

    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT_EQUAL(GunnsBasicLink::READY, tArticle->mUserPortSetControl);
    CPPUNIT_ASSERT_EQUAL(3,                     tArticle->mNodeMap[1]);

    UT_PASS_LAST;
}
