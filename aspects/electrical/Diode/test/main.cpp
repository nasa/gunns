////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_GUNNS_ELECT_DIODE   Diode Unit tests
/// @ingroup   UT_GUNNS_ELECT
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
/// @details   Unit test class for Diode Elect.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <cppunit/ui/text/TestRunner.h>
#include "UtDiodeElect.hh"
#include "UtGunnsElectRealDiode.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    argc  int     --  not used
/// @param    argv  char**  --  not used
///
/// @return    --  status (always 0)
///
/// @details  Main for BatteryRelaySignal unit tests in the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {
    CppUnit::TextTestRunner runner;

    runner.addTest(UtDiodeElect::suite());
    runner.addTest(UtGunnsElectRealDiode::suite());

    runner.run();

    return 0;
}

/// @}
