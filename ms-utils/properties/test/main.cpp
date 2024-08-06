////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_TSM_PROPERTIES  Properties Unit tests
/// @ingroup   UT_TSM
/// @copyright Copyright 2022 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
/// @details   Unit test classes for the properties models.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/ui/text/TestRunner.h>

#include "UtChemicalCompound.hh"
#include "UtChemicalReaction.hh"
#include "UtFluidProperties.hh"
#include "UtMaterialProperties.hh"
#include "UtSolidProperties.hh"
#include "UtFluidCurveFit.hh"
#include "UtCombust.hh"
#include "UtSorbantProperties.hh"
#include <cppunit/XmlOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>

int main(int, char**)
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

    runner.addTest( UtFluidProperties::suite() );
    runner.addTest( UtMaterialProperties::suite() );
    runner.addTest( UtSolidProperties::suite() );
    runner.addTest( UtChemicalCompound::suite() );
    runner.addTest( UtChemicalReaction::suite() );
    runner.addTest( UtCombust::suite() );
    runner.addTest( UtSorbantProperties::suite() );

    runner.run(testresult);
    // Output results in compiler format
    CPPUNIT_NS::CompilerOutputter compilerOutputter(&collectedResults, std::cout);
    compilerOutputter.write();

    // Output results in XML for Jenkins xUnit Plugin
    std::ofstream xmlFileOut("ts-models_ms-utils_propertiesTestResults.xml");
    CppUnit::XmlOutputter xmlOut(&collectedResults, xmlFileOut);
    xmlOut.write();

    return 0;
}
