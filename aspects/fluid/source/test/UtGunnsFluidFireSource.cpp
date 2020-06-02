/**
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
(
   (aspects/fluid/source/GunnsFluidFireSource.o)
)
*/

#include "strings/UtResult.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "UtGunnsFluidFireSource.hh"

/// @details  Test identification number.
int UtGunnsFluidFireSource::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsFluidFireSource class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidFireSource::UtGunnsFluidFireSource()
    :
    tConfigData(),
    tInputData(),
    tArticle(),
    tFireFlag(),
    tHeatOutput(),
    tO2ConsumpRate(),
    tCO2ProductRate(),
    tH2OProductRate(),
    tMinReqO2(),
    tLinkName(),
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
/// @details  This is the default destructor for the UtGunnsFluidFireSource class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidFireSource::~UtGunnsFluidFireSource()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFireSource::tearDown()
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
void UtGunnsFluidFireSource::setUp()
{
    tLinkName           = "Test Fluid Fire Source";
    tNodeList.mNumNodes = 3;
    tNodeList.mNodes    = tNodes;
    tFireFlag           = true;
    tHeatOutput         = 100;
    tO2ConsumpRate      = 1e-2;
    tCO2ProductRate     = 1e-2;
    tH2OProductRate     = 1e-2;
    tMinReqO2           = 10.34213592;
    tTcRatesState       = new double[2];
    tTcRatesState[0]    = 1.0e-3;
    tTcRatesState[1]    = 2.0e-3;
    tTcInput            = new GunnsFluidTraceCompoundsInputData(tTcRatesState);
    tPort0              = 0;
    tTcTypes[0]         = ChemicalCompound::CO;
    tTcTypes[1]         = ChemicalCompound::NH3;
    tTimeStep           = 0.1;
    tTolerance          = 0.01;

    /// - Setup some fluid nodes
    tFluidProperties = new DefinedFluidProperties();
    FluidProperties::FluidType types[4];
    types[0]    = FluidProperties::GUNNS_N2;
    types[1]    = FluidProperties::GUNNS_O2;
    types[2]    = FluidProperties::GUNNS_H2O;
    types[3]    = FluidProperties::GUNNS_CO2;
    tFractions1[0] = 0.7;
    tFractions1[1] = 0.25;
    tFractions1[2] = 0.025;
    tFractions1[3] = 0.025;

    tCompoundProperties = new DefinedChemicalCompounds();
    tFluidTcConfig = new GunnsFluidTraceCompoundsConfigData(tTcTypes, 2, "tFluidTcConfig");
    tFluidConfig = new PolyFluidConfigData(tFluidProperties, types, 4, tFluidTcConfig);

    double tcConcentrations[2];
    tcConcentrations[0] = 5.0e-3;
    tcConcentrations[1] = 6.0e-3;
    tFluidTcInput = new GunnsFluidTraceCompoundsInputData(tcConcentrations);

    tFluidInput1 = new PolyFluidInputData(283.15,                   //temperature
                                          700.728,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          tFractions1,              //massFractions
                                          tFluidTcInput);           //trace compounds

    tFractions2[0] = 0.0;
    tFractions2[1] = 1.0;
    tFractions2[2] = 0.0;
    tFractions2[3] = 0.0;
    tFluidInput2 = new PolyFluidInputData(283.15,                   //temperature
                                          700.728,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          tFractions2,              //massFractions
                                          tTcInput);



    //trace compounds

    /// - Have to initialize the nodes with the fluid configs (normally done by GUNNS)
    tNodes[0].initialize("UtTestNode0", tFluidConfig);
    tNodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput1);
    tNodes[0].resetFlows();

    /// - Define nominal configuration data
    tConfigData = new GunnsFluidFireSourceConfigData(tLinkName, &tNodeList, tO2ConsumpRate,
                                                         tCO2ProductRate, tH2OProductRate,
                                                         tMinReqO2, tTcRatesState);


    /// - Define default input data
    tInputData = new GunnsFluidFireSourceInputData(false, 0.0, tFireFlag, tHeatOutput);

    /// - Default construct the nominal test article.
    tArticle = new FriendlyGunnsFluidFireSource;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFireSource::testConfig()
{
    UT_RESULT_FIRST;

    /// - Check nominal config construction
    GunnsFluidFireSourceConfigData nominalConfig(tLinkName, &tNodeList, tO2ConsumpRate,
                                                 tCO2ProductRate, tH2OProductRate, tMinReqO2,
                                                 tTcRatesState);
    CPPUNIT_ASSERT(tLinkName        == nominalConfig.mName);
    CPPUNIT_ASSERT(tNodes           == nominalConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tCO2ProductRate  == nominalConfig.mFireCO2ProductRate);
    CPPUNIT_ASSERT(tH2OProductRate  == nominalConfig.mFireH2OProductRate);
    CPPUNIT_ASSERT(tO2ConsumpRate   == nominalConfig.mFireO2ConsumpRate);
    CPPUNIT_ASSERT(tMinReqO2        == nominalConfig.mFireMinReqO2);
    CPPUNIT_ASSERT(tTcRatesState[0] == nominalConfig.mFireTraceCompoundRates[0]);
    CPPUNIT_ASSERT(tTcRatesState[1] == nominalConfig.mFireTraceCompoundRates[1]);

    /// - Check default config construction
    GunnsFluidFireSourceConfigData defaultConfig;
    CPPUNIT_ASSERT(""               == defaultConfig.mName);
    CPPUNIT_ASSERT(0                == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0              == defaultConfig.mFireCO2ProductRate);
    CPPUNIT_ASSERT(0.0              == defaultConfig.mFireH2OProductRate);
    CPPUNIT_ASSERT(0.0              == defaultConfig.mFireO2ConsumpRate);
    CPPUNIT_ASSERT(0.0              == defaultConfig.mFireMinReqO2);
    CPPUNIT_ASSERT(0                == defaultConfig.mFireTraceCompoundRates);

    /// - Check copy config construction
    GunnsFluidFireSourceConfigData copyConfig(nominalConfig);
    CPPUNIT_ASSERT(tLinkName        == copyConfig.mName);
    CPPUNIT_ASSERT(tNodes           == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tCO2ProductRate  == copyConfig.mFireCO2ProductRate);
    CPPUNIT_ASSERT(tH2OProductRate  == copyConfig.mFireH2OProductRate);
    CPPUNIT_ASSERT(tO2ConsumpRate   == copyConfig.mFireO2ConsumpRate);
    CPPUNIT_ASSERT(tMinReqO2        == copyConfig.mFireMinReqO2);
    CPPUNIT_ASSERT(tTcRatesState[0] == copyConfig.mFireTraceCompoundRates[0]);
    CPPUNIT_ASSERT(tTcRatesState[1] == copyConfig.mFireTraceCompoundRates[1]);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFireSource::testInput()
{
    UT_RESULT;

    /// - Check nominal input construction
    CPPUNIT_ASSERT(false       == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0         == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tFireFlag   == tInputData->mMalfFireFlag);
    CPPUNIT_ASSERT(tHeatOutput == tInputData->mMalfFireHeat);

    /// - Check default input construction
    GunnsFluidFireSourceInputData defaultInput;
    CPPUNIT_ASSERT(false       == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0         == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(false       == defaultInput.mMalfFireFlag);
    CPPUNIT_ASSERT(0.0         == defaultInput.mMalfFireHeat);

    /// - Check copy input construction
    GunnsFluidFireSourceInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(false       == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0         == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(tFireFlag   == copyInput.mMalfFireFlag);
    CPPUNIT_ASSERT(tHeatOutput == copyInput.mMalfFireHeat);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFireSource::testDefaultConstruction()
{
    UT_RESULT;

    /// @test state data
    CPPUNIT_ASSERT(false == tArticle->mMalfFireFlag);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfFireHeat);
    CPPUNIT_ASSERT(0.0   == tArticle->mFireO2ConsumpRate);
    CPPUNIT_ASSERT(0     == tArticle->miO2);
    CPPUNIT_ASSERT(0.0   == tArticle->mFlowO2);
    CPPUNIT_ASSERT(0.0   == tArticle->mFireCO2ProductRate);
    CPPUNIT_ASSERT(0     == tArticle->miCO2);
    CPPUNIT_ASSERT(0.0   == tArticle->mFlowCO2);
    CPPUNIT_ASSERT(0.0   == tArticle->mFireH2OProductRate);
    CPPUNIT_ASSERT(0     == tArticle->miH2O);
    CPPUNIT_ASSERT(0.0   == tArticle->mFlowH2O);
    CPPUNIT_ASSERT(0     == tArticle->mTraceCompoundRates);
    CPPUNIT_ASSERT(0     == tArticle->mTraceCompoundMassRates);
    CPPUNIT_ASSERT(0     == tArticle->mFireMinReqO2);
    /// @test init flag
    CPPUNIT_ASSERT(false == tArticle->mInitFlag);

    /// @test new/delete for code coverage
    GunnsFluidFireSource* article = new GunnsFluidFireSource();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFireSource::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize the test article with nominal data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);

    /// @test config, input & state data
    CPPUNIT_ASSERT(tFireFlag          == tArticle->mMalfFireFlag);
    CPPUNIT_ASSERT(tHeatOutput        == tArticle->mMalfFireHeat);
    CPPUNIT_ASSERT(tArticle->mInternalFluid);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluidInput1->mTemperature,
                                 tArticle->mInternalFluid->getTemperature(),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFractions1[0],
                                 tArticle->mInternalFluid->getMassFraction(FluidProperties::GUNNS_N2),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFractions1[1],
                                 tArticle->mInternalFluid->getMassFraction(FluidProperties::GUNNS_O2),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFractions1[2],
                                 tArticle->mInternalFluid->getMassFraction(FluidProperties::GUNNS_CO2),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFractions1[3],
                                 tArticle->mInternalFluid->getMassFraction(FluidProperties::GUNNS_H2O),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tO2ConsumpRate,
                                 tArticle->mFireO2ConsumpRate,
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCO2ProductRate,
                                 tArticle->mFireCO2ProductRate,
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tH2OProductRate,
                                 tArticle->mFireH2OProductRate,
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMinReqO2,
                                 tArticle->mFireMinReqO2,
                                 DBL_EPSILON);

    CPPUNIT_ASSERT(tArticle->miO2    == 1);
    CPPUNIT_ASSERT(tArticle->miCO2   == 3);
    CPPUNIT_ASSERT(tArticle->miH2O   == 2);

    CPPUNIT_ASSERT(tArticle->mTraceCompoundRates);
    CPPUNIT_ASSERT(tArticle->mTraceCompoundRates[0] == tTcRatesState[0]);
    CPPUNIT_ASSERT(tArticle->mTraceCompoundRates[1] == tTcRatesState[1]);

    /// @test init flag
    CPPUNIT_ASSERT(true               == tArticle->mInitFlag);

    delete tConfigData;
    /// - Define nominal configuration data with no TC Rates
    tConfigData = new GunnsFluidFireSourceConfigData(tLinkName, &tNodeList, tO2ConsumpRate,
                                                             tCO2ProductRate, tH2OProductRate,
                                                             tMinReqO2);
    delete tArticle;
    tArticle = new FriendlyGunnsFluidFireSource;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);
    CPPUNIT_ASSERT(tFireFlag      == tArticle->mMalfFireFlag);
    CPPUNIT_ASSERT(tHeatOutput == tArticle->mMalfFireHeat);
    CPPUNIT_ASSERT(tArticle->mInternalFluid);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluidInput1->mTemperature,
                                 tArticle->mInternalFluid->getTemperature(),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFractions1[0],
                                 tArticle->mInternalFluid->getMassFraction(FluidProperties::GUNNS_N2),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFractions1[1],
                                 tArticle->mInternalFluid->getMassFraction(FluidProperties::GUNNS_O2),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFractions1[2],
                                 tArticle->mInternalFluid->getMassFraction(FluidProperties::GUNNS_CO2),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFractions1[3],
                                 tArticle->mInternalFluid->getMassFraction(FluidProperties::GUNNS_H2O),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tO2ConsumpRate,
                                 tArticle->mFireO2ConsumpRate,
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCO2ProductRate,
                                 tArticle->mFireCO2ProductRate,
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tH2OProductRate,
                                 tArticle->mFireH2OProductRate,
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMinReqO2,
                                 tArticle->mFireMinReqO2,
                                 DBL_EPSILON);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 tArticle->mTraceCompoundRates[0],
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 tArticle->mTraceCompoundRates[1],
                                 DBL_EPSILON);

    CPPUNIT_ASSERT(tArticle->miO2    == 1);
    CPPUNIT_ASSERT(tArticle->miCO2   == 3);
    CPPUNIT_ASSERT(tArticle->miH2O   == 2);
    CPPUNIT_ASSERT(true              == tArticle->mInitFlag);

    delete tConfigData;
    /// - Define nominal configuration data with no TC Rates
    tConfigData = new GunnsFluidFireSourceConfigData(tLinkName, &tNodeList, tO2ConsumpRate,
                                                             tCO2ProductRate, tH2OProductRate,
                                                             tMinReqO2, tTcRatesState);

    /// @test initialization for no trace compounds in network
    tFluidConfig->mTraceCompounds = 0;
    GunnsFluidNode nodes[2];
    tNodeList.mNodes    = nodes;
    tFluidInput1->mTraceCompounds = 0;
    nodes[0].initialize("nodes_0", tFluidConfig);
    nodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput1);
    nodes[0].resetFlows();

    delete tArticle;
    tArticle = new FriendlyGunnsFluidFireSource;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);

    CPPUNIT_ASSERT(tFireFlag      == tArticle->mMalfFireFlag);
    CPPUNIT_ASSERT(tHeatOutput == tArticle->mMalfFireHeat);
    CPPUNIT_ASSERT(tArticle->mInternalFluid);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluidInput1->mTemperature,
                                 tArticle->mInternalFluid->getTemperature(),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFractions1[0],
                                 tArticle->mInternalFluid->getMassFraction(FluidProperties::GUNNS_N2),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFractions1[1],
                                 tArticle->mInternalFluid->getMassFraction(FluidProperties::GUNNS_O2),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFractions1[2],
                                 tArticle->mInternalFluid->getMassFraction(FluidProperties::GUNNS_CO2),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFractions1[3],
                                 tArticle->mInternalFluid->getMassFraction(FluidProperties::GUNNS_H2O),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tO2ConsumpRate,
                                 tArticle->mFireO2ConsumpRate,
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCO2ProductRate,
                                 tArticle->mFireCO2ProductRate,
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tH2OProductRate,
                                 tArticle->mFireH2OProductRate,
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMinReqO2,
                                 tArticle->mFireMinReqO2,
                                 DBL_EPSILON);

    CPPUNIT_ASSERT(tArticle->miO2    == 1);
    CPPUNIT_ASSERT(tArticle->miCO2   == 3);
    CPPUNIT_ASSERT(tArticle->miH2O   == 2);
    CPPUNIT_ASSERT(!tArticle->mTraceCompoundRates);
    CPPUNIT_ASSERT(true             == tArticle->mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for exceptions thrown during initialization.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFireSource::testInitializationExceptions()
{
    UT_RESULT;

    /// - Test throw on sum of bulk flow rates equal zero.
    GunnsFluidFireSourceConfigData* tZeroConfigData =
            new GunnsFluidFireSourceConfigData(tLinkName, &tNodeList, 0.0, 0.0, 0.0, tMinReqO2,
                                               tTcRatesState);

    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tZeroConfigData, *tInputData, tLinks, tPort0),
            TsInitializationException);

    /// - Setup a water (liquid) polyfluid to initialize a node as liquid.
    DefinedFluidProperties* tWaterProperties = new DefinedFluidProperties();
    FluidProperties::FluidType type[1];
    type[0]    = FluidProperties::GUNNS_WATER;
    double tfrac[1];
    tfrac[0] = 1.0;

    PolyFluidConfigData* tWaterConfig = new PolyFluidConfigData(tFluidProperties, type, 1);

    PolyFluidInputData* tWaterInput1 = new PolyFluidInputData(283.15,             //temperature
                                                              700.728,            //pressure
                                                              0.0,                //flowRate
                                                              0.0,                //mass
                                                              tfrac);             //massFractions

    /// - Have to initialize the nodes with the fluid configs (normally done by GUNNS)
    tNodes[2].initialize("UtTestNode0", tWaterConfig);
    tNodes[2].getContent()->initialize(*tWaterConfig, *tWaterInput1);

    /// - Test throw on attaching the link to a liquid node.
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, 2),
            TsInitializationException);
    delete tWaterInput1;
    delete tWaterConfig;
    delete tWaterProperties;
    delete tZeroConfigData;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFireSource::testRestart()
{
    UT_RESULT;

    /// - Initialize the test article with nominal data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);

    tArticle->mFlowO2  = 1.0;
    tArticle->mFlowCO2 = 2.0;
    tArticle->mFlowH2O = 3.0;

    /// @test reset of non-config and non-checkpointed attributes.
    tArticle->restart();

    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mFlowO2);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mFlowCO2);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mFlowH2O);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test accessors and getters.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFireSource::testAccessorsAndGetters()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);

    /// @test setting the fire malfunction.
    tArticle->setMalfFire(true, 200.0);
    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mMalfFireFlag);
    CPPUNIT_ASSERT_EQUAL(200.0, tArticle->mMalfFireHeat);

    /// @test resetting the fire malfunction.
    tArticle->setMalfFire();
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mMalfFireFlag);
    CPPUNIT_ASSERT_EQUAL(0.0,   tArticle->mMalfFireHeat);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the step method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFireSource::testStep()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);

    /// - Nominal step with no blockage.
    tArticle->step(tTimeStep);
    double expectedMdot = (tCO2ProductRate + tH2OProductRate - tO2ConsumpRate) *tHeatOutput;
    double expectedFlux = expectedMdot / tArticle->mInternalFluid->getMWeight();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot, tArticle->mFlowRate,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mFlux,            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mSourceVector[0], DBL_EPSILON);

    /// - Step again with the blockage malfunction activated and flow sign not flipped.
    tArticle->setMalfBlockage(true, 0.2);
    tArticle->step(tTimeStep);
    expectedMdot = 0.8 * (tCO2ProductRate + tH2OProductRate - tO2ConsumpRate) * tHeatOutput;
    expectedFlux = expectedMdot / tArticle->mInternalFluid->getMWeight();
    CPPUNIT_ASSERT(true == tArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.2  == tArticle->mMalfBlockageValue);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot, tArticle->mFlowRate,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mFlux,            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mSourceVector[0], DBL_EPSILON);

    /// - Step with no blockage and not enough oxygen
    tArticle->setMalfBlockage(false, 0.0);
    double temp_reqO2 = tArticle->mFireMinReqO2;
    tArticle->mFireMinReqO2 = tArticle->mNodes[0]->getContent()->getPartialPressure(FluidProperties::GUNNS_O2)*2.0;
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT(tArticle->mMalfFireFlag == false);
    tArticle->mFireMinReqO2 = temp_reqO2;

    /// - Step again with zero molecular weight in the internal fluid.  We do this by hacking the
    ///   link's internal fluid to point to the Ground node's contents, which hasn't been init'd.
    tArticle->setMalfBlockage(false, 0.0);
    tArticle->setMalfFire(true, tHeatOutput);
    PolyFluid* saveFluid     = tArticle->mInternalFluid;
    tArticle->mInternalFluid = tNodes[1].getContent();
    tArticle->step(tTimeStep);
    tArticle->mInternalFluid = saveFluid;
    expectedMdot = (tCO2ProductRate + tH2OProductRate - tO2ConsumpRate) * tHeatOutput;
    expectedFlux = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot, tArticle->mFlowRate,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mFlux,            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mSourceVector[0], DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows with flow out of the node.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFireSource::testComputeFlowsFromNode()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data, reverse flow and no
    ///   blockage.
    tInputData->mMalfBlockageFlag = false;
    tInputData->mMalfFireHeat       = tHeatOutput;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);

    tArticle->step(tTimeStep);
    tArticle->computeFlows(tTimeStep);
    tArticle->transportFlows(tTimeStep);

    const double expectedP   = tNodes[0].getPotential();
    const double expectedQ   = (tCO2ProductRate + tH2OProductRate - tO2ConsumpRate + tTcRatesState[0] + tTcRatesState[1]) * tHeatOutput / tArticle->getInternalFluid()->getDensity();
    const double expectedPwr = tInputData->mMalfFireHeat;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP,                 tArticle->mPotentialDrop,                DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedPwr,               tArticle->mPower,                        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( (tCO2ProductRate + tH2OProductRate - tO2ConsumpRate) * tHeatOutput,        tNodes[0].getInflux(),                   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluidInput2->mTemperature, tNodes[0].getInflow()->getTemperature(), FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,                       tNodes[0].getOutflux(),                  DBL_EPSILON);

    /// - Test extinguishing of fire due to low O2.
    tArticle->mFireMinReqO2 = tArticle->mNodes[0]->getContent()->getPartialPressure(FluidProperties::GUNNS_O2)*2.0;

    tArticle->step(tTimeStep);
    tArticle->computeFlows(tTimeStep);
    CPPUNIT_ASSERT(GunnsBasicLink::SINK == tArticle->getPortDirections()[0]);
    tArticle->transportFlows(tTimeStep);
    CPPUNIT_ASSERT(tArticle->mMalfFireFlag == false);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mFlowO2,  0.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mFlowCO2, 0.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mFlowH2O, 0.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mPower,   0.0, DBL_EPSILON);

    tArticle->step(tTimeStep);
    tArticle->computeFlows(tTimeStep);
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tArticle->getPortDirections()[0]);

    UT_PASS_LAST;
}
