/**
@defgroup  UT_UTILITIES_MATH_LINEAR_ALGEBRA Linear Algebra Utilities Unit Tests
@ingroup   UT_UTILITIES_MATH
@details   This is the unit test main for the ts-models linear algebra utilities classes.

@{

LIBRARY DEPENDENCY:
(
)

*/

#include <cppunit/ui/text/TestRunner.h>

#include "UtCholeskyLdu.hh"
#include "UtSor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    argc  int     --  not used
/// @param    argv  char**  --  not used
///
/// @return    --  status (always 0)
///
/// @details  Main for Gunns unit tests in the CPPUNIT framework.
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
    CppUnit::TextTestRunner runner;

    runner.addTest( UtCholeskyLdu::suite() );
    runner.addTest( UtSor::suite() );

    runner.run(testresult);
    // Output results in compiler format
    CPPUNIT_NS::CompilerOutputter compilerOutputter(&collectedResults, std::cout);
    compilerOutputter.write();

    // Output results in XML for Jenkins xUnit Plugin
    std::ofstream xmlFileOut("ts-models_ms-utils_math_linear_algebraTestResults.xml");
    CppUnit::XmlOutputter xmlOut(&collectedResults, xmlFileOut);
    xmlOut.write();

    return 0;
}

/// @}

