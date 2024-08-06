#ifndef GunnsSolarArrayRegulator_EXISTS
#define GunnsSolarArrayRegulator_EXISTS

/**
@defgroup  GUNNS_SOLAR_ARRAY_REGULATOR    GUNNS Solar Array Regulator Link
@ingroup   GUNNS_ELECTRICAL

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Class for the GUNNS Solar Array Regulator link. This link is intended to be placed downstream of
   the solar array link. Its purpose is to regulate the power bus in order to trickle charge the battery
   at a commandable current setpoint, and also continue to provide power for downstream loads connected
   to PDU's via the MBSU. It accomplishes this by utilizing the ConverterElect link (its base class),
   and a simple proportional-derivative controller with battery current feedback. In order to have knowledge
   of the battery charge current, this link must be provided with a reference to the battery model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- ((aspects/electrical/sar/GunnsSolarArrayRegulator.o))

PROGRAMMERS:
- ((Mike Moore) (L3) (2015-01)
@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "math/MsMath.hh"
#include "aspects/electrical/Converter/ConverterElect.hh"

// Forward class declarations for pointer types.
class GunnsBasicConductor;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Solar Array Regulator Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Solar Array
///           Regulator configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsSolarArrayRegulatorConfigData : public ConverterElectConfigData
{
    public:
        const GunnsBasicConductor *mBattery;     /**< (--) trick_chkpnt_io(**) Reference to the battery model in the network. */
        double mNominalTrickleChargeRate;        /**< (amp) trick_chkpnt_io(**) The nominal desired battery charge current. */
        double mRegulatedVoltageLowLimit;        /**< (V) trick_chkpnt_io(**)  The lower limit on output voltage for the SAR.*/
        double mRegulatedVoltageHighLimit;       /**< (V) trick_chkpnt_io(**)  The upper limit on output voltage for the SAR.*/

        /// @brief Default constructs the regulator's configuration data.
        GunnsSolarArrayRegulatorConfigData(const std::string& name                                   = "",
                                                GunnsNodeList*     nodes                             = 0,
                                                const GunnsBasicConductor* battery                   = 0,
                                                const SensorAnalogConfigData* outVoltageSensorConfig = 0,
                                                const SensorAnalogConfigData* outCurrentSensorConfig = 0,
                                                const double       outputConductance                 = 0.0,
                                                const double       converterOffConductance           = 0.0,
                                                const int       tripPriority                         = 0,
                                                const double    standbyPower                         = 0.0,
                                                const double    trickleChargeRate                    = 0.0,
                                                const double    regulatedVoltageLowLimit             = 0.0,
                                                const double    regulatedVoltageHighLimit            = 0.0);

        /// @brief Default destructs the regulator's configuration data.
        virtual ~GunnsSolarArrayRegulatorConfigData();

        /// @brief Copy constructs the regulator's configuration data.
        GunnsSolarArrayRegulatorConfigData(const GunnsSolarArrayRegulatorConfigData& that);
    protected:

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsSolarArrayRegulatorConfigData& operator =(const GunnsSolarArrayRegulatorConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Solar Array Regulator Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the solar array
///           regulator input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsSolarArrayRegulatorInputData : public ConverterElectInputData
{
    public:
        double mProportionalGain;         /**< (--)  trick_chkpnt_io(**) Proportional gain for the battery
                                                     charge current PD feedback controller. */
        double mDerivativeGain;           /**< (--)  trick_chkpnt_io(**) Derivative gain for the battery
                                                     charge current PD feedback controller. */

        /// @brief    Default constructs this Solar Array Regulator input data.
        GunnsSolarArrayRegulatorInputData(const bool   malfBlockageFlag                       = false,
                                           const double malfBlockageValue                     = 0.0,
                                           const SensorAnalogInputData* outVoltageSensorInput = 0,
                                           const SensorAnalogInputData* outCurrentSensorInput = 0,
                                           const bool malfOpOverCurrentFlag                   = false,
                                           const bool malfOpOverVoltageFlag                   = false,
                                           const bool malfRegulatedVoltageFlag                = false,
                                           const double inputVoltage                          = 0.0,
                                           const double regulatedVoltage                      = 0.0,
                                           const double efficiency                            = 0.0,
                                           const double opOverCurrentLimit                    = 0.0,
                                           const double opOverVoltageLimit                    = 0.0,
                                           const bool opOverCurrentTripActive                 = false,
                                           const bool opOverVoltageTripActive                 = false,
                                           const double inputOverVoltageLimit                 = 0.0,
                                           const double inputUnderVoltageLimit                = 0.0,
                                           const bool inOverVoltageTripActive                 = false,
                                           const bool inUnderVoltageTripActive                = false,
                                           const double proportionalGain                      = 0.0,
                                           const double derivativeGain                        = 0.0);

        /// @brief    Default destructs this solar array regulator input data.
        virtual ~GunnsSolarArrayRegulatorInputData();

        /// @brief    Copy constructs this solar array regulator input data.
        GunnsSolarArrayRegulatorInputData(const GunnsSolarArrayRegulatorInputData& that);
    protected:

    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsSolarArrayRegulatorInputData& operator =(const GunnsSolarArrayRegulatorInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Solar Array Regulator Class
///
/// @details  The Solar Array Regulator class is designed to provide a simple way to regulate
///           power bus voltage when you have a solar array, battery, and downstream loads. The link
///           regulates its output voltage in order to target a commandable battery trickle charge
///           current in the presenece of downstream loads.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsSolarArrayRegulator : public ConverterElect
{
        TS_MAKE_SIM_COMPATIBLE(GunnsSolarArrayRegulator);

    public:

        /// @brief Default Solar Array Regulator Constructor, deprecated due to obsolescence by GunnsElectPvRegConv.
        GunnsSolarArrayRegulator() __attribute__ ((deprecated));

        /// @brief Default Solar Array Regulator Destructor
        virtual ~GunnsSolarArrayRegulator();

        /// @brief Initialize method
        void initialize(const GunnsSolarArrayRegulatorConfigData& configData,
                        const GunnsSolarArrayRegulatorInputData&  inputData,
                        std::vector<GunnsBasicLink*>&       networkLinks,
                        const int                           port0,
                        const int                           port1,
                        const int                           port2,
                        const int                           port3);

        /// @brief Need to override the ConverterElect's minor step function.
        virtual void minorStep(const double dt, const int);

        /// @brief P-D controller for battery current feedback control
        void updateControl(const double timeStep);

    protected:
        /// @brief Validates the link
        void validate() const;
        /// @brief Method to update admittance and source potential of the link.
        virtual void step(const double timeStep);

        bool mInitFlag;                      /**< (--) Flag indicating if the solar array regulator link has been initialized and validated.   */
        const GunnsBasicConductor* mBattery; /**< (--) The battery in the network that this regulator is going to help trickle charge. */
        double mDesiredChargeCurrent;        /**< (amp) trick_chkpnt_io(**) Desired battery charge current. Controllable. */
        double mPreviousChargeCurrentError;  /**< (amp) trick_chkpnt_io(**) The controller error in the battery current from the previous pass. */
        double mKp;                          /**< (--) trick_chkpnt_io(**) Proportional gain for the battery current PD feedback controller. */
        double mKd;                          /**< (--) trick_chkpnt_io(**) Derivative gain for the battery current PD feedback controller. */
        double mRegulatedVoltageLowLimit;    /**< (V) trick_chkpnt_io(**)  The lower limit on output voltage for the SAR.*/
        double mRegulatedVoltageHighLimit;   /**< (V) trick_chkpnt_io(**)  The upper limit on output voltage for the SAR.*/
        double mControlledVoltage;           /**< (V) trick_chkpnt_io(**) The controlled voltage. Battery current P-D feedback control. */
        double mMinInVoltage;                /**< (V) trick_chkpnt_io(**) The minimum input voltage to this converter. */
        double mMarginTurnOnVoltage;         /**< (V) trick_chkpnt_io(**) The acceptable margin voltage to turn back on. */

    private:

        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsSolarArrayRegulator(const GunnsSolarArrayRegulator& that);

        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsSolarArrayRegulator& operator =(const GunnsSolarArrayRegulator& that);
};

/// @}

#endif
