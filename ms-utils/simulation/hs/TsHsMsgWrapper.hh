#ifndef TsHsMsgWrapper_EXIST
#define TsHsMsgWrapper_EXIST

/**
@defgroup  TSM_UTILITIES_SIMULATION_HS_WRAPPER C Wrapper
@ingroup   TSM_UTILITIES_SIMULATION_HS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (This is a "C" wrapper class used to log health and status messages. "C" code that needs
   to log message should include this header file, TsHsMsgWrapper.hh", instead of "TsHsMsg.hh".
   "C++" code should continue to use "TsHsMsg.hh".

REQUIREMENTS:
- (TBD)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- (
     (simulation/hs/TsHsMsgWrapper.o)
  )

PROGRAMMERS:
- (
    ((Wesley A. White) (Tietronix Software) (August 2011))
  )

@{
*/

#include <stdarg.h>
#include "TS_hs_msg_types.h"

/// @brief Provides access to TsHsMsg's hsSendMsg macro functionality from "C" code.
#define hsSendMsgC(args...) tsHsSendMsgFileLineC(__FILE__, __LINE__, __func__, ##args)

/// @brief Macro to be used only when logging messages from "C" which must use field specifiers (e.g. "%s %d").
#define TS_hs_msg(args...) tsHsSendMsgFileLineFormattedC(__FILE__, __LINE__, __func__, ##args)

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Used by the hsSendMsg macro when the macro is invoked with severity, subsystem, and message text arguments.
void tsHsSendMsgFileLineC(
   const char    *file,
   unsigned       line,
   const char    *function,
   TS_HS_MSG_TYPE severity,
   const char    *subsys,
   const char    *mtext);


/// @brief Function to be used only when logging messages from "C" which must use field specifiers (e.g. "%s %d").
void tsHsSendMsgFileLineFormattedC(
   const char     *file,
   int             line,
   const char     *function,
   TS_HS_MSG_TYPE  severity,
   const char     *subsys,
   const char     *hsmsg,
   ...)

   // direct gcc to check the arguments in the same way as for printf
   __attribute__ ((format (printf, 6, 7)));

#ifdef __cplusplus
}
#endif

/// @}

#endif /* TsHsMsgWrapper_EXIST */
