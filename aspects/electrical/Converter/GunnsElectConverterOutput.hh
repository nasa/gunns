#ifndef GunnsElectConverterOutput_EXISTS
#define GunnsElectConverterOutput_EXISTS

/**
@file
@brief    GUNNS Electrical Converter Output Link declarations

@defgroup  TSM_GUNNS_ELECTRICAL_CONVERTER_OUTPUT_LINK    GUNNS Electrical Converter Output Link
@ingroup   TSM_GUNNS_ELECTRICAL_CONVERTER

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Electrical Converter Output link.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsElectConverterOutput.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2019-10) (Initial))

@{
*/

#include "core/GunnsBasicLink.hh"
#include "aspects/electrical/Converter/GunnsElectConverterInput.hh"
#include "core/GunnsSensorAnalogWrapper.hh"
#include "aspects/electrical/TripLogic/GunnsTripLogic.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "math/MsMath.hh"

// Forward-declaration of types.
class GunnsElectConverterOutputConfigData;
class GunnsElectConverterOutputInputData;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical Converter Output link.
///
/// @details  This models the output side of a DC-DC converter.  This pairs with a
///           GunnsElectConverterInput link, which may be in the same network, a different network
///           or even a different sim.  Such a pair can model many things, such as:
///           - DC-DC converter,
///           - Transformer,
///           - Battery charger,
///           - Short-circuit or normal load between networks,
///           - A general supply-demand interface between networks or models.
///
///           This receives supplied voltage from the input side link (mInputVoltage), and returns a
///           power load (mInputPower) in response.  It can control the output node in several ways
///           (mRegulatorType and mSetpoint):
///           - VOLTAGE: regulates the output voltage to a commandable mSetpoint as a voltage
///                 source, like an adjustable DC-DC converter.
///           - TRANSFORMER: regulates the output voltage to mInputVoltage * mSetpoint as a voltage
///                 source, like a fixed-coil voltage transformer.  mSetpoint is the output:input
///                 winding ratio.
///           - CURRENT: regulates output current to mSetpoint as a current source.
///           - POWER: regulates output power to mSetpoint as a current source.
///
///           This link can be connected to the Ground node for storage as a spare.  When connected
///           to the Ground node, it does nothing.  A converter input-output pair can be used to
///           simulate a short-circuit between separate networks.
///
///           This contains optional trip logic for output over-volt, under-volt, and over-current
///           trips.  For voltage regulation modes (VOLTAGE and TRANSFORMER), this can optionally do
///           output current limiting instead of over-current tripping.  For current regulation
///           modes (CURRENT and POWER), this can optionally do output voltage limiting instead of
///           over-voltage or under-voltage tripping.  These features can be fed by optional sensors
///           in the network, or use 'truth' values.
///
///           This has an optional pointer to a paired GunnsElectConverterInput link.  This should
///           be used when and only when both links are in the same network.  This causes the two
///           links to share input voltage and load values between the network minor steps, for less
///           latency in the supply-demand loop.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectConverterOutput : public GunnsBasicLink
{
    TS_MAKE_SIM_COMPATIBLE(GunnsElectConverterOutput);

    public:
        /// @brief  Enumeration of the load types.
        enum RegulatorType {
            VOLTAGE     = 0, ///< Controls output voltage to setpoint as a voltage source.
            TRANSFORMER = 1, ///< Controls output to input voltage ratio setpoint as a voltage source.
            CURRENT     = 2, ///< Controls output current to setpoint as a current source.
            POWER       = 3  ///< Controls output power to setpoint as a current source.
        };
        /// @brief  Enumeration of the limit states.
        enum LimitStates {
            NO_LIMIT    = 0, ///< Not in a limiting state.
            LIMIT_OC    = 1, ///< The voltage-controlling regulator is limiting over-current.
            LIMIT_OV    = 2, ///< The current-controlling regulator is limiting over-voltage.
            LIMIT_UV    = 3  ///< The current-controlling regulator is limiting under-voltage.
        };
        /// @brief  Default Constructor.
        GunnsElectConverterOutput();
        /// @brief  Default Destructor.
        virtual ~GunnsElectConverterOutput();
        /// @brief  Initialize the link with config data and port assignments.
        void initialize(      GunnsElectConverterOutputConfigData& configData,
                        const GunnsElectConverterOutputInputData&  inputData,
                        std::vector<GunnsBasicLink*>&              networkLinks,
                        const int                                  port0);
        /// @brief  Checks if the given node list is the same as this link's node list.
        void checkNodeList(GunnsNodeList* nodeList);
        /// @brief  Updates the link contributions to the system of equations.
        virtual void step(const double dt);
        /// @brief  Minor step method for non-linear iterations.
        virtual void minorStep(const double dt, const int minorStep);
        /// @brief  Calculates flows across the link.
        virtual void computeFlows(const double);
        /// @brief  Returns the link's assessment of the network solution.
        virtual SolutionResult confirmSolutionAcceptable(const int convergedStep,
                                                         const int absoluteStep);
        /// @brief  Resets the link back to the previous minor step iteration.
        virtual bool resetLastMinorStep(const int convergedStep, const int absoluteStep);
        /// @brief  Computes and returns the input channel power and validity.
        bool computeInputPower(double& inputPower);
        /// @brief  Returns if the link is non-linear.
        virtual bool isNonLinear();
        /// @brief  Resets any trips.
        void resetTrips();
        /// @brief  Sets the enabled flag.
        void setEnabled(const bool enabled);
        /// @brief  Sets the input voltage.
        void setInputVoltage(const double inputVoltage);
        /// @brief  Sets the commanded setpoint.
        void setSetpoint(const double setpoint);
        /// @brief  Sets the current/voltage limiting state.
        void setLimitingState(const LimitStates state);
        /// @brief  Sets the output conductance.
        void setOutputConductance(const double conductance);
        /// @brief  Returns the commanded setpoint.
        double getSetpoint() const;
        /// @brief  Returns the input power.
        double getInputPower() const;
        /// @brief  Returns the input power valid flag.
        bool getInputPowerValid() const;
        /// @brief  Returns whether the controller is in a current/voltage limiting state.
        bool getLimitingState() const;
        /// @brief  Returns the enabled flag.
        bool getEnabled() const;
        /// @brief  Returns the controlled voltage.
        double getControlVoltage() const;
        /// @brief  Returns the output over-voltage trip logic.
        GunnsTripLogic* getOutputOverVoltageTrip();
        /// @brief  Returns the output under-voltage trip logic.
        GunnsTripLogic* getOutputUnderVoltageTrip();
        /// @brief  Returns the output over-current trip logic.
        GunnsTripLogic* getOutputOverCurrentTrip();

    protected:
        GunnsElectConverterOutput::RegulatorType mRegulatorType;          /**<    (1)     trick_chkpnt_io(**) The type of output regulation. */
        double                                   mOutputConductance;      /**<    (1/ohm) trick_chkpnt_io(**) The output conductance. */
        double                                   mConverterEfficiency;    /**<    (1)     trick_chkpnt_io(**) The conversion efficiency (0-1). */
        SensorAnalog*                            mOutputVoltageSensor;    /**<    (1)     trick_chkpnt_io(**) Pointer to the output voltage sensor. */
        SensorAnalog*                            mOutputCurrentSensor;    /**<    (1)     trick_chkpnt_io(**) Pointer to the output current sensor. */
        GunnsElectConverterInput*                mInputLink;              /**< *o (1)     trick_chkpnt_io(**) Pointer to the converter input side link. */
        bool                                     mEnableLimiting;         /**<    (1)     trick_chkpnt_io(**) Limits output current or voltage instead of tripping. */
        unsigned int                             mStateFlipsLimit;        /**<    (1)     trick_chkpnt_io(**) Number of times a reverse bias or limit state can be entered per major step. */
        bool                                     mEnabled;                /**<    (1)                         Operation is enabled. */
        double                                   mInputVoltage;           /**<    (V)                         Input channel voltage received from the input side. */
        bool                                     mInputVoltageValid;      /**<    (1)     trick_chkpnt_io(**) The input channel voltage value is valid. */
        bool                                     mOutputPowerAvailable;   /**<    (1)     trick_chkpnt_io(**) Power is available to the output channel during this major step. */
        double                                   mSetpoint;               /**<    (1)                         Commanded regulation setpoint. */
        bool                                     mResetTrips;             /**<    (1)     trick_chkpnt_io(**) Input command to reset trips. */
        double                                   mInputPower;             /**<    (W)                         Input channel power load sent to the input side. */
        bool                                     mInputPowerValid;        /**<    (1)     trick_chkpnt_io(**) The input channel power load value is valid. */
        double                                   mOutputChannelLoss;      /**<    (W)     trick_chkpnt_io(**) Power loss through the output channel resistance. */
        double                                   mLoadResistance;         /**<    (ohm)                       Estimate of total downstream load resistance. */
        double                                   mTotalPowerLoss;         /**<    (W)                         Total power loss through converter efficiency and output channel resistance. */
        GunnsTripGreaterThan                     mOutputOverVoltageTrip;  /**<    (1)                         Output over-voltage trip function. */
        GunnsTripLessThan                        mOutputUnderVoltageTrip; /**<    (1)                         Output under-voltage trip function. */
        GunnsTripGreaterThan                     mOutputOverCurrentTrip;  /**<    (1)                         Output over-current trip function. */
        bool                                     mLeadsInterface;         /**< *o (1)     trick_chkpnt_io(**) This precedes the mInputLink in the network. */
        bool                                     mReverseBiasState;       /**<    (1)     trick_chkpnt_io(**) Converter is dioded off due to reverse voltage bias. */
        unsigned int                             mReverseBiasFlips;       /**<    (1)     trick_chkpnt_io(**) Number of times voltage bias has flipped reverse during this major step. */
        LimitStates                              mLimitState;             /**<    (1)                         The limiting state the controller is currently in. */
        unsigned int                             mLimitStateFlips;        /**<    (1)     trick_chkpnt_io(**) Number of times converter has flipped to current/voltage limiting this major step. */
        double                                   mSourceVoltage;          /**<    (V)     trick_chkpnt_io(**) Active voltage source value when acting in a voltage source mode. */
        /// @brief  Validates the configuration and input data.
        void validate(const GunnsElectConverterOutputConfigData& configData,
                      const GunnsElectConverterOutputInputData&  inputData) const;
        /// @brief  Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief  Computes contributions to the sysstem of equations.
        void computeRegulationSources(double& conductance, double& voltage, double& current);
        /// @brief  Updates the output current of this link.
        void computeFlux();
        /// @brief  Updates the forward/reverse bias state of the converter.
        void updateBias(GunnsBasicLink::SolutionResult& result, const int convergedStep,
                        const bool noReverseBias);
        /// @brief  Updates the current/voltage limiting state.
        bool updateLimitState(GunnsBasicLink::SolutionResult& result,
                              const float                     voltage,
                              const float                     current);
        /// @brief  Updates the current limiting state for voltage regulators.
        void updateCurrentLimitState(GunnsBasicLink::SolutionResult& result,
                                     const float                     voltage,
                                     const float                     current);
        /// @brief  Updates the voltage limiting state for current regulators.
        bool updateVoltageLimitState(GunnsBasicLink::SolutionResult& result,
                                     const float                     voltage,
                                     const float                     current);
        /// @brief  Computes the effective output voltage control setpoint.
        float computeVoltageControlSetpoint();
        /// @brief  Computes the effective output current control setpoint.
        float computeCurrentControlSetpoint();
        /// @brief  Rejects the solution and sets the lmiting state to the given value.
        void rejectWithLimitState(GunnsBasicLink::SolutionResult& result,
                                  const LimitStates               state);
        /// @brief  Updates the estimate of downstream load resistance.
        void estimateLoad();
        /// @brief  Applies the blockage malfunction to the given scalar.
        double applyBlockage(const double scalar) const;
        /// @brief  Returns whether this is a converter type that normally controls voltage. */
        bool isVoltageRegulator() const;
        /// @brief  Returns whether any trip logics are tripped.
        bool isAnyTrips() const;

    private:
        /// @details Define the number of ports this link class has.  All objects of the same link
        ///          class always have the same number of ports.  We use an enum rather than a
        ///          static const int so that we can reuse the NPORTS name and allow each class to
        ///          define its own value.
        enum {NPORTS = 1};
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsElectConverterOutput(const GunnsElectConverterOutput& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsElectConverterOutput& operator =(const GunnsElectConverterOutput& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical Converter Output link configuration data.
///
/// @details  This is a data structure for the Electrical Converter Output link configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectConverterOutputConfigData: public GunnsBasicLinkConfigData
{
    public:
        GunnsElectConverterOutput::RegulatorType mRegulatorType;               /**< (1)     trick_chkpnt_io(**) The type of output regulation. */
        bool                                     mEnableLimiting;              /**< (1)     trick_chkpnt_io(**) Limits output current or voltage instead of tripping. */
        double                                   mOutputConductance;           /**< (1/ohm) trick_chkpnt_io(**) The output conductance. */
        double                                   mConverterEfficiency;         /**< (1)     trick_chkpnt_io(**) The voltage conversion efficiency (0-1). */
        GunnsSensorAnalogWrapper*                mOutputVoltageSensor;         /**< (1)     trick_chkpnt_io(**) Pointer to the output voltage sensor spotter. */
        GunnsSensorAnalogWrapper*                mOutputCurrentSensor;         /**< (1)     trick_chkpnt_io(**) Pointer to the output current sensor spotter. */
        unsigned int                             mTripPriority;                /**< (1)     trick_chkpnt_io(**) Priority of trips in the network. */
        float                                    mOutputOverVoltageTripLimit;  /**< (V)     trick_chkpnt_io(**) Output over-voltage trip limit. */
        float                                    mOutputUnderVoltageTripLimit; /**< (V)     trick_chkpnt_io(**) Output under-voltage trip limit. */
        float                                    mOutputOverCurrentTripLimit;  /**< (amp)   trick_chkpnt_io(**) Output over-current trip limit. */
        GunnsElectConverterInput*                mInputLink;                   /**< (1)     trick_chkpnt_io(**) Pointer to the converter input side link. */
        unsigned int                             mStateFlipsLimit;             /**< (1)     trick_chkpnt_io(**) Number of times a reverse bias or limit state can be entered per major step. */
        /// @brief  Default constructs this Electrical Converter Output configuration data.
        GunnsElectConverterOutputConfigData(
                const std::string&                             name                        = "",
                GunnsNodeList*                                 nodes                       = 0,
                const GunnsElectConverterOutput::RegulatorType regulatorType               = GunnsElectConverterOutput::VOLTAGE,
                const double                                   outputConductance           = 0.0,
                const double                                   converterEfficiency         = 0.0,
                GunnsSensorAnalogWrapper*                      outputVoltageSensor         = 0,
                GunnsSensorAnalogWrapper*                      outputCurrentSensor         = 0,
                const unsigned int                             tripPriority                = 0,
                const float                                    outputOverVoltageTripLimit  = 0.0,
                const float                                    outputOverCurrentTripLimit  = 0.0,
                GunnsElectConverterInput*                      inputLink                   = 0,
                const bool                                     enableLimiting              = false,
                const float                                    outputUnderVoltageTripLimit = 0.0);
        /// @brief  Default destructs this Electrical Converter Output configuration data.
        virtual ~GunnsElectConverterOutputConfigData();
        /// @brief  Copy constructs this Electrical Converter Output configuration data.
        GunnsElectConverterOutputConfigData(const GunnsElectConverterOutputConfigData& that);

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsElectConverterOutputConfigData& operator =(const GunnsElectConverterOutputConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical Converter Output link input data.
///
/// @details  This is a data structure for the Electrical Converter Output link input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectConverterOutputInputData : public GunnsBasicLinkInputData
{
    public:
        bool   mEnabled;            /**< (1) trick_chkpnt_io(**) Initial operation enabled state. */
        double mInputVoltage;       /**< (V) trick_chkpnt_io(**) Initial input voltage. */
        double mInputPower;         /**< (W) trick_chkpnt_io(**) Initial input power load. */
        double mSetpoint;           /**< (1) trick_chkpnt_io(**) Initial commanded setpoint. */
        /// @brief  Default constructs this Electrical Converter Output input data.
        GunnsElectConverterOutputInputData(const bool   malfBlockageFlag  = false,
                                           const double malfBlockageValue = 0.0,
                                           const bool   enabled           = false,
                                           const double inputVoltage      = 0.0,
                                           const double inputPower        = 0.0,
                                           const double setpoint          = 0.0);
        /// @brief  Default destructs this Electrical Converter Output input data.
        virtual ~GunnsElectConverterOutputInputData();
        /// @brief  Copy constructs this Electrical Converter Output input data.
        GunnsElectConverterOutputInputData(const GunnsElectConverterOutputInputData& that);

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsElectConverterOutputInputData& operator =(const GunnsElectConverterOutputInputData&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool  (1)  Always returns true.
///
/// @details  The link is always non-linear.  Even though the load type may be linear, we still use
///           minor steps to run the trip logic.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectConverterOutput::isNonLinear()
{
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Resets the tripped state of the contained trip logic functions.  If we have a pointer
///           to the other side, reset its trips as well.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectConverterOutput::resetTrips()
{
    mOutputOverVoltageTrip.resetTrip();
    mOutputUnderVoltageTrip.resetTrip();
    mOutputOverCurrentTrip.resetTrip();
    if (mInputLink) {
        mInputLink->resetTrips();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Updates mFlux as the output current.  Does not allow negative output current.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectConverterOutput::computeFlux()
{
    mFlux = fmax(0.0, mSourceVector[0] - mPotentialVector[0] * mAdmittanceMatrix[0]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  enabled  (--)  Enabled flag value to use.
///
/// @details  Sets mEnabled to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectConverterOutput::setEnabled(const bool enabled)
{
    mEnabled = enabled;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  inputVoltage  (V)  Input voltage value to use.
///
/// @details  Sets mInputVoltage to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectConverterOutput::setInputVoltage(const double inputVoltage)
{
    mInputVoltage = inputVoltage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  setpoint  (--)  Setpoint value to use.
///
/// @details  Sets mSetpoint to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectConverterOutput::setSetpoint(const double setpoint)
{
    mSetpoint = setpoint;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  state  (--)  The limit state to put the controller into.
///
/// @details  Sets mLimitState to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectConverterOutput::setLimitingState(const LimitStates state)
{
    mLimitState = state;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  conductance  (1/ohm)  Conductance value for the converter output.
///
/// @details  Sets mOutputConductance to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectConverterOutput::setOutputConductance(const double conductance)
{
    mOutputConductance = conductance;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double  (--)  The controller setpoint value.
///
/// @details  Returns the value of the mSetpoint attribute.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectConverterOutput::getSetpoint() const
{
    return mSetpoint;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double  (W)  The power load on the input side of the interface.
///
/// @details  Returns the value of mInputPower.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectConverterOutput::getInputPower() const
{
    return mInputPower;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool  (--)  Whether the input-side power load value is valid.
///
/// @details  Returns the value of mInputPowerValid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectConverterOutput::getInputPowerValid() const
{
    return mInputPowerValid;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool  (--)  True if the converter is in a current/voltage limiting state.
///
/// @details  Returns true if the value of mLimitState is other than NO_LIMIT.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectConverterOutput::getLimitingState() const
{
    return (NO_LIMIT != mLimitState);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool  (--)  True if the converter is enabled.
///
/// @details  Returns the value of mEnabled.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectConverterOutput::getEnabled() const
{
    return mEnabled;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  GunnsTripLogic*  (--)  Pointer to the output over-voltage trip logic.
///
/// @details  Returns the address of the mOutputOverVoltageTrip attribute.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline GunnsTripLogic* GunnsElectConverterOutput::getOutputOverVoltageTrip()
{
    return &mOutputOverVoltageTrip;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  GunnsTripLogic*  (--)  Pointer to the output under-voltage trip logic.
///
/// @details  Returns the address of the mOutputUnderVoltageTrip attribute.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline GunnsTripLogic* GunnsElectConverterOutput::getOutputUnderVoltageTrip()
{
    return &mOutputUnderVoltageTrip;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  GunnsTripLogic*  (--)  Pointer to the output over-current trip logic.
///
/// @details  Returns the address of the mOutputOverCurrentTrip attribute.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline GunnsTripLogic* GunnsElectConverterOutput::getOutputOverCurrentTrip()
{
    return &mOutputOverCurrentTrip;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Computes the estimated total downstream load resistance as the downstream voltage over
///           the output current, when both are > 0.  If either is <= 0, then the estimate is reset
///           to the starting guess of 1.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectConverterOutput::estimateLoad()
{
    computeFlux();
    if (mFlux > DBL_EPSILON && mPotentialVector[0] > DBL_EPSILON) {
        mLoadResistance = mPotentialVector[0] / mFlux;
    } else {
        mLoadResistance = 1.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in, out] scalar (--) The term to be scaled by the blockage malfunction.
///
/// @details  Applies the blockage malfunction to the given scalar.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectConverterOutput::applyBlockage(const double scalar) const
{
    if (mMalfBlockageFlag) {
        return scalar * (1.0 - MsMath::limitRange(0.0, mMalfBlockageValue, 1.0));
    }
    return scalar;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool (--) True if any trip logics are tripped, false if none are tripped.
///
/// @details  Returns true if any trip logics are tripped.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectConverterOutput::isAnyTrips() const
{
    return mOutputOverVoltageTrip.isTripped() or mOutputOverCurrentTrip.isTripped() or
           mOutputUnderVoltageTrip.isTripped();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool (--) True if this is a voltage regulator or transformer.
///
/// @details  Returns true if this is a voltage regulator or transformer.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectConverterOutput::isVoltageRegulator() const
{
    return (VOLTAGE == mRegulatorType) or (TRANSFORMER == mRegulatorType);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] convergedStep (--) Unused.
/// @param[in] absoluteStep  (--) Unused.
///
/// @returns  bool (--) Always returns true.
///
/// @details  After a network reset to the last minor step, any input power calculation will be
///           invalid until we've confirmed the next minor step solution.  The solver resets the
///           potential vector to the previous value, and this becomes out of sync with our inputs
///           to the system of equations, so any calculation of flux and power based on those values
///           may be incorrect.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectConverterOutput::resetLastMinorStep(const int convergedStep, __attribute__((unused))
                                                          const int absoluteStep __attribute__((unused)))
{
    mInputPowerValid = false;
    return true;
}

#endif
