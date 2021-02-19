#ifndef TsHsUtMacro_EXISTS
#define TsHsUtMacro_EXISTS
/************************************** TRICK HEADER **********************************************
@defgroup  TSM_UTILITIES_SIMULATION_HS_UTMACRO UT macro for Health And Status Logging
@ingroup   TSM_UTILITIES_SIMULATION_HS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (ts-model unit-test macro definition.)

REQUIREMENTS:
- ()

REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- ()

LIBRARY DEPENDENCY:
- ()

PROGRAMMERS:
    (
    (Joe Valerioti (L3) (Dec 2012) (initial)
    )

***************************************************************************************************/
#include "TsHsMsg.hh"
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   text       std::string     The text details of the message.
///
/// @details  This macro creates a TsHsMsg object to log a health & status (H&S) message.
///           The message will automatically indicate class, line of code, and method.
///           The message will also include the detailed text supplied in the call. This macro
///           supplies subsystem = "TEST" and severity = TS_HS_INFO, for consistent reporting
///           from all UT objects. The text argument can use the << operator.
///
/// @par      Example 1 (static text):
/// @code
///           TEST_HS("Beginning update() test.");
/// @endcode
///
/// @par      Example 2 (composed text):
/// @code
///           TEST_HS("Attempt to initialize " << linkType << " link.");
/// @endcode
/// @hideinitializer
////////////////////////////////////////////////////////////////////////////////////////////////////
#define TEST_HS(text) \
{ \
    TsHsMsg msg(TS_HS_INFO, "TEST"); \
    msg << text; \
    hsSendMsg(msg); \
}

#endif
