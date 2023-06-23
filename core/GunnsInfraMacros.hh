#ifndef GunnsInfraMacros_EXISTS
#define GunnsInfraMacros_EXISTS

/**
 @file
 @brief     Infrastructure Integration Macro Definitions

@defgroup  TSM_GUNNS_CORE_MACROS_INFRA Infrastructure Integration Macro Definitions
@ingroup   TSM_GUNNS_CORE

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE: (Defines macros for infrastructure interfaces & dependencies, such as Trick.  These can be
          modified to interface with other simulation environments.)

@details
REQUIREMENTS:
- ()

REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- ()

LIBRARY DEPENDENCY:
- ()

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2018-061) (Initial))

@{
*/

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Default macro variables.
///
/// @details  This contains variables for use by the default, non-Trick version macros below.  These
///           can be used for things like unit testing.
////////////////////////////////////////////////////////////////////////////////////////////////////
namespace GUNNS
{
    static double InfraMacrosClockTime = 0.0;
    static bool   InfraMacrosMcIsSlave = false;
    static int    InfraMacrosMcSlaveId = 0;
    static double InfraMacrosMcRead    = 0.0;
    static double InfraMacrosMcWrite   = 0.0;
}

#ifndef no_TRICK_ENV
#if (TRICK_VER >= 17)
#include "trick/clock_proto.h"
#else
#include "sim_services/Clock/include/clock_proto.h"
#endif
#include "sim_services/MonteCarlo/include/montecarlo_c_intf.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Macro for system clock time.
///
/// @returns  double (s) Time value in seconds.
///
/// @details  This macro replaces the generic clockTime with the specific function call for the sim
///           environment infrastructure.  Currently only Trick is supported, and when not in the
///           Trick environment, this returns the value of GUNNS::InfraMacrosClockTime.
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef no_TRICK_ENV
#define CLOCK_TIME \
clock_time() / 1000000.0
#else  // non-Trick
#define CLOCK_TIME \
GUNNS::InfraMacrosClockTime
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Macro to return whether the sim is a monte carlo slave.
///
/// @returns  bool (--) True if this is a monte carlo slave.
///
/// @details  This macro returns whether the sim is a monte carlo slave.  In the Trick environment,
///           this returns Trick's mc_is_slave() function result.  Otherwise, this return the value
///           of GUNNS::InfraMacrosMcIsSlave.
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef no_TRICK_ENV
#define MC_IS_SLAVE \
mc_is_slave()
#else  // non-Trick
#define MC_IS_SLAVE \
GUNNS::InfraMacrosMcIsSlave
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Macro to return the monte carlo slave ID.
///
/// @returns  int (--) The monte carlo slave ID.
///
/// @details  This macro returns the monte carlo slave ID.  In the Trick environment, this returns
///           Trick's mc_get_slave_id() function result.  Otherwise, this return the value of
///           GUNNS::InfraMacrosMcSlaveId.
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef no_TRICK_ENV
#define MC_GET_SLAVE_ID \
mc_get_slave_id()
#else  // non-Trick
#define MC_GET_SLAVE_ID \
GUNNS::InfraMacrosMcSlaveId
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Macro to read a value from the monte carlo master/slave buffer.
///
/// @param[out]  var (--) The variable to set, must be of type double
///
/// @details  This macro returns the reads a single value of type double from the monte carlo
///           master/slave buffer and stores it into the given var.  In the Trick environment, this
///           uses Trick's mc_read function.  Otherwise, this sets var equal to the value of
///           GUNNS::InfraMacrosMcRead.
///
/// @note  The var argument must be of type double.
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef no_TRICK_ENV
#define MC_READ(var) \
mc_read((char*) &var, sizeof(double))
#else  // non-Trick
#define MC_READ(var) \
var = GUNNS::InfraMacrosMcRead
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Macro to write a value to the monte carlo master/slave buffer.
///
/// @param[in]  var (--) The variable to copy, must be of type double
///
/// @details  This macro writes the the given value to monte carlo master/slave buffer.  In the
///           Trick environment, this uses Trick's mc_write function.  Otherwise, this copies the
///           given value of var into GUNNS::InfraMacrosMcWrite.
///
/// @note  The var argument must be of type double.
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef no_TRICK_ENV
#define MC_WRITE(var) \
mc_write((char*) &var, sizeof(double))
#else  // non-Trick
#define MC_WRITE(var) \
GUNNS::InfraMacrosMcWrite = var
#endif

#endif

