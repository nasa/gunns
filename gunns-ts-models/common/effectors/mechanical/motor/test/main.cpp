/********************************* TRICK HEADER *******************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
    (Motor main test runners)

LIBRARY DEPENDENCY:
(
     (common/effectors/mechanical/motor/TsDcPwmDynMotor.o)
     (common/effectors/mechanical/motor/DcDynPumpMotor.o)
)

PROGRAMMERS:
    ((Jeffrey Middleton) (L-3) (December 2010)
     (Rachel Obeidzinski)(L-3) (TS21) (November 2011)
     (Matt Wilkinson)    (L-3) (TS21) (July 2012) )
    )
*******************************************************************************/

/**
 LIBRARY DEPENDENCY:
- (
   (simulation/hs/TsHsStdMngr.o)
  )
*/
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include "UtTsDcPwmDynMotor.hh"
#include "UtDcDynPumpMotor.hh"
#include <cppunit/XmlOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>

int main(int argc, char** argv)
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
    runner.addTest(UtTsDcPwmDynMotor::suite() );
    runner.addTest(UtDcDynPumpMotor::suite() );

    runner.run(testresult);
    // Output results in compiler format
    CPPUNIT_NS::CompilerOutputter compilerOutputter(&collectedResults, std::cout);
    compilerOutputter.write();

    // Output results in XML for Jenkins xUnit Plugin
    std::ofstream xmlFileOut("ts-models_common_effectors_mechanical_motorTestResults.xml");
    CppUnit::XmlOutputter xmlOut(&collectedResults, xmlFileOut);
    xmlOut.write();

    return 0;
}

