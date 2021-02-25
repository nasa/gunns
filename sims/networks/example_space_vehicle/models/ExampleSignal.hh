#ifndef ExampleSignal_EXISTS
#define ExampleSignal_EXISTS

/**
@file
@brief     Example Space Vehicle Signal Aspect declarations.

@defgroup  GUNNS_EXAMPLE_SIGNAL   Example Space Vehicle Signal Aspect Manager
@ingroup   GUNNS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:  (This is our example Signal Aspect class.  It holds various sensors models, effector
           controller models and related logic.  In an actual application we would normally break
           this up into separate classes by vehicle section or system/subsystem.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((ExampleSignal.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2019-01))

@{
*/

#include <string>

// Includes from ms-utils
#include "software/SimCompatibility/TsSimCompatibility.hh"

// Includes from gunns-ts-models
#include "common/sensors/SensorAnalog.hh"
#include "common/controllers/generic/TsPidController.hh"
#include "common/controllers/fluid/TsPumpMotorController.hh"
#include "common/controllers/fluid/TsOpenCloseValveController.hh"
#include "common/controllers/fluid/TsPositionValveController.hh"
#include "aspects/signal/effectors/thermostat/Thermostat.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Example Space Vehicle Signal Aspect Manager Configuration Data
///
/// @details  This class provides a data structure for the Example Space Vehicle Signal Aspect
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class ExampleSignalConfigData
{
    public:
        SensorAnalogConfigData              sensorHxExitT;      /**< (--) Cabin heat exchanger exit temperature sensor. */
        SensorAnalogConfigData              sensorCabinP;       /**< (--) Cabin pressure sensor. */
        SensorAnalogConfigData              sensorCabinT;       /**< (--) Cabin temperature sensor. */
        SensorAnalogConfigData              sensorCabinPPO2;    /**< (--) Cabin partial pressure O2 sensor. */
        SensorAnalogConfigData              sensorCabinPPCO2;   /**< (--) Cabin partial pressure CO2 sensor. */
        SensorAnalogConfigData              sensorCabinRH;      /**< (--) Cabin relative humidity sensor. */
        SensorAnalogConfigData              sensorCabinFanDp;   /**< (--) Cabin fan delta-pressure sensor. */
        SensorAnalogConfigData              sensorCabinFanN;    /**< (--) Cabin fan speed sensor. */
        SensorAnalogConfigData              sensorAtcsFlow;     /**< (--) ATCS flow rate sensor. */
        SensorAnalogConfigData              sensorAtcsPumpDp;   /**< (--) ATCS pump delta-pressure sensor. */
        SensorAnalogConfigData              sensorAtcsPumpN;    /**< (--) ATCS pump speed sensor. */
        SensorAnalogConfigData              sensorAtcsPumpInT;  /**< (--) ATCS pump inlet temperature sensor. */
        SensorAnalogConfigData              sensorAtcsPumpInP;  /**< (--) ATCS pump inlet pressure sensor. */
        SensorAnalogConfigData              sensorAtcsRadInT;   /**< (--) ATCS radiator inlet temperature sensor. */
        SensorAnalogConfigData              sensorTcvPos;       /**< (--) Cabin temperature control valve position sensor. */
        SensorAnalogConfigData              sensorRadBypassPos; /**< (--) ATCS radiator bypass valve position sensor. */
        TsPidControllerConfigData           cabinTempCntlr;     /**< (--) Cabin temperature controller. */
        TsPidControllerConfigData           atcsTempCntlr;      /**< (--) ATCS temperature controller. */
        TsPumpMotorControllerConfigData     cabinFan;           /**< (--) Cabin fan motor & controller. */
        TsPumpMotorControllerConfigData     pump;               /**< (--) ATCS pump motor & controller. */
        TsPoweredValveControllerConfigData  gasSelectValve;     /**< (--) PCS gas select valve driver. */
        TsPositionValveControllerConfigData tcv;                /**< (--) Cabin temperature control valve driver. */
        TsPositionValveControllerConfigData radBypassValve;     /**< (--) ATCS radiator bypass valve driver. */
        SensorAnalogConfigData              sensorsShellT;      /**< (--) PTCS shell temperature sensors for thermostats. */
        ThermostatConfigData                fwdShellThermostat; /**< (--) PTCS forward shell thermostat. */
        ThermostatConfigData                aftShellThermostat; /**< (--) PTCS aft shell thermostat. */
        /// @brief  Default constructs this Example Space Vehicle Signal Aspect config data.
        ExampleSignalConfigData();
        /// @brief  Default destructs this Example Space Vehicle Signal Aspect config data.
        virtual ~ExampleSignalConfigData();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        ExampleSignalConfigData(const ExampleSignalConfigData& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        ExampleSignalConfigData& operator =(const ExampleSignalConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Example Space Vehicle Signal Aspect Manager Input Data
///
/// @details  This class provides a data structure for the Example Space Vehicle Signal Aspect input
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class ExampleSignalInputData
{
    public:
        SensorAnalogInputData             sensorHxExitT;      /**< (--) Cabin heat exchanger exit temperature sensor. */
        SensorAnalogInputData             sensorCabinP;       /**< (--) Cabin pressure sensor. */
        SensorAnalogInputData             sensorCabinT;       /**< (--) Cabin temperature sensor. */
        SensorAnalogInputData             sensorCabinPPO2;    /**< (--) Cabin partial pressure O2 sensor. */
        SensorAnalogInputData             sensorCabinPPCO2;   /**< (--) Cabin partial pressure CO2 sensor. */
        SensorAnalogInputData             sensorCabinRH;      /**< (--) Cabin relative humidity sensor. */
        SensorAnalogInputData             sensorCabinFanDp;   /**< (--) Cabin fan delta-pressure sensor. */
        SensorAnalogInputData             sensorCabinFanN;    /**< (--) Cabin fan speed sensor. */
        SensorAnalogInputData             sensorAtcsFlow;     /**< (--) ATCS flow rate sensor. */
        SensorAnalogInputData             sensorAtcsPumpDp;   /**< (--) ATCS pump delta-pressure sensor. */
        SensorAnalogInputData             sensorAtcsPumpN;    /**< (--) ATCS pump speed sensor. */
        SensorAnalogInputData             sensorAtcsPumpInT;  /**< (--) ATCS pump inlet temperature sensor. */
        SensorAnalogInputData             sensorAtcsPumpInP;  /**< (--) ATCS pump inlet pressure sensor. */
        SensorAnalogInputData             sensorAtcsRadInT;   /**< (--) ATCS radiator inlet temperature sensor. */
        SensorAnalogInputData             sensorTcvPos;       /**< (--) Cabin temperature control valve position sensor. */
        SensorAnalogInputData             sensorRadBypassPos; /**< (--) ATCS radiator bypass valve position sensor. */
        TsPidControllerInputData          cabinTempCntlr;     /**< (--) Cabin temperature controller. */
        TsPidControllerInputData          atcsTempCntlr;      /**< (--) ATCS temperature controller. */
        TsPumpMotorControllerInputData    cabinFan;           /**< (--) Cabin fan motor & controller. */
        TsPumpMotorControllerInputData    pump;               /**< (--) ATCS pump motor & controller. */
        TsPoweredValveControllerInputData gasSelectValve;     /**< (--) PCS gas select valve driver. */
        TsPoweredValveControllerInputData tcv;                /**< (--) Cabin temperature control valve driver. */
        TsPoweredValveControllerInputData radBypassValve;     /**< (--) ATCS radiator bypass valve driver. */
        SensorAnalogInputData             sensorsShellT;      /**< (--) PTCS shell temperature sensors for thermostats. */
        ThermostatInputData               fwdShellThermostat; /**< (--) PTCS forward shell thermostat. */
        ThermostatInputData               aftShellThermostat; /**< (--) PTCS aft shell thermostat. */
        /// @brief  Default constructs this Example Space Vehicle Signal Aspect input data.
        ExampleSignalInputData();
        /// @brief  Default destructs this Example Space Vehicle Signal Aspect input data.
        virtual ~ExampleSignalInputData();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        ExampleSignalInputData(const ExampleSignalInputData&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        ExampleSignalInputData& operator =(const ExampleSignalInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Example Space Vehicle Signal Aspect
///
/// @details  Refer to class Purpose.
////////////////////////////////////////////////////////////////////////////////////////////////////
class ExampleSignal
{
    TS_MAKE_SIM_COMPATIBLE(ExampleSignal);
    public:
        ExampleSignalConfigData    mConfig;            /**< *i (--) Configuration data (not checkpointed). */
        ExampleSignalInputData     mInput;             /**< *i (--) Input data (not checkpointed). */
        SensorAnalog               sensorHxExitT;      /**< (--) Cabin heat exchanger exit temperature sensor. */
        SensorAnalog               sensorCabinP;       /**< (--) Cabin pressure sensor. */
        SensorAnalog               sensorCabinT;       /**< (--) Cabin temperature sensor. */
        SensorAnalog               sensorCabinPPO2;    /**< (--) Cabin partial pressure O2 sensor. */
        SensorAnalog               sensorCabinPPCO2;   /**< (--) Cabin partial pressure CO2 sensor. */
        SensorAnalog               sensorCabinRH;      /**< (--) Cabin relative humidity sensor. */
        SensorAnalog               sensorCabinFanDp;   /**< (--) Cabin fan delta-pressure sensor. */
        SensorAnalog               sensorCabinFanN;    /**< (--) Cabin fan speed sensor. */
        SensorAnalog               sensorAtcsFlow;     /**< (--) ATCS flow rate sensor. */
        SensorAnalog               sensorAtcsPumpDp;   /**< (--) ATCS pump delta-pressure sensor. */
        SensorAnalog               sensorAtcsPumpN;    /**< (--) ATCS pump speed sensor. */
        SensorAnalog               sensorAtcsPumpInT;  /**< (--) ATCS pump inlet temperature sensor. */
        SensorAnalog               sensorAtcsPumpInP;  /**< (--) ATCS pump inlet pressure sensor. */
        SensorAnalog               sensorAtcsRadInT;   /**< (--) ATCS radiator inlet temperature sensor. */
        SensorAnalog               sensorTcvPos;       /**< (--) Cabin temperature control valve position sensor. */
        SensorAnalog               sensorRadBypassPos; /**< (--) ATCS radiator bypass valve position sensor. */
        TsPidController            cabinTempCntlr;     /**< (--) Cabin temperature controller. */
        TsPidController            atcsTempCntlr;      /**< (--) ATCS temperature controller. */
        TsPumpMotorController      cabinFan;           /**< (--) Cabin fan motor & controller. */
        TsPumpMotorController      pump;               /**< (--) ATCS pump motor & controller. */
        TsOpenCloseValveController gasSelectValve;     /**< (--) PCS gas select valve driver. */
        TsPositionValveController  tcv;                /**< (--) Cabin temperature control valve driver. */
        TsPositionValveController  radBypassValve;     /**< (--) ATCS radiator bypass valve driver. */
        Thermostat                 fwdShellThermostat; /**< (--) PTCS forward shell thermostat. */
        Thermostat                 aftShellThermostat; /**< (--) PTCS aft shell thermostat. */
        /// @brief  Default constructs this Example Space Vehicle Signal Aspect.
        ExampleSignal();
        /// @brief  Default destructs this Example Space Vehicle Signal Aspect.
        virtual ~ExampleSignal();
        /// @brief  Initializes this Example Space Vehicle Signal Aspect with configuration and input data.
        void init(const std::string& name);
        /// @brief  Updates this Example Space Vehicle Signal Aspect.
        void step(const double dt);

    protected:
        std::string                mName;              /**< (--) Model name string for messages. */
        bool                       mInitialized;       /**< (--) Initialization complete flag. */
        /// @brief    Initializes this Example Space Vehicle Signal Aspect.
        void initialize();
        /// @brief    Is this Example Space Vehicle Signal Aspect initialized?
        bool isInitialized() const;
        /// @brief    Updates this Example Space Vehicle Signal Aspect.
        void update(const double dt);

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        ExampleSignal(const ExampleSignal&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        ExampleSignal& operator =(const ExampleSignal&);
};

/// @}

#endif
