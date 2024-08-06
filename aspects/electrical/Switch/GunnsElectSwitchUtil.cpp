/*
@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
 (
  (simulation/hs/TsHsMsg.o)
  (software/exceptions/TsInitializationException.o)
 )
*/

#include "GunnsElectSwitchUtil.hh"
#include "math.h"
#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "simulation/hs/TS_hs_msg_types.h"

/// @details
const double GunnsElectSwitchUtil::mConductanceLimit = 1.0E15;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    defaultSwitchResistance    (--) default switch Resistance when closed
/// @param[in]    overCurrentProtection      (--) flag for whether a switch trips open for over-current
/// @param[in]    underVoltProtection        (--) flag for whether a switch trips open from under-voltage
/// @param[in]    minVoltage                 (V) value in volts below which a switch under-volt trips
/// @param[in]    isTwoPortSwitch            (--) flag for whether this switch powers another switchcard
/// @param[in]    portAssigned               (--) for two port switches, which port do I talk to? 0 for non two port switches
/// @param[in]    tripPriority               (--) which minor step this switch is allowed to trip on
/// @param[in]    reverse                    (--) if true, ports will be reversed and current flow will be opposite.
///
/// @details Constructs the Basic Capacitor Config data.  Configuration data is considered to be
///           static for the lifetime of the instance of the object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectSwitchUtilConfigData::GunnsElectSwitchUtilConfigData(const double defaultSwitchResistance,
                                                               const bool   overCurrentProtection,
                                                               const bool   underVoltProtection,
                                                               const double minVoltage,
                                                               const bool   isTwoPortSwitch,
                                                               const int    portAssigned,
                                                               const int    tripPriority,
                                                               const bool   reverse)
    :
    mDefaultSwitchResistance(defaultSwitchResistance),
    mOverCurrentProtection(overCurrentProtection),
    mUnderVoltProtection(underVoltProtection),
    mMinVoltage(minVoltage),
    mIsTwoPortSwitch(isTwoPortSwitch),
    mPortAssigned(portAssigned),
    mTripPriority(tripPriority),
    mPortsAreReversed(reverse)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the GunnsElectSwitchUtil Config Data Object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectSwitchUtilConfigData::~GunnsElectSwitchUtilConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    that   (--)  Object to copy.
///
/// @details  Copy constructs this GunnsElectSwitchUtil Config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectSwitchUtilConfigData::GunnsElectSwitchUtilConfigData(const GunnsElectSwitchUtilConfigData& that)
    :
    mDefaultSwitchResistance(that.mDefaultSwitchResistance),
    mOverCurrentProtection(that.mOverCurrentProtection),
    mUnderVoltProtection(that.mUnderVoltProtection),
    mMinVoltage(that.mMinVoltage),
    mIsTwoPortSwitch(that.mIsTwoPortSwitch),
    mPortAssigned(that.mPortAssigned),
    mTripPriority(that.mTripPriority),
    mPortsAreReversed(that.mPortsAreReversed)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    defaultSwitchResistance     -- On Resistance
/// @param[in]    overCurrentProtection       -- switch over current protection
/// @param[in]    mUnderVoltProtection        -- flag for whether a switch trips open from under-voltage
/// @param[in]    mMinVoltage                 -- value in volts below which a switch under-volt trips
/// @param[in]    mIsTwoPortSwitch            -- flag for whether this switch powers another switchcard
/// @param[in]    mPortAssigned               -- for two port switches, which port do I talk to? 0 for userloadswitches
/// @param[in]    mTripPriority               -- which minor step this switch is allowed to trip on
/// @param[in]    reverse                    (--) if true, ports will be reversed and current flow will be opposite.
///
/// @brief GunnsElectSwitchUtil Configuration Data Initialize.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectSwitchUtilConfigData::init(const double defaultSwitchResistance,
                                          const bool   overCurrentProtection,
                                          const bool   underVoltProtection,
                                          const double minVoltage,
                                          const bool   isTwoPortSwitch,
                                          const int    portAssigned,
                                          const int    tripPriority,
                                          const bool   reversed)
{
    mDefaultSwitchResistance = defaultSwitchResistance;
    mOverCurrentProtection = overCurrentProtection;
    mUnderVoltProtection = underVoltProtection;
    mMinVoltage = minVoltage;
    mIsTwoPortSwitch = isTwoPortSwitch;
    mPortAssigned = portAssigned;
    mTripPriority = tripPriority;
    mPortsAreReversed = reversed;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Object assigned from.
///
/// @return     GunnsElectSwitchUtilConfigData&  (--)  Object assigned to.
///
/// @details    Assignment operator for this GunnsElectSwitchUtil Configuration Data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectSwitchUtilConfigData& GunnsElectSwitchUtilConfigData::operator=(const GunnsElectSwitchUtilConfigData& that)
{
    /// - Skip attribute assignment if objects are the same
    if (this != &that) {
        mDefaultSwitchResistance = that.mDefaultSwitchResistance;
        mOverCurrentProtection = that.mOverCurrentProtection;
        mUnderVoltProtection = that.mUnderVoltProtection;
        mMinVoltage = that.mMinVoltage;
        mIsTwoPortSwitch = that.mIsTwoPortSwitch;
        mPortAssigned = that.mPortAssigned;
        mTripPriority = that.mTripPriority;
        mPortsAreReversed = that.mPortsAreReversed;
    }

    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    switchMalfFlag      (--)  flag to activate or deactivate a switch malfunction
/// @param[in]    switchMalf          (--)  enumeration to determine which malfunction to do
/// @param[in]    switchIsClosed      (--)  open/closed state of the switch
/// @param[in]    posTripLimit        (amp)  current level in the positive direction at which the switch will open
/// @param[in]    negTripLimit        (amp)  current level in the negative direction at which the switch will open
///
/// @details  Constructs the GunnsElectSwitchUtil Input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectSwitchUtilInputData::GunnsElectSwitchUtilInputData(const bool   switchMalfFlag,
                                                             const int    switchMalfValue,
                                                             const bool   switchIsClosed,
                                                             const double posTripLimit,
                                                             const double negTripLimit)
    :
    mSwitchMalfFlag(switchMalfFlag),
    mSwitchMalfValue(switchMalfValue),
    mSwitchIsAutoClosed(switchIsClosed),
    mPosTripLimit(posTripLimit),
    mNegTripLimit(negTripLimit)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    switchMalfFlag      (--)  flag to activate or deactivate a switch malfunction
/// @param[in]    switchMalf          (--)  enumeration to determine which malfunction to do
/// @param[in]    switchIsClosed      (--)  open/closed state of the switch
/// @param[in]    posTripLimit        (amp)  current level in the positive direction at which the switch will open
/// @param[in]    negTripLimit        (amp)  current level in the negative direction at which the switch will open
///
/// @details  Initialize the GunnsElectSwitchUtil Input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectSwitchUtilInputData::init(const bool   switchMalfFlag,
                                         const int    switchMalfValue,
                                         const bool   switchIsClosed,
                                         const double posTripLimit,
                                         const double negTripLimit)
{
    mSwitchMalfFlag = switchMalfFlag;
    mSwitchMalfValue = switchMalfValue;
    mSwitchIsAutoClosed = switchIsClosed;
    mPosTripLimit = posTripLimit;
    mNegTripLimit = negTripLimit;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    that  (--)  Object to copy.
///
/// @details  Copy constructs this GunnsElectSwitchUtil input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectSwitchUtilInputData::GunnsElectSwitchUtilInputData(const GunnsElectSwitchUtilInputData& that)
    :
    mSwitchMalfFlag(that.mSwitchMalfFlag),
    mSwitchMalfValue(that.mSwitchMalfValue),
    mSwitchIsAutoClosed(that.mSwitchIsAutoClosed),
    mPosTripLimit(that.mPosTripLimit),
    mNegTripLimit(that.mNegTripLimit)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the GunnsElectSwitchUtil Input Data Object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectSwitchUtilInputData::~GunnsElectSwitchUtilInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Object assigned from.
///
/// @return     GunnsElectSwitchUtilInputData&  (--)  Object assigned to.
///
/// @details    Assignment operator for this GunnsElectSwitchUtil Input Data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectSwitchUtilInputData& GunnsElectSwitchUtilInputData::operator=(const GunnsElectSwitchUtilInputData& that)
{
    /// - Skip attribute assignment if objects are the same
    if (this != &that) {
        mSwitchMalfFlag = that.mSwitchMalfFlag;
        mSwitchMalfValue = that.mSwitchMalfValue;
        mSwitchIsAutoClosed = that.mSwitchIsAutoClosed;
        mPosTripLimit = that.mPosTripLimit;
        mNegTripLimit = that.mNegTripLimit;
    }

    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details GunnsElectSwitchUtil default constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectSwitchUtil::GunnsElectSwitchUtil()
    :
    mMalfFailClosed(false),
    mMalfFailOpen(false),
    mMalfFailResistance(false),
    mMalfFailResistPercent(100.0),
    mMalfPosTripSetpointFail(false),
    mMalfPosTripSetpointFailValue(3.5),
    mMalfNegTripSetpointFail(false),
    mMalfNegTripSetpointFailValue(-0.1),
    mMalfIgnoreCurrentTrip(false),
    mCurrentActual(0.0),
    mCurrentSensed(0.0),
    mVoltageIn(0.0),
    mPowerDissipation(0.0),
    mSwitchCommandedClosed(false),
    mSwitchIsClosed(false),
    mPosTrip(false),
    mNegTrip(false),
    mExternalTrip(false),
    mWaitingToTrip(false),
    mJustTripped(false),
    mTripPriority(1),          // trip on the first minor step
    mIsTwoPortSwitch(false),
    mCurrentMinorStep(1),      // first step for trip priority
    mTripReset(false),
    mDefaultSwitchResistance(0.0168),  // resistance for ISS Type V RPCM
    mActiveSwitchResistance(0.0168),   // nominally the same as default resistance
    mOverCurrentProtection(true),
    mConfigDataCurrentProtection(true),
    mLastMalfIgnoreCurrentTrip(false),
    mDefaultPosTripLimit(3.5),         // forward trip point for ISS Type V RPCM
    mActivePosTripLimit(3.5),
    mDefaultNegTripLimit(-0.1),        // reverse trip point for ISS Type V RPCM
    mActiveNegTripLimit(-0.1),
    mPortAssigned(0),
    mSwitchStateEnum(0),
    mSwitchCardName(),
    mSwitchCardPosition(1),
    mPortsAreReversed(false),
    mInitFlag(false)
{
    // do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details GunnsElectSwitchUtil destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectSwitchUtil::~GunnsElectSwitchUtil()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] configData        (--) Reference to switch configuration data
/// @param[in] inputData         (--) Reference to Switch input data
/// @param[in] switchCardName    (--) string to identify the owner of this switch
/// @param[in] switchPosition    (--) identify which switch in a switchcard this is
///
/// @throw TsInitializationException
///
/// @details GunnsElectSwitchUtil object data initialization.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectSwitchUtil::initialize(const GunnsElectSwitchUtilConfigData& configData,
                                      const GunnsElectSwitchUtilInputData&  inputData,
                                      const std::string                     switchCardName,
                                      const int                             switchPosition)
{
    mInitFlag = false;

    if (configData.mDefaultSwitchResistance > 0.0) {
        mDefaultSwitchResistance = configData.mDefaultSwitchResistance;
    } else {
        mDefaultSwitchResistance = 1.0 / mConductanceLimit;
        TS_HS_EXCEPTION(TS_HS_WARNING, TS_HS_EPS, "Bad Initialization data", TsInitializationException,
                "Can't have zero or negative switch resistance", "GunnsElectSwitchUtil");
    }

    mSwitchCardName = switchCardName;
    mSwitchCardPosition = switchPosition;

    mOverCurrentProtection = configData.mOverCurrentProtection;
    // synchronize the over-current protection flag
    mConfigDataCurrentProtection = mOverCurrentProtection;

    mIsTwoPortSwitch = configData.mIsTwoPortSwitch;

    if (mIsTwoPortSwitch) {
        if (0 == configData.mPortAssigned) {
            TS_HS_EXCEPTION(TS_HS_WARNING, TS_HS_EPS, "Bad Initialization data", TsInitializationException,
                    "Can't have two-port switch output assigned to port 0", "GunnsElectSwitchUtil");
        } else {
            mPortAssigned = configData.mPortAssigned;
        }
    } else {  // is a load switch
        if (configData.mPortAssigned > 0) {
            TS_HS_EXCEPTION(TS_HS_WARNING, TS_HS_EPS, "Bad Initialization data", TsInitializationException,
                    "Can't have port other than 0 assigned to output of non-two port switch", "GunnsElectSwitchUtil");
        }
    }

    if (configData.mTripPriority < 1) {
        TS_HS_EXCEPTION(TS_HS_WARNING, TS_HS_EPS, "Bad Initialization data", TsInitializationException,
                "Can't have a trip priority less than 1", "GunnsElectSwitchUtil");
    } else {
        mTripPriority = configData.mTripPriority;
    }

    if (!configData.mIsTwoPortSwitch && configData.mTripPriority > 1) {
        TS_HS_EXCEPTION(TS_HS_WARNING, TS_HS_EPS, "Bad Initialization data", TsInitializationException,
                "non two-port switch can't have trip priority greater than 1", "GunnsElectSwitchUtil");
    }

    mSwitchCommandedClosed = inputData.mSwitchIsAutoClosed;
    mSwitchIsClosed = inputData.mSwitchIsAutoClosed;

    if (inputData.mPosTripLimit < 0.0) {
        TS_HS_EXCEPTION(TS_HS_WARNING, TS_HS_EPS, "Bad Initialization data", TsInitializationException,
                "Can't have positive trip limit less than zero", "GunnsElectSwitchUtil");
    } else {
        mDefaultPosTripLimit = inputData.mPosTripLimit;
    }

    if (inputData.mNegTripLimit > 0.0) {
        TS_HS_EXCEPTION(TS_HS_WARNING, TS_HS_EPS, "Bad Initialization data", TsInitializationException,
                "Can't have negative trip limit greater than zero", "GunnsElectSwitchUtil");
    } else {
        mDefaultNegTripLimit = inputData.mNegTripLimit;
    }

    mPortsAreReversed = configData.mPortsAreReversed;
    mActiveSwitchResistance = mDefaultSwitchResistance;
    mActivePosTripLimit = mDefaultPosTripLimit;
    mActiveNegTripLimit = mDefaultNegTripLimit;

    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  powerSupplyValid    (--)  flag for whether the switch has power to close.
///
/// @details  Method: updateSwitchState processes open and close commands and malfunctions.
///                   The CLOSE command is accepted if the switch is not currently tripped,
///                   waiting to trip, or malfunctioned to be failed stuck open.  The OPEN
///                   command is accepted if the switch is not malfunctioned to be stuck
///                   closed.  This is done *before* the GUNNs network is updated.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectSwitchUtil::updateSwitchState(const bool powerSupplyValid)
{
    // calculate any active malfunctions

    // switch positive trip setpoint malfunction
    if (mMalfPosTripSetpointFail) {
        mActivePosTripLimit = mMalfPosTripSetpointFailValue;
    } else {
        mActivePosTripLimit = mDefaultPosTripLimit;
    }

    // switch negative trip setpoint malfunction
    if (mMalfNegTripSetpointFail) {
        mActiveNegTripLimit = mMalfNegTripSetpointFailValue;
    } else {
        mActiveNegTripLimit = mDefaultNegTripLimit;
    }

    // block/ignore current trips only when malf active other wise take the value from the signal ICD.
    if (mMalfIgnoreCurrentTrip) {
        mOverCurrentProtection = false;
    } else {
        // go back to whatever the signal aspect sends over the ICD
    }

    // sanity check for minimum switch resistance, if someone changes it directly
    if (mDefaultSwitchResistance <= 0.0) {
        mDefaultSwitchResistance = 1.0 / mConductanceLimit;
    }

    // Switch resistance fail malfunction
    if (mMalfFailResistance) {
        if (mMalfFailResistPercent > 1.0) {   // if user value is 0 to 100 scale it to 0 to 1
            mMalfFailResistPercent /= 100.0;
        }
        mActiveSwitchResistance = mDefaultSwitchResistance * mMalfFailResistPercent;
    } else {
        mActiveSwitchResistance = mDefaultSwitchResistance;
    }

    // apply commanded switch position
    if (!isTripped() && !isWaitingToTrip() && powerSupplyValid) {
        mSwitchIsClosed = mSwitchCommandedClosed;
    }

    // if power supply is off then open the switch and reset all trip flags
    if (!powerSupplyValid) {
        mSwitchIsClosed = false;
        mSwitchCommandedClosed = false;
        mTripReset = true;
    } else {
        // PR 41530:  close malfs shouldn't be applicable if the switch power is removed
        if (mMalfFailClosed) {
            mSwitchIsClosed = true;
        } else if (mMalfFailOpen) {
            mSwitchIsClosed = false;
        }
    }

    // if switch is not commanded close, assume this to be an open command and clear the trip flags
    if (isTripped()) {
        if (!mSwitchCommandedClosed) {
            mTripReset = true;
        }
    }

    // flag to reset all trip flags
    if (mTripReset) {
        mExternalTrip = false;
        mPosTrip = false;
        mNegTrip = false;
        mWaitingToTrip = false;
        mTripReset = false;
    }

    // build an enumeration to use on display pages to call out when the switch is malf'd
    if(!mSwitchIsClosed) {
        mSwitchStateEnum = 0;
    } else {
        mSwitchStateEnum = 1;
    }
    if(mMalfFailOpen) {
        mSwitchStateEnum = 2;
    }
    if(mMalfFailClosed) {
        mSwitchStateEnum = 3;
    }
    if(mMalfFailOpen && mMalfFailClosed) {
        mSwitchStateEnum = 4;
    }
    if(mPosTrip) {
        mSwitchStateEnum = 5;
    }
    if(mNegTrip) {
        mSwitchStateEnum = 6;
    }
    if(mExternalTrip) {
        mSwitchStateEnum = 7;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    currentActual        (amp) current through the switch, to calculate parasitic heat
/// @param[in[    currentSensed        (amp) sensed version of current through the switch, for tripping
/// @param[in]    voltage              (V) voltage at the switch input, to calculate under-volt trip
/// @param[in]    currentStep          (--) will be used for trip timing calculation
/// @param[in]    isMinorStep          (--) flag to determine if this is being run in minorStep or confirmSolutionAcceptable
///
/// @details  Method: updateSwitchFlow   calculate switch current and perform trips if necessary.
/// This is done *after* the GUNNS network is updated/solved. To get the switches to trip in the
/// correct order get the current minor step from the GUNNS network, then only allow the switches
/// to trip if they are high enough priority.
/// EXAMPLE:  A DDCU powers an RPCM (A)that powers another RPCM (B).  The switches on RPCM B
/// should trip before the switches on RPCM A, which should trip before the DDCU does.  Each
/// is assigned a trip priority, 1 to the max number of minor steps, with the first to trip
/// assigned the lowest number.  If the current minor step is less than the trip priority,
/// then the switch is not allowed to open even if the current is greater than the trip
/// limit.  Then the network is solved.  As the minor steps are gone through eventually the
/// switch responsible for the trip will be opened and the network will resolve without
/// a trip condition
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectSwitchUtil::updateSwitchFlow(const double currentActual,
                                            const double currentSensed,
                                            const double voltage,
                                            const int    currentStep,
                                            const bool   isMinorStep = false)
{
    mWaitingToTrip = false;
    mCurrentActual = 0.0;
    mCurrentSensed = 0.0;
    mVoltageIn = voltage;
    bool lStartingState = false;

    if (mSwitchIsClosed) {
        mCurrentActual = currentActual;
        mCurrentSensed = currentSensed;
    }

    // save trip status before processing
    lStartingState = isTripped();

    if (!mMalfFailClosed) {                          // if switch is failed closed don't bother checking for trips, it isn't supposed to open
        if (mOverCurrentProtection) {                // if this is a switch that reacts to over-current condition
            if (mCurrentSensed > mActivePosTripLimit) {    // if current from port 0 to port 1 is greater than the trip point
                if (currentStep >= mTripPriority) {  // if this switch is allowed to trip yet
                    TsHsMsg msg(TS_HS_WARNING, TS_HS_EPS);
                    msg << mSwitchCardName << " Switch " << mSwitchCardPosition << " Positive Current trip at minor step " << currentStep << ".  " << mCurrentSensed << " > trip limit of " << mActivePosTripLimit;
                    hsSendMsg(msg);

                    mCurrentActual = 0.0;
                    mSwitchIsClosed = false;
                    mWaitingToTrip = false;
                    mPosTrip = true;

                } else {                              // can't trip yet, but need to let GUNNS know
                    mWaitingToTrip = true;            // to keep minor stepping until it can
                    mPosTrip = false;
                }
            }  // if current > limit

            if (mCurrentSensed < mActiveNegTripLimit) {   // if current from port 1 to port 0 is greater than the trip point
                if (currentStep >= mTripPriority) {
                    TsHsMsg msg(TS_HS_WARNING, TS_HS_EPS);
                    msg << mSwitchCardName << " Switch " << mSwitchCardPosition << " Negative Current trip at minor step " << currentStep << ".  " << mCurrentSensed << " > trip limit of " << mActiveNegTripLimit;
                    hsSendMsg(msg);

                    mCurrentActual = 0.0;
                    mSwitchIsClosed = false;
                    mWaitingToTrip = false;
                    mNegTrip = true;

                } else {                    // can't trip yet, but need to let GUNNS know
                    mWaitingToTrip = true;  // to keep minor stepping until it can
                    mNegTrip = false;
                }
            }  // if current < limit
        }  // if can current trip

        // only announce the externally determined trip if the switch is still closed
        if (mExternalTrip && mSwitchIsClosed) {
            mCurrentActual = 0.0;
            mSwitchIsClosed = false;
            mWaitingToTrip = false;
            TsHsMsg msg(TS_HS_WARNING, TS_HS_EPS);
            msg << mSwitchCardName << " Switch " << mSwitchCardPosition << " Externally determined trip at minor step " << currentStep;
            hsSendMsg(msg);
        }
    }  // if not malf'd closed

    // only change the Just-tripped status during confirmSolutionAcceptable
    if (!isMinorStep) {
        // flag the instant the switch transitions from not tripped to tripped
        if (isTripped() && !lStartingState) {
            mJustTripped = true;
        } else {
            mJustTripped = false;
        }
    }

    if (getConductance() > 0.0) {
        mPowerDissipation = fabs((mCurrentActual * mCurrentActual)) / getConductance();
    } else {
        mPowerDissipation = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Method: set the flag to clear all trips.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectSwitchUtil::setTripReset()
{
    mTripReset = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Method:  sets flag for a trip determined by the switch owning object.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectSwitchUtil::setExternalTrip(const bool externalTripFlag)
{
    mExternalTrip = externalTripFlag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Method: setPosTriplimit   for SwitchCard unit testing, to adjust the positive trip setpoint.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectSwitchUtil::setPosTripLimit(const double posTripLimit)
{
    mDefaultPosTripLimit = posTripLimit;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Method: setNegTripLimit   for SwitchCard unit testing, to adjust the negative trip setpoint.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectSwitchUtil::setNegTripLimit(const double negTripLimit)
{
    mDefaultNegTripLimit = negTripLimit;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Method: setSwitchCommandedClosed   for SwitchCard unit testing, to close the switch.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectSwitchUtil::setSwitchCommandedClosed(const bool switchCommandedClosed)
{
    mSwitchCommandedClosed = switchCommandedClosed;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--) Malfunction activation flag, true activates.
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectSwitchUtil::setMalfFailClosed(const bool flag)
{
    mMalfFailClosed = flag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--) Malfunction activation flag, true activates.
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectSwitchUtil::setMalfFailOpen(const bool flag)
{
    mMalfFailOpen = flag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--) Malfunction activation flag, true activates.
/// @param[in] value (--) Malfunction resistance value.
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectSwitchUtil::setMalfFailResistance(const bool flag, const double value)
{
    mMalfFailResistance    = flag;
    mMalfFailResistPercent = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--) Malfunction activation flag, true activates.
/// @param[in] value (--) Malfunction setpoint value.
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectSwitchUtil::setMalfPosTripSetpoint(const bool flag, const double value)
{
    mMalfPosTripSetpointFail      = flag;
    mMalfPosTripSetpointFailValue = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--) Malfunction activation flag, true activates.
/// @param[in] value (--) Malfunction setpoint value.
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectSwitchUtil::setMalfNegTripSetpoint(const bool flag, const double value)
{
    mMalfNegTripSetpointFail      = flag;
    mMalfNegTripSetpointFailValue = value;
}
