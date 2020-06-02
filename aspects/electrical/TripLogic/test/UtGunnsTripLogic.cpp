/**
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((aspects/electrical/TripLogic/GunnsTripLogic.o))
*/
#include <iostream>
#include "strings/UtResult.hh"
#include "UtGunnsTripLogic.hh"

/// @details  Test identification number.
int UtGunnsTripLogic::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsTripLogic class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsTripLogic::UtGunnsTripLogic()
    :
    tLimit(0.0),
    tPriority(0),
    tIsTripped(false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsTripLogic class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsTripLogic::~UtGunnsTripLogic()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsTripLogic::setUp()
{
    /// - Define the initialization data.
    tLimit     = 5.0;
    tPriority  = 3;
    tIsTripped = true;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsTripLogic::tearDown()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for default construction of a GunnsTripLogic.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsTripLogic::testConstruction()
{
    UT_RESULT_FIRST;

    {
        /// @test    Nominal construction of a GunnsTripGreaterThan.
        FriendlyGunnsTripGreaterThan* article = new FriendlyGunnsTripGreaterThan();
        CPPUNIT_ASSERT(false == article->mMalfInhibitTrip);
        CPPUNIT_ASSERT(false == article->mMalfForceTrip);
        CPPUNIT_ASSERT(0.0   == article->mLimit);
        CPPUNIT_ASSERT(0     == article->mPriority);
        CPPUNIT_ASSERT(false == article->mIsTripped);
        delete article;
    } {
        /// @test    Nominal construction of a GunnsTripLessThan.
        FriendlyGunnsTripLessThan* article = new FriendlyGunnsTripLessThan();
        CPPUNIT_ASSERT(false == article->mMalfInhibitTrip);
        CPPUNIT_ASSERT(false == article->mMalfForceTrip);
        CPPUNIT_ASSERT(0.0   == article->mLimit);
        CPPUNIT_ASSERT(0     == article->mPriority);
        CPPUNIT_ASSERT(false == article->mIsTripped);
        delete article;
    } {
        /// @test    New/delete for code coverage.
        GunnsTripGreaterThan* article = new GunnsTripGreaterThan();
        delete article;
    } {
        /// @test    New/delete for code coverage.
        GunnsTripLessThan* article = new GunnsTripLessThan();
        delete article;
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Trip Logic model initialization.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsTripLogic::testNominalInitialization()
{
    UT_RESULT;

    {
        /// @test    Initialization of a GunnsTripGreaterThan.
        FriendlyGunnsTripGreaterThan* article = new FriendlyGunnsTripGreaterThan();
        article->initialize(tLimit, tPriority, tIsTripped);
        CPPUNIT_ASSERT(false      == article->mMalfInhibitTrip);
        CPPUNIT_ASSERT(false      == article->mMalfForceTrip);
        CPPUNIT_ASSERT(tLimit     == article->mLimit);
        CPPUNIT_ASSERT(tPriority  == article->mPriority);
        CPPUNIT_ASSERT(tIsTripped == article->mIsTripped);
        delete article;
    } {
        /// @test    Initialization of a GunnsTripLessThan.
        FriendlyGunnsTripLessThan* article = new FriendlyGunnsTripLessThan();
        article->initialize(tLimit, tPriority, tIsTripped);
        CPPUNIT_ASSERT(false      == article->mMalfInhibitTrip);
        CPPUNIT_ASSERT(false      == article->mMalfForceTrip);
        CPPUNIT_ASSERT(tLimit     == article->mLimit);
        CPPUNIT_ASSERT(tPriority  == article->mPriority);
        CPPUNIT_ASSERT(tIsTripped == article->mIsTripped);
        delete article;
    }

    UT_PASS;
}

#include "core/GunnsBasicLink.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the checkForTrip method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsTripLogic::testCheckForTrip()
{
    UT_RESULT;

    {
        /// @test    Initialization of a GunnsTripGreaterThan.
        FriendlyGunnsTripGreaterThan* article = new FriendlyGunnsTripGreaterThan();
        article->initialize(tLimit, tPriority, false);

        /// @test    No trip condition and not priority yet.
        GunnsBasicLink::SolutionResult result = GunnsBasicLink::CONFIRM;
        bool justTripped = article->checkForTrip(result, 4.9, 2);
        CPPUNIT_ASSERT(false                   == justTripped);
        CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == result);
        CPPUNIT_ASSERT(false                   == article->mIsTripped);

        /// @test    Trip condition but not priority yet.
        justTripped = article->checkForTrip(result, 5.1, 2);
        CPPUNIT_ASSERT(false                   == justTripped);
        CPPUNIT_ASSERT(GunnsBasicLink::DELAY   == result);
        CPPUNIT_ASSERT(false                   == article->mIsTripped);

        /// @test    Trip condition and priority met.
        justTripped = article->checkForTrip(result, 5.1, 3);
        CPPUNIT_ASSERT(true                    == justTripped);
        CPPUNIT_ASSERT(GunnsBasicLink::REJECT  == result);
        CPPUNIT_ASSERT(true                    == article->mIsTripped);

        /// @test    Trip condition and past priority.
        justTripped = article->checkForTrip(result, 5.1, 4);
        result      = GunnsBasicLink::CONFIRM;
        CPPUNIT_ASSERT(false                   == justTripped);
        CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == result);
        CPPUNIT_ASSERT(true                    == article->mIsTripped);

        /// @test    Tripped and priority met but no trip condition.
        justTripped = article->checkForTrip(result, 4.9, 4);
        CPPUNIT_ASSERT(false                   == justTripped);
        CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == result);
        CPPUNIT_ASSERT(true                    == article->mIsTripped);

        /// @test    Priority met but no trip condition.
        result      = GunnsBasicLink::CONFIRM;
        article->mIsTripped = false;
        justTripped = article->checkForTrip(result, 4.9, 4);
        CPPUNIT_ASSERT(false                   == justTripped);
        CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == result);
        CPPUNIT_ASSERT(false                   == article->mIsTripped);

        /// @test    Trip condition, not priority yet, but result is already REJECT.
        result      = GunnsBasicLink::REJECT;
        justTripped = article->checkForTrip(result, 5.1, 2);
        CPPUNIT_ASSERT(false                   == justTripped);
        CPPUNIT_ASSERT(GunnsBasicLink::REJECT  == result);
        CPPUNIT_ASSERT(false                   == article->mIsTripped);

        /// @test    Force trip and not priority met.
        article->mIsTripped     = false;
        article->mMalfForceTrip = true;
        result                  = GunnsBasicLink::CONFIRM;
        justTripped = article->checkForTrip(result, 4.9, 2);
        CPPUNIT_ASSERT(true                    == justTripped);
        CPPUNIT_ASSERT(GunnsBasicLink::REJECT  == result);
        CPPUNIT_ASSERT(true                    == article->mIsTripped);

        /// @test    Inhibit trip and force trip.
        article->mIsTripped       = false;
        article->mMalfForceTrip   = true;
        article->mMalfInhibitTrip = true;
        result                    = GunnsBasicLink::CONFIRM;
        justTripped = article->checkForTrip(result, 5.1, 3);
        CPPUNIT_ASSERT(false                   == justTripped);
        CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == result);
        CPPUNIT_ASSERT(false                   == article->mIsTripped);

        /// @test    Limit = 0.
        article->mIsTripped       = false;
        article->mMalfForceTrip   = false;
        article->mMalfInhibitTrip = false;
        article->mLimit           = 0.0;
        result                    = GunnsBasicLink::CONFIRM;
        justTripped = article->checkForTrip(result, 5.1, 3);
        CPPUNIT_ASSERT(false                   == justTripped);
        CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == result);
        CPPUNIT_ASSERT(false                   == article->mIsTripped);

        /// @test    Priority = 0.
        article->mLimit           = tLimit;
        article->mPriority        = 0;
        result                    = GunnsBasicLink::CONFIRM;
        justTripped = article->checkForTrip(result, 5.1, 3);
        CPPUNIT_ASSERT(false                   == justTripped);
        CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == result);
        CPPUNIT_ASSERT(false                   == article->mIsTripped);

        delete article;
    } {
        /// @test    Less than trip.
        FriendlyGunnsTripLessThan* article = new FriendlyGunnsTripLessThan();
        article->initialize(tLimit, tPriority, false);

        GunnsBasicLink::SolutionResult result = GunnsBasicLink::CONFIRM;
        bool justTripped = article->checkForTrip(result, 5.1, 3);
        CPPUNIT_ASSERT(false                   == justTripped);
        CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == result);
        CPPUNIT_ASSERT(false                   == article->mIsTripped);

        justTripped = article->checkForTrip(result, 4.9, 3);
        CPPUNIT_ASSERT(true                    == justTripped);
        CPPUNIT_ASSERT(GunnsBasicLink::REJECT  == result);
        CPPUNIT_ASSERT(true                    == article->mIsTripped);

        delete article;
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the isTripped method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsTripLogic::testIsTripped()
{
    UT_RESULT;

    /// @test    isTripped method.
    FriendlyGunnsTripGreaterThan* article = new FriendlyGunnsTripGreaterThan();
    article->initialize(tLimit, tPriority, tIsTripped);

    CPPUNIT_ASSERT(true == article->isTripped());

    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the resetTrip method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsTripLogic::testResetTrip()
{
    UT_RESULT;

    /// @test    resetTrip method.
    FriendlyGunnsTripGreaterThan* article = new FriendlyGunnsTripGreaterThan();
    article->initialize(tLimit, tPriority, tIsTripped);

    article->resetTrip();
    CPPUNIT_ASSERT(false == article->isTripped());

    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the access methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsTripLogic::testAccessors()
{
    UT_RESULT;

    /// @test    setLimit and getLimit methods.
    FriendlyGunnsTripGreaterThan* article = new FriendlyGunnsTripGreaterThan();
    article->initialize(tLimit, tPriority, tIsTripped);

    const float limit = 42.0;
    article->setLimit(limit);
    CPPUNIT_ASSERT(limit == article->getLimit());

    delete article;

    UT_PASS_LAST;
}
