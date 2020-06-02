/***************************************** TRICK HEADER ********************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    ((SensorBooleanBi.o))

PROGRAMMERS:
    ((Jason Harvey) (L-3 Communications) (2012-04) (Initial))

 **************************************************************************************************/

#include <cmath>
#include "SensorBooleanAi.hh"
#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] offValue  (--) Sensed output value of the sensor when off
/// @param[in] target    (--) Target value of truth input to trigger sensor
/// @param[in] tolerance (--) Tolerance range of truth input to target to trigger sensor
///
/// @details  Constructs Analog Input Boolean Sensor config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorBooleanAiConfigData::SensorBooleanAiConfigData(const bool         offValue,
                                                     const float        target,
                                                     const float        tolerance)
    :
    SensorBooleanBiConfigData(offValue),
    mTarget   (target),
    mTolerance(tolerance)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Analog Input Boolean Sensor config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorBooleanAiConfigData::~SensorBooleanAiConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  Copy constructs Analog Input Boolean Sensor config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorBooleanAiConfigData::SensorBooleanAiConfigData(const SensorBooleanAiConfigData& that)
    :
    SensorBooleanBiConfigData(that),
    mTarget   (that.mTarget),
    mTolerance(that.mTolerance)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to assign to
///
/// @details  Assignment operator for the Analog Input Boolean Sensor config data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorBooleanAiConfigData& SensorBooleanAiConfigData::operator =(const SensorBooleanAiConfigData& that)
{
    if (this != &that)
    {
        SensorBooleanBiConfigData::operator = (that);
        mTarget    = that.mTarget;
        mTolerance = that.mTolerance;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] powerFlag        (--) Initial powered state of the sensor
/// @param[in] truthInput       (--) Initial value of the input truth value to sense (boolean)
/// @param[in] truthInputAnalog (--) Initial value of the input truth value to sense (analog)
///
/// @details  Default constructs this Analog Input Boolean Sensor input data.  The malfunction terms
///           are only present in the input data class to support unit testing of parent objects
///           that contain this sensor in an aggregate.  We deliberately omit them as arguments to
///           this constructor to reduce cluttering the method signature.  Users can still set the
///           malfunction input data after the object has been constructed and before the sensor is
///           initialized.
///
/// @note     In this class, truthInput is not used, use truthInputAnalog instead.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorBooleanAiInputData::SensorBooleanAiInputData(const bool  powerFlag,
                                                   const bool  truthInput,
                                                   const float truthInputAnalog)
    :
    SensorBooleanBiInputData(powerFlag, truthInput),
    mTruthInputAnalog(truthInputAnalog)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Analog Input Boolean Sensor input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorBooleanAiInputData::~SensorBooleanAiInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  Copy constructs this Analog Input Boolean Sensor input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorBooleanAiInputData::SensorBooleanAiInputData(const SensorBooleanAiInputData& that)
    :
    SensorBooleanBiInputData(that),
    mTruthInputAnalog(that.mTruthInputAnalog)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to assign to
///
/// @details  Assignment operator for the Analog Input Boolean Sensor input data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorBooleanAiInputData& SensorBooleanAiInputData::operator =(const SensorBooleanAiInputData& that)
{
    if (this != &that)
    {
        SensorBooleanBiInputData::operator = (that);
        mTruthInputAnalog = that.mTruthInputAnalog;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the Analog Input Boolean Sensor object.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorBooleanAi::SensorBooleanAi()
    :
    mTarget          (0.0),
    mTolerance       (0.0),
    mTruthInputAnalog(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Analog Input Boolean Sensor object.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorBooleanAi::~SensorBooleanAi()
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
/// @details  Initializes the Analog Input Boolean Sensor object with config and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void SensorBooleanAi::initialize(const SensorBooleanAiConfigData& configData,
                                 const SensorBooleanAiInputData&  inputData,
                                 const std::string&                name)
{
    /// - Initialize the base class and reset the init flag.
    SensorBooleanBi::initialize(configData, inputData, name);
    mInitFlag         = false;

    /// - Initialize with config data.
    mTarget           = configData.mTarget;
    mTolerance        = configData.mTolerance;

    /// - Initialize with input data.
    mTruthInputAnalog = inputData.mTruthInputAnalog;

    /// - Validate initial conditions.
    validate();

    /// - Use the update method to calculate the initial sensed output, set the init completed flag.
    update(0.0);
    mInitFlag         = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This class currently has nothing to validate.
////////////////////////////////////////////////////////////////////////////////////////////////////
void SensorBooleanAi::validate() const
{
    /// - Throw an exception when tolerance < 0.
    if (mTolerance < 0.0) {
        TS_HS_EXCEPTION(TS_HS_ERROR, TS_HS_SENSORS, "Invalid Configuration Data", TsInitializationException,
                "tolerance cannot be negative.", mName);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] timeStep   (s)  Execution time step.
/// @param[in] powerFlag  (--) Desired power state, true = powered, false = unpowered.
/// @param[in] truthInput (--) The truth input analog value to the sensor.
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
bool SensorBooleanAi::sense(const double timeStep,
                            const bool   powerFlag,
                            const double truthInput)
{
    setPowerFlag(powerFlag);
    setTruthInput(truthInput);
    update(timeStep);
    return getSensedOutput();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method sets the truth input boolean term of the base boolean sensor class based
///           on whether the input analog value is within tolerance of a target real value.  Then
///           the base class update method performs the same processing on that boolean that the
///           normal boolean sensor does.  This method sets the boolean true when the input analog
///           is within tolerance, and false outside of it.
///
/// @note     If a logical inverse is desired, where the sensor outputs false when within the
///           tolerance and true otherwise, either extend this class and override this method, or
///           perform the inversion outside of the sensor.
////////////////////////////////////////////////////////////////////////////////////////////////////
void SensorBooleanAi::processInput()
{
    mTruthInput = (fabs(mTruthInputAnalog - mTarget) <= mTolerance);

    /// - Call the base class processInput method to do any other transformation that the boolean
    ///   sensor normally does.
    SensorBooleanBi::processInput();
}
