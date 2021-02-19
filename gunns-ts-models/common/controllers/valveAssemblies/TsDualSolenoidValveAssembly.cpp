/**
@file
@brief     Dual Solenoid Valve Assembly Model implementation.

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
 ((common/sensors/SensorBooleanAi.o)
  (common/controllers/fluid/TsDualSolenoidValveController.o))
*/

#include "TsDualSolenoidValveAssembly.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  controller    (--)  Valve controller config data.
/// @param[in]  sensorOpen    (--)  Open sensor config data.
/// @param[in]  sensorClosed  (--)  Closed sensor config data.
///
/// @details  Default constructs this Dual Solenoid Valve Assembly configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsDualSolenoidValveAssemblyConfigData::TsDualSolenoidValveAssemblyConfigData(
        const TsDualSolenoidValveControllerConfigData& controller,
        const SensorBooleanAiConfigData&               sensorOpen,
        const SensorBooleanAiConfigData&               sensorClosed)
    :
    mController  (controller),
    mSensorOpen  (sensorOpen),
    mSensorClosed(sensorClosed)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Dual Solenoid Valve Assembly configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsDualSolenoidValveAssemblyConfigData::~TsDualSolenoidValveAssemblyConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that  (--)  Object to copy.
///
/// @details  Copy constructs this Dual Solenoid Valve Assembly configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsDualSolenoidValveAssemblyConfigData::TsDualSolenoidValveAssemblyConfigData(
        const TsDualSolenoidValveAssemblyConfigData& that)
    :
    mController  (that.mController),
    mSensorOpen  (that.mSensorOpen),
    mSensorClosed(that.mSensorClosed)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Object assigned from.
///
/// @return   TsDualSolenoidValveAssemblyConfigData&  (--)  Object assigned to.
///
/// @details  Assignment operator for this Dual Solenoid Valve Assembly configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsDualSolenoidValveAssemblyConfigData& TsDualSolenoidValveAssemblyConfigData::operator=(
        const TsDualSolenoidValveAssemblyConfigData& that)
{
    if (this != &that) {
        mController   = that.mController;
        mSensorOpen   = that.mSensorOpen;
        mSensorClosed = that.mSensorClosed;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  controller    (--)  Valve controller input data.
/// @param[in]  sensorOpen    (--)  Open sensor input data.
/// @param[in]  sensorClosed  (--)  Closed sensor input data.
///
/// @details  Default constructs this Dual Solenoid Valve Assembly input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsDualSolenoidValveAssemblyInputData::TsDualSolenoidValveAssemblyInputData(
        const TsDualSolenoidValveControllerInputData& controller,
        const SensorBooleanAiInputData&               sensorOpen,
        const SensorBooleanAiInputData&               sensorClosed)
    :
    mController  (controller),
    mSensorOpen  (sensorOpen),
    mSensorClosed(sensorClosed)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Dual Solenoid Valve Assembly input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsDualSolenoidValveAssemblyInputData::~TsDualSolenoidValveAssemblyInputData()
{
    // nothing to do
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that  (--)  Object to copy.
///
/// @details  Copy constructs this Dual Solenoid Valve Assembly input data.
//////////////////////////////////////////////////////////////////////////////////////////////////////
TsDualSolenoidValveAssemblyInputData::TsDualSolenoidValveAssemblyInputData(
        const TsDualSolenoidValveAssemblyInputData& that)
    :
    mController  (that.mController),
    mSensorOpen  (that.mSensorOpen),
    mSensorClosed(that.mSensorClosed)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Object assigned from.
///
/// @return   TsDualSolenoidValveAssemblyInputData&  (--)  Object assigned to.
///
/// @details  Assignment operator for this Dual Solenoid Valve Assembly input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsDualSolenoidValveAssemblyInputData& TsDualSolenoidValveAssemblyInputData::operator=(
        const TsDualSolenoidValveAssemblyInputData& that)
{
    if (this != &that) {
        mController   = that.mController;
        mSensorOpen   = that.mSensorOpen;
        mSensorClosed = that.mSensorClosed;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Dual Solenoid Valve Assembly model.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsDualSolenoidValveAssembly::TsDualSolenoidValveAssembly()
    :
    mController(),
    mSensorOpen(),
    mSensorClosed(),
    mInitialized(false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Dual Solenoid Valve Assembly model.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsDualSolenoidValveAssembly::~TsDualSolenoidValveAssembly()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] configData  (--)  Configuration data.
/// @param[in] inputData   (--)  Input data.
/// @param[in] name        (--)  Object name.
///
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Initializes this Dual Solenoid Valve Assembly model with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsDualSolenoidValveAssembly::initialize(const TsDualSolenoidValveAssemblyConfigData& configData,
                                             const TsDualSolenoidValveAssemblyInputData&  inputData,
                                             const std::string&                           name)
{
    /// - Reset the initialization complete flag.
    mInitialized = false;

    /// - Initialize controller and sensors with config & input data.
    mController  .initialize(configData.mController,   inputData.mController,   name + ".mController");
    mSensorOpen  .initialize(configData.mSensorOpen,   inputData.mSensorOpen,   name + ".mSensorOpen");
    mSensorClosed.initialize(configData.mSensorClosed, inputData.mSensorClosed, name + ".mSensorClosed");

    /// - Initialize remaining state data.
    mInitialized = mController  .isInitialized() and
                   mSensorOpen  .isInitialized() and
                   mSensorClosed.isInitialized();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Execution time step
///
/// @return   void
///
/// @details  Updates the sensors and controller.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsDualSolenoidValveAssembly::update(const double dt)
{
    /// - Update controller.
    mController.update(dt);

    /// - Update sensors.  Sensors are assumed powered by their telemetry card so are effectively
    ///   always on.
    mSensorOpen  .sense(dt, true, mController.getPosition());
    mSensorClosed.sense(dt, true, mController.getPosition());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  open   (--)  Valve open command/power input.
/// @param[in]  close  (--)  Valve close command/power input.
/// @param[in]  dt     (s)   Time step.
///
/// @return   void
///
/// @details  Updates the sensors and controller with arguments, so it can be run from a container.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsDualSolenoidValveAssembly::update(const bool   open,
                                         const bool   close,
                                         const double dt)
{
    /// - Set supply voltage flag and command.
    mController.setOpenSolenoidCmd(open);
    mController.setCloseSolenoidCmd(close);

    /// - Update the sensors and controller.
    update(dt);
}
