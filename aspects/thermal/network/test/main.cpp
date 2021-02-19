/***************************************************************************************************
///@defgroup  UT_PTCS_ThermalNetwork   PTCS ThermalNetwork tests
///@ingroup   UT_PTCS
///@details   This is the unit test main for the PTCS Thermal Network.
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
LIBRARY DEPENDENCY:
  ((simulation/hs/TsHsStdMngr.o))
***************************************************************************************************/
#include <cppunit/ui/text/TestRunner.h>
#include "UtThermalNetwork.hh"
#include "UtThermFileParser.hh"
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    argc  int     --  used in enabling H&S
/// @param    argv  char**  --  not used
///
/// @return    --  status (always 0)
///
/// @details  Main for Thermal Network series of unit tests in the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
    CppUnit::TextTestRunner runner;
    runner.addTest( UtThermFileParser::suite() );
    runner.addTest( UtThermalNetwork::suite() );
    runner.run();
    return 0;
}
