/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    (
        (aspects/fluid/source/GunnsFluidEvaporation.o)
    )
***************************************************************************************************/
#include "UtGunnsFluidEvaporation.hh"

#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsFluidEvaporation class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidEvaporation::UtGunnsFluidEvaporation()
    :
    tAccum(),
    tLinkName(),
    tGasType(),
    tEvaporationCoeff(),
    tPoolMassExponent(),
    tLiquidPoolAccum(),
    tConfigData(),
    tMalfBlockageFlag(),
    tMalfBlockageValue(),
    tGasTotalPressure(),
    tVaporPotential(),
    tEvaporationRate(),
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
/// @details  This is the default destructor for the UtGunnsFluidEvaporation class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidEvaporation::~UtGunnsFluidEvaporation()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidEvaporation::tearDown()
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
void UtGunnsFluidEvaporation::setUp()
{
    tLinkName           = "Test Fluid Evaporation";
    tNodeList.mNumNodes = 3;
    tNodeList.mNodes    = tNodes;
    tPort0              = 0;
    tPort1              = 1;
    tTimeStep           = 0.1;

    /// - Setup some fluid nodes
    tFluidProperties = new DefinedFluidProperties();
    FluidProperties::FluidType types[3];
    types[0]     = FluidProperties::GUNNS_N2;
    types[1]     = FluidProperties::GUNNS_H2O;
    types[2]     = FluidProperties::GUNNS_PG40;
    tFluidConfig = new PolyFluidConfigData(tFluidProperties, types, 3);

    /// - Have to initialize the nodes with the fluid configs (normally done by GUNNS)
    tNodes[0].initialize("UtTestNode0", tFluidConfig);
    tNodes[1].initialize("UtTestNode1", tFluidConfig);
    tNodes[2].initialize("UtTestNode2", tFluidConfig);

    tFractions = new double[3];
    tFractions[0] = 0.999;
    tFractions[1] = 0.001;
    tFractions[2] = 0.0;
    tFluidInput1 = new PolyFluidInputData(294.261,                  //temperature
                                          101.325,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          tFractions);              //massFractions

    tNodes[1].getContent()->initialize(*tFluidConfig, *tFluidInput1);
    tNodes[1].resetFlows();

    tFractions[0] = 0.0;
    tFractions[1] = 0.0;
    tFractions[2] = 1.0;
    tFluidInput2 = new PolyFluidInputData(294.261,                  //temperature
                                          101.325,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          tFractions);              //massFractions

    tNodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput2);
    tNodes[0].resetFlows();

    /// - Define nominal configuration data
    tGasType          = FluidProperties::GUNNS_H2O;
    tEvaporationCoeff = 0.0001;
    tPoolMassExponent = 2.0/3.0;
    tLiquidPoolAccum  = &tAccum;
    tConfigData       = new GunnsFluidEvaporationConfigData(tLinkName,
                                                            &tNodeList,
                                                            tGasType,
                                                            tEvaporationCoeff,
                                                            tPoolMassExponent,
                                                            tLiquidPoolAccum);

    /// - Define nominal input data
    tMalfBlockageFlag  = false;
    tMalfBlockageValue = 1.0;
    tGasTotalPressure  = 101.325;
    tVaporPotential    = 0.0001;
    tEvaporationRate   = 0.00001;
    tInputData         = new GunnsFluidEvaporationInputData(tMalfBlockageFlag,
                                                            tMalfBlockageValue,
                                                            tGasTotalPressure,
                                                            tVaporPotential,
                                                            tEvaporationRate);

    tArticle = new FriendlyGunnsFluidEvaporation;

    /// - Initialize the accumulator link with nominal data.
    GunnsFluidAccumConfigData accumConfig("tAccum", &tNodeList, 1.0e-5, 0.1, 1.0, 0.1, 1.0, 0.016, 5.0, 273.15, 373.15, 200.0);
    GunnsFluidAccumInputData  accumInput(false, 0.0, 0.1, tFluidInput2);
    tAccum.initialize(accumConfig, accumInput, tLinks, 2, tPort0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidEvaporation::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsFluidEvaporation .... 001: testConfig ....................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tLinkName                 == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                    == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tGasType                  == tConfigData->mGasType);
    CPPUNIT_ASSERT(tEvaporationCoeff         == tConfigData->mEvaporationCoeff);
    CPPUNIT_ASSERT(tPoolMassExponent         == tConfigData->mPoolMassExponent);
    CPPUNIT_ASSERT(tLiquidPoolAccum          == tConfigData->mLiquidPoolAccum);

    /// - Check default config construction
    GunnsFluidEvaporationConfigData defaultConfig;
    CPPUNIT_ASSERT(""                        == defaultConfig.mName);
    CPPUNIT_ASSERT(0                         == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(FluidProperties::NO_FLUID == defaultConfig.mGasType);
    CPPUNIT_ASSERT(0.0                       == defaultConfig.mEvaporationCoeff);
    CPPUNIT_ASSERT(0.0                       == defaultConfig.mPoolMassExponent);
    CPPUNIT_ASSERT(0                         == defaultConfig.mLiquidPoolAccum);

    /// - Check copy config construction
    GunnsFluidEvaporationConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tLinkName                 == copyConfig.mName);
    CPPUNIT_ASSERT(tNodes                    == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tGasType                  == copyConfig.mGasType);
    CPPUNIT_ASSERT(tEvaporationCoeff         == copyConfig.mEvaporationCoeff);
    CPPUNIT_ASSERT(tPoolMassExponent         == copyConfig.mPoolMassExponent);
    CPPUNIT_ASSERT(tLiquidPoolAccum          == copyConfig.mLiquidPoolAccum);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidEvaporation::testInput()
{
    std::cout << "\n UtGunnsFluidEvaporation .... 002: testInput .....................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tMalfBlockageFlag  == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tGasTotalPressure  == tInputData->mGasTotalPressure);
    CPPUNIT_ASSERT(tVaporPotential    == tInputData->mVaporPotential);
    CPPUNIT_ASSERT(tEvaporationRate   == tInputData->mEvaporationRate);

    /// - Check default config construction
    GunnsFluidEvaporationInputData defaultInput;
    CPPUNIT_ASSERT(false              == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0                == defaultInput.mGasTotalPressure);
    CPPUNIT_ASSERT(0.0                == defaultInput.mVaporPotential);
    CPPUNIT_ASSERT(0.0                == defaultInput.mEvaporationRate);

    /// - Check copy config construction
    GunnsFluidEvaporationInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tMalfBlockageFlag  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(tGasTotalPressure  == copyInput.mGasTotalPressure);
    CPPUNIT_ASSERT(tVaporPotential    == copyInput.mVaporPotential);
    CPPUNIT_ASSERT(tEvaporationRate   == copyInput.mEvaporationRate);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidEvaporation::testDefaultConstruction()
{
    std::cout << "\n UtGunnsFluidEvaporation .... 003: testDefaultConstruction .......";

    /// @test proper default construction of class member data
    CPPUNIT_ASSERT(FluidProperties::NO_FLUID == tArticle->mGasType);
    CPPUNIT_ASSERT(0.0                       == tArticle->mEvaporationCoeff);
    CPPUNIT_ASSERT(0.0                       == tArticle->mPoolMassExponent);
    CPPUNIT_ASSERT(0                         == tArticle->mLiquidPoolAccum);
    CPPUNIT_ASSERT(0.0                       == tArticle->mGasTotalPressure);
    CPPUNIT_ASSERT(0.0                       == tArticle->mGasMWeight);
    CPPUNIT_ASSERT(0                         == tArticle->mEvaporationFluid);
    CPPUNIT_ASSERT(0.0                       == tArticle->mLiquidPoolMass);

    /// @test init flag
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    /// @test new/delete for code coverage
    GunnsFluidEvaporation* article = new GunnsFluidEvaporation();
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidEvaporation::testNominalInitialization()
{
    std::cout << "\n UtGunnsFluidEvaporation .... 004: testNominalInitialization .....";

    /// - Default construct and initialize (with nominal data) a test article
    FriendlyGunnsFluidEvaporation article;
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Verify the parent method is called
    CPPUNIT_ASSERT(tLinkName          == article.getName());
    CPPUNIT_ASSERT(tMalfBlockageValue == article.mMalfBlockageValue);

    /// - Verify member variables are properly set
    DefinedFluidProperties definedFluidProps;
    const double MW = definedFluidProps.getProperties(FluidProperties::GUNNS_H2O)->getMWeight();
    CPPUNIT_ASSERT(tGasType                   == article.mGasType);
    CPPUNIT_ASSERT(tEvaporationCoeff          == article.mEvaporationCoeff);
    CPPUNIT_ASSERT(tPoolMassExponent          == article.mPoolMassExponent);
    CPPUNIT_ASSERT(tLiquidPoolAccum           == article.mLiquidPoolAccum);
    CPPUNIT_ASSERT(MW                         == article.mGasMWeight);
    CPPUNIT_ASSERT(tGasTotalPressure          == article.mGasTotalPressure);
    CPPUNIT_ASSERT(tVaporPotential            == article.mPotentialDrop);
    CPPUNIT_ASSERT(tEvaporationRate           == article.mFlowRate);
    CPPUNIT_ASSERT(MW                         == article.mEvaporationFluid->getMWeight());
    CPPUNIT_ASSERT(tFluidInput1->mTemperature == article.mEvaporationFluid->getTemperature());

    /// @test init flag
    CPPUNIT_ASSERT(article.mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidEvaporation::testNominalInitNoGasNode()
{
    std::cout << "\n UtGunnsFluidEvaporation .... 005: testNominalInitNoGasNode ......";

    /// - Default construct and initialize (with nominal data) a test article
    FriendlyGunnsFluidEvaporation article;
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, 2);

    /// - Verify the parent method is called
    CPPUNIT_ASSERT(tLinkName          == article.getName());
    CPPUNIT_ASSERT(tMalfBlockageValue == article.mMalfBlockageValue);

    /// - Verify member variables are properly set
    DefinedFluidProperties definedFluidProps;
    const double MW = definedFluidProps.getProperties(FluidProperties::GUNNS_H2O)->getMWeight();
    CPPUNIT_ASSERT(tGasType                   == article.mGasType);
    CPPUNIT_ASSERT(tEvaporationCoeff          == article.mEvaporationCoeff);
    CPPUNIT_ASSERT(tPoolMassExponent          == article.mPoolMassExponent);
    CPPUNIT_ASSERT(tLiquidPoolAccum           == article.mLiquidPoolAccum);
    CPPUNIT_ASSERT(MW                         == article.mGasMWeight);
    CPPUNIT_ASSERT(tGasTotalPressure          == article.mGasTotalPressure);
    CPPUNIT_ASSERT(tVaporPotential            == article.mPotentialDrop);
    CPPUNIT_ASSERT(tEvaporationRate           == article.mFlowRate);
    CPPUNIT_ASSERT(0                          == article.mEvaporationFluid);

    /// @test init flag
    CPPUNIT_ASSERT(article.mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test accessors and getters.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidEvaporation::testInitializationExceptions()
{
    std::cout << "\n UtGunnsFluidEvaporation .... 006: testInitializationExceptions ..";

    /// @test for exception on mass exponent not in limits.
    tConfigData->mPoolMassExponent = -10.01;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mPoolMassExponent = 10.01;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mPoolMassExponent = tPoolMassExponent;

    /// @test for exception on evaporation coeff < 0.
    tConfigData->mEvaporationCoeff = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mEvaporationCoeff = tEvaporationCoeff;

    /// @test for exception on NO_FLUID gas type.
    tConfigData->mGasType = FluidProperties::NO_FLUID;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);

    /// @test for exception on gas type not in network when gas port is connected to non-Ground node.
    tConfigData->mGasType = FluidProperties::GUNNS_CO2;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);

    /// @test for exception on gas type not a gas when gas port is connected to non-Ground node.
    tConfigData->mGasType = FluidProperties::GUNNS_PG40;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mGasType = tGasType;

    /// @test for exception on null accumulator pointer when liquid port is connected to non-Ground
    /// node.
    tConfigData->mLiquidPoolAccum = 0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mLiquidPoolAccum = tLiquidPoolAccum;

    /// @test for exception on initial total gas pressure < 0.
    tInputData->mGasTotalPressure = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tInputData->mGasTotalPressure = tGasTotalPressure;

    /// @test for exception on initial vapor potential < 0.
    tInputData->mVaporPotential = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tInputData->mVaporPotential = tVaporPotential;

    /// @test for exception on initial evaporation rate < 0.
    tInputData->mEvaporationRate = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tInputData->mEvaporationRate = tEvaporationRate;

    /// @test for initialization exception on gas port connected to a liquid node.
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, 2, tPort0),
                         TsInitializationException);

    /// @test for initialization exception on liquid port connected to a gas node.
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort1, 2),
                         TsInitializationException);

    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidEvaporation::testRestart()
{
    std::cout << "\n UtGunnsFluidEvaporation .... 007: testRestart ...................";

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Set some stuff in the article that should be reset on restart.
    tArticle->mLiquidPoolMass = 1.0;

    /// @test restart resets terms.
    tArticle->restart();

    CPPUNIT_ASSERT(0.0 == tArticle->mLiquidPoolMass);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for the step method for the gas side only in the separate networks use case.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidEvaporation::testStepGas()
{
    std::cout << "\n UtGunnsFluidEvaporation .... 008: testStepGas ...................";

    /// - Initialize default test article with nominal initialization data, with liquid port 0
    ///   connected to Ground.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, 2, tPort1);

    /// @test outputs.
    DefinedFluidProperties definedFluidProps;
    const FluidProperties* gasProps = definedFluidProps.getProperties(tGasType);
    const double pSat         = gasProps->getSaturationPressure(tFluidInput1->mTemperature);
    const double ppH2O        = tFluidInput1->mPressure * tNodes[tPort1].getContent()->getMoleFraction(tGasType);
    const double expectedDp   = pSat - ppH2O;
    const double expectedMdot = tEvaporationRate;
    const double expectedPwr  = expectedMdot * 1000.0
                              * gasProps->getHeatOfVaporization(tFluidInput1->mTemperature);
    const double expectedFlux = expectedMdot / gasProps->getMWeight();

    tArticle->mPotentialVector[0] = 0.0;
    tArticle->mPotentialVector[1] = tFluidInput1->mPressure;
    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedDp,   tArticle->mPotentialDrop,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedMdot, tArticle->mFlowRate,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedPwr,  tArticle->mPower,           FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedFlux, tArticle->mFlux,            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedFlux, tArticle->mSourceVector[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedFlux, tArticle->mSourceVector[1], DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for the step method for the liquid side only in the separate networks use case.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidEvaporation::testStepLiquid()
{
    std::cout << "\n UtGunnsFluidEvaporation .... 009: testStepLiquid ................";

    /// - Initialize default test article with nominal initialization data, with gas port 1
    ///   connected to Ground.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, 2);

    /// @test nominal outputs.
    DefinedFluidProperties definedFluidProps;
    const FluidProperties* gasProps = definedFluidProps.getProperties(tGasType);
    const double expectedDp           = tVaporPotential;
    const double expectedSpringCoeff0 = tGasTotalPressure;
    const double expectedSpringCoeff1 = 1.0e-6;
    const double expectedSpringCoeff2 = 0.0;
    const double expectedMpool        = tAccum.getUsableMass();
    const double expectedMdot         = tVaporPotential * tEvaporationCoeff
                                      * powf(expectedMpool, tPoolMassExponent);
    const double expectedPwr          = 0.0;
    const double expectedFlux         = expectedMdot / gasProps->getMWeight();

    tArticle->mPotentialVector[0] = tFluidInput2->mPressure;
    tArticle->mPotentialVector[1] = 0.0;
    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedDp,           tArticle->mPotentialDrop,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedSpringCoeff0, tAccum.mSpringCoeff0,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedSpringCoeff1, tAccum.mSpringCoeff1,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedSpringCoeff2, tAccum.mSpringCoeff2,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedDp,           tArticle->mPotentialDrop,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedMpool,        tArticle->mLiquidPoolMass,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedMdot,         tArticle->mFlowRate,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedPwr,          tArticle->mPower,           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedFlux,         tArticle->mFlux,            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedFlux,         tArticle->mSourceVector[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedFlux,         tArticle->mSourceVector[1], DBL_EPSILON);

    /// @test outputs with the blockage malfunction.
    const double expectedMdotMalf = expectedMdot * 0.5;
    const double expectedFluxMalf = expectedFlux * 0.5;

    tArticle->setMalfBlockage(true, 0.5);
    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedMdotMalf,     tArticle->mFlowRate,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedPwr,          tArticle->mPower,           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedFluxMalf,     tArticle->mFlux,            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedFluxMalf,     tArticle->mSourceVector[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedFluxMalf,     tArticle->mSourceVector[1], DBL_EPSILON);

    /// @test outputs with zero liquid pool mass.
    GunnsFluidAccumConfigData accumConfig("tAccum", &tNodeList, 1.0e-5, 0.1, 1.0, 0.1, 1.0, 0.016, 5.0, 273.15, 373.15, 200.0);
    GunnsFluidAccumInputData  accumInput(false, 0.0, 0.0, tFluidInput2);
    tAccum.initialize(accumConfig, accumInput, tLinks, 2, tPort0);

    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mFlowRate,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mPower,           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mFlux,            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSourceVector[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSourceVector[1], DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for the step method for the gas and liquid sides in the same network use case.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidEvaporation::testStepBoth()
{
    std::cout << "\n UtGunnsFluidEvaporation .... 010: testStepBoth ..................";

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test nominal outputs.
    DefinedFluidProperties definedFluidProps;
    const FluidProperties* gasProps = definedFluidProps.getProperties(tGasType);
    const double pSat                 = gasProps->getSaturationPressure(tFluidInput1->mTemperature);
    const double ppH2O                = tFluidInput1->mPressure * tNodes[tPort1].getContent()->getMoleFraction(tGasType);
    const double expectedDp           = pSat - ppH2O;
    const double expectedSpringCoeff0 = tGasTotalPressure;
    const double expectedSpringCoeff1 = 1.0e-6;
    const double expectedSpringCoeff2 = 0.0;
    const double expectedMpool        = tAccum.getUsableMass();
    const double expectedMdot         = expectedDp * tEvaporationCoeff
                                      * powf(expectedMpool, tPoolMassExponent);
    const double expectedPwr          = expectedMdot * 1000.0
                                      * gasProps->getHeatOfVaporization(tFluidInput1->mTemperature);
    const double expectedFlux         = expectedMdot / gasProps->getMWeight();

    tArticle->mPotentialVector[0] = tFluidInput2->mPressure;
    tArticle->mPotentialVector[1] = tFluidInput1->mPressure;
    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedDp,           tArticle->mPotentialDrop,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedSpringCoeff0, tAccum.mSpringCoeff0,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedSpringCoeff1, tAccum.mSpringCoeff1,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedSpringCoeff2, tAccum.mSpringCoeff2,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedDp,           tArticle->mPotentialDrop,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedMpool,        tArticle->mLiquidPoolMass,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedMdot,         tArticle->mFlowRate,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedPwr,          tArticle->mPower,           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedFlux,         tArticle->mFlux,            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedFlux,         tArticle->mSourceVector[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedFlux,         tArticle->mSourceVector[1], DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows with liquid flow-thru.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidEvaporation::testComputeFlows()
{
    std::cout << "\n UtGunnsFluidEvaporation .... 011: testComputeFlows ..............";

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test nominal outputs.
    DefinedFluidProperties definedFluidProps;
    const FluidProperties* gasProps = definedFluidProps.getProperties(tGasType);
    const double pSat               = gasProps->getSaturationPressure(tFluidInput1->mTemperature);
    const double ppH2O              = tFluidInput1->mPressure * tNodes[tPort1].getOutflow()->getMoleFraction(tGasType);
    const double expectedDp         = pSat - ppH2O;
    const double expectedMpool      = tAccum.getUsableMass();
    const double expectedMdot       = expectedDp * tEvaporationCoeff
                                    * powf(expectedMpool, tPoolMassExponent);
    const double expectedPwr        = expectedMdot * 1000.0
                                    * gasProps->getHeatOfVaporization(tFluidInput1->mTemperature);
    const double expectedFlux       = expectedMdot / gasProps->getMWeight();
    const double expectedQ          = expectedMdot / tNodes[1].getOutflow()->getMassFraction(1)
                                    / tNodes[1].getOutflow()->getDensity();
    const double expectedPtotal     = tFluidInput1->mPressure;
    const double expectedT          = tFluidInput1->mTemperature;
    const double expectedMdotLiquid = expectedFlux * tNodes[0].getOutflow()->getMWeight();

    tArticle->mPotentialVector[0] = tFluidInput2->mPressure;
    tArticle->mPotentialVector[1] = tFluidInput1->mPressure;
    tArticle->step(tTimeStep);
    tArticle->computeFlows(tTimeStep);
    tArticle->transportFlows(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedQ,          tArticle->mVolFlowRate,                        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPtotal,     tArticle->mGasTotalPressure,                   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedT,          tArticle->mEvaporationFluid->getTemperature(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedT,          tNodes[1].getInflow()->getTemperature(),       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot,       tNodes[1].getInflux(),                         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                tNodes[1].getOutflux(),                        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                tNodes[0].getInflux(),                         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdotLiquid, tNodes[0].getOutflux(),                        DBL_EPSILON);

    /// - Re-init the gas node with zero water vapor.
    delete tFluidInput1;
    tFractions[0] = 1.0;
    tFractions[1] = 0.0;
    tFractions[2] = 0.0;
    tFluidInput1 = new PolyFluidInputData(294.261,                  //temperature
                                          101.325,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          tFractions);              //massFractions
    tNodes[1].getContent()->initialize(*tFluidConfig, *tFluidInput1);
    tNodes[1].resetFlows();

    /// @test zero water vapor density in gas node.
    tArticle->step(tTimeStep);
    tArticle->computeFlows(tTimeStep);

    /// - Confirm correct source port allocation with forward flow (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tArticle->mPortDirections[0]);

    /// - Confirm correct source port allocation with forward flow (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::SINK == tArticle->mPortDirections[1]);

    tArticle->transportFlows(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mVolFlowRate,                        DBL_EPSILON);

    tArticle->step(tTimeStep);
    tArticle->mFlux = -0.6;
    tArticle->computeFlows(tTimeStep);

    /// - Confirm correct source port allocation with reverse flow (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::SINK == tArticle->mPortDirections[0]);

    /// - Confirm correct source port allocation with reverse flow (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tArticle->mPortDirections[1]);

    tArticle->step(tTimeStep);
    tArticle->mFlux = 0.0;
    tArticle->computeFlows(tTimeStep);

    /// - Confirm correct source port allocation with no flow (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tArticle->mPortDirections[0]);

    /// - Confirm correct source port allocation with no flow (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tArticle->mPortDirections[1]);

    /// - Test transportFlows EvaporationFluid temp
    tNodes[1].getContent()->setTemperature(200.0);
    tNodes[1].resetFlows();
    tNodes[1].getContent()->setTemperature(294.261);
    tArticle->mFlowRate = 1.0;
    tArticle->transportFlows(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[1].getOutflow()->getTemperature(),tArticle->mEvaporationFluid->getTemperature(),DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mFlowRate, tNodes[1].getInflux(), DBL_EPSILON);
    double Fractions[3];
    Fractions[0] = 0.5;
    Fractions[1] = 0.5;
    Fractions[2] = 0.0;
    tNodes[0].getContent()->setMassAndMassFractions(2.0,Fractions);
    tNodes[0].resetFlows();
    tNodes[0].getContent()->setMassAndMassFractions(0.0,tFractions);
    tArticle->mFlux = 1.0;
    tArticle->transportFlows(tTimeStep);

    double tempLiqFlowRate = tArticle->mFlux * tNodes[0].getOutflow()->getMWeight();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tempLiqFlowRate, tNodes[0].getOutflux(), DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test setter & getter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidEvaporation::testAccessMethods()
{
    std::cout << "\n UtGunnsFluidEvaporation .... 012: testAccessMethods .............";

    /// - There are currently no access methods to test.

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test port mapping and validity checks for liquid flow-thru links.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidEvaporation::testPortMapping()
{
    std::cout << "\n UtGunnsFluidEvaporation .... 013: testPortMapping ...............";

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test A valid gas port 1 move to Ground.
    tArticle->mUserPortSelect     = 1;
    tArticle->mUserPortSetControl = GunnsBasicLink::GROUND;

    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT_EQUAL(GunnsBasicLink::READY,           tArticle->mUserPortSetControl);
    CPPUNIT_ASSERT_EQUAL(2,                               tArticle->mNodeMap[1]);

    /// @test An invalid liquid port 0 move to a gas node.
    tArticle->mUserPortSelect     = 0;
    tArticle->mUserPortSelectNode = 1;
    tArticle->mUserPortSetControl = GunnsBasicLink::EXECUTE;

    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT_EQUAL(GunnsBasicLink::FAILED,          tArticle->mUserPortSetControl);
    CPPUNIT_ASSERT_EQUAL(tPort0,                          tArticle->mNodeMap[0]);

    /// @test A valid liquid port 0 move to Ground.
    tArticle->mGasType = FluidProperties::NO_FLUID;
    tArticle->mUserPortSelect     = 0;
    tArticle->mUserPortSetControl = GunnsBasicLink::GROUND;

    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT_EQUAL(GunnsBasicLink::READY,           tArticle->mUserPortSetControl);
    CPPUNIT_ASSERT_EQUAL(2,                               tArticle->mNodeMap[0]);

    /// @test An invalid gas port 1 move to a liquid node.
    tArticle->mUserPortSelect     = 1;
    tArticle->mUserPortSelectNode = 0;
    tArticle->mUserPortSetControl = GunnsBasicLink::EXECUTE;

    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT_EQUAL(GunnsBasicLink::FAILED,          tArticle->mUserPortSetControl);
    CPPUNIT_ASSERT_EQUAL(2,                               tArticle->mNodeMap[1]);

    /// @test An invalid liquid port 0 move to a liquid node with null accumulator link pointer.
    tArticle->mLiquidPoolAccum    = 0;
    tArticle->mUserPortSelect     = 0;
    tArticle->mUserPortSelectNode = 0;
    tArticle->mUserPortSetControl = GunnsBasicLink::EXECUTE;

    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT_EQUAL(GunnsBasicLink::FAILED,          tArticle->mUserPortSetControl);
    CPPUNIT_ASSERT_EQUAL(2,                               tArticle->mNodeMap[0]);

    std::cout << "... Pass";
    std::cout << "\n -----------------------------------------------------------------------------";
}
