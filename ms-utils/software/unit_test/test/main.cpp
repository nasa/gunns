/******************************** TRICK HEADER *****************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
()

REQUIREMENTS:
(())

REFERENCE:
(())

ASSUMPTIONS AND LIMITATIONS:
(())

LIBRARY DEPENDENCY:
(())

PROGRAMMERS:
(((Ryan Prentice) (L-3) (Feb 15, 2013) (TS21) (--)))
***************************************************************************************************/

// System includes
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

// Test includes
#include "UtTsAssert.hh"

#include <cppunit/XmlOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
int main()
{
    // Informs test-listener about testresults
    CPPUNIT_NS::TestResult testresult;

    // Register listener for collecting the test-results
    CPPUNIT_NS::TestResultCollector collectedResults;
    testresult.addListener(&collectedResults);

    // Register listener for per-test progress output
    CPPUNIT_NS::BriefTestProgressListener progress;
    testresult.addListener(&progress);
    CppUnit::TextTestRunner runner;

    runner.addTest(UtTsAssert::suite());

    runner.run(testresult);
    // Output results in compiler format
    CPPUNIT_NS::CompilerOutputter compilerOutputter(&collectedResults, std::cout);
    compilerOutputter.write();

    // Output results in XML for Jenkins xUnit Plugin
    std::ofstream xmlFileOut("ts-models_ms-utils_software_unit_testTestResults.xml");
    CppUnit::XmlOutputter xmlOut(&collectedResults, xmlFileOut);
    xmlOut.write();
}

