/**
@file
@brief     Toggle Valve Assembly Model implementation.

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
 ((common/sensors/SensorBooleanAi.o)
  (common/controllers/fluid/TsToggleValveController.o))
*/

#include "TsToggleValveAssembly.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  controller    (--)  Valve controller config data.
/// @param[in]  sensorOpen    (--)  Open sensor config data.
/// @param[in]  sensorClosed  (--)  Closed sensor config data.
///
/// @details  Default constructs this Toggle Valve Assembly configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsToggleValveAssemblyConfigData::TsToggleValveAssemblyConfigData(
        const TsPoweredValveControllerConfigData& controller,
        const SensorBooleanAiConfigData&          sensorOpen,
        const SensorBooleanAiConfigData&          sensorClosed)
    :
    mController  (controller),
    mSensorOpen  (sensorOpen),
    mSensorClosed(sensorClosed)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Toggle Valve Assembly configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsToggleValveAssemblyConfigData::~TsToggleValveAssemblyConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that  (--)  Object to copy.
///
/// @details  Copy constructs this Toggle Valve Assembly configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsToggleValveAssemblyConfigData::TsToggleValveAssemblyConfigData(
        const TsToggleValveAssemblyConfigData& that)
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
/// @return   TsToggleValveAssemblyConfigData&  (--)  Object assigned to.
///
/// @details  Assignment operator for this Toggle Valve Assembly configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsToggleValveAssemblyConfigData& TsToggleValveAssemblyConfigData::operator=(
        const TsToggleValveAssemblyConfigData& that)
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
/// @details  Default constructs this Toggle Valve Assembly input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsToggleValveAssemblyInputData::TsToggleValveAssemblyInputData(
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
/// @details  Default destructs this Toggle Valve Assembly input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsToggleValveAssemblyInputData::~TsToggleValveAssemblyInputData()
{
    // nothing to do
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that  (--)  Object to copy.
///
/// @details  Copy constructs this Toggle Valve Assembly input data.
//////////////////////////////////////////////////////////////////////////////////////////////////////
TsToggleValveAssemblyInputData::TsToggleValveAssemblyInputData(
        const TsToggleValveAssemblyInputData& that)
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
/// @return   TsToggleValveAssemblyInputData&  (--)  Object assigned to.
///
/// @details  Assignment operator for this Toggle Valve Assembly input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsToggleValveAssemblyInputData& TsToggleValveAssemblyInputData::operator=(
        const TsToggleValveAssemblyInputData& that)
{
    if (this != &that) {
        mController   = that.mController;
        mSensorOpen   = that.mSensorOpen;
        mSensorClosed = that.mSensorClosed;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Toggle Valve Assembly model.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsToggleValveAssembly::TsToggleValveAssembly()
    :
    mController(),
    mSensorOpen(),
    mSensorClosed(),
    mInitialized(false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Toggle Valve Assembly model.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsToggleValveAssembly::~TsToggleValveAssembly()
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
/// @details  Initializes this Toggle Valve Assembly model with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsToggleValveAssembly::initialize(const TsToggleValveAssemblyConfigData& configData,
                                       const TsToggleValveAssemblyInputData&  inputData,
                                       const std::string&                     name)
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
void TsToggleValveAssembly::update(const double dt)
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
/// @param[in]  enable             (--)  Valve enable command.
/// @param[in]  toggle             (--)  Valve toggle command.
/// @param[in]  dt                 (s)   Time step.
///
/// @return   void
///
/// @details  Updates the sensors and controller with arguments, so it can be run from a container.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsToggleValveAssembly::update(const bool   supplyVoltageFlag,
                                   const bool   enable,
                                   const bool   toggle,
                                   const double dt)
{
    /// - Set supply voltage flag and command.
    mController.setSupplyVoltageFlag(supplyVoltageFlag);
    mController.setEnabledFlag(enable);
    mController.setToggle(toggle);

    /// - Update the sensors and controller.
    update(dt);
}
