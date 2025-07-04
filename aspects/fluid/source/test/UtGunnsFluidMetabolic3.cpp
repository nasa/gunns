/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2025 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/fluid/source/GunnsFluidMetabolic3.o))
***************************************************************************************************/

#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidMetabolic3.hh"

/// @details  Test identification number.
int UtGunnsFluidMetabolic3::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructor for the UtGunnsFluidMetabolic3 test class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidMetabolic3::UtGunnsFluidMetabolic3()
    :
    tFluidProperties(),
    tTypes(),
    tCompoundProperties(),
    tTcTypes(),
    tFluidTcConfig(),
    tFractions(),
    tFluidConfig(),
    tFluidInput(),
    tLinks(),
    tName(),
    tNodes(),
    tNodeList(),
    tConfigData(),
    tNNominal(),
    tNSleep(),
    tNRecovery0(),
    tNRecovery1(),
    tNRecovery2(),
    tNRecovery3(),
    tNAerobic0(),
    tNAerobic1(),
    tNResistive0(),
    tNResistive1(),
    tNResistive2(),
    tNResistive3(),
    tInputData(),
    tArticle()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructor for the UtGunnsFluidMetabolic3 test class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidMetabolic3::~UtGunnsFluidMetabolic3()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMetabolic3::setUp()
{
    /// - Set up the nodes list
    tNodeList.mNumNodes = 3;
    tNodeList.mNodes    = tNodes;

    /// - Define the nominal atmosphere
    tFluidProperties = new DefinedFluidProperties();
    tTypes[0]        = FluidProperties::GUNNS_N2;  tFractions[0] = 0.750;
    tTypes[1]        = FluidProperties::GUNNS_O2;  tFractions[1] = 0.200;
    tTypes[2]        = FluidProperties::GUNNS_H2O; tFractions[2] = 0.035;
    tTypes[3]        = FluidProperties::GUNNS_CO2; tFractions[3] = 0.015;

    tCompoundProperties = new DefinedChemicalCompounds();
    tTcTypes[0]      = ChemicalCompound::CH4O;
    tTcTypes[1]      = ChemicalCompound::C2H6O;
    tTcTypes[2]      = ChemicalCompound::C4H10O;
    tTcTypes[3]      = ChemicalCompound::CH2O;
    tTcTypes[4]      = ChemicalCompound::C2H4O;
    tTcTypes[5]      = ChemicalCompound::C6H6;
    tTcTypes[6]      = ChemicalCompound::C7H8;
    tTcTypes[7]      = ChemicalCompound::C8H10;
    tTcTypes[8]      = ChemicalCompound::CH2CL2;
    tTcTypes[9]      = ChemicalCompound::C3H6O;
    tTcTypes[10]     = ChemicalCompound::NH3;
    tTcTypes[11]     = ChemicalCompound::CO;
    tTcTypes[12]     = ChemicalCompound::H2;
    tTcTypes[13]     = ChemicalCompound::CH4;
    tFluidTcConfig   = new GunnsFluidTraceCompoundsConfigData(tTcTypes,
                                                              UtGunnsFluidMetabolic3::N_TC,
                                                              "tFluidTcConfig");

    tFluidConfig     = new PolyFluidConfigData(tFluidProperties, tTypes,
                                               UtGunnsFluidMetabolic3::N_CONSTITUENTS, tFluidTcConfig);
    tFluidInput      = new PolyFluidInputData(294.261, 101.325, 1.0, 0.0, tFractions);

    /// - Initialize the node fluids
    tNodes[0].initialize("UtNode1", tFluidConfig);
    tNodes[1].initialize("UtNode2", tFluidConfig);
    tNodes[2].initialize("UtNode3", tFluidConfig);
    tNodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput);
    tNodes[1].getContent()->initialize(*tFluidConfig, *tFluidInput);
    tNodes[2].getContent()->initialize(*tFluidConfig, *tFluidInput);
    tNodes[0].resetFlows();
    tNodes[1].resetFlows();
    tNodes[2].resetFlows();
    tNodes[1].initVolume(10.0);

    /// - Define the nominal metabolic config data
    tName            = "nominal";
    tConfigData      = new GunnsFluidMetabolic3ConfigData(tName, &tNodeList);

    /// - Define the nominal metabolic input data
    tNNominal        = 3.0;
    tNSleep          = 2.0;
    tNRecovery0      = 1.0;
    tNRecovery1      = 0.5;
    tNRecovery2      = 0.25;
    tNRecovery3      = 0.05;
    tNAerobic0       = 4.0;
    tNAerobic1       = 0.01;
    tNResistive0     = 4.0;
    tNResistive1     = 0.01;
    tNResistive2     = 4.0;
    tNResistive3     = 0.01;
    tInputData       = new GunnsFluidMetabolic3InputData(false, 0.0, 0.0,
                                                         tNNominal,
                                                         tNSleep,
                                                         tNRecovery0,
                                                         tNRecovery1,
                                                         tNRecovery2,
                                                         tNRecovery3,
                                                         tNAerobic0,
                                                         tNAerobic1,
                                                         tNResistive0,
                                                         tNResistive1,
                                                         tNResistive2,
                                                         tNResistive3);

    /// - Create the nominal test article
    tArticle = new FriendlyGunnsFluidMetabolic3();

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMetabolic3::tearDown()
{
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete tFluidInput;
    delete tFluidConfig;
    delete tFluidTcConfig;
    delete tCompoundProperties;
    delete tFluidProperties;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Metabolic link model construction of config and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMetabolic3::testConfigAndInput()
{
    UT_RESULT_FIRST;

    /// @test config nominal construction
    CPPUNIT_ASSERT(tName                          == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                         == tConfigData->mNodeList->mNodes);

    /// @test of input nominal construction
    CPPUNIT_ASSERT(false                          == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                            == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0                            == tInputData->mFlowDemand);
    CPPUNIT_ASSERT(tNNominal                      == tInputData->mNNominal);
    CPPUNIT_ASSERT(tNSleep                        == tInputData->mNSleep);
    CPPUNIT_ASSERT(tNRecovery0                    == tInputData->mNRecovery0);
    CPPUNIT_ASSERT(tNRecovery1                    == tInputData->mNRecovery1);
    CPPUNIT_ASSERT(tNRecovery2                    == tInputData->mNRecovery2);
    CPPUNIT_ASSERT(tNRecovery3                    == tInputData->mNRecovery3);
    CPPUNIT_ASSERT(tNAerobic0                     == tInputData->mNAerobic0);
    CPPUNIT_ASSERT(tNAerobic1                     == tInputData->mNAerobic1);
    CPPUNIT_ASSERT(tNResistive0                   == tInputData->mNResistive0);
    CPPUNIT_ASSERT(tNResistive1                   == tInputData->mNResistive1);
    CPPUNIT_ASSERT(tNResistive2                   == tInputData->mNResistive2);
    CPPUNIT_ASSERT(tNResistive3                   == tInputData->mNResistive3);

    /// @test default config construction
    GunnsFluidMetabolic3ConfigData defaultConfig;
    CPPUNIT_ASSERT(0                             == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0                             == defaultConfig.mNodeList);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.8333333333e-6,   defaultConfig.mO2ConsumptionRate_Nominal,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.1666666667e-6,   defaultConfig.mO2ConsumptionRate_Sleep,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.8333333333e-6,   defaultConfig.mO2ConsumptionRate_Recovery0,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.8333333333e-6,   defaultConfig.mO2ConsumptionRate_Recovery1,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.8333333333e-6,   defaultConfig.mO2ConsumptionRate_Recovery2,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.8333333333e-6,   defaultConfig.mO2ConsumptionRate_Recovery3,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.65e-5,           defaultConfig.mO2ConsumptionRate_Aerobic0,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.65e-5,           defaultConfig.mO2ConsumptionRate_Aerobic1,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.38333333333e-5,  defaultConfig.mO2ConsumptionRate_Resistive0,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.38333333333e-5,  defaultConfig.mO2ConsumptionRate_Resistive1,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.38333333333e-5,  defaultConfig.mO2ConsumptionRate_Resistive2,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.38333333333e-5,  defaultConfig.mO2ConsumptionRate_Resistive3,  DBL_EPSILON);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.15e-5,           defaultConfig.mCO2ProductionRate_Nominal,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(7.3333333333e-6,   defaultConfig.mCO2ProductionRate_Sleep,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.15e-5,           defaultConfig.mCO2ProductionRate_Recovery0,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.15e-5,           defaultConfig.mCO2ProductionRate_Recovery1,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.15e-5,           defaultConfig.mCO2ProductionRate_Recovery2,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.15e-5,           defaultConfig.mCO2ProductionRate_Recovery3,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(8.7e-5,            defaultConfig.mCO2ProductionRate_Aerobic0,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(8.7e-5,            defaultConfig.mCO2ProductionRate_Aerobic1,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.15e-5,           defaultConfig.mCO2ProductionRate_Resistive0,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.15e-5,           defaultConfig.mCO2ProductionRate_Resistive1,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.15e-5,           defaultConfig.mCO2ProductionRate_Resistive2,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.15e-5,           defaultConfig.mCO2ProductionRate_Resistive3,  DBL_EPSILON);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.21666666667e-5,  defaultConfig.mH2OProductionRate_Nominal,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.8e-5,            defaultConfig.mH2OProductionRate_Sleep,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.45e-5,           defaultConfig.mH2OProductionRate_Recovery0,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.98333333333e-5,  defaultConfig.mH2OProductionRate_Recovery1,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.66666666667e-5,  defaultConfig.mH2OProductionRate_Recovery2,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.1e-5,            defaultConfig.mH2OProductionRate_Recovery3,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.388333333333e-4, defaultConfig.mH2OProductionRate_Aerobic0,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.216666666667e-4, defaultConfig.mH2OProductionRate_Aerobic1,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.91e-4,           defaultConfig.mH2OProductionRate_Resistive0,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.77e-4,           defaultConfig.mH2OProductionRate_Resistive1,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.24e-4,           defaultConfig.mH2OProductionRate_Resistive2,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.063333333333e-4, defaultConfig.mH2OProductionRate_Resistive3,  DBL_EPSILON);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(85.0,              defaultConfig.mHeatProductionRate_Nominal,    FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(44.4444444,        defaultConfig.mHeatProductionRate_Sleep,      FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(82.7777778,        defaultConfig.mHeatProductionRate_Recovery0,  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(83.6111111,        defaultConfig.mHeatProductionRate_Recovery1,  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(81.9444444,        defaultConfig.mHeatProductionRate_Recovery2,  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(83.3333333,        defaultConfig.mHeatProductionRate_Recovery3,  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(133.8888889,       defaultConfig.mHeatProductionRate_Aerobic0,   FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(129.7222222,       defaultConfig.mHeatProductionRate_Aerobic1,   FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(115.8333333,       defaultConfig.mHeatProductionRate_Resistive0, FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(96.3888889,        defaultConfig.mHeatProductionRate_Resistive1, FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(93.8888889,        defaultConfig.mHeatProductionRate_Resistive2, FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(94.4444444,        defaultConfig.mHeatProductionRate_Resistive3, FLT_EPSILON);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.04166666667e-11, defaultConfig.mCH4OProductionRate,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.97685185185e-11, defaultConfig.mC2H6OProductionRate,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.78703703704e-12, defaultConfig.mC4H10OProductionRate, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.62962962963e-12, defaultConfig.mCH2OProductionRate,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.94444444444e-12, defaultConfig.mC2H4OProductionRate,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.54629629630e-11, defaultConfig.mC6H6ProductionRate,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.94444444444e-12, defaultConfig.mC7H8ProductionRate,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.31481481481e-12, defaultConfig.mC8H10ProductionRate,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.04166666667e-12, defaultConfig.mCH2CL2ProductionRate, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.19907407407e-10, defaultConfig.mC3H6OProductionRate,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.78703703704e-10, defaultConfig.mNH3ProductionRate,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.08333333333e-10, defaultConfig.mCOProductionRate,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.86111111111e-10, defaultConfig.mH2ProductionRate,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.80787037037e-09, defaultConfig.mCH4ProductionRate,    DBL_EPSILON);

    /// @test of input default construction
    GunnsFluidMetabolic3InputData defaultInput;
    CPPUNIT_ASSERT(false                          == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                            == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0                            == defaultInput.mFlowDemand);
    CPPUNIT_ASSERT(0.0                            == defaultInput.mNNominal);
    CPPUNIT_ASSERT(0.0                            == defaultInput.mNSleep);
    CPPUNIT_ASSERT(0.0                            == defaultInput.mNRecovery0);
    CPPUNIT_ASSERT(0.0                            == defaultInput.mNRecovery1);
    CPPUNIT_ASSERT(0.0                            == defaultInput.mNRecovery2);
    CPPUNIT_ASSERT(0.0                            == defaultInput.mNRecovery3);
    CPPUNIT_ASSERT(0.0                            == defaultInput.mNAerobic0);
    CPPUNIT_ASSERT(0.0                            == defaultInput.mNAerobic1);
    CPPUNIT_ASSERT(0.0                            == defaultInput.mNResistive0);
    CPPUNIT_ASSERT(0.0                            == defaultInput.mNResistive1);
    CPPUNIT_ASSERT(0.0                            == defaultInput.mNResistive2);
    CPPUNIT_ASSERT(0.0                            == defaultInput.mNResistive3);

    /// @test config copy construction
    GunnsFluidMetabolic3ConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tConfigData->mName             == copyConfig.mName);
    CPPUNIT_ASSERT(tConfigData->mNodeList->mNodes == copyConfig.mNodeList->mNodes);

    /// @test input copy construction
    GunnsFluidMetabolic3InputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageValue == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(tInputData->mFlowDemand        == copyInput.mFlowDemand);
    CPPUNIT_ASSERT(tInputData->mNNominal          == copyInput.mNNominal);
    CPPUNIT_ASSERT(tInputData->mNSleep            == copyInput.mNSleep);
    CPPUNIT_ASSERT(tInputData->mNRecovery0        == copyInput.mNRecovery0);
    CPPUNIT_ASSERT(tInputData->mNRecovery1        == copyInput.mNRecovery1);
    CPPUNIT_ASSERT(tInputData->mNRecovery2        == copyInput.mNRecovery2);
    CPPUNIT_ASSERT(tInputData->mNRecovery3        == copyInput.mNRecovery3);
    CPPUNIT_ASSERT(tInputData->mNAerobic0         == copyInput.mNAerobic0);
    CPPUNIT_ASSERT(tInputData->mNAerobic1         == copyInput.mNAerobic1);
    CPPUNIT_ASSERT(tInputData->mNResistive0       == copyInput.mNResistive0);
    CPPUNIT_ASSERT(tInputData->mNResistive1       == copyInput.mNResistive1);
    CPPUNIT_ASSERT(tInputData->mNResistive2       == copyInput.mNResistive2);
    CPPUNIT_ASSERT(tInputData->mNResistive3       == copyInput.mNResistive3);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Metabolic link model default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMetabolic3::testDefaultConstruction()
{
    UT_RESULT;

    CPPUNIT_ASSERT(0.0 == tArticle->mNCrew[GunnsFluidMetabolic3::NOMINAL]);
    CPPUNIT_ASSERT(0.0 == tArticle->mNCrew[GunnsFluidMetabolic3::SLEEP]);
    CPPUNIT_ASSERT(0.0 == tArticle->mNCrew[GunnsFluidMetabolic3::RECOVERY_0]);
    CPPUNIT_ASSERT(0.0 == tArticle->mNCrew[GunnsFluidMetabolic3::RECOVERY_1]);
    CPPUNIT_ASSERT(0.0 == tArticle->mNCrew[GunnsFluidMetabolic3::RECOVERY_2]);
    CPPUNIT_ASSERT(0.0 == tArticle->mNCrew[GunnsFluidMetabolic3::RECOVERY_3]);
    CPPUNIT_ASSERT(0.0 == tArticle->mNCrew[GunnsFluidMetabolic3::AEROBIC_0]);
    CPPUNIT_ASSERT(0.0 == tArticle->mNCrew[GunnsFluidMetabolic3::AEROBIC_1]);
    CPPUNIT_ASSERT(0.0 == tArticle->mNCrew[GunnsFluidMetabolic3::RESISTIVE_0]);
    CPPUNIT_ASSERT(0.0 == tArticle->mNCrew[GunnsFluidMetabolic3::RESISTIVE_1]);
    CPPUNIT_ASSERT(0.0 == tArticle->mNCrew[GunnsFluidMetabolic3::RESISTIVE_2]);
    CPPUNIT_ASSERT(0.0 == tArticle->mNCrew[GunnsFluidMetabolic3::RESISTIVE_3]);

    CPPUNIT_ASSERT(0.0 == tArticle->mConsumedO2);
    CPPUNIT_ASSERT(0.0 == tArticle->mProducedCO2);
    CPPUNIT_ASSERT(0.0 == tArticle->mProducedH2O);
    CPPUNIT_ASSERT(0.0 == tArticle->mProducedHeat);
    CPPUNIT_ASSERT(0.0 == tArticle->mProducedCH4O);
    CPPUNIT_ASSERT(0.0 == tArticle->mProducedC2H6O);
    CPPUNIT_ASSERT(0.0 == tArticle->mProducedC4H10O);
    CPPUNIT_ASSERT(0.0 == tArticle->mProducedCH2O);
    CPPUNIT_ASSERT(0.0 == tArticle->mProducedC2H4O);
    CPPUNIT_ASSERT(0.0 == tArticle->mProducedC6H6);
    CPPUNIT_ASSERT(0.0 == tArticle->mProducedC7H8);
    CPPUNIT_ASSERT(0.0 == tArticle->mProducedC8H10);
    CPPUNIT_ASSERT(0.0 == tArticle->mProducedCH2CL2);
    CPPUNIT_ASSERT(0.0 == tArticle->mProducedC3H6O);
    CPPUNIT_ASSERT(0.0 == tArticle->mProducedNH3);
    CPPUNIT_ASSERT(0.0 == tArticle->mProducedCO);
    CPPUNIT_ASSERT(0.0 == tArticle->mProducedH2);
    CPPUNIT_ASSERT(0.0 == tArticle->mProducedCH4);

    CPPUNIT_ASSERT(-1  == tArticle->mO2);
    CPPUNIT_ASSERT(-1  == tArticle->mCO2);
    CPPUNIT_ASSERT(-1  == tArticle->mH2O);
    CPPUNIT_ASSERT(-1  == tArticle->mNH3);
    CPPUNIT_ASSERT(-1  == tArticle->mCO);
    CPPUNIT_ASSERT(-1  == tArticle->mH2);
    CPPUNIT_ASSERT(-1  == tArticle->mCH4);
    CPPUNIT_ASSERT(-1  == tArticle->mTcCH4O);
    CPPUNIT_ASSERT(-1  == tArticle->mTcC2H6O);
    CPPUNIT_ASSERT(-1  == tArticle->mTcC4H10O);
    CPPUNIT_ASSERT(-1  == tArticle->mTcCH2O);
    CPPUNIT_ASSERT(-1  == tArticle->mTcC2H4O);
    CPPUNIT_ASSERT(-1  == tArticle->mTcC6H6);
    CPPUNIT_ASSERT(-1  == tArticle->mTcC7H8);
    CPPUNIT_ASSERT(-1  == tArticle->mTcC8H10);
    CPPUNIT_ASSERT(-1  == tArticle->mTcCH2CL2);
    CPPUNIT_ASSERT(-1  == tArticle->mTcC3H6O);
    CPPUNIT_ASSERT(-1  == tArticle->mTcNH3);
    CPPUNIT_ASSERT(-1  == tArticle->mTcCO);
    CPPUNIT_ASSERT(-1  == tArticle->mTcH2);
    CPPUNIT_ASSERT(-1  == tArticle->mTcCH4);

    /// @test Destructor for code coverage
    GunnsFluidMetabolic3* article = new GunnsFluidMetabolic3;
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Metabolic link model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMetabolic3::testNominalInitialization()
{
    UT_RESULT;

    tArticle->initialize(*tConfigData, *tInputData, tLinks, 2, 1);

    CPPUNIT_ASSERT(tName        == tArticle->mName);
    CPPUNIT_ASSERT(                tArticle->mInternalFluid);
    CPPUNIT_ASSERT(&tNodes[2]   == tArticle->mNodes[0]);
    CPPUNIT_ASSERT(&tNodes[1]   == tArticle->mNodes[1]);
    CPPUNIT_ASSERT(0.0          == tArticle->mFlowDemand);
    CPPUNIT_ASSERT(0.0          == tArticle->mFlowRate);
    CPPUNIT_ASSERT(0.0          == tArticle->mFlux);
    CPPUNIT_ASSERT(tNNominal    == tArticle->mNCrew[GunnsFluidMetabolic3::NOMINAL]);
    CPPUNIT_ASSERT(tNSleep      == tArticle->mNCrew[GunnsFluidMetabolic3::SLEEP]);
    CPPUNIT_ASSERT(tNRecovery0  == tArticle->mNCrew[GunnsFluidMetabolic3::RECOVERY_0]);
    CPPUNIT_ASSERT(tNRecovery1  == tArticle->mNCrew[GunnsFluidMetabolic3::RECOVERY_1]);
    CPPUNIT_ASSERT(tNRecovery2  == tArticle->mNCrew[GunnsFluidMetabolic3::RECOVERY_2]);
    CPPUNIT_ASSERT(tNRecovery3  == tArticle->mNCrew[GunnsFluidMetabolic3::RECOVERY_3]);
    CPPUNIT_ASSERT(tNAerobic0   == tArticle->mNCrew[GunnsFluidMetabolic3::AEROBIC_0]); 
    CPPUNIT_ASSERT(tNAerobic1   == tArticle->mNCrew[GunnsFluidMetabolic3::AEROBIC_1]); 
    CPPUNIT_ASSERT(tNResistive0 == tArticle->mNCrew[GunnsFluidMetabolic3::RESISTIVE_0]);
    CPPUNIT_ASSERT(tNResistive1 == tArticle->mNCrew[GunnsFluidMetabolic3::RESISTIVE_1]);
    CPPUNIT_ASSERT(tNResistive2 == tArticle->mNCrew[GunnsFluidMetabolic3::RESISTIVE_2]);
    CPPUNIT_ASSERT(tNResistive3 == tArticle->mNCrew[GunnsFluidMetabolic3::RESISTIVE_3]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.8333333333e-6,   tArticle->mO2ConsumptionRate[GunnsFluidMetabolic3::NOMINAL],      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.1666666667e-6,   tArticle->mO2ConsumptionRate[GunnsFluidMetabolic3::SLEEP],        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.8333333333e-6,   tArticle->mO2ConsumptionRate[GunnsFluidMetabolic3::RECOVERY_0],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.8333333333e-6,   tArticle->mO2ConsumptionRate[GunnsFluidMetabolic3::RECOVERY_1],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.8333333333e-6,   tArticle->mO2ConsumptionRate[GunnsFluidMetabolic3::RECOVERY_2],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.8333333333e-6,   tArticle->mO2ConsumptionRate[GunnsFluidMetabolic3::RECOVERY_3],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.65e-5,           tArticle->mO2ConsumptionRate[GunnsFluidMetabolic3::AEROBIC_0],    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.65e-5,           tArticle->mO2ConsumptionRate[GunnsFluidMetabolic3::AEROBIC_1],    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.38333333333e-5,  tArticle->mO2ConsumptionRate[GunnsFluidMetabolic3::RESISTIVE_0],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.38333333333e-5,  tArticle->mO2ConsumptionRate[GunnsFluidMetabolic3::RESISTIVE_1],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.38333333333e-5,  tArticle->mO2ConsumptionRate[GunnsFluidMetabolic3::RESISTIVE_2],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.38333333333e-5,  tArticle->mO2ConsumptionRate[GunnsFluidMetabolic3::RESISTIVE_3],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.15e-5,           tArticle->mCO2ProductionRate[GunnsFluidMetabolic3::NOMINAL],      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(7.3333333333e-6,   tArticle->mCO2ProductionRate[GunnsFluidMetabolic3::SLEEP],        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.15e-5,           tArticle->mCO2ProductionRate[GunnsFluidMetabolic3::RECOVERY_0],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.15e-5,           tArticle->mCO2ProductionRate[GunnsFluidMetabolic3::RECOVERY_1],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.15e-5,           tArticle->mCO2ProductionRate[GunnsFluidMetabolic3::RECOVERY_2],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.15e-5,           tArticle->mCO2ProductionRate[GunnsFluidMetabolic3::RECOVERY_3],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(8.7e-5,            tArticle->mCO2ProductionRate[GunnsFluidMetabolic3::AEROBIC_0],    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(8.7e-5,            tArticle->mCO2ProductionRate[GunnsFluidMetabolic3::AEROBIC_1],    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.15e-5,           tArticle->mCO2ProductionRate[GunnsFluidMetabolic3::RESISTIVE_0],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.15e-5,           tArticle->mCO2ProductionRate[GunnsFluidMetabolic3::RESISTIVE_1],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.15e-5,           tArticle->mCO2ProductionRate[GunnsFluidMetabolic3::RESISTIVE_2],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.15e-5,           tArticle->mCO2ProductionRate[GunnsFluidMetabolic3::RESISTIVE_3],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.21666666667e-5,  tArticle->mH2OProductionRate[GunnsFluidMetabolic3::NOMINAL],      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.8e-5,            tArticle->mH2OProductionRate[GunnsFluidMetabolic3::SLEEP],        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.45e-5,           tArticle->mH2OProductionRate[GunnsFluidMetabolic3::RECOVERY_0],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.98333333333e-5,  tArticle->mH2OProductionRate[GunnsFluidMetabolic3::RECOVERY_1],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.66666666667e-5,  tArticle->mH2OProductionRate[GunnsFluidMetabolic3::RECOVERY_2],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.1e-5,            tArticle->mH2OProductionRate[GunnsFluidMetabolic3::RECOVERY_3],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.388333333333e-4, tArticle->mH2OProductionRate[GunnsFluidMetabolic3::AEROBIC_0],    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.216666666667e-4, tArticle->mH2OProductionRate[GunnsFluidMetabolic3::AEROBIC_1],    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.91e-4,           tArticle->mH2OProductionRate[GunnsFluidMetabolic3::RESISTIVE_0],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.77e-4,           tArticle->mH2OProductionRate[GunnsFluidMetabolic3::RESISTIVE_1],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.24e-4,           tArticle->mH2OProductionRate[GunnsFluidMetabolic3::RESISTIVE_2],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.063333333333e-4, tArticle->mH2OProductionRate[GunnsFluidMetabolic3::RESISTIVE_3],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(85.0,              tArticle->mHeatProductionRate[GunnsFluidMetabolic3::NOMINAL],     FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(44.4444444,        tArticle->mHeatProductionRate[GunnsFluidMetabolic3::SLEEP],       FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(82.7777778,        tArticle->mHeatProductionRate[GunnsFluidMetabolic3::RECOVERY_0],  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(83.6111111,        tArticle->mHeatProductionRate[GunnsFluidMetabolic3::RECOVERY_1],  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(81.9444444,        tArticle->mHeatProductionRate[GunnsFluidMetabolic3::RECOVERY_2],  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(83.3333333,        tArticle->mHeatProductionRate[GunnsFluidMetabolic3::RECOVERY_3],  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(133.8888889,       tArticle->mHeatProductionRate[GunnsFluidMetabolic3::AEROBIC_0],   FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(129.7222222,       tArticle->mHeatProductionRate[GunnsFluidMetabolic3::AEROBIC_1],   FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(115.8333333,       tArticle->mHeatProductionRate[GunnsFluidMetabolic3::RESISTIVE_0], FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(96.3888889,        tArticle->mHeatProductionRate[GunnsFluidMetabolic3::RESISTIVE_1], FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(93.8888889,        tArticle->mHeatProductionRate[GunnsFluidMetabolic3::RESISTIVE_2], FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(94.4444444,        tArticle->mHeatProductionRate[GunnsFluidMetabolic3::RESISTIVE_3], FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.04166666667e-11, tArticle->mCH4OProductionRate,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.97685185185e-11, tArticle->mC2H6OProductionRate,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.78703703704e-12, tArticle->mC4H10OProductionRate, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.62962962963e-12, tArticle->mCH2OProductionRate,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.94444444444e-12, tArticle->mC2H4OProductionRate,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.54629629630e-11, tArticle->mC6H6ProductionRate,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.94444444444e-12, tArticle->mC7H8ProductionRate,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.31481481481e-12, tArticle->mC8H10ProductionRate,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.04166666667e-12, tArticle->mCH2CL2ProductionRate, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.19907407407e-10, tArticle->mC3H6OProductionRate,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.78703703704e-10, tArticle->mNH3ProductionRate,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.08333333333e-10, tArticle->mCOProductionRate,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.86111111111e-10, tArticle->mH2ProductionRate,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.80787037037e-09, tArticle->mCH4ProductionRate,    DBL_EPSILON);
    CPPUNIT_ASSERT(0.0         == tArticle->mConsumedO2);
    CPPUNIT_ASSERT(0.0         == tArticle->mProducedCO2);
    CPPUNIT_ASSERT(0.0         == tArticle->mProducedH2O);
    CPPUNIT_ASSERT(0.0         == tArticle->mProducedHeat);
    CPPUNIT_ASSERT(0.0         == tArticle->mProducedCH4O);
    CPPUNIT_ASSERT(0.0         == tArticle->mProducedC2H6O);
    CPPUNIT_ASSERT(0.0         == tArticle->mProducedC4H10O);
    CPPUNIT_ASSERT(0.0         == tArticle->mProducedCH2O);
    CPPUNIT_ASSERT(0.0         == tArticle->mProducedC2H4O);
    CPPUNIT_ASSERT(0.0         == tArticle->mProducedC6H6);
    CPPUNIT_ASSERT(0.0         == tArticle->mProducedC7H8);
    CPPUNIT_ASSERT(0.0         == tArticle->mProducedC8H10);
    CPPUNIT_ASSERT(0.0         == tArticle->mProducedCH2CL2);
    CPPUNIT_ASSERT(0.0         == tArticle->mProducedC3H6O);
    CPPUNIT_ASSERT(0.0         == tArticle->mProducedNH3);
    CPPUNIT_ASSERT(0.0         == tArticle->mProducedCO);
    CPPUNIT_ASSERT(0.0         == tArticle->mProducedH2);
    CPPUNIT_ASSERT(0.0         == tArticle->mProducedCH4);
    CPPUNIT_ASSERT( 1          == tArticle->mO2);
    CPPUNIT_ASSERT( 3          == tArticle->mCO2);
    CPPUNIT_ASSERT( 2          == tArticle->mH2O);
    CPPUNIT_ASSERT(-1          == tArticle->mNH3);
    CPPUNIT_ASSERT(-1          == tArticle->mCO);
    CPPUNIT_ASSERT(-1          == tArticle->mH2);
    CPPUNIT_ASSERT(-1          == tArticle->mCH4);
    CPPUNIT_ASSERT( 0          == tArticle->mTcCH4O);
    CPPUNIT_ASSERT( 1          == tArticle->mTcC2H6O);
    CPPUNIT_ASSERT( 2          == tArticle->mTcC4H10O);
    CPPUNIT_ASSERT( 3          == tArticle->mTcCH2O);
    CPPUNIT_ASSERT( 4          == tArticle->mTcC2H4O);
    CPPUNIT_ASSERT( 5          == tArticle->mTcC6H6);
    CPPUNIT_ASSERT( 6          == tArticle->mTcC7H8);
    CPPUNIT_ASSERT( 7          == tArticle->mTcC8H10);
    CPPUNIT_ASSERT( 8          == tArticle->mTcCH2CL2);
    CPPUNIT_ASSERT( 9          == tArticle->mTcC3H6O);
    CPPUNIT_ASSERT(10          == tArticle->mTcNH3);
    CPPUNIT_ASSERT(11          == tArticle->mTcCO);
    CPPUNIT_ASSERT(12          == tArticle->mTcH2);
    CPPUNIT_ASSERT(13          == tArticle->mTcCH4);
    CPPUNIT_ASSERT(               tArticle->mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Metabolic link model nominal initialization with no trace
///           compounds.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMetabolic3::testNominalInitializationNoTc()
{
    UT_RESULT;

    /// - Define a new fluid config that adds 4 trace compounds as fluid types instead.
    FluidProperties::FluidType types[8];
    double                     fractions[8];
    types[0]         = FluidProperties::GUNNS_N2;  fractions[0] = 0.750;
    types[1]         = FluidProperties::GUNNS_O2;  fractions[1] = 0.200;
    types[2]         = FluidProperties::GUNNS_H2O; fractions[2] = 0.035;
    types[3]         = FluidProperties::GUNNS_CO2; fractions[3] = 0.010;
    types[4]         = FluidProperties::GUNNS_NH3; fractions[4] = 0.001;
    types[5]         = FluidProperties::GUNNS_CO;  fractions[5] = 0.001;
    types[6]         = FluidProperties::GUNNS_H2;  fractions[6] = 0.001;
    types[7]         = FluidProperties::GUNNS_CH4; fractions[7] = 0.002;

    delete tFluidConfig;
    tFluidConfig     = new PolyFluidConfigData(tFluidProperties, types, 8);
    delete tFluidInput;
    tFluidInput      = new PolyFluidInputData(294.261, 101.325, 1.0, 0.0, fractions);

    FriendlyGunnsFluidNodeMetabolic3 nodes[3];
    tNodeList.mNodes = nodes;

    /// - Initialize the node fluids
    nodes[0].initialize("UtNode1", tFluidConfig);
    nodes[1].initialize("UtNode2", tFluidConfig);
    nodes[2].initialize("UtNode3", tFluidConfig);
    nodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput);
    nodes[1].getContent()->initialize(*tFluidConfig, *tFluidInput);
    nodes[2].getContent()->initialize(*tFluidConfig, *tFluidInput);
    nodes[1].initVolume(10.0);

    tArticle->initialize(*tConfigData, *tInputData, tLinks, 2, 1);

    CPPUNIT_ASSERT(tName        == tArticle->mName);
    CPPUNIT_ASSERT(                tArticle->mInternalFluid);
    CPPUNIT_ASSERT(&nodes[2]    == tArticle->mNodes[0]);
    CPPUNIT_ASSERT(&nodes[1]    == tArticle->mNodes[1]);
    CPPUNIT_ASSERT(0.0          == tArticle->mFlowDemand);
    CPPUNIT_ASSERT(0.0          == tArticle->mFlowRate);
    CPPUNIT_ASSERT(0.0          == tArticle->mFlux);
    CPPUNIT_ASSERT(tNNominal    == tArticle->mNCrew[GunnsFluidMetabolic3::NOMINAL]);
    CPPUNIT_ASSERT(tNSleep      == tArticle->mNCrew[GunnsFluidMetabolic3::SLEEP]);
    CPPUNIT_ASSERT(tNRecovery0  == tArticle->mNCrew[GunnsFluidMetabolic3::RECOVERY_0]);
    CPPUNIT_ASSERT(tNRecovery1  == tArticle->mNCrew[GunnsFluidMetabolic3::RECOVERY_1]);
    CPPUNIT_ASSERT(tNRecovery2  == tArticle->mNCrew[GunnsFluidMetabolic3::RECOVERY_2]);
    CPPUNIT_ASSERT(tNRecovery3  == tArticle->mNCrew[GunnsFluidMetabolic3::RECOVERY_3]);
    CPPUNIT_ASSERT(tNAerobic0   == tArticle->mNCrew[GunnsFluidMetabolic3::AEROBIC_0]);
    CPPUNIT_ASSERT(tNAerobic1   == tArticle->mNCrew[GunnsFluidMetabolic3::AEROBIC_1]);
    CPPUNIT_ASSERT(tNResistive0 == tArticle->mNCrew[GunnsFluidMetabolic3::RESISTIVE_0]);
    CPPUNIT_ASSERT(tNResistive1 == tArticle->mNCrew[GunnsFluidMetabolic3::RESISTIVE_1]);
    CPPUNIT_ASSERT(tNResistive2 == tArticle->mNCrew[GunnsFluidMetabolic3::RESISTIVE_2]);
    CPPUNIT_ASSERT(tNResistive3 == tArticle->mNCrew[GunnsFluidMetabolic3::RESISTIVE_3]);
    CPPUNIT_ASSERT(0.0          == tArticle->mConsumedO2);
    CPPUNIT_ASSERT(0.0          == tArticle->mProducedCO2);
    CPPUNIT_ASSERT(0.0          == tArticle->mProducedH2O);
    CPPUNIT_ASSERT(0.0          == tArticle->mProducedHeat);
    CPPUNIT_ASSERT(0.0          == tArticle->mProducedCH4O);
    CPPUNIT_ASSERT(0.0          == tArticle->mProducedC2H6O);
    CPPUNIT_ASSERT(0.0          == tArticle->mProducedC4H10O);
    CPPUNIT_ASSERT(0.0          == tArticle->mProducedCH2O);
    CPPUNIT_ASSERT(0.0          == tArticle->mProducedC2H4O);
    CPPUNIT_ASSERT(0.0          == tArticle->mProducedC6H6);
    CPPUNIT_ASSERT(0.0          == tArticle->mProducedC7H8);
    CPPUNIT_ASSERT(0.0          == tArticle->mProducedC8H10);
    CPPUNIT_ASSERT(0.0          == tArticle->mProducedCH2CL2);
    CPPUNIT_ASSERT(0.0          == tArticle->mProducedC3H6O);
    CPPUNIT_ASSERT(0.0          == tArticle->mProducedNH3);
    CPPUNIT_ASSERT(0.0          == tArticle->mProducedCO);
    CPPUNIT_ASSERT(0.0          == tArticle->mProducedH2);
    CPPUNIT_ASSERT(0.0          == tArticle->mProducedCH4);
    CPPUNIT_ASSERT( 1           == tArticle->mO2);
    CPPUNIT_ASSERT( 3           == tArticle->mCO2);
    CPPUNIT_ASSERT( 2           == tArticle->mH2O);
    CPPUNIT_ASSERT( 4           == tArticle->mNH3);
    CPPUNIT_ASSERT( 5           == tArticle->mCO);
    CPPUNIT_ASSERT( 6           == tArticle->mH2);
    CPPUNIT_ASSERT( 7           == tArticle->mCH4);
    CPPUNIT_ASSERT(-1           == tArticle->mTcCH4O);
    CPPUNIT_ASSERT(-1           == tArticle->mTcC2H6O);
    CPPUNIT_ASSERT(-1           == tArticle->mTcC4H10O);
    CPPUNIT_ASSERT(-1           == tArticle->mTcCH2O);
    CPPUNIT_ASSERT(-1           == tArticle->mTcC2H4O);
    CPPUNIT_ASSERT(-1           == tArticle->mTcC6H6);
    CPPUNIT_ASSERT(-1           == tArticle->mTcC7H8);
    CPPUNIT_ASSERT(-1           == tArticle->mTcC8H10);
    CPPUNIT_ASSERT(-1           == tArticle->mTcCH2CL2);
    CPPUNIT_ASSERT(-1           == tArticle->mTcC3H6O);
    CPPUNIT_ASSERT(-1           == tArticle->mTcNH3);
    CPPUNIT_ASSERT(-1           == tArticle->mTcCO);
    CPPUNIT_ASSERT(-1           == tArticle->mTcH2);
    CPPUNIT_ASSERT(-1           == tArticle->mTcCH4);
    CPPUNIT_ASSERT(                tArticle->mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Metabolic link model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMetabolic3::testAccess()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, 2, 1);

    /// @test    Test the access methods.
    tArticle->mConsumedO2   =  5.0;
    tArticle->mProducedCO2  =  7.0;
    tArticle->mProducedH2O  =  9.0;
    tArticle->mProducedHeat = 11.0;
    tArticle->mProducedNH3  = 13.0;
    tArticle->mProducedCO   = 15.0;
    tArticle->mProducedH2   = 17.0;
    tArticle->mProducedCH4  = 19.0;
    tArticle->mNCrew[GunnsFluidMetabolic3::AEROBIC_0] = 2.5;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 5.0, tArticle->getConsumedO2(),   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 7.0, tArticle->getProducedCO2(),  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 9.0, tArticle->getProducedH2O(),  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(11.0, tArticle->getProducedHeat(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(13.0, tArticle->getProducedNH3(),  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(15.0, tArticle->getProducedCO(),   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(17.0, tArticle->getProducedH2(),   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(19.0, tArticle->getProducedCH4(),  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.5, *(tArticle->getNCrew()+GunnsFluidMetabolic3::AEROBIC_0), 0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Metabolic link model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMetabolic3::testModify()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, 2, 1);

    /// @test transition single crew member nominal -> sleep.
    tArticle->transition(1.0, GunnsFluidMetabolic3::NOMINAL, GunnsFluidMetabolic3::SLEEP);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNNominal - 1.0, tArticle->mNCrew[GunnsFluidMetabolic3::NOMINAL],     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNSleep   + 1.0, tArticle->mNCrew[GunnsFluidMetabolic3::SLEEP],       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNRecovery0,     tArticle->mNCrew[GunnsFluidMetabolic3::RECOVERY_0],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNRecovery1,     tArticle->mNCrew[GunnsFluidMetabolic3::RECOVERY_1],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNRecovery2,     tArticle->mNCrew[GunnsFluidMetabolic3::RECOVERY_2],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNRecovery3,     tArticle->mNCrew[GunnsFluidMetabolic3::RECOVERY_3],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNAerobic0,      tArticle->mNCrew[GunnsFluidMetabolic3::AEROBIC_0],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNAerobic1,      tArticle->mNCrew[GunnsFluidMetabolic3::AEROBIC_1],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNResistive0,    tArticle->mNCrew[GunnsFluidMetabolic3::RESISTIVE_0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNResistive1,    tArticle->mNCrew[GunnsFluidMetabolic3::RESISTIVE_1], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNResistive2,    tArticle->mNCrew[GunnsFluidMetabolic3::RESISTIVE_2], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNResistive3,    tArticle->mNCrew[GunnsFluidMetabolic3::RESISTIVE_3], 0.0);

    /// @test adding total # crew, new crew going into nominal.
    tArticle->transition(2.0, GunnsFluidMetabolic3::NO_METABOLIC, GunnsFluidMetabolic3::NOMINAL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNNominal + 1.0, tArticle->mNCrew[GunnsFluidMetabolic3::NOMINAL],     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNSleep   + 1.0, tArticle->mNCrew[GunnsFluidMetabolic3::SLEEP],       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNRecovery0,     tArticle->mNCrew[GunnsFluidMetabolic3::RECOVERY_0],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNRecovery1,     tArticle->mNCrew[GunnsFluidMetabolic3::RECOVERY_1],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNRecovery2,     tArticle->mNCrew[GunnsFluidMetabolic3::RECOVERY_2],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNRecovery3,     tArticle->mNCrew[GunnsFluidMetabolic3::RECOVERY_3],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNAerobic0,      tArticle->mNCrew[GunnsFluidMetabolic3::AEROBIC_0],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNAerobic1,      tArticle->mNCrew[GunnsFluidMetabolic3::AEROBIC_1],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNResistive0,    tArticle->mNCrew[GunnsFluidMetabolic3::RESISTIVE_0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNResistive1,    tArticle->mNCrew[GunnsFluidMetabolic3::RESISTIVE_1], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNResistive2,    tArticle->mNCrew[GunnsFluidMetabolic3::RESISTIVE_2], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNResistive3,    tArticle->mNCrew[GunnsFluidMetabolic3::RESISTIVE_3], 0.0);

    /// @test removing total # crew, more than what is currently in sleep.
    tArticle->transition(tNSleep + 10, GunnsFluidMetabolic3::SLEEP, GunnsFluidMetabolic3::NO_METABOLIC);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNNominal + 1.0, tArticle->mNCrew[GunnsFluidMetabolic3::NOMINAL],     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0,               tArticle->mNCrew[GunnsFluidMetabolic3::SLEEP],       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNRecovery0,     tArticle->mNCrew[GunnsFluidMetabolic3::RECOVERY_0],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNRecovery1,     tArticle->mNCrew[GunnsFluidMetabolic3::RECOVERY_1],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNRecovery2,     tArticle->mNCrew[GunnsFluidMetabolic3::RECOVERY_2],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNRecovery3,     tArticle->mNCrew[GunnsFluidMetabolic3::RECOVERY_3],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNAerobic0,      tArticle->mNCrew[GunnsFluidMetabolic3::AEROBIC_0],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNAerobic1,      tArticle->mNCrew[GunnsFluidMetabolic3::AEROBIC_1],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNResistive0,    tArticle->mNCrew[GunnsFluidMetabolic3::RESISTIVE_0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNResistive1,    tArticle->mNCrew[GunnsFluidMetabolic3::RESISTIVE_1], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNResistive2,    tArticle->mNCrew[GunnsFluidMetabolic3::RESISTIVE_2], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNResistive3,    tArticle->mNCrew[GunnsFluidMetabolic3::RESISTIVE_3], 0.0);

    /// @test transition more than # of nominal into aerobic 1.
    tArticle->transition(tNNominal + 10, GunnsFluidMetabolic3::NOMINAL, GunnsFluidMetabolic3::AEROBIC_1);
    const double expected = tNAerobic1 + tNNominal + 1.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0,               tArticle->mNCrew[GunnsFluidMetabolic3::NOMINAL],     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0,               tArticle->mNCrew[GunnsFluidMetabolic3::SLEEP],       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNRecovery0,     tArticle->mNCrew[GunnsFluidMetabolic3::RECOVERY_0],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNRecovery1,     tArticle->mNCrew[GunnsFluidMetabolic3::RECOVERY_1],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNRecovery2,     tArticle->mNCrew[GunnsFluidMetabolic3::RECOVERY_2],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNRecovery3,     tArticle->mNCrew[GunnsFluidMetabolic3::RECOVERY_3],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNAerobic0,      tArticle->mNCrew[GunnsFluidMetabolic3::AEROBIC_0],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,        tArticle->mNCrew[GunnsFluidMetabolic3::AEROBIC_1],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNResistive0,    tArticle->mNCrew[GunnsFluidMetabolic3::RESISTIVE_0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNResistive1,    tArticle->mNCrew[GunnsFluidMetabolic3::RESISTIVE_1], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNResistive2,    tArticle->mNCrew[GunnsFluidMetabolic3::RESISTIVE_2], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNResistive3,    tArticle->mNCrew[GunnsFluidMetabolic3::RESISTIVE_3], 0.0);

    /// @test nothing happens if given a negative number.
    tArticle->transition(-1, GunnsFluidMetabolic3::AEROBIC_1, GunnsFluidMetabolic3::NOMINAL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0,               tArticle->mNCrew[GunnsFluidMetabolic3::NOMINAL],     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0,               tArticle->mNCrew[GunnsFluidMetabolic3::SLEEP],       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNRecovery0,     tArticle->mNCrew[GunnsFluidMetabolic3::RECOVERY_0],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNRecovery1,     tArticle->mNCrew[GunnsFluidMetabolic3::RECOVERY_1],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNRecovery2,     tArticle->mNCrew[GunnsFluidMetabolic3::RECOVERY_2],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNRecovery3,     tArticle->mNCrew[GunnsFluidMetabolic3::RECOVERY_3],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNAerobic0,      tArticle->mNCrew[GunnsFluidMetabolic3::AEROBIC_0],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,        tArticle->mNCrew[GunnsFluidMetabolic3::AEROBIC_1],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNResistive0,    tArticle->mNCrew[GunnsFluidMetabolic3::RESISTIVE_0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNResistive1,    tArticle->mNCrew[GunnsFluidMetabolic3::RESISTIVE_1], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNResistive2,    tArticle->mNCrew[GunnsFluidMetabolic3::RESISTIVE_2], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNResistive3,    tArticle->mNCrew[GunnsFluidMetabolic3::RESISTIVE_3], 0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Metabolic link model update state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMetabolic3::testStep()
{
    UT_RESULT;

    tArticle->initialize(*tConfigData, *tInputData, tLinks, 2, 1);

    /// @test nominal flows
    double expectedO2  = tNNominal    * tArticle->mO2ConsumptionRate[GunnsFluidMetabolic3::NOMINAL]
                       + tNSleep      * tArticle->mO2ConsumptionRate[GunnsFluidMetabolic3::SLEEP]
                       + tNRecovery0  * tArticle->mO2ConsumptionRate[GunnsFluidMetabolic3::RECOVERY_0]
                       + tNRecovery1  * tArticle->mO2ConsumptionRate[GunnsFluidMetabolic3::RECOVERY_1]
                       + tNRecovery2  * tArticle->mO2ConsumptionRate[GunnsFluidMetabolic3::RECOVERY_2]
                       + tNRecovery3  * tArticle->mO2ConsumptionRate[GunnsFluidMetabolic3::RECOVERY_3]
                       + tNAerobic0   * tArticle->mO2ConsumptionRate[GunnsFluidMetabolic3::AEROBIC_0]
                       + tNAerobic1   * tArticle->mO2ConsumptionRate[GunnsFluidMetabolic3::AEROBIC_1]
                       + tNResistive0 * tArticle->mO2ConsumptionRate[GunnsFluidMetabolic3::RESISTIVE_0]
                       + tNResistive1 * tArticle->mO2ConsumptionRate[GunnsFluidMetabolic3::RESISTIVE_1]
                       + tNResistive2 * tArticle->mO2ConsumptionRate[GunnsFluidMetabolic3::RESISTIVE_2]
                       + tNResistive3 * tArticle->mO2ConsumptionRate[GunnsFluidMetabolic3::RESISTIVE_3];

    double expectedQ   = tNNominal    * tArticle->mHeatProductionRate[GunnsFluidMetabolic3::NOMINAL]
                       + tNSleep      * tArticle->mHeatProductionRate[GunnsFluidMetabolic3::SLEEP]
                       + tNRecovery0  * tArticle->mHeatProductionRate[GunnsFluidMetabolic3::RECOVERY_0]
                       + tNRecovery1  * tArticle->mHeatProductionRate[GunnsFluidMetabolic3::RECOVERY_1]
                       + tNRecovery2  * tArticle->mHeatProductionRate[GunnsFluidMetabolic3::RECOVERY_2]
                       + tNRecovery3  * tArticle->mHeatProductionRate[GunnsFluidMetabolic3::RECOVERY_3]
                       + tNAerobic0   * tArticle->mHeatProductionRate[GunnsFluidMetabolic3::AEROBIC_0]
                       + tNAerobic1   * tArticle->mHeatProductionRate[GunnsFluidMetabolic3::AEROBIC_1]
                       + tNResistive0 * tArticle->mHeatProductionRate[GunnsFluidMetabolic3::RESISTIVE_0]
                       + tNResistive1 * tArticle->mHeatProductionRate[GunnsFluidMetabolic3::RESISTIVE_1]
                       + tNResistive2 * tArticle->mHeatProductionRate[GunnsFluidMetabolic3::RESISTIVE_2]
                       + tNResistive3 * tArticle->mHeatProductionRate[GunnsFluidMetabolic3::RESISTIVE_3];

    double expectedCO2 = tNNominal    * tArticle->mCO2ProductionRate[GunnsFluidMetabolic3::NOMINAL]
                       + tNSleep      * tArticle->mCO2ProductionRate[GunnsFluidMetabolic3::SLEEP]
                       + tNRecovery0  * tArticle->mCO2ProductionRate[GunnsFluidMetabolic3::RECOVERY_0]
                       + tNRecovery1  * tArticle->mCO2ProductionRate[GunnsFluidMetabolic3::RECOVERY_1]
                       + tNRecovery2  * tArticle->mCO2ProductionRate[GunnsFluidMetabolic3::RECOVERY_2]
                       + tNRecovery3  * tArticle->mCO2ProductionRate[GunnsFluidMetabolic3::RECOVERY_3]
                       + tNAerobic0   * tArticle->mCO2ProductionRate[GunnsFluidMetabolic3::AEROBIC_0]
                       + tNAerobic1   * tArticle->mCO2ProductionRate[GunnsFluidMetabolic3::AEROBIC_1]
                       + tNResistive0 * tArticle->mCO2ProductionRate[GunnsFluidMetabolic3::RESISTIVE_0]
                       + tNResistive1 * tArticle->mCO2ProductionRate[GunnsFluidMetabolic3::RESISTIVE_1]
                       + tNResistive2 * tArticle->mCO2ProductionRate[GunnsFluidMetabolic3::RESISTIVE_2]
                       + tNResistive3 * tArticle->mCO2ProductionRate[GunnsFluidMetabolic3::RESISTIVE_3];

    double expectedH2O = tNNominal    * tArticle->mH2OProductionRate[GunnsFluidMetabolic3::NOMINAL]
                       + tNSleep      * tArticle->mH2OProductionRate[GunnsFluidMetabolic3::SLEEP]
                       + tNRecovery0  * tArticle->mH2OProductionRate[GunnsFluidMetabolic3::RECOVERY_0]
                       + tNRecovery1  * tArticle->mH2OProductionRate[GunnsFluidMetabolic3::RECOVERY_1]
                       + tNRecovery2  * tArticle->mH2OProductionRate[GunnsFluidMetabolic3::RECOVERY_2]
                       + tNRecovery3  * tArticle->mH2OProductionRate[GunnsFluidMetabolic3::RECOVERY_3]
                       + tNAerobic0   * tArticle->mH2OProductionRate[GunnsFluidMetabolic3::AEROBIC_0]
                       + tNAerobic1   * tArticle->mH2OProductionRate[GunnsFluidMetabolic3::AEROBIC_1]
                       + tNResistive0 * tArticle->mH2OProductionRate[GunnsFluidMetabolic3::RESISTIVE_0]
                       + tNResistive1 * tArticle->mH2OProductionRate[GunnsFluidMetabolic3::RESISTIVE_1]
                       + tNResistive2 * tArticle->mH2OProductionRate[GunnsFluidMetabolic3::RESISTIVE_2]
                       + tNResistive3 * tArticle->mH2OProductionRate[GunnsFluidMetabolic3::RESISTIVE_3];

    double totalCrew = tNNominal + tNSleep + tNRecovery0 + tNRecovery1
                     + tNRecovery2 + tNRecovery3 + tNAerobic0 + tNAerobic1
                     + tNResistive0 + tNResistive1 + tNResistive2 + tNResistive3;

    double expectedNH3    = totalCrew * tArticle->mNH3ProductionRate;
    double expectedCO     = totalCrew * tArticle->mCOProductionRate;
    double expectedH2     = totalCrew * tArticle->mH2ProductionRate;
    double expectedCH4    = totalCrew * tArticle->mCH4ProductionRate;
    double expectedCH4O   = totalCrew * tArticle->mCH4OProductionRate;
    double expectedC2H6O  = totalCrew * tArticle->mC2H6OProductionRate;
    double expectedC4H10O = totalCrew * tArticle->mC4H10OProductionRate;
    double expectedCH2O   = totalCrew * tArticle->mCH2OProductionRate;
    double expectedC2H4O  = totalCrew * tArticle->mC2H4OProductionRate;
    double expectedC6H6   = totalCrew * tArticle->mC6H6ProductionRate;
    double expectedC7H8   = totalCrew * tArticle->mC7H8ProductionRate;
    double expectedC8H10  = totalCrew * tArticle->mC8H10ProductionRate;
    double expectedCH2CL2 = totalCrew * tArticle->mCH2CL2ProductionRate;
    double expectedC3H6O  = totalCrew * tArticle->mC3H6OProductionRate;
    double expectedFlow   = expectedCO2 + expectedH2O - expectedO2;

    tArticle->step(0.1);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedO2,     tArticle->mConsumedO2,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedQ,      tArticle->mProducedHeat,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCO2,    tArticle->mProducedCO2,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH2O,    tArticle->mProducedH2O,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNH3,    tArticle->mProducedNH3,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCO,     tArticle->mProducedCO,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH2,     tArticle->mProducedH2,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCH4,    tArticle->mProducedCH4,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCH4O,   tArticle->mProducedCH4O,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedC2H6O,  tArticle->mProducedC2H6O,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedC4H10O, tArticle->mProducedC4H10O, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCH2O,   tArticle->mProducedCH2O,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedC2H4O,  tArticle->mProducedC2H4O,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedC6H6,   tArticle->mProducedC6H6,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedC7H8,   tArticle->mProducedC7H8,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedC8H10,  tArticle->mProducedC8H10,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCH2CL2, tArticle->mProducedCH2CL2, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedC3H6O,  tArticle->mProducedC3H6O,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlow,   tArticle->mFlowDemand,     DBL_EPSILON);

    /// @test when NH3, CO, H2, CH4 are fluids and O2 limited to available mass in the node
    tArticle->mNH3 = 0;
    tArticle->mCO  = 0;
    tArticle->mH2  = 0;
    tArticle->mCH4 = 0;
    tNodes[1].initVolume(0.0);
    expectedFlow += expectedNH3 + expectedCO + expectedH2 + expectedCH4 + expectedO2;

    tArticle->step(0.1);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mConsumedO2,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNH3,    tArticle->mProducedNH3,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCO,     tArticle->mProducedCO,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH2,     tArticle->mProducedH2,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCH4,    tArticle->mProducedCH4,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlow,   tArticle->mFlowDemand,     DBL_EPSILON);

    /// @test zero total flow rate
    for (unsigned int i=0; i<GunnsFluidMetabolic3::NO_METABOLIC; ++i) {
        tArticle->mNCrew[i] = 0.0;
    }
    tArticle->step(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mFlowDemand,     DBL_EPSILON);

    /// @test zero time step
    tArticle->step(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mFlowDemand,     DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Metabolic link model update fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMetabolic3::testUpdateFluid()
{
    UT_RESULT;

    tArticle->initialize(*tConfigData, *tInputData, tLinks, 2, 1);

    const double timestep = 0.1;
    tArticle->step(timestep);
    tArticle->computeFlows(timestep);
    tArticle->transportFlows(timestep);

    /// @test nominal fluid flow added to the node
    PolyFluid* internal      = tArticle->mInternalFluid;
    PolyFluid* content       = tNodes[1].getContent();
    PolyFluid* inFlow        = tNodes[1].getInflow();
    double     expectedNodeQ = tArticle->mProducedHeat
                             + tArticle->mFlowDemand * internal->getSpecificEnthalpy();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(content->getTemperature(), internal->getTemperature(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNodeQ,             tNodes[1].mInflowHeatFlux,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mFlowDemand,     tNodes[1].getInflux(),      DBL_EPSILON);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mProducedH2O,
                                 tNodes[1].getInflux() * inFlow->getMassFraction(FluidProperties::GUNNS_H2O),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mProducedCO2,
                                 tNodes[1].getInflux() * inFlow->getMassFraction(FluidProperties::GUNNS_CO2),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mConsumedO2,
                                -tNodes[1].getInflux() * inFlow->getMassFraction(FluidProperties::GUNNS_O2),
                                 DBL_EPSILON);

    verifyTraceCompoundInFlow(tArticle->mProducedCH4O,   ChemicalCompound::CH4O);
    verifyTraceCompoundInFlow(tArticle->mProducedC2H6O,  ChemicalCompound::C2H6O);
    verifyTraceCompoundInFlow(tArticle->mProducedC4H10O, ChemicalCompound::C4H10O);
    verifyTraceCompoundInFlow(tArticle->mProducedCH2O,   ChemicalCompound::CH2O);
    verifyTraceCompoundInFlow(tArticle->mProducedC2H4O,  ChemicalCompound::C2H4O);
    verifyTraceCompoundInFlow(tArticle->mProducedC6H6,   ChemicalCompound::C6H6);
    verifyTraceCompoundInFlow(tArticle->mProducedC7H8,   ChemicalCompound::C7H8);
    verifyTraceCompoundInFlow(tArticle->mProducedC8H10,  ChemicalCompound::C8H10);
    verifyTraceCompoundInFlow(tArticle->mProducedCH2CL2, ChemicalCompound::CH2CL2);
    verifyTraceCompoundInFlow(tArticle->mProducedC3H6O,  ChemicalCompound::C3H6O);
    verifyTraceCompoundInFlow(tArticle->mProducedNH3,    ChemicalCompound::NH3);
    verifyTraceCompoundInFlow(tArticle->mProducedCO,     ChemicalCompound::CO);
    verifyTraceCompoundInFlow(tArticle->mProducedH2,     ChemicalCompound::H2);
    verifyTraceCompoundInFlow(tArticle->mProducedCH4,    ChemicalCompound::CH4);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  mdot  (--)  Expected mass flow of the given trace compound.
/// @param[in]  type  (--)  Trace compound type to verify.
///
/// @details  Verifies the given trace compound's mass and mole fraction in the node's inflow fluid
///           object.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMetabolic3::verifyTraceCompoundInFlow(const double                 mdot,
                                                       const ChemicalCompound::Type type)
{
    const PolyFluid*                inFlow     = tNodes[1].getInflow();
    const GunnsFluidTraceCompounds* inTc       = inFlow->getTraceCompounds();
    const double                    inMoleRate = tNodes[1].getInflux() / inFlow->getMWeight();
    const double                    MW         = tCompoundProperties->getCompound(type)->mMWeight;
    const double                    mole       = mdot / MW / inMoleRate;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mdot, inTc->getMass(type),         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mole, inTc->getMoleFraction(type), DBL_EPSILON);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Metabolic link model update fluid with no trace compounds.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMetabolic3::testUpdateFluidNoTc()
{
    UT_RESULT;

    /// - Define a new fluid config that adds 4 trace compounds as fluid types instead.
    FluidProperties::FluidType types[8];
    double                     fractions[8];
    types[0]         = FluidProperties::GUNNS_N2;  fractions[0] = 0.750;
    types[1]         = FluidProperties::GUNNS_O2;  fractions[1] = 0.200;
    types[2]         = FluidProperties::GUNNS_H2O; fractions[2] = 0.035;
    types[3]         = FluidProperties::GUNNS_CO2; fractions[3] = 0.010;
    types[4]         = FluidProperties::GUNNS_NH3; fractions[4] = 0.001;
    types[5]         = FluidProperties::GUNNS_CO;  fractions[5] = 0.001;
    types[6]         = FluidProperties::GUNNS_H2;  fractions[6] = 0.001;
    types[7]         = FluidProperties::GUNNS_CH4; fractions[7] = 0.002;

    delete tFluidConfig;
    tFluidConfig     = new PolyFluidConfigData(tFluidProperties, types, 8);
    delete tFluidInput;
    tFluidInput      = new PolyFluidInputData(294.261, 101.325, 1.0, 0.0, fractions);

    FriendlyGunnsFluidNodeMetabolic3 nodes[3];
    tNodeList.mNodes = nodes;

    /// - Initialize the node fluids
    nodes[0].initialize("UtNode1", tFluidConfig);
    nodes[1].initialize("UtNode2", tFluidConfig);
    nodes[2].initialize("UtNode3", tFluidConfig);
    nodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput);
    nodes[1].getContent()->initialize(*tFluidConfig, *tFluidInput);
    nodes[2].getContent()->initialize(*tFluidConfig, *tFluidInput);
    nodes[1].initVolume(10.0);

    tArticle->initialize(*tConfigData, *tInputData, tLinks, 2, 1);

    const double timestep = 0.1;
    tArticle->step(timestep);
    tArticle->computeFlows(timestep);
    tArticle->transportFlows(timestep);

    /// @test nominal fluid flow added to the node
    PolyFluid* internal      = tArticle->mInternalFluid;
    PolyFluid* content       = nodes[1].getContent();
    PolyFluid* inFlow        = nodes[1].getInflow();
    double     expectedNodeQ = tArticle->mProducedHeat
                             + tArticle->mFlowDemand * internal->getSpecificEnthalpy();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(content->getTemperature(), internal->getTemperature(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNodeQ,             nodes[1].mInflowHeatFlux,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mFlowDemand,     nodes[1].getInflux(),      DBL_EPSILON);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mProducedH2O,
                                 nodes[1].getInflux() * inFlow->getMassFraction(FluidProperties::GUNNS_H2O),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mProducedCO2,
                                 nodes[1].getInflux() * inFlow->getMassFraction(FluidProperties::GUNNS_CO2),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mConsumedO2,
                                -nodes[1].getInflux() * inFlow->getMassFraction(FluidProperties::GUNNS_O2),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mProducedNH3,
                                 nodes[1].getInflux() * inFlow->getMassFraction(FluidProperties::GUNNS_NH3),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mProducedCO,
                                 nodes[1].getInflux() * inFlow->getMassFraction(FluidProperties::GUNNS_CO),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mProducedH2,
                                 nodes[1].getInflux() * inFlow->getMassFraction(FluidProperties::GUNNS_H2),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mProducedCH4,
                                 nodes[1].getInflux() * inFlow->getMassFraction(FluidProperties::GUNNS_CH4),
                                 DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Metabolic link model specific port mapping rules.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMetabolic3::testPortMapping()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, 2, 1);

    /// @test    Fail if port 1 is the vacuum boundary node.
    CPPUNIT_ASSERT(!tArticle->checkSpecificPortRules(1,2));

    /// @test    Fail if port 0 is not the vacuum boundary node.
    CPPUNIT_ASSERT(!tArticle->checkSpecificPortRules(0,1));

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Metabolic link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMetabolic3::testInitializationExceptions()
{
    UT_RESULT;
    {
        /// @test for exception on negative number of crew members in nominal state.
        GunnsFluidMetabolic3 article;
        tInputData->mNNominal = -0.1;
        CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, 2, 1),
                             TsInitializationException);
        tInputData->mNNominal = tNNominal;
    }  {
        /// @test for exception on negative number of crew members in sleep state.
        GunnsFluidMetabolic3 article;
        tInputData->mNSleep = -0.1;
        CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, 2, 1),
                             TsInitializationException);
        tInputData->mNSleep = tNSleep;
    } {
        /// @test for exception on negative number of crew members in recovery_0 state.
        GunnsFluidMetabolic3 article;
        tInputData->mNRecovery0 = -0.1;
        CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, 2, 1),
                             TsInitializationException);
        tInputData->mNRecovery0 = tNRecovery0;
    } {
        /// @test for exception on negative number of crew members in recovery_1 state.
        GunnsFluidMetabolic3 article;
        tInputData->mNRecovery1 = -0.1;
        CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, 2, 1),
                             TsInitializationException);
        tInputData->mNRecovery1 = tNRecovery1;
    } {
        /// @test for exception on negative number of crew members in recovery_2 state.
        GunnsFluidMetabolic3 article;
        tInputData->mNRecovery2 = -0.1;
        CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, 2, 1),
                             TsInitializationException);
        tInputData->mNRecovery2 = tNRecovery2;
    } {
        /// @test for exception on negative number of crew members in recovery_3 state.
        GunnsFluidMetabolic3 article;
        tInputData->mNRecovery3 = -0.1;
        CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, 2, 1),
                             TsInitializationException);
        tInputData->mNRecovery3 = tNRecovery3;
    } {
        /// @test for exception on negative number of crew members in aerobic_0 state.
        GunnsFluidMetabolic3 article;
        tInputData->mNAerobic0 = -0.1;
        CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, 2, 1),
                             TsInitializationException);
        tInputData->mNAerobic0 = tNAerobic0;
    } {
        /// @test for exception on negative number of crew members in aerobic_1 state.
        GunnsFluidMetabolic3 article;
        tInputData->mNAerobic1 = -0.1;
        CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, 2, 1),
                             TsInitializationException);
        tInputData->mNAerobic1 = tNAerobic1;
    } {
        /// @test for exception on negative number of crew members in resistive_0 state.
        GunnsFluidMetabolic3 article;
        tInputData->mNResistive0 = -0.1;
        CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, 2, 1),
                             TsInitializationException);
        tInputData->mNResistive0 = tNResistive0;
    } {
        /// @test for exception on negative number of crew members in resistive_1 state.
        GunnsFluidMetabolic3 article;
        tInputData->mNResistive1 = -0.1;
        CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, 2, 1),
                             TsInitializationException);
        tInputData->mNResistive1 = tNResistive1;
    } {
        /// @test for exception on negative number of crew members in resistive_2 state.
        GunnsFluidMetabolic3 article;
        tInputData->mNResistive2 = -0.1;
        CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, 2, 1),
                             TsInitializationException);
        tInputData->mNResistive2 = tNResistive2;
    } {
        /// @test for exception on negative number of crew members in resistive_3 state.
        GunnsFluidMetabolic3 article;
        tInputData->mNResistive3 = -0.1;
        CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, 2, 1),
                             TsInitializationException);
        tInputData->mNResistive3 = tNResistive3;
    } {
        /// @test for exception on O2 not a network fluid.
        GunnsFluidMetabolic3 article;
        tFluidConfig->mTypes[1] = FluidProperties::GUNNS_NH3;

        FriendlyGunnsFluidNodeMetabolic3 nodes[3];
        tNodeList.mNodes = nodes;

        nodes[0].initialize("UtNode1", tFluidConfig);
        nodes[1].initialize("UtNode2", tFluidConfig);
        nodes[2].initialize("UtNode3", tFluidConfig);
        nodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput);
        nodes[1].getContent()->initialize(*tFluidConfig, *tFluidInput);
        nodes[2].getContent()->initialize(*tFluidConfig, *tFluidInput);

        CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, 2, 1),
                             TsOutOfBoundsException);
        tFluidConfig->mTypes[1] = FluidProperties::GUNNS_O2;
    } {
        /// @test for exception on CO2 not a network fluid.
        GunnsFluidMetabolic3 article;
        tFluidConfig->mTypes[3] = FluidProperties::GUNNS_NH3;

        FriendlyGunnsFluidNodeMetabolic3 nodes[3];
        tNodeList.mNodes = nodes;

        nodes[0].initialize("UtNode1", tFluidConfig);
        nodes[1].initialize("UtNode2", tFluidConfig);
        nodes[2].initialize("UtNode3", tFluidConfig);
        nodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput);
        nodes[1].getContent()->initialize(*tFluidConfig, *tFluidInput);
        nodes[2].getContent()->initialize(*tFluidConfig, *tFluidInput);

        CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, 2, 1),
                             TsOutOfBoundsException);
        tFluidConfig->mTypes[3] = FluidProperties::GUNNS_CO2;
    } {
        /// @test for exception on H2O not a network fluid.
        GunnsFluidMetabolic3 article;
        tFluidConfig->mTypes[2] = FluidProperties::GUNNS_NH3;

        FriendlyGunnsFluidNodeMetabolic3 nodes[3];
        tNodeList.mNodes = nodes;

        nodes[0].initialize("UtNode1", tFluidConfig);
        nodes[1].initialize("UtNode2", tFluidConfig);
        nodes[2].initialize("UtNode3", tFluidConfig);
        nodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput);
        nodes[1].getContent()->initialize(*tFluidConfig, *tFluidInput);
        nodes[2].getContent()->initialize(*tFluidConfig, *tFluidInput);

        CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, 2, 1),
                             TsOutOfBoundsException);
        tFluidConfig->mTypes[2] = FluidProperties::GUNNS_H2O;
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Metabolic link model restart.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMetabolic3::testRestart()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, 2, 1);
    tArticle->step(0.1);
    tArticle->restart();

    CPPUNIT_ASSERT(0.0 == tArticle->mConsumedO2);
    CPPUNIT_ASSERT(0.0 == tArticle->mProducedCO2);
    CPPUNIT_ASSERT(0.0 == tArticle->mProducedH2O);
    CPPUNIT_ASSERT(0.0 == tArticle->mProducedHeat);
    CPPUNIT_ASSERT(0.0 == tArticle->mProducedCH4O);
    CPPUNIT_ASSERT(0.0 == tArticle->mProducedC2H6O);
    CPPUNIT_ASSERT(0.0 == tArticle->mProducedC4H10O);
    CPPUNIT_ASSERT(0.0 == tArticle->mProducedCH2O);
    CPPUNIT_ASSERT(0.0 == tArticle->mProducedC2H4O);
    CPPUNIT_ASSERT(0.0 == tArticle->mProducedC6H6);
    CPPUNIT_ASSERT(0.0 == tArticle->mProducedC7H8);
    CPPUNIT_ASSERT(0.0 == tArticle->mProducedC8H10);
    CPPUNIT_ASSERT(0.0 == tArticle->mProducedCH2CL2);
    CPPUNIT_ASSERT(0.0 == tArticle->mProducedC3H6O);
    CPPUNIT_ASSERT(0.0 == tArticle->mProducedNH3);
    CPPUNIT_ASSERT(0.0 == tArticle->mProducedCO);
    CPPUNIT_ASSERT(0.0 == tArticle->mProducedH2);
    CPPUNIT_ASSERT(0.0 == tArticle->mProducedCH4);

    UT_PASS_LAST;
}
