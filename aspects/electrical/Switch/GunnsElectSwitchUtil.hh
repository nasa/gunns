#ifndef GunnsElectSwitchUtil_EXISTS
#define GunnsElectSwitchUtil_EXISTS

/*
@defgroup TSM_GUNNS_ELECTRICAL_SWITCH_MODEL Electrical Switch Utility Model
@ingroup  TSM_GUNNS_ELECTRICAL_SWITCH

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@{

PURPOSE:
(This is a copy of the old Switch class, renamed to eliminate name collisions on 'Switch'.
 The GunnsElectSwitchUtil class emulates an electrical physical or transistor switch.  The switch
 can be configured to trip open at variable electrical current levels in either direction.The
 current trip function can also be disabled so the switch can handle infinite current. The switch
 can be set to trip open below a certain voltage threshold. The under-voltage trip function
 can also be disabled.  The switch will (eventually) incorporate a trip priority function to
 model the staggered trip times in some electrical networks, such as in the International
 Space Station.  This system will ensure that the switch closest to a short circuit or trip
 condition is the one that opens, minimizing the impact to the overall network as intended.)

REQUIREMENTS:
()

REFERENCE:
()

ASSUMPTIONS AND LIMITATIONS:
()

LIBRARY DEPENDENCY:
(
 (GunnsElectSwitchUtil.o)
)

PROGRAMMERS:
(
 ((Jason Harvey) (CACI) (December 2019) (Initial))
)
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include <string>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GunnsElectSwitchUtil Configuration Data.
///
/// @details  This provides a data structure for the GunnsElectSwitchUtil configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectSwitchUtilConfigData {
    public:
        double mDefaultSwitchResistance; /**< (ohm) trick_chkpnt_io(**) Nominal resistance. */
        bool   mOverCurrentProtection;   /**< (--)  trick_chkpnt_io(**) flag to enable over current protection */
        bool   mUnderVoltProtection;     /**< (--)  trick_chkpnt_io(**) flag to enable under voltage protection */
        double mMinVoltage;              /**< (V)   trick_chkpnt_io(**) minimum working input voltage */
        bool   mIsTwoPortSwitch;         /**< (--)  trick_chkpnt_io(**) flag for whether this is a switch that feeds other RPCMs */
        int    mPortAssigned;            /**< (--)  trick_chkpnt_io(**) for two port switches, which network port do I talk to? */
        int    mTripPriority;            /**< (--)  trick_chkpnt_io(**) minor frame that this switch is allowed to trip at to ensure that a series of switches trips in the correct order */
        bool   mPortsAreReversed;        /**< (--)  trick_chkpnt_io(**) if true lets the switchcard know to reverse the ports when calculating current flow.*/
        /// @brief Default GunnsElectSwitchUtil Configuration Data Constructor.
        GunnsElectSwitchUtilConfigData(const double defaultSwitchResistance = 0.0668,
                                       const bool   overCurrentProtection   = true,
                                       const bool   underVoltProtection     = true,
                                       const double minVoltage              = 110.0,
                                       const bool   isTwoPortSwitch         = false,
                                       const int    portAssigned            = 0,
                                       const int    tripPriority            = 1,
                                       const bool   reversed                = false);
        /// @brief GunnsElectSwitchUtil configuration destructor.
        virtual ~GunnsElectSwitchUtilConfigData();
        /// @brief GunnsElectSwitchUtil Configuration Data Initialize.
        void init(const double defaultSwitchResistance,
                  const bool   overCurrentProtection,
                  const bool   underVoltProtection,
                  const double minVoltage,
                  const bool   isTwoPortSwitch,
                  const int    portAssigned,
                  const int    tripPriority,
                  const bool   reversed = false);
        /// @brief GunnsElectSwitchUtil configuration copy constructor.
        GunnsElectSwitchUtilConfigData(const GunnsElectSwitchUtilConfigData& that);
        /// @brief GunnsElectSwitchUtil Configuration Data assignment operator.
        GunnsElectSwitchUtilConfigData& operator =(const GunnsElectSwitchUtilConfigData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GunnsElectSwitchUtil Input Data.
///
/// @details  This class provides a data structure for the GunnsElectSwitchUtil input data.
///           Input data are model characteristics that may change over the course
///           of a sim.  Current through a switch is considered positive if it is flowing from
///           port 0 to port 1, and negative if it is flowing from port 1 to port 0 of the
///           network link object that the switch is in.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectSwitchUtilInputData {
    public:
        bool   mSwitchMalfFlag;     /**< (--) trick_chkpnt_io(**) */
        int    mSwitchMalfValue;    /**< (--) trick_chkpnt_io(**) */
        bool   mSwitchIsAutoClosed; /**< (--) trick_chkpnt_io(**) */
        double mPosTripLimit;       /**< (--) trick_chkpnt_io(**) */
        double mNegTripLimit;       /**< (--) trick_chkpnt_io(**) */
        /// @brief Default GunnsElectSwitchUtil Input Data Constructor.
        GunnsElectSwitchUtilInputData(const bool   switchMalfFlag  = false,
                                      const int    switchMalfValue = 0,
                                      const bool   switchIsClosed  = false,
                                      const double posTripLimit    = 3.5,
                                      const double negTripLimit    = -0.1);
        /// @brief Default GunnsElectSwitchUtil Input DataDestructor.
        virtual ~GunnsElectSwitchUtilInputData();
        /// @brief GunnsElectSwitchUtil Input Data Initialize.
        void init(const bool   switchMalfFlag,
                  const int    switchMalfValue,
                  const bool   switchIsClosed,
                  const double posTripLimit,
                  const double NegTripLimit);
        /// @brief Copy constructs GunnsElectSwitchUtil Input Data.
        GunnsElectSwitchUtilInputData(const GunnsElectSwitchUtilInputData& that);
        /// @brief GunnsElectSwitchUtil Input Data assignment operator.
        GunnsElectSwitchUtilInputData& operator=(const GunnsElectSwitchUtilInputData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS electrical switch utility model.
///
/// @details  Refer to PURPOSE above.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectSwitchUtil {
    TS_MAKE_SIM_COMPATIBLE(GunnsElectSwitchUtil);
    public:
        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool   mMalfFailClosed;               /**< (--) flag to fail switch closed*/
        bool   mMalfFailOpen;                 /**< (--) flag to fail switch open*/
        bool   mMalfFailResistance;           /**< (--) flag to activate switch resistance malf*/
        double mMalfFailResistPercent;        /**< (--) value to scale the switch resistance by*/
        bool   mMalfPosTripSetpointFail;      /**< (--) flag to activate switch positive trip set point malf*/
        double mMalfPosTripSetpointFailValue; /**< (--) new value for trip setpoint*/
        bool   mMalfNegTripSetpointFail;      /**< (--) flag to activate switch negative trip set point malf*/
        double mMalfNegTripSetpointFailValue; /**< (--) new value for trip setpoint*/
        bool   mMalfIgnoreCurrentTrip;        /**< (--) malf to prevent switch from tripping in event of over-current */
        /// @}
        /// @brief
        GunnsElectSwitchUtil();
        /// @brief
        virtual ~GunnsElectSwitchUtil();
        /// @brief
        void initialize(const GunnsElectSwitchUtilConfigData& configData,
                        const GunnsElectSwitchUtilInputData&  inputData,
                        const std::string                     switchCardName = "NONE",
                        const int                             switchPosition = 0);
        /// @brief
        void updateSwitchState(const bool powerSupplyValid);
        /// @brief
        void updateSwitchFlow(const double currentActual,
                const double currentSensed,
                const double voltage,
                const int    currentStep,
                const bool   isMinorStep);
        /// @brief
        void setPosTripLimit(const double posTripLimit);
        /// @brief
        void setNegTripLimit(const double negTripLimit);
        /// @brief
        void setSwitchCommandedClosed(const bool switchCommandedClosed);
        /// @brief
        void setTripReset();
        /// @brief
        void setExternalTrip(const bool externalTripFlag);
        /// @brief
        double getCurrent() const;
        /// @brief
        double getConductance() const;
        /// @brief
        double getPowerDissipation() const;
        /// @brief
        int getPortAssigned() const;
        /// @brief
        double getOutputVolts() const;
        /// @brief
        double getInputVolts() const;
        /// @brief
        double getPosTripLimit() const;
        /// @brief
        double getNegTripLimit() const;
        /// @brief
        bool isPortsReversed() const;
        /// @brief
        bool isClosed() const;
        /// @brief
        bool isCommandedClosed() const;
        /// @brief
        bool isTripped() const;
        /// @brief
        bool isPosTrip() const;
        /// @brief
        bool isNegTrip() const;
        /// @brief
        bool isWaitingToTrip() const;
        /// @brief
        bool isJustTripped() const;
        /// @brief
        bool isTwoPortSwitch() const;
        /// @brief Sets and resets the switch fail closed malfunction.
        void setMalfFailClosed(const bool flag = false);
        /// @brief Sets and resets the switch fail closed malfunction.
        void setMalfFailOpen(const bool flag = false);
        /// @brief Sets and resets the switch resistance malfunction.
        void setMalfFailResistance(const bool flag = false, const double value = 0.0);
        /// @brief Sets and resets the positive trip setpoint malfunction.
        void setMalfPosTripSetpoint(const bool flag = false, const double value = 0.0);
        /// @brief Sets and resets the negative trip setpoint malfunction.
        void setMalfNegTripSetpoint(const bool flag = false, const double value = 0.0);
        /// @brief Returns whether this switch is initialized.
        bool isInitialized() const;

    protected:
        double mCurrentActual;               /**<    (amp) actual current passing through the switch */
        double mCurrentSensed;               /**<    (amp) current value from sensor on the switch */
        double mVoltageIn;                   /**<    (V)   Switch input voltage*/
        double mPowerDissipation;            /**<    (W)   Power consumed by the internal switch resistance*/
        bool   mSwitchCommandedClosed;       /**<    (--)  Switch commanded position, TRUE = close, FALSE = open*/
        bool   mSwitchIsClosed;              /**<    (--)  Switch actual position, TRUE = closed; FALSE = open*/
        bool   mPosTrip;                     /**<    (--)  Switch positive trip status, TRUE = tripped open, FALSE = not tripped*/
        bool   mNegTrip;                     /**<    (--)  Switch Negative trip status, TRUE = tripped open, FALSE = not tripped*/
        bool   mExternalTrip;                /**<    (--)  Switch trip from some logic in the switch-owning object (example: DCSU RBI 1 over-voltage trip) */
        bool   mWaitingToTrip;               /**<    (--)  This switch needs to trip, but can't yet because of trip priority*/
        bool   mJustTripped;                 /**<    (--)  capture when the switch transitions from waitingToTrip to Tripped*/
        int    mTripPriority;                /**<    (--)  trick_chkpnt_io(**) after which minor step is this switch allowed to trip*/
        bool   mIsTwoPortSwitch;             /**<    (--)  trick_chkpnt_io(**) flag for whether this switch powers other RPCMs*/
        int    mCurrentMinorStep;            /**<    (--)  which minor step am I at*/
        bool   mTripReset;                   /**<    (--)  flag to reset switch trip flags*/
        double mDefaultSwitchResistance;     /**<    (ohm) trick_chkpnt_io(**) Nominal value*/
        double mActiveSwitchResistance;      /**<    (ohm) switch resistance accounting for malfunctions*/
        bool   mOverCurrentProtection;       /**<    (--)  trick_chkpnt_io(**) flag for whether a switch trips at all*/
        bool   mConfigDataCurrentProtection; /**<    (--)  keep track of trip protection for when 'ignore current trips' malf is removed, so it can be set back */
        bool   mLastMalfIgnoreCurrentTrip;   /**<    (--)  previous state of malf to prevent switch from tripping in event of over-current */
        double mDefaultPosTripLimit;         /**<    (amp) trip point for current out of the bus/from port 0 to port 1*/
        double mActivePosTripLimit;          /**<    (amp) trip point accounting for malfunction*/
        double mDefaultNegTripLimit;         /**<    (amp) trip point for current in to the bus/from port 1 to port 0*/
        double mActiveNegTripLimit;          /**<    (amp) trip point accounting for malfunction*/
        int    mPortAssigned;                /**<    (--)  trick_chkpnt_io(**) for two port switches which port do I talk to?  0 for userLoadSwitches*/
        int    mSwitchStateEnum;             /**<    (--)  an enumeration to use on display pages for the state's open/close/malf'd state */
        std::string mSwitchCardName;         /**<    (--)  what switchcard am I in? */
        int    mSwitchCardPosition;          /**<    (--)  which switch in the switchcard am I? */
        bool   mPortsAreReversed;            /**<    (--)  trick_chkpnt_io(**) if true lets switch card know to reverse the ports when calculating current flow.*/
        bool   mInitFlag;                    /**< *o (--)  trick_chkpnt_io(**) True if this switch has been initialized. */
        static const double mConductanceLimit;
        /// @brief
        GunnsElectSwitchUtil(const GunnsElectSwitchUtil& that);
        /// @brief
        GunnsElectSwitchUtil& operator =(const GunnsElectSwitchUtil& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method: isPortsReversed        -- return bool whether ports are reversed
///
/// @return   bool                    -- TRUE if reversed, FALSE if not
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectSwitchUtil::isPortsReversed() const {
    return mPortsAreReversed;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method: isClosed        -- return the current switch state (open/closed)
///
/// @return   bool                    -- TRUE if switch is closed, FALSE if switch is open
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectSwitchUtil::isClosed() const {
    return mSwitchIsClosed;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method: isCommandedClosed   -- return the commanded switch state (open/closed)
///
/// @return   bool                        -- TRUE if switch is supposed to be closed, FALSE if switch is supposed to be open
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectSwitchUtil::isCommandedClosed() const {
    return mSwitchCommandedClosed;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method: getCurrent        -- return the current through the switch
///
/// @return double                      -- amps going through the switch
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectSwitchUtil::getCurrent() const {
    return mCurrentActual;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method: getConductance        -- return the current switch conductance
///
/// @return double                          -- conductance value (1 / resistance) of the switch
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectSwitchUtil::getConductance() const {
    double conductance = 0.0;
    if (mSwitchIsClosed) {
        if(mActiveSwitchResistance > 0.0) {
            conductance = 1.0 / mActiveSwitchResistance;
        } else {
            return mConductanceLimit;
        }
    }
    return conductance;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method: getPowerDissipation        -- return the current switch power dissipation
///
/// @return double                               -- power lost by the switch itself
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectSwitchUtil::getPowerDissipation() const {
    return mPowerDissipation;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method: getPortAssigned      -- return the network port this two port switch talks to
///
/// @return int                            -- port this switch is connected to (one end always connected to port 0)
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int GunnsElectSwitchUtil::getPortAssigned() const {
    return mPortAssigned;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method: isTripped      TRUE if switch has any trip active
///
/// @return bool                     whether this switch is tripped open or not
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectSwitchUtil::isTripped() const {
    //return (mPosTrip || mNegTrip || mVoltTrip);
    return (mPosTrip || mNegTrip || mExternalTrip);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Method: isPosTrip    TRUE if switch had a positive current trip (from port 0 to port 1)
///
/// @return bool                  whether this switch is open due to too much current in the positive direction
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectSwitchUtil::isPosTrip() const {
    return mPosTrip;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Method: isNegTrip    TRUE if switch had a negative current trip (from port 1 to port 0)
///
/// @return bool                  whether this switch is open due to too much current in the negative direction
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectSwitchUtil::isNegTrip() const {
    return mNegTrip;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method: isWaitingToTrip    TRUE if switch has any trip active but can't trip on this minorStep
///
/// @return bool                         flag for whether this switch is in a trip state but can't open yet because of trip staging
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectSwitchUtil::isWaitingToTrip() const {
    return mWaitingToTrip;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method: isJustTripped    TRUE if switch has just transitioned from waitingToTrip to Tripped
///
/// @return bool                       flag for whether this switch has just tripped
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectSwitchUtil::isJustTripped() const {
    return mJustTripped;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Method: isTwoPortSwitch   TRUE if this is a switch that powers another network object
///
/// @return bool                        whether this is a switch that powers another network object
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectSwitchUtil::isTwoPortSwitch() const {
    return mIsTwoPortSwitch;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Method: getOutputVolts   returns switch output voltage (0.0 if open, mVoltage if closed)
///
/// @return double                    switch output voltage
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectSwitchUtil::getOutputVolts() const {
    if (mSwitchIsClosed) {
        return mVoltageIn;
    } else {
        return 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Method: getInputVolts   returns switch input voltage
///
/// @return double                   switch input voltage
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectSwitchUtil::getInputVolts() const {
    return mVoltageIn;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Method: getPosTripLimit   returns switch nominal positive current trip limit
///
/// @return double                   switch nominal positive current trip limit
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectSwitchUtil::getPosTripLimit() const {
    return mDefaultPosTripLimit;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Method: getNegTripLimit   returns switch nominal negative current trip limit
///
/// @return double                   switch nominal negative current trip limit
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectSwitchUtil::getNegTripLimit() const {
    return mDefaultNegTripLimit;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Returns mInitFlag
///
/// @return  bool
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectSwitchUtil::isInitialized() const {
    return mInitFlag;
}

#endif
