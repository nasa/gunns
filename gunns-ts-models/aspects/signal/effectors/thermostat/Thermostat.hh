#ifndef Thermostat_EXISTS
#define Thermostat_EXISTS
/***************************************************************************************************
@defgroup TSM_SIGNAL_EFFECTORS_THERMOSTAT Thermostat model
@ingroup  TSM_SIGNAL_EFFECTORS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
   (The Thermostat is an optional signal aspect within a heater system. Its purpose is to
    determine an ON/OFF status (mCommandActual) that it passes to the electrical aspect. The
    Thermostat contains one or more SensorAnalogs and compares the sensed values against
    configured temperature setpoints. Using logic, the Thermostat determines if the
    heater's electrical aspect should be activated and publishes the ON/OFF flag to the
    simbus. The Thermostat is essentially a switch that can kill power to a heater even
    if its RPC switch is closed.

    The Thermostat is configured with one of three LogicTypes: SINGLE, OR, or AND. The behavior
    varies only in the returnLoadModePrimary() and returnLoadModeSecondary() methods. Other logic
    patterns can be implemented by deriving from this class and overwriting the readSensors() method.

    Logic in the readSensors() method loops through all the thermostat's sensors, searching for a
    sensor that evaluates to the "Primary" load mode, either ON or OFF. Once a Primary is found, the
    overall command state is set to the Primary mode. If no Primary is found, the overall command
    is set to the "Secondary" mode. Primary and Secondary values are specific to the logic
    pattern of the Thermostat instance.

        -- ThermostatConfigData::SINGLE --
                (single-sensor) Thermostat decides ON/OFF status based on temperature reading
                from one sensor.

        -- ThermostatConfigData::AND --
                (multi-sensor:AND) Multiple sensors in series;
                once ALL sensors read 'low', command set to ON.
                The "Primary" mode is OFF, since the first sensor that evaluates to OFF
                establishes the overall command state as OFF.

        -- ThermostatConfigData::OR --
                (multi-sensor:OR) Multiple sensors in parallel;
                once ANY sensor reads 'low', command set to ON.
                The "Primary" mode is ON, since the first sensor that evaluates to ON
                establishes the overall command state as ON.

    The Thermostat class is loaded with two different malfunction types. Note that if malfs
    are active, the mCommandNominal member will continue to be maintained, just the
    mCommandActual member will be overwritten.

        -- Command:ON and Command:OFF
                These malfunctions, set by activating mMalfCmdOnFlag or mMalfCmdOffFlag,
                force the Thermostat to command status ON or OFF, respectively. The command OFF
                malfunction takes precedence, and will automatically de-activate the ON malf
                if both are active at once. Once both malfunctions are no longer active, the
                Thermostat returns to command status based on its sensor logic, as if no malf
                had ever happened at all.

        -- Setpoint malfunction
                This malfunction is activated by triggering either the mMalfSetptLowFlag or
                mMalfSeptHighFlag. Once active, the Thermostat acts based on its respective
                malfunction setpoint, set in input data, instead of the nominal one.)

REQUIREMENTS:
    (R.TS210-0002: TS21 vehicle subsystem models shall provide component values within spec limits.)
    (R.TS210-0010: TS21 vehicle subsystem models shall respond to thermal inputs.)
    (R.TS228-0006: Spacecraft thermal models shall perform heater control.)
    (R.TS228-0031: Spacecraft thermal models shall provide heater malfunctions.)

REFERENCES:
    (MDC-95H0225E, Boeing Heater System Notebook)
    (ISS Thermal Control Training Manual, 01.07.05(0)T0012)
    (Module, Electronic, Thermostat, Remote Sense, Single, drawing 1F977596 rev D)
    (Module, Electronic, Thermostat, Remote Sense,  Dual,  drawing 1F977597 rev J)

ASSUMPTIONS AND LIMITATIONS:
    (All Thermostats are assumed to use basic RTD (resistive temperature device) sensors. These
     are contained within the Thermostat class and are given the same limits and default readings
     for every Thermostat object. They can still be malfunctioned as possible within the
     SensorAnalog class.)
    (IMPORTANT - Each sensor must be given a truth value from the simbus or another outside
     source.)

LIBRARY DEPENDENCY:
    (Thermostat.o)

PROGRAMMERS:
    (
    (Joe Valerioti) (L3) (Aug 2012) (prototype, unit testing)
                         (Jun 2013) (convert to inherit from HtrControl class)
    (Bob Hafernick) (FDOC) (July 31, 2014) (remove config data from checkpoint)
    )
@{
***************************************************************************************************/
#include "aspects/signal/effectors/thermostat/HtrControl.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "common/sensors/SensorAnalog.hh"

/// Forward declarations
//class std::string;   //cannot fwd-declare since <string> is currently #included in UserLoadBase.hh

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Thermostat Configuration Data.
///
/// @details  The sole purpose of this class is to provide a data structure for the Thermostat
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class ThermostatConfigData : public HtrControlConfigData
{
    public:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief    Enumeration of t types.
        ///
        /// @details  This enumeration is used to designate each of the thermostat logic types.
        ///           SINGLE: (default) thermostat has only one sensor
        ///           AND: multi-sensor, in series; once ALL sensors read 'low', command set to ON.
        ///           OR: multi-sensor, in parallel; once ANY sensor reads 'low', command set to ON.
        ////////////////////////////////////////////////////////////////////////////////////////////
        enum LogicType
        {
            SINGLE = 1,
            AND = 2,
            OR = 3,
        };

        SensorAnalogConfigData cSensorConfig; /**< (--) Config data for Thermostat's sensor(s) */

        double cNominalSetptLow;   /**< (K)   Temperature at which to command heater on */
        double cNominalSetptHigh;  /**< (K)   Temperature at which to command heater off */
        int cNumSensors;           /**< (--)  Number of sensors monitored by the Thermostat */
        LogicType cLogicType;      /**< (--)  Logic type of the Thermostat */

        /// @brief  Default constructs this Thermostat configuration data.
        ThermostatConfigData(SensorAnalogConfigData& sensorConfig,
                             const double temperatureSetptLow  = 220.0,
                             const double temperatureSetptHigh = 240.0,
                             const int numTempSensors = 1,
                             const LogicType logicType = SINGLE);

        /// @brief  Copy constructs this Thermostat configuration data.
        ThermostatConfigData(const ThermostatConfigData& that);

        /// @brief  Default destructs this Thermostat configuration data.
        virtual ~ThermostatConfigData();

    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ThermostatConfigData& operator=(const ThermostatConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Thermostat Input Data.
///
/// @details  The sole purpose of this class is to provide a data structure for the Thermostat
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class ThermostatInputData : public HtrControlInputData
{
    public:
        SensorAnalogInputData iSensorInput; /**< (--) Input data for Thermostat's sensor(s) */

        bool iMalfSetptLowFlag;      /**< (--)  Malfunction flag, override temp setpoint-low */
        bool iMalfSetptHighFlag;     /**< (--)  Malfunction flag, override temp setpoints-high */
        double iMalfSetptLowValue;   /**< (K)   Malfunction value, override setpoint-low */
        double iMalfSetptHighValue;  /**< (K)   Malfunction value, override setpoint-high */

        /// @brief  Default constructs this Thermostat input data.
        ThermostatInputData(SensorAnalogInputData& sensorInput,
                            const bool malfCmdOnFlag  = false,
                            const bool malfCmdOffFlag = false,
                            const bool malfSetptLowFlag  = false,
                            const bool malfSetptHighFlag = false,
                            const double malfSetptLowValue  =    0.0,
                            const double malfSetptHighValue = 1000.0);

        /// @brief  Copy constructs this Thermostat input data.
        ThermostatInputData(const ThermostatInputData& that);

        /// @brief  Default destructs this Thermostat input data.
        virtual ~ThermostatInputData();

    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ThermostatInputData& operator=(const ThermostatInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Thermostat class
///
/// @details  The Thermostat is an optional signal aspect within a heater system. Its purpose is to
///           determine an ON/OFF command that it passes to the electrical aspect. This command is
///           determined from malfunction state and the readings of one or more contained
///           temperature sensors.
////////////////////////////////////////////////////////////////////////////////////////////////////
class Thermostat : public HtrControl
{
    TS_MAKE_SIM_COMPATIBLE(Thermostat);

    public:
        /// @name    Embedded objects.
        /// @{
        /// @details Embedded objects are public to allow access from the Trick events processor.
        SensorAnalog* mTempSensor; /**< (--)  Pointer to temperature sensor(s) */
        /// @}

        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool mMalfSetptLowFlag;      /**< (--)  Malfunction flag, override temp setpoint-low */
        bool mMalfSetptHighFlag;     /**< (--)  Malfunction flag, override temp setpoints-high */
        double mMalfSetptLowValue;   /**< (K)   Malfunction value, override setpoint-low */
        double mMalfSetptHighValue;  /**< (K)   Malfunction value, override setpoint-high */
        /// @}

        /// @brief  Default constructs the object.
        Thermostat();

        /// @brief  Default destructs this Thermostat.
        virtual ~Thermostat();

        /// @brief  Loads object with config & input data and gives the object a name.
        void initialize(const ThermostatConfigData& config,
                        const ThermostatInputData& input,
                        const std::string& name = "Unnamed");

        /// @brief  Simulation update task.
        virtual void update(const double timeStep);

        /// @brief  Sets the setpoint bias command based on temperature setpoint parameter.
        void setTemperatureSetpt(const double newTempSetpt);

        /// @brief  Sets the setpoint bias command.
        void setCommandSetptBias(const double bias);

        /// @brief  Gets nominal set point low
        double getNominalSetptLow();

        /// @brief  Gets nominal set point high
        double getNominalSetptHigh();


   protected:
        int mNumSensors;                  /**< (--) trick_chkpnt_io(**) Number of sensors monitored by this Thermostat */

        ThermostatConfigData::LogicType mLogicType;   /**< (--) trick_chkpnt_io(**) Type of logic to apply to sensor reads */

        UserLoadMode mCommandSetpt;       /**< (--) Command from evaluating sensor temps against setpts */
        double mCommandSetptBias;         /**< (K)  Command to bias the nominal setpoint. */

        double mTemperatureSetpt;         /**< (K)  Temperature setpoint, nominal unless otherwise commanded. */
        double mSetptLow;                 /**< (K)  Temperature at which to command heater on */
        double mSetptHigh;                /**< (K)  Temperature at which to command heater off */

        double mNominalTemperatureSetpt;  /**< (K) trick_chkpnt_io(**) Nominal Temperature setpoint. */
        double mNominalSetptLow;          /**< (K) trick_chkpnt_io(**) Nominal low temperature setpoint */
        double mNominalSetptHigh;         /**< (K) trick_chkpnt_io(**) Nominal high temperature setpoint */

        /// @brief  Validates this Thermostat configuration and input data.
        void validate(const ThermostatConfigData& config,
                      const ThermostatInputData& input) const;

        /// @brief  Analyzes sensor readings and returns an overall setpoint command status.
        virtual UserLoadMode readSensors(const UserLoadMode commandStatus);

        /// @brief  Determines command status based on input temperature and given setpoints.
        UserLoadMode evaluateAgainstSetpoints(const UserLoadMode commandStatus,
                                              const double sensedTemp);

        /// @brief  Returns the "Primary" UserLoadMode, specific to logic type.
        UserLoadMode returnLoadModePrimary() const;

        /// @brief  Returns the "Secondary" UserLoadMode, specific to logic type.
        UserLoadMode returnLoadModeSecondary() const;

    private:
        /// @details  Copy constructor unavailable since declared private and not implemented.
        Thermostat(const Thermostat&);
        /// @details  Assignment operator unavailable since declared private and not implemented.
        Thermostat& operator=(const Thermostat&);
};
/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Returns nominal set point low.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double Thermostat::getNominalSetptLow()
{
    return (mNominalSetptLow);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Returns nominal set point high.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double Thermostat::getNominalSetptHigh()
{
    return (mNominalSetptHigh);
}

#endif // Thermostat_EXISTS
