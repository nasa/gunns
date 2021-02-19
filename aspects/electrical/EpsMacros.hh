#ifndef EpsMacros_EXISTS
#define EpsMacros_EXISTS

/**
 @file
 @brief     Macro Definitions

@defgroup  TSM_GUNNS_ELECTRICAL_MACROS Macro Definitions
@ingroup   TSM_GUNNS_ELECTRICAL

@defgroup  TSM_GUNNS_CORE_ORCHESTRATOR  GUNNS Orchestrator
@ingroup   TSM_GUNNS_CORE

PURPOSE: (Defines macros for H&S messages, exception throws, object name initialization,
          default data task and RT step method by EPS classes that have an mName
          attribute for consistent reporting from EPS objects.)

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
- ((Kenneth McMurtrie) (Tietronix Software) (add default data task & RT step macros) (2013-07))

@{
*/

#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Macro for H&S fatal message.
///
/// @param[in]   cause      std::string     The text details (cause) for the message.
///
/// @details  This macro creates a TsHsMsg object to issue a health & status (H&S) fatal message and returns.\n
///           The message will automatically indicate the file, line of code, method, thrower
///           (identified by the instance's mName attribute) and the detailed text supplied in the call.\n
///           This macro specifies severity = TS_HS_FATAL and subsystem = TS_HS_EPS.\n
///           A  char* or const char* is also acceptable for the std:string argument. \n
///           If the mName attribute is null, then it is not included in the message.
///
/// @par      Example 1 (std::string or char* or const char*):
/// @code
///           LIBRARY DEPENDENCY:
///               ((simulation/hs/TsHsMsg.o))
///           ...
///           #include "aspects/electrical/EpsMacros.hh"
///           ...
///           TS_EPS_FATAL("User error, replace user.");
/// @endcode
///
/// @par      Example 2 (composed text):
/// @code
///           LIBRARY DEPENDENCY:
///               ((simulation/hs/TsHsMsg.o))
///           ...
///           #include "aspects/electrical/EpsMacros.hh"
///           ...
///           TS_EPS_FATAL("You've been warned " << i << " times that this file does not exist.");
/// @endcode
/// @hideinitializer
///////////////////////////////////////////////////////////////////////////////////////////////////
#define TS_EPS_FATAL(cause) \
{ \
    TsHsMsg msg(TS_HS_FATAL, TS_HS_EPS); \
    if (mName.empty()) { \
        msg << cause; \
    } else { \
        msg << mName << " " << cause; \
    } \
    hsSendMsg(msg); \
    return;\
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Macro for H&S error message with stack trace and exception throw.
///
/// @param[in]   exception  Ts*Exception    The exception type (derived from TsException) to throw.
/// @param[in]   subtype    std::string     The subtype description of the exception.
/// @param[in]   cause      std::string     The text details (cause) for the message and exception.
///
/// @throws   Ts*Exception
///
/// @details  This macro creates a TsHsMsg object to issue a health & status (H&S) warning message
///           and throws an exception.\n
///           The H&S message will automatically indicate the file, line of code, method, thrower
///           (identified by the instance's mName attribute) and cause supplied in the call along
///           with a stack trace.\n
///           The exception will use the specified exception type, exception subtype and cause
///           supplied in the call.\n
///           This macro supplies severity = TS_HS_ERROR, subsystem = TS_HS_EPS, and
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
///           #include "aspects/electrical/EpsMacros.hh"
///           #include "software/exceptions/TsInitializationException.hh"
///           ...
///           TS_EPS_ERREX(TsInitializationException, "Invalid Initialization Data", "Your dog is ugly and he smells bad too.");
/// @endcode
///
/// @par      Example 2 (composed cause):
/// @code
///           LIBRARY DEPENDENCY:
///               ((simulation/hs/TsHsMsg.o)
///                (software/exceptions/TsOutOfBoundsException.o))
///           ...
///           #include "aspects/electrical/EpsMacros.hh"
///           #include "software/exceptions/TsOutOfBoundsException.hh"
///           ...
///           std::ostringstream cause;
///           cause << i << " toes on one foot is too many.";
///           TS_EPS_ERREX(TsOutOfBoundsException, "Invalid Input Argument", cause.str());
/// @endcode
/// @hideinitializer
///////////////////////////////////////////////////////////////////////////////////////////////////
#define TS_EPS_ERREX(exception, subtype, cause) \
{ \
    TsHsMsg msg(TS_HS_ERROR, TS_HS_EPS); \
    if (mName.empty()) { \
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
/// @brief    Macro for H&S error message without stack trace and with exception throw.
///
/// @throws   Ts*Exception
///
/// @param[in]   exception  Ts*Exception    The exception type (derived from TsException) to throw.
/// @param[in]   subtype    std::string     The subtype description of the exception.
/// @param[in]   cause      std::string     The text details (cause) for the message and exception.
///
/// @details  This macro creates a TsHsMsg object to issue a health & status (H&S) warning message
///           and throws an exception.\n
///           The H&S message will automatically indicate the file, line of code, method, thrower
///           (identified by the instance's mName attribute) and cause supplied in the call.\n
///           The exception will use the specified exception type, exception subtype and cause
///           supplied in the call.\n
///           This macro supplies severity = TS_HS_ERROR, subsystem = TS_HS_EPS, and
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
///           #include "aspects/electrical/EpsMacros.hh"
///           #include "software/exceptions/TsInitializationException.hh"
///           ...
///           TS_EPS_ERREX(TsInitializationException, "Invalid Initialization Data", "Your dog is ugly and he smells bad too.");
/// @endcode
///
/// @par      Example 2 (composed cause):
/// @code
///           LIBRARY DEPENDENCY:
///               ((simulation/hs/TsHsMsg.o)
///                (software/exceptions/TsOutOfBoundsException.o))
///           ...
///           #include "aspects/electrical/EpsMacros.hh"
///           #include "software/exceptions/TsOutOfBoundsException.hh"
///           ...
///           std::ostringstream cause;
///           cause << i << " toes on one foot is too many.";
///           TS_EPS_ERREX(TsOutOfBoundsException, "Invalid Input Argument", cause.str());
/// @endcode
/// @hideinitializer
///////////////////////////////////////////////////////////////////////////////////////////////////
#define TS_EPS_ERREX_NO_STACK_TRACE(exception, subtype, cause) \
{ \
    TsHsMsg msg(TS_HS_ERROR, TS_HS_EPS); \
    if (mName.empty()) { \
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
/// @throws   Ts*Exception
///
/// @param[in]   condition  bool            Condition to check, true will throw exception.
/// @param[in]   exception  Ts*Exception    The exception type (derived from TsException) to throw.
/// @param[in]   subtype    std::string     The subtype description of the exception.
/// @param[in]   cause      std::string     The text details (cause) for the message and exception.
///
/// @details  This macro invokes the TS_EPS_ERREX macro to throw an exception and issue a health
///           & status (H&S) error message if the argument condition is true.\n
///
/// @par      Example 1 (static cause):
/// @code
///           LIBRARY DEPENDENCY:
///               ((simulation/hs/TsHsMsg.o)
///                (software/exceptions/TsInitializtionException.o))
///           ...
///           #include "aspects/electrical/EpsMacros.hh"
///           #include "software/exceptions/TsInitializationException.hh"
///           ...
///           bool isConditionMet = true;
///           TS_EPS_IF_ERREX(isConditionMet, TsInitializationException, "Invalid Initialization Data", "Your dog is ugly and he smells bad too.");
/// @endcode
///
/// @par      Example 2 (composed cause):
/// @code
///           LIBRARY DEPENDENCY:
///               ((simulation/hs/TsHsMsg.o)
///                (software/exceptions/TsOutOfBoundsException.o))
///           ...
///           #include "aspects/electrical/EpsMacros.hh"
///           #include "software/exceptions/TsOutOfBoundsException.hh"
///           ...
///           double toes = 10;
///           std::ostringstream cause;
///           cause << i << " toes on one foot is too many.";
///           TS_EPS_IF_ERREX(5 < toes, TsOutOfBoundsException, "Invalid Input Argument", cause.str());
/// @endcode
/// @hideinitializer
///////////////////////////////////////////////////////////////////////////////////////////////////
#define TS_EPS_IF_ERREX(condition, exception, subtype, cause) \
{ \
    if (condition) { \
        TS_EPS_ERREX(exception, subtype, cause); \
    } \
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Macro for H&S error message.
///
/// @param[in]   cause      std::string     The text details (cause) for the message.
///
/// @details  This macro creates a TsHsMsg object to issue a health & status (H&S) error message.\n
///           The message will automatically indicate the file, line of code, method, thrower
///           (identified by the instance's mName attribute) and the detailed text supplied in the call.\n
///           This macro specifies severity = TS_HS_WARN and subsystem = TS_HS_EPS.\n
///           A  char* or const char* is also acceptable for the std:string argument. \n
///           If the mName attribute is null, then it is not included in the message.
///
/// @par      Example 1 (static cause):
/// @code
///           LIBRARY DEPENDENCY:
///               ((simulation/hs/TsHsMsg.o))
///           ...
///           #include "aspects/electrical/EpsMacros.hh"
///           ...
///           TS_EPS_ERROR("An error occurred while displaying the previous error.");
/// @endcode
///
/// @par      Example 2 (composed text):
/// @code
///           LIBRARY DEPENDENCY:
///               ((simulation/hs/TsHsMsg.o))
///           ...
///           #include "aspects/electrical/EpsMacros.hh"
///           ...
///           TS_EPS_ERROR("No errors in " << i << " seconds, that's too long.");
/// @endcode
/// @hideinitializer
///////////////////////////////////////////////////////////////////////////////////////////////////
#define TS_EPS_ERROR(cause) \
{ \
    TsHsMsg msg(TS_HS_ERROR, TS_HS_EPS); \
    if (mName.empty()) { \
        msg << cause; \
    } else { \
        msg << mName << " " << cause; \
    } \
    hsSendMsg(msg); \
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Macro for H&S warning message.
///
/// @param[in]   cause      std::string     The text details (cause) for the message.
///
/// @details  This macro creates a TsHsMsg object to issue a health & status (H&S) warning message.\n
///           The message will automatically indicate the file, line of code, method, thrower
///           (identified by the instance's mName attribute) and the detailed text supplied in the call.\n
///           This macro specifies severity = TS_HS_WARN and subsystem = TS_HS_EPS.\n
///           A  char* or const char* is also acceptable for the std:string argument. \n
///           If the mName attribute is null, then it is not included in the message.
///
/// @par      Example 1 (static cause):
/// @code
///           LIBRARY DEPENDENCY:
///               ((simulation/hs/TsHsMsg.o))
///           ...
///           #include "aspects/electrical/EpsMacros.hh"
///           ...
///           TS_EPS_WARNING("Run as fast as you can and don't look back.");
/// @endcode
///
/// @par      Example 2 (composed text):
/// @code
///           LIBRARY DEPENDENCY:
///               ((simulation/hs/TsHsMsg.o))
///           ...
///           #include "aspects/electrical/EpsMacros.hh"
///           ...
///           TS_EPS_WARNING(<< i << " seconds until detonation.");
/// @endcode
/// @hideinitializer
///////////////////////////////////////////////////////////////////////////////////////////////////
#define TS_EPS_WARNING(cause) \
{ \
    TsHsMsg msg(TS_HS_WARNING, TS_HS_EPS); \
    if (mName.empty()) { \
        msg << cause; \
    } else { \
        msg << mName << " " << cause; \
    } \
    hsSendMsg(msg); \
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Macro for H&S info message.
///
/// @param[in]   cause      std::string     The text details (cause) for the message.
///
/// @details  This macro creates a TsHsMsg object to issue a health & status (H&S) info message.\n
///           The message will automatically indicate the file, line of code, method, thrower
///           (identified by the instance's mName attribute) and the detailed text supplied in the call.\n
///           This macro specifies severity = TS_HS_INFO and subsystem = TS_HS_EPS.\n
///           A  char* or const char* is also acceptable for the std:string argument. \n
///           If the mName attribute is null, then it is not included in the message.
///
/// @par      Example 1 (static cause):
/// @code
///           LIBRARY DEPENDENCY:
///               ((simulation/hs/TsHsMsg.o))
///           ...
///           #include "aspects/electrical/EpsMacros.hh"
///           ...
///           TS_EPS_INFO("You really screwed up this time.");
/// @endcode
///
/// @par      Example 2 (composed text):
/// @code
///           LIBRARY DEPENDENCY:
///               ((simulation/hs/TsHsMsg.o))
///           ...
///           #include "aspects/electrical/EpsMacros.hh"
///           ...
///           TS_EPS_INFO("It's " << i << " PM, do you know where your code is?");
/// @endcode
/// @hideinitializer
///////////////////////////////////////////////////////////////////////////////////////////////////
#define TS_EPS_INFO(cause) \
{ \
    TsHsMsg msg(TS_HS_INFO, TS_HS_EPS); \
    if (mName.empty()) { \
        msg << cause; \
    } else { \
        msg << mName << " " << cause; \
    } \
    hsSendMsg(msg); \
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Macro to copy a name.
///
/// @details  This macro initializes an EPS std::string class attribute mName from an std::string
///           name.
///
/// @par      Example:
/// @code
///           TS_EPS_NAME;
/// @endcode
/// @hideinitializer
///////////////////////////////////////////////////////////////////////////////////////////////////
#define TS_EPS_NAME \
{ \
    mName = name; \
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Macro to copy a name with an H&S error message and exception throw on empty name.
///
/// @param[in]   caller     std::string     The name of the calling class.
///
/// @throws   TsInitializationException
///
/// @details  This macro first checks an std::string name. If it is empty, an H&S error message
///           is issued and a TsInitializationException is thrown. Otherwise it initializes an
///           EPS std::string class attribute mName from name.
///
/// @par      Example:
/// @code
///           LIBRARY DEPENDENCY:
///               ((simulation/hs/TsHsMsg.o)
///                (software/exceptions/TsInitializtionException.o))
///           ...
///           #include "aspects/electrical/EpsMacros.hh"
///           #include "software/exceptions/TsInitializationException.hh"
///           ...
///           TS_EPS_NAME_ERREX("MyClass");
/// @endcode
/// @hideinitializer
///////////////////////////////////////////////////////////////////////////////////////////////////
#define TS_EPS_NAME_ERREX(caller) \
{ \
    if (name.empty()) { \
        TsHsMsg msg(TS_HS_ERROR, TS_HS_EPS); \
        msg << "throwing  TsInitializationException Invalid Initialization Data  -  Empty object name.\n" << tsStackTrace(); \
        hsSendMsg(msg); \
        throw TsInitializationException("Invalid Initialization Data", caller, "Empty object name."); \
    } \
    TS_EPS_NAME; \
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Macro to copy a name with an H&S fatal message on empty name.
///
/// @param[in]   caller     std::string     The name of the calling class.
///
/// @details  This macro first checks an std::string name. If it is empty, it issues an H&S fatal
///           message and returns. Otherwise it initializes an EPS std::string class attribute
///           mName from name.
///
/// @note     The calling class destructor should TS_DELETE_ARRAY(mName).
///
/// @par      Example:
/// @code
///           LIBRARY DEPENDENCY:
///               ((simulation/hs/TsHsMsg.o))
///           ...
///           #include "aspects/electrical/EpsMacros.hh"
///           ...
///           TS_EPS_NAME_FATAL("MyClass");
/// @endcode
/// @hideinitializer
///////////////////////////////////////////////////////////////////////////////////////////////////
#define TS_EPS_NAME_FATAL(caller) \
{ \
    if (name.empty()) { \
        TsHsMsg msg(TS_HS_FATAL, TS_HS_EPS); \
        msg << caller << " Empty object name."; \
        hsSendMsg(msg); \
        return; \
    } \
    TS_EPS_NAME; \
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Macro for a default data task method.
///
/// @param[in]   caller     std::string     The name of the calling class.
///
/// @note     The calling task must provide the initConfig and initInput methods.
///
/// @details  This macro provides the body of the standard default data task method that:
///           - Initializes the configuration data.
///           - Initializes the input data.
///           - Sends an H&S fatal message and returns on an unexpected exception derived from std::exception.
///           - Sends an H&S fatal message and returns on an unexpected exception not derived from std::exception.
///
/// @par      Example:
/// @code
///           LIBRARY DEPENDENCY:
///               ((simulation/hs/TsHsMsg.o))
///           ...
///           #include "aspects/electrical/EpsMacros.hh"
///           ...
///           void ExampleDefaultData::initialize(ExampleConfigData& config, ExampleInputData& input)
///           {
///               TS_EPS_DEFAULT_DATA;
///           }
/// @endcode
/// @hideinitializer
///////////////////////////////////////////////////////////////////////////////////////////////////
#define TS_EPS_DEFAULT_DATA(caller) \
{ \
    try { \
        initConfig(config); \
        initInput(input); \
    } catch (const std::exception& e) { \
        TsHsMsg msg(TS_HS_FATAL, TS_HS_EPS); \
        msg << caller << " unexpected exception in default data task.\n" << std::string(e.what()) << '\n' << tsStackTrace(); \
    } catch (...) { \
        TsHsMsg msg(TS_HS_FATAL, TS_HS_EPS); \
        msg << caller << " unexpected exception in default data task.\n" << tsStackTrace(); \
    } \
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Macro for a RT model step method.
///
/// @param[in]   valid     boolean     Validity flag (skip processing if not valid). An active and
///                                    healthy flag from telemetry may be and'ed with any other
///                                    conditions.
///
/// @note     The calling task must provide the getCommand and setTelemetry methods as well as attributes
///           mName for the object name and mInitialized for the initialization complete flag
///
/// @details  This macro provides the body of the standard RT step method that:
///           - Sends an H&S error message and returns on an uninitialized object.
///           - Sends an H&S error message and returns on the rtComm not enabled.
///           - Returns if the validity flag is not set
///           - Receives a command message into the model command buffer.
///           -- Sends an H&S error message and returns on any anticipated exceptions.
///           - Sends a telemetry message from the model telemetry buffer.
///           -- Sends an H&S error message and returns on any anticipated exceptions.
///           - Sends an H&S error message and returns on an unanticipated exception derived from std::exception.
///           - Sends an H&S error message and returns on an unanticipated exception not derived from std::exception.
///
/// @par      Example:
/// @code
///           LIBRARY DEPENDENCY:
///               ((simulation/hs/TsHsMsg.o))
///           ...
///           #include "aspects/electrical/EpsMacros.hh"
///           ...
///           void ExampleRt::step(const double dt __attribute__((unused)))
///           {
///               TS_EPS_RT_STEP;
///           }
/// @endcode
/// @hideinitializer
///////////////////////////////////////////////////////////////////////////////////////////////////
#define TS_EPS_RT_STEP(valid) \
{ \
    try { \
        if (!mInitialized) { \
            TS_EPS_ERROR("is not initialized."); \
            return; \
        } \
        if (!rtComm.isEnabled()) { \
            TS_EPS_ERROR("rtComm is not enabled."); \
            return; \
        } \
        if (valid) { \
            try { \
                getCommand(); \
            } catch (const RecvError& e) { \
                TS_EPS_ERROR("ivTepcDiscriminator.processRecvGroup - RecvError."        << '\n' << std::string(e.what())); \
            } catch (const RtCommNotInitialized& e) { \
                TS_EPS_ERROR("rtComm.getMessage(id, buffer) - RtCommNotInitialized."    << '\n' << std::string(e.what())); \
            } catch (const CalibrationOutOfRange& e) { \
                TS_EPS_ERROR("mUnified.unpack(id, buffer) - CalibrationOutOfRange."     << '\n' << std::string(e.what())); \
            } catch (const CalibrationUndefined& e) { \
                TS_EPS_ERROR("mUnified.unpack(id, buffer) - CalibrationUndefined."      << '\n' << std::string(e.what())); \
            } \
            try { \
                setTelemetry(); \
            } catch (const CalibrationOutOfRange& e) { \
                TS_EPS_ERROR("mUnified.packPCAFC00n0L(buffer) - CalibrationOutOfRange." << '\n' << std::string(e.what())); \
            } catch (const CalibrationUndefined& e) { \
                 TS_EPS_ERROR("mUnified.packPCAFC00n0L(buffer) - CalibrationUndefined." << '\n' << std::string(e.what())); \
            } catch (const RtCommNotInitialized& e) { \
                TS_EPS_ERROR("rtComm.sendMessage(id, buffer) - RtCommNotInitialized."   << '\n' << std::string(e.what())); \
            } \
        } \
    } catch (const std::exception& e) { \
        TS_EPS_ERROR("caught unexpected exception." << '\n' << std::string(e.what())); \
    } catch (...) { \
        TS_EPS_ERROR("caught unexpected exception."); \
    } \
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Macro for a signal aspect manager init method.
///
/// @note     The calling class must provide the methods
///           - void initialize(const std::string& name)
///           - void validate() const
///           - bool isInitialized() const
///           as well as the attributes
///           - char* mName        (object name)
///           - bool  mInitialized (initialization complete status flag)
///
/// @details  This macro provides the body of the standard signal aspect manager initialize method that:
///           - Resets the initialization complete flag.
///           - Allocates and initializes the object name or issues an H&S fatal message and returns if empty.
///           - Validates the configuration and input data.
///           - Initializes the object from configuration and input data.
///           - Initializes the outputs.
///           - Sets the initialization complete flag based on component initialization completion.
///           - Sends an H&S info message on successful initialization.
///           - Sends an H&S fatal message and returns on TsInitializationException.
///           - Sends an H&S fatal message and returns on unexpected exception derived from std::exception.
///           - Sends an H&S fatal message and returns on unexpected exception not derived from std::exception.
///
/// @par      Example:
/// @code
///           LIBRARY DEPENDENCY:
///               ((simulation/hs/TsHsMsg.o)
///                (software/exceptions/TsInitializationException.o))
///           ...
///           #include "aspects/electrical/EpsMacros.hh"
///           ...
///           void Example::init(const std::string& name)
///           {
///               TS_EPS_INIT(Example);
///           }
/// @endcode
/// @hideinitializer
///////////////////////////////////////////////////////////////////////////////////////////////////
#define TS_EPS_INIT(ClassName) \
{ \
    try { \
        mInitialized = false; \
        TS_EPS_NAME_FATAL(ClassName); \
        validate(); \
        initialize(name); \
        update(0.0); \
        mInitialized = isInitialized(); \
        TS_EPS_INFO("initialization complete."); \
    } catch (const TsInitializationException& e) { \
        TS_EPS_FATAL("caught initialization exception."); \
    } catch (const std::exception& e) { \
        TS_EPS_FATAL("caught unexpected exception." << '\n' << std::string(e.what())); \
    } catch (...) { \
        TS_EPS_FATAL("caught unexpected exception."); \
    } \
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Macro for a signal aspect manager step method.
///
/// @note     The calling class must provide the method
///           - void update(const double dt)
///           as well as the attributes
///           - char* mName        (object name)
///           - bool  mInitialized (initialization complete status flag)
///
/// @details  This macro provides the body of the standard signal aspect manager step method that:
///           - Sends an H&S error message and returns on an uninitialized object.
///           - Updates.
///           - Sends an H&S error message and returns on an unexpected exception derived from std::exception.
///           - Sends an H&S error message and returns on an unexpected exception not derived from std::exception.
///
/// @par      Example:
/// @code
///           LIBRARY DEPENDENCY:
///               ((simulation/hs/TsHsMsg.o))
///           ...
///           #include "aspects/electrical/EpsMacros.hh"
///           ...
///           void Example::step(const double dt)
///           {
///               TS_EPS_STEP;
///           }
/// @endcode
/// @hideinitializer
///////////////////////////////////////////////////////////////////////////////////////////////////
#define TS_EPS_STEP \
{ \
    try { \
        if (!mInitialized) { \
            TS_EPS_ERROR("is not initialized."); \
            return; \
        } \
        update(dt); \
    } catch (const std::exception& e) { \
        TS_EPS_ERROR("caught unexpected exception." << '\n' << std::string(e.what())); \
    } catch (...) { \
        TS_EPS_ERROR("caught unexpected exception."); \
    }\
}

/// @}

#endif

