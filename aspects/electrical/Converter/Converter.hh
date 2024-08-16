#ifndef Converter_EXISTS
#define Converter_EXISTS

/**
@defgroup TSM_GUNNS_ELECTRICAL_CONVERTER_UTILITY  Converter Electrical Utility
@ingroup  TSM_GUNNS_ELECTRICAL_CONVERTER

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   ((The Converter class is a simple single input single output conversion tool
which can be used as a stand alone object or one included in another class.)

@details

(It provides basic health and status monitoring, input/output over voltage
   protection, output over current protection, and input under voltage
   protection. It can be configured to also inhibit trip protection functionality.))

REQUIREMENTS:
-(Users of the converter must supply an input voltage, and an output current
 for use during conversion.)

REFERENCE:
-()

ASSUMPTIONS AND LIMITATIONS:
-()

LIBRARY DEPENDENCY:
-((Converter.o))

 PROGRAMMERS:
-((Chetan Patel) (ESCG) (January 2012) (Added inhibit functionality)
  (Nick Kaufmann) (L-3) (May 2011) (Initial Prototype))
  (Mark Wiederholt) (Lockheed Martin) (March 2014) (added additional trip checks. When a
 * trip is active, inhbiting that trip and another current trip is active gives a wrong status. Trying to
 * use one 1 trip check doesnt work.))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////
/// @brief Converter Configuration Data
////////////////////////////////////////////////////////////////////////////////
class ConverterConfigData {
 public:
     double mStandbyPower;  /**< (W) trick_chkpnt_io(**) Power draw when converter is on with no load */

    /// @brief Converter Configuration Data constructor.
    explicit ConverterConfigData(const double mStandbyPower = 0.0);

    /// @brief Converter Configuration Data destructor.
    virtual ~ConverterConfigData();

    /// @brief Converter Configuration Data copy constructor.
    ConverterConfigData(const ConverterConfigData& that);

    /// @brief Converter Configuration assignment operator
    ConverterConfigData& operator =(const ConverterConfigData&);
};


////////////////////////////////////////////////////////////////////////////////
/// @brief Converter Input Data
////////////////////////////////////////////////////////////////////////////////
class ConverterInputData {
 public:

    double mInputVoltage;                /**< (V)     trick_chkpnt_io(**) Input voltage to the converter */
    double mRegulatedVoltage;            /**< (V)     trick_chkpnt_io(**) Regulated voltage coming out from the converter */
    double mEfficiency;                  /**< (--)    trick_chkpnt_io(**) Efficiency of the converter conversion process */
    double mOutputOverCurrentLimit;      /**< (amp)   trick_chkpnt_io(**) Output over current limit of the converter */
    double mOutputOverVoltageLimit;      /**< (V)     trick_chkpnt_io(**) Output over voltage limit of the converter */
    bool mOutOverCurrentTripActive;      /**< (--)    trick_chkpnt_io(**) Output over current trip active flag */
    bool mOutOverVoltageTripActive;      /**< (--)    trick_chkpnt_io(**) Output over voltage trip active flag */
    double mInputOverVoltageLimit;       /**< (V)     trick_chkpnt_io(**) Input over voltage limit of the converter */
    double mInputUnderVoltageLimit;      /**< (V)     trick_chkpnt_io(**) Input under voltage limit of the converter */
    bool mInOverVoltageTripActive;       /**< (--)    trick_chkpnt_io(**) Input over voltage trip active flag */
    bool mInUnderVoltageTripActive;      /**< (--)    trick_chkpnt_io(**) Input under voltage trip active flag */
    double mOutputOverCurrentFastLimit;  /**< (amp)   trick_chkpnt_io(**) Output over current fast limit of the converter */
    bool mOutOverCurrentFastTripActive;  /**< (--)    trick_chkpnt_io(**) Output over current fast trip active flag */
    double mInputOverCurrentLimit;    /**< (amp)      trick_chkpnt_io(**) Output over current hardware limit of the converter */
    bool mInputOverCurrentTripActive;  /**< (--)      trick_chkpnt_io(**) Input over current trip active flag */

    /// @brief Converter Input Data constructor
    ConverterInputData(const double mInputVoltage            =    0.0,
                       const double mRegulatedVoltage        =    0.0,
                       const double mEfficiency              =    0.0,
                       const double mOutputOverCurrentLimit  =    0.0,
                       const double mOutputOverVoltageLimit  =    0.0,
                       const bool mOutOverCurrentTripActive  =  false,
                       const bool mOutOverVoltageTripActive  =  false,
                       const double mInputOverVoltageLimit   =    0.0,
                       const double mInputUnderVoltageLimit  =    0.0,
                       const bool mInOverVoltageTripActive   =  false,
                       const bool mInUnderVoltageTripActive  =  false,
                       const double mOutputOverCurrentFastLimit = 0.0,
                       const bool mOutOverCurrentFastTripActive = false,
                       const  double mInputOverCurrentLimit = 0.0,
                       const bool mInputOverCurrentTripActive = false
                       );

    /// @brief Converter Input Data destructor
    virtual ~ConverterInputData();

    /// @brief Converter Input Data copy constructor
    ConverterInputData(const ConverterInputData& that);

    /// @brief Converter Input Data assignment operator
    ConverterInputData& operator =(const ConverterInputData&);
};

////////////////////////////////////////////////////////////////////////////////
/// @brief This class provides a simple converter with the following
/// capabilities: health status flags, on/off cmd, disable output cmd,
/// generated heat, and single input to single output voltage conversion.
////////////////////////////////////////////////////////////////////////////////
class Converter {
    TS_MAKE_SIM_COMPATIBLE(Converter);
 public:

    /// @brief Converter constructor
    Converter();

    /// @brief Converter destructor.
    virtual ~Converter();

    /// @brief Initialize converter utility with configuration and input data and return init flag
    /// (true if successful initializing).
    bool initialize(
            const ConverterConfigData* configDataIn,
            const ConverterInputData* inputDataIn);

    /// @brief Update health and status flags using latest values, then check if converter is
    /// enabled, is receiving valid voltage, and is not tripped.
    virtual void update();

    /// @brief Method to handle input trips
    virtual void handleInputTrips(const double inVoltage, const double inCurrent);

    /// @brief Method to handle output trips
    virtual void handleOutputTrips(const double outVoltage, const double outCurrent);

    /// @brief Method to calculate power used by converter and also checks for output trips
    virtual void doConversion(const double outCurrent);

    /// @brief Method to reset all trips if any of the trip occured
    virtual void resetTrips();

    /// @brief Method to set reset trip functionality
    void setResetTrips(const bool state);

    /// @brief Method to enable or disable converter.
    void setConverterState(const bool state);

    /// @brief Method to set input voltage to the converter
    void setInputVoltage(const double voltage);

    /// @brief Method to set input voltage to the converter
    void setInputPower(const double power);

    /// @brief Method to set the regulated voltage of the converter.
    void setRegulatedVoltage(const double voltage);

    /// @brief Method to set the efficiency of conversion process.
    void setEfficiency(const double eff);

    /// @brief Method to set the efficiency of conversion process.
    double getEfficiency() const;

    /// @brief Method to set output over current trip state of the converter.
    void setOutputOverCurrentTrip(const bool flag);

    /// @brief Method to set output over current limit of the converter.
    void setOutputOverCurrentLimit(const double current);


    /// @brief Method to set output over current trip active function (false bypasses trip event).
    void setOutOverCurrentTripActive(const bool flag);

    /// @brief Method to get status of output over current trip event.
    bool isOutputOverCurrentTrip() const;

    /// @brief Method to get the output over current limit of the converter.
    double getOutputOverCurrentLimit() const;



      /// @brief Method to set output over current trip state of the converter.
    void setOutputOverCurrentFastTrip(const bool flag);

    /// @brief Method to set output over current limit of the converter.
    void setOutputOverCurrentFastLimit(const double current);


    /// @brief Method to set output over current trip active function (false bypasses trip event).
    void setOutOverCurrentFastTripActive(const bool flag);

    /// @brief Method to get status of output over current trip event.
    bool isOutputOverCurrentFastTrip() const;

    /// @brief Method to get the output over current limit of the converter.
    double getOutputOverCurrentFastLimit() const;

        /// @brief Method to set output over current trip state of the converter.
    void setInputOverCurrentTrip(const bool flag);

    /// @brief Method to set output over current limit of the converter.
    void setInputOverCurrentLimit(const double current);

     /// @brief Method to set input over current trip active function (false bypasses trip event).
    void setInputOverCurrentTripActive(const bool flag);

    /// @brief Method to get status of output over current trip event.
    bool isInputOverCurrentTrip() const;

    /// @brief Method to get the output over current limit of the converter.
    double getInputOverCurrentLimit() const;

    /// @brief Method to set output over voltage limit of the converter.
    void setOutputOverVoltageLimit(const double voltage);


    /// @brief Method to set flag to trip on sensed value or truth value.
    /// (true bypasses truth value and uses sensed value). The sensed value need to be provided
    /// in order to trip on sensed values.By default, the converter will trip on truth value of
    /// the model itself.
    void setTripOnSensedValue(const bool flag);

    /// @brief Method to get input current of the converter.
    double getInputCurrent() const;

    /// @brief Method to get input voltage of the converter.
    double getInputVoltage() const;

    /// @brief Method to get output current of the converter.
    double getOutputCurrent() const;

    /// @brief Method to get output voltage of the converter.
    double getOutputVoltage() const;

    /// @brief Method to get input power used by the converter.
    double getInputPower() const;

    /// @brief Method to get the output power supplied by the converter.
    double getOutputPower() const;

    /// @brief Method to get the output over voltage limit of the converter.
    double getOutputOverVoltageLimit() const;

    /// @brief Method to get the input under voltage limit of the converter.
    double getInputUnderVoltageLimit() const;

    /// @brief Method to get status of the converter (On/Off).
    bool isConverterOn() const;

    /// @brief Method to get status of output over voltage trip event.
    bool isOutputOverVoltageTrip() const;

    /// @brief Method to get status of input over voltage trip event.
    bool isInputOverVoltageTrip() const;

    /// @brief Method to get status of input under voltage trip event.
    bool isInputUnderVoltageTrip() const;

    /// @brief Method to get status if converter will trip on sensed or truth value.
    /// By default, converter will trip based on truth value (false)
    bool isTripOnSensedValue() const;

    /// @brief Method to reset the state of the converter due to over current trip.
    void resetStateDueToOverCurrentTrip();

    /// @brief Method to set power output bias.
    void setBiasOutputPower(const double value);

 protected:

    double mStandbyPower;                 /**< (W)   trick_chkpnt_io(**) Power drawn when converter is on with no load attached */
    double mInputVoltage;                 /**< (V)   Input voltage to the converter */
    double mRegulatedVoltage;             /**< (V)   Regulated output voltage of the converter */
    double mEfficiency;                   /**< (--)  Efficiency of the converter's conversion process */
    double mOutputOverCurrentLimit;       /**< (amp) Output over current limit for the converter */
    double mOutputOverVoltageLimit;       /**< (V)   Output over voltage limit for the converter */
    bool mOutOverCurrentTripActive;       /**< (--)  Output over current trip active flag */
    bool mOutOverVoltageTripActive;       /**< (--)  Output over voltage trip active flag */
    double mInputOverVoltageLimit;        /**< (V)   Input over voltage limit for the converter */
    double mInputUnderVoltageLimit;       /**< (V)   Input under voltage limit for the converter */
    bool mInOverVoltageTripActive;        /**< (--)  Input over voltage trip active flag */
    bool mInUnderVoltageTripActive;       /**< (--)  Input under voltage trip active flag */
    bool mOutputOverCurrentTrip;          /**< (--)  Output over current trip flag */
    bool mOutputOverVoltageTrip;          /**< (--)  Output over voltage trip flag */
    bool mInputOverVoltageTrip;           /**< (--)  Input over voltage trip flag  */
    bool mInputUnderVoltageTrip;          /**< (--)  Input under voltage trip flag */
    bool mResetTrips;                     /**< (--)  Clears all trips */
    bool mTripOnSensedValue;              /**< (--)  Trip on sensed value instead of truth value */
    bool mConverterOn;                    /**< (--)  Converter's on/off flag */
    bool mBadEfficiency;                  /**< (--)  Efficiency is out of range (0 <= eff <= 1) */
    double mInputCurrent;                 /**< (amp) Input current to the converter */
    double mOutputCurrent;                /**< (amp) Output current to the converter */
    double mOutputVoltage;                /**< (V)   Output voltage of the converter */
    double mInputPower;                   /**< (W)   Input power consumed by the converter */
    double mOutputPower;                  /**< (W)   Output power supplied by the converter */
    double mHeatDissipation;              /**< (W)   Heat dissipated by the converter's conversion process */
    double mOutputOverCurrentFastLimit;   /**< (amp)  Output over current fast limit of the converter */
    bool mOutOverCurrentFastTripActive;   /**< (--)   Output over current fast trip active flag */
    bool mOutputOverCurrentFastTrip;      /**< (--)  Output over current Fast trip flag */
    double mInputOverCurrentLimit;        /**< (amp)  Input over current HW limit of the converter */
    bool mInputOverCurrentTrip;           /**< (--)  Input over current HW trip flag */
    bool mInputOverCurrentTripActive;     /**< (--)  Input over current trip active flag */
    double mThermalTuningFactor;          /**< (--)  multiplier for tuning the effects of extra heat added from downstream load */
    double mBiasOutputPower;              /**< (W)   amount to add or subtract to the output power */

    /// @brief Method to validate configuration and input data to the converter.
    virtual void validate();

    /// @brief Method to calculate heat dissipated by the conversion process.
    virtual void calculateHeatDissipation();

    /// @brief Method to update trip and converter status.
    virtual void updateHealthStatus();
};

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @brief Method to reset all trips if any of the trip occurred
////////////////////////////////////////////////////////////////////////////////
inline void Converter::resetTrips() {
    mOutputOverCurrentTrip = false;
    mOutputOverCurrentFastTrip = false;
    mInputOverCurrentTrip = false;
    mOutputOverVoltageTrip = false;
    mInputOverVoltageTrip = false;
    mInputUnderVoltageTrip = false;
    mResetTrips    = false;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Method to set reset trip functionality
////////////////////////////////////////////////////////////////////////////////
inline void Converter::setResetTrips(const bool state) {
    mResetTrips = state;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Method to enable or disable converter.
////////////////////////////////////////////////////////////////////////////////
inline void Converter::setConverterState(const bool state) {
    mConverterOn = state;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Method to set input voltage to the converter
////////////////////////////////////////////////////////////////////////////////
inline void Converter::setInputVoltage(const double voltage) {
    mInputVoltage = voltage;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Method to set input voltage to the converter
////////////////////////////////////////////////////////////////////////////////
inline void Converter::setInputPower(const double power) {
    mInputPower = power;
}

////////////////////////////////////////////////////////////////////////////////
/// @param[in] voltage (V)  Voltage to which converter regulate its output to.
/// -
/// @brief Method to set regulated voltage of the converter.
////////////////////////////////////////////////////////////////////////////////
inline void Converter::setRegulatedVoltage(const double voltage) {
    mRegulatedVoltage = voltage;
}

////////////////////////////////////////////////////////////////////////////////
/// @param[in] eff (--) Efficiency of the converter's conversion process.
/// -
/// @brief Method to set the efficiency of conversion process
////////////////////////////////////////////////////////////////////////////////
inline double Converter::getEfficiency() const {
    return mEfficiency;
}

////////////////////////////////////////////////////////////////////////////////
/// @param[in] eff (--) Efficiency of the converter's conversion process.
/// -
/// @brief Method to set the efficiency of conversion process
////////////////////////////////////////////////////////////////////////////////
inline void Converter::setEfficiency(const double eff) {
    mEfficiency = eff;
}

////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag (--) state of output over current trip
/// -
/// @brief Method to set the output over current trip of the converter
////////////////////////////////////////////////////////////////////////////////
inline void Converter::setOutputOverCurrentTrip(const bool flag) {
    mOutputOverCurrentTrip = flag;
}

////////////////////////////////////////////////////////////////////////////////
/// @param[in]    current    (amp)    Output over current limit
/// -
////////////////////////////////////////////////////////////////////////////////
inline void Converter::setOutputOverCurrentLimit(const double current) {
        mOutputOverCurrentLimit = current;
}

////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag (--) state of output over current fast trip
/// -
/// @brief Method to set the output over current fast trip of the converter
////////////////////////////////////////////////////////////////////////////////
inline void Converter::setOutputOverCurrentFastTrip(const bool flag) {
    mOutputOverCurrentFastTrip = flag;
}

////////////////////////////////////////////////////////////////////////////////
/// @param[in]    current    (amp)    Output over current fast limit
/// -
////////////////////////////////////////////////////////////////////////////////
inline void Converter::setOutputOverCurrentFastLimit(const double current) {
        mOutputOverCurrentFastLimit = current;
}

////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag (--) state of output over current Hw trip
/// -
/// @brief Method to set the output over current Hw trip of the converter
////////////////////////////////////////////////////////////////////////////////
inline void Converter::setInputOverCurrentTrip(const bool flag) {
    mInputOverCurrentTrip = flag;
}

////////////////////////////////////////////////////////////////////////////////
/// @param[in]    current    (amp)    Output over current Hw limit
/// -
////////////////////////////////////////////////////////////////////////////////
inline void Converter::setInputOverCurrentLimit(const double current) {
        mInputOverCurrentLimit = current;
}

////////////////////////////////////////////////////////////////////////////////
/// @param[in]    flag    (--)    Flag to inhibit output over current trip event
/// -
/// @brief Method
////////////////////////////////////////////////////////////////////////////////
inline void Converter::setInputOverCurrentTripActive(const bool flag) {
    mInputOverCurrentTripActive = flag;
}

////////////////////////////////////////////////////////////////////////////////
/// @param[in]    voltage    (V)    Output over voltage limit
/// -
/// @brief Method
////////////////////////////////////////////////////////////////////////////////
inline void Converter::setOutputOverVoltageLimit(const double voltage) {
    mOutputOverVoltageLimit = voltage;
}


////////////////////////////////////////////////////////////////////////////////
/// @param[in]    flag    (--)    Flag to inhibit output over current trip event
/// -
/// @brief Method
////////////////////////////////////////////////////////////////////////////////
inline void Converter::setOutOverCurrentTripActive(const bool flag) {
    mOutOverCurrentTripActive = flag;
}


////////////////////////////////////////////////////////////////////////////////
/// @param[in]    flag    (--)    Flag to inhibit output over current trip event
/// -
/// @brief Method
////////////////////////////////////////////////////////////////////////////////
inline void Converter::setOutOverCurrentFastTripActive(const bool flag) {
    mOutOverCurrentFastTripActive = flag;
}


////////////////////////////////////////////////////////////////////////////////
/// @param[in]    flag    (--)    Flag to trip on sensed or truth values
/// -
/// @brief Method
////////////////////////////////////////////////////////////////////////////////
inline void Converter::setTripOnSensedValue(const bool flag) {
    mTripOnSensedValue = flag;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Method to get input current of the converter.
/// -
/// @return    double mInputCurrent
////////////////////////////////////////////////////////////////////////////////
inline double Converter::getInputCurrent() const {
    return mInputCurrent;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Method to get input voltage of the converter.
/// -
/// @return    double mInputVoltage
////////////////////////////////////////////////////////////////////////////////
inline double Converter::getInputVoltage() const {
    return mInputVoltage;
}

////////////////////////////////////////////////////////////////////////////////
///// @brief Method to get output current of the converter.
///// -
///// @return    double mOutputCurrent
//////////////////////////////////////////////////////////////////////////////////
inline double Converter::getOutputCurrent() const {
    return mOutputCurrent;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Method to get output voltage of the converter.
/// -
/// @return    double mOutputVoltage
////////////////////////////////////////////////////////////////////////////////
inline double Converter::getOutputVoltage() const {
    return mOutputVoltage;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief Method to get input power used by the converter.
/// -
/// @return double mInputPower
////////////////////////////////////////////////////////////////////////////////
inline double Converter::getInputPower() const {
    return mInputPower;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Method to get the output power supplied by the converter.
/// -
/// @return double mOutputPower
////////////////////////////////////////////////////////////////////////////////
inline double Converter::getOutputPower() const {
    return mOutputPower;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Method to get the output over current limit of the converter.
/// -
/// @return double mOutputOverCurrentLimit
////////////////////////////////////////////////////////////////////////////////
inline double Converter::getOutputOverCurrentLimit() const {
    return mOutputOverCurrentLimit;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Method to get the output over current fast limit of the converter.
/// -
/// @return double mOutputOverCurrentFastLimit
////////////////////////////////////////////////////////////////////////////////
inline double Converter::getOutputOverCurrentFastLimit() const {
    return mOutputOverCurrentFastLimit;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief Method to get the output over current fast limit of the converter.
/// -
/// @return double mOutputOverCurrentFastLimit
////////////////////////////////////////////////////////////////////////////////
inline double Converter::getInputOverCurrentLimit() const {
    return mInputOverCurrentLimit;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Method to get the output over voltage limit of the converter.
/// -
/// @return double mOutputOverVoltageLimit
////////////////////////////////////////////////////////////////////////////////
inline double Converter::getOutputOverVoltageLimit() const {
    return mOutputOverVoltageLimit;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Method to get the input under voltage limit of the converter.
/// -
/// @return double mOutputOverVoltageLimit
////////////////////////////////////////////////////////////////////////////////
inline double Converter::getInputUnderVoltageLimit() const {
    return mInputUnderVoltageLimit;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Method to get status of the converter (On/Off).
/// -
/// @return bool mConverterOn
////////////////////////////////////////////////////////////////////////////////
inline bool Converter::isConverterOn() const {
    return mConverterOn;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Method to get status of output over current trip event.
/// -
/// @return bool mOutputOverCurrentTrip
////////////////////////////////////////////////////////////////////////////////
inline bool Converter::isOutputOverCurrentTrip() const {
    return mOutputOverCurrentTrip;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Method to get status of output over current fast trip event.
/// -
/// @return bool mOutputOverCurrentTrip
////////////////////////////////////////////////////////////////////////////////
inline bool Converter::isOutputOverCurrentFastTrip() const {
    return mOutputOverCurrentFastTrip;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Method to get status of output over current Hw trip event.
/// -
/// @return bool mOutputOverCurrentTrip
////////////////////////////////////////////////////////////////////////////////
inline bool Converter::isInputOverCurrentTrip() const {
    return mInputOverCurrentTrip;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Method to get status of output over voltage trip event.
/// -
/// @return bool mOutputOverVoltageTrip
////////////////////////////////////////////////////////////////////////////////
inline bool Converter::isOutputOverVoltageTrip() const {
    return mOutputOverVoltageTrip;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Method to get status of input over voltage trip event.
/// -
/// @return bool mInputOverVoltageTrip
////////////////////////////////////////////////////////////////////////////////
inline bool Converter::isInputOverVoltageTrip() const {
    return mInputOverVoltageTrip;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Method to get status of input under voltage trip event.
/// -
/// @return bool mInputUnderVoltageTrip
////////////////////////////////////////////////////////////////////////////////
inline bool Converter::isInputUnderVoltageTrip() const {
    return mInputUnderVoltageTrip;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Method to get status if converter will trip on sensed or truth value.
/// By default, converter will trip based on truth value (false)
/// -
/// @return bool mTripOnSensedValue
////////////////////////////////////////////////////////////////////////////////
inline bool Converter::isTripOnSensedValue() const {
    return mTripOnSensedValue;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief Method to reset the state of the converter when a over current trip cannot occur
///        at this time for the model using this converter.
///
/// @return void
////////////////////////////////////////////////////////////////////////////////
inline void Converter::resetStateDueToOverCurrentTrip() {
    mOutputOverCurrentTrip = false;
    mOutputOverCurrentFastTrip = false;
    mInputOverCurrentTrip = false;
    mConverterOn = true;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Method to get status if converter's efficiency is out of range.
/// -
/// @return bool mBadEfficiency
////////////////////////////////////////////////////////////////////////////////
//inline bool Converter::isBadEfficiency() const {
//    return mBadEfficiency;
//}

#endif /* CONVERTER_HH_ */
