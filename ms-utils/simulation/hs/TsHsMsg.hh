#ifndef TsHsMsg_EXISTS
#define TsHsMsg_EXISTS

/**
@defgroup  TSM_UTILITIES_SIMULATION_HS_MSG Logger
@ingroup   TSM_UTILITIES_SIMULATION_HS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (This is the main class used to log model health and status messages. Any message that can
   be helpful for debugging purposes should be logged using this class. The message severity
   should also be specified in order to indicate the importance of the message to the
   application that is logging it.)

REQUIREMENTS:
- (TBD)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((TsHsMsg.o))

PROGRAMMERS:
- (
    ((Jeff Wilson) (L3) (July 2011))
    ((Jeffrey Middleton) (L3) (August 2010)))
    ((Wesley A. White) (Tietronix Software) (August 2011))
  )

ICG: (No)

@{
*/

#include <sstream>
#include "TS_hs_msg_types.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief   Developer's interface to health and status message logging. Any message that
///          can be helpful for debugging purposes should be logged using this class. The message
///          severity should also be specified in order to indicate the importance of the message
///          to the application that is logging it. <b>Developer note:</b> developers will
///          interact with health and status primarily via the methods and macros defined by
///          this class.
///
/// @details To log messages, you have several options. If you just want to log a simple char* or
///          string message, the easiest way to do it is to use the 3-argument version of the
///          hsSendMsg macro passing the message type, subsystem name, and message text as arguments:
/// @code
///          hsSendMsg(TS_HS_WARNING, "ECLSS", "...some message...");
/// @endcode
/// @details This macro will put the file name and the line number into the message automagically.
///          If you need to build up a message which includes numeric fields the easiest way to
///          do this is to use the stream insertion operator "<<":
/// @code
///          // cshComponentNamePtr is a char* string. The variable "ii" is an integer card number.
///          TsHsMsg msg(TS_HS_WARNING, "CARDS CMN");
///          msg << cdhComponentNamePtr << "-DISABLED, card[" << ii << "]-DISABLED as a result";
///          hsSendMsg(msg);
/// @endcode
/// @details In the example above, we first create a message object "msg" which contains
///          the message type and subsystem. We use the "<<" operator to pipe information into
///          the message text part of the message object. Then we send the message to the log
///          by using the 1-argument version of the hsSendMacro. The "<<" operator supports
///          the use of stream manipulators, i.e. setw, setfill, etc. Just remember to include
///          "<iomanip>".
///
/// @details There are other ways to use the TsHsMsg class which involve using setters and the
///          send methods, but the ways outlined above will usually suffice. The 'send' methods
///          have a slight disadvantage compared to the macros in that they do not automatically
///          insert file and line numbers into messages; users must pass them in as arguments.
///
//////////////////////////////////////////////////////////////////////////////////////////////////
class TsHsMsg : public std::ostringstream
{
public:

    /// @brief Constructor that sets only severity and subsystem. The message should be
    ///        set later using setMessageText or the stream insertion operator "<<".
    TsHsMsg(TS_HS_MSG_TYPE severity, const std::string& subsys);

    /// @brief Constructor that takes all available information describing the error.
    TsHsMsg(TS_HS_MSG_TYPE severity, const std::string& subsys, const std::string& msg);

    /// @brief Destructor
    ~TsHsMsg();

    /// @brief Sets a message's severity type, for example, TS_HS_WARNING.
    void setSeverity(TS_HS_MSG_TYPE severity);

    /// @brief Sets the message's subsystem. This can be any string, for example, "ENV".
    void setSubsys(const std::string& subsys);

    /// @brief Sets the message's message text. Note that message text can also be set using the stream insertion operator "<<".
    void setMessageText(const std::string& mtext);

    /// @brief Clears a message's message text. This does not affect the severity or subsystem fields.
    void clear();

    /// @brief Sends the message. While this method is public, the
    ///        helper macro "hsSendMsg" can be used instead to automatically
    ///        determine the file and line number.
    void send(const std::string& file, unsigned line, const std::string& function);

    /// @brief Send the message. While this method is public, the
    ///        helper macro "hsSendMsg" can be used instead to automatically
    ///        determine the file and line number.
    void send(const std::string& file, unsigned line, const std::string& function, const std::string& msg);

    /// @brief Get the message's subsystem name.
    std::string    getSubsys() const;

    /// @brief Get the message's message text.
    std::string    getMessageText() const;

    /// @brief Get the message's severity, e.g. TS_HS_WARNING.
    TS_HS_MSG_TYPE getSeverity() const;

    /// @brief Static methods to turn on/off logging capability.
    static void disable()   {mEnabled = false;}
    static void enable()    {mEnabled = true; }
    static bool isEnabled() {return mEnabled; }

private:

    // Do not allow default ctor, cctor, or assignment
    TsHsMsg();
    TsHsMsg(const TsHsMsg&);
    TsHsMsg& operator=(const TsHsMsg&);

    // Data Members

    TS_HS_MSG_TYPE mSeverity;  // ** (--) The type of message: info, warning, etc.
    std::string    mSubsys;    // ** (--) The subsystem issuing the message: ECLSS, EPS, etc.

    static bool    mEnabled;   // ** (--) Logging is enabled. If disabled messages will be ignored.
};

/// @brief Convenience macro which creates a message object, supplies file name and line information, and
///        sends the created message.
#define hsSendMsg(args...) tsHsSendMsgFileLine(__FILE__, __LINE__, __func__, ##args)

/// @brief Used by the hsSendMsg macro when the macro is invoked with a TsHsMsg object.
void tsHsSendMsgFileLine(const std::string&, unsigned, const std::string&, const TsHsMsg&);

/// @brief Used by the hsSendMsg macro when the macro is invoked with severity, subsystem, and message text arguments.
void tsHsSendMsgFileLine(const std::string&, unsigned, const std::string&, TS_HS_MSG_TYPE, const std::string&, const std::string&);

/// @brief Generate a stack trace string
std::string tsStackTrace();

/// @brief register a string for the subsystem of a given SimObject id
void tsHsRegisterSubsys(const int id, const char* subsys);

/// @}

#endif /* TsHsMsg_EXISTS */
