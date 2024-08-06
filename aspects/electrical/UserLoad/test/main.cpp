/**
@defgroup  UT_GUNNS   GUNNS Thermal Unit Tests
@ingroup   UT_TS_MODELS
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
@details   This is the unit test for General-Use Nodal Network Solver (GUNNS) thermal components.

@{

LIBRARY DEPENDENCY:
(

)

*/

#include <cppunit/ui/text/TestRunner.h>

#include "UtConstantPowerLoad.hh"
#include "UtResistiveLoad.hh"
#include "UtDerivedResistanceLoadExample.hh"
#include "UtGunnsElectResistiveUserLoad.hh"
#include "UtGunnsElectConstantPowerUserLoad.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    argc  int     --  not used
/// @param    argv  char**  --  not used
///
/// @return    --  status (always 0)
///
/// @details  Main for Gunns: Thermal components unit tests in the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {

    ResistiveLoad lResistanceLoad;
    ConstantPowerLoad lPowerLoad;

    CppUnit::TextTestRunner runner;

    runner.addTest( UtConstantPowerLoad::suite() );
    runner.addTest( UtResistiveLoad::suite() );
    runner.addTest( UtDerivedResistanceLoadExample::suite() );
    runner.addTest( UtGunnsElectResistiveUserLoad::suite() );
    runner.addTest( UtGunnsElectConstantPowerUserLoad::suite() );

    runner.run();

    return 0;
}

/// @}
