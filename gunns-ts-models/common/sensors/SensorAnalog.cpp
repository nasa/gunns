/***************************************** TRICK HEADER ********************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    ((math/UnitConversion.o)
     (SensorBase.o))

PROGRAMMERS:
    ((Jason Harvey) (L-3 Communications) (2012-04) (Initial))

 **************************************************************************************************/

#include "SensorAnalog.hh"
#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "math/MsMath.hh"
#include <cmath> // fabs

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] minRange          (--) Minimum sensed output value, in output units
/// @param[in] maxRange          (--) Maximum sensed output value, in output units
/// @param[in] offValue          (--) Sensed output value when the sensor is off, in output units
/// @param[in] nominalBias       (--) Nominal bias in the sensed output, in output units
/// @param[in] nominalScale      (--) Nominal scale factor applied to the sensed output
/// @param[in] nominalNoiseScale (--) Nominal constant noise amplitude in the sensed output, in output units
/// @param[in] nominalResolution (--) Nominal sensor resolution, in output units
/// @param[in] noiseFunction     (--) Pointer to noise function for this sensor
/// @param[in] unitConversion    (--) Unit conversion type to convert truth input to output units
/// @param[in] nominalNoiseMult  (--) Nominal input-scaling noise amplitude in the sensed output, in fraction of total sensor output range
///
/// @details  Constructs Analog Sensor config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorAnalogConfigData::SensorAnalogConfigData(const float                minRange,
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
    SensorBaseConfigData(),
    mMinRange           (minRange),
    mMaxRange           (maxRange),
    mOffValue           (offValue),
    mNominalBias        (nominalBias),
    mNominalScale       (nominalScale),
    mNominalNoiseScale  (nominalNoiseScale),
    mNominalResolution  (nominalResolution),
    mNoiseFunction      (noiseFunction),
    mUnitConversion     (unitConversion),
    mNominalNoiseMult   (nominalNoiseMult)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Analog Sensor config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorAnalogConfigData::~SensorAnalogConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  Copy constructs Analog Sensor config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorAnalogConfigData::SensorAnalogConfigData(const SensorAnalogConfigData& that)
    :
    SensorBaseConfigData(that),
    mMinRange           (that.mMinRange),
    mMaxRange           (that.mMaxRange),
    mOffValue           (that.mOffValue),
    mNominalBias        (that.mNominalBias),
    mNominalScale       (that.mNominalScale),
    mNominalNoiseScale  (that.mNominalNoiseScale),
    mNominalResolution  (that.mNominalResolution),
    mNoiseFunction      (that.mNoiseFunction),
    mUnitConversion     (that.mUnitConversion),
    mNominalNoiseMult   (that.mNominalNoiseMult)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to assign to
///
/// @details  Assignment operator for the Analog Sensor config data class.  The mNoiseFunction
///           function pointer is a shallow copy since it points to an external object.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorAnalogConfigData& SensorAnalogConfigData::operator =(const SensorAnalogConfigData& that)
{
    if (this != &that)
    {
        SensorBaseConfigData::operator = (that);
        mMinRange          = that.mMinRange;
        mMaxRange          = that.mMaxRange;
        mOffValue          = that.mOffValue;
        mNominalBias       = that.mNominalBias;
        mNominalScale      = that.mNominalScale;
        mNominalNoiseScale = that.mNominalNoiseScale;
        mNominalResolution = that.mNominalResolution;
        mNoiseFunction     = that.mNoiseFunction;
        mUnitConversion    = that.mUnitConversion;
        mNominalNoiseMult  = that.mNominalNoiseMult;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] powerFlag  (--) Initial powered state of the sensor
/// @param[in] truthInput (--) Initial value of the input truth value to sense
///
/// @details  Default constructs this Analog Sensor input data.  The malfunction terms are only
///           present in the input data class to support unit testing of parent objects that contain
///           this sensor in an aggregate.  We deliberately omit them as arguments to this
///           constructor to reduce cluttering the method signature.  Users can still set the malf
///           input data after the object has been constructed and before the sensor is initialized.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorAnalogInputData::SensorAnalogInputData(const bool   powerFlag,
                                             const double truthInput)
    :
    SensorBaseInputData (powerFlag),
    mTruthInput         (truthInput),
    mMalfFailToValue    (0.0),
    mMalfScaleFlag      (false),
    mMalfScaleValue     (0.0),
    mMalfBiasFlag       (false),
    mMalfBiasValue      (0.0),
    mMalfDriftFlag      (false),
    mMalfDriftRate      (0.0),
    mMalfNoiseFlag      (false),
    mMalfNoiseScale     (0.0),
    mMalfNoiseMult      (0.0),
    mMalfResolutionFlag (false),
    mMalfResolutionValue(0.0),
    mDrift              (0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Analog Sensor input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorAnalogInputData::~SensorAnalogInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  Copy constructs this Analog Sensor input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorAnalogInputData::SensorAnalogInputData(const SensorAnalogInputData& that)
    :
    SensorBaseInputData (that),
    mTruthInput         (that.mTruthInput),
    mMalfFailToValue    (that.mMalfFailToValue),
    mMalfScaleFlag      (that.mMalfScaleFlag),
    mMalfScaleValue     (that.mMalfScaleValue),
    mMalfBiasFlag       (that.mMalfBiasFlag),
    mMalfBiasValue      (that.mMalfBiasValue),
    mMalfDriftFlag      (that.mMalfDriftFlag),
    mMalfDriftRate      (that.mMalfDriftRate),
    mMalfNoiseFlag      (that.mMalfNoiseFlag),
    mMalfNoiseScale     (that.mMalfNoiseScale),
    mMalfNoiseMult      (that.mMalfNoiseMult),
    mMalfResolutionFlag (that.mMalfResolutionFlag),
    mMalfResolutionValue(that.mMalfResolutionValue),
    mDrift              (that.mDrift)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to assign to
///
/// @details  Assignment operator for the Analog Sensor input data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorAnalogInputData& SensorAnalogInputData::operator =(const SensorAnalogInputData& that)
{
    if (this != &that)
    {
        SensorBaseInputData::operator = (that);
        mTruthInput          = that.mTruthInput;
        mMalfFailToValue     = that.mMalfFailToValue;
        mMalfScaleFlag       = that.mMalfScaleFlag;
        mMalfScaleValue      = that.mMalfScaleValue;
        mMalfBiasFlag        = that.mMalfBiasFlag;
        mMalfBiasValue       = that.mMalfBiasValue;
        mMalfDriftFlag       = that.mMalfDriftFlag;
        mMalfDriftRate       = that.mMalfDriftRate;
        mMalfNoiseFlag       = that.mMalfNoiseFlag;
        mMalfNoiseScale      = that.mMalfNoiseScale;
        mMalfNoiseMult       = that.mMalfNoiseMult;
        mMalfResolutionFlag  = that.mMalfResolutionFlag;
        mMalfResolutionValue = that.mMalfResolutionValue;
        mDrift               = that.mDrift;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the Analog Sensor object.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorAnalog::SensorAnalog()
    :
    mMalfFailToValue    (0.0),
    mMalfScaleFlag      (false),
    mMalfScaleValue     (0.0),
    mMalfBiasFlag       (false),
    mMalfBiasValue      (0.0),
    mMalfDriftFlag      (false),
    mMalfDriftRate      (0.0),
    mMalfNoiseFlag      (false),
    mMalfNoiseScale     (0.0),
    mMalfNoiseMultiplier(0.0),
    mMalfResolutionFlag (false),
    mMalfResolutionValue(0.0),
    mDrift              (0.0),
    mMinRange           (0.0),
    mMaxRange           (0.0),
    mOffValue           (0.0),
    mNominalBias        (0.0),
    mNominalScale       (0.0),
    mNominalNoiseScale  (0.0),
    mNominalNoiseMult   (0.0),
    mNominalResolution  (0.0),
    mNoiseFunction      (0),
    mUnitConversion     (UnitConversion::NO_CONVERSION),
    mTruthInput         (0.0),
    mTruthOutput        (0.0),
    mSensedOutput       (0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Analog Sensor object.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorAnalog::~SensorAnalog()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] configData (--) Sensor Base config data.
/// @param[in] inputData  (--) Sensor Base input data.
/// @param[in] name       (--) Name of the sensor for output messages.
///
/// @throws TsInitializationException
///
/// @details  Initializes the Analog Sensor object with config and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void SensorAnalog::initialize(const SensorAnalogConfigData& configData,
                              const SensorAnalogInputData&  inputData,
                              const std::string&            name)
{
    /// - Initialize the base class and reset the init flag.
    SensorBase::initialize(configData, inputData, name);
    mInitFlag            = false;

    /// - Initialize with config data.
    mMinRange            = configData.mMinRange;
    mMaxRange            = configData.mMaxRange;
    mOffValue            = configData.mOffValue;
    mNominalBias         = configData.mNominalBias;
    mNominalScale        = configData.mNominalScale;
    mNominalNoiseScale   = configData.mNominalNoiseScale;
    mNominalNoiseMult    = configData.mNominalNoiseMult;
    mNominalResolution   = configData.mNominalResolution;
    mNoiseFunction       = configData.mNoiseFunction;
    mUnitConversion      = configData.mUnitConversion;

    /// - Initialize with input data.
    mTruthInput          = inputData.mTruthInput;
    mMalfFailToValue     = inputData.mMalfFailToValue;
    mMalfScaleFlag       = inputData.mMalfScaleFlag;
    mMalfScaleValue      = inputData.mMalfScaleValue;
    mMalfBiasFlag        = inputData.mMalfBiasFlag;
    mMalfBiasValue       = inputData.mMalfBiasValue;
    mMalfDriftFlag       = inputData.mMalfDriftFlag;
    mMalfDriftRate       = inputData.mMalfDriftRate;
    mMalfNoiseFlag       = inputData.mMalfNoiseFlag;
    mMalfNoiseScale      = inputData.mMalfNoiseScale;
    mMalfNoiseMultiplier = inputData.mMalfNoiseMult;
    mMalfResolutionFlag  = inputData.mMalfResolutionFlag;
    mMalfResolutionValue = inputData.mMalfResolutionValue;
    mDrift               = inputData.mDrift;
    mDegradedFlag        = determineDegradation();

    /// - Validate initial conditions.
    validate();

    /// - Use the update method to calculate the initial sensed output, set the init completed flag.
    update(0.0);
    mInitFlag            = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  Checks for valid configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void SensorAnalog::validate() const
{
    /// - Throw an exception when minimum range >= maximum range.
    if (mMinRange >= mMaxRange) {
        TS_HS_EXCEPTION(TS_HS_ERROR, TS_HS_SENSORS, "Invalid Configuration Data", TsInitializationException,
                "maximum output value must be larger than minimum output value.", mName);
    }

    /// - Throw an exception when off value is not between minimum range & maximum range.
    if (not MsMath::isInRange(mMinRange, mOffValue, mMaxRange)) {
        TS_HS_EXCEPTION(TS_HS_ERROR, TS_HS_SENSORS, "Invalid Configuration Data", TsInitializationException,
                "off value must be between the minimum & maximum output values.", mName);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] timeStep (s) Execution time step.
///
/// @details  Determines sensor output based on configuration, input, power, and malfunctions.  The
///           order of malf/effect precedence is as follows:
///
///       perfect > fail-off > fail-on > fail-to > stuck > resolution > noise > drift > bias > scale
////////////////////////////////////////////////////////////////////////////////////////////////////
void SensorAnalog::update(const double timeStep)
{
    if (mMalfPerfectSensor) {
        processInput();
    } else {
        if (determinePower()) {
            if (mMalfFailToFlag) {
                mSensedOutput = mMalfFailToValue;
            } else if (not mMalfFailStuckFlag) {
                processInput();
                applyScale();
                applyBias();
                applyDrift(timeStep);
                applyNoise();
                applyResolution();
            }
        } else {
            mSensedOutput = mOffValue;
        }
    }

    processOutput();
    mDegradedFlag = determineDegradation();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] timeStep   (s)  Execution time step.
/// @param[in] powerFlag  (--) Desired power state, true = powered, false = unpowered.
/// @param[in] truthInput (--) The truth input analog value to the sensor.
///
/// @returns  float (--) The sensed output value.
///
/// @details  This is a convenience method intended to be used for sensors embedded in other
///           objects.  It combines the setPowerFlag, setTruthInput, update, and getSensedOutputs
///           methods together into one easy call.  This allows the parent object to provide the
///           input power flag and truth value to the sensor and receive the resulting sensed
///           output in one step.  Sensors that use the simbus for some or all of the inputs/outputs
///           should not use this method - they should use the update method instead.
////////////////////////////////////////////////////////////////////////////////////////////////////
float SensorAnalog::sense(const double timeStep,
                          const bool   powerFlag,
                          const double truthInput)
{
    setPowerFlag(powerFlag);
    setTruthInput(truthInput);
    update(timeStep);
    return getSensedOutput();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method calls an optional conversion method to convert the truth input to the
///           desired internal units.
////////////////////////////////////////////////////////////////////////////////////////////////////
void SensorAnalog::processInput()
{
    /// - Protect against underflow in the type casting.
    if (std::fabs(mTruthInput) < static_cast<double>(FLT_MIN)) {
        mSensedOutput = 0.0;
    } else {
        mSensedOutput = static_cast<float>(mTruthInput);
    }

    /// - Apply units conversion method.
    mSensedOutput = static_cast<float>(UnitConversion::convert(mUnitConversion, static_cast<double>(mSensedOutput)));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method limits the sensed output to be within the absolute sensor range, and
///           updates the truth output by converting the truth input to output units.
////////////////////////////////////////////////////////////////////////////////////////////////////
void SensorAnalog::processOutput()
{
    mSensedOutput = static_cast<float>(MsMath::limitRange(mMinRange, mSensedOutput, mMaxRange));
    mTruthOutput  = UnitConversion::convert(mUnitConversion, mTruthInput);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Multiplies the sensor output by the nominal scale.  Also multiplies by the scale
///           malfunction value if the malf is active.  Note that when the scale malf is active, the
///           sensor output is scaled by the product of both the scale malf and nominal scale.
////////////////////////////////////////////////////////////////////////////////////////////////////
void SensorAnalog::applyScale()
{
    mSensedOutput *= mNominalScale;
    if (mMalfScaleFlag) {
        mSensedOutput *= mMalfScaleValue;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Adds the nominal sensor bias to the sensor output.  Also adds the bias malfunction
///           value if the malf is active.
////////////////////////////////////////////////////////////////////////////////////////////////////
void SensorAnalog::applyBias()
{
    mSensedOutput += mNominalBias;
    if (mMalfBiasFlag) {
        mSensedOutput += mMalfBiasValue;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Increments the total drift amount by the drift malfunction rate multiplied by the
///           simulation time step.  Only adds this drift to the sensor output when the malf is
///           active.  The total drift amount is only zeroed when the malfunction is removed.
////////////////////////////////////////////////////////////////////////////////////////////////////
void SensorAnalog::applyDrift(const double timeStep)
{
    if (mMalfDriftFlag) {
        mDrift += static_cast<float>(static_cast<double>(mMalfDriftRate) * timeStep);
        mSensedOutput += mDrift;
    } else {
        mDrift = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method calls the external noise generator function given by function pointer from
///           the config data.  This allows individual sensors to be configured to use noise
///           functions that have different distributions, such as Gaussian vs. uniform, etc.  The
///           noise has two components applied in this order: 1) a noise that scales with the input,
///           as input varies from the 'off' (zero signal) value of the sensor, and 2) a constant-
///           amplitude noise.  Both components have nominal scale values defined by config data,
///           and both scale values can be overridden by malfunction.  Each component is applied
///           with a different output of the noise generator function.
///
/// @note     Unlike the sensor bias & scale malfunctions, which are activated with separate flags,
///           a single activation flag activates the malf overrides of both noise components
///           simultaneously.
///
/// @note     The units definition of the 2 component scales are different.  The constant-scale
///           component is given in sensed output units, whereas the input-scaling component is in
///           fraction of total sensor output range.
///
/// @note     The input value used in the input-scaling component is after sensor bias, scale &
///           drift are applied.
////////////////////////////////////////////////////////////////////////////////////////////////////
void SensorAnalog::applyNoise()
{
    /// - Skip all processing if no noise function was provided in config data.  This makes it easy
    ///   to configure noise-less sensors - just leave the noise function null.
    if (mNoiseFunction) {

        /// - Noise malfunction overrides both components simultaneously.
        float activeNoiseScale = mNominalNoiseScale;
        float activeNoiseMult  = mNominalNoiseMult;
        if (mMalfNoiseFlag) {
            activeNoiseScale   = mMalfNoiseScale;
            activeNoiseMult    = mMalfNoiseMultiplier;
        }

        /// - An example resulting output of both noise components is as follows.  Given a sensor
        ///   with min/off/max range values of 0/5/10, mSensedOutput input to this function = 7,
        ///   noise scale = 1, noise mult = 0.1:
        ///     the constant-scale component amplitude will be 1,
        ///     the input-scaling component amplitude will be (7 - 5) * 0.1 = 0.2
        ///
        /// - To avoid wasting CPU time, skip calling the noise generator function if we would scale
        ///   it to zero anyway.
        ///
        /// - Apply the input-scaling noise component.
        activeNoiseMult *= std::fabs(mSensedOutput - mOffValue);
        if (std::fabs(activeNoiseMult) > FLT_EPSILON) {
            mSensedOutput += static_cast<float>((*mNoiseFunction)()) * activeNoiseMult;
        }

        /// - Apply the constant scale noise component.
        if (std::fabs(activeNoiseScale) > FLT_EPSILON) {
            mSensedOutput += static_cast<float>((*mNoiseFunction)()) * activeNoiseScale;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method rounds the sensor output to the nearest multiple of the resolution value.
///           This models the accuracy of the sensor and is useful for damping model noise or
///           discretizing the sensor output as desired.  Unlike the other malfunctions, if the
///           resolution malfunction is active, it is used by itself as the resolution amount,
///           rather than being in some combination with the nominal amount.
///
/// @note     The absolute value of the nominal & malfunction resolution is used.  It doesn't make
///           sense to use negative values, but the method allows it since we use the absolute value
///           anyway.
////////////////////////////////////////////////////////////////////////////////////////////////////
void SensorAnalog::applyResolution()
{
    /// - Use the malfunction value if the malf is active, otherwise use the nominal value.
    float value = mNominalResolution;
    if (mMalfResolutionFlag) {
        value = mMalfResolutionValue;
    }

    /// - Flip the sign if necessary so the used value is always positive.
    value = std::fabs(value);

    if (value > FLT_EPSILON) {
        mSensedOutput = value * static_cast<float>(round(mSensedOutput/value));
    }
}
