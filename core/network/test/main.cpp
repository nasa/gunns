/**
@defgroup  UT_GUNNS_NETWORK   GUNNS Network Unit Tests
@ingroup   UT_TS_MODELS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details   This is the unit test main for the GUNNS network classes.

@{
*/

#include <cppunit/ui/text/TestRunner.h>

#include "UtGunnsNetworkBase.hh"
#include "UtGunnsMultiLinkConnector.hh"
#include "UtGunnsBasicSuperNetwork.hh"
#include "UtGunnsFluidSuperNetwork.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    argc  int     --  not used
/// @param    argv  char**  --  not used
///
/// @return    --  status (always 0)
///
/// @details  Main for Gunns unit tests in the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {
    CppUnit::TextTestRunner runner;

    runner.addTest( UtGunnsNetworkBase::suite() );
    runner.addTest( UtGunnsMultiLinkConnector::suite() );
    runner.addTest( UtGunnsBasicSuperNetwork::suite() );
    runner.addTest( UtGunnsFluidSuperNetwork::suite() );

    runner.run();

    return 0;
}

/// @}
