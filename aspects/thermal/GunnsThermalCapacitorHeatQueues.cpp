/**
@file      GunnsThermalCapacitorHeatQueues.cpp
@brief     GUNNS Thermal Capacitor Heat Queues Spotter implementation

@copyright Copyright 2020 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((core/GunnsNetworkSpotter.o)
   (simulation/hs/TsHsMsg.o)
   (software/exceptions/TsInitializationException.o)
   (aspects/thermal/GunnsThermalCapacitor.o)
   (software/SimBus/SimBusQutils.o))
*/

#include "GunnsThermalCapacitorHeatQueues.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/SimBus/SimBusQutils.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name  (--)  Instance name for self-identification in messages.
///
/// @details  Default constructs this GUNNS Thermal Capacitor Heat Queues Spotter configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalCapacitorHeatQueuesConfigData::GunnsThermalCapacitorHeatQueuesConfigData(const std::string& name)
    :
    GunnsNetworkSpotterConfigData(name)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Thermal Capacitor Heat Queues Spotter configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalCapacitorHeatQueuesConfigData::~GunnsThermalCapacitorHeatQueuesConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  postStepCounter  (--)  Initial counter to support unit tests.
///
/// @details  Default constructs this GUNNS Thermal Capacitor Heat Queues Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalCapacitorHeatQueuesInputData::GunnsThermalCapacitorHeatQueuesInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Thermal Capacitor Heat Queues Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalCapacitorHeatQueuesInputData::~GunnsThermalCapacitorHeatQueuesInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Thermal Capacitor Heat Queue data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalCapacitorHeatQueueData::GunnsThermalCapacitorHeatQueueData()
    :
    mHeat_queue(0),
    mHeat_queue_size(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Thermal Capacitor Heat Queue data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalCapacitorHeatQueueData::~GunnsThermalCapacitorHeatQueueData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  capacitor  (--)  Reference to the thermal capacitor link.
///
/// @details Default constructs this GUNNS Thermal Capacitor Heat Queues Spotter.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalCapacitorHeatQueues::GunnsThermalCapacitorHeatQueues(GunnsThermalCapacitor& capacitor)
    :
    GunnsNetworkSpotter(),
    mCapacitor(capacitor),
    mQueues()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Thermal Capacitor Heat Queues Spotter.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalCapacitorHeatQueues::~GunnsThermalCapacitorHeatQueues()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  Instance configuration data.
/// @param[in]  inputData   (--)  Instance input data.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Thermal Capacitor Heat Queues Spotter with its configuration
///           and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalCapacitorHeatQueues::initialize(const GunnsNetworkSpotterConfigData* configData,
                                          const GunnsNetworkSpotterInputData*  inputData)
{
    /// - Initialize the base class.
    GunnsNetworkSpotter::initialize(configData, inputData);

    /// - Reset the init flag.
    mInitFlag = false;

    /// - Validate config & input data.
    validateConfig(configData);
    validateInput(inputData);

    /// - Set the init flag.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  Instance configuration data.
///
/// @returns  GunnsThermalCapacitorHeatQueuesConfigData (--) Type-casted and validated config data pointer.
///
/// @throws   TsInitializationException
///
/// @details  Type-casts the base config data class pointer to this spotter's config data type,
///           checks for valid type-cast and validates contained data.
////////////////////////////////////////////////////////////////////////////////////////////////////
const GunnsThermalCapacitorHeatQueuesConfigData* GunnsThermalCapacitorHeatQueues::validateConfig(const GunnsNetworkSpotterConfigData* config)
{
    const GunnsThermalCapacitorHeatQueuesConfigData* result = dynamic_cast<const GunnsThermalCapacitorHeatQueuesConfigData*>(config);
    if (!result) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Bad config data pointer type.");
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  inputData  (--)  Instance input data.
///
/// @returns  GunnsThermalCapacitorHeatQueuesInputData (--) Type-casted and validated input data pointer.
///
/// @throws   TsInitializationException
///
/// @details  Type-casts the base input data class pointer to this spotter's input data type,
///           checks for valid type-cast and validates contained data.
////////////////////////////////////////////////////////////////////////////////////////////////////
const GunnsThermalCapacitorHeatQueuesInputData* GunnsThermalCapacitorHeatQueues::validateInput(const GunnsNetworkSpotterInputData* input)
{
    const GunnsThermalCapacitorHeatQueuesInputData* result = dynamic_cast<const GunnsThermalCapacitorHeatQueuesInputData*>(input);
    if (!result) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Bad input data pointer type.");
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Execution time step (not used).
///
/// @details  For each queue, if the queue has data in it then use the SimBusQutils to get the
///           average heat in the queue and give this average to the thermal capacitor.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalCapacitorHeatQueues::stepPreSolver(const double dt __attribute__((unused)))
{
    for (int i=0; i<GunnsThermalCapacitor::NUM_EXT_HEATFLUXES; i++) {
        if (mQueues[i].mHeat_queue_size > 0) {
            mCapacitor.setExternalHeatFlux(i, SimBusQutils::getAverage(mQueues[i].mHeat_queue, mQueues[i].mHeat_queue_size));
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Execution time step (not used).
///
/// @details  Integrate the constituent masses flowed through the link.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalCapacitorHeatQueues::stepPostSolver(const double dt __attribute__((unused)))
{
    // nothing to do
}
