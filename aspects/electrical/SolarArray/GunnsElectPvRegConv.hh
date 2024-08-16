#ifndef GunnsElectPvRegConv_EXISTS
#define GunnsElectPvRegConv_EXISTS

/**
@file
@brief    GUNNS Electrical Photovoltaic Array Converting Regulator Model declarations

@defgroup  GUNNS_ELECTRICAL_PHOTOVOLTAIC_REG_CONV    Photovoltaic Array Converting Regulator Model
@ingroup   GUNNS_ELECTRICAL_PHOTOVOLTAIC

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the Photovoltaic Array Converting Regulator GUNNS Link Model.  Refer to the
   GunnsElectPvRegConv class details below.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsElectPvRegConv.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2017-10) (Initial))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "core/GunnsBasicLink.hh"
#include "GunnsElectPvRegSensors.hh"
#include "GunnsElectPvRegTrips.hh"
#include "math/MsMath.hh"

// Forward-declare pointer types.
class GunnsElectPvArray;
class GunnsSensorAnalogWrapper;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Photovoltaic Array Converting Regulator Configuration Data
///
/// @details  This class provides a structure for the Photovoltaic Array Converting Regulator
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectPvRegConvConfigData : public GunnsBasicLinkConfigData
{
    public:
        double                    mVoltageConvLimit;      /**< (V)   trick_chkpnt_io(**) Maximum ratio this regulator can increase the input voltage from the array to the output. */
        double                    mVoltageConvEfficiency; /**< (--)  trick_chkpnt_io(**) Power efficiency (0-1) of input to output voltage conversion. */
        GunnsElectPvArray*        mArray;                 /**< (--)  trick_chkpnt_io(**) Pointer to the PV array link. */
        GunnsSensorAnalogWrapper* mInCurrentSensor;       /**< (--)  trick_chkpnt_io(**) Optional pointer to the input current sensor wrapper. */
        GunnsSensorAnalogWrapper* mInVoltageSensor;       /**< (--)  trick_chkpnt_io(**) Optional pointer to the input voltage sensor wrapper. */
        GunnsSensorAnalogWrapper* mOutCurrentSensor;      /**< (--)  trick_chkpnt_io(**) Optional pointer to the output current sensor wrapper. */
        GunnsSensorAnalogWrapper* mOutVoltageSensor;      /**< (--)  trick_chkpnt_io(**) Optional pointer to the output voltage sensor wrapper. */
        double                    mInOverCurrentTrip;     /**< (amp) trick_chkpnt_io(**) Optional input over-current trip limit. */
        double                    mInOverVoltageTrip;     /**< (V)   trick_chkpnt_io(**) Optional input over-volt trip limit. */
        double                    mOutOverCurrentTrip;    /**< (amp) trick_chkpnt_io(**) Optional output over-current trip limit. */
        double                    mOutOverVoltageTrip;    /**< (V)   trick_chkpnt_io(**) Optional output over-volt trip limit. */
        double                    mOutUnderVoltageTrip;   /**< (V)   trick_chkpnt_io(**) Optional output under-volt trip limit. */
        unsigned int              mTripPriority;          /**< (--)  trick_chkpnt_io(**) Trip network step priority. */
        /// @brief Default constructs this Photovoltaic Array Converting Regulator configuration data.
        GunnsElectPvRegConvConfigData(const std::string&        name                   = "",
                                      GunnsNodeList*            nodes                  = 0,
                                      const double              voltageConvLimit       = 0.0,
                                      const double              voltageConvEfficiency  = 0.0,
                                      GunnsElectPvArray*        array                  = 0,
                                      GunnsSensorAnalogWrapper* inCurrentSensor        = 0,
                                      GunnsSensorAnalogWrapper* inVoltageSensor        = 0,
                                      GunnsSensorAnalogWrapper* outCurrentSensor       = 0,
                                      GunnsSensorAnalogWrapper* outVoltageSensor       = 0,
                                      const double              inputOverCurrentTrip   = 0.0,
                                      const double              inputOverVoltageTrip   = 0.0,
                                      const double              outputOverCurrentTrip  = 0.0,
                                      const double              outputOverVoltageTrip  = 0.0,
                                      const double              outputUnderVoltageTrip = 0.0,
                                      const unsigned int        tripPriority           = 0);
        /// @brief Default destructs this Photovoltaic Array Converting Regulator configuration data.
        virtual ~GunnsElectPvRegConvConfigData();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectPvRegConvConfigData(const GunnsElectPvRegConvConfigData& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsElectPvRegConvConfigData& operator =(const GunnsElectPvRegConvConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Photovoltaic Array Converting Regulator Input Data
///
/// @details  This class provides a structure for the Photovoltaic Array Converting Regulator input
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectPvRegConvInputData : public GunnsBasicLinkInputData
{
    public:
        double mVoltageSetpoint;      /**< (V)  trick_chkpnt_io(**) Initial setpoint for the regulated output voltage. */
        double mVoltageSetpointDelta; /**< (V)  trick_chkpnt_io(**) Initial nominal delta to voltage setpoint. */
        bool   mPowered;              /**< (--) trick_chkpnt_io(**) Initial state of power on flag. */
        bool   mEnabled;              /**< (--) trick_chkpnt_io(**) Initial state of enable command. */
        /// @brief Default constructs this Photovoltaic Array Converting Regulator input data.
        GunnsElectPvRegConvInputData(const double voltageSetpoint      = 0.0,
                                     const double voltageSetpointDelta = 0.0,
                                     const bool   powered              = false,
                                     const bool   enabled              = false);
        /// @brief Default destructs this Photovoltaic Array Converting Regulator input data.
        virtual ~GunnsElectPvRegConvInputData();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectPvRegConvInputData(const GunnsElectPvRegConvInputData& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsElectPvRegConvInputData& operator =(const GunnsElectPvRegConvInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Photovoltaic Array Converting Regulator
///
/// @details  This models an output voltage regulator for a GunnsElectPvArray that performs a DC-DC
///           conversion of the array voltage to a regulated output voltage, similar to the Solar
///           Array Regulator used in AMPS.
///
///           The DC-DC voltage conversion is configured with a conversion efficiency, and an upper
///           limit to the output/input voltage ratio.  No lower limit is applied -- the output
///           voltage can be lowered to near zero.  Losses due to the conversion efficiency are
///           output as waste heat.
///
///           This link has an execution order dependency on the GunnsElectPvArray it interfaces
///           with.  This link must be initialized and stepped after the array.  This order can be
///           established with proper setup in the GUNNS network.
///
///           This is a one-port GUNNS link:
///           - Port 0 is the output node to the downstream circuit, whose voltage is controlled.
///
///           Rather than an input node, this regulator has a pointer to interact with the array
///           directly.  Since this regulator and the array calculate the interfacing state between
///           them, array strings are always tied to a common output (the input to the regulator),
///           and there is never a need to bypass the regulator and connect the array directly to
///           the downstream circuit, a network node between the regulator and array is wasteful, so
///           not used.
///
///           This link has 4 operating states, which it automatically switches between:
///           - REG:   when the array can meet the demanded power load from the downstream ciruit,
///                    the output voltage is regulated.  This link acts like an ideal voltage source
///                    on the output node.
///           - SAG:   when the array can't meet the demanded power load at the regulated voltage or
///                    the regulator can't boost the array voltage up to the regulated voltage, but
///                    the array is still on the open-circuit side of its I-V performance curve, the
///                    output voltage of this regulator begins to drop below the regulated value.
///                    This link acts like a (non-ideal) voltage source on the output node.
///           - SHORT: same as SAG, except the array is now on the short-circuit side of its I-V
///                    curve.  This happens at high loads on the regulator.  The regulator output
///                    voltage drops more steeply.
///           - OFF:   when the link is disabled by command input, or the downstream circuit is at
///                    a higher voltage than the regulator setpoint.  The link places a very small
///                    leak conductance on the output node to Ground.
///
///           This link allows optional sensors for input and output voltage and current.  Optional
///           trip functions can also be used with or without the sensors.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectPvRegConv : public GunnsBasicLink
{
    TS_MAKE_SIM_COMPATIBLE(GunnsElectPvRegConv);

    public:
        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool   mMalfVoltageBiasFlag;  /**< (--) Malfunction activation flag for regulated voltage bias. */
        double mMalfVoltageBiasValue; /**< (V)  Regulated voltage bias malfunction value. */
        /// @}

        /// @brief Enumeration of the Photovoltaic Array Converting Regulator states.
        enum PvRegStates {
            OFF      = 0, ///< Powered off, bypassed, dioded or otherwise inactive.
            REG      = 1, ///< On and actively regulating output voltage.
            SAG      = 2, ///< On but at reduced output voltage because insufficient PV power.
            SHORT    = 3, ///< On but at reduced output voltage on the short-circuit side of the PV performance curve.
            N_STATES = 4  ///< Invalid, keep this last!
        };

        /// @brief Default Constructor.
        GunnsElectPvRegConv();
        /// @brief Default constructor.
        virtual ~GunnsElectPvRegConv();
        /// @brief Initialize method.
        void initialize(const GunnsElectPvRegConvConfigData& configData,
                        const GunnsElectPvRegConvInputData&  inputData,
                        std::vector<GunnsBasicLink*>&        networkLinks,
                        const int                            port0);
        /// @brief Default constructor.
        virtual void step(const double dt);
        /// @brief Minor Step method for non-linear systems.
        virtual void minorStep(const double dt, const int minorStep);
        /// @brief Compute Flows.
        virtual void computeFlows(const double dt);
        /// @brief Boolean for telling the solver this is a non-linear link.
        virtual bool isNonLinear();
        /// @brief Returns the link's assessment of the network solution.
        virtual SolutionResult confirmSolutionAcceptable(const int convergedStep,
                                                         const int absoluteStep);
        /// @brief Sets the regulated voltage setpoint value.
        void   setVoltageSetpoint(const double voltage);
        /// @brief Sets the regulated voltage setpoint delta value.
        void   setVoltageSetpointDelta(const double voltage);
        /// @brief Sets the regulator enabled flag.
        void   setEnabled(const bool flag);
        /// @brief Returns the maximum current that can be output at the regulated voltage.
        double getMaxRegCurrent() const;
        /// @brief Returns the current regulated voltage setpoint.
        double getVoltageSetpoint() const;
        /// @brief Returns a pointer to the trip logic group.
        GunnsElectPvRegTrips* getTrips();

    protected:
        double                 mVoltageConvLimit;          /**<    (V)     trick_chkpnt_io(**) Maximum ratio this regulator can increase the input voltage from the array to the output. */
        double                 mVoltageConvEfficiency;     /**<    (--)    trick_chkpnt_io(**) Power efficiency (0-1) of input to output voltage conversion. */
        GunnsElectPvArray*     mArray;                     /**<    (--)    trick_chkpnt_io(**) Pointer to the PV array link. */
        double                 mVoltageSetpoint;           /**<    (V)                         Setpoint value for the regulated output voltage. */
        double                 mVoltageSetpointDelta;      /**<    (V)                         Nominal delta to voltage setpoint as a separate input command. */
        bool                   mPowered;                   /**<    (--)                        Input power on flag. */
        bool                   mEnabled;                   /**<    (--)                        Input command to enable control of output voltage. */
        bool                   mResetTrips;                /**<    (--)                        Input command to reset all trips. */
        GunnsElectPvRegSensors mSensors;                   /**<    (--)                        Sensors package. */
        GunnsElectPvRegTrips   mTrips;                     /**<    (--)                        Trip logic. */
        PvRegStates            mState;                     /**<    (--)    trick_chkpnt_io(**) Current state of the regulator. */
        double                 mRegulatedVoltage;          /**<    (V)     trick_chkpnt_io(**) Actual regulated output voltage including bias. */
        double                 mInputPower;                /**<    (W)     trick_chkpnt_io(**) Total power load on the PV array. */
        double                 mOutputPower;               /**<    (W)     trick_chkpnt_io(**) Total power to the output channel. */
        double                 mWasteHeat;                 /**<    (W)     trick_chkpnt_io(**) Total waste heat. */
        double                 mMaxRegCurrent;             /**<    (amp)   trick_chkpnt_io(**) Maximum current that can be output at regulated voltage. */
        double                 mStateAdmittance[N_STATES]; /**<    (1/ohm) trick_chkpnt_io(**) Link admittance by state. */
        double                 mStateSource[N_STATES];     /**<    (amp)   trick_chkpnt_io(**) Link source by state. */
        bool                   mStateUpmodeLatch;          /**<    (--)    trick_chkpnt_io(**) Link has transitioned to an improved state this major step. */
        static const double    mMaxAdmittance;             /**< ** (1/ohm) trick_chkpnt_io(**) Maximum link admittance. */
        static const double    mMinAdmittance;             /**< ** (1/ohm) trick_chkpnt_io(**) Minimum non-zero link admittance. */
        /// @brief Validates the initialization of this Gunns Photovoltaic Array Converting Regulator.
        void validate(const GunnsElectPvRegConvConfigData& configData,
                      const GunnsElectPvRegConvInputData&  inputData) const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief Computes the regulated voltage for this pass.
        void updateRegulatedVoltage();
        /// @brief Computes maximum array outputs for this pass and initial array loading.
        void updateMaxOutputs();
        /// @brief Updates the link admittance matrix.
        void buildAdmittanceMatrix();
        /// @brief Updates the link source vector.
        void buildSourceVector();
        /// @brief Performs state transitions based on array and load states.
        bool updateRegulatorState();

    private:
        /// @details Define the number of ports this link class has.  All objects of the same link
        ///          class always have the same number of ports.  We use an enum rather than a
        ///          static const int so that we can reuse the NPORTS name and allow each class to
        ///          define its own value.
        enum {NPORTS = 1};
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectPvRegConv(const GunnsElectPvRegConv& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsElectPvRegConv& operator =(const GunnsElectPvRegConv& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool  (--)  Always true.
///
/// @details  This is always a non-linear link.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectPvRegConv::isNonLinear()
{
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  voltage  (V)  The desired voltage setpoint value.
///
/// @details  Sets the mVoltageSetpoint attribute to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectPvRegConv::setVoltageSetpoint(const double voltage)
{
    mVoltageSetpoint = voltage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  voltage  (V)  The desired voltage setpoint delta value.
///
/// @details  Sets the mVoltageSetpointDelta attribute to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectPvRegConv::setVoltageSetpointDelta(const double voltage)
{
    mVoltageSetpointDelta = voltage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  flag  (--)  True to enable the regulator, false to disable.
///
/// @details  Sets the mEnabled attribute to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectPvRegConv::setEnabled(const bool flag)
{
    mEnabled = flag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double  (amp)  The maximum current that can be output at the regulated voltage.
///
/// @details  Returns the mMaxRegCurrent value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectPvRegConv::getMaxRegCurrent() const
{
    return mMaxRegCurrent;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double  (V)  The current regulated voltage setpoint.
///
/// @details  Returns the mVoltageSetpoint value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectPvRegConv::getVoltageSetpoint() const
{
    return mVoltageSetpoint;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  GunnsElectPvRegTrips*  (--)  Pointer to the trips logic object.
///
/// @details  Returns the address of the mTrips object.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline GunnsElectPvRegTrips* GunnsElectPvRegConv::getTrips()
{
    return &mTrips;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Computes the active regulated voltage level for this pass including the setpoint
///           commands and biases.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectPvRegConv::updateRegulatedVoltage()
{
    mRegulatedVoltage = mVoltageSetpoint + mVoltageSetpointDelta;
    if (mMalfVoltageBiasFlag) {
        mRegulatedVoltage += mMalfVoltageBiasValue;
    }
    mRegulatedVoltage = std::max(mRegulatedVoltage, DBL_EPSILON);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Builds the Admittance Matrix for the link.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectPvRegConv::buildAdmittanceMatrix()
{
    mAdmittanceUpdate    = mStateAdmittance[mState] != mAdmittanceMatrix[0];
    mAdmittanceMatrix[0] = mStateAdmittance[mState];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Builds the Source Vector for the link.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectPvRegConv::buildSourceVector()
{
    mSourceVector[0] = mStateSource[mState];
}

#endif
