/*
@copyright Copyright 2025 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/

#include "UtDistributed2WayBusBase.hh"
#include "strings/UtResult.hh"

/// @details  Test identification number.
int UtDistributed2WayBusBase::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtDistributed2WayBusBase class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtDistributed2WayBusBase::UtDistributed2WayBusBase()
    :
    tArticle(0),
    tIsPairMaster(),
    tInData(),
    tOutData()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtDistributed2WayBusBase class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtDistributed2WayBusBase::~UtDistributed2WayBusBase()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDistributed2WayBusBase::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDistributed2WayBusBase::setUp()
{
    tIsPairMaster = true;
    tArticle      = new FriendlyDistributed2WayBusBase(&tInData, &tOutData);

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDistributed2WayBusBase::testDefaultConstruction()
{
    UT_RESULT_INITIAL("Interoperability Models Unit Tests");

    /// @test state data
    CPPUNIT_ASSERT(false                        == tArticle->mIsPairMaster);
    CPPUNIT_ASSERT(false                        == tArticle->mInDataLastDemandMode);
    CPPUNIT_ASSERT(0                            == tArticle->mFramesSinceFlip);
    CPPUNIT_ASSERT(0                            == tArticle->mLoopLatency);
    CPPUNIT_ASSERT(Distributed2WayBusBase::NONE == tArticle->mForcedRole);
    CPPUNIT_ASSERT(0                            == tArticle->mNotifications.size());

    /// @test new/delete for code coverage
    Distributed2WayBusBase* article = new Distributed2WayBusBase(&tInData, &tOutData);
    delete article;

    /// @test interface data assignment operator
    tInData.mFrameCount    = 2;
    tInData.mFrameLoopback = 1;
    tInData.mDemandMode    = true;

    tOutData = tInData;
    CPPUNIT_ASSERT(tInData.mFrameCount    == tOutData.mFrameCount);
    CPPUNIT_ASSERT(tInData.mFrameLoopback == tOutData.mFrameLoopback);
    CPPUNIT_ASSERT(tInData.mDemandMode    == tOutData.mDemandMode);

    /// @test self-assign
    tOutData = tOutData;
    CPPUNIT_ASSERT(tInData.mFrameCount    == tOutData.mFrameCount);
    CPPUNIT_ASSERT(tInData.mFrameLoopback == tOutData.mFrameLoopback);
    CPPUNIT_ASSERT(tInData.mDemandMode    == tOutData.mDemandMode);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the initialization method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDistributed2WayBusBase::testInitialization()
{
    UT_RESULT;

    tArticle->initialize(tIsPairMaster);

    CPPUNIT_ASSERT(tIsPairMaster == tArticle->mIsPairMaster);
    CPPUNIT_ASSERT(false         == tArticle->mInDataLastDemandMode);
    CPPUNIT_ASSERT(0             == tArticle->mFramesSinceFlip);
    CPPUNIT_ASSERT(0             == tArticle->mLoopLatency);
    CPPUNIT_ASSERT(false         == tOutData.mDemandMode);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the updateFrameCounts method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDistributed2WayBusBase::testFrameCounts()
{
    UT_RESULT;

    tArticle->initialize(tIsPairMaster);

    tInData.mFrameCount    = 9;
    tInData.mFrameLoopback = 8;
    tOutData.mFrameCount   = 10;
    const unsigned int expectedOutFrameCount = tOutData.mFrameCount + 1;
    const unsigned int expectedFramesFlip    = 1;
    const unsigned int expectedLoopLatency   = expectedOutFrameCount - tInData.mFrameLoopback;
    tArticle->updateFrameCounts();
    CPPUNIT_ASSERT(expectedOutFrameCount == tOutData.mFrameCount);
    CPPUNIT_ASSERT(expectedFramesFlip    == tArticle->mFramesSinceFlip);
    CPPUNIT_ASSERT(expectedLoopLatency   == static_cast<unsigned int>(tArticle->mLoopLatency));
    CPPUNIT_ASSERT(tInData.mFrameCount   == tOutData.mFrameLoopback);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the push and pop notifications methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDistributed2WayBusBase::testNotifications()
{
    UT_RESULT;

    tArticle->initialize(tIsPairMaster);

    /// @test pushNotification
    const std::string message1 = "1test1";
    tArticle->pushNotification(Distributed2WayBusNotification::ERR, message1);
    CPPUNIT_ASSERT(1 == tArticle->mNotifications.size());

    const std::string message2 = "2test2";
    tArticle->pushNotification(Distributed2WayBusNotification::INFO, message2);
    CPPUNIT_ASSERT(2 == tArticle->mNotifications.size());

    /// @test popNotification
    Distributed2WayBusNotification notif;
    unsigned int numMessages = tArticle->popNotification(notif);
    CPPUNIT_ASSERT(1 == numMessages);
    CPPUNIT_ASSERT(1 == tArticle->mNotifications.size());
    CPPUNIT_ASSERT(0 == notif.mMessage.rfind("2test2", 0));
    CPPUNIT_ASSERT(Distributed2WayBusNotification::INFO == notif.mLevel);

    numMessages = tArticle->popNotification(notif);
    CPPUNIT_ASSERT(0 == numMessages);
    CPPUNIT_ASSERT(0 == tArticle->mNotifications.size());
    CPPUNIT_ASSERT(0 == notif.mMessage.rfind("1test1", 0));
    CPPUNIT_ASSERT(Distributed2WayBusNotification::ERR == notif.mLevel);

    numMessages = tArticle->popNotification(notif);
    CPPUNIT_ASSERT(0 == numMessages);
    CPPUNIT_ASSERT(0 == tArticle->mNotifications.size());
    CPPUNIT_ASSERT(0 == notif.mMessage.rfind("", 0));
    CPPUNIT_ASSERT(Distributed2WayBusNotification::NONE == notif.mLevel);

    /// @test assignment operator for code coverage
    Distributed2WayBusNotification notif2(Distributed2WayBusNotification::WARN, "notif2");
    notif = notif2;
    CPPUNIT_ASSERT(0 == notif.mMessage.rfind("notif2", 0));
    CPPUNIT_ASSERT(Distributed2WayBusNotification::WARN == notif.mLevel);

    /// @test self assign
    notif = notif;
    CPPUNIT_ASSERT(0 == notif.mMessage.rfind("notif2", 0));
    CPPUNIT_ASSERT(Distributed2WayBusNotification::WARN == notif.mLevel);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the mode forcing flags.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDistributed2WayBusBase::testAccessors()
{
    UT_RESULT;

    /// @test isInDemandRole
    tArticle->initialize(tIsPairMaster);
    CPPUNIT_ASSERT(false == tArticle->isInDemandRole());
    tOutData.mDemandMode = true;
    CPPUNIT_ASSERT(true  == tArticle->isInDemandRole());

    /// @test forceSupplyRole
    tArticle->forceSupplyRole();
    CPPUNIT_ASSERT(Distributed2WayBusBase::SUPPLY == tArticle->mForcedRole);

    /// @test forceSupplyRole
    tArticle->forceDemandRole();
    CPPUNIT_ASSERT(Distributed2WayBusBase::DEMAND == tArticle->mForcedRole);

    /// @test resetForceRole
    tArticle->resetForceRole();
    CPPUNIT_ASSERT(Distributed2WayBusBase::NONE   == tArticle->mForcedRole);

    UT_PASS_LAST;
}
