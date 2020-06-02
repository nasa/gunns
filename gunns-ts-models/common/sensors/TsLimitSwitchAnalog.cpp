/****************************** TRICK HEADER ******************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
 ()

 ASSUMPTIONS AND LIMITATIONS:
 ()


 REFERENCE:
 ()

 LIBRARY DEPENDENCY:
 (
    (simulation/hs/TsHsMsg.o)
    (software/exceptions/TsInitializationException.o)
    (common/sensors/TsLimitSwitch.o)
 )

 PROGRAMMERS:
 (
  ((Chuck Sorensen) (LZT) (Dec 2, 2011) (TS21) (initial))
 )
 **************************************************************************************************/

#include "TsLimitSwitchAnalog.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"
#include <cmath>
#include <cfloat>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Config data Default constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsLimitSwitchAnalogConfigData::TsLimitSwitchAnalogConfigData() :
    TsLimitSwitchConfigData(),
    mTargetValue(0.0),
    mTolerance(0.001) {
    ; // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Config data Default destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
TsLimitSwitchAnalogConfigData::~TsLimitSwitchAnalogConfigData() {
    ; // nothing to do
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Input data Default constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsLimitSwitchAnalogInputData::TsLimitSwitchAnalogInputData(
        const bool trueValue,
        const bool sensedValue,
        const bool failedValue) :
    TsLimitSwitchInputData(trueValue, sensedValue, failedValue) {
    ; // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Input data Default destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
TsLimitSwitchAnalogInputData::~TsLimitSwitchAnalogInputData() {
    ; // nothing to do
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
TsLimitSwitchAnalog::TsLimitSwitchAnalog() :
   TsLimitSwitch(),
   mAnalogValue(0.0),
   mTargetValue(0.0),
   mTolerance(0.1)  {
    ; // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
TsLimitSwitchAnalog::~TsLimitSwitchAnalog() {
    ; // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] configData  (--) configuration data
/// @param[in] inputData   (--) initialization data
/// @throws TsInitializaton error when tolerance is too small
/// @details initializes the member variables to config & init values.
///////////////////////////////////////////////////////////////////////////////////////////////////
void TsLimitSwitchAnalog::initialize(
        const TsLimitSwitchAnalogConfigData &configData,
        const TsLimitSwitchAnalogInputData  &inputData) {

    TsLimitSwitch::initialize(configData, inputData);

    if(configData.mTolerance < FLT_EPSILON) {
        hsSendMsg(TS_HS_FATAL, "SENSORS",
                "TsLimitSwitchAnalog - initialization error: tolerance too small" );
        throw TsInitializationException("Invalid initialization - tolerance too small",
                "TsLimitSwitchAnalog::initialize");
    }

    mTolerance = configData.mTolerance;
    mTargetValue = configData.mTargetValue;

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] realValue (--) actual value
/// @param[in] isPowered (--) indicates if the sensor has sufficient power to function
/// @param[in] isFailed  (--) indicates if the sensor has failed
/// @details set the sensed value
///////////////////////////////////////////////////////////////////////////////////////////////////
void TsLimitSwitchAnalog::update(
        const double realValue,
        const bool isPowered,
        const bool isFailed) {

    mAnalogValue = realValue;

    /// - set local variable inRange true if realValue within tolerance of target
    bool inRange = false;
    if(fabs(realValue - mTargetValue) < mTolerance) {
        inRange = true;
    }

    /// - check if realValue goes past mTargetValue without ever being within tolerance
    else if((mTargetValue > 0.0)&&(realValue > mTargetValue)) {
        inRange = true;
    }
    else if((mTargetValue < 0.0)&&(realValue < mTargetValue)) {
        inRange = true;
    }

    /// - call limit switch sensor with inRange.
    TsLimitSwitch::update(inRange, isPowered, isFailed);
}
