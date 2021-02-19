/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/fluid/capacitor/GunnsFluidBalloon.o))
***************************************************************************************************/

#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "core/GunnsFluidUtils.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidBalloon.hh"

/// @details  Test identification number.
int UtGunnsFluidBalloon::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Balloon link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidBalloon::UtGunnsFluidBalloon()
    :
    CppUnit::TestFixture(),
    tTypes(),
    tFluidProperties(0),
    tFluidConfig(0),
    tFractGas(),
    tFractLiq(),
    tFluidInputGas(0),
    tFluidInputLiq(0),
    tLinks(),
    tName(""),
    tNodes(),
    tNodeList(),
    tPort0(0),
    tPort1(0),
    tExpansionScaleFactor(0.0),
    tDpdtFilterGain(0.0),
    tThermalDampingMass(0.0),
    tEditFluxTarget(0.0),
    tSurfaceArea(0.0),
    tShellRadius(0.0),
    tInflatability(0.0),
    tMaxVolume(0.0),
    tShellTemperature(0.0),
    tBiasHeatFlux(0.0),
    tConfigData(0),
    tInputData(0),
    tArticle(0),
    tTimeStep(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Balloon link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidBalloon::~UtGunnsFluidBalloon()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidBalloon::setUp()
{
    /// - Define nominal port fluids.
    tFluidProperties      = new DefinedFluidProperties();
    tTypes[0]             = FluidProperties::GUNNS_N2;
    tTypes[1]             = FluidProperties::GUNNS_O2;
    tTypes[2]             = FluidProperties::GUNNS_WATER;
    tFractGas[0] = 0.5;
    tFractGas[1] = 0.5;
    tFractGas[2] = 0.0;
    tFluidConfig          = new PolyFluidConfigData(tFluidProperties, tTypes, N_FLUIDS);
    tFluidInputGas        = new PolyFluidInputData(283.0,
                                                   100.0,
                                                     0.0,
                                                     0.0,
                                                   tFractGas);

    tFractLiq[0] = 0.0;
    tFractLiq[1] = 0.0;
    tFractLiq[2] = 1.0;
    tFluidInputLiq        = new PolyFluidInputData(283.0,
                                                   100.0,
                                                     0.0,
                                                     0.0,
                                                   tFractLiq);

    /// - Initialize the nodes.
    tNodes[0].initialize("UtNode0", tFluidConfig);
    tNodes[1].initialize("UtNode1", tFluidConfig);
    tNodes[2].initialize("UtNode2", tFluidConfig);
    tNodes[0].getContent()->initialize(*tFluidConfig, *tFluidInputGas);
    tNodes[1].getContent()->initialize(*tFluidConfig, *tFluidInputGas);
    tNodes[2].getContent()->initialize(*tFluidConfig, *tFluidInputLiq);

    /// - Initialize the nodes list.
    tNodeList.mNodes      = tNodes;
    tNodeList.mNumNodes   = N_NODES;

    /// - Define the nominal configuration data.
    tName                 = "nominal";
    tExpansionScaleFactor = 0.5;
    tDpdtFilterGain       = 0.1;
    tThermalDampingMass   = 1.0;
    tEditFluxTarget       = 1.0E-7;
    tShellRadius          = 0.0;
    tSurfaceArea          = 0.0;
    tInflatability        = 0.01;
    tMaxVolume            = 2.0;
    tConfigData           = new GunnsFluidBalloonConfigData(tName,
                                                           &tNodeList,
                                                            tExpansionScaleFactor,
                                                            tDpdtFilterGain,
                                                            tThermalDampingMass,
                                                            tEditFluxTarget,
                                                            tSurfaceArea,
                                                            tShellRadius,
                                                            tInflatability,
                                                            tMaxVolume);

    /// - Define the nominal input data.
    tShellTemperature     = 300.0;
    tBiasHeatFlux         = 10.0;
    tInputData            = new GunnsFluidBalloonInputData(false, false, 0.0,
                                                           tFluidInputGas,
                                                           tShellTemperature,
                                                           tBiasHeatFlux);

    /// - Define the nominal port mapping.
    tPort0                = 0;
    tPort1                = 3;

    /// - Default construct the nominal test article.
    tArticle              = new FriendlyGunnsFluidBalloon;

    /// - Define the nominal time step.
    tTimeStep             = 0.1;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidBalloon::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete tFluidInputLiq;
    delete tFluidInputGas;
    delete tFluidConfig;
    delete tFluidProperties;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of GUNNS Fluid Balloon link model configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidBalloon::testConfigAndInput()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(tName                 == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tExpansionScaleFactor == tConfigData->mExpansionScaleFactor);
    CPPUNIT_ASSERT(tDpdtFilterGain       == tConfigData->mDpdtFilterGain);
    CPPUNIT_ASSERT(tThermalDampingMass   == tConfigData->mThermalDampingMass);
    CPPUNIT_ASSERT(tEditFluxTarget       == tConfigData->mEditFluxTarget);
    CPPUNIT_ASSERT(tSurfaceArea          == tConfigData->mSurfaceArea);
    CPPUNIT_ASSERT(tShellRadius          == tConfigData->mShellRadius);
    CPPUNIT_ASSERT(tInflatability        == tConfigData->mInflatability);
    CPPUNIT_ASSERT(tMaxVolume            == tConfigData->mMaxVolume);

    /// @test    Input data nominal construction.
    GunnsFluidBalloonInputData nominalInput(true, true, 1.0, tFluidInputGas,
                                            tShellTemperature, tBiasHeatFlux);
    CPPUNIT_ASSERT(false                 == nominalInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                   == nominalInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(true                  == nominalInput.mMalfStuckFlag);
    CPPUNIT_ASSERT(true                  == nominalInput.mMalfInflatabilityScaleFlag);
    CPPUNIT_ASSERT(1.0                   == nominalInput.mMalfInflatabilityScaleValue);
    CPPUNIT_ASSERT(tFluidInputGas        == nominalInput.mInitialFluidState);
    CPPUNIT_ASSERT(0.0                   == nominalInput.mInitialVolume);
    CPPUNIT_ASSERT(tShellTemperature     == nominalInput.mShellTemperature);
    CPPUNIT_ASSERT(tBiasHeatFlux         == nominalInput.mBiasHeatFlux);

    /// @test    Configuration data default construction.
    GunnsFluidBalloonConfigData defaultConfig;
    CPPUNIT_ASSERT(""                    == defaultConfig.mName);
    CPPUNIT_ASSERT(0                     == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mDpdtFilterGain);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mThermalDampingMass);
    CPPUNIT_ASSERT(1.0E-6                == defaultConfig.mEditFluxTarget);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mSurfaceArea);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mShellRadius);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mInflatability);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mMaxVolume);

    /// @test    Input data default construction.
    GunnsFluidBalloonInputData defaultInput;
    CPPUNIT_ASSERT(false                 == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                   == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(false                 == defaultInput.mMalfStuckFlag);
    CPPUNIT_ASSERT(false                 == defaultInput.mMalfInflatabilityScaleFlag);
    CPPUNIT_ASSERT(0.0                   == defaultInput.mMalfInflatabilityScaleValue);
    CPPUNIT_ASSERT(0                     == defaultInput.mInitialFluidState);
    CPPUNIT_ASSERT(0.0                   == defaultInput.mInitialVolume);
    CPPUNIT_ASSERT(0.0                   == defaultInput.mShellTemperature);
    CPPUNIT_ASSERT(0.0                   == defaultInput.mBiasHeatFlux);

    /// @test    Configuration data copy construction.
    GunnsFluidBalloonConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tName                 == copyConfig.mName);
    CPPUNIT_ASSERT(tNodes                == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tExpansionScaleFactor == copyConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(tDpdtFilterGain       == copyConfig.mDpdtFilterGain);
    CPPUNIT_ASSERT(tThermalDampingMass   == copyConfig.mThermalDampingMass);
    CPPUNIT_ASSERT(tEditFluxTarget       == copyConfig.mEditFluxTarget);
    CPPUNIT_ASSERT(tSurfaceArea          == copyConfig.mSurfaceArea);
    CPPUNIT_ASSERT(tShellRadius          == copyConfig.mShellRadius);
    CPPUNIT_ASSERT(tInflatability        == copyConfig.mInflatability);
    CPPUNIT_ASSERT(tMaxVolume            == copyConfig.mMaxVolume);

    /// @test    Input data copy construction.
    GunnsFluidBalloonInputData copyInput(nominalInput);
    CPPUNIT_ASSERT(false                 == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                   == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(true                  == copyInput.mMalfStuckFlag);
    CPPUNIT_ASSERT(true                  == copyInput.mMalfInflatabilityScaleFlag);
    CPPUNIT_ASSERT(1.0                   == copyInput.mMalfInflatabilityScaleValue);
    CPPUNIT_ASSERT(tFluidInputGas        == copyInput.mInitialFluidState);
    CPPUNIT_ASSERT(0.0                   == copyInput.mInitialVolume);
    CPPUNIT_ASSERT(tShellTemperature     == copyInput.mShellTemperature);
    CPPUNIT_ASSERT(tBiasHeatFlux         == copyInput.mBiasHeatFlux);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Balloon link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidBalloon::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default base class data.
    CPPUNIT_ASSERT(""                          == tArticle->mName);
    CPPUNIT_ASSERT(0                           == tArticle->mNConstituents);

    /// @test    Default balloon data.
    CPPUNIT_ASSERT(false                       == tArticle->mMalfStuckFlag);
    CPPUNIT_ASSERT(false                       == tArticle->mMalfInflatabilityScaleFlag);
    CPPUNIT_ASSERT(0.0                         == tArticle->mMalfInflatabilityScaleValue);
    CPPUNIT_ASSERT(0.0                         == tArticle->mInflatability);
    CPPUNIT_ASSERT(0.0                         == tArticle->mMaxVolume);
    CPPUNIT_ASSERT(0.0                         == tArticle->mInflation);
    CPPUNIT_ASSERT(GunnsFluidBalloon::DEFLATED == tArticle->mInflationState);
    CPPUNIT_ASSERT(0.0                         == tArticle->mPressureCorrection);
    CPPUNIT_ASSERT(0.0                         == tArticle->mPressureCorrectionGain);
    CPPUNIT_ASSERT(0.0                         == tArticle->mInflatableCapacitance);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsFluidBalloon* article = new GunnsFluidBalloon();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Balloon link model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidBalloon::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Nominal configuration data & base class init.
    CPPUNIT_ASSERT(tName                 == tArticle->mName);
    CPPUNIT_ASSERT(tNodes                == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tExpansionScaleFactor == tArticle->mExpansionScaleFactor);
    CPPUNIT_ASSERT(tDpdtFilterGain       == tArticle->mDpdtFilterGain);
    CPPUNIT_ASSERT(tEditFluxTarget       == tArticle->mEditFluxTarget);
    CPPUNIT_ASSERT(tSurfaceArea          == tArticle->mSurfaceArea);
    CPPUNIT_ASSERT(tShellRadius          == tArticle->mShellRadius);
    CPPUNIT_ASSERT(tInflatability        == tArticle->mInflatability);
    CPPUNIT_ASSERT(tMaxVolume            == tArticle->mMaxVolume);

    /// @test    Nominal state data.
    double volume    = tInflatability * (tNodes[tPort0].getPotential()
                                       - tNodes[tPort1].getPotential());
    double inflation = (volume     - GunnsFluidBalloon::mMinVolume)
                     / (tMaxVolume - GunnsFluidBalloon::mMinVolume);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(volume,    tArticle->mNodes[0]->getVolume(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(volume,    tArticle->getVolume(),            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(inflation, tArticle->mInflation,             FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(inflation, tArticle->getInflation(),         FLT_EPSILON);
    CPPUNIT_ASSERT(GunnsFluidBalloon::INFLATING  == tArticle->mInflationState);
    CPPUNIT_ASSERT(GunnsFluidBalloon::INFLATING  == tArticle->getInflationState());
    CPPUNIT_ASSERT(0.0                           == tArticle->mPressureCorrection);
    CPPUNIT_ASSERT(0.0                           == tArticle->mPressureCorrectionGain);
    CPPUNIT_ASSERT(0.0                           == tArticle->mInflatableCapacitance);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(tArticle->mInitFlag);

    /// @test    Init with non-Ground external node.
    tPort1    = 1;
    volume    = GunnsFluidBalloon::mMinVolume;
    inflation = (volume     - GunnsFluidBalloon::mMinVolume)
              / (tMaxVolume - GunnsFluidBalloon::mMinVolume);
    FriendlyGunnsFluidBalloon article;
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(volume,    article.getVolume(),    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(inflation, article.getInflation(), DBL_EPSILON);
    CPPUNIT_ASSERT(GunnsFluidBalloon::DEFLATED == article.getInflationState());
    CPPUNIT_ASSERT(article.mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Balloon link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidBalloon::testInitializationExceptions()
{
    UT_RESULT;

    /// @test    Initialization exception on zero inflatability.
    tConfigData->mInflatability = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mInflatability = tInflatability;

    /// @test    Initialization exception on max volume = min volume.
    tConfigData->mMaxVolume = GunnsFluidBalloon::mMinVolume;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mMaxVolume = tMaxVolume;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Balloon link model restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidBalloon::testRestart()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Set attributes to be reset during restart.
    tArticle->mEditTemperatureFlag    = true;
    tArticle->mPressureCorrection     = 1.0;
    tArticle->mPressureCorrectionGain = 1.0;
    tArticle->mInflatableCapacitance  = 1.0;

    /// - Set up a loaded internal pressure from checkpoint.
    tArticle->mPotentialVector[0] = 1.0e6;

    /// @test    Restart method.
    tArticle->restart();
    CPPUNIT_ASSERT(false                       == tArticle->mEditTemperatureFlag);
    CPPUNIT_ASSERT(0.0                         == tArticle->mPressureCorrection);
    CPPUNIT_ASSERT(0.0                         == tArticle->mPressureCorrectionGain);
    CPPUNIT_ASSERT(0.0                         == tArticle->mInflatableCapacitance);
    CPPUNIT_ASSERT(1.0                         == tArticle->mInflation);
    CPPUNIT_ASSERT(GunnsFluidBalloon::INFLATED == tArticle->mInflationState);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Balloon link model step method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidBalloon::testStep()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    tArticle->mPotentialVector[0] = tNodes[tPort0].getPotential();
    tArticle->mPotentialVector[1] = tNodes[tPort1].getPotential();

    /// @test    Capacitance effect when partially inflated.
    double inflatableCap = tInflatability * tNodes[tPort0].getContent()->getDensity()
                                          / tNodes[tPort0].getContent()->getMWeight();
    double capacitance = inflatableCap
                       + GunnsFluidUtils::computeCapacitance(tNodes[tPort0].getContent(),
                                                             tNodes[tPort0].getVolume());
    double prevNodeT = tNodes[tPort0].mPreviousTemperature;

    tArticle->step(tTimeStep);

    double admittance = capacitance / tTimeStep;
    double source     = admittance * tNodes[tPort0].getPotential()
                      + tNodes[tPort0].mThermalCapacitance
                      * (tNodes[tPort0].getContent()->getTemperature() - prevNodeT)
                      / tTimeStep;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(inflatableCap, tArticle->mInflatableCapacitance, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(capacitance,   tArticle->mCapacitance,           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(admittance,    tArticle->mAdmittanceMatrix[0],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tArticle->mAdmittanceMatrix[1],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tArticle->mAdmittanceMatrix[2],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tArticle->mAdmittanceMatrix[3],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(source,        tArticle->mSourceVector[0],       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tArticle->mSourceVector[1],       DBL_EPSILON);

    /// @test    Capacitance effect when deflated.
    tArticle->mInflatability = 0.0;
    tNodes[tPort0].initVolume(GunnsFluidBalloon::mMinVolume);
    inflatableCap = DBL_EPSILON * tNodes[tPort0].getContent()->getDensity()
                                / tNodes[tPort0].getContent()->getMWeight();
    capacitance = inflatableCap + GunnsFluidUtils::computeCapacitance(tNodes[tPort0].getContent(),
                                                                      tNodes[tPort0].getVolume());
    prevNodeT = tNodes[tPort0].mPreviousTemperature;

    tArticle->step(tTimeStep);

    admittance = capacitance / tTimeStep;
    source     = admittance * tNodes[tPort0].getPotential() + tNodes[tPort0].mThermalCapacitance
               * (tNodes[tPort0].getContent()->getTemperature() - prevNodeT) / tTimeStep;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(DBL_EPSILON,   tArticle->mActualInflatability,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(inflatableCap, tArticle->mInflatableCapacitance, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(capacitance,   tArticle->mCapacitance,           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(admittance,    tArticle->mAdmittanceMatrix[0],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tArticle->mAdmittanceMatrix[1],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tArticle->mAdmittanceMatrix[2],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tArticle->mAdmittanceMatrix[3],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(source,        tArticle->mSourceVector[0],       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tArticle->mSourceVector[1],       DBL_EPSILON);

    /// @test    Capacitance effect when inflated.
    tArticle->setMalfInflatabilityScale(true, 100.0);
    tNodes[tPort0].initVolume(tMaxVolume);
    inflatableCap = 0.0;
    capacitance = inflatableCap + GunnsFluidUtils::computeCapacitance(tNodes[tPort0].getContent(),
                                                                      tNodes[tPort0].getVolume());
    prevNodeT = tNodes[tPort0].mPreviousTemperature;

    tArticle->step(tTimeStep);

    admittance = capacitance / tTimeStep;
    source     = admittance * tNodes[tPort0].getPotential() + tNodes[tPort0].mThermalCapacitance
               * (tNodes[tPort0].getContent()->getTemperature() - prevNodeT) / tTimeStep;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(inflatableCap, tArticle->mInflatableCapacitance, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(capacitance,   tArticle->mCapacitance,           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(admittance,    tArticle->mAdmittanceMatrix[0],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tArticle->mAdmittanceMatrix[1],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tArticle->mAdmittanceMatrix[2],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tArticle->mAdmittanceMatrix[3],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(source,        tArticle->mSourceVector[0],       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tArticle->mSourceVector[1],       DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Balloon link model updateFluid method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidBalloon::testUpdateFluid()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    updateFluid when deflating.
    tNodes[tPort0].setPotential(tNodes[tPort0].getPotential() - 1.0);
    tArticle->mPotentialVector[0] = tNodes[tPort0].getPotential();

    tArticle->updateFluid(tTimeStep, 0.0);

    double volume    = tInflatability * (tNodes[tPort0].getPotential()
                                       - tNodes[tPort1].getPotential());
    double inflation = (volume     - GunnsFluidBalloon::mMinVolume)
                     / (tMaxVolume - GunnsFluidBalloon::mMinVolume);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(volume,    tArticle->getVolume(),      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(volume,    tNodes[tPort0].getVolume(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(inflation, tArticle->getInflation(),   FLT_EPSILON);
    CPPUNIT_ASSERT(GunnsFluidBalloon::DEFLATING  == tArticle->getInflationState());

    /// @test    updateFluid when inflating.
    tNodes[tPort0].setPotential(tNodes[tPort0].getPotential() + 1.0);
    tArticle->mPotentialVector[0] = tNodes[tPort0].getPotential();

    tArticle->updateFluid(tTimeStep, 0.0);

    volume    = tInflatability * (tNodes[tPort0].getPotential()
                                - tNodes[tPort1].getPotential());
    inflation = (volume     - GunnsFluidBalloon::mMinVolume)
              / (tMaxVolume - GunnsFluidBalloon::mMinVolume);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(volume,    tArticle->getVolume(),      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(volume,    tNodes[tPort0].getVolume(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(inflation, tArticle->getInflation(),   FLT_EPSILON);
    CPPUNIT_ASSERT(GunnsFluidBalloon::INFLATING  == tArticle->getInflationState());

    /// @test    updateFluid when holding steady, partially inflated.
    tArticle->updateFluid(tTimeStep, 0.0);

    volume    = tInflatability * (tNodes[tPort0].getPotential()
                                - tNodes[tPort1].getPotential());
    inflation = (volume     - GunnsFluidBalloon::mMinVolume)
              / (tMaxVolume - GunnsFluidBalloon::mMinVolume);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(volume,    tArticle->getVolume(),      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(volume,    tNodes[tPort0].getVolume(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(inflation, tArticle->getInflation(),   FLT_EPSILON);
    CPPUNIT_ASSERT(GunnsFluidBalloon::PARTIALLY_INFLATED  == tArticle->getInflationState());

    /// @test    updateFluid with the stuck malfunction active.
    tNodes[tPort0].setPotential(tNodes[tPort0].getPotential() + 1.0);
    tArticle->mPotentialVector[0] = tNodes[tPort0].getPotential();

    tArticle->setMalfStuck(true);
    tArticle->updateFluid(tTimeStep, 0.0);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(volume,    tArticle->getVolume(),      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(volume,    tNodes[tPort0].getVolume(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(inflation, tArticle->getInflation(),   FLT_EPSILON);
    CPPUNIT_ASSERT(GunnsFluidBalloon::PARTIALLY_INFLATED  == tArticle->getInflationState());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Balloon link model volume edit.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidBalloon::testEditVolume()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    edit to a nominal volume.
    double volume = 0.5;
    tArticle->editVolume(true, volume);
    tArticle->step(tTimeStep);

    double pressure    = volume / tInflatability;
    double temperature = tNodes[tPort0].getContent()->getTemperature();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(volume,      tArticle->getVolume(),           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(volume,      tNodes[tPort0].getVolume(),      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(pressure,    tArticle->mEditPressureValue,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature, tArticle->mEditTemperatureValue, DBL_EPSILON);

    /// @test    edit to below minimum volume.
    volume = GunnsFluidBalloon::mMinVolume;
    tArticle->editVolume(true, 0.0);
    tArticle->mEditTemperaturePressureFlag = false;
    tArticle->step(tTimeStep);

    pressure    = volume / tInflatability;
    temperature = tNodes[tPort0].getContent()->getTemperature();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(volume,      tArticle->getVolume(),           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(volume,      tNodes[tPort0].getVolume(),      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(pressure,    tArticle->mEditPressureValue,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature, tArticle->mEditTemperatureValue, DBL_EPSILON);

    /// @test    edit to above maximum volume.
    volume = tMaxVolume;
    tArticle->editVolume(true, 100.0);
    tArticle->mEditTemperaturePressureFlag = false;
    tArticle->step(tTimeStep);

    pressure    = volume / tInflatability;
    temperature = tNodes[tPort0].getContent()->getTemperature();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(volume,      tArticle->getVolume(),           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(volume,      tNodes[tPort0].getVolume(),      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(pressure,    tArticle->mEditPressureValue,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature, tArticle->mEditTemperatureValue, DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Balloon link model temperature and pressure edits.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidBalloon::testEditTemperatureAndPressures()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    temperature and total pressure edit.
    double temperature = 300.0;
    double pressure    = 150.0;
    double volume      = tInflatability * pressure;

    tArticle->editTemperaturePressure(true, temperature, pressure);
    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(volume,      tArticle->getVolume(),           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(volume,      tNodes[tPort0].getVolume(),      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(volume,      tArticle->mEditVolume,           DBL_EPSILON);

    /// @test    temperature and partial pressures edit.
    double pp[N_FLUIDS] = {75.0, 25.0};
    volume              = tInflatability * (pp[0] + pp[1]);

    tArticle->mEditTemperaturePressureFlag = false;
    tArticle->editTemperaturePartialPressure(true, temperature, pp);
    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(volume,      tArticle->getVolume(),           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(volume,      tNodes[tPort0].getVolume(),      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(volume,      tArticle->mEditVolume,           DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Balloon link model edit partial pressure rate and update state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidBalloon::testEditPartialPressureRate()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    tArticle->editPartialPressureRate(FluidProperties::GUNNS_O2, true, 75.0, 1.0);
    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT(-DBL_EPSILON > tArticle->mFlowRate);
    CPPUNIT_ASSERT(0.0 < (tArticle->mInternalFluid->getMassFraction(0)
                        * tArticle->mInternalFluid->getMassFraction(1)));

    double delPressure = 1.0 * tTimeStep;
    double oldPressure = tNodes[tPort0].getPotential();
    double newPressure = oldPressure + delPressure;
    double oldVolume   = tNodes[tPort0].getVolume();
    double newVolume   = newPressure * tInflatability;
    double delVolume   = newVolume - oldVolume;
    double oldMoles    = 1000.0 * tNodes[tPort0].getContent()->getMole();   // gmol
    double RT          = 1000.0 * oldPressure * oldVolume / oldMoles;       // J/gmol
    double delMoles    = 1000.0 * delPressure * delVolume / RT;
    double mdot        = 2.0 * delMoles * tArticle->mInternalFluid->getMWeight() / tTimeStep;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(mdot, -tArticle->mFlowRate, FLT_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the computePressureCorrection method with a positively-signed correction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidBalloon::testPressureCorrectionPos()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Introduce a 1% state error in the node.
    double Perr = 1.0;
    double P0 = tNodes[0].getPotential() + Perr;
    double V0 = P0 * tInflatability;
    tNodes[0].setPotential(P0);
    tNodes[0].setVolume(V0);
    tArticle->mPotentialVector[0] = P0;
    tArticle->mInflation = (V0         - GunnsFluidBalloon::mMinVolume)
                         / (tMaxVolume - GunnsFluidBalloon::mMinVolume);

    /// @test    pressure correction applied when not disabled.
    tArticle->step(tTimeStep);

    double nodePc = tNodes[0].mPressureCorrection;
    double linkG  = tArticle->mPressureCorrectionGain;
    double linkPc = nodePc * linkG;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(-Perr,  linkPc,                        FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(linkPc, tArticle->mPressureCorrection, FLT_EPSILON);

    /// @test    pressure correction not applied when disabled.
    tArticle->mDisablePressureCorrection = true;
    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mPressureCorrection, FLT_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the computePressureCorrection method with a negatively-signed correction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidBalloon::testPressureCorrectionNeg()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Introduce a 1% state error in the node.
    double Perr = -1.0;
    double P0 = tNodes[0].getPotential() + Perr;
    double V0 = P0 * tInflatability;
    tNodes[0].setPotential(P0);
    tNodes[0].setVolume(V0);
    tArticle->mPotentialVector[0] = P0;
    tArticle->mInflation = (V0         - GunnsFluidBalloon::mMinVolume)
                         / (tMaxVolume - GunnsFluidBalloon::mMinVolume);

    /// @test    pressure correction.
    tArticle->step(tTimeStep);

    double nodePc = tNodes[0].mPressureCorrection;
    double linkG  = tArticle->mPressureCorrectionGain;
    double linkPc = nodePc * linkG;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(-Perr,  linkPc,                        FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(linkPc, tArticle->mPressureCorrection, FLT_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the testSpecificPortRules method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidBalloon::testSpecificPortRules()
{
    UT_RESULT;

    /// @test    exception thrown if Port 0 assigned to Ground.
    tPort0 = 3;
    tPort1 = 1;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);

    /// @test    exception thrown if Port 0 assigned to a liquid node.
    tPort0 = 2;
    tPort1 = 1;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);

    UT_PASS_LAST;
}
