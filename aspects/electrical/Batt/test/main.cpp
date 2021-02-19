////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_GUNNS_ELECTRICAL_BATTERY   Battery Unit tests
/// @ingroup   UT_GUNNS_ELECTRICAL
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
/// @details   Unit test class for GUNNS battery models.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/ui/text/TestRunner.h>
#include "UtGunnsElectBatteryCell.hh"
#include "UtGunnsElectBattery.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    argc  int     --  not used
/// @param    argv  char**  --  not used
///
/// @return    --  status (always 0)
///
/// @details  Main for Battery unit tests in the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {
    CppUnit::TextTestRunner runner;

    runner.addTest(UtGunnsElectBatteryCell::suite());
    runner.addTest(UtGunnsElectBattery::suite());

    runner.run();

    return 0;
}

/// @}
