/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/fluid/source/GunnsLiquidDisplacementPump.o))
***************************************************************************************************/

#include "core/GunnsFluidUtils.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsLiquidDisplacementPump.hh"

/// @details  Test identification number.
int UtGunnsLiquidDisplacementPump::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Liquid Displacement Pump link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsLiquidDisplacementPump::UtGunnsLiquidDisplacementPump()
    :
    CppUnit::TestFixture(),
    tTypes(),
    tFractions(),
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
    tCycleVolume(0.0),
    tDriveRatio(0.0),
    tThermalLength(0.0),
    tThermalDiameter(0.0),
    tSurfaceRoughness(0.0),
    tCheckValveActive(false),
    tLiquidType(FluidProperties::NO_FLUID),
    tAutoCavitationDuration(0.0),
    tAutoCavitationAmplitude(0.0),
    tConfigData(0),
    tBlockageFlag(false),
    tBlockage(0.0),
    tFlowDemand(0.0),
    tMotorSpeed(0.0),
    tWallTemperature(0.0),
    tEnableAutoCavitation(false),
    tInputData(0),
    tArticle(0),
    tTimeStep(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Liquid Displacement Pump link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsLiquidDisplacementPump::~UtGunnsLiquidDisplacementPump()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLiquidDisplacementPump::setUp()
{
    /// - Define nominal port fluids.
    tFluidProperties      = new DefinedFluidProperties();
    tTypes[0]             = FluidProperties::GUNNS_WATER;
    tTypes[1]             = FluidProperties::GUNNS_N2;
    tFractions[0]         = 1.0;
    tFractions[1]         = 0.0;
    tFluidConfig          = new PolyFluidConfigData(tFluidProperties, tTypes, N_FLUIDS);
    tFluidInput0          = new PolyFluidInputData(283.0,
                                                   109.0,
                                                     0.0,
                                                     0.0,
                                                     tFractions);
    tFluidInput1          = new PolyFluidInputData(283.0,
                                                   111.0,
                                                     0.0,
                                                     0.0,
                                                     tFractions);

    /// - Initialize the nodes.
    tNodes[0].initialize("UtNode1", tFluidConfig);
    tNodes[1].initialize("UtNode2", tFluidConfig);
    tNodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput0);
    tNodes[1].getContent()->initialize(*tFluidConfig, *tFluidInput1);

    /// - Initialize the nodes list.
    tNodeList.mNodes      = tNodes;
    tNodeList.mNumNodes   = N_NODES;

    /// - Define the nominal configuration data, roughly resembling an ISS ITCS pump.
    tName                    = "nominal";
    tCycleVolume             = 2.3E-6;
    tDriveRatio              = 1.0;
    tThermalLength           = 0.1;
    tThermalDiameter         = 0.01;
    tSurfaceRoughness        = 2.1336E-6;
    tCheckValveActive        = false;
    tLiquidType              = FluidProperties::GUNNS_WATER;
    tAutoCavitationDuration  = 0.3;
    tAutoCavitationAmplitude = 3.0;
    tConfigData              = new GunnsLiquidDisplacementPumpConfigData(tName,
                                                                         &tNodeList,
                                                                         tCycleVolume,
                                                                         tDriveRatio,
                                                                         tThermalLength,
                                                                         tThermalDiameter,
                                                                         tSurfaceRoughness,
                                                                         tCheckValveActive,
                                                                         tLiquidType,
                                                                         tAutoCavitationDuration,
                                                                         tAutoCavitationAmplitude);

    /// - Define the nominal input data
    tBlockageFlag         = true;
    tBlockage             = 0.0;
    tFlowDemand           = 0.0;
    tMotorSpeed           = 10000.0;
    tWallTemperature      = 300.0;
    tEnableAutoCavitation = true;
    tInputData            = new GunnsLiquidDisplacementPumpInputData(tBlockageFlag,
                                                                     tBlockage,
                                                                     tFlowDemand,
                                                                     tMotorSpeed,
                                                                     tWallTemperature,
                                                                     tEnableAutoCavitation);

    /// - Define the nominal port mapping.
    tPort0                = 0;
    tPort1                = 1;

    /// - Default construct the nominal test article.
    tArticle              = new FriendlyGunnsLiquidDisplacementPump;

    /// - Define the nominal time step.
    tTimeStep             = 0.1;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLiquidDisplacementPump::tearDown()
{
    /// - Deletes in reverse order of news in setUp.
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete tFluidInput1;
    delete tFluidInput0;
    delete tFluidConfig;
    delete tFluidProperties;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the construction of GUNNS Liquid Displacement Pump link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLiquidDisplacementPump::testConfig()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(tName                     == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                    == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tCycleVolume              == tConfigData->mCycleVolume);
    CPPUNIT_ASSERT(tDriveRatio               == tConfigData->mDriveRatio);
    CPPUNIT_ASSERT(tThermalLength            == tConfigData->mThermalLength);
    CPPUNIT_ASSERT(tThermalDiameter          == tConfigData->mThermalDiameter);
    CPPUNIT_ASSERT(tSurfaceRoughness         == tConfigData->mSurfaceRoughness);
    CPPUNIT_ASSERT(tLiquidType               == tConfigData->mLiquidType);
    CPPUNIT_ASSERT(tAutoCavitationDuration   == tConfigData->mAutoCavitationDuration);
    CPPUNIT_ASSERT(tAutoCavitationAmplitude  == tConfigData->mAutoCavitationAmplitude);

    /// @test    Configuration data default construction.
    GunnsLiquidDisplacementPumpConfigData defaultConfig;
    CPPUNIT_ASSERT(1.0                       == defaultConfig.mDriveRatio);
    CPPUNIT_ASSERT(FluidProperties::NO_FLUID == defaultConfig.mLiquidType);
    CPPUNIT_ASSERT(0.0                       == defaultConfig.mAutoCavitationDuration);
    CPPUNIT_ASSERT(0.0                       == defaultConfig.mAutoCavitationAmplitude);

    /// @test    Configuration data copy construction.
    GunnsLiquidDisplacementPumpConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tDriveRatio               == copyConfig.mDriveRatio);
    CPPUNIT_ASSERT(tLiquidType               == copyConfig.mLiquidType);
    CPPUNIT_ASSERT(tAutoCavitationDuration   == copyConfig.mAutoCavitationDuration);
    CPPUNIT_ASSERT(tAutoCavitationAmplitude  == copyConfig.mAutoCavitationAmplitude);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of GUNNS Liquid Displacement Pump link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLiquidDisplacementPump::testInput()
{
    UT_RESULT;

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(tBlockageFlag         == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tBlockage             == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tFlowDemand           == tInputData->mFlowDemand);
    CPPUNIT_ASSERT(tMotorSpeed           == tInputData->mMotorSpeed);
    CPPUNIT_ASSERT(tWallTemperature      == tInputData->mWallTemperature);
    CPPUNIT_ASSERT(tEnableAutoCavitation == tInputData->mEnableAutoCavitation);

    /// @test    Input data default construction.
    GunnsLiquidDisplacementPumpInputData defaultInput;
    CPPUNIT_ASSERT(0.0                   == defaultInput.mWallTemperature);
    CPPUNIT_ASSERT(0.0                   == defaultInput.mEnableAutoCavitation);

    /// @test    Input data copy construction.
    GunnsLiquidDisplacementPumpInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tWallTemperature      == copyInput.mWallTemperature);
    CPPUNIT_ASSERT(tEnableAutoCavitation == copyInput.mEnableAutoCavitation);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Liquid Displacement Pump link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLiquidDisplacementPump::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default construction of class & base class attributes.
    CPPUNIT_ASSERT(0.0                       == tArticle->mFlowDemand);
    CPPUNIT_ASSERT(FluidProperties::NO_FLUID == tArticle->mLiquidType);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsLiquidDisplacementPump* article = new GunnsLiquidDisplacementPump();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Liquid Displacement Pump link model nominal initialization.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLiquidDisplacementPump::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    FriendlyGunnsLiquidDisplacementPump article;
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Base class initialization.
    CPPUNIT_ASSERT(tName           == article.mName);
    CPPUNIT_ASSERT(tDriveRatio     == article.mDriveRatio);

    /// @test    Terms initialized from configuration data.
    CPPUNIT_ASSERT(tLiquidType     == article.mLiquidType);

    /// - Note that we have no visibility into the contained GunnsPumpCavitation object to verify
    ///   its initialization.  However, it is verified by observing its effects in test #8 below,
    ///   which must correspond to the initial data we supplied in our set-up.

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(article.mInitFlag);

    /// @test    Verify restartModel functionality
    tArticle->mImpellerSpeed = 1.0;
    tArticle->mImpellerPower = 1.0;

    tArticle->restartModel();

    CPPUNIT_ASSERT(0.0 == tArticle->mImpellerSpeed);
    CPPUNIT_ASSERT(0.0 == tArticle->mImpellerPower);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Liquid Displacement Pump link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLiquidDisplacementPump::testInitializationExceptions()
{
    UT_RESULT;

    /// - Default construct a test article.
    FriendlyGunnsLiquidDisplacementPump article;

    /// @test    Initialization exception (from the base class) on no name.
    tConfigData->mName = "";
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mName = tName;

    /// @test    Initialization exception on fluid type not supported.
    tConfigData->mLiquidType = FluidProperties::GUNNS_CO2;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);

    /// @test    Initialization exception on fluid type not in network.
    tConfigData->mLiquidType = FluidProperties::GUNNS_HFE7000;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mLiquidType = FluidProperties::GUNNS_WATER;

    /// @test    Initialization exception on auto-cavitation duration too small.
    tConfigData->mAutoCavitationDuration = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Liquid Displacement Pump link model getter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLiquidDisplacementPump::testAccessors()
{
    UT_RESULT;

    /// @test    Nothing, there are no accessors (yet).

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Liquid Displacement Pump link setter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLiquidDisplacementPump::testModifiers()
{
    UT_RESULT;

    /// @test    Nothing, there are no modifiers (yet).

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Liquid Displacement Pump link model update state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLiquidDisplacementPump::testUpdateState()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Pump outputs with no cavitation present.
    tArticle->updateState(tTimeStep);
    double nominalFlowRate = tArticle->mVolFlowRate;
    CPPUNIT_ASSERT(nominalFlowRate > 0.0);

    /// @test    Repeat the previous update, only this time the inlet pressure is just low enough
    ///          to trigger cavitation in the operating fluid.  Auto-cavitation occurs because it
    ///          was enabled and configured during initialization.  This verifies the
    ///          PumpCavitation model's effect on the source pressure and that it was initialized
    ///          properly.
    const FluidProperties* propertiesH2O = tNodes[0].getContent()->getProperties(FluidProperties::GUNNS_WATER);
    tNodes[0].setPotential(0.9 * propertiesH2O->getSaturationPressure(
                                                         tNodes[0].getOutflow()->getTemperature()));
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT(nominalFlowRate > tArticle->mVolFlowRate);

    /// @test    Repeat the same cavitation test for negative flow direction.
    tNodes[0].setPotential(109.0);
    tNodes[1].setPotential(0.9 * propertiesH2O->getSaturationPressure(
                                                         tNodes[1].getOutflow()->getTemperature()));
    tArticle->mMotorSpeed = -tMotorSpeed;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT(-nominalFlowRate < tArticle->mVolFlowRate);

    UT_PASS_LAST;
}
