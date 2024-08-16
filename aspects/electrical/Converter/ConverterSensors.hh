#ifndef ConverterSensors_EXISTS
#define ConverterSensors_EXISTS

/**
@defgroup  TSM_GUNNS_ELECTRICAL_CONVERTER_SENSORS Converter Electrical Sensors
@ingroup   TSM_GUNNS_ELECTRICAL_CONVERTER

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE: (The Converter class is a simple single input single output conversion tool.)

 REQUIREMENTS:
 ()

 REFERENCE:
 -()

 ASSUMPTIONS AND LIMITATIONS:
 -()

 LIBRARY DEPENDENCY:
 ((ConverterSensors.o))

 PROGRAMMERS:
 ((Chetan Patel) (JETS) (May 2013) (Initial prototype)
  (Sheena Judson Miller) (JETS) (July 2013) (Added Sensor functionality)
 */

#include <string>
#include "Converter.hh"
#include "common/sensors/SensorAnalog.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////
/// @brief ConverterSensors Configuration Data
////////////////////////////////////////////////////////////////////////////////
class ConverterSensorsConfigData : public ConverterConfigData {
 public:
    SensorAnalogConfigData outVoltageSensor;  /**< (--) trick_chkpnt_io(**) Voltage sensor config data */
    SensorAnalogConfigData outCurrentSensor;  /**< (--) trick_chkpnt_io(**) Current sensor config data */

    /// @brief Default constructor for ConverterSensors Configuration Data.
    ConverterSensorsConfigData(
            const double                  standbyPower      =  0.0,
            const SensorAnalogConfigData& outVoltageSensor  =  SensorAnalogConfigData(),
            const SensorAnalogConfigData& outCurrentSensor  =  SensorAnalogConfigData());

    /// @brief Copy constructor for ConverterSensors Configuration Data.
    ConverterSensorsConfigData(const ConverterSensorsConfigData& that);

    /// @brief Default destructor for ConverterSensors Configuration Data.
    virtual ~ConverterSensorsConfigData();

    /// @brief Assignment operator for ConverterSensors Configuration Data.
    ConverterSensorsConfigData& operator =(const ConverterSensorsConfigData&);
};


////////////////////////////////////////////////////////////////////////////////
/// @brief ConverterSensors Input Data
////////////////////////////////////////////////////////////////////////////////
class ConverterSensorsInputData : public ConverterInputData {
 public:
    double mOutputPower;      /**< (W)    trick_chkpnt_io(**) Output power supplied by the converter */
    SensorAnalogInputData outVoltageSensor; /**< (--)    trick_chkpnt_io(**) Voltage Sensor Input Data */
    SensorAnalogInputData outCurrentSensor; /**< (--)    trick_chkpnt_io(**) Current Sensor Input Data */

    /// @brief Default constructor for ConverterSensors Input Data.
    ConverterSensorsInputData(const double inputVoltage              =   0.0,
                          const double outputPower                       =   0.0,
                          const double regulatedVoltage                  =   0.0,
                          const double efficiency                        =   0.0,
                          const double outOverCurrentLimit               =   0.0,
                          const double outOverVoltageLimit               =   0.0,
                          const bool   outOverCurrentTripActive          = false,
                          const bool   outOverVoltageTripActive          = false,
                          const double inOverVoltageLimit                =   0.0,
                          const double inUnderVoltageLimit               =   0.0,
                          const bool   inOverCurrentTripActive           = false,
                          const bool   inUnderCurrentTripActive          = false,
                          const SensorAnalogInputData& outVoltageSensor  =  SensorAnalogInputData(),
                          const SensorAnalogInputData& outCurrentSensor  =  SensorAnalogInputData());
    /// @brief Copy Constructor for ConverterSensors Input Data.
    ConverterSensorsInputData(const ConverterSensorsInputData& that);
    /// @brief Default destructor for ConverterSensors Input Data.
    virtual ~ConverterSensorsInputData();
    /// @brief Assignment operator for ConverterSensors Input Data.
    ConverterSensorsInputData& operator =(const ConverterSensorsInputData&);
};

////////////////////////////////////////////////////////////////////////////////
/// @brief his class provides a simple converter with the following
/// capabilities: health status flags, on/off cmd, disable output cmd,
/// generated heat, and single input to single output voltage conversion.
////////////////////////////////////////////////////////////////////////////////
class ConverterSensors : public Converter {
    TS_MAKE_SIM_COMPATIBLE(ConverterSensors);
 public:
    SensorAnalog outVoltageSensor;              /**< (--) Instance of a sensor for output voltage */
    SensorAnalog outCurrentSensor;              /**< (--) Instance of a sensor for output current */

    bool   mMalfOverrideOutOverCurrentLimit;    /**< (--) Override out over current */
    double mMalfOverrideOutOverCurrentValue;    /**< (amp) New value for  out over current */
    bool   mMalfOverrideOutOverVoltageLimit;    /**< (--) Override hardware out over voltage */
    double mMalfOverrideOutOverVoltageValue;    /**< (V) New value for hardware out over voltage */
    bool   mMalfOverrideOutVoltage;             /**< (--) Override out voltage of the converter */
    double mMalfOverrideOutVoltageValue;        /**< (V) New value for converter's out voltage */
    bool   mMalfConverterFailed;                /**< (--) Malfunction for Converter failure */

    /// @brief ConverterSensors constructor.
    ConverterSensors();

    /// @brief ConverterSensors destructor.
    virtual ~ConverterSensors();

    /// @brief Method to initialize ConverterSensors data.
    void initialize(const ConverterSensorsConfigData&   configData,
                    const ConverterSensorsInputData&    inputData);

    /// @brief Method to update converter behavior.
    virtual void step(double timeStep);

    void cmdConverterOn(const bool converterOn);

    /// @brief Method to set converter to be on/off.
    void setConverterOn(const bool converterOn);

    /// @brief Method to set output power supplied by the converter.
    void setLoad(const double load);

    /// @brief Method to set trips
    void setTrips();

    /// @brief Method to fail the converter;
    void setConverterFailed(const bool failConverter);

    /// @brief Method to get output current of the converter.
    double getSensedOutputCurrent() const;

    /// @brief Method to get output voltage of the converter.
    double getSensedOutputVoltage() const;

    /// @brief Method to get the heat dissipated by conversion process
    double getHeatLoss() const ;

    /// @brief Method to check if converter is commanded on.
    bool isConverterCmdOn() const;

    /// @brief Method to check if converter has tripped.
    bool isTripped() const;

    /// @brief Method to check if converter failure has occurred.
    bool isConverterFailed() const;

    /// @brief    Returns this  initialization complete flag.
    bool isInitialized() const;

 protected:
    /// @brief Method to validate configuration and input data to the converter.
    virtual void validate();

    /// @brief Method to handle override malfunctions.
    void handleOverrideMalfs();

    /// @brief Method to handle override malfunctions.
    void handleConverterFailMalf();

    /// @brief Method to update converter state.
    void updateConverterState();

    /// @brief Method to handle inhibit trip events to output voltage and current
    void handleTrips();

    /// @brief Method to Update output sensor values
    void updateSensors(const double dt);

    /// @brief Method to check if converter has failed.
    bool isFailed() const;


 protected:
    std::string outVoltageSensorName;           /**< (--) Name of the output voltage sensor */
    std::string outCurrentSensorName;           /**< (--) Name of the output current sensor */


    double mDefaultOutOverCurrentLimit;         /**< (amp) Default out over current limit */
    double mDefaultOutOverVoltageLimit;         /**< (V) Default output over voltage limit */
    double mDefaultOutVoltage;                  /**< (V) Default output voltage */

    bool   mConverterFailed;                    /**< (--)  Converter failed flag */

    bool   mConverterOnCmd;                     /**< (--)  Converter's on/off command */
    bool   mPrevConverterOnCmd;                 /**< (--)  Previous converter's on/off command */

    double mOutputCurrentSensed;                /**< (amp) Output current coming from sensor */
    double mOutputVoltageSensed;                /**< (V)     Output Voltage coming from sensor */

    bool  mInitFlag;                            /**< (--) Initialize flag */

 private:
     /// @brief Copy constructor unavailable since declared private and not implemented.
    ConverterSensors(const ConverterSensors& that);

    /// @details  Assignment operator unavailable since declared private and not implemented.
    ConverterSensors& operator =(const ConverterSensors& that);
};

/// @}
////////////////////////////////////////////////////////////////////////////////
/// @return   bool  (--)  True if initialization completed successfully, false otherwise.
///
/// @details  Gets the initialization complete flag
////////////////////////////////////////////////////////////////////////////////
inline bool ConverterSensors::isInitialized() const
{
    return mInitFlag;
}

////////////////////////////////////////////////////////////////////////////////
/// @details  Sets converter on/off command
////////////////////////////////////////////////////////////////////////////////
inline void ConverterSensors::setConverterOn(const bool converterOn) {
    mConverterOnCmd = converterOn;
}

////////////////////////////////////////////////////////////////////////////////
/// @details  Sets output load powered by this converter
////////////////////////////////////////////////////////////////////////////////
inline void ConverterSensors::setLoad(const double load) {
    mOutputPower = load;
}

////////////////////////////////////////////////////////////////////////////////
/// @details  Method to turn converter off depending on trips and on/off command.
////////////////////////////////////////////////////////////////////////////////
inline void ConverterSensors::updateConverterState() {
    mConverterOn = not(mOutputOverCurrentTrip || mOutputOverVoltageTrip || mConverterFailed) and
                   mConverterOnCmd;
}
////////////////////////////////////////////////////////////////////////////////
/// @details  Sets the convertFailed to true/false.
////////////////////////////////////////////////////////////////////////////////
inline void ConverterSensors::setConverterFailed(const bool failConverter) {
    mConverterFailed = failConverter;
}


////////////////////////////////////////////////////////////////////////////////
/// @details  Method to determine if converter is commanded on.
///
/// @return   bool (--) mConverterOnCmd
////////////////////////////////////////////////////////////////////////////////
inline bool ConverterSensors::isConverterCmdOn() const {
    return mConverterOnCmd;
}

////////////////////////////////////////////////////////////////////////////////
/// @details  Method to get converter failure status
///
/// @return   bool (--) mConverterFailed
////////////////////////////////////////////////////////////////////////////////
inline bool ConverterSensors::isConverterFailed() const {
    return mConverterFailed;
}

////////////////////////////////////////////////////////////////////////////////
/// @details  Method to determine if a converter trip has occurred.
///
/// @return bool (--) True if converter tripped, false otherwise.
////////////////////////////////////////////////////////////////////////////////
inline bool ConverterSensors::isTripped() const {
    return (mOutputOverCurrentTrip|| mOutputOverVoltageTrip);
}

////////////////////////////////////////////////////////////////////////////////
/// @details  Method to get output current of the converter.
///
/// @return   double (amp) mOutputCurrent
////////////////////////////////////////////////////////////////////////////////
inline double ConverterSensors::getSensedOutputCurrent() const {
    return mOutputCurrentSensed;
}

////////////////////////////////////////////////////////////////////////////////
/// @details  Method to get output voltage of the converter.
///
/// @return   double (V) mOutputVoltage
////////////////////////////////////////////////////////////////////////////////
inline double ConverterSensors::getSensedOutputVoltage() const {
    return mOutputVoltageSensed;
}

////////////////////////////////////////////////////////////////////////////////
/// @details  Method to calculate head dissipated by conversion process
///
/// @return   double (W) mHeatDissipations
////////////////////////////////////////////////////////////////////////////////
inline double ConverterSensors::getHeatLoss() const {
    return mHeatDissipation;
}

#endif /* ConverterSensors_EXISTS */
