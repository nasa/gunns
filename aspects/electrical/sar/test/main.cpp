////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_GUNNS_ELECTRICAL_SOLAR_ARRAY_REGULATOR   Conductor Unit tests
/// @ingroup   UT_GUNNS_ELECTRICAL
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
/// @details   Unit test classes for GUNNS Electrical Resistor link models.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/ui/text/TestRunner.h>

#include "UtGunnsSolarArrayRegulator.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    argc  int     --  not used
/// @param    argv  char**  --  not used
///
/// @return    --  status (always 0)
///
/// @details  Main for GUNNS Electrical Solar Array Regulator link model unit tests in the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int, char**)
{
    CppUnit::TextTestRunner runner;

    runner.addTest(UtGunnsSolarArrayRegulator::suite());

    runner.run();

    return 0;
}
