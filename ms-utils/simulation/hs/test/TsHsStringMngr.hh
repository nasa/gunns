#ifndef HsStringMnger_EXISTS
#define HsStringMnger_EXISTS

/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.

#include <string>

#include "TsHsMngr.hh"
#include "TsHsStringPlugin.hh"

/// @brief For unit testing. A TsHsMngr designed to work with the TsHsStringPlugin.
class TsHsStringMngr : public TsHsMngr
{
    public:
        TsHsStringMngr() : stringPlugin(0) { }

        void init();
        void restart();
        void update();
        void shutdown();
        void msg(const std::string&, const int, const std::string& function, TS_HS_MSG_TYPE, const std::string&, const std::string&);

        std::string str();
        void str(const std::string&);

    private:
        TsHsStringPlugin stringPlugin;

};

#endif /* HsStringMnger_EXISTS */
