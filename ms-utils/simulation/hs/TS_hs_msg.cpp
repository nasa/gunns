/****************************************** TRICK HEADER ******************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
    (Health and status C-Style messaging functions.)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
   ((TsHsMngr.o)
    (simulation/timer/TS_timer.o)
   )

PROGRAMMERS:
   (
      ((Jeffrey Middleton) (L3) (December 2009) (complete rewrite))
      ((Paul Carradine) (L3) (August 2009) (Initial version))
   )
**************************************************************************************************/

#include "simulation/timer/TS_timer.h"

#include <cstdio>
#include <iostream>

#include "TS_hs_msg.h"
#include "TsHsMngr.hh"

// Note: though this file implements C-style functions with C linkage (and
// therefore is named with the C naming convention), it is in fact C++

void TS_hs_fileline_msg(
   const char*     file,    // IN     -- filename
   const int       line,    // IN     -- line number
   TS_HS_MSG_TYPE  type,    // IN     -- type of message (criticality)
   const char*     subsys,  // IN     -- subsystem
   const char*     mtext,   // IN     -- message (format string)
   ...)                     // IN     -- arguments to format string
{
    va_list mtext_args;
    va_start(mtext_args, mtext);

    // The only way this will fail is if the H&S initialization job is never
    // run. That means the sim's pretty much broken, so no need for
    // significant error handling
    if (tsGlobalHsMngr)
    {
        char expanded_msg[1024];
        vsnprintf(expanded_msg, 1024, mtext, mtext_args);
        tsGlobalHsMngr->msg(file, line, "", type, subsys, expanded_msg);
    }

    va_end(mtext_args);
}

