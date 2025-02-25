////////////////////////////////////////////////////////////////////////////////////////////////////
/// @copyright Copyright 2025 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @defgroup  UT_UTILITIES_INTEROP_INTEROP Interoperability Models Unit Tests
/// @ingroup   UT_UTILITIES_INTEROP
/// @details   Unit test classes for interoperability models.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/ui/text/TestRunner.h>

#include "UtDistributed2WayBusBase.hh"
#include "UtDistributed2WayBusFluid.hh"
#include "UtDistributed2WayBusElect.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    argc  int     --  not used
/// @param    argv  char**  --  not used
///
/// @return    --  status (always 0)
///
/// @details  Main for interoperability models unit tests in the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
    CppUnit::TextTestRunner runner;

    runner.addTest(UtDistributed2WayBusBase::suite());
    runner.addTest(UtDistributed2WayBusFluid::suite());
    runner.addTest(UtDistributed2WayBusElect::suite());

    runner.run();

    return 0;
}

