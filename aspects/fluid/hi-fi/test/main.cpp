////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_GUNNS_FLUID_HIFI   Hi-Fi Fluid Link Unit tests
/// @ingroup   UT_GUNNS_FLUID
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
/// @details   Unit test classes for GUNNS High-Fidelity fluid link models.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/ui/text/TestRunner.h>

#include "UtGunnsFluidHiFiOrifice.hh"
#include "UtGunnsFluidHiFiValve.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param     int     --  not used
/// @param     char**  --  not used
///
/// @return    --  status (always 0)
///
/// @details  Main for GUNNS fluid hi-fi link model unit tests in the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int, char**)
{
    CppUnit::TextTestRunner runner;

    runner.addTest(UtGunnsFluidHiFiOrifice::suite());
    runner.addTest(UtGunnsFluidHiFiValve::suite());

    runner.run();

    return 0;
}
