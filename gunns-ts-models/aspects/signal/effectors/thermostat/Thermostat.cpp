/************************************** TRICK HEADER ***********************************************
@file     Thermostat.cpp
@brief    Thermostat model implementation.

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    (
    (aspects/signal/effectors/thermostat/HtrControl.o)
    (common/sensors/SensorAnalog.o)
    )
***************************************************************************************************/
#include "Thermostat.hh"
#include "aspects/thermal/PtcsMacros.hh"
#include <string>
#include <iostream> //needed for std::ostringstream in initialize()

/**************************************************************************************************/
/* ConfigData class */
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  sensorConfig         (--)  config data for Thermostat's sensor(s)
/// @param[in]  temperatureSetptLow  (K)   nominal thermostat low setpoint
/// @param[in]  temperatureSetptHigh (K)   nominal thermostat high setpoint
/// @param[in]  numTempSensors       (--)  number of temperature sensors monitored by thermostat
/// @param[in]  logicType            (--)  type of logic to apply to sensor readings;
///                                        (ThermostatConfigData::SINGLE, AND, or OR)
///
/// @details    Constructs this Thermostat model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
ThermostatConfigData::ThermostatConfigData(SensorAnalogConfigData& sensorConfig,
                                           const double temperatureSetptLow,
                                           const double temperatureSetptHigh,
                                           const int numTempSensors,
                                           const LogicType logicType)
    :
    HtrControlConfigData(),
    cSensorConfig(sensorConfig),
    cNominalSetptLow(temperatureSetptLow),
    cNominalSetptHigh(temperatureSetptHigh),
    cNumSensors(numTempSensors),
    cLogicType(logicType)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  source to copy
///
/// @details    Copy constructs this Thermostat model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
ThermostatConfigData::ThermostatConfigData(const ThermostatConfigData& that)
    :
    HtrControlConfigData(that),
    cSensorConfig(that.cSensorConfig),
    cNominalSetptLow(that.cNominalSetptLow),
    cNominalSetptHigh(that.cNominalSetptHigh),
    cNumSensors(that.cNumSensors),
    cLogicType(that.cLogicType)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default destructs this Thermostat model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
ThermostatConfigData::~ThermostatConfigData()
{
    // nothing to do
}


/**************************************************************************************************/
/* InputData class */
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  sensorInput         (--)  Input data for Thermostat's sensor(s)
/// @param[in]  malfCmdOnFlag       (--)  Malfunction flag, heater command on
/// @param[in]  malfCmdOffFlag      (--)  Malfunction flag, heater command off
/// @param[in]  malfSetptLowFlag    (--)  Malfunction flag, override temp setpoint-low
/// @param[in]  malfSetptHighFlag   (--)  Malfunction flag, override temp setpoint-high
/// @param[in]  malfSetptLowValue   (K)   Malfunction value, override setpoint-low
/// @param[in]  malfSetptHighValue  (K)   Malfunction value, override setpoint-high
///
/// @details    Default constructs this Thermostat model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
ThermostatInputData::ThermostatInputData(SensorAnalogInputData& sensorInput,
                                         const bool malfCmdOnFlag,
                                         const bool malfCmdOffFlag,
                                         const bool malfSetptLowFlag,
                                         const bool malfSetptHighFlag,
                                         const double malfSetptLowValue,
                                         const double malfSetptHighValue)
    :
    HtrControlInputData(malfCmdOnFlag, malfCmdOffFlag),
    iSensorInput(sensorInput),
    iMalfSetptLowFlag(malfSetptLowFlag),
    iMalfSetptHighFlag(malfSetptHighFlag),
    iMalfSetptLowValue(malfSetptLowValue),
    iMalfSetptHighValue(malfSetptHighValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  source to copy
///
/// @details    Copy constructs this Thermostat model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
ThermostatInputData::ThermostatInputData(const ThermostatInputData& that)
    :
    HtrControlInputData(that),
    iSensorInput(that.iSensorInput),
    iMalfSetptLowFlag(that.iMalfSetptLowFlag),
    iMalfSetptHighFlag(that.iMalfSetptHighFlag),
    iMalfSetptLowValue(that.iMalfSetptLowValue),
    iMalfSetptHighValue(that.iMalfSetptHighValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default destructs this Thermostat model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
ThermostatInputData::~ThermostatInputData()
{
    // nothing to do
}


/**************************************************************************************************/
/* Thermostat class */
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default Thermostat constructor. Defaults all pointers and members to zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
Thermostat::Thermostat()
    :
    HtrControl(),
    mTempSensor(0),
    mMalfSetptLowFlag(false),
    mMalfSetptHighFlag(false),
    mMalfSetptLowValue(0.0),
    mMalfSetptHighValue(0.0),
    mNumSensors(0),
    mLogicType(ThermostatConfigData::SINGLE),
    mCommandSetpt(LoadOFF),
    mCommandSetptBias(0.0),
    mTemperatureSetpt(0.0),
    mSetptLow(0.0),
    mSetptHigh(0.0),
    mNominalTemperatureSetpt(0.0),
    mNominalSetptLow(0.0),
    mNominalSetptHigh(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructor, deletes allocated arrays.
////////////////////////////////////////////////////////////////////////////////////////////////////
Thermostat::~Thermostat()
{
    TS_DELETE_ARRAY(mTempSensor);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  config  (--)  configuration data
/// @param[in]  input   (--)  input data
/// @param[in]  name    (--)  Thermostat name
///
/// @throw      TsInitializationException in validate() method.
///
/// @details    Initializes this Thermostat model from configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void Thermostat::initialize(const ThermostatConfigData& config,
                            const ThermostatInputData& input,
                            const std::string& name)
{
    /// - First initialize & validate parent.
    HtrControl::initialize(config, input, name);

    /// - Reset the initialization complete flag.
    mInitialized = false;

    /// - Validate the configuration and input data.
    validate(config, input);

    /// - Initialize from the validated config data.
    mNominalSetptLow = config.cNominalSetptLow;
    mNominalSetptHigh = config.cNominalSetptHigh;
    mNominalTemperatureSetpt = 0.5 * (mNominalSetptLow + mNominalSetptHigh);
    mNumSensors = config.cNumSensors;
    mLogicType = config.cLogicType;

    /// - Initialize setpoints to nominal.
    mTemperatureSetpt = mNominalTemperatureSetpt;
    mSetptLow = mNominalSetptLow;
    mSetptHigh = mNominalSetptHigh;

    /// - Initialize from the validated input data.
    mMalfSetptLowFlag = input.iMalfSetptLowFlag;
    mMalfSetptHighFlag = input.iMalfSetptHighFlag;
    mMalfSetptLowValue = input.iMalfSetptLowValue;
    mMalfSetptHighValue = input.iMalfSetptHighValue;

    /// - Allocate the sensor array.
    TS_DELETE_ARRAY(mTempSensor);
    TS_NEW_CLASS_ARRAY_EXT(mTempSensor, mNumSensors, SensorAnalog, (), mName + ".mTempSensor" );

    /// - Loop through all sensors.
    for(int i = 0; i < mNumSensors; ++i)
    {
        /// - Store a sensor name based on Thermostat name and the number in series.
        std::ostringstream sensorName;
        sensorName << mName << "_rtd" << i;

        /// - Initialize each sensor.
        mTempSensor[i].initialize(config.cSensorConfig, input.iSensorInput, sensorName.str() );
    }

    /// - Set the initialization complete flag.
    mInitialized = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  config  (--)  configuration data
/// @param[in]  input   (--)  input data
///
/// @throw      TsInitializationException if config/input parameters not valid.
///
/// @details    Validates this Thermostat model configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void Thermostat::validate(const ThermostatConfigData& config,
                          const ThermostatInputData& input) const
{
    /// - Throw an exception if the number of sensors is less than one.
    TS_PTCS_IF_ERREX(config.cNumSensors < 1, TsInitializationException,
            "Invalid config data", "Thermostat should have at least one sensor.")

    /// - Throw an exception if the logic type is not valid.
    TS_PTCS_IF_ERREX(config.cNumSensors > 1 && ThermostatConfigData::SINGLE == config.cLogicType,
        TsInitializationException,
        "Invalid config data", "Multi-sensor thermostat must have an AND or OR logic type")

    /// - Throw an exception if the low setpt is greater than or equal to the high.
    TS_PTCS_IF_ERREX(config.cNominalSetptLow >= config.cNominalSetptHigh, TsInitializationException,
        "Invalid config data", "Low setpt >= high setpt")

    /// - Throw an exception if malf low setpt is greater than or equal to the high.
    TS_PTCS_IF_ERREX(input.iMalfSetptLowValue >= input.iMalfSetptHighValue, TsInitializationException,
        "Invalid input data", "Malf override low setpt >= high setpt")
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  timeStep  (s)   time step (passed to sensor update)
///
/// @throw      TsInitializationException if Thermostat not initialized.
///
/// @details    Updates this Thermostat model. Analyzes sensor readings and sets both
///             mCommandNominal and mCommandSetpt members. (These two command states will be
///             the same if the setpoint malf is not active.) Then, the command malf is considered,
///             and mCommandActual is set.
///
/// @note       mCommandSetpt is maintained as a separate member for one specific
///             scenario, when both Command:ON/OFF and Setpoint-override malfunctions are active.
///             At the point when Command:ON/OFF is de-activated, the Thermostat needs to return
///             to the malf'd-setpoint command state. This may or may not be equal to the
///             Nominal command status, which represents the status at a state where no malfunctions
///             are active.
////////////////////////////////////////////////////////////////////////////////////////////////////
void Thermostat::update(const double timeStep)
{
    /// - Check initialization.
    verifyInitialization();

    /// - Loop through all sensors.
    for(int i = 0; i < mNumSensors; ++i)
    {
        /// - Update() each sensor.
        mTempSensor[i].update(timeStep);
    }

    /// - Compute setpoint bias. mTemperatureSetpt may have changed via setter or direct entry.
    mCommandSetptBias = mTemperatureSetpt - mNominalTemperatureSetpt;

    /// - Reset the setpoints to nominal state. They might be overwritten by malf's.
    mSetptLow  = mNominalSetptLow  + mCommandSetptBias;
    mSetptHigh = mNominalSetptHigh  + mCommandSetptBias;

    /// - Create a default command mode variable based on the previous time step.
    UserLoadMode defaultCommand = mCommandNominal;

    /// - Determine nominal heater command using specifically the nominal temperature setpoints.
    mCommandNominal = readSensors(defaultCommand);

    /// - Consider the two setpoint malfs. If none are active, the default command will stay
    ///   at the nominal state. If one or both are active, the command should default
    ///   to the last setpoint command, which may/may not have been previously malf'd.
    if (mMalfSetptLowFlag)
    {
        defaultCommand = mCommandSetpt;
        mSetptLow = mMalfSetptLowValue;
    }
    if (mMalfSetptHighFlag)
    {
        defaultCommand = mCommandSetpt;
        mSetptHigh = mMalfSetptHighValue;
    }

    /// - Determine a CommandSetpt based on the setpoints, which may have been malf'd.
    mCommandSetpt = readSensors(defaultCommand);

    /// - Consider the Command:ON/OFF malfunction if active, otherwise the CommandActual will be
    ///   the same as the CommandSetpt.
    mCommandActual = considerCommandMalfs(mCommandSetpt);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  commandStatus (--)  default UserLoadMode status
///
/// @return     UserLoadMode  (--)  net LoadON / LoadOFF command status resulting from reading
///                                 the sensors
///
/// @details    Loops through all the thermostat's sensors, searching for a sensor that
///             evaluates to the "Primary" load mode, either ON or OFF.
///             Once a Primary is found, the loop breaks and the Primary command state is returned.
///             If no Primary is found, the the "Secondary" command state is returned.
///             Primary and Secondary values are specific to the logic pattern of the
///             Thermostat instance (single, OR, AND).
////////////////////////////////////////////////////////////////////////////////////////////////////
UserLoadMode Thermostat::readSensors(const UserLoadMode commandStatus)
{
    /// - Loop through all sensors.
    for (int i = 0; i < mNumSensors; ++i)
    {
        /// - Declare a temporary variable that represents the sensed value of this specific sensor.
        double sensedTemp = mTempSensor[i].getSensedOutput();

        /// - Determine the command status for this particular temperature.
        UserLoadMode commandFromThisSensor = evaluateAgainstSetpoints( commandStatus, sensedTemp);

        /// - If the command state evaluated from this one sensor matches the "Primary" load mode...
        if (returnLoadModePrimary() == commandFromThisSensor)
        {
            /// - ...return the Primary. All it takes is one Primary to determine the return value.
            return returnLoadModePrimary();
        }
    }

    /// - If all the sensors were read and not one evaluated to the Primary load mode, return the
    ///   "Secondary" load mode.
    return returnLoadModeSecondary();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  commandStatus (--)  default UserLoadMode status to return if temp b/t setpoints
/// @param[in]  sensedTemp    (K)   temperature reading from sensor
///
/// @return     UserLoadMode  (--)  LoadON / LoadOFF command status resulting from these setpts
///
/// @details    Command status is determined based on the input sensed temperature and the given
///             setpoints. If the temperature is under the low setpoint, the command is returned
///             LoadON. If the temperature is above the high setpoint, the command is returned
///             LoadOFF. If the temperature is within both setpoints, the command is returned
///             unchanged from its passed-in value.
////////////////////////////////////////////////////////////////////////////////////////////////////
UserLoadMode Thermostat::evaluateAgainstSetpoints(const UserLoadMode commandStatus,
                                                  const double sensedTemp)
{
    /// - Create temporary UserLoadMode.
    UserLoadMode result = commandStatus;

    /// - Too hot. Sensor temperature above high setpoint.
    if (sensedTemp > mSetptHigh)
    {
        /// - Set heater command to OFF.
        result = LoadOFF;

    /// - Too cold. Sensor temperature under low setpoint.
    } else if (sensedTemp < mSetptLow)
    {
        /// - Set heater command to ON.
        result = LoadON;
    }

    /// - Return result. Note that if the given temperature was in between the two setpoints,
    ///   the result remains unchanged from the original commandStatus.
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   UserLoadMode  (--)  Primary command mode based on logic-pattern for this thermostat.
///
/// @details  Returns Primary load mode for this thermostat. The "Primary" load mode is defined
///           as such: When just one of a thermostat's sensors evaluate to the Primary mode,
///           the overall command for the thermostat is set to the Primary mode.
///
/// @note     (AND) Since sensors are in series, it takes only one to evaluate to "LoadOFF" for
///                 an 'AND' thermostat to trip OFF. LoadOFF is returned as its Primary load mode.
///           (OR)  Since sensors are in parallel, it takes only one to evaluate to "LoadON" for
///                 an 'OR' thermostat to trip ON. LoadON is returned as its Primary load mode.
///
/// @note     (SINGLE) Since only one sensor is being read, it doesn't matter which is the Primary
///           mode for a single-sensor thermostat. LoadON is returned just to pick one.
////////////////////////////////////////////////////////////////////////////////////////////////////
UserLoadMode Thermostat::returnLoadModePrimary() const
{
    if (ThermostatConfigData::AND == mLogicType)
    {
        /// - A multi-sensor:AND thermostat returns LoadOFF as its Primary UserLoadMode.
        return LoadOFF;

    } else
    {
        /// - A single-sensor or multi-sensor:OR thermostat has LoadON as its Primary UserLoadMode.
        return LoadON;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   UserLoadMode  (--)  Secondary command mode based on logic-pattern for this thermostat.
///
/// @details  Returns Secondary load mode for this thermostat. The "Secondary"  load mode is defined
///           as such: If NONE of a thermostat's sensors evaluate to the Primary mode,
///           the overall command for the thermostat is set to the Secondary.
///
/// @note     (AND) Since sensors are in series, ALL of them must evaluate to "LoadON" for
///                 an 'AND' thermostat to trip ON. LoadON is returned as its Secondary load mode.
///           (OR)  Since sensors are in parallel, ALL of them must evaluate to "LoadOFF" for
///                 an 'OR' thermostat to trip OFF. LoadOFF is returned as its Secondary load mode.
///
/// @note     (SINGLE) Since only one sensor is being read, it doesn't matter which is the Seconday
///           mode for a single-sensor thermostat, as long is it's opposite to the Primary mode.
////////////////////////////////////////////////////////////////////////////////////////////////////
UserLoadMode Thermostat::returnLoadModeSecondary() const
{
    if (ThermostatConfigData::AND == mLogicType)
    {
        /// - A multi-sensor:AND thermostat returns LoadON as its Secondary UserLoadMode.
        return LoadON;

    } else
    {
        /// - Single-sensor or multi-sensor:OR thermostat has LoadOFF as its Secondary UserLoadMode.
        return LoadOFF;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  bias  (K)  Value to set the commanded setpoint bias to.
///
/// @details  Sets mCommandSetptBias to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
void Thermostat::setTemperatureSetpt(const double newTempSetpt)
{
        mTemperatureSetpt = newTempSetpt;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  bias  (K)  Value to set the commanded setpoint bias to.
///
/// @details  Sets mCommandSetptBias to the given value and back computes mTemperatureSetpt.
////////////////////////////////////////////////////////////////////////////////////////////////////
void Thermostat::setCommandSetptBias(const double bias)
{
    mCommandSetptBias = bias;
    /// - Compute new temperature setpoint. This maintains backwards compatibility with
    ///   applications that are using this method.
    mTemperatureSetpt = mNominalTemperatureSetpt + mCommandSetptBias;
}
