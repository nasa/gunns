#ifndef GunnsElectPvRegTrips_EXISTS
#define GunnsElectPvRegTrips_EXISTS

/**
@file
@brief    GUNNS Photovoltaic Regulator Trip Functions declarations

@defgroup  GUNNS_ELECTRICAL_PHOTOVOLTAIC_TRIPS    GUNNS Photovoltaic Regulator Trip Functions
@ingroup   GUNNS_ELECTRICAL_PHOTOVOLTAIC

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Photovoltaic Regulator Trip Functions.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsElectPvRegTrips.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2017-12) (Initial))

@{
*/

#include "aspects/electrical/TripLogic/GunnsTripLogic.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Photovoltaic Regulator Trip Functions.
///
/// @details  This encapsulates the trip functions for the Photovoltaic regulator links in a simple
///           container class to reduce clutter.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectPvRegTrips
{
    public:
        GunnsTripGreaterThan mInOverVoltage;     /**< (--) Input over-voltage trip function. */
        GunnsTripGreaterThan mInOverCurrent;     /**< (--) Input over-current trip function. */
        GunnsTripGreaterThan mOutOverVoltage;    /**< (--) Output over-voltage trip function. */
        GunnsTripGreaterThan mOutOverCurrent;    /**< (--) Output over-current trip function. */
        GunnsTripLessThan    mOutUnderVoltage;   /**< (--) Output under-voltage trip function. */
        /// @brief  Default constructor.
        GunnsElectPvRegTrips();
        /// @brief  Default destructor.
        virtual ~GunnsElectPvRegTrips();
        /// @brief  Resets the tripped state for all trip logics.
        void resetTrips();
        /// @brief  Returns whether any trip logics are currently tripped.
        bool isTripped() const;

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsElectPvRegTrips(const GunnsElectPvRegTrips& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsElectPvRegTrips& operator =(const GunnsElectPvRegTrips&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool  (--)  True if any trip logics are tripped, false if none are.
///
/// @details  Returns whether any trip logics are currently tripped.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectPvRegTrips::isTripped() const
{
    return mInOverVoltage.isTripped() or
           mInOverCurrent.isTripped() or
           mOutOverVoltage.isTripped() or
           mOutOverCurrent.isTripped() or
           mOutUnderVoltage.isTripped();
}

#endif
