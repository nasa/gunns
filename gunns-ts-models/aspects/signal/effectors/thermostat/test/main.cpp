////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_ASPECTS_SIGNAL_EFFECTORS_THERMOSTAT Unit tests for thermostat
/// @ingroup   TSM_ASPECTS_SIGNAL_EFFECTORS
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 LIBRARY DEPENDENCY:
- (
    (simulation/hs/TsHsStdMngr.o)
  )
*/
#include <cppunit/ui/text/TestRunner.h>

#include "UtThermostat.hh"
#include <cppunit/XmlOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>

///@file main.cpp
///@brief Unit Tests for thermostat related classes

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    argc  int     --  not used
/// @param    argv  char**  --  not used
///
/// @return    --  status (always 0)
///
/// @details  Main for Thermostat unit tests in the CPPUNIT framework.
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

    runner.addTest( UtThermostat::UtThermostat::suite() );
    runner.run(testresult);

    // Output results in compiler format
    CPPUNIT_NS::CompilerOutputter compilerOutputter(&collectedResults, std::cout);
    compilerOutputter.write();

    // Output results in XML for Jenkins xUnit Plugin
    std::ofstream xmlFileOut("gunns-ts-models_aspects_signal_effectors_thermostatTestResults.xml");
    CppUnit::XmlOutputter xmlOut(&collectedResults, xmlFileOut);
    xmlOut.write();
    return 0;
}

