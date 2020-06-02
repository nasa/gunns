/**
@file
@brief    GUNNS Photovoltaic Regulator Trip Functions implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((aspects/electrical/TripLogic/GunnsTripLogic.o))
*/

#include "GunnsElectPvRegTrips.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Photovoltaic Regulator Trip Functions.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvRegTrips::GunnsElectPvRegTrips()
    :
    mInOverVoltage(),
    mInOverCurrent(),
    mOutOverVoltage(),
    mOutOverCurrent(),
    mOutUnderVoltage()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Photovoltaic Regulator Trip Functions.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvRegTrips::~GunnsElectPvRegTrips()
{
    // nothing to do
}

void GunnsElectPvRegTrips::resetTrips()
{
    mInOverVoltage.resetTrip();
    mInOverCurrent.resetTrip();
    mOutOverVoltage.resetTrip();
    mOutOverCurrent.resetTrip();
    mOutUnderVoltage.resetTrip();
}
