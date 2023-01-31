#ifndef GunnsElectSwitchUtil2_EXISTS
#define GunnsElectSwitchUtil2_EXISTS

/**
@file   GunnsElectSwitchUtil2.hh
@brief  GUNNS Electrical Switch Utility Model Variant 2 declarations

@defgroup TSM_GUNNS_ELECTRICAL_SWITCH_MODEL_2    GUNNS Electrical Switch Utility Model Variant 2
@ingroup  TSM_GUNNS_ELECTRICAL_SWITCH

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.


@details
PURPOSE:
- (Classes for the Electrical Switch Utility Model Variant 2.)

REQUIREMENTS:
- (TBD)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsElectSwitchUtil2.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (January 2023) (Initial))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "aspects/electrical/TripLogic/GunnsTripLogic.hh"
#include <cfloat>
#include <string>

// Forward-declaration of types.
class GunnsElectSwitchUtil2ConfigData;
class GunnsElectSwitchUtil2InputData;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS electrical switch utility model variant 2.
///
/// @details  This is a generic switch utility model, based on GunnsElectSwitchUtil with some
///           improvements & simplifications:
///           - add input under-voltage & over-voltage trips with automatic trip reset,
///           - reuse the GunnsTripLogic code,
///           - cleanup code style and names for consistency,
///           - remove trip malfunctions and setpoint malfunctions, relegating these to external
///             voltage & current sensor models,
///           - remove resistance malfunction, relegating to the parent link blockage malfunction,
///           - remove power supply, relegating to a signal aspect,
///           - remove switch state enumeration, relegating to external users,
///           - remove power dissipation, relegating to the parent link,
///           - remove two-port switch considerations and knowledge of a switch card, to simplify.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectSwitchUtil2
{
    TS_MAKE_SIM_COMPATIBLE(GunnsElectSwitchUtil2);

    public:
        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool mMalfFailClosed; /**< (--) Switch failed closed malfunction activation flag, overrides failed open malfunction. */
        bool mMalfFailOpen;   /**< (--) Switch failed open malfunction activation flag. */
        /// @}
        /// @brief Constructs the switch model.
        GunnsElectSwitchUtil2();
        /// @brief Destructs the switch model.
        virtual ~GunnsElectSwitchUtil2();
        /// @brief Initializes the switch model with configuration and input data.
        void initialize(const GunnsElectSwitchUtil2ConfigData& configData,
                        const GunnsElectSwitchUtil2InputData&  inputData,
                        const std::string&                     name);
        /// @brief Updates the switch position state.
        void updateState();
        /// @brief Updates the switch trip logic.
        void updateTrips(const double current, const double voltage, const int convergedStep);
        /// @brief Sets the switch position command.
        void setPositionCommand(const bool position);
        /// @brief Causes the switch to reset all of its trip states and logic.
        void resetTrips();
        /// @brief Returns the effective electrical resistance between the switch poles.
        double getResistance() const;
        /// @brief Returns whether the switch is physically closed.
        bool getPosition() const;
        /// @brief Returns the position command value.
        bool getPositionCommand() const;
        /// @brief Returns whether the switch is tripped open.
        bool isTripped() const;
        /// @brief Returns whether the switch is waiting to trip or trip auto-reset.
        bool isWaitingToTrip() const;
        /// @brief Returns whether the switch has just tripped or trip auto-reset on this minor step.
        bool hasJustTripped() const;
        /// @brief  Returns the input under-voltage trip logic.
        GunnsTripLogic* getInputUnderVoltageTrip();
        /// @brief  Returns the input under-voltage trip reset logic.
        GunnsTripLogic* getInputUnderVoltageReset();
        /// @brief  Returns the input over-voltage trip logic.
        GunnsTripLogic* getInputOverVoltageTrip();
        /// @brief  Returns the input over-voltage trip reset logic.
        GunnsTripLogic* getInputOverVoltageReset();
        /// @brief  Returns the positive over-current trip logic.
        GunnsTripLogic* getPosOverCurrentTrip();
        /// @brief  Returns the negative over-current trip logic.
        GunnsTripLogic* getNegOverCurrentTrip();
        /// @brief Sets and resets the switch fail closed malfunction.
        void setMalfFailClosed(const bool flag = false);
        /// @brief Sets and resets the switch fail open malfunction.
        void setMalfFailOpen(const bool flag = false);
        /// @brief Returns whether this switch is initialized.
        bool isInitialized() const;

    protected:
        float                mResistance;             /**<    (ohm) trick_chkpnt_io(**) Default electrical resistance of the switch. */
        bool                 mPosition;               /**<    (1)                       Actual position of the switch (true = closed). */
        bool                 mPositionCommand;        /**<    (1)                       Switch position command (true = closed). */
        bool                 mResetTripsCommand;      /**<    (1)                       Reset trips command (true = reset). */
        GunnsTripLessThan    mInputUnderVoltageTrip;  /**<    (1)                       Input under-voltage trip function. */
        GunnsTripGreaterThan mInputUnderVoltageReset; /**<    (V)                       Input under-voltage trip reset function. */
        GunnsTripGreaterThan mInputOverVoltageTrip;   /**<    (1)                       Input over-voltage trip function. */
        GunnsTripLessThan    mInputOverVoltageReset;  /**<    (V)                       Input over-voltage trip reset function. */
        GunnsTripGreaterThan mPosOverCurrentTrip;     /**<    (1)                       Positive over-current trip function. */
        GunnsTripLessThan    mNegOverCurrentTrip;     /**<    (1)                       Negative over-current trip function. */
        bool                 mWaitingToTrip;          /**<    (1)   trick_chkpnt_io(**) Switch is going to trip, waiting for its tip priority minor step. */
        bool                 mJustTripped;            /**<    (1)   trick_chkpnt_io(**) Switch has tripped on this minor step. */
        std::string          mName;                   /**< *o (1)                       Name of this switch instance for messaging. */
        bool                 mInitFlag;               /**< *o (1)   trick_chkpnt_io(**) True if this switch has been initialized. */
        /// @brief  Validates the configuration and input data.
        void validate(const GunnsElectSwitchUtil2ConfigData& configData,
                      const GunnsElectSwitchUtil2InputData&  inputData) const;
        /// @brief  Applies range limits to runtime variables.
        void applyConstraints();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsElectSwitchUtil2(const GunnsElectSwitchUtil2& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsElectSwitchUtil2& operator =(const GunnsElectSwitchUtil2& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GunnsElectSwitchUtil2 Configuration Data.
///
/// @details  This provides a data structure for the GunnsElectSwitchUtil2 configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectSwitchUtil2ConfigData {
    public:
        float        mResistance;   /**< (ohm) trick_chkpnt_io(**) Default electrical resistance of this switch. */
        unsigned int mTripPriority; /**< (1)   trick_chkpnt_io(**) Priority of trips in the network. */
        /// @brief Default GunnsElectSwitchUtil2 Configuration Data Constructor.
        GunnsElectSwitchUtil2ConfigData(const float        resistance   = 0.0,
                                        const unsigned int tripPriority = 0);
        /// @brief GunnsElectSwitchUtil2 configuration destructor.
        virtual ~GunnsElectSwitchUtil2ConfigData();
        /// @brief GunnsElectSwitchUtil2 configuration copy constructor.
        GunnsElectSwitchUtil2ConfigData(const GunnsElectSwitchUtil2ConfigData& that);
        /// @brief GunnsElectSwitchUtil2 Configuration Data assignment operator.
        GunnsElectSwitchUtil2ConfigData& operator =(const GunnsElectSwitchUtil2ConfigData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GunnsElectSwitchUtil2 Input Data.
///
/// @details  This provides a data structure for the GunnsElectSwitchUtil2 input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectSwitchUtil2InputData {
    public:
        bool  mPosition;                   /**< (1)   trick_chkpnt_io(**) Initial position of the switch (true = closed). */
        bool  mPositionCommand;            /**< (1)   trick_chkpnt_io(**) Initial switch position command (true = closed). */
        bool  mResetTripsCommand;          /**< (1)   trick_chkpnt_io(**) Initial reset trips command (true = reset). */
        float mInputUnderVoltageTripLimit; /**< (V)   trick_chkpnt_io(**) Initial input under-voltage trip limit. */
        float mInputUnderVoltageTripReset; /**< (V)   trick_chkpnt_io(**) Initial input under-voltage trip reset value. */
        float mInputOverVoltageTripLimit;  /**< (V)   trick_chkpnt_io(**) Initial input over-voltage trip limit. */
        float mInputOverVoltageTripReset;  /**< (V)   trick_chkpnt_io(**) Initial input over-voltage trip reset value. */
        float mPosOverCurrentTripLimit;    /**< (amp) trick_chkpnt_io(**) Initial positive over-current trip limit. */
        float mNegOverCurrentTripLimit;    /**< (amp) trick_chkpnt_io(**) Initial negative over-current trip limit. */
            /// @brief Default GunnsElectSwitchUtil2 Input Data Constructor.
        GunnsElectSwitchUtil2InputData(const bool  position                   = false,
                                       const bool  positionCommand            = false,
                                       const bool  resetTripsCommand          = false,
                                       const float inputUnderVoltageTripLimit = 0.0,
                                       const float inputUnderVoltageTripReset = 0.0,
                                       const float inputOverVoltageTripLimit  = 0.0,
                                       const float inputOverVoltageTripReset  = 0.0,
                                       const float posOverCurrentTripLimit    = 0.0,
                                       const float negOverCurrentTripLimit    = 0.0);
        /// @brief Default GunnsElectSwitchUtil2 Input DataDestructor.
        virtual ~GunnsElectSwitchUtil2InputData();
        /// @brief Copy constructs GunnsElectSwitchUtil2 Input Data.
        GunnsElectSwitchUtil2InputData(const GunnsElectSwitchUtil2InputData& that);
        /// @brief GunnsElectSwitchUtil2 Input Data assignment operator.
        GunnsElectSwitchUtil2InputData& operator=(const GunnsElectSwitchUtil2InputData& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] position (--) The switch position, true = closed.
///
/// @details  Sets mPositionCommand to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectSwitchUtil2::setPositionCommand(const bool position)
{
    mPositionCommand = position;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--) Malfunction activation flag, true activates.
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectSwitchUtil2::setMalfFailClosed(const bool flag)
{
    mMalfFailClosed = flag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--) Malfunction activation flag, true activates.
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectSwitchUtil2::setMalfFailOpen(const bool flag)
{
    mMalfFailOpen = flag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool (--) True if the switch is closed, false if it is open.
///
/// @details  Returns the value of mPosition.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectSwitchUtil2::getPosition() const
{
    return mPosition;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool (--) The position command value.
///
/// @details  Returns the value of mPositionCommand.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectSwitchUtil2::getPositionCommand() const
{
    return mPositionCommand;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return double (ohm) Effective resistance between the switch poles.
///
/// @details  Returns the effective resistance between the switch poles, which is the nominal switch
///           resistance if the switch is closed, or 1/DBL_EPSILON if the switch is open.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectSwitchUtil2::getResistance() const
{
    if (mPosition) {
        return mResistance;
    }
    return (1.0 / DBL_EPSILON);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return bool (--) True if the switch is tripped open, false if not.
///
/// @details  Returns whether the switch is tripped open, by or-ing each of the trip logics.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectSwitchUtil2::isTripped() const
{
    return (mInputUnderVoltageTrip.isTripped() or mInputOverVoltageTrip.isTripped() or
            mPosOverCurrentTrip.isTripped() or mNegOverCurrentTrip.isTripped());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return bool (--) True if the switch is waiting to trip.
///
/// @details  Returns the value of mWaitingToTrip.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectSwitchUtil2::isWaitingToTrip() const
{
    return mWaitingToTrip;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return bool (--) True if the switch has just tripped on this minor step..
///
/// @details  Returns the value of mJustTripped.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectSwitchUtil2::hasJustTripped() const
{
    return mJustTripped;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  GunnsTripLogic*  (--)  Pointer to the input under-voltage trip logic.
///
/// @details  Returns the address of the mInputUnderVoltageTrip attribute.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline GunnsTripLogic* GunnsElectSwitchUtil2::getInputUnderVoltageTrip()
{
    return &mInputUnderVoltageTrip;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  GunnsTripLogic*  (--)  Pointer to the input under-voltage trip reset logic.
///
/// @details  Returns the address of the mInputUnderVoltageReset attribute.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline GunnsTripLogic* GunnsElectSwitchUtil2::getInputUnderVoltageReset()
{
    return &mInputUnderVoltageReset;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  GunnsTripLogic*  (--)  Pointer to the input over-voltage trip logic.
///
/// @details  Returns the address of the mInputOverVoltageTrip attribute.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline GunnsTripLogic* GunnsElectSwitchUtil2::getInputOverVoltageTrip()
{
    return &mInputOverVoltageTrip;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  GunnsTripLogic*  (--)  Pointer to the input over-voltage trip reset logic.
///
/// @details  Returns the address of the mInputOverVoltageReset attribute.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline GunnsTripLogic* GunnsElectSwitchUtil2::getInputOverVoltageReset()
{
    return &mInputOverVoltageReset;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  GunnsTripLogic*  (--)  Pointer to the positive over-current trip logic.
///
/// @details  Returns the address of the mPosOverCurrentTrip attribute.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline GunnsTripLogic* GunnsElectSwitchUtil2::getPosOverCurrentTrip()
{
    return &mPosOverCurrentTrip;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  GunnsTripLogic*  (--)  Pointer to the negative over-current trip logic.
///
/// @details  Returns the address of the mNegOverCurrentTrip attribute.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline GunnsTripLogic* GunnsElectSwitchUtil2::getNegOverCurrentTrip()
{
    return &mNegOverCurrentTrip;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return  bool (--) True if this switch model has been initialized.
///
/// @details  Returns the value of mInitFlag.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectSwitchUtil2::isInitialized() const {
    return mInitFlag;
}

#endif
