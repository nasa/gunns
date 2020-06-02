/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ()
***************************************************************************************************/

#include <iostream>

#include "strings/UtResult.hh"

#include "UtTsOpenCloseValveSensed.hh"
#include "UtTsValveControllerAsserts.hh"

/// @details  Test identification number.
int UtTsOpenCloseValveSensed::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Powered Open/Close Valve sensed unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsOpenCloseValveSensed::UtTsOpenCloseValveSensed()
    :
    CppUnit::TestFixture(),
    mArticle(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Powered Open/Close Valve sensed unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsOpenCloseValveSensed::~UtTsOpenCloseValveSensed()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpenCloseValveSensed::setUp()
{
    mArticle = new TsOpenCloseValveSensed(true, true);

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpenCloseValveSensed::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete mArticle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Powered Open/Close Valve sensed default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpenCloseValveSensed::testDefaultConstruction()
{
    UT_RESULT_FIRST;

    /// @test  Default construction (all false).
    TsOpenCloseValveSensed returned;
    ASSERT_OPEN_CLOSE_VALVE_SENSED_VALUES_EQUAL(false, false, returned);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Powered Open/Close Valve sensed nominal construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpenCloseValveSensed::testNominalConstruction()
{
    UT_RESULT;

    /// @test  Construction with each combination of arguments.
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            const bool openSensed   = static_cast<bool>(1);
            const bool closeSensed  = static_cast<bool>(j);
            TsOpenCloseValveSensed article(openSensed, closeSensed);
            ASSERT_OPEN_CLOSE_VALVE_SENSED_VALUES_EQUAL(openSensed, closeSensed, article);
        }
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Powered Open/Close Valve sensed copy construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpenCloseValveSensed::testCopyConstruction()
{
    UT_RESULT;

    /// @test  Copy.
    TsOpenCloseValveSensed article(*mArticle);
    ASSERT_OPEN_CLOSE_VALVE_SENSED_VALUES_EQUAL(true, true, article);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Powered Open/Close Valve sensed assignment.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsOpenCloseValveSensed::testAssignment()
{
    UT_RESULT;

    /// @test  Assignment to other.
    TsOpenCloseValveSensed article;
    *mArticle = article;
    ASSERT_OPEN_CLOSE_VALVE_SENSED_EQUAL(article, *mArticle);

    /// @test  Assignment to self.
    *mArticle = *mArticle;
    ASSERT_OPEN_CLOSE_VALVE_SENSED_VALUES_EQUAL(false, false, *mArticle);

    UT_PASS_LAST;
}
