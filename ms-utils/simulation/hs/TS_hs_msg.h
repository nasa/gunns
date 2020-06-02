#ifndef TS_HS_H
#define TS_HS_H

/**
@defgroup  TSM_UTILITIES_SIMULATION_HS_MESSAGE_C C Style Logger
@ingroup   TSM_UTILITIES_SIMULATION_HS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
- (Health and status C-Style messaging functions.)

PROGRAMMERS:
- (
     (Jeff Wilson) (L3) (July 2011)
     (Jeffrey Middleton) (L3) (December 2009)
  )
@{
*/

#include <stdarg.h>

// Include these in order to provide the template message functions to other
// code including this header file.
#include "TS_hs_msg_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Macro which supplies file and line info to TS_hs_fileline_msg function.
 */
#define TS_hs_msg(args...) TS_hs_fileline_msg(__FILE__, __LINE__, ##args)

/**
 * The main "C-style" logging function used to log health and status messages.
 *
 * @param file - const char representing the filename the error occurred in
 * @param line - int representing the line number where the error occurred
 * @param type - TS_HS_MSG_TYPE the criticality of the message
 * @param subsys - const char representing the subsystem where the error occurred.
 * @param hsmsg - const char representing the message string
 *
 * @deprecated - This "C-style" method is being replaced by the C++ class TsHsMsg. Please change all references to TsHsMsg.
 */
#ifndef SWIG
void TS_hs_fileline_msg(
   const char *file,
   int line,
   TS_HS_MSG_TYPE  type,
   const char       *subsys,
   const char       *hsmsg,
   ...) __attribute__ ((deprecated))

   // direct gcc to check the arguments in the same way as for printf
   __attribute__ ((format (printf, 5, 6)));


/**
 * This method disables message logging
 */
void TS_hs_disable(void);
#endif /* SWIG */
#ifdef __cplusplus
}
#endif

/// @}

#endif /* TS_HS_H */
