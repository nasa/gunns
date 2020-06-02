/**
@file
@brief    Model implementation for trip manager class
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    (
     ((core/GunnsBasicLink.o))
    )

PROGRAMMERS:
    (
     ((Shailaja Janapati) (LM) (July, 2013) (TS21) (Initial Version))
    )
 */

#include "EpsTripMgr.hh"
#include "aspects/electrical/EpsMacros.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default constructs this Example model.
////////////////////////////////////////////////////////////////////////////////////////////////////
EpsTripMgr::EpsTripMgr()
      : mName(),
        mTripOccured(false),
        mWaitToTrip(false),
        mTimeToTrip(false),
        mTripPriority(0)
{
    // Nothing to do.
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default destructs this Example model.
////////////////////////////////////////////////////////////////////////////////////////////////////
EpsTripMgr::~EpsTripMgr()
{
    // Nothing to do.
}


/////////////////////////////////////////////////////////////////////////////
///
/// @param[in] tripPriority   (--) trip priority data.
/// @param[in] name (--) name of link trip manager object
/// @throws TsInitializationException on invalid initialization.
/// @details   Method for class creation.
/////////////////////////////////////////////////////////////////////////////
void EpsTripMgr::initialize(const std::string& name, const int tripPriority)
{
    TS_EPS_NAME_ERREX("EpsTripMgr");

    TS_EPS_IF_ERREX(tripPriority < 1, TsInitializationException,
            "Invalid Initialization Data", "Trip priority can't be less than 1.");
    mTripPriority = tripPriority;

}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  tripped  (--)  if the trip occured for the link
/// @return     void
///
/// @details    Check to see if the wait to trip flag can be set to false so the solution
///             can be confirmed.
////////////////////////////////////////////////////////////////////////////////////////////////////
void EpsTripMgr::computeTripState(const bool tripped)
{
    mTripOccured = tripped;

    if (mTripOccured) {
        if (!mTimeToTrip) {
            mWaitToTrip = true;
        } else {
            mWaitToTrip = false;
        }
    } else {
        resetTrips();
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  convergedStep  (--)  if the trip occured for the link
/// @return     SolutionResult (--)  status of the solution based on wait to trip
///
/// @details    verify if the solution can be confirmed, depending on if the link is waiting to trip
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicLink::SolutionResult EpsTripMgr::verifyTimeToTrip(const int convergedStep)
{

    GunnsBasicLink::SolutionResult result = GunnsBasicLink::CONFIRM;
    if (mWaitToTrip) { // If output over current trip occurs
        if (convergedStep >= mTripPriority) {  // and if it is my time to trip
            mTimeToTrip = true;
            result = GunnsBasicLink::REJECT;
        } else {
            mTimeToTrip = false;
            result = GunnsBasicLink::DELAY;
        }
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return     void
///
/// @details    reset the trip flags when the link is reset, or when there is no trip
////////////////////////////////////////////////////////////////////////////////////////////////////
void EpsTripMgr::resetTrips()
{
    mWaitToTrip = false;
    mTimeToTrip = false;
    mTripOccured = false;
}

