/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/fluid/conductor/GunnsFluidSimpleRocket.o))
***************************************************************************************************/

#include "core/GunnsFluidUtils.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidSimpleRocket.hh"

/// @details  Test identification number.
int UtGunnsFluidSimpleRocket::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Simple Rocket link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidSimpleRocket::UtGunnsFluidSimpleRocket()
    :
    CppUnit::TestFixture(),
    tTypes(),
    tFractions(),
    tFractions2(),
    tFractions3(),
    tFluidProperties(0),
    tFluidConfig(0),
    tFluidInput0(0),
    tFluidInput1(0),
    tFluidInput2(0),
    tFluidInput3(0),
    tLinks(),
    tName(""),
    tNodes(),
    tNodeList(),
    tPort0(0),
    tPort1(0),
    tThroatArea(0.0),
    tAreaRatio(0.0),
    tCharacteristicVelocity(0.0),
    tCombustionTemperature(0.0),
    tCombustionGamma(0.0),
    tThrustTuning(0.0),
    tRecombinationTuning(0.0),
    tNeedsIgnition(false),
    tCombustType(CombustFactory::NONE),
    tConfigData(0),
    tMalfBlockageFlag(false),
    tMalfBlockageValue(0.0),
    tIgnition(false),
    tCombustion(false),
    tInputData(0),
    tArticle(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Simple Rocket link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidSimpleRocket::~UtGunnsFluidSimpleRocket()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////

void UtGunnsFluidSimpleRocket::setUp()
{
    /// - Define the nominal port fluids.
    tFluidProperties        = new DefinedFluidProperties();
    tTypes[0]               = FluidProperties::GUNNS_CH4;
    tTypes[1]               = FluidProperties::GUNNS_O2;
    tTypes[2]               = FluidProperties::GUNNS_HE;
    tFluidConfig            = new PolyFluidConfigData(tFluidProperties, tTypes, N_FLUIDS);

    tFractions[0]           = 0.2;
    tFractions[1]           = 0.75;
    tFractions[2]           = 0.05;
    tFluidInput0            = new PolyFluidInputData( 110.0,
                                                     1206.583,
                                                        0.0,
                                                        0.0,
                                                     tFractions);
    tFluidInput1            = new PolyFluidInputData( 110.0,
                                                      101.325,
                                                        0.0,
                                                        0.0,
                                                     tFractions);
    tFractions2[0]          = 0.05;
    tFractions2[1]          = 0.05;
    tFractions2[2]          = 0.9;
    tFluidInput2            = new PolyFluidInputData( 110.0,
                                                     1206.583,
                                                        0.0,
                                                        0.0,
                                                     tFractions2);

    tFractions3[0]          = 0.0;
    tFractions3[1]          = 0.0;
    tFractions3[2]          = 1.0;
    tFluidInput3            = new PolyFluidInputData( 110.0,
                                                     1206.583,
                                                        0.0,
                                                        0.0,
                                                     tFractions3);

    /// - Initialize the nodes.
    tNodes[0].initialize("UtNode0", tFluidConfig);
    tNodes[1].initialize("UtNode1", tFluidConfig);
    tNodes[2].initialize("UtNode2", tFluidConfig);
    tNodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput0);
    tNodes[1].getContent()->initialize(*tFluidConfig, *tFluidInput1);
    tNodes[2].getContent()->initialize(*tFluidConfig, *tFluidInput2);

    tNodes[0].resetFlows();
    tNodes[1].resetFlows();
    tNodes[2].resetFlows();

    /// - Initialize the nodes list.
    tNodeList.mNodes        = tNodes;
    tNodeList.mNumNodes     = N_NODES;

    /// - Define the nominal configuration data.
    tName                   = "nominal";
    tThroatArea             = 0.0001923;
    tAreaRatio              = 3.0;
    tCharacteristicVelocity = 1738.0;
    tCombustionTemperature  = 3342.0;
    tCombustionGamma        = 1.2;
    tThrustTuning           = 1.0;
    tRecombinationTuning    = 1.0;
    tNeedsIgnition          = true;
    tConfigData             = new GunnsFluidSimpleRocketConfigData(tName,
                                                                   &tNodeList,
                                                                   tThroatArea,
                                                                   tAreaRatio,
                                                                   tCharacteristicVelocity,
                                                                   tCombustionTemperature,
                                                                   tCombustionGamma,
                                                                   tThrustTuning,
                                                                   tRecombinationTuning,
                                                                   tNeedsIgnition,
                                                                   tCombustType);

    /// - Define the nominal input data.
    tMalfBlockageFlag       = false;
    tMalfBlockageValue      = 0.0;
    tIgnition               = false;
    tCombustion             = true;
    tInputData              = new GunnsFluidSimpleRocketInputData(tMalfBlockageFlag,
                                                                  tMalfBlockageValue,
                                                                  tIgnition,
                                                                  tCombustion);

    /// - Define the nominal port mapping.
    tPort0                = 0;
    tPort1                = 1;

    /// - Default construct the nominal test article.
    tArticle              = new FriendlyGunnsFluidSimpleRocket;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleRocket::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete tFluidInput3;
    delete tFluidInput2;
    delete tFluidInput1;
    delete tFluidInput0;
    delete tFluidConfig;
    delete tFluidProperties;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Simple Rocket link model construction of configuration.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleRocket::testConfig()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data nominal construction.
    GunnsFluidSimpleRocketConfigData nominalConfig(tName,
                                                   &tNodeList,
                                                   1.0,
                                                   2.0,
                                                   3.0,
                                                   4.0,
                                                   5.0,
                                                   0.5,
                                                   0.6,
                                                   true,
                                                   CombustFactory::CH4);
    CPPUNIT_ASSERT(tName                                 == nominalConfig.mName);
    CPPUNIT_ASSERT(tNodes                                == nominalConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(1.0                                   == nominalConfig.mThroatArea);
    CPPUNIT_ASSERT(2.0                                   == nominalConfig.mAreaRatio);
    CPPUNIT_ASSERT(3.0                                   == nominalConfig.mCharacteristicVelocity);
    CPPUNIT_ASSERT(4.0                                   == nominalConfig.mCombustionTemperature);
    CPPUNIT_ASSERT(5.0                                   == nominalConfig.mCombustionGamma);
    CPPUNIT_ASSERT(0.5                                   == nominalConfig.mThrustTuning);
    CPPUNIT_ASSERT(0.6                                   == nominalConfig.mRecombinationTuning);
    CPPUNIT_ASSERT(true                                  == nominalConfig.mNeedsIgnition);
    CPPUNIT_ASSERT(CombustFactory::CH4                   == nominalConfig.mCombustType);

    /// @test    Configuration data default construction.
    GunnsFluidSimpleRocketConfigData defaultConfig;
    CPPUNIT_ASSERT(0                                     == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0                                     == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0                                   == defaultConfig.mThroatArea);
    CPPUNIT_ASSERT(0.0                                   == defaultConfig.mAreaRatio);
    CPPUNIT_ASSERT(0.0                                   == defaultConfig.mCharacteristicVelocity);
    CPPUNIT_ASSERT(0.0                                   == defaultConfig.mCombustionTemperature);
    CPPUNIT_ASSERT(0.0                                   == defaultConfig.mCombustionGamma);
    CPPUNIT_ASSERT(0.0                                   == defaultConfig.mThrustTuning);
    CPPUNIT_ASSERT(0.0                                   == defaultConfig.mRecombinationTuning);
    CPPUNIT_ASSERT(false                                 == defaultConfig.mNeedsIgnition);
    CPPUNIT_ASSERT(CombustFactory::NONE                  == defaultConfig.mCombustType);

       /// @test    Configuration data copy construction.
    GunnsFluidSimpleRocketConfigData copyConfig(nominalConfig);
    CPPUNIT_ASSERT(nominalConfig.mName                   == copyConfig.mName);
    CPPUNIT_ASSERT(nominalConfig.mNodeList->mNodes       == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(nominalConfig.mThroatArea             == copyConfig.mThroatArea);
    CPPUNIT_ASSERT(nominalConfig.mAreaRatio              == copyConfig.mAreaRatio);
    CPPUNIT_ASSERT(nominalConfig.mCharacteristicVelocity == copyConfig.mCharacteristicVelocity);
    CPPUNIT_ASSERT(nominalConfig.mCombustionTemperature  == copyConfig.mCombustionTemperature);
    CPPUNIT_ASSERT(nominalConfig.mCombustionGamma        == copyConfig.mCombustionGamma);
    CPPUNIT_ASSERT(nominalConfig.mThrustTuning           == copyConfig.mThrustTuning);
    CPPUNIT_ASSERT(nominalConfig.mRecombinationTuning    == copyConfig.mRecombinationTuning);
    CPPUNIT_ASSERT(nominalConfig.mNeedsIgnition          == copyConfig.mNeedsIgnition);
    CPPUNIT_ASSERT(nominalConfig.mCombustType            == copyConfig.mCombustType);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Simple Rocket link model construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleRocket::testInput()
{
    UT_RESULT;

    GunnsFluidSimpleRocketInputData nominalInput(true,
                                                 0.5,
                                                 true,
                                                 true);

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(true                            == nominalInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.5                             == nominalInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(true                            == nominalInput.mIgnition);
    CPPUNIT_ASSERT(true                            == nominalInput.mCombustion);

    /// @test    Input data default construction.
    GunnsFluidSimpleRocketInputData defaultInput;
    CPPUNIT_ASSERT(false                           == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                             == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(false                           == defaultInput.mIgnition);
    CPPUNIT_ASSERT(false                           == defaultInput.mCombustion);

    /// @test    Input data copy construction.
    GunnsFluidSimpleRocketInputData copyInput(nominalInput);
    CPPUNIT_ASSERT(nominalInput.mMalfBlockageFlag  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(nominalInput.mMalfBlockageValue == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(nominalInput.mIgnition          == copyInput.mIgnition);
    CPPUNIT_ASSERT(nominalInput.mCombustion        == copyInput.mCombustion);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Simple Rocket link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleRocket::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default of attributes.
    CPPUNIT_ASSERT(""    == tArticle->mName);
    CPPUNIT_ASSERT(0     == tArticle->mNodes);
    CPPUNIT_ASSERT(0.0   == tArticle->mThroatArea);
    CPPUNIT_ASSERT(0.0   == tArticle->mAreaRatio);
    CPPUNIT_ASSERT(0.0   == tArticle->mCombustionTemperature);
    CPPUNIT_ASSERT(0.0   == tArticle->mCombustionGamma);
    CPPUNIT_ASSERT(0.0   == tArticle->mCombustionMWeight);
    CPPUNIT_ASSERT(0.0   == tArticle->mThrustTuning);
    CPPUNIT_ASSERT(0.0   == tArticle->mRecombinationTuning);
    CPPUNIT_ASSERT(false == tArticle->mNeedsIgnition);
    CPPUNIT_ASSERT(0     == tArticle->mCombustModel);
    CPPUNIT_ASSERT(false == tArticle->mIgnition);
    CPPUNIT_ASSERT(false == tArticle->mCombustion);
    CPPUNIT_ASSERT(0.0   == tArticle->mDefaultConductance);
    CPPUNIT_ASSERT(0.0   == tArticle->mChamberPressure);
    CPPUNIT_ASSERT(0.0   == tArticle->mChamberTemperature);
    CPPUNIT_ASSERT(0.0   == tArticle->mChamberGamma);
    CPPUNIT_ASSERT(0.0   == tArticle->mChamberMWeight);
    CPPUNIT_ASSERT(0.0   == tArticle->mExitMach);
    CPPUNIT_ASSERT(0.0   == tArticle->mExitPressure);
    CPPUNIT_ASSERT(0.0   == tArticle->mExitTemperature);
    CPPUNIT_ASSERT(0.0   == tArticle->mExitGamma);
    CPPUNIT_ASSERT(0.0   == tArticle->mExitMWeight);
    CPPUNIT_ASSERT(0.0   == tArticle->mExitVelocity);
    CPPUNIT_ASSERT(0.0   == tArticle->mSpecificImpulse);
    CPPUNIT_ASSERT(0.0   == tArticle->mThrust);
    CPPUNIT_ASSERT(0.0   == tArticle->mCharacteristicVelocity);
    CPPUNIT_ASSERT(0.0   == tArticle->mSystemConductance);
    CPPUNIT_ASSERT(0     == tArticle->mCombustionCount);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(false == tArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsFluidSimpleRocket* article = new GunnsFluidSimpleRocket();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Simple Rocket link model nominal initialization without
///           exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleRocket::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1));

    /// @test    Nominal configuration data.
    CPPUNIT_ASSERT(tName                  == tArticle->mName);
    CPPUNIT_ASSERT(&tNodes[0]             == tArticle->mNodes[0]);
    CPPUNIT_ASSERT(&tNodes[1]             == tArticle->mNodes[1]);
    CPPUNIT_ASSERT(tThroatArea            == tArticle->mThroatArea);
    CPPUNIT_ASSERT(tAreaRatio             == tArticle->mAreaRatio);
    CPPUNIT_ASSERT(tCombustionTemperature == tArticle->mCombustionTemperature);
    CPPUNIT_ASSERT(tCombustionGamma       == tArticle->mCombustionGamma);
    CPPUNIT_ASSERT(tThrustTuning          == tArticle->mThrustTuning);
    CPPUNIT_ASSERT(tRecombinationTuning   == tArticle->mRecombinationTuning);
    CPPUNIT_ASSERT(tNeedsIgnition         == tArticle->mNeedsIgnition);
    const double expectedG  = 1000.0 * tThroatArea / tCharacteristicVelocity;
    const double expectedMW = 21.87156537;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG,  tArticle->mDefaultConductance, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMW, tArticle->mCombustionMWeight,  FLT_EPSILON);
    CPPUNIT_ASSERT(0                      == tArticle->mCombustModel);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT(tMalfBlockageFlag      == tArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue     == tArticle->mMalfBlockageValue);
    CPPUNIT_ASSERT(tIgnition              == tArticle->mIgnition);
    CPPUNIT_ASSERT(tCombustion            == tArticle->mCombustion);

    /// @test    Nominal state data.
    CPPUNIT_ASSERT(0.0                    == tArticle->mChamberPressure);
    CPPUNIT_ASSERT(0.0                    == tArticle->mChamberTemperature);
    CPPUNIT_ASSERT(0.0                    == tArticle->mChamberGamma);
    CPPUNIT_ASSERT(0.0                    == tArticle->mChamberMWeight);
    CPPUNIT_ASSERT(0.0                    == tArticle->mExitMach);
    CPPUNIT_ASSERT(0.0                    == tArticle->mExitPressure);
    CPPUNIT_ASSERT(0.0                    == tArticle->mExitTemperature);
    CPPUNIT_ASSERT(0.0                    == tArticle->mExitGamma);
    CPPUNIT_ASSERT(0.0                    == tArticle->mExitMWeight);
    CPPUNIT_ASSERT(0.0                    == tArticle->mExitVelocity);
    CPPUNIT_ASSERT(0.0                    == tArticle->mSpecificImpulse);
    CPPUNIT_ASSERT(0.0                    == tArticle->mThrust);
    CPPUNIT_ASSERT(0.0                    == tArticle->mCharacteristicVelocity);
    CPPUNIT_ASSERT(0.0                    == tArticle->mSystemConductance);
    CPPUNIT_ASSERT(0                      == tArticle->mCombustionCount);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(tArticle->mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Simple Rocket link model nominal initialization without
///           exceptions and with a combustion model specified.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleRocket::testCombustionInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    tConfigData->mCombustType = CombustFactory::CH4;
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1));

    /// @test    Nominal configuration data.
    CPPUNIT_ASSERT(tName                  == tArticle->mName);
    CPPUNIT_ASSERT(&tNodes[0]             == tArticle->mNodes[0]);
    CPPUNIT_ASSERT(&tNodes[1]             == tArticle->mNodes[1]);
    CPPUNIT_ASSERT(tThroatArea            == tArticle->mThroatArea);
    CPPUNIT_ASSERT(tAreaRatio             == tArticle->mAreaRatio);
    CPPUNIT_ASSERT(tCombustionTemperature == tArticle->mCombustionTemperature);
    CPPUNIT_ASSERT(tCombustionGamma       == tArticle->mCombustionGamma);
    CPPUNIT_ASSERT(tThrustTuning          == tArticle->mThrustTuning);
    CPPUNIT_ASSERT(tRecombinationTuning   == tArticle->mRecombinationTuning);
    CPPUNIT_ASSERT(tNeedsIgnition         == tArticle->mNeedsIgnition);
    const double expectedG  = 1000.0 * tThroatArea / tCharacteristicVelocity;
    const double expectedMW = 21.87156537;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG,  tArticle->mDefaultConductance, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMW, tArticle->mCombustionMWeight,  FLT_EPSILON);
    CPPUNIT_ASSERT(0                      != tArticle->mCombustModel);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT(tMalfBlockageFlag      == tArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue     == tArticle->mMalfBlockageValue);
    CPPUNIT_ASSERT(tIgnition              == tArticle->mIgnition);
    CPPUNIT_ASSERT(tCombustion            == tArticle->mCombustion);

    /// @test    Nominal state data.
    CPPUNIT_ASSERT(0.0                    == tArticle->mChamberPressure);
    CPPUNIT_ASSERT(0.0                    == tArticle->mChamberTemperature);
    CPPUNIT_ASSERT(0.0                    == tArticle->mChamberGamma);
    CPPUNIT_ASSERT(0.0                    == tArticle->mChamberMWeight);
    CPPUNIT_ASSERT(0.0                    == tArticle->mExitMach);
    CPPUNIT_ASSERT(0.0                    == tArticle->mExitPressure);
    CPPUNIT_ASSERT(0.0                    == tArticle->mExitTemperature);
    CPPUNIT_ASSERT(0.0                    == tArticle->mExitGamma);
    CPPUNIT_ASSERT(0.0                    == tArticle->mExitMWeight);
    CPPUNIT_ASSERT(0.0                    == tArticle->mExitVelocity);
    CPPUNIT_ASSERT(0.0                    == tArticle->mSpecificImpulse);
    CPPUNIT_ASSERT(0.0                    == tArticle->mThrust);
    CPPUNIT_ASSERT(0.0                    == tArticle->mCharacteristicVelocity);
    CPPUNIT_ASSERT(0.0                    == tArticle->mSystemConductance);
    CPPUNIT_ASSERT(0                      == tArticle->mCombustionCount);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(tArticle->mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Simple Rocket link model restart.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleRocket::testRestart()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    tArticle->mDefaultConductance     = 1.0;
    tArticle->mCombustionMWeight      = 1.0;
    tArticle->mChamberGamma           = 1.0;
    tArticle->mChamberMWeight         = 1.0;
    tArticle->mExitMach               = 1.0;
    tArticle->mExitPressure           = 1.0;
    tArticle->mExitTemperature        = 1.0;
    tArticle->mExitGamma              = 1.0;
    tArticle->mExitMWeight            = 1.0;
    tArticle->mExitVelocity           = 1.0;
    tArticle->mSpecificImpulse        = 1.0;
    tArticle->mCharacteristicVelocity = 1.0;
    tArticle->mSystemConductance      = 1.0;
    tArticle->restart();

    /// @ test    These class state attributes are not reset:
    CPPUNIT_ASSERT(1.0 == tArticle->mDefaultConductance);
    CPPUNIT_ASSERT(1.0 == tArticle->mCombustionMWeight);

    /// @ test    These class state attributes are reset:
    CPPUNIT_ASSERT(0.0 == tArticle->mChamberGamma);
    CPPUNIT_ASSERT(0.0 == tArticle->mChamberMWeight);
    CPPUNIT_ASSERT(0.0 == tArticle->mExitMach);
    CPPUNIT_ASSERT(0.0 == tArticle->mExitPressure);
    CPPUNIT_ASSERT(0.0 == tArticle->mExitTemperature);
    CPPUNIT_ASSERT(0.0 == tArticle->mExitGamma);
    CPPUNIT_ASSERT(0.0 == tArticle->mExitMWeight);
    CPPUNIT_ASSERT(0.0 == tArticle->mExitVelocity);
    CPPUNIT_ASSERT(0.0 == tArticle->mSpecificImpulse);
    CPPUNIT_ASSERT(0.0 == tArticle->mCharacteristicVelocity);
    CPPUNIT_ASSERT(0.0 == tArticle->mSystemConductance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Simple Rocket link model step method outputs to the solver.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleRocket::testStep()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test  normal combustion, 1st frame.
    double Pc  = tNodes[0].getOutflow()->getPressure();
    double MW0 = tNodes[0].getOutflow()->getMWeight();
    double G   = tArticle->mDefaultConductance / MW0;
    double MWc = tArticle->mCombustionMWeight;
    double Me  =    2.3970;
    double Pe  =   79.18;
    double Ue  = 2358.63;
    double Te  = 2122.50;
    double Isp = Ue / 9.80665;

    tArticle->mCombustion         = true;
    tArticle->mFlowRate           = 1.0;
    tArticle->mPotentialVector[0] = tNodes[0].getOutflow()->getPressure();
    tArticle->mPotentialVector[1] = tNodes[1].getOutflow()->getPressure();
    tArticle->mAdmittanceUpdate   = false;
    tArticle->setMinLinearizationPotential(1.0e-6);
    tArticle->step(0.01);

    CPPUNIT_ASSERT(Pc                     == tArticle->mChamberPressure);
    CPPUNIT_ASSERT(Pc                     == tArticle->getChamberPressure());
    CPPUNIT_ASSERT(tCombustionTemperature == tArticle->mCombustionTemperature);
    CPPUNIT_ASSERT(tCombustionGamma       == tArticle->mCombustionGamma);
    CPPUNIT_ASSERT(MWc                    == tArticle->mCombustionMWeight);
    CPPUNIT_ASSERT(tCombustionTemperature == tArticle->mChamberTemperature);
    CPPUNIT_ASSERT(tCombustionTemperature == tArticle->getChamberTemperature());
    CPPUNIT_ASSERT(tCombustionGamma       == tArticle->mChamberGamma);
    CPPUNIT_ASSERT(MWc                    == tArticle->mChamberMWeight);
    CPPUNIT_ASSERT(tCombustionGamma       == tArticle->mExitGamma);
    CPPUNIT_ASSERT(MWc                    == tArticle->mExitMWeight);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(Me,  tArticle->mExitMach,              0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(Pe,  tArticle->mExitPressure,          0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(Ue,  tArticle->mExitVelocity,          0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(Te,  tArticle->mExitTemperature,       0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(Isp, tArticle->mSpecificImpulse,       0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( G,  tArticle->mSystemConductance,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( G,  tArticle->mAdmittanceMatrix[0],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-G,  tArticle->mAdmittanceMatrix[1],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-G,  tArticle->mAdmittanceMatrix[2],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( G,  tArticle->mAdmittanceMatrix[3],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0,  tArticle->mSourceVector[0],       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0,  tArticle->mSourceVector[1],       DBL_EPSILON);
    CPPUNIT_ASSERT(true == tArticle->mAdmittanceUpdate);

    /// @test  normal combustion, >1 frame.
    double p0   = tNodes[0].getOutflow()->getPressure();
    double p1   = tNodes[1].getOutflow()->getPressure();
    double mdot = 0.133524;
    G           = mdot / (p0 - p1) / MW0;

    tArticle->mAdmittanceUpdate   = false;
    tArticle->step(0.01);

    CPPUNIT_ASSERT_DOUBLES_EQUAL( G, tArticle->mSystemConductance,     1.0e-9);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( G, tArticle->mAdmittanceMatrix[0],   1.0e-9);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-G, tArticle->mAdmittanceMatrix[1],   1.0e-9);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-G, tArticle->mAdmittanceMatrix[2],   1.0e-9);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( G, tArticle->mAdmittanceMatrix[3],   1.0e-9);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, tArticle->mSourceVector[0],       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, tArticle->mSourceVector[1],       DBL_EPSILON);
    CPPUNIT_ASSERT(true == tArticle->mAdmittanceUpdate);

    /// @test  no combustion.
    mdot = 0.768442;
    G    = mdot / (p0 - p1) / MW0;

    tArticle->mCombustion         = false;
    tArticle->mAdmittanceUpdate   = false;
    tArticle->step(0.01);

    CPPUNIT_ASSERT_DOUBLES_EQUAL( G, tArticle->mSystemConductance,     1.0e-9);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( G, tArticle->mAdmittanceMatrix[0],   1.0e-9);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-G, tArticle->mAdmittanceMatrix[1],   1.0e-9);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-G, tArticle->mAdmittanceMatrix[2],   1.0e-9);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( G, tArticle->mAdmittanceMatrix[3],   1.0e-9);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, tArticle->mSourceVector[0],       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, tArticle->mSourceVector[1],       DBL_EPSILON);
    CPPUNIT_ASSERT(true == tArticle->mAdmittanceUpdate);

    /// @test   double step.
    tArticle->mAdmittanceUpdate = false;
    tArticle->step(0.01);

    CPPUNIT_ASSERT_DOUBLES_EQUAL( G, tArticle->mSystemConductance,     1.0e-9);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( G, tArticle->mAdmittanceMatrix[0],   1.0e-9);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-G, tArticle->mAdmittanceMatrix[1],   1.0e-9);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-G, tArticle->mAdmittanceMatrix[2],   1.0e-9);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( G, tArticle->mAdmittanceMatrix[3],   1.0e-9);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, tArticle->mSourceVector[0],       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, tArticle->mSourceVector[1],       DBL_EPSILON);
    CPPUNIT_ASSERT(false == tArticle->mAdmittanceUpdate);

    /// @test   negative pressure gradient.
    tArticle->mPotentialVector[0] = tNodes[1].getOutflow()->getPressure() - FLT_EPSILON;
    tArticle->mAdmittanceUpdate = false;
    tArticle->step(0.01);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSystemConductance,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mAdmittanceMatrix[0],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mAdmittanceMatrix[1],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mAdmittanceMatrix[2],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mAdmittanceMatrix[3],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSourceVector[0],       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSourceVector[1],       DBL_EPSILON);
    CPPUNIT_ASSERT(true == tArticle->mAdmittanceUpdate);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Simple Rocket link model moding of combustion state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleRocket::testStepIgnition()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test  combustion switches off if no forward flow.
    tArticle->mCombustion      = true;
    tArticle->mCombustionCount = 5;
    tArticle->step(0.01);
    CPPUNIT_ASSERT(false == tArticle->mCombustion);
    CPPUNIT_ASSERT(0     == tArticle->mCombustionCount);

    /// @test  combustion stays off if ignition but no forward flow.
    tArticle->setIgnition(true);
    tArticle->step(0.01);
    CPPUNIT_ASSERT(true  == tArticle->mIgnition);
    CPPUNIT_ASSERT(false == tArticle->mCombustion);
    CPPUNIT_ASSERT(0     == tArticle->mCombustionCount);

    /// @test  combustion switches on if ignition and forward flow.
    tArticle->mFlowRate = 1.0;
    tArticle->step(0.01);
    CPPUNIT_ASSERT(true  == tArticle->mCombustion);
    CPPUNIT_ASSERT(1     == tArticle->mCombustionCount);

    /// @test  combustion stays on if ignition and forward flow.
    tArticle->step(0.01);
    CPPUNIT_ASSERT(true  == tArticle->mCombustion);
    CPPUNIT_ASSERT(2     == tArticle->mCombustionCount);

    /// @test  combustion stays on if forward flow and no ignition.
    tArticle->setIgnition(false);
    tArticle->step(0.01);
    CPPUNIT_ASSERT(false == tArticle->mIgnition);
    CPPUNIT_ASSERT(true  == tArticle->mCombustion);
    CPPUNIT_ASSERT(3     == tArticle->mCombustionCount);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Simple Rocket link model step method moding of combustion state
///           for hypergolic engine.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleRocket::testStepIgnitionHypergolic()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tConfigData->mNeedsIgnition = false;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test  combustion switches off if no forward flow.
    tArticle->mCombustion          = true;
    tArticle->mCombustionCount     = 5;
    tArticle->step(0.01);
    CPPUNIT_ASSERT(false == tArticle->mCombustion);
    CPPUNIT_ASSERT(0     == tArticle->mCombustionCount);

    /// @test  combustion stays off if ignition but no forward flow.
    tArticle->setIgnition(true);
    tArticle->step(0.01);
    CPPUNIT_ASSERT(false == tArticle->mCombustion);
    CPPUNIT_ASSERT(0     == tArticle->mCombustionCount);

    /// @test  combustion switches on if forward flow and no ignition.
    tArticle->setIgnition(false);
    tArticle->mFlowRate = 1.0;
    tArticle->step(0.01);
    CPPUNIT_ASSERT(false == tArticle->mIgnition);
    CPPUNIT_ASSERT(true  == tArticle->mCombustion);
    CPPUNIT_ASSERT(1     == tArticle->mCombustionCount);

    /// @test  combustion stays on if no ignition and forward flow.
    tArticle->step(0.01);
    CPPUNIT_ASSERT(true  == tArticle->mCombustion);
    CPPUNIT_ASSERT(2     == tArticle->mCombustionCount);

    /// @test  combustion switches off if ignition and no forward flow.
    tArticle->mFlowRate = 0.0;
    tArticle->setIgnition(true);
    tArticle->step(0.01);
    CPPUNIT_ASSERT(false == tArticle->mCombustion);
    CPPUNIT_ASSERT(0     == tArticle->mCombustionCount);

    /// @test  combustion switches on if forward flow and ignition.
    tArticle->mFlowRate = 1.0;
    tArticle->step(0.01);
    CPPUNIT_ASSERT(true  == tArticle->mCombustion);
    CPPUNIT_ASSERT(1     == tArticle->mCombustionCount);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Simple Rocket link model step method outputs to the solver.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleRocket::testStepCombustion()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tConfigData->mCombustType         = CombustFactory::CH4;
    tConfigData->mRecombinationTuning = 0.5;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test  normal combustion, >1 frame.
    double MW0  = tNodes[0].getOutflow()->getMWeight();
    double gc   =    1.198117;
    double MWc  =   17.769601;
    double Tc   = 3204.204517;
    double Me   =    2.394867;
    double Pe   =   79.426419;
    double Ue   = 2798.536327;
    double Te   = 2101.402561;
    double MWe  =   17.881357;
    double Isp  = Ue / 9.80665;
    double p0   = tNodes[0].getOutflow()->getPressure();
    double p1   = tNodes[1].getOutflow()->getPressure();
    double mdot = 0.1208166;
    double G    = mdot / (p0 - p1) / MW0;

    tArticle->mCombustion         = true;
    tArticle->mCombustionCount    = 2;
    tArticle->mFlowRate           = 1.0;
    tArticle->mPotentialVector[0] = tNodes[0].getOutflow()->getPressure();
    tArticle->mPotentialVector[1] = tNodes[1].getOutflow()->getPressure();
    tArticle->mAdmittanceUpdate   = false;
    tArticle->setMinLinearizationPotential(1.0e-6);
    tArticle->step(0.01);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(Tc,  tArticle->mCombustionTemperature, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(gc,  tArticle->mCombustionGamma,       0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(MWc, tArticle->mCombustionMWeight,     0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(Tc,  tArticle->mChamberTemperature,    0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(gc,  tArticle->mChamberGamma,          0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(MWc, tArticle->mChamberMWeight,        0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(MWe, tArticle->mExitMWeight,           0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(Me,  tArticle->mExitMach,              0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(Pe,  tArticle->mExitPressure,          0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(Ue,  tArticle->mExitVelocity,          0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(Te,  tArticle->mExitTemperature,       0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(Isp, tArticle->mSpecificImpulse,       0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( G,  tArticle->mSystemConductance,     0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( G,  tArticle->mAdmittanceMatrix[0],   0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-G,  tArticle->mAdmittanceMatrix[1],   0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-G,  tArticle->mAdmittanceMatrix[2],   0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( G,  tArticle->mAdmittanceMatrix[3],   0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0,  tArticle->mSourceVector[0],       0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0,  tArticle->mSourceVector[1],       0.0001);
    CPPUNIT_ASSERT(true == tArticle->mAdmittanceUpdate);

    /// @test flammability limits exceeded in the combustion model, combustion switches off.  Switch
    ///       inlet port to a node that is mostly helium, check this extinguishes the combustion.
    tArticle->setPort(0, 2);
    tArticle->step(0.01);
    CPPUNIT_ASSERT(false == tArticle->mCombustion);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Simple Rocket link model compute flows.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleRocket::testComputeFlows()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tConfigData->mThrustTuning = 0.5;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test  Outputs when combusting.
    tArticle->mPotentialVector[0] = tNodes[0].getOutflow()->getPressure();
    tArticle->mPotentialVector[1] = tNodes[1].getOutflow()->getPressure();
    tArticle->setMinLinearizationPotential(1.0e-6);
    tArticle->mCombustion      = true;
    tArticle->mCombustionCount = 2;
    tArticle->step(0.01);

    double dP     = tArticle->mPotentialVector[0] - tArticle->mPotentialVector[1];
    double flux   = dP * tArticle->mAdmittanceMatrix[0];
    double mdot   = flux * tNodes[0].getOutflow()->getMWeight();
    double cstar  = tArticle->mPotentialVector[0] * 1000.0 * tThroatArea / mdot;
    double thrust = 0.5 * mdot * tArticle->mSpecificImpulse * 9.80665;
    double power  = 0.5 * thrust * tArticle->mExitVelocity;

    tArticle->computeFlows(0.01);
    tArticle->transportFlows(0.01);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(dP,     tArticle->mPotentialDrop,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(flux,   tArticle->mFlux,                   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mdot,   tArticle->mFlowRate,               DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(cstar,  tArticle->mCharacteristicVelocity, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(thrust, tArticle->mThrust,                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(thrust, tArticle->getThrust(),             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mdot,   tNodes[0].getOutflux(),            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mdot,   tNodes[1].getInflux(),             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(power,  tArticle->mPower,                  DBL_EPSILON);

    /// @test  Outputs when no flow.
    tArticle->mPotentialVector[0] = 0.0;
    tArticle->mPotentialVector[1] = 0.0;
    tArticle->step(0.01);

    dP     = 0.0;
    flux   = 0.0;
    mdot   = 0.0;
    cstar  = 0.0;
    thrust = 0.0;
    power  = 0.0;

    tNodes[0].resetFlows();
    tNodes[1].resetFlows();
    tArticle->computeFlows(0.01);
    tArticle->transportFlows(0.01);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(dP,     tArticle->mPotentialDrop,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(flux,   tArticle->mFlux,                   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mdot,   tArticle->mFlowRate,               DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(cstar,  tArticle->mCharacteristicVelocity, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(thrust, tArticle->mThrust,                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(thrust, tArticle->getThrust(),             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mdot,   tNodes[0].getOutflux(),            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mdot,   tNodes[1].getInflux(),             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(power,  tArticle->mPower,                  DBL_EPSILON);

    /// @test  Zero flux noise logic.
    tArticle->mPotentialVector[0] = 1.0;
    tArticle->mPotentialVector[1] = 1.0 + 10.0*DBL_EPSILON;
    tArticle->computeFlows(0.01);
    tArticle->transportFlows(0.01);

    CPPUNIT_ASSERT(0.0 == tArticle->mFlux);
    CPPUNIT_ASSERT(0.0 == tArticle->mFlowRate);

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    tArticle->step(0.01);
    tArticle->computeFlows(0.01);

   /// - Confirm correct null port allocation with zero potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tArticle->mPortDirections[0]);

    /// - Confirm correct null port allocation with zero potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tArticle->mPortDirections[1]);

    tArticle->transportFlows(0.01);

    /// - Conductance Link should have zero flow rate because the potential vector is zero
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mFlowRate, DBL_EPSILON);

    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    tArticle->mCombustionCount = 2;
    tArticle->mPotentialVector[0] = 1.0;
    tArticle->mPotentialVector[1] = 0.0;
    tArticle->step(0.01);
    tArticle->computeFlows(0.01);

    /// - Confirm correct source port allocation with postive potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tArticle->mPortDirections[0]);

    /// - Confirm correct Sink port allocation with postive potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::SINK == tArticle->mPortDirections[1]);

    /// - Nodal outflux scheduling should be equal to step molar flux for source node
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getScheduledOutflux(),tArticle->mFlux,DBL_EPSILON);

    tArticle->transportFlows(0.01);

    /// - Conductance Link should have positive flow rate because the port 0 potential vector is positive
    CPPUNIT_ASSERT(tArticle->mFlowRate > 0.0);

    tArticle->mCombustionCount = 2;
    tArticle->mPotentialVector[0] = -1.0;
    tArticle->mPotentialVector[1] = 0.0;
    tArticle->computeFlows(0.01);

    /// - Confirm correct sink port allocation with negative potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::SINK == tArticle->mPortDirections[0]);

    /// - Confirm correct source port allocation with negative potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tArticle->mPortDirections[1]);

    /// - Nodal outflux scheduling should be equal to step molar flux for source node
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[1].getScheduledOutflux(),-tArticle->mFlux,DBL_EPSILON);

    tArticle->transportFlows(0.01);

   /// - Conductance Link should have zero flow rate because the potential vector is zero
    CPPUNIT_ASSERT(tArticle->mFlowRate < 0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Simple Rocket link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleRocket::testInitializationExceptions()
{
    UT_RESULT;

    /// - Default construct a test article.
    FriendlyGunnsFluidSimpleRocket article;

    /// @test    Initialization exception invalid config data:no name.
    tConfigData->mName = "";
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mName = tName;

    /// @test    Initialization exception on invalid config data: throat area < DBL_EPSILON.
    tConfigData->mThroatArea = 0.0;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mThroatArea = tThroatArea;

    /// @test    Initialization exception on invalid config data: area ratio < 1.
    tConfigData->mAreaRatio = 0.999;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mAreaRatio = tAreaRatio;

    /// @test    Initialization exception on invalid config data: characteristic velocity < DBL_EPSILON.
    tConfigData->mCharacteristicVelocity = 0.0;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mCharacteristicVelocity = tCharacteristicVelocity;

    /// @test    Initialization exception on invalid config data: combustion temperature < DBL_EPSILON.
    tConfigData->mCombustionTemperature = 0.0;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mCombustionTemperature = tCombustionTemperature;

    /// @test    Initialization exception on invalid config data: combustion gamma <= 1.
    tConfigData->mCombustionGamma = 1.0;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mCombustionGamma = tCombustionGamma;

    /// @test    Initialization exception on invalid config data: thrust tuning factor < 0.
    tConfigData->mThrustTuning = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mThrustTuning = tThrustTuning;

    /// @test    Initialization exception on invalid config data: recombination tuning factor not (0-1).
    tConfigData->mRecombinationTuning = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mRecombinationTuning = 1.001;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mRecombinationTuning = tRecombinationTuning;

    CPPUNIT_ASSERT(!article.mInitFlag);

    UT_PASS_LAST;
}
