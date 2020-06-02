////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_CONTROLLER Unit tests for controllers
/// @ingroup   TSM_CONTROLLER
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/ui/text/TestRunner.h>

#include "UtTsLimitSwitch.hh"
#include "UtTsLimitSwitchAnalog.hh"
#include "UtTsNoise.hh"
#include "UtTsOpticSmokeDetector.hh"
#include "UtSensorBase.hh"
#include "UtSensorAnalog.hh"
#include "UtSensorBooleanBi.hh"
#include "UtSensorBooleanAi.hh"
#include "UtSensorVlvOpenClose.hh"
#include "UtTsFanSpeedSensors.hh"
#include <cppunit/XmlOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>

///@file main.cpp
///@brief Unit Tests for controller related classes

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    argc  int     --  not used
/// @param    argv  char**  --  not used
///
/// @return    --  status (always 0)
///
/// @details  Main for controllerAspect unit tests in the CPPUNIT framework.
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

   runner.addTest( UtTsNoise::suite() );
   runner.addTest( UtTsLimitSwitch::suite() );
   runner.addTest( UtTsLimitSwitchAnalog::suite() );
   runner.addTest( UtTsOpticSmokeDetector::suite() );
   runner.addTest( UtSensorBase::suite() );
   runner.addTest( UtSensorAnalog::suite() );
   runner.addTest( UtSensorBooleanBi::suite() );
   runner.addTest( UtSensorBooleanAi::suite() );
   runner.addTest( UtTsFanSpeedSensors::suite() );
   runner.addTest( UtSensorVlvOpenClose::suite() );

    runner.run(testresult);
    // Output results in compiler format
    CPPUNIT_NS::CompilerOutputter compilerOutputter(&collectedResults, std::cout);
    compilerOutputter.write();

    // Output results in XML for Jenkins xUnit Plugin
    std::ofstream xmlFileOut("ts-models_common_sensorsTestResults.xml");
    CppUnit::XmlOutputter xmlOut(&collectedResults, xmlFileOut);
    xmlOut.write();

   return 0;
}

