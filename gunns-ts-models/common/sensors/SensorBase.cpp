/***************************************** TRICK HEADER ********************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    ((simulation/hs/TsHsMsg.o)
     (software/exceptions/TsInitializationException.o))

PROGRAMMERS:
    ((Jason Harvey) (L-3 Communications) (2012-04) (Initial))

 **************************************************************************************************/

#include "SensorBase.hh"
#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "GenericMacros.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs Sensor Base config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorBaseConfigData::SensorBaseConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Sensor Base config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorBaseConfigData::~SensorBaseConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  Copy constructs Sensor Base config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorBaseConfigData::SensorBaseConfigData(const SensorBaseConfigData& that __attribute__((unused)))
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to assign to
///
/// @details  Assignment operator for the Sensor Base config data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorBaseConfigData& SensorBaseConfigData::operator =(const SensorBaseConfigData& that)
{
    if (this != &that)
    {
        // nothing to do
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] powerFlag (--) Initial powered state of the sensor
///
/// @details  Default constructs this Sensor Base input data.  The malfunction terms are only
///           present in the input data class to support unit testing of parent objects that contain
///           this sensor in an aggregate.  We deliberately omit them as arguments to this
///           constructor to reduce cluttering the method signature.  Users can still set the malf
///           input data after the object has been constructed and before the sensor is initialized.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorBaseInputData::SensorBaseInputData(const bool powerFlag)
    :
    mPowerFlag        (powerFlag),
    mMalfFailToFlag   (false),
    mMalfFailOffFlag  (false),
    mMalfFailStuckFlag(false),
    mMalfIgnorePower  (false),
    mMalfPerfectSensor(false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Sensor Base input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorBaseInputData::~SensorBaseInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  Copy constructs this Sensor Base input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorBaseInputData::SensorBaseInputData(const SensorBaseInputData& that)
    :
    mPowerFlag        (that.mPowerFlag),
    mMalfFailToFlag   (that.mMalfFailToFlag),
    mMalfFailOffFlag  (that.mMalfFailOffFlag),
    mMalfFailStuckFlag(that.mMalfFailStuckFlag),
    mMalfIgnorePower  (that.mMalfIgnorePower),
    mMalfPerfectSensor(that.mMalfPerfectSensor)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to assign to
///
/// @details  Assignment operator for the Sensor Base input data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorBaseInputData& SensorBaseInputData::operator =(const SensorBaseInputData& that)
{
    if (this != &that)
    {
        mPowerFlag         = that.mPowerFlag;
        mMalfFailToFlag    = that.mMalfFailToFlag;
        mMalfFailOffFlag   = that.mMalfFailOffFlag;
        mMalfFailStuckFlag = that.mMalfFailStuckFlag;
        mMalfIgnorePower   = that.mMalfIgnorePower;
        mMalfPerfectSensor = that.mMalfPerfectSensor;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the Sensor Base object.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorBase::SensorBase()
    :
    mMalfFailToFlag   (false),
    mMalfFailOffFlag  (false),
    mMalfFailStuckFlag(false),
    mMalfIgnorePower  (false),
    mMalfPerfectSensor(false),
    mName             (),
    mPowerFlag        (false),
    mDegradedFlag     (false),
    mInitFlag         (false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Sensor Base object.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorBase::~SensorBase()
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
/// @details  Initializes the Sensor Base object with config and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void SensorBase::initialize(const SensorBaseConfigData& configData  __attribute__((unused)),
                            const SensorBaseInputData&  inputData,
                            const std::string&          name)
{
    /// - Reset the init flag.
    mInitFlag          = false;

    /// - Allocate and initialize the object name or throw a TsInitializationException exception if empty.
    TS_GENERIC_NAME_ERREX("SensorBase")

    /// - Initialize with input data.
    mPowerFlag         = inputData.mPowerFlag;
    mMalfFailToFlag    = inputData.mMalfFailToFlag;
    mMalfFailOffFlag   = inputData.mMalfFailOffFlag;
    mMalfFailStuckFlag = inputData.mMalfFailStuckFlag;
    mMalfIgnorePower   = inputData.mMalfIgnorePower;
    mMalfPerfectSensor = inputData.mMalfPerfectSensor;
    mDegradedFlag      = determineDegradation();

    /// - Validate initial conditions
    validate();

    /// - Set the init completed flag.
    mInitFlag          = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws TsInitializationException
///
/// @details  Checks that all initial condition requirements have been met.
////////////////////////////////////////////////////////////////////////////////////////////////////
void SensorBase::validate() const
{
    // nothing to do
}
