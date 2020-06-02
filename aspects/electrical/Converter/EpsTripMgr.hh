#ifndef EpsTripMgr_EXISTS
#define EpsTripMgr_EXISTS

/**
@defgroup TSM_GUNNS_ELECTRICAL_CONVERTER_TRIP_MANAGER   Electrical Model Trip Manager
@ingroup  TSM_GUNNS_ELECTRICAL_CONVERTER

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
(Tracks the trip tier logic for the converter link)

@details
REQUIREMENTS:
- ()

REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- ()

LIBRARY DEPENDENCY:
    (
     (EpsTripMgr.o)
    )

PROGRAMMERS:
    (
     ((Shailaja Janapati) (LM) (July, 2013) (TS21) (Initial Version))
    )
@{
 */

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "math/UnitConversion.hh"
#include "core/GunnsBasicLink.hh"

//////////////////////////////////////////////////////////////////////////////////////
/// @brief    Class to simulates the trip implementation of tiered EPS network
///
/////////////////////////////////////////////////////////////////////////////////////
class EpsTripMgr {
    TS_MAKE_SIM_COMPATIBLE(EpsTripMgr);

public:

    /// @brief  Default Constructor.
    EpsTripMgr();

    /// @brief  Default Destructor.
    virtual ~EpsTripMgr();

    /// @brief  Initializes the data with Config data values
    void initialize(const std::string& name, const int tripPriority);

    /// @brief  Check the state of the Trip flags when trip occurs
    void computeTripState(const bool tripped);

    /// @brief  Check is links priority is up and can be tripped
    bool isNotTimeToTrip() const;

    /// @brief  getter for time to trip flag
    bool getTimeToTrip() const;

    /// @brief  getter for waiting to trip flag
    bool getWaitToTrip() const;

   ///
   /// @param [in] convergedStep (--) the minor step count in the solver
   /// @brief   Method to check if the priority for the link is reached so the link can trip.
   ///
    GunnsBasicLink::SolutionResult verifyTimeToTrip(const int convergedStep);

    /// @brief  reset the flags when link resets itself or when trip is not active.
    void resetTrips();

protected:
    std::string mName;         /**< *o (--) trick_chkpnt_io(**) Name of links trip manager. */
    bool mTripOccured;         /**<    (--)                     Flag to notify if there is a trip that needs till to wait for its priority. */
    bool mWaitToTrip;          /**<    (--)                     Flag to make the link wait for downstream links with less priority to trip. */
    bool mTimeToTrip;          /**<    (--)                     Flag to indicate that the trip priority has reached */
    int  mTripPriority;        /**<    (--) trick_chkpnt_io(**) Trip tier to envoke trip logic in Gunns non-linear network */

private:
    /// @brief private copy constructor, prevents invocation
    EpsTripMgr(const EpsTripMgr&);
    /// @brief private assignment operator, prevents invocation
    EpsTripMgr & operator =(const EpsTripMgr& n);
};
/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return bool (--) true if not time to trip
/// @details  check if the link can trip
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool EpsTripMgr::isNotTimeToTrip() const {
    return mTripOccured && !mTimeToTrip;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return bool (--) return flag for time to trip
/// @details  get the time to trip flag
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool EpsTripMgr::getTimeToTrip() const {
    return mTimeToTrip;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return bool (--) return flag for wait to trip
/// @details  get the wait to trip flag
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool EpsTripMgr::getWaitToTrip() const {
    return mWaitToTrip;
}


#endif /* EpsTripMgr_EXIST */
