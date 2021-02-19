#ifndef GunnsSensorAnalogWrapper_EXISTS
#define GunnsSensorAnalogWrapper_EXISTS

/**
@file
@brief     GUNNS Analog Sensor Wrapper Spotter declarations

@defgroup  TSM_GUNNS_CORE_SENSOR_ANALOG_WRAPPER    GUNNS Analog Sensor Wrapper Spotter
@ingroup   TSM_GUNNS_CORE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Provides the classes for the GUNNS Analog Sensor Wrapper Spotter.)

@details
REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsSensorAnalogWrapper.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2017-12) (Initial))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "GunnsNetworkSpotter.hh"
#include "common/sensors/SensorAnalog.hh"
#include "common/sensors/TsNoise.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Analog Sensor Wrapper Spotter Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Analog
///           Sensor Wrapper Spotter configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsSensorAnalogWrapperConfigData : public GunnsNetworkSpotterConfigData
{
    public:
        /// @brief A function pointer type of signature: double function(), used by the sensor class
        ///        and this config data class to store pointers to external noise functions.
        typedef double (*fctnPtr)();
        SensorAnalogConfigData mSensor;     /**< (--) trick_chkpnt_io(**) The sensor config data. */
        /// @brief  Default constructs this GUNNS Analog Sensor Wrapper Spotter configuration data.
        GunnsSensorAnalogWrapperConfigData(const std::string&         name,
                                           const float                minRange          = 0.0,
                                           const float                maxRange          = 0.0,
                                           const float                offValue          = 0.0,
                                           const float                nominalBias       = 0.0,
                                           const float                nominalScale      = 1.0,
                                           const float                nominalNoiseScale = 0.0,
                                           const float                nominalResolution = 0.0,
                                           fctnPtr                    noiseFunction     = TsNoise::getNoise,
                                           const UnitConversion::Type unitConversion    = UnitConversion::NO_CONVERSION,
                                           const float                nominalNoiseMult  = 0.0);
        /// @brief  Default destructs this GUNNS Analog Sensor Wrapper Spotter configuration data.
        virtual ~GunnsSensorAnalogWrapperConfigData();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsSensorAnalogWrapperConfigData(const GunnsSensorAnalogWrapperConfigData& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsSensorAnalogWrapperConfigData& operator =(const GunnsSensorAnalogWrapperConfigData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Analog Sensor Wrapper Spotter Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Analog
///           Sensor Wrapper Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsSensorAnalogWrapperInputData : public GunnsNetworkSpotterInputData
{
    public:
        SensorAnalogInputData mSensor;       /**< (--) trick_chkpnt_io(**) The sensor input data. */
        /// @brief  Default constructs this GUNNS Analog Sensor Wrapper Spotter input data.
        GunnsSensorAnalogWrapperInputData(const bool   powerFlag  = false,
                                          const double truthInput = 0.0);
        /// @brief  Default destructs this GUNNS Analog Sensor Wrapper Spotter input data.
        virtual ~GunnsSensorAnalogWrapperInputData();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsSensorAnalogWrapperInputData(const GunnsSensorAnalogWrapperInputData& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsSensorAnalogWrapperInputData& operator =(const GunnsSensorAnalogWrapperInputData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Analog Sensor Wrapper Spotter Class.
///
/// @details  This spotter is used to wrap a generic analog sensor in a GUNNS network.  It
///           configures, initializes, and can step the sensor.  The sensor is public to allow other
///           objects to interface to it.
///
///           This spotter can step the sensor either before or after the network solution, or
///           neither, but not both.  The default configuration will step the sensor after the
///           network solution.  This can be changed by a call to the setStepPreSolverFlag and
///           setStepPostSolverFlag methods from outside.  If both flags are set, this will only
///           step the sensor after the network solution, and not before.
///
///           The sensor can also be stepped by external models.  External models are also
///           responsible for setting the sensor's truth value input and getting its output.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsSensorAnalogWrapper : public GunnsNetworkSpotter
{
    TS_MAKE_SIM_COMPATIBLE(GunnsSensorAnalogWrapper);
    public:
        SensorAnalog mSensor;             /**< (--)                     The sensor. */
        /// @brief   Default constructor.
        GunnsSensorAnalogWrapper();
        /// @brief   Default destructor.
        virtual     ~GunnsSensorAnalogWrapper();
        /// @brief   Initializes the GUNNS Analog Sensor Wrapper Spotter with configuration and
        ///          input data.
        virtual void initialize(const GunnsNetworkSpotterConfigData* configData,
                                const GunnsNetworkSpotterInputData*  inputData);
        /// @brief   Steps the GUNNS Analog Sensor Wrapper Spotter prior to the GUNNS solver step.
        virtual void stepPreSolver(const double dt);
        /// @brief   Steps the GUNNS Analog Sensor Wrapper Spotter after the GUNNS solver step.
        virtual void stepPostSolver(const double dt);
        /// @brief   Sets the flag controlling update of the sensor before the network solution.
        void         setStepPreSolverFlag(const bool flag);
        /// @brief   Sets the flag controlling update of the sensor after the network solution.
        void         setStepPostSolverFlag(const bool flag);

    protected:
        bool         mStepPreSolverFlag;  /**< (--) trick_chkpnt_io(**) When true, the sensor will be updated before the network solution. */
        bool         mStepPostSolverFlag; /**< (--) trick_chkpnt_io(**) When true, the sensor will be updated after the network solution. */
        /// @brief   Validates the supplied configuration data.
        const GunnsSensorAnalogWrapperConfigData* validateConfig(const GunnsNetworkSpotterConfigData* config);
        /// @brief   Validates the supplied input data.
        const GunnsSensorAnalogWrapperInputData*  validateInput (const GunnsNetworkSpotterInputData* input);

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsSensorAnalogWrapper(const GunnsSensorAnalogWrapper& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsSensorAnalogWrapper& operator =(const GunnsSensorAnalogWrapper& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  flag  (--)  True enables the sensor update, false disables.
///
/// @details  Sets the mStepPreSolverFlag attribute to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsSensorAnalogWrapper::setStepPreSolverFlag(const bool flag)
{
    mStepPreSolverFlag = flag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  flag  (--)  True enables the sensor update, false disables.
///
/// @details  Sets the mStepPostSolverFlag attribute to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsSensorAnalogWrapper::setStepPostSolverFlag(const bool flag)
{
    mStepPostSolverFlag = flag;
}

#endif
