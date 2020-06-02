/**
@file
@brief     GUNNS Analog Sensor Wrapper Spotter implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((GunnsNetworkSpotter.o)
   (simulation/hs/TsHsMsg.o)
   (software/exceptions/TsInitializationException.o)
   (common/sensors/SensorAnalog.o)
*/

#include "GunnsSensorAnalogWrapper.hh"
#include "GunnsBasicNode.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name              (--) Instance name for self-identification in messages.
/// @param[in] minRange          (--) Minimum sensed output value, in output units.
/// @param[in] maxRange          (--) Maximum sensed output value, in output units.
/// @param[in] offValue          (--) Sensed output value when the sensor is off, in output units.
/// @param[in] nominalBias       (--) Nominal bias in the sensed output, in output units.
/// @param[in] nominalScale      (--) Nominal scale factor applied to the sensed output.
/// @param[in] nominalNoiseScale (--) Nominal constant noise amplitude in the sensed output, in output units.
/// @param[in] nominalResolution (--) Nominal sensor resolution, in output units.
/// @param[in] noiseFunction     (--) Pointer to noise function for this sensor.
/// @param[in] unitConversion    (--) Unit conversion type to convert truth input to output units.
/// @param[in] nominalNoiseMult  (--) Nominal input-scaling noise amplitude in the sensed output, in fraction of total sensor output range.
///
/// @details  Default constructs this GUNNS Analog Sensor Wrapper Spotter configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsSensorAnalogWrapperConfigData::GunnsSensorAnalogWrapperConfigData(
        const std::string&         name,
        const float                minRange,
        const float                maxRange,
        const float                offValue,
        const float                nominalBias,
        const float                nominalScale,
        const float                nominalNoiseScale,
        const float                nominalResolution,
        fctnPtr                    noiseFunction,
        const UnitConversion::Type unitConversion,
        const float                nominalNoiseMult)
    :
    GunnsNetworkSpotterConfigData(name),
    mSensor(minRange, maxRange, offValue, nominalBias, nominalScale, nominalNoiseScale,
            nominalResolution, noiseFunction, unitConversion, nominalNoiseMult)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Analog Sensor Wrapper Spotter configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsSensorAnalogWrapperConfigData::~GunnsSensorAnalogWrapperConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] powerFlag  (--) Initial powered state of the sensor.
/// @param[in] truthInput (--) Initial value of the input truth value to sense.
///
/// @details  Default constructs this GUNNS Analog Sensor Wrapper Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsSensorAnalogWrapperInputData::GunnsSensorAnalogWrapperInputData(const bool   powerFlag,
                                                                     const double truthInput)
    :
    mSensor(powerFlag, truthInput)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Analog Sensor Wrapper Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsSensorAnalogWrapperInputData::~GunnsSensorAnalogWrapperInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Default constructs this GUNNS Analog Sensor Wrapper Spotter.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsSensorAnalogWrapper::GunnsSensorAnalogWrapper()
    :
    GunnsNetworkSpotter(),
    mSensor(),
    mStepPreSolverFlag(false),
    mStepPostSolverFlag(false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Analog Sensor Wrapper Spotter.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsSensorAnalogWrapper::~GunnsSensorAnalogWrapper()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  Instance configuration data.
/// @param[in]  inputData   (--)  Instance input data.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Analog Sensor Wrapper Spotter with its configuration and
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsSensorAnalogWrapper::initialize(const GunnsNetworkSpotterConfigData* configData,
                                          const GunnsNetworkSpotterInputData*  inputData)
{
    /// - Initialize the base class.
    GunnsNetworkSpotter::initialize(configData, inputData);

    /// - Reset the init flag.
    mInitFlag = false;

    /// - Validate & type-cast config & input data.
    const GunnsSensorAnalogWrapperConfigData* config = validateConfig(configData);
    const GunnsSensorAnalogWrapperInputData*  input  = validateInput(inputData);

    /// - Initialize the sensor.
    mSensor.initialize(config->mSensor, input->mSensor, mName + ".mSensor");

    /// - By default, sensor will be stepped after the network solution.  This can be changed later
    ///   by calling these setter functions from outside.
    setStepPreSolverFlag(false);
    setStepPostSolverFlag(true);

    /// - Set the init flag.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  Instance configuration data.
///
/// @returns  GunnsSensorAnalogWrapperConfigData (--) Type-casted and validated config data pointer.
///
/// @throws   TsInitializationException
///
/// @details  Type-casts the base config data class pointer to this spotter's config data type,
///           checks for valid type-cast and validates contained data.
////////////////////////////////////////////////////////////////////////////////////////////////////
const GunnsSensorAnalogWrapperConfigData* GunnsSensorAnalogWrapper::validateConfig(const GunnsNetworkSpotterConfigData* config)
{
    const GunnsSensorAnalogWrapperConfigData* result = dynamic_cast<const GunnsSensorAnalogWrapperConfigData*>(config);
    if (!result) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Bad config data pointer type.");
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  inputData  (--)  Instance input data.
///
/// @returns  GunnsSensorAnalogWrapperInputData (--) Type-casted and validated input data pointer.
///
/// @throws   TsInitializationException
///
/// @details  Type-casts the base input data class pointer to this spotter's input data type,
///           checks for valid type-cast and validates contained data.
////////////////////////////////////////////////////////////////////////////////////////////////////
const GunnsSensorAnalogWrapperInputData* GunnsSensorAnalogWrapper::validateInput(const GunnsNetworkSpotterInputData* input)
{
    const GunnsSensorAnalogWrapperInputData* result = dynamic_cast<const GunnsSensorAnalogWrapperInputData*>(input);
    if (!result) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Bad input data pointer type.");
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Execution time step.
///
/// @details  Updates the sensor prior to the network solver update and solution.  Only updates the
///           sensor if the pre-step control flag is set.  If the post-step flag is also set, the
///           post-step option takes priority over pre-step so we won't step here.  We don't allow
///           the sensor to be updated at both times because this would double up its drift malf
///           integration.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsSensorAnalogWrapper::stepPreSolver(const double dt)
{
    if (mStepPreSolverFlag and not mStepPostSolverFlag) {
        mSensor.update(dt);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Execution time step.
///
/// @details  Updates the sensor after the network solver update and solution if the post-step
///           control flag is set.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsSensorAnalogWrapper::stepPostSolver(const double dt)
{
    if (mStepPostSolverFlag) {
        mSensor.update(dt);
    }
}
