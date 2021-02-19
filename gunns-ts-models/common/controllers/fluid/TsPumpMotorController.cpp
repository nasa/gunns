/**
@file
@brief     Pump Motor Controller Model implementation.

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
 ((common/effectors/mechanical/motor/DcDynPumpMotor.o))
*/

#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "GenericMacros.hh"
#include "math/MsMath.hh"
#include <cmath>
#include "TsPumpMotorController.hh"

const double TsPumpMotorController::MAX_RESISTANCE = 1.0e+06;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] motor                (--)             Motor configuration data.
/// @param[in] forcingGain          (--)             (0-1) Control filter forcing gain.
/// @param[in] dampingGain          (--)             (0-1) Control filter damping gain.
/// @param[in] dampingCutoff        (--)             (0-1) Control filter damping cutoff error.
/// @param[in] temperatureTripLimit (K)              Temperature trip limit.
/// @param[in] temperatureTripReset (K)              Temperature trip reset.
/// @param[in] minVoltage           (V)              Minimum voltage for operation.
/// @param[in] maxVoltage           (V)              Maximum voltage for operation.
/// @param[in] controllerPowerLoad  (W)              Controller circuitry power load.
/// @param[in] noiseAmplitude       (revolution/min) Amplitude of speed bias function.
/// @param[in] noiseFrequency       (Hz)             Frequency of speed bias function.
/// @param[in] startupCurrentLimit  (amp)            Limited motor current during startup.
///
/// @details  Default constructs this Controller configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsPumpMotorControllerConfigData::TsPumpMotorControllerConfigData(const DcDynPumpMotorConfigData& motor,
                                                   const double forcingGain,
                                                   const double dampingGain,
                                                   const double dampingCutoff,
                                                   const double temperatureTripLimit,
                                                   const double temperatureTripReset,
                                                   const double minVoltage,
                                                   const double maxVoltage,
                                                   const double controllerPowerLoad,
                                                   const double noiseAmplitude,
                                                   const double noiseFrequency,
                                                   const double startupCurrentLimit)
    :
    mMotor               (motor),
    mForcingGain         (forcingGain),
    mDampingGain         (dampingGain),
    mDampingCutoff       (dampingCutoff),
    mTemperatureTripLimit(temperatureTripLimit),
    mTemperatureTripReset(temperatureTripReset),
    mMinVoltage          (minVoltage),
    mMaxVoltage          (maxVoltage),
    mControllerPowerLoad (controllerPowerLoad),
    mNoiseAmplitude      (noiseAmplitude),
    mNoiseFrequency      (noiseFrequency),
    mStartupCurrentLimit (startupCurrentLimit)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Source object to copy.
///
/// @details  Copy constructs this Controller configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsPumpMotorControllerConfigData::TsPumpMotorControllerConfigData(const TsPumpMotorControllerConfigData& that)
    :
    mMotor               (that.mMotor),
    mForcingGain         (that.mForcingGain),
    mDampingGain         (that.mDampingGain),
    mDampingCutoff       (that.mDampingCutoff),
    mTemperatureTripLimit(that.mTemperatureTripLimit),
    mTemperatureTripReset(that.mTemperatureTripReset),
    mMinVoltage          (that.mMinVoltage),
    mMaxVoltage          (that.mMaxVoltage),
    mControllerPowerLoad (that.mControllerPowerLoad),
    mNoiseAmplitude      (that.mNoiseAmplitude),
    mNoiseFrequency      (that.mNoiseFrequency),
    mStartupCurrentLimit (that.mStartupCurrentLimit)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to assign to
///
/// @details  Assignment operator for this Controller configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsPumpMotorControllerConfigData& TsPumpMotorControllerConfigData::operator =(const TsPumpMotorControllerConfigData& that)
{
    if (this != &that)
    {
        mMotor                = that.mMotor;
        mForcingGain          = that.mForcingGain;
        mDampingGain          = that.mDampingGain;
        mDampingCutoff        = that.mDampingCutoff;
        mTemperatureTripLimit = that.mTemperatureTripLimit;
        mTemperatureTripReset = that.mTemperatureTripReset;
        mMinVoltage           = that.mMinVoltage;
        mMaxVoltage           = that.mMaxVoltage;
        mControllerPowerLoad  = that.mControllerPowerLoad;
        mNoiseAmplitude       = that.mNoiseAmplitude;
        mNoiseFrequency       = that.mNoiseFrequency;
        mStartupCurrentLimit  = that.mStartupCurrentLimit;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Controller configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsPumpMotorControllerConfigData::~TsPumpMotorControllerConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] motor             (--)             Motor input data.
/// @param[in] voltage           (V)              Initial supply voltage.
/// @param[in] sensedSpeed       (revolution/min) Initial sensed speed.
/// @param[in] sensedTemperature (K)              Initial sensed temperature.
/// @param[in] startupState      (--)             Initially limiting startup current.
/// @param[in] commandEnable     (--)             Initial enable command.
/// @param[in] commandSpeed      (revolution/min) Initial commanded speed.
/// @param[in] noisePhase        (rad)            Initial phase of speed bias function.
///
/// @details  Default constructs this Controller input data.  Malfunction parameters are provided as
///           class attributes to support unit testing, but are not needed to be specified in the
///           constructor, thus no arguments are provided.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsPumpMotorControllerInputData::TsPumpMotorControllerInputData(const DcDynPumpMotorInputData& motor,
                                                               const double voltage,
                                                               const double sensedSpeed,
                                                               const double sensedTemperature,
                                                               const double startupState,
                                                               const bool   commandEnable,
                                                               const double commandSpeed,
                                                               const double noisePhase)
    :
    mMotor                 (motor),
    mVoltage               (voltage),
    mSensedSpeed           (sensedSpeed),
    mSensedTemperature     (sensedTemperature),
    mStartupState          (startupState),
    mCommandEnable         (commandEnable),
    mCommandSpeed          (commandSpeed),
    mNoisePhase            (noisePhase),
    mMalfFailPower         (false),
    mMalfPowerOverrideFlag (false),
    mMalfPowerOverrideValue(0.0),
    mMalfCommandSpeedFlag  (false),
    mMalfCommandSpeedValue (0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Source object to copy.
///
/// @details  Copy constructs this Controller input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsPumpMotorControllerInputData::TsPumpMotorControllerInputData(const TsPumpMotorControllerInputData& that)
    :
    mMotor                 (that.mMotor),
    mVoltage               (that.mVoltage),
    mSensedSpeed           (that.mSensedSpeed),
    mSensedTemperature     (that.mSensedTemperature),
    mStartupState          (that.mStartupState),
    mCommandEnable         (that.mCommandEnable),
    mCommandSpeed          (that.mCommandSpeed),
    mNoisePhase            (that.mNoisePhase),
    mMalfFailPower         (that.mMalfFailPower),
    mMalfPowerOverrideFlag (that.mMalfPowerOverrideFlag),
    mMalfPowerOverrideValue(that.mMalfPowerOverrideValue),
    mMalfCommandSpeedFlag  (that.mMalfCommandSpeedFlag),
    mMalfCommandSpeedValue (that.mMalfCommandSpeedValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to assign to
///
/// @details  Assignment operator for this Controller input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsPumpMotorControllerInputData& TsPumpMotorControllerInputData::operator =(const TsPumpMotorControllerInputData& that)
{
    if (this != &that)
    {
        mMotor                  = that.mMotor;
        mVoltage                = that.mVoltage;
        mSensedSpeed            = that.mSensedSpeed;
        mSensedTemperature      = that.mSensedTemperature;
        mStartupState           = that.mStartupState;
        mCommandEnable          = that.mCommandEnable;
        mCommandSpeed           = that.mCommandSpeed;
        mNoisePhase             = that.mNoisePhase;
        mMalfFailPower          = that.mMalfFailPower;
        mMalfPowerOverrideFlag  = that.mMalfPowerOverrideFlag;
        mMalfPowerOverrideValue = that.mMalfPowerOverrideValue;
        mMalfCommandSpeedFlag   = that.mMalfCommandSpeedFlag;
        mMalfCommandSpeedValue  = that.mMalfCommandSpeedValue;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Controller input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsPumpMotorControllerInputData::~TsPumpMotorControllerInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling the step
///           method.
///
/// @details  Default constructs this Controller.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsPumpMotorController::TsPumpMotorController()
    :
    mMotor(),
    mMalfFailPower(false),
    mMalfPowerOverrideFlag(false),
    mMalfPowerOverrideValue(0.0),
    mMalfCommandSpeedFlag(false),
    mMalfCommandSpeedValue(0.0),
    mName(),
    mInitFlag(false),
    mForcingGain(0.0),
    mDampingGain(0.0),
    mDampingCutoff(0.0),
    mTemperatureTripLimit(0.0),
    mTemperatureTripReset(0.0),
    mMinVoltage(0.0),
    mMaxVoltage(0.0),
    mControllerPowerLoad(0.0),
    mNoiseAmplitude(0.0),
    mNoiseFrequency(0.0),
    mStartupCurrentLimit(0.0),
    mVoltage(0.0),
    mSensedSpeed(0.0),
    mSensedTemperature(0.0),
    mStartupState(false),
    mCommandEnable(false),
    mCommandSpeed(0.0),
    mNoisePhase(0.0),
    mPulseWidth(0.0),
    mSpeedError(0.0),
    mTrippedTemperature(false),
    mControllerPowerBus(false),
    mMotorPowerBus(false),
    mTotalResistiveLoad(MAX_RESISTANCE),
    mTotalWasteHeat(0.0),
    mTotalCurrent(0.0)
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Controller.
///////////////////////////////////////////////////////////////////////////////////////////////////
TsPumpMotorController::~TsPumpMotorController()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] configData (--) Configuration data.
/// @param[in] inputData  (--) Input data.
/// @param[in] name       (--) Object name.
///
/// @returns  void
///
/// @throws   TsInitializationException
///
/// @details  Initializes this Controller with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsPumpMotorController::initialize(const TsPumpMotorControllerConfigData& configData,
                                       const TsPumpMotorControllerInputData&  inputData,
                                       const std::string&                     name)
{
    /// - Reset init flag.
    mInitFlag = false;

    /// - Allocate and initialize the object name or throw a TsInitializationException exception if empty.
    TS_GENERIC_NAME_ERREX("TsPumpMotorController")

    /// - Validate initialization data.
    validate(configData, inputData);

    /// - Initialize with config data.
    mForcingGain            = configData.mForcingGain;
    mDampingGain            = configData.mDampingGain;
    mDampingCutoff          = configData.mDampingCutoff;
    mTemperatureTripLimit   = configData.mTemperatureTripLimit;
    mTemperatureTripReset   = configData.mTemperatureTripReset;
    mMinVoltage             = configData.mMinVoltage;
    mMaxVoltage             = configData.mMaxVoltage;
    mControllerPowerLoad    = configData.mControllerPowerLoad;
    mNoiseAmplitude         = configData.mNoiseAmplitude;
    mNoiseFrequency         = configData.mNoiseFrequency;
    mStartupCurrentLimit    = configData.mStartupCurrentLimit;

    /// - Initialize with input data.
    mVoltage                = inputData.mVoltage;
    mSensedSpeed            = inputData.mSensedSpeed;
    mSensedTemperature      = inputData.mSensedTemperature;
    mStartupState           = inputData.mStartupState;
    mCommandEnable          = inputData.mCommandEnable;
    mCommandSpeed           = inputData.mCommandSpeed;
    mNoisePhase             = inputData.mNoisePhase;
    mMalfFailPower          = inputData.mMalfFailPower;
    mMalfPowerOverrideFlag  = inputData.mMalfPowerOverrideFlag;
    mMalfPowerOverrideValue = inputData.mMalfPowerOverrideValue;
    mMalfCommandSpeedFlag   = inputData.mMalfCommandSpeedFlag;
    mMalfCommandSpeedValue  = inputData.mMalfCommandSpeedValue;

    /// - Initialize remaining state data.
    mPulseWidth             = 0.0;
    mSpeedError             = 0.0;
    mTrippedTemperature     = false;
    mControllerPowerBus     = false;
    mMotorPowerBus          = false;
    mTotalResistiveLoad     = MAX_RESISTANCE;
    mTotalWasteHeat         = 0.0;
    mTotalCurrent           = 0.0;

    /// - Initialize the motor.
    mMotor.initialize(configData.mMotor, inputData.mMotor, name + ".mMotor");

    /// - Mark controller as having completed initialization successfully.
    mInitFlag = mMotor.isInitialized();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] configData (--) Configuration data.
/// @param[in] inputData  (--) Input data.
///
/// @returns  void
///
/// @throws   TsInitializationException
///
/// @details  Validates this Controller's configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsPumpMotorController::validate(const TsPumpMotorControllerConfigData& configData,
                              const TsPumpMotorControllerInputData&  inputData) const
{
    /// - Throw an exception if forcing gain is outside 0-1.
    if (!MsMath::isInRange(0.0, configData.mForcingGain, 1.0)) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "TSM", "Invalid Configuration Data", TsInitializationException,
                "Forcing gain cannot be outside 0-1.", mName);
    }

    /// - Throw an exception if damping gain is outside 0-1.
    if (!MsMath::isInRange(0.0, configData.mDampingGain, 1.0)) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "TSM", "Invalid Configuration Data", TsInitializationException,
                "Damping gain cannot be outside 0-1.", mName);
    }

    /// - Throw an exception if damping cut-off is outside 0-1.
    if (!MsMath::isInRange(0.0, configData.mDampingCutoff, 1.0)) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "TSM", "Invalid Configuration Data", TsInitializationException,
                "Damping cut-off cannot be outside 0-1.", mName);
    }

    /// - Throw an exception if minimum temperature is < 0.
    if (configData.mTemperatureTripReset < 0.0) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "TSM", "Invalid Configuration Data", TsInitializationException,
                "Minimum temperature cannot be < 0.", mName);
    }

    /// - Throw an exception if maximum temperature is <= minimum temperature.
    if (configData.mTemperatureTripLimit <= configData.mTemperatureTripReset) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "TSM", "Invalid Configuration Data", TsInitializationException,
                "Maximum temperature cannot be <= minimum.", mName);
    }

    /// - Throw an exception if minimum voltage is < 0.
    if (configData.mMinVoltage < 0.0) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "TSM", "Invalid Configuration Data", TsInitializationException,
                "Minimum voltage cannot be < 0.", mName);
    }

    /// - Throw an exception if maximum voltage is <= minimum voltage.
    if (configData.mMaxVoltage <= configData.mMinVoltage) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "TSM", "Invalid Configuration Data", TsInitializationException,
                "Maximum voltage cannot be <= minimum.", mName);
    }

    /// - Throw an exception if controller power load is < 0.
    if (configData.mControllerPowerLoad < 0.0) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "TSM", "Invalid Configuration Data", TsInitializationException,
                "Controller power load cannot be < 0.", mName);
    }

    /// - Throw an exception if noise amplitude is < 0.
    if (configData.mNoiseAmplitude < 0.0) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "TSM", "Invalid Configuration Data", TsInitializationException,
                "Noise amplitude cannot be < 0.", mName);
    }

    /// - Throw an exception if noise frequency is < 0.
    if (configData.mNoiseFrequency < 0.0) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "TSM", "Invalid Configuration Data", TsInitializationException,
                "Noise frequency cannot be < 0.", mName);
    }

    /// - Throw an exception if initial voltage is < 0.
    if (inputData.mVoltage < 0.0) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "TSM", "Invalid Input Data", TsInitializationException,
                "Initial voltage cannot be < 0.", mName);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Time step.
///
/// @returns  void
///
/// @details  Updates all controller parameters over the time step.  The called virtual methods are
///           meant to be overridden by derived classes to implement extra features as needed.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsPumpMotorController::step(const double dt)
{
    determinePower();
    updateControlFilter(dt);
    updateMotor(dt);
    updatePowerLoad();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt            (s)              Time step.
/// @param[in] voltage       (V)              Supply voltage.
/// @param[in] speed         (revolution/min) Sensed motor speed.
/// @param[in] temperature   (K)              Sensed motor temperature.
/// @param[in] speedCommand  (revolution/min) Motor speed command.
/// @param[in] enableCommand (--)             Motor command enable flag.
///
/// @returns  void
///
/// @details  Convenience method to update the controller with one call, providing all inputs as
///           calling arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsPumpMotorController::stepWithArgs(const double dt,
                                  const double voltage,
                                  const double speed,
                                  const double temperature,
                                  const double speedCommand,
                                  const bool   enableCommand)
{
    setVoltage(voltage);
    setSensedSpeed(speed);
    setSensedTemperature(temperature);
    setCommandSpeed(speedCommand);
    setCommandEnable(enableCommand);
    step(dt);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  void
///
/// @details  Determines if the controller and motor buses is adequately powered.  Order of
///           precedence for the controller bus is:
///              (fail power malf) > (voltage override malf) > (voltage in limits).
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsPumpMotorController::determinePower()
{
    /// - Controller power bus responds to supply voltage and malfunctions.
    double useVoltage = mVoltage;
    if (mMalfPowerOverrideFlag) {
        useVoltage = mMalfPowerOverrideValue;
    }
    mControllerPowerBus = MsMath::isInRange(mMinVoltage, useVoltage, mMaxVoltage)
                          and not mMalfFailPower;

    checkTrips();

    /// - Motor bus power is interrupted by trips and the Enable Command.
    mMotorPowerBus = mControllerPowerBus and mCommandEnable
                     and not (mTrippedTemperature);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  void
///
/// @details  Performs trip logic for motor temperature.  This acts like a thermostat - the trip is
///           acitvated when temperature exceeds a maximum setpoint, and resets when temperature
///           falls below a minimum setpoint.  The trip can also be reset by cycling controller
///           power, or manually reset by the user.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsPumpMotorController::checkTrips()
{
    if (mControllerPowerBus) {
        mTrippedTemperature = ((mSensedTemperature > mTemperatureTripLimit) or mTrippedTemperature)
                       and not (mSensedTemperature < mTemperatureTripReset);
    } else {
        mTrippedTemperature = false;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Time step.
///
/// @returns  void
///
/// @details  Updates the control filter for a new pulse width to control the motor to the commanded
///           speed.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsPumpMotorController::updateControlFilter(const double dt)
{
    /// - Speed command malfunction overrides the commanded speed.
    double speedCommand = 0.0;
    if (mMalfCommandSpeedFlag) {
        speedCommand = mMalfCommandSpeedValue;
    } else {
        speedCommand = mCommandSpeed;
    }

    if (mMotorPowerBus and FLT_EPSILON < speedCommand) {

        /// - Apply noise as a sine wave to speed command.
        mNoisePhase += UnitConversion::TWO_PI * mNoiseFrequency * dt;
        mNoisePhase  = fmod(mNoisePhase, UnitConversion::TWO_PI);
        if (mNoisePhase < 0.0) {
            mNoisePhase += UnitConversion::TWO_PI;
        }
        speedCommand += mNoiseAmplitude * std::sin(mNoisePhase);

        /// - Compute speed error and speed error rate of change.
        double dSpeedError = -mSpeedError;
        mSpeedError = -1.0;
        if (speedCommand > FLT_EPSILON) {
            mSpeedError = (speedCommand - mSensedSpeed) / speedCommand;
        }
        dSpeedError += mSpeedError;

        /// - Control filter forcing function.
        mPulseWidth += mSpeedError * mForcingGain;

        /// - Control filter damping function.  Damping cuts out at small speed errors to avoid
        ///   instability.  The minimum of 0.0001 represent 0.01% of speed scale and avoids divide-
        ///   by-zero.  We also use this condition to end the start-up state for current limiting.
        if (fabs(mSpeedError) > mDampingCutoff) {
            mPulseWidth   += mDampingGain * dSpeedError / std::max(fabs(mSpeedError), 0.0001);
        } else {
            mStartupState  = false;
        }

        /// - Pulse width is always limited to 0-1.
        mPulseWidth = MsMath::limitRange(0.0, mPulseWidth, 1.0);

    } else {
        mPulseWidth    = 0.0;
        mSpeedError    = 0.0;
        mStartupState  = true;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Time step.
///
/// @returns  void
///
/// @details  Updates the motor with a control voltage.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsPumpMotorController::updateMotor(const double dt)
{
    if (mMalfPowerOverrideFlag) {
        mMotor.setVoltage(mPulseWidth * mMalfPowerOverrideValue);
    } else {
        mMotor.setVoltage(mPulseWidth * mVoltage);
    }

    /// - When in start-up state, send the start-up current limit to the motor to allow it to self-
    ///   limit its electrical current.  Otherwise set an arbitrary high limit to effectively
    ///   disable the limiting.
    if (mStartupState) {
        mMotor.setCurrentLimit(mStartupCurrentLimit);
    } else {
        mMotor.setCurrentLimit(1.0E15);
    }

    /// - The motor is always stepped regardless of power or command to allow it to coast to a stop.
    mMotor.step(dt);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  void
///
/// @details  Updates the total motor + controller power load and waste heat, for output to the
///           electrical & thermal aspects.  Power load is sent to the electrical aspect as a
///           resistance, since the majority of the load usually comes from the motor, which is not
///           a constant-power device.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsPumpMotorController::updatePowerLoad()
{
    double totalConductance = 0.0;
    mTotalWasteHeat         = 0.0;
    double voltage          = mVoltage;
    if (mControllerPowerBus) {
        if (mMalfPowerOverrideFlag) {
            voltage = mMalfPowerOverrideValue;
        }
        totalConductance += mControllerPowerLoad / std::max(DBL_EPSILON, voltage * voltage);
        mTotalWasteHeat  += mControllerPowerLoad;
    }
    if (mMotorPowerBus) {
        // motor's resistance is at it's controlled voltage, so need to convert back to supply
        // voltage to get the same power
        // P = V^2/R, P1 = P2, V1^2/R1 = V2^2/R2, R2 = R1*(V2/V1)^2
        totalConductance += mPulseWidth * mPulseWidth / std::max(DBL_EPSILON, mMotor.getResistance());
        mTotalWasteHeat  += mMotor.getWasteHeat();
    }
    mTotalResistiveLoad = 1.0 / std::max(DBL_EPSILON, totalConductance);
    mTotalCurrent       = voltage / mTotalResistiveLoad;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  (W) The electrical power load of the motor + controller.
///
/// @details  Returns the electrical power load of the motor + controller.
////////////////////////////////////////////////////////////////////////////////////////////////////
double TsPumpMotorController::getTotalPower() const
{
    return mVoltage * mTotalCurrent;
}

