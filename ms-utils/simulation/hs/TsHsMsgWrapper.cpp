/**
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
- (This is a C wrapper class used to log model health and status messages.)


REQUIREMENTS:
  ()

REFERENCE:
  ()

ASSUMPTIONS AND LIMITATIONS:
  ()

LIBRARY DEPENDENCY:
   (
    (simulation/hs/TsHsMsgWrapper.o)
    (simulation/hs/TsHsMngr.o)
    (simulation/timer/TS_timer.o)
   )

PROGRAMMERS:
  (
    ((Wesley A. White) (Tietronix Software) (August 2011))
  )
*/

#include <cstdio>
#include <iostream>

#include "TsHsMsgWrapper.hh"
#include "TsHsMngr.hh"
#include "simulation/timer/TS_timer.h"

// todo: I think the include of TS_timer is there for some reason, but I don't recall
//       what it was. Something to do with forcing trick to compile the file in certain cases. Look
//       into this and remove references if they are not required.

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Used by the "C" wrapper version of the hsSendMsg macro when the macro is invoked
///          with severity, subsystem, and message text arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////
void tsHsSendMsgFileLineC(
   const char    *file,
   unsigned       line,
   const char    *function,
   TS_HS_MSG_TYPE severity,
   const char    *subsys,
   const char    *mtext)
{
    if (tsGlobalHsMngr == 0)
        return;

    tsGlobalHsMngr->msg(file, line, function, severity, subsys, mtext);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Used by the "C" wrapper version to log message that contain field specifiers. The
///          hsSendMsg macro should be used log messages which do not contain field specifiers.
///
/// @param[in] file     (--) Source file name invoking 'send'; typically __FILE__ macro is used.
/// @param[in] line     (--) Line in file invoking 'send'; typically __LINE__ macro is used.
/// @param[in] function (--) The name of the function logging the message; typically __func__ (unused).
/// @param[in] severity (--) Enumeration constant that represents the type of message.
/// @param[in] subsys   (--) The subsystem that is logging the message.
/// @param[in] mtext    (--) The message text.
///
/// @note    It is undesirable to use field specifiers since they can lead to run-time errors.
///          In C++ they can be replaced with "<<" and I/O manipulators; however, in "C" there
///          are no practical alternatives, so we continue to provide the functionality here.
///          This function is only intended to be used by the TS_hs_msg macro and that macro
///          is only intended to be used by "C" code which needs to log messages that contain
///          field specifiers. Messages which do not use field specifiers should use the
///          hsSendMsg macro.
////////////////////////////////////////////////////////////////////////////////////////////////////
void tsHsSendMsgFileLineFormattedC(
   const char*     file,
   const int       line,
   const char*     function __attribute__((unused)),
   TS_HS_MSG_TYPE  severity,
   const char*     subsys,
   const char*     mtext,
   ...)
{
    const int MESSAGE_BUFFER_MAX_SIZE = 1024;

    va_list mtext_args;
    va_start(mtext_args, mtext);

    if (tsGlobalHsMngr)
    {
        char expanded_msg[MESSAGE_BUFFER_MAX_SIZE];
        vsnprintf(expanded_msg, MESSAGE_BUFFER_MAX_SIZE, mtext, mtext_args);
        tsGlobalHsMngr->msg(file, line, "", severity, subsys, expanded_msg);
    }

    va_end(mtext_args);
}

