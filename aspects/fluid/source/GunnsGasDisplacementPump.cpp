/************************** TRICK HEADER **********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (Classes for the GUNNS Gas Displacement Pump link model.)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
   ((core/GunnsFluidSource.o))

PROGRAMMERS:
   ((Jason Harvey) (L-3 Communications) (Initial) (2012-07))
**************************************************************************************************/

#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "core/GunnsFluidUtils.hh"
#include "GunnsGasDisplacementPump.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     name             (--) Name of object.
/// @param[in,out] nodes            (--) Pointer to network nodes.
/// @param[in]     cycleVolume      (--) Volume of fluid displaced per cycle.
/// @param[in]     driveRatio       (--) Gear ratio of motor to impeller speed.
/// @param[in]     thermalLength    (m)  Impeller length for thermal convection.
/// @param[in]     thermalDiameter  (m)  Impeller inner diameter for thermal convection.
/// @param[in]     surfaceRoughness (m)  Impeller wall surface roughness for convection.
///
/// @details  Default constructs this GUNNS Gas Displacement Pump link model configuration data with
///           arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsGasDisplacementPumpConfigData::GunnsGasDisplacementPumpConfigData(
                                              const std::string& name,
                                              GunnsNodeList*     nodes,
                                              const double       cycleVolume,
                                              const double       driveRatio,
                                              const double       thermalLength,
                                              const double       thermalDiameter,
                                              const double       surfaceRoughness,
                                              const bool         checkValveActive)
    :
    GunnsFluidSourceConfigData(name, nodes),
    mCycleVolume     (cycleVolume),
    mDriveRatio      (driveRatio),
    mThermalLength   (thermalLength),
    mThermalDiameter (thermalDiameter),
    mSurfaceRoughness(surfaceRoughness),
    mCheckValveActive(checkValveActive)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy.
///
/// @details  Copy constructs this GUNNS Gas Displacement Pump link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsGasDisplacementPumpConfigData::GunnsGasDisplacementPumpConfigData(
                                                     const GunnsGasDisplacementPumpConfigData& that)
    :
    GunnsFluidSourceConfigData(that),
    mCycleVolume     (that.mCycleVolume),
    mDriveRatio      (that.mDriveRatio),
    mThermalLength   (that.mThermalLength),
    mThermalDiameter (that.mThermalDiameter),
    mSurfaceRoughness(that.mSurfaceRoughness),
    mCheckValveActive(that.mCheckValveActive)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Gas Displacement Pump link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsGasDisplacementPumpConfigData::~GunnsGasDisplacementPumpConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--)             Blockage malfunction flag.
/// @param[in] malfBlockageValue (--)             Blockage malfunction fractional value (0-1).
/// @param[in] flowDemand        (kg/s)           Demanded mass flow rate of the link.
/// @param[in] motorSpeed        (revolution/min) Initial speed of the motor.
/// @param[in] wallTemperature   (K)              Initial impeller wall temperature.
///
/// @details  Default constructs this GUNNS Gas Displacement Pump link model input data with
///           arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsGasDisplacementPumpInputData::GunnsGasDisplacementPumpInputData(const bool   malfBlockageFlag,
                                                                     const double malfBlockageValue,
                                                                     const double flowDemand,
                                                                     const double motorSpeed,
                                                                     const double wallTemperature)
    :
    GunnsFluidSourceInputData(malfBlockageFlag, malfBlockageValue, flowDemand),
    mMotorSpeed     (motorSpeed),
    mWallTemperature(wallTemperature)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Source to copy from.
///
/// @details  Copy constructs this GUNNS Gas Displacement Pump link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsGasDisplacementPumpInputData::GunnsGasDisplacementPumpInputData(
                                                      const GunnsGasDisplacementPumpInputData& that)
    :
    GunnsFluidSourceInputData(that),
    mMotorSpeed     (that.mMotorSpeed),
    mWallTemperature(that.mWallTemperature)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Gas Displacement Pump link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsGasDisplacementPumpInputData::~GunnsGasDisplacementPumpInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Gas Displacement Pump link model with obviously invalid
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsGasDisplacementPump::GunnsGasDisplacementPump()
    :
    GunnsFluidSource(),
    mCycleVolume(0.0),
    mDriveRatio(0.0),
    mThermalDiameter(0.0),
    mThermalSurfaceArea(0.0),
    mThermalROverD(0.0),
    mMotorSpeed(0.0),
    mWallTemperature(0.0),
    mWallHeatFlux(0.0),
    mImpellerTorque(0.0),
    mImpellerSpeed(0.0),
    mImpellerPower(0.0),
    mCheckValveActive(false),
    mCheckValvePosition(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Gas Displacement Pump link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsGasDisplacementPump::~GunnsGasDisplacementPump()
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
/// @returns  void
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Gas Displacement Pump link model with configuration and input
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsGasDisplacementPump::initialize(const GunnsGasDisplacementPumpConfigData& configData,
                                          const GunnsGasDisplacementPumpInputData&  inputData,
                                          std::vector<GunnsBasicLink*>&             links,
                                          const int                                 port0,
                                          const int                                 port1)
{
    /// - First initialize & validate parent.
    GunnsFluidSource::initialize(configData, inputData, links, port0, port1);

    /// - Reset initialization status flag.
    mInitFlag = false;

    /// - Validate configuration and input data.
    validate(configData, inputData);

    /// - Initialize from config data.
    mCycleVolume        = configData.mCycleVolume;
    mDriveRatio         = configData.mDriveRatio;
    mThermalDiameter    = configData.mThermalDiameter;
    mThermalSurfaceArea = configData.mThermalLength * UnitConversion::PI_UTIL * mThermalDiameter;
    if (mThermalSurfaceArea > DBL_EPSILON) {
        mThermalROverD  = configData.mSurfaceRoughness / mThermalDiameter;
    } else {
        mThermalROverD  = 0.0;
    }
    mCheckValveActive   = configData.mCheckValveActive;

    /// - Initialize from input data.
    mMotorSpeed         = inputData.mMotorSpeed;
    mWallTemperature    = inputData.mWallTemperature;

    /// - Initialize remaining state data.
    mWallHeatFlux       = 0.0;
    mImpellerTorque     = 0.0;
    mImpellerSpeed      = 0.0;
    mImpellerPower      = 0.0;
    mCheckValvePosition = 0.0;

    /// - Create the internal fluid.
    createInternalFluid();

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData   --  Configuration data.
/// @param[in]      inputData    --  Input data.
///
/// @returns  void
///
/// @throws   TsInitializationException
///
/// @details  Validates the initialization of this GUNNS Gas Displacement Pump link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsGasDisplacementPump::validate(const GunnsGasDisplacementPumpConfigData& configData,
                                        const GunnsGasDisplacementPumpInputData&  inputData) const
{
    /// - Throw an exception on cycle volume < DBL_EPSILON.
    if (configData.mCycleVolume < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Cycle volume < DBL_EPSILON.");
    }

    /// - Throw an exception on drive ratio < 0.
    if (configData.mDriveRatio < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Drive Ratio < 0.");
    }

    /// - Throw an exception if drive ratio is non-positive.
    if (configData.mDriveRatio < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Drive ratio < DBL_EPSILON.");
    }

    /// - Throw an exception if initial motor speed is negative,
    if (inputData.mMotorSpeed < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data", "Motor speed < 0.");
    }

    /// - Throw an exception if initial wall temperature is negative,
    if (inputData.mWallTemperature < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data", "Wall temperature < 0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsGasDisplacementPump::restartModel()
{
    /// - Reset the base class.
    GunnsFluidSource::restartModel();

    /// - Reset non-config & non-checkpointed attributes.
    mImpellerSpeed = 0.0;
    mImpellerPower = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Time step.
///
/// @returns  void
///
/// @details  Updates the link flow rate through this Gas Displacement Pump.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsGasDisplacementPump::updateState(const double dt)
{
    /// - Protect for divide by zero on drive ratio; also setting drive ratio to zero disconnects
    ///   the impeller from the motor, allowing manual control of the impeller speed for tuning.
    if (mDriveRatio > DBL_EPSILON) {
        mImpellerSpeed = mMotorSpeed / mDriveRatio;
    } else {
        GUNNS_WARNING("impeller is disconnected from motor.");
    }

    /// - Volumetric flow rate is the impeller speed times the volume per cycle.
    mVolFlowRate = mImpellerSpeed * mCycleVolume / UnitConversion::SEC_PER_MIN;

    /// - Check if check valve active, and if so, check for positive flow
    if(mVolFlowRate > 0.0 or !mCheckValveActive){
        mCheckValvePosition = 1.0;
    } else{
        mCheckValvePosition = 0.0;
    }
    mVolFlowRate *= mCheckValvePosition;

    /// - Run & apply the cavitation model in derived classes.
    applyCavitation(dt);

    /// - Demanded flow rate through the link.  Unlike the fan & centrifugal pump classes, this
    ///   displacement pump allows flow in either direction.
    if (mImpellerSpeed > 0.0) {
        mFlowDemand = mVolFlowRate * mNodes[0]->getContent()->getDensity();
    } else {
        mFlowDemand = mVolFlowRate * mNodes[1]->getContent()->getDensity();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt       (s)    Time step.
/// @param[in] flowRate (kg/s) Mass flow rate.
///
/// @returns  void
///
/// @details  Updates this Gas Displacement Pump link model internal fluid thermal state and fluid
///           outputs to the motor.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsGasDisplacementPump::updateFluid(const double dt __attribute__((unused)), const double flowRate)
{
    /// - Impeller power & torque.  Power is kept as a class attribute for reference & display.
    ///   Torque opposes motor spin so has opposite sign.  Motor speed units are converted to r/s
    ///   to relate to torque in N*m and power in Watts.  Torque on the shaft is zero if the drive
    ///   ratio is zero, i.e. impeller is disconnected from the motor.
    mImpellerPower = UnitConversion::PA_PER_KPA * fabs(mVolFlowRate * mPotentialDrop);
    if (std::fabs(mMotorSpeed ) > static_cast<double>(FLT_EPSILON) and mDriveRatio > DBL_EPSILON) {
        mImpellerTorque = -mImpellerPower * UnitConversion::SEC_PER_MIN_PER_2PI / mMotorSpeed;
    } else {
        mImpellerTorque = 0.0;
    }

    /// - Copy the source node contents into the internal fluid.
    mInternalFluid->setState(mNodes[determineSourcePort(mFlux, 0, 1)]->getOutflow());

    /// - Perform heat convection between the internal fluid and pipe wall.
    mWallHeatFlux = GunnsFluidUtils::computeConvectiveHeatFlux(mInternalFluid,
                                                               flowRate,
                                                               mThermalROverD,
                                                               mThermalDiameter,
                                                               mThermalSurfaceArea,
                                                               mWallTemperature);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] value   (m2)    New Thermal Surface Area.
///
/// @returns  void
///
/// @details  Sets the thermal surface area of this this GUNNS Fluid Gas Displacement Pump model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsGasDisplacementPump::setThermalSurfaceArea(const double value)
{
    mThermalSurfaceArea = std::max(0.0, value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] value   (K)    New Wall Temperature.
///
/// @returns  void
///
/// @details  Sets the wall temperature of this this GUNNS Fluid Gas Displacement Pump model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsGasDisplacementPump::setWallTemperature(const double value)
{
    mWallTemperature = std::max(0.0, value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] value   (--)    Check valve on/off flag
///
/// @returns  void
///
/// @details  Turns on or off the check valve functionality
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsGasDisplacementPump::setCheckValveFlag(const bool value)
{
    mCheckValveActive = value;
}
