/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2020 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
 ((core/GunnsThermalCapacitorHeatQueues.o))
***************************************************************************************************/

#include "software/exceptions/TsInitializationException.hh"
#include "UtGunnsThermalCapacitorHeatQueues.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsThermalCapacitorHeatQueues class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsThermalCapacitorHeatQueues::UtGunnsThermalCapacitorHeatQueues()
    :
    tArticle(tCapacitor),
    tName("test article"),
    tConfig(tName),
    tInput(),
    tCapacitor()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsThermalCapacitorHeatQueues class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsThermalCapacitorHeatQueues::~UtGunnsThermalCapacitorHeatQueues()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalCapacitorHeatQueues::tearDown()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalCapacitorHeatQueues::setUp()
{
    /// - Test spotter configuration.
    tConfig.mName       = tName;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the GunnsThermalCapacitorHeatQueuesConfigData class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalCapacitorHeatQueues::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsThermalCapacitorHeatQueues 01: testConfig ................";

    /// - Test nominal config data construction.
    CPPUNIT_ASSERT(tName == tConfig.mName);

    /// - Test default config data construction.
    GunnsThermalCapacitorHeatQueuesConfigData article(tName);
    CPPUNIT_ASSERT(tName == article.mName);

    /// @test new/delete for code coverage
    GunnsThermalCapacitorHeatQueuesConfigData* config = new GunnsThermalCapacitorHeatQueuesConfigData("name");
    delete config;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the GunnsThermalCapacitorHeatQueuesInputData class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalCapacitorHeatQueues::testInput()
{
    std::cout << "\n UtGunnsThermalCapacitorHeatQueues 02: testInput .................";

    /// @test new/delete for code coverage
    GunnsThermalCapacitorHeatQueuesInputData* input = new GunnsThermalCapacitorHeatQueuesInputData;
    delete input;


    std::cout << "... Pass";
}

#include <iostream>
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the default constructor of the GunnsThermalCapacitorHeatQueues class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalCapacitorHeatQueues::testDefaultConstruction()
{
    std::cout << "\n UtGunnsThermalCapacitorHeatQueues 03: testDefaultConstruction ...";

    /// @test state data
    CPPUNIT_ASSERT(""                   ==  tArticle.mName);
    CPPUNIT_ASSERT(tCapacitor.getName() == tArticle.mCapacitor.getName());

    /// @test init flag
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);

    /// @test new/delete for code coverage
    GunnsThermalCapacitorHeatQueues* article = new GunnsThermalCapacitorHeatQueues(tCapacitor);
    delete article;

    GunnsThermalCapacitorHeatQueueData* data = new GunnsThermalCapacitorHeatQueueData;
    delete data;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the initialize method of the GunnsThermalCapacitorHeatQueues class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalCapacitorHeatQueues::testInitialize()
{
    std::cout << "\n UtGunnsThermalCapacitorHeatQueues 04: testInitialize ............";

    tArticle.initialize(&tConfig, &tInput);

    /// - Test nominal initialization.
    CPPUNIT_ASSERT(tName      ==  tArticle.mName);

    /// @test init flag
    CPPUNIT_ASSERT(true       ==  tArticle.mInitFlag);

    /// - Test exception thrown from missing name.
    tConfig.mName = "";
    CPPUNIT_ASSERT_THROW(tArticle.initialize(&tConfig, &tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);
    tConfig.mName = tName;

    /// - Test exception thrown from null config data.
    const GunnsThermalCapacitorHeatQueuesConfigData* nullConfig = 0;
    CPPUNIT_ASSERT_THROW(tArticle.initialize(nullConfig, &tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);

    /// - Test exception thrown from null input data.
    const GunnsThermalCapacitorHeatQueuesInputData* nullInput = 0;
    CPPUNIT_ASSERT_THROW(tArticle.initialize(&tConfig, nullInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);

    /// - Test exception thrown on bad config data pointer type.
    BadGunnsThermalCapacitorHeatQueuesConfigData* badConfig = new BadGunnsThermalCapacitorHeatQueuesConfigData(tName);
    CPPUNIT_ASSERT_THROW(tArticle.initialize(badConfig, &tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);
    delete badConfig;

    /// - Test exception thrown on bad input data pointer type.
    BadGunnsThermalCapacitorHeatQueuesInputData* badInput = new BadGunnsThermalCapacitorHeatQueuesInputData();
    CPPUNIT_ASSERT_THROW(tArticle.initialize(&tConfig, badInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);
    delete badInput;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the stepPreSolver method of the GunnsThermalCapacitorHeatQueues class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalCapacitorHeatQueues::testPreSolver()
{
    std::cout << "\n UtGunnsThermalCapacitorHeatQueues 05: testPreSolver .............";

    tArticle.initialize(&tConfig, &tInput);

    double heats[4] = {100.0, 200.0, 300.0, 400.0};
    const double expectedHeat = 250.0; // average of heats[4]
    tArticle.mQueues[5].mHeat_queue      = heats;
    tArticle.mQueues[5].mHeat_queue_size = 4;

    tArticle.stepPreSolver(0.0);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHeat, tCapacitor.mExternalHeatFlux[5], DBL_EPSILON);

    std::cout << "... Pass";
}

#include <iostream>
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the stepPostSolver method of the GunnsThermalCapacitorHeatQueues class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalCapacitorHeatQueues::testPostSolver()
{
    std::cout << "\n UtGunnsThermalCapacitorHeatQueues 06: testPostSolver ............";

    tArticle.initialize(&tConfig, &tInput);
    tArticle.stepPostSolver(0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the setter & getter methods of the GunnsThermalCapacitorHeatQueues class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalCapacitorHeatQueues::testAccessors()
{
    std::cout << "\n UtGunnsThermalCapacitorHeatQueues 07: testAccessors .............";

    /// - Currently nothing to test.

    std::cout << "... Pass";
}
