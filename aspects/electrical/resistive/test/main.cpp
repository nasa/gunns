////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_GUNNS_ELECTRICAL_RESISTOR   Conductor Unit tests
/// @ingroup   UT_GUNNS_ELECTRICAL
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
/// @details   Unit test classes for GUNNS Electrical Resistor link models.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/ui/text/TestRunner.h>

#include "UtGunnsElectricalResistor.hh"
#include "UtGunnsResistorPowerFunction.hh"
#include "UtGunnsResistiveLoad.hh"
#include "UtGunnsShortUtil.hh"
#include "UtGunnsElectShort.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    argc  int     --  not used
/// @param    argv  char**  --  not used
///
/// @return    --  status (always 0)
///
/// @details  Main for GUNNS Electrical Resistor link model unit tests in the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int, char**)
{
    CppUnit::TextTestRunner runner;

    runner.addTest(UtGunnsElectricalResistor::suite());
    runner.addTest(UtGunnsResistorPowerFunction::suite());
    runner.addTest(UtGunnsResistiveLoad::suite());
    runner.addTest(UtGunnsShortUtil::suite());
    runner.addTest(UtGunnsElectShort::suite());

    runner.run();

    return 0;
}
