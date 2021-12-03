/*
@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/

#include "core/GunnsFluidUtils.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidSimpleH2Redox.hh"

/// @details  Test identification number.
int UtGunnsFluidSimpleH2Redox::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Simple H2 Redox link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidSimpleH2Redox::UtGunnsFluidSimpleH2Redox()
    :
    CppUnit::TestFixture(),
    tFluidProperties(0),
    tTypes(),
//    mFractions(),
    tFluidConfig(0),
    tFluidInput0(0),
    tFluidInput1(0),
    tFluidInput2(0),
    tLinks(),
    tName(""),
    tNodes(),
    tNodeList(),
    tPort0(0),
    tPort1(0),
    tNumCells(0),
    tCellVoltageLoaded(0.0),
    tCellH2ReactRate(0.0),
    tMaxEfficiency(0.0),
    tConfigData(0),
    tMalfBlockageFlag(false),
    tMalfBlockageValue(0.0),
    tCurrent(0.0),
    tTrippedOff(false),
    tInputData(0),
//    mFluidTemperature(0.0),
//    mWallHeatFlux(0.0),
//    mMass(0.0),
//    mSorbtionFlowRate(0.0),
    tArticle(0),
//    mFlowRate(0.0),
    tTimeStep(0.0)
//    mTolerance(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Simple H2 Redox link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidSimpleH2Redox::~UtGunnsFluidSimpleH2Redox()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////

void UtGunnsFluidSimpleH2Redox::setUp()
{
    /// - Define the nominal port fluids.
    tFluidProperties = new DefinedFluidProperties();
    tTypes[0]        = FluidProperties::GUNNS_H2;
    tTypes[1]        = FluidProperties::GUNNS_O2;
    tTypes[2]        = FluidProperties::GUNNS_N2;
    tTypes[3]        = FluidProperties::GUNNS_H2O;
    tTypes[4]        = FluidProperties::GUNNS_WATER;
    tFluidConfig     = new PolyFluidConfigData(tFluidProperties, tTypes, N_FLUIDS);

    double fractions0[] = {0.6, 0.0, 0.1, 0.3, 0.0};
    tFluidInput0        = new PolyFluidInputData(294.261,
                                                 200.0,
                                                   0.0,
                                                   0.0,
                                                 fractions0);

    double fractions1[] = {0.0, 0.9, 0.1, 0.0, 0.0};
    tFluidInput1        = new PolyFluidInputData(294.261,
                                                 200.0,
                                                   0.0,
                                                   0.0,
                                                 fractions1);

    double fractions2[] = {0.0, 0.0, 0.0, 0.0, 1.0};
    tFluidInput2        = new PolyFluidInputData(294.261,
                                                 200.0,
                                                   0.0,
                                                   0.0,
                                                 fractions2);

    /// - Initialize the nodes.
    tNodes[0].initialize("UtNode0", tFluidConfig);
    tNodes[1].initialize("UtNode1", tFluidConfig);
    tNodes[2].initialize("UtNode2", tFluidConfig);
    tNodes[3].initialize("UtNode3", tFluidConfig);
    tNodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput0);
    tNodes[1].getContent()->initialize(*tFluidConfig, *tFluidInput1);
    tNodes[2].getContent()->initialize(*tFluidConfig, *tFluidInput0);
    tNodes[3].getContent()->initialize(*tFluidConfig, *tFluidInput2);

    /// - Initialize the nodes list.
    tNodeList.mNodes    = tNodes;
    tNodeList.mNumNodes = N_NODES;

    /// - Define the nominal configuration data.
    tName              = "tArticle";
    tNumCells          = 30;
    tCellVoltageLoaded = 0.8;
    tCellH2ReactRate   = 1.0e-5;
    tMaxEfficiency     = 0.9;
    tConfigData        = new GunnsFluidSimpleH2RedoxConfigData(tName,
                                                               &tNodeList,
                                                               tNumCells,
                                                               tCellVoltageLoaded,
                                                               tCellH2ReactRate,
                                                               tMaxEfficiency);

    /// - Define the nominal input data.
    tMalfBlockageFlag  = true;
    tMalfBlockageValue = 0.3;
    tCurrent           = 0.5;
    tTrippedOff        = false;
    tInputData         = new GunnsFluidSimpleH2RedoxInputData(tMalfBlockageFlag,
                                                              tMalfBlockageValue,
                                                              tCurrent,
                                                              tTrippedOff);

    /// - Define the nominal port mapping.
    tPort0 = 0;
    tPort1 = 1;

    /// - Default construct the nominal test article.
    tArticle = new FriendlyGunnsFluidSimpleH2Redox;

    /// - Define the nominal time step.
    tTimeStep = 0.1;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleH2Redox::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete tFluidInput2;
    delete tFluidInput1;
    delete tFluidInput0;
    delete tFluidConfig;
    delete tFluidProperties;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Simple H2 Redox link model construction of configuration and input
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleH2Redox::testConfigAndInput()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(tName                          == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                         == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tNumCells                      == tConfigData->mNumCells);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellVoltageLoaded, tConfigData->mCellVoltageLoaded, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellH2ReactRate,   tConfigData->mCellH2ReactRate,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMaxEfficiency,     tConfigData->mMaxEfficiency,     0.0);

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(tMalfBlockageFlag              == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMalfBlockageValue, tInputData->mMalfBlockageValue, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCurrent,           tInputData->mCurrent,           0.0);
    CPPUNIT_ASSERT(tTrippedOff                    == tInputData->mTrippedOff);

    /// @test    Configuration data default construction.
    GunnsFluidSimpleH2RedoxConfigData defaultConfig;
    CPPUNIT_ASSERT(0               == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0               == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0               == defaultConfig.mNumCells);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultConfig.mCellVoltageLoaded, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultConfig.mCellH2ReactRate,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultConfig.mMaxEfficiency,     0.0);

    /// @test    Input data default construction.
    GunnsFluidSimpleH2RedoxInputData defaultInput;
    CPPUNIT_ASSERT(                  !defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultInput.mMalfBlockageValue, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultInput.mCurrent,           0.0);
    CPPUNIT_ASSERT(                  !defaultInput.mTrippedOff);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Simple H2 Redox link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleH2Redox::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default construction configuration data.
    CPPUNIT_ASSERT(""              == tArticle->mName);
    CPPUNIT_ASSERT(0               == tArticle->mNodes);
    CPPUNIT_ASSERT(0               == tArticle->mNumCells);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mCellVoltageLoaded, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mCellH2ReactRate,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mMaxEfficiency,     0.0);

    /// @test    Default construction input data.
    CPPUNIT_ASSERT(                  !tArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mMalfBlockageValue, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mCurrent,           0.0);
    CPPUNIT_ASSERT(                  !tArticle->mTrippedOff);

    /// @test    Default construction state data.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mOutputStackVoltage, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mEfficiency,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mH2MassRate,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mO2MassRate,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mH2OMassRate,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mH2MoleRate,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mO2MoleRate,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mH2OMoleRate,        0.0);
    CPPUNIT_ASSERT(0               == tArticle->mH2Fluid);
    CPPUNIT_ASSERT(0               == tArticle->mO2Fluid);
    CPPUNIT_ASSERT(0               == tArticle->mH2OFluid);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsFluidSimpleH2Redox* article = new GunnsFluidSimpleH2Redox();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Simple H2 Redox link model nominal initialization without
///           exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleH2Redox::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    FriendlyGunnsFluidSimpleH2Redox article;
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Nominal configuration data.
    CPPUNIT_ASSERT(tName              == article.mName);
    CPPUNIT_ASSERT(&tNodes[tPort0]    == article.mNodes[0]);
    CPPUNIT_ASSERT(&tNodes[tPort1]    == article.mNodes[1]);
    CPPUNIT_ASSERT(tNumCells          == article.mNumCells);
    CPPUNIT_ASSERT(tCellVoltageLoaded == article.mCellVoltageLoaded);
    CPPUNIT_ASSERT(tCellH2ReactRate   == article.mCellH2ReactRate);
    CPPUNIT_ASSERT(tMaxEfficiency     == article.mMaxEfficiency);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag  == article.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageValue == article.mMalfBlockageValue);
    CPPUNIT_ASSERT(tCurrent                       == article.mCurrent);
    CPPUNIT_ASSERT(tTrippedOff                    == article.mTrippedOff);

    /// @test    Nominal state data.
    CPPUNIT_ASSERT(0.0 == article.mEfficiency);
    CPPUNIT_ASSERT(0.0 == article.mOutputStackVoltage);
    CPPUNIT_ASSERT(0.0 == article.mH2MassRate);
    CPPUNIT_ASSERT(0.0 == article.mO2MassRate);
    CPPUNIT_ASSERT(0.0 == article.mH2OMassRate);
    CPPUNIT_ASSERT(0.0 == article.mH2MoleRate);
    CPPUNIT_ASSERT(0.0 == article.mO2MoleRate);
    CPPUNIT_ASSERT(0.0 == article.mH2MoleRate);
    CPPUNIT_ASSERT(0   != article.mH2Fluid);
    CPPUNIT_ASSERT(0   != article.mO2Fluid);
    CPPUNIT_ASSERT(0   != article.mH2OFluid);
    CPPUNIT_ASSERT(1.0 == article.mH2Fluid->getMassFraction(0));
    CPPUNIT_ASSERT(1.0 == article.mO2Fluid->getMassFraction(1));
    CPPUNIT_ASSERT(1.0 == article.mH2OFluid->getMassFraction(3));

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(article.mInitFlag);

    /// @test    Nominal initialization with fluid objects already allocated.
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    CPPUNIT_ASSERT(0   != article.mH2Fluid);
    CPPUNIT_ASSERT(0   != article.mO2Fluid);
    CPPUNIT_ASSERT(0   != article.mH2OFluid);
    CPPUNIT_ASSERT(1.0 == article.mH2Fluid->getMassFraction(0));
    CPPUNIT_ASSERT(1.0 == article.mO2Fluid->getMassFraction(1));
    CPPUNIT_ASSERT(1.0 == article.mH2OFluid->getMassFraction(3));
    CPPUNIT_ASSERT(article.mInitFlag);
//
    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Simple H2 Redox link model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleH2Redox::testAccessors()
{
    UT_RESULT;

    /// @test    isInitialized.
    tArticle->mInitFlag = true;
    CPPUNIT_ASSERT(true == tArticle->isInitialized());

    /// @test    getOutputStackVoltage.
    tArticle->mOutputStackVoltage = 100.0;
    CPPUNIT_ASSERT(100.0 == tArticle->getOutputStackVoltage());

    /// @test    getOutputHeat.
    tArticle->mOutputHeat = -7.0;
    CPPUNIT_ASSERT(-7.0 == tArticle->getOutputHeat());

    /// @test    getH2MassRate
    tArticle->mH2MassRate = -2.0;
    CPPUNIT_ASSERT(-2.0 == tArticle->getH2MassRate());

    /// @test    getO2MassRate
    tArticle->mO2MassRate = -3.0;
    CPPUNIT_ASSERT(-3.0 == tArticle->getO2MassRate());

    /// @test    getH2OMassRate
    tArticle->mH2OMassRate = 4.0;
    CPPUNIT_ASSERT(4.0 == tArticle->getH2OMassRate());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Simple H2 Redox link model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleH2Redox::testModifiers()
{
    UT_RESULT;

    /// @test    setCurrent.
    tArticle->setCurrent(0.1);
    CPPUNIT_ASSERT(0.1 == tArticle->mCurrent);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Simple H2 Redox link model restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleH2Redox::testRestart()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    the state attributes are reset on restart.
    tArticle->mEfficiency   = 1.0;
    tArticle->mH2MassRate   = 1.0;
    tArticle->mO2MassRate   = 1.0;
    tArticle->mH2OMassRate  = 1.0;
    tArticle->mH2MoleRate   = 1.0;
    tArticle->mO2MoleRate   = 1.0;
    tArticle->mH2OMoleRate  = 1.0;
    tArticle->mPower        = 1.0;
    tArticle->restart();
    CPPUNIT_ASSERT(0.0 == tArticle->mEfficiency);
    CPPUNIT_ASSERT(0.0 == tArticle->mH2MassRate);
    CPPUNIT_ASSERT(0.0 == tArticle->mO2MassRate);
    CPPUNIT_ASSERT(0.0 == tArticle->mH2OMassRate);
    CPPUNIT_ASSERT(0.0 == tArticle->mH2MoleRate);
    CPPUNIT_ASSERT(0.0 == tArticle->mO2MoleRate);
    CPPUNIT_ASSERT(0.0 == tArticle->mH2OMoleRate);
    CPPUNIT_ASSERT(0.0 == tArticle->mPower);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Simple H2 Redox link model step method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleH2Redox::testStep()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    tArticle->mPotentialVector[0] = tFluidInput0->mPressure;
    tArticle->mPotentialVector[1] = tFluidInput1->mPressure;

    /// - Step the link.
    tArticle->step(tTimeStep);

    /// @test    Nominal outputs for forward (fuel cell) reaction.
    double expectedH2mass  = -tCellH2ReactRate * tNumCells * tCurrent
                           / tMaxEfficiency / (1.0 - tMalfBlockageValue);
    double expectedH2mole  =  expectedH2mass  / 2.01588;    // MW of H2
    double expectedO2mole  =  expectedH2mole  * 0.5;
    double expectedH2Omole = -expectedH2mole  * 1.0;
    double expectedO2mass  =  expectedO2mole  * 31.9988;    // MW of O2
    double expectedH2Omass =  expectedH2Omole * 18.0153;    // MW of H2O
    double expectedVolts   =  tCellVoltageLoaded * tNumCells;
    double expectedHeat    = fabs(expectedH2mole) * 4.86e7;
    double expectedW[2]    = {expectedH2mole + expectedH2Omole, expectedO2mole};
    CPPUNIT_ASSERT_EQUAL        (false,           tArticle->mTrippedOff);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH2mass,  tArticle->mH2MassRate,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedO2mass,  tArticle->mO2MassRate,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH2Omass, tArticle->mH2OMassRate,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH2mole,  tArticle->mH2MoleRate,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedO2mole,  tArticle->mO2MoleRate,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH2Omole, tArticle->mH2OMoleRate,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVolts,   tArticle->mOutputStackVoltage, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHeat,    tArticle->mOutputHeat,         FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW[0],    tArticle->mSourceVector[0],    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW[1],    tArticle->mSourceVector[1],    DBL_EPSILON);

    /// @test    Nominal outputs for zero reaction.
    tArticle->setCurrent(0.0);
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT_EQUAL        (false,         tArticle->mTrippedOff);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tArticle->mH2MassRate,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tArticle->mO2MassRate,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tArticle->mH2OMassRate,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tArticle->mH2MoleRate,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tArticle->mO2MoleRate,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tArticle->mH2OMoleRate,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVolts, tArticle->mOutputStackVoltage, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tArticle->mOutputHeat,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tArticle->mSourceVector[0],    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tArticle->mSourceVector[1],    DBL_EPSILON);

    /// @test    Nominal outputs for reverse (electrolysis) reaction, and no blockage malf.
    tArticle->setCurrent(-tCurrent);
    tArticle->setMalfBlockage();
    tArticle->step(tTimeStep);
    expectedH2mass  = -tCellH2ReactRate * tNumCells * tCurrent * tMaxEfficiency;
    expectedH2mole  =  expectedH2mass  / 2.01588;    // MW of H2
    expectedO2mole  =  expectedH2mole  * 0.5;
    expectedH2Omole = -expectedH2mole  * 1.0;
    expectedO2mass  =  expectedO2mole  * 31.9988;    // MW of O2
    expectedH2Omass =  expectedH2Omole * 18.0153;    // MW of H2O
    expectedVolts   =  tCellVoltageLoaded * tNumCells;
    expectedHeat    =  fabs(expectedH2mole) * -4.86e7;
    expectedW[0]    =  expectedH2mole + expectedH2Omole;
    expectedW[1]    =  expectedO2mole;
    CPPUNIT_ASSERT_EQUAL        ( false,           tArticle->mTrippedOff);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedH2mass,  tArticle->mH2MassRate,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedO2mass,  tArticle->mO2MassRate,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedH2Omass, tArticle->mH2OMassRate,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedH2mole,  tArticle->mH2MoleRate,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedO2mole,  tArticle->mO2MoleRate,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedH2Omole, tArticle->mH2OMoleRate,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedVolts,   tArticle->mOutputStackVoltage, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedHeat,    tArticle->mOutputHeat,         FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedW[0],    tArticle->mSourceVector[0],    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedW[1],    tArticle->mSourceVector[1],    DBL_EPSILON);

    /// @test    trip off for inlet pressure forward.
    tArticle->mPotentialVector[1] = 1.0e-5;
    tArticle->setCurrent(tCurrent);
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT_EQUAL        (true, tArticle->mTrippedOff);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  tArticle->mH2MassRate,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  tArticle->mO2MassRate,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  tArticle->mH2OMassRate,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  tArticle->mH2MoleRate,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  tArticle->mO2MoleRate,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  tArticle->mH2OMoleRate,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  tArticle->mOutputStackVoltage, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  tArticle->mOutputHeat,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  tArticle->mSourceVector[0],    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  tArticle->mSourceVector[1],    DBL_EPSILON);

    /// @test    reset trip.
    tArticle->mPotentialVector[1] = 200.0;
    tArticle->mTrippedOff         = false;
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT(false == tArticle->mTrippedOff);
    CPPUNIT_ASSERT(0     != tArticle->mH2MassRate);

    /// @test    trip off for inlet pressure reverse.
    tArticle->mPotentialVector[0] = 1.0e-5;
    tArticle->setCurrent(-tCurrent);
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT_EQUAL        (true, tArticle->mTrippedOff);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  tArticle->mH2MassRate,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  tArticle->mO2MassRate,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  tArticle->mH2OMassRate,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  tArticle->mH2MoleRate,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  tArticle->mO2MoleRate,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  tArticle->mH2OMoleRate,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  tArticle->mOutputStackVoltage, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  tArticle->mSourceVector[0],    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  tArticle->mSourceVector[1],    DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Simple H2 Redox link model compute flows.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleH2Redox::testComputeFlows()
{
    UT_RESULT;

    /// @test     No flow.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    tArticle->computeFlows(tTimeStep);
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tArticle->mPortDirections[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tArticle->mPortDirections[1]);
    CPPUNIT_ASSERT(0.0                  == tNodes[tPort0].getScheduledOutflux());
    CPPUNIT_ASSERT(0.0                  == tNodes[tPort1].getScheduledOutflux());

    /// @test    Positive net flow into the H2/H2O node.  Normally there is always a 1:1 exchange
    ///          of H2 & H2O moles so mSourceVector[0] will always be zero.  But the code checks
    ///          for a net imbalance, in case some future upgrade causes one.  So for now, we
    ///          force the source vector to non-zero to test.
    tArticle->mSourceVector[0] = 1.0;
    tArticle->computeFlows(tTimeStep);
    CPPUNIT_ASSERT(GunnsBasicLink::SINK == tArticle->mPortDirections[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tArticle->mPortDirections[1]);
    CPPUNIT_ASSERT(0.0                  == tNodes[tPort0].getScheduledOutflux());
    CPPUNIT_ASSERT(0.0                  == tNodes[tPort1].getScheduledOutflux());

    /// @test    Positive flow into the O2 node (electrolysis).
    tArticle->mPotentialVector[0] = tFluidInput0->mPressure;
    tArticle->mPotentialVector[1] = tFluidInput1->mPressure;
    tArticle->setCurrent(-tCurrent);
    tArticle->step(tTimeStep);
    tArticle->computeFlows(tTimeStep);
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tArticle->mPortDirections[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::SINK == tArticle->mPortDirections[1]);
    CPPUNIT_ASSERT(0.0                  == tNodes[tPort0].getScheduledOutflux());
    CPPUNIT_ASSERT(0.0                  == tNodes[tPort1].getScheduledOutflux());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Simple H2 Redox link model transport flows.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleH2Redox::testTransportFlows()
{
    UT_RESULT;

    /// @test    no flow.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    tArticle->mPotentialVector[0] = tFluidInput0->mPressure;
    tArticle->mPotentialVector[1] = tFluidInput1->mPressure;
    tArticle->transportFlows(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[tPort0].getContent()->getPressure(), tArticle->mH2Fluid->getPressure(),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[tPort0].getContent()->getPressure(), tArticle->mH2OFluid->getPressure(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[tPort1].getContent()->getPressure(), tArticle->mO2Fluid->getPressure(),  DBL_EPSILON);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[tPort0].getOutflow()->getTemperature(), tArticle->mH2Fluid->getTemperature(),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[tPort0].getOutflow()->getTemperature(), tArticle->mH2OFluid->getTemperature(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[tPort1].getOutflow()->getTemperature(), tArticle->mO2Fluid->getTemperature(),  DBL_EPSILON);

    CPPUNIT_ASSERT(0.0 == tNodes[tPort0].getInflux());
    CPPUNIT_ASSERT(0.0 == tNodes[tPort1].getInflux());

    /// @test    fuel cell flow.
    tArticle->step(tTimeStep);
    tArticle->computeFlows(tTimeStep);
    tArticle->transportFlows(tTimeStep);

    const double expectedMdot[2] = {tArticle->mH2MassRate + tArticle->mH2OMassRate,
                                    tArticle->mO2MassRate};
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot[0], tNodes[tPort0].getInflux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot[1], tNodes[tPort1].getInflux(), DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Simple H2 Redox link model specific port mapping rules.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleH2Redox::testPortMapping()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Successful move port 0 to another gas node.
    CPPUNIT_ASSERT(tArticle->checkSpecificPortRules(0,2));

    /// @test    Successful move port 1 to another gas node.
    CPPUNIT_ASSERT(tArticle->checkSpecificPortRules(1,0));

    /// @test    Fail move port 0 to a liquid node.
    CPPUNIT_ASSERT(!tArticle->checkSpecificPortRules(0,3));

    /// @test    Fail move port 1 to a liquid node.
    CPPUNIT_ASSERT(!tArticle->checkSpecificPortRules(1,3));

    /// @test    Fail move port 0 to Ground.
    CPPUNIT_ASSERT(!tArticle->checkSpecificPortRules(0,4));

    /// @test    Fail move port 1 to Ground.
    CPPUNIT_ASSERT(!tArticle->checkSpecificPortRules(1,4));

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Simple H2 Redox link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleH2Redox::testInitializationExceptions()
{
    UT_RESULT;

    /// - Default construct a test article.
    GunnsFluidSimpleH2Redox article;

    /// @test    Initialization exception on invalid config data: no name.
    tConfigData->mName = "";
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mName = tName;

    /// @test    Initialization exception on invalid config data: mNumCells < 1.
    tConfigData->mNumCells = 0;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mNumCells = tNumCells;

    /// @test    Initialization exception on invalid config data: mCellVoltageLoaded < DBL_EPSILON.
    tConfigData->mCellVoltageLoaded = 0.0;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mCellVoltageLoaded = tCellVoltageLoaded;

    /// @test    Initialization exception on invalid config data: mCellH2ReactRate < DBL_EPSILON.
    tConfigData->mCellH2ReactRate = 0.0;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mCellH2ReactRate = tCellH2ReactRate;

    /// @test    Initialization exception on invalid config data: mMaxEfficiency not in (0-1).
    tConfigData->mMaxEfficiency = 0.0;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mMaxEfficiency = 1.001;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mMaxEfficiency = tMaxEfficiency;

    {
        /// @test    Initialization exception on invalid config data: H2 not in the network.
        tTypes[0] = FluidProperties::GUNNS_N2;
        PolyFluidConfigData* fluidConfig = new PolyFluidConfigData(tFluidProperties, tTypes, N_FLUIDS);

        double fractions0[N_FLUIDS] = {0.9, 0.0, 0.1, 0.0, 0.0};
        PolyFluidInputData* fluidInput0 = new PolyFluidInputData(294.261,
                                                                 200.0,
                                                                 0.0,
                                                                 0.0,
                                                                 fractions0);
        GunnsFluidNode nodes[N_NODES];
        nodes[0].initialize("UtNode0", fluidConfig);
        nodes[1].initialize("UtNode1", fluidConfig);
        nodes[0].getContent()->initialize(*fluidConfig, *fluidInput0);
        nodes[1].getContent()->initialize(*fluidConfig, *fluidInput0);
        tNodeList.mNodes    = nodes;
        tNodeList.mNumNodes = N_NODES;
        CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                             TsInitializationException);
        delete fluidInput0;
        delete fluidConfig;
        tTypes[0] = FluidProperties::GUNNS_H2;
    } {
        /// @test    Initialization exception on invalid config data: O2 not in the network.
        tTypes[1] = FluidProperties::GUNNS_N2;
        PolyFluidConfigData* fluidConfig = new PolyFluidConfigData(tFluidProperties, tTypes, N_FLUIDS);

        double fractions0[N_FLUIDS] = {0.9, 0.0, 0.1, 0.0, 0.0};
        PolyFluidInputData* fluidInput0 = new PolyFluidInputData(294.261,
                                                                 200.0,
                                                                 0.0,
                                                                 0.0,
                                                                 fractions0);
        GunnsFluidNode nodes[N_NODES];
        nodes[0].initialize("UtNode0", fluidConfig);
        nodes[1].initialize("UtNode1", fluidConfig);
        nodes[0].getContent()->initialize(*fluidConfig, *fluidInput0);
        nodes[1].getContent()->initialize(*fluidConfig, *fluidInput0);
        tNodeList.mNodes    = nodes;
        tNodeList.mNumNodes = N_NODES;
        CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                             TsInitializationException);
        delete fluidInput0;
        delete fluidConfig;
        tTypes[1] = FluidProperties::GUNNS_O2;
    } {
        /// @test    Initialization exception on invalid config data: H2O not in the network.
        tTypes[3] = FluidProperties::GUNNS_N2;
        PolyFluidConfigData* fluidConfig = new PolyFluidConfigData(tFluidProperties, tTypes, N_FLUIDS);

        double fractions0[N_FLUIDS] = {0.9, 0.0, 0.1, 0.0, 0.0};
        PolyFluidInputData* fluidInput0 = new PolyFluidInputData(294.261,
                                                                 200.0,
                                                                 0.0,
                                                                 0.0,
                                                                 fractions0);
        GunnsFluidNode nodes[N_NODES];
        nodes[0].initialize("UtNode0", fluidConfig);
        nodes[1].initialize("UtNode1", fluidConfig);
        nodes[0].getContent()->initialize(*fluidConfig, *fluidInput0);
        nodes[1].getContent()->initialize(*fluidConfig, *fluidInput0);
        tNodeList.mNodes    = nodes;
        tNodeList.mNumNodes = N_NODES;
        CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                             TsInitializationException);
        delete fluidInput0;
        delete fluidConfig;
        tTypes[3] = FluidProperties::GUNNS_H2O;
    }

    UT_PASS_LAST;
}
