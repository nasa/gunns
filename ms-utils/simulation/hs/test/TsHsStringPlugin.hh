#ifndef TsHsStringPlugin_EXISTS
#define TsHsStringPlugin_EXISTS

/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.

#include <string>

#include "simulation/timer/TS_timer.h"
#include "simulation/hs/TsHsOutputPlugin.hh"

/// @brief For unit testing. Allows messages to be logged to a string so they can be compared using assert macros.

class TsHsStringPlugin : public TsHsOutputPlugin
{
public:
    TsHsStringPlugin(int id) : TsHsOutputPlugin(id), data() { }

    bool init();
    bool update();
    void shutdown();
    bool msg(const std::string& file, const int line, const std::string& function, TS_HS_MSG_TYPE type, const std::string& subsys,
            const TS_TIMER_TYPE& met, unsigned long timestamp, const std::string& message);

    std::string data;
};

#endif /* TsHsStringPlugin_EXISTS */
