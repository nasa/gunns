/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/fluid/conductor/GunnsFluidPhaseChangeConductor.o))
***************************************************************************************************/

#include "core/GunnsFluidUtils.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidPhaseChangeConductor.hh"

/// @details  Test identification number.
int UtGunnsFluidPhaseChangeConductor::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Phase Change Conductor link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidPhaseChangeConductor::UtGunnsFluidPhaseChangeConductor()
    :
    CppUnit::TestFixture(),
    tTypes(),
    tFractions0(),
    tFractions1(),
    tFluidProperties(0),
    tFluidConfig(0),
    tFluidInput0(0),
    tFluidInput1(0),
    tLinks(),
    tName(""),
    tNodes(),
    tNodeList(),
    tPort0(0),
    tPort1(0),
    tMaxConductivity(0.0),
    tLiquidType(),
    tGasType(),
    tEfficiency(0.0),
    tConfigData(0),
    tMalfBlockageFlag(false),
    tMalfBlockageValue(0.0),
    tWallTemperature(0.0),
    tInputData(0),
    tArticle(0),
    tFlowRate(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Phase Change Conductor link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidPhaseChangeConductor::~UtGunnsFluidPhaseChangeConductor()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////

void UtGunnsFluidPhaseChangeConductor::setUp()
{
    /// - Define the nominal port fluids.
    tFluidProperties      = new DefinedFluidProperties();
    tTypes[0]             = FluidProperties::GUNNS_WATER;
    tTypes[1]             = FluidProperties::GUNNS_H2O;
    tTypes[2]             = FluidProperties::GUNNS_NH3;
    tFluidConfig          = new PolyFluidConfigData(tFluidProperties, tTypes, N_FLUIDS);

    tFractions0[0]        = 1.0;
    tFractions0[1]        = 0.0;
    tFractions0[2]        = 0.0;
    tFluidInput0          = new PolyFluidInputData(283.0,
                                                   111.0,
                                                     0.0,
                                                     0.0,
                                                     tFractions0);
    tFractions1[0]        = 0.0;
    tFractions1[1]        = 1.0;
    tFractions1[2]        = 0.0;
    tFluidInput1          = new PolyFluidInputData(300.0,
                                                   109.0,
                                                     0.0,
                                                     0.0,
                                                     tFractions1);

    /// - Initialize the nodes.
    tNodes[0].initialize("UtNode1", tFluidConfig);
    tNodes[1].initialize("UtNode2", tFluidConfig);
    tNodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput0);
    tNodes[1].getContent()->initialize(*tFluidConfig, *tFluidInput1);

    tNodes[0].resetFlows();
    tNodes[1].resetFlows();

    /// - Initialize the nodes list.
    tNodeList.mNodes      = tNodes;
    tNodeList.mNumNodes   = N_NODES;

    /// - Define the nominal configuration data.
    tName                 = "nominal";
    tMaxConductivity      = 20.0;
    tLiquidType           = tTypes[0];
    tGasType              = tTypes[1];
    tEfficiency           = 1.0;
    tConfigData           = new GunnsFluidPhaseChangeConductorConfigData(tName,
                                                                         &tNodeList,
                                                                         tMaxConductivity,
                                                                         tLiquidType,
                                                                         tGasType,
                                                                         tEfficiency);

    /// - Define the nominal input data.
    tMalfBlockageFlag     = false;
    tMalfBlockageValue    = 0.3;
    tWallTemperature      = 300.0;
    tInputData            = new GunnsFluidPhaseChangeConductorInputData(tMalfBlockageFlag,
                                                                        tMalfBlockageValue,
                                                                        tWallTemperature);

    /// - Define the nominal port mapping.
    tPort0                = 0;
    tPort1                = 1;

    /// - Default construct the nominal test article.
    tArticle              = new FriendlyGunnsFluidPhaseChangeConductor;

    /// - Define the nominal flow rate.
    tFlowRate             = 0.3;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPhaseChangeConductor::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete tFluidInput1;
    delete tFluidInput0;
    delete tFluidConfig;
    delete tFluidProperties;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Phase Change Conductor link model construction of configuration.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPhaseChangeConductor::testConfig()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(tName                          == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                         == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductivity               == tConfigData->mMaxConductivity);
    CPPUNIT_ASSERT(tLiquidType                    == tConfigData->mLiquidType);
    CPPUNIT_ASSERT(tGasType                       == tConfigData->mGasType);
    CPPUNIT_ASSERT(tEfficiency                    == tConfigData->mEfficiency);

    /// @test    Configuration data default construction.
    GunnsFluidPhaseChangeConductorConfigData defaultConfig;
    CPPUNIT_ASSERT(0                              == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0                              == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0                            == defaultConfig.mMaxConductivity);
    CPPUNIT_ASSERT(FluidProperties::NO_FLUID      == defaultConfig.mLiquidType);
    CPPUNIT_ASSERT(FluidProperties::NO_FLUID      == defaultConfig.mGasType);
    CPPUNIT_ASSERT(0.0                            == defaultConfig.mEfficiency);

       /// @test    Configuration data copy construction.
    GunnsFluidPhaseChangeConductorConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tConfigData->mName             == copyConfig.mName);
    CPPUNIT_ASSERT(tConfigData->mNodeList->mNodes == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tConfigData->mMaxConductivity  == copyConfig.mMaxConductivity);
    CPPUNIT_ASSERT(tConfigData->mLiquidType       == copyConfig.mLiquidType);
    CPPUNIT_ASSERT(tConfigData->mGasType          == copyConfig.mGasType);
    CPPUNIT_ASSERT(tConfigData->mEfficiency       == copyConfig.mEfficiency);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Phase Change Conductor link model construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPhaseChangeConductor::testInput()
{
    UT_RESULT;

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(tMalfBlockageFlag              == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue             == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tWallTemperature               == tInputData->mWallTemperature);

    /// @test    Input data default construction.
    GunnsFluidPhaseChangeConductorInputData defaultInput;
    CPPUNIT_ASSERT(false                          == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                            == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0                            == defaultInput.mWallTemperature);

    /// @test    Input data copy construction.
    GunnsFluidPhaseChangeConductorInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageValue == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(tInputData->mWallTemperature   == copyInput.mWallTemperature);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Phase Change Conductor link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPhaseChangeConductor::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default of attributes.
    CPPUNIT_ASSERT(""                        == tArticle->mName);
    CPPUNIT_ASSERT(0                         == tArticle->mNodes);
    CPPUNIT_ASSERT(0.0                       == tArticle->mMaxConductivity);
    CPPUNIT_ASSERT(0.0                       == tArticle->mExpansionScaleFactor);
    CPPUNIT_ASSERT(0                         == tArticle->mLiquidFluid);
    CPPUNIT_ASSERT(0                         == tArticle->mGasFluid);
    CPPUNIT_ASSERT(FluidProperties::NO_FLUID == tArticle->mLiquidType);
    CPPUNIT_ASSERT(FluidProperties::NO_FLUID == tArticle->mGasType);
    CPPUNIT_ASSERT(0.0                       == tArticle->mEfficiency);
    CPPUNIT_ASSERT(0.0                       == tArticle->mWallTemperature);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(false == tArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsFluidPhaseChangeConductor* article = new GunnsFluidPhaseChangeConductor();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Phase Change Conductor link model nominal initialization without
///           exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPhaseChangeConductor::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    FriendlyGunnsFluidPhaseChangeConductor article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1));

    /// @test    Nominal configuration data.
    CPPUNIT_ASSERT(tName              == article.mName);
    CPPUNIT_ASSERT(&tNodes[0]         == article.mNodes[0]);
    CPPUNIT_ASSERT(&tNodes[1]         == article.mNodes[1]);
    CPPUNIT_ASSERT(tMaxConductivity   == article.mMaxConductivity);
    CPPUNIT_ASSERT(0.0                == article.mExpansionScaleFactor);
    CPPUNIT_ASSERT(tEfficiency        == article.mEfficiency);
    CPPUNIT_ASSERT(tLiquidType        == article.mLiquidType);
    CPPUNIT_ASSERT(tGasType           == article.mGasType);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT(tMalfBlockageFlag  == article.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue == article.mMalfBlockageValue);
    CPPUNIT_ASSERT(tWallTemperature   == article.mWallTemperature);

    /// @test    Nominal state data.
    CPPUNIT_ASSERT(1.0                == article.mLiquidFluid->getMassFraction(tTypes[0]));
    CPPUNIT_ASSERT(0.0                == article.mLiquidFluid->getMassFraction(tTypes[1]));
    CPPUNIT_ASSERT(0.0                == article.mLiquidFluid->getMassFraction(tTypes[2]));
    CPPUNIT_ASSERT(0.0                == article.mGasFluid->getMassFraction(tTypes[0]));
    CPPUNIT_ASSERT(1.0                == article.mGasFluid->getMassFraction(tTypes[1]));
    CPPUNIT_ASSERT(0.0                == article.mGasFluid->getMassFraction(tTypes[2]));

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(article.mInitFlag);

    /// @test    Double initialization of dynamic fluid objects.
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1));
    CPPUNIT_ASSERT(1.0                == article.mLiquidFluid->getMassFraction(tTypes[0]));
    CPPUNIT_ASSERT(0.0                == article.mLiquidFluid->getMassFraction(tTypes[1]));
    CPPUNIT_ASSERT(0.0                == article.mLiquidFluid->getMassFraction(tTypes[2]));
    CPPUNIT_ASSERT(0.0                == article.mGasFluid->getMassFraction(tTypes[0]));
    CPPUNIT_ASSERT(1.0                == article.mGasFluid->getMassFraction(tTypes[1]));
    CPPUNIT_ASSERT(0.0                == article.mGasFluid->getMassFraction(tTypes[2]));

    /// @test   Verify restartModel functionality
    tArticle->mEffectiveConductivity = 1.0;
    tArticle->mSystemConductance     = 1.0;

    tArticle->restartModel();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,tArticle->mEffectiveConductivity,DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,tArticle->mSystemConductance,DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Phase Change Conductor link model restart.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPhaseChangeConductor::testRestart()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @ test    Base class attributes are reset.
    tArticle->mEffectiveConductivity = 10.0;
    tArticle->restart();
    CPPUNIT_ASSERT(0.0 == tArticle->mEffectiveConductivity);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Phase Change Conductor link model transport fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPhaseChangeConductor::testTransportFluid()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tConfigData->mEfficiency = 0.0;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

//    /// @test   Update fluid with flowrate too small.
//    mArticle->updateFluid(0.0, 0.5 * DBL_EPSILON);
//    CPPUNIT_ASSERT(0.0 == mArticle->mWallHeatFlux);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNodes[0].getContent()->getTemperature(),
//                                 mArticle->mInternalFluid->getTemperature(), DBL_EPSILON);
//
    /// @test   Transport fluid with forward flow.
    tArticle->mPotentialVector[0] = tFluidInput0->mPressure;
    tArticle->mPotentialVector[1] = tFluidInput1->mPressure;
    tArticle->step(0.01);
    tArticle->computeFlows(0.1);
    tArticle->transportFlows(0.1);

    double flowRate = tArticle->mFlowRate;
    CPPUNIT_ASSERT(0.0 < flowRate);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(flowRate, tNodes[0].getOutflux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(flowRate, tNodes[1].getInflux(),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluidInput0->mTemperature, tNodes[1].getInflow()->getTemperature(),
                                                                   FLT_EPSILON);
    CPPUNIT_ASSERT(0.0 == tNodes[1].getInflow()->getMassFraction(tTypes[0]));
    CPPUNIT_ASSERT(1.0 == tNodes[1].getInflow()->getMassFraction(tTypes[1]));
    CPPUNIT_ASSERT(0.0 == tNodes[1].getInflow()->getMassFraction(tTypes[2]));

    /// @test   Transport fluid with reverse flow.
    tNodes[0].resetFlows();
    tNodes[1].resetFlows();
    tNodes[0].setPotential(tFluidInput1->mPressure);
    tNodes[1].setPotential(tFluidInput0->mPressure);
    tArticle->mPotentialVector[0] = tFluidInput1->mPressure;
    tArticle->mPotentialVector[1] = tFluidInput0->mPressure;
    tArticle->step(0.01);
    tArticle->computeFlows(0.1);
    tArticle->transportFlows(0.1);

    flowRate = tArticle->mFlowRate;
    CPPUNIT_ASSERT(0.0 > flowRate);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(-flowRate, tNodes[1].getOutflux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-flowRate, tNodes[0].getInflux(),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluidInput1->mTemperature, tNodes[0].getInflow()->getTemperature(),
                                                                   FLT_EPSILON);
    CPPUNIT_ASSERT(1.0 == tNodes[0].getInflow()->getMassFraction(tTypes[0]));
    CPPUNIT_ASSERT(0.0 == tNodes[0].getInflow()->getMassFraction(tTypes[1]));
    CPPUNIT_ASSERT(0.0 == tNodes[0].getInflow()->getMassFraction(tTypes[2]));

    /// @test    Transport fluid with no flow.
    tNodes[0].resetFlows();
    tNodes[1].resetFlows();
    tNodes[0].setPotential(tFluidInput1->mPressure);
    tNodes[1].setPotential(tFluidInput1->mPressure);
    tArticle->mPotentialVector[0] = tFluidInput1->mPressure;
    tArticle->mPotentialVector[1] = tFluidInput1->mPressure;
    tArticle->step(0.01);
    tArticle->computeFlows(0.1);
    tArticle->transportFlows(0.1);

    flowRate = tArticle->mFlowRate;
    CPPUNIT_ASSERT(0.0 == flowRate);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(flowRate, tNodes[1].getOutflux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(flowRate, tNodes[0].getInflux(),  DBL_EPSILON);

    /// @test    Check difference between content and outflow conditions
    tArticle->step(0.01);
    tArticle->mFlux = 1.0;
    tNodes[0].getContent()->setTemperature(25.0);
    tNodes[0].resetFlows();
    tNodes[0].getContent()->setTemperature(283.0);
    tArticle->transportFlows(0.1);

    CPPUNIT_ASSERT(tNodes[0].getOutflow()->getTemperature()!=
                                    tNodes[0].getContent()->getTemperature());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mGasFluid->getTemperature(),
                                    tNodes[0].getOutflow()->getTemperature(), DBL_EPSILON);

    tNodes[0].resetFlows();
    tNodes[1].resetFlows();

    tArticle->step(0.01);
    tArticle->mFlux = - 1.0;
    tNodes[1].getContent()->setTemperature(50.0);
    tNodes[1].resetFlows();
    tNodes[1].getContent()->setTemperature(300.0);
    tArticle->transportFlows(0.1);

    CPPUNIT_ASSERT(tNodes[1].getOutflow()->getTemperature()!=
                                    tNodes[1].getContent()->getTemperature());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mLiquidFluid->getTemperature(),
                                    tNodes[1].getOutflow()->getTemperature(), DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Phase Change Conductor link model transport fluid with phase-
///           change heat calculations.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPhaseChangeConductor::testTransportFluidWithHeat()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test   Transport fluid with forward flow.
    tArticle->mPotentialVector[0] = tFluidInput0->mPressure;
    tArticle->mPotentialVector[1] = tFluidInput1->mPressure;
    tArticle->setWallTemperature(400.0);
    tArticle->step(0.01);
    tArticle->computeFlows(0.1);
    tArticle->transportFlows(0.1);

    /// - From NIST saturation curve data for H2O @ saturation P = 109 kPa.
    double expectedT  = 375.18;
    double expectedCp = 4218.1;
    double expectedL  = 2678.8 - 427.74;
    double expectedDh = expectedCp * (283.0 - expectedT) - expectedL * 1000.0;
    double expectedQ  = tEfficiency * expectedDh * tArticle->mFlowRate;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedT, tNodes[1].getInflow()->getTemperature(), 1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedQ, tArticle->getWallHeatFlux(),            -0.01 * expectedQ);

    /// @test   Heat flux shuts off for temperature overshoot, forward flow.
    tArticle->setWallTemperature(350.0);
    tArticle->transportFlows(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,       tArticle->getWallHeatFlux(),             DBL_EPSILON);

    /// @test   Transport fluid with reverse flow.
    tNodes[0].resetFlows();
    tNodes[1].resetFlows();
    tNodes[0].setPotential(tFluidInput1->mPressure);
    tNodes[1].setPotential(tFluidInput0->mPressure);
    tArticle->mPotentialVector[0] = tFluidInput1->mPressure;
    tArticle->mPotentialVector[1] = tFluidInput0->mPressure;
    tArticle->step(0.01);
    tArticle->computeFlows(0.1);
    tArticle->transportFlows(0.1);

    /// - From NIST saturation curve data for H2O @ saturation P = 111 kPa.
    expectedT  = 375.7;
    expectedCp = 2090.6;
    expectedL  = 2679.6 - 429.93;
    expectedDh = expectedCp * (300.0 - expectedT) + expectedL * 1000.0;
    expectedQ  = tEfficiency * expectedDh * -tArticle->mFlowRate;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedT, tNodes[0].getInflow()->getTemperature(), 1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedQ, tArticle->getWallHeatFlux(),             0.01 * expectedQ);

    /// @test   Heat flux shuts off for temperature overshoot, reverse flow.
    tArticle->setWallTemperature(400.0);
    tArticle->transportFlows(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,       tArticle->getWallHeatFlux(),             DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Phase Change Conductor link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPhaseChangeConductor::testInitializationExceptions()
{
    UT_RESULT;

    /// - Default construct a test article.
    FriendlyGunnsFluidPhaseChangeConductor article;

    /// @test    Initialization exception invalid config data:no name.
    tConfigData->mName = "";
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mName = tName;

    /// @test    Initialization exception on invalid config data: liquid type not in network.
    tConfigData->mLiquidType = FluidProperties::GUNNS_AMMONIA;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mLiquidType = tLiquidType;

    /// @test    Initialization exception on invalid config data: gas type not in network.
    tConfigData->mGasType = FluidProperties::GUNNS_CO2;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mGasType = tGasType;

    /// @test    Initialization exception on invalid config data: gas & liquid types have unequal
    ///          molecular weight.
    tConfigData->mGasType = FluidProperties::GUNNS_NH3;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mGasType = tGasType;

    /// @test    Initialization exception on invalid config data: gas type isn't a gas.
    tConfigData->mGasType = FluidProperties::GUNNS_WATER;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mGasType = tGasType;

    /// @test    Initialization exception on invalid config data: liquid type isn't a liquid.
    tConfigData->mLiquidType = FluidProperties::GUNNS_H2O;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mLiquidType = tLiquidType;

    /// @test    Initialization exception on invalid init data: port 0 isn't a liquid node.
    tPort0 = 1;
    tPort1 = 2;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);

    /// @test    Initialization exception on invalid init data: port 1 isn't a gas node.
    tPort0 = 2;
    tPort1 = 0;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tPort0 = 0;
    tPort1 = 1;

    /// @test    Initialization exception on invalid input data: wall temperature < 0.
    tInputData->mWallTemperature = -1.0;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tInputData->mWallTemperature = tWallTemperature;

    CPPUNIT_ASSERT(!article.mInitFlag);

    UT_PASS_LAST;
}
