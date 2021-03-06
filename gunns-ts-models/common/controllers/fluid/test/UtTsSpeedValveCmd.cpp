/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ()
***************************************************************************************************/

#include <iostream>

#include "strings/UtResult.hh"

#include "UtTsSpeedValveCmd.hh"
#include "UtTsValveControllerAsserts.hh"

/// @details  Test identification number.
int UtTsSpeedValveCmd::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Powered Speed Valve command unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsSpeedValveCmd::UtTsSpeedValveCmd()
    :
    CppUnit::TestFixture(),
    mArticle(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Powered Speed Valve command unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsSpeedValveCmd::~UtTsSpeedValveCmd()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsSpeedValveCmd::setUp()
{
    mArticle = new TsSpeedValveCmd(true, 0.5);

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsSpeedValveCmd::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete mArticle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Powered Speed Valve command default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsSpeedValveCmd::testDefaultConstruction()
{
    UT_RESULT_FIRST;

    /// @test  Default construction (all false).
    TsSpeedValveCmd returned;
    ASSERT_SPEED_VALVE_CMD_VALUES_EQUAL(false, 0.0, returned, FLT_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Powered Speed Valve command nominal construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsSpeedValveCmd::testNominalConstruction()
{
    UT_RESULT;

    /// @test  Construction with each combination of arguments.
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            const bool enableCmd = static_cast<bool>(i);
            const double cmd = 1.0 * j;
            TsSpeedValveCmd article(enableCmd, cmd);
            ASSERT_SPEED_VALVE_CMD_VALUES_EQUAL(enableCmd, cmd, article, FLT_EPSILON);
        }
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Powered Speed Valve command copy construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsSpeedValveCmd::testCopyConstruction()
{
    UT_RESULT;

    /// @test  Copy.
    TsSpeedValveCmd article(*mArticle);
    ASSERT_SPEED_VALVE_CMD_VALUES_EQUAL(true, 0.5, article, FLT_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Powered Speed Valve command assignment.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsSpeedValveCmd::testAssignment()
{
    UT_RESULT;

    /// @test  Assignment to other.
    TsSpeedValveCmd article;
    *mArticle = article;
    ASSERT_SPEED_VALVE_CMD_EQUAL(article, *mArticle, FLT_EPSILON);

    /// @test  Assignment to self.
    *mArticle = *mArticle;
    ASSERT_SPEED_VALVE_CMD_VALUES_EQUAL(false, 0.0, *mArticle, FLT_EPSILON);

    UT_PASS_LAST;
}
