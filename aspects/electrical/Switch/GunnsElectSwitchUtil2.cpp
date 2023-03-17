/**

@file   GunnsElectSwitchUtil2.cpp
@brief  GUNNS Electrical Switch Utility Model Variant 2 implementation

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
 ((simulation/hs/TsHsMsg.o)
  (software/exceptions/TsInitializationException.o)
  (aspects/electrical/TripLogic/GunnsTripLogic.hh))
*/

#include "GunnsElectSwitchUtil2.hh"
#include "core/GunnsMacros.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] resistance   (--) Default electrical resistance of the switch.
/// @param[in] tripPriority (--) Priority of trips in the network.
///
/// @details Constructs the GunnsElectSwitchUtil2 configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectSwitchUtil2ConfigData::GunnsElectSwitchUtil2ConfigData(const float        resistance,
                                                                 const unsigned int tripPriority)
    :
    mResistance(resistance),
    mTripPriority(tripPriority)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the GunnsElectSwitchUtil2 configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectSwitchUtil2ConfigData::~GunnsElectSwitchUtil2ConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) The object to copy.
///
/// @details  Copy constructs the GunnsElectSwitchUtil2 configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectSwitchUtil2ConfigData::GunnsElectSwitchUtil2ConfigData(const GunnsElectSwitchUtil2ConfigData& that)
{
    /// - Reuse the assignment operator.
    *this = that;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) The object assigned from.
///
/// @return  GunnsElectSwitchUtil2ConfigData& (--) Reference to the object assigned to.
///
/// @details  Assignment operator for the GunnsElectSwitchUtil2 configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectSwitchUtil2ConfigData& GunnsElectSwitchUtil2ConfigData::operator=(const GunnsElectSwitchUtil2ConfigData& that)
{
    /// - Skip attribute assignment if objects are the same
    if (this != &that) {
        mResistance   = that.mResistance;
        mTripPriority = that.mTripPriority;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] position                   (--)  Initial position of the switch (true = closed).
/// @param[in] positionCommand            (--)  Initial switch position command (true = closed).
/// @param[in] resetTripsCommand          (--)  Initial reset trips command (true = reset).
/// @param[in] inputUnderVoltageTripLimit (V)   Initial input under-voltage trip limit.
/// @param[in] inputUnderVoltageTripReset (V)   Initial input under-voltage trip reset value.
/// @param[in] inputOverVoltageTripLimit  (V)   Initial input over-voltage trip limit.
/// @param[in] inputOverVoltageTripReset  (V)   Initial input over-voltage trip reset value.
/// @param[in] posOverCurrentTripLimit    (amp) Initial positive over-current trip limit.
/// @param[in] negOverCurrentTripLimit    (amp) Initial negative over-current trip limit.
///
/// @details  Constructs the GunnsElectSwitchUtil2 input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectSwitchUtil2InputData::GunnsElectSwitchUtil2InputData(const bool  position,
                                                               const bool  positionCommand,
                                                               const bool  resetTripsCommand,
                                                               const float inputUnderVoltageTripLimit,
                                                               const float inputUnderVoltageTripReset,
                                                               const float inputOverVoltageTripLimit,
                                                               const float inputOverVoltageTripReset,
                                                               const float posOverCurrentTripLimit,
                                                               const float negOverCurrentTripLimit)
    :
    mPosition(position),
    mPositionCommand(positionCommand),
    mResetTripsCommand(resetTripsCommand),
    mInputUnderVoltageTripLimit(inputUnderVoltageTripLimit),
    mInputUnderVoltageTripReset(inputUnderVoltageTripReset),
    mInputOverVoltageTripLimit(inputOverVoltageTripLimit),
    mInputOverVoltageTripReset(inputOverVoltageTripReset),
    mPosOverCurrentTripLimit(posOverCurrentTripLimit),
    mNegOverCurrentTripLimit(negOverCurrentTripLimit)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the GunnsElectSwitchUtil2 input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectSwitchUtil2InputData::~GunnsElectSwitchUtil2InputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) The object to copy.
///
/// @details  Copy constructs the GunnsElectSwitchUtil2 input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectSwitchUtil2InputData::GunnsElectSwitchUtil2InputData(const GunnsElectSwitchUtil2InputData& that)
{
    /// - Reuse the assignment operator.
    *this = that;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) The object assigned from.
///
/// @return  GunnsElectSwitchUtil2InputData& (--) Reference to the object assigned to.
///
/// @details  Assignment operator for the GunnsElectSwitchUtil2 input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectSwitchUtil2InputData& GunnsElectSwitchUtil2InputData::operator=(const GunnsElectSwitchUtil2InputData& that)
{
    /// - Skip attribute assignment if objects are the same
    if (this != &that) {
        mPosition                   = that.mPosition;
        mPositionCommand            = that.mPositionCommand;
        mResetTripsCommand          = that.mResetTripsCommand;
        mInputUnderVoltageTripLimit = that.mInputUnderVoltageTripLimit;
        mInputUnderVoltageTripReset = that.mInputUnderVoltageTripReset;
        mInputOverVoltageTripLimit  = that.mInputOverVoltageTripLimit;
        mInputOverVoltageTripReset  = that.mInputOverVoltageTripReset;
        mPosOverCurrentTripLimit    = that.mPosOverCurrentTripLimit;
        mNegOverCurrentTripLimit    = that.mNegOverCurrentTripLimit;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details GunnsElectSwitchUtil2 default constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectSwitchUtil2::GunnsElectSwitchUtil2()
    :
    mMalfFailClosed(false),
    mMalfFailOpen(false),
    mResistance(0.0),
    mPosition(false),
    mPositionCommand(false),
    mResetTripsCommand(false),
    mInputUnderVoltageTrip(),
    mInputUnderVoltageReset(),
    mInputOverVoltageTrip(),
    mInputOverVoltageReset(),
    mPosOverCurrentTrip(),
    mNegOverCurrentTrip(),
    mWaitingToTrip(false),
    mJustTripped(false),
    mName(),
    mInitFlag(false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details GunnsElectSwitchUtil2 destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectSwitchUtil2::~GunnsElectSwitchUtil2()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] configData (--) Reference to the switch configuration data.
/// @param[in] inputData  (--) Reference to the Switch input data.
/// @param[in] name       (--) Name of this instance for messaging.
///
/// @throws  TsInitializationException
///
/// @details  Initializes the GunnsElectSwitchUtil2 with name, configuration & input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectSwitchUtil2::initialize(const GunnsElectSwitchUtil2ConfigData& configData,
                                       const GunnsElectSwitchUtil2InputData&  inputData,
                                       const std::string&                     name)
{
    mInitFlag = false;

    /// - Initialize this instance name.
    GUNNS_NAME_ERREX("GunnsElectSwitchUtil2", name);

    /// - Validate initialization.
    validate(configData, inputData);

    /// - Initialize from config & input data.
    mResistance        = configData.mResistance;
    mPosition          = inputData.mPosition;
    mPositionCommand   = inputData.mPositionCommand;
    mResetTripsCommand = inputData.mResetTripsCommand;
    mInputUnderVoltageTrip .initialize(inputData.mInputUnderVoltageTripLimit, configData.mTripPriority, false);
    mInputUnderVoltageReset.initialize(inputData.mInputUnderVoltageTripReset, configData.mTripPriority, false);
    mInputOverVoltageTrip  .initialize(inputData.mInputOverVoltageTripLimit,  configData.mTripPriority, false);
    mInputOverVoltageReset .initialize(inputData.mInputOverVoltageTripReset,  configData.mTripPriority, false);
    mPosOverCurrentTrip    .initialize(inputData.mPosOverCurrentTripLimit,    configData.mTripPriority, false);
    mNegOverCurrentTrip    .initialize(inputData.mNegOverCurrentTripLimit,    configData.mTripPriority, false);

    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] configData (--) Reference to the switch configuration data.
/// @param[in] inputData  (--) Reference to the Switch input data.
///
/// @throws  TsInitializationException
///
/// @details  Checks configuration and input data for valid values and ranges and throws H&S errors
///           and exceptions for failures.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectSwitchUtil2::validate(const GunnsElectSwitchUtil2ConfigData& configData,
                                     const GunnsElectSwitchUtil2InputData&  inputData) const
{
    /// - Throw exception for invalid resistance value.
    if (configData.mResistance < FLT_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "resistance is < FLT_EPSILON.");
    }

    /// - Throw exception for input under-volt trip reset value < trip value.
    if (inputData.mInputUnderVoltageTripReset < inputData.mInputUnderVoltageTripLimit) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data", "undervolt trip reset < trip limit.");
    }

    /// - Throw exception for input over-volt trip reset value > trip value.
    if (inputData.mInputOverVoltageTripReset > inputData.mInputOverVoltageTripLimit) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data", "overvolt trip reset > trip limit.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Limits state variables to valid ranges, in cause they have been set to invalid values
///           by the user between model updates.  These are the same limits checked by validate,
///           except here we quietly apply the limits rather than throwing exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectSwitchUtil2::applyConstraints()
{
    if (mResistance < FLT_EPSILON) {
        mResistance = FLT_EPSILON;
    }

    if (mInputUnderVoltageReset.getLimit() < mInputUnderVoltageTrip.getLimit()) {
        mInputUnderVoltageReset.setLimit(mInputUnderVoltageTrip.getLimit());
    }

    if (mInputOverVoltageReset.getLimit() > mInputOverVoltageTrip.getLimit()) {
        mInputOverVoltageReset.setLimit(mInputOverVoltageTrip.getLimit());
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This should be called during a network minor step before the network solution (from a
///           link's step or minorStep function, etc.)  This updates the switch position based on
///           command and malfunctions, and resets the trip logic when commanded open.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectSwitchUtil2::updateState()
{
    applyConstraints();

    /// - Clear trips on reception of the open command.
    if (isTripped() and not mPositionCommand) {
        resetTrips();
    }

    /// - Clear trips on reception of trip reset command.
    if (mResetTripsCommand) {
        resetTrips();
        mResetTripsCommand = false;
    }

    /// - Update switch position with commands and malfunctions.
    if (mMalfFailClosed) {
        mPosition = true;
    } else if (mMalfFailOpen) {
        mPosition = false;
    } else if (not (isTripped() or isWaitingToTrip())) {
        mPosition = mPositionCommand;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] current       (amp) Electrical current through the switch for over-current trip logic.
/// @param[in] voltage       (V)   Input voltage to the switch for over- and under-voltage trip logic.
/// @param[in] convergedStep (--)  The network converged minor step number for the trip logic.
///
/// @details  This should be called after the network solution in each minor step, such as from the
///           parent link's confirmSolutionAcceptable method.  This updates the switch trip logic.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectSwitchUtil2::updateTrips(const double current, const double voltage,
                                        const int    convergedStep)
{
    GunnsBasicLink::SolutionResult result = GunnsBasicLink::CONFIRM;

    mWaitingToTrip = false;
    mJustTripped   = false;

    if (convergedStep > 0) {

        /// - Update the trip reset logic if the switch is tripped open and not failed open or has
        ///   a non-reseting over-current trip.
        if ( (mInputUnderVoltageTrip.isTripped() or mInputOverVoltageTrip.isTripped()) and not
                (mMalfFailOpen or mPosOverCurrentTrip.isTripped() or mNegOverCurrentTrip.isTripped()) ) {

            /// - Input under-voltage trip reset check and warning.  Upon trip of this reset
            ///   (rejection of the network solution), this resets itself and the actual under-
            ///   voltage trip.
            if (mInputUnderVoltageTrip.isTripped() and
                    mInputUnderVoltageReset.checkForTrip(result, voltage, convergedStep)) {
                mInputUnderVoltageReset.resetTrip();
                mInputUnderVoltageTrip.resetTrip();
                TsHsMsg msg(TS_HS_WARNING, TS_HS_EPS);
                msg << mName << " input under-voltage trip reset at converged step " << convergedStep << ", " << voltage << " > reset limit of " << mInputUnderVoltageReset.getLimit();
                GUNNS_WARNING(msg.str());
            }

            /// - Input over-voltage trip reset check and warning.  Upon trip of this reset
            ///   (rejection of the network solution), this resets itself and the actual over-
            ///   voltage trip.
            if (mInputOverVoltageTrip.isTripped() and
                    mInputOverVoltageReset.checkForTrip(result, voltage, convergedStep)) {
                mInputOverVoltageReset.resetTrip();
                mInputOverVoltageTrip.resetTrip();
                TsHsMsg msg(TS_HS_WARNING, TS_HS_EPS);
                msg << mName << " input over-voltage trip reset at converged step " << convergedStep << ", " << voltage << " < reset limit of " << mInputOverVoltageReset.getLimit();
                GUNNS_WARNING(msg.str());
            }

            if (GunnsBasicLink::DELAY == result) {
                mWaitingToTrip = true;
            } else if (GunnsBasicLink::REJECT == result) {
                mJustTripped = true;
                /// - Upon reset of the voltage trip, the switch automatically closes if it is
                ///   still commanded to close.  This feature can be avoided by the user resetting
                ///   the position command to open whenever the switch trips open.
                if (mPositionCommand) {
                    mPosition = true;
                }
            }

        /// - Update the trip logic if the switch is not tripped open and not failed closed.
        } else if (mPositionCommand and not isTripped() and not mMalfFailClosed) {

            /// - Input under-voltage trip check and warning.
            if (mInputUnderVoltageTrip.checkForTrip(result, voltage, convergedStep)) {
                TsHsMsg msg(TS_HS_WARNING, TS_HS_EPS);
                msg << mName << " input under-voltage trip at converged step " << convergedStep << ", " << voltage << " < trip limit of " << mInputUnderVoltageTrip.getLimit();
                GUNNS_WARNING(msg.str());
            }

            /// - Input over-voltage trip check and warning.
            if (mInputOverVoltageTrip.checkForTrip(result, voltage, convergedStep)) {
                TsHsMsg msg(TS_HS_WARNING, TS_HS_EPS);
                msg << mName << " input over-voltage trip at converged step " << convergedStep << ", " << voltage << " > trip limit of " << mInputOverVoltageTrip.getLimit();
                GUNNS_WARNING(msg.str());
            }

            /// - Positive over-current trip check and warning.
            if (mPosOverCurrentTrip.checkForTrip(result, current, convergedStep)) {
                TsHsMsg msg(TS_HS_WARNING, TS_HS_EPS);
                msg << mName << " positive over-current trip at converged step " << convergedStep << ", " << current << " > trip limit of " << mPosOverCurrentTrip.getLimit();
                GUNNS_WARNING(msg.str());
            }

            /// - Negative over-current trip check and warning.
            if (mNegOverCurrentTrip.checkForTrip(result, current, convergedStep)) {
                TsHsMsg msg(TS_HS_WARNING, TS_HS_EPS);
                msg << mName << " negative over-current trip at converged step " << convergedStep << ", " << current << " < trip limit of " << mPosOverCurrentTrip.getLimit();
                GUNNS_WARNING(msg.str());
            }

            if (GunnsBasicLink::DELAY == result) {
                mWaitingToTrip = true;
            } else if (GunnsBasicLink::REJECT == result) {
                mJustTripped = true;
                mPosition    = false;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Resets all trip and trip reset logic, and the waiting to trip and just tripped flags.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectSwitchUtil2::resetTrips()
{
    mWaitingToTrip = false;
    mJustTripped   = false;
    mInputUnderVoltageTrip.resetTrip();
    mInputUnderVoltageReset.resetTrip();
    mInputOverVoltageTrip.resetTrip();
    mInputOverVoltageReset.resetTrip();
    mPosOverCurrentTrip.resetTrip();
    mNegOverCurrentTrip.resetTrip();
}
