/**
@file
@brief     GUNNS Gas Turbine Model implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   ((aspects/fluid/potential/GunnsGasFanCurve.o)
    (software/exceptions/TsInitializationException.o))
**************************************************************************************************/

#include "GunnsGasTurbine.hh"

#include "software/exceptions/TsInitializationException.hh"
#include "math/MsMath.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     name                 (--)             Name of object.
/// @param[in,out] nodes                (--)             Pointer to nodes.
/// @param[in]     maxConductivity      (m2)             Max conductivity.
/// @param[in]     expansionScaleFactor (--)             Scale factor for isentropic gas cooling.
/// @param[in]     referenceTemp        (K)              Reference fluid temperature for corrected mass flow
/// @param[in]     referencePress       (--)             Reference fluid pressure for corrected mass flow
/// @param[in]     maxFlowLowSpeed      (kg/s)           Low Speed maximum corrected flow rate
/// @param[in]     maxFlowHighSpeed     (kg/s)           High Speed maximum corrected flow rate
/// @param[in]     coeffLowSpeed1       (--)             Low speed 1st curve fit coefficient
/// @param[in]     coeffHighSpeed1      (--)             High speed 1st curve fit coefficient
/// @param[in]     coeffLowSpeed2       (--)             Low speed 2nd curve fit coefficient
/// @param[in]     coeffHighSpeed2      (--)             High speed 2nd curve fit coefficient
/// @param[in]     efficiencyCoeffLow0  (--)             Low Speed Reference efficiency curve 0th-order coefficient
/// @param[in]     efficiencyCoeffLow1  (--)             Low Speed Reference efficiency curve 1st-order coefficient
/// @param[in]     efficiencyCoeffLow2  (--)             Low Speed Reference efficiency curve 2nd-order coefficient
/// @param[in]     efficiencyCoeffLow3  (--)             Low Speed Reference efficiency curve 3rd-order coefficient
/// @param[in]     efficiencyCoeffLow4  (--)             Low Speed Reference efficiency curve 4th-order coefficient
/// @param[in]     efficiencyCoeffLow5  (--)             Low Speed Reference efficiency curve 5th-order coefficient
/// @param[in]     efficiencyCoeffHigh0 (--)             High Speed Reference efficiency curve 0th-order coefficient
/// @param[in]     efficiencyCoeffHigh1 (--)             High Speed Reference efficiency curve 1st-order coefficient
/// @param[in]     efficiencyCoeffHigh2 (--)             High Speed Reference efficiency curve 2nd-order coefficient
/// @param[in]     efficiencyCoeffHigh3 (--)             High Speed Reference efficiency curve 3rd-order coefficient
/// @param[in]     efficiencyCoeffHigh4 (--)             High Speed Reference efficiency curve 4th-order coefficient
/// @param[in]     efficiencyCoeffHigh5 (--)             High Speed Reference efficiency curve 5th-order coefficient
/// @param[in]     minEffLimitLowSpeed  (--)             Min PR that the low speed efficiency curve can be evaluated at
/// @param[in]     minEffLimitHighSpeed (--)             Min PR that the high speed efficiency curve can be evaluated at
/// @param[in]     maxEffLimitLowSpeed  (--)             Max PR that the low speed efficiency curve can be evaluated at
/// @param[in]     maxEffLimitHighSpeed (--)             Max PR that the high speed efficiency curve can be evaluated at
/// @param[in]     correctedSpeedLow    (revolution/min) Lowest corrected speed on turbine map.
/// @param[in]     correctedSpeedHigh   (revolution/min) Highest corrected speed on turbine map.
/// @param[in]     filterGain           (--)             Low pass filter gain to ensure stability.
/// @param[in]     driveRatio           (--)             Gear ratio of motor to impeller speed.
/// @param[in]     thermalLength        (m)              Impeller length for thermal convection.
/// @param[in]     thermalDiameter      (m)              Impeller inner diameter for thermal convection.
/// @param[in]     surfaceRoughness     (m)              Impeller wall surface roughness for convection.
///
/// @details  Default constructs this GUNNS Gas Turbine link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsGasTurbineConfigData::GunnsGasTurbineConfigData(const std::string& name,
                                                     GunnsNodeList*     nodes,
                                                     const double       maxConductivity,
                                                     const double       expansionScaleFactor,
                                                     const double       referenceTemp,
                                                     const double       referencePress,
                                                     const double       maxFlowLowSpeed,
                                                     const double       maxFlowHighSpeed,
                                                     const double       coeffLowSpeed1,
                                                     const double       coeffHighSpeed1,
                                                     const double       coeffLowSpeed2,
                                                     const double       coeffHighSpeed2,
                                                     const double       efficiencyCoeffLow0,
                                                     const double       efficiencyCoeffLow1,
                                                     const double       efficiencyCoeffLow2,
                                                     const double       efficiencyCoeffLow3,
                                                     const double       efficiencyCoeffLow4,
                                                     const double       efficiencyCoeffLow5,
                                                     const double       efficiencyCoeffHigh0,
                                                     const double       efficiencyCoeffHigh1,
                                                     const double       efficiencyCoeffHigh2,
                                                     const double       efficiencyCoeffHigh3,
                                                     const double       efficiencyCoeffHigh4,
                                                     const double       efficiencyCoeffHigh5,
                                                     const double       minEffLimitLowSpeed,
                                                     const double       minEffLimitHighSpeed,
                                                     const double       maxEffLimitLowSpeed,
                                                     const double       maxEffLimitHighSpeed,
                                                     const double       correctedSpeedLow,
                                                     const double       correctedSpeedHigh,
                                                     const double       filterGain,
                                                     const double       driveRatio,
                                                     const double       thermalLength,
                                                     const double       thermalDiameter,
                                                     const double       surfaceRoughness)
    :
    GunnsFluidConductorConfigData(name, nodes, maxConductivity, expansionScaleFactor),
    mReferenceTemp      (referenceTemp),
    mReferencePress     (referencePress),
    mMaxFlowLowSpeed    (maxFlowLowSpeed),
    mMaxFlowHighSpeed   (maxFlowHighSpeed),
    mCoeffLowSpeed1     (coeffLowSpeed1),
    mCoeffHighSpeed1    (coeffHighSpeed1),
    mCoeffLowSpeed2     (coeffLowSpeed2),
    mCoeffHighSpeed2    (coeffHighSpeed2),
    mEffCoeffLowSpeed0  (efficiencyCoeffLow0),
    mEffCoeffLowSpeed1  (efficiencyCoeffLow1),
    mEffCoeffLowSpeed2  (efficiencyCoeffLow2),
    mEffCoeffLowSpeed3  (efficiencyCoeffLow3),
    mEffCoeffLowSpeed4  (efficiencyCoeffLow4),
    mEffCoeffLowSpeed5  (efficiencyCoeffLow5),
    mEffCoeffHighSpeed0 (efficiencyCoeffHigh0),
    mEffCoeffHighSpeed1 (efficiencyCoeffHigh1),
    mEffCoeffHighSpeed2 (efficiencyCoeffHigh2),
    mEffCoeffHighSpeed3 (efficiencyCoeffHigh3),
    mEffCoeffHighSpeed4 (efficiencyCoeffHigh4),
    mEffCoeffHighSpeed5 (efficiencyCoeffHigh5),
    mMinEffLimLowSpeed  (minEffLimitLowSpeed),
    mMinEffLimHighSpeed (minEffLimitHighSpeed),
    mMaxEffLimLowSpeed  (maxEffLimitLowSpeed),
    mMaxEffLimHighSpeed (maxEffLimitHighSpeed),
    mCorrectedSpeedLow  (correctedSpeedLow),
    mCorrectedSpeedHigh (correctedSpeedHigh),
    mFilterGain         (filterGain),
    mDriveRatio         (driveRatio),
    mThermalLength      (thermalLength),
    mThermalDiameter    (thermalDiameter),
    mSurfaceRoughness   (surfaceRoughness)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Source object to copy.
///
/// @details  Copy constructs this GUNNS Gas Turbine link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsGasTurbineConfigData::GunnsGasTurbineConfigData(const GunnsGasTurbineConfigData& that)
    :
    GunnsFluidConductorConfigData(that),
    mReferenceTemp      (that.mReferenceTemp),
    mReferencePress     (that.mReferencePress),
    mMaxFlowLowSpeed    (that.mMaxFlowLowSpeed),
    mMaxFlowHighSpeed   (that.mMaxFlowHighSpeed),
    mCoeffLowSpeed1     (that.mCoeffLowSpeed1),
    mCoeffHighSpeed1    (that.mCoeffHighSpeed1),
    mCoeffLowSpeed2     (that.mCoeffLowSpeed2),
    mCoeffHighSpeed2    (that.mCoeffHighSpeed2),
    mEffCoeffLowSpeed0  (that.mEffCoeffLowSpeed0),
    mEffCoeffLowSpeed1  (that.mEffCoeffLowSpeed1),
    mEffCoeffLowSpeed2  (that.mEffCoeffLowSpeed2),
    mEffCoeffLowSpeed3  (that.mEffCoeffLowSpeed3),
    mEffCoeffLowSpeed4  (that.mEffCoeffLowSpeed4),
    mEffCoeffLowSpeed5  (that.mEffCoeffLowSpeed5),
    mEffCoeffHighSpeed0 (that.mEffCoeffHighSpeed0),
    mEffCoeffHighSpeed1 (that.mEffCoeffHighSpeed1),
    mEffCoeffHighSpeed2 (that.mEffCoeffHighSpeed2),
    mEffCoeffHighSpeed3 (that.mEffCoeffHighSpeed3),
    mEffCoeffHighSpeed4 (that.mEffCoeffHighSpeed4),
    mEffCoeffHighSpeed5 (that.mEffCoeffHighSpeed5),
    mMinEffLimLowSpeed  (that.mMinEffLimLowSpeed),
    mMinEffLimHighSpeed (that.mMinEffLimHighSpeed),
    mMaxEffLimLowSpeed  (that.mMaxEffLimLowSpeed),
    mMaxEffLimHighSpeed (that.mMaxEffLimHighSpeed),
    mCorrectedSpeedLow  (that.mCorrectedSpeedLow),
    mCorrectedSpeedHigh (that.mCorrectedSpeedHigh),
    mFilterGain         (that.mFilterGain),
    mDriveRatio         (that.mDriveRatio),
    mThermalLength      (that.mThermalLength),
    mThermalDiameter    (that.mThermalDiameter),
    mSurfaceRoughness   (that.mSurfaceRoughness)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Gas Turbine link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsGasTurbineConfigData::~GunnsGasTurbineConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--)             Blockage malfunction flag.
/// @param[in] malfBlockageValue (--)             Blockage malfunction fractional value (0-1).
/// @param[in] motorSpeed        (revolution/min) Initial speed of the motor.
/// @param[in] wallTemperature   (K)              Initial impeller wall temperature.
///
/// @details  Default constructs this GUNNS Gas Turbine link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsGasTurbineInputData::GunnsGasTurbineInputData(const bool   malfBlockageFlag,
                                                   const double malfBlockageValue,
                                                   const double motorSpeed,
                                                   const double wallTemperature)
    :
    GunnsFluidConductorInputData(malfBlockageFlag, malfBlockageValue),
    mMotorSpeed     (motorSpeed),
    mWallTemperature(wallTemperature)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Source object to copy.
///
/// @details  Copy constructs this GUNNS Gas Turbine link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsGasTurbineInputData::GunnsGasTurbineInputData(const GunnsGasTurbineInputData& that)
    :
    GunnsFluidConductorInputData(that),
    mMotorSpeed     (that.mMotorSpeed),
    mWallTemperature(that.mWallTemperature)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Gas Turbine link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsGasTurbineInputData::~GunnsGasTurbineInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Gas Turbine link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsGasTurbine::GunnsGasTurbine()
    :
    GunnsFluidConductor(),
    mReferenceTemp(0.0),
    mReferencePress(0.0),
    mLowSpeedMaxFlow(0.0),
    mHighSpeedMaxFlow(0.0),
    mCoeffLowSpeed1(0.0),
    mCoeffLowSpeed2(0.0),
    mCoeffHighSpeed1(0.0),
    mCoeffHighSpeed2(0.0),
    mCorrectedSpeedLow(0.0),
    mCorrectedSpeedHigh(0.0),
    mMinEffLimLowSpeed(0.0),
    mMinEffLimHighSpeed(0.0),
    mMaxEffLimLowSpeed(0.0),
    mMaxEffLimHighSpeed(0.0),
    mDriveRatio(0.0),
    mFilterGain(0.0),
    mThermalDiameter(0.0),
    mThermalSurfaceArea(0.0),
    mThermalROverD(0.0),
    mMotorSpeed(0.0),
    mWallTemperature(0.0),
    mCurve(),
    mImpellerSpeed(0.0),
    mPressureRatio(0.0),
    mPressureDrop(0.0),
    mEfficiency(0.0),
    mWallHeatFlux(0.0),
    mImpellerTorque(0.0),
    mImpellerPower(0.0),
    mPredictedFlowRate(0.0)
{
    for (int i=0; i<6; ++i) {
        mEffCoeffLowSpeed[i] = 0.0;
        mEffCoeffHighSpeed[i] = 0.0;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Gas Turbine link model.
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsGasTurbine::~GunnsGasTurbine()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData (--) Configuration data.
/// @param[in]     inputData  (--) Input data.
/// @param[in,out] links      (--) Link vector.
/// @param[in]     port0      (--) Nominal inlet port map index.
/// @param[in]     port1      (--) Nominal outlet port map index.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Gas Turbine link model with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsGasTurbine::initialize(const GunnsGasTurbineConfigData&  configData,
                                 const GunnsGasTurbineInputData&   inputData,
                                 std::vector<GunnsBasicLink*>&     links,
                                 const int                         port0,
                                 const int                         port1)
{
    /// - First initialize & validate parent.
    GunnsFluidConductor::initialize(configData, inputData, links, port0, port1);

    /// - Reset initialization status flag.
    mInitFlag           = false;

    /// - Initialize from configuration data.
    mReferenceTemp        = configData.mReferenceTemp;
    mReferencePress       = configData.mReferencePress;
    mLowSpeedMaxFlow      = configData.mMaxFlowLowSpeed;
    mHighSpeedMaxFlow     = configData.mMaxFlowHighSpeed;
    mCoeffLowSpeed1       = configData.mCoeffLowSpeed1;
    mCoeffLowSpeed2       = configData.mCoeffLowSpeed2;
    mCoeffHighSpeed1      = configData.mCoeffHighSpeed1;
    mCoeffHighSpeed2      = configData.mCoeffHighSpeed2;
    mEffCoeffLowSpeed[0]  = configData.mEffCoeffLowSpeed0;
    mEffCoeffLowSpeed[1]  = configData.mEffCoeffLowSpeed1;
    mEffCoeffLowSpeed[2]  = configData.mEffCoeffLowSpeed2;
    mEffCoeffLowSpeed[3]  = configData.mEffCoeffLowSpeed3;
    mEffCoeffLowSpeed[4]  = configData.mEffCoeffLowSpeed4;
    mEffCoeffLowSpeed[5]  = configData.mEffCoeffLowSpeed5;
    mEffCoeffHighSpeed[0] = configData.mEffCoeffHighSpeed0;
    mEffCoeffHighSpeed[1] = configData.mEffCoeffHighSpeed1;
    mEffCoeffHighSpeed[2] = configData.mEffCoeffHighSpeed2;
    mEffCoeffHighSpeed[3] = configData.mEffCoeffHighSpeed3;
    mEffCoeffHighSpeed[4] = configData.mEffCoeffHighSpeed4;
    mEffCoeffHighSpeed[5] = configData.mEffCoeffHighSpeed5;
    mCorrectedSpeedLow    = configData.mCorrectedSpeedLow;
    mCorrectedSpeedHigh   = configData.mCorrectedSpeedHigh;
    mFilterGain           = configData.mFilterGain;
    mDriveRatio           = configData.mDriveRatio;
    mMinEffLimLowSpeed    = configData.mMinEffLimLowSpeed;
    mMinEffLimHighSpeed   = configData.mMinEffLimHighSpeed;
    mMaxEffLimLowSpeed    = configData.mMaxEffLimLowSpeed;
    mMaxEffLimHighSpeed   = configData.mMaxEffLimHighSpeed;

    mThermalDiameter     = configData.mThermalDiameter;
    mThermalSurfaceArea  = configData.mThermalLength * UnitConversion::PI_UTIL * mThermalDiameter;
    if (mThermalSurfaceArea > DBL_EPSILON) {
        mThermalROverD     = configData.mSurfaceRoughness / configData.mThermalDiameter;
    } else {
        mThermalROverD     = 0.0;
    }

    /// - Initialize from input data.
    mMotorSpeed         = inputData.mMotorSpeed;
    mWallTemperature    = inputData.mWallTemperature;

    /// - Initialize remaining state data.
    mWallHeatFlux       = 0.0;
    mImpellerTorque     = 0.0;
    mImpellerSpeed      = 0.0;
    mImpellerPower      = 0.0;
    mEfficiency         = 0.0;
    mPressureRatio      = 0.0;
    mPressureDrop       = 0.0;
    mPredictedFlowRate  = 0.0;

    /// - Create the internal fluid.
    createInternalFluid();

    /// - Validates the link initialization.
    validate();

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag           = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  Validates this GUNNS Gas Turbine Model link model initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsGasTurbine::validate() const
{
    /// - Throw an exception if reference Pressure is non-positive.
    if (mReferencePress < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Reference pressure < DBL_EPSILON.");
    }

    /// - Throw an exception if reference temperature is non-positive.
    if (mReferenceTemp < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Reference temperature < DBL_EPSILON.");
    }

    /// - Throw an exception if drive ratio is non-positive.
    if (mDriveRatio < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Drive ratio < DBL_EPSILON.");
    }

    /// - Throw an exception if either max flow rate is less than zero
    if (mLowSpeedMaxFlow < DBL_EPSILON or mHighSpeedMaxFlow < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Max Flow rate < DBL_EPSILON.");
    }

    /// - Throw an exception if either reference corrected speed is less than zero
    if (mCorrectedSpeedLow < DBL_EPSILON or mCorrectedSpeedHigh < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Reference corrected speed < DBL_EPSILON.");
    }

    /// - Throw an exception if the reference corrected speeds are equal
    if (fabs(mCorrectedSpeedLow - mCorrectedSpeedHigh) < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Reference corrected speeds are equal.");
    }

    /// - Throw an exception if either efficiency limit is less than zero
    if (mMaxEffLimHighSpeed < mMinEffLimHighSpeed or mMaxEffLimLowSpeed < mMinEffLimLowSpeed) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Max efficiency limit < min efficiency limit.");
    }

    /// - Throw an exception if either min efficiency limit > max efficiency limit
    if (mMinEffLimHighSpeed < DBL_EPSILON or mMinEffLimLowSpeed < DBL_EPSILON or mMaxEffLimHighSpeed
            < DBL_EPSILON or mMaxEffLimLowSpeed < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Efficiency limits < DBL_EPSILON.");
    }

    /// - Throw an exception if initial motor speed is negative,
    if (mMotorSpeed < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data", "Motor speed < 0.");
    }

    /// - Throw an exception if initial wall temperature is negative,
    if (mWallTemperature < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data", "Wall temperature < 0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsGasTurbine::restartModel()
{
    /// - Reset the base class.
    GunnsFluidConductor::restartModel();

    /// - Reset non-config & non-checkpointed attributes.
    mImpellerSpeed      = 0.0;
    mImpellerPower      = 0.0;
    mPressureDrop       = 0.0;
    mEfficiency         = 0.0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s)  Time step (not used).
///
/// @details  Updates this GUNNS Gas Turbine link model effective conductivity.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsGasTurbine::updateState(const double dt __attribute__((unused)))
{
    /// - Protect for divide by zero on drive ratio; also setting drive ratio to zero disconnects
    ///   the impeller from the motor, allowing manual control of the impeller speed for tuning.
    if (mDriveRatio > DBL_EPSILON) {
        mImpellerSpeed = mMotorSpeed / mDriveRatio;
    } else {
        GUNNS_WARNING("impeller is disconnected from motor.");
    }
    computeFlowRate();
    if(mPredictedFlowRate >= 0.0){
        mEffectiveConductivity = GunnsFluidUtils::predictConductivity(mPredictedFlowRate,
                                                                      mMinLinearizationPotential,
                                                                      mNodes[0]->getOutflow(),
                                                                      mNodes[1]->getOutflow());
    } else{
        mEffectiveConductivity = mMaxConductivity ;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt       (s)    Time step (not used).
/// @param[in] flowRate (kg/s) Mass flow rate.
///
/// @return   void
///
/// @details  Updates this GUNNS Gas Turbine link model internal fluid thermal state and fluid
///           outputs to the motor.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsGasTurbine::updateFluid(const double dt __attribute__((unused)), const double flowRate)
{
    mReferenceTemp = std::max(DBL_EPSILON, mReferenceTemp);
    mReferencePress = std::max(DBL_EPSILON, mReferencePress);
    /// - Throw an exception if the reference corrected speeds are equal
    if (fabs(mCorrectedSpeedLow - mCorrectedSpeedHigh) < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Reference corrected speeds are equal.");
    }

    /// - Impeller inlet fluid properties uses the source node based on last-pass flow direction.
    const int    sourcePort = determineSourcePort(mFlux, 0, 1);
    const double sourceTemp = std::max(DBL_EPSILON, mNodes[sourcePort]->getOutflow()->getTemperature());

    /// - Scale efficiency curve based on impeller speed, using interpolation
    double scaledEffCoeffs[6];
    const double correctedSpeed = MsMath::limitRange(mCorrectedSpeedLow,
            mImpellerSpeed/(sourceTemp/mReferenceTemp), mCorrectedSpeedHigh);
    const double frac = (correctedSpeed   - mCorrectedSpeedLow)
                      / (mCorrectedSpeedHigh - mCorrectedSpeedLow);
    for(int order = 0; order < 6; order ++){
        scaledEffCoeffs[order] =  interpolate(mEffCoeffLowSpeed[order], mEffCoeffHighSpeed[order], frac);
    }
    const double minEffLimit = interpolate(mMinEffLimLowSpeed, mMinEffLimHighSpeed, frac);
    const double maxEffLimit = interpolate(mMaxEffLimLowSpeed, mMaxEffLimHighSpeed, frac);

    if(mPressureRatio >= minEffLimit and mPressureRatio <= maxEffLimit){
        mCurve.setCoeffs(scaledEffCoeffs);
        mEfficiency = MsMath::limitRange(0.0, mCurve.evaluate(mPressureRatio), 1.0);
    } else{
        mEfficiency = 0.0 ;
    }

    mImpellerPower = -UnitConversion::PA_PER_KPA * fabs(mVolFlowRate) * mPressureDrop * mEfficiency;

    if (mMotorSpeed > FLT_EPSILON and mDriveRatio > DBL_EPSILON) {
        mImpellerTorque = -mImpellerPower * UnitConversion::SEC_PER_MIN_PER_2PI / mMotorSpeed;
    } else {
        mImpellerTorque = 0.0;
        mImpellerPower  = 0.0;
    }

    /// - Perform heat convection between the internal fluid and turbine wall.
    mWallHeatFlux = GunnsFluidUtils::computeConvectiveHeatFlux(mInternalFluid,
                                                               flowRate,
                                                               mThermalROverD,
                                                               mThermalDiameter,
                                                               mThermalSurfaceArea,
                                                               mWallTemperature);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   void
///
/// @details  Calculates the flow rate across the impeller. It uses the current motor speed to
///           select the appropriate performance curve, then uses the previous pressure drop to
///           predict the flow rate.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsGasTurbine::computeFlowRate()
{
    mReferenceTemp = std::max(DBL_EPSILON, mReferenceTemp);
    mReferencePress = std::max(DBL_EPSILON, mReferencePress);
    /// - Throw an exception if the reference corrected speeds are equal
    if (fabs(mCorrectedSpeedLow - mCorrectedSpeedHigh) < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Reference corrected speeds are equal.");
    }

    /// - Impeller inlet fluid density uses the source node based on last-pass flow direction.
    const int    sourcePort    = determineSourcePort(mFlux, 0, 1);
    const double sourceDensity = std::max(DBL_EPSILON,mNodes[sourcePort]->getOutflow()->getDensity());
    const double sourceTemp    = std::max(DBL_EPSILON, mNodes[sourcePort]->getOutflow()->getTemperature());
    const double sourcePress   = std::max(DBL_EPSILON,mNodes[sourcePort]->getOutflow()->getPressure());

    /// - The impeller generates no flow if there is no inlet density
    if (sourceDensity > FLT_EPSILON ) {
        /// - Scale Turbine curve coefficients based on corrected impeller speed, using interpolation
        const double correctedSpeed = MsMath::limitRange(mCorrectedSpeedLow,
                mImpellerSpeed/(sourceTemp/mReferenceTemp), mCorrectedSpeedHigh);
        const double frac = (correctedSpeed   - mCorrectedSpeedLow)
                          / (mCorrectedSpeedHigh - mCorrectedSpeedLow);

        const double maxFlow =  interpolate(mLowSpeedMaxFlow, mHighSpeedMaxFlow, frac);
        const double riseCoeff1 =  interpolate(mCoeffLowSpeed1, mCoeffHighSpeed1, frac);
        const double riseCoeff2 =  interpolate(mCoeffLowSpeed2, mCoeffHighSpeed2, frac);

        /// - Calculate flow rate based on scaled performance curve and previous pressure drop.
        mPressureDrop  = mFilterGain * getDeltaPotential() + (1.0 - mFilterGain) * mPressureDrop;
        mPressureRatio =  std::max(1.0, sourcePress/std::max(DBL_EPSILON, (sourcePress
                       - mPressureDrop)));
        const double correctedMassFlow = maxFlow*((mPressureRatio - 1.0)*(mPressureRatio - 1.0)
                                       + riseCoeff1*(mPressureRatio - 1.0))/std::max(DBL_EPSILON,
                                               (mPressureRatio*mPressureRatio + riseCoeff2));
        const double correctionFactor = (sourcePress/mReferencePress)/sqrt(sourceTemp/mReferenceTemp);

        mPredictedFlowRate =  correctedMassFlow * correctionFactor;
    }else {
        mPredictedFlowRate = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] value   (m2)    New Thermal Surface Area.
///
/// @details  Sets the thermal surface area of this this GUNNS Gas Turbine link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsGasTurbine::interpolate(double y1, double y2, double fraction){
    return y1 + fraction*(y2 - y1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] value   (m2)    New Thermal Surface Area.
///
/// @details  Sets the thermal surface area of this this GUNNS Gas Turbine link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsGasTurbine::setThermalSurfaceArea(const double value)
{
    mThermalSurfaceArea = std::max(0.0, value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] value   (K)    New Wall Temperature.
///
/// @details  Sets the wall temperature of this this GUNNS Gas Turbine link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsGasTurbine::setWallTemperature(const double value)
{
    mWallTemperature = std::max(0.0, value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] value   (revolution/min)    New Motor Speed Temperature.
///
/// @details  Sets the motor speed of this this GUNNS Gas Turbine link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsGasTurbine::setMotorSpeed(const double value)
{
    mMotorSpeed = value;
}
