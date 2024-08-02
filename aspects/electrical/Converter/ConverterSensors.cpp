/**
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
(
 (aspects/electrical/Converter/Converter.o)
 (common/sensors/SensorAnalog.o)
)
*/

#include "math/MsMath.hh"
#include "ConverterSensors.hh"

////////////////////////////////////////////////////////////////////////////////
/// @param[in]  standbyPower     (W)  Standby power when converter disables its output
/// @param[in]  outVoltageSensor (--) Output voltage sensor configuration data
/// @param[in]  outCurrentSensor (--) Output current sensor configuration data
///
/// @details Default constructs this ConverterSensors configuration data.
////////////////////////////////////////////////////////////////////////////////
ConverterSensorsConfigData::ConverterSensorsConfigData(
        const double standbyPower,
        const SensorAnalogConfigData& outVoltageSensor,
        const SensorAnalogConfigData& outCurrentSensor)
    :
    ConverterConfigData(standbyPower),
    outVoltageSensor(outVoltageSensor),
    outCurrentSensor(outCurrentSensor) {}

////////////////////////////////////////////////////////////////////////////////
/// @param[in]   that     (--)    ConverterSensors configuration data to be copied
///
/// @details Copy contructs this ConverterSensors configuration data.
////////////////////////////////////////////////////////////////////////////////
ConverterSensorsConfigData::ConverterSensorsConfigData(
        const ConverterSensorsConfigData& that)
    :
    ConverterConfigData(that) ,
    outVoltageSensor(that.outVoltageSensor),
    outCurrentSensor(that.outCurrentSensor)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////
/// @param[in]   that     (--)    ConverterSensors to copy.
///
/// @details Assignment operator for this ConverterSensors configuration data.
////////////////////////////////////////////////////////////////////////////////
ConverterSensorsConfigData& ConverterSensorsConfigData::operator =(const ConverterSensorsConfigData& that)
{
    if(this != &that) {
        ConverterConfigData::operator=(that);
        mStandbyPower                = that.mStandbyPower;
        outVoltageSensor             = that.outVoltageSensor;
        outCurrentSensor             = that.outCurrentSensor;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this ConverterSensors configuration data.
////////////////////////////////////////////////////////////////////////////////
ConverterSensorsConfigData::~ConverterSensorsConfigData()
{
    // nothing to do
}


////////////////////////////////////////////////////////////////////////////////
/// @param[in]  inputVoltage                     (V)   Input voltage to the converter
/// @param[in]  regulatedVoltage                 (V)   Regulated voltage coming out from the converter
/// @param[in]  efficiency                       (--)  Efficiency of the converter conversion process
/// @param[in]  outOverCurrentLimit              (amp) Output over current limit (hardware)
/// @param[in]  outOverVoltageLimit              (V)   Output over voltage limit (hardware)
/// @param[in]  outOverCurrentTripActive         (--)  Output over current trip active
/// @param[in]  outOverVoltageTripActive         (--)  Output over voltage trip active
/// @param[in]  malfOverrideOutOverCurrentLimit  (--)  Activate override of hardware over current
/// @param[in]  malfOverrideOutOverVoltageLimit  (--)  Activate override of out over voltage limit
/// @param[in]  malfOverrideOutVoltage           (--)  Activate override of output voltage
/// @param[in]  malfConverterFailed              (--)  Activate the fail converter flag
///
/// @details Default destructs this ConverterSensors input data.
////////////////////////////////////////////////////////////////////////////////
ConverterSensorsInputData::ConverterSensorsInputData(
                                    const double inputVoltage,
                                    const double outputPower,
                                    const double regulatedVoltage,
                                    const double efficiency,
                                    const double outOverCurrentLimit,
                                    const double outOverVoltageLimit,
                                    const bool outOverCurrentTripActive,
                                    const bool outOverVoltageTripActive,
                                    const double inOverVoltageLimit,
                                    const double inUnderVoltageLimit,
                                    const bool inOverVoltageTripActive,
                                    const bool inUnderVoltageTripActive,
                                    const SensorAnalogInputData&    outVoltageSensor,
                                    const SensorAnalogInputData&    outCurrentSensor)
    :
    ConverterInputData(inputVoltage, regulatedVoltage, efficiency, outOverCurrentLimit,
                       outOverVoltageLimit, outOverCurrentTripActive, outOverVoltageTripActive,
                       inOverVoltageLimit, inUnderVoltageLimit, inOverVoltageTripActive,
                       inUnderVoltageTripActive),
    mOutputPower(outputPower),
    outVoltageSensor(outVoltageSensor), // default sensor data
    outCurrentSensor(outCurrentSensor)  // default sensor data
{
    // nothing to do
}


////////////////////////////////////////////////////////////////////////////////
/// @param[in]    that    --    ConverterSensors Input Data to be copied
///
/// @details Copy constructs this ConverterSensors input data.
////////////////////////////////////////////////////////////////////////////////
ConverterSensorsInputData::ConverterSensorsInputData(
        const ConverterSensorsInputData& that)
    :
    ConverterInputData(that),
    mOutputPower(that.mOutputPower),
    outVoltageSensor(that.outVoltageSensor),
    outCurrentSensor(that.outCurrentSensor)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////
/// @param[in]   that     (--)    ConverterWithSensors object to be copied.
///
/// @details  Assignment operator for this ConverterSensors configuration data.
////////////////////////////////////////////////////////////////////////////////
ConverterSensorsInputData& ConverterSensorsInputData::operator =(const ConverterSensorsInputData& that)
{
    if(this != &that) {
        ConverterInputData::operator=(that);
        mOutputPower                = that.mOutputPower;
        outVoltageSensor            = that.outVoltageSensor;
        outCurrentSensor            = that.outCurrentSensor;
    }
    return *this;
}


////////////////////////////////////////////////////////////////////////////////
/// @details    ConverterSensors Input Data destructor
////////////////////////////////////////////////////////////////////////////////
ConverterSensorsInputData::~ConverterSensorsInputData()
{
    // nothing to do
}


////////////////////////////////////////////////////////////////////////////////
/// @details ConverterSensors constructor.
////////////////////////////////////////////////////////////////////////////////
ConverterSensors::ConverterSensors()
    :
    outVoltageSensor(),
    outCurrentSensor(),
    mMalfOverrideOutOverCurrentLimit(false),
    mMalfOverrideOutOverCurrentValue(0.0),
    mMalfOverrideOutOverVoltageLimit(false),
    mMalfOverrideOutOverVoltageValue(0.0),
    mMalfOverrideOutVoltage(false),
    mMalfOverrideOutVoltageValue(0.0),
    mMalfConverterFailed(false),
    outVoltageSensorName("OutVoltageSensor"),
    outCurrentSensorName("OutCurrentSensor"),
    mDefaultOutOverCurrentLimit(0.0),
    mDefaultOutOverVoltageLimit(0.0),
    mDefaultOutVoltage(0.0),
    mConverterFailed(false),
    mConverterOnCmd(false),
    mPrevConverterOnCmd(false),
    mOutputCurrentSensed(0.0),
    mOutputVoltageSensed(0.0),
    mInitFlag(false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////
/// @details ConverterSensors destructor.
////////////////////////////////////////////////////////////////////////////////
ConverterSensors::~ConverterSensors() {
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////
/// @param[in]    configData    (--)    Configuration data for ConverterSensors model
/// @param[in]    inputData     (--)    Input data for ConverterSensors model
/// @param[in]    name          (--)    Converter name
///
/// @details Checks if all configuration data and input data are valid
///////////////////////////////////////////////////////////////////////////////////////////////////
void ConverterSensors::initialize(const ConverterSensorsConfigData& configData,
                                      const ConverterSensorsInputData&  inputData)
{

    /// -- Resets init flag.
    mInitFlag = false;

    /// - Converter validate and initialize
    bool converterInit = false;
    converterInit = Converter::initialize(&configData, &inputData);

    /// -- Initialize from input data.
    mOutputPower                     = inputData.mOutputPower;
    mDefaultOutOverCurrentLimit      = inputData.mOutputOverCurrentLimit;
    mDefaultOutOverVoltageLimit      = inputData.mOutputOverVoltageLimit;
    mDefaultOutVoltage               = inputData.mRegulatedVoltage;

    mMalfOverrideOutOverCurrentLimit = false;
    mMalfOverrideOutOverVoltageLimit = false;
    mMalfOverrideOutVoltage          = false;
    mMalfConverterFailed             = false;

    outVoltageSensor.initialize(configData.outVoltageSensor, inputData.outVoltageSensor,
            outVoltageSensorName.c_str());
    outCurrentSensor.initialize(configData.outCurrentSensor, inputData.outCurrentSensor,
            outCurrentSensorName.c_str());

    mInitFlag = converterInit and
                outVoltageSensor.isInitialized() and
                outCurrentSensor.isInitialized();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Method to validate configuration and input data to the converter.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ConverterSensors::validate() {
    /// - Call base method to do all the checks.
    Converter::validate();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   timeStep  (s)  Time step
///
/// @details  Method to update converter functionality
////////////////////////////////////////////////////////////////////////////////////////////////////
void ConverterSensors::step(double timeStep) {
    /// -- Reset trips if commanded or if converter is commanded off
    if ((not mConverterOnCmd and mPrevConverterOnCmd) or mResetTrips) {
        resetTrips();
    }

    /// -- Handled override malfunctions if commanded
    handleOverrideMalfs();

    /// -- Set converter failed malfunctions if commanded
    mConverterFailed = mMalfConverterFailed;

    /// -- Updates converter state depending on given inputs
    updateConverterState();

    /// - Set output voltage depending on status of the converter
    if (mConverterOn) {
        mOutputVoltage = mRegulatedVoltage;
    } else {
        mOutputVoltage = 0.0;
    }

    /// -- Compute output current if converter is On
    if (mOutputVoltage > DBL_EPSILON and mConverterOn) {
        mOutputCurrent = mOutputPower / mOutputVoltage;
    } else {
        mOutputCurrent = 0.0;
    }

    /// -- Update sensors
    updateSensors(timeStep);

    /// -- Handle output inhibit functionality
    handleTrips();
    updateConverterState();

    /// -- Compute total power consumed by this converter if no trip occured
    if (mEfficiency > DBL_EPSILON and mConverterOn) {
        mInputPower = mStandbyPower + (mOutputPower / mEfficiency);
    } else {
        mInputPower = 0.0;
        mInputCurrent = 0.0;
        mOutputCurrent = 0.0;
        mOutputPower = 0.0;
        mOutputVoltage = 0.0;
    }

    /// -- Compute input current
    if (mInputVoltage > DBL_EPSILON) {
        mInputCurrent = mInputPower / mInputVoltage;
    } else {
        mInputCurrent = 0.0;
    }

    /// -- Calculate heat dissipation
    if (mInputPower > DBL_EPSILON) {
        calculateHeatDissipation();
    }

    mPrevConverterOnCmd = mConverterOnCmd;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Time step
///
/// @details    Method to update sensor values
////////////////////////////////////////////////////////////////////////////////////////////////////
void ConverterSensors::updateSensors(const double dt) {
    mOutputVoltageSensed = outVoltageSensor.sense(dt, mConverterOn, mOutputVoltage);
    mOutputCurrentSensed = outCurrentSensor.sense(dt, mConverterOn, mOutputCurrent);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  converterCmdOn (--) command converter on/off
///
/// @details    Commands the converter on or off.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ConverterSensors::cmdConverterOn(const bool converterCmdOn)
{
    if(not isConverterOn() and isConverterCmdOn())
    {
        if (isTripped()) {
            resetTrips();
        }
        setConverterOn(converterCmdOn);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method to handle override malfunctions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ConverterSensors::handleOverrideMalfs() {

    /// -- Malfunction to override output over current limit
    mOutputOverCurrentLimit = mDefaultOutOverCurrentLimit;
    if (mMalfOverrideOutOverCurrentLimit) {
        mOutputOverCurrentLimit = mMalfOverrideOutOverCurrentValue;
    }

    /// -- Malfunction to override output over voltage limit
    mOutputOverVoltageLimit = mDefaultOutOverVoltageLimit;
    if (mMalfOverrideOutOverVoltageLimit) {
        mOutputOverVoltageLimit = mMalfOverrideOutOverVoltageValue;
    }

    /// -- Malfunction to override regulated voltage
    mRegulatedVoltage = mDefaultOutVoltage;
    if (mMalfOverrideOutVoltage) {
         mRegulatedVoltage = mMalfOverrideOutVoltageValue;
    }

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Method to handle inhibit trip events
////////////////////////////////////////////////////////////////////////////////////////////////
void ConverterSensors::handleTrips() {
    mOutputOverVoltageTrip =  ((mOutputVoltageSensed >= mOutputOverVoltageLimit) and mOutOverVoltageTripActive) or mOutputOverVoltageTrip;

    mOutputOverCurrentTrip =  ((mOutputCurrentSensed >= mOutputOverCurrentLimit) and mOutOverCurrentTripActive) or mOutputOverCurrentTrip;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method to set trips
////////////////////////////////////////////////////////////////////////////////////////////////////
void ConverterSensors::setTrips() {
    if ( (mOutputVoltageSensed >= mOutputOverVoltageLimit) and  mOutOverVoltageTripActive ) {
        mOutputOverVoltageTrip = true;
    }

    if ( (mOutputCurrentSensed >= mOutputOverCurrentLimit) and  mOutOverCurrentTripActive ) {
         mOutputOverCurrentTrip = true;
    }
}
