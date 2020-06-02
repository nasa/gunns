/****************************************** TRICK HEADER ******************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
    (Health and status message manager, base class - code)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
   (
    (TsHsTextPlugin.o)
    (TsHsOutputPlugin.o)
   )

PROGRAMMERS:
   (
    ((Jeffrey Middleton) (L3))
   )
**************************************************************************************************/

#include "TsHsMngr.hh"
//    (TsHsSqlitePlugin.o)

// Global used by message macros to log messages.
TsHsMngr*  tsGlobalHsMngr = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsHsMngr::TsHsMngr()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsHsMngr::~TsHsMngr()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Should be called once to initialize the manager. Called during trick initialization.
///        Sets the global pointer tsGlobalHsMngr to point to this TsHsMngr (or actually the
///        subclass of TsHsMngr).
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsMngr::init()
{
    tsGlobalHsMngr = this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Deprecated. Use init method. Left here for backwards compatibility so input files
///          don't break.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsMngr::registerSelf()
{
    tsGlobalHsMngr = this;
}

