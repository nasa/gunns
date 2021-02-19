/**
@file
@brief     Open/Close Valve Assembly Model implementation.

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
 ((common/sensors/SensorBooleanAi.o)
  (common/controllers/fluid/TsOpenCloseValveController.o))
*/

#include "TsOpenCloseValveAssembly.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  controller    (--)  Valve controller config data.
/// @param[in]  sensorOpen    (--)  Open sensor config data.
/// @param[in]  sensorClosed  (--)  Closed sensor config data.
///
/// @details  Default constructs this Open/Close Valve Assembly configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsOpenCloseValveAssemblyConfigData::TsOpenCloseValveAssemblyConfigData(
        const TsPoweredValveControllerConfigData& controller,
        const SensorBooleanAiConfigData&          sensorOpen,
        const SensorBooleanAiConfigData&          sensorClosed)
    :
    mController  (controller),
    mSensorOpen  (sensorOpen),
    mSensorClosed(sensorClosed)
{
    //    nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Open/Close Valve Assembly configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsOpenCloseValveAssemblyConfigData::~TsOpenCloseValveAssemblyConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Source to copy.
///
/// @details  Copy constructs this Open/Close Valve Assembly configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsOpenCloseValveAssemblyConfigData::TsOpenCloseValveAssemblyConfigData(
        const TsOpenCloseValveAssemblyConfigData& that)
    :
    mController  (that.mController),
    mSensorOpen  (that.mSensorOpen),
    mSensorClosed(that.mSensorClosed)
{
    //    nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Object assigned from.
///
/// @return   TsOpenCloseValveAssemblyConfigData&  (--)  Object assigned to.
///
/// @details  Assignment operator for this Open/Close Valve Assembly configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsOpenCloseValveAssemblyConfigData& TsOpenCloseValveAssemblyConfigData::operator=(
        const TsOpenCloseValveAssemblyConfigData& that)
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
/// @details  Default constructs this Open/Close Valve Assembly input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsOpenCloseValveAssemblyInputData::TsOpenCloseValveAssemblyInputData(
        const TsPoweredValveControllerInputData& controller,
        const SensorBooleanAiInputData&          sensorOpen,
        const SensorBooleanAiInputData&          sensorClosed)
    :
    mController  (controller),
    mSensorOpen  (sensorOpen),
    mSensorClosed(sensorClosed)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Open/Close Valve Assembly input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsOpenCloseValveAssemblyInputData::~TsOpenCloseValveAssemblyInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Source to copy.
///
/// @details  Copy constructs this Open/Close Valve Assembly input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsOpenCloseValveAssemblyInputData::TsOpenCloseValveAssemblyInputData(
        const TsOpenCloseValveAssemblyInputData& that)
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
/// @return   TsPoweredValveControllerInputData&  (--)  Object assigned to.
///
/// @details  Assignment operator for this Open/Close Valve Assembly input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsOpenCloseValveAssemblyInputData& TsOpenCloseValveAssemblyInputData::operator=(
        const TsOpenCloseValveAssemblyInputData& that)
{
    if (this != &that) {
        mController   = that.mController;
        mSensorOpen   = that.mSensorOpen;
        mSensorClosed = that.mSensorClosed;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default constructs this Open/Close Valve Assembly model.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsOpenCloseValveAssembly::TsOpenCloseValveAssembly()
    :
    mController(),
    mSensorOpen(),
    mSensorClosed(),
    mInitialized(false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default destructs this Open/Close Valve Assembly model.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsOpenCloseValveAssembly::~TsOpenCloseValveAssembly()
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
/// @details  Initializes this Open/Close Valve Assembly model with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsOpenCloseValveAssembly::initialize(const TsOpenCloseValveAssemblyConfigData& configData,
                                          const TsOpenCloseValveAssemblyInputData&  inputData,
                                          const std::string&                        name)
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
void TsOpenCloseValveAssembly::update(const double dt)
{
    /// - Update controller.
    mController.update(dt);

    /// - Update sensors.  Sensors are assumed powered by their telemetry card so are effectively
    ///   always on.
    mController.setOpenSensed (mSensorOpen  .sense(dt, true, mController.getPosition()));
    mController.setCloseSensed(mSensorClosed.sense(dt, true, mController.getPosition()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  supplyVoltageFlag  (--)  Power flag.
/// @param[in]  command            (--)  Valve open/close command.
/// @param[in]  dt                 (s)   Time step.
///
/// @return   void
///
/// @details  Updates the sensors and controller with arguments, so it can be run from a container.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsOpenCloseValveAssembly::update(const bool                 supplyVoltageFlag,
                                      const TsOpenCloseValveCmd& command,
                                      const double               dt)
{
    /// - Set supply voltage flag and command.
    mController.setSupplyVoltageFlag(supplyVoltageFlag);
    mController.setCommand(command);

    /// - Update the sensors and controller.
    update(dt);
}
