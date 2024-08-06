/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/fluid/source/GunnsGasDisplacementPump.o))
***************************************************************************************************/

#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsGasDisplacementPump.hh"

/// @details  Acts as a redundant test on UnitConversion class.
const double UtGunnsGasDisplacementPump::PI = 3.14159265358979;

/// @details  Test identification number.
int UtGunnsGasDisplacementPump::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Gas Displacement Pump link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsGasDisplacementPump::UtGunnsGasDisplacementPump()
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
    tConfigData(0),
    tBlockageFlag(false),
    tBlockage(0.0),
    tFlowDemand(0.0),
    tMotorSpeed(0.0),
    tWallTemperature(0.0),
    tInputData(0),
    tArticle(0),
    tTimeStep(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Gas Displacement Pump link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsGasDisplacementPump::~UtGunnsGasDisplacementPump()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasDisplacementPump::setUp()
{
    /// - Define nominal port fluids.
    tFluidProperties      = new DefinedFluidProperties();
    tTypes[0]             = FluidProperties::GUNNS_N2;
    tTypes[1]             = FluidProperties::GUNNS_O2;
    tFractions[0]         = 0.5;
    tFractions[1]         = 0.5;
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
    tNodes[0].resetFlows();
    tNodes[1].resetFlows();

    /// - Initialize the nodes list.
    tNodeList.mNodes      = tNodes;
    tNodeList.mNumNodes   = N_NODES;

    /// - Define the nominal configuration data.
    tName                 = "nominal";
    tCycleVolume          = 0.0001;
    tDriveRatio           = 0.5;
    tThermalLength        = 0.1;
    tThermalDiameter      = 0.1;
    tSurfaceRoughness     = 2.1336E-6;
    tCheckValveActive     = false;
    tConfigData           = new GunnsGasDisplacementPumpConfigData(tName,
                                                                   &tNodeList,
                                                                   tCycleVolume,
                                                                   tDriveRatio,
                                                                   tThermalLength,
                                                                   tThermalDiameter,
                                                                   tSurfaceRoughness,
                                                                   tCheckValveActive);

    /// - Define the nominal input data
    tBlockageFlag         = true;
    tBlockage             = 0.1;
    tFlowDemand           = 1.0;
    tMotorSpeed           = 1000.0;
    tWallTemperature      = 300.0;
    tInputData            = new GunnsGasDisplacementPumpInputData(tBlockageFlag,
                                                                  tBlockage,
                                                                  tFlowDemand,
                                                                  tMotorSpeed,
                                                                  tWallTemperature);

    /// - Define the nominal port mapping.
    tPort0                = 0;
    tPort1                = 1;

    /// - Default construct the nominal test article.
    tArticle              = new FriendlyGunnsGasDisplacementPump;

    /// - Define the nominal time step.
    tTimeStep             = 0.1;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasDisplacementPump::tearDown()
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
/// @details  Tests the construction of GUNNS Gas Displacement Pump link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasDisplacementPump::testConfig()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(tName             == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes            == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tCycleVolume      == tConfigData->mCycleVolume);
    CPPUNIT_ASSERT(tDriveRatio       == tConfigData->mDriveRatio);
    CPPUNIT_ASSERT(tThermalLength    == tConfigData->mThermalLength);
    CPPUNIT_ASSERT(tThermalDiameter  == tConfigData->mThermalDiameter);
    CPPUNIT_ASSERT(tSurfaceRoughness == tConfigData->mSurfaceRoughness);
    CPPUNIT_ASSERT(tCheckValveActive == tConfigData->mCheckValveActive);

    /// @test    Configuration data default construction.
    GunnsGasDisplacementPumpConfigData defaultConfig;
    CPPUNIT_ASSERT(0                 == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0                 == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0               == defaultConfig.mCycleVolume);
    CPPUNIT_ASSERT(1.0               == defaultConfig.mDriveRatio);
    CPPUNIT_ASSERT(0.0               == defaultConfig.mThermalLength);
    CPPUNIT_ASSERT(0.0               == defaultConfig.mThermalDiameter);
    CPPUNIT_ASSERT(false             == defaultConfig.mCheckValveActive);

    /// @test    Configuration data copy construction.
    GunnsGasDisplacementPumpConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tName             == copyConfig.mName);
    CPPUNIT_ASSERT(tNodes            == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tCycleVolume      == copyConfig.mCycleVolume);
    CPPUNIT_ASSERT(tDriveRatio       == copyConfig.mDriveRatio);
    CPPUNIT_ASSERT(tThermalLength    == copyConfig.mThermalLength);
    CPPUNIT_ASSERT(tThermalDiameter  == copyConfig.mThermalDiameter);
    CPPUNIT_ASSERT(tSurfaceRoughness == copyConfig.mSurfaceRoughness);
    CPPUNIT_ASSERT(tCheckValveActive == copyConfig.mCheckValveActive);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of GUNNS Gas Displacement Pump link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasDisplacementPump::testInput()
{
    UT_RESULT;

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(tBlockageFlag    == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tBlockage        == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tFlowDemand      == tInputData->mFlowDemand);
    CPPUNIT_ASSERT(tMotorSpeed      == tInputData->mMotorSpeed);
    CPPUNIT_ASSERT(tWallTemperature == tInputData->mWallTemperature);

    /// @test    Input data default construction.
    GunnsGasDisplacementPumpInputData defaultInput;
    CPPUNIT_ASSERT(false            == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0              == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0              == defaultInput.mFlowDemand);
    CPPUNIT_ASSERT(0.0              == defaultInput.mMotorSpeed);
    CPPUNIT_ASSERT(0.0              == defaultInput.mWallTemperature);

    /// @test    Input data copy construction.
    GunnsGasDisplacementPumpInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tBlockageFlag    == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tBlockage        == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(tFlowDemand      == copyInput.mFlowDemand);
    CPPUNIT_ASSERT(tMotorSpeed      == copyInput.mMotorSpeed);
    CPPUNIT_ASSERT(tWallTemperature == copyInput.mWallTemperature);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Gas Displacement Pump link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasDisplacementPump::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default construction of class & base class attributes.
    CPPUNIT_ASSERT(0.0 == tArticle->mCycleVolume);
    CPPUNIT_ASSERT(0.0 == tArticle->mDriveRatio);
    CPPUNIT_ASSERT(0.0 == tArticle->mThermalDiameter);
    CPPUNIT_ASSERT(0.0 == tArticle->mThermalSurfaceArea);
    CPPUNIT_ASSERT(0.0 == tArticle->mThermalROverD);
    CPPUNIT_ASSERT(0.0 == tArticle->mMotorSpeed);
    CPPUNIT_ASSERT(0.0 == tArticle->mWallTemperature);
    CPPUNIT_ASSERT(0.0 == tArticle->mWallHeatFlux);
    CPPUNIT_ASSERT(0.0 == tArticle->mImpellerTorque);
    CPPUNIT_ASSERT(0.0 == tArticle->mImpellerSpeed);
    CPPUNIT_ASSERT(0.0 == tArticle->mImpellerPower);
    CPPUNIT_ASSERT(false == tArticle->mCheckValveActive);
    CPPUNIT_ASSERT(0.0 == tArticle->mCheckValvePosition);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsGasDisplacementPump* article = new GunnsGasDisplacementPump();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Gas Displacement Pump link model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasDisplacementPump::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    FriendlyGunnsGasDisplacementPump article;
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Base class initialization.
    CPPUNIT_ASSERT(tFlowDemand  == article.mFlowDemand);

    /// @test    Terms initialized from configuration data.
    CPPUNIT_ASSERT(tName            == article.mName);
    CPPUNIT_ASSERT(&tNodes[0]       == article.mNodes[0]);
    CPPUNIT_ASSERT(&tNodes[1]       == article.mNodes[1]);
    CPPUNIT_ASSERT(tCycleVolume     == article.mCycleVolume);
    CPPUNIT_ASSERT(tDriveRatio      == article.mDriveRatio);
    CPPUNIT_ASSERT(tThermalDiameter == article.mThermalDiameter);
    CPPUNIT_ASSERT(tCheckValveActive== article.mCheckValveActive);

    const double expectedArea   = tThermalLength * PI * tThermalDiameter;
    const double expectedROverD = tSurfaceRoughness / tThermalDiameter;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedArea,   article.mThermalSurfaceArea, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedROverD, article.mThermalROverD,      DBL_EPSILON);

    /// @test    Terms initialized from input data.
    CPPUNIT_ASSERT(tMotorSpeed      == article.mMotorSpeed);
    CPPUNIT_ASSERT(tWallTemperature == article.mWallTemperature);

    /// @test    Initialized state data.
    CPPUNIT_ASSERT(0.0 == article.mWallHeatFlux);
    CPPUNIT_ASSERT(0.0 == article.mImpellerTorque);
    CPPUNIT_ASSERT(0.0 == article.mImpellerSpeed);
    CPPUNIT_ASSERT(0.0 == article.mImpellerPower);
    CPPUNIT_ASSERT(0.0 == article.mCheckValvePosition);

    /// @test    Internal fluid initialization.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getTemperature(),
                                 article.getInternalFluid()->getTemperature(), DBL_EPSILON);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(article.mInitFlag);

    /// @test    Initialization without thermal surface area.
    tConfigData->mThermalDiameter = 0.0;
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    CPPUNIT_ASSERT_EQUAL(0.0, article.mThermalROverD);

    /// @test    Verify restartModel functionality
    tArticle->mImpellerSpeed = 1.0;
    tArticle->mImpellerPower = 1.0;

    tArticle->restartModel();

    CPPUNIT_ASSERT(0.0 == tArticle->mImpellerSpeed);
    CPPUNIT_ASSERT(0.0 == tArticle->mImpellerPower);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Gas Displacement Pump link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasDisplacementPump::testInitializationExceptions()
{
    UT_RESULT;

    /// - Default construct a test article.
    FriendlyGunnsGasDisplacementPump article;

    /// @test    Initialization exception (from the base class) on no name.
    tConfigData->mName = "";
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mName = tName;

    /// @test    Initialization exception on invalid config data: cycle volume too small.
    tConfigData->mCycleVolume = DBL_EPSILON * 0.5;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mCycleVolume = tCycleVolume;

    /// @test    Initialization exception on invalid config data: drive ratio too small.
    tConfigData->mDriveRatio = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mDriveRatio = tDriveRatio;

    /// @test    Initialization exception on invalid config data: drive ratio too small.
    tConfigData->mDriveRatio = DBL_EPSILON * 0.5;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mDriveRatio = tDriveRatio;

    /// @test    Initialization exception on invalid input data: motor speed < 0.
    tInputData->mMotorSpeed = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tInputData->mMotorSpeed = tMotorSpeed;

    /// @test    Initialization exception on invalid input data: wall temperature < 0.
    tInputData->mWallTemperature = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Gas Displacement Pump link model getter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasDisplacementPump::testAccessors()
{
    UT_RESULT;

    /// @test    The getWallHeatFlux method.
    tArticle->mWallHeatFlux = 1.0;
    CPPUNIT_ASSERT( 1.0 == tArticle->getWallHeatFlux());

    /// @test    The getImpellerTorque method.
    tArticle->mImpellerTorque = -0.1;
    CPPUNIT_ASSERT(-0.1 == tArticle->getImpellerTorque());

    /// @test    The getImpellerSpeed method.
    tArticle->mImpellerSpeed = 10.0;
    CPPUNIT_ASSERT(10.0 == tArticle->getImpellerSpeed());

    /// @test    The getImpellerPower method.
    tArticle->mImpellerPower = 5.0;
    CPPUNIT_ASSERT( 5.0 == tArticle->getImpellerPower());

    /// @test    The getWallTemperature method.
    tArticle->mWallTemperature = 295.0;
    CPPUNIT_ASSERT(295.0 == tArticle->getWallTemperature());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Gas Displacement Pump link model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasDisplacementPump::testModifiers()
{
    UT_RESULT;

    /// @test    The thermal surface area setter with good value.
    tArticle->setThermalSurfaceArea(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1, tArticle->mThermalSurfaceArea, 0.0);

    /// @test    The thermal surface area setter with out of range value.
    tArticle->setThermalSurfaceArea(-0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mThermalSurfaceArea, 0.0);

    /// @test    The wall temperature setter with good value.
    tArticle->setWallTemperature(280.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(280.0, tArticle->mWallTemperature, 0.0);

    /// @test    The wall temperature setter with out of range value.
    tArticle->setWallTemperature(-0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mWallTemperature, 0.0);

    /// @test    The wall temperature setter with out of range value.
    tArticle->setCheckValveFlag(true);
    CPPUNIT_ASSERT(tArticle->mCheckValveActive);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Gas Displacement Pump link model update state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasDisplacementPump::testUpdateState()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Zero flow rate when drive ratio and impeller speed are zero.
    tArticle->mDriveRatio = 0.0;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT(0.0 == tArticle->mImpellerSpeed);
    CPPUNIT_ASSERT(0.0 == tArticle->mVolFlowRate);
    CPPUNIT_ASSERT(0.0 == tArticle->mFlowDemand);

    /// @test    Outputs under normal running condition.
    tArticle->mDriveRatio = tDriveRatio;
    tArticle->updateState(tTimeStep);

    double expectedImpellerSpeed = tMotorSpeed / tDriveRatio;
    double sourceDensity         = tNodes[0].getOutflow()->getDensity();
    double expectedVolFlowRate   = expectedImpellerSpeed * tCycleVolume / 60.0;
    double expectedFlowDemand    = expectedVolFlowRate * sourceDensity;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedImpellerSpeed, tArticle->mImpellerSpeed, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVolFlowRate,   tArticle->mVolFlowRate,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlowDemand,    tArticle->mFlowDemand,    DBL_EPSILON);

    /// @test    Outputs when check valve is enabled, and pressure differential is positive
    tArticle->mCheckValveActive = true;
    tArticle->updateState(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tArticle->mCheckValvePosition, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVolFlowRate,   tArticle->mVolFlowRate,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlowDemand,    tArticle->mFlowDemand,    DBL_EPSILON);
    tArticle->mCheckValveActive = false;

    /// @test    Outputs with negative motor speed.
    tArticle->mMotorSpeed = -tMotorSpeed;
    tArticle->updateState(tTimeStep);

    expectedImpellerSpeed = -tMotorSpeed / tDriveRatio;
    sourceDensity         = tNodes[1].getOutflow()->getDensity();
    expectedVolFlowRate   = expectedImpellerSpeed * tCycleVolume / 60.0;
    expectedFlowDemand    = expectedVolFlowRate * sourceDensity;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedImpellerSpeed, tArticle->mImpellerSpeed, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVolFlowRate,   tArticle->mVolFlowRate,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlowDemand,    tArticle->mFlowDemand,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tArticle->mCheckValvePosition, DBL_EPSILON);

    /// @test    Outputs when check valve is enabled, and pressure differential is negative
    tArticle->mCheckValveActive = true;
    tArticle->updateState(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mCheckValvePosition, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mVolFlowRate,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mFlowDemand,    DBL_EPSILON);


    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Impeller link model update fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasDisplacementPump::testUpdateFluid()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test   Update fluid with flowrate too small.
    tArticle->updateFluid(tTimeStep, 0.5 * DBL_EPSILON);
    CPPUNIT_ASSERT(0.0 == tArticle->mWallHeatFlux);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getOutflow()->getTemperature(),
                                 tArticle->mInternalFluid->getTemperature(), DBL_EPSILON);

    /// @test   Update fluid with nominal time step, flowrate and temperature.
    tArticle->updateFluid(tTimeStep, 0.01);
    CPPUNIT_ASSERT(tArticle->mWallHeatFlux < 0.0);
    CPPUNIT_ASSERT(tArticle->mInternalFluid->getTemperature() > 283.0);

    /// @test   Update fluid with negative flow rate.
    tArticle->updateFluid(tTimeStep, -0.01);
    CPPUNIT_ASSERT(tArticle->mWallHeatFlux < 0.0);
    CPPUNIT_ASSERT(tArticle->mInternalFluid->getTemperature() > 283.0);

    /// @test   Impeller power & torque at speed.
    tArticle->mVolFlowRate    = 0.06;
    tArticle->mPotentialDrop  =-0.25;
    tArticle->mMotorSpeed     = tMotorSpeed;
    /// - Delta-pressure converted from kPa to Pa to relate power in Watts.
    double expectedPower      = 1000.0 * 0.06 * 0.25;
    /// - Motor speed converted from rpm to r/s to relate torque in N*m.
    double expectedTorque     = -expectedPower / tMotorSpeed * 60.0 / 2.0 / PI;
    tArticle->updateFluid(tTimeStep, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower,  tArticle->mImpellerPower,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTorque, tArticle->mImpellerTorque, DBL_EPSILON);

    /// @test   Impeller power & torque at negative speed.
    tArticle->mVolFlowRate    =-0.06;
    tArticle->mPotentialDrop  = 0.25;
    tArticle->mMotorSpeed     =-tMotorSpeed;
    /// - Delta-pressure converted from kPa to Pa to relate power in Watts.
    expectedPower             = 1000.0 * 0.06 * 0.25;
    /// - Motor speed converted from rpm to r/s to relate torque in N*m.
    expectedTorque            = expectedPower / tMotorSpeed * 60.0 / 2.0 / PI;
    tArticle->updateFluid(tTimeStep, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower,  tArticle->mImpellerPower,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTorque, tArticle->mImpellerTorque, DBL_EPSILON);

    /// @test   Impeller torque with zero drive ratio.
    tArticle->mDriveRatio     = 0.0;
    expectedTorque            = 0.0;
    tArticle->updateFluid(tTimeStep, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTorque, tArticle->mImpellerTorque, DBL_EPSILON);

    /// @test   Impeller power & torque at zero speed.
    tArticle->mMotorSpeed     = 0.0;
    tArticle->mVolFlowRate    = 0.0;
    tArticle->mPotentialDrop  = 0.0;
    expectedPower             = 0.0;
    tArticle->updateFluid(tTimeStep, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower,  tArticle->mImpellerPower,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTorque, tArticle->mImpellerTorque, DBL_EPSILON);

    /// @test  Setting internal fluid state with correct outflow conditions

    tNodes[0].getContent()->setTemperature(200.0);
    tNodes[0].getContent()->setPressure(100.0);
    tNodes[0].resetFlows();

    tNodes[0].getContent()->setTemperature(283.0);
    tNodes[0].getContent()->setPressure(109.0);

    tArticle->mFlux = 1.0;
    tArticle->updateFluid(tTimeStep, 0.0);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(100.0, tArticle->mInternalFluid->getPressure(),DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(200.0, tArticle->mInternalFluid->getTemperature(),DBL_EPSILON);

    tNodes[1].getContent()->setTemperature(200.0);
    tNodes[1].getContent()->setPressure(100.0);
    tNodes[1].resetFlows();

    tNodes[1].getContent()->setTemperature(283.0);
    tNodes[1].getContent()->setPressure(109.0);

    tArticle->mFlux = -1.0;
    tArticle->updateFluid(tTimeStep, 0.0);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(100.0, tArticle->mInternalFluid->getPressure(),DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(200.0, tArticle->mInternalFluid->getTemperature(),DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Impeller link model compute flows.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasDisplacementPump::testComputeFlows()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    tArticle->mPotentialVector[0]  = 25.0;
    tArticle->mPotentialVector[1]  = 0.1;
    tArticle->mFlux                = 1.0;
    tArticle->computeFlows(tTimeStep);
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tArticle->mPortDirections[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::SINK   == tArticle->mPortDirections[1]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getScheduledOutflux(),tArticle->mFlux,DBL_EPSILON);
    tArticle->transportFlows(tTimeStep);

    double tempFlowRate    = tArticle->mFlux * tNodes[0].getOutflow()->getMWeight();
    double dP              = tArticle->mPotentialVector[0] - tArticle->mPotentialVector[1];
    double tempVolFlowRate = tArticle->mFlowRate / tNodes[0].getOutflow()->getDensity();
    double tempPower       = -UnitConversion::PA_PER_KPA * tempVolFlowRate * dP;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(dP,              tArticle->mPotentialDrop, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tempVolFlowRate, tArticle->mVolFlowRate,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tempPower,       tArticle->mPower,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tempFlowRate,    tNodes[0].getOutflux(),   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tempFlowRate,    tNodes[1].getInflux(),    DBL_EPSILON);

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    tArticle->mPotentialVector[1]  = 25.0;
    tArticle->mPotentialVector[0]  = 0.1;
    tArticle->mFlux                = - 1.0;
    tArticle->computeFlows(tTimeStep);
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tArticle->mPortDirections[1]);
    CPPUNIT_ASSERT(GunnsBasicLink::SINK   == tArticle->mPortDirections[0]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[1].getScheduledOutflux(),-tArticle->mFlux,DBL_EPSILON);
    tArticle->transportFlows(tTimeStep);

    tempFlowRate    = tArticle->mFlux * tNodes[1].getOutflow()->getMWeight();
    dP              = tArticle->mPotentialVector[0] - tArticle->mPotentialVector[1];
    tempVolFlowRate = tArticle->mFlowRate / tNodes[0].getOutflow()->getDensity();
    tempPower       = -UnitConversion::PA_PER_KPA * tempVolFlowRate * dP;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(dP,              tArticle->mPotentialDrop, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tempVolFlowRate, tArticle->mVolFlowRate,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tempPower,       tArticle->mPower,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-tempFlowRate,   tNodes[0].getInflux(),    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-tempFlowRate,   tNodes[1].getOutflux(),   DBL_EPSILON);

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    tArticle->mPotentialVector[1]  = 0.0;
    tArticle->mPotentialVector[0]  = 0.0;
    tArticle->mFlux                = 0.0;
    tArticle->computeFlows(tTimeStep);
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tArticle->mPortDirections[1]);
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tArticle->mPortDirections[0]);
    tArticle->transportFlows(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mPotentialDrop, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mVolFlowRate,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mPower,         DBL_EPSILON);

    UT_PASS_LAST;
}
