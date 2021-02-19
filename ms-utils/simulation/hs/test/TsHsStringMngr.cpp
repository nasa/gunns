///@copyright Copyright 2019 United States Government as represented by the Administrator of the
///           National Aeronautics and Space Administration.  All Rights Reserved.
#include "TsHsStringMngr.hh"
#include "TsHsStringPlugin.hh"
#include "simulation/timer/TS_timer.h"


void TsHsStringMngr::init()
{
    stringPlugin.init();
}

void TsHsStringMngr::restart()
{

}

void TsHsStringMngr::update()
{
    stringPlugin.update();
}

void TsHsStringMngr::shutdown()
{
    stringPlugin.shutdown();
}

void TsHsStringMngr::msg(
        const std::string& file,
        const int          line,
        const std::string& function,
        TS_HS_MSG_TYPE     type,
        const std::string& subsys,
        const std::string& message)
{
    const TS_TIMER_TYPE met = {0, 0, 0, 0, 0, 0, 0};
    unsigned long timestamp = 0L;
    stringPlugin.msg(file, line, function, type, subsys, met, timestamp, message);
}

std::string TsHsStringMngr::str()
{
    return stringPlugin.data;
}

void TsHsStringMngr::str(const std::string& str)
{
    stringPlugin.data = str;
}
