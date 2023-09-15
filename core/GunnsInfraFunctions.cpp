/**
@file
@brief    GUNNS Infrastructure Integration Function implementation

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    ()
*/

#include "GunnsInfraFunctions.hh"

#ifndef no_TRICK_ENV
#if (TRICK_VER >= 17)
#include "trick/clock_proto.h"
#else
#include "sim_services/Clock/include/clock_proto.h"
#endif
#include "sim_services/MonteCarlo/include/montecarlo_c_intf.h"
#endif

/// @details Defines the static variable, useful for non-Trick implementations like testing.
double GunnsInfraFunctions::mClockTime = 0.0;

/// @details Defines the static variable, useful for non-Trick implementations like testing.
bool GunnsInfraFunctions::mMcIsSlave = false;

/// @details Defines the static variable, useful for non-Trick implementations like testing.
int GunnsInfraFunctions::mMcSlaveId = 0;

/// @details Defines the static variable, useful for non-Trick implementations like testing.
std::vector<double> GunnsInfraFunctions::mMcRead = GunnsInfraFunctions::makeEmptyVector();

/// @details Defines the static variable, useful for non-Trick implementations like testing.
std::vector<double> GunnsInfraFunctions::mMcWrite = GunnsInfraFunctions::makeEmptyVector();

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double  (s)  A clock time value.
///
/// @details  In the Trick environment, returns the Trick's clock_time() value converted to units of
///           seconds.  In non-Trick environments, returns the value of mClockTime.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsInfraFunctions::clockTime()
{
#ifndef no_TRICK_ENV
    return clock_time() / 1000000.0;
#else  // non-Trick
    return mClockTime;
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool  (--)  A monte carlo slave flag.
///
/// @details  In the Trick environment, returns the Trick's mc_is_slave() value.  In non-Trick
///           environments, returns the value of mMcIsSlave.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsInfraFunctions::mcIsSlave()
{
#ifndef no_TRICK_ENV
    return mc_is_slave();
#else  // non-Trick
    return mMcIsSlave;
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  int  (--)  A monte carlo slave ID value.
///
/// @details  In the Trick environment, returns the Trick's mc_get_slave_id() value.  In non-Trick
///           environments, returns the value of mMcSlaveId.
////////////////////////////////////////////////////////////////////////////////////////////////////
int GunnsInfraFunctions::mcSlaveId()
{
#ifndef no_TRICK_ENV
    return mc_get_slave_id();
#else  // non-Trick
    return mMcSlaveId;
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out]  var  (--)  A double value read from the monte carlo data buffer.
///
/// @details  In the Trick environment, returns the next double from the MC Slave-to-Master data
///           buffer.  In non-Trick environments, pops the back off of the mMcRead vector and
///           returns its value.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsInfraFunctions::mcRead(double& var)
{
#ifndef no_TRICK_ENV
    mc_read((char*) &var, sizeof(double));
#else  // non-Trick
    var = mMcRead.back();
    mMcRead.pop_back();
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  var  (--)  A double value written to the monte carlo data buffer.
///
/// @details  In the Trick environment, writes the given value to the MC Slave-to-Master data
///           buffer.  In non-Trick environments, pushes the value to the back of the mMcWrite
///           vector.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsInfraFunctions::mcWrite(double& var)
{
#ifndef no_TRICK_ENV
    mc_write((char*) &var, sizeof(double));
#else  // non-Trick
    mMcWrite.push_back(var);
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  std::vector<double> (--) An empty vector of doubles.
///
/// @details  Constructs and returns an empty vector of doubles, used elsewhere for static
///           definition.
////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<double> GunnsInfraFunctions::makeEmptyVector() {
    std::vector<double> v;
    v.clear();
    return v;
}
