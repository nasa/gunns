/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
(
   (aspects/fluid/capacitor/GunnsFluidAccumGas.o)
   (software/exceptions/TsInitializationException.o)
)
***************************************************************************************************/

#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidAccumGas.hh"

/// @details  Test identification number.
int UtGunnsFluidAccumGas::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsFluidAccumGas class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidAccumGas::UtGunnsFluidAccumGas()
    :
    tFluidProperties(),
    tAccumFluidConfigData(),
    tLiquidFluidInputData(),
    tGasFluidInputData(),
    tMassFractions(),
    tGasMassFractions(),
    tConfigData(),
    tInputData(),
    tModel(),
    tLinkName(),
    tMaxConductivity(),
    tMinConductivityScale(),
    tGasMaxConductivity(),
    tGasMinConductivity(),
    tExpansionScaleFactor(),
    tAccumVolume(),
    tNodes(),
    tNodeList(),
    tLinks(),
    tPort0(),
    tPort1(),
    tTimeStep(),
    tTolerance(),
    tInitialBellowsPosition(),
    tInitialPressure(),
    tInitialTemperature(),
    tMinChamberVolPercent(),
    tMinChamberVolDeadBandPercent(),
    tForceBellowsMaxRate(),
    tEditHoldTime(),
    tMinTemperature(),
    tMaxTemperature(),
    tMaxPressure(),
    tMaxForcePressureRate(),
    tSpringCoeff0(),
    tSpringCoeff1(),
    tSpringCoeff2(),
    tFillModePressureThreshold(),
    tEffCondScaleRate()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsFluidAccumGas class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidAccumGas::~UtGunnsFluidAccumGas()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccumGas::tearDown()
{
    /// - Deletes for news in setUp
    delete tModel;
    delete tInputData;
    delete tConfigData;
    delete tLiquidFluidInputData;
    delete tGasFluidInputData;
    delete tAccumFluidConfigData;
    delete [] tMassFractions;
    delete [] tGasMassFractions;
    delete tFluidProperties;

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccumGas::setUp()
{
    tLinkName             = "Test Accumulator";
    tMaxConductivity = 1.0;
    tGasMaxConductivity = 1.0;
    tGasMinConductivity = 1.0E-09;
    tMaxPressure = 600.0;
    tMaxForcePressureRate = 30.0;
    tMinChamberVolPercent = 10.0;
    tMinChamberVolDeadBandPercent = 15.0;
    tForceBellowsMaxRate = 1.0 / UnitConversion::SEC_PER_MIN;
    tExpansionScaleFactor = 0.0;
    tAccumVolume = 0.0156;
    tNodeList.mNumNodes = 3;
    tNodeList.mNodes = tNodes;
    tPort0                = 0;
    tPort1                = 1;
    tTimeStep             = 0.1;
    tTolerance            = 1.0E-06;
    tEditHoldTime = 5.0;
    tMinTemperature = 274.0;
    tMaxTemperature = 373.0;
    tMinConductivityScale = 0.1;
    tSpringCoeff0 = -50.0;
    tSpringCoeff1 = 100.0;
    tSpringCoeff2 = 0.0;
    tFillModePressureThreshold = 0.0;
    tEffCondScaleRate = 0.0;
//    tBellowsNeutralPos = 0.5;
//    tMinSpringPressure = 0.0;
//    tMaxSpringPressure = 10.0;

    /// - Setup some fluid nodes
    tFluidProperties = new DefinedFluidProperties();
    FluidProperties::FluidType types[3];
    types[0]    = FluidProperties::GUNNS_WATER;
    types[1]    = FluidProperties::GUNNS_N2;
    types[2]    = FluidProperties::GUNNS_O2;
    tMassFractions = new double[3];
    tMassFractions[0] = 1.0;
    tMassFractions[1] = 0.0;
    tMassFractions[2] = 0.0;
    tGasMassFractions = new double[3];
    tGasMassFractions[0] = 0.0;
    tGasMassFractions[1] = 1.0;
    tGasMassFractions[2] = 0.0;
    tInitialBellowsPosition = 0.5;
    tInitialPressure = 200.0;
    tInitialTemperature = 290.0;

    tAccumFluidConfigData = new PolyFluidConfigData(tFluidProperties, types, 3);

    tLiquidFluidInputData = new PolyFluidInputData(tInitialTemperature,                    //temperature
                                                   tInitialPressure,         //pressure
                                                   0.0,                      //flowRate
                                                   0.0,                      //mass
                                                   tMassFractions);          //massFractions

    tGasFluidInputData = new PolyFluidInputData(tInitialTemperature,                    //temperature
                                                tInitialPressure,         //pressure
                                                0.0,                      //flowRate
                                                0.0,                      //mass
                                                tGasMassFractions);       //massFractions

    /// - Have to initialize the nodes with the fluid configs (normally done by GUNNS)
    /// - Set both to liquid (same as GUNNS will do), even though one is gas.
    /// - The accumulator will overwrite in initialize.
    tNodes[0].initialize("Node0", tAccumFluidConfigData, tGasFluidInputData);
    tNodes[1].initialize("Node1", tAccumFluidConfigData, tLiquidFluidInputData);
    tNodes[2].initialize("Node2", tAccumFluidConfigData, 0);

    tNodes[0].setPotential(200.0);
    tNodes[1].setPotential(200.0);
    tNodes[2].setPotential(0.0);

    /// - Set both to liquid (same as GUNNS will do), even though one is gas.
    /// - The accumulator will overwrite in initialize.
    tNodes[0].getContent()->initialize(*tAccumFluidConfigData, *tGasFluidInputData);
    tNodes[1].getContent()->initialize(*tAccumFluidConfigData, *tLiquidFluidInputData);

    tConfigData = new GunnsFluidAccumGasConfigData (
            tLinkName,
            &tNodeList,
            tMaxConductivity,
            tMinConductivityScale,
            tAccumVolume,
            tMinChamberVolPercent,
            tMinChamberVolDeadBandPercent,
            tForceBellowsMaxRate,
            tEditHoldTime,
            tMinTemperature,
            tMaxTemperature,
            tMaxPressure,
            tSpringCoeff0,
            tSpringCoeff1,
            tSpringCoeff2,
            tGasMaxConductivity,
            tGasMinConductivity,
            tMaxForcePressureRate,
            tFillModePressureThreshold,
            tEffCondScaleRate);

    tInputData = new GunnsFluidAccumGasInputData (
            false, 0.0, tInitialBellowsPosition, tLiquidFluidInputData, tGasFluidInputData);

    tModel = new FriendlyGunnsFluidAccumGas;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccumGas::testConfig()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data default construction.
    GunnsFluidAccumGasConfigData defaultConfig;
    CPPUNIT_ASSERT("" == defaultConfig.mName);
    CPPUNIT_ASSERT(0 == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mMaxConductivity);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mMinConductivityScale);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mAccumVolume);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mMinChamberVolPercent);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mMinChamberVolDeadBandPercent);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mForceBellowsMaxRate);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mEditHoldTime);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mMinTemperature);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mMaxTemperature);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mMaxPressure);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mSpringCoeff0);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mSpringCoeff1);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mSpringCoeff2);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mFillModePressureThreshold);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mEffCondScaleOneWayRate);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mGasMaxConductivity);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mGasMinConductivity);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mMaxForcePressureRate);

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(tLinkName == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductivity == tConfigData->mMaxConductivity);
    CPPUNIT_ASSERT(tMinConductivityScale == tConfigData->mMinConductivityScale);
    CPPUNIT_ASSERT(tAccumVolume == tConfigData->mAccumVolume);
    CPPUNIT_ASSERT(tMinChamberVolPercent == tConfigData->mMinChamberVolPercent);
    CPPUNIT_ASSERT(tMinChamberVolDeadBandPercent == tConfigData->mMinChamberVolDeadBandPercent);
    CPPUNIT_ASSERT(tForceBellowsMaxRate == tConfigData->mForceBellowsMaxRate);
    CPPUNIT_ASSERT(tEditHoldTime == tConfigData->mEditHoldTime);
    CPPUNIT_ASSERT(tMinTemperature == tConfigData->mMinTemperature);
    CPPUNIT_ASSERT(tMaxTemperature == tConfigData->mMaxTemperature);
    CPPUNIT_ASSERT(tMaxPressure == tConfigData->mMaxPressure);
    CPPUNIT_ASSERT(tSpringCoeff0 == tConfigData->mSpringCoeff0);
    CPPUNIT_ASSERT(tSpringCoeff1 == tConfigData->mSpringCoeff1);
    CPPUNIT_ASSERT(tSpringCoeff2 == tConfigData->mSpringCoeff2);
    CPPUNIT_ASSERT(tFillModePressureThreshold == tConfigData->mFillModePressureThreshold);
    CPPUNIT_ASSERT(tEffCondScaleRate == tConfigData->mEffCondScaleOneWayRate);
    CPPUNIT_ASSERT(tGasMaxConductivity == tConfigData->mGasMaxConductivity);
    CPPUNIT_ASSERT(tGasMinConductivity == tConfigData->mGasMinConductivity);
    CPPUNIT_ASSERT(tMaxForcePressureRate == tConfigData->mMaxForcePressureRate);

    /// @test    Configuration data copy construction.
    GunnsFluidAccumGasConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tLinkName == copyConfig.mName);
    CPPUNIT_ASSERT(tNodes == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductivity == copyConfig.mMaxConductivity);
    CPPUNIT_ASSERT(tMinConductivityScale == copyConfig.mMinConductivityScale);
    CPPUNIT_ASSERT(tAccumVolume == copyConfig.mAccumVolume);
    CPPUNIT_ASSERT(tMinChamberVolPercent == copyConfig.mMinChamberVolPercent);
    CPPUNIT_ASSERT(tMinChamberVolDeadBandPercent == copyConfig.mMinChamberVolDeadBandPercent);
    CPPUNIT_ASSERT(tForceBellowsMaxRate == copyConfig.mForceBellowsMaxRate);
    CPPUNIT_ASSERT(tEditHoldTime == copyConfig.mEditHoldTime);
    CPPUNIT_ASSERT(tMinTemperature == copyConfig.mMinTemperature);
    CPPUNIT_ASSERT(tMaxTemperature == copyConfig.mMaxTemperature);
    CPPUNIT_ASSERT(tMaxPressure == copyConfig.mMaxPressure);
    CPPUNIT_ASSERT(tSpringCoeff0 == copyConfig.mSpringCoeff0);
    CPPUNIT_ASSERT(tSpringCoeff1 == copyConfig.mSpringCoeff1);
    CPPUNIT_ASSERT(tSpringCoeff2 == copyConfig.mSpringCoeff2);
    CPPUNIT_ASSERT(tFillModePressureThreshold == copyConfig.mFillModePressureThreshold);
    CPPUNIT_ASSERT(tEffCondScaleRate == copyConfig.mEffCondScaleOneWayRate);
    CPPUNIT_ASSERT(tGasMaxConductivity == copyConfig.mGasMaxConductivity);
    CPPUNIT_ASSERT(tGasMinConductivity == copyConfig.mGasMinConductivity);
    CPPUNIT_ASSERT(tMaxForcePressureRate == copyConfig.mMaxForcePressureRate);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccumGas::testInput()
{
    UT_RESULT;

    /// @test    Input data default construction.
    GunnsFluidAccumGasInputData defaultInput;
    CPPUNIT_ASSERT(false == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0 == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0 == defaultInput.mInitialBellowsPosition);
    CPPUNIT_ASSERT(0 == defaultInput.mLiquidFluidInputData);
    CPPUNIT_ASSERT(0 == defaultInput.mGasFluidInputData);

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(false == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0 == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tInitialBellowsPosition == tInputData->mInitialBellowsPosition);
    CPPUNIT_ASSERT(tLiquidFluidInputData->mPressure == tInputData->mLiquidFluidInputData->mPressure);
    CPPUNIT_ASSERT(tGasFluidInputData->mPressure == tInputData->mGasFluidInputData->mPressure);

    /// @test    Input data copy construction.
    GunnsFluidAccumGasInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageValue == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(tInputData->mInitialBellowsPosition == copyInput.mInitialBellowsPosition);
    CPPUNIT_ASSERT(tLiquidFluidInputData->mPressure == copyInput.mLiquidFluidInputData->mPressure);
    CPPUNIT_ASSERT(tGasFluidInputData->mPressure == copyInput.mGasFluidInputData->mPressure);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccumGas::testDefaultConstruction()
{
    UT_RESULT;

    /// @test default construction.
    CPPUNIT_ASSERT(0.0 == tModel->mGasMaxConductivity);
    CPPUNIT_ASSERT(0.0 == tModel->mGasMinConductivity);
    CPPUNIT_ASSERT(0.0 == tModel->mGasEffectiveConductivity);
    CPPUNIT_ASSERT(0.0 == tModel->mGasCapacitance);
    CPPUNIT_ASSERT(0.0 == tModel->mGasPressureReading);
    CPPUNIT_ASSERT(0.0 == tModel->mGasFlowRate);
    CPPUNIT_ASSERT(0.0 == tModel->mGasFlux);
    CPPUNIT_ASSERT(0.0 == tModel->mGasHousingQ);
    CPPUNIT_ASSERT(false == tModel->mEditPressureFlag),
    CPPUNIT_ASSERT(0.0 == tModel->mEditPressureValue);
    CPPUNIT_ASSERT(0.0 == tModel->mEditPressureRate);
    CPPUNIT_ASSERT(0.0 == tModel->mEditPressureTimer);
    CPPUNIT_ASSERT(0 == tModel->mGasInternalFluid);
    CPPUNIT_ASSERT(0.0 == tModel->mMalfBellowsRupturePress);
    CPPUNIT_ASSERT(0.0 == tModel->mMalfBellowsRupturePressRate);
    CPPUNIT_ASSERT(0.0 == tModel->mMaxForcePressureRate);

    /// @test init flag
    CPPUNIT_ASSERT(!tModel->mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccumGas::testNominalInitialization()
{
    UT_RESULT;

    /// - Default construct and initialize test model with nominal data.
    FriendlyGunnsFluidAccumGas accumModel;
    accumModel.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Verify inherited model was called.
    CPPUNIT_ASSERT(tMaxConductivity == accumModel.mMaxConductivity);

    /// @test config and input data
    CPPUNIT_ASSERT(tGasMaxConductivity ==  accumModel.mGasMaxConductivity);
    CPPUNIT_ASSERT(tGasMinConductivity ==  accumModel.mGasMinConductivity);
    CPPUNIT_ASSERT(tGasMaxConductivity == accumModel.mGasEffectiveConductivity);
    CPPUNIT_ASSERT(0.0 == accumModel.mGasCapacitance);
    CPPUNIT_ASSERT(tNodes[0].getPotential() == accumModel.mGasPressureReading);
    CPPUNIT_ASSERT(0.0 == accumModel.mGasFlowRate);
    CPPUNIT_ASSERT(0.0 == accumModel.mGasFlux);
    CPPUNIT_ASSERT(0.0 == accumModel.mGasHousingQ);

    /// - Verify Edit attributes
    CPPUNIT_ASSERT(false == accumModel.mEditPressureFlag);
    CPPUNIT_ASSERT(0.0 == accumModel.mEditPressureValue);
    CPPUNIT_ASSERT(tMaxForcePressureRate == accumModel.mEditPressureRate);
    CPPUNIT_ASSERT(0.0 == accumModel.mEditPressureTimer);

    /// - Verify fluid attributes
    CPPUNIT_ASSERT(tInputData->mGasFluidInputData->mMassFraction[0] == accumModel.mGasInternalFluid->getMassFraction(0));
    CPPUNIT_ASSERT(tInputData->mGasFluidInputData->mMassFraction[1] == accumModel.mGasInternalFluid->getMassFraction(1));
    CPPUNIT_ASSERT(tInputData->mGasFluidInputData->mPressure == accumModel.mGasInternalFluid->getPressure());
    CPPUNIT_ASSERT(tInputData->mGasFluidInputData->mTemperature == accumModel.mGasInternalFluid->getTemperature());
    CPPUNIT_ASSERT(accumModel.mPressurizerVolume * accumModel.mGasInternalFluid->getDensity() == accumModel.mGasInternalFluid->getMass());
    CPPUNIT_ASSERT(0.0 == accumModel.mGasInternalFluid->getFlowRate());

    /// - Verify that the non-capacitive node controlled by the gas side is set to gas
    ///   (normally, the node is defaulted to the first constuent in the list, which has
    ///    to be the liquid.)
    CPPUNIT_ASSERT(0.0 == tNodes[0].getContent()->getMassFraction(0));
    CPPUNIT_ASSERT(1.0 == tNodes[0].getContent()->getMassFraction(1));
    CPPUNIT_ASSERT(tInitialPressure == tNodes[0].getContent()->getPressure());
    CPPUNIT_ASSERT(tInitialTemperature == tNodes[0].getContent()->getTemperature());

    /// - Verify malfunctions
    CPPUNIT_ASSERT(0.0 == accumModel.mMalfBellowsRupturePress);
    CPPUNIT_ASSERT(tMaxForcePressureRate == accumModel.mMalfBellowsRupturePressRate);

    /// - Verify maximums
    CPPUNIT_ASSERT(tMaxPressure == accumModel.mMaxPressure);
    CPPUNIT_ASSERT(tMaxForcePressureRate == accumModel.mMaxForcePressureRate);

    /// @test init flag
    CPPUNIT_ASSERT(accumModel.mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for initialization exceptions
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccumGas::testInitializationExceptions()
{
    UT_RESULT;

    double tempVal = 0.0;

    /// - Default construct a local test model.
    FriendlyGunnsFluidAccumGas model;

    /// @test   Exception on mGasMaxConductivity < 0.0
    tempVal = tConfigData->mGasMaxConductivity;
    tConfigData->mGasMaxConductivity = -0.5;
    CPPUNIT_ASSERT_THROW(model.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mGasMaxConductivity = tempVal;

    /// @test   Exception on mGasMinConductivity < DBL_EPSILON
    tempVal = tConfigData->mGasMinConductivity;
    tConfigData->mGasMinConductivity = 0.0;
    CPPUNIT_ASSERT_THROW(model.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mGasMinConductivity = tempVal;

    /// @test   Exception on mMaxForcePressureRate <= 0.0
    tempVal = tConfigData->mMaxForcePressureRate;
    tConfigData->mMaxForcePressureRate = 0.0;
    CPPUNIT_ASSERT_THROW(model.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mMaxForcePressureRate = tempVal;

    /// @test   Exception on mGasFluidInputData == NULL
    PolyFluidInputData* tempFluidInput;
    tempFluidInput = tInputData->mGasFluidInputData;
    tInputData->mGasFluidInputData = 0;
    CPPUNIT_ASSERT_THROW(model.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tInputData->mGasFluidInputData = tempFluidInput;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for step method
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccumGas::testStep()
{
    UT_RESULT;

    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Create a delta pressure
    tNodes[0].setPotential(205.0);
    tNodes[0].getContent()->setPressure(205.0);

    tModel->step(tTimeStep);

    /// Position 0 holds the admittance between port 0 and ground, which is controlled by
    /// the gas side of the accumulator.
    CPPUNIT_ASSERT(0.0 < tModel->mAdmittanceMatrix[0]);
    // Check source vector for gas side (port 0).
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tModel->mGasInternalFluid->getPressure() *
                                 tModel->mAdmittanceMatrix[0], tModel->mSourceVector[0], tTolerance);

    /// - verify that both capacitances and effective conductivity are greater than 0.0
    CPPUNIT_ASSERT(0.0 < tModel->mGasCapacitance);
    CPPUNIT_ASSERT(0.0 < tModel->mGasEffectiveConductivity);
    CPPUNIT_ASSERT(0.0 < tModel->mLiqCapacitance);

    tNodes[0].setPotential(200.0);
    tNodes[0].getContent()->setPressure(200.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for step method with changing bellows positions to verify liquid capacitance
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccumGas::testLiquidCapacitance()
{
    UT_RESULT;

    /// - set up a reference capacitance to compare to...
    tInputData->mInitialBellowsPosition = 0.5;
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    tModel->step(tTimeStep);
    double liquidCapacitanceReference = tModel->mLiqCapacitance;

    /// - With increased water volume, there is less gas volume and therefore less liquid capacitance
    tInputData->mInitialBellowsPosition = 0.51;
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    tModel->step(tTimeStep);
    CPPUNIT_ASSERT(liquidCapacitanceReference > tModel->mLiqCapacitance);

    /// - With decreased water volume, there is more gas volume and therefore more liquid capacitance
    tInputData->mInitialBellowsPosition = 0.49;
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    tModel->step(tTimeStep);
    CPPUNIT_ASSERT(liquidCapacitanceReference < tModel->mLiqCapacitance);

    tInputData->mInitialBellowsPosition = 0.5;

    /// - Initialize test model with nominal data, but no spring pressure.
    ///   Test for code coverage.
    tConfigData->mSpringCoeff0 = 0.0;
    tConfigData->mSpringCoeff1 = 0.0;
    tConfigData->mSpringCoeff2 = 0.0;

    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    tModel->step(tTimeStep);
    CPPUNIT_ASSERT(0.0 == tModel->mSpringCapacitance);

    tInputData->mInitialBellowsPosition = 0.5;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for step method with gas-side conductivity limits exceeded.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccumGas::testGasConductivityLimits()
{
    UT_RESULT;

    /// - Set up a test so that the mGasEffectiveConductivity will be limited to tGasMaxConductivity
    ///   and verify the limited link conductivity value goes into the admittance matrix on
    ///   On the second frame, cause a change in the liquid side conductance and verify the change
    ///   in liquid side doesn't zero out the gas side effect in the admittance matrix.
    tInputData->mInitialBellowsPosition = 0.5;
    tConfigData->mGasMaxConductivity    = 1.0e-5;
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    tModel->step(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0e-5, tModel->mAdmittanceMatrix[0], DBL_EPSILON);

    /// - Force a bellows position and chamber volume update.
    tModel->mBellowsPosition += 0.001;
    tModel->deriveChamberVolumes();

    /// - 2nd frame: test that with liquid-side conductance changed but gas-side still unchanged at
    ///   the same limit as last pass, gas-side limited conductance is still set into the matrix.
    tModel->step(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0e-5, tModel->mAdmittanceMatrix[0], DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows - incoming flow
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccumGas::testComputeFlowsNomFlowIn()
{
    UT_RESULT;

    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    double prevGasPressure = tModel->mGasInternalFluid->getPressure();

    tNodes[0].setPotential(200.1);
    tNodes[0].getContent()->setPressure(200.1);
    tNodes[0].resetFlows();

    // set up the gas node's in-flow with pure O2 gas and test the change in accum gas mixture.
    double inFlowFractions[3] = {0.0, 0.0, 1.0};

    PolyFluidInputData inFlowInputData(tInitialTemperature,      //temperature
                                       tInitialPressure,         //pressure
                                       1.0,                      //flowRate
                                       0.0,                      //mass
                                       inFlowFractions);         //massFractions
    PolyFluid inFlow(*tAccumFluidConfigData, inFlowInputData);
    tNodes[0].collectInflux(1.0, &inFlow);

    const double initialMass = tModel->mGasInternalFluid->getMass();

    tModel->step(tTimeStep);
    tModel->computeFlows(tTimeStep);

    CPPUNIT_ASSERT(0.0                    <  tModel->mGasFlux);
    CPPUNIT_ASSERT(0.0                    == tModel->mFlux);
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tModel->getPortDirections()[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::NONE   == tModel->getPortDirections()[1]);
    CPPUNIT_ASSERT(tModel->mGasFlux       == tNodes[tPort0].getScheduledOutflux());
    CPPUNIT_ASSERT(0.0                    == tNodes[tPort1].getScheduledOutflux());

    tModel->transportFlows(tTimeStep);

    CPPUNIT_ASSERT(0.0 < tModel->mGasFlowRate);
    CPPUNIT_ASSERT(prevGasPressure < tModel->mGasInternalFluid->getPressure());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(fabs(tModel->mGasFlowRate), tNodes[0].getOutflux(), tTolerance);

    const double inMass =  tModel->mGasFlowRate * tTimeStep;
    const double newMass = initialMass + inMass;
    const double expectedXO2 = inMass / newMass;
    const double expectedMO2 = newMass * expectedXO2;
    const double expectedXN2 = 1.0 - expectedXO2;
    const double expectedMN2 = newMass - expectedMO2;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(newMass,
                                 tModel->mGasInternalFluid->getMass(),
                                 tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedXO2,
                                 tModel->mGasInternalFluid->getMassFraction(2),
                                 tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMO2,
                                 tModel->mGasInternalFluid->getConstituent(FluidProperties::GUNNS_O2)->getMass(),
                                 tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedXN2,
                                 tModel->mGasInternalFluid->getMassFraction(1),
                                 tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMN2,
                                 tModel->mGasInternalFluid->getConstituent(FluidProperties::GUNNS_N2)->getMass(),
                                 tTolerance);
    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows - Outgoing flow
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccumGas::testComputeFlowsNomFlowOut()
{
    UT_RESULT;

    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    double prevGasPressure = tModel->mGasInternalFluid->getPressure();

    tNodes[0].setPotential(199.8);
    tNodes[0].getContent()->setPressure(199.8);
    tNodes[0].resetFlows();

    tModel->step(tTimeStep);
    tModel->computeFlows(tTimeStep);

    CPPUNIT_ASSERT(0.0                  >  tModel->mGasFlux);
    CPPUNIT_ASSERT(0.0                  == tModel->mFlux);
    CPPUNIT_ASSERT(GunnsBasicLink::SINK == tModel->getPortDirections()[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tModel->getPortDirections()[1]);
    CPPUNIT_ASSERT(0.0                  == tNodes[tPort0].getScheduledOutflux());
    CPPUNIT_ASSERT(0.0                  ==  tNodes[tPort1].getScheduledOutflux());

    tModel->transportFlows(tTimeStep);

    CPPUNIT_ASSERT(0.0 > tModel->mGasFlowRate);
    CPPUNIT_ASSERT(prevGasPressure > tModel->mGasInternalFluid->getPressure());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(fabs(tModel->mGasFlowRate), tNodes[0].getInflux(), tTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Temperature - in flow
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccumGas::testTemperatureInFlow()
{
    UT_RESULT;

    tInputData->mInitialBellowsPosition = 0.5;
    tInputData->mGasFluidInputData->mTemperature = 285.0;
    /// - Initialize default test article with nominal initialization data
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - test warmer temperatures coming in
    tNodes[0].setPotential(205.0);
    tNodes[0].getContent()->setPressure(205.0);
    tNodes[0].getContent()->setTemperature(300.0);
    tNodes[0].resetFlows();
    tNodes[0].getOutflow()->setTemperature(350.0);
    double previousTemperature = tModel->mGasInternalFluid->getTemperature();
    double previousEnthalpy    = tModel->mGasInternalFluid->getSpecificEnthalpy();
    double previousMass        = tModel->mGasInternalFluid->getMass();

    tModel->step(tTimeStep);
    tModel->computeFlows(tTimeStep);
    tModel->transportFlows(tTimeStep);

    /// @test    Inflow uses the node's outflow fluid, not its contents.
    double deltaMass    = tModel->mGasFlowRate * tTimeStep;
    double expectedMass = previousMass + deltaMass;
    double expectedEnthalpy = (previousEnthalpy * previousMass
            + deltaMass * tNodes[0].getOutflow()->getSpecificEnthalpy()) / expectedMass;
    CPPUNIT_ASSERT(previousTemperature < tModel->mGasInternalFluid->getTemperature());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,     tModel->mGasInternalFluid->getMass(),             FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedEnthalpy, tModel->mGasInternalFluid->getSpecificEnthalpy(), FLT_EPSILON);

    /// - test cooler temperatures coming in
    tNodes[0].setPotential(210.0);
    tNodes[0].getContent()->setPressure(210.0);
    tNodes[0].getContent()->setTemperature(200.0);
    tNodes[0].resetFlows();
    previousTemperature = tModel->mGasInternalFluid->getTemperature();

    tModel->step(tTimeStep);
    tModel->computeFlows(tTimeStep);
    tModel->transportFlows(tTimeStep);

    CPPUNIT_ASSERT(previousTemperature > tModel->mGasInternalFluid->getTemperature());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Temperature - out flow
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccumGas::testTemperatureOutFlow()
{
    UT_RESULT;

    tInputData->mInitialBellowsPosition = 0.5;
    tInputData->mGasFluidInputData->mTemperature = 285.0;
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    tNodes[0].setPotential(195.0);
    tNodes[0].getContent()->setPressure(195.0);
    tNodes[0].getContent()->setTemperature(300.0);
    tNodes[0].resetFlows();

    double previousTemperature = tModel->mGasInternalFluid->getTemperature();

    tModel->step(tTimeStep);
    tModel->computeFlows(tTimeStep);
    tModel->transportFlows(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(previousTemperature, tModel->mGasInternalFluid->getTemperature(), tTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Temperature - thermal interface
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccumGas::testTemperatureThermal()
{
    UT_RESULT;

    tInputData->mGasFluidInputData->mTemperature = 285.0;
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    tNodes[0].setPotential(200.0);
    tNodes[0].getContent()->setPressure(200.0);
    tNodes[0].getContent()->setTemperature(285.0);
    tNodes[0].resetFlows();

    double previousTemperature = tModel->mGasInternalFluid->getTemperature();

    tModel->mGasHousingQ = 0.001;
    tModel->step(tTimeStep);
    tModel->computeFlows(tTimeStep);
    tModel->transportFlows(tTimeStep);

    CPPUNIT_ASSERT(previousTemperature < tModel->mGasInternalFluid->getTemperature());

    previousTemperature = tModel->mGasInternalFluid->getTemperature();

    tModel->mGasHousingQ = -0.001;
    tModel->step(tTimeStep);
    tModel->computeFlows(tTimeStep);
    tModel->transportFlows(tTimeStep);

    CPPUNIT_ASSERT(previousTemperature > tModel->mGasInternalFluid->getTemperature());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for pressure edit
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccumGas::testPressureEdit()
{
    UT_RESULT;

    /// - Test with mEditsAreReactive false
    tInputData->mGasFluidInputData->mPressure = 200.0;
    tConfigData->mMaxForcePressureRate = 30.0;
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    tModel->mEditsAreReactive = false;
    tModel->mEditPressureValue = 300.0;
    tModel->mEditPressureRate = 30.0;
    tModel->mEditPressureFlag = true;
    double originalBellowsPos = tModel->mBellowsPosition;
    double originalTemperature = tModel->mGasInternalFluid->getTemperature();

    int numIterations = ((tModel->mEditPressureValue - tInputData->mGasFluidInputData->mPressure)    // Delta pressure to edit
                          / tModel->mEditPressureRate                                                // number of seconds to achieve edit
                          + tModel->mEditHoldTime)                                                   // add edit hold time
                          / tTimeStep                                                                // convert to iterations
                          + 5;                                                                       // small pad at the end

    for (int i=0;i<numIterations;i++)
    {
        tModel->step(tTimeStep);
        tModel->computeFlows(tTimeStep);
        tModel->transportFlows(tTimeStep);
        tNodes[0].setPotential(tModel->mGasInternalFluid->getPressure());
        tNodes[0].getContent()->setPressure(tModel->mGasInternalFluid->getPressure());
        tNodes[0].resetFlows();
        tNodes[1].setPotential(tModel->mInternalFluid->getPressure());
        tNodes[1].getContent()->setPressure(tModel->mInternalFluid->getPressure());
        tNodes[1].resetFlows();
    }

    CPPUNIT_ASSERT_DOUBLES_EQUAL(originalBellowsPos, tModel->mBellowsPosition, tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(originalTemperature, tModel->mGasInternalFluid->getTemperature(), tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tModel->mEditPressureValue, tModel->mGasInternalFluid->getPressure() + tModel->mSpringPressure, tTolerance);
    CPPUNIT_ASSERT(0.0 == tModel->mEditPressureTimer);
    CPPUNIT_ASSERT(false == tModel->mEditPressureFlag);

    /// - Test with mEditsAreReactive true
    tInputData->mGasFluidInputData->mPressure = 200.0;
    tConfigData->mMaxForcePressureRate = 30.0;
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    tModel->mEditsAreReactive = true;
    tModel->mEditPressureValue = 300.0;
    tModel->mEditPressureRate = 30.0;
    tModel->mEditPressureFlag = true;
    originalBellowsPos = tModel->mBellowsPosition;
    originalTemperature = tModel->mGasInternalFluid->getTemperature();

    numIterations = ((tModel->mEditPressureValue - tInputData->mGasFluidInputData->mPressure)    // Delta pressure to edit
                      / tModel->mEditPressureRate                                                // number of seconds to achieve edit
                      + tModel->mEditHoldTime)                                                   // add edit hold time
                      / tTimeStep                                                                // convert to iterations
                      + 5;                                                                       // small pad at the end

    for (int i=0;i<numIterations;i++)
    {
        tModel->step(tTimeStep);
        tModel->computeFlows(tTimeStep);
        tModel->transportFlows(tTimeStep);
        tNodes[0].setPotential(tModel->mGasInternalFluid->getPressure());
        tNodes[0].getContent()->setPressure(tModel->mGasInternalFluid->getPressure());
        tNodes[0].resetFlows();
        tNodes[1].setPotential(tModel->mInternalFluid->getPressure());
        tNodes[1].getContent()->setPressure(tModel->mInternalFluid->getPressure());
        tNodes[1].resetFlows();
    }

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tModel->mEditPressureValue, tModel->mGasInternalFluid->getPressure() + tModel->mSpringPressure, tTolerance);
    CPPUNIT_ASSERT(0.0 == tModel->mEditPressureTimer);
    CPPUNIT_ASSERT(false == tModel->mEditPressureFlag);

    bool bellowsNotEqual = (fabs(tModel->mBellowsPosition - originalBellowsPos) > tTolerance);
    CPPUNIT_ASSERT(bellowsNotEqual);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for temperature edit
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccumGas::testTempertureEdit()
{
    UT_RESULT;

    tInputData->mGasFluidInputData->mTemperature = 283.0;
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    tModel->mEditTemperatureValue = 290.0;
    tModel->mEditTemperatureFlag = true;

    tModel->step(tTimeStep);
    tModel->computeFlows(tTimeStep);
    tModel->transportFlows(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tModel->mEditTemperatureValue, tModel->mGasInternalFluid->getTemperature(), tTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for malfunction = bellows rupture
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccumGas::testMalfBellowsRupture()
{
    UT_RESULT;

    tInputData->mInitialBellowsPosition = 0.5;
    tInputData->mGasFluidInputData->mPressure = 200.0;
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    double prevGasPressure = tModel->mGasInternalFluid->getPressure();

    tNodes[0].setPotential(195.0);
    tNodes[0].getContent()->setPressure(195.0);
    tNodes[0].resetFlows();

    tModel->mMalfBellowsRuptureFlag = true;
    tModel->mMalfBellowsRupturePress = 203.0;
    tModel->mMalfBellowsRupturePressRate = 30.0;

    for (int i=0; i<1000; i++)
    {
        tModel->step(tTimeStep);
        tModel->computeFlows(tTimeStep);
        tModel->transportFlows(tTimeStep);
    }

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tModel->mMalfBellowsRupturePress, tModel->mGasInternalFluid->getPressure(), 10.0);

    /// @test  The bellows rupture malf parameters are set by the access method.
    tModel->setMalfBellowsRupture(true, 0.5, 0.01, 203.0, 30.0);
    CPPUNIT_ASSERT(true  == tModel->mMalfBellowsRuptureFlag);
    CPPUNIT_ASSERT(0.5   == tModel->mMalfBellowsRuptureBellowsPos);
    CPPUNIT_ASSERT(0.01  == tModel->mMalfBellowsRuptureBellowsRate);
    CPPUNIT_ASSERT(203.0 == tModel->mMalfBellowsRupturePress);
    CPPUNIT_ASSERT(30.0  == tModel->mMalfBellowsRupturePressRate);

    /// @test  The bellows rupture malf parameters are reset by the access method.
    tModel->setMalfBellowsRupture();
    CPPUNIT_ASSERT(false == tModel->mMalfBellowsRuptureFlag);
    CPPUNIT_ASSERT(0.0   == tModel->mMalfBellowsRuptureBellowsPos);
    CPPUNIT_ASSERT(0.0   == tModel->mMalfBellowsRuptureBellowsRate);
    CPPUNIT_ASSERT(0.0   == tModel->mMalfBellowsRupturePress);
    CPPUNIT_ASSERT(0.0   == tModel->mMalfBellowsRupturePressRate);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for hold accumulator
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccumGas::testHoldAccum()
{
    UT_RESULT;

    tInputData->mGasFluidInputData->mPressure = 200.0;
    tInputData->mGasFluidInputData->mTemperature = 283;
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    tNodes[0].setPotential(195.0);
    tNodes[0].getContent()->setPressure(195.0);

    double previousMass = tModel->mGasInternalFluid->getMass();

    tModel->mHoldAccumFlag = true;

    tModel->step(tTimeStep);
    tModel->computeFlows(tTimeStep);
    tModel->transportFlows(tTimeStep);

    CPPUNIT_ASSERT(200.0 == tModel->mGasInternalFluid->getPressure());
    CPPUNIT_ASSERT(283.0 == tModel->mGasInternalFluid->getTemperature());
    CPPUNIT_ASSERT(previousMass == tModel->mGasInternalFluid->getMass());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for pressurizer bellows override
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccumGas::testPressurizerOrideBellows()
{
    UT_RESULT;

    /// - Call pressurizerOrideBellows to test resetting edit flags and timers.
    tInputData->mGasFluidInputData->mPressure = 200.0;
    tInputData->mGasFluidInputData->mTemperature = 283;
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Test as Rupture malfunction. Edit flags and timers are reset.
    tModel->mEditPressureFlag = true;
    tModel->mEditPressureTimer = 0.2;
    tModel->mMalfBellowsRuptureFlag = true;

    tModel->pressurizerOrideBellows(tTimeStep);

    CPPUNIT_ASSERT(false == tModel->mEditPressureFlag);
    CPPUNIT_ASSERT(0.0 == tModel->mEditPressureTimer);

    /// - Test as pressure edit. Edit flags and timers are not reset.
    tModel->mEditPressureFlag = true;
    tModel->mEditPressureTimer = 0.2;
    tModel->mMalfBellowsRuptureFlag = false;

    tModel->pressurizerOrideBellows(tTimeStep);

    CPPUNIT_ASSERT(true == tModel->mEditPressureFlag);
    CPPUNIT_ASSERT(0.2 == tModel->mEditPressureTimer);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Liquid side bellows edit - should not affect pressure or temperature.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccumGas::testBellowsEdit()
{
    UT_RESULT;

    /// - Test bellows edit without pressure edit.
    tInputData->mGasFluidInputData->mPressure = 200.0;
    tInputData->mInitialBellowsPosition = 0.5;
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    tModel->mEditsAreReactive = false;
    tModel->mEditBellowsPosition = 0.75;
    tModel->mEditBellowsRate = 0.016;
    tModel->mEditBellowsFlag = true;
    double originalPressure = tModel->mInternalFluid->getPressure();
    double originalTemperature = tModel->mGasInternalFluid->getTemperature();

    int numIterations = ((tModel->mEditBellowsPosition - tInputData->mInitialBellowsPosition) // Delta bellows pos to edit
                          / tModel->mEditBellowsRate                                          // number of seconds to achieve edit
                          + tModel->mEditHoldTime)                                            // add edit hold time
                          / tTimeStep                                                         // convert to iterations
                          + 5;                                                                // A little pad at the end.

    for (int i=0;i<numIterations;i++)
    {
        tModel->step(tTimeStep);
        tModel->computeFlows(tTimeStep);
        tModel->transportFlows(tTimeStep);
        tNodes[0].setPotential(tModel->mGasInternalFluid->getPressure());
        tNodes[0].getContent()->setPressure(tModel->mGasInternalFluid->getPressure());
        tNodes[0].resetFlows();
        tNodes[1].setPotential(tModel->mInternalFluid->getPressure());
        tNodes[1].getContent()->setPressure(tModel->mInternalFluid->getPressure());
        tNodes[1].resetFlows();
    }

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tModel->mEditBellowsPosition, tModel->mBellowsPosition, tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(originalPressure, tModel->mInternalFluid->getPressure(), tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(originalTemperature, tModel->mGasInternalFluid->getTemperature(), tTolerance);
    CPPUNIT_ASSERT(0.0 == tModel->mEditBellowsTimer);
    CPPUNIT_ASSERT(false == tModel->mEditBellowsFlag);

    /// - Test bellows edit with simultaneous pressure edit.
    tInputData->mGasFluidInputData->mPressure = 200.0;
    tInputData->mInitialBellowsPosition = 0.5;
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    tModel->mEditsAreReactive = true;
    tModel->mEditBellowsPosition = 0.75;
    tModel->mEditBellowsRate = 0.016;
    tModel->mEditBellowsFlag = true;
    tModel->mEditPressureValue = 175.0;
    tModel->mEditPressureRate = 30.0;
    tModel->mEditPressureFlag = true;
    originalPressure = tModel->mGasInternalFluid->getPressure();
    originalTemperature = tModel->mGasInternalFluid->getTemperature();

    int numIterationsPressure = ((tModel->mEditPressureValue - tInputData->mGasFluidInputData->mPressure)    // Delta pressure to edit
                                  / tModel->mEditPressureRate                                                 // number of seconds to achieve edit
                                  + tModel->mEditHoldTime)                                                    // add edit hold time
                                  / tTimeStep;                                                                // convert to iterations

    int numIterationsBellows = ((tModel->mEditBellowsPosition - tInputData->mInitialBellowsPosition) // Delta bellows pos to edit
                                 / tModel->mEditBellowsRate                                          // number of seconds to achieve edit
                                 + tModel->mEditHoldTime)                                            // add edit hold time
                                 / tTimeStep;                                                        // convert to iterations

    numIterations = numIterationsPressure + numIterationsBellows + 5;                                // Allow more than enough iterations.

    for (int i=0;i<numIterations;i++)
    {
        tModel->step(tTimeStep);
        tModel->computeFlows(tTimeStep);
        tModel->transportFlows(tTimeStep);
        tNodes[0].setPotential(tModel->mGasInternalFluid->getPressure());
        tNodes[0].getContent()->setPressure(tModel->mGasInternalFluid->getPressure());
        tNodes[0].resetFlows();
        tNodes[1].setPotential(tModel->mInternalFluid->getPressure());
        tNodes[1].getContent()->setPressure(tModel->mInternalFluid->getPressure());
        tNodes[1].resetFlows();
    }

    CPPUNIT_ASSERT(false == tModel->mEditBellowsFlag);
    CPPUNIT_ASSERT(0.0 == tModel->mEditBellowsTimer);
    CPPUNIT_ASSERT(false == tModel->mEditPressureFlag);
    CPPUNIT_ASSERT(0.0 == tModel->mEditPressureTimer);
    CPPUNIT_ASSERT(false == tModel->mSimultaneousEditsInProgress);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tModel->mBellowsPosition, tModel->mEditBellowsPosition, tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tModel->mInternalFluid->getPressure(), tModel->mEditPressureValue, tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(originalTemperature, tModel->getGasInternalFluid()->getTemperature(), tTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccumGas::testRestart()
{
    UT_RESULT;

    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Reset of non-checkpoint and non-config-derived data
    tModel->mFillMode   = GunnsFluidAccum::DRAINING;
    tModel->mBellowsZone = GunnsFluidAccum::FULL;

    tModel->restart();

    CPPUNIT_ASSERT(GunnsFluidAccum::EQUALIZED == tModel->mFillMode);
    CPPUNIT_ASSERT(GunnsFluidAccum::MIDDLE    == tModel->mBellowsZone);
    UT_PASS_FINAL;
}
