/****************************************** TRICK HEADER ******************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
    (Console output plugin for health & status message framework - code)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
    ((TsHsOutputPlugin.o))

PROGRAMMERS:
   (
     ((Wesley A. White) (Tietronix Software) (August 2011) (Initial version))
   )
**************************************************************************************************/

#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include "sim_services/Message/include/message_proto.h"
#include "simulation/timer/TS_timer.h"
#include "TS_hs_msg_types.h"
#include "TsHsConsolePlugin.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details A local function which returns an ISO-8601 compliant timestamp.
///
/// @param[in] unix_time  (--) A Unix time_t value produced by calling the time() function.
///
/// @return  The timestamp as a std::string.
////////////////////////////////////////////////////////////////////////////////////////////////////
static std::string message_timestamp(time_t* unix_time)
{
   using std::setw;
   using std::setfill;

   // An ISO-8601 compliant timestamp:
   //   YYYY-MM-DDThh:mm:ssTZD (eg 1997-07-16T19:20:30+01:00)

   struct tm* t = gmtime(unix_time);

   std::ostringstream timestamp;
   timestamp << std::right << setw(4) << setfill('0') << t->tm_year+1900 << "-";
   timestamp << std::right << setw(2) << setfill('0') << t->tm_mon+1     << "-";
   timestamp << std::right << setw(2) << setfill('0') << t->tm_mday      << "T";
   timestamp << std::right << setw(2) << setfill('0') << t->tm_hour      << ":";
   timestamp << std::right << setw(2) << setfill('0') << t->tm_min       << ":";
   timestamp << std::right << setw(2) << setfill('0') << t->tm_sec       << "Z";

   return timestamp.str();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Constructor
///
////////////////////////////////////////////////////////////////////////////////////////////////////
TsHsConsolePlugin::TsHsConsolePlugin(int id) :
    TsHsOutputPlugin(id),
    mTryLockFailures(0),
    mResourceLock(),
    mBlocking(false)
{
    pthread_mutex_init(&mResourceLock, NULL);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
TsHsConsolePlugin::~TsHsConsolePlugin()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Configure the plugin.
///
/// @return  configData - contains the plugin configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsConsolePlugin::configure(const TsHsPluginConfig& configData)
{
    mEnabled   = configData.mEnabled;
    mBlocking  = configData.mBlocking;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Initialize the console plugin.
///
/// @return  True if successful, or false if initialization failed.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool TsHsConsolePlugin::init(void)
{
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Restarts a plugin.
///
/// @return  True if successful, or false if initialization failed.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool TsHsConsolePlugin::restart(void)
{
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Log a health and status message to a text file.
///
/// @param   [in] file      (--) name of file which initiated logging the message.
/// @param   [in] line      (--) line of file which initiated logging the message.
/// @param   [in] function  (--) The name of the function logging the message.
/// @param   [in] type      (--) the type of message (e.g. info, warning, etc.).
/// @param   [in] subsys    (--) the subsystem from which the message originated.
/// @param   [in] met       (--) the mission-elapsed time that the message was sent.
/// @param   [in] timestamp (--) the unix timestamp that the message was sent.
/// @param   [in] mtext     (--) the message text.
///
/// @return  True if successful, or false on failure.
///
/// @note    This method opens and closes the log file each time it logs a message. Not very
///          efficient, but perhaps more robust in multi-threaded sims - TBD?
////////////////////////////////////////////////////////////////////////////////////////////////////
bool TsHsConsolePlugin::msg(
   const std::string&   file,
   const int            line,
   const std::string&   function,
   TS_HS_MSG_TYPE       type,
   const std::string&   subsys,
   const TS_TIMER_TYPE& met,
   unsigned long        timestamp,
   const std::string&   mtext)
{
    if (!mEnabled)
    {
        return true;
    }

    // Use some std things to make the code more readable
    using std::setw;
    using std::setfill;

    std::ostringstream logentry;

    // Print string for type of message
    switch (type)
    {
        case TS_HS_DEBUG:   logentry << "DBG "; break;
        case TS_HS_INFO:    logentry << "INFO"; break;
        case TS_HS_WARNING: logentry << "WARN"; break;
        case TS_HS_ERROR:   logentry << "ERR "; break;
        case TS_HS_FATAL:   logentry << "FAT "; break;
        default:            logentry << "NA  ";
    }

    // Print subsystem
    logentry << " | " << std::left << setw(12) << subsys << " | ";

    // Print MET
    char sign = met.pre < 0 ? '-' : '+';

    logentry << sign;
    logentry << std::right << setw(3) << setfill('0') << met.day  << " ";
    logentry << std::right << setw(2) << setfill('0') << met.hour << ":";
    logentry << std::right << setw(2) << setfill('0') << met.min  << ":";
    logentry << std::right << setw(2) << setfill('0') << met.sec  << " | ";

    // Print Unix GMT time (a.k.a Zulu time, UTC)
    time_t tp = timestamp;
    std::string zulu = message_timestamp(&tp);
    logentry << std::right << zulu << " | ";

    // Print filename and line number
    std::ostringstream location;
    location << file << ":" << line << " ";
    if (function.length() > 0)
    {
       location << function << "() ";
    }

    logentry << std::left << setw(45) << setfill(' ') << location.str() << " | ";

    // Print user message
    logentry << mtext; // << std::endl;

    if (mBlocking)
    {
        // We will wait if necessary, no messages will be lost.
        if (pthread_mutex_lock(&mResourceLock) == 0) // 0 means lock granted
        {
            insertMessage(logentry.str());
            pthread_mutex_unlock(&mResourceLock);
        }
    }
    else
    {
        // Don't wait. Discard message if resource conflict.
        if (pthread_mutex_trylock(&mResourceLock) == 0) // 0 means lock granted
        {
            insertMessage(logentry.str());
            pthread_mutex_unlock(&mResourceLock);
        }
        else
        {
            mTryLockFailures++;
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Shutdown the text plugin
////////////////////////////////////////////////////////////////////////////////////////////////////
bool TsHsConsolePlugin::insertMessage(const std::string& message)
{
    std::cout << message << std::endl;
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Shutdown the text plugin
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsConsolePlugin::shutdown(void)
{
    if (mTryLockFailures > 0)
    {
        message_publish(MSG_WARNING, "TsHsConsolePlugin skipped %d messages due to mutex conflicts\n", mTryLockFailures);
    }
}

