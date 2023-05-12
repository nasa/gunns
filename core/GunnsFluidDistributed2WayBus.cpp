/**
@file     GunnsFluidDistributed2WayBus.cpp
@brief    GUNNS Fluid Distributed 2-Way Bus Interface implementation

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   ()
*/

#include "GunnsFluidDistributed2WayBus.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this distributed fluid mixture data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidDistributedMixtureData::GunnsFluidDistributedMixtureData()
    :
    mEnergy(0.0),
    mMoleFractions(0),
    mTcMoleFractions(0),
    mNumFluid(0),
    mNumTc(0),
    mNumFluidIf(0),
    mNumTcIf(0),
    mNumFluidCommon(0),
    mNumTcCommon(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this distributed fluid mixture data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidDistributedMixtureData::~GunnsFluidDistributedMixtureData()
{
    delete [] mTcMoleFractions;
    delete [] mMoleFractions;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] nBulk   (--) Number of bulk fluid constituents in the model.
/// @param[in] nTc     (--) Number of trace compounds in the model.
/// @param[in] nIfBulk (--) Number of bulk fluid constituents in the interface data.
/// @param[in] nIfTc   (--) Number of trace compounds in the interface data.
///
/// @details  Allocates arrays for bulk fluid and trace compounds mole fractions.  The sizes of the
///           model's mixture arrays can be smaller than the interface data arrays, because the
///           interface arrays may be sized to handle other models as well.  Our interface arrays
///           are sized to match the interface data size, but we also store the model's array sizes
///           so we know not to overstep them when reading & writing between them and the interface
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedMixtureData::initialize(const unsigned int nBulk,
                                                  const unsigned int nTc,
                                                  const unsigned int nIfBulk,
                                                  const unsigned int nIfTc)
{
    mNumFluid       = nBulk;
    mNumTc          = nTc;
    mNumFluidIf     = nIfBulk;
    mNumTcIf        = nIfTc;
    mNumFluidCommon = std::min(mNumFluid, mNumFluidIf);
    mNumTcCommon    = std::min(mNumTc,    mNumTcIf);
    if (nIfBulk > 0) {
        delete [] mMoleFractions;
        mMoleFractions = new double[nIfBulk];
        for (unsigned int i=0; i<nIfBulk; ++i) {
            mMoleFractions[i] = 0.0;
        }
    }
    if (nIfTc > 0) {
        delete [] mTcMoleFractions;
        mTcMoleFractions = new double[nIfTc];
        for (unsigned int i=0; i<nIfTc; ++i) {
            mTcMoleFractions[i] = 0.0;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] fractions (--) Array of bulk fluid mole fraction values to copy to the interface.
///
/// @details  Sets this interface's bulk fluid mole fractions to the given values.  If the
///           interface array is larger than the given array, then the remaining values in the
///           interface array are filled with zeroes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedMixtureData::setMoleFractions(const double* fractions)
{
    for (unsigned int i=0; i<mNumFluidCommon; ++i) {
        mMoleFractions[i] = fractions[i];
    }
    for (unsigned int i=mNumFluidCommon; i<mNumFluidIf; ++i) {
        mMoleFractions[i] = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] fractions (--) Array of trace compound mole fractions to copy to the interface.
///
/// @details  Sets this interface's trace compound mole fractions to the given values.  If the
///           interface array is larger than the given array, then the remaining values in the
///           interface array are filled with zeroes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedMixtureData::setTcMoleFractions(const double* fractions)
{
    for (unsigned int i=0; i<mNumTcCommon; ++i) {
        mTcMoleFractions[i] = fractions[i];
    }
    for (unsigned int i=mNumTcCommon; i<mNumTcIf; ++i) {
        mTcMoleFractions[i] = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] fractions (--) Array of bulk fluid mole fractions to copy from the interface.
///
/// @details  Sets the given builk fluid mole fractions to this interface's values.  If the given
///           array is larger than the interface array, then the remaining values in the given array
///           are filled with zeroes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedMixtureData::getMoleFractions(double* fractions) const
{
    for (unsigned int i=0; i<mNumFluidCommon; ++i) {
        fractions[i] = mMoleFractions[i];
    }
    for (unsigned int i=mNumFluidCommon; i<mNumFluid; ++i) {
        fractions[i] = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] fractions (--) Array of trace compound mole fractions to copy from the interface.
///
/// @details  Sets the given builk trace compound fractions to this interface's values.  If the
///           given array is larger than the interface array, then the remaining values in the given
///           array are filled with zeroes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedMixtureData::getTcMoleFractions(double* fractions) const
{
    for (unsigned int i=0; i<mNumTcCommon; ++i) {
        fractions[i] = mTcMoleFractions[i];
    }
    for (unsigned int i=mNumTcCommon; i<mNumTc; ++i) {
        fractions[i] = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this distributed fluid state data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidDistributed2WayBusFluidState::GunnsFluidDistributed2WayBusFluidState()
    :
    GunnsFluidDistributedMixtureData(),
    mPressure(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this distributed fluid state data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidDistributed2WayBusFluidState::~GunnsFluidDistributed2WayBusFluidState()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this distributed flow state data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidDistributed2WayBusFlowState::GunnsFluidDistributed2WayBusFlowState()
    :
    GunnsFluidDistributedMixtureData(),
    mFlowRate(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this distributed flow state data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidDistributed2WayBusFlowState::~GunnsFluidDistributed2WayBusFlowState()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] level   (--) Severity level of the message.
/// @param[in] message (--) Detailed message string.
///
/// @details  Constructs this notification message with the given values.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidDistributed2WayBusNotification::GunnsFluidDistributed2WayBusNotification(
        const NotificationLevel level,
        const std::string& message)
    :
    mLevel(level),
    mMessage(message)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Notification message default destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidDistributed2WayBusNotification::~GunnsFluidDistributed2WayBusNotification()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Object to be copied.
///
/// @details  Notification message copy constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidDistributed2WayBusNotification::GunnsFluidDistributed2WayBusNotification(const GunnsFluidDistributed2WayBusNotification& that)
    :
    mLevel(that.mLevel),
    mMessage(that.mMessage)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Object that this is to be assigned equal to.
///
/// @details  Notification message assignment operator.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidDistributed2WayBusNotification& GunnsFluidDistributed2WayBusNotification::operator =(const GunnsFluidDistributed2WayBusNotification& that)
{
    if (this != &that) {
        this->mLevel   = that.mLevel;
        this->mMessage = that.mMessage;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Fluid Distributed 2-Way Bus interface data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidDistributed2WayBusInterfaceData::GunnsFluidDistributed2WayBusInterfaceData()
    :
    GunnsFluidDistributedMixtureData(),
    mFrameCount(0),
    mFrameLoopback(0),
    mDemandMode(false),
    mCapacitance(0.0),
    mSource(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Fluid Distributed 2-Way Bus interface data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidDistributed2WayBusInterfaceData::~GunnsFluidDistributed2WayBusInterfaceData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Object that this is to be assigned equal to.
///
/// @details  Assigns values of this object's attributes to the given object's values.  The
///           mNumFluid and similar terms are not changed, and we assume that the two objects were
///           initialized identically; the array sizes and mNum* terms are the same.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidDistributed2WayBusInterfaceData& GunnsFluidDistributed2WayBusInterfaceData::operator =(const GunnsFluidDistributed2WayBusInterfaceData& that)
{
    if (this != &that) {
        mFrameCount    = that.mFrameCount;
        mFrameLoopback = that.mFrameLoopback;
        mDemandMode    = that.mDemandMode;
        mCapacitance   = that.mCapacitance;
        mSource        = that.mSource;
        mEnergy        = that.mEnergy;
        for (unsigned int i=0; i<mNumFluid; ++i) {
            mMoleFractions[i] = that.mMoleFractions[i];
        }
        for (unsigned int i=0; i<mNumTc; ++i) {
            mTcMoleFractions[i] = that.mTcMoleFractions[i];
        }
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  (--)  True if all data validation checks passed.
///
/// @details  Checks for all of the following conditions to be met:  Frame count > 0, energy > 0,
///           capacitance >= 0, pressure >= 0 (only in Supply mode), and all mixture fractions >= 0.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidDistributed2WayBusInterfaceData::hasValidData() const
{
    if (mFrameCount < 1 or mEnergy <= 0.0 or mCapacitance < 0.0 or (mSource < 0.0 and not mDemandMode)) {
        return false;
    }
    for (unsigned int i=0; i<mNumFluid; ++i) {
        if (mMoleFractions[i] < 0.0) {
            return false;
        }
    }
    for (unsigned int i=0; i<mNumTc; ++i) {
        if (mTcMoleFractions[i] < 0.0) {
            return false;
        }
    }
    return true;
}

/// @details  Upper limit of ratio of Supply-side capacitance over Demand-side capacitance, above
///           which the stability filter imposes no limit on Demand-side flow rate.
const double GunnsFluidDistributed2WayBus::mModingCapacitanceRatio = 1.25;
/// @details  Constant in the lag gain equation: lag_gain = 1.5 * 0.75^lag_frames
const double GunnsFluidDistributed2WayBus::mDemandFilterConstA = 1.5;
/// @details  Constant in the lag gain equation: lag_gain = 1.5 * 0.75^lag_frames
const double GunnsFluidDistributed2WayBus::mDemandFilterConstB = 0.75;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Fluid Distributed 2-Way Bus Interface.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidDistributed2WayBus::GunnsFluidDistributed2WayBus()
    :
    mInData                (),
    mOutData               (),
    mIsPairMaster          (false),
    mInDataLastDemandMode  (false),
    mFramesSinceFlip       (0),
    mLoopLatency           (0),
    mDemandLimitGain       (0.0),
    mDemandLimitFlowRate   (0.0),
    mForcedRole            (NONE),
    mNotifications         ()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Fluid Distributed 2-Way Bus Interface.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidDistributed2WayBus::~GunnsFluidDistributed2WayBus()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] isPairMaster (--) True if this is the master side of the interface, for tie-breaking.
/// @param[in] nBulk        (--) Number of bulk fluid constituents in the model.
/// @param[in] nTc          (--) Number of trace compounds in the model.
/// @param[in] nIfBulk      (--) Number of bulk fluid constituents in the interface data.
/// @param[in] nIfTc        (--) Number of trace compounds in the interface data.
///
/// @details  Initializes this Fluid Distributed 2-Way Bus Interface.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributed2WayBus::initialize(const bool         isPairMaster,
                                              const unsigned int nBulk,
                                              const unsigned int nTc,
                                              const unsigned int nIfBulk,
                                              const unsigned int nIfTc)
{
    /// - Initialize the interface data objects so they can allocate memory.
    mInData .initialize(nBulk, nTc, nIfBulk, nIfTc);
    mOutData.initialize(nBulk, nTc, nIfBulk, nIfTc);

    /// - Initialize remaining state variables.  mForcedRole is not initialized, assuming the user
    ///   may have already set it.
    mIsPairMaster         = isPairMaster;
    mInDataLastDemandMode = false;
    mFramesSinceFlip      = 0;
    mLoopLatency          = 0;
    mDemandLimitGain      = 0.0;
    mDemandLimitFlowRate  = 0.0;

    /// - Both sides start out in Supply mode by default.  Upon going to run and transmitting their
    ///   capacitances, the side with the smaller capacitance will transition to Demand mode, with
    ///   the Master side determining a tie-breaker.
    mOutData.mDemandMode = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] fluid (--) The fluid state in the interface volume.
///
/// @details  When this in the Supply role, this copies the given fluid state, describing the fluid
///           state in the interface volume, into the outgoing interface data for transmission to
///           the other side.
///
/// @note  This should only be called when this interface is in the Supply role, and this will push
///        a warning notification if called in the Demand role.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributed2WayBus::setFluidState(const GunnsFluidDistributed2WayBusFluidState& fluid)
{
    if (isInDemandRole()) {
        pushNotification(GunnsFluidDistributed2WayBusNotification::WARN,
                "setFluidState was called when in the Demand role.");
    } else {
        mOutData.mSource = fluid.mPressure;
        mOutData.mEnergy = fluid.mEnergy;
        mOutData.setMoleFractions(fluid.mMoleFractions);
        mOutData.setTcMoleFractions(fluid.mTcMoleFractions);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] fluid (--) The fluid state in the interface volume.
///
/// @returns  bool (--) True if the supplied fluid state object was updated, otherwise false.
///
/// @details  When in the Demand role, this copies the received interface volume fluid state from
///           the other side of the interface into the supplied state object.  The local model
///           should drive their interface volume to this fluid state boundary condition.
///
/// @note  The supplied fluid state object is not updated if this interface is not in the Demand
///        role, or if Supply role data has not been received from the other side, which can happen
///        briefly during run start or role swaps.  The returned bool value indicates whether the
///        supplied fluid state object was updated.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidDistributed2WayBus::getFluidState(GunnsFluidDistributed2WayBusFluidState& fluid) const
{
    if (isInDemandRole() and mInData.hasValidData() and not mInData.mDemandMode) {
        fluid.mPressure = mInData.mSource;
        fluid.mEnergy = mInData.mEnergy;
        mInData.getMoleFractions(fluid.mMoleFractions);
        mInData.getTcMoleFractions(fluid.mTcMoleFractions);
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flow (--) The flow state to/from the interface volume.
///
/// @details  When this in the Demand role, this copies the given flow state, describing the flow
///           state to or from the interface volume, into the outgoing interface data for
///           transmission to the other side.  By convention, positive flow rate values are for flow
///           pulled from the interface volume into the local Demand-side model, i.e. flow from the
///           Supply side to the Demand side, and negative flow rates are flow from the Demand side
///           to the Supply side.
///
/// @note  This should only be called when this interface is in the Supply role, and this will push
///        a warning notification if called in the Demand role.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributed2WayBus::setFlowState(const GunnsFluidDistributed2WayBusFlowState& flow)
{
    if (not isInDemandRole()) {
        pushNotification(GunnsFluidDistributed2WayBusNotification::WARN,
                "setFloowState was called when in the Supply role.");
    } else {
        mOutData.mSource = flow.mFlowRate;
        mOutData.mEnergy = flow.mEnergy;
        mOutData.setMoleFractions(flow.mMoleFractions);
        mOutData.setTcMoleFractions(flow.mTcMoleFractions);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] flow (--) The flow state to/from the interface volume.
///
/// @returns  bool (--) True if the supplied flow state object was updated, otherwise false.
///
/// @details  When in the Supply role, this copies the received interface flow state from the other
///           side of the interface into the supplied flow state object.  The local model should
///           apply this flow to or from their interface volume model.  By convention, positive flow
///           rate values are for flow pulled from the interface volume into the local Demand-side
///           model, i.e. flow from the Supply side to the Demand side, and negative flow rates are
///           flow from the Demand side to the Supply side.  This means that the local Supply volume
///           model should subtract the integral of the given flow rate from its fluid mass.
///
/// @note  The supplied flow state object is not updated if this interface is not in the Supply
///        role, or if Demand role data has not been received from the other side, which can happen
///        briefly during run start or role swaps.  The returned bool value indicates whether the
///        supplied flow state object was updated.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidDistributed2WayBus::getFlowState(GunnsFluidDistributed2WayBusFlowState& flow) const
{
    if (not isInDemandRole() and mInData.hasValidData() and mInData.mDemandMode) {
        flow.mFlowRate = mInData.mSource;
        flow.mEnergy   = mInData.mEnergy;
        mInData.getMoleFractions(flow.mMoleFractions);
        mInData.getTcMoleFractions(flow.mTcMoleFractions);
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Processes incoming data from the other side of the interface: checks for role swaps,
///           and updates the frame counters and loop latency measurement.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributed2WayBus::processInputs()
{
    /// - Mode changes and associated node volume update in response to incoming data.
    flipModesOnInput();

    /// - Update frame counters and loop latency measurement.
    mOutData.mFrameCount   += 1;
    mLoopLatency            = mOutData.mFrameCount - mInData.mFrameLoopback;
    mOutData.mFrameLoopback = mInData.mFrameCount;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] timestep           (s)  Model integration time step.
/// @param[in] demandSidePressure (Pa) Model pressure adjacent to the interface volume.
///
/// @details  Calculates and returns the maximum limit on molar flow rate between the Demand-side
///           model and the interface volume, based on data loop latency, and relative pressures and
///           capacitances of the interfacing sides.  When the Demand-side model limits its flow
///           rate to/from the interface volume to this limit, the interface will be stable.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidDistributed2WayBus::computeDemandLimit(const double timestep,
                                                        const double demandSidePressure)
{
    double gain      = 0.0;
    double ndotLimit = 0.0;
    if (isInDemandRole() and not mInData.mDemandMode) {
        /// - Limit inputs to avoid divide-by-zero.
        if (timestep > FLT_EPSILON and mOutData.mCapacitance > FLT_EPSILON and mInData.mCapacitance > FLT_EPSILON) {
            /// - Limited exponent for the lag gain:
            const int exponent = std::min(100, std::max(1, mLoopLatency));
            /// - Stability filter 'lag gain' imposes limit on demand flow as latency increases.
            const double lagGain = std::min(1.0, mDemandFilterConstA * powf(mDemandFilterConstB, exponent));
            /// - Limited capacitance ratio for the gain:
            const double csOverCd = std::min(mModingCapacitanceRatio, std::max(1.0, mInData.mCapacitance / mOutData.mCapacitance));
            /// - Stability filter 'gain' further limits the demand flow as Supply-side capacitance
            ///   approaches Demand-side capacitance.
            gain = lagGain + (1.0 - lagGain) * (csOverCd - 1.0) * 4.0;
            /// - Demand flow rate limit.
            ndotLimit = gain * fabs(demandSidePressure - mInData.mSource)
                      / (timestep * (1.0 / mOutData.mCapacitance + 1.0 / mInData.mCapacitance));
        }
    }
    mDemandLimitGain     = gain;
    mDemandLimitFlowRate = ndotLimit;
    return mDemandLimitFlowRate;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Handles mode flips in response to incoming data, and the initial mode flip at run
///           start.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributed2WayBus::flipModesOnInput()
{
    /// - Force mode swap based on the mode force flags.
    if (DEMAND == mForcedRole and not mOutData.mDemandMode) {
        flipToDemandMode();
    } else if (SUPPLY == mForcedRole and mOutData.mDemandMode) {
        flipToSupplyMode();
    } else if (mInData.hasValidData()) {
        /// - If in demand mode and the incoming data is also demand, then the other side has
        ///   initialized the demand/supply swap, so we flip to supply.
        if (mOutData.mDemandMode and mInData.mDemandMode and not mInDataLastDemandMode) {
            flipToSupplyMode();
        } else if (not mInData.mDemandMode and not mOutData.mDemandMode) {
            if ( (mOutData.mCapacitance < mInData.mCapacitance) or
                    (mIsPairMaster and mOutData.mCapacitance == mInData.mCapacitance) ) {
                /// - If in supply mode and the incoming data is also supply, then this is the start
                ///   of the run and the side with the smaller capacitance switches to demand mode,
                ///   and the master side is the tie-breaker.
                flipToDemandMode();
            }
        }
        mInDataLastDemandMode = mInData.mDemandMode;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Flips from supply to demand mode whenever the supply side capacitance drops below
///           some fraction of the demand side's capacitance.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributed2WayBus::flipModesOnCapacitance()
{
    /// - We do not check until we've been in supply mode for at least one full lag cycle.  This
    ///   prevents unwanted extra mode flips during large transients.
    if (mFramesSinceFlip > mLoopLatency and
            mOutData.mCapacitance * mModingCapacitanceRatio < mInData.mCapacitance) {
        flipToDemandMode();
        /// - Zero the output pressure/flow source term so the other side doesn't interpret our old
        ///   pressure value as a demand flux.  This will be set to a demand flux on the next full
        ///   pass in demand mode.
        mOutData.mSource = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Processes flipping to Demand mode.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributed2WayBus::flipToDemandMode()
{
    if (SUPPLY != mForcedRole) {
        mOutData.mDemandMode = true;
        mFramesSinceFlip = 0;
        pushNotification(GunnsFluidDistributed2WayBusNotification::INFO, "switched to Demand mode.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Processes flipping to Supply mode.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributed2WayBus::flipToSupplyMode()
{
    if (DEMAND != mForcedRole) {
        mOutData.mDemandMode = false;
        mFramesSinceFlip = 0;
        pushNotification(GunnsFluidDistributed2WayBusNotification::INFO, "switched to Supply mode.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Final updates for output data before transmission to the other side.  Sets outputs
///           capacitance to the given value.  Flips from Supply to Demand role if the new
///           capacitance is low enough, and updates the count of frames since the last mode flip.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributed2WayBus::processOutputs(const double capacitance)
{
    mOutData.mCapacitance = capacitance;
    if (not isInDemandRole()) {
        flipModesOnCapacitance();
    }
    mFramesSinceFlip++;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] GunnsElectDistributed2WayBusNotification (--) Reference to the caller's message object to copy the message into.
///
/// @returns  unsigned int (--) Number of notifications remaining in the queue.
///
/// @details  Set the caller's supplied notification object equal to the tail of the queue and pops
///           that message off of the queue, reducing the queue size by one.  If the queue size is
///           already zero, then returns an empty message.
////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int GunnsFluidDistributed2WayBus::popNotification(GunnsFluidDistributed2WayBusNotification& notification)
{
    if (mNotifications.size() > 0) {
        notification = mNotifications.back();
        mNotifications.pop_back();
    } else {
        notification.mLevel   = GunnsFluidDistributed2WayBusNotification::NONE;
        notification.mMessage = "";
    }
    return mNotifications.size();
}
