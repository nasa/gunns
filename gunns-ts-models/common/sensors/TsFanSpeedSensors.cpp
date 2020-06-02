/****************************** TRICK HEADER *******************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
 ((SensorAnalog.o))
***************************************************************************************************/

#include <cfloat>

#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "GenericMacros.hh"

#include "TsFanSpeedSensors.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   sensor  (--)  Common speed sensor configuration data.
///
/// @details     Default constructs this Fan Sensors Model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsFanSpeedSensorsConfigData::TsFanSpeedSensorsConfigData(const SensorAnalogConfigData& sensor)
    :
    mSensor(sensor)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   that    (--)  Object to copy.
///
/// @details     Copy constructs this Fan Speed Sensors Model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsFanSpeedSensorsConfigData::TsFanSpeedSensorsConfigData(const TsFanSpeedSensorsConfigData& that)
    :
    mSensor(that.mSensor)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details     Default destructs this Fan Speed Sensors Model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsFanSpeedSensorsConfigData::~TsFanSpeedSensorsConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   that   (--)  Object assigned from.
///
/// @return      TsFanSpeedSensorsConfigData&  (--)  Object assigned to.
///
/// @details     Assignment operator for this Fan Speed Sensors Model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsFanSpeedSensorsConfigData& TsFanSpeedSensorsConfigData::operator=(const TsFanSpeedSensorsConfigData &that)
{
    /// - Check for assignment to self.
    if(this != &that) {
        mSensor = that.mSensor;
    }

    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   sensorA  (--)  Speed sensor A input data.
/// @param[in]   sensorB  (--)  Speed sensor B input data.
/// @param[in]   sensorC  (--)  Speed sensor C input data.
///
/// @details     Default constructs this Fan Sensors Model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsFanSpeedSensorsInputData::TsFanSpeedSensorsInputData(const SensorAnalogInputData& sensorA,
                                                         const SensorAnalogInputData& sensorB,
                                                         const SensorAnalogInputData& sensorC)
    :
    mSensorA(sensorA),
    mSensorB(sensorB),
    mSensorC(sensorC)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   that    (--)  Object to copy.
///
/// @details     Copy constructs this Fan Speed Sensors Model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsFanSpeedSensorsInputData::TsFanSpeedSensorsInputData(const TsFanSpeedSensorsInputData& that)
    :
    mSensorA(that.mSensorA),
    mSensorB(that.mSensorB),
    mSensorC(that.mSensorC)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details     Default destructs this Fan Speed Sensors Model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsFanSpeedSensorsInputData::~TsFanSpeedSensorsInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   that   (--)  Object assigned from.
///
/// @return      TsFanSpeedSensorsInputData&  (--)  Object assigned to.
///
/// @details     Assignment operator for this Fan Speed Sensors Model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsFanSpeedSensorsInputData& TsFanSpeedSensorsInputData::operator =(const TsFanSpeedSensorsInputData &that)
{
    /// - Check for assignment to self.
    if(this != &that) {
        mSensorA = that.mSensorA;
        mSensorB = that.mSensorB;
        mSensorC = that.mSensorC;
    }

    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details     Default constructs this Fan Speed Sensors Model.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsFanSpeedSensors::TsFanSpeedSensors()
    :
    mSensorA(),
    mSensorB(),
    mSensorC(),
    mName(),
    mAverageSpeed(0.0),
    mInitialized(false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details     Default destructs this Fan Speed Sensors Model.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsFanSpeedSensors::~TsFanSpeedSensors()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   config  (--)  Configuration data.
/// @param[in]   input   (--)  Input data.
/// @param[in]   name    (--)  Object name.
///
/// @return      void
///
/// @throws      TsInitializationException
///
/// @details     Initializes this Fan Speed Sensors Model with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsFanSpeedSensors::initialize(const TsFanSpeedSensorsConfigData &config,
                                   const TsFanSpeedSensorsInputData  &input,
                                   const std::string                 &name)
{
    /// - Reset the initialization complete flag.
    mInitialized       = false;

//    /// - Initialize object name or throw an exception if empty.
//    if (name.empty()) {
//        TS_HS_EXCEPTION(TS_HS_ERROR, TS_HS_SENSORS, "Invalid Configuration Data", TsInitializationException,
//                "A sensor name is undefined", "");
//    }
//    TS_DELETE_ARRAY(mName);
//    TS_STRDUP(mName, name.c_str());

    /// - Allocate and initialize the object name or throw a TsInitializationException exception if empty.
    TS_GENERIC_NAME_ERREX("SensorBase")

    /// - Initialize the individual speed sensors.
    mSensorA.initialize(config.mSensor, input.mSensorA, name + ".A");
    mSensorB.initialize(config.mSensor, input.mSensorB, name + ".B");
    mSensorC.initialize(config.mSensor, input.mSensorC, name + ".C");

    /// - Update the output (average sensed speed).
    update(0.0);

    /// - Set the initialization complete flag.
    mInitialized       = mSensorA.isInitialized() && mSensorB.isInitialized() && mSensorC.isInitialized();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   dt  (s)  Time step.
///
/// @return      void
///
/// @details     Updates this Fan Speed Sensors Model using previously set power flag and
///              truth value.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsFanSpeedSensors::update(const double dt)
{
    /// - Update the sensors.
    mSensorA.update(dt);
    mSensorB.update(dt);
    mSensorC.update(dt);

    /// - Compute the average sensed speed.
    mAverageSpeed = (mSensorA.getSensedOutput() + mSensorB.getSensedOutput() + mSensorC.getSensedOutput()) / 3.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   dt         (s)    Time step.
/// @param[in]   powerFlag  (--)   Power flag.
/// @param[in]   speed      (s)    Speed truth value.
///
/// @return      float      (rpm)  Average sensed speed.
///
/// @details     Updates this Fan Speed Sensors Model.
////////////////////////////////////////////////////////////////////////////////////////////////////
float TsFanSpeedSensors::sense(const double dt,
                               const bool    powerFlag,
                               const double  speed)
{
    /// - Set the sensor power status flags.
    mSensorA.setPowerFlag(powerFlag);
    mSensorB.setPowerFlag(powerFlag);
    mSensorC.setPowerFlag(powerFlag);

    /// - Set the sensor truth value.
    setTruthInput(speed);

    /// - Update the sensors.
    update(dt);

    /// - Return the average sensed speed.
    return mAverageSpeed;
}
