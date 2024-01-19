/**
@copyright Copyright 2024 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
- (This is the main class used to log model health and status messages. Any message that can
   be helpful for debugging purposes should be logged using this class. The message severity
   should also be specified in order to indicate the importance of the message to the
   application that is logging it.)


REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
   (
    (TsHsMngr.o)
    (TsHsTermination.o)
   )

PROGRAMMERS:
   (
    ((Jeff Wilson) (L3) (July 2010))
    ((Jeffrey Middleton) (L3) (August 2010))
    ((Wesley A. White) (Tietronix Software) (August 2011))
   )
*/

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <vector>

#include <cxxabi.h>
#include <execinfo.h>

#include "TsHsMngr.hh"
#include "TsHsMsg.hh"
#include "TsHsTermination.hh"
#ifndef no_TRICK_ENV
#include "sim_services/Executive/include/exec_proto.hh"
#endif

// Static variable used to enable or disable logging methods.
bool TsHsMsg::mEnabled = true;

// Static variable used to retrive subsys string based on SimObject::id
std::vector<const char*> SubSysStringVector(20, static_cast<const char*>(0) );


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Non-member function which registers an subsys override string for all HS activity
///          associated with the SimObject identified by the id.
///          A vector is used to store the strings.
///          Should be called from a "default_data" or "initialization" job.
///          Both job classes are called prior to entering freeze so no special treatment for
///          check-pointing.
///
/// @param[in] id     (int)           SimObject id  - index into the vector
/// @param[in] subsys (const char *)  override string for the subsystem.
////////////////////////////////////////////////////////////////////////////////////////////////////
void tsHsRegisterSubsys(const int id, const char* subsys)
{
    if(id < 0){
        //std::cout << "\ntsHsRegisterSubsys: negative index for " << subsys << std::endl;
        hsSendMsg(TS_HS_ERROR, subsys, "Negative index");
        return;
    }

    try {
         //std::cout << id << " " << subsys << " " << SubSysStringVector.size() << std::endl;
         if(id >= static_cast<const int>(SubSysStringVector.size())) {
            SubSysStringVector.resize(id+10, static_cast<const char*>(0) );
         }
         SubSysStringVector.at(id) = subsys;
    } catch (...) {
        hsSendMsg(TS_HS_ERROR, subsys, "Unknown error in registration");
    }

}

#ifndef no_TRICK_ENV
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Non-member function which based on the current job looks a override string for
///          the subsystem.  If the current job provides access to the parent sim object which has
///          an integer identifier.  The identifier is used as an index into a vector of strings.
///          The string at the index is populated by tsHsRegisterSubsys. If the sim object has not
///          registered the default from the supplied std::string is returned.
///
/// @param[in] subsystem (const std::string&) default subsystem string
/// @return  subsystem string.
////////////////////////////////////////////////////////////////////////////////////////////////////
const char* tsHsGetSubstringFromJob(const std::string & subSystem)
{
    Trick::JobData* jd = exec_get_curr_job();
    const char* str = 0;
    if(jd){
        unsigned int id = jd->parent_object->id;
        if (id < SubSysStringVector.size() ){
            str =  SubSysStringVector[id];
        }
    }

    if (0 == str) {
        // use default
        str = subSystem.c_str();
    }

    return str;
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Non-member function which produces a stack trace string for debugging purposes.
///          The string is generated using gcc's backtrace function. The application must be
///          compiled with "-g" option and linked with the "-rdynamic" option for the backtrace to
///          contain meaningful information.
///
/// @return  The stack trace string with embedded newlines between each stack frame line.
/// @see     http://www.gnu.org/software/libc/manual/html_node/Backtraces.html
/// @see     http://stackoverflow.com/questions/77005/how-to-generate-a-stacktrace-when-my-gcc-c-app-crashes
////////////////////////////////////////////////////////////////////////////////////////////////////
std::string tsStackTrace()
{
    void*  array[10];
    int    size         = backtrace(array, 10);
    char** stackStrings = backtrace_symbols(array, size);

    // We're out of memory, so just get a message out in a way that most likely to succeed.
    if (0 == stackStrings) {
        std::cerr << "malloc failed in function tsStackTrace()" << std::endl;
        return ("");
    }

    if (1 == size) {
        return ("Only one entry in the stack trace and that is in tsStackTrace.");
    }

    std::ostringstream out;

    // Skip the first stack frame, it points here
    for (int i = 1; i < size; ++i) {
        char* mangledName = 0;
        char* offsetBegin = 0;
        char* offsetEnd   = 0;

        // Find parentheses and +address offset surrounding mangled name
        for (char *p = stackStrings[i]; *p; ++p) {
            if (*p == '(') {
                mangledName = p;
            } else if (*p == '+') {
                offsetBegin = p;
            } else if (*p == ')') {
                offsetEnd = p;
                break;
            }
        }

        // if the line could be processed, attempt to demangle the symbol
        if (mangledName && offsetBegin && offsetEnd && mangledName < offsetBegin) {
            *mangledName++  = '\0';
            *offsetBegin++  = '\0';
            *offsetEnd++    = '\0';
            int status;
            char* realName  = abi::__cxa_demangle(mangledName, 0, 0, &status);
            // if demangling is successful, output the demangled function name
            if (0 == status && 0 != realName) {
                if (0 == std::string(realName).compare("Trick::JobData::call()")) {
                    // quit at Trick task call
                    free(realName);
                    break;
                }
                out << stackStrings[i] << " : " << realName    << " +" << offsetBegin << offsetEnd << '\n';
            } else {
                // otherwise, output the mangled function name
                out << stackStrings[i] << " : " << mangledName << " +" << offsetBegin << offsetEnd << '\n';
            }
            free(realName);
        } else {
            // otherwise, print the whole line
            out << stackStrings[i] << '\n';
        }
    }

    free(stackStrings);

    return out.str();
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Constructor that sets the message severity and subsystem, but not the message text.
///
/// @param[in] severity   (--) Enumeration constant that represents the type of message.
/// @param[in] subsys     (--) The subsystem that is logging the message.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsHsMsg::TsHsMsg(TS_HS_MSG_TYPE severity, const std::string& subsys) :
    mSeverity(severity),
    mSubsys(subsys)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Constructor which sets all data members, including the message text.
///
/// @param[in] severity   (--) Enumeration constant that represents the type of message.
/// @param[in] subsys     (--) The subsystem that is logging the message.
/// @param[in] mtext      (--) The message text.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsHsMsg::TsHsMsg(TS_HS_MSG_TYPE severity, const std::string& subsys, const std::string& mtext) :
    mSeverity(severity),
    mSubsys(subsys)
{
    setMessageText(mtext);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
TsHsMsg::~TsHsMsg()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Clears the message text. Does not affect other message fields.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsMsg::clear()
{
    str("");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Gets the messages message text.
/// @return  The message's message text.
////////////////////////////////////////////////////////////////////////////////////////////////////
std::string TsHsMsg::getMessageText() const
{
    return (str());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Gets the message's subsystem.
/// @return  The name of the subsystem which created this message.
////////////////////////////////////////////////////////////////////////////////////////////////////
std::string TsHsMsg::getSubsys() const
{
    return mSubsys;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Gets the message's severity.
/// @return  The message's severity enumeration constant (e.g. TS_HS_WARNING).
////////////////////////////////////////////////////////////////////////////////////////////////////
TS_HS_MSG_TYPE TsHsMsg::getSeverity() const
{
    return mSeverity;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Sets the message's severity.
/// @param[in] severity   (--) The message's severity enumeration constant (e.g. TS_HS_WARNING).
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsMsg::setSeverity(TS_HS_MSG_TYPE severity)
{
    mSeverity = severity;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Sets the message's subsystem.
/// @param[in] subsys   (--) The name of the subsystem which created this message.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsMsg::setSubsys(const std::string& subsys)
{
    mSubsys = subsys;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Sets the message's message text. This is the part of the message which tells the
///            reader what actually happened.
///
/// @param[in] mtext   (--) The message text.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsMsg::setMessageText(const std::string& mtext)
{
    // Use the inherited SIO to put text in buffer. This method results in the desired 'append'
    // behavior during subsequent SIO use.

    clear();
    *this << mtext;

    // Note: the code below was not compatible with the SIO. Text set using str()
    //       would get overwritten by subsequent use of the SIO. The desired behavior
    //       was for the SIO to append to existing message text not replace it.

    // Get the stringbuf from the inherited ostringstream and put str into the it
    //str(mtext);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Sends a message to the message log assuming the message's message text has
///            already been set either by the ctor or a setter. While this method is public, the
///            helper macro hsSendMsg can be used instead to automatically determine the file
///            and line number.
///
/// @param[in] file      (--) Source file name invoking 'send'; typically __FILE__ macro is used.
/// @param[in] line      (--) Line in file invoking 'send'; typically __LINE__ macro is used.
/// @param[in] function  (--) Function invoking 'send'; typically __func__ macro is used.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsMsg::send(const std::string& file, unsigned line, const std::string& function)
{
    // Note tsGlobalHsMngr can be 0 when running unit tests because some unit tests code calls the
    // HS macro w/o creating a HS sim object. Ignore such requests.
    if (!mEnabled  || tsGlobalHsMngr == 0)
        return;

#ifdef no_TRICK_ENV
    tsGlobalHsMngr->msg(file, line, function, mSeverity, mSubsys.c_str(), getMessageText().c_str());
#else
    const char* subsys =  tsHsGetSubstringFromJob(mSubsys);

    tsGlobalHsMngr->msg(file, line, function, mSeverity, subsys, getMessageText().c_str());
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Sends a message to the message log. While this method is public, the
///            helper macro hsSendMsg can be used instead to automatically determine the file
///            and line number.
///
/// @param[in] file      (--) Source file name invoking 'send'; typically __FILE__ macro is used.
/// @param[in] line      (--) Line in file invoking 'send'; typically __LINE__ macro is used.
/// @param[in] function  (--) Function invoking 'send'; typically __func__ macro is used.
/// @param[in] mtext     (--) The message text.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsMsg::send(const std::string& file, unsigned line, const std::string& function, const std::string& mtext)
{
    if (!mEnabled  || tsGlobalHsMngr == 0)
        return;

    setMessageText(mtext);
    send(file, line, function);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   A non-class function which sends a message to the message log. This version is
///            primarily intended for use by the hsSendMsg macro.
///
/// @param[in] file      (--) Source file name invoking 'send'; typically __FILE__ macro is used.
/// @param[in] line      (--) Line in file invoking 'send'; typically __LINE__ macro is used.
/// @param[in] function  (--) Function invoking 'send'; typically __func__ macro is used.
/// @param[in] msg       (--) A message object.
////////////////////////////////////////////////////////////////////////////////////////////////////
void tsHsSendMsgFileLine(const std::string& file, unsigned line, const std::string& function, const TsHsMsg & msg)
{
    if (!TsHsMsg::isEnabled() || tsGlobalHsMngr == 0)
        return;

    std::string mtext = msg.getMessageText();
#ifdef no_TRICK_ENV
    tsGlobalHsMngr->msg(file, line, function, msg.getSeverity(),  msg.getSubsys().c_str(), mtext.c_str());
#else
    std::string subsys =  tsHsGetSubstringFromJob(msg.getSubsys());

    tsGlobalHsMngr->msg(file, line, function, msg.getSeverity(), subsys, mtext.c_str());
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   A non-class function which sends a message to the message log. This version is
///            primarily intended for use by the hsSendMsg macro.
///
/// @param[in] file     (--) Source file name invoking 'send'; typically __FILE__ macro is used.
/// @param[in] line     (--) Line in file invoking 'send'; typically __LINE__ macro is used.
/// @param[in] function (--) The name of the function logging the message; typically __func__.
/// @param[in] severity (--) Enumeration constant that represents the type of message.
/// @param[in] subsys   (--) The subsystem that is logging the message.
/// @param[in] mtext    (--) The message text.
////////////////////////////////////////////////////////////////////////////////////////////////////
void tsHsSendMsgFileLine(const std::string& file, unsigned line, const std::string& function,
        TS_HS_MSG_TYPE severity, const std::string& subsys, const std::string& mtext)
{
    if (!TsHsMsg::isEnabled() || tsGlobalHsMngr == 0)
        return;

#ifdef no_TRICK_ENV
    tsGlobalHsMngr->msg(file, line, function, severity, subsys.c_str(), mtext.c_str());
#else
    const char* simsubsys =  tsHsGetSubstringFromJob(subsys);

    tsGlobalHsMngr->msg(file, line, function, severity, simsubsys, mtext.c_str());
#endif
}


