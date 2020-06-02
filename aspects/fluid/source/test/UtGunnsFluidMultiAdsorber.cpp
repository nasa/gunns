/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/fluid/source/GunnsFluidMultiAdsorber.o))
***************************************************************************************************/

#include "core/GunnsFluidUtils.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidMultiAdsorber.hh"

/// @details  Test identification number.
int UtGunnsFluidMultiAdsorber::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Adsorber link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidMultiAdsorber::UtGunnsFluidMultiAdsorber()
    :
    CppUnit::TestFixture(),
    tFluidProperties(0),
    tTypes(),
    tFractions0(),
    tFractions1(),
    tCompoundProperties(),
    tTcTypes(),
    tFluidTcConfig(),
    tTcFractions(),
    tFluidTcInput(),
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
    tThermalLength(0.0),
    tThermalDiameter(0.0),
    tSurfaceRoughness(0.0),
    tThermalSurfaceArea(0.0),
    tThermalROverD(0.0),
    tConfigData(0),
    tMalfBlockageFlag(false),
    tMalfBlockageValue(0.0),
    tWallTemperature(0.0),
    tCompound1Type(ChemicalCompound::NO_COMPOUND),
    tCompound1MaxAdsorbedMass(0.0),
    tCompound1EfficiencyCoeff0(0.0),
    tCompound1EfficiencyCoeff1(0.0),
    tCompound1DesorbPartialPressure(0.0),
    tCompound1DesorbRateFactor(0.0),
    tCompound1HeatOfAdsorption(0.0),
    tCompound1TaperOffFlag(false),
    tCompound1DependentType(ChemicalCompound::NO_COMPOUND),
    tCompound1MalfEfficiencyFlag(false),
    tCompound1MalfEfficiencyValue(0.0),
    tCompound1AdsorbedMass(0.0),
    tCompound2Type(ChemicalCompound::NO_COMPOUND),
    tCompound2MaxAdsorbedMass(0.0),
    tCompound2EfficiencyCoeff0(0.0),
    tCompound2EfficiencyCoeff1(0.0),
    tCompound2DesorbPartialPressure(0.0),
    tCompound2DesorbRateFactor(0.0),
    tCompound2HeatOfAdsorption(0.0),
    tCompound2TaperOffFlag(false),
    tCompound2DependentType(ChemicalCompound::NO_COMPOUND),
    tCompound2MalfEfficiencyFlag(false),
    tCompound2MalfEfficiencyValue(0.0),
    tCompound2AdsorbedMass(0.0),
    tInputData(0),
    tFluidTemperature(0.0),
    tWallHeatFlux(0.0),
    tMass(0.0),
    tSorbtionFlowRate(0.0),
    tArticle(0),
    tFlowRate(0.0),
    tTimeStep(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Adsorber link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidMultiAdsorber::~UtGunnsFluidMultiAdsorber()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////

void UtGunnsFluidMultiAdsorber::setUp()
{
    /// - Define the nominal port fluids.
    tFluidProperties      = new DefinedFluidProperties();
    tTypes[0]             = FluidProperties::GUNNS_CO2;
    tTypes[1]             = FluidProperties::GUNNS_O2;
    tTypes[2]             = FluidProperties::GUNNS_WATER;

    tCompoundProperties   = new DefinedChemicalCompounds();
    tTcTypes[0]           = ChemicalCompound::CH4O;
    tTcTypes[1]           = ChemicalCompound::C2H6O;
    tTcTypes[2]           = ChemicalCompound::C4H10O;
    tFluidTcConfig        = new GunnsFluidTraceCompoundsConfigData(tTcTypes, N_TC, "tFluidTcConfig");

    tFluidConfig          = new PolyFluidConfigData(tFluidProperties, tTypes, N_FLUIDS, tFluidTcConfig);

    tFractions0[0]        = 0.1;
    tFractions0[1]        = 0.9;
    tFractions0[2]        = 0.0;
    tTcFractions[0]       = 1.0e-5;
    tTcFractions[1]       = 2.0e-6;
    tTcFractions[2]       = 3.0e-7;
    tFluidTcInput         = new GunnsFluidTraceCompoundsInputData(tTcFractions);

    tFluidInput0          = new PolyFluidInputData(283.0,
                                                   111.0,
                                                     0.0,
                                                     1.0,
                                                     tFractions0,
                                                     tFluidTcInput);
    tFractions1[0]        = 0.0;
    tFractions1[1]        = 0.0;
    tFractions1[2]        = 1.0;
    tFluidInput1          = new PolyFluidInputData(283.0,
                                                   109.0,
                                                     0.0,
                                                     0.0,
                                                     tFractions1,
                                                     0);

    /// - Initialize the nodes.
    tNodes[0].initialize("UtNode1", tFluidConfig, tFluidInput0);
    tNodes[1].initialize("UtNode2", tFluidConfig, tFluidInput1);
    tNodes[2].initialize("UtNode3", tFluidConfig, tFluidInput0);

    tNodes[0].resetFlows();
    tNodes[1].resetFlows();
    tNodes[2].resetFlows();

    /// - Initialize the nodes list.
    tNodeList.mNodes      = tNodes;
    tNodeList.mNumNodes   = N_NODES;

    /// - Define the nominal port mapping.
    tPort0                = 0;
    tPort1                = 2;

    /// - Define the nominal configuration data.
    tName                 = "nominal";
    tMaxConductivity      =  1.5;
    tExpansionScaleFactor =  0.5;
    tThermalLength        = 1.0;
    tThermalDiameter      = 1.0;
    tSurfaceRoughness     = 1.0e-06;
    tThermalSurfaceArea   = UnitConversion::PI_UTIL * tThermalDiameter * tThermalLength;
    tThermalROverD        = tSurfaceRoughness / tThermalDiameter;
    tConfigData           = new GunnsFluidMultiAdsorberConfigData(tName,
                                                                  &tNodeList,
                                                                  tMaxConductivity,
                                                                  tExpansionScaleFactor,
                                                                  tThermalLength,
                                                                  tThermalDiameter,
                                                                  tSurfaceRoughness);

    tCompound1Type                  = ChemicalCompound::CO2;
    tCompound1MaxAdsorbedMass       = 1.0;
    tCompound1EfficiencyCoeff0      = 0.5;
    tCompound1EfficiencyCoeff1      = 0.001;
    tCompound1DesorbPartialPressure = 0.05;
    tCompound1DesorbRateFactor      = 0.1;
    tCompound1HeatOfAdsorption      = -100.0;
    tCompound1TaperOffFlag          = true;
    tCompound1DependentType         = ChemicalCompound::CH4O;
    tCompound1MalfEfficiencyFlag    = true;
    tCompound1MalfEfficiencyValue   = 0.6;
    tCompound1AdsorbedMass          = 0.4;

    tConfigData->addCompound(tCompound1Type,
                             tCompound1MaxAdsorbedMass,
                             tCompound1EfficiencyCoeff0,
                             tCompound1EfficiencyCoeff1,
                             tCompound1DesorbPartialPressure,
                             tCompound1DesorbRateFactor,
                             tCompound1HeatOfAdsorption,
                             tCompound1TaperOffFlag,
                             tCompound1DependentType,
                             tCompound1MalfEfficiencyFlag,
                             tCompound1MalfEfficiencyValue,
                             tCompound1AdsorbedMass);

    tCompound2Type                  = ChemicalCompound::CH4O;
    tCompound2MaxAdsorbedMass       = 0.0001;
    tCompound2EfficiencyCoeff0      = 0.9;
    tCompound2AdsorbedMass          = 0.5 * tCompound2MaxAdsorbedMass;
    tCompound2HeatOfAdsorption      = -100.0;

    tConfigData->addCompound(tCompound2Type,
                             tCompound2MaxAdsorbedMass,
                             tCompound2EfficiencyCoeff0);

    /// - Define the nominal input data.
    tMalfBlockageFlag     = false;
    tMalfBlockageValue    = 0.3;
    tWallTemperature      = 290.0;
    tInputData            = new GunnsFluidMultiAdsorberInputData(tMalfBlockageFlag,
                                                                 tMalfBlockageValue,
                                                                 tWallTemperature);

    /// - Default construct the nominal test article.
    tFluidTemperature     = 0.5 * (tWallTemperature + tFluidInput0->mTemperature);
    tWallHeatFlux         = 0.0;
    tMass                 = 0.0;
    tSorbtionFlowRate     = 0.0;
    tArticle              = new FriendlyGunnsFluidMultiAdsorber;

    /// - Define the nominal flow rate and time step.
    tFlowRate             = 0.1;
    tTimeStep             = 1.0;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMultiAdsorber::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete tFluidInput1;
    delete tFluidInput0;
    delete tFluidTcInput;
    delete tFluidConfig;
    delete tFluidTcConfig;
    delete tCompoundProperties;
    delete tFluidProperties;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Adsorber link model construction of configuration and input
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMultiAdsorber::testConfigAndInput()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(tName                           == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                          == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductivity                == tConfigData->mMaxConductivity);
    CPPUNIT_ASSERT(tExpansionScaleFactor           == tConfigData->mExpansionScaleFactor);
    CPPUNIT_ASSERT(tThermalLength                  == tConfigData->mThermalLength);
    CPPUNIT_ASSERT(tThermalDiameter                == tConfigData->mThermalDiameter);
    CPPUNIT_ASSERT(tSurfaceRoughness               == tConfigData->mSurfaceRoughness);
    CPPUNIT_ASSERT(2                               == tConfigData->mCompounds.size());
    CPPUNIT_ASSERT(tCompound1Type                  == tConfigData->mCompounds[0].mType);
    CPPUNIT_ASSERT(tCompound1MaxAdsorbedMass       == tConfigData->mCompounds[0].mMaxAdsorbedMass);
    CPPUNIT_ASSERT(tCompound1EfficiencyCoeff0      == tConfigData->mCompounds[0].mEfficiencyCoeff0);
    CPPUNIT_ASSERT(tCompound1EfficiencyCoeff1      == tConfigData->mCompounds[0].mEfficiencyCoeff1);
    CPPUNIT_ASSERT(tCompound1DesorbPartialPressure == tConfigData->mCompounds[0].mDesorbPartialPressure);
    CPPUNIT_ASSERT(tCompound1DesorbRateFactor      == tConfigData->mCompounds[0].mDesorbRateFactor);
    CPPUNIT_ASSERT(tCompound1HeatOfAdsorption      == tConfigData->mCompounds[0].mHeatOfAdsorption);
    CPPUNIT_ASSERT(tCompound1TaperOffFlag          == tConfigData->mCompounds[0].mTaperOffFlag);
    CPPUNIT_ASSERT(tCompound1DependentType         == tConfigData->mCompounds[0].mDependentType);
    CPPUNIT_ASSERT(tCompound1MalfEfficiencyFlag    == tConfigData->mCompounds[0].mMalfEfficiencyFlag);
    CPPUNIT_ASSERT(tCompound1MalfEfficiencyValue   == tConfigData->mCompounds[0].mMalfEfficiencyValue);
    CPPUNIT_ASSERT(tCompound1AdsorbedMass          == tConfigData->mCompounds[0].mAdsorbedMass);
    CPPUNIT_ASSERT(tCompound2Type                  == tConfigData->mCompounds[1].mType);
    CPPUNIT_ASSERT(tCompound2MaxAdsorbedMass       == tConfigData->mCompounds[1].mMaxAdsorbedMass);
    CPPUNIT_ASSERT(tCompound2EfficiencyCoeff0      == tConfigData->mCompounds[1].mEfficiencyCoeff0);
    CPPUNIT_ASSERT(0.0                             == tConfigData->mCompounds[1].mEfficiencyCoeff1);
    CPPUNIT_ASSERT(0.0                             == tConfigData->mCompounds[1].mDesorbPartialPressure);
    CPPUNIT_ASSERT(0.0                             == tConfigData->mCompounds[1].mDesorbRateFactor);
    CPPUNIT_ASSERT(0.0                             == tConfigData->mCompounds[1].mHeatOfAdsorption);
    CPPUNIT_ASSERT(false                           == tConfigData->mCompounds[1].mTaperOffFlag);
    CPPUNIT_ASSERT(ChemicalCompound::NO_COMPOUND   == tConfigData->mCompounds[1].mDependentType);
    CPPUNIT_ASSERT(false                           == tConfigData->mCompounds[1].mMalfEfficiencyFlag);
    CPPUNIT_ASSERT(0.0                             == tConfigData->mCompounds[1].mMalfEfficiencyValue);
    CPPUNIT_ASSERT(0.0                             == tConfigData->mCompounds[1].mAdsorbedMass);

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(tMalfBlockageFlag               == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue              == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tWallTemperature                == tInputData->mWallTemperature);

    /// @test    Configuration data default construction.
    GunnsFluidMultiAdsorberConfigData defaultConfig;
    CPPUNIT_ASSERT(0                               == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0                               == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0                             == defaultConfig.mMaxConductivity);
    CPPUNIT_ASSERT(0.0                             == defaultConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(0.0                             == defaultConfig.mThermalLength);
    CPPUNIT_ASSERT(0.0                             == defaultConfig.mThermalDiameter);
    CPPUNIT_ASSERT(0.0                             == defaultConfig.mSurfaceRoughness);
    CPPUNIT_ASSERT(true                            == defaultConfig.mCompounds.empty());

    /// @test    Input data default construction.
    GunnsFluidMultiAdsorberInputData   defaultInput;
    CPPUNIT_ASSERT(false                           == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                             == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0                             == defaultInput.mWallTemperature);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Adsorber link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMultiAdsorber::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default construction data.
    CPPUNIT_ASSERT(""  == tArticle->mName);
    CPPUNIT_ASSERT(0   == tArticle->mNodes);
    CPPUNIT_ASSERT(0.0 == tArticle->mMaxConductivity);
    CPPUNIT_ASSERT(0.0 == tArticle->mExpansionScaleFactor);
    CPPUNIT_ASSERT(0   == tArticle->mCompounds);
    CPPUNIT_ASSERT(0   == tArticle->mNCompounds);
    CPPUNIT_ASSERT(0.0 == tArticle->mThermalDiameter);
    CPPUNIT_ASSERT(0.0 == tArticle->mThermalSurfaceArea);
    CPPUNIT_ASSERT(0.0 == tArticle->mThermalROverD);
    CPPUNIT_ASSERT(0.0 == tArticle->mWallTemperature);
    CPPUNIT_ASSERT(0.0 == tArticle->mWallHeatFlux);
    CPPUNIT_ASSERT(0.0 == tArticle->mSorptionHeat);
    CPPUNIT_ASSERT(0   == tArticle->mSorptionFluid);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsFluidMultiAdsorber* article = new GunnsFluidMultiAdsorber();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Adsorber link model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMultiAdsorber::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Nominal configuration data.
    CPPUNIT_ASSERT(tName                           == tArticle->mName);
    CPPUNIT_ASSERT(&tNodes[tPort0]                 == tArticle->mNodes[0]);
    CPPUNIT_ASSERT(&tNodes[tPort1]                 == tArticle->mNodes[1]);
    CPPUNIT_ASSERT(tMaxConductivity                == tArticle->mMaxConductivity);
    CPPUNIT_ASSERT(tExpansionScaleFactor           == tArticle->mExpansionScaleFactor);
    CPPUNIT_ASSERT(tThermalDiameter                == tArticle->mThermalDiameter);
    CPPUNIT_ASSERT(tThermalSurfaceArea             == tArticle->mThermalSurfaceArea);
    CPPUNIT_ASSERT(tThermalROverD                  == tArticle->mThermalROverD);
    CPPUNIT_ASSERT(2                               == tArticle->mNCompounds);
    CPPUNIT_ASSERT(tCompound1Type                  == tArticle->mCompounds[0].mType);
    CPPUNIT_ASSERT(tCompound1MaxAdsorbedMass       == tArticle->mCompounds[0].mMaxAdsorbedMass);
    CPPUNIT_ASSERT(tCompound1EfficiencyCoeff0      == tArticle->mCompounds[0].mEfficiencyCoeff0);
    CPPUNIT_ASSERT(tCompound1EfficiencyCoeff1      == tArticle->mCompounds[0].mEfficiencyCoeff1);
    CPPUNIT_ASSERT(tCompound1DesorbPartialPressure == tArticle->mCompounds[0].mDesorbPartialPressure);
    CPPUNIT_ASSERT(tCompound1DesorbRateFactor      == tArticle->mCompounds[0].mDesorbRateFactor);
    CPPUNIT_ASSERT(tCompound1HeatOfAdsorption      == tArticle->mCompounds[0].mHeatOfAdsorption);
    CPPUNIT_ASSERT(tCompound1TaperOffFlag          == tArticle->mCompounds[0].mTaperOffFlag);
    CPPUNIT_ASSERT(tCompound1DependentType         == tArticle->mCompounds[0].mDependentType);
    CPPUNIT_ASSERT(tCompound1MalfEfficiencyFlag    == tArticle->mCompounds[0].mMalfEfficiencyFlag);
    CPPUNIT_ASSERT(tCompound1MalfEfficiencyValue   == tArticle->mCompounds[0].mMalfEfficiencyValue);
    CPPUNIT_ASSERT(tCompound1AdsorbedMass          == tArticle->mCompounds[0].mAdsorbedMass);
    CPPUNIT_ASSERT(tCompound2Type                  == tArticle->mCompounds[1].mType);
    CPPUNIT_ASSERT(tCompound2MaxAdsorbedMass       == tArticle->mCompounds[1].mMaxAdsorbedMass);
    CPPUNIT_ASSERT(tCompound2EfficiencyCoeff0      == tArticle->mCompounds[1].mEfficiencyCoeff0);
    CPPUNIT_ASSERT(0.0                             == tArticle->mCompounds[1].mEfficiencyCoeff1);
    CPPUNIT_ASSERT(0.0                             == tArticle->mCompounds[1].mDesorbPartialPressure);
    CPPUNIT_ASSERT(0.0                             == tArticle->mCompounds[1].mDesorbRateFactor);
    CPPUNIT_ASSERT(0.0                             == tArticle->mCompounds[1].mHeatOfAdsorption);
    CPPUNIT_ASSERT(false                           == tArticle->mCompounds[1].mTaperOffFlag);
    CPPUNIT_ASSERT(ChemicalCompound::NO_COMPOUND   == tArticle->mCompounds[1].mDependentType);
    CPPUNIT_ASSERT(false                           == tArticle->mCompounds[1].mMalfEfficiencyFlag);
    CPPUNIT_ASSERT(0.0                             == tArticle->mCompounds[1].mMalfEfficiencyValue);
    CPPUNIT_ASSERT(0.0                             == tArticle->mCompounds[1].mAdsorbedMass);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT(tMalfBlockageFlag               == tArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue              == tArticle->mMalfBlockageValue);
    CPPUNIT_ASSERT(tWallTemperature                == tArticle->mWallTemperature);

    /// @test    Nominal state data.
    CPPUNIT_ASSERT(0.0                             == tArticle->mWallHeatFlux);
    CPPUNIT_ASSERT(0.0                             == tArticle->mSorptionHeat);
    CPPUNIT_ASSERT(0                               != tArticle->mSorptionFluid);
    CPPUNIT_ASSERT(0                               != tArticle->mInternalFluid);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(true                            == tArticle->mInitFlag);

    /// @test    Nominal initialization with thermal convection terms zeroed.
    tConfigData->mThermalLength    = 0.0;
    tConfigData->mThermalDiameter  = 0.0;
    tConfigData->mSurfaceRoughness = 0.0;
    FriendlyGunnsFluidMultiAdsorber article;
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    CPPUNIT_ASSERT(0.0                             == article.mThermalROverD);
    CPPUNIT_ASSERT(true                            == article.mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Adsorber link model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMultiAdsorber::testAccessors()
{
    UT_RESULT;

//  nothing to test

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Adsorber link model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMultiAdsorber::testModifiers()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    {
        /// @test    The thermal surface area setter with good value.
        const double surfaceArea = 0.1;
        tArticle->setThermalSurfaceArea(surfaceArea);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(surfaceArea,     tArticle->mThermalSurfaceArea, 0.0);
    } {
        /// @test    The thermal surface area setter with out of range value.
        const double surfaceArea = -0.1;
        tArticle->setThermalSurfaceArea(surfaceArea);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,             tArticle->mThermalSurfaceArea, 0.0);
    } {
        /// @test    The wall temperature setter with good value.
        const double wallTemperature = 280.0;
        tArticle->setWallTemperature(wallTemperature);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(wallTemperature, tArticle->mWallTemperature,    0.0);
    } {
        /// @test    The wall temperature setter with out of range value.
        const double wallTemperature = -0.1;
        tArticle->setWallTemperature(wallTemperature);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,             tArticle->mWallTemperature,    0.0);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Adsorber link model update fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMultiAdsorber::testUpdateFluidNominal()
{
    UT_RESULT;

    /// @test     Nominal initialization data.
    tConfigData->mCompounds[1].mAdsorbedMass     = tCompound2AdsorbedMass;
    tConfigData->mCompounds[1].mHeatOfAdsorption = tCompound2HeatOfAdsorption;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    tArticle->mFlowRate           = tFlowRate;
    tArticle->mFlux               = tFlowRate / tNodes[tPort0].getOutflow()->getMWeight();
    tArticle->mInternalFluid->setState(tNodes[tPort0].getOutflow());
    tArticle->mPotentialVector[0] = 101.325;
    tArticle->mPotentialVector[1] = 100.0;
    //tArticle->mCompounds[1].mDesorbPartialPressure = 0.05;
    tArticle->updateFluid(tTimeStep, 0.0);

    /// @test    Both compounds sorbed.
    CPPUNIT_ASSERT(0.0 < tArticle->mCompounds[0].mAdsorptionRate);
    CPPUNIT_ASSERT(0.0 < tArticle->mCompounds[1].mAdsorptionRate);
    CPPUNIT_ASSERT(0.0 < tArticle->mCompounds[0].mSorptionHeat);
    CPPUNIT_ASSERT(0.0 < tArticle->mCompounds[1].mSorptionHeat);

    /// @test    Wall heat flux includes both compounds sorption heat.
    const double expectedConvection = GunnsFluidUtils::computeConvectiveHeatFlux(
                                                               tNodes[tPort0].getOutflow(),
                                                               tFlowRate,
                                                               tThermalROverD,
                                                               tThermalDiameter,
                                                               tThermalSurfaceArea,
                                                               tWallTemperature);
    const double expectedSorbQ = tArticle->mCompounds[0].mSorptionHeat
                               + tArticle->mCompounds[1].mSorptionHeat;
    const double expectedWallQ = expectedConvection + expectedSorbQ;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSorbQ, tArticle->mSorptionHeat, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedWallQ, tArticle->mWallHeatFlux, DBL_EPSILON);

    /// @test    Sorption fluid temperature & mixture.
    CPPUNIT_ASSERT(tFluidInput0->mTemperature < tArticle->mInternalFluid->getTemperature());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mInternalFluid->getTemperature(),
                                 tArticle->mSorptionFluid->getTemperature(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluidProperties->mMWeightCO2,
                                 tArticle->mSorptionFluid->getMWeight(),     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tArticle->mSorptionFluid->getMassFraction(0), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tArticle->mSorptionFluid->getMoleFraction(0), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSorptionFluid->getMassFraction(1), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSorptionFluid->getMoleFraction(1), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSorptionFluid->getMassFraction(2), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSorptionFluid->getMoleFraction(2), DBL_EPSILON);

    /// @test    Only sorbed fluid types appear in exit node inputs and source vector.
    const double expectedW = -tArticle->mCompounds[0].mAdsorptionRate / tFluidProperties->mMWeightCO2;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,       tArticle->mSourceVector[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, tArticle->mSourceVector[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-tArticle->mCompounds[0].mAdsorptionRate,
                                 tNodes[tPort1].getInflux(), DBL_EPSILON);

    /// @test    Internal fluid TC masses are the same as the inlet node minus adsorbed compounds.
    double expectedTcMass[N_TC] = {tNodes[tPort0].getOutflow()->getTraceCompounds()->getMasses()[0]
                                   - tArticle->mCompounds[1].mAdsorptionRate,
                                   tNodes[tPort0].getOutflow()->getTraceCompounds()->getMasses()[1],
                                   tNodes[tPort0].getOutflow()->getTraceCompounds()->getMasses()[2]};
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTcMass[0],
                                 tArticle->mInternalFluid->getTraceCompounds()->getMasses()[0],
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTcMass[1],
                                 tArticle->mInternalFluid->getTraceCompounds()->getMasses()[1],
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTcMass[2],
                                 tArticle->mInternalFluid->getTraceCompounds()->getMasses()[2],
                                 DBL_EPSILON);

    /// @test    Internal fluid main fluid mixture is unchanged from the inlet node.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[tPort0].getOutflow()->getMassFraction(0),
                                 tArticle->mInternalFluid->getMassFraction(0), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[tPort0].getOutflow()->getMassFraction(1),
                                 tArticle->mInternalFluid->getMassFraction(1), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[tPort0].getOutflow()->getMassFraction(2),
                                 tArticle->mInternalFluid->getMassFraction(2), DBL_EPSILON);

    /// @test    Zero sorbtion and flows at zero bulk flow rate.
    tArticle->mFlowRate           = 0.0;
    tArticle->mFlux               = 0.0;
    tArticle->mPotentialVector[1] = tArticle->mPotentialVector[0];

    tNodes[tPort0].resetFlows();
    tNodes[tPort1].resetFlows();

    tArticle->computeFlows(tTimeStep);
    tArticle->transportFlows(tTimeStep);


    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mFlowRate,                     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mFlux,                         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mCompounds[0].mAdsorptionRate, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mCompounds[1].mAdsorptionRate, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSorptionHeat,                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mWallHeatFlux,                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSourceVector[0],              DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSourceVector[1],              DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[tPort0].getInflux(),              DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[tPort1].getInflux(),              DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[tPort0].getOutflux(),             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[tPort1].getOutflux(),             DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Adsorber link model update fluid with reverse flow.  This is the
///           same test as testUpdateFluidNominal, just with the flow direction reversed.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMultiAdsorber::testUpdateFluidReverse()
{
    UT_RESULT;

    /// @test     Nominal initialization data.
    tConfigData->mCompounds[1].mAdsorbedMass     = tCompound2AdsorbedMass;
    tConfigData->mCompounds[1].mHeatOfAdsorption = tCompound2HeatOfAdsorption;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    tArticle->mFlowRate           = -tFlowRate;
    tArticle->mFlux               = -tFlowRate / tNodes[tPort1].getOutflow()->getMWeight();
    tArticle->mInternalFluid->setState(tNodes[tPort0].getOutflow());
    tArticle->mPotentialVector[1] = 101.325;
    tArticle->mPotentialVector[0] = 100.0;

    tArticle->updateFluid(tTimeStep, 0.0);

    /// @test    Both compounds sorbed.
    CPPUNIT_ASSERT(0.0 < tArticle->mCompounds[0].mAdsorptionRate);
    CPPUNIT_ASSERT(0.0 < tArticle->mCompounds[1].mAdsorptionRate);
    CPPUNIT_ASSERT(0.0 < tArticle->mCompounds[0].mSorptionHeat);
    CPPUNIT_ASSERT(0.0 < tArticle->mCompounds[1].mSorptionHeat);

    /// @test    Wall heat flux includes both compounds sorption heat.
    const double expectedConvection = GunnsFluidUtils::computeConvectiveHeatFlux(
                                                               tNodes[tPort1].getOutflow(),
                                                               tFlowRate,
                                                               tThermalROverD,
                                                               tThermalDiameter,
                                                               tThermalSurfaceArea,
                                                               tWallTemperature);
    const double expectedSorbQ = tArticle->mCompounds[0].mSorptionHeat
                               + tArticle->mCompounds[1].mSorptionHeat;
    const double expectedWallQ = expectedConvection + expectedSorbQ;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSorbQ, tArticle->mSorptionHeat, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedWallQ, tArticle->mWallHeatFlux, DBL_EPSILON);

    /// @test    Sorption fluid temperature & mixture.
    CPPUNIT_ASSERT(tFluidInput0->mTemperature < tArticle->mInternalFluid->getTemperature());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mInternalFluid->getTemperature(),
                                 tArticle->mSorptionFluid->getTemperature(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluidProperties->mMWeightCO2,
                                 tArticle->mSorptionFluid->getMWeight(),     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tArticle->mSorptionFluid->getMassFraction(0), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tArticle->mSorptionFluid->getMoleFraction(0), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSorptionFluid->getMassFraction(1), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSorptionFluid->getMoleFraction(1), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSorptionFluid->getMassFraction(2), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSorptionFluid->getMoleFraction(2), DBL_EPSILON);

    /// @test    Only sorbed fluid types appear in exit node inputs and source vector.
    const double expectedW = -tArticle->mCompounds[0].mAdsorptionRate / tFluidProperties->mMWeightCO2;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,       tArticle->mSourceVector[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, tArticle->mSourceVector[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-tArticle->mCompounds[0].mAdsorptionRate,
                                 tNodes[tPort0].getInflux(), DBL_EPSILON);

    /// @test    Internal fluid TC masses are the same as the inlet node minus adsorbed compounds.
    double expectedTcMass[N_TC] = {tNodes[tPort1].getOutflow()->getTraceCompounds()->getMasses()[0]
                                   - tArticle->mCompounds[1].mAdsorptionRate,
                                   tNodes[tPort1].getOutflow()->getTraceCompounds()->getMasses()[1],
                                   tNodes[tPort1].getOutflow()->getTraceCompounds()->getMasses()[2]};
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTcMass[0],
                                 tArticle->mInternalFluid->getTraceCompounds()->getMasses()[0],
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTcMass[1],
                                 tArticle->mInternalFluid->getTraceCompounds()->getMasses()[1],
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTcMass[2],
                                 tArticle->mInternalFluid->getTraceCompounds()->getMasses()[2],
                                 DBL_EPSILON);

    /// @test    Internal fluid main fluid mixture is unchanged from the inlet node.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[tPort1].getOutflow()->getMassFraction(0),
                                 tArticle->mInternalFluid->getMassFraction(0), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[tPort1].getOutflow()->getMassFraction(1),
                                 tArticle->mInternalFluid->getMassFraction(1), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[tPort1].getOutflow()->getMassFraction(2),
                                 tArticle->mInternalFluid->getMassFraction(2), DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Adsorber link model specific port mapping rules.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMultiAdsorber::testPortMapping()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    {
        /// @test    Fail if port 1 is a liquid node.
        const bool returned = tArticle->checkSpecificPortRules(1,1);
        CPPUNIT_ASSERT(!returned);
    } {
        /// @test    Fail if port 0 is a liquid node.
        const bool returned = tArticle->checkSpecificPortRules(0,1);
        CPPUNIT_ASSERT(!returned);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Adsorber link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMultiAdsorber::testInitializationExceptions()
{
    UT_RESULT;

    /// - Default construct a test article.
    GunnsFluidMultiAdsorber article;

    /// @test    Initialization exception on invalid config data: no name.
    tConfigData->mName = "";
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mName = tName;

    /// @test    Initialization exception on invalid config data: max conductivity < 0.
    tConfigData->mMaxConductivity = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mMaxConductivity = tMaxConductivity;

    /// @test    Initialization exception on invalid config data: empty compounds vector.
    GunnsFluidMultiAdsorberConfigData config(tName, &tNodeList, tMaxConductivity,
                                             tExpansionScaleFactor, tThermalLength,
                                             tThermalDiameter, tSurfaceRoughness);
    CPPUNIT_ASSERT_THROW(article.initialize(config, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);

    /// @test    Initialization exception on invalid config data: duplicated compounds.
    config.addCompound(tCompound1Type, tCompound1MaxAdsorbedMass, tCompound1EfficiencyCoeff0);
    config.addCompound(tCompound1Type, tCompound1MaxAdsorbedMass, tCompound1EfficiencyCoeff0);
    CPPUNIT_ASSERT_THROW(article.initialize(config, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    
    /// @test    Initialization exception on invalid config data: exception from compound init.
    config.mCompounds.clear();
    config.addCompound(tCompound1Type, -DBL_EPSILON, tCompound1EfficiencyCoeff0);
    CPPUNIT_ASSERT_THROW(article.initialize(config, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);

    /// @test    Initialization exception on invalid input data: wall temperature < 0.
    tInputData->mWallTemperature = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tInputData->mWallTemperature = tWallTemperature;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Adsorber link model restart.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMultiAdsorber::testRestart()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    tArticle->mSorptionHeat = 1.0;
    tArticle->restart();

    CPPUNIT_ASSERT(0.0 == tArticle->mSorptionHeat);

    UT_PASS_LAST;
}
