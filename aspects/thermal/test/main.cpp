/***************************************************************************************************
@defgroup  UT_GUNNS_THERMAL   GUNNS Thermal Unit Tests
@ingroup   UT_GUNNS
@details   This is the unit test for General-Use Nodal Network Solver (GUNNS) thermal components.
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
LIBRARY DEPENDENCY:
  ((simulation/hs/TsHsStdMngr.o))
***************************************************************************************************/
#include <cppunit/ui/text/TestRunner.h>

#include "UtGunnsThermalCapacitor.hh"
#include "UtGunnsThermalCapacitorHeatQueues.hh"
#include "UtGunnsThermalRadiation.hh"
#include "UtGunnsThermalHeater.hh"
#include "UtGunnsThermalPanel.hh"
#include "UtGunnsThermalMultiPanel.hh"
#include "UtGunnsThermalPotential.hh"
#include "UtGunnsThermalSource.hh"
#include "UtGunnsThermalPhaseChangeBattery.hh"
#include "UtGunnsThermoelectricEffect.hh"
#include "UtGunnsThermoelectricDevice.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    argc  int     --  not used
/// @param    argv  char**  --  not used
///
/// @return    --  status (always 0)
///
/// @details  Main for Gunns: Thermal components unit tests in the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
    CppUnit::TextTestRunner runner;
    runner.addTest( UtGunnsThermalCapacitor::suite() );
    runner.addTest( UtGunnsThermalCapacitorHeatQueues::suite() );
    runner.addTest( UtGunnsThermalRadiation::suite() );
    runner.addTest( UtGunnsThermalHeater::suite() );
    runner.addTest( UtGunnsThermalPotential::suite() );
    runner.addTest( UtGunnsThermalPanel::suite() );
    runner.addTest( UtGunnsThermalMultiPanel::suite() );
    runner.addTest( UtGunnsThermalSource::suite() );
    runner.addTest( UtGunnsThermalPhaseChangeBattery::suite() );
    runner.addTest( UtGunnsThermoelectricEffect::suite() );
    runner.addTest( UtGunnsThermoelectricDevice::suite() );
    runner.run();
    return 0;
}
