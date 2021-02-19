////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_XML_FILE_SEARCH Unit tests for XmlFileSearch.
/// @ingroup   UT_XML_FILE_SEARCH
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
/// @details   Unit test class for Xml file search utility.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/ui/text/TestRunner.h>

#include "UtXmlFileSearch.hh"

///@file main.cpp
///@brief Unit Tests for XmlFileSearch.

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    argc  int     --  not used
/// @param    argv  char**  --  not used
///
/// @return    --  status (always 0)
///
/// @details  Main for XmlFileSearch tests in the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
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

//#include "aspects/UtHsEnable.txt"

   CppUnit::TextTestRunner runner;

   runner.addTest( UtXmlFileSearch::suite());

    runner.run(testresult);
    // Output results in compiler format
    CPPUNIT_NS::CompilerOutputter compilerOutputter(&collectedResults, std::cout);
    compilerOutputter.write();

    // Output results in XML for Jenkins xUnit Plugin
    std::ofstream xmlFileOut("ts-models_ms-utils_fileSearchTestResults.xml");
    CppUnit::XmlOutputter xmlOut(&collectedResults, xmlFileOut);
    xmlOut.write();

   return 0;
}

