/***************************************** TRICK HEADER ********************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    ((SensorBase.o))

PROGRAMMERS:
    ((Jason Harvey) (L-3 Communications) (2012-04) (Initial))

 **************************************************************************************************/

#include "SensorBooleanBi.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] offValue (--) Sensed output value of the sensor when off
///
/// @details  Constructs Boolean Input Boolean Sensor config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorBooleanBiConfigData::SensorBooleanBiConfigData(const bool offValue)
    :
    SensorBaseConfigData(),
    mOffValue(offValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Boolean Input Boolean Sensor config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorBooleanBiConfigData::~SensorBooleanBiConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  Copy constructs Boolean Input Boolean Sensor config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorBooleanBiConfigData::SensorBooleanBiConfigData(const SensorBooleanBiConfigData& that)
    :
    SensorBaseConfigData(that),
    mOffValue(that.mOffValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to assign to
///
/// @details  Assignment operator for the Boolean Input Boolean Sensor config data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorBooleanBiConfigData& SensorBooleanBiConfigData::operator =(const SensorBooleanBiConfigData& that)
{
    if (this != &that)
    {
        SensorBaseConfigData::operator = (that);
        mOffValue = that.mOffValue;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] powerFlag  (--) Initial powered state of the sensor
/// @param[in] truthInput (--) Initial value of the input truth value to sense
///
/// @details  Default constructs this Boolean Input Boolean Sensor input data.  The malfunction
///           terms are only present in the input data class to support unit testing of parent
///           objects that contain this sensor in an aggregate.  We deliberately omit them as
///           arguments to this constructor to reduce cluttering the method signature.  Users can
///           still set the malfunction input data after the object has been constructed and before
///           the sensor is initialized.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorBooleanBiInputData::SensorBooleanBiInputData(const bool powerFlag,
                                                   const bool truthInput)
    :
    SensorBaseInputData(powerFlag),
    mTruthInput        (truthInput),
    mMalfFailToValue   (false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Boolean Input Boolean Sensor input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorBooleanBiInputData::~SensorBooleanBiInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  Copy constructs this Boolean Input Boolean Sensor input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorBooleanBiInputData::SensorBooleanBiInputData(const SensorBooleanBiInputData& that)
    :
    SensorBaseInputData(that),
    mTruthInput        (that.mTruthInput),
    mMalfFailToValue   (that.mMalfFailToValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to assign to
///
/// @details  Assignment operator for the Boolean Input Boolean Sensor input data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorBooleanBiInputData& SensorBooleanBiInputData::operator =(const SensorBooleanBiInputData& that)
{
    if (this != &that)
    {
        SensorBaseInputData::operator = (that);
        mTruthInput      = that.mTruthInput;
        mMalfFailToValue = that.mMalfFailToValue;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the Boolean Input Boolean Sensor object.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorBooleanBi::SensorBooleanBi()
    :
    mMalfFailToValue(false),
    mOffValue       (false),
    mTruthInput     (false),
    mSensedOutput   (false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Boolean Input Boolean Sensor object.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorBooleanBi::~SensorBooleanBi()
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
/// @details  Initializes the Boolean Input Boolean Sensor object with config and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void SensorBooleanBi::initialize(const SensorBooleanBiConfigData& configData,
                                 const SensorBooleanBiInputData&  inputData,
                                 const std::string&               name)
{
    /// - Initialize the base class and reset the init flag.
    SensorBase::initialize(configData, inputData, name);
    mInitFlag        = false;

    /// - Initialize with config data.
    mOffValue        = configData.mOffValue;

    /// - Initialize with input data.
    mTruthInput      = inputData.mTruthInput;
    mMalfFailToValue = inputData.mMalfFailToValue;

    /// - Validate initial conditions.
    validate();

    /// - Use the update method to calculate the initial sensed output, set the init completed flag.
    update(0.0);
    mInitFlag        = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This class currently has nothing to validate.
////////////////////////////////////////////////////////////////////////////////////////////////////
void SensorBooleanBi::validate() const
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] timeStep (s) Execution time step (not used).
///
/// @details  Determines sensor output based on configuration, input, power, and malfunctions.  The
///           order of malf/effect precedence is as follows:
///
///                              perfect > fail-off > fail-on > fail-to > stuck
////////////////////////////////////////////////////////////////////////////////////////////////////
void SensorBooleanBi::update(const double timeStep __attribute__((unused)))
{
    if (mMalfPerfectSensor) {
        processInput();
    } else {
        if (determinePower()) {
            if (mMalfFailToFlag) {
                mSensedOutput = mMalfFailToValue;
            } else if (not mMalfFailStuckFlag) {
                processInput();
            }
        } else {
            mSensedOutput = mOffValue;
        }
    }
    mDegradedFlag = determineDegradation();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] timeStep   (s)  Execution time step.
/// @param[in] powerFlag  (--) Desired power state, true = powered, false = unpowered.
/// @param[in] truthInput (--) The truth input boolean value to the sensor.
///
/// @returns  bool (--) The sensed output value.
///
/// @details  This is a convenience method intended to be used for sensors embedded in other
///           objects.  It combines the setPowerFlag, setTruthInput, update, and getSensedOutputs
///           methods together into one easy call.  This allows the parent object to provide the
///           input power flag and truth value to the sensor and receive the resulting sensed
///           output in one step.  Sensors that use the simbus for some or all of the inputs/outputs
///           should not use this method - they should use the update method instead.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool SensorBooleanBi::sense(const double timeStep,
                                  const bool   powerFlag,
                                  const bool   truthInput)
{
    setPowerFlag(powerFlag);
    setTruthInput(truthInput);
    update(timeStep);
    return getSensedOutput();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Currently there are no transformations needed, so this method simply passes on the
///           truth input without modification.
////////////////////////////////////////////////////////////////////////////////////////////////////
void SensorBooleanBi::processInput()
{
    mSensedOutput = mTruthInput;
}
