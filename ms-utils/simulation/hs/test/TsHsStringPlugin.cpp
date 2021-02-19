/****************************************** TRICK HEADER ******************************************
 * @copyright Copyright 2019 United States Government as represented by the Administrator of the
 *            National Aeronautics and Space Administration.  All Rights Reserved.
 *
 * PURPOSE:
 *    (Health and status message framework, with output plugin architecture - code)
 *
 * REQUIREMENTS:
 *    ()
 *
 * REFERENCE:
 *    ()
 *
 * ASSUMPTIONS AND LIMITATIONS:
 *    ()
 *
 * LIBRARY DEPENDENCY:
 *    ((TsHsStringPlugin.o))
 *
 * PROGRAMMERS:
 *    (
 *    ((Derek Bredbenner) (L3) (June 2011) (Initial version))
 *    ((Wes White) (L3) (August 2011))
 *    ((Wes White) (L3) (September 2011))
 *    ((Wes White) (L3) (October 2011))
 *    ((Derek Bredbenner) (L3) (March 2012))
 *    )
 ***************************************************************************************************/


#include <string>
#include <sstream>

#include "simulation/timer/TS_timer.h"
#include "TsHsStringPlugin.hh"

bool TsHsStringPlugin::init()
{
    data = "initialized";
    return true;
};

bool TsHsStringPlugin::update()
{
    data = "updated";
    return true;
};

void TsHsStringPlugin::shutdown()
{
    data = "shut down";
};

bool TsHsStringPlugin::msg(
        const std::string&    file,
        const int             line,
        const std::string&    function,
        TS_HS_MSG_TYPE        type,
        const std::string&    subsys,
        const TS_TIMER_TYPE&  met,
        unsigned long         timestamp __attribute__((unused)),
        const std::string&    message)
{
    std::ostringstream oss;

    // For testing purposes just cram the data into the stream object without formatting it
    oss << file << "|" << line << "|" << function << "|" << static_cast<int>(type) << "|" << subsys << "|";
    oss << met.pre << met.day << met.hour << met.min << met.sec << "|" << met.seconds << "|" << met.hold << "|";
    oss << message;

    // Extract the char* from the stream
    data = oss.str();

    return true;
}
