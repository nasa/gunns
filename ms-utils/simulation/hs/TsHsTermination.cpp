/*
@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
    (TsHsTermination can terminate the simulation under certain conditions during attempts to
     log fatal messages. The class maintains a list of subsystems for which an attempt to log a
     fatal message should terminate the simulation. When a subsystem attempts to log a fatal
     message the simulation is terminated if the subsystem sending the message is in the
     classes list of subsystems.)

PROGRAMMERS:
   (
     ((Wesley A. White) (Tietronix Software) (June 2013) (Initial version))
   )
*/

#include <sstream>
#include <string>
#ifndef no_TRICK_ENV
#include "sim_services/include/exec_proto.h"
#endif
#include "TsHsTermination.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Default constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
TsHsTermination::TsHsTermination():
    mFatalToSubsystem(),
    mEnabled(true)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
TsHsTermination::~TsHsTermination()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Log a health and status message to the terminate plugin. This will terminate the
///            simulation if the message type TS_HS_FATAL and the originating subsystem is in
///            the list of "is-fatal-to" subsystems.
///
/// @param[in] file      (--) name of file which initiated logging the message.
/// @param[in] line      (--) line of file which initiated logging the message.
/// @param[in] function  (--) The name of the function logging the message.
/// @param[in] type      (--) the type of message (e.g. info, warning, etc.).
/// @param[in] subsystem (--) the subsystem from which the message originated.
/// @param[in] mtext     (--) the message text.
///
/// @return  True if successful, or false on failure.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool TsHsTermination::msg(
    const std::string&   file,
    const int            line,
    const std::string&   function,
    TS_HS_MSG_TYPE       type,
    const std::string&   subsystem,
    const std::string&   mtext)
{
    if (mEnabled && type == TS_HS_FATAL && isFatalToSubsystem(subsystem))
    {
        std::ostringstream location;
        location << file << ":" << line << " ";
        if (function.length() > 0)
        {
           location << function << "() ";
        }

        // Skip this for non-Trick environments
#ifndef no_TRICK_ENV
        // Const cast required for trick 7 do not remove while there are still trick 7 users
        exec_terminate(const_cast<char*>(location.str().c_str()), const_cast<char*>(mtext.c_str()));
#endif
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Determine if a TS_HS_FATAL message should cause termination of the simulation.
///
/// @param[in] subsystem   (--) Check if the message should be fatal to this subsystem.
///
/// @return    True if the message should terminate the simulation, else false.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool TsHsTermination::isFatalToSubsystem(const std::string subsystem)
{
    // If the key is not in the set, it's not fatal
    StringSet::const_iterator iter = mFatalToSubsystem.find(subsystem);
    return (iter != mFatalToSubsystem.end());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Turn on/off sim termination behavior for the specified subsystem.
///
/// @param[in] subsystem  (--) The relevant subsystem.
/// @param[in] fatal      (--) True means turn termination behavior "on", false means "off".
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsTermination::setFatalToSubsystem(const std::string subsystem, bool fatal)
{
    if (fatal == true)
    {
        mFatalToSubsystem.insert(subsystem);
    }
    else
    {
        mFatalToSubsystem.erase(subsystem);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Turn on sim termination behavior for the specified delimited list of subsystems.
///            Clears existing settings before applying new ones, if any, i.e. it replaces.
///
/// @param[in] subsystems   (--) List of delimited subsystem names.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsTermination::setFatalToMultipleSubsystems(const std::string subsystems)
{
    mFatalToSubsystem.clear();
    appendFatalToMultipleSubsystems(subsystems);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Turn on sim termination behavior for the specified delimited list of subsystems.
///            Does not clear existing settings before applying new ones, i.e. it appends.
///
/// @param[in] subsystems  (--) A string containing a list of delimited subsystems for which
///                             logging a "fatal" message should terminate the simulation.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsTermination::appendFatalToMultipleSubsystems(const std::string subsystems)
{
    const std::string delimiters = ":,; ";

    // Skip leading delimiters
    std::string::size_type lastPos = subsystems.find_first_not_of(delimiters, 0);

    // Find the next delimiter
    std::string::size_type pos = subsystems.find_first_of(delimiters, lastPos);

    while (pos != std::string::npos || lastPos != std::string::npos)
    {
        // Token is from lastPos to pos
        mFatalToSubsystem.insert(subsystems.substr(lastPos, pos - lastPos));

        // Skip past delimiters
        lastPos = subsystems.find_first_not_of(delimiters, pos);

        // Find the next delimiter
        pos = subsystems.find_first_of(delimiters, lastPos);
    }
}
