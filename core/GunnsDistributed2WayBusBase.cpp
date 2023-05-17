/**
@file     GunnsDistributed2WayBusBase.cpp
@brief    GUNNS Distributed 2-Way Bus Base Interface implementation

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   ()
*/

#include "GunnsDistributed2WayBusBase.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] level   (--) Severity level of the message.
/// @param[in] message (--) Detailed message string.
///
/// @details  Constructs this notification message with the given values.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsDistributed2WayBusNotification::GunnsDistributed2WayBusNotification(
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
GunnsDistributed2WayBusNotification::~GunnsDistributed2WayBusNotification()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Object to be copied.
///
/// @details  Notification message copy constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsDistributed2WayBusNotification::GunnsDistributed2WayBusNotification(const GunnsDistributed2WayBusNotification& that)
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
GunnsDistributed2WayBusNotification& GunnsDistributed2WayBusNotification::operator =(const GunnsDistributed2WayBusNotification& that)
{
    if (this != &that) {
        this->mLevel   = that.mLevel;
        this->mMessage = that.mMessage;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Distributed 2-Way Bus Base interface data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsDistributed2WayBusBaseInterfaceData::GunnsDistributed2WayBusBaseInterfaceData()
    :
    mFrameCount(0),
    mFrameLoopback(0),
    mDemandMode(false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Distributed 2-Way Bus Base interface data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsDistributed2WayBusBaseInterfaceData::~GunnsDistributed2WayBusBaseInterfaceData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Object that this is to be assigned equal to.
///
/// @details  Assigns values of this object's attributes to the given object's values.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsDistributed2WayBusBaseInterfaceData& GunnsDistributed2WayBusBaseInterfaceData::operator =(const GunnsDistributed2WayBusBaseInterfaceData& that)
{
    if (this != &that) {
        mFrameCount    = that.mFrameCount;
        mFrameLoopback = that.mFrameLoopback;
        mDemandMode    = that.mDemandMode;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] inDataPtr  (--) Pointer to the interface incoming data object.
/// @param[in] outDataPtr (--) Pointer to the interface outgoing data object.
///
/// @details  Default constructs this Distributed 2-Way Bus Base Interface.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsDistributed2WayBusBase::GunnsDistributed2WayBusBase(GunnsDistributed2WayBusBaseInterfaceData* inDataPtr,
                                                         GunnsDistributed2WayBusBaseInterfaceData* outDataPtr)
    :
    mIsPairMaster          (false),
    mInDataLastDemandMode  (false),
    mFramesSinceFlip       (0),
    mLoopLatency           (0),
    mForcedRole            (NONE),
    mNotifications         (),
    mInDataPtr             (inDataPtr),
    mOutDataPtr            (outDataPtr)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Distributed 2-Way Bus Base Interface.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsDistributed2WayBusBase::~GunnsDistributed2WayBusBase()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] isPairMaster (--) True if this is the master side of the interface, for tie-breaking.
///
/// @details  Initializes this Distributed 2-Way Bus Base Interface.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDistributed2WayBusBase::initialize(const bool isPairMaster)
{
    /// - Initialize remaining state variables.  mForcedRole is not initialized, assuming the user
    ///   may have already set it.
    mIsPairMaster         = isPairMaster;
    mInDataLastDemandMode = false;
    mFramesSinceFlip      = 0;
    mLoopLatency          = 0;

    /// - Both sides start out in Supply mode by default.  Upon going to run, the sides will
    ///   negotiate one of them taking over the Demand role.
    mOutDataPtr->mDemandMode = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Update frame counters and loop latency measurement.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDistributed2WayBusBase::updateFrameCounts()
{
    mOutDataPtr->mFrameCount++;
    mFramesSinceFlip++;
    mLoopLatency                = mOutDataPtr->mFrameCount - mInDataPtr->mFrameLoopback;
    mOutDataPtr->mFrameLoopback = mInDataPtr->mFrameCount;
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
unsigned int GunnsDistributed2WayBusBase::popNotification(GunnsDistributed2WayBusNotification& notification)
{
    if (mNotifications.size() > 0) {
        notification = mNotifications.back();
        mNotifications.pop_back();
    } else {
        notification.mLevel   = GunnsDistributed2WayBusNotification::NONE;
        notification.mMessage = "";
    }
    return mNotifications.size();
}
