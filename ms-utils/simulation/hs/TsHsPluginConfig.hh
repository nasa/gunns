#ifndef TsHsPluginConfig_EXISTS
#define TsHsPluginConfig_EXISTS

/**
@defgroup  TSM_UTILITIES_SIMULATION_HS_PLUGIN_CONFIGURATION Plugin Configuration
@ingroup   TSM_UTILITIES_SIMULATION_HS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Health & Status message framework plugin configuration structure.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- (
    (simulation/hs/TsHsMngr.o)
    (simulation/hs/TsHsStdMngr.o)
  )

PROGRAMMERS:
- (
    ((Jeffrey Middleton) (L3) (January 2010) (Initial version))
    ((Wesley A. White) (Tietronix Software) (August 2011))
  )

@{
*/

#include <string>

/// @brief A simple container class used by the TsHsStdMngr to pass configuration parameters to output plugins.
struct TsHsPluginConfig
{
    TsHsPluginConfig();

    bool        mEnabled;       //    (--) True if plugin is enabled
    bool        mOverwrite;     //    (--) Reuse plugin target (e.g. overwrite file vs. create a new timestamped one)
    std::string mPath;          //    (--) Path to file to which the plugin writes
    bool        mBlocking;      //    (--) This plugin will wait when needed resources are busy
};

/// @}

#endif /* TsHsPluginConfig_EXISTS */
