////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_CONTROLLERFLUID Unit tests for fluid controllers
/// @ingroup   UT_CONTROLLER
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
/// @details   Unit test classes for fluid controller models.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/ui/text/TestRunner.h>

#include "UtTsDualSolenoidValveController.hh"
#include "UtTsOpenCloseValveCmd.hh"
#include "UtTsOpenCloseValveController.hh"
#include "UtTsOpenCloseValveSensed.hh"
#include "UtTsPositionValveCmd.hh"
#include "UtTsPositionValveController.hh"
#include "UtTsPoweredValveController.hh"
#include "UtTsPumpMotorController.hh"
#include "UtTsSpeedValveCmd.hh"
#include "UtTsSpeedValveController.hh"
#include "UtTsToggleValveCmd.hh"
#include "UtTsToggleValveController.hh"
#include "UtTsTValveController.hh"
#include <cppunit/XmlOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include "UtTsValveController.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    argc  int     (--)  argument count (unused)
/// @param    argv  char**  (--)  arguments (unused)
///
/// @return   (--)  status (always 0)
///
/// @details  Main for fluid controller unit tests in the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc __attribute__((unused)), char** argv __attribute__((unused))) {

    // Informs test-listener about testresults
    CPPUNIT_NS::TestResult testresult;

    // Register listener for collecting the test-results
    CPPUNIT_NS::TestResultCollector collectedResults;
    testresult.addListener(&collectedResults);

    // Register listener for per-test progress output
    CPPUNIT_NS::BriefTestProgressListener progress;
    testresult.addListener(&progress);
    CppUnit::TextTestRunner runner;

    runner.addTest( UtTsValveController::suite() );
    runner.addTest( UtTsTValveController::suite() );
    runner.addTest( UtTsDualSolenoidValveController::suite() );
    runner.addTest( UtTsPoweredValveController::suite() );
    runner.addTest( UtTsPumpMotorController::suite() );
    runner.addTest( UtTsOpenCloseValveCmd::suite() );
    runner.addTest( UtTsOpenCloseValveSensed::suite() );
    runner.addTest( UtTsOpenCloseValveController::suite() );
    runner.addTest( UtTsToggleValveCmd::suite() );
    runner.addTest( UtTsToggleValveController::suite() );
    runner.addTest( UtTsSpeedValveCmd::suite() );
    runner.addTest( UtTsSpeedValveController::suite() );
    runner.addTest( UtTsPositionValveCmd::suite() );
    runner.addTest( UtTsPositionValveController::suite() );

    runner.run(testresult);
    // Output results in compiler format
    CPPUNIT_NS::CompilerOutputter compilerOutputter(&collectedResults, std::cout);
    compilerOutputter.write();

    // Output results in XML for Jenkins xUnit Plugin
    std::ofstream xmlFileOut("ts-models_common_controllers_fluidTestResults.xml");
    CppUnit::XmlOutputter xmlOut(&collectedResults, xmlFileOut);
    xmlOut.write();

    return 0;
}

