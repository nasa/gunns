/**
@file     Distributed2WayBusFluid.cpp
@brief    Fluid Distributed 2-Way Bus Interface implementation

@copyright Copyright 2025 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((interop/Distributed2WayBusBase.o))
*/

#include "Distributed2WayBusFluid.hh"
#include <cfloat>
#include <cmath>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this distributed fluid mixture data.
////////////////////////////////////////////////////////////////////////////////////////////////////
FluidDistributedMixtureData::FluidDistributedMixtureData()
    :
    mEnergy(0.0),
    mMoleFractions(0),
    mTcMoleFractions(0),
    mNumFluid(0),
    mNumTc(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this distributed fluid mixture data.
////////////////////////////////////////////////////////////////////////////////////////////////////
FluidDistributedMixtureData::~FluidDistributedMixtureData()
{
    if (mTcMoleFractions) {
        delete [] mTcMoleFractions;
    }
    if (mMoleFractions) {
        delete [] mMoleFractions;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Object that this is to be assigned equal to.
///
/// @details  Assigns values of this object's attributes to the given object's values.  This is a
///           'deep' copy, as this object's mixture arrays remain separate from that's.  We do not
///           assume the objects have the same sized mixture arrays, so we only assign up to the
///           index of the smaller array.  If this object's array is larger than that's, we do not
///           assign the indexes in this for which that doesn't have indexes.  Since this is a deep
///           copy, the mNum array size variables are not assigned because they must reflect our
///           arrays, which are not resized.  This doesn't assume the objects have been initialized,
///           so we avoid setting or referencing mixture arrays that haven't been allocated.
////////////////////////////////////////////////////////////////////////////////////////////////////
FluidDistributedMixtureData& FluidDistributedMixtureData::operator =(const FluidDistributedMixtureData& that)
{
    if (this != &that) {
        mEnergy = that.mEnergy;
        for (unsigned int i=0; i<std::min(mNumFluid, that.mNumFluid); ++i) {
            mMoleFractions[i] = that.mMoleFractions[i];
        }
        for (unsigned int i=0; i<std::min(mNumTc, that.mNumTc); ++i) {
            mTcMoleFractions[i] = that.mTcMoleFractions[i];
        }
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] nBulk   (--) Number of bulk fluid constituents.
/// @param[in] nTc     (--) Number of trace compounds.
/// @param[in] name    (--) Not used.
///
/// @details  Allocates arrays for bulk fluid and trace compounds mole fractions.  This function is
///           virtual and the name argument exists to support derived types needing to allocate the
///           mixture arrays using a specific sim memory manager.
////////////////////////////////////////////////////////////////////////////////////////////////////
void FluidDistributedMixtureData::initialize(const unsigned int nBulk,
                                                  const unsigned int nTc,
                                                  const std::string& name __attribute__((unused)))
{
    mNumFluid       = nBulk;
    mNumTc          = nTc;

    /// - Delete & re-allocate fractions arrays in case of repeated calls to this function.
    if (mMoleFractions) {
        delete [] mMoleFractions;
        mMoleFractions = 0;
    }
    if (nBulk > 0) {
        mMoleFractions = new double[nBulk];
        for (unsigned int i=0; i<nBulk; ++i) {
            mMoleFractions[i] = 0.0;
        }
    }
    if (mTcMoleFractions) {
        delete [] mTcMoleFractions;
        mTcMoleFractions = 0;
    }
    if (nTc > 0) {
        mTcMoleFractions = new double[nTc];
        for (unsigned int i=0; i<nTc; ++i) {
            mTcMoleFractions[i] = 0.0;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] fractions (--) Array of bulk fluid mole fraction values to read from.
/// @param[in] size      (--) Size of the given fractions array.
///
/// @details  Sets this object's bulk fluid mole fractions equal to the given values.  The given
///           array can be larger or smaller than our internal array.  If our array is larger, then
///           the remaining values in the our array are filled with zeroes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void FluidDistributedMixtureData::setMoleFractions(const double* fractions, const unsigned int size)
{
    const unsigned int smallerSize = std::min(mNumFluid, size);
    for (unsigned int i=0; i<smallerSize; ++i) {
        mMoleFractions[i] = fractions[i];
    }
    for (unsigned int i=smallerSize; i<mNumFluid; ++i) {
        mMoleFractions[i] = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] fractions (--) Array of trace compound mole fractions to read from.
/// @param[in] size      (--) Size of the given fractions array.
///
/// @details  Sets this object's trace compound mole fractions equal to the given values.  The given
///           array can be larger or smaller than our internal array.  If our array is larger, then
///           the remaining values in the our array are filled with zeroes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void FluidDistributedMixtureData::setTcMoleFractions(const double* fractions, const unsigned int size)
{
    const unsigned int smallerSize = std::min(mNumTc, size);
    for (unsigned int i=0; i<smallerSize; ++i) {
        mTcMoleFractions[i] = fractions[i];
    }
    for (unsigned int i=smallerSize; i<mNumTc; ++i) {
        mTcMoleFractions[i] = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] fractions (--) Array of bulk fluid mole fractions to write to.
/// @param[in]  size      (--) Size of the given fractions array.
///
/// @details  Sets the given bulk fluid mole fractions equal to this object's values.  The given
///           array can be larger or smaller than our internal array.  If our array is smaller, then
///           the remaining values in the given array are filled with zeroes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void FluidDistributedMixtureData::getMoleFractions(double* fractions, const unsigned int size) const
{
    const unsigned int smallerSize = std::min(mNumFluid, size);
    for (unsigned int i=0; i<smallerSize; ++i) {
        fractions[i] = mMoleFractions[i];
    }
    for (unsigned int i=smallerSize; i<size; ++i) {
        fractions[i] = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] fractions (--) Array of trace compound mole fractions to write to.
/// @param[in]  size      (--) Size of the given fractions array.
///
/// @details  Sets the given builk trace compound fractions to this interface's values.  The given
///           array can be larger or smaller than our internal array.  If our array is smaller, then
///           the remaining values in the given array are filled with zeroes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void FluidDistributedMixtureData::getTcMoleFractions(double* fractions, const unsigned int size) const
{
    const unsigned int smallerSize = std::min(mNumTc, size);
    for (unsigned int i=0; i<smallerSize; ++i) {
        fractions[i] = mTcMoleFractions[i];
    }
    for (unsigned int i=smallerSize; i<size; ++i) {
        fractions[i] = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this distributed fluid state data.
////////////////////////////////////////////////////////////////////////////////////////////////////
Distributed2WayBusFluidFluidState::Distributed2WayBusFluidFluidState()
    :
    FluidDistributedMixtureData(),
    mPressure(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this distributed fluid state data.
////////////////////////////////////////////////////////////////////////////////////////////////////
Distributed2WayBusFluidFluidState::~Distributed2WayBusFluidFluidState()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Object that this is to be assigned equal to.
///
/// @details  Assigns values of this object's attributes to the given object's values.
////////////////////////////////////////////////////////////////////////////////////////////////////
Distributed2WayBusFluidFluidState& Distributed2WayBusFluidFluidState::operator =(const Distributed2WayBusFluidFluidState& that)
{
    if (this != &that) {
        FluidDistributedMixtureData::operator = (that);
        mPressure = that.mPressure;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this distributed flow state data.
////////////////////////////////////////////////////////////////////////////////////////////////////
Distributed2WayBusFluidFlowState::Distributed2WayBusFluidFlowState()
    :
    FluidDistributedMixtureData(),
    mFlowRate(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this distributed flow state data.
////////////////////////////////////////////////////////////////////////////////////////////////////
Distributed2WayBusFluidFlowState::~Distributed2WayBusFluidFlowState()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Object that this is to be assigned equal to.
///
/// @details  Assigns values of this object's attributes to the given object's values.
////////////////////////////////////////////////////////////////////////////////////////////////////
Distributed2WayBusFluidFlowState& Distributed2WayBusFluidFlowState::operator =(const Distributed2WayBusFluidFlowState& that)
{
    if (this != &that) {
        FluidDistributedMixtureData::operator = (that);
        mFlowRate = that.mFlowRate;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Fluid Distributed 2-Way Bus interface data.
////////////////////////////////////////////////////////////////////////////////////////////////////
Distributed2WayBusFluidInterfaceData::Distributed2WayBusFluidInterfaceData()
    :
    FluidDistributedMixtureData(),
    Distributed2WayBusBaseInterfaceData(),
    mCapacitance(0.0),
    mSource(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Fluid Distributed 2-Way Bus interface data.
////////////////////////////////////////////////////////////////////////////////////////////////////
Distributed2WayBusFluidInterfaceData::~Distributed2WayBusFluidInterfaceData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Object that this is to be assigned equal to.
///
/// @details  Assigns values of this object's attributes to the given object's values.
////////////////////////////////////////////////////////////////////////////////////////////////////
Distributed2WayBusFluidInterfaceData& Distributed2WayBusFluidInterfaceData::operator =(const Distributed2WayBusFluidInterfaceData& that)
{
    if (this != &that) {
        Distributed2WayBusBaseInterfaceData::operator = (that);
        FluidDistributedMixtureData::operator = (that);
        mCapacitance = that.mCapacitance;
        mSource      = that.mSource;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  (--)  True if all data validation checks passed.
///
/// @details  Checks for all of the following conditions to be met:  Frame count > 0, energy > 0,
///           capacitance >= 0, pressure >= 0 (only in Supply mode), and all mixture fractions >= 0.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool Distributed2WayBusFluidInterfaceData::hasValidData() const
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
const double Distributed2WayBusFluid::mModingCapacitanceRatio = 1.25;
/// @details  Constant in the lag gain equation: lag_gain = 1.5 * 0.75^lag_frames
const double Distributed2WayBusFluid::mDemandFilterConstA = 1.5;
/// @details  Constant in the lag gain equation: lag_gain = 1.5 * 0.75^lag_frames
const double Distributed2WayBusFluid::mDemandFilterConstB = 0.75;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Fluid Distributed 2-Way Bus Interface.
////////////////////////////////////////////////////////////////////////////////////////////////////
Distributed2WayBusFluid::Distributed2WayBusFluid()
    :
    Distributed2WayBusBase(&mInData, &mOutData),
    mInData                (),
    mOutData               (),
    mDemandLimitGain       (0.0),
    mDemandLimitFlowRate   (0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Fluid Distributed 2-Way Bus Interface.
////////////////////////////////////////////////////////////////////////////////////////////////////
Distributed2WayBusFluid::~Distributed2WayBusFluid()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] isPairMaster (--) True if this is the master side of the interface, for tie-breaking.
/// @param[in] nIfBulk      (--) Number of bulk fluid constituents in the interface data.
/// @param[in] nIfTc        (--) Number of trace compounds in the interface data.
///
/// @details  Initializes this Fluid Distributed 2-Way Bus Interface.
////////////////////////////////////////////////////////////////////////////////////////////////////
void Distributed2WayBusFluid::initialize(const bool         isPairMaster,
                                              const unsigned int nIfBulk,
                                              const unsigned int nIfTc)
{
    /// - Initialize the interface data objects so they can allocate memory.
    mInData .initialize(nIfBulk, nIfTc);
    mOutData.initialize(nIfBulk, nIfTc);

    /// - Initialize remaining state variables.
    Distributed2WayBusBase::initialize(isPairMaster);
    mDemandLimitGain     = 0.0;
    mDemandLimitFlowRate = 0.0;
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
void Distributed2WayBusFluid::setFluidState(const Distributed2WayBusFluidFluidState& fluid)
{
    if (isInDemandRole()) {
        pushNotification(Distributed2WayBusNotification::WARN,
                "setFluidState was called when in the Demand role.");
    } else {
        mOutData.mSource = fluid.mPressure;
        mOutData.mEnergy = fluid.mEnergy;
        mOutData.setMoleFractions(fluid.mMoleFractions, fluid.getNumFluid());
        mOutData.setTcMoleFractions(fluid.mTcMoleFractions, fluid.getNumTc());
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
bool Distributed2WayBusFluid::getFluidState(Distributed2WayBusFluidFluidState& fluid) const
{
    if (isInDemandRole() and mInData.hasValidData() and not mInData.mDemandMode) {
        fluid.mPressure = mInData.mSource;
        fluid.mEnergy = mInData.mEnergy;
        mInData.getMoleFractions(fluid.mMoleFractions, fluid.getNumFluid());
        mInData.getTcMoleFractions(fluid.mTcMoleFractions, fluid.getNumTc());
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
void Distributed2WayBusFluid::setFlowState(const Distributed2WayBusFluidFlowState& flow)
{
    if (not isInDemandRole()) {
        pushNotification(Distributed2WayBusNotification::WARN,
                "setFlowState was called when in the Supply role.");
    } else {
        mOutData.mSource = flow.mFlowRate;
        mOutData.mEnergy = flow.mEnergy;
        mOutData.setMoleFractions(flow.mMoleFractions, flow.getNumFluid());
        mOutData.setTcMoleFractions(flow.mTcMoleFractions, flow.getNumTc());
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
bool Distributed2WayBusFluid::getFlowState(Distributed2WayBusFluidFlowState& flow) const
{
    if (not isInDemandRole() and mInData.hasValidData() and mInData.mDemandMode) {
        flow.mFlowRate = mInData.mSource;
        flow.mEnergy   = mInData.mEnergy;
        mInData.getMoleFractions(flow.mMoleFractions, flow.getNumFluid());
        mInData.getTcMoleFractions(flow.mTcMoleFractions, flow.getNumTc());
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Processes incoming data from the other side of the interface: checks for role swaps,
///           and updates the frame counters and loop latency measurement.
////////////////////////////////////////////////////////////////////////////////////////////////////
void Distributed2WayBusFluid::processInputs()
{
    /// - Update frame counters and loop latency measurement.
    updateFrameCounts();

    /// - Mode changes and associated node volume update in response to incoming data.
    flipModesOnInput();
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
double Distributed2WayBusFluid::computeDemandLimit(const double timestep,
                                                        const double demandSidePressure)
{
    double gain      = 0.0;
    double ndotLimit = 0.0;
    if (isInDemandRole() and not mInData.mDemandMode) {
        /// - Limit inputs to avoid divide-by-zero.
        if (timestep > DBL_EPSILON and mOutData.mCapacitance > DBL_EPSILON and mInData.mCapacitance > DBL_EPSILON) {
            /// - Limited exponent for the lag gain:
            const int exponent = std::min(100, std::max(1, mLoopLatency));
            /// - Stability filter 'lag gain' imposes limit on demand flow as latency increases.
            const double lagGain = std::min(1.0, mDemandFilterConstA * std::pow(mDemandFilterConstB, exponent));
            /// - Limited capacitance ratio for the gain:
            const double csOverCd = std::min(mModingCapacitanceRatio, std::max(1.0, mInData.mCapacitance / mOutData.mCapacitance));
            /// - Stability filter 'gain' further limits the demand flow as Supply-side capacitance
            ///   approaches Demand-side capacitance.
            gain = lagGain + (1.0 - lagGain) * (csOverCd - 1.0) * 4.0;
            /// - Demand flow rate limit.
            ndotLimit = gain * std::fabs(demandSidePressure - mInData.mSource)
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
void Distributed2WayBusFluid::flipModesOnInput()
{
    /// - Force mode swap based on the mode force flags.
    if (DEMAND == mForcedRole and not isInDemandRole()) {
        flipToDemandMode();
    } else if (SUPPLY == mForcedRole and isInDemandRole()) {
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
void Distributed2WayBusFluid::flipModesOnCapacitance()
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
void Distributed2WayBusFluid::flipToDemandMode()
{
    if (SUPPLY != mForcedRole) {
        mOutData.mDemandMode = true;
        mFramesSinceFlip = 0;
        pushNotification(Distributed2WayBusNotification::INFO, "switched to Demand mode.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Processes flipping to Supply mode.
////////////////////////////////////////////////////////////////////////////////////////////////////
void Distributed2WayBusFluid::flipToSupplyMode()
{
    if (DEMAND != mForcedRole) {
        mOutData.mDemandMode = false;
        mFramesSinceFlip = 0;
        pushNotification(Distributed2WayBusNotification::INFO, "switched to Supply mode.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Final updates for output data before transmission to the other side.  Sets outputs
///           capacitance to the given value.  Flips from Supply to Demand role if the new
///           capacitance is low enough, and updates the count of frames since the last mode flip.
////////////////////////////////////////////////////////////////////////////////////////////////////
void Distributed2WayBusFluid::processOutputs(const double capacitance)
{
    mOutData.mCapacitance = capacitance;
    if (not isInDemandRole()) {
        flipModesOnCapacitance();
    }
}
