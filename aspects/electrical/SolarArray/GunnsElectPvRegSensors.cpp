/**
@file
@brief    GUNNS Photovoltaic Regulator Sensors implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((core/GunnsSensorAnalogWrapper.o))
*/

#include "GunnsElectPvRegSensors.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Photovoltaic Regulator Sensors.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvRegSensors::GunnsElectPvRegSensors()
    :
    mInVoltage(0),
    mInCurrent(0),
    mOutVoltage(0),
    mOutCurrent(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Photovoltaic Regulator Sensors.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvRegSensors::~GunnsElectPvRegSensors()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in, out] wrapper (--) The sensor wrapper network spotter containing the actual sensor.
///
/// @details  Additionally, calls configureWrapperStepFlags.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvRegSensors::connectInVoltage(GunnsSensorAnalogWrapper* wrapper)
{
    if (wrapper) {
        mInVoltage = &wrapper->mSensor;
        configureWrapperStepFlags(wrapper);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in, out] wrapper (--) The sensor wrapper network spotter containing the actual sensor.
///
/// @details  Additionally, calls configureWrapperStepFlags.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvRegSensors::connectInCurrent(GunnsSensorAnalogWrapper* wrapper)
{
    if (wrapper) {
        mInCurrent = &wrapper->mSensor;
        configureWrapperStepFlags(wrapper);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in, out] wrapper (--) The sensor wrapper network spotter containing the actual sensor.
///
/// @details  Additionally, calls configureWrapperStepFlags.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvRegSensors::connectOutVoltage(GunnsSensorAnalogWrapper* wrapper)
{
    if (wrapper) {
        mOutVoltage = &wrapper->mSensor;
        configureWrapperStepFlags(wrapper);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in, out] wrapper (--) The sensor wrapper network spotter containing the actual sensor.
///
/// @details  Additionally, calls configureWrapperStepFlags.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvRegSensors::connectOutCurrent(GunnsSensorAnalogWrapper* wrapper)
{
    if (wrapper) {
        mOutCurrent = &wrapper->mSensor;
        configureWrapperStepFlags(wrapper);
    }
}
