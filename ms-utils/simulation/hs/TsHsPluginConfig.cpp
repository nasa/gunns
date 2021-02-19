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

#include "TsHsPluginConfig.hh"

TsHsPluginConfig::TsHsPluginConfig() :
    mEnabled(false),
    mOverwrite(true),
    mPath(""),
    mBlocking(false)
{
}
