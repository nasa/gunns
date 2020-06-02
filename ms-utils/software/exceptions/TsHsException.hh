#ifndef TsHsException_EXISTS
#define TsHsException_EXISTS

/**
@file
@brief     Macro definitions

@defgroup  TSM_UTILITIES_SOFTWARE_EXCEPTIONS_HSEXCEPTION Macro Definitions
@ingroup   TSM_UTILITIES_SOFTWARE_EXCEPTIONS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Provides the macro to both issue an H&S message and throw an exception.)

@details
ASSUMPTIONS AND LIMITATIONS:
- ((Outside  Trick environment, no H&S message is sent, although the cause message is constructed.)
   (When used in the Trick environment, the S_define must \#define P_LOG P1 to set the initialization
   phase logging priority and \#include the appropriate H&S .sm file.))

LIBRARY DEPENDENCY:
- ((simulation/hs/TsHsMngr.o))

PROGRAMMERS:
 (
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2011-09))
- ((Ryan Prentice) (L-3) (Sep 2012) (Added the TS_HS_RETHROW macro))
 )

@{
*/

#include <sstream>

#include "simulation/hs/TsHsMngr.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   severity   TS_HS_MSG_TYPE  The severity level of this message.
/// @param[in]   subsystem  std::string     The subsystem from which this message is sent.
/// @param[in]   cause      std::string     The root cause for this message.
/// @param[in]   exception  Ts*Exception    The exception type (derived from TsException) to throw.
/// @param[in]   subtype    std::string     The subtype description of the exception.
/// @param[in]   thrower    std::string     The entity throwing this exception.
///
/// @details  Sends the message consisting of file, line, function, severity, subsystem and a
///           message string consisting of exception, subtype, thrower and cause to the global H&S
///           manager. It then throws the exception specifying the subtype, thrower and cause.
///           A char* or const char* is also acceptable as an std:string argument
///
/// @par      Example 1 (static cause):
/// @code
///           #include "software/exceptions/TsHsException.hh"
///           #include "software/exceptions/TsInitializationException.hh"
///           ...
///           TS_HS_EXCEPTION(TS_HS_ERROR, "GUNNS", "All nodes uninitialized or vacuum.",
///                           TsInitializationException, "Invalid Initialization Data", mName);
/// @endcode
///
/// @par      Example 2 (composed cause):
/// @code
///           #include "software/exceptions/TsHsException.hh"
///           #include "software/exceptions/TsOutOfBoundsException.hh"
///           ...
///           std::ostringstream msg;
///           msg << "x argument " << x << " outside valid range " << mMinX << " to " << mMaxX;
///           TS_HS_EXCEPTION(TS_HS_ERROR, "MATH", msg.str(), TsOutOfBoundsException,
///                           "Invalid Input Argument", "TsApproximation";
/// @endcode
/// @hideinitializer
////////////////////////////////////////////////////////////////////////////////////////////////////
#define TS_HS_EXCEPTION(severity, subsystem, cause, exception, subtype, thrower) \
{ \
    std::ostringstream causeMessage; \
    causeMessage << #exception << " thrown on " << subtype << " by " << thrower << " - " << cause; \
    if (tsGlobalHsMngr) { \
        tsGlobalHsMngr->msg(__FILE__, __LINE__, __func__, \
                            severity, subsystem, causeMessage.str()); \
    } \
    throw exception(subtype, thrower, cause); \
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   subsystem  std::string    The subsystem from which this message is sent.
/// @param[in]   exception  Ts*Exception   The exception type (derived from TsException) to rethrow.
///
/// @details  Rethrows a Ts*Exception that was caught by a try/catch statement. This produces the
///           same general output as the TS_HS_EXCEPTION macro, and can be used to generate a stack
///           trace in the H&S log. The exception parameter must be a fully initialized exception.
///
/// @par      Example:
///
/// Thrower:
/// @code
///           #include "software/exceptions/TsHsException.hh"
///           #include "software/exceptions/TsInitializationException.hh"
///
///           void ExampleThrower::initialize()
///           ...
///           TS_HS_EXCEPTION(TS_HS_ERROR, "GUNNS", "All nodes uninitialized or vacuum.",
///                           TsInitializationException, "Invalid Initialization Data", mName);
/// @endcode
///
/// Catcher:
/// @code
///           #include "software/exceptions/TsHsException.hh"
///           #include "software/exceptions/TsInitializationException.hh"
///
///           void ExampleCatcher::initialize()
///           {
///               try {
///                   ExampleThrower::initialize();
///               }
///               catch(TsInitializationException& e) {
///                   TS_HS_RETHROW(TS_HS_ERROR, "GUNNS", e);
///               }
///           }
/// @endcode
/// @hideinitializer
////////////////////////////////////////////////////////////////////////////////////////////////////
#define TS_HS_RETHROW(subsystem, exception) \
{ \
    std::ostringstream causeMessage; \
    causeMessage << "    " << "|--> " << exception.getName() << " re-thrown from " << \
                    __PRETTY_FUNCTION__; \
    if (tsGlobalHsMngr) { \
        tsGlobalHsMngr->msg(__FILE__, __LINE__, __func__, \
                            TS_HS_DEBUG, subsystem, causeMessage.str()); \
    } \
    throw; \
}

#define TS_HS_CATCH(subsystem, exception) \
{ \
    std::ostringstream causeMessage; \
    causeMessage << "    " << "|--> " << exception.getName() << " caught by " << \
                    __PRETTY_FUNCTION__; \
    if (tsGlobalHsMngr) { \
        tsGlobalHsMngr->msg(__FILE__, __LINE__, __func__, \
                            TS_HS_DEBUG, subsystem, causeMessage.str()); \
    } \
}

/// @}

#endif
