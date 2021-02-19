////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_GUNNS_FLUID_CAPACITOR   Capacitor Unit tests
/// @ingroup   UT_GUNNS_FLUID
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
/// @details   Unit test classes for GUNNS fluid capacitor link models.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/ui/text/TestRunner.h>

#include "UtGunnsFluidTank.hh"
#include "UtGunnsFluidBalloon.hh"
#include "UtGunnsFluidAccum.hh"
#include "UtGunnsFluidAccumGas.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param     int     --  not used
/// @param     char**  --  not used
///
/// @return    --  status (always 0)
///
/// @details  Main for GUNNS fluid capacitor link model unit tests in the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int, char**)
{
    CppUnit::TextTestRunner runner;

    runner.addTest(UtGunnsFluidTank::suite());
    runner.addTest(UtGunnsFluidBalloon::suite());
    runner.addTest(UtGunnsFluidAccum::suite());
    runner.addTest(UtGunnsFluidAccumGas::suite());

    runner.run();

    return 0;
}
