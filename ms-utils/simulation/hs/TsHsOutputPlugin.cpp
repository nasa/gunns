/****************************************** TRICK HEADER ******************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
    (Health and status message framework, with output plugin architecture - code)

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
     ((Jeffrey Middleton) (L3) (January 2010) (Initial version))
   )
**************************************************************************************************/
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "simulation/timer/TS_timer.h"
#include "TS_hs_msg_types.h"
#include "TsHsOutputPlugin.hh"


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details A utility function which returns a timestamp of the form: "_MMDDHHmm" where the letters
///          represent month, day, hour, minute, respectively.
///
/// @return  The timestamp as a std::string.
////////////////////////////////////////////////////////////////////////////////////////////////////
std::string tsHsFileTimestamp()
{
   using std::setw;
   using std::setfill;

   time_t ltime = time(NULL);
   struct tm* t = localtime(&ltime);

   std::ostringstream timestamp;
   timestamp << "_";
   timestamp << std::right << setw(2) << setfill('0') << t->tm_mday;
   timestamp << std::right << setw(2) << setfill('0') << t->tm_mon+1;
   timestamp << std::right << setw(2) << setfill('0') << t->tm_hour;
   timestamp << std::right << setw(2) << setfill('0') << t->tm_min;

   return timestamp.str();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Deletes all managed plugins.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsHsOutputPluginMngr::~TsHsOutputPluginMngr()
{
    for (unsigned i=0; i < plugins.size(); i++)
    {
        delete plugins[i];
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Initializes all managed plugins.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsOutputPluginMngr::init(void)
{
    for (unsigned i=0; i < plugins.size(); i++)
    {
        if (plugins[i]->isEnabled())
        {
            if (!plugins[i]->init())
            {
                plugins[i]->setEnabled(false);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Updates all managed plugins.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsOutputPluginMngr::update(void)
{
    for (unsigned i=0; i < plugins.size(); i++)
    {
        if (plugins[i]->isEnabled())
        {
            if (!plugins[i]->update())
            {
                plugins[i]->setEnabled(false);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Shuts down all managed plugins.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsOutputPluginMngr::shutdown(void)
{
    for (unsigned i=0; i < plugins.size(); i++)
    {
        if (plugins[i]->isEnabled())
        {
            plugins[i]->shutdown();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Forwards the specified message to all managed plugins.
///
/// @param[in] file      (--) name of file which initiated logging the message.
/// @param[in] line      (--) line of file which initiated logging the message.
/// @param[in] function  (--) The name of the function logging the message; typically __func__.
/// @param[in] type      (--) the type of message (e.g. info, warning, etc.).
/// @param[in] subsys    (--) the subsystem from which the message originated.
/// @param[in] met       (--) the mission-elapsed time that the message was sent.
/// @param[in] timestamp (--) the unix timestamp that the message was sent.
/// @param[in] mtext     (--) the message text.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsOutputPluginMngr::msg(
        const std::string&   file,
        const int            line,
        const std::string&   function,
        TS_HS_MSG_TYPE       type,
        const std::string&   subsys,
        const TS_TIMER_TYPE& met,
        unsigned long        timestamp,
        const std::string&   mtext)
{
    for (unsigned i=0; i < plugins.size(); i++)
    {
        if (plugins[i]->isEnabled())
        {
            if (!plugins[i]->msg(file, line, function, type, subsys, met, timestamp, mtext))
            {
                plugins[i]->setEnabled(false);
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Adds a plugin to the plugin managers list of output plugins.
///
/// @param[in] plugin   (--) A pointer to the plugin to add.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsOutputPluginMngr::add(TsHsOutputPlugin* plugin)
{
    plugins.push_back(plugin);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Returns a pointer to the specified plugin.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsHsOutputPlugin* TsHsOutputPluginMngr::getPlugin(int id)
{
    for (unsigned i=0; i < plugins.size(); i++)
    {
        if (plugins[i]->getId() == id)
        {
            return plugins[i];
        }
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Returns the number of plugins currently being managed by the plugin manager.
///
/// @return  The number of managed plugins.
////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned TsHsOutputPluginMngr::numPlugins()
{
    return static_cast<unsigned>(plugins.size());
}
