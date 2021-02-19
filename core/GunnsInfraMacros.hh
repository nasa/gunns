#ifndef GunnsInfraMacros_EXISTS
#define GunnsInfraMacros_EXISTS

/**
 @file
 @brief     Infrastructure Integration Macro Definitions

@defgroup  TSM_GUNNS_CORE_MACROS_INFRA Infrastructure Integration Macro Definitions
@ingroup   TSM_GUNNS_CORE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
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
- ((Jason Harvey) (CACI) (2018-061)

@{
*/

#ifndef no_TRICK_ENV
#if (TRICK_VER >= 17)
#include "trick/clock_proto.h"
#else
#include "sim_services/Clock/include/clock_proto.h"
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Macro for system clock time.
///
/// @returns  double (s) Time value in seconds.
///
/// @details  This macro replaces the generic clockTime with the specific function call for the sim
///           environment infrastructure.  Currently only Trick is supported, and when not in the
///           Trick environment, this just returns zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef no_TRICK_ENV
#define CLOCK_TIME \
clock_time() / 1000000.0
#else  // non-Trick
#define CLOCK_TIME \
0.0
#endif

#endif

