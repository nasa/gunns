#ifndef Switch_EXISTS
#define Switch_EXISTS

/// @defgroup TSM_GUNNS_ELECTRICAL_SWITCH_MODEL Model
/// @ingroup  TSM_GUNNS_ELECTRICAL_SWITCH
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
/// @{
/*********************** TRICK HEADER ************************************************************

 PURPOSE:
 (The Switch class emulates an electrical switch, either physical or transistor.  The switch
  can be configured to trip open at variable electrical current levels in either direction.The
  current trip function can also be disabled so the switch can handle infinite current. The switch
  can be set to trip open below a certain voltage threshold. The under-voltage trip function
  can also be disabled.  The switch will (eventually) incorporate a trip priority function to
  model the staggered trip times in some electrical networks, such as in the International
  Space Station.  This system will ensure that the switch closest to a short circuit or trip
  condition is the one that opens, minimizing the impact to the overall network as intended.
 )

 REQUIREMENTS:
 (none)

 REFERENCE:
 (
  (RJ00200 RPCM hardware specification)
  (RJ00204 DCSU hardware specification)
  (RJ00111 MBSU hardware specification)
 )

 ASSUMPTIONS AND LIMITATIONS:
 (For now the Trip timing/staging logic is non functional.  Switches will always trip on the
  first minor step, and in a situation where one switch feeds another switch, a short circuit
  resulting in a high current will be seen by both switches in the chain and they will both
  trip open at the same time.)

 LIBRARY DEPENDENCY:
 (
  (Switch.o)
 )

 PROGRAMMERS:
 (
  (Minh Tran) (L-3) (Oct 2011) (Initial Prototype)
  (Philip Trottman) (USA) (Dec 2011) (Expansions for SSTF)
  (Nicholas Kaufmann) (L-3) (Mar 2014) (Added switch reverse)
 )

 ************************************************************************************************/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include <string>

//////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Switch Configuration Data
/// @details  The  purpose of this class is to provide a data structure for the Switch object
///           configuration data.
//////////////////////////////////////////////////////////////////////////////////////////////////
class SwitchConfigData {
  public:
    double mDefaultSwitchResistance;  /**< (ohm) trick_chkpnt_io(**) Nominal resistance. */
    bool mOverCurrentProtection;      /**< (--) trick_chkpnt_io(**) flag to enable over current protection */
    bool mUnderVoltProtection;        /**< (--) trick_chkpnt_io(**) flag to enable under voltage protection */
    double mMinVoltage;               /**< (V)  trick_chkpnt_io(**) minimum working input voltage */
    bool mIsTwoPortSwitch;            /**< (--) trick_chkpnt_io(**) flag for whether this is a switch that feeds other RPCMs */
    int mPortAssigned;                /**< (--) trick_chkpnt_io(**) for two port switches, which network port do I talk to? */
    int mTripPriority;                /**< (--) trick_chkpnt_io(**) minor frame that this switch is allowed to trip at to ensure that a series of switches trips in the correct order */
    bool mPortsAreReversed;           /**< (--) trick_chkpnt_io(**) if true lets the switchcard know to reverse the ports when calculating current flow.*/

    /// @brief Default Switch Configuration Data Constructor
    SwitchConfigData(const double defaultSwitchResistance = 0.0668,
            const bool overCurrentProtection = true,
            const bool underVoltProtection = true,
            const double minVoltage = 110.0,
            const bool isTwoPortSwitch = false,
            const int portAssigned = 0,
            const int tripPriority = 1,
            const bool reversed = false);

    /// @brief Switch configuration destructor
    virtual ~SwitchConfigData();

    /// @brief Switch Configuration Data Initialize
    void init(const double defaultSwitchResistance,
            const bool overCurrentProtection,
            const bool underVoltProtection,
            const double minVoltage,
            const bool isTwoPortSwitch,
            const int portAssigned,
            const int tripPriority,
            const bool reversed = false);

    /// @brief Switch configuration copy constructor
    SwitchConfigData(const SwitchConfigData& that);

    /// @brief Switch Configuration Data assignment operator
    SwitchConfigData& operator =(const SwitchConfigData& that);
};

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Switch Input Data
/// @details  The  purpose of this class is to provide a data structure for the Switch object
///           input data.  Input data are model characteristics that may change over the course
///           of a sim.  Current through a switch is considered positive if it is flowing from
///           port 0 to port 1, and negative if it is flowing from port 1 to port 0 of the
///           network link object that the switch is in.
/////////////////////////////////////////////////////////////////////////////////////////////////
class SwitchInputData {
  public:

    bool mSwitchMalfFlag;  /**< (--) trick_chkpnt_io(**)  not used but here to not break GUNNSHOW*/
    int mSwitchMalfValue;  /**< (--) trick_chkpnt_io(**)  not used but here to not break GUNNSHOW*/
    bool mSwitchIsAutoClosed;  /**< (--) trick_chkpnt_io(**) */
    double mPosTripLimit;  /**< (--) trick_chkpnt_io(**) */
    double mNegTripLimit;  /**< (--) trick_chkpnt_io(**) */

    /// @brief Default Switch Input Data Constructor
    SwitchInputData(const bool switchMalfFlag = false,
            const int switchMalfValue = 0,
            const bool switchIsClosed = false,
            const double posTripLimit = 3.5,
            const double negTripLimit = -0.1);

    /// @brief Default Switch Input DataDestructor
    virtual ~SwitchInputData();

    /// @brief Switch Input Data Initialize
    void init(const bool switchMalfFlag,
            const int switchMalfValue,
            const bool switchIsClosed,
            const double posTripLimit,
            const double NegTripLimit);

    /// @brief Copy constructs Switch Input Data.
    SwitchInputData(const SwitchInputData& that);

    /// @brief Switch Input Data assignment operator
    SwitchInputData& operator=(const SwitchInputData& that);
};

//////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Switch
/// @details  The  purpose of this class is to provide a data structure for the Switch object.
//////////////////////////////////////////////////////////////////////////////////////////////////
class Switch {
    TS_MAKE_SIM_COMPATIBLE(Switch);
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

    /// @details Deprecated due to obsolescence by GunnsElectSwitchUtil.
    Switch() __attribute__ ((deprecated));
    virtual ~Switch();

    void initialize(const SwitchConfigData& configData,
                    const SwitchInputData& inputData,
                    const std::string switchCardName = "NONE",
                    const int switchPosition = 0);

    void updateSwitchState(const bool powerSupplyValid);
//    void updateSwitchFlow(const double current,
//                          const double voltageActual,
//                          const double voltageSensed,
//                          const int    currentStep,
//                          const bool   isMinorStep,
//                          const bool   undervoltFdirActive);
    void updateSwitchFlow(const double currentActual,
                          const double currentSensed,
                          const double voltage,
                          const int    currentStep,
                          const bool   isMinorStep);

    void setPosTripLimit(const double posTripLimit);
    void setNegTripLimit(const double negTripLimit);
    void setSwitchCommandedClosed(const bool switchCommandedClosed);
    void setTripReset();
    void setExternalTrip(const bool externalTripFlag);
    // void clearVoltTrip();

    double getCurrent() const;
    double getConductance() const;
    double getPowerDissipation() const;
    int getPortAssigned() const;
    double getOutputVolts() const;
    double getInputVolts() const;
    double getPosTripLimit() const;
    double getNegTripLimit() const;
    bool isPortsReversed() const;
    bool isClosed() const;
    bool isCommandedClosed() const;
    bool isTripped() const;
    // bool isVoltTrip() const;
    bool isPosTrip() const;
    bool isNegTrip() const;
    bool isWaitingToTrip() const;
    bool isJustTripped() const;
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

    double mCurrentActual;            /**< (amp)    actual current passing through the switch */
    double mCurrentSensed;            /**< (amp)    current value from sensor on the switch */
    double mVoltageIn;                /**< (V)      Switch input voltage*/
    double mPowerDissipation;         /**< (W)      power consumed by the internal switch resistance*/
    bool mSwitchCommandedClosed;      /**< (--)     Switch commanded position, TRUE = close, FALSE = open*/
    bool mSwitchIsClosed;             /**< (--)     Switch actual position, TRUE = closed; FALSE = open*/
    bool mPosTrip;                    /**< (--)     Switch positive trip status, TRUE = tripped open, FALSE = not tripped*/
    bool mNegTrip;                    /**< (--)     Switch Negative trip status, TRUE = tripped open, FALSE = not tripped*/
    bool mExternalTrip;               /**< (--)     Switch trip from some logic in the switch-owning object (example: DCSU RBI 1 over-voltage trip) */
    bool mWaitingToTrip;              /**< (--)     This switch needs to trip, but can't yet because of trip priority*/
    bool mJustTripped;                /**< (--)     Capture when the switch transitions from waitingToTrip to Tripped*/
    int mTripPriority;                /**< (--)     trick_chkpnt_io(**) after which minor step is this switch allowed to trip*/
    bool mIsTwoPortSwitch;            /**< (--)     trick_chkpnt_io(**) flag for whether this switch powers other RPCMs*/
    int mCurrentMinorStep;            /**< (--)     which minor step am I at*/
    bool mTripReset;                  /**< (--)     flag to reset switch trip flags*/
    double mDefaultSwitchResistance;  /**< (ohm)    trick_chkpnt_io(**) Nominal value*/
    double mActiveSwitchResistance;   /**< (ohm)    switch resistance accounting for malfunctions*/
    bool mOverCurrentProtection;      /**< (--)     trick_chkpnt_io(**) flag for whether a switch trips at all*/
    bool mConfigDataCurrentProtection;  /**< (--)     keep track of trip protection for when 'ignore current trips' malf is removed, so it can be set back */
    bool mLastMalfIgnoreCurrentTrip;  /**< (--)     previous state of malf to prevent switch from tripping in event of over-current */
    double mDefaultPosTripLimit;      /**< (amp)    trip point for current out of the bus/from port 0 to port 1*/
    double mActivePosTripLimit;       /**< (amp)    trip point accounting for malfunction*/
    double mDefaultNegTripLimit;      /**< (amp)    trip point for current in to the bus/from port 1 to port 0*/
    double mActiveNegTripLimit;       /**< (amp)    trip point accounting for malfunction*/
    int mPortAssigned;                /**< (--)     trick_chkpnt_io(**) for two port switches which port do I talk to?  0 for userLoadSwitches*/
    int mSwitchStateEnum;             /**< (--)     an enumeration to use on display pages for the state's open/close/malf'd state */

    // following are to make more useful H&S log error messages during trips
    std::string mSwitchCardName;      /**< (--)     what switchcard am I in? */
    int mSwitchCardPosition;          /**< (--)     which switch in the switchcard am I? */
    bool mPortsAreReversed;           /**< (--) trick_chkpnt_io(**) if true lets switch card know to reverse the ports when calculating current flow.*/
    bool mInitFlag;                   /**< *o (--) trick_chkpnt_io(**) True if this switch has been initialized. */

    static const double mConductanceLimit;

    Switch(const Switch& that);
    Switch& operator =(const Switch& that);
};

/// @}
///////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method: isPortsReversed        -- return bool whether ports are reversed
/// @return   bool                    -- TRUE if reversed, FALSE if not
///////////////////////////////////////////////////////////////////////////////////////////////
inline bool Switch::isPortsReversed() const {
    return mPortsAreReversed;
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method: isClosed        -- return the current switch state (open/closed)
/// @return   bool                    -- TRUE if switch is closed, FALSE if switch is open
///////////////////////////////////////////////////////////////////////////////////////////////
inline bool Switch::isClosed() const {
    return mSwitchIsClosed;
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method: isCommandedClosed   -- return the commanded switch state (open/closed)
/// @return   bool                        -- TRUE if switch is supposed to be closed, FALSE if switch is supposed to be open
///////////////////////////////////////////////////////////////////////////////////////////////
inline bool Switch::isCommandedClosed() const {
    return mSwitchCommandedClosed;
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method: getCurrent        -- return the current through the switch
/// @return double                      -- amps going through the switch
///////////////////////////////////////////////////////////////////////////////////////////////
inline double Switch::getCurrent() const {
    return mCurrentActual;
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method: getConductance        -- return the current switch conductance
/// @return double                          -- conductance value (1 / resistance) of the switch
///////////////////////////////////////////////////////////////////////////////////////////////
inline double Switch::getConductance() const {
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

///////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method: getPowerDissipation        -- return the current switch power dissipation
/// @return double                               -- power lost by the switch itself
///////////////////////////////////////////////////////////////////////////////////////////////
inline double Switch::getPowerDissipation() const {
    return mPowerDissipation;
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method: getPortAssigned      -- return the network port this two port switch talks to
/// @return int                            -- port this switch is connected to (one end always connected to port 0)
///////////////////////////////////////////////////////////////////////////////////////////////
inline int Switch::getPortAssigned() const {
    return mPortAssigned;
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method: isTripped      TRUE if switch has any trip active
/// @return bool                     whether this switch is tripped open or not
///////////////////////////////////////////////////////////////////////////////////////////////
inline bool Switch::isTripped() const {
    //return (mPosTrip || mNegTrip || mVoltTrip);
    return (mPosTrip || mNegTrip || mExternalTrip);
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// @details Method: isVoltTrip    TRUE if switch had an undervolt trip
/// @return bool                   whether this switch is open due to too low voltage
///////////////////////////////////////////////////////////////////////////////////////////////
//inline bool Switch::isVoltTrip() const {
//    return mVoltTrip;
//}

///////////////////////////////////////////////////////////////////////////////////////////////
/// @details Method: isPosTrip    TRUE if switch had a positive current trip (from port 0 to port 1)
/// @return bool                  whether this switch is open due to too much current in the positive direction
///////////////////////////////////////////////////////////////////////////////////////////////
inline bool Switch::isPosTrip() const {
    return mPosTrip;
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// @details Method: isNegTrip    TRUE if switch had a negative current trip (from port 1 to port 0)
/// @return bool                  whether this switch is open due to too much current in the negative direction
///////////////////////////////////////////////////////////////////////////////////////////////
inline bool Switch::isNegTrip() const {
    return mNegTrip;
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method: isWaitingToTrip    TRUE if switch has any trip active but can't trip on this minorStep
/// @return bool                         flag for whether this switch is in a trip state but can't open yet because of trip staging
///////////////////////////////////////////////////////////////////////////////////////////////
inline bool Switch::isWaitingToTrip() const {
    return mWaitingToTrip;
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method: isJustTripped    TRUE if switch has just transitioned from waitingToTrip to Tripped
/// @return bool                       flag for whether this switch has just tripped
///////////////////////////////////////////////////////////////////////////////////////////////
inline bool Switch::isJustTripped() const {
    return mJustTripped;
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// @details Method: isTwoPortSwitch   TRUE if this is a switch that powers another network object
/// @return bool                        whether this is a switch that powers another network object
///////////////////////////////////////////////////////////////////////////////////////////////
inline bool Switch::isTwoPortSwitch() const {
    return mIsTwoPortSwitch;
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// @details Method: getOutputVolts   returns switch output voltage (0.0 if open, mVoltage if closed)
/// @return double                    switch output voltage
///////////////////////////////////////////////////////////////////////////////////////////////
inline double Switch::getOutputVolts() const {
    if (mSwitchIsClosed) {
        return mVoltageIn;
    } else {
        return 0.0;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// @details Method: getInputVolts   returns switch input voltage
/// @return double                   switch input voltage
///////////////////////////////////////////////////////////////////////////////////////////////
inline double Switch::getInputVolts() const {
    return mVoltageIn;
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// @details Method: getPosTripLimit   returns switch nominal positive current trip limit
/// @return double                   switch nominal positive current trip limit
///////////////////////////////////////////////////////////////////////////////////////////////
inline double Switch::getPosTripLimit() const {
    return mDefaultPosTripLimit;
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// @details Method: getNegTripLimit   returns switch nominal negative current trip limit
/// @return double                   switch nominal negative current trip limit
///////////////////////////////////////////////////////////////////////////////////////////////
inline double Switch::getNegTripLimit() const {
    return mDefaultNegTripLimit;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    none
/// @details  Returns mInitFlag
/// @return  bool
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool Switch::isInitialized() const {
    return mInitFlag;
}

#endif
