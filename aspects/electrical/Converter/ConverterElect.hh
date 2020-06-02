#ifndef ConverterElect_EXISTS
#define ConverterElect_EXISTS

/**
@defgroup TSM_GUNNS_ELECTRICAL_CONVERTER_LINK  Converter Electrical Link
@ingroup  TSM_GUNNS_ELECTRICAL_CONVERTER

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE: ((Converter Electrical Link Model to feed power to loads connected to its output.)

@details

(Converter Link is modeled as a four port GUNNS link with two ports on the input side and two ports on the output side.
 One input and one output port are grounded. Converter will have one input feed and one output which connects to loads downstream.
 Configuration data allows to set the Output conductance when converter is On, and the converter Off
 conductance during initialization.
 The Converter utility handles converting the voltage source to the desired output voltage, and has Input Under Voltage,
 Input Over Voltage, Output Over Current, and Output Over Voltage trips. These trips can be inhibited to bypass the trip logic.))

REQUIREMENTS:
-()

REFERENCE:
-()

ASSUMPTIONS AND LIMITATIONS:
-(Used in a GUNNS network)

LIBRARY DEPENDENCY:
- ((ConverterElect.o))

PROGRAMMERS:
- ((Shailaja Janapati) (L-3 Comm) (May 2013) (Final version))
@{
*/

#include "Converter.hh"
#include "EpsTripMgr.hh"
#include "common/sensors/SensorAnalog.hh"
#include "core/GunnsBasicLink.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    ConverterElect Configuration Data
///
///
/// @details  The sole purpose of this class is to provide a data structure for this model
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class ConverterElectConfigData: public GunnsBasicLinkConfigData {
 public:
    const SensorAnalogConfigData *mOutVoltageSensorConfig;  /**< (--) trick_chkpnt_io(**) Out voltage sensor config data */
    const SensorAnalogConfigData *mOutCurrentSensorConfig;  /**< (--) trick_chkpnt_io(**) Out current sensor config data */
    double mOutputConductance;                              /**< (1/ohm) trick_chkpnt_io(**) Converter link's on condunctance */
    double mConverterOffConductance;                        /**< (1/ohm) trick_chkpnt_io(**) Converter link's off conductance */
    int mTripPriority;                                      /**< (--) trick_chkpnt_io(**) priority to trip this link compared to the downstream link, should be > 2 */
    ConverterConfigData mConverterConfig;                   /**< (--) trick_chkpnt_io(**) Configuration Data to be passed to for converter. */

    /// @brief ConverterElect Configuration Data constructor.
    ConverterElectConfigData(const std::string& name                         = "",
                        GunnsNodeList* nodes                                 = 0,
                        const SensorAnalogConfigData* outVoltageSensorConfig = 0,
                        const SensorAnalogConfigData* outCurrentSensorConfig = 0,
                        const double outputConductance                       = 0.0,
                        const double converterOffConductance                 = 0.0,
                        const int    tripPriority                            = 0,
                        const double standbyPower                            = 0.0);

    /// @brief ConverterElect Configuration Data copy constructor.
    ConverterElectConfigData(const ConverterElectConfigData& that);

    /// @brief ConverterElect Configuration Data destructor.
    virtual ~ConverterElectConfigData();

 private:
    /// @details  Assignment operator unavailable since declared private and not implemented.
    ConverterElectConfigData& operator =(const ConverterElectConfigData& );
};


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    ConverterElect Input Data Input Data.
///
///
/// @details  The sole purpose of this class is to provide a data structure for the this model
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class ConverterElectInputData: public GunnsBasicLinkInputData {
 public:

    const SensorAnalogInputData *mOutVoltageSensorInput;    /**< (--)  trick_chkpnt_io(**) Sensor Input Data */
    const SensorAnalogInputData *mOutCurrentSensorInput;    /**< (--)  trick_chkpnt_io(**) Sensor Input Data */
    bool mMalfOpOverCurrentFlag;                            /**< (--)  trick_chkpnt_io(**) Override over current limit (malf) */
    bool mMalfOpOverVoltageFlag;                            /**< (--)  trick_chkpnt_io(**) Override out over voltage limit(malf) */
    bool mMalfRegulatedVoltageFlag;                         /**< (--)  trick_chkpnt_io(**) Override output voltage limit (malf) */
    double mOpOverCurrentLimit;                             /**< (amp)  trick_chkpnt_io(**) Converter's output over current limit value */
    ConverterInputData mConverterInput;                     /**< (--)  trick_chkpnt_io(**) The Input Data to converter */

    /// @brief ConverterElect Input Data constructor.
    ConverterElectInputData(const bool   malfBlockageFlag                     = false,
                           const double malfBlockageValue                     = 0.0,
                           const SensorAnalogInputData* outVoltageSensorInput = 0,
                           const SensorAnalogInputData* outCurrentSensorInput = 0,
                           const bool malfOpOverCurrentFlag                   = false,
                           const bool malfOpOverVoltageFlag                   = false,
                           const bool malfRegulatedVoltageFlag                = false,
                           const double inputVoltage                          = 0.0,
                           const double regulatedVoltage                      = 0.0,
                           const double efficiency                            = 0.0,
                           const double outputOverCurrentLimit                = 0.0,
                           const double outputOverVoltageLimit                = 0.0,
                           const bool outOverCurrentTripActive                = false,
                           const bool outOverVoltageTripActive                = false,
                           const double inputOverVoltageLimit                 = 0.0,
                           const double inputUnderVoltageLimit                = 0.0,
                           const bool inOverVoltageTripActive                 = false,
                           const bool inUnderVoltageTripActive                = false);

    /// @brief ConverterElect Input Data copy constructor.
    ConverterElectInputData(const ConverterElectInputData& that);

    /// @brief ConverterElect Input Data destructor.
    virtual ~ConverterElectInputData();

 private:
    /// @brief  Assignment operator unavailable since declared private and not implemented.
    ConverterElectInputData& operator =(const ConverterElectInputData& );
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Converter elect link will feed power to all the loads downstream connected to the converter.
////////////////////////////////////////////////////////////////////////////////////////////////////
class ConverterElect: public GunnsBasicLink {
    TS_MAKE_SIM_COMPATIBLE(ConverterElect);
 public:
    /// @name    Embedded objects.
    /// @{
    /// @details Embedded objects are public to allow access from the Trick events processor.
    Converter mConverter;               /**< (--) Instance of a converter */
    SensorAnalog mOutVoltageSensor;     /**< (--) Instance of the sensor for output voltage */
    SensorAnalog mOutCurrentSensor;     /**< (--) Instance of the sensor for output current */
    /// @}

    /// @brief minimum conductance of the converter
    static const double MIN_CONDUCTANCE;   /**< (1/ohm)  trick_chkpnt_io(**) minimum conductance of the converter */

    /// @brief Number of nodes to the link.
    static const int CONV_NUMPORTS = 4;   /**< (--)  trick_chkpnt_io(**) number of ports for the converter */

    enum portType {
        IN_POSITIVE  = 0,  ///< port0, input side positive port
        IN_NEGITIVE  = 1,  ///< port1, input side negative port (ground)
        OUT_POSITIVE = 2,  ///< port2, output side positive port
        OUT_NEGITIVE = 3   ///< port3, output side negative port (ground)
    };

    /// @brief ConverterElect constructor, deprecated due to obsolescence by GunnsElectConverterInput and -Output.
    ConverterElect() __attribute__ ((deprecated));

    /// @brief ConverterElect destructor.
    virtual ~ConverterElect();

    /// @brief Method to initialize ConverterElect data.
    void initialize(
            const ConverterElectConfigData& configData,
            const ConverterElectInputData& inputData,
            std::vector<GunnsBasicLink*>& networkLinks,
            const int  port0,
            const int  port1,
            const int  port2,
            const int  port3);

    /// @brief Method to use for non-linear link which ConverterElect is.
    virtual void minorStep(const double dt, const int);

    /// @brief Method to update admittance and source potential of the link.
    virtual void step(const double timeStep);

    /// @brief Method to compute flow across the link.
    virtual void computeFlows(const double timeStep);

    /// @brief Method to let gunns know this is a non-linear link.
    virtual bool isNonLinear();

    /// @brief Method for for telling the gunns solver that link has an acceptable solution or not
    virtual GunnsBasicLink::SolutionResult confirmSolutionAcceptable(const int convergedStep,
                                                     const int absoluteStep);

    /// @brief Method to override the converter On cmd value.
    ///        Mainly used in setting up the network when running without signal aspect
    void setConverterOnCmd(const bool converterOnCmd);

 protected:
    /// @brief Checks for valid implementation-specific port node assignment
    virtual bool checkSpecificPortRules(const int port, const int node) const;
//    virtual bool checkSpecificPortRules() const;

    /// @brief Method to validate link's configuration and input data as well as node mapping
    void validate(const ConverterElectConfigData& configData);

    /// @brief Virtual method for derived links to perform their restart functions.
    virtual void restartModel();

    /// @brief Method to turn on/off converter based on command received and internal state
    void updateConverterState();

    /// @brief Method to handle override malfunctions.
    void handleOverrideMalfs();

    /// @brief Method to Update output over current and over voltage sensor values
    void updateOutputSensors(const double dt);

    /// @brief Reset all the flags set for priority tripping as well as converter trip limits
    void resetTrips();

    /// @brief Method to build the links conductance
    void buildConductance();

    /// @brief Method to add admittance to the link.
    void buildAdmittance();

    /// @brief Method to build the links source vector
    void buildSourceVector();

    /// @brief Method to compute flux or the output current of the link.
    void computeFlux();

    // input data
    bool mMalfOpOverCurrentFlag;        /**< (--) Override output over current limit flag */
    double mMalfOpOverCurrentValue;     /**< (amp) New value for output over current limit */
    bool mMalfOpOverVoltageFlag;        /**< (--) Override output over voltage limit flag*/
    double mMalfOpOverVoltageValue;     /**< (V) New value for output over voltage limit */
    bool mMalfRegulatedVoltageFlag;     /**< (--) Override regulated voltage of the converter flag*/
    double mMalfRegulatedVoltageValue;  /**< (V) New value for converter's regulated output voltage */
    bool mMalfFailConverterFlag;        /**< (--) Override converter On and set converter to fail Off */

    // input and output port conductance
    double mConverterOffConductance;    /**< (1/ohm) trick_chkpnt_io(**) The conductance of converter when disabled. */
    double mOutputConductance;          /**< (1/ohm) trick_chkpnt_io(**) conductance of the converter in Discharge Mode, conductance of converter when enabled.*/
    double mInputActiveConductance;     /**< (1/ohm) active conductance used in system of equations for the battery side */
    double mOutputActiveConductance;    /**< (1/ohm) active conductance used in system of equations for the primary side */
    // output current and voltage limits
    double mOpOverCurrentLimit;         /**< (amp) Converter output over current limit */
    double mOutputOverVoltageLimit;     /**< (V) Converter output over voltage limit */
    double mRegulatedVoltage;           /**< (V) Default regulated output voltage */
    double mOutputCurrent;              /**< (amp) Output current of the converter */
    double mOutputCurrentSensed;        /**< (amp) Output current coming from sensor */
    double mOutputVoltageSensed;        /**< (V)   Output Voltage coming from sensor */

    bool mConverterOnCmd;               /**< (--) Converter On/Off command from signal aspect */
    bool mTlmPowerSupplyValid;          /**< (--) Flag for whether the power supply is working and is supplying power */
    EpsTripMgr mConverterTripMgr;       /**< (--)  */
    bool mResetTrips;                   /**< (--) Resets all trips */
    double mOutputSource;               /**< (--) Output source vector for the output nodes */
    double mDeltaPotentialOutputSide;   /**< (--) potential vector on the output side ports */
    double mDeltaPotentialInputSide;    /**< (--) potential vector on the input side ports */

 private:
     /// @brief Copy constructor unavailable since declared private and not implemented.
    ConverterElect(const ConverterElect& );

    /// @brief  Assignment operator unavailable since declared private and not implemented.
    ConverterElect& operator =(const ConverterElect& );
};

/// @}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return void
/// @details  Depending on commands received and internal state of the converter, converter will
/// be turned on/off.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void ConverterElect::updateConverterState() {
    // Turns converter on/off
    mConverter.setConverterState(mConverterOnCmd && !mMalfFailConverterFlag);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return - always true since its a non-linear link
/// @details  Method to let gunns know if this is a non-linear link.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool ConverterElect::isNonLinear() {
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    converterOnCmd    -- value to set On/Off the converter
/// @return void
/// @details Method to set Converter On Commanded value
///////////////////////////////////////////////////////////////////////////////////////////////
inline void ConverterElect::setConverterOnCmd(const bool converterOnCmd){
    mConverterOnCmd = converterOnCmd;
}

#endif
