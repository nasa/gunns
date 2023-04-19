#ifndef GunnsElectPvRegShunt_EXISTS
#define GunnsElectPvRegShunt_EXISTS

/**
@file
@brief    GUNNS Electrical Photovoltaic Array Shunting Regulator Model declarations

@defgroup  GUNNS_ELECTRICAL_PHOTOVOLTAIC_REG_SHUNT    Photovoltaic Array Shunting Regulator Model
@ingroup   GUNNS_ELECTRICAL_PHOTOVOLTAIC

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the Photovoltaic Array Shunting Regulator GUNNS Link Model.  Refer to the
   GunnsElectPvRegShunt class details below.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsElectPvRegShunt.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2017-10) (Initial))

@{
*/

#include "core/GunnsBasicLink.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "GunnsElectPvRegSensors.hh"
#include "GunnsElectPvRegTrips.hh"
#include "math/MsMath.hh"
#include <vector>

// Forward-declare pointer types.
class GunnsElectPvArray;
class GunnsSensorAnalogWrapper;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Photovoltaic Array Shunting Regulator String Load Order
///
/// @details  This class provides a structure for the Photovoltaic Array Shunting Regulator string
///           load order.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectPvStringLoadOrder
{
    public:
        unsigned int mSection; /**< (--) Array section number of this load order. */
        unsigned int mString;  /**< (--) Section string number of this load order. */
        /// @brief Default constructs this Photovoltaic Array Shunting Regulator String Load Order.
        GunnsElectPvStringLoadOrder(const unsigned int section, const unsigned int string);
        /// @brief Default destructs this Photovoltaic Array Shunting Regulator String Load Order.
        virtual ~GunnsElectPvStringLoadOrder();
        /// @brief Copy constructs this Photovoltaic Array Shunting Regulator String Load Order.
        GunnsElectPvStringLoadOrder(const GunnsElectPvStringLoadOrder& that);
        /// @brief Assignment operator for Photovoltaic Array Shunting Regulator String Load Order.
        GunnsElectPvStringLoadOrder& operator =(const GunnsElectPvStringLoadOrder&);
        /// @brief Equality operator for Photovoltaic Array Shunting Regulator String Load Order.
        bool operator ==(const GunnsElectPvStringLoadOrder&) const;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Photovoltaic Array Shunting Regulator Configuration Data
///
/// @details  This class provides a structure for the Photovoltaic Array Shunting Regulator
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectPvRegShuntConfigData : public GunnsBasicLinkConfigData
{
    public:
        typedef std::vector<GunnsElectPvStringLoadOrder> LoadOrder;    // for readability below
        double                    mOutputConductance;   /**<    (1/ohm) trick_chkpnt_io(**) Conductance of the regulated output. */
        double                    mShuntConductance;    /**<    (1/ohm) trick_chkpnt_io(**) Conductance of each string shunt. */
        GunnsElectPvArray*        mArray;               /**<    (--)    trick_chkpnt_io(**) Pointer to the PV array link. */
        GunnsSensorAnalogWrapper* mInCurrentSensor;     /**<    (--)    trick_chkpnt_io(**) Optional pointer to the input current sensor wrapper. */
        GunnsSensorAnalogWrapper* mInVoltageSensor;     /**<    (--)    trick_chkpnt_io(**) Optional pointer to the input voltage sensor wrapper. */
        GunnsSensorAnalogWrapper* mOutCurrentSensor;    /**<    (--)    trick_chkpnt_io(**) Optional pointer to the output current sensor wrapper. */
        GunnsSensorAnalogWrapper* mOutVoltageSensor;    /**<    (--)    trick_chkpnt_io(**) Optional pointer to the output voltage sensor wrapper. */
        double                    mInOverCurrentTrip;   /**<    (amp)   trick_chkpnt_io(**) Optional input over-current trip limit. */
        double                    mInOverVoltageTrip;   /**<    (V)     trick_chkpnt_io(**) Optional input over-volt trip limit. */
        double                    mOutOverCurrentTrip;  /**<    (amp)   trick_chkpnt_io(**) Optional output over-current trip limit. */
        double                    mOutOverVoltageTrip;  /**<    (V)     trick_chkpnt_io(**) Optional output over-volt trip limit. */
        double                    mOutUnderVoltageTrip; /**<    (V)     trick_chkpnt_io(**) Optional output under-volt trip limit. */
        unsigned int              mTripPriority;        /**<    (--)    trick_chkpnt_io(**) Trip network step priority. */
        LoadOrder                 mStringLoadOrder;     /**< ** (--)    trick_chkpnt_io(**) Optional string loading order. */
        /// @brief Default constructs this Photovoltaic Array Shunting Regulator configuration data.
        GunnsElectPvRegShuntConfigData(const std::string&        name                   = "",
                                       GunnsNodeList*            nodes                  = 0,
                                       const double              outputConductance      = 0.0,
                                       const double              shuntConductance       = 0.0,
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
        /// @brief Default destructs this Photovoltaic Array Shunting Regulator configuration data.
        virtual ~GunnsElectPvRegShuntConfigData();
        /// @brief Adds an entry to the string load order with the given values.
        void addStringLoadOrder(const unsigned int section, const unsigned int string);

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectPvRegShuntConfigData(const GunnsElectPvRegShuntConfigData& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsElectPvRegShuntConfigData& operator =(const GunnsElectPvRegShuntConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Photovoltaic Array Shunting Regulator Input Data
///
/// @details  This class provides a structure for the Photovoltaic Array Shunting Regulator input
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectPvRegShuntInputData : public GunnsBasicLinkInputData
{
    public:
        double mVoltageSetpoint; /**< (V)  trick_chkpnt_io(**) Initial setpoint for the regulated output voltage. */
        bool   mPowered;         /**< (--) trick_chkpnt_io(**) Initial state of power on flag. */
        bool   mEnabled;         /**< (--) trick_chkpnt_io(**) Initial state of enabled, powered and commanded on, etc. */
        double mMinOperatePower; /**< (W)  trick_chkpnt_io(**) Initial minimum bulk power available from array to operate. */
        /// @brief Default constructs this Photovoltaic Array Shunting Regulator input data.
        GunnsElectPvRegShuntInputData(const double voltageSetpoint = 0.0,
                                      const bool   powered         = false,
                                      const bool   enabled         = false,
                                      const double minOperatePower = 0.0);
        /// @brief Default destructs this Photovoltaic Array Shunting Regulator input data.
        virtual ~GunnsElectPvRegShuntInputData();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectPvRegShuntInputData(const GunnsElectPvRegShuntInputData& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsElectPvRegShuntInputData& operator =(const GunnsElectPvRegShuntInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Photovoltaic Array Shunting Regulator
///
/// @details  This models an output voltage regulator for a GunnsElectPvArray that uses shunting of
///           array strings to control the output voltage, similar to the Sequential Shunting Units
///           (SSU) in the ISS.
///
///           In this shunting control scheme, some of the array strings are tied to the regulator
///           output channel until the demanded power load from downstream is met and the desired
///           voltage setpoint begins to be exceeded -- then the remaining strings are shunted
///           (short-circuited) to reduce their voltage and wasted power to safe levels.
///
///           This link allows the order in which the strings are loaded & shunted to be customized,
///           or the order can be defaulted to the same order they're defined in the attached array.
///
///           This link has an execution order dependency on the GunnsElectPvArray it interfaces
///           with.  This link must be initialized and stepped after the array.  This order can be
///           established with proper setup in the GUNNS network.
///
///           This is a two-port GUNNS link:
///           - Port 0 is the input node, shared with the array output.
///           - Port 1 is the output node to the downstream circuit, whose voltage is controlled.
///
///           This link has 2 operating states, which it automatically switches between:
///           - REG: when the array can meet the demanded power load from the downstream circuit,
///                  the output voltage is regulated and the above string loading & shunting scheme
///                  is used.  This link acts like a potential source on the output node, loads the
///                  array strings individually, a total load is placed on the input node that
///                  approximates the average array load, and the two nodes are not directly
///                  connected.
///           - OFF: when the link is disabled by command input.  All strings are shunted and the
///                  output node is isolated and unregulated.
///
///           A 3rd state, SAG, used to be modeled:
///           - SAG: when the array can't meet the demanded power load at the regulated voltage,
///                  then the array is connected directly to the output node thru this link, and
///                  the output voltage will drop below the regulated value.  We've stopped modeling
///                  this condition because of instabilities in complicated networks, and instead
///                  just transition to OFF mode when the array can't meet the demanded power.
///
///           This link allows optional sensors for input and output voltage and current.  Optional
///           trip functions can also be used with or without the sensors.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectPvRegShunt : public GunnsBasicLink
{
    TS_MAKE_SIM_COMPATIBLE(GunnsElectPvRegShunt);

    public:
        /// @name    Public and Malfunction attributes.
        /// @{
        /// @details These objects and Malfunction targets are public to allow access from the Trick
        ///          events processor.
        bool                 mMalfVoltageBiasFlag;  /**< (--) Malfunction activation flag for regulated voltage bias. */
        double               mMalfVoltageBiasValue; /**< (V)  Regulated voltage bias malfunction value. */
        GunnsTripGreaterThan mInputOverVoltTrip;    /**< (--) Input over-volt trip functions. */
        GunnsTripLessThan    mOutputUnderVoltTrip;  /**< (--) Output under-volt trip functions. */
        /// @}

        /// @brief Enumeration of the Photovoltaic Array Shunting Regulator states.
        enum PvRegStates {
            OFF = 0,    ///< Powered off or otherwise inactive.
            REG = 1,    ///< On and actively regulating output voltage and PV load.
            SAG = 2     ///< (No longer used.)
        };

        /// @brief Default Constructor.
        GunnsElectPvRegShunt();
        /// @brief Default constructor.
        virtual ~GunnsElectPvRegShunt();
        /// @brief Initialize method.
        void initialize(const GunnsElectPvRegShuntConfigData& configData,
                        const GunnsElectPvRegShuntInputData&  inputData,
                        std::vector<GunnsBasicLink*>&         networkLinks,
                        const int                             port0,
                        const int                             port1);
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
        /// @brief Sets the regulator enabled flag to the given value.
        void   setEnabled(const bool flag);
        /// @brief Returns the maximum current that can be output at the regulated voltage.
        double getMaxRegCurrent() const;
        /// @brief Returns the minimum power available from the array before regulator can operate.
        double getMinOperatePower() const;
        /// @brief Sets the minimum power available from the array before regulator can operate.
        void   setMinOperatePower(const double value);
        /// @brief Returns the current regulated voltage setpoint.
        double getVoltageSetpoint() const;
        /// @brief Returns a pointer to the trip logic group.
        GunnsElectPvRegTrips* getTrips();

    protected:
        typedef std::vector<GunnsElectPvStringLoadOrder> LoadOrder;    // for readability below
        double                 mOutputConductance; /**<    (1/ohm) trick_chkpnt_io(**) Conductance of the regulated output. */
        double                 mShuntConductance;  /**<    (1/ohm) trick_chkpnt_io(**) Conductance of each string shunt. */
        GunnsElectPvArray*     mArray;             /**<    (--)    trick_chkpnt_io(**) Pointer to the PV array link. */
        unsigned int           mTripPriority;      /**<    (--)    trick_chkpnt_io(**) Trip network step priority. */
        LoadOrder              mStringLoadOrder;   /**< ** (--)    trick_chkpnt_io(**) String loading order. */
        double                 mVoltageSetpoint;   /**<    (V)                         Setpoint value for the regulated output voltage. */
        bool                   mPowered;           /**<    (--)                        Input power on flag. */
        bool                   mEnabled;           /**<    (--)                        Regulator is enabled, powered and commanded on, etc. */
        double                 mMinOperatePower;   /**<    (W)                         Minimum bulk power available from array to operate. */
        bool                   mResetTrips;        /**<    (--)                        Input command to reset all trips. */
        GunnsElectPvRegSensors mSensors;           /**<    (--)                        Sensors package. */
        GunnsElectPvRegTrips   mTrips;             /**<    (--)                        Trip logic. */
        PvRegStates            mState;             /**<    (--)    trick_chkpnt_io(**) Current state of the regulator. */
        double                 mRegulatedVoltage;  /**<    (V)     trick_chkpnt_io(**) Actual regulated output voltage including bias. */
        double                 mInputConductance;  /**<    (1/ohm) trick_chkpnt_io(**) Conductance load on the PV array. */
        double                 mShuntPower;        /**<    (W)     trick_chkpnt_io(**) Total shunt power and waste heat from all shunted strings. */
        double                 mInputPower;        /**<    (W)     trick_chkpnt_io(**) Total power load on the PV array. */
        double                 mOutputPower;       /**<    (W)     trick_chkpnt_io(**) Total power to the output channel. */
        double                 mWasteHeat;         /**<    (W)     trick_chkpnt_io(**) Total waste heat. */
        double                 mPvBulkPowerAvail;  /**<    (W)     trick_chkpnt_io(**) Available power from the PV array bulk model. */
        double                 mMaxRegCurrent;     /**<    (amp)   trick_chkpnt_io(**) Maximum current that can be output at regulated voltage. */
        bool                   mOffToRegOccurred;  /**<    (--)    trick_chkpnt_io(**) Transition from OFF to REG states has occurred this major frame. */
        /// @brief Validates the initialization of this Gunns Photovoltaic Array Shunting Regulator.
        void validate(const GunnsElectPvRegShuntConfigData& configData,
                      const GunnsElectPvRegShuntInputData&  inputData) const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief Computes the regulated voltage for this pass.
        void updateRegulatedVoltage();
        /// @brief Computes maximum array outputs for this pass and initial array loading.
        void updateMaxOutputs();
        /// @brief Performs state transitions based on new input states.
        void updateInitialState();
        /// @brief Updates the link admittance matrix.
        void buildAdmittanceMatrix();
        /// @brief Updates the link source vector.
        void buildSourceVector();
        /// @brief Computes the output flux.
        void computeFlux();
        /// @brief Loads the PV array strings with demanded power.
        void loadArray(const double power);
        /// @brief Returns whether the regulator is operating.
        bool isOperating() const;

    private:
        /// @details Define the number of ports this link class has.  All objects of the same link
        ///          class always have the same number of ports.  We use an enum rather than a
        ///          static const int so that we can reuse the NPORTS name and allow each class to
        ///          define its own value.
        enum {NPORTS = 2};
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectPvRegShunt(const GunnsElectPvRegShunt& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsElectPvRegShunt& operator =(const GunnsElectPvRegShunt& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool  (--)  Always true.
///
/// @details  This is always a non-linear link.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectPvRegShunt::isNonLinear()
{
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  voltage  (V)  The desired voltage setpoint value.
///
/// @details  Sets the mVoltageSetpoint attribute to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectPvRegShunt::setVoltageSetpoint(const double voltage)
{
    mVoltageSetpoint = voltage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  flag  (--)  True to enable the regulator, false to disable.
///
/// @details  Sets the mEnabled attribute to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectPvRegShunt::setEnabled(const bool flag)
{
    mEnabled = flag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double  (amp)  The maximum current that can be output at the regulated voltage.
///
/// @details  Returns the mMaxRegCurrent value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectPvRegShunt::getMaxRegCurrent() const
{
    return mMaxRegCurrent;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double  (W)  The minimum power available from the array before regulator can operate.
///
/// @details  Returns the mMinOperatePower value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectPvRegShunt::getMinOperatePower() const
{
    return mMinOperatePower;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  value  (W)  The minimum power available from the array before regulator can operate.
///
/// @details  Sets the mMinOperatePower attribute to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectPvRegShunt::setMinOperatePower(const double value)
{
    mMinOperatePower = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double  (V)  The current regulated voltage setpoint.
///
/// @details  Returns the mVoltageSetpoint value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectPvRegShunt::getVoltageSetpoint() const
{
    return mVoltageSetpoint;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  GunnsElectPvRegTrips*  (--)  Pointer to the trips logic object.
///
/// @details  Returns the address of the mTrips object.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline GunnsElectPvRegTrips* GunnsElectPvRegShunt::getTrips()
{
    return &mTrips;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Computes the active regulated voltage level for this pass including the setpoint
///           command and biases.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectPvRegShunt::updateRegulatedVoltage()
{
    mRegulatedVoltage = mVoltageSetpoint;
    if (mMalfVoltageBiasFlag) {
        mRegulatedVoltage += mMalfVoltageBiasValue;
    }
    mRegulatedVoltage = std::max(mRegulatedVoltage, DBL_EPSILON);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Builds the Source Vector for the link.  The behavior depends on state.  In REG state,
///           this link creates a potential source on the output node at the regulated voltage.  In
///           the SAG & OFF states, there is no source.  There is never a source on the input node.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectPvRegShunt::buildSourceVector()
{
    switch (mState) {
        case REG:
            mSourceVector[1] = mRegulatedVoltage * mOutputConductance;
            break;
        default:    // SAG, OFF, or invalid
            mSourceVector[1] = 0.0;
    }
    mSourceVector[0] = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Computes the output flux resulting from the network solution.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectPvRegShunt::computeFlux()
{
    switch (mState) {
        case REG: {
            mFlux = (mRegulatedVoltage - mPotentialVector[1]) * mAdmittanceMatrix[3];
        } break;
        case SAG: {
            ; // do nothing, SAG no longer modeled
        } break;
        default: { // OFF or invalid
            mFlux = 0.0;
        } break;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns bool (--) True if the regulator is operating.
///
/// @details  The regulator is operating if it is powered, enabled, and not tripped.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectPvRegShunt::isOperating() const
{
    return (mPowered and mEnabled and not mTrips.isTripped());
}

#endif
