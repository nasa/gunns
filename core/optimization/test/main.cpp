/**
@defgroup  UT_GUNNS_OPTIM   GUNNS Optimization Unit Tests
@ingroup   UT_TS_MODELS

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details   This is the unit test main for the GUNNS optimization code.

@{

*/

#include <cppunit/ui/text/TestRunner.h>

#include "UtGunnsOptimMonteCarlo.hh"
#include "UtGunnsOptimParticleSwarm.hh"
#include "UtGunnsOptimGradientDescent.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    argc  int     --  not used
/// @param    argv  char**  --  not used
///
/// @return    --  status (always 0)
///
/// @details  Main for GUNNS unit tests in the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {
    CppUnit::TextTestRunner runner;

    runner.addTest( UtGunnsOptimMonteCarlo::suite() );
    runner.addTest( UtGunnsOptimParticleSwarm::suite() );
    runner.addTest( UtGunnsOptimGradientDescent::suite() );

    runner.run();

    return 0;
}

/// @}
