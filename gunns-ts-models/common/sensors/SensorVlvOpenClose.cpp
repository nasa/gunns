/****************************** TRICK HEADER ******************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
 (
    (common/sensors/SensorBooleanAi.o)
 )

 PROGRAMMERS:
 (
  ((Chuck Sorensen) (LZT) (May 2012) (TS21) (initial))
 )
 **************************************************************************************************/

#include "SensorVlvOpenClose.hh"

//////////////////////////////////////////////////////////////////////////////////////////////////
// @param[in] open (--) open config data
// @param[in] close (--) close config data
// @details  Config data constructor
//////////////////////////////////////////////////////////////////////////////////////////////////
SensorVlvOpenCloseConfigData::SensorVlvOpenCloseConfigData(
        const SensorBooleanAiConfigData &open,
        const SensorBooleanAiConfigData &close ) :
    mOpen(open),
    mClose(close) {
    // nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Config data Default destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorVlvOpenCloseConfigData::~SensorVlvOpenCloseConfigData() {
    // nothing to do
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] isPowered (--) power flag - apply to both sensors
/// @details  Input data Default constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorVlvOpenCloseInputData::SensorVlvOpenCloseInputData(const bool isPowered) :
    mOpen(isPowered),
    mClose(isPowered) {
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Input data Default destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorVlvOpenCloseInputData::~SensorVlvOpenCloseInputData() {
    // nothing to do
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorVlvOpenClose::SensorVlvOpenClose() :
    mOpen(),
    mClose(),
    mInitFlag(false) {
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
SensorVlvOpenClose::~SensorVlvOpenClose() {
    // nothing to do
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] config (--) configuration data
/// @param[in] input  (--) input data
/// @param[in] name   (--) object name
/// @throw TsInitializationException
/// @details  initialization
////////////////////////////////////////////////////////////////////////////////////////////////////
void SensorVlvOpenClose::initialize(
    const SensorVlvOpenCloseConfigData& config,
    const SensorVlvOpenCloseInputData& input,
    const std::string& name) {
    mInitFlag = false;
    if ("" == name) {
        mOpen.initialize( config.mOpen,  input.mOpen,  "Open sensor");
        mClose.initialize(config.mClose, input.mClose, "Close sensor");
    } else {
        mOpen.initialize( config.mOpen,  input.mOpen,  (name + "_Open_Sensor").c_str());
        mClose.initialize(config.mClose, input.mClose, (name + "_Close_Sensor").c_str());
    }
    mInitFlag = mOpen.isInitialized() && mClose.isInitialized();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] deltaTime (s)  time step
/// @param[in] isPowered (--) flag indicating sensors has adequate power
/// @param[in] position  (--) valve position
/// @details updates the open & close sensor given the position, powered flag and internal
/// sensor failure flags.
////////////////////////////////////////////////////////////////////////////////////////////////////
void SensorVlvOpenClose::sense(const double deltaTime, const bool isPowered, const double position) {
    mOpen.setPowerFlag(isPowered);
    mClose.setPowerFlag(isPowered);
    mOpen.setTruthInput(position);
    mClose.setTruthInput(position);
    mOpen.update(deltaTime);
    mClose.update(deltaTime);

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] deltaTime (s)  time step
/// @details update the sensor pair
////////////////////////////////////////////////////////////////////////////////////////////////////
void SensorVlvOpenClose::update(const double deltaTime) {
    mClose.update(deltaTime);
    mOpen.update(deltaTime);
}
