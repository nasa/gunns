/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ()
***************************************************************************************************/

#include <iostream>

#include "strings/UtResult.hh"

#include "UtTsOpenCloseValveCmd.hh"
#include "UtTsValveControllerAsserts.hh"

/// @details  Test identification number.
int UtTsOpenCloseValveCmd::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Powered Open/Close Valve command unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsOpenCloseValveCmd::UtTsOpenCloseValveCmd()
    :
    CppUnit::TestFixture(),
    mArticle(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Powered Open/Close Valve command unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsOpenCloseValveCmd::~UtTsOpenCloseValveCmd()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpenCloseValveCmd::setUp()
{
    mArticle = new TsOpenCloseValveCmd(true, true, true);

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpenCloseValveCmd::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete mArticle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Powered Open/Close Valve command default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpenCloseValveCmd::testDefaultConstruction()
{
    UT_RESULT_FIRST;

    /// @test  Default construction (all false).
    TsOpenCloseValveCmd returned;
    ASSERT_OPEN_CLOSE_VALVE_CMD_VALUES_EQUAL(false, false, false, returned);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Powered Open/Close Valve command nominal construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpenCloseValveCmd::testNominalConstruction()
{
    UT_RESULT;

    /// @test  Construction with each combination of arguments.
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                const bool enableCmd = static_cast<bool>(i);
                const bool openCmd   = static_cast<bool>(j);
                const bool closeCmd  = static_cast<bool>(k);
                TsOpenCloseValveCmd article(enableCmd, openCmd, closeCmd);
                ASSERT_OPEN_CLOSE_VALVE_CMD_VALUES_EQUAL(enableCmd, openCmd, closeCmd, article);
            }
        }
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Powered Open/Close Valve command copy construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpenCloseValveCmd::testCopyConstruction()
{
    UT_RESULT;

    /// @test  Copy.
    TsOpenCloseValveCmd article(*mArticle);
    ASSERT_OPEN_CLOSE_VALVE_CMD_VALUES_EQUAL(true, true, true, article);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Powered Open/Close Valve command assignment.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpenCloseValveCmd::testAssignment()
{
    UT_RESULT;

    /// @test  Assignment to other.
    TsOpenCloseValveCmd article;
    *mArticle = article;
    ASSERT_OPEN_CLOSE_VALVE_CMD_EQUAL(article, *mArticle);

    /// @test  Assignment to self.
    *mArticle = *mArticle;
    ASSERT_OPEN_CLOSE_VALVE_CMD_VALUES_EQUAL(false, false, false, *mArticle);

    UT_PASS_LAST;
}
