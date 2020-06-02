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
    ()
 )

 PROGRAMMERS:
 (
  ((Chuck Sorensen) (LZT) (Dec 2, 2011) (TS21) (initial))
 )
 **************************************************************************************************/

#include "TsLimitSwitch.hh"


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Config data Default constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsLimitSwitchConfigData::TsLimitSwitchConfigData() :
    mNotPoweredValue(false) {
    ; // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Config data Default destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
TsLimitSwitchConfigData::~TsLimitSwitchConfigData() {
    ; // nothing to do
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] trueValue   (--) initial true value
/// @param[in] sensedValue (--) initial sensed value
/// @param[in] failedValue (--) initial failed value
/// @details  Input data Default constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsLimitSwitchInputData::TsLimitSwitchInputData(
        const bool trueValue,
        const bool sensedValue,
        const bool failedValue) :
    mTrueValue(trueValue),
    mSensedValue(sensedValue),
    mFailedValue(failedValue) {
    ; // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Input data Default destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
TsLimitSwitchInputData::~TsLimitSwitchInputData() {
    ; // nothing to do
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
TsLimitSwitch::TsLimitSwitch() :
    mInitialized(false),
    mTrueValue(false),
    mSensedValue(false),
    mFailedValue(false),
    mNotPoweredValue(false) {
    ; // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
TsLimitSwitch::~TsLimitSwitch() {
    ; // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] configData (--) configuration data
/// @param[in] inputData  (--) input data
/// @details  initialization
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsLimitSwitch::initialize(
        const TsLimitSwitchConfigData &configData,
        const TsLimitSwitchInputData &inputData) {
    mTrueValue       = inputData.mTrueValue;
    mSensedValue     = inputData.mSensedValue;
    mFailedValue     = inputData.mFailedValue;
    mNotPoweredValue = configData.mNotPoweredValue;
    mInitialized     = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] realValue  (--) actual position
/// @param[in] isPowered  (--) is power available
/// @param[in] isFailed   (--) is switch/sensor failed
/// @details  sets a sensed value based on if the switch/sensor has power, is failed and actual
/// value.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsLimitSwitch::update(
   const bool realValue,
   const bool isPowered,
   const bool isFailed) {
    mTrueValue = realValue;
    if(isFailed) {
        mSensedValue = mFailedValue;
    } else if (!isPowered) {
        mSensedValue = mNotPoweredValue;
    } else {
        mSensedValue = realValue;
    }
}
