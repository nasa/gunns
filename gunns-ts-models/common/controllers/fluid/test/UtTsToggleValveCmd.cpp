/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ()
***************************************************************************************************/

#include <iostream>

#include "strings/UtResult.hh"

#include "UtTsToggleValveCmd.hh"
#include "UtTsValveControllerAsserts.hh"

/// @details  Test identification number.
int UtTsToggleValveCmd::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Powered Toggle Valve command unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsToggleValveCmd::UtTsToggleValveCmd()
    :
    CppUnit::TestFixture(),
    mArticle(0),
    mTimeStep(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Powered Toggle Valve command unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsToggleValveCmd::~UtTsToggleValveCmd()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsToggleValveCmd::setUp()
{
    mArticle  = new FriendlyTsToggleValveCmd();
    mTimeStep = 0.1;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsToggleValveCmd::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete mArticle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Powered Toggle Valve command default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsToggleValveCmd::testDefaultConstruction()
{
    UT_RESULT_FIRST;

    /// @test  Default construction.
    CPPUNIT_ASSERT(    !mArticle->mEnable);
    CPPUNIT_ASSERT(    !mArticle->mToggle);
    CPPUNIT_ASSERT(0 == mArticle->mEnable_queue);
    CPPUNIT_ASSERT(0 == mArticle->mToggle_queue);
    CPPUNIT_ASSERT(0 == mArticle->mEnable_queue_size);
    CPPUNIT_ASSERT(0 == mArticle->mToggle_queue_size);
    CPPUNIT_ASSERT(     mArticle->mArmed);
    CPPUNIT_ASSERT(0 == mArticle->mToggleCount);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Powered Toggle Valve command nominal construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsToggleValveCmd::testNominalConstruction()
{
    UT_RESULT;

    /// @test  Construction with each combination of arguments.
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            const bool enable = static_cast<bool>(i);
            const bool toggle = static_cast<bool>(j);
            FriendlyTsToggleValveCmd article(enable, toggle);
            CPPUNIT_ASSERT(enable == article.mEnable);
            CPPUNIT_ASSERT(toggle == article.mToggle);
            CPPUNIT_ASSERT(0      == article.mEnable_queue);
            CPPUNIT_ASSERT(0      == article.mToggle_queue);
            CPPUNIT_ASSERT(0      == article.mEnable_queue_size);
            CPPUNIT_ASSERT(0      == article.mToggle_queue_size);
            CPPUNIT_ASSERT(          article.mArmed);
            CPPUNIT_ASSERT(0      == article.mToggleCount);
        }
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Powered Toggle Valve command accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsToggleValveCmd::testAccessors()
{
    UT_RESULT;

    /// - Default construct test article
    TsToggleValveCmd article;

    /// @test  getEnable.
    CPPUNIT_ASSERT(    !article.getEnable());
    article.mEnable = true;
    CPPUNIT_ASSERT(    article.getEnable());

    /// @test  getToggle.
    CPPUNIT_ASSERT(    !article.getToggle());
    article.mToggle = true;
    CPPUNIT_ASSERT(    article.getToggle());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Powered Toggle Valve command modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsToggleValveCmd::testModifiers()
{
    UT_RESULT;

    /// - Default construct test article
    TsToggleValveCmd article;

    /// @test  nothing, this class has no modifiers.

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Powered Toggle Valve command update state (nominal).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsToggleValveCmd::testUpdateNominal()
{
    UT_RESULT;

    /// @test enable and toggle with zero time step
    mArticle->mEnable = true;
    mArticle->mToggle = true;
    mArticle->update(0.0);
    CPPUNIT_ASSERT( mArticle->mEnable);
    CPPUNIT_ASSERT( mArticle->mToggle);
    CPPUNIT_ASSERT( mArticle->mArmed);
    CPPUNIT_ASSERT(0 == mArticle->mToggleCount);

    /// @test enable and toggle with nominal time step
    mArticle->mEnable = true;
    mArticle->mToggle = true;
    mArticle->update(mTimeStep);
    CPPUNIT_ASSERT( mArticle->mEnable);
    CPPUNIT_ASSERT( mArticle->mToggle);
    CPPUNIT_ASSERT(!mArticle->mArmed);
    CPPUNIT_ASSERT(1 == mArticle->mToggleCount);

    /// @test enable and toggle again
    mArticle->mEnable = true;
    mArticle->mToggle = true;
    mArticle->update(mTimeStep);
    CPPUNIT_ASSERT( mArticle->mEnable);
    CPPUNIT_ASSERT(!mArticle->mToggle);
    CPPUNIT_ASSERT(!mArticle->mArmed);
    CPPUNIT_ASSERT(1 == mArticle->mToggleCount);

    /// @test enable and !toggle
    mArticle->mEnable = true;
    mArticle->mToggle = false;
    mArticle->update(mTimeStep);
    CPPUNIT_ASSERT( mArticle->mEnable);
    CPPUNIT_ASSERT(!mArticle->mToggle);
    CPPUNIT_ASSERT( mArticle->mArmed);
    CPPUNIT_ASSERT(1 == mArticle->mToggleCount);

    /// @test enable and toggle
    mArticle->mEnable = true;
    mArticle->mToggle = true;
    mArticle->update(mTimeStep);
    CPPUNIT_ASSERT( mArticle->mEnable);
    CPPUNIT_ASSERT( mArticle->mToggle);
    CPPUNIT_ASSERT(!mArticle->mArmed);
    CPPUNIT_ASSERT(2 == mArticle->mToggleCount);

    /// @test !enable and toggle
    mArticle->mEnable = false;
    mArticle->mToggle = true;
    mArticle->update(mTimeStep);
    CPPUNIT_ASSERT(!mArticle->mEnable);
    CPPUNIT_ASSERT(!mArticle->mToggle);
    CPPUNIT_ASSERT(!mArticle->mArmed);
    CPPUNIT_ASSERT(2 == mArticle->mToggleCount);

    /// @test enable and toggle
    mArticle->mEnable = true;
    mArticle->mToggle = true;
    mArticle->update(mTimeStep);
    CPPUNIT_ASSERT( mArticle->mEnable);
    CPPUNIT_ASSERT(!mArticle->mToggle);
    CPPUNIT_ASSERT(!mArticle->mArmed);
    CPPUNIT_ASSERT(2 == mArticle->mToggleCount);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Powered Toggle Valve command update state (nominal).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsToggleValveCmd::testUpdateQueue()
{
    UT_RESULT;

    /// - Default construct test article
    FriendlyTsToggleValveCmd article;
    bool  enable_queue[4];
    bool  toggle_queue[4];
    article.mEnable_queue = &enable_queue[0];
    article.mToggle_queue = &toggle_queue[0];
    article.mEnable_queue_size = 4;
    article.mToggle_queue_size = 4;

    /// @test alternating toggle
    enable_queue[0] = true;
    toggle_queue[0] = true;
    enable_queue[1] = true;
    toggle_queue[1] = false;
    enable_queue[2] = true;
    toggle_queue[2] = true;
    enable_queue[3] = true;
    toggle_queue[3] = false;
    article.update(mTimeStep);
    CPPUNIT_ASSERT( article.mEnable);
    CPPUNIT_ASSERT(!article.mToggle);
    CPPUNIT_ASSERT( article.mArmed);
    CPPUNIT_ASSERT_EQUAL(2, article.mToggleCount);

    /// @test alternating enable and toggle
    enable_queue[0] = true;
    toggle_queue[0] = true;
    enable_queue[1] = false;
    toggle_queue[1] = false;
    enable_queue[2] = true;
    toggle_queue[2] = false;
    enable_queue[3] = false;
    toggle_queue[3] = false;
    article.update(mTimeStep);
    CPPUNIT_ASSERT( article.mEnable);
    CPPUNIT_ASSERT( article.mToggle);
    CPPUNIT_ASSERT( article.mArmed);
    CPPUNIT_ASSERT_EQUAL(3, article.mToggleCount);

    /// @test enable and toggle with final toggle off
    enable_queue[0] = true;
    toggle_queue[0] = true;
    enable_queue[1] = true;
    toggle_queue[1] = true;
    enable_queue[2] = true;
    toggle_queue[2] = true;
    enable_queue[3] = true;
    toggle_queue[3] = false;
    article.update(mTimeStep);
    CPPUNIT_ASSERT( article.mEnable);
    CPPUNIT_ASSERT( article.mToggle);
    CPPUNIT_ASSERT( article.mArmed);
    CPPUNIT_ASSERT_EQUAL(4, article.mToggleCount);

    /// @test alternating toggle
    enable_queue[0] = false;
    toggle_queue[0] = true;
    enable_queue[1] = false;
    toggle_queue[1] = false;
    enable_queue[2] = false;
    toggle_queue[2] = true;
    enable_queue[3] = true;
    toggle_queue[3] = false;
    article.update(mTimeStep);
    CPPUNIT_ASSERT(!article.mEnable);
    CPPUNIT_ASSERT(!article.mToggle);
    CPPUNIT_ASSERT( article.mArmed);
    CPPUNIT_ASSERT_EQUAL(4, article.mToggleCount);

    /// @test always enable and toggle
    enable_queue[0] = true;
    toggle_queue[0] = true;
    enable_queue[1] = true;
    toggle_queue[1] = true;
    enable_queue[2] = true;
    toggle_queue[2] = true;
    enable_queue[3] = true;
    toggle_queue[3] = true;
    article.update(mTimeStep);
    CPPUNIT_ASSERT( article.mEnable);
    CPPUNIT_ASSERT( article.mToggle);
    CPPUNIT_ASSERT(!article.mArmed);
    CPPUNIT_ASSERT_EQUAL(5, article.mToggleCount);

    UT_PASS_LAST;
}

