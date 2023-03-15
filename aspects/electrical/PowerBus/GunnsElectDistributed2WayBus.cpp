/**
@file     GunnsElectDistributed2WayBus.cpp
@brief    GUNNS Electrical Distributed 2-Way Bus Interface implementation

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   ()
*/

#include "GunnsElectDistributed2WayBus.hh"
#include <sstream>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] level   (--) Severity level of the message.
/// @param[in] message (--) Detailed message string.
///
/// @details  Electrical Distributed 2-Way Bus notification message default constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectDistributed2WayBusNotification::GunnsElectDistributed2WayBusNotification(
        const NotificationLevel level,
        const std::string& message)
    :
    mLevel(level),
    mMessage(message)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Electrical Distributed 2-Way Bus notification message default destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectDistributed2WayBusNotification::~GunnsElectDistributed2WayBusNotification()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Electrical Distributed 2-Way Bus notification message copy constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectDistributed2WayBusNotification::GunnsElectDistributed2WayBusNotification(const GunnsElectDistributed2WayBusNotification& that)
    :
    mLevel(that.mLevel),
    mMessage(that.mMessage)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Electrical Distributed 2-Way Bus notification message assignment operator.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectDistributed2WayBusNotification& GunnsElectDistributed2WayBusNotification::operator =(const GunnsElectDistributed2WayBusNotification& that)
{
    if (this != &that) {
        this->mLevel   = that.mLevel;
        this->mMessage = that.mMessage;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Electrical Distributed 2-Way Bus Interface default constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectDistributed2WayBus::GunnsElectDistributed2WayBus()
    :
    mInData(),
    mOutData(),
    mIsPrimarySide(false),
    mForcedRole(NONE),
    mSupplyDatas(),
    mLoopLatency(0),
    mFramesSinceFlip(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Electrical Distributed 2-Way Bus Interface default destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectDistributed2WayBus::~GunnsElectDistributed2WayBus()
{
    /// - Delete the supply data objects that we allocated.
    for (unsigned int i=0; i<mSupplyDatas.size(); ++i) {
        delete mSupplyDatas.at(i);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] isPrimarySide (--) This is the primary side of the interface if true.
/// @param[in] voltage       (V)  Initial voltage at the interface location.
///
/// @details  Initializes this Electrical Distributed 2-Way Bus Interface with its name and initial
///           voltage.  This should be called once at simulation initialization, before any calls to
///           the 'update' functions.  The primary side of the interface defaults to Supply role at
///           init, while the other side defaults to Demand.  If both sides initialize to the same
///           mode, they will sort it out at runtime.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectDistributed2WayBus::initialize(const bool isPrimarySide, const float voltage)
{
    mIsPrimarySide = isPrimarySide;

    mInData.mDemandMode    = mIsPrimarySide;
    mInData.mDemandPower   = 0.0;
    mInData.mSupplyVoltage = voltage;

    mOutData.mDemandMode    = not mIsPrimarySide;
    mOutData.mDemandPower   = 0.0;
    mOutData.mSupplyVoltage = voltage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Increments the outgoing frame counter by one, reflects the incoming frame count back
///           to the other side, and calculates the round-trip data loop time in frames.
///
/// @note  This should be called exactly once per main model step, and before any calls to update.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectDistributed2WayBus::updateFrameCounts()
{
    mOutData.mFrameCount++;
    mFramesSinceFlip++;
    mLoopLatency            = mOutData.mFrameCount - mInData.mFrameLoopback;
    mOutData.mFrameLoopback = mInData.mFrameCount;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// param[in] localVoltage     (V) Voltage in the local model at this interface location.
/// param[in] localPowerDemand (W) Power demanded from the remote model at this interface location.
///
/// @details  Updates the mode switching logic and drives the output supply voltage and power demand
///           to the other side of the interface.  This should be called as many times as needed
///           during each main model step to ensure this is updated with the latest local circuit
///           voltage amd power draw.
///
/// @note  This should be called after updateFrameCounts() during each main model step.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectDistributed2WayBus::update(const float localVoltage, const float localPowerDemand)
{
    /// - Find the highest available voltage that can be supplied here from the local model.
    float availV = 0.0;
    for (unsigned int i=0; i<mSupplyDatas.size(); ++i) {
        if (mSupplyDatas.at(i)->mAvailable and mSupplyDatas.at(i)->mMaximumVoltage > availV) {
            availV = mSupplyDatas.at(i)->mMaximumVoltage;
        }
    }

    switch (mForcedRole) {
        case SUPPLY:
            mOutData.mDemandMode = false;
            break;
        case DEMAND:
            mOutData.mDemandMode = true;
            break;
        default: // default role determination
            /// - Mode switching logic:
            ///   - If we are in Demand, the remote side is also in Demand, and sufficient time has
            ///     passed since our last mode change, then we switch to Supply.
            ///   - If we are in Supply and our highest available local supply voltage < remote's
            ///     supply voltage, then we switch to Demand.  This keeps the Supply role on the
            ///     side with the higher available local supply voltage.
            /// - Wait and only do mode changes when we have valid data from other side.
            if (mInData.mFrameLoopback > 0) {
                if (mOutData.mDemandMode) {
                    if (mInData.mDemandMode and (mFramesSinceFlip > mLoopLatency)) {
                        mOutData.mDemandMode = false;
                        mFramesSinceFlip     = 0;
                        pushNotification(GunnsElectDistributed2WayBusNotification::INFO,
                                "flipping to Supply role in response to remote's takeover of Demand role.");
                    }
                } else {
                    if (availV < mInData.mSupplyVoltage) {
                        mOutData.mDemandMode = true;
                        mFramesSinceFlip     = 0;
                        std::ostringstream msg;
                        msg << "flipping to Demand role with available V: " << availV << " < remote V: " << mInData.mSupplyVoltage << ".";
                        pushNotification(GunnsElectDistributed2WayBusNotification::INFO, msg.str());
                    }
                }
            }
            break;
    }

    /// - Update the voltage sent to the other side.  When in Demand mode, we send our highest
    ///   available local supply voltage.  This allows the other side (Supply mode) to decide when
    ///   to switch to Demand mode based on the available voltage supplies in both models.  When in
    ///   Supply mode, we send the actual voltage at our interface location.  This allows the two
    ///   model's voltage boundary conditions to match.
    /// - Update the power demand sent to the other side.  In Supply mode, since we are supplying
    ///   voltage to the other side, we force the output power demand to be zero.
    if (mOutData.mDemandMode) {
        mOutData.mSupplyVoltage = availV;
        mOutData.mDemandPower   = localPowerDemand;
    } else {
        mOutData.mSupplyVoltage = localVoltage;
        mOutData.mDemandPower   = 0.0;
    }
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
unsigned int GunnsElectDistributed2WayBus::popNotification(GunnsElectDistributed2WayBusNotification& notification)
{
    if (mNotifications.size() > 0) {
        notification = mNotifications.back();
        mNotifications.pop_back();
    } else {
        notification.mLevel   = GunnsElectDistributed2WayBusNotification::NONE;
        notification.mMessage = "";
    }
    return mNotifications.size();
}
