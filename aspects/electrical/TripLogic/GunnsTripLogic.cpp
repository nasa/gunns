/**
@file
@brief    GUNNS Trip Logic Model implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ()
*/

#include "GunnsTripLogic.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Trip Logic.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsTripLogic::GunnsTripLogic()
    :
    mMalfInhibitTrip(false),
    mMalfForceTrip(false),
    mLimit(0.0),
    mPriority(0),
    mIsTripped(false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Trip Logic.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsTripLogic::~GunnsTripLogic()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] limit     (--) Trip limit value.
/// @param[in] priority  (--) Network converged step trip priority.
/// @param[in] isTripped (--) Initial tripped state.
///
/// @details  Initializes this GUNNS Trip Logic.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsTripLogic::initialize(const float        limit,
                                const unsigned int priority,
                                const bool         isTripped)
{
    /// - Initialize from arguments.
    mLimit     = limit;
    mPriority  = priority;
    mIsTripped = isTripped;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in, out] result        (--) Caller's network solution assessment to be updated.
/// @param[in]      param         (--) The value to be checked against the trip limit.
/// @param[in]      convergedStep (--) The network's current converged minor step number.
///
/// @returns  bool (--) True if a trip just occurred during this call, false otherwise.
///
/// @details  Checks the trip condition, sets the tripped state at the appropriate time, and updates
///           the caller's network solution assessment.  This returns whether a trip just occurred
///           during this call, which allows the caller to take action when the trip occurs.
///
/// @note This does NOT set result = CONFIRM if not in the trip condition.  The caller should
///       initialize result before checking for trips.
///
/// @note If result is already REJECT when passed in, we still check for the trip.  This allows a
///       link to have multiple trip logics that can occur in the same minor step.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsTripLogic::checkForTrip(GunnsBasicLink::SolutionResult& result,
                                  const float param, const int convergedStep)
{
    bool justTripped = false;

    /// - The inhibit malfunction overrides the force malfunction, and resets any current trip.
    ///   Checks for trips is also skipped if already tripped or the trip limit or converged step
    ///   priority values haven't been initialized.
    if (mMalfInhibitTrip) resetTrip();
    if (not (mIsTripped or mMalfInhibitTrip or 0.0 == mLimit or 1 > mPriority)) {
        /// - The force trip malfunction causes an immediate trip on the first call to this
        ///   function, regardless of the network converged step or trip priority.  To force a trip
        ///   at the normal converged step priority, use the sensor's fail-to malf.
        if (mMalfForceTrip or isTripCondition(param)) {

            if (mMalfForceTrip or static_cast<unsigned int>(convergedStep) == mPriority) {
                /// - When the trip condition is satisfied and at the appropriate time, then set the
                ///   tripped state and set the caller's network solution assessment to rejected.
                mIsTripped  = true;
                justTripped = true;
                result      = GunnsBasicLink::REJECT;

            } else if (GunnsBasicLink::REJECT != result) {
                /// - We only DELAY if the result wasn't already REJECT, so that we don't interfere with
                ///   another trip that has already occurred.
                result = GunnsBasicLink::DELAY;
            }
        }
    }
    return justTripped;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Trip Greater Than Logic.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsTripGreaterThan::GunnsTripGreaterThan()
    :
    GunnsTripLogic()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Trip Greater Than Logic.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsTripGreaterThan::~GunnsTripGreaterThan()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Trip Less Than Logic.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsTripLessThan::GunnsTripLessThan()
    :
    GunnsTripLogic()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Trip Less Than Logic.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsTripLessThan::~GunnsTripLessThan()
{
    // nothing to do
}
