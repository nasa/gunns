#ifndef GUNNSMacros_EXISTS
#define GUNNSMacros_EXISTS

/**
 @file
 @brief     Macro Definitions

@defgroup  TSM_GUNNS_CORE_MACROS Macro Definitions
@ingroup   TSM_GUNNS_CORE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE: (Defines macros for H&S messages, exception throws and object name initialization by
          GUNNS classes that have an mName attribute for consistent reporting from GUNNS objects.)

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
- ((Kenneth McMurtrie) (Tietronix Software) (add stack trace) (2013-06))

@{
*/

#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Macro for H&S fatal message.
///
/// @param[in]   text      std::string     The text details of the message.
///
/// @details  This macro creates a TsHsMsg object to log a health & status (H&S) fatal message and returns.
///           The message will automatically indicate class, line of code, method, thrower
///           (identified by the instance's mName attribute) and the detailed text supplied in the
///           call.  This macros supplies severity = TS_HS_FATAL and subsystem = "GUNNS", for
///           consistent reporting from all Gunns objects.  The text argument can use the
///           << operator.
///
/// @par      Example 1 (static text):
/// @code
///           GUNNS_FATAL("User error, replace user.");
/// @endcode
///
/// @par      Example 2 (composed text):
/// @code
///           GUNNS_FATAL("You've been warned " << i << " times that this file does not exist.");
/// @endcode
/// @hideinitializer
///////////////////////////////////////////////////////////////////////////////////////////////////
#define GUNNS_FATAL(text) \
{ \
    TsHsMsg gunnsMsg(TS_HS_FATAL, TS_HS_GUNNS); \
    gunnsMsg << mName << " " << text; \
    hsSendMsg(gunnsMsg); \
    return; \
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Macro for H&S error message with stack trace and exception throw.
///
/// @param[in]   exception  Ts*Exception    The exception type (derived from TsException) to throw.
/// @param[in]   subtype    std::string     The subtype description of the exception.
/// @param[in]   cause      std::string     The text details (cause) for the message and exception.
///
/// @details  This macro creates a TsHsMsg object to issue a health & status (H&S) warning message
///           and throws an exception.\n
///           The H&S message will automatically indicate the file, line of code, method, thrower
///           (identified by the instance's mName attribute) and cause supplied in the call along
///           with a stack trace.\n
///           The exception will use the specified exception type, exception subtype and cause
///           supplied in the call.\n
///           This macro supplies severity = TS_HS_ERROR, subsystem = TS_HS_GUNNS, and
///           thrower = mName.\n
///           A  char* or const char* is also acceptable for any of the std:string arguments. \n
///           If the mName attribute is null, then it is not included in the message. \n
///           If this macro is used in a constructor, there will be a memory leak due to at least
///           the name not being deallocated.
///
/// @par      Example 1 (static cause):
/// @code
///           LIBRARY DEPENDENCY:
///               ((simulation/hs/TsHsMsg.o)
///                (software/exceptions/TsInitializtionException.o))
///           ...
///           #include "core/GunnsMacros.hh"
///           #include "software/exceptions/TsInitializationException.hh"
///           ...
///           GUNNS_ERREX(TsInitializationException, "Invalid Initialization Data", "Your dog is ugly and he smells bad too.");
/// @endcode
///
/// @par      Example 2 (composed cause):
/// @code
///           LIBRARY DEPENDENCY:
///               ((simulation/hs/TsHsMsg.o)
///                (software/exceptions/TsOutOfBoundsException.o))
///           ...
///           #include "core/GunnsMacros.hh"
///           #include "software/exceptions/TsOutOfBoundsException.hh"
///           ...
///           std::ostringstream cause;
///           cause << i << " toes on one foot is too many.";
///           GUNNS_ERREX(TsOutOfBoundsException, "Invalid Input Argument", cause.str());
/// @endcode
/// @hideinitializer
///////////////////////////////////////////////////////////////////////////////////////////////////
#define GUNNS_ERREX(exception, subtype, cause) \
{ \
    TsHsMsg msg(TS_HS_ERROR, TS_HS_GUNNS); \
    if (0 == mName) { \
        msg << "throwing " << #exception << " " << subtype << " - " << cause  << '\n' << tsStackTrace(); \
        hsSendMsg(msg); \
        throw exception(subtype, "no name", cause); \
    } else { \
        msg << mName << " throwing " << #exception << " " << subtype << " - " << cause  << '\n' << tsStackTrace(); \
        hsSendMsg(msg); \
        throw exception(subtype, mName, cause); \
    } \
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Macro for H&S error message without stack trace and exception throw.
///
/// @param[in]   exception  Ts*Exception    The exception type (derived from TsException) to throw.
/// @param[in]   subtype    std::string     The subtype description of the exception.
/// @param[in]   cause      std::string     The text details (cause) for the message and exception.
///
/// @details  This macro creates a TsHsMsg object to issue a health & status (H&S) warning message
///           and throws an exception.\n
///           The H&S message will automatically indicate the file, line of code, method, thrower
///           (identified by the instance's mName attribute) and cause supplied in the call\n
///           The exception will use the specified exception type, exception subtype and cause
///           supplied in the call.\n
///           This macro supplies severity = TS_HS_ERROR, subsystem = TS_HS_GUNNS, and
///           thrower = mName.\n
///           A  char* or const char* is also acceptable for any of the std:string arguments. \n
///           If the mName attribute is null, then it is not included in the message. \n
///           If this macro is used in a constructor, there will be a memory leak due to at least
///           the name not being deallocated.
///
/// @par      Example 1 (static cause):
/// @code
///           LIBRARY DEPENDENCY:
///               ((simulation/hs/TsHsMsg.o)
///                (software/exceptions/TsInitializtionException.o))
///           ...
///           #include "core/GunnsMacros.hh"
///           #include "software/exceptions/TsInitializationException.hh"
///           ...
///           GUNNS_ERREX(TsInitializationException, "Invalid Initialization Data", "Your dog is ugly and he smells bad too.");
/// @endcode
///
/// @par      Example 2 (composed cause):
/// @code
///           LIBRARY DEPENDENCY:
///               ((simulation/hs/TsHsMsg.o)
///                (software/exceptions/TsOutOfBoundsException.o))
///           ...
///           #include "core/GunnsMacros.hh"
///           #include "software/exceptions/TsOutOfBoundsException.hh"
///           ...
///           std::ostringstream cause;
///           cause << i << " toes on one foot is too many.";
///           GUNNS_ERREX(TsOutOfBoundsException, "Invalid Input Argument", cause.str());
/// @endcode
/// @hideinitializer
///////////////////////////////////////////////////////////////////////////////////////////////////
#define GUNNS_ERREX_NO_STACK_TRACE(exception, subtype, cause) \
{ \
    TsHsMsg msg(TS_HS_ERROR, TS_HS_GUNNS); \
    if (0 == mName) { \
        msg << "throwing " << #exception << " " << subtype << " - " << cause; \
        hsSendMsg(msg); \
        throw exception(subtype, "no name", cause); \
    } else { \
        msg << mName << " throwing " << #exception << " " << subtype << " - " << cause; \
        hsSendMsg(msg); \
        throw exception(subtype, mName, cause); \
    } \
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Macro for conditional H&S error message and exception throw.
///
/// @param[in]   condition  bool            Condition to check, true will throw exception.
/// @param[in]   exception  Ts*Exception    The exception type (derived from TsException) to throw.
/// @param[in]   subtype    std::string     The subtype description of the exception.
/// @param[in]   cause      std::string     The text details (cause) for the message and exception.
///
/// @details  This macro invokes the GUNNS_ERREX macro to throw an exception and issue a health
///           & status (H&S) error message if the argument condition is true.\n
///
/// @par      Example 1 (static cause):
/// @code
///           LIBRARY DEPENDENCY:
///               ((simulation/hs/TsHsMsg.o)
///                (software/exceptions/TsInitializtionException.o))
///           ...
///           #include "core/GunnsMacros.hh"
///           #include "software/exceptions/TsInitializationException.hh"
///           ...
///           bool isConditionMet = true;
///           GUNNS_IF_ERREX(isConditionMet, TsInitializationException, "Invalid Initialization Data", "Your dog is ugly and he smells bad too.");
/// @endcode
///
/// @par      Example 2 (composed cause):
/// @code
///           LIBRARY DEPENDENCY:
///               ((simulation/hs/TsHsMsg.o)
///                (software/exceptions/TsOutOfBoundsException.o))
///           ...
///           #include "core/GunnsMacros.hh"
///           #include "software/exceptions/TsOutOfBoundsException.hh"
///           ...
///           double toes = 10;
///           std::ostringstream cause;
///           cause << i << " toes on one foot is too many.";
///           GUNNS_IF_ERREX(5 < toes, TsOutOfBoundsException, "Invalid Input Argument", cause.str());
/// @endcode
/// @hideinitializer
///////////////////////////////////////////////////////////////////////////////////////////////////
#define GUNNS_IF_ERREX(condition, exception, subtype, cause) \
{ \
    if (condition) { \
        GUNNS_ERREX(exception, subtype, cause); \
    } \
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   exception  Ts*Exception    The exception type (derived from TsException) to throw.
/// @param[in]   subtype    std::string     The subtype description of the exception.
/// @param[in]   cause      std::string     The root cause for this message.
///
/// @details  This macro invokes the TS_HS_EXCEPTION macro to throw an exception and issue a H&S
///           error message consisting of the given exception type, subtype, and cause.  This macro
///           supplies severity = TS_HS_ERROR, subsystem = "GUNNS", and thrower = the object name to
///           TS_HS_EXCEPTION, for consistent error reporting from all Gunns objects.  A char* or
///           const char* is also acceptable as an std:string argument
///
/// @par      Example 1 (static cause):
/// @code
///           #include "software/exceptions/TsInitializationException.hh"
///           ...
///           GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
///                       "All nodes uninitialized or vacuum.");
/// @endcode
///
/// @par      Example 2 (composed cause):
/// @code
///           #include "software/exceptions/TsOutOfBoundsException.hh"
///           ...
///           std::ostringstream msg;
///           msg << "x argument " << x << " outside valid range " << mMinX << " to " << mMaxX;
///           GUNNS_ERROR(TsOutOfBoundsException, "Invalid Input Argument", msg.str());
/// @endcode
/// @hideinitializer
///////////////////////////////////////////////////////////////////////////////////////////////////
#define GUNNS_ERROR(exception, subtype, cause) \
{ \
    TS_HS_EXCEPTION(TS_HS_ERROR, TS_HS_GUNNS, cause, exception, subtype, mName); \
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   text      std::string     The text details of the message.
///
/// @details  This macro creates a TsHsMsg object to log a health & status (H&S) warning message.
///           The message will automatically indicate class, line of code, method, thrower
///           (identified by the instance's mName attribute) and the detailed text supplied in the
///           call.  This macros supplies severity = TS_HS_WARNING and subsystem = "GUNNS", for
///           consistent warning reporting from all Gunns objects.  The text argument can use the
///           << operator.
///
/// @par      Example 1 (static text):
/// @code
///           GUNNS_WARNING("All nodes uninitialized or vacuum.");
/// @endcode
///
/// @par      Example 2 (composed text):
/// @code
///           GUNNS_WARNING("Port setting of " << i << " is illegal.");
/// @endcode
/// @hideinitializer
///////////////////////////////////////////////////////////////////////////////////////////////////
#define GUNNS_WARNING(text) \
{ \
    TsHsMsg gunnsMsg(TS_HS_WARNING, TS_HS_GUNNS); \
    gunnsMsg << mName << " " << text; \
    hsSendMsg(gunnsMsg); \
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Macro for H&S info message.
///
/// @param[in]   text      std::string     The text details of the message.
///
/// @details  This macro creates a TsHsMsg object to log a health & status (H&S) info message.
///           The message will automatically indicate class, line of code, method, thrower
///           (identified by the instance's mName attribute) and the detailed text supplied in the
///           call.  This macros supplies severity = TS_HS_INFO and subsystem = "GUNNS", for
///           consistent info reporting from all Gunns objects.  The text argument can use the
///           << operator.
///
/// @par      Example 1 (static text):
/// @code
///           GUNNS_INFO("By your command.");
/// @endcode
///
/// @par      Example 2 (composed text):
/// @code
///           GUNNS_INFO("For the " << i << " colonies, toaster!");
/// @endcode
/// @hideinitializer
///////////////////////////////////////////////////////////////////////////////////////////////////
#define GUNNS_INFO(text) \
{ \
    TsHsMsg gunnsMsg(TS_HS_INFO, TS_HS_GUNNS); \
    gunnsMsg << mName << " " << text; \
    hsSendMsg(gunnsMsg); \
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Macro to copy name.
///
/// @param[in]   name       std::string     The given instance name.
///
/// @details  This macro initializes a class std::string attribute mName from the given std::string
///           name argument.
///
/// @par      Example:
/// @code
///           GUNNS_NAME("MyObjectName");
/// @endcode
/// @hideinitializer
////////////////////////////////////////////////////////////////////////////////////////////////////
#define GUNNS_NAME(name) \
{ \
    mName = name; \
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Macro to copy name with H&S error message and exception throw on empty name.
///
/// @param[in]   caller     std::string     The name of the calling class i.e. GunnsBasicNode.
/// @param[in]   name       std::string     The given instance name.
///
/// @details  This macro first checks the std::string name argument. If it is empty, an H&S error
///           message is issued and a TsInitializationException is thrown.  Otherwise it initializes
///           a class std::string attribute mName from the given std::string name argument.
///
/// @par      Example:
/// @code
///           LIBRARY DEPENDENCY:
///               ((simulation/hs/TsHsMsg.o)
///                (software/exceptions/TsInitializtionException.o))
///           ...
///           #include "core/GunnsMacros.hh"
///           #include "software/exceptions/TsInitializationException.hh"
///           ...
///           GUNNS_NAME_ERREX("MyClass", "MyObjectName");
/// @endcode
/// @hideinitializer
///////////////////////////////////////////////////////////////////////////////////////////////////
#define GUNNS_NAME_ERREX(caller, name) \
{ \
    if (name.empty()) { \
        TsHsMsg msg(TS_HS_ERROR, TS_HS_GUNNS); \
        msg << "throwing " << "TsInitializationException" << " " << "Invalid Initialization Data" << " - " << "Empty object name."  << '\n' << tsStackTrace(); \
        hsSendMsg(msg); \
        throw TsInitializationException("Invalid Initialization Data", caller, "Empty object name."); \
    } \
    GUNNS_NAME(name); \
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Macro to copy name with H&S fatal message on empty name.
///
/// @param[in]   caller     std::string     The calling class.
///
/// @details  This macro first checks an std::string name. If it is empty, it issues an H&S fatal
///           message and returns. Otherwise it initializes an GUNNS char* object name attribute
///           mName from name by invoking first the TS_DELETE_ARRAY macro and then the TS_STRDUP
///           macro.
///
/// @note     The calling class destructor should TS_DELETE_ARRAY(mName).
///
/// @par      Example:
/// @code
///           LIBRARY DEPENDENCY:
///               ((simulation/hs/TsHsMsg.o))
///           ...
///           #include "core/GunnsMacros.hh"
///           ...
///           GUNNS_NAME_FATAL("MyClass");
/// @endcode
/// @hideinitializer
///////////////////////////////////////////////////////////////////////////////////////////////////
#define GUNNS_NAME_FATAL(caller) \
{ \
    if (name.empty()) { \
        TsHsMsg msg(TS_HS_FATAL, TS_HS_GUNNS); \
        msg << caller << " " << "Empty object name."; \
        hsSendMsg(msg); \
        return; \
    } \
    GUNNS_NAME; \
}

/// @}

#endif

