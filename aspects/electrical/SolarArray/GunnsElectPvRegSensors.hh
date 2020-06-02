#ifndef GunnsElectPvRegSensors_EXISTS
#define GunnsElectPvRegSensors_EXISTS

/**
@file
@brief    GUNNS Photovoltaic Regulator Sensors declarations

@defgroup  GUNNS_ELECTRICAL_PHOTOVOLTAIC_SENSORS    GUNNS Photovoltaic Regulator Sensors
@ingroup   GUNNS_ELECTRICAL_PHOTOVOLTAIC

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Photovoltaic Regulator Sensors.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsElectPvRegSensors.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2017-12) (Initial))

@{
*/

#include "core/GunnsSensorAnalogWrapper.hh"

// Forward-declare pointer types.
class SensorAnalog;
class GunnsSensorAnalogWrapper;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Photovoltaic Regulator Sensors.
///
/// @details  This encapsulates the sensors for the Photovoltaic regulator links in a simple
///           container class to reduce clutter.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectPvRegSensors
{
    public:
        SensorAnalog* mInVoltage;     /**< (--) Pointer to the input voltage sensor. */
        SensorAnalog* mInCurrent;     /**< (--) Pointer to the input current sensor. */
        SensorAnalog* mOutVoltage;    /**< (--) Pointer to the output voltage sensor. */
        SensorAnalog* mOutCurrent;    /**< (--) Pointer to the output current sensor. */
        /// @brief  Default constructor.
        GunnsElectPvRegSensors();
        /// @brief  Default destructor.
        virtual ~GunnsElectPvRegSensors();
        /// @brief  Configures the input voltage sensor pointer with the given wrapper spotter.
        void connectInVoltage(GunnsSensorAnalogWrapper* wrapper);
        /// @brief  Configures the input current sensor pointer with the given wrapper spotter.
        void connectInCurrent(GunnsSensorAnalogWrapper* wrapper);
        /// @brief  Configures the output voltage sensor pointer with the given wrapper spotter.
        void connectOutVoltage(GunnsSensorAnalogWrapper* wrapper);
        /// @brief  Configures the output current sensor pointer with the given wrapper spotter.
        void connectOutCurrent(GunnsSensorAnalogWrapper* wrapper);

    private:
        /// @brief  Configures the sensor wrapper spotter to step the sensor after the solver.
        void configureWrapperStepFlags(GunnsSensorAnalogWrapper* wrapper);
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsElectPvRegSensors(const GunnsElectPvRegSensors& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsElectPvRegSensors& operator =(const GunnsElectPvRegSensors&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in, out] wrapper (--) The sensor wrapper network spotter containing the actual sensor.
///
/// @details  Even though the PV regulators step the sensors multiple times each network update
///           cycle, they always pass zero time step to the sensor, so the sensor's drift malf isn't
///           integrated by those calls.  So we have the spotter step the sensor one last time at
///           the end of the network cycle, this time with a time step to finally update the drift.
///
/// @note  Caller must ensure that wrapper argument isn't NULL.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectPvRegSensors::configureWrapperStepFlags(GunnsSensorAnalogWrapper* wrapper)
{
    wrapper->setStepPreSolverFlag(false);
    wrapper->setStepPostSolverFlag(true);
}

#endif
