////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_CONTROLLER_VALVE_ASSEMBLIES Unit tests for valve controller assemblies
/// @ingroup   UT_CONTROLLER
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
/// @details   Unit test classes for valve controller assembly models.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/ui/text/TestRunner.h>

#include "UtTsDualSolenoidValveAssembly.hh"
#include "UtTsOpenCloseValveAssembly.hh"
#include <cppunit/XmlOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include "UtTsToggleValveAssembly.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    argc  int     (--)  argument count (not used)
/// @param    argv  char**  (--)  arguments (not used)
///
/// @return   (--)  status (always 0)
///
/// @details  Main for fluid controller unit tests in the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {
    // Informs test-listener about testresults
    CPPUNIT_NS::TestResult testresult;

    // Register listener for collecting the test-results
    CPPUNIT_NS::TestResultCollector collectedResults;
    testresult.addListener(&collectedResults);

    // Register listener for per-test progress output
    CPPUNIT_NS::BriefTestProgressListener progress;
    testresult.addListener(&progress);

    CppUnit::TextTestRunner runner;

    runner.addTest( UtTsDualSolenoidValveAssembly::suite() );
    runner.addTest( UtTsOpenCloseValveAssembly::suite() );
    runner.addTest( UtTsToggleValveAssembly::suite() );

    runner.run(testresult);
    // Output results in compiler format
    CPPUNIT_NS::CompilerOutputter compilerOutputter(&collectedResults, std::cout);
    compilerOutputter.write();

    // Output results in XML for Jenkins xUnit Plugin
    std::ofstream xmlFileOut("ts-models_common_controllers_valveAssembliesTestResults.xml");
    CppUnit::XmlOutputter xmlOut(&collectedResults, xmlFileOut);
    xmlOut.write();

    return 0;
}

