#ifndef TS_hs_msg_types_EXIST
#define TS_hs_msg_types_EXIST

/**
@defgroup  TSM_UTILITIES_SIMULATION_HS_TYPES Definitions and Enumerations
@ingroup   TSM_UTILITIES_SIMULATION_HS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
- (Health and status messaging functions - message types)

PROGRAMMERS:
- ((Jeffrey Middleton) (L3) (August 2010))

@{
*/

/// Enumerated health and status message types (criticality)
typedef enum {

    TS_HS_DEBUG    = 0,
    TS_HS_INFO     = 1,
    TS_HS_WARNING  = 2,
    TS_HS_ERROR    = 3,
    TS_HS_FATAL    = 4,

} TS_HS_MSG_TYPE;

/// Subsystem names to be used in calls to the message logging methods. Using consistent names
/// is good in general and some logging capabilities depend on it (e.g. the capability of H&S to
/// terminate the simulation on fatal errors depends on consistent subsystem names).

// The names are encoded using "define" macros. This was optimal given the constraint that the
// addition of the names not break existing code -- this includes legacy "C" code (e.g. CTNS).

#define TS_HS_ATCS     "ATCS"
#define TS_HS_CDH      "CDH"
#define TS_HS_CHS      "CHS"
#define TS_HS_CS       "CS"
#define TS_HS_CT       "CT"
#define TS_HS_CTNS     "CTNS"
#define TS_HS_ECLSS    "ECLSS"
#define TS_HS_ENV      "ENV"
#define TS_HS_EPS      "EPS"
#define TS_HS_EVAS     "EVAS"
#define TS_HS_GEMS     "GEMS"
#define TS_HS_GNC      "GNC"
#define TS_HS_GUNNS    "GUNNS"
#define TS_HS_HS       "HS"
#define TS_HS_IPP      "IPP"
#define TS_HS_JEOD     "JEOD"
#define TS_HS_MECH     "MECH"
#define TS_HS_PROP     "PROP"
#define TS_HS_PTCS     "PTCS"
#define TS_HS_SENSORS  "SENSORS"
#define TS_HS_SIGNAL   "SIGNAL"
#define TS_HS_TCS      "TCS"
#define TS_HS_OBCS     "OBCS"
#define TS_HS_GENERIC  "GENERIC"
#define TS_HS_UNKNOWN  "UNKNOWN"
#define TS_HS_VV       "VV"
#define TS_HS_HCP      "HCP"

/// @}

#endif /* TS_hs_msg_types_EXIST */
