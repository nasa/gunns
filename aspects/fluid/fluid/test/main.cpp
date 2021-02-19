////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_PLUMBING_FLUID   Fluid Unit tests
/// @ingroup   UT_PLUMBING
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
/// @details   Unit test classes for plumbing fluids.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/ui/text/TestRunner.h>

#include "UtMonoFluid.hh"
#include "UtPolyFluid.hh"
#include "UtGunnsFluidTraceCompounds.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param     int     --  not used
/// @param     char**  --  not used
///
/// @return    --  status (always 0)
///
/// @details  Main for Plumbing Fluid unit tests in the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int, char**) {
    CppUnit::TextTestRunner runner;

    runner.addTest(UtMonoFluid::suite());
    runner.addTest(UtPolyFluid::suite());
    runner.addTest(UtGunnsFluidTraceCompounds::suite());

    runner.run();

    return 0;
}

/// @}
