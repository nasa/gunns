/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/fluid/conductor/GunnsFluidBalancedPrv.o))
***************************************************************************************************/

#include "core/GunnsFluidUtils.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidBalancedPrv.hh"

/// @details  Test identification number.
int UtGunnsFluidBalancedPrv::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Balanced Pressure Reducing Valve link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidBalancedPrv::UtGunnsFluidBalancedPrv()
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
    tPort2(0),
    tExitPressureDroop(0.0),
    tConfigData(0),
    tMalfBlockageFlag(false),
    tMalfBlockageValue(0.0),
    tMalfPressureBiasFlag(false),
    tMalfPressureBiasValue(0.0),
    tPressureSetpoint(0.0),
    tInputData(0),
    tArticle(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Balanced Pressure Reducing Valve link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidBalancedPrv::~UtGunnsFluidBalancedPrv()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////

void UtGunnsFluidBalancedPrv::setUp()
{
    /// - Define the nominal port fluids.
    tFluidProperties        = new DefinedFluidProperties();
    tTypes[0]               = FluidProperties::GUNNS_N2;
    tTypes[1]               = FluidProperties::GUNNS_O2;
    tFluidConfig            = new PolyFluidConfigData(tFluidProperties, tTypes, N_FLUIDS);

    tFractions0[0]          = 0.8;
    tFractions0[1]          = 0.2;
    tFluidInput0            = new PolyFluidInputData( 330.0,
                                                     9999.0,
                                                        0.0,
                                                        0.0,
                                                     tFractions0);

    tFractions1[0]          = 1.0;
    tFractions1[1]          = 0.0;
    tFluidInput1            = new PolyFluidInputData( 294.261,
                                                      101.325,
                                                        0.0,
                                                        0.0,
                                                     tFractions1);

    /// - Initialize the nodes.  Nodes 0, 1, 2 are nominal inlet, outlet & reference pressure nodes,
    ///   respectively.  Node 3 is an un-initialized non-Ground node for edge-checking.  Node 4 is
    ///   the Ground node.
    tNodes[0].initialize("UtNode0", tFluidConfig);
    tNodes[1].initialize("UtNode1", tFluidConfig);
    tNodes[2].initialize("UtNode2", tFluidConfig);
    tNodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput0);
    tNodes[1].getContent()->initialize(*tFluidConfig, *tFluidInput1);
    tNodes[2].getContent()->initialize(*tFluidConfig, *tFluidInput1);

    tNodes[0].resetFlows();
    tNodes[1].resetFlows();
    tNodes[2].resetFlows();

    /// - Initialize the nodes list.
    tNodeList.mNodes       = tNodes;
    tNodeList.mNumNodes    = N_NODES;

    /// - Define the nominal configuration data.
    tName                  = "nominal";
    tExitPressureDroop     = 2000.0;
    tConfigData            = new GunnsFluidBalancedPrvConfigData(tName,
                                                                 &tNodeList,
                                                                 tExitPressureDroop);

    /// - Define the nominal input data.
    tMalfBlockageFlag      = false;
    tMalfBlockageValue     = 0.0;
    tMalfPressureBiasFlag  = false;
    tMalfPressureBiasValue = 0.0;
    tPressureSetpoint      = 1000.0;
    tInputData             = new GunnsFluidBalancedPrvInputData(tMalfBlockageFlag,
                                                                tMalfBlockageValue,
                                                                tMalfPressureBiasFlag,
                                                                tMalfPressureBiasValue,
                                                                tPressureSetpoint);

    /// - Define the nominal port mapping.
    tPort0                 = 0;
    tPort1                 = 1;
    tPort2                 = 2;

    /// - Default construct the nominal test article.
    tArticle               = new FriendlyGunnsFluidBalancedPrv;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidBalancedPrv::tearDown()
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
/// @details  Tests for Balanced Pressure Reducing Valve link model construction of configuration.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidBalancedPrv::testConfig()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data nominal construction.
    GunnsFluidBalancedPrvConfigData nominalConfig(tName,
                                                  &tNodeList,
                                                  1.0);
    CPPUNIT_ASSERT(tName                            == nominalConfig.mName);
    CPPUNIT_ASSERT(tNodes                           == nominalConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(1.0                              == nominalConfig.mExitPressureDroop);

    /// @test    Configuration data default construction.
    GunnsFluidBalancedPrvConfigData defaultConfig;
    CPPUNIT_ASSERT(0                                == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0                                == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0                              == defaultConfig.mExitPressureDroop);

       /// @test    Configuration data copy construction.
    GunnsFluidBalancedPrvConfigData copyConfig(nominalConfig);
    CPPUNIT_ASSERT(nominalConfig.mName              == copyConfig.mName);
    CPPUNIT_ASSERT(nominalConfig.mNodeList->mNodes  == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(nominalConfig.mExitPressureDroop == copyConfig.mExitPressureDroop);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Balanced Pressure Reducing Valve link model construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidBalancedPrv::testInput()
{
    UT_RESULT;

    GunnsFluidBalancedPrvInputData nominalInput(true,
                                                   0.5,
                                                   true,
                                                   1.0,
                                                   2.0);

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(true                                == nominalInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.5                                 == nominalInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(true                                == nominalInput.mMalfPressureBiasFlag);
    CPPUNIT_ASSERT(1.0                                 == nominalInput.mMalfPressureBiasValue);
    CPPUNIT_ASSERT(2.0                                 == nominalInput.mPressureSetpoint);

    /// @test    Input data default construction.
    GunnsFluidBalancedPrvInputData defaultInput;
    CPPUNIT_ASSERT(false                               == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                                 == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(false                               == defaultInput.mMalfPressureBiasFlag);
    CPPUNIT_ASSERT(0.0                                 == defaultInput.mMalfPressureBiasValue);
    CPPUNIT_ASSERT(0.0                                 == defaultInput.mPressureSetpoint);

    /// @test    Input data copy construction.
    GunnsFluidBalancedPrvInputData copyInput(nominalInput);
    CPPUNIT_ASSERT(nominalInput.mMalfBlockageFlag      == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(nominalInput.mMalfBlockageValue     == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(nominalInput.mMalfPressureBiasFlag  == copyInput.mMalfPressureBiasFlag);
    CPPUNIT_ASSERT(nominalInput.mMalfPressureBiasValue == copyInput.mMalfPressureBiasValue);
    CPPUNIT_ASSERT(nominalInput.mPressureSetpoint      == copyInput.mPressureSetpoint);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Balanced Pressure Reducing Valve link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidBalancedPrv::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default of attributes.
    CPPUNIT_ASSERT(""    == tArticle->mName);
    CPPUNIT_ASSERT(0     == tArticle->mNodes);
    CPPUNIT_ASSERT(false == tArticle->mMalfPressureBiasFlag);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfPressureBiasValue);
    CPPUNIT_ASSERT(0.0   == tArticle->mExitPressureDroop);
    CPPUNIT_ASSERT(0.0   == tArticle->mPressureSetpoint);
    CPPUNIT_ASSERT(0.0   == tArticle->mInletConductance);
    CPPUNIT_ASSERT(0.0   == tArticle->mExitConductance);
    CPPUNIT_ASSERT(0.0   == tArticle->mInletMassBuffer);
    CPPUNIT_ASSERT(0.0   == tArticle->mRegulatedPressure);
    CPPUNIT_ASSERT(0.0   == tArticle->mInletFlux);
    CPPUNIT_ASSERT(0.0   == tArticle->mInletFlowRate);
    CPPUNIT_ASSERT(false == tArticle->mBackPressureCutoff);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(false == tArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsFluidBalancedPrv* article = new GunnsFluidBalancedPrv();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Balanced Pressure Reducing Valve link model nominal initialization without
///           exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidBalancedPrv::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2));

    /// @test    Nominal configuration data.
    CPPUNIT_ASSERT(tName                  == tArticle->mName);
    CPPUNIT_ASSERT(&tNodes[0]             == tArticle->mNodes[0]);
    CPPUNIT_ASSERT(&tNodes[1]             == tArticle->mNodes[1]);
    CPPUNIT_ASSERT(&tNodes[2]             == tArticle->mNodes[2]);
    CPPUNIT_ASSERT(tExitPressureDroop     == tArticle->mExitPressureDroop);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT(tMalfBlockageFlag      == tArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue     == tArticle->mMalfBlockageValue);
    CPPUNIT_ASSERT(tMalfPressureBiasFlag  == tArticle->mMalfPressureBiasFlag);
    CPPUNIT_ASSERT(tMalfPressureBiasValue == tArticle->mMalfPressureBiasValue);
    CPPUNIT_ASSERT(tPressureSetpoint      == tArticle->mPressureSetpoint);

    /// @test    Nominal state data.
    CPPUNIT_ASSERT(0.0                    == tArticle->mInletConductance);
    CPPUNIT_ASSERT(0.0                    == tArticle->mExitConductance);
    CPPUNIT_ASSERT(0.0                    == tArticle->mInletMassBuffer);
    CPPUNIT_ASSERT(0.0                    == tArticle->mRegulatedPressure);
    CPPUNIT_ASSERT(0.0                    == tArticle->mInletFlux);
    CPPUNIT_ASSERT(0.0                    == tArticle->mInletFlowRate);
    CPPUNIT_ASSERT(false                  == tArticle->mBackPressureCutoff);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(tArticle->mInitFlag);

    /// @test    Verify restartModel functionality
    tArticle->mInletConductance  = 1.0;
    tArticle->mExitConductance   = 1.0;
    tArticle->mRegulatedPressure = 1.0;
    tArticle->mInletFlux         = 1.0;
    tArticle->mInletFlowRate     = 1.0;

    tArticle->restartModel();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,tArticle->mInletConductance,DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,tArticle->mExitConductance,DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,tArticle->mRegulatedPressure,DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,tArticle->mInletFlux,DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,tArticle->mInletFlowRate,DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Balanced Pressure Reducing Valve link model restart.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidBalancedPrv::testRestart()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2);

    /// @ test    Class state attributes are reset.
    tArticle->mInletConductance      = 1.0;
    tArticle->mExitConductance       = 1.0;
    tArticle->mRegulatedPressure     = 1.0;
    tArticle->mInletFlux             = 1.0;
    tArticle->mInletFlowRate         = 1.0;
    tArticle->restart();
    CPPUNIT_ASSERT(0.0 == tArticle->mInletConductance);
    CPPUNIT_ASSERT(0.0 == tArticle->mExitConductance);
    CPPUNIT_ASSERT(0.0 == tArticle->mRegulatedPressure);
    CPPUNIT_ASSERT(0.0 == tArticle->mInletFlux);
    CPPUNIT_ASSERT(0.0 == tArticle->mInletFlowRate);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Balanced Pressure Reducing Valve link step method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidBalancedPrv::testStep()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2);

    /// @test Nominal exit potential source.
    tArticle->mPotentialVector[0] = tNodes[tPort0].getOutflow()->getPressure();
    tArticle->mPotentialVector[1] = tNodes[tPort1].getOutflow()->getPressure();
    tArticle->mPotentialVector[2] = tNodes[tPort2].getOutflow()->getPressure();
    tArticle->setMinLinearizationPotential(1.0e-6);
    tArticle->step(0.01);

    double expectedRegP = tPressureSetpoint + tNodes[tPort2].getOutflow()->getPressure();
    double expectedOutG = 1.0 / tExitPressureDroop / tNodes[tPort0].getOutflow()->getMWeight();
    double expectedInG  = 0.0;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRegP, tArticle->mRegulatedPressure,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedOutG, tArticle->mExitConductance,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedInG,  tArticle->mInletConductance,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedInG,  tArticle->mAdmittanceMatrix[0],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mAdmittanceMatrix[1],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mAdmittanceMatrix[2],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mAdmittanceMatrix[3],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedOutG, tArticle->mAdmittanceMatrix[4],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mAdmittanceMatrix[5],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mAdmittanceMatrix[6],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mAdmittanceMatrix[7],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mAdmittanceMatrix[8],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0,            tArticle->mSourceVector[0],      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRegP * expectedOutG,
                                               tArticle->mSourceVector[1],      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0,            tArticle->mSourceVector[2],      DBL_EPSILON);
    CPPUNIT_ASSERT(true  == tArticle->mAdmittanceUpdate);
    CPPUNIT_ASSERT(false == tArticle->mBackPressureCutoff);

    /// @test  Nominal exit conductance and nominal inlet conductance.
    tArticle->mInletMassBuffer = 1.0;
    tArticle->step(0.01);

    double inMdot = 1.0 / 5.0 / 0.01;
    expectedInG = inMdot / tNodes[0].getOutflow()->getMWeight()
                / tNodes[tPort0].getOutflow()->getPressure();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRegP, tArticle->mRegulatedPressure,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedOutG, tArticle->mExitConductance,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedInG,  tArticle->mInletConductance,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedInG,  tArticle->mAdmittanceMatrix[0],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mAdmittanceMatrix[1],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mAdmittanceMatrix[2],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mAdmittanceMatrix[3],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedOutG, tArticle->mAdmittanceMatrix[4],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mAdmittanceMatrix[5],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mAdmittanceMatrix[6],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mAdmittanceMatrix[7],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mAdmittanceMatrix[8],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0,            tArticle->mSourceVector[0],      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRegP * expectedOutG,
                                               tArticle->mSourceVector[1],      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0,            tArticle->mSourceVector[2],      DBL_EPSILON);
    CPPUNIT_ASSERT(true  == tArticle->mAdmittanceUpdate);
    CPPUNIT_ASSERT(false == tArticle->mBackPressureCutoff);

    /// @test  Backflow cut-off, and nominal inlet conductance.
    tArticle->mPotentialVector[1] = 10000.0;
    tArticle->step(0.01);

    expectedOutG = 0.0;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRegP, tArticle->mRegulatedPressure,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedOutG, tArticle->mExitConductance,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedInG,  tArticle->mInletConductance,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedInG,  tArticle->mAdmittanceMatrix[0],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mAdmittanceMatrix[1],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mAdmittanceMatrix[2],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mAdmittanceMatrix[3],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedOutG, tArticle->mAdmittanceMatrix[4],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mAdmittanceMatrix[5],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mAdmittanceMatrix[6],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mAdmittanceMatrix[7],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mAdmittanceMatrix[8],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0,            tArticle->mSourceVector[0],      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRegP * expectedOutG,
                                               tArticle->mSourceVector[1],      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0,            tArticle->mSourceVector[2],      DBL_EPSILON);
    CPPUNIT_ASSERT(true  == tArticle->mAdmittanceUpdate);
    CPPUNIT_ASSERT(true  == tArticle->mBackPressureCutoff);

    /// @test  Backflow middle zone.
    tArticle->mPotentialVector[1] = 5000.0;
    tArticle->step(0.01);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRegP, tArticle->mRegulatedPressure,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedOutG, tArticle->mExitConductance,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedInG,  tArticle->mInletConductance,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedInG,  tArticle->mAdmittanceMatrix[0],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mAdmittanceMatrix[1],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mAdmittanceMatrix[2],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mAdmittanceMatrix[3],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedOutG, tArticle->mAdmittanceMatrix[4],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mAdmittanceMatrix[5],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mAdmittanceMatrix[6],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mAdmittanceMatrix[7],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mAdmittanceMatrix[8],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0,            tArticle->mSourceVector[0],      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRegP * expectedOutG,
                                               tArticle->mSourceVector[1],      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0,            tArticle->mSourceVector[2],      DBL_EPSILON);
    CPPUNIT_ASSERT(false == tArticle->mAdmittanceUpdate);
    CPPUNIT_ASSERT(true  == tArticle->mBackPressureCutoff);

    /// @test  Backflow reset, setpoint bias malf, blockage malf, and dt = 0.
    tArticle->mPotentialVector[1] = tNodes[tPort1].getOutflow()->getPressure();
    tArticle->setMalfBlockage(true, 0.5);
    tArticle->setMalfPressureBias(true, 10.0);
    tArticle->step(0.0);

    expectedRegP = tPressureSetpoint + tNodes[tPort2].getOutflow()->getPressure() + 10.0;
    expectedOutG = 0.5 / tExitPressureDroop / tNodes[tPort0].getOutflow()->getMWeight();
    expectedInG  = 0.0;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRegP, tArticle->mRegulatedPressure,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedOutG, tArticle->mExitConductance,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedInG,  tArticle->mInletConductance,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedInG,  tArticle->mAdmittanceMatrix[0],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mAdmittanceMatrix[1],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mAdmittanceMatrix[2],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mAdmittanceMatrix[3],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedOutG, tArticle->mAdmittanceMatrix[4],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mAdmittanceMatrix[5],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mAdmittanceMatrix[6],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mAdmittanceMatrix[7],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mAdmittanceMatrix[8],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0,            tArticle->mSourceVector[0],      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRegP * expectedOutG,
                                               tArticle->mSourceVector[1],      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0,            tArticle->mSourceVector[2],      DBL_EPSILON);
    CPPUNIT_ASSERT(true  == tArticle->mAdmittanceUpdate);
    CPPUNIT_ASSERT(false == tArticle->mBackPressureCutoff);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Balanced Pressure Reducing Valve link step method edge cases.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidBalancedPrv::testStepEdgeCases()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2);

    /// @test  Regulated pressure limited to inlet pressure.
    tArticle->mPotentialVector[0] = 10.0;
    tArticle->step(0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(10.0, tArticle->mRegulatedPressure, DBL_EPSILON);

    /// @test  Link port mapping prevents connecting inlet to Ground node.
    tArticle->mUserPortSelect     = 0;
    tArticle->mUserPortSelectNode = 4;
    tArticle->mUserPortSetControl = GunnsBasicLink::EXECUTE;
    tArticle->step(0.01);
    CPPUNIT_ASSERT(&tNodes[0]             == tArticle->mNodes[0]);
    CPPUNIT_ASSERT(&tNodes[1]             == tArticle->mNodes[1]);
    CPPUNIT_ASSERT(&tNodes[2]             == tArticle->mNodes[2]);
    CPPUNIT_ASSERT(GunnsBasicLink::FAILED == tArticle->mUserPortSetControl);

    /// @test  Limited lower value of exit pressure droop.
    tArticle->mExitPressureDroop = 0.0;
    tArticle->step(0.01);
    double expectedOutG = 1.0 / FLT_EPSILON / tNodes[tPort0].getOutflow()->getMWeight();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedOutG, tArticle->mExitConductance, DBL_EPSILON);

    /// @test  Limited upper value of exit pressure droop.
    tArticle->mExitPressureDroop = 1.0e15;
    tArticle->step(0.01);
    expectedOutG = 1.0 * FLT_EPSILON / tNodes[tPort0].getOutflow()->getMWeight();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedOutG, tArticle->mExitConductance, DBL_EPSILON);

    /// @test  Link port mapping and protect against zero inlet molecular weight.
    tArticle->mUserPortSelect     = 0;
    tArticle->mUserPortSelectNode = 3;
    tArticle->mUserPortSetControl = GunnsBasicLink::EXECUTE;
    tArticle->step(0.01);
    expectedOutG = 0.0;
    CPPUNIT_ASSERT(&tNodes[3]             == tArticle->mNodes[0]);
    CPPUNIT_ASSERT(&tNodes[1]             == tArticle->mNodes[1]);
    CPPUNIT_ASSERT(&tNodes[2]             == tArticle->mNodes[2]);
    CPPUNIT_ASSERT(GunnsBasicLink::READY  == tArticle->mUserPortSetControl);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedOutG, tArticle->mExitConductance, DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Balanced Pressure Reducing Valve link model compute flows.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidBalancedPrv::testComputeFlows()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2);

    tArticle->mPotentialVector[0]  = tNodes[tPort0].getOutflow()->getPressure();
    tArticle->mPotentialVector[1]  = tNodes[tPort1].getOutflow()->getPressure();
    tArticle->mAdmittanceMatrix[0] = 0.001;
    tArticle->mAdmittanceMatrix[4] = 0.005;
    tArticle->mSourceVector[1]     = 1.5;
    tArticle->mInletMassBuffer     = 3.0;

    const double expectedDp      = tArticle->mPotentialVector[0] - tArticle->mPotentialVector[1];
    const double expectedInFlux  = tArticle->mPotentialVector[0] * tArticle->mAdmittanceMatrix[0];
    const double expectedOutFlux = tArticle->mSourceVector[1]
                                 - tArticle->mPotentialVector[1] * tArticle->mAdmittanceMatrix[4];
    const double expectedInMdot  = expectedInFlux  * tNodes[tPort0].getOutflow()->getMWeight();
    const double expectedOutMdot = expectedOutFlux * tNodes[tPort0].getOutflow()->getMWeight();
    const double expectedQ       = expectedOutMdot / tNodes[tPort0].getOutflow()->getDensity();
    const double expectedPower   = -1000.0 * expectedQ * expectedDp;
    const double expectedBuffer  = tArticle->mInletMassBuffer + (expectedOutMdot - expectedInMdot) * 0.01;

    /// @test  Nominal inlet and exit flows.
    tArticle->computeFlows(0.01);
    tArticle->transportFlows(0.01);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDp,      tArticle->mPotentialDrop,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedInFlux,  tArticle->mInletFlux,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedOutFlux, tArticle->mFlux,            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedInMdot,  tArticle->mInletFlowRate,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedOutMdot, tArticle->mFlowRate,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedQ,       tArticle->mVolFlowRate,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower,   tArticle->mPower,           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedBuffer,  tArticle->mInletMassBuffer, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedInMdot,  tNodes[0].getOutflux(),     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedOutMdot, tNodes[1].getInflux(),      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[1].getInflow()->getMWeight(),
                                 tNodes[0].getOutflow()->getMWeight(),        FLT_EPSILON);

    /// @test  Verify correct port directions
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2);
    tArticle->mPotentialVector[0] = 0.8;
    tArticle->mAdmittanceMatrix[0] = 0.001;
    tArticle->computeFlows(0.01);

    /// - Molar flux should be less than zero because the port 0 potential vector is positive
    CPPUNIT_ASSERT( tArticle->mInletFlux > 0.0);

    /// - Confirm correct source port allocation with positive potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tArticle->mPortDirections[0]);

    /// - Confirm correct sink port allocation with positive potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::SINK == tArticle->mPortDirections[1]);

    /// - Confirm correct none port allocation with positive potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tArticle->mPortDirections[2]);

    /// - Nodal outflux scheduling should be equal to step molar flux for source node
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getScheduledOutflux(),
                                           tArticle->mInletFlux + expectedInFlux,DBL_EPSILON);

    tArticle->mPotentialVector[0] = 0.0;
    tArticle->computeFlows(0.01);

    /// - Molar flux should be equal to zero because the port 0 potential vector is zero
    CPPUNIT_ASSERT( tArticle->mInletFlux == 0.0);

    /// - Confirm correct port allocation with positive potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tArticle->mPortDirections[0]);

    /// - Confirm correct port allocation with positive potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tArticle->mPortDirections[1]);

    /// - Confirm correct port allocation with positive potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tArticle->mPortDirections[2]);

    /// - Verify proper updating of portleakFlow values with outflow parameters
    tArticle->step(0.01);
    tArticle->computeFlows(0.01);
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
    tArticle->transportFlows(0.01);

    double tempMFlowRate = tNodes->getOutflow()->getMWeight() * tArticle->mFlux;
    double tempInletFlowRate = tNodes->getOutflow()->getMWeight() * tArticle->mInletFlux;

    CPPUNIT_ASSERT(tNodes[0].getOutflow()->getMWeight() !=
            tNodes[0].getContent()->getMWeight());

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tempMFlowRate, tArticle->mFlowRate, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tempInletFlowRate, tArticle->mInletFlowRate, DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Balanced Pressure Reducing Valve link model compute flows edge cases.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidBalancedPrv::testComputeFlowsEdgeCases()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2);

    /// @test  Negative inlet & exit flow rates
    tArticle->mPotentialVector[0]  = -1.0;
    tArticle->mPotentialVector[1]  =  1.0e5;
    tArticle->mAdmittanceMatrix[0] = 0.001;
    tArticle->mAdmittanceMatrix[4] = 0.005;
    tArticle->mSourceVector[1]     = 1.5;
    tArticle->mInletMassBuffer     = 3.0;

    tArticle->computeFlows(0.01);
    tArticle->transportFlows(0.01);

    CPPUNIT_ASSERT(0.0 == tArticle->mInletFlowRate);
    CPPUNIT_ASSERT(0.0 == tArticle->mFlowRate);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[0].getOutflux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[1].getInflux(),  DBL_EPSILON);

    /// @test  Negative inlet mass buffer.
    tArticle->mPotentialVector[0]  = 1000.0;
    tArticle->mAdmittanceMatrix[0] = 0.1;
    tArticle->mAdmittanceMatrix[4] = 0.0;
    tArticle->mSourceVector[1]     = 0.0;

    tArticle->computeFlows(0.01);
    tArticle->transportFlows(0.01);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  tArticle->mInletMassBuffer, DBL_EPSILON);

    /// @test  Zero source density.
    tArticle->mUserPortSelect     = 0;
    tArticle->mUserPortSelectNode = 3;
    tArticle->mUserPortSetControl = GunnsBasicLink::EXECUTE;
    tArticle->step(0.01);
    tArticle->computeFlows(0.01);
    tArticle->transportFlows(0.01);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mVolFlowRate, DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Balanced Pressure Reducing Valve setter and getter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidBalancedPrv::testAccessors()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2);

    /// @test  setPressureSetpoint.
    tArticle->setPressureSetpoint(15.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(15.0,  tArticle->mPressureSetpoint, DBL_EPSILON);

    /// @test  Get gauge pressures
    tArticle->mPotentialVector[0] = 1000.0;
    tArticle->mPotentialVector[1] =  200.0;
    tArticle->mPotentialVector[2] =  100.0;
    const double expectedInDp     = 1000.0 - 100.0;
    const double expectedOutDp    =  200.0 - 100.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedInDp,  tArticle->getInletGaugePressure(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedOutDp, tArticle->getExitGaugePressure(),  DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Balanced Pressure Reducing Valve link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidBalancedPrv::testInitializationExceptions()
{
    UT_RESULT;

    /// - Default construct a test article.
    FriendlyGunnsFluidBalancedPrv article;

    /// @test    Initialization exception invalid config data:no name.
    tConfigData->mName = "";
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2),
                         TsInitializationException);
    tConfigData->mName = tName;

    /// @test    Initialization exception on invalid config data: exit droop < 0.
    tConfigData->mExitPressureDroop = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2),
                         TsInitializationException);
    tConfigData->mExitPressureDroop = tExitPressureDroop;

    /// @test    Initialization exception on invalid input data: pressure setpoint < 0.
    tInputData->mPressureSetpoint = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2),
                         TsInitializationException);
    tInputData->mPressureSetpoint = tPressureSetpoint;

    CPPUNIT_ASSERT(!article.mInitFlag);

    UT_PASS_LAST;
}
