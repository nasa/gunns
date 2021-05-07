/*
@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/

#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidAccum.hh"

/// @details  Test identification number.
int UtGunnsFluidAccum::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsFluidAccum class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidAccum::UtGunnsFluidAccum()
    :
    tFluidProperties(),
    tLiquidFluidConfigData(),
    tLiquidFluidInputData(),
    tMassFractions(),
    tConfigData(),
    tInputData(),
    tModel(),
    tLinkName(),
    tMaxConductivity(),
    tMinConductivityScale(),
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
    tMinChamberVolPercent(),
    tMinChamberVolDeadBandPercent(),
    tForceBellowsMaxRate(),
    tEditHoldTime(),
    tMinTemperature(),
    tMaxTemperature(),
    tMaxPressure(),
    tSpringCoeff0(),
    tSpringCoeff1(),
    tSpringCoeff2(),
    tFillModePressureThreshold(),
    tEffCondScaleRate()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsFluidAccum class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidAccum::~UtGunnsFluidAccum()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccum::tearDown()
{
    /// - Deletes for news in setUp
    delete tModel;
    delete tInputData;
    delete tConfigData;
    delete tLiquidFluidInputData;
    delete tLiquidFluidConfigData;
    delete[] tMassFractions;
    delete tFluidProperties;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccum::setUp()
{
    tLinkName             = "Test Accumulator";
    tMaxConductivity = 1.0;
    tMinConductivityScale = 0.1;
    tExpansionScaleFactor = 0.0;
    tAccumVolume = 0.0156;
    tMinChamberVolPercent = 10.0;
    tMinChamberVolDeadBandPercent = 15.0;
    tForceBellowsMaxRate = 1.0 / UnitConversion::SEC_PER_MIN;
    tNodeList.mNumNodes = 2;
    tNodeList.mNodes = tNodes;
    tPort0 = 1;     // Node 1 - vacuum node in place of what could be gas node.
    tPort1 = 0;     // Node 0 - liquid node. Liquid node must be port 1.
    tTimeStep = 0.1;
    tTolerance = 1.0E-06;
    tEditHoldTime = 5.0;
    tMinTemperature = 274.0;
    tMaxTemperature = 373.0;

    tMaxPressure = 600.0;
    tSpringCoeff0 = -50.0;
    tSpringCoeff1 = 100.0;
    tSpringCoeff2 = 0.0;
    tFillModePressureThreshold = 0.0;
    tEffCondScaleRate = 0.0;

    /// - Setup some fluid nodes
    tFluidProperties = new DefinedFluidProperties();
    FluidProperties::FluidType types[2];
    types[0]    = FluidProperties::GUNNS_WATER;
    types[1]    = FluidProperties::GUNNS_N2;
    tMassFractions = new double[2];
    tMassFractions[0] = 1.0;
    tMassFractions[1] = 0.0;
    tInitialBellowsPosition = 0.5;

    tLiquidFluidConfigData = new PolyFluidConfigData(tFluidProperties, types, 2);

    tLiquidFluidInputData = new PolyFluidInputData(283.0,         //temperature
                                          200.0,                  //pressure
                                          0.0,                    //flowRate
                                          0.0,                    //mass
                                          tMassFractions);               //massFractions

    /// - Have to initialize the nodes with the fluid configs (normally done by GUNNS)
    /// - because there are only 2 nodes. Node 1 is the vacuum boundary node, Node 0 is the
    ///   liquid node (port 1 of the accumulator link).
    tNodes[0].initialize("Node0", tLiquidFluidConfigData, tLiquidFluidInputData);
    tNodes[1].initialize("Node1", tLiquidFluidConfigData, 0);

    tNodes[0].setPotential(200.0);
    tNodes[1].setPotential(0.0);

    tNodes[0].getContent()->initialize(*tLiquidFluidConfigData, *tLiquidFluidInputData);
//    tNodes[1].getContent()->initialize(*tLiquidFluidConfigData, *tLiquidFluidInputData);

    tConfigData = new GunnsFluidAccumConfigData (
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
            tFillModePressureThreshold,
            tEffCondScaleRate);

    tInputData = new GunnsFluidAccumInputData (false, 0.0, tInitialBellowsPosition, tLiquidFluidInputData);

    tModel = new FriendlyGunnsFluidAccum;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccum::testConfig()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data default construction.
    GunnsFluidAccumConfigData defaultConfig;
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

    /// @test    Configuration data copy construction.
    GunnsFluidAccumConfigData copyConfig(*tConfigData);
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

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccum::testInput()
{
    UT_RESULT;

    /// @test    Input data default construction.
    GunnsFluidAccumInputData defaultInput;
    CPPUNIT_ASSERT(false == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0 == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0 == defaultInput.mInitialBellowsPosition);
    CPPUNIT_ASSERT(0 == defaultInput.mLiquidFluidInputData);

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(false == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0 == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tInitialBellowsPosition == tInputData->mInitialBellowsPosition);
    CPPUNIT_ASSERT(tLiquidFluidInputData->mPressure == tInputData->mLiquidFluidInputData->mPressure);

    /// @test    Input data copy construction.
    GunnsFluidAccumInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageValue == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(tInputData->mInitialBellowsPosition == copyInput.mInitialBellowsPosition);
    CPPUNIT_ASSERT(tLiquidFluidInputData->mPressure == copyInput.mLiquidFluidInputData->mPressure);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccum::testDefaultConstruction()
{
    UT_RESULT;

    /// @test default construction.
    CPPUNIT_ASSERT(0.0 == tModel->mMaxConductivity);
    CPPUNIT_ASSERT(0.0 == tModel->mMinConductivityScale);
    CPPUNIT_ASSERT(0.0 == tModel->mEffectiveConductivity);
    CPPUNIT_ASSERT(0.0 == tModel->mEffConductivityScale);
    CPPUNIT_ASSERT(0.0 == tModel->mBellowsPosition);
    CPPUNIT_ASSERT(0.0 == tModel->mLiquidPressureReading);
    CPPUNIT_ASSERT(0.0 == tModel->mLiquidHousingQ);
    CPPUNIT_ASSERT(0.0 == tModel->mTotalVolume);
    CPPUNIT_ASSERT(0.0 == tModel->mMinChamberVol);
    CPPUNIT_ASSERT(0.0 == tModel->mMaxChamberVol);
    CPPUNIT_ASSERT(0.0 == tModel->mMinDeadBandVol);
    CPPUNIT_ASSERT(0.0 == tModel->mActiveVolRange);
    CPPUNIT_ASSERT(0.0 == tModel->mLiquidVolume);
    CPPUNIT_ASSERT(0.0 == tModel->mPressurizerVolume);
    CPPUNIT_ASSERT(0.0 == tModel->mSpringCoeff0);
    CPPUNIT_ASSERT(0.0 == tModel->mSpringCoeff1);
    CPPUNIT_ASSERT(0.0 == tModel->mSpringCoeff2);
    CPPUNIT_ASSERT(false == tModel->mEditBellowsFlag);
    CPPUNIT_ASSERT(0.0 == tModel->mEditBellowsPosition);
    CPPUNIT_ASSERT(0.0 == tModel->mEditBellowsRate);
    CPPUNIT_ASSERT(0.0 == tModel->mEditBellowsTimer);
    CPPUNIT_ASSERT(false == tModel->mEditTemperatureFlag);
    CPPUNIT_ASSERT(0.0 == tModel->mEditTemperatureValue);
    CPPUNIT_ASSERT(false == tModel->mHoldAccumFlag);
    CPPUNIT_ASSERT(false == tModel->mHoldTemperatureFlag);
    CPPUNIT_ASSERT(false == tModel->mBellowsStuck);
    CPPUNIT_ASSERT(false == tModel->mMalfBellowsStickFlag);
    CPPUNIT_ASSERT(false == tModel->mMalfBellowsStickToPosFlag);
    CPPUNIT_ASSERT(0.0 == tModel->mMalfBellowsStickToPosValue);
    CPPUNIT_ASSERT(0.0 == tModel->mMalfBellowsStickToPosRate);
    CPPUNIT_ASSERT(0.0 == tModel->mForceBellowsMaxRate);
    CPPUNIT_ASSERT(0.0 == tModel->mEditHoldTime);
    CPPUNIT_ASSERT(0.0 == tModel->mMinTemperature);
    CPPUNIT_ASSERT(0.0 == tModel->mMaxTemperature);
    CPPUNIT_ASSERT(0.0 == tModel->mMaxPressure);
    CPPUNIT_ASSERT(false == tModel->mBellowsMalfSet);
    CPPUNIT_ASSERT(true == tModel->mEditsAreReactive);
    CPPUNIT_ASSERT(false == tModel->mSimultaneousEditsInProgress);
    CPPUNIT_ASSERT(GunnsFluidAccum::EQUALIZED == tModel->mFillMode);
    CPPUNIT_ASSERT(GunnsFluidAccum::MIDDLE == tModel->mBellowsZone);
    CPPUNIT_ASSERT(0.0 == tModel->mFillModePressureThreshold);
    CPPUNIT_ASSERT(0.0 == tModel->mEffCondScaleOneWayRate);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccum::testNominalInitialization()
{
    UT_RESULT;

    /// - Default construct and initialize test model with nominal data.
    FriendlyGunnsFluidAccum accumModel;
    accumModel.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Verify inherited model was called.
    CPPUNIT_ASSERT(tLinkName == accumModel.getName());
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag == accumModel.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageValue == accumModel.mMalfBlockageValue);

    /// @test nominal initialization values
    CPPUNIT_ASSERT(tMaxConductivity == accumModel.mMaxConductivity);
    CPPUNIT_ASSERT(tMinConductivityScale == accumModel.mMinConductivityScale);
    CPPUNIT_ASSERT(tMaxConductivity == accumModel.mEffectiveConductivity);
    CPPUNIT_ASSERT(1.0 == accumModel.mEffConductivityScale);

    CPPUNIT_ASSERT(tInitialBellowsPosition == accumModel.mBellowsPosition);
    CPPUNIT_ASSERT(tInputData->mLiquidFluidInputData->mPressure == accumModel.mLiquidPressureReading);
    CPPUNIT_ASSERT(0.0 == accumModel.mLiquidHousingQ);
    CPPUNIT_ASSERT(tAccumVolume * tMinChamberVolPercent / UnitConversion::PERCENTAGE == accumModel.mMinChamberVol);
    CPPUNIT_ASSERT(tAccumVolume == accumModel.mMaxChamberVol);
    CPPUNIT_ASSERT(tAccumVolume * tMinChamberVolDeadBandPercent / UnitConversion::PERCENTAGE == accumModel.mMinDeadBandVol);
    CPPUNIT_ASSERT(accumModel.mMaxChamberVol - accumModel.mMinChamberVol == accumModel.mActiveVolRange);
    CPPUNIT_ASSERT(tAccumVolume + accumModel.mMinChamberVol == accumModel.mTotalVolume);
    CPPUNIT_ASSERT(accumModel.mTotalVolume - accumModel.mLiquidVolume == accumModel.mPressurizerVolume);
    CPPUNIT_ASSERT(tInitialBellowsPosition * accumModel.mActiveVolRange +
                   accumModel.mMinChamberVol == accumModel.mLiquidVolume);
    CPPUNIT_ASSERT(tSpringCoeff0 == accumModel.mSpringCoeff0);
    CPPUNIT_ASSERT(tSpringCoeff1 == accumModel.mSpringCoeff1);
    CPPUNIT_ASSERT(tSpringCoeff2 == accumModel.mSpringCoeff2);
    CPPUNIT_ASSERT((tInitialBellowsPosition *tInitialBellowsPosition) * accumModel.mSpringCoeff2
                    + (tInitialBellowsPosition) * accumModel.mSpringCoeff1 + tSpringCoeff0 == accumModel.mSpringPressure);

    /// - Verify fluid attributes
    CPPUNIT_ASSERT(tInputData->mLiquidFluidInputData->mMassFraction[0] == accumModel.mInternalFluid->getMassFraction(0));
    CPPUNIT_ASSERT(tInputData->mLiquidFluidInputData->mMassFraction[1] == accumModel.mInternalFluid->getMassFraction(1));
    CPPUNIT_ASSERT(tInputData->mLiquidFluidInputData->mPressure == accumModel.mInternalFluid->getPressure());
    CPPUNIT_ASSERT(tInputData->mLiquidFluidInputData->mTemperature == accumModel.mInternalFluid->getTemperature());
    CPPUNIT_ASSERT(accumModel.mLiquidVolume * accumModel.mInternalFluid->getDensity() == accumModel.mInternalFluid->getMass());
    CPPUNIT_ASSERT(0.0 == accumModel.mFlowRate);
    CPPUNIT_ASSERT(0.0 == accumModel.mInternalFluid->getFlowRate());

    /// - Verify Edit attributes
    CPPUNIT_ASSERT(tEditHoldTime == accumModel.mEditHoldTime);
    CPPUNIT_ASSERT(false == accumModel.mEditBellowsFlag);
    CPPUNIT_ASSERT(0.0 == accumModel.mEditBellowsPosition);
    CPPUNIT_ASSERT(tForceBellowsMaxRate == accumModel.mEditBellowsRate);
    CPPUNIT_ASSERT(0.0 == accumModel.mEditBellowsTimer);
    CPPUNIT_ASSERT(false == accumModel.mEditTemperatureFlag);
    CPPUNIT_ASSERT(0.0 == accumModel.mEditTemperatureValue);

    /// - Verify malfunctions and overrides
    CPPUNIT_ASSERT(false == accumModel.mHoldAccumFlag);
    CPPUNIT_ASSERT(false == accumModel.mHoldTemperatureFlag);
    CPPUNIT_ASSERT(false == accumModel.mPressurizerOrideBellowsFlag);
    CPPUNIT_ASSERT(false == accumModel.mMalfBellowsStickFlag);
    CPPUNIT_ASSERT(false == accumModel.mMalfBellowsStickToPosFlag);
    CPPUNIT_ASSERT(0.0 == accumModel.mMalfBellowsStickToPosValue);
    CPPUNIT_ASSERT(tForceBellowsMaxRate == accumModel.mMalfBellowsStickToPosRate);

    CPPUNIT_ASSERT(false == accumModel.mBellowsStuck);

    /// Verify other maximum values
    CPPUNIT_ASSERT(tForceBellowsMaxRate == accumModel.mForceBellowsMaxRate);
    CPPUNIT_ASSERT(tMinTemperature == accumModel.mMinTemperature);
    CPPUNIT_ASSERT(tMaxTemperature == accumModel.mMaxTemperature);
    CPPUNIT_ASSERT(tMaxPressure == accumModel.mMaxPressure);

    CPPUNIT_ASSERT(false == tModel->mBellowsMalfSet);
    CPPUNIT_ASSERT(true == tModel->mEditsAreReactive);
    CPPUNIT_ASSERT(false == tModel->mSimultaneousEditsInProgress);

    CPPUNIT_ASSERT(GunnsFluidAccum::EQUALIZED == tModel->mFillMode);
    CPPUNIT_ASSERT(GunnsFluidAccum::MIDDLE == tModel->mBellowsZone);
    CPPUNIT_ASSERT(0.0 == tModel->mFillModePressureThreshold);
    CPPUNIT_ASSERT(0.0 == tModel->mEffCondScaleOneWayRate);

    /// Verify init flag
    CPPUNIT_ASSERT(accumModel.mInitFlag);

    /// - Initialize test model with nominal data, but no spring pressure.
    ///   Test for code coverage.
    tConfigData->mSpringCoeff0 = 0.0;
    tConfigData->mSpringCoeff1 = 0.0;
    tConfigData->mSpringCoeff2 = 0.0;

    accumModel.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Verify spring capacitance is 0.0
    CPPUNIT_ASSERT(0.0 == accumModel.mSpringCapacitance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for initialization exceptions
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccum::testInitializationExceptions()
{
    UT_RESULT;

    double tempVal = 0.0;
    double bellowsMidPoint = 50.0;

    /// - Default construct a local test model.
    FriendlyGunnsFluidAccum model;

    /// @test   Exception on mMaxConductivity < 0.0
    tempVal = tConfigData->mMaxConductivity;
    tConfigData->mMaxConductivity = -0.5;
    CPPUNIT_ASSERT_THROW(model.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mMaxConductivity = tempVal;

    /// @test   Exception on mMinConductivityScale < DBL_EPSILON and > 1.0
    tempVal = tConfigData->mMinConductivityScale;
    tConfigData->mMinConductivityScale = 0.0;
    CPPUNIT_ASSERT_THROW(model.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mMinConductivityScale = 1.1;
    CPPUNIT_ASSERT_THROW(model.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mMinConductivityScale = tempVal;

    /// @test   Exception on mAccumVolume <= 0.0
    tempVal = tConfigData->mAccumVolume;
    tConfigData->mAccumVolume = 0.0;
    CPPUNIT_ASSERT_THROW(model.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mAccumVolume = tempVal;

    /// @test   Exception on mMinChamberVolPercent <= 0.0
    tempVal = tConfigData->mMinChamberVolPercent;
    tConfigData->mMinChamberVolPercent = 0.0;
    CPPUNIT_ASSERT_THROW(model.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mMinChamberVolPercent = tempVal;

    /// @test   Exception on mMinChamberVolPercent >= midpoint (50%)
    tempVal = tConfigData->mMinChamberVolPercent;
    tConfigData->mMinChamberVolPercent = bellowsMidPoint;
    CPPUNIT_ASSERT_THROW(model.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mMinChamberVolPercent = tempVal;

    /// @test   Exception on mMinChamberVolDeadBandPercent <= mMinChamberVolPercent
    tempVal = tConfigData->mMinChamberVolDeadBandPercent;
    tConfigData->mMinChamberVolDeadBandPercent = 10.0;
    CPPUNIT_ASSERT_THROW(model.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mMinChamberVolDeadBandPercent = tempVal;

    /// @test   Exception on mMinChamberVolDeadBandPercent >= midpoint (50%)
    tempVal = tConfigData->mMinChamberVolDeadBandPercent;
    tConfigData->mMinChamberVolDeadBandPercent = bellowsMidPoint;
    CPPUNIT_ASSERT_THROW(model.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mMinChamberVolDeadBandPercent = tempVal;

    /// @test   Exception on mForceBellowsMaxRate <= 0.0
    tempVal = tConfigData->mForceBellowsMaxRate;
    tConfigData->mForceBellowsMaxRate = 0.0;
    CPPUNIT_ASSERT_THROW(model.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mForceBellowsMaxRate = tempVal;

    /// @test   Exception on mEditHoldTime < 0.0
    tempVal = tConfigData->mEditHoldTime;
    tConfigData->mEditHoldTime = -1.0;
    CPPUNIT_ASSERT_THROW(model.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mEditHoldTime = tempVal;

    /// @test   Exception on mMinTemperature <= 0.0 K.
    tempVal = tConfigData->mMinTemperature;
    tConfigData->mMinTemperature = 0.0;
    CPPUNIT_ASSERT_THROW(model.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mMinTemperature = tempVal;

    /// @test   Exception on mMaxTemperature <= 0.0 K.
    tempVal = tConfigData->mMaxTemperature;
    tConfigData->mMaxTemperature = 0.0;
    CPPUNIT_ASSERT_THROW(model.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mMaxTemperature = tempVal;

    /// @test   Exception on mMaxTemperature <= mMinTemperature.
    tempVal = tConfigData->mMaxTemperature;
    tConfigData->mMaxTemperature = 273.0;  // mMinTemperature = 274.0
    CPPUNIT_ASSERT_THROW(model.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mMaxTemperature = tempVal;

    /// @test   Exception on mMaxPressure <= DBL_EPSILON
    tempVal = tConfigData->mMaxPressure;
    tConfigData->mMaxPressure = 0.0;
    CPPUNIT_ASSERT_THROW(model.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mMaxPressure = tempVal;

    /// @test   Exception on mEffCondScaleRate <= DBL_EPSILON if mFillModePressureThreshold > DBL_EPSILON
    tempVal = tConfigData->mFillModePressureThreshold;
    double tempEffCondScaleRate = tConfigData->mEffCondScaleOneWayRate;
    tConfigData->mFillModePressureThreshold = 1.0E-05;
    tConfigData->mEffCondScaleOneWayRate = 0.0;
    CPPUNIT_ASSERT_THROW(model.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mFillModePressureThreshold = tempVal;
    tConfigData->mEffCondScaleOneWayRate = tempEffCondScaleRate;

    /// @test   Exception on mInitialBellowsPosition < 0.0 and > 1.0
    tempVal = tInputData->mInitialBellowsPosition;
    tInputData->mInitialBellowsPosition = -0.1;
    CPPUNIT_ASSERT_THROW(model.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tInputData->mInitialBellowsPosition = 1.1;
    CPPUNIT_ASSERT_THROW(model.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tInputData->mInitialBellowsPosition = tempVal;

    /// @test   Exception on fluid input data having a NULL pointer
    PolyFluidInputData* tempFluidInput;
    tempFluidInput = tInputData->mLiquidFluidInputData;
    tInputData->mLiquidFluidInputData = 0;
    CPPUNIT_ASSERT_THROW(model.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tInputData->mLiquidFluidInputData = tempFluidInput;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for step method
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccum::testStep()
{
    UT_RESULT;

    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Create a delta pressure
    tNodes[0].setPotential(205.0);
    tNodes[0].getContent()->setPressure(205.0);

    const double accelPressureHead = 1.0;
    tModel->setAccelPressureHead(accelPressureHead);
    const double expectedPslope = 2.0 * tInitialBellowsPosition * tSpringCoeff2 + tSpringCoeff1
                                + accelPressureHead / tInitialBellowsPosition;
    const double expectedCap = tModel->mActiveVolRange * tModel->mInternalFluid->getDensity()
                             / tModel->mInternalFluid->getMWeight() / expectedPslope;
    tModel->step(tTimeStep);

    /// - Conductor should always have a positive admittance and zero potential
    CPPUNIT_ASSERT(0.0 == tModel->mAdmittanceMatrix[0]);
    CPPUNIT_ASSERT(0.0 == tModel->mAdmittanceMatrix[1]);
    CPPUNIT_ASSERT(0.0 == tModel->mAdmittanceMatrix[2]);
    /// Position 3 holds the admittance between port 1 and ground, which is controlled by
    /// the liquid side of the accumulator.
    CPPUNIT_ASSERT(0.0 < tModel->mAdmittanceMatrix[3]);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tModel->mSourceVector[0], tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tModel->mInternalFluid->getPressure() *
                                 tModel->mAdmittanceMatrix[3], tModel->mSourceVector[1], tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCap, tModel->mSpringCapacitance, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCap, tModel->mLiqCapacitance,    DBL_EPSILON);

    tNodes[0].setPotential(200.0);
    tNodes[0].getContent()->setPressure(200.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows - incoming flow
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccum::testComputeFlowsNomFlowIn()
{
    UT_RESULT;

    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    double prevBellowsPosition = tModel->mBellowsPosition;

    tNodes[0].setPotential(200.1);
    tNodes[0].getContent()->setPressure(200.1);
    tNodes[0].resetFlows();

    const double accelPressureHead = 1.0;
    tModel->setAccelPressureHead(accelPressureHead);
    tModel->step(tTimeStep);
    tModel->computeFlows(tTimeStep);

    CPPUNIT_ASSERT(0.0                    <  tModel->mFlux);
    CPPUNIT_ASSERT(GunnsBasicLink::NONE   == tModel->getPortDirections()[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tModel->getPortDirections()[1]);
    CPPUNIT_ASSERT(tModel->mFlux          == tNodes[tPort1].getScheduledOutflux());
    CPPUNIT_ASSERT(0.0                    == tNodes[tPort0].getScheduledOutflux());

    tModel->transportFlows(tTimeStep);
    const double newBellowsPosition = tModel->mBellowsPosition;

    CPPUNIT_ASSERT(0.0 < tModel->mFlowRate);
    CPPUNIT_ASSERT(prevBellowsPosition < newBellowsPosition);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(fabs(tModel->mFlowRate), tNodes[0].getOutflux(), tTolerance);

    const double expectedSpringP = newBellowsPosition * newBellowsPosition * tSpringCoeff2
                                 + newBellowsPosition * tSpringCoeff1 + tSpringCoeff0;
    const double expectedLiquidP = expectedSpringP + accelPressureHead;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSpringP, tModel->mSpringPressure,               DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedLiquidP, tModel->mInternalFluid->getPressure(), DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows - outgoing flow
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccum::testComputeFlowsNomFlowOut()
{
    UT_RESULT;

    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    double prevBellowsPosition = tModel->mBellowsPosition;

    tNodes[0].setPotential(199.0);
    tNodes[0].getContent()->setPressure(199.0);
    tNodes[0].resetFlows();

    tModel->step(tTimeStep);
    tModel->computeFlows(tTimeStep);

    CPPUNIT_ASSERT(0.0                  >  tModel->mFlux);
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tModel->getPortDirections()[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::SINK == tModel->getPortDirections()[1]);
    CPPUNIT_ASSERT(0.0                  == tNodes[tPort1].getScheduledOutflux());
    CPPUNIT_ASSERT(0.0                  == tNodes[tPort0].getScheduledOutflux());

    tModel->transportFlows(tTimeStep);

    CPPUNIT_ASSERT(0.0 > tModel->mFlowRate);
    CPPUNIT_ASSERT(prevBellowsPosition > tModel->mBellowsPosition);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(fabs(tModel->mFlowRate), tNodes[0].getInflux(), tTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for effective conductivity - bellows 100%
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccum::testEffConductivityFull()
{
    UT_RESULT;

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// - Initialize mFillModePressureThreshold to be 0.0 (simple effective conductivity scale logic)
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    tConfigData->mFillModePressureThreshold = 0.0;
    tConfigData->mEffCondScaleOneWayRate = 0.0;
    tInputData->mInitialBellowsPosition = 1.0;
    /// - Initialize default test article with nominal initialization data
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Pressure equalized
    tNodes[0].setPotential(200.0);
    tNodes[0].getContent()->setPressure(200.0);

    tModel->updateEffectiveConductivity(tTimeStep);

    CPPUNIT_ASSERT(0.0 == tModel->mEffConductivityScale);
    CPPUNIT_ASSERT(0.0 == tModel->mEffectiveConductivity);

    /// - Push bellows into hard stop
    tNodes[0].setPotential(210.0);
    tNodes[0].getContent()->setPressure(210.0);

    tModel->updateEffectiveConductivity(tTimeStep);

    CPPUNIT_ASSERT(0.0 == tModel->mEffConductivityScale);
    CPPUNIT_ASSERT(0.0 == tModel->mEffectiveConductivity);

    /// - Push bellows away from hard stop
    tNodes[0].setPotential(198.0);
    tNodes[0].getContent()->setPressure(198.0);

    tModel->updateEffectiveConductivity(tTimeStep);

    CPPUNIT_ASSERT(0.0 < tModel->mEffConductivityScale && 1.0 > tModel->mEffConductivityScale);
    CPPUNIT_ASSERT(0.0 < tModel->mEffectiveConductivity);

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// - Initialize mFillModePressureThreshold to be > 0.0 (complex effective conductivity scale logic)
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    tConfigData->mFillModePressureThreshold = 1.0E-4;
    tConfigData->mEffCondScaleOneWayRate = 2.0;
    tInputData->mInitialBellowsPosition = 1.0;
    /// - Initialize default test article with nominal initialization data
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Pressure equalized
    tNodes[0].setPotential(200.0);
    tNodes[0].getContent()->setPressure(200.0);

    tModel->updateEffectiveConductivity(tTimeStep);

    CPPUNIT_ASSERT(0.0 == tModel->mEffConductivityScale);
    CPPUNIT_ASSERT(0.0 == tModel->mEffectiveConductivity);

    /// - Push bellows into hard stop
    tNodes[0].setPotential(210.0);
    tNodes[0].getContent()->setPressure(210.0);

    tModel->updateEffectiveConductivity(tTimeStep);

    CPPUNIT_ASSERT(0.0 == tModel->mEffConductivityScale);
    CPPUNIT_ASSERT(0.0 == tModel->mEffectiveConductivity);

    /// - Push bellows away from hard stop
    tNodes[0].setPotential(198.0);
    tNodes[0].getContent()->setPressure(198.0);

    tModel->updateEffectiveConductivity(tTimeStep);

    double expectedResult = tModel->mEffCondScaleOneWayRate * tTimeStep;
    CPPUNIT_ASSERT(expectedResult == tModel->mEffConductivityScale);
    CPPUNIT_ASSERT(0.0 < tModel->mEffectiveConductivity);
    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for effective conductivity - bellows 0%
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccum::testEffConductivityEmpty()
{
    UT_RESULT;

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// - Initialize mFillModePressureThreshold to be 0.0 (simple effective conductivity scale logic)
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    tConfigData->mFillModePressureThreshold = 0.0;
    tConfigData->mEffCondScaleOneWayRate = 0.0;
    tInputData->mInitialBellowsPosition = 0.0;
    /// - Initialize default test article with nominal initialization data
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Pressure equalized
    tNodes[0].setPotential(200.0);
    tNodes[0].getContent()->setPressure(200.0);

    tModel->updateEffectiveConductivity(tTimeStep);

    CPPUNIT_ASSERT(0.0 == tModel->mEffConductivityScale);
    CPPUNIT_ASSERT(0.0 == tModel->mEffectiveConductivity);

    /// - Push bellows into hard stop
    tNodes[0].setPotential(198.0);
    tNodes[0].getContent()->setPressure(198.0);

    tModel->updateEffectiveConductivity(tTimeStep);

    CPPUNIT_ASSERT(0.0 == tModel->mEffConductivityScale);
    CPPUNIT_ASSERT(0.0 == tModel->mEffectiveConductivity);

    /// - Push bellows away from hard stop
    tNodes[0].setPotential(210.0);
    tNodes[0].getContent()->setPressure(210.0);

    tModel->updateEffectiveConductivity(tTimeStep);

    CPPUNIT_ASSERT(0.0 < tModel->mEffConductivityScale && 1.0 > tModel->mEffConductivityScale);
    CPPUNIT_ASSERT(0.0 < tModel->mEffectiveConductivity);

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// - Initialize mFillModePressureThreshold to be > 0.0 (complex effective conductivity scale logic)
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    tConfigData->mFillModePressureThreshold = 1.0E-3;
    tConfigData->mEffCondScaleOneWayRate = 10.0;
    tInputData->mInitialBellowsPosition = 0.0;
    /// - Initialize default test article with nominal initialization data
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Pressure equalized
    tNodes[0].setPotential(200);
    tNodes[0].getContent()->setPressure(200.0);

    tModel->updateEffectiveConductivity(tTimeStep);

    CPPUNIT_ASSERT(0.0 == tModel->mEffConductivityScale);
    CPPUNIT_ASSERT(0.0 == tModel->mEffectiveConductivity);

    /// - Push bellows into hard stop
    tNodes[0].setPotential(198.0);
    tNodes[0].getContent()->setPressure(198.0);

    tModel->updateEffectiveConductivity(tTimeStep);

    CPPUNIT_ASSERT(0.0 == tModel->mEffConductivityScale);
    CPPUNIT_ASSERT(0.0 == tModel->mEffectiveConductivity);

    /// - Push bellows away from hard stop
    tNodes[0].setPotential(210.0);
    tNodes[0].getContent()->setPressure(210.0);

    tModel->updateEffectiveConductivity(tTimeStep);

    double expectedResult = tModel->mEffCondScaleOneWayRate * tTimeStep;
    CPPUNIT_ASSERT(expectedResult == tModel->mEffConductivityScale);
    CPPUNIT_ASSERT(0.0 < tModel->mEffectiveConductivity);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for effective conductivity - bellows nominal position
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccum::testEffConductivityNom()
{
    UT_RESULT;

    /// - Initialize mFillModePressureThreshold to be 0.0 (simple effective conductivity scale logic)
    tConfigData->mFillModePressureThreshold = 0.0;
    tConfigData->mEffCondScaleOneWayRate = 0.0;
    tInputData->mInitialBellowsPosition = 0.75;
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    tNodes[0].setPotential(200.0);
    tNodes[0].getContent()->setPressure(200.0);

    tModel->updateEffectiveConductivity(tTimeStep);

    CPPUNIT_ASSERT(1.0 == tModel->mEffConductivityScale);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tModel->mLiqCapacitance / tTimeStep, tModel->mEffectiveConductivity, tTolerance);

    /// - Initialize mFillModePressureThreshold to be > 0.0 (complex effective conductivity scale logic)
    tConfigData->mFillModePressureThreshold = 1.0E-4;
    tConfigData->mEffCondScaleOneWayRate = 2.0;
    tInputData->mInitialBellowsPosition = 0.75;
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    tNodes[0].setPotential(200.0);
    tNodes[0].getContent()->setPressure(200.0);

    tModel->updateEffectiveConductivity(tTimeStep);

    CPPUNIT_ASSERT(1.0 == tModel->mEffConductivityScale);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tModel->mLiqCapacitance / tTimeStep, tModel->mEffectiveConductivity, tTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for effective conductivity - bellows within the conductivity scale deadband
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccum::testEffConductivityDeadBand()
{
    UT_RESULT;

    /// - initialize with nominal data so that volume calculation will be valid.
    tInputData->mInitialBellowsPosition = 0.5;
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - set volume to be in the dead band in lower bellows range, so that the conductivity scale will be used.
    double currentLiquidVolume = (tModel->mMinDeadBandVol - tModel->mMinChamberVol) / 2.0 + tModel->mMinChamberVol;
    tInputData->mInitialBellowsPosition = (currentLiquidVolume - tModel->mMinChamberVol) /
            tModel->mActiveVolRange;

    ///- re-initialize model with new liquid volume.
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    tNodes[0].setPotential(200.0);
    tNodes[0].getContent()->setPressure(200.0);

    tModel->updateEffectiveConductivity(tTimeStep);

    double scale = (tModel->mLiquidVolume - tModel->mMinChamberVol) / (tModel->mMinDeadBandVol - tModel->mMinChamberVol);
    double conductivityResult = tModel->mLiqCapacitance / tTimeStep * scale;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(conductivityResult, tModel->mEffectiveConductivity, tTolerance);

    ///////////////////////////////////////////
    /// - Test dead band at other end or range.
    ///////////////////////////////////////////

    /// - set volume to be in the dead band in bellows upper range, so that the conductivity scale will be used.
    currentLiquidVolume = tModel->mMaxChamberVol - ((tModel->mMinDeadBandVol - tModel->mMinChamberVol) / 2.0);
    tInputData->mInitialBellowsPosition = (currentLiquidVolume - tModel->mMinChamberVol) /
            tModel->mActiveVolRange;

        ///- re-initialize model with new liquid volume.
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    tModel->mInternalFluid->setPressure(200.0);
    tModel->updateEffectiveConductivity(tTimeStep);

    scale = (tModel->mPressurizerVolume - tModel->mMinChamberVol) / (tModel->mMinDeadBandVol - tModel->mMinChamberVol);
    conductivityResult = tModel->mLiqCapacitance / tTimeStep * scale;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(conductivityResult, tModel->mEffectiveConductivity, tTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for effective conductivity - bellows within the conductivity scale deadband
///           mFillModePressureThreshold > 0.0.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccum::testEffCondDeadBandWithFillModeThresh()
{
    UT_RESULT;

    //////////////////////////////////////////////
    /// - Test liquid side dead band (near empty)
    ///   approaching hard stop.
    //////////////////////////////////////////////

    /// - Initialize mFillModePressureThreshold to be > 0.0 (complex effective conductivity scale logic)
    tConfigData->mFillModePressureThreshold = 1.0E-4;
    tConfigData->mEffCondScaleOneWayRate = 2.0;
    /// - initialize with nominal data so that volume calculation will be valid.
    tInputData->mInitialBellowsPosition = 0.5;
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - set volume to be in the dead band in lower bellows range, so that the conductivity scale will be used.
    double currentLiquidVolume = (tModel->mMinDeadBandVol - tModel->mMinChamberVol) / 2.0 + tModel->mMinChamberVol;
    tInputData->mInitialBellowsPosition = (currentLiquidVolume - tModel->mMinChamberVol) /
            tModel->mActiveVolRange;

    ///- re-initialize model with new liquid volume.
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    tNodes[0].setPotential(198.0);
    tNodes[0].getContent()->setPressure(198.0);

    tModel->updateEffectiveConductivity(tTimeStep);

    double scale = (tModel->mLiquidVolume - tModel->mMinChamberVol) / (tModel->mMinDeadBandVol - tModel->mMinChamberVol);
    double conductivityResult = tModel->mLiqCapacitance / tTimeStep * scale;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(conductivityResult, tModel->mEffectiveConductivity, tTolerance);

    //////////////////////////////////////////////
    /// - Test liquid side dead band (near empty)
    ///   leaving hard stop.
    //////////////////////////////////////////////

    /// - Initialize mFillModePressureThreshold to be > 0.0 (complex effective conductivity scale logic)
    tConfigData->mFillModePressureThreshold = 1.0E-4;
    tConfigData->mEffCondScaleOneWayRate = 2.0;
    /// - initialize with nominal data so that volume calculation will be valid.
    tInputData->mInitialBellowsPosition = 0.0;
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - set volume to be in the dead band in lower bellows range, so that the conductivity scale will be used.
    currentLiquidVolume = (tModel->mMinDeadBandVol - tModel->mMinChamberVol) / 2.0 + tModel->mMinChamberVol;
    tInputData->mInitialBellowsPosition = (currentLiquidVolume - tModel->mMinChamberVol) /
            tModel->mActiveVolRange;

    ///- re-initialize model with new liquid volume.
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    /// - Assume conductivity scale is 0.0 because bellows was on empty hard stop.
    tModel->mEffConductivityScale = 0.0;

    tNodes[0].setPotential(210.0);
    tNodes[0].getContent()->setPressure(210.0);

    tModel->updateEffectiveConductivity(tTimeStep);

    scale = tModel->mEffCondScaleOneWayRate * tTimeStep;
    conductivityResult = tModel->mLiqCapacitance / tTimeStep * scale;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(conductivityResult, tModel->mEffectiveConductivity, tTolerance);

    //////////////////////////////////////////////
    /// - Test liquid side dead band (near empty)
    ///   equalized.
    //////////////////////////////////////////////

    /// - Initialize mFillModePressureThreshold to be > 0.0 (complex effective conductivity scale logic)
    tConfigData->mFillModePressureThreshold = 1.0E-4;
    tConfigData->mEffCondScaleOneWayRate = 2.0;
    /// - initialize with nominal data so that volume calculation will be valid.
    tInputData->mInitialBellowsPosition = 0.5;
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - set volume to be in the dead band in lower bellows range, so that the conductivity scale will be used.
    currentLiquidVolume = (tModel->mMinDeadBandVol - tModel->mMinChamberVol) / 2.0 + tModel->mMinChamberVol;
    tInputData->mInitialBellowsPosition = (currentLiquidVolume - tModel->mMinChamberVol) /
            tModel->mActiveVolRange;

    ///- re-initialize model with new liquid volume.
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    tNodes[0].setPotential(200.0);
    tNodes[0].getContent()->setPressure(200.0);

    tModel->updateEffectiveConductivity(tTimeStep);

    scale = (tModel->mLiquidVolume - tModel->mMinChamberVol) / (tModel->mMinDeadBandVol - tModel->mMinChamberVol);
    conductivityResult = tModel->mLiqCapacitance / tTimeStep * scale;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(conductivityResult, tModel->mEffectiveConductivity, tTolerance);

    ////////////////////////////////////////////////////
    /// - Test pressurizer side dead band (near full)
    ///   approaching hard stop.
    ////////////////////////////////////////////////////

    /// - Initialize mFillModePressureThreshold to be > 0.0 (complex effective conductivity scale logic)
    tConfigData->mFillModePressureThreshold = 1.0E-4;
    tConfigData->mEffCondScaleOneWayRate = 2.0;
    /// - initialize with nominal data so that volume calculation will be valid.
    tInputData->mInitialBellowsPosition = 0.5;
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - set volume to be in the dead band in bellows upper range, so that the conductivity scale will be used.
    currentLiquidVolume = tModel->mMaxChamberVol - ((tModel->mMinDeadBandVol - tModel->mMinChamberVol) / 2.0);
    tInputData->mInitialBellowsPosition = (currentLiquidVolume - tModel->mMinChamberVol) /
            tModel->mActiveVolRange;

        ///- re-initialize model with new liquid volume.
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    tNodes[0].setPotential(210.0);
    tNodes[0].getContent()->setPressure(210.0);
    tModel->updateEffectiveConductivity(tTimeStep);

    scale = (tModel->mPressurizerVolume - tModel->mMinChamberVol) / (tModel->mMinDeadBandVol - tModel->mMinChamberVol);
    conductivityResult = tModel->mLiqCapacitance / tTimeStep * scale;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(conductivityResult, tModel->mEffectiveConductivity, tTolerance);

    ////////////////////////////////////////////////////
    /// - Test pressurizer side dead band (near full)
    ///   leaving hard stop.
    ////////////////////////////////////////////////////

    /// - Initialize mFillModePressureThreshold to be > 0.0 (complex effective conductivity scale logic)
    tConfigData->mFillModePressureThreshold = 1.0E-4;
    tConfigData->mEffCondScaleOneWayRate = 2.0;
    /// - initialize with nominal data so that volume calculation will be valid.
    tInputData->mInitialBellowsPosition = 1.0;
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - set volume to be in the dead band in bellows upper range, so that the conductivity scale will be used.
    currentLiquidVolume = tModel->mMaxChamberVol - ((tModel->mMinDeadBandVol - tModel->mMinChamberVol) / 2.0);
    tInputData->mInitialBellowsPosition = (currentLiquidVolume - tModel->mMinChamberVol) /
            tModel->mActiveVolRange;

        ///- re-initialize model with new liquid volume.
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    /// - Assume conductivity scale is 0.0 because bellows was on full hard stop.
    tModel->mEffConductivityScale = 0.0;

    tNodes[0].setPotential(198.0);
    tNodes[0].getContent()->setPressure(198.0);
    tModel->updateEffectiveConductivity(tTimeStep);

    scale = tModel->mEffCondScaleOneWayRate * tTimeStep;
    conductivityResult = tModel->mLiqCapacitance / tTimeStep * scale;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(conductivityResult, tModel->mEffectiveConductivity, tTolerance);

    ////////////////////////////////////////////////////
    /// - Test pressurizer side dead band (near full)
    ///   equalized.
    ////////////////////////////////////////////////////

    /// - Initialize mFillModePressureThreshold to be > 0.0 (complex effective conductivity scale logic)
    tConfigData->mFillModePressureThreshold = 1.0E-4;
    tConfigData->mEffCondScaleOneWayRate = 2.0;
    /// - initialize with nominal data so that volume calculation will be valid.
    tInputData->mInitialBellowsPosition = 0.5;
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - set volume to be in the dead band in bellows upper range, so that the conductivity scale will be used.
    currentLiquidVolume = tModel->mMaxChamberVol - ((tModel->mMinDeadBandVol - tModel->mMinChamberVol) / 2.0);
    tInputData->mInitialBellowsPosition = (currentLiquidVolume - tModel->mMinChamberVol) /
            tModel->mActiveVolRange;

        ///- re-initialize model with new liquid volume.
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    tNodes[0].setPotential(200.0);
    tNodes[0].getContent()->setPressure(200.0);
    tModel->updateEffectiveConductivity(tTimeStep);

    scale = (tModel->mPressurizerVolume - tModel->mMinChamberVol) / (tModel->mMinDeadBandVol - tModel->mMinChamberVol);
    conductivityResult = tModel->mLiqCapacitance / tTimeStep * scale;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(conductivityResult, tModel->mEffectiveConductivity, tTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for effective conductivity - bellows stuck due to malf
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccum::testEffConductivityStuckBellows()
{
    UT_RESULT;

    tInputData->mInitialBellowsPosition = 0.5;
    /// - Initialize default test article with nominal initialization data
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    tModel->mMalfBellowsStickFlag = true;

    tNodes[0].setPotential(200.0);
    tNodes[0].getContent()->setPressure(200.0);

    tModel->step(tTimeStep);
    tModel->computeFlows(tTimeStep);
    tModel->transportFlows(tTimeStep);
    // update conductivity after computeFlows (next iteration), so that malfunction has been processed.
    tModel->updateEffectiveConductivity(tTimeStep);

    CPPUNIT_ASSERT(0.0 == tModel->mEffectiveConductivity);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for effective conductivity - bad volumes
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccum::testEffConductivityBadVolumes()
{
    UT_RESULT;

    /// - purely for code coverage. This situation should never happen,
    ///   but the divide by 0.0 is protected against regardless.
    /// - this could happen if a user modifies the volumes incorrectly during runtime.
    tInputData->mInitialBellowsPosition = 0.75;
    /// - Initialize default test article with nominal initialization data
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    /// - udpate conductivity to get first computed value.
    tModel->updateEffectiveConductivity(tTimeStep);

    /// - force a divide by zero condition inside "updateEffConductivityScale"
    tModel->mMinDeadBandVol = tModel->mMinChamberVol;
    double previousConductivity = tModel->mEffectiveConductivity;
    tModel->updateEffectiveConductivity(tTimeStep);

    CPPUNIT_ASSERT(previousConductivity == tModel->mEffectiveConductivity);

    /// - purely for code coverage. This situation should never happen,
    ///   but the divide by 0.0 is protected against regardless.
    /// - this could happen if a user modifies the volumes incorrectly during runtime.
    tInputData->mInitialBellowsPosition = 0.75;
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    double previousBellowsPosition = tModel->mBellowsPosition;
    /// - force a divide by zero condition inside "updateLiqChamber"
    tModel->mActiveVolRange = 0.0;

    tModel->step(tTimeStep);
    tModel->computeFlows(tTimeStep);
    tModel->transportFlows(tTimeStep);

    CPPUNIT_ASSERT(previousBellowsPosition == tModel->mBellowsPosition);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test compute conductivity - max conductivity == 0 and dt == 0.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccum::testComputeConductivity()
{
    UT_RESULT;

    /// - Test the debug situation where the link has been closed off. This is only
    ///   applicable to derived classes, but still tested here.
    tInputData->mInitialBellowsPosition = 0.75;
    /// - Initialize default test article with nominal initialization data
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    /// - Pass in max conductivity of 0.0 and min conductivity > 0.0.
    double computedConductivity = tModel->computeConductivity(0.002,       // capacitance
                                                              0.0,         // max conductivity
                                                              0.01,        // current conductivity
                                                              0.0001,      // minimum conductivity
                                                              tTimeStep);  // dt
    CPPUNIT_ASSERT(0.0 == computedConductivity);
    /// - Pass in timestep = 0.0 for code coverage.
    computedConductivity = tModel->computeConductivity(0.002,       // capacitance
                                                       0.0,         // max conductivity
                                                       0.01,        // current conductivity
                                                       0.0001,      // minimum conductivity
                                                       0.0);  // dt
    CPPUNIT_ASSERT(0.01 == computedConductivity);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Temperature - in flow
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccum::testTemperatureInFlow()
{
    UT_RESULT;

    tInputData->mInitialBellowsPosition = 0.5;
    tInputData->mLiquidFluidInputData->mTemperature = 285.0;
    /// - Initialize default test article with nominal initialization data
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - test warmer temperatures coming in
    tNodes[0].setPotential(200.1);
    tNodes[0].getContent()->setPressure(200.1);
    tNodes[0].getContent()->setTemperature(300.0);
    tNodes[0].resetFlows();
    tNodes[0].getOutflow()->setTemperature(350.0);

    double previousTemperature = tModel->mInternalFluid->getTemperature();
    double previousEnthalpy    = tModel->mInternalFluid->getSpecificEnthalpy();
    double previousMass        = tModel->mInternalFluid->getMass();

    tModel->updateEffectiveConductivity(tTimeStep);
    tModel->step(tTimeStep);
    tModel->computeFlows(tTimeStep);
    tModel->transportFlows(tTimeStep);

    /// @test    Inflow uses the node's outflow fluid, not its contents.
    double deltaMass    = tModel->mFlowRate * tTimeStep;
    double expectedMass = previousMass + deltaMass;
    double expectedEnthalpy = (previousEnthalpy * previousMass
            + deltaMass * tNodes[0].getOutflow()->getSpecificEnthalpy()) / expectedMass;
    CPPUNIT_ASSERT(previousTemperature < tModel->mInternalFluid->getTemperature());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,     tModel->mInternalFluid->getMass(),             FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedEnthalpy, tModel->mInternalFluid->getSpecificEnthalpy(), FLT_EPSILON);

    /// - test cooler temperatures coming in
    tInputData->mInitialBellowsPosition = 0.5;
    tInputData->mLiquidFluidInputData->mTemperature = 285.0;
    /// - Initialize default test article with nominal initialization data
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    tNodes[0].getContent()->setTemperature(274.0);
    tNodes[0].resetFlows();
    previousTemperature = tModel->mInternalFluid->getTemperature();

    tModel->step(tTimeStep);
    tModel->computeFlows(tTimeStep);
    tModel->transportFlows(tTimeStep);

    CPPUNIT_ASSERT(previousTemperature > tModel->mInternalFluid->getTemperature());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Temperature - out flow
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccum::testTemperatureOutFlow()
{
    UT_RESULT;

    tInputData->mInitialBellowsPosition = 0.5;
    tInputData->mLiquidFluidInputData->mTemperature = 285.0;
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    tNodes[0].setPotential(199.9);
    tNodes[0].getContent()->setPressure(199.9);
    tNodes[0].getContent()->setTemperature(300.0);

    double previousTemperature = tModel->mInternalFluid->getTemperature();

    tModel->updateEffectiveConductivity(tTimeStep);
    tModel->step(tTimeStep);
    tModel->computeFlows(tTimeStep);
    tModel->transportFlows(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(previousTemperature, tModel->mInternalFluid->getTemperature(), tTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Temperature - thermal interface
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccum::testTemperatureThermal()
{
    UT_RESULT;

    tInputData->mInitialBellowsPosition = 0.5;
    tInputData->mLiquidFluidInputData->mTemperature = 285.0;
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    tNodes[0].setPotential(200);
    tNodes[0].getContent()->setPressure(200);
    tNodes[0].getContent()->setTemperature(285);

    double previousTemperature = tModel->mInternalFluid->getTemperature();

    tModel->mLiquidHousingQ = 0.001;
    tModel->step(tTimeStep);
    tModel->computeFlows(tTimeStep);
    tModel->transportFlows(tTimeStep);

    CPPUNIT_ASSERT(previousTemperature < tModel->mInternalFluid->getTemperature());

    previousTemperature = tModel->mInternalFluid->getTemperature();

    tModel->mLiquidHousingQ = -0.001;
    tModel->step(tTimeStep);
    tModel->computeFlows(tTimeStep);
    tModel->transportFlows(tTimeStep);

    CPPUNIT_ASSERT(previousTemperature > tModel->mInternalFluid->getTemperature());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Bellows edit
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccum::testBellowsEdit()
{
    UT_RESULT;

    tInputData->mInitialBellowsPosition = 0.5;
    tInputData->mLiquidFluidInputData->mTemperature = 283.0;
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    // Test case where temperature is updated normally.
    tModel->mEditBellowsPosition = 0.55;
    tModel->mEditBellowsRate = 0.016;
    tModel->mEditBellowsFlag = true;
    tModel->mEditsAreReactive = true;
    tModel->mSimultaneousEditsInProgress = false;

    int numIterations = int((tModel->mEditBellowsPosition - tInputData->mInitialBellowsPosition)
            / tModel->mEditBellowsRate / tTimeStep + 5);

    for (int i=0;i<numIterations;i++)
    {
        tModel->step(tTimeStep);
        tModel->computeFlows(tTimeStep);
        tModel->transportFlows(tTimeStep);
    }

    double newLiquidVolume = tModel->mEditBellowsPosition *
            tModel->mActiveVolRange + tModel->mMinChamberVol;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tModel->mEditBellowsPosition, tModel->mBellowsPosition, tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(newLiquidVolume, tModel->mLiquidVolume, tTolerance);
    CPPUNIT_ASSERT(tModel->mEditBellowsTimer < tModel->mEditHoldTime);

    numIterations = int(tModel->mEditHoldTime / tTimeStep + 5);

    for (int i=0;i<numIterations;i++)
    {
        tModel->step(tTimeStep);
        tModel->computeFlows(tTimeStep);
        tModel->transportFlows(tTimeStep);
    }

    CPPUNIT_ASSERT(0.0 == tModel->mEditBellowsTimer);
    CPPUNIT_ASSERT(false == tModel->mEditBellowsFlag);

    // Test case where temperature is forced to be static due to edits not being reactive (code coverage).
    tInputData->mInitialBellowsPosition = 0.5;
    tInputData->mLiquidFluidInputData->mTemperature = 283.0;
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    tModel->mEditBellowsPosition = 0.55;
    tModel->mEditBellowsRate = 0.016;
    tModel->mEditBellowsFlag = true;
    tModel->mEditsAreReactive = false;
    tModel->mSimultaneousEditsInProgress = false;

    numIterations = int((tModel->mEditBellowsPosition - tInputData->mInitialBellowsPosition)
            / tModel->mEditBellowsRate / tTimeStep + 5);

    for (int i=0;i<numIterations;i++)
    {
        tModel->step(tTimeStep);
        tModel->computeFlows(tTimeStep);
        tModel->transportFlows(tTimeStep);
    }

    newLiquidVolume = tModel->mEditBellowsPosition *
            tModel->mActiveVolRange + tModel->mMinChamberVol;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tModel->mEditBellowsPosition, tModel->mBellowsPosition, tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(newLiquidVolume, tModel->mLiquidVolume, tTolerance);
    CPPUNIT_ASSERT(tModel->mEditBellowsTimer < tModel->mEditHoldTime);

    numIterations = int(tModel->mEditHoldTime / tTimeStep + 5);

    for (int i=0;i<numIterations;i++)
    {
        tModel->step(tTimeStep);
        tModel->computeFlows(tTimeStep);
        tModel->transportFlows(tTimeStep);
    }

    CPPUNIT_ASSERT(0.0 == tModel->mEditBellowsTimer);
    CPPUNIT_ASSERT(false == tModel->mEditBellowsFlag);

    // Test case where temperature is forced to be static due to simultaneous edits (code coverage).
    tInputData->mInitialBellowsPosition = 0.5;
    tInputData->mLiquidFluidInputData->mTemperature = 283.0;
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    tModel->mEditBellowsPosition = 0.55;
    tModel->mEditBellowsRate = 0.016;
    tModel->mEditBellowsFlag = true;
    tModel->mEditsAreReactive = true;
    tModel->mSimultaneousEditsInProgress = true;

    numIterations = int((tModel->mEditBellowsPosition - tInputData->mInitialBellowsPosition)
            / tModel->mEditBellowsRate / tTimeStep + 5);

    for (int i=0;i<numIterations;i++)
    {
        tModel->step(tTimeStep);
        tModel->computeFlows(tTimeStep);
        tModel->transportFlows(tTimeStep);
    }

    newLiquidVolume = tModel->mEditBellowsPosition *
            tModel->mActiveVolRange + tModel->mMinChamberVol;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tModel->mEditBellowsPosition, tModel->mBellowsPosition, tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(newLiquidVolume, tModel->mLiquidVolume, tTolerance);
    CPPUNIT_ASSERT(tModel->mEditBellowsTimer < tModel->mEditHoldTime);

    numIterations = int(tModel->mEditHoldTime / tTimeStep + 5);

    for (int i=0;i<numIterations;i++)
    {
        tModel->step(tTimeStep);
        tModel->computeFlows(tTimeStep);
        tModel->transportFlows(tTimeStep);
    }

    CPPUNIT_ASSERT(0.0 == tModel->mEditBellowsTimer);
    CPPUNIT_ASSERT(false == tModel->mEditBellowsFlag);

    // reset simultaneous edits flag
    tModel->mSimultaneousEditsInProgress = false;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for temperature edit
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccum::testTemperatureEdit()
{
    UT_RESULT;

    // test normal case
    tInputData->mLiquidFluidInputData->mTemperature = 283.0;
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    tModel->mEditTemperatureValue = 290.0;
    tModel->mEditTemperatureFlag = true;

    tModel->step(tTimeStep);
    tModel->computeFlows(tTimeStep);
    tModel->transportFlows(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tModel->mEditTemperatureValue, tModel->mInternalFluid->getTemperature(), tTolerance);

    // test with malf. Malfs do not stop temp edit.
    tModel->mMalfBellowsStickFlag = true;

    tModel->mEditTemperatureValue = 285.0;
    tModel->mEditTemperatureFlag = true;

    tModel->step(tTimeStep);
    tModel->computeFlows(tTimeStep);
    tModel->transportFlows(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tModel->mEditTemperatureValue, tModel->mInternalFluid->getTemperature(), tTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for malfunction = stick bellows
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccum::testMalfStickBellows()
{
    UT_RESULT;

    tInputData->mLiquidFluidInputData->mPressure = 200.0;
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    double prevBellowsPosition = tModel->mBellowsPosition;

    tNodes[0].setPotential(195.0);
    tNodes[0].getContent()->setPressure(195.0);

    tModel->mMalfBellowsStickFlag = true;

    tModel->step(tTimeStep);
    tModel->computeFlows(tTimeStep);
    tModel->transportFlows(tTimeStep);

    CPPUNIT_ASSERT(prevBellowsPosition == tModel->mBellowsPosition);
    CPPUNIT_ASSERT(true == tModel->mBellowsStuck);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for malfunction = stick bellows to percent
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccum::testMalfStickBellowsToPos()
{
    UT_RESULT;

    tInputData->mInitialBellowsPosition = 0.5;
    tInputData->mLiquidFluidInputData->mPressure = 200.0;
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    double prevBellowsPosition = tModel->mBellowsPosition;

    tNodes[0].setPotential(200.1);
    tNodes[0].getContent()->setPressure(200.1);

    tModel->mMalfBellowsStickToPosFlag = true;
    tModel->mMalfBellowsStickToPosValue = 0.49;
    tModel->mMalfBellowsStickToPosRate = 0.016;

    for (double i=0;i<15;i++)
    {
        tModel->step(tTimeStep);
        tModel->computeFlows(tTimeStep);
        tModel->transportFlows(tTimeStep);
    }
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tModel->mMalfBellowsStickToPosValue, tModel->mBellowsPosition, tTolerance);
    CPPUNIT_ASSERT(true == tModel->mBellowsStuck);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for pressurizer bellows override
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccum::testPressurizerBellowsOride()
{
    UT_RESULT;

    /// - Test place holder function for derived pressurizer bellows override
    ///   For code coverage only
    tInputData->mInitialBellowsPosition = 0.5;
    tInputData->mLiquidFluidInputData->mPressure = 200.0;
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    double prevBellowsPos = tModel->mBellowsPosition;
    tModel->mPressurizerOrideBellowsFlag = true;

    tModel->step(tTimeStep);
    tModel->computeFlows(tTimeStep);
    tModel->transportFlows(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(prevBellowsPos, tModel->mBellowsPosition, tTolerance);

    tModel->mPressurizerOrideBellowsFlag = false;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for hold accumulator
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccum::testHoldFlags()
{
    UT_RESULT;

    tInputData->mInitialBellowsPosition = 0.5;
    tInputData->mLiquidFluidInputData->mPressure = 200.0;
    tInputData->mLiquidFluidInputData->mTemperature = 283;
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    tNodes[0].setPotential(195.0);
    tNodes[0].getContent()->setPressure(195.0);

    double previousMass = tModel->mInternalFluid->getMass();

    tModel->mHoldAccumFlag = true;

    tModel->step(tTimeStep);
    tModel->computeFlows(tTimeStep);
    tModel->transportFlows(tTimeStep);

    CPPUNIT_ASSERT(0.5 == tModel->mBellowsPosition);
    CPPUNIT_ASSERT(283.0 == tModel->mInternalFluid->getTemperature());
    CPPUNIT_ASSERT(previousMass == tModel->mInternalFluid->getMass());

    tModel->mHoldAccumFlag = false;
    tModel->mHoldTemperatureFlag = true;
    tModel->mLiquidHousingQ = 1.0;

    double prevTemperature = tModel->mInternalFluid->getTemperature();

    tModel->step(tTimeStep);
    tModel->computeFlows(tTimeStep);
    tModel->transportFlows(tTimeStep);

    CPPUNIT_ASSERT(283.0 == tModel->mInternalFluid->getTemperature());
    CPPUNIT_ASSERT(previousMass != tModel->mInternalFluid->getMass());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for testCheckSpecificPortRules
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccum::testCheckSpecificPortRules()
{
    UT_RESULT;

    int port = 0;
    int nodeNum = 0;

    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    /// @test   Exception on port 0 having liquid content.
    CPPUNIT_ASSERT(!tModel->checkSpecificPortRules(port, nodeNum));

    /// @test   Exception on port 1 being ground.
    port = 1;
    nodeNum = 1;
    CPPUNIT_ASSERT(!tModel->checkSpecificPortRules(port, nodeNum));

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for testCheckSpecificPortRules for Port 0 containing gas instead of liquid.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccum::testCheckSpecificPortRulesGasPort0()
{
    UT_RESULT;

    GunnsNodeList  nodeList;
    GunnsFluidNode nodes[2];
    double* badMassFractions;
    PolyFluidInputData* badLiquidFluidInputData;
    FriendlyGunnsFluidAccum*  model;
    GunnsFluidAccumConfigData* configData;
    std::vector<GunnsBasicLink*> links;

    nodeList.mNumNodes = 2;
    nodeList.mNodes = nodes;

    badMassFractions = new double[2];
    badMassFractions[0] = 0.0;       // gas instead of liquid.
    badMassFractions[1] = 1.0;

    badLiquidFluidInputData = new PolyFluidInputData(283.0,         //temperature
                                          200.0,                  //pressure
                                          0.0,                    //flowRate
                                          0.0,                    //mass
                                          badMassFractions);               //massFractions

    /// - Have to initialize the nodes with the fluid configs (normally done by GUNNS)
    /// - because there are only 2 nodes. Node 1 is the vacuum boundary node, Node 0 is the
    ///   liquid node (port 1 of the accumulator link).
    nodes[0].initialize("Node0", tLiquidFluidConfigData, badLiquidFluidInputData);
    nodes[1].initialize("Node1", tLiquidFluidConfigData, 0);

    nodes[0].setPotential(200.0);
    nodes[1].setPotential(0.0);

    nodes[0].getContent()->initialize(*tLiquidFluidConfigData, *badLiquidFluidInputData);

    configData = new GunnsFluidAccumConfigData (
            tLinkName,
            &nodeList,
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
            tFillModePressureThreshold);

    GunnsFluidAccumInputData*  badInputData;

    badInputData = new GunnsFluidAccumInputData (false, 0.0, tInitialBellowsPosition, badLiquidFluidInputData);

    model = new FriendlyGunnsFluidAccum;

    /// @test   Initialization exception because port 0 does not having liquid content.
    CPPUNIT_ASSERT_THROW(model->initialize(*configData, *badInputData, links, tPort0, tPort1), TsInitializationException);

    delete (model);
    delete (badInputData);
    delete (configData);
    delete (badLiquidFluidInputData);
    delete[] (badMassFractions);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test getters and setters
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccum::testAccessMethods()
{
    UT_RESULT;

    tInputData->mInitialBellowsPosition = 0.5;
    tInputData->mLiquidFluidInputData->mPressure = 200.0;
    tInputData->mLiquidFluidInputData->mTemperature = 283;
    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5 , tModel->getBellowsPosition(), 0.0);

    /// @test   The stuck bellows in place malf is set.
    tModel->setMalfBellowsStick(true);
    CPPUNIT_ASSERT(tModel->mMalfBellowsStickFlag);

    /// @test   The stuck bellows in place malf is reset.
    tModel->setMalfBellowsStick();
    CPPUNIT_ASSERT(!tModel->mMalfBellowsStickFlag);

    /// @test   The stuck bellows to position malf parameters are set.
    tModel->setMalfBellowsStickToPos(true, 1.0, 0.1);
    CPPUNIT_ASSERT(tModel->mMalfBellowsStickToPosFlag);
    CPPUNIT_ASSERT(1.0 == tModel->mMalfBellowsStickToPosValue);
    CPPUNIT_ASSERT(0.1 == tModel->mMalfBellowsStickToPosRate);

    /// @test   The stuck bellows to position malf parameters are reset.
    tModel->setMalfBellowsStickToPos();
    CPPUNIT_ASSERT(!tModel->mMalfBellowsStickToPosFlag);
    CPPUNIT_ASSERT(0.0 == tModel->mMalfBellowsStickToPosValue);
    CPPUNIT_ASSERT(0.0 == tModel->mMalfBellowsStickToPosRate);

    /// @test   The setTemperature edit
    tModel->setTemperature(310.0);
    CPPUNIT_ASSERT(tModel->mEditTemperatureFlag);
    CPPUNIT_ASSERT(310.0 == tModel->mEditTemperatureValue);

    /// @test   The get usable mass.
    const double expectedM = tModel->mInternalFluid->getMass() * (1.0 - tModel->mMinChamberVol
                                                                      / tModel->mLiquidVolume);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedM, tModel->getUsableMass(), DBL_EPSILON);

    /// @test   The spring coefficients setter with args.
    tModel->setSpringCoeffs(1.0, 2.0, 3.0);
    CPPUNIT_ASSERT(1.0 == tModel->mSpringCoeff0);
    CPPUNIT_ASSERT(2.0 == tModel->mSpringCoeff1);
    CPPUNIT_ASSERT(3.0 == tModel->mSpringCoeff2);

    /// @test   The spring coefficients setter with no args.
    tModel->setSpringCoeffs();
    CPPUNIT_ASSERT(0.0 == tModel->mSpringCoeff0);
    CPPUNIT_ASSERT(0.0 == tModel->mSpringCoeff1);
    CPPUNIT_ASSERT(0.0 == tModel->mSpringCoeff2);

    /// @test   The acceleration pressure head setter.
    tModel->setAccelPressureHead(42.0);
    CPPUNIT_ASSERT(42.0 == tModel->mAccelPressureHead);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAccum::testRestart()
{
    UT_RESULT;

    tModel->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Reset of non-checkpoint and non-config-derived data
    tModel->mFillMode   = GunnsFluidAccum::DRAINING;
    tModel->mBellowsZone = GunnsFluidAccum::FULL;

    tModel->restart();

    CPPUNIT_ASSERT(GunnsFluidAccum::EQUALIZED == tModel->mFillMode);
    CPPUNIT_ASSERT(GunnsFluidAccum::MIDDLE    == tModel->mBellowsZone);

    UT_PASS_LAST;
}
