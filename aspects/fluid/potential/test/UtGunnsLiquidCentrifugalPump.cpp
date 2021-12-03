/*
@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/

#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsLiquidCentrifugalPump.hh"

/// @details  Test identification number.
int UtGunnsLiquidCentrifugalPump::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Liquid Centrifugal Pump link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsLiquidCentrifugalPump::UtGunnsLiquidCentrifugalPump()
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
    tMaxConductivity(0.0),
    tExpansionScaleFactor(0.0),
    tReferenceDensity(0.0),
    tReferenceSpeed(0.0),
    tReferenceCoeff0(0.0),
    tReferenceCoeff1(0.0),
    tReferenceCoeff2(0.0),
    tReferenceCoeff3(0.0),
    tReferenceCoeff4(0.0),
    tReferenceCoeff5(0.0),
    tBestEfficiency(0.0),
    tReferenceQBep(0.0),
    tFilterGain(0.0),
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
    tSourcePressure(0.0),
    tMotorSpeed(0.0),
    tWallTemperature(0.0),
    tEnableAutoCavitation(false),
    tInputData(0),
    tArticle(0),
    tReferenceQ(0.0),
    tTimeStep(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Liquid Centrifugal Pump link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsLiquidCentrifugalPump::~UtGunnsLiquidCentrifugalPump()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLiquidCentrifugalPump::setUp()
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

    tNodes[0].resetFlows();
    tNodes[1].resetFlows();

    /// - Initialize the nodes list.
    tNodeList.mNodes      = tNodes;
    tNodeList.mNumNodes   = N_NODES;

    /// - Define the nominal configuration data, roughly resembling an ISS ITCS pump.
    tReferenceQ              = 0.0904418832603137;
    tName                    = "nominal";
    tMaxConductivity         = 0.0001;
    tExpansionScaleFactor    = 0.0;
    tReferenceDensity        = 1000.0;
    tReferenceSpeed          = 13000.0;
    tReferenceCoeff0         = 508.162;
    tReferenceCoeff1         = 1.17275E5;
    tReferenceCoeff2         =-5.99511E8;
    tReferenceCoeff3         =-4.91887E12;
    tReferenceCoeff4         = 2.94647E16;
    tReferenceCoeff5         =-4.5228E19;
    tBestEfficiency          = 0.0;
    tReferenceQBep           = 0.0;
    tFilterGain              = 0.5;
    tDriveRatio              = 1.0;
    tThermalLength           = 0.1;
    tThermalDiameter         = 0.1;
    tSurfaceRoughness        = 2.1336E-6;
    tCheckValveActive     = false;
    tLiquidType              = FluidProperties::GUNNS_WATER;
    tAutoCavitationDuration  = 0.3;
    tAutoCavitationAmplitude = 3.0;
    tConfigData              = new GunnsLiquidCentrifugalPumpConfigData(tName,
                                                                        &tNodeList,
                                                                        tMaxConductivity,
                                                                        tExpansionScaleFactor,
                                                                        tReferenceDensity,
                                                                        tReferenceSpeed,
                                                                        tReferenceCoeff0,
                                                                        tReferenceCoeff1,
                                                                        tReferenceCoeff2,
                                                                        tReferenceCoeff3,
                                                                        tReferenceCoeff4,
                                                                        tReferenceCoeff5,
                                                                        tBestEfficiency,
                                                                        tReferenceQBep,
                                                                        tFilterGain,
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
    tSourcePressure       = 0.0;
    tMotorSpeed           = 10000.0;
    tWallTemperature      = 300.0;
    tEnableAutoCavitation = true;
    tInputData            = new GunnsLiquidCentrifugalPumpInputData(tBlockageFlag,
                                                                    tBlockage,
                                                                    tSourcePressure,
                                                                    tMotorSpeed,
                                                                    tWallTemperature,
                                                                    tEnableAutoCavitation);

    /// - Define the nominal port mapping.
    tPort0                = 0;
    tPort1                = 1;

    /// - Default construct the nominal test article.
    tArticle              = new FriendlyGunnsLiquidCentrifugalPump;

    /// - Define the nominal time step.
    tTimeStep             = 0.1;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLiquidCentrifugalPump::tearDown()
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
/// @details  Tests the construction of GUNNS Liquid Centrifugal Pump link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLiquidCentrifugalPump::testConfig()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(tName                     == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                    == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductivity          == tConfigData->mMaxConductivity);
    CPPUNIT_ASSERT(tExpansionScaleFactor     == tConfigData->mExpansionScaleFactor);
    CPPUNIT_ASSERT(tReferenceDensity         == tConfigData->mReferenceDensity);
    CPPUNIT_ASSERT(tReferenceSpeed           == tConfigData->mReferenceSpeed);
    CPPUNIT_ASSERT(tReferenceCoeff0          == tConfigData->mReferenceCoeff0);
    CPPUNIT_ASSERT(tReferenceCoeff1          == tConfigData->mReferenceCoeff1);
    CPPUNIT_ASSERT(tReferenceCoeff2          == tConfigData->mReferenceCoeff2);
    CPPUNIT_ASSERT(tReferenceCoeff3          == tConfigData->mReferenceCoeff3);
    CPPUNIT_ASSERT(tReferenceCoeff4          == tConfigData->mReferenceCoeff4);
    CPPUNIT_ASSERT(tReferenceCoeff5          == tConfigData->mReferenceCoeff5);
    CPPUNIT_ASSERT(tBestEfficiency           == tConfigData->mBestEfficiency);
    CPPUNIT_ASSERT(tReferenceQBep            == tConfigData->mReferenceQBep);
    CPPUNIT_ASSERT(tFilterGain               == tConfigData->mFilterGain);
    CPPUNIT_ASSERT(tDriveRatio               == tConfigData->mDriveRatio);
    CPPUNIT_ASSERT(tThermalLength            == tConfigData->mThermalLength);
    CPPUNIT_ASSERT(tThermalDiameter          == tConfigData->mThermalDiameter);
    CPPUNIT_ASSERT(tCheckValveActive     == tConfigData->mCheckValveActive);
    CPPUNIT_ASSERT(tSurfaceRoughness         == tConfigData->mSurfaceRoughness);
    CPPUNIT_ASSERT(tLiquidType               == tConfigData->mLiquidType);
    CPPUNIT_ASSERT(tAutoCavitationDuration   == tConfigData->mAutoCavitationDuration);
    CPPUNIT_ASSERT(tAutoCavitationAmplitude  == tConfigData->mAutoCavitationAmplitude);

    /// @test    Configuration data default construction.
    GunnsLiquidCentrifugalPumpConfigData defaultConfig;
    CPPUNIT_ASSERT(1.0                       == defaultConfig.mDriveRatio);
    CPPUNIT_ASSERT(FluidProperties::NO_FLUID == defaultConfig.mLiquidType);
    CPPUNIT_ASSERT(0.0                       == defaultConfig.mAutoCavitationDuration);
    CPPUNIT_ASSERT(0.0                       == defaultConfig.mAutoCavitationAmplitude);

    /// @test    Configuration data copy construction.
    GunnsLiquidCentrifugalPumpConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tDriveRatio               == copyConfig.mDriveRatio);
    CPPUNIT_ASSERT(tLiquidType               == copyConfig.mLiquidType);
    CPPUNIT_ASSERT(tAutoCavitationDuration   == copyConfig.mAutoCavitationDuration);
    CPPUNIT_ASSERT(tAutoCavitationAmplitude  == copyConfig.mAutoCavitationAmplitude);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of GUNNS Liquid Centrifugal Pump link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLiquidCentrifugalPump::testInput()
{
    UT_RESULT;

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(tBlockageFlag         == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tBlockage             == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tSourcePressure       == tInputData->mSourcePressure);
    CPPUNIT_ASSERT(tMotorSpeed           == tInputData->mMotorSpeed);
    CPPUNIT_ASSERT(tWallTemperature      == tInputData->mWallTemperature);
    CPPUNIT_ASSERT(tEnableAutoCavitation == tInputData->mEnableAutoCavitation);

    /// @test    Input data default construction.
    GunnsLiquidCentrifugalPumpInputData defaultInput;
    CPPUNIT_ASSERT(0.0                   == defaultInput.mWallTemperature);
    CPPUNIT_ASSERT(0.0                   == defaultInput.mEnableAutoCavitation);

    /// @test    Input data copy construction.
    GunnsLiquidCentrifugalPumpInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tWallTemperature      == copyInput.mWallTemperature);
    CPPUNIT_ASSERT(tEnableAutoCavitation == copyInput.mEnableAutoCavitation);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Liquid Centrifugal Pump link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLiquidCentrifugalPump::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default construction of class & base class attributes.
    CPPUNIT_ASSERT(0.0                       == tArticle->mSourcePressure);
    CPPUNIT_ASSERT(0.0                       == tArticle->mReferenceDensity);
    CPPUNIT_ASSERT(FluidProperties::NO_FLUID == tArticle->mLiquidType);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsLiquidCentrifugalPump* article = new GunnsLiquidCentrifugalPump();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Liquid Centrifugal Pump link model nominal initialization.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLiquidCentrifugalPump::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    FriendlyGunnsLiquidCentrifugalPump article;
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Base class initialization.
    CPPUNIT_ASSERT(tSourcePressure == article.mSourcePressure);
    CPPUNIT_ASSERT(tName           == article.mName);
    CPPUNIT_ASSERT(tReferenceSpeed == article.mReferenceSpeed);

    /// @test    Terms initialized from configuration data.
    CPPUNIT_ASSERT(tLiquidType     == article.mLiquidType);

    /// - Note that we have no visibility into the contained GunnsPumpCavitation object to verify
    ///   its initialization.  However, it is verified by observing its effects in test #8 below,
    ///   which must correspond to the initial data we supplied in our set-up.

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(article.mInitFlag);

    // - Verify restartModel Functionality
    article.mImpellerSpeed      = 1.0;
    article.mImpellerPower      = 1.0;
    article.mAffinityCoeffs[0]  = 1.0;
    article.mAffinityCoeffs[1]  = 1.0;
    article.mAffinityCoeffs[2]  = 1.0;
    article.mAffinityCoeffs[3]  = 1.0;
    article.mAffinityCoeffs[4]  = 1.0;
    article.mAffinityCoeffs[5]  = 1.0;

    article.restartModel();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article.mImpellerSpeed,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article.mImpellerPower,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article.mAffinityCoeffs[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article.mAffinityCoeffs[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article.mAffinityCoeffs[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article.mAffinityCoeffs[3], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article.mAffinityCoeffs[4], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article.mAffinityCoeffs[5], DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Liquid Centrifugal Pump link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLiquidCentrifugalPump::testInitializationExceptions()
{
    UT_RESULT;

    /// - Default construct a test article.
    FriendlyGunnsLiquidCentrifugalPump article;

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
/// @details  Tests for GUNNS Liquid Centrifugal Pump link model getter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLiquidCentrifugalPump::testAccessors()
{
    UT_RESULT;

    /// @test    Nothing, there are no accessors (yet).

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Liquid Centrifugal Pump link setter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLiquidCentrifugalPump::testModifiers()
{
    UT_RESULT;

    /// @test    Nothing, there are no modifiers (yet).

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Liquid Centrifugal Pump link model update state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLiquidCentrifugalPump::testUpdateState()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Fan outputs with no cavitation present.
    tArticle->mDriveRatio     = tDriveRatio;
    tArticle->mVolFlowRate    = 0.0001; // (m3/s), 0.1 kg/s
    tArticle->mSourcePressure = 100.0;  // (kPa)
    tArticle->updateState(tTimeStep);
    double nominalSourcePressure = tArticle->mSourcePressure;
    CPPUNIT_ASSERT(nominalSourcePressure > 0.0);

    /// @test    Repeat the previous update, only this time the inlet pressure is just low enough
    ///          to trigger cavitation in the operating fluid.  Auto-cavitation occurs because it
    ///          was enabled and configured during initialization.  This verifies the
    ///          PumpCavitation model's effect on the source pressure and that it was initialized
    ///          properly.
    const FluidProperties* propertiesH2O = tNodes[0].getOutflow()->getProperties(FluidProperties::GUNNS_WATER);
    tNodes[0].setPotential(0.9 * propertiesH2O->getSaturationPressure(
                                                         tNodes[0].getOutflow()->getTemperature()));
    tArticle->mVolFlowRate    = 0.0001;
    tArticle->mSourcePressure = 100.0;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT(nominalSourcePressure > tArticle->mSourcePressure);

    UT_PASS_LAST;
}
