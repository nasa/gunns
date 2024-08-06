/***************************************** TRICK HEADER ********************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
     (Provides the classes for modeling the GUNNS Solar Array Regulator.)

 REQUIREMENTS:
     ()

 REFERENCE:
     ()

 ASSUMPTIONS AND LIMITATIONS:
     ()

LIBRARY DEPENDENCY:
    (
      (aspects/electrical/sar/GunnsSolarArrayRegulator.o)
      (aspects/electrical/Converter/ConverterElect.o)
      (core/GunnsBasicConductor.o)
      (common/sensors/SensorAnalog.o)
      (core/GunnsBasicLink.o)
      (software/exceptions/TsInitializationException.o)
    )

 PROGRAMMERS:
    (
     (Mike Moore) (L3) (2015-01)
    )
 **************************************************************************************************/
#include "aspects/electrical/sar/GunnsSolarArrayRegulator.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "core/GunnsBasicConductor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   name                      (--)    Name of the link being created
/// @param[in]   nodes                     (--)    Pointer to  nodes
/// @param[in]   battery                   (--)    Pointer to  the battery in the network.
/// @param[in]   outVoltageSensorConfig    (--)    output voltage sensor configuration data
/// @param[in]   outCurrentSensorConfig    (--)    output current sensor configuration data
/// @param[in]   outputConductance         (one/ohm) Converter's ON conductance on the load side of the link
/// @param[in]   converterOffConductance   (one/ohm) Converter's OFF conductance of the link
/// @param[in]   tripPriority              (--)   Trip tier for this link in gunns network
/// @param[in]   standbyPower              (W)    Standby Power when converter disables its output
/// @param[in]   trickleChargeRate         (amp)    Minimum voltage that this load can operate at.
/// @param[in]   regulatedVoltageLowLimit  (amp)  Lower limit of the voltage output of the regulator
/// @param[in]   regulatedVoltageHighLimit (amp)  Upper limit of the voltage output of the regulator
///
/// @details  Constructs the Solar Array Regulator Config data
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsSolarArrayRegulatorConfigData::GunnsSolarArrayRegulatorConfigData(const std::string& name,
                                                           GunnsNodeList*     nodes,
                                                           const GunnsBasicConductor* battery,
                                                           const SensorAnalogConfigData*       outVoltageSensorConfig,
                                                           const SensorAnalogConfigData*       outCurrentSensorConfig,
                                                           const double       outputConductance,
                                                           const double       converterOffConductance,
                                                           const int       tripPriority,
                                                           const double    standbyPower,
                                                           const double    trickleChargeRate,
                                                           const double    regulatedVoltageLowLimit,
                                                           const double    regulatedVoltageHighLimit)
    :
    ConverterElectConfigData(name, nodes, outVoltageSensorConfig, outCurrentSensorConfig, outputConductance, converterOffConductance,
                                tripPriority, standbyPower),
    mBattery(battery),
    mNominalTrickleChargeRate(trickleChargeRate),
    mRegulatedVoltageLowLimit(regulatedVoltageLowLimit),
    mRegulatedVoltageHighLimit(regulatedVoltageHighLimit)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy from
///
/// @details  Copy Constructs the Solar Array Regulator Config data
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsSolarArrayRegulatorConfigData::GunnsSolarArrayRegulatorConfigData(
        const GunnsSolarArrayRegulatorConfigData& that)
    :
    ConverterElectConfigData(that),
    mBattery(that.mBattery),
    mNominalTrickleChargeRate(that.mNominalTrickleChargeRate),
    mRegulatedVoltageLowLimit(that.mRegulatedVoltageLowLimit),
    mRegulatedVoltageHighLimit(that.mRegulatedVoltageHighLimit)
{
    // nothing to do
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Solar Array Regulator Config Data Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsSolarArrayRegulatorConfigData::~GunnsSolarArrayRegulatorConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    malfBlockageFlag           (--)  Flag to enable blockage in the link
/// @param[in]    malfBlockageValue          (--)  Value to use for blockage calculation - basic link data
/// @param[in]    outVoltageSensorInput      (--)  output voltage sensor data to set the instance input data
/// @param[in]    outCurrentSensorInput      (--)  output current sensor data to set the instance input data
/// @param[in]    malfOpOverCurrentFlag      (--)  Malfunction flag to override the output over current Limit
/// @param[in]    malfOpOverVoltageFlag      (--)  Malfunction flag to override the output over voltage limit
/// @param[in]    malfRegulatedVoltageFlag   (--)  Malfunction flag to override the regulated voltage Limit
/// @param[in]    inputVoltage               (V)   Input voltage to the converter
/// @param[in]    regulatedVoltage           (V)   Regualted voltage coming out from the converter
/// @param[in]    efficiency                 (--)  Efficiency of the converter conversion process
/// @param[in]    opOverCurrentLimit         (amp)  Output over current limit
/// @param[in]    opOverVoltageLimit         (V)    Output over voltage limit (hardware)
/// @param[in]    opOverCurrentTripActive    (--)   Output over current trip active
/// @param[in]    opOverVoltageTripActive    (--)   Output over voltage trip active
/// @param[in]    inputOverVoltageLimit      (V)    Input over voltage limit of the converter
/// @param[in]    inputUnderVoltageLimit     (V)    Input under voltage limit of the converter
/// @param[in]    inOverVoltageTripActive    (--)   Input over voltage trip active
/// @param[in]    inUnderVoltageTripActive   (--)   Input under voltage trip active
///
/// @details  Default constructs this Solar Array Regulator input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsSolarArrayRegulatorInputData::GunnsSolarArrayRegulatorInputData(const bool   malfBlockageFlag,
                                                         const double malfBlockageValue,
                                                         const SensorAnalogInputData* outVoltageSensorInput,
                                                         const SensorAnalogInputData* outCurrentSensorInput,
                                                         const bool malfOpOverCurrentFlag,
                                                         const bool malfOpOverVoltageFlag,
                                                         const bool malfRegulatedVoltageFlag,
                                                         const double inputVoltage,
                                                         const double regulatedVoltage,
                                                         const double efficiency,
                                                         const double opOverCurrentLimit,
                                                         const double opOverVoltageLimit,
                                                         const bool opOverCurrentTripActive,
                                                         const bool opOverVoltageTripActive,
                                                         const double inputOverVoltageLimit,
                                                         const double inputUnderVoltageLimit,
                                                         const bool inOverVoltageTripActive,
                                                         const bool inUnderVoltageTripActive,
                                                         const double proportionalGain,
                                                         const double derivativeGain)
    :
    ConverterElectInputData(malfBlockageFlag, malfBlockageValue, outVoltageSensorInput, outCurrentSensorInput, malfOpOverCurrentFlag,
                              malfOpOverVoltageFlag, malfRegulatedVoltageFlag, inputVoltage, regulatedVoltage, efficiency, opOverCurrentLimit,
                                opOverVoltageLimit, opOverCurrentTripActive, opOverVoltageTripActive, inputOverVoltageLimit, inputUnderVoltageLimit,
                                  inOverVoltageTripActive, inUnderVoltageTripActive),
    mProportionalGain(proportionalGain),
    mDerivativeGain(derivativeGain)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  Copy constructs this Solar Array Regulator input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsSolarArrayRegulatorInputData::GunnsSolarArrayRegulatorInputData(const GunnsSolarArrayRegulatorInputData& that)
    :
    ConverterElectInputData(that),
    mProportionalGain(that.mProportionalGain),
    mDerivativeGain(that.mDerivativeGain)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Solar Array Regulator input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsSolarArrayRegulatorInputData::~GunnsSolarArrayRegulatorInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the Solar Array Regulator Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsSolarArrayRegulator::GunnsSolarArrayRegulator():
    mInitFlag(false),
    mBattery(0),
    mDesiredChargeCurrent(0.0),
    mPreviousChargeCurrentError(0.0),
    mKp(0.0),
    mKd(0.0),
    mRegulatedVoltageLowLimit(0.0),
    mRegulatedVoltageHighLimit(0.0),
    mControlledVoltage(0.0),
    mMinInVoltage(0.0),
    mMarginTurnOnVoltage(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Solar Array Regulator Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsSolarArrayRegulator::~GunnsSolarArrayRegulator()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) Reference to Solar Array Regulator Config Data
/// @param[in]     inputData    (--) Reference to Solar Array Regulator Input Data
/// @param[in,out] networkLinks (--) Reference to the Network Link Vector
/// @param[in]     port0        (--) Port 0 Mapping
/// @param[in]     port1        (--) Port 1 Mapping
/// @param[in]     port2        (--) Port 2 Mapping
/// @param[in]     port3        (--) Port 3 Mapping
///
/// @throws   TsInitializationException
///
/// @details  Initializes the Solar Array Regulator with config and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsSolarArrayRegulator::initialize(const GunnsSolarArrayRegulatorConfigData& configData,
                                     const GunnsSolarArrayRegulatorInputData&  inputData,
                                     std::vector<GunnsBasicLink*>&        networkLinks,
                                     const int                            port0,
                                     const int                            port1,
                                     const int                            port2,
                                     const int                            port3)
{
    /// - Reset init flag.
    mInitFlag = false;

    /// - Initialize the parent class.
    ConverterElect::initialize(configData, inputData, networkLinks, port0, port1, port2, port3);

    /// - Initialize class attributes.
    mBattery                    = configData.mBattery;
    mDesiredChargeCurrent       = configData.mNominalTrickleChargeRate;
    mRegulatedVoltageLowLimit   = configData.mRegulatedVoltageLowLimit;
    mRegulatedVoltageHighLimit  = configData.mRegulatedVoltageHighLimit;

    mKp      = inputData.mProportionalGain;
    mKd      = inputData.mDerivativeGain;

    /// - Validate the model configuration.
    validate();

    /// - Just go ahead and default the converter to be on.
    setConverterOnCmd(true);

    /// - Warn of deprecation due to obsolescence by GunnsElectPvRegConv.
    GUNNS_WARNING("this link is deprecated!  It is obsoleted by GunnsElectPvRegConv.");

    /// - Set init flag on successful validation.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  Validates the solar array regulator
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsSolarArrayRegulator::validate() const
{
    /// - Issue an error if the battery pointer is null. This link cannot function
    ///   without a valid reference to a battery.
    if (mBattery == 0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    " Solar Array Regulator has a null reference to the battery link. This link cannot function without a valid battery reference (BattElect type).");
    }
    /// - Issue an error on battery trickle charge rate being set to less than zero.
    if (mDesiredChargeCurrent < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    " Desired battery charge current is  < 0. This indicates a battery discharge, and is therefore not a valid value.");
    }
    /// - Issue an error when the regulated low voltage limit is zero or below.
    if (mRegulatedVoltageLowLimit < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Config Data",
                    " Desired regulated voltage lower limit is < 0. This is not a valid voltage.");
    }
    /// - Issue an error when the regulated high voltage limit is zero or below.
    if (mRegulatedVoltageHighLimit < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Config Data",
                    " Desired regulated voltage lower limit is < 0. This is not a valid voltage.");
    }
    /// - Issue an error when the regulated high voltage limit is less than or equal to the low limit voltage.
    if (mRegulatedVoltageHighLimit <= mRegulatedVoltageLowLimit) {
        GUNNS_ERROR(TsInitializationException, "Invalid Config Data",
                    " Desired regulated voltage lower limit is greater than or equal to the upper limit. This is not a valid voltage range.");
    }
    /// - Issue an error when the controller proportional gain is set to less than zero.
    if (mKp < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    " Desired proportional gain for the battery charge current feedback controller is < 0. This is not a valid controller gain.");
    }
    /// - Issue an error when the controller derivative gain is set to less than zero.
    if (mKd < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    " Desired derivative gain for the battery charge current feedback controller is < 0. This is not a valid controller gain.");
    }

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt        (s)  Link time step
/// @param[in] minorstep       (--) The minor step number that the network is on (not used)
/// @return void
/// @details  Updates the link during the minor time step
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsSolarArrayRegulator::minorStep(const double dt, const int __attribute__((unused)) minorstep)
{
    computeFlows(dt);
    ConverterElect::step(dt);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   timeStep  (s)  Time step
/// @return void
/// @details  Method to update admittance and source potential of the link.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsSolarArrayRegulator::step(const double timeStep)
{
  /// - Perform the automatic voltage regulator control.
  updateControl(timeStep);
  ConverterElect::step(timeStep);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   timeStep  (s)  Time step
/// @return void
/// @details  Method to update the battery current feedback controller.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsSolarArrayRegulator::updateControl(const double timeStep)
{
  /// - This is necessary if done in a non-linear GUNNS minor step b/c the BattElect link
  ///   is a linear link and the Battery current will not vary across a minor step. To be on the
  ///   safe side, we calculate this value ourselves.
  double* battPotentialVector = mBattery->getPotentialVector();
  double* battAdmittanceMatrix = mBattery->getAdmittanceMatrix();
  double* battSourceVector = mBattery->getSourceVector();
  double battCurrent = ((battPotentialVector[1] - battPotentialVector[0]) * battAdmittanceMatrix[0]) + battSourceVector[0];

  /// - Calculate the error and an approximation of the derivative of the error in order to apply P-D
  ///   feedback control to the regulated ouput voltage based on the error in the desired battery current
  double chargeCurrentError = mDesiredChargeCurrent - battCurrent;
  double chargeCurrentErrorDot = (chargeCurrentError - mPreviousChargeCurrentError) / timeStep;

  /// - We must cache this value across calls to this function in order to properly calculate the charge current
  ///   error derivative term in the next pass.
  mPreviousChargeCurrentError = chargeCurrentError;

  /// - Apply proportional-derivative feedback control and the voltage saturation limits to the controlled
  ///   output voltage.
  mControlledVoltage += mKp*chargeCurrentError + mKd*chargeCurrentErrorDot;
  mControlledVoltage = MsMath::limitRange(mRegulatedVoltageLowLimit, mControlledVoltage, mRegulatedVoltageHighLimit);
  /// - Final step is to update the RegulatedVoltage term that the ConverterElect base class uses to perform
  ///   its converter function. The base class step function will be called after this.
  mRegulatedVoltage = mControlledVoltage;
}
