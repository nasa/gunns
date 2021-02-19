/********************************* TRICK HEADER *******************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
    (Health and Status (H&S) main test runner)

PROGRAMMERS:
    ((Jeffrey Middleton) (L-3) (August 2010))
    ((Wesley A. White)   (TSI) (August 2011))
    ((Wesley A. White)   (TSI) (June   2013))
*******************************************************************************/

#include <cppunit/ui/text/TestRunner.h>

#include "UtTsHsOutputPluginMngr.hh"
//#include "UtTsHsTerminatePlugin.hh"
#include "UtTsHsMsg.hh"
#include "UtTsHsMsgWrapper.hh"
#include "UT_TS_hs.hh"
#include "UtTsHsMsgStdFilter.hh"
#include "UtTsHsMsgQueue.hh"

#include <cppunit/XmlOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
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

    // Add each subsystem test suite

    runner.addTest(UtTsHsOutputPluginMngr::suite());
    //runner.addTest(UtTsHsTerminatePlugin::suite());
    runner.addTest(UtTsHsMsg::suite());
    runner.addTest(UtTsHsMsgWrapper::suite());
    runner.addTest(UT_TS_hs::suite());
    runner.addTest(UtTsHsMsgStdFilter::suite());
    runner.addTest(UtTsHsMsgQueue::suite());

    runner.run(testresult);
    // Output results in compiler format
    CPPUNIT_NS::CompilerOutputter compilerOutputter(&collectedResults, std::cout);
    compilerOutputter.write();

    // Output results in XML for Jenkins xUnit Plugin
    std::ofstream xmlFileOut("ts-models_ms-utils_simulation_hsTestResults.xml");
    CppUnit::XmlOutputter xmlOut(&collectedResults, xmlFileOut);
    xmlOut.write();

    return 0;
}

